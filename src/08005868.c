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

s32 Func_080072f0(s32, s32, s32, s32);
s32 Func_08006c68(s32, s32);

struct Unk { u8 pad[64]; s32 field; };
u32 Func_08005868(u16 arg0) {
    s32 *var_r2 = (s32 *)0x02004C04;
    s32 temp_r0;
    struct Unk *temp_r6;
    s32 temp_r5;
    temp_r6 = *(struct Unk **)0x03001F1C;
    temp_r5 = arg0 & 0xFFFF;
    if ((Func_080072f0(temp_r5, (s32) &temp_r6->field, (s32) var_r2, *var_r2) << 0x10) != 0) {
        return 1U;
    }
    temp_r0 = Func_08006c68(temp_r5, (s32) &temp_r6->field);
    return (u32) ((0 - temp_r0) | temp_r0) >> 0x1F;
}
