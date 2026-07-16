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
s32 Func_080b5078(s32, s32, s32, s32);
s32 Func_080d4604(void *, s32);

void Func_080eceac(void *arg0) {
    register s32 first asm("r0");
    register s32 second asm("r1");
    register s32 offset asm("r3") = 0x24;

    asm volatile("ldrsh %0, [%1, %2]"
                 : "=r" (second) : "r" (arg0), "r" (offset));
    first = M2C_FIELD(arg0, s32 *, 8);
    asm volatile("" : "+r" (first) : "r" (second));
    Func_080b5078(first, second, 0x18, 0x73333);
    Func_080030f8(0xC);
    M2C_FIELD(arg0, s32 *, 0x18) = 3;
    Func_080d4604(arg0, 2);
}
