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

void *Func_08077008();

s32 Func_080bf54c(void) {
    s32 var_r0;
    register u32 temp_r2 asm("r2");
    register u32 temp_r3 asm("r3");
    register u8 *temp_r1 asm("r1");
    register void *temp_r0 asm("r0");

    temp_r0 = Func_08077008();
    temp_r3 = 0x13F;
    asm volatile("add r1, r0, r3" : "=r" (temp_r1)
                 : "r" (temp_r0), "r" (temp_r3));
    temp_r2 = *temp_r1;
    temp_r3 = temp_r2;
    asm volatile("" : "+r" (temp_r1), "+r" (temp_r2), "+r" (temp_r3));
    if ((temp_r3 == 0) || (temp_r3 += 0xFF, *temp_r1 = temp_r3, var_r0 = 1, ((temp_r3 << 0x18) != 0))) {
        var_r0 = 0;
    }
    return var_r0;
}
