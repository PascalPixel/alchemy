typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
extern u16 Data_080c593c[];
u32 Func_080b6e7c(s32 arg0) {
s32 i; s32 v; for(i=0;;i++){ v=Data_080c593c[i]; if(arg0==(v&0x1FF)){register u32 w __asm__("r3"); register u32 res __asm__("r0"); __asm__ __volatile__("":::"memory"); w=Data_080c593c[i]; res=w>>9; return res;} if((s16)v==-1) break; } return 6;
}
