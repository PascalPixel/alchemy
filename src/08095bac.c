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

void Func_08095bac(void *arg0) {
    u32 temp_r2;

    temp_r2 = M2C_FIELD(arg0, s16 *, 0x64) * 0x50;
    M2C_FIELD(arg0, u16 *, 6) = (u16) (M2C_FIELD(arg0, u16 *, 6) + temp_r2 + 0x1000);
    if (temp_r2 < 0x1000U) {
        M2C_FIELD(arg0, s16 *, 0x64) = (s16) ((u16) M2C_FIELD(arg0, s16 *, 0x64) + 1);
    }
}
