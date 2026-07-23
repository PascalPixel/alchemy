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

s32 Func_0200248e(s32);
s32 Func_02002498(s32);
s32 Func_020024a6(s32);
void Func_020025a8(s32);

s32 Func_02000054(void) {
    s32 var_r0;

    if (Func_0200248e(0x818) != 0) {
        if (Func_02002498(0x813) == 0) {
            var_r0 = 3;
            goto block_5;
        }
        goto block_6;
    }
    if (Func_020024a6(0x812) == 0) {
        var_r0 = 4;
block_5:
        Func_020025a8(var_r0);
        return 1;
    }
block_6:
    return -1;
}
