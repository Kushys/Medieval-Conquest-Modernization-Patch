/* v34: save balance includes escrow; restore escrow only for validated queues. */
#define QMAGIC 0x3351434du
struct QueueFingerprint {U size,h1,h2,time_lo,time_hi;};
struct QueueRecord {U id,count,defid[QUEUE_DEPTH],cost[QUEUE_DEPTH];};
struct QueueDisk {U magic,version;struct QueueFingerprint fp;U count;struct QueueRecord rows[QUEUE_MAX];U checksum;};
static struct QueueDisk queue_disk;
static B fingerprint_buf[8192];
static I queue_path(U addr,char *out){
 U n=0;B c=0;for(;n<960;n++){if(!rd(addr+n,&c,1))return 0;out[n]=(char)c;if(!c)break;}
 if(n<4||n>=960||out[n-4]!='.'||(out[n-3]!='s'&&out[n-3]!='S')||(out[n-2]!='t'&&out[n-2]!='T')||(out[n-1]!='s'&&out[n-1]!='S'))return 0;return 1;
}
static void queue_suffix(char *out,const char *base,const char *suffix){U i=0,j=0;while(base[i]){out[i]=base[i];i++;}while(suffix[j])out[i++]=suffix[j++];out[i]=0;}
static I fingerprint(const char *file,struct QueueFingerprint *fp){
 H f=CreateFileA(file,0x80000000,1,0,3,0x80,0);if(f==BAD)return 0;
 U high=0,size=GetFileSize(f,&high),ft[2],left=size,h1=2166136261u,h2=0x9e3779b9u,got=0;
 I ok=!high&&size&&size<=0x20000000u&&GetFileTime(f,0,0,ft);
 while(ok&&left){U n=left>sizeof(fingerprint_buf)?sizeof(fingerprint_buf):left;if(!ReadFile(f,fingerprint_buf,n,&got,0)||got!=n){ok=0;break;}for(U i=0;i<n;i++){h1=(h1^fingerprint_buf[i])*16777619u;h2=(h2+fingerprint_buf[i])*2246822519u;h2=(h2<<13)|(h2>>19);}left-=n;}
 CloseHandle(f);if(!ok)return 0;fp->size=size;fp->h1=h1;fp->h2=h2;fp->time_lo=ft[0];fp->time_hi=ft[1];return 1;
}
static I fingerprint_equal(const struct QueueFingerprint*a,const struct QueueFingerprint*b){return a->size==b->size&&a->h1==b->h1&&a->h2==b->h2&&a->time_lo==b->time_lo&&a->time_hi==b->time_hi;}
static U queue_checksum(const struct QueueDisk*d){const U *p=(const U*)d;U h=2166136261u;for(U i=0;i<(sizeof(*d)-4)/4;i++)h=(h^p[i])*16777619u;return h;}
static I queue_disk_valid(const struct QueueDisk*d,const struct QueueFingerprint*fp){
 if(d->magic!=QMAGIC||(d->version!=1&&d->version!=2)||d->count>QUEUE_MAX||!fingerprint_equal(&d->fp,fp)||d->checksum!=queue_checksum(d))return 0;
 for(U i=0;i<d->count;i++){const struct QueueRecord*r=&d->rows[i];if(!r->id||!r->count||r->count>QUEUE_DEPTH)return 0;
  for(U j=0;j<i;j++)if(d->rows[j].id==r->id)return 0;
  for(U j=0;j<r->count;j++){if(r->defid[j]==~0u||(r->cost[j]&QCOST)>100000000u||(d->version==1&&(r->cost[j]&QPAID)))return 0;for(U k=0;k<j;k++)if(r->defid[k]==r->defid[j])return 0;}
 }return 1;
}
/* Same ID lookup and RTTI cast as native CBuildingStatsMenu at 48FD95..48FDC4. */
static U queue_resolve(U id){
 U mgr=word(0x6e7b88);if(!mgr||!id)return 0;
 typedef U (__thiscall *Find)(void*,U);typedef U (__cdecl *Cast)(U,U,U,U,U);
 U obj=((Find)0x4b1310)((void*)mgr,id);if(!obj)return 0;
 U st=((Cast)0x61fc69)(obj,0,0x6a61f8,0x6a91d8,0);B destroyed=1;
 if(!st||word(st+4)!=id||!rd(st+0x274,&destroyed,1)||destroyed)return 0;return st;
}
static void queue_snapshot(const struct QueueFingerprint*fp){
 zero(&queue_disk,sizeof(queue_disk));queue_disk.magic=QMAGIC;queue_disk.version=2;queue_disk.fp=*fp;
 for(U i=0;i<QUEUE_MAX;i++){struct ResearchQueueEntry*q=&research_queue[i];if(!q->used||!q->count||q->count>QUEUE_DEPTH||queue_resolve(q->building_id)!=q->structure)continue;
  struct QueueRecord*r=&queue_disk.rows[queue_disk.count++];r->id=q->building_id;r->count=q->count;for(U j=0;j<q->count;j++){r->defid[j]=q->defid[j];r->cost[j]=q->cost[j];}
 }queue_disk.checksum=queue_checksum(&queue_disk);
}
static void queue_save_sidecar(const char *file,const struct QueueFingerprint*fp){
 char side[1024],tmp[1024];queue_suffix(side,file,".mcqueue");queue_suffix(tmp,file,".mcqueue.tmp");queue_snapshot(fp);
 U got=0;H f=CreateFileA(tmp,0x40000000,0,0,2,0x80,0);I ok=0;
 if(f!=BAD){ok=WriteFile(f,&queue_disk,sizeof(queue_disk),&got,0)&&got==sizeof(queue_disk)&&FlushFileBuffers(f);CloseHandle(f);}
 if(ok)ok=MoveFileExA(tmp,side,0x9);if(!ok){DeleteFileA(tmp);DeleteFileA(side);}
 begin(ok?"QUEUE_SAVE_OK":"QUEUE_SAVE_FAILED");num("buildings",queue_disk.count);num("bytes",sizeof(queue_disk));emit();
}
static void queue_load_sidecar(const char *file,const struct QueueFingerprint*fp){
 char side[1024];queue_suffix(side,file,".mcqueue");H f=CreateFileA(side,0x80000000,1,0,3,0x80,0);if(f==BAD){message("QUEUE_LOAD_ABSENT");return;}
 U high=0,got=0;I ok=GetFileSize(f,&high)==sizeof(queue_disk)&&!high&&ReadFile(f,&queue_disk,sizeof(queue_disk),&got,0)&&got==sizeof(queue_disk);CloseHandle(f);
 if(!ok||!queue_disk_valid(&queue_disk,fp)){message("QUEUE_LOAD_REJECTED");return;}
 U restored=0,entries=0,prepaid=0;for(U i=0;i<queue_disk.count;i++){
  struct QueueRecord*r=&queue_disk.rows[i];U st=queue_resolve(r->id),state[4],vec[2],count=0;if(!st||!research_read(st,state,vec,&count))continue;
  U slot=research_queue_slot(st,1);if(slot>=QUEUE_MAX)continue;struct ResearchQueueEntry*q=&research_queue[slot];
  for(U j=0;j<r->count;j++)for(U k=0;k<count;k++){U id=~0u,cost=0;if(!research_entry_info(st,k,&id,0,0,&cost)||id!=r->defid[j]||((state[0]&255)==1&&k==state[1]))continue;U saved=r->cost[j],account=active_account();if(queue_disk.version==2&&(saved&QPAID)){if(!affordable(account,saved&QCOST)||!queue_balance_change(account,-(I)(saved&QCOST)))break;queue_account=account;cost=saved;prepaid+=saved&QCOST;}
   q->defid[q->count]=id;q->cost[q->count++]=cost;entries++;break;}
  if(q->count){q->used=1;q->structure=st;q->building_id=r->id;restored++;}
 }begin("QUEUE_LOAD_OK");num("saved_buildings",queue_disk.count);num("restored_buildings",restored);num("entries",entries);num("prepaid",prepaid);emit();
}
extern B __thiscall native_game_save(void*,const char*,U);
B __thiscall game_save(void *self,const char *file,U mode){
 char name[1024];struct QueueFingerprint before,after;
 if(persistence_busy||!config_loaded||!thread||thread!=GetCurrentThreadId()||!queue_path((U)file,name))return native_game_save(self,file,mode);
 persistence_busy=1;I writing=(mode&255)!=0,had=writing&&fingerprint(name,&before);
 U account=active_account(),reserved=0;float original=0;I adjusted=0;
 if(writing){if(!queue_reserved_sum(&reserved)){persistence_busy=0;message("QUEUE_SAVE_ESCROW_FAILED");return 0;}
  if(reserved){if(account!=queue_account||!rd(account+0x10,&original,4)||!queue_balance_change(account,(I)reserved)){persistence_busy=0;message("QUEUE_SAVE_ESCROW_FAILED");return 0;}adjusted=1;begin("QUEUE_SAVE_ESCROW");num("amount",reserved);hx("save_balance_bits",word(account+0x10));emit();}}
 B ok=native_game_save(self,file,mode);
 if(adjusted&&account==active_account())queue_balance_store(account,original);
 if(ok){if(!writing){zero(research_queue,sizeof(research_queue));queue_account=0;}if(fingerprint(name,&after)){
  if(writing){if(!had||!fingerprint_equal(&before,&after))queue_save_sidecar(name,&after);else message("QUEUE_SAVE_UNCHANGED");}
  else if(research_queue_enabled)queue_load_sidecar(name,&after);
 }else message("QUEUE_FILE_FINGERPRINT_FAILED");}else message("QUEUE_NATIVE_SAVELOAD_FAILED");
 persistence_busy=0;return ok;
}
