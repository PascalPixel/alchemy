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

s32 Func_080acab8(s32, s32, s32, u8, s32, s32, s32, s32, s32);

s32 Func_080aca04(s32 arg0, s32 arg1) {
    s32 sp14;
    void *temp_r7;

    temp_r7 = *(void **)0x03001F2C;
    if (arg0 == 0) {
        sp14 = arg0;
        Func_080acab8(M2C_FIELD(temp_r7, s32 *, 0x34), 0, 0, M2C_FIELD(temp_r7, u8 *, 0x259), 1, arg0, 2, arg1, 1);
        Func_080acab8(M2C_FIELD(temp_r7, s32 *, 0x24), 0, 0, M2C_FIELD(temp_r7, u8 *, 0x258), arg0, 1, 2, arg1, arg0);
    } else {
        Func_080acab8(M2C_FIELD(temp_r7, s32 *, 0x34), 0, 0, M2C_FIELD(temp_r7, u8 *, 0x21B), 1, 0, 2, arg1, 1);
        Func_080acab8(M2C_FIELD(temp_r7, s32 *, 0x24), 0, 0, M2C_FIELD(temp_r7, u8 *, 0x21A), 0, 0, 1, arg1, 0);
    }
    return 1;
}
