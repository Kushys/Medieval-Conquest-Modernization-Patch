/* Medieval Conquest diagnostic probe, baseline MC(3).exe. No game data writes. */
typedef unsigned int U; typedef int I; typedef unsigned char B; typedef void* H;
#define API __declspec(dllimport) __stdcall
#define BAD ((H)-1)
extern H API GetModuleHandleA(const char*); extern I API VirtualProtect(H,U,U,U*); extern I API FlushInstructionCache(H,const void*,U); extern H API GetCurrentProcess(void); extern I API ReadProcessMemory(H,const void*,void*,U,U*); extern H API CreateFileA(const char*,U,U,H,U,U,H); extern I API WriteFile(H,const void*,U,U*,H); extern I API CloseHandle(H); extern U API GetTickCount(void); extern U API GetCurrentProcessId(void); extern U API GetCurrentThreadId(void); extern U API GetModuleFileNameA(H,char*,U); extern I API DisableThreadLibraryCalls(H);
extern U API GetPrivateProfileStringA(const char*,const char*,const char*,char*,U,const char*);
extern U API GetPrivateProfileIntA(const char*,const char*,I,const char*); extern I API WritePrivateProfileStringA(const char*,const char*,const char*,const char*);
extern short API GetAsyncKeyState(I); extern H API GetForegroundWindow(void); extern U API GetWindowThreadProcessId(H,U*); extern I API GetCursorPos(I*); extern I API ScreenToClient(H,I*); extern I API GetClientRect(H,I*);
extern void h_frame(void),h_root(void),h_root_end(void),h_surface(void),h_input(void),h_ownership(void);
extern void h_font(void),h_map_pass(void),h_map_quad(void),h_map_input(void);
extern void h_menu_input(void),h_menu_contain(void),h_menu_point(void);
extern void h_cursor(void);
extern void h_window_local(void),h_window_drag_start(void),h_window_move(void);
extern void h_font_path_a(void),h_font_path_b(void),h_drawtext_a(void),h_drawtext_b(void);
extern void h_tab_input(void),h_tab_contain(void),h_tab_point(void),h_adv_preview(void),h_mon_preview(void),h_profit_graph(void),h_king_graph(void),h_needs_graph(void);
extern void h_layer_key(void),h_button_key(void);
extern void h_camera_begin(void),h_camera_end(void),h_camera_event(void);
extern void h_mouse_event(void),h_research_sample(void),h_gold_sample(void);
extern void h_bookmark_init(void),h_bookmark_home(void);
extern void h_upgrade_draw(void),h_cheat_event(void);
extern void h_gold_credit(void),h_research_complete(void),h_research_start(void);
extern void h_research_live(void),h_money_keyboard(void),h_game_save(void);
extern I API ReadFile(H,void*,U,U*,H); extern U API GetFileSize(H,U*); extern I API GetFileTime(H,void*,void*,void*); extern I API FlushFileBuffers(H); extern I API MoveFileExA(const char*,const char*,U); extern I API DeleteFileA(const char*);
#define NH 47
U back[NH]; U map_alt=0x4a1905;
static struct Hook { U at; void(*fn)(void); U n; B bytes[12]; U protection; } hooks[]={
{0x4b20c0,h_frame,7,{0x83,0xec,0x20,0x53,0x55,0x56,0x57},0},
{0x4b212f,h_root,8,{0x8b,0x01,0x8d,0x54,0x24,0x14,0x52,0x55},0},
{0x4b2137,h_root_end,6,{0xff,0x50,0x24,0x8a,0x46,0x60},0},
{0x665510,h_surface,5,{0x51,0x8b,0x44,0x24,0x24},0},
{0x4b0050,h_input,5,{0xa1,0x88,0x7b,0x6e,0x00},0},
{0x49f1ae,h_ownership,5,{0x51,0x03,0xc6,0x52,0x50},0},
{0x5f81d0,h_font,7,{0x83,0xec,0x1c,0x8b,0x44,0x24,0x2c},0},
{0x4a18db,h_map_pass,10,{0x85,0xed,0x75,0x26,0xd9,0x86,0x20,0x01,0x00,0x00},0},
{0x53b260,h_map_quad,8,{0x83,0xec,0x08,0x53,0x8b,0x5c,0x24,0x18},0},
{0x4a0aac,h_map_input,8,{0xd9,0x44,0x24,0x30,0xd8,0x64,0x24,0x24},0},
{0x5f37e0,h_menu_input,7,{0x55,0x56,0x8b,0xf1,0x8a,0x46,0x2d},0},
{0x5f3ca0,h_menu_contain,6,{0x8a,0x41,0x2d,0x83,0xec,0x20},0},
{0x5f3d21,h_menu_point,6,{0xff,0x15,0x0c,0xf3,0x67,0x00},0},
{0x5f38e0,h_cursor,7,{0x8b,0x44,0x24,0x10,0x8b,0x4e,0x50},0},
{0x5f3827,h_window_local,7,{0x8b,0x44,0x24,0x1c,0x8b,0x11,0x50},0},
{0x5f3940,h_window_drag_start,8,{0x8b,0x54,0x24,0x10,0x8b,0x44,0x24,0x14},0},
{0x5f3c80,h_window_move,7,{0x8b,0x44,0x24,0x04,0x8b,0x51,0x50},0},
{0x5f7c00,h_font_path_a,6,{0x83,0xec,0x54,0x53,0x55,0x56},0},
{0x5f87c0,h_font_path_b,7,{0x83,0xec,0x5c,0x53,0x55,0x56,0x57},0},
{0x5f7fac,h_drawtext_a,6,{0xff,0x15,0xb4,0xf2,0x67,0x00},0},
{0x5f8aea,h_drawtext_b,6,{0xff,0x15,0xb4,0xf2,0x67,0x00},0},
{0x5f471b,h_tab_input,8,{0x2b,0xda,0x8b,0x96,0xa8,0x00,0x00,0x00},0},
{0x5f4810,h_tab_contain,6,{0x8a,0x41,0x2d,0x83,0xec,0x20},0},
{0x5f48a0,h_tab_point,6,{0xff,0x15,0x0c,0xf3,0x67,0x00},0},
{0x48683f,h_adv_preview,6,{0x8b,0x8d,0xe8,0x00,0x00,0x00},0},
{0x4a41f3,h_mon_preview,6,{0x8b,0x8e,0xe4,0x00,0x00,0x00},0},
{0x495b4f,h_profit_graph,5,{0xdb,0x44,0x24,0x18,0x57},0},
{0x49a8fd,h_king_graph,6,{0x8b,0x83,0x9c,0x00,0x00,0x00},0},
{0x49b473,h_needs_graph,6,{0x8b,0x86,0xa4,0x00,0x00,0x00},0},
{0x4b0110,h_layer_key,5,{0xa1,0x88,0x7b,0x6e,0x00},0},
{0x5ee090,h_button_key,6,{0x56,0x8b,0xf1,0x8a,0x46,0x14},0},
{0x49f3c8,h_camera_begin,7,{0x8b,0x74,0x24,0x10,0x8b,0x45,0x6c},0},
{0x49f50d,h_camera_end,6,{0x8b,0x8d,0x84,0x00,0x00,0x00},0},
{0x4ce8a0,h_camera_event,7,{0x51,0xd9,0x05,0x78,0xfc,0x67,0x00},0},
{0x4b9a70,h_mouse_event,8,{0x83,0xec,0x18,0xe8,0x28,0xe1,0x00,0x00},0},
{0x48fdea,h_research_sample,8,{0x8b,0x4d,0x20,0xe8,0xde,0xdd,0x15,0x00},0},
{0x422d40,h_gold_sample,6,{0x83,0xec,0x08,0x56,0x8b,0xf1},0},
{0x4ce6a0,h_bookmark_init,6,{0x53,0x55,0x56,0x57,0x8b,0xc1},0},
{0x49f5d6,h_bookmark_home,5,{0xe8,0x85,0xf6,0x02,0x00},0},
{0x5ee0f0,h_upgrade_draw,6,{0x56,0x8b,0xf1,0x8b,0x46,0x08},0},
{0x4bfac0,h_cheat_event,6,{0x8b,0x44,0x24,0x04,0x8b,0x08},0},
{0x422d80,h_gold_credit,7,{0x83,0xec,0x08,0xd9,0x44,0x24,0x0c},0},
{0x435320,h_research_complete,6,{0x64,0xa1,0x00,0x00,0x00,0x00},0},
{0x490e96,h_research_start,5,{0xa1,0x88,0x7b,0x6e,0x00},0},
{0x431171,h_research_live,6,{0x8a,0x85,0x80,0x07,0x00,0x00},0},
{0x4b93d0,h_money_keyboard,5,{0x8d,0x44,0x24,0x20,0x50},0},
{0x405ec0,h_game_save,8,{0x83,0xec,0x08,0x53,0x8b,0x5c,0x24,0x14},0}};
static const struct Class {U vt;const char* name;U flags;} classes[]={
#include "classes.inc"
};
U active; static H process,logfile=BAD; static U thread,prior_key,session,now,last_sample,sample_frame,frame,root,world,root_index,lines,limited;
static char path[1024],line[2048]; static U pos; static U mouse[2],buttons;
static void zero(void* p,U n){B*q=p;while(n--)*q++=0;}
static I rd(U addr,void* buf,U n){U got=0; if(addr<0x10000 || addr>0x7fffffff || n>0x8000 || addr+n<addr)return 0; return ReadProcessMemory(process,(void*)addr,buf,n,&got)&&got==n;}
static U word(U p){U v=0;rd(p,&v,4);return v;}
static void txt(const char*s){while(*s&&pos<sizeof(line)-2)line[pos++]=*s++;}
static void hex(U v){static const char d[]="0123456789ABCDEF";I i;txt("0x");for(i=28;i>=0;i-=4)if(pos<sizeof(line)-2)line[pos++]=d[(v>>i)&15];}
static void dec(U v){char b[12];U i=0;do{b[i++]=(char)('0'+v%10);v/=10;}while(v);while(i&&pos<sizeof(line)-2)line[pos++]=b[--i];}
static void signeddec(U v){if(v&0x80000000){txt("-");v=0-v;}dec(v);}
static void hx(const char*k,U v){txt(" ");txt(k);txt("=");hex(v);}
static void num(const char*k,U v){txt(" ");txt(k);txt("=");signeddec(v);}
static void arr(const char*k,const U*a,U n,I ashex){U i;txt(" ");txt(k);txt("=");for(i=0;i<n;i++){if(i)txt(",");if(ashex)hex(a[i]);else signeddec(a[i]);}}
static const struct Class* cls(U vt){U i;for(i=0;i<sizeof(classes)/sizeof(classes[0]);i++)if(classes[i].vt==vt)return &classes[i];return 0;}
static void type(U vt){const struct Class*c=cls(vt);hx("vt",vt);txt(" class=");txt(c?c->name:"unknown");}
static void begin(const char*s){pos=0;txt(s);num("session",session);num("ms",now);num("frame",frame);}
static U verbose_log;
static I log_prefix(const char *s){U i=0;while(s[i]&&line[i]==s[i])i++;return !s[i];}
static I log_keep(void){return verbose_log||log_prefix("READY")||log_prefix("REFUSED")||log_prefix("CONFIG ")||log_prefix("SCALE_")||log_prefix("UI_RESTORE")||log_prefix("QUEUE_")||log_prefix("RESEARCH_QUEUE_")||log_prefix("RESEARCH_QUEUED_START")||log_prefix("RESEARCH_CANCELLED")||log_prefix("MONEY_CHEAT")||log_prefix("PROCESS_EXIT")||log_prefix("COUNTERS")||log_prefix("GAME_HOTKEY_")||log_prefix("MOD_HOTKEY_");}
static void emit(void){U got;if(logfile==BAD||!log_keep())return;line[pos++]='\n';if(!WriteFile(logfile,line,pos,&got,0)||got!=pos){CloseHandle(logfile);logfile=BAD;active=0;}lines++;}
static void message(const char*s){begin(s);emit();}
static B font_copy[80*800];
static void openlog(void){U i,n=GetModuleFileNameA(0,path,sizeof(path));const char*s="MC_Modernization.log";if(!n||n>=sizeof(path)-40)n=0;while(n&&path[n-1]!='\\'&&path[n-1]!='/')n--;for(i=0;s[i];i++)path[n+i]=s[i];path[n+i]=0;logfile=CreateFileA(path,0x40000000,1,0,2,0x80,0);}
/* Direct-mapped change cache; collisions cause extra logs, never unsafe reads. */
#define CACHE 8192
static struct Seen {U key,kind,hash;} seen[CACHE];
static U mix(U h,U v){return (h^v)*16777619u;}
static U hashwords(const U*p,U n){U h=2166136261u;while(n--)h=mix(h,*p++);return h;}
static I changed(U kind,U key,U h){U i=mix(key,kind)&(CACHE-1);struct Seen*s=&seen[i];if(s->key==key&&s->kind==kind&&s->hash==h)return 0;s->key=key;s->kind=kind;s->hash=h;return 1;}


/* Native 1440p layout template; translated by screen anchors at startup. */
static I screen_w=2560,screen_h=1440;
static I sides[8][4]={{0,0,91,434},{2469,0,2560,434},{945,0,1615,70},{0,1309,221,1440},{1107,1361,1453,1440},{2338,1290,2560,1440},{0,0,0,0},{1080,671,1480,768}};
static I anchors[8][2]={{0,0},{2560,0},{1280,0},{0,1440},{1280,1440},{2560,1440},{0,0},{1280,720}};
static U pause_draws,pause_inputs,hi_enabled=1,prior_f6,font_paths,unscaled_fonts;
static U cache_enabled=1,prior_f4,cache_hits,cache_misses,cache_valid_hits,cache_rejected_hits,cache_bypass,art_decodes,art_saved,config_loaded,config_start,config_save=1,perf_last_ms,perf_last_frame;
static char config_path[1024];
static U small_map_markers=0;
static U tip_enabled=1,prior_f10,expanded_enabled=1,prior_f11,prior_f12,scale_index=1;
static I sn=3,sd=2;
static const I scale_num[]={5,3,7,2},scale_den[]={4,2,4,1};
static const char* preset_key(void){return screen_w==1920?"Scale1080p":screen_w==3840?"Scale2160p":"Scale1440p";}
static U preset_index(void){return screen_w==1920?0:screen_w==3840?3:1;}
static I supported_resolution(I w,I h){return (w==1920&&h==1080)||(w==2560&&h==1440)||(w==3840&&h==2160);}
static void read_config(void){U n=GetModuleFileNameA(0,config_path,sizeof(config_path)),i;const char*name="MC_Modernization.ini";if(!n||n>=sizeof(config_path)-32)n=0;while(n&&config_path[n-1]!='\\'&&config_path[n-1]!='/')n--;for(i=0;name[i];i++)config_path[n+i]=name[i];config_path[n+i]=0;
 U pct=GetPrivateProfileIntA("UI","ScalePercent",0,config_path);if(!pct)pct=GetPrivateProfileIntA("UI",preset_key(),100*scale_num[preset_index()]/scale_den[preset_index()],config_path);scale_index=preset_index();for(i=0;i<4;i++)if(pct==100*scale_num[i]/scale_den[i]&&(screen_w!=1920||i<3))scale_index=i;
 sn=scale_num[scale_index];sd=scale_den[scale_index];config_start=GetPrivateProfileIntA("UI","StartEnabled",1,config_path)!=0;config_save=GetPrivateProfileIntA("UI","SaveScale",1,config_path)!=0;cache_enabled=GetPrivateProfileIntA("UI","FontCache",1,config_path)!=0;hi_enabled=GetPrivateProfileIntA("UI","HighResolutionFonts",1,config_path)!=0;tip_enabled=GetPrivateProfileIntA("UI","Tooltips",1,config_path)!=0;expanded_enabled=GetPrivateProfileIntA("UI","ExpandedMap",1,config_path)!=0;small_map_markers=GetPrivateProfileIntA("UI","SmallMapMarkers",0,config_path)!=0;config_loaded=1;}
static void save_scale(void){const char* v[]={"125","150","175","200"};if(config_save){const char*k=GetPrivateProfileIntA("UI","ScalePercent",0,config_path)?"ScalePercent":preset_key();WritePrivateProfileStringA("UI",k,v[scale_index],config_path);}}

static U hi_attempts,hi_draws,hi_fallbacks,tip_draws,tip_fallbacks,hi_reason_mask;
static U hi_path,hi_frame,hi_root,hi_shift,hi_ready,hi_maskobj,tip_scope,tip_valid;
static I tip_dx,tip_dy,tip_l,tip_t,tip_x,tip_y,tip_w,tip_h;
static U hi_key,hi_checked,hi_ok,hi_fit_draws;static char hi_face[32];
static I hi_pad,hi_extent_x,hi_extent_y,hi_fit_x,hi_fit_y,hi_scan_x,hi_scan_y;
static U root_vt,window_cursor_owner; static I window_cursor_raw[2];
static U menu_root,contain_menu,menu_draws,menu_fonts,menu_inputs,menu_owns,menu_skips;
static U hud,client_ok,draws,mapped,claimed,skipped,busy,font_draws,font_fallback;
static U last_dst,last_dst_frame;
static U map_pass=2,map_quads,map_input,map_fallback,expanded_quads,expanded_inputs,marker_quads,draw_seen,font_log_mask,map_log_mask;
/* A copy of the four native 28-byte XY/UV/color vertices; native submission copies synchronously. */
static U map_vertices[28];
static U float_bits(float f){union {float f; U u;} v;v.f=f;return v.u;}
static I map_supported(U p){U a[4];if(!p||!rd(p+0x114,a,16))return 0;return a[0]>=0x3d4ccccd&&a[0]<=0x41a00000&&a[1]==float_bits((float)(screen_w-84))&&a[2]==float_bits((float)(screen_h-78))&&a[3]==0x42b40000;}
static I expanded_supported(U p){U a[3];B on=0;return expanded_enabled&&p&&rd(p+0x124,a,12)&&rd(p+0x130,&on,1)&&on&&a[0]==float_bits((float)(screen_w/2))&&a[1]==float_bits((float)(screen_h/2))&&a[2]==0x43960000;}
static float forward_map(float v,float anchor){return anchor+(v-anchor)*((float)sn/(float)sd);}
static float inverse_map(float v,float anchor){return anchor+(v-anchor)*((float)sd/(float)sn);}
static U scratch[9][808*604];
static struct PhaseAxis {I q[2],phase[2],weight;} phase_x[1600],phase_y[1200];
static I seed_x[3][800],seed_y[3][600];
static I mode;

static I side_rect(const I*r){I i,j;for(i=0;i<6;i++){for(j=0;j<4;j++)if(r[j]!=sides[i][j])break;if(j==4)return i;}return -1;}
static I live_layout(U p){U v[2],i;I r[4],mask=0;if(!p||word(p)!=0x67fe40||!rd(p+0xe4,v,8)||v[1]<v[0]||v[1]-v[0]!=96)return 0;for(i=0;i<6;i++){if(!rd(v[0]+16*i,r,16))return 0;I s=side_rect(r);if(s>=0)mask|=1<<s;}return mask==63;}
/* Half-open edges and inverse in sidebar-local coordinates. */
static I edge(I n){return (sn*n+sd-1)/sd;}
static I inverse(I n){return sd*n/sn;}
static I menu_base(I n,I a);
static I floor_div(I x,I d);
/* Group 0 is the untouched v14 window set. New groups can be toggled independently. */
static U group_on[4]={1,1,1,1},prior_group[4];
static U new_draws[4],new_rejects[4],tab_inputs,tab_owns,preview_draws,chart_draws,geometry_rejects;
static const struct WindowSpec {U vt;I w,h,group,center;} windows[]={
 {0x682c68,321,461,0,3},{0x681f98,351,501,0,0},{0x68233c,350,500,0,0},
 {0x68243c,800,600,1,3},{0x68253c,0,0,1,0},{0x682098,0,0,1,0},{0x682738,800,600,1,3},{0x682198,350,500,1,0},{0x6823bc,350,500,1,0},{0x681ea0,340,550,1,0},
 {0x682b58,301,401,1,3},{0x682fd0,301,301,1,3},{0x6828b4,800,600,1,1},{0x682de0,800,600,1,3},
 {0x681e00,340,550,2,0},{0x682218,340,550,2,0},{0x682bd4,340,550,2,0},
 {0x6824bc,320,455,3,0},{0x6827b4,320,455,3,0},{0x682834,320,455,3,0}};
static const struct WindowSpec* window_spec(U vt){for(U i=0;i<sizeof(windows)/sizeof(windows[0]);i++)if(windows[i].vt==vt)return windows+i;return 0;}
static I window_vt(U vt){const struct WindowSpec*s=window_spec(vt);return s&&group_on[s->group]&&(vt!=0x682b58||expanded_enabled);}
static I window_type(U p){return window_vt(word(p));}
/* Exact read-only equivalent of native sprite width/height helpers 005e9700/005e9730. */
static I sprite_size(U p,I *w,I *h){U spr=word(p+0x3c),resource=word(spr+0x34),off=word(spr+8),rects=word(resource+0x1c);I q[4];if(!spr||!resource||!rects||word(resource+0x14)<1||off>0x100000||(off&3)||!rd(rects+off,q,16))return 0;if(q[0]<0||q[1]<0||q[2]<q[0]||q[3]<q[1]||q[2]>16383||q[3]>16383)return 0;*w=q[2]-q[0]+1;*h=q[3]-q[1]+1;return *w>=200&&*w<=800&&*h>=200&&*h<=600;}
static I window_layout(U p,I*r){I a[10],w,h;const struct WindowSpec*s=window_spec(word(p));if(!s||!group_on[s->group]||(s->vt==0x682b58&&!expanded_enabled)||!rd(p+0x40,a,40))return 0;
 if(s->vt==0x68243c){if(a[0]!=88||a[1]!=99||a[2]!=708||a[3]!=476||a[4]!=screen_w/2-400||a[5]!=screen_h/2-300)return 0;w=800;h=600;}else if(s->vt==0x682738){if(a[0]||a[1]||a[2]!=800||a[3]!=600||a[4]!=screen_w/2-400||a[5]!=screen_h/2-300)return 0;w=800;h=600;}else{if(a[0]||a[1])return 0;w=a[2]+1;h=a[3]+1;}
 if(!s->w){I sw,sh;if(!sprite_size(p,&sw,&sh))return 0;if(w!=sw||h!=sh)return 0;}
 if((s->group==0?(w<319||w>351||h<454||h>501):(s->w&&(w!=s->w||h!=s->h)))||a[4]<0||a[5]<0||a[4]+w>screen_w||a[5]+h>screen_h)return 0;
 if(edge(w)>screen_w||edge(h)>screen_h)return 0;
 r[0]=a[4];r[1]=a[5];r[2]=a[4]+w;r[3]=a[5]+h;return 1;}
static I window_offset(I*r,I axis,U vt){I size=r[axis+2]-r[axis];const struct WindowSpec*s=window_spec(vt);if(!axis&&s&&(s->group==2||vt==0x681ea0||vt==0x682198||vt==0x6823bc||vt==0x68233c||vt==0x68253c||vt==0x682098||s->group==3))return edge(size)-size+edge(91)-91+54;return s&&(s->center&(1<<axis))?(edge(size)-size+1)/2:0;}
static I window_base(I*r,I axis,U vt){I max=(axis?screen_h:screen_w)-edge(r[axis+2]-r[axis]),n=r[axis]-window_offset(r,axis,vt);return n<0?0:n>max?max:n;}
static I map_popup_r[4]={1130,482,1431,883};
static void init_resolution(I w,I h){I dx=w-2560,dy=h-1440;screen_w=w;screen_h=h;
 for(I i=0;i<8;i++){if(i==6)continue;I x=i==1||i==5?dx:i==2||i==4||i==7?dx/2:0;I y=i==3||i==4||i==5?dy:i==7?dy/2:0;sides[i][0]+=x;sides[i][2]+=x;sides[i][1]+=y;sides[i][3]+=y;anchors[i][0]+=x;anchors[i][1]+=y;}
 map_popup_r[0]+=dx/2;map_popup_r[2]+=dx/2;map_popup_r[1]+=dy/2;map_popup_r[3]+=dy/2;}
static float expanded_offset(I axis){I a=axis?(screen_h/2):(screen_w/2);return (float)window_base(map_popup_r,axis,0x682b58)+(float)(a-map_popup_r[axis])*((float)sn/(float)sd)-(float)a;}
static I window_inv(I v,I*r,I axis,U vt){return r[axis]+floor_div(sd*(v-window_base(r,axis,vt)),sn);}
static I base_axis(I s,I axis){I lo=sides[s][axis],a=anchors[s][axis];return s==7?((axis?screen_h:screen_w)-edge(sides[7][axis+2]-lo))/2:s==6?(window_vt(root_vt)?window_base(sides[6],axis,root_vt):menu_base(lo,a)):a-edge(a-lo);}
/* Strict class and fixed, non-draggable menu geometry. Never write native geometry. */
static I menu_y_anchor(U vt){return vt==0x681d84?screen_h:0;}
static I menu_layout(U p,I *r,I *anchor){U v=word(p),a[10],drag=0;I x,y,w,h;
 if(window_type(p)){*anchor=0;return window_layout(p,r);}
 /* Selected-character badges and trash panel: recorded dimensions and native placement formulas. */
 if(v==0x681d84||v==0x6829dc){if(!group_on[1]||!rd(p+0x40,a,40)||a[0]||a[1]||a[6]||a[7]||a[8]||a[9])return 0;
  x=(I)a[4];y=(I)a[5];w=(I)a[2]+1;h=(I)a[3]+1;
  if(v==0x681d84){if(w!=45||h!=42||x<screen_w/2-176||x>screen_w/2+132||(x-(screen_w/2-176))%44||y<0||y>screen_h-120||(screen_h-120-y)%41)return 0;*anchor=(screen_w/2);r[0]=x;}
  else{if(w!=75||h!=72||x!=screen_w-165||y!=93)return 0;*anchor=screen_w;r[0]=x-((x-1)%sd);}
  r[1]=y-y%sd;r[2]=x+w;r[3]=y+h;return 1;}
 if(v!=0x6822bc&&v!=0x682018&&v!=0x681d04&&v!=0x682118&&v!=0x6825c0&&v!=0x682ad8&&v!=0x682a58&&v!=0x682ce4)return 0;
 if(!rd(p+0x40,a,40)||!rd(p+0x80,&drag,1)||drag||a[0]||a[6]||a[7]||a[8]||a[9])return 0;
 x=(I)a[4];y=(I)a[5];w=(I)a[2]+1;h=(I)a[3]+1;
 if(v==0x682018){if(a[1]!=55||a[2]!=231||a[3]!=170||x<51||x>600||y<0||y>400)return 0;*anchor=0;}
 else if(v==0x6822bc){if(a[1]||w<1||w>670||h!=55||x!=51||y<96||y>356||(y-96)%52)return 0;*anchor=0;}
 else {if(a[1]||w<1||w>670||h!=44||y<0||y>400||x<screen_w-760||x+w!=screen_w-53)return 0;*anchor=screen_w;h=54;}
 /* Use common parity for every parent/child and adjacent HUD region; bounds are visual, not hit rectangles. */
 r[0]=x-((x-(*anchor?1:0))%sd);r[1]=y-(y%sd);r[2]=x+w;r[3]=y+h;return 1;}
static I floor_div(I x,I d){return x>=0?x/d:-((-x+d-1)/d);}
static I menu_base(I n,I a){return a-floor_div(sn*(a-n)+sd-1,sd);}
static I menu_inverse(I v,I n,I a){return n+floor_div(sd*(v-menu_base(n,a)),sn);}
static I menu_scope(void){return menu_root&&root==menu_root;}
/* New render-only coordinate overrides. Native fields are restored before leaving the owning layer. */
static U temporary_owner,temporary_at,temporary_vt;static I temporary_saved[4];
static void restore_geometry(void){if(temporary_owner&&word(temporary_owner)==temporary_vt){I*d=(I*)temporary_at;for(I i=0;i<4;i++)d[i]=temporary_saved[i];}temporary_owner=temporary_at=temporary_vt=0;}
static I rect_inside(const I *q,const I*r){return q[0]>=r[0]&&q[1]>=r[1]&&q[2]>=q[0]&&q[3]>=q[1]&&q[2]<r[2]&&q[3]<r[3];}
static I geometry_override(U obj,U offset,I preview,I*locals){I r[4],q[4],out[4],expected[4];U vt=word(obj);const struct WindowSpec*s=window_spec(vt);
 if(!s||!menu_scope()||obj!=root||temporary_owner||!window_layout(obj,r)||!rd(obj+offset,q,16))return 0;
 if(preview){expected[0]=r[0]+82;expected[1]=r[1]+112;expected[2]=r[0]+146;expected[3]=r[1]+290;}
 else if(vt==0x6824bc){expected[0]=r[0]+60;expected[1]=r[1]+150;expected[2]=r[0]+255;expected[3]=r[1]+340;}
 else if(vt==0x6827b4){expected[0]=r[0]+60;expected[1]=r[1]+150;expected[2]=r[0]+256;expected[3]=r[1]+355;}
 else {expected[0]=r[0]+68;expected[1]=r[1]+150;expected[2]=r[0]+262;expected[3]=r[1]+354;}
 I ok=rect_inside(q,r);for(I i=0;i<4;i++)if(q[i]!=expected[i])ok=0;
 if(!ok){geometry_rejects++;if(changed(140,obj,hashwords((U*)q,4))){begin("V29_GEOMETRY_NATIVE");type(vt);num("offset",offset);arr("actual",(U*)q,4,0);arr("expected",(U*)expected,4,0);emit();}return 0;}
 for(I i=0;i<4;i++){I axis=i&1;out[i]=window_base(r,axis,vt)+edge(q[i]-r[axis]+(i>=2&&preview?1:0))-(i>=2&&preview?1:0);}
 if(locals){locals[0x18/4]=out[0];locals[0x84/4]=out[3];}
 temporary_owner=obj;temporary_at=obj+offset;temporary_vt=vt;for(I i=0;i<4;i++){temporary_saved[i]=q[i];((I*)temporary_at)[i]=out[i];}
 if(preview)preview_draws++;else chart_draws++;
 if(changed(141,obj,hashwords((U*)out,4))){begin(preview?"V29_PREVIEW":"V29_CHART");type(vt);arr("native",(U*)q,4,0);arr("physical",(U*)out,4,0);emit();}return 1;
}
static I screen_side(I x,I y){I s;for(s=0;s<6;s++){I l=base_axis(s,0),t=base_axis(s,1);if(x>=l&&x<l+edge(sides[s][2]-sides[s][0])&&y>=t&&y<t+edge(sides[s][3]-sides[s][1]))return s;}return -1;}
static I inv_x(I x,I s){return sides[s][0]+inverse(x-base_axis(s,0));}
static I inv_y(I y,I s){return sides[s][1]+inverse(y-base_axis(s,1));}
static void counters(void){begin("COUNTERS");num("scaled_draws",draws);num("mapped_input",mapped);num("ownership_matches",claimed);num("unsupported_side_draws",skipped);num("font_draws",font_draws);num("font_fallback",font_fallback);num("map_quads",map_quads);num("map_input",map_input);num("map_fallback",map_fallback);num("expanded_quads",expanded_quads);num("expanded_inputs",expanded_inputs);num("marker_quads",marker_quads);num("scale_percent",100*sn/sd);num("menu_draws",menu_draws);num("menu_fonts",menu_fonts);num("menu_inputs",menu_inputs);num("menu_owns",menu_owns);num("menu_skips",menu_skips);num("pause_draws",pause_draws);num("pause_inputs",pause_inputs);num("hi_font",hi_enabled);num("hi_attempts",hi_attempts);num("hi_fit_draws",hi_fit_draws);num("hi_draws",hi_draws);num("hi_fallbacks",hi_fallbacks);num("tooltip_draws",tip_draws);num("tooltip_fallbacks",tip_fallbacks);num("font_paths",font_paths);num("unscaled_font_samples",unscaled_fonts);num("new_2d_draws",new_draws[1]);num("new_stats_draws",new_draws[2]);num("new_chart_draws",new_draws[3]);num("new_2d_rejects",new_rejects[1]);num("new_stats_rejects",new_rejects[2]);num("new_chart_rejects",new_rejects[3]);num("tab_inputs",tab_inputs);num("tab_owns",tab_owns);num("preview_draws",preview_draws);num("chart_draws",chart_draws);num("geometry_rejects",geometry_rejects);num("cache_hits",cache_hits);num("cache_misses",cache_misses);num("cache_valid_hits",cache_valid_hits);num("cache_rejected_hits",cache_rejected_hits);num("cache_bypass",cache_bypass);num("art_decodes",art_decodes);num("art_saved",art_saved);num("interval_ms",now-perf_last_ms);num("interval_frames",frame-perf_last_frame);num("cache_on",cache_enabled);emit();perf_last_ms=now;perf_last_frame=frame;}
#include "hotkeys.inc"
static Binding game_keys[ACTIONS],mod_keys[2];
static U continuous_camera=1;
static U camera_bookmarks=1,bookmark_restore_view=1;
static U research_indicator,research_queue_enabled=1,research_cancel_enabled=1;
static U mouse_drag=1,mouse_wheel=1,invert_drag,camera_diagnostics,research_diagnostics;
static I drag_sensitivity=100,wheel_sensitivity=100;
static I bounded_percent(I n){return n<10?10:n>400?400:n;}
static void load_mouse_config(void){research_queue_enabled=GetPrivateProfileIntA("Research","ResearchQueue",1,config_path)!=0;research_cancel_enabled=GetPrivateProfileIntA("Research","CancelResearch",1,config_path)!=0;research_indicator=GetPrivateProfileIntA("Research","ActiveResearchIndicator",1,config_path)!=0;camera_bookmarks=GetPrivateProfileIntA("Camera","CameraBookmarks",1,config_path)!=0;bookmark_restore_view=GetPrivateProfileIntA("Camera","BookmarkRestoreView",1,config_path)!=0;mouse_drag=GetPrivateProfileIntA("Camera","MiddleMouseDrag",1,config_path)!=0;mouse_wheel=GetPrivateProfileIntA("Camera","MouseWheelZoom",1,config_path)!=0;invert_drag=GetPrivateProfileIntA("Camera","InvertDrag",0,config_path)!=0;drag_sensitivity=bounded_percent(GetPrivateProfileIntA("Camera","DragSensitivity",100,config_path));wheel_sensitivity=bounded_percent(GetPrivateProfileIntA("Camera","WheelSensitivity",100,config_path));camera_diagnostics=GetPrivateProfileIntA("Debug","CameraDiagnostics",0,config_path)!=0;research_diagnostics=GetPrivateProfileIntA("Debug","ResearchDiagnostics",0,config_path)!=0;}

static U ui_debug,game_keys_on,game_key_scope,prior_reset,prior_2d,native_debug_log,native_flags_previous=~0u;
static I bookmark_conflict(const Binding*b,U n){if(!camera_bookmarks)return 0;for(U i=0;i<n;i++)for(U k=0x70;k<=0x73;k++)if(contains_key(b+i,k))return 1;return 0;}
static void load_hotkeys(void){char buf[128];I ok=1;ui_debug=GetPrivateProfileIntA("Debug","UIDebugMode",0,config_path)!=0;game_keys_on=GetPrivateProfileIntA("GameHotkeys","Enabled",1,config_path)!=0;continuous_camera=GetPrivateProfileIntA("GameHotkeys","ContinuousCamera",1,config_path)!=0;native_debug_log=GetPrivateProfileIntA("Debug","NativeDebugLog",0,config_path)!=0;
 const char*names[]={"ToggleUIScaling","RestoreNativeUI"},*defs[]={"F5","F6"};for(U i=0;i<2;i++){U n=GetPrivateProfileStringA("ModHotkeys",names[i],defs[i],buf,sizeof(buf),config_path);if(n>=sizeof(buf)-1||!parse_keys(buf,mod_keys+i))ok=0;}if(!ok||!valid_mod_keys(mod_keys)||bookmark_conflict(mod_keys,2)){parse_keys("F5",mod_keys);parse_keys("F6",mod_keys+1);message("MOD_HOTKEY_CONFIG_FALLBACK");}
 ok=1;for(U i=0;i<ACTIONS;i++){U n=GetPrivateProfileStringA("GameHotkeys",actions[i].name,actions[i].def,buf,sizeof(buf),config_path);if(n>=sizeof(buf)-1||!parse_keys(buf,game_keys+i))ok=0;}if(!ok||!valid_game_keys(game_keys,mod_keys)||bookmark_conflict(game_keys,ACTIONS)){for(U i=0;i<ACTIONS;i++)parse_keys(actions[i].def,game_keys+i);message("GAME_HOTKEY_CONFIG_FALLBACK");}if(!valid_game_keys(game_keys,mod_keys)||bookmark_conflict(game_keys,ACTIONS)){game_keys_on=0;message("GAME_HOTKEY_CONFLICT_DISABLED");}}
static U held(U k){return (GetAsyncKeyState(k)&0x8000)!=0;}
static U binding_held(const Binding*b){for(U i=0;i<b->n;i++)if(held(b->k[i]))return 1;return 0;}
extern void __thiscall native_layer_key(void*,U*);
extern void __thiscall native_button_key(void*,U,I);
void __thiscall game_layer_key(void*p,U*event){if(!thread||thread!=GetCurrentThreadId()){native_layer_key(p,event);return;}U previous=game_key_scope,pid=0;B typing=1;U sampler=word(0x6e7bd4);if(sampler)rd(sampler+8,&typing,1);game_key_scope=0;H fg=GetForegroundWindow();if(fg)GetWindowThreadProcessId(fg,&pid);if((game_keys_on||camera_bookmarks)&&config_loaded&&thread==GetCurrentThreadId()&&pid==GetCurrentProcessId()&&word((U)p)==0x67fe40&&event&&event[0]==0&&!(((B*)event)[16]&7)&&!typing)game_key_scope=(U)p;native_layer_key(p,event);game_key_scope=previous;}
void __thiscall game_button_key(void*p,U raw,I ch){if(game_key_scope&&thread==GetCurrentThreadId()&&word((U)p)==0x6886b4){U id=word((U)p+4);I accel=(I)word((U)p+0x34);for(U i=0;i<HUD_ACTIONS;i++)if(id==actions[i].id&&accel==actions[i].ch&&(!actions[i].offset||word(game_key_scope+actions[i].offset)==(U)p)){if(i==6&&camera_bookmarks)return;if(!game_keys_on)break;I c=mapped_char(i,game_keys+i,raw,ch);if(continuous_camera&&i<6){if(c==actions[i].ch||contains_key(game_keys+i,raw))return;}if(c!=ch&&contains_key(game_keys+i,raw)&&ui_debug){begin("GAME_HOTKEY_ALIAS");txt(" action=");txt(actions[i].name);hx("vk",raw);emit();}ch=c;break;}}native_button_key(p,raw,ch);}

/* Native camera update already applies frame delta and bounds. No persistent button writes. */
static U cam_buttons[6],cam_saved[6],cam_owner,cam_last_mask;
static I camera_context(void){U pid=0,mgr=word(0x6e7b88),sampler=word(0x6e7bd4);B typing=1,blocked=1;H fg=GetForegroundWindow();if(fg)GetWindowThreadProcessId(fg,&pid);return config_loaded&&thread==GetCurrentThreadId()&&pid==GetCurrentProcessId()&&sampler&&rd(sampler+8,&typing,1)&&!typing&&mgr&&rd(mgr+0x62,&blocked,1)&&!blocked;}
static I camera_unmodified(void){return !held(0x10)&&!held(0x11)&&!held(0x12);}
extern void __thiscall native_camera_event(void*,U*);
void __thiscall camera_event(void*p,U*e){if(game_keys_on&&camera_context()&&e&&e[0]==0&&!(((B*)e)[16]&7)&&word(word((U)p))==0x4ce8a0){float raw=0;if(rd((U)e+4,&raw,4)&&raw>=1&&raw<=255){U k=(U)raw;I pan=pan_key(game_keys,k);I native=pan_native(k);if(continuous_camera&&(pan>=0||native>=0||k==107||k==109||camera_key(game_keys,k)))return;if(!continuous_camera&&(pan>=0||native>=0)){if(pan<0)return;U copy[5];for(U i=0;i<5;i++)copy[i]=e[i];union {float f;U u;} v;v.f=(float)pan_vk[pan];copy[1]=v.u;native_camera_event(p,copy);return;}}}native_camera_event(p,e);}
static void camera_restore(U owner){if(cam_owner!=owner)return;for(U i=0;i<6;i++){U b=cam_buttons[i];if(b&&word(b)==0x6886b4&&word(owner+actions[i].offset)==b&&word(b+8)==3)*(U*)(b+8)=cam_saved[i];cam_buttons[i]=0;}cam_owner=0;}
#include "gameplay.inc"
#include "persistence.inc"
#include "mouse_camera.inc"
#include "bookmarks.inc"
static void camera_tick(U owner,U camera,float dt){I valid=camera_context()&&word(owner)==0x67fe40&&camera&&dt>0.0f&&dt<=0.25f;B hidden=1;if(valid)valid=rd(owner+0x2d,&hidden,1)&&!hidden;bookmark_tick(owner,camera,valid);valid=valid&&camera_unmodified();mouse_camera_tick(owner,camera,valid);if(!valid||!continuous_camera||!game_keys_on)return;cam_owner=owner;U mask=0;for(U i=0;i<6;i++){U b=word(owner+actions[i].offset);B en=0;U state=word(b+8);if(binding_held(game_keys+i)&&word(b)==0x6886b4&&word(b+4)==actions[i].id&&rd(b+0x14,&en,1)&&en&&(state==1||state==2)){cam_buttons[i]=b;cam_saved[i]=state;*(U*)(b+8)=3;mask|=1u<<i;}}
 I x=(I)binding_held(game_keys+HUD_ACTIONS+3)-(I)binding_held(game_keys+HUD_ACTIONS+2),y=(I)binding_held(game_keys+HUD_ACTIONS)-(I)binding_held(game_keys+HUD_ACTIONS+1);if(x||y){float angle=pan_angle(x,y);typedef void (__thiscall *Pan)(void*,float,float);((Pan)0x4cee60)((void*)camera,angle,dt*0.8f);mask|=(U)(x+1)<<6;mask|=(U)(y+1)<<8;}if(mask!=cam_last_mask){begin("CAMERA_HELD");num("mask",mask);emit();cam_last_mask=mask;}}
static void observe_native_flags(void){B a[3],movie;if(!native_debug_log||!rd(0x6b65d8,a,3)||!rd(0x6e7c10,&movie,1))return;U flags=(a[0]!=0)|((a[1]!=0)<<1)|((a[2]!=0)<<2)|((movie!=0)<<3);if(flags==native_flags_previous)return;native_flags_previous=flags;begin("NATIVE_DEBUG_FLAGS");num("engine",flags&1);num("game",(flags>>1)&1);num("cheats",(flags>>2)&1);num("movie",(flags>>3)&1);emit();}
void observe(U kind,U* regs){U p=regs[6],sp=regs[3]+4;H hwnd;U pid=0;I rc[4];
 if(kind==0){if(!thread)thread=GetCurrentThreadId();if(thread!=GetCurrentThreadId())return;now=GetTickCount();frame++;world=p;indicator_root=0;root=0;hud=0;menu_root=0;map_pass=2;hi_ready=tip_scope=tip_valid=0;hwnd=GetForegroundWindow();if(hwnd)GetWindowThreadProcessId(hwnd,&pid);client_ok=pid==GetCurrentProcessId()&&GetClientRect(hwnd,rc)&&rc[0]==0&&rc[1]==0; gameplay_frame(); if(client_ok&&!config_loaded&&supported_resolution(rc[2],rc[3]))init_resolution(rc[2],rc[3]);client_ok=client_ok&&rc[2]==screen_w&&rc[3]==screen_h;
 if(!config_loaded&&client_ok){read_config();load_mouse_config();load_hotkeys();verbose_log=ui_debug||native_debug_log||camera_diagnostics||research_diagnostics||GetPrivateProfileIntA("Debug","VerboseLog",0,config_path)!=0;begin("CONFIG");num("width",screen_w);num("height",screen_h);num("percent",100*sn/sd);num("cache",cache_enabled);num("start",config_start);num("small_map_markers",small_map_markers);num("ui_debug",ui_debug);num("game_hotkeys",game_keys_on);num("continuous_camera",continuous_camera);num("mouse_drag",mouse_drag);num("mouse_wheel",mouse_wheel);num("drag_sensitivity",drag_sensitivity);num("wheel_sensitivity",wheel_sensitivity);num("research_diagnostics",research_diagnostics);num("camera_diagnostics",camera_diagnostics);num("camera_bookmarks",camera_bookmarks);num("bookmark_restore_view",bookmark_restore_view);num("research_indicator",research_indicator);emit();}
 observe_native_flags();U key=binding_held(mod_keys),reset=binding_held(mod_keys+1),mods=held(0x10)||held(0x11)||held(0x12);U startup=config_start&&client_ok&&!active; if(startup)config_start=0; U reset_edge=reset&&!prior_reset&&!mods&&client_ok;prior_reset=reset; if(reset_edge){if(active)counters();restore_geometry();mode=active=0;hi_ready=tip_scope=tip_valid=0;message("UI_RESTORE_NATIVE");}
 U f4=held(0x76)&&held(0x11)&&!held(0x10)&&!held(0x12);if(ui_debug&&f4&&!prior_f4&&active&&client_ok){cache_enabled=!cache_enabled;message(cache_enabled?"FONT_CACHE_ON":"FONT_CACHE_OFF");}prior_f4=f4;
 if(!reset_edge&&(startup||(key&&!prior_key&&!mods))&&pid==GetCurrentProcessId()){if(active)counters();if(client_ok){mode=active?0:4;active=mode!=0;sn=scale_num[scale_index];sd=scale_den[scale_index];if(active){session++;cache_hits=cache_misses=cache_valid_hits=cache_rejected_hits=cache_bypass=art_decodes=art_saved=0;perf_last_ms=now;perf_last_frame=frame;draws=mapped=claimed=skipped=font_draws=font_fallback=map_quads=map_input=map_fallback=draw_seen=font_log_mask=map_log_mask=expanded_quads=expanded_inputs=marker_quads=0;menu_draws=menu_fonts=menu_inputs=menu_owns=menu_skips=0;zero(seen,sizeof(seen));pause_draws=pause_inputs=font_paths=unscaled_fonts=hi_fit_draws=hi_attempts=hi_draws=hi_fallbacks=tip_draws=tip_fallbacks=hi_reason_mask=0;hi_ready=tip_scope=tip_valid=0;zero(new_draws,sizeof(new_draws));zero(new_rejects,sizeof(new_rejects));tab_inputs=tab_owns=preview_draws=chart_draws=geometry_rejects=0;}begin(active?"SCALE_ON_FILTERED":"SCALE_OFF");num("percent",100*sn/sd);emit();}else{mode=0;active=0;message("REFUSED_CLIENT_SIZE");}}prior_key=key;
 U f6=held(0x76);if(ui_debug&&!mods&&f6&&!prior_f6&&active&&client_ok){hi_enabled=!hi_enabled;hi_ready=0;message(hi_enabled?"FONT_HIRES_ON":"FONT_HIRES_OFF");}prior_f6=f6;
 U f10=(GetAsyncKeyState(0x79)&0x8000)!=0;if(ui_debug&&!mods&&f10&&!prior_f10&&active&&client_ok){tip_enabled=!tip_enabled;message(tip_enabled?"TOOLTIPS_ON":"TOOLTIPS_NATIVE");}prior_f10=f10;
 U f11=(GetAsyncKeyState(0x7a)&0x8000)!=0;if(ui_debug&&!mods&&f11&&!prior_f11&&active&&client_ok){expanded_enabled=!expanded_enabled;message(expanded_enabled?"EXPANDED_MAP_ON":"EXPANDED_MAP_NATIVE");}prior_f11=f11;
 U f12=(GetAsyncKeyState(0x7b)&0x8000)!=0;if(ui_debug&&!mods&&f12&&!prior_f12&&active&&client_ok){scale_index=(scale_index+1)%(screen_w==1920?3:4);sn=scale_num[scale_index];sd=scale_den[scale_index];save_scale();hi_ready=tip_scope=tip_valid=0;draw_seen=font_log_mask=map_log_mask=0;zero(seen,sizeof(seen));begin("SCALE_CHANGE");num("percent",100*sn/sd);emit();}prior_f12=f12;
 U two=held(0x76)&&held(0x12);if(ui_debug&&two&&!prior_2d&&active&&client_ok){group_on[1]=!group_on[1];message(group_on[1]?"UI_2D_ON":"UI_2D_NATIVE");}prior_2d=two;
 for(I g=2;g<=3;g++){U pressed=held(0x75+g);if(ui_debug&&!mods&&pressed&&!prior_group[g]&&active&&client_ok){group_on[g]=!group_on[g];begin("UI_GROUP");num("group",g);num("enabled",group_on[g]);emit();}prior_group[g]=pressed;}
 if(active&&frame%120==0)counters();return;}
 if(kind==31){gameplay_reset();bookmark_reset();if(config_loaded&&camera_bookmarks)message("BOOKMARK_RESET");return;}
 if(kind==29){research_sample(regs[2],regs[4]);return;}
 if(kind==30){gold_sample(p,word(sp),word(sp+4));return;}
 if(kind==35){research_live_tick(regs[2]);return;}
 if(kind==36){money_keyboard(sp+0x20);return;}
 if(kind==32){credit_sample(p,word(sp),word(sp+4),regs[2]);return;}
 if(kind==33){research_transition(1,p,word(sp+4),0);return;}
 if(kind==34){research_transition(0,regs[0],0,0);return;}
 if(kind==27){float dt=0;U camera=0;rd(sp+0x10,&camera,4);rd(sp+0x250,&dt,4);camera_tick(regs[2],camera,dt);return;}
 if(kind==28){camera_restore(regs[2]);return;}
 if(kind==2){if(thread&&GetCurrentThreadId()==thread){indicator_root=0;restore_geometry();root=0;menu_root=0;map_pass=2;}return;}
 if(kind==1&&thread&&thread==GetCurrentThreadId())indicator_root=(research_indicator||research_queue_enabled||research_cancel_enabled)&&word(p)==0x682218?p:0;
 if(!active||!client_ok||!thread||GetCurrentThreadId()!=thread)return;
 if(kind==1){root=p;root_vt=word(p);menu_root=0;if(root_vt==0x68295c&&changed(97,p,1)){I r[10];if(rd(p+0x40,r,40)){begin("PAUSE_ROOT");arr("geometry",(U*)r,10,0);emit();}}if(word(p)==0x67fe40&&live_layout(p))hud=p;
 I a;if((hud||(window_spec(root_vt)&&window_spec(root_vt)->group))&&menu_layout(p,sides[6],&a)){anchors[6][0]=a;anchors[6][1]=menu_y_anchor(root_vt);menu_root=p;if(root_vt==0x682b58)for(I i=0;i<4;i++)map_popup_r[i]=sides[6][i];if(changed(90,p,hashwords((U*)sides[6],4))){begin("MENU_ROOT");type(word(p));arr("native",(U*)sides[6],4,0);num("physical_x",base_axis(6,0));num("physical_y",base_axis(6,1));emit();}}else if(cls(root_vt)&&(cls(root_vt)->flags&2)&&changed(142,p,root_vt)){I a[6];if(rd(p+0x40,a,24)){begin("WINDOW_NATIVE");type(root_vt);arr("bounds_origin",(U*)a,6,0);emit();}}return;}

 if(kind==21){I r[4];U obj=regs[1];if(window_layout(obj,r)){const struct WindowSpec*s=window_spec(word(obj));if(s&&s->group==2){regs[4]=window_inv((I)regs[4],r,0,s->vt);regs[0]=window_inv((I)regs[0],r,1,s->vt);tab_inputs++;}}return;}
 if(kind==22){geometry_override(regs[2],0xe8,1,0);return;}
 if(kind==23){geometry_override(regs[1],0xe4,1,0);return;}
 if(kind==24){geometry_override(regs[1],0xb8,0,(I*)sp);return;}
 if(kind==25){geometry_override(regs[4],0x9c,0,0);return;}
 if(kind==26){geometry_override(regs[1],0xa4,0,0);return;}
 if(kind==10){I r[4],a;if(word(p)==0x68295c){I *args=(I*)(sp+4);if(screen_side(args[0],args[1])==3){args[0]=inv_x(args[0],3);args[1]=inv_y(args[1],3);pause_inputs++;if(changed(102,p,args[2])){begin("PAUSE_INPUT_MAP");arr("native_xy_flag",(U*)args,3,0);emit();}}return;}if(window_type(p))return;if(!menu_layout(p,r,&a))return;I *args=(I*)(sp+4),x=args[0],y=args[1];if(x<-10000||x>10000||y<-10000||y>10000)return;args[0]=menu_inverse(x,r[0],a);args[1]=menu_inverse(y,r[1],menu_y_anchor(word(p)));menu_inputs++;if(changed(91,p,args[2])){begin("MENU_INPUT");type(word(p));num("x",x);num("y",y);num("native_x",args[0]);num("native_y",args[1]);emit();}return;}
 if(kind==12){I r[4],a;if(!contain_menu||!menu_layout(contain_menu,r,&a))return;I *args=(I*)sp,x=args[1],y=args[2];if(x<-10000||x>10000||y<-10000||y>10000)return;args[1]=window_type(contain_menu)?window_inv(x,r,0,word(contain_menu)):menu_inverse(x,r[0],a);args[2]=window_type(contain_menu)?window_inv(y,r,1,word(contain_menu)):menu_inverse(y,r[1],menu_y_anchor(word(contain_menu)));if(args[1]>=r[0]&&args[1]<r[2]-1&&args[2]>=r[1]&&args[2]<r[3]-1){menu_owns++;if(window_spec(word(contain_menu))&&window_spec(word(contain_menu))->group==2)tab_owns++;if(changed(92,contain_menu,1)){begin("MENU_OWNERSHIP");type(word(contain_menu));num("x",x);num("y",y);emit();}}return;}
 if(kind==13){I r[4],a;U obj=regs[1];if(!menu_layout(obj,r,&a))return;I *xy=(I*)(sp+0x10);if(window_type(obj)){window_cursor_owner=obj;window_cursor_raw[0]=xy[0];window_cursor_raw[1]=xy[1];xy[0]=window_inv(xy[0],r,0,word(obj));xy[1]=window_inv(xy[1],r,1,word(obj));}else{xy[0]=menu_inverse(xy[0],r[0],a);xy[1]=menu_inverse(xy[1],r[1],menu_y_anchor(word(obj)));}return;}
 if(kind==14){I r[4];if(!window_layout(regs[1],r))return;/* EBX and stack keep raw coordinates for dragging; EDI/EBP are local button coordinates. */regs[0]=window_inv((I)regs[4],r,0,word(regs[1]))-r[0];regs[2]=window_inv(*(I*)(sp+0x18),r,1,word(regs[1]))-r[1];if(changed(99,regs[1],*(U*)(sp+0x1c))){begin("WINDOW_INPUT");type(word(regs[1]));arr("local",(U[]){regs[0],regs[2]},2,0);emit();}return;}
 if(kind==15){I r[4];if(!window_layout(regs[1],r))return;/* Restore the exact sample from this update, not a second cursor read. */if(window_cursor_owner==regs[1]){*(I*)(sp+0x10)=window_cursor_raw[0];*(I*)(sp+0x14)=window_cursor_raw[1];window_cursor_owner=0;}return;}
 if(kind==16){I r[4];if(!window_layout(p,r)||(*(U*)sp!=0x5f3864&&*(U*)sp!=0x5f479f))return;I *d=(I*)(sp+4);for(I i=0;i<2;i++){I max=(i?screen_h:screen_w)-edge(r[i+2]-r[i]);I q=window_base(r,i,word(p))+d[i];if(q<0)q=0;if(q>max)q=max;I nn=q+window_offset(r,i,word(p));I native_max=(i?screen_h:screen_w)-(r[i+2]-r[i]);if(nn>native_max)nn=native_max;d[i]=nn-r[i];}if(changed(100,p,hashwords((U*)d,2))){begin("WINDOW_MOVE");type(word(p));arr("delta",(U*)d,2,0);emit();}return;}
 if(kind==17||kind==18){/* Capture per-call identity; never change the original font object or its metrics. */
  hi_ready=hi_checked=hi_ok=tip_scope=tip_valid=hi_path=0;
  if(word(p)==0x688a7c){U caller=*(U*)sp,shift=word(p+0x1c);
   if(shift<=2&&(hud||menu_scope())){hi_path=kind;hi_shift=shift;hi_root=root;hi_frame=frame;
    if(tip_enabled&&kind==18&&!root&&caller==0x4277d8){tip_dx=(I)word(p+0x10);tip_dy=(I)word(p+0x14);if(tip_dx>=-3&&tip_dx<=3&&tip_dy>=-3&&tip_dy<=3){U mgr=word(0x6e7b88);B own=0;I xy[2];if(mgr&&rd(mgr+0x61,&own,1)&&own)tip_scope=1;else if(GetCursorPos(xy)&&ScreenToClient(GetForegroundWindow(),xy)&&screen_side(xy[0],xy[1])>=0)tip_scope=1;}}}
   if(font_paths<256&&changed(110+kind,hashwords((U[]){caller,root?root_vt:0,shift},3),1)){font_paths++;begin("FONT_PATH");num("path",kind);hx("caller",caller);hx("root_vt",root?root_vt:0);num("shift",shift);emit();}}return;}
 if(kind==7){if(root==hud&&hud&&regs[1]==hud)map_pass=regs[2];else map_pass=2;return;}
 if(kind==8){/* Only the four calls inside CMCHUD minimap renderer; never generic world quads. */
  U* a=(U*)sp;U ret=a[0],bit=ret==0x4a1a1c?1:ret==0x4a1a2f?2:ret==0x4a1c00?4:ret==0x4a1cdc?8:0;
  if(!bit||!hud||root!=hud||map_pass>1||(map_pass==1&&(!group_on[1]||!expanded_supported(hud))))return;
  if((map_pass==0&&!map_supported(hud))||a[3]!=4||a[4]!=0||!rd(a[1],map_vertices,112)){map_fallback++;if(!(map_log_mask&32)){map_log_mask|=32;begin("MAP_QUAD_NATIVE");hx("caller",ret);num("reason",1);num("count",a[3]);num("flag",a[4]);emit();}return;}
  for(I i=0;i<4;i++){float *v=(float*)(map_vertices+i*7);if(!(map_pass==0?(v[0]>=screen_w-360.f&&v[0]<=(float)screen_w&&v[1]>=screen_h-240.f&&v[1]<=(float)screen_h):(v[0]>=screen_w/2-300.f&&v[0]<=screen_w/2+300.f&&v[1]>=screen_h/2-300.f&&v[1]<=screen_h/2+300.f))){map_fallback++;if(!(map_log_mask&64)){map_log_mask|=64;begin("MAP_QUAD_NATIVE");hx("caller",ret);num("reason",2);arr("xy_bits",map_vertices+i*7,2,1);emit();}return;}v[0]=forward_map(v[0],map_pass?(float)(screen_w/2):(float)screen_w);v[1]=forward_map(v[1],map_pass?(float)(screen_h/2):(float)screen_h);if(map_pass){v[0]+=expanded_offset(0);v[1]+=expanded_offset(1);}}
  if(bit==4)marker_quads++;if(bit==4&&small_map_markers){float cx=0,cy=0;for(I i=0;i<4;i++){float*v=(float*)(map_vertices+i*7);cx+=v[0]*.25f;cy+=v[1]*.25f;}for(I i=0;i<4;i++){float*v=(float*)(map_vertices+i*7);v[0]=inverse_map(v[0],cx);v[1]=inverse_map(v[1],cy);}}a[1]=(U)map_vertices;map_quads++;if(map_pass)expanded_quads++;if(!(map_log_mask&bit)){map_log_mask|=bit;begin("MAP_QUAD");hx("caller",ret);arr("xy_bits",map_vertices,2,1);emit();}return;}
 if(kind==9){/* At each pass reload the original raw arguments, preserving alternate-map coordinates. */
  U p=regs[1];if(word(p)!=0x67fe40||!live_layout(p)||!map_supported(p))return;
  I x=*(I*)(sp+0xd8),y=*(I*)(sp+0xdc);float *xy=(float*)(sp+0x30);
  xy[0]=(float)x;xy[1]=(float)y;
  if(regs[0]==1&&group_on[1]&&expanded_supported(p)){expanded_inputs++;xy[0]=inverse_map((float)x-expanded_offset(0),(float)(screen_w/2));xy[1]=inverse_map((float)y-expanded_offset(1),(float)(screen_h/2));}if(regs[0]==0){xy[0]=inverse_map((float)x,(float)screen_w);xy[1]=inverse_map((float)y,(float)screen_h);if(screen_side(x,y)==5){map_input++;if(!(map_log_mask&16)){map_log_mask|=16;begin("MAP_NAV_INPUT");num("x",x);num("y",y);hx("native_x_bits",*(U*)xy);hx("native_y_bits",*((U*)xy+1));emit();}}}return;}
 if(kind==4){if(word(p)!=0x67fe40||!live_layout(p))return;I *args=(I*)(sp+4);I x=args[0],y=args[1],s=screen_side(x,y);if(s<0||(s==5&&!map_supported(p)))return;args[0]=inv_x(x,s);args[1]=inv_y(y,s);mapped++;if(changed(62,s,(U)args[2])){begin("HUD_INPUT_MAP");num("side",s);num("x",x);num("y",y);num("native_x",args[0]);num("native_y",args[1]);hx("flag",args[2]);emit();}return;}
 if(kind==5){/* EAX+ESI is the original RECT pointer; EDX/ECX are POINT x/y. */
 I r[4];if(!rd(regs[7]+regs[1],r,16))return;I s=side_rect(r),x=(I)regs[5],y=(I)regs[6];if(s<0||!live_layout(regs[2])||screen_side(x,y)!=s||(s==5&&!map_supported(regs[2])))return;regs[5]=(U)inv_x(x,s);regs[6]=(U)inv_y(y,s);claimed++;if(changed(63,s,1)){begin("OWNERSHIP_MAP");num("side",s);num("x",x);num("y",y);emit();}return;}
}
extern I __thiscall native_menu_contain(void*);
I __thiscall scale_menu_contain(void *p){U previous=contain_menu;I r[4],a;contain_menu=0;if(active&&client_ok&&GetCurrentThreadId()==thread&&menu_layout((U)p,r,&a))contain_menu=(U)p;I result=native_menu_contain(p);contain_menu=previous;return result;}
/* Area filter: 3 units per source pixel, 2 per physical pixel.
   Decode each contributing source against the actual destination background.
   Clamp the final partial footprint to the last source pixel. */
extern I __thiscall native_tab_contain(void*);
I __thiscall scale_tab_contain(void *p){U previous=contain_menu;I r[4];contain_menu=0;if(active&&client_ok&&GetCurrentThreadId()==thread&&window_layout((U)p,r))contain_menu=(U)p;I result=native_tab_contain(p);contain_menu=previous;return result;}
static I clamp(I v,I a,I b){return v<a?a:v>b?b:v;}
static I weight(I p,I q0,I q1){return q0==q1?0:sd*(p+1)-sn*(q0+1);}
static U blend4(U a,U b,U c,U d,I wx,I wy){U out=0;I i,den=sd*sd,sh=sd==4?4:sd==2?2:0;for(i=0;i<32;i+=8){U v=((a>>i)&255)*(sd-wx)*(sd-wy)+((b>>i)&255)*wx*(sd-wy)+((c>>i)&255)*(sd-wx)*wy+((d>>i)&255)*wx*wy;out|=((v+den/2)>>sh)<<i;}return out;}
/* Original entry trampoline; compiler emits native thiscall, 56-byte callee cleanup. */
extern I __thiscall native_surface(void*,U*,I,I,I,I,I,I,I,I,I,I,I,I,I);
I __thiscall scale_surface(void*self,U*dst,I sl,I st,I sr,I sb,I dl,I dt,I dr,I db,I width,I height,I pitch,I bpp,I flag){
 I s=-1,w=dr-dl+1,h=db-dt+1,x,y,k,l,t,stride,sh,result=1,ox,oy,basey,px,py,nx,ny,xx,yy;
 I pause_scope=root&&root_vt==0x68295c;
 if(!active||!client_ok||(!hud&&!menu_scope())||(root!=hud&&!menu_scope()&&!pause_scope)||busy||GetCurrentThreadId()!=thread)goto native;
 if(pause_scope){/* Exact measured source and destination; no global overlay scaling. */
  if(dl==0&&dt==screen_h-131&&dr==220&&db==screen_h-1&&sl==0&&st==0&&sr==220&&sb==130)s=3;
  else if(dl==screen_w/2-200&&dt==screen_h/2-49&&dr==screen_w/2+199&&db==screen_h/2+47&&sl==0&&st==0&&sr==399&&sb==96)s=7;
  else goto native;
 }else for(k=menu_scope()?6:0;k<(menu_scope()?7:6);k++)if(dl>=sides[k][0]&&dr<sides[k][2]&&dt>=sides[k][1]&&db<sides[k][3]){s=k;break;}
 if(s<0||(s==5&&!map_supported(hud))){if(menu_scope()){menu_skips++;const struct WindowSpec*ws=window_spec(root_vt);if(ws)new_rejects[ws->group]++;if(changed(95,root,hashwords((U[]){dl,dt,dr,db},4))){begin("MENU_DRAW_REJECT");type(word(root));arr("dest",(U[]){dl,dt,dr,db},4,0);arr("visual",(U*)sides[6],4,0);emit();}}goto native;}
 if(!dst||width!=screen_w||height!=screen_h||pitch!=(screen_w*4)||bpp!=32||flag||w<1||h<1||w>800||h>600||sl<0||st<0||sr>16383||sb>16383||sr-sl+1!=w||sb-st+1!=h){skipped++;goto native;}
 last_dst=(U)dst;last_dst_frame=frame;
 ox=dl-sides[s][0];oy=dt-sides[s][1];basey=base_axis(s,1);l=base_axis(s,0)+edge(ox);t=edge(oy);stride=(w+3)&~3;stride+=4;sh=h+2;if(stride>808||sh>604||l<0||t<0||l+edge(ox+w)-edge(ox)>width||basey+edge(oy+h)>height){skipped++;goto native;}
 if(mode==4){
 I base=base_axis(s,0), xr=edge(ox+w), yb=edge(oy+h), ix0,ix1,iy0,iy1,wx,wy,p,q,idx[4],j;
 busy=1;
 /* Precompute source indices, phase offsets and weights once per axis. Only
    decode phases actually consumed by nonzero output weights. */
 U xm=0,ym=0;I xl=edge(ox);
 for(px=xl;px<xr;px++){struct PhaseAxis*f=&phase_x[px-xl];f->q[0]=clamp(sd*px/sn,ox,ox+w-1);f->q[1]=clamp((sd*(px+1)-1)/sn,ox,ox+w-1);f->weight=weight(px,f->q[0],f->q[1]);for(j=0;j<2;j++){if(j&&f->weight==0)f->q[j]=f->q[0]+ox;f->phase[j]=px-sn*f->q[j]/sd;xm|=1u<<f->phase[j];f->q[j]-=ox;}}
 for(py=t;py<yb;py++){struct PhaseAxis*f=&phase_y[py-t];f->q[0]=clamp(sd*py/sn,oy,oy+h-1);f->q[1]=clamp((sd*(py+1)-1)/sn,oy,oy+h-1);f->weight=weight(py,f->q[0],f->q[1]);for(j=0;j<2;j++){if(j&&f->weight==0)f->q[j]=f->q[0]+oy;f->phase[j]=py-sn*f->q[j]/sd;ym|=1u<<f->phase[j];f->q[j]-=oy;}}
 for(k=0;k<3;k++){if(xm&(1u<<k))for(x=0;x<w;x++)seed_x[k][x]=base+clamp(sn*(ox+x)/sd+k,xl,xr-1);if(ym&(1u<<k))for(y=0;y<h;y++)seed_y[k][y]=(basey+clamp(sn*(oy+y)/sd+k,t,yb-1))*screen_w;}
 for(k=0;k<9;k++){if(!(xm&(1u<<(k%3)))||!(ym&(1u<<(k/3)))){art_saved++;continue;}U*buf=scratch[k];for(U z=0;z<(U)(stride*sh);z++)buf[z]=0;for(y=0;y<h;y++){U*src=dst+seed_y[k/3][y];U*out=buf+y*stride;for(x=0;x<w;x++)out[x]=src[seed_x[k%3][x]];}
 art_decodes++;result=native_surface(self,buf,sl,st,sr,sb,0,0,w-1,h-1,stride,sh,stride*4,32,0);if(!result){busy=0;skipped++;goto native;}}
 for(py=t;py<yb;py++){struct PhaseAxis*fy=&phase_y[py-t];I a0=fy->phase[0]*3,a1=fy->phase[1]*3,r0=fy->q[0]*stride,r1=fy->q[1]*stride;U*out=dst+(basey+py)*screen_w+base;
 for(px=xl;px<xr;px++){struct PhaseAxis*fx=&phase_x[px-xl];I q0=fx->q[0],q1=fx->q[1],p0=fx->phase[0],p1=fx->phase[1];U a=scratch[a0+p0][r0+q0];if(!fx->weight&&!fy->weight){out[px]=a;continue;}out[px]=blend4(a,scratch[a0+p1][r0+q1],scratch[a1+p0][r1+q0],scratch[a1+p1][r1+q1],fx->weight,fy->weight);}}

 }else{goto native;}
 busy=0;draws++;if(pause_scope){pause_draws++;if(changed(104,s,1)){begin("PAUSE_SCALED");num("side",s);num("physical_l",l);num("physical_t",basey+t);num("physical_w",edge(ox+w)-edge(ox));num("physical_h",edge(oy+h)-edge(oy));emit();}}if(s==6){menu_draws++;const struct WindowSpec*ws=window_spec(root_vt);if(ws)new_draws[ws->group]++;}if(!(draw_seen&(1u<<s))){draw_seen|=1u<<s;begin("FIRST_SCALED_DRAW");num("side",s);num("native_l",dl);num("native_t",dt);num("physical_l",l);num("physical_t",basey+t);num("w",w);num("h",h);emit();}if(indicator_scope)indicator_capture(dst,width,height,pitch,bpp,flag,l,basey+t,base_axis(s,0)+edge(ox+w),basey+edge(oy+h),edge(4));return result;
 native:result=native_surface(self,dst,sl,st,sr,sb,dl,dt,dr,db,width,height,pitch,bpp,flag);if(indicator_scope&&result)indicator_capture(dst,width,height,pitch,bpp,flag,dl,dt,dr+1,db+1,4);return result;
}
/* Native GDI calls are forwarded unchanged; only an independent, top-down
   32bpp DIB receives the optional second rasterization. All sizes are bounded. */
typedef struct {I height,width,escapement,orientation,weight;B italic,underline,strikeout,charset,outprecision,clipprecision,quality,pitch;char face[32];} LF;
typedef struct {U size;I width,height;unsigned short planes,bpp;U compression,sizeimage;I xppm,yppm;U used,important;} BI;
extern I API DrawTextA(H,const char*,I,I*,U);
extern H API CreateCompatibleDC(H);extern H API CreateDIBSection(H,const BI*,U,void**,H,U);
extern H API SelectObject(H,H);extern H API GetCurrentObject(H,U);extern I API GetObjectA(H,I,void*);
extern H API CreateFontIndirectA(const LF*);extern I API DeleteObject(H);extern I API DeleteDC(H);
extern I API SetBkMode(H,I);extern U API SetTextColor(H,U);extern I API GdiFlush(void);
extern I API GetTextExtentExPointA(H,const char*,I,I,I*,I*,I*);
extern I API ExtTextOutA(H,I,I,U,const I*,const char*,U,const I*);
#define HI_W 2048
#define HI_H 320
static H hi_dc,hi_bitmap,hi_old_bitmap,hi_old_font;
static U* hi_bits;
static struct {LF lf;H font;} hi_fonts[16];
static U hi_next;
static I samebytes(const void*a,const void*b,U n){const B*x=a,*y=b;while(n--)if(*x++!=*y++)return 0;return 1;}
#define FONT_CACHE_N 64
#define FONT_CACHE_PIXELS (256*1024)
static struct RasterCache {U valid,hash,flags,shift,path,len,validated,ok,building_arial;LF lf;I rc[4],sx,sy,w,h,ex,ey,fx,fy,bx,by;U positioned;I advances[256];char text[1024];B pixels[FONT_CACHE_PIXELS];B native[64000];} raster_cache[FONT_CACHE_N];
static struct RasterCache* current_raster;
static I hi_bias_x,hi_bias_y;
static I hi_building_arial;
static void copybytes(void*d,const void*s,U n){B*a=d;const B*b=s;while(n--)*a++=*b++;}
static U bytehash(U h,const void*p,U n){const B*b=p;while(n--)h=mix(h,*b++);return h;}
static U raster_hash(const LF*lf,const I*rc,const char*text,U len,U flags,U shift,U path){U h=bytehash(2166136261u,lf,sizeof(*lf));h=bytehash(h,rc,16);h=bytehash(h,text,len);return mix(mix(mix(h,flags),shift),path);}
static I raster_match(const struct RasterCache*c,U key,const LF*lf,const I*rc,const char*text,U len,U flags,U shift,U path){return c->valid&&c->hash==key&&c->flags==flags&&c->shift==shift&&c->path==path&&c->len==len&&samebytes(&c->lf,lf,sizeof(*lf))&&samebytes(c->rc,rc,16)&&samebytes(c->text,text,len);}
static I scale3(I v,I d){return v<0?-((-v*3+d/2)/d):(v*3+d/2)/d;}
static I native_advances(const char*text,I len,I*partial,I limit){I prev=0; if(len<1||len>256||limit<1)return 0;for(I i=0;i<len;i++){I v=partial[i];if((B)text[i]<32||(B)text[i]>126||v<prev||v>limit+8||v-prev>128)return 0;partial[i]=(v-prev)*3;prev=v;}return prev>0;}
static I hi_init(void){BI bi={40,HI_W,-HI_H,1,32,0,0,0,0,0,0};if(hi_dc)return hi_bits!=0&&hi_old_font!=0;
 hi_dc=CreateCompatibleDC(0);if(!hi_dc)return 0;hi_bitmap=CreateDIBSection(hi_dc,&bi,0,(void**)&hi_bits,0,0);
 if(!hi_bitmap||!hi_bits){if(hi_bitmap)DeleteObject(hi_bitmap);DeleteDC(hi_dc);hi_dc=0;hi_bitmap=0;hi_bits=0;return 0;}
 hi_old_bitmap=SelectObject(hi_dc,hi_bitmap);if(!hi_old_bitmap||hi_old_bitmap==BAD){DeleteObject(hi_bitmap);DeleteDC(hi_dc);hi_dc=0;hi_bitmap=0;hi_bits=0;return 0;}
 hi_old_font=GetCurrentObject(hi_dc,6);return hi_old_font!=0;
}
static I hi_select(LF*lf){U i;H h,prev;for(i=0;i<16;i++)if(hi_fonts[i].font&&samebytes(&hi_fonts[i].lf,lf,sizeof(*lf))){prev=SelectObject(hi_dc,hi_fonts[i].font);return prev&&prev!=BAD;}
 h=CreateFontIndirectA(lf);if(!h)return 0;i=hi_next++&15;
 prev=hi_old_font?SelectObject(hi_dc,hi_old_font):0;if(!prev||prev==BAD){DeleteObject(h);return 0;}
 if(hi_fonts[i].font)DeleteObject(hi_fonts[i].font);hi_fonts[i].font=h;hi_fonts[i].lf=*lf;
 prev=SelectObject(hi_dc,h);return prev&&prev!=BAD;
}
static void hi_note(U reason){U facehash=2166136261u;for(I i=0;i<31&&hi_face[i];i++)facehash=mix(facehash,(B)hi_face[i]);if(reason<32&&changed(145,hashwords((U[]){reason,hi_path,hi_shift,root?root_vt:0,facehash},5),1)){begin("FONT_HIRES_REASON");num("reason",reason);num("path",hi_path);num("shift",hi_shift);hx("root_vt",root?root_vt:0);txt(" face=");txt(hi_face);emit();}}
/* Invoked only at the two real DrawTextA paint sites (not measurement). */
I __stdcall hi_drawtext(H dc,const char*text,I len,I*rect,U flags){
 I rc[4],hr[4],i,ret,fac,w,h,dx[256],sz[2];U positioned=0; char copy[1025];LF lf;H old;U expected=(flags==0x820?17:flags==0x900?18:0);
 I eligible=active&&hi_enabled&&client_ok&&(hud||menu_scope())&&GetCurrentThreadId()==thread&&hi_path==expected&&expected&&hi_frame==frame&&hi_root==root&&(root==hud||menu_scope()||tip_scope);
 if(eligible&&rect)for(i=0;i<4;i++)rc[i]=rect[i];else eligible=0;
 /* Call the exact original import; the native return and any RECT changes are preserved. */
 ret=(*(I (__stdcall **)(H,const char*,I,I*,U))0x67f2b4)(dc,text,len,rect,flags);
 if(!eligible)return ret;
 hi_ready=hi_checked=hi_ok=0;current_raster=0;hi_bias_x=hi_bias_y=0;hi_face[0]=0;hi_building_arial=0;hi_attempts++;
 if(len==-1&&text){for(len=0;len<1024;len++){if(!rd((U)text+len,copy+len,1)){len=0;break;}if(!copy[len])break;}if(len==1024)len=0;}
 if(ret<=0||!text||len<=0||len>1024||hi_shift>2){hi_note(1);return ret;}
 fac=1<<hi_shift;
 for(i=0;i<4;i++){if(rc[i]<-64*fac||rc[i]>680*fac){hi_note(2);return ret;}hr[i]=scale3(rc[i],fac);}
 if(hr[0]<-12||hr[1]<-12||hr[0]>24||hr[1]>24||hr[2]<=hr[0]||hr[3]<=hr[1]||hr[2]>HI_W-48||hr[3]>HI_H-48){hi_note(2);return ret;}
 zero(&lf,sizeof(lf));old=GetCurrentObject(dc,6);if(!old||GetObjectA(old,sizeof(lf),&lf)!=sizeof(lf)||!lf.height||lf.height<-256||lf.height>256||lf.width<0||lf.width>256||lf.escapement||lf.orientation){hi_note(3);return ret;}
 for(i=0;i<31;i++)hi_face[i]=lf.face[i];hi_face[31]=0;
 /* DrawText hinting changes cumulative advances at 3x. For the known Arial
    single-line building-statistics path, preserve native per-character spacing. */
 hi_building_arial=(root_vt==0x682218||root_vt==0x681f98)&&hi_path==17&&hi_shift==0&&flags==0x820&&samebytes(lf.face,"Arial",6);
 if(hi_building_arial&&len<=256&&rd((U)text,copy,len)&&GetTextExtentExPointA(dc,text,len,0x7fffffff,0,dx,sz)&&native_advances(copy,len,dx,rc[2]-rc[0]+(rc[2]-rc[0])/8))positioned=1;
 lf.height=scale3(lf.height,fac);lf.width=scale3(lf.width,fac);lf.quality=4; /* grayscale antialias, not ClearType */
 struct RasterCache*c=0;U ck=0;I cw=hr[2]+40,ch=hr[3]+40;
 if(cache_enabled&&cw>0&&ch>0&&cw<=HI_W&&ch<=HI_H&&(U)(cw*ch)<=FONT_CACHE_PIXELS&&rd((U)text,copy,len)){
  ck=raster_hash(&lf,rc,copy,len,flags,hi_shift,hi_path);if(hi_building_arial)ck=mix(ck,0x20a11);if(positioned)ck=bytehash(mix(ck,1),dx,len*sizeof(I));c=&raster_cache[ck&(FONT_CACHE_N-1)];
  if(raster_match(c,ck,&lf,rc,copy,len,flags,hi_shift,hi_path)&&c->building_arial==hi_building_arial&&c->positioned==positioned&&(!positioned||samebytes(c->advances,dx,len*sizeof(I)))){current_raster=c;hi_pad=16;hi_scan_x=c->sx-16;hi_scan_y=c->sy-16;hi_maskobj=word(0x6f1420);hi_ready=hi_maskobj!=0;hi_key=0;hi_fit_x=hi_fit_y=0;cache_hits++;return ret;}cache_misses++;
 }else cache_bypass++;
 if(!hi_init()||!hi_select(&lf)){hi_note(4);return ret;}
 /* Full DIB clear avoids stale pixels even if DT_NOCLIP draws outside its rectangle. */
 if(!GdiFlush()){hi_note(5);return ret;}for(U q=0;q<HI_W*HI_H;q++)hi_bits[q]=0;SetBkMode(hi_dc,1);SetTextColor(hi_dc,0xffffff);
 w=hr[2]-hr[0];h=hr[3]-hr[1];
 /* Preserve the logical rectangle and alignment, but allow glyph overhang and
    different hinting advances to be captured instead of silently clipped. */
 hi_pad=16;hi_scan_x=hr[2]+24;hi_scan_y=hr[3]+24;hi_fit_x=hi_fit_y=0;
 for(i=0;i<4;i++)hr[i]+=hi_pad;
 if(!(positioned?ExtTextOutA(hi_dc,hr[0],hr[1],0,0,text,len,dx):DrawTextA(hi_dc,text,len,hr,flags|0x100)>0)||!GdiFlush()){hi_note(5);return ret;}
 if(c){c->valid=0;c->validated=0;c->hash=ck;c->flags=flags;c->shift=hi_shift;c->path=hi_path;c->len=len;c->lf=lf;copybytes(c->rc,rc,16);copybytes(c->text,copy,len);c->sx=cw;c->sy=ch;c->positioned=positioned;c->building_arial=hi_building_arial;if(positioned)copybytes(c->advances,dx,len*sizeof(I));
  for(I y=0;y<ch;y++)for(I x=0;x<cw;x++){U v=hi_bits[y*HI_W+x];c->pixels[y*cw+x]=(B)(((v&255)+((v>>8)&255)+((v>>16)&255)+1)/3);}c->valid=1;current_raster=c;}
 hi_maskobj=word(0x6f1420);hi_ready=hi_maskobj!=0;hi_key=0;
 if(hi_ready&&hi_attempts<=8){begin("FONT_HIRES_RASTER");num("path",hi_path);num("shift",hi_shift);num("font_height",lf.height);arr("native_gdi_rect",(U*)rc,4,0);arr("new_gdi_rect",(U*)hr,4,0);num("w",w);num("h",h);emit();}return ret;
}
static U hi_raw(I x,I y){I xx=x+hi_pad+hi_bias_x,yy=y+hi_pad+hi_bias_y;U v;if(xx<0||yy<0||xx>=HI_W||yy>=HI_H)return 0;if(current_raster)return xx<current_raster->sx&&yy<current_raster->sy?current_raster->pixels[yy*current_raster->sx+xx]:0;v=hi_bits[yy*HI_W+xx];return ((v&255)+((v>>8)&255)+((v>>16)&255)+1)/3;}
/* Fit only excess advance/height into the original logical footprint.
   Endpoint-preserving fixed-point interpolation keeps normal rasters unmodified. */
static U hi_cov(I x,I y){I fx=x*256,fy=y*256,xx,yy;U a,b;
 if((hi_fit_x&&x>=hi_fit_x)||(hi_fit_y&&y>=hi_fit_y))return 0;if(!hi_fit_x&&!hi_fit_y)return hi_raw(x,y);
 if(hi_fit_x&&x>=0)fx=x*(hi_extent_x-1)*256/(hi_fit_x-1);
 if(hi_fit_y&&y>=0)fy=y*(hi_extent_y-1)*256/(hi_fit_y-1);
 xx=fx/256;yy=fy/256;fx%=256;fy%=256;
 if(fx<0||fy<0)return hi_raw(x,y);
 a=hi_raw(xx,yy)*(256-fx)+hi_raw(xx+1,yy)*fx;
 b=hi_raw(xx,yy+1)*(256-fx)+hi_raw(xx+1,yy+1)*fx;
 return (a*(256-fy)+b*fy+32768)/65536;
}
static I hi_fit(I w,I h){I x,y,right=-1,bottom=-1,left=HI_W,top=HI_H;I ex=hi_scan_x,ey=hi_scan_y;
 hi_fit_x=hi_fit_y=hi_bias_x=hi_bias_y=0;
 if(!hi_pad)return 1;
 if(ex<1||ey<1||ex+hi_pad>HI_W||ey+hi_pad>HI_H)return 0;
 for(y=-hi_pad;y<ey;y++)for(x=-hi_pad;x<ex;x++)if(hi_raw(x,y)>8){if(x>right)right=x;if(y>bottom)bottom=y;if(x<left)left=x;if(y<top)top=y;}
 if(right<0||bottom<0||left< -6||top< -6||right>=ex-1||bottom>=ey-1)return 0;
 /* Small italic bearings may be negative. Shift the coverage origin, not the
    native layout; the native-mask comparison still rejects bad alignment. */
 if(left<0){hi_bias_x=left;right-=left;}if(top<0){hi_bias_y=top;bottom-=top;}
 /* A small bounded compression is preferable to losing the last glyph.
    Reject large differences rather than squash or reflow text. */
 hi_extent_x=right+1;hi_extent_y=bottom+1;
 if(right>=3*w){if(right+1>3*w+3*w/8+3)return 0;hi_fit_x=3*w;}
 if(bottom>=3*h){if(bottom+1>3*h+9)return 0;hi_fit_y=3*h;}
 return 1;
}
/* Compare the candidate ink footprint to the game's native 8-bit mask. This
   rejects a shifted/reflowed, blank, clipped or otherwise mismatched raster. */
static U hi_metrics[17],building_reject_logs;
static I hi_compare(const B*m,I w,I h){I x,y,i,j;U sa=0,sb=0,overlap=0,near=0,spill=0,rows=0,ca[100],cb[100],cols=0;I al=w,at=h,ar=-1,ab=-1,bl=w,bt=h,br=-1,bb=-1;
 hi_metrics[0]=1;if(w<1||w>800||h<1||h>80||w*3>HI_W-4||h*3>HI_H-4)return 0;
 for(x=0;x<100;x++){((volatile U*)ca)[x]=0;((volatile U*)cb)[x]=0;}
 for(y=0;y<h;y++){U ra=0,rb=0;for(x=0;x<w;x++){U a=m[y*w+x],b=0;for(j=0;j<3;j++)for(i=0;i<3;i++)b+=hi_cov(3*x+i,3*y+j);b=(b+4)/9;sa+=a;sb+=b;ra+=a;rb+=b;ca[x/8]+=a;cb[x/8]+=b;overlap+=a<b?a:b;
  if(a>24){if(x<al)al=x;if(x>ar)ar=x;if(y<at)at=y;if(y>ab)ab=y;}
  if(b>24){if(x<bl)bl=x;if(x>br)br=x;if(y<bt)bt=y;if(y>bb)bb=y;U mx=0;for(j=-1;j<=1;j++)for(i=-1;i<=1;i++){I xx=x+i,yy=y+j;if(xx>=0&&xx<w&&yy>=0&&yy<h&&m[yy*w+xx]>mx)mx=m[yy*w+xx];}near+=b<mx?b:mx;} }rows+=ra<rb?ra:rb;}
 for(x=0;x<(w+7)/8;x++)cols+=ca[x]<cb[x]?ca[x]:cb[x];
 for(y=0;y<h*3+3;y++)for(x=0;x<3;x++)spill+=hi_cov(w*3+x,y);
 for(y=0;y<3;y++)for(x=0;x<w*3;x++)spill+=hi_cov(x,h*3+y);
 hi_metrics[0]=2;hi_metrics[1]=sa;hi_metrics[2]=sb;hi_metrics[3]=overlap;hi_metrics[4]=near;hi_metrics[5]=spill;hi_metrics[6]=rows;hi_metrics[7]=cols;hi_metrics[8]=al;hi_metrics[9]=at;hi_metrics[10]=ar;hi_metrics[11]=ab;hi_metrics[12]=bl;hi_metrics[13]=bt;hi_metrics[14]=br;hi_metrics[15]=bb;hi_metrics[16]=0;
 if(sa<32||sb<32||sb*3<sa||sa*3<sb||spill>sb*2||ar<0||br<0)return 0;hi_metrics[0]=3;
 if(bl<al-2||bl>al+2||bt<at-2||bt>at+2||br<ar-2-w/50||br>ar+2+w/50||bb<ab-2||bb>ab+2)return 0;
 hi_metrics[0]=4;if(overlap*7>=sa+sb&&near*5>=sb*3)return 1;
 /* Second chance for hinting/advance differences: require strong row and
    coarse-column correspondence, bounded ink mass and nearby native ink. */
 return sb*2>=sa&&sa*2>=sb&&overlap*10>=sa+sb&&near*2>=sb&&rows*5>= (sa+sb)*2&&cols*5>= (sa+sb)*2;
}
/* Correct cumulative width mismatch only after an otherwise valid raster
   fails the native-bound comparison. Keep the original mask validator. */
static I hi_retry_coverage(I w,I h);
static I hi_retry_width(const B*m,I w,I h){
 I ar=(I)hi_metrics[10],br=(I)hi_metrics[14],al=(I)hi_metrics[8],bl=(I)hi_metrics[12],at=(I)hi_metrics[9],bt=(I)hi_metrics[13],ab=(I)hi_metrics[11],bb=(I)hi_metrics[15];
 I target=3*(ar+1),old=hi_fit_x;
 if(hi_metrics[0]!=3||w<40||h<8||h>32||old||hi_extent_x<2||ar>=w||br<0||ar-br<3||ar-br>w/8||al<0||al>3||bl<0||bl>3||bt<at-2||bt>at+2||bb<ab-2||bb>ab+2||target>3*w||target<=hi_extent_x||target-hi_extent_x>hi_extent_x/7)return 0;
 hi_fit_x=target;
 if(hi_compare(m,w,h)||hi_retry_coverage(w,h))return 1;
 hi_fit_x=old;hi_compare(m,w,h);return 0;
}
/* A bounded last chance for the known Arial single-line building path.
   Bounds already passed; require strong nearby ink, row and column agreement.
   No additional geometric transformation or change to other fonts. */
static I hi_retry_coverage(I w,I h){
 U a=hi_metrics[1],b=hi_metrics[2],lo=a<b?a:b,high=a>b?a:b;
 I al=(I)hi_metrics[8],at=(I)hi_metrics[9],ar=(I)hi_metrics[10],ab=(I)hi_metrics[11],bl=(I)hi_metrics[12],bt=(I)hi_metrics[13],br=(I)hi_metrics[14],bb=(I)hi_metrics[15];
 if(hi_metrics[0]!=4||w<40||h<8||h>24||lo<256||high>lo+lo/5||hi_metrics[5]>lo/20)return 0;
 if(al<0||bl<0||ar>=w||br>=w||at<0||bt<0||ab>=h||bb>=h||ar<al||br<bl||ab<at||bb<bt)return 0;
 if(bl<al-1||bl>al+1||br<ar-1||br>ar+1||bt<at-1||bt>at+1||bb<ab-1||bb>ab+1)return 0;
 return hi_metrics[3]*4>=lo&&hi_metrics[4]*100>=b*85&&hi_metrics[6]*4>=lo*3&&hi_metrics[7]*100>=lo*85;
}
static U hi_sample(I x,I y,I w,I h){I x0=clamp(x,0,3*w-1),x1=clamp(x+1,0,3*w-1),y0=clamp(y,0,3*h-1),y1=clamp(y+1,0,3*h-1);return (hi_cov(x0,y0)+hi_cov(x1,y0)+hi_cov(x0,y1)+hi_cov(x1,y1)+2)/4;}
/* Scale coverage at the final compositor; original cleanup remains native. */
/* Integrate the 3x glyph coverage over an output-pixel footprint. */
static U hi_scaled_sample(I px,I py,I ox,I oy,I w,I h){
 if(sn==3&&sd==2)return hi_sample(2*px-3*ox,2*py-3*oy,w,h);
 I lx=3*(sd*px-sn*ox),ly=3*(sd*py-sn*oy),rx=lx+3*sd,by=ly+3*sd,den=9*sd*sd;U sum=0;
 for(I y=ly/sn;y<=(by-1)/sn;y++){I wy=clamp(by-y*sn,0,sn)-clamp(ly-y*sn,0,sn);for(I x=lx/sn;x<=(rx-1)/sn;x++){I wx=clamp(rx-x*sn,0,sn)-clamp(lx-x*sn,0,sn);sum+=hi_cov(clamp(x,0,3*w-1),clamp(y,0,3*h-1))*wx*wy;}}
 return (sum+den/2)/den;
}
extern void __cdecl native_font(U maskobj,U maskaux,U* surf,I l,I t,I r,I b,U color);
void __cdecl scale_font(U maskobj,U maskaux,U* surf,I l,I t,I r,I b,U color){
 I s=-1,k,x,y,x0,x1,y0,y1,ox,oy,wx,wy,px,py,base,top,stride,w,h,use_hi=0,is_tip=0; U caller=(U)__builtin_return_address(0); U a,c,d,v,shift; B*mask; U*dst; U info[10]={0},si[5]={0},reason=1;
 is_tip=active&&client_ok&&hud&&!root&&tip_scope&&hi_frame==frame&&hi_root==0&&hi_path==18&&GetCurrentThreadId()==thread&&(caller==0x5f8c0a||caller==0x5f8c61);
 if(active&&client_ok&&hud&&GetCurrentThreadId()==thread&&unscaled_fonts<256&&!is_tip&&(root!=hud&&!menu_scope())){if(changed(120,hashwords((U[]){caller,root?root_vt:0,(U)color},3),1)){unscaled_fonts++;begin("FONT_UNSCOPED");hx("caller",caller);hx("root_vt",root?root_vt:0);arr("rect",(U[]){l,t,r,b},4,0);hx("color",color);emit();}}
 if(!active||!client_ok||(!hud&&!menu_scope())||(root!=hud&&!menu_scope()&&!is_tip)||busy||GetCurrentThreadId()!=thread)goto native;
 for(k=menu_scope()?6:0;k<(menu_scope()?7:6);k++)if(l>=sides[k][0]&&r<sides[k][2]&&t>=sides[k][1]&&b<sides[k][3]){s=k;break;}
 if(is_tip)s=8;reason=2;if(s<0||(s==5&&!map_supported(hud))||r<l||b<t)goto native;
 reason=3;if(!maskobj||maskobj!=word(0x6f1420))goto native;
 reason=4;if(!rd(maskobj,info,40)||!rd((U)surf,si,20))goto native;
 w=r-l+1;h=b-t+1;stride=(I)info[4];mask=(B*)info[9];dst=(U*)si[0];
 reason=5;if(!mask||!((stride==screen_w*4&&info[5]==screen_h*4)||(screen_w!=2560&&stride==10240&&info[5]==5760))||w<1||w>800||h<1||h>80||w>stride)goto native;
 /* Native descriptor can describe a different logical extent. Only use a buffer independently verified by the sprite path in this frame. */
 reason=6;if(!dst||si[3]!=(screen_w*4)||(si[4]!=32&&si[4]!=24)||si[1]<1||si[2]<1)goto native;
 reason=7;if((si[1]!=screen_w||si[2]!=screen_h)&&((U)dst!=last_dst||last_dst_frame!=frame))goto native;
 if(r>= (I)si[1]||b>= (I)si[2])goto native;
 if(is_tip){
  I sh=caller==0x5f8c0a,fl=l-(sh?tip_dx:0),ft=t-(sh?tip_dy:0);
  if(!tip_valid){tip_l=fl+(tip_dx<0?tip_dx:0);tip_t=ft+(tip_dy<0?tip_dy:0);tip_w=w+(tip_dx<0?-tip_dx:tip_dx);tip_h=h+(tip_dy<0?-tip_dy:tip_dy);
   tip_x=clamp(tip_l,0,screen_w-edge(tip_w));tip_y=clamp(tip_t-(edge(tip_h)-tip_h),0,screen_h-edge(tip_h));tip_valid=1;}
  ox=l-tip_l;oy=t-tip_t;base=tip_x;top=tip_y;
  if(ox<0||oy<0||ox+w>tip_w||oy+h>tip_h)goto native;
 }else{ox=l-sides[s][0];oy=t-sides[s][1];base=base_axis(s,0);top=base_axis(s,1);}
 reason=8;if(base+edge(ox)<0||top+edge(oy)<0||base+edge(ox+w)>screen_w||top+edge(oy+h)>screen_h)goto native;
 for(y=0;y<h;y++)if(!rd((U)mask+y*stride,font_copy+y*w,w))goto native;
 mask=font_copy; stride=w;
 if(hi_enabled){
  U key=hashwords((U[]){(U)w,(U)h,maskobj,hi_path},4);
  I known=(hi_path==17&&(caller==0x5f80ad||caller==0x5f80ed))||(hi_path==18&&(caller==0x5f8c0a||caller==0x5f8c61));
  if(hi_ready&&hi_maskobj==maskobj&&hi_frame==frame&&hi_root==root&&known){
   if(!hi_checked||hi_key!=key){struct RasterCache*c=current_raster;
    if(c&&c->validated&&c->w==w&&c->h==h&&samebytes(c->native,mask,w*h)){hi_ok=c->ok;hi_extent_x=c->ex;hi_extent_y=c->ey;hi_fit_x=c->fx;hi_fit_y=c->fy;hi_bias_x=c->bx;hi_bias_y=c->by;if(hi_ok)cache_valid_hits++;else cache_rejected_hits++;}
    else{I fit=hi_fit(w,h);hi_ok=fit&&hi_compare(mask,w,h);if(fit&&!hi_ok&&hi_building_arial&&hi_retry_width(mask,w,h)){if(changed(147,w,h)){begin("FONT_HIRES_WIDTH_RECOVERED");hx("root_vt",root_vt);num("w",w);num("h",h);num("source_width",hi_extent_x);num("target_width",hi_fit_x);emit();}hi_ok=1;}if(fit&&!hi_ok&&hi_building_arial&&hi_retry_coverage(w,h)){hi_ok=1;if(changed(148,w,h)){begin("FONT_HIRES_COVERAGE_RECOVERED");hx("root_vt",root_vt);num("w",w);num("h",h);arr("metrics",hi_metrics,16,0);emit();}}if(!fit)hi_note(8);if(!hi_ok&&(root_vt==0x682218||root_vt==0x681f98)&&building_reject_logs<128&&changed(146,hashwords((U[]){w,h,c?c->hash:0,fit},4),fit?hashwords(hi_metrics,16):0)){building_reject_logs++;begin("FONT_HIRES_TEXT_REJECT");hx("root_vt",root_vt);num("w",w);num("h",h);num("fit_ok",fit);num("positioned",c?c->positioned:0);if(fit)arr("metrics",hi_metrics,16,0);emit();}if(c){c->validated=1;c->w=w;c->h=h;c->ok=hi_ok;c->ex=hi_extent_x;c->ey=hi_extent_y;c->fx=hi_fit_x;c->fy=hi_fit_y;c->bx=hi_bias_x;c->by=hi_bias_y;copybytes(c->native,mask,w*h);}}
    hi_checked=1;hi_key=key;}
   if(hi_ok){use_hi=1;hi_draws++;if(hi_fit_x||hi_fit_y){hi_fit_draws++;if(hi_fit_draws<=12){begin("FONT_HIRES_FIT");num("w",w);num("h",h);num("ink_x",hi_extent_x);num("ink_y",hi_extent_y);num("fit_x",hi_fit_x);num("fit_y",hi_fit_y);emit();}}if(hi_draws<=8){begin("FONT_HIRES_USED");num("side",s);num("w",w);num("h",h);num("shift",hi_shift);emit();}}
   else if(hi_fit_x||hi_fit_y||hi_checked)hi_note(6);
  }else hi_note(7);
  if(!use_hi)hi_fallbacks++;
 }
 for(py=edge(oy);py<edge(oy+h);py++){
 y0=clamp(sd*py/sn-oy,0,h-1);y1=clamp((sd*(py+1)-1)/sn-oy,0,h-1);wy=weight(py,y0+oy,y1+oy);
 for(px=edge(ox);px<edge(ox+w);px++){
 x0=clamp(sd*px/sn-ox,0,w-1);x1=clamp((sd*(px+1)-1)/sn-ox,0,w-1);wx=weight(px,x0+ox,x1+ox);
 if(use_hi)a=hi_scaled_sample(px,py,ox,oy,w,h);else a=(mask[y0*stride+x0]*(sd-wx)*(sd-wy)+mask[y0*stride+x1]*wx*(sd-wy)+mask[y1*stride+x0]*(sd-wx)*wy+mask[y1*stride+x1]*wx*wy+sd*sd/2)/(sd*sd);
 if(!a)continue;U*dp=dst+(top+py)*screen_w+base+px;d=*dp;v=d&0xff000000;
 for(shift=0;shift<24;shift+=8){c=(color>>shift)&255;k=(d>>shift)&255;v|=((c*a+k*(255-a)+127)/255)<<shift;}*dp=v;
 }}
 font_draws++;if(s==6)menu_fonts++;if(is_tip){tip_draws++;if(tip_draws<=8){begin("TOOLTIP_SCALED");arr("native",(U[]){l,t,r,b},4,0);arr("physical",(U[]){base+edge(ox),top+edge(oy),base+edge(ox+w),top+edge(oy+h)},4,0);num("hi",use_hi);emit();}}if(!(font_log_mask&(1u<<s))){font_log_mask|=1u<<s;begin("FONT_SCALED");num("side",s);num("l",l);num("t",t);num("r",r);num("b",b);num("stride",info[4]);hx("color",color);emit();}
 /* Empty height skips all native drawing but still releases its by-value smart reference. */
 native_font(maskobj,maskaux,surf,l,t,r,-1,color);return;
 native:if(is_tip){tip_fallbacks++;if(tip_fallbacks<=8){begin("TOOLTIP_NATIVE");num("reason",reason);arr("rect",(U[]){l,t,r,b},4,0);emit();}}if(active&&client_ok&&(hud||menu_scope())&&(root==hud||menu_scope())&&GetCurrentThreadId()==thread){font_fallback++;if(changed(66,(U)l,hashwords((U[]){(U)l,(U)t,(U)r,(U)b,reason,info[4],si[1],si[2],si[3],si[4]},10))){begin("FONT_NATIVE");num("l",l);num("t",t);num("r",r);num("b",b);num("reason",reason);hx("maskobj",maskobj);hx("global_mask",word(0x6f1420));hx("surf",(U)surf);arr("mask_fields",info,10,1);arr("surface_fields",si,5,1);hx("last_dst",last_dst);emit();}}native_font(maskobj,maskaux,surf,l,t,r,b,color);
}
static I install(void){U i,j,tmp;B b[12];if((U)GetModuleHandleA(0)!=0x400000){message("REFUSED_IMAGE_BASE");return 0;}for(i=0;i<NH;i++){struct Hook*h=&hooks[i];if(!rd(h->at,b,h->n)){message("REFUSED_READ");return 0;}for(j=0;j<h->n;j++)if(b[j]!=h->bytes[j]){begin("REFUSED_SIGNATURE");hx("address",h->at);num("byte",j);emit();return 0;}back[i]=h->at+h->n;}
 for(i=0;i<NH;i++)if(!VirtualProtect((H)hooks[i].at,hooks[i].n,0x40,&hooks[i].protection)){while(i){i--;VirtualProtect((H)hooks[i].at,hooks[i].n,hooks[i].protection,&tmp);}message("REFUSED_PROTECT");return 0;}
 for(i=0;i<NH;i++){struct Hook*h=&hooks[i];B*d=(B*)h->at;U delta=(U)h->fn-h->at-5;d[0]=0xe9;for(j=0;j<4;j++)d[j+1]=(B)(delta>>(j*8));for(j=5;j<h->n;j++)d[j]=0x90;FlushInstructionCache(process,d,h->n);}for(i=NH;i>0;i--){struct Hook*h=&hooks[i-1];VirtualProtect((H)h->at,h->n,h->protection,&tmp);}message("READY_F5_OFF");return 1;}
I __stdcall DllMain(H module,U why,H reserved){(void)reserved;if(why==1){DisableThreadLibraryCalls(module);process=GetCurrentProcess();now=GetTickCount();openlog();if(logfile!=BAD)install();}else if(why==0&&logfile!=BAD){if(active)counters();message("PROCESS_EXIT");CloseHandle(logfile);}return 1;}

int _fltused=0;
