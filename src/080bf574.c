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

s32 Func_080bf574(void) {
    u8 *base = Func_08077008();
    u8 *value = base + 0x146;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            base[0x147] = 0;
            return 1;
        }
    }
    return 0;
}
