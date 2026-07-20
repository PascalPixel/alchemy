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

s32 Func_080770c0(s32 arg0);
void Func_080770c8(s32 arg0);
void Func_080770d0(s32 arg0);

s32 Func_0800d880(void *arg0) {
    s32 temp_r5;
    s32 temp_r0;

    temp_r5 = M2C_FIELD((M2C_FIELD(arg0, s16, 4) * 4) + M2C_FIELD(arg0, s32, 0), s32, 4);
    temp_r0 = Func_080770c0(temp_r5);
    M2C_FIELD(arg0, s8, 0x57) = temp_r0;
    if ((temp_r0 << 0x18) == 0x01000000) {
        Func_080770d0(temp_r5);
    } else {
        Func_080770c8(temp_r5);
    }
    M2C_FIELD(arg0, s16, 4) = (s16) ((u16) M2C_FIELD(arg0, s16, 4) + 2);
    return 1;
}
