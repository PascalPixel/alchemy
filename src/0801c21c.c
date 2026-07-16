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

s32 Func_08003f3c(u16);

void Func_0801c21c(void) {
    register s32 zero asm("r3");
    void *temp_r5;

    temp_r5 = *(s32 *)0x03001E98 + 0x30C;
    if (M2C_FIELD(temp_r5, u16 *, 0xA) != 0) {
        Func_08003f3c(M2C_FIELD(temp_r5, u16 *, 0xC));
        zero = 0;
        asm volatile("" : "+r" (zero));
        M2C_FIELD(temp_r5, s16 *, 0xA) = zero;
    }
}
