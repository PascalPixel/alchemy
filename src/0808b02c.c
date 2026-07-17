typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
extern u8 *Data_03001ebc;

extern s32 Func_0808ae74(u8 arg0);

s32 Func_0808b02c(u32 arg0) {
    u8 *table = Data_03001ebc + 0x1a0;
    return Func_0808ae74(table[arg0]);
}
