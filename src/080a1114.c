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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08015018();

void Func_080a1114(s32 *arg0) {
    register s32 value asm("r0") = *arg0;

    if (value != 0) {
        Func_08015018();
        *arg0 = 0;
    }
}
