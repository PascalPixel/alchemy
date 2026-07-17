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
extern u8 Data_02010000[];

extern void Func_08003f3c(u16 arg0);
extern void Func_08015018(u32 arg0, u32 arg1);

void Func_0809bcd4(void) {
    Func_08003f3c(*(u16 *)(Data_02010000 + 0));
    Func_08003f3c(*(u16 *)(Data_02010000 + 2));
    Func_08015018(*(u32 *)(Data_02010000 + 0x1c), 2);
}
