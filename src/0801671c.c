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

s32 Func_080072f0(s32, s32, s32, s32);

s32 Func_0801671c(void) {
    register s32 size asm("r1");
    register s32 destination asm("r3");
    register s32 zero asm("r2");
    register s32 source asm("r0");

    size = 0xF0;
    destination = 0x03000168;
    asm volatile("" : "+r" (destination) : "r" (size));
    size <<= 4;
    zero = 0;
    source = 0x06002500;
    return Func_080072f0(source, size, zero, destination);
}
