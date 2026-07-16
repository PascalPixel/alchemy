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

s32 Func_08009098(void *, s32);

void Func_08097a54(void *arg0) {
    s32 temp_r2;
    s32 temp_r3;

    temp_r3 = M2C_FIELD(arg0, s32 *, 0x38);
    if (temp_r3 == 0x80000000) {
        temp_r2 = M2C_FIELD(arg0, s32 *, 0x3C);
        if ((temp_r2 == temp_r3) && (M2C_FIELD(arg0, s32 *, 0x40) == temp_r2)) {
            Func_08009098(arg0, 0x080A0128);
        }
    }
}
