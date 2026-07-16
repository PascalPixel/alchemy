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

s32 Func_080041d8(s32, s32);

s32 Func_080bd808(s32 arg0) {
    void *temp_r3;

    temp_r3 = *(void **)0x03001E74;
    M2C_FIELD(temp_r3, s32 *, 0x7FC) = 0;
    M2C_FIELD(temp_r3, s32 *, 0x804) = 0;
    M2C_FIELD(temp_r3, s32 *, 0x808) = arg0;
    M2C_FIELD(temp_r3, s32 *, 0x800) = 2;
    M2C_FIELD(temp_r3, s8 *, 0x655) = 0;
    return Func_080041d8(0x080BD899, 0xC80);
}
