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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080e2974(void *, s32);

void Func_080e28f4(void *arg0) {
    s32 temp_r3;

    temp_r3 = M2C_FIELD(arg0, s32 *, 0x18);
    if (temp_r3 == 0) {
        Func_080e2974(arg0, 6);
        return;
    }
    if (temp_r3 == 1) {
        Func_080e2974(arg0, 7);
        return;
    }
    Func_080e2974(arg0, 8);
}
