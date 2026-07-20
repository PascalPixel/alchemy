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

s32 Func_0800d6d8(void *, s32);

s32 Func_0800d710(void *arg0) {
    s32 *ptr;
    s32 temp_r4;
    s32 temp_r1;
    u8 *p;
    s32 temp_r2;

    ptr = (s32 *)(M2C_FIELD(arg0, s32, 0) + (M2C_FIELD(arg0, s16, 4) << 2) + 4);
    temp_r4 = *ptr++;
    temp_r1 = *ptr;
    if (temp_r4 == 0xFFFF) {
        M2C_FIELD(arg0, s16, 4) = Func_0800d6d8(arg0, temp_r1);
    } else {
        p = (u8 *) arg0 + 0x5D;
        temp_r2 = *p + 1;
        *p = temp_r2;
        if ((s32) (u8) temp_r2 < (s32) (s16) temp_r4) {
            M2C_FIELD(arg0, s16, 4) = Func_0800d6d8(arg0, temp_r1);
        } else {
            *p = 0;
            M2C_FIELD(arg0, s16, 4) = M2C_FIELD(arg0, s16, 4) + 3;
        }
    }
    return 1;
}
