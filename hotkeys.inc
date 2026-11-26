/* Bounded, case-insensitive virtual-key lists. No modifier chords. */
typedef struct {U n,k[4];} Binding;
static I eqkey(const char*a,const char*b){while(*a&&*b){I x=*a++,y=*b++;if(x>='a'&&x<='z')x-=32;if(y>='a'&&y<='z')y-=32;if(x!=y)return 0;}return !*a&&!*b;}
static U keycode(const char*s){if(s[0]&&!s[1]){U k=s[0];if(k>='a'&&k<='z')k-=32;if((k>='A'&&k<='Z')||(k>='0'&&k<='9'))return k;}if(s[0]=='F'||s[0]=='f'){U n=0,i=1;while(s[i]>='0'&&s[i]<='9'&&i<3)n=n*10+s[i++]-'0';if(!s[i]&&n>=1&&n<=24)return 0x6f+n;}static const struct{const char*s;U k;} t[]={{"Space",32},{"Escape",27},{"Up",38},{"Down",40},{"Left",37},{"Right",39},{"NumPadPlus",107},{"NumPadMinus",109},{"PageUp",33},{"PageDown",34},{"Home",36},{"End",35},{"Insert",45},{"Delete",46}};for(U i=0;i<sizeof(t)/sizeof(t[0]);i++)if(eqkey(s,t[i].s))return t[i].k;return 0;}
static I contains_key(const Binding*b,U k){for(U i=0;i<b->n;i++)if(b->k[i]==k)return 1;return 0;}
static I parse_keys(const char*s,Binding*b){Binding r={0,{0}};if(eqkey(s,"None")){*b=r;return 1;}while(*s){char t[32];U n=0,k;while(*s==' '||*s=='\t')s++;while(*s&&*s!=','){if(n>=31)return 0;t[n++]=*s++;}while(n&&(t[n-1]==' '||t[n-1]=='\t'))n--;t[n]=0;k=keycode(t);if(!k||r.n==4||contains_key(&r,k))return 0;r.k[r.n++]=k;if(*s==','){s++;if(!*s)return 0;}}if(!r.n)return 0;*b=r;return 1;}
static const struct {const char*name,*def;U id;I ch;U offset;} actions[]={
{"RotateLeft","A,Q",0xd,'a',0x78},{"RotateRight","D,E",0xe,'d',0x74},{"TiltDown","W",0x10,'w',0x7c},{"TiltUp","S",0xf,'s',0x80},{"ZoomIn","NumPadPlus",0xb,-1,0x6c},{"ZoomOut","NumPadMinus",0xc,-1,0x70},{"Home","H",0x24,'h',0x84},{"MessageHistory","B",0x21,'b',0},{"ExpandedMap","M",0x2d,'m',0},{"Pause","Space",0xa,32,0},{"Options","Escape",9,27,0},{"SpeedNormal","Z",0x12,'z',0},{"SpeedFast","X",0x13,'x',0},{"SpeedFastest","C",0x14,'c',0},
{"PanUp","Up",0,-1,0},{"PanDown","Down",0,-1,0},{"PanLeft","Left",0,-1,0},{"PanRight","Right",0,-1,0}};
#define HUD_ACTIONS 14
static const U pan_vk[]={38,40,37,39};
static I pan_native(U k){for(U i=0;i<4;i++)if(k==pan_vk[i])return i;return -1;}
static I pan_key(const Binding*b,U k){for(U i=0;i<4;i++)if(contains_key(b+HUD_ACTIONS+i,k))return i;return -1;}
static I camera_key(const Binding*b,U k){for(U i=0;i<6;i++)if(contains_key(b+i,k))return 1;return 0;}
static float pan_angle(I x,I y){if(y>0)return x>0?45.0f:x<0?135.0f:90.0f;if(y<0)return x>0?315.0f:x<0?225.0f:270.0f;return x<0?180.0f:0.0f;}
#define ACTIONS (sizeof(actions)/sizeof(actions[0]))
static I valid_game_keys(const Binding*b,const Binding*m){for(U i=0;i<ACTIONS;i++)for(U j=0;j<b[i].n;j++){U k=b[i].k[j];if((k>='1'&&k<='6')||(k>=0x73&&k<=0x7b)||contains_key(m,k)||contains_key(m+1,k))return 0;for(U z=0;z<i;z++)if(contains_key(b+z,k))return 0;}return 1;}
static I valid_mod_keys(const Binding*m){for(U i=0;i<2;i++){if(!m[i].n)return 0;for(U j=0;j<m[i].n;j++){U k=m[i].k[j];if(k<0x70||k>0x87||(k>=0x73&&k<=0x7b&&k!=0x74&&k!=0x75)||(i&&contains_key(m,k)))return 0;}}return 1;}
static I mapped_char(U i,const Binding*b,U raw,I ch){if(contains_key(b,raw))return actions[i].ch;I c=actions[i].ch;if(c!=-1&&(ch==c||(c>32&&((ch|32)==(c|32)))))return 0;return ch;}
