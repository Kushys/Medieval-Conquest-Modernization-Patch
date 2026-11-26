/* v34: reserve queued gold immediately; post native expense only on start. */
static U indicator_root,indicator_sample_menu,indicator_sample_structure,indicator_sample_id,indicator_sample_frame,indicator_sample_button,indicator_sample_count;
U indicator_scope;
static U *indicator_dst;
static I indicator_rect[4],indicator_radius,indicator_failed;
static U indicator_logs;
static U cheat_pos,cheat_last_ms;
static U cheat_world;
static I gp_cursor_valid,gp_cursor_x,gp_cursor_y;
static U gp_left_down,gp_right_down,gp_left_edge,gp_right_edge,gp_click_handled;
#define QUEUE_MAX 32
#define QUEUE_DEPTH 8
#define QPAID 0x80000000u
#define QCOST 0x7fffffffu
struct ResearchQueueEntry { U used,structure,building_id,count,defid[QUEUE_DEPTH],cost[QUEUE_DEPTH]; };
static struct ResearchQueueEntry research_queue[QUEUE_MAX];
static U queue_logs;
static U persistence_busy;
static U queue_account;
static U active_account(void);static I affordable(U,U);
static U queue_resolve(U id);
/* Escrow adjusts available gold only; native expense ledger is posted at initiation. */
static void queue_balance_store(U account,float b){*(float*)(account+0x10)=b;}
static I queue_balance_change(U account,I delta){float b=0,n; if(!account||!rd(account+0x10,&b,4)||!(b>=-1000000000.0f&&b<=1000000000.0f))return 0; n=b+(float)delta;if(!(n>=-1000000000.0f&&n<=1000000000.0f)||(delta<0&&n<0.0f))return 0;queue_balance_store(account,n);return 1;}
static I queue_release(U cost){return !(cost&QPAID)||(queue_account==active_account()&&queue_balance_change(queue_account,(I)(cost&QCOST)));}
static I queue_drop(U slot){struct ResearchQueueEntry*q=&research_queue[slot];U sum=0;for(U j=0;j<q->count;j++)if(q->cost[j]&QPAID){U c=q->cost[j]&QCOST;if(c>1000000000u-sum)return 0;sum+=c;}if(sum&&!queue_release(sum|QPAID))return 0;if(sum){begin("RESEARCH_QUEUE_BUILDING_REFUND");num("amount",sum);hx("building_id",q->building_id);emit();}zero(q,sizeof(*q));return 1;}
static I queue_reserved_sum(U *sum){*sum=0;for(U i=0;i<QUEUE_MAX;i++)if(research_queue[i].used)for(U j=0;j<research_queue[i].count;j++)if(research_queue[i].cost[j]&QPAID){U c=research_queue[i].cost[j]&QCOST;if(c>1000000000u-*sum)return 0;*sum+=c;}return 1;}

static void gameplay_reset(void){
 indicator_root=indicator_sample_menu=indicator_sample_structure=indicator_sample_id=indicator_sample_frame=indicator_sample_button=indicator_sample_count=indicator_scope=0;
 indicator_dst=0;cheat_pos=cheat_last_ms=cheat_world=0;indicator_logs=0;gp_cursor_valid=gp_cursor_x=gp_cursor_y=0;gp_left_down=gp_right_down=gp_left_edge=gp_right_edge=gp_click_handled=0;zero(research_queue,sizeof(research_queue));queue_account=0;queue_logs=0;
}
/* Native button ID = 0x16 + current available-vector index. Never use +794
   (ID is not reliably serialized). Reject invalid or inactive entries. */
static U active_upgrade_button(U state,U index,U count){return (state&255)==1&&index<8&&index<count?0x16+index:0;}
static void indicator_sample(U menu,U structure,U id,U state,U index,U count){indicator_sample_menu=menu;indicator_sample_structure=structure;indicator_sample_id=id;indicator_sample_frame=frame;indicator_sample_count=count;indicator_sample_button=active_upgrade_button(state,index,count);}
static I indicator_match(U menu,U id,U sample_menu,U sample_id,U age,U button,U count,U tab){return menu&&menu==sample_menu&&id==sample_id&&age<=1&&button>=0x16&&button<0x16+count&&tab==0;}

static I research_read(U structure,U *state,U *vec,U *count){
 if(!structure||!rd(structure+0x78c,state,16)||!rd(structure+0x75c,vec,8)||vec[1]<vec[0]||(vec[1]-vec[0])%32||vec[1]-vec[0]>2048)return 0;
 *count=(vec[1]-vec[0])/32;return 1;
}
static I research_entry_info(U structure,U index,U *defid,U *base_cost,U *stored_id,U *effective_cost){
 U state[4],vec[2],count,entry[8],def=0,base=0,id=~0u,stored=0; float mult=1.0f; if(!research_read(structure,state,vec,&count)||index>=count)return 0;
 if(!rd(vec[0]+32*index,entry,32))return 0; def=entry[3]; stored=entry[4]; if(def){id=word(def+0x18);base=word(def+0x1c);} rd(vec[0]+32*index+0x14,&mult,4);
 if(!def||id==~0u||base>100000000u||!(mult>=0.0f&&mult<=100.0f)||!((float)base*mult<=100000000.0f))return 0; if(defid)*defid=id; if(base_cost)*base_cost=base; if(stored_id)*stored_id=stored; if(effective_cost)*effective_cost=(U)((float)base*mult); return 1;
}
static U research_queue_slot(U structure,U create){
 if(!structure)return QUEUE_MAX;
 U id=word(structure+4),freei=QUEUE_MAX;
 for(U i=0;i<QUEUE_MAX;i++){
  if(research_queue[i].used&&research_queue[i].structure==structure){
   if(research_queue[i].building_id==id)return i;
   if(!queue_drop(i))return QUEUE_MAX;
  }
  if(!research_queue[i].used&&freei==QUEUE_MAX)freei=i;
 }
 if(create&&freei<QUEUE_MAX){zero(&research_queue[freei],sizeof(research_queue[freei]));return freei;}return QUEUE_MAX;
}
static void research_queue_remove(U slot,U at,const char *reason){
 if(slot>=QUEUE_MAX)return;struct ResearchQueueEntry *q=&research_queue[slot];if(at>=q->count)return;
 if(queue_logs<256){begin("RESEARCH_QUEUE_CLEAR");hx("structure",q->structure);hx("building_id",q->building_id);hx("definition_id",q->defid[at]);num("position",at+1);txt(" reason=");txt(reason);emit();queue_logs++;}
 for(U j=at+1;j<q->count;j++){q->defid[j-1]=q->defid[j];q->cost[j-1]=q->cost[j];}q->count--;if(!q->count)zero(q,sizeof(*q));
}
static U research_queue_rank(U structure,U defid){U i=research_queue_slot(structure,0);if(i<QUEUE_MAX)for(U j=0;j<research_queue[i].count;j++)if(research_queue[i].defid[j]==defid)return j+1;return 0;}
static U active_account(void);static I affordable(U,U);
static I research_queue_toggle(U structure,U building_id,U defid,U index){
 if(!structure||defid==~0u||word(structure+4)!=building_id)return 0;
 U i=research_queue_slot(structure,0),rank=research_queue_rank(structure,defid);
 if(rank){U held=research_queue[i].cost[rank-1];if(queue_release(held)){if(held&QPAID){begin("RESEARCH_QUEUE_REFUND");num("amount",held&QCOST);hx("after_bits",word(active_account()+0x10));emit();}research_queue_remove(i,rank-1,"toggle_off");}return 0;}
 U cost=0;if(!research_entry_info(structure,index,0,0,0,&cost)||cost>100000000)return 0;
 U account=active_account();if(!account)return 0; if(!affordable(account,cost)){if(queue_logs<256){begin("RESEARCH_QUEUE_UNAFFORDABLE");num("cost",cost);emit();queue_logs++;}return 1;}
 if(i>=QUEUE_MAX)i=research_queue_slot(structure,1);if(i>=QUEUE_MAX)return 2;
 struct ResearchQueueEntry *q=&research_queue[i];if(q->count>=QUEUE_DEPTH)return 2;
 if(queue_account&&queue_account!=account)return 0;
 if(!queue_balance_change(account,-(I)cost))return 0;queue_account=account;
 q->used=1;q->structure=structure;q->building_id=building_id;q->defid[q->count]=defid;q->cost[q->count]=cost|QPAID;q->count++;
 begin("RESEARCH_QUEUE_PAID");num("amount",cost);hx("after_bits",word(account+0x10));emit();
 if(queue_logs<256){begin("RESEARCH_QUEUE_SET");hx("structure",structure);hx("building_id",building_id);num("index",index);hx("definition_id",defid);num("position",q->count);num("cost",cost);emit();queue_logs++;}
 return 0;
}

/* Capture actual successful native/physical sprite extents during precisely one
   upgrade-button draw. No framebuffer pointer survives that synchronous call. */
static void indicator_capture(U*dst,I width,I height,I pitch,I bpp,I flag,I l,I t,I r,I b,I radius){
 if(!indicator_scope||indicator_failed||GetCurrentThreadId()!=thread)return;
 if(!dst||width!=screen_w||height!=screen_h||pitch!=4*width||bpp!=32||flag||l<0||t<0||r>width||b>height||r<=l||b<=t||r-l>144||b-t>144||radius<3||radius>8){indicator_failed=1;return;}
 if(!indicator_dst){indicator_dst=dst;indicator_rect[0]=l;indicator_rect[1]=t;indicator_rect[2]=r;indicator_rect[3]=b;indicator_radius=radius;}
 else if(indicator_dst!=dst||indicator_radius!=radius){indicator_failed=1;return;}
 else{if(l<indicator_rect[0])indicator_rect[0]=l;if(t<indicator_rect[1])indicator_rect[1]=t;if(r>indicator_rect[2])indicator_rect[2]=r;if(b>indicator_rect[3])indicator_rect[3]=b;}
}
static I paint_upgrade_dot(U *dst,I width,I height,const I *rect,I radius,U fill,U rim,I corner){
 I l=rect[0],t=rect[1],r=rect[2],b=rect[3],margin=radius/2,cx=(corner&1)?(r-1-margin-radius):(l+margin+radius),cy=(corner&2)?(b-1-margin-radius):(t+margin+radius);
 if(!dst||width<1||height<1||radius<3||radius>8||l<0||t<0||r>width||b>height||r-l<2*radius+2*margin+1||b-t<2*radius+2*margin+1||r-l>144||b-t>144)return 0;
 for(I y=-radius;y<=radius;y++)for(I x=-radius;x<=radius;x++){I d=x*x+y*y;if(d<=radius*radius){U *p=dst+(cy+y)*width+cx+x;*p=(*p&0xff000000u)|(d<=(radius-1)*(radius-1)?fill:rim);}}return 1;
}
/* 3x5 digit glyphs; bounded by the already validated badge rectangle. */
static I paint_queue_number(U *dst,I width,I height,const I *rect,I radius,U rank){
 static const U glyphs[]={0x0,0x2c97,0x73e7,0x73cf,0x5bc9,0x79cf,0x79ef,0x7249,0x7bef};
 if(rank<1||rank>8)return 0;if(!paint_upgrade_dot(dst,width,height,rect,radius,0x00193b50u,0x0050d8ffu,1))return 0;
 I scale=radius>=6?2:1,cx=rect[2]-1-radius/2-radius,cy=rect[1]+radius/2+radius;
 I left=cx-(3*scale)/2,top=cy-(5*scale)/2;
 for(I y=0;y<5;y++)for(I x=0;x<3;x++)if(glyphs[rank]&(1u<<(14-y*3-x)))for(I dy=0;dy<scale;dy++)for(I dx=0;dx<scale;dx++){U *p=dst+(top+y*scale+dy)*width+left+x*scale+dx;*p=(*p&0xff000000u)|0x00eafaffu;}return 1;
}
static I point_in_rect(I x,I y,const I *r){return x>=r[0]&&x<r[2]&&y>=r[1]&&y<r[3];}

static U active_account(void){U logic=word(0x6e7794),game=logic?word(logic+0x100):0;return game?word(game+0x78):0;}
static I affordable(U account,U cost){float bal=0.0f;return account&&cost<=1000000000u&&rd(account+0x10,&bal,4)&&bal>=0.0f&&bal<=1000000000.0f&&bal>=(float)cost;}
static I safe_write_ok(U addr,U n){U tmp[4]; return rd(addr,tmp,n<=16?n:16); }
static void research_cancel(U structure,U building_id,U index){
 U defid=0,base=0,stored=0,cost=0,account=0; if(!research_entry_info(structure,index,&defid,&base,&stored,&cost)||!safe_write_ok(structure+0x78c,16))return; account=active_account(); if(!account)return;
 typedef void (__thiscall *Credit)(void*,float,U); ((Credit)0x422d80)((void*)account,(float)cost,0);
 if(safe_write_ok(structure+0x78c,16)){ *(B*)(structure+0x78c)=0; *(U*)(structure+0x790)=0; *(U*)(structure+0x794)=0; *(float*)(structure+0x798)=0.0f; }
 if(indicator_sample_structure==structure)indicator_sample_button=0;
 if(queue_logs<256){ begin("RESEARCH_CANCELLED"); hx("structure",structure); hx("building_id",building_id); num("index",index); hx("definition_id",defid); num("refund",cost); hx("after_bits",word(account+0x10)); emit(); queue_logs++; }
}
static I research_start_direct(U structure,U building_id,U wanted_defid){
 U state[4],vec[2],count,index=~0u,defid=0,base=0,stored=0,cost=0,account=active_account();
 if(!account||!research_read(structure,state,vec,&count))return 0; if((state[0]&255)==1)return 0; if(state[3]!=0&&state[3]!=0x40000000)return 0;
 for(U i=0;i<count;i++) if(research_entry_info(structure,i,&defid,&base,&stored,&cost)&&defid==wanted_defid){ index=i; break; }
 U slot=research_queue_slot(structure,0);if(slot>=QUEUE_MAX)return 0;U reserved=research_queue[slot].cost[0];
 if(index==~0u){if(queue_release(reserved))research_queue_remove(slot,0,"missing_after_completion");return 0;}
 if(!safe_write_ok(structure+0x78c,16))return 0;
 U paid=(reserved&QPAID)?reserved&QCOST:0;
 if(!affordable(account,cost>paid?cost-paid:0))return 0;
 if(!queue_release(reserved))return 0;
 typedef void (__thiscall *Debit)(void*,float,U); ((Debit)0x422d40)((void*)account,(float)cost,0);
 *(U*)(structure+0x790)=index; *(B*)(structure+0x78c)=1; *(U*)(structure+0x794)=stored; *(float*)(structure+0x798)=0.0f;
 if(queue_logs<256){ begin("RESEARCH_QUEUED_START"); hx("structure",structure); hx("building_id",building_id); num("index",index); hx("definition_id",wanted_defid); num("cost",cost); num("prepaid",paid); hx("after_bits",word(account+0x10)); emit(); queue_logs++; }
 research_queue_remove(research_queue_slot(structure,0),0,"started"); return 1;
}
static void research_live_tick(U structure){
 if(persistence_busy||!config_loaded||thread!=GetCurrentThreadId()||!research_queue_enabled||!structure)return;
 B destroyed=1;if(!rd(structure+0x274,&destroyed,1))return;if(destroyed){U slot=research_queue_slot(structure,0);if(slot<QUEUE_MAX)queue_drop(slot);return;}
 U id=word(structure+4);for(U i=0;i<QUEUE_MAX;i++)if(research_queue[i].used&&research_queue[i].structure==structure&&research_queue[i].building_id!=id)queue_drop(i);
 U slot=research_queue_slot(structure,0);if(slot>=QUEUE_MAX)return;
 U state[4],vec[2],count,defid=~0u;if(!research_read(structure,state,vec,&count))return;
 if((state[0]&255)==1){if(research_entry_info(structure,state[1],&defid,0,0,0)){U rank=research_queue_rank(structure,defid);if(rank&&queue_release(research_queue[slot].cost[rank-1]))research_queue_remove(slot,rank-1,"already_started");}return;}
 research_start_direct(structure,id,research_queue[slot].defid[0]);
}
static void gameplay_frame(void){
 I xy[2]; U pid=0; H fg=GetForegroundWindow(); if(fg)GetWindowThreadProcessId(fg,&pid); gp_cursor_valid=0; if(client_ok&&fg&&pid==GetCurrentProcessId()&&GetCursorPos(xy)&&ScreenToClient(fg,xy)){ gp_cursor_valid=1; gp_cursor_x=xy[0]; gp_cursor_y=xy[1]; }
 U left=(GetAsyncKeyState(1)&0x8000)!=0,right=(GetAsyncKeyState(2)&0x8000)!=0; gp_left_edge=left&&!gp_left_down; gp_right_edge=right&&!gp_right_down; gp_left_down=left; gp_right_down=right; gp_click_handled=0;
 /* Resolve queued building identities through the native registry, never dereference stale pointers. */
 if(!persistence_busy&&config_loaded&&queue_account&&queue_account==active_account()&&frame%30==0){for(U i=0;i<QUEUE_MAX;i++)if(research_queue[i].used&&queue_resolve(research_queue[i].building_id)!=research_queue[i].structure){begin("RESEARCH_QUEUE_BUILDING_LOST");hx("building_id",research_queue[i].building_id);emit();queue_drop(i);}}
 /* Queue starts only at verified live simulation hook. */
}
/* Reproduce native 00490eac..00490f0b failure response without entering purchase code. */
static void research_queue_feedback(I reason){
 U mgr=word(0x6e7b88),logic=word(0x6e7794);if(!mgr||!logic)return;
 if(reason==1){
  typedef U (__thiscall *Translate)(void*,U);typedef U (__thiscall *Assign)(void*,U,U);
  U str=((Translate)0x5ecd90)((void*)0x6e7820,0x6b3384),len=0;B c=0;
  if(str){while(len<1024&&rd(str+len,&c,1)&&c)len++;if(len<1024&&rd(str+len,&c,1)&&!c){((Assign)0x404120)((void*)0x6e7b90,str,str+len);*(U*)(logic+0x24)=0x40200000;}}
 }
 U loader=word(mgr+0x70),sound=word(mgr+0x6c);
 if(loader&&sound){typedef U (__thiscall *Load)(void*,U);typedef U (__thiscall *Play)(void*,U,U,U,float,U);U handle=((Load)0x4c9d60)((void*)loader,0x6a96c0);((Play)0x5e57a0)((void*)sound,handle,0,0,1.0f,~0u);}
 begin("RESEARCH_QUEUE_NATIVE_FEEDBACK");num("reason",reason);emit();
}
extern void __thiscall native_upgrade_draw(void*,U,I,I);
void __thiscall upgrade_draw(void *button,U surface,I x,I y){
 U p=(U)button,m=indicator_root,button_id=(word(p)==0x6886b4?word(p+4):0),state=word(p+8),structure=indicator_sample_structure,building_id=indicator_sample_id; U count=indicator_sample_count; I eligible=(research_indicator||research_queue_enabled||research_cancel_enabled)&&m&&m==indicator_sample_menu&&frame-indicator_sample_frame<=1&&config_loaded&&client_ok&&thread==GetCurrentThreadId()&&!persistence_busy&&!indicator_scope&&word(m)==0x682218&&word(p)==0x6886b4&&state!=0&&indicator_match(m,word(m+0xc0),indicator_sample_menu,indicator_sample_id,frame-indicator_sample_frame,button_id,count,word(m+0xa8));
 if(!eligible){native_upgrade_draw(button,surface,x,y);return;}
 indicator_dst=0;indicator_failed=0;indicator_scope=1;native_upgrade_draw(button,surface,x,y);indicator_scope=0;
 if(!indicator_failed&&indicator_dst){ U index=button_id-0x16,defid=~0u,base=0,stored=0,cost=0; research_entry_info(structure,index,&defid,&base,&stored,&cost);
  if(gp_cursor_valid&&!gp_click_handled&&point_in_rect(gp_cursor_x,gp_cursor_y,indicator_rect)){
   U rs[4],rv[2],rc=0; if(research_read(structure,rs,rv,&rc)){ U active=(rs[0]&255)==1&&rs[1]<rc; if(research_cancel_enabled&&gp_right_edge&&active&&index==rs[1]){ research_cancel(structure,building_id,index); gp_click_handled=1; }
    else if(research_queue_enabled&&defid!=~0u&&((gp_left_edge&&active&&index!=rs[1])||(gp_right_edge&&!active&&research_queue_rank(structure,defid)))){ I reason=research_queue_toggle(structure,building_id,defid,index); if(reason)research_queue_feedback(reason); gp_click_handled=1; }} }
  if(research_indicator&&button_id==indicator_sample_button){ if(paint_upgrade_dot(indicator_dst,screen_w,screen_h,indicator_rect,indicator_radius,0x00e9c34bu,0x0043321du,1)&&indicator_logs<128&&changed(150,indicator_sample_id,indicator_sample_button)){ indicator_logs++; begin("RESEARCH_INDICATOR"); hx("building_id",indicator_sample_id); num("index",indicator_sample_button-0x16); arr("physical_rect",(U*)indicator_rect,4,0); num("radius",indicator_radius); emit(); } }
  if(research_queue_enabled){U rank=research_queue_rank(structure,defid);if(rank)paint_queue_number(indicator_dst,screen_w,screen_h,indicator_rect,indicator_radius,rank);}
 }
 else if(research_diagnostics&&indicator_logs<128&&changed(151,indicator_sample_id,indicator_sample_button)){indicator_logs++;begin("RESEARCH_INDICATOR_SKIP");hx("building_id",indicator_sample_id);num("index",indicator_sample_button>=0x16?indicator_sample_button-0x16:0);num("bad_surface",indicator_failed);emit();}
 indicator_dst=0;
}
/* Exact ASCII command matcher: native parser provides the character as float.
   Non-character events are ignored; unrelated printable chars reset progress.
   No string/keypress logging. Native cheat enable is checked for every input. */
static U money_command_step(U *pos,U c){static const char command[]="GIVEMONEY";if(*pos>=sizeof(command)-1)*pos=0;if(c>='a'&&c<='z')c-=32;if(c==(U)command[*pos]){(*pos)++;if(*pos==sizeof(command)-1){*pos=0;return 1;}}else *pos=c=='G'?1:0;return 0;}
static I money_valid(float balance,U income){return balance>=-1000000000.0f&&balance<=1000000000.0f&&income<=1000000000u;}
static void money_grant(U logic,U account){float balance;U ledger;if(!account||!rd(account+0x10,&balance,4)||!rd(account+0x28,&ledger,4)||!money_valid(balance,ledger))return;U before=word(account+0x10);typedef void (__thiscall *Credit)(void*,float,U);((Credit)0x422d80)((void*)account,10000.0f,0);begin("MONEY_CHEAT");hx("logic",logic);hx("before_bits",before);hx("after_bits",word(account+0x10));num("amount",10000);emit();}
/* Relaxed from v29: only require native cheat mode plus live game/account. Keep
   focus check and reject text-input modifiers, but do not depend on sampler typing. */
static I money_context(void){U pid=0;H fg=GetForegroundWindow();if(fg)GetWindowThreadProcessId(fg,&pid);return config_loaded&&thread==GetCurrentThreadId()&&pid==GetCurrentProcessId();}
extern void __cdecl native_cheat_event(U*);
void __cdecl cheat_event(U *event){native_cheat_event(event);}
static U money_diag_count;
static void money_keyboard(U event){
 U e[5];B enabled=0; if(!config_loaded||!money_context()||!rd(event,e,sizeof(e))||e[0]!=0)return;
 rd(0x6b65da,&enabled,1); U logic=word(0x6e7794),game=logic?word(logic+0x100):0,account=active_account();
 union{U u;float f;}ch;ch.u=e[2]; U tick=GetTickCount();
 if(!enabled||!account||!game){cheat_pos=0;return;}if(cheat_world!=game||tick-cheat_last_ms>3000)cheat_pos=0;cheat_world=game;cheat_last_ms=tick;
 if((e[4]&6)||!(ch.f>=1.0f&&ch.f<=127.0f)){cheat_pos=0;return;}
 U old=cheat_pos;U hit=money_command_step(&cheat_pos,(U)ch.f);
 if((old||cheat_pos||hit)&&money_diag_count++<100){begin("MONEY_MATCH");num("previous",old);num("matched",cheat_pos);num("complete",hit);num("enabled",enabled);hx("account",account);emit();}
 if(hit)money_grant(logic,account);
}

/* Actual research lifecycle hooks. State reads primarily for diagnostics. */
static U research_transition_logs,credit_logs;
static void research_transition(U stage,U structure,U arg,U account){
 if(!thread||thread!=GetCurrentThreadId())return;
 if(stage!=0&&indicator_sample_structure==structure)indicator_sample_button=0;
 /* Native cancellation leaves later queue entries intact. */
 if(!research_diagnostics||!config_loaded||!structure||research_transition_logs>=512)return;
 U state[4],vec[2],id=word(structure+4),count=0,defid=~0u,cost=0;
 if(!rd(structure+0x78c,state,16)||!rd(structure+0x75c,vec,8)||vec[1]<vec[0]||(vec[1]-vec[0])%32||vec[1]-vec[0]>2048)return;
 count=(vec[1]-vec[0])/32;
 if(state[1]<count){U def=word(vec[0]+32*state[1]+0xc);if(def){defid=word(def+0x18);cost=word(def+0x1c);}}
 research_transition_logs++;begin(stage==0?"RESEARCH_STARTED":stage==1?"RESEARCH_COMPLETING":"RESEARCH_NATIVE_REFUND");hx("building_id",id);hx("structure",structure);num("active",state[0]&255);num("index",state[1]);hx("stored_id",state[2]);hx("definition_id",defid);hx("progress_bits",state[3]);num("available",count);num("definition_cost",cost);hx("argument_bits",arg);if(account){hx("account",account);hx("balance_bits",word(account+0x10));}emit();
}
static void credit_sample(U account,U caller,U amount,U structure){if(caller==0x43112c)research_transition(2,structure,amount,account);if(!research_diagnostics||!config_loaded||thread!=GetCurrentThreadId()||credit_logs>=1000)return;credit_logs++;begin("GOLD_CREDIT");hx("caller",caller);hx("account",account);hx("before_bits",word(account+0x10));hx("amount_bits",amount);emit();}
