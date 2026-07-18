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

s32 Func_08096810(void);
s32 Func_08096960(void);
s32 Func_08096ab0(void);
s32 Func_08096af0(void);

void Func_080967e4(s32 arg0) {
    if (arg0 == 0) {
        Func_08096810();
    } else if (arg0 == 1) {
        Func_08096960();
    } else if (arg0 == 2) {
        Func_08096ab0();
    } else if (arg0 == 3) {
        Func_08096af0();
    }
}
