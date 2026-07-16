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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080030f8(s32);
s32 Func_08009240(void *, s32);

void Func_08097174(void) {
    void *temp_r0;

    temp_r0 = M2C_FIELD(*(void **)0x03001F30, void **, 0x10);
    M2C_FIELD(temp_r0, s32 *, 0x6C) = 0;
    Func_08009240(temp_r0, 0);
    Func_080030f8(1);
}
