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

u8 *Func_08077008(void);
s32 Func_080bf208(s32, u8, s32);

s32 Func_080bf400(s32 arg0) {
    u8 *value = Func_08077008() + 0x13A;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
        if (Func_080bf208(arg0, *value, 70) != 0) {
            *value = 0;
            return 1;
        }
    }
    return 0;
}
