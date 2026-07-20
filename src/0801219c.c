typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
s32 Func_0801219c(void *arg0){ s32 x,y,rx,ry; void *tbl; u8 *base;
 x=M2C_FIELD(arg0,s32,0)/65536; y=(M2C_FIELD(arg0,s32,8)-M2C_FIELD(arg0,s32,4))/65536;
 tbl=*(void**)0x03001E70; if(tbl==0)return 0; base=M2C_FIELD(tbl,u8*,0x190);
 rx=x/16; ry=y/16; base+=(rx+(ry<<7))<<2; return (base[2]!=0xFF)-1; }
