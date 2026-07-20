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

void Func_080041d8(s32, s32);

void Func_08097a7c(void) {
    volatile s16 *temp_r3;

    M2C_FIELD(*(void **)0x03001E8C, s8, 0xEA4) = 1;
    temp_r3 = (s16 *)0x050001E2;
    *temp_r3 = 0x739C;
    temp_r3 += 2;
    *temp_r3 = 0x739C;
    temp_r3 += 1;
    *temp_r3 = 0x739C;
    temp_r3 += 1;
    *temp_r3 = 0x739C;
    temp_r3 += 1;
    *temp_r3 = 0x739C;
    temp_r3 += 1;
    *temp_r3 = 0x739C;
    temp_r3 += 1;
    *temp_r3 = 0x739C;
    temp_r3 += 1;
    *temp_r3 = 0x739C;
    temp_r3 += 1;
    *temp_r3 = 0x739C;
    temp_r3 += 1;
    *temp_r3 = 0x739C;
    temp_r3 += 1;
    *temp_r3 = 0x739C;
    {
        s32 n = 0x480;
        Func_080041d8(0x08097869, n);
    }
}
