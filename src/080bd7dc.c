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

void Func_080bd7dc(s32 arg0) {
    void *temp_r1;

    temp_r1 = *(void **)0x03001E74;
    if (M2C_FIELD(temp_r1, s32 *, 0x800) == 0) {
        M2C_FIELD(temp_r1, s32 *, 0x800) = 1;
        if (arg0 != 0) {
            M2C_FIELD(temp_r1, s32 *, 0x820) = arg0;
        }
    }
}
