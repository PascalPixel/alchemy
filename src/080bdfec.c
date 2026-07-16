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

void Func_080bdfec(void) {
    void *temp_r2;

    temp_r2 = *(void **)0x03001E74;
    M2C_FIELD(temp_r2, s32 *, 0x800) = 0;
    M2C_FIELD(temp_r2, s32 *, 0x7FC) = 0;
    M2C_FIELD(temp_r2, s32 *, 0x804) = 0;
    M2C_FIELD(temp_r2, s32 *, 0x808) = 0;
    M2C_FIELD(temp_r2, s32 *, 0x7F8) = 0;
    M2C_FIELD(temp_r2, s32 *, 0x820) = 0x86;
    M2C_FIELD(temp_r2, s32 *, 0x824) = 0;
}
