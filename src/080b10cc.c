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

s32 Func_08015080(s32, s32, s32, s32);
s32 Func_080150b0(s32, s32, s32, s32, s32);

void Func_080b10cc(void) {
    s32 temp_r5;
    register s32 first asm("r0");
    register s32 *base asm("r3");

    temp_r5 = M2C_FIELD(*(void **)0x03001F2C, s32 *, 0xC);
    if (temp_r5 != 0) {
        first = 0xC8A;
        asm volatile("" : "+r" (first));
        Func_08015080(first, temp_r5, 0, 0);
        base = (s32 *) 0x02000240;
        asm volatile("" : "+r" (base));
        Func_080150b0(base[4], 6, temp_r5, 0x20, 8);
    }
}
