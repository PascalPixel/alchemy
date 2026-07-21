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

typedef struct {
    u8 padding_00[6];
    s16 field_06;
    s16 field_08;
} Object;

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);

void Func_080a1c6c(Object **arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Object *temp_r5;
    s32 temp_r0;
    s32 temp_arg4;
    s32 var_r6;

    var_r6 = arg1;
    if (var_r6 > 0xF) {
        var_r6 = 0;
    }
    temp_arg4 = *(volatile s32 *)&arg4;
    temp_r5 = *(Object * volatile *)arg0;
    temp_r0 = Func_080022ec(var_r6, temp_arg4);
    temp_r5->field_08 = (s16)((temp_r0 * 0x10) + arg3);
    temp_r5->field_06 = (s16)((Func_080022fc(var_r6, arg4) * 0x18) + arg2);
    Func_080a17c4(temp_r5);
}
