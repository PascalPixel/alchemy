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

s32 Func_080090f0(void *, s32, s32, s32);

void Func_0809a890(void *arg0) {
    s32 temp_r3;

    temp_r3 = M2C_FIELD(arg0, s32 *, 0x18) - 0x80;
    M2C_FIELD(arg0, s32 *, 0x1C) = temp_r3;
    M2C_FIELD(arg0, s32 *, 0x18) = temp_r3;
    if (temp_r3 < 0x8000) {
        Func_080090f0(arg0, 0, 0, 0);
        M2C_FIELD(arg0, s32 *, 0x6C) = 0;
    }
}
