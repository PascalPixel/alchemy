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

s32 Func_08009098(void *, s32);

void Func_08099040(void *arg0) {
    register s32 delta asm("r1");
    register s32 first asm("r2");
    register s32 second asm("r3");

    if (arg0 != NULL) {
        delta = 0xFFFFF000;
        first = M2C_FIELD(arg0, s32 *, 0x1C);
        second = M2C_FIELD(arg0, s32 *, 0x18);
        asm volatile("" : "+r" (delta), "+r" (first), "+r" (second));
        first += delta;
        M2C_FIELD(arg0, s32 *, 0x1C) = first;
        second += delta;
        M2C_FIELD(arg0, s32 *, 0x18) = second;
        if (second <= 0x1000) {
            Func_08009098(arg0, 0x0809F0B0);
        }
    }
}
