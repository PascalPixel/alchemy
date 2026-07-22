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

s32 Func_0200649a();
s32 Func_020064b4();

s32 Func_02000058(void *arg0) {
    s16 var_r3;
    s32 temp_r3;
    s32 temp_r3_2;

    if (M2C_FIELD(arg0, s16 *, 0x66) != 0) {
        temp_r3 = (M2C_FIELD(arg0, s32 *, 0xC) - ((u32) (Func_0200649a() << 0xF) >> 0x10)) + 0xFFFF8000;
        M2C_FIELD(arg0, s32 *, 0xC) = temp_r3;
        if (temp_r3 < 0) {
            var_r3 = 0;
            goto block_5;
        }
    } else {
        temp_r3_2 = M2C_FIELD(arg0, s32 *, 0xC) + ((u32) (Func_020064b4() << 0xF) >> 0x10) + 0x8000;
        M2C_FIELD(arg0, s32 *, 0xC) = temp_r3_2;
        if (temp_r3_2 > 0x80000) {
            var_r3 = 1;
block_5:
            M2C_FIELD(arg0, s16 *, 0x66) = var_r3;
        }
    }
    return 1;
}
