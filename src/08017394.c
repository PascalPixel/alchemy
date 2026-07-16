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

s32 Func_08017394(void *arg0) {
    s32 value;

    value = M2C_FIELD(arg0, u16 *, 0x16);
    if (value == 0) {
        register s32 result asm("r0");

        value = M2C_FIELD(arg0, s16 *, 0x1A);
        result = 1;
        asm volatile("" : "+r" (result) : "r" (value));
        if (value == 0) {
            return result;
        }
    }
    return 0;
}
