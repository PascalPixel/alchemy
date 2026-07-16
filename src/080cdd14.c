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

s32 Func_080030f8(s32);
s32 Func_0800387c(s32, s32);
s32 Func_080b5038(s32, u16, s32);

void Func_080cdd14(void) {
    void *temp_r5;
    void **base;
    void *temp_r2;
    register s32 second asm("r1");
    register s32 first asm("r0");

    base = (void **) 0x03001F00;
    temp_r2 = *base;
    base = (void **) ((u8 *) base - 0x8C);
    temp_r5 = *base;
    M2C_FIELD(temp_r2, s32 *, 0xC) = 1;
    Func_0800387c(0x04000000, 0x1541);
    Func_080030f8(1);
    second = M2C_FIELD(temp_r5, u16 *, 0x648);
    asm volatile("mov %0, #2" : "=r" (first));
    Func_080b5038(first, second, 0);
    Func_080030f8(1);
}
