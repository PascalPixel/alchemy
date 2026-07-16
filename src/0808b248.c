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

extern u8 Data_02000240[];

s16 Func_0808b248(void) {
    register u8 *base asm("r3");
    register s32 offset asm("r2");
    base = Data_02000240;
    asm volatile("" : "+r" (base));
    offset = 0xEB;
    asm volatile("" : "+r" (offset) : "r" (base));
    offset <<= 1;
    base += offset;
    asm volatile("" : "+r" (base));
    offset = 0;
    asm volatile("" : "+r" (offset) : "r" (base));
    return *(s16 *)(base + offset);
}
