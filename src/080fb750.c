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

void Func_080fb750(s32 arg0, void *arg1) {
    register u8 *temp_r0 asm("r0");
    register u8 value asm("r2");

    temp_r0 = M2C_FIELD(arg1, u8 **, 0x40);
    value = *temp_r0;
    asm volatile("" : "+r" (value) : "r" (temp_r0));
    M2C_FIELD(arg1, u8 *, 0x1E) = value;
    asm volatile("" : "+r" (temp_r0) : "r" (value) : "memory");
    M2C_FIELD(arg1, u8 **, 0x40) = (u8 *) (temp_r0 + 1);
}
