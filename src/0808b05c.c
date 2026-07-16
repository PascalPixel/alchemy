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

u32 Func_0808b05c(s32 arg0, s32 arg1) {
    register s32 offset asm("r3");
    register u8 *base asm("r2");
    register u32 result asm("r0");

    offset = arg0 << 3;
    offset -= arg0;
    offset <<= 1;
    offset += arg1;
    base = (u8 *) 0x0809C610;
    asm volatile("" : "+r" (base) : "r" (offset));
    offset <<= 1;
    offset += 4;
    asm volatile("ldrh %0, [%1, %2]"
                 : "=r" (result)
                 : "r" (base), "r" (offset));
    return result;
}
