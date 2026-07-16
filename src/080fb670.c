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

s32 Func_080072ec(s32, void *, s32);

void Func_080fb670(s32 arg0, void *arg1) {
    register u8 *temp_r2 asm("r2");
    register s32 index asm("r3");

    temp_r2 = M2C_FIELD(arg1, u8 **, 0x40);
    index = *temp_r2;
    asm volatile("" : "+r" (index) : "r" (temp_r2));
    temp_r2 += 1;
    M2C_FIELD(arg1, u8 **, 0x40) = temp_r2;
    Func_080072ec(arg0, arg1, M2C_FIELD((index * 4), s32 *, 0x080FBA48));
}
