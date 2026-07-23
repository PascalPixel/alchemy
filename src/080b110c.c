typedef unsigned char u8;
typedef signed int s32;

void Func_08015080(s32, s32, s32, s32);
void Func_080150b0(s32, s32, s32, s32, s32);
void Func_08015270(s32);
extern u8 Value_00000182;

void Func_080b110c(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 effect;

    while (arg0 != 0)
        goto main_block;
    goto end;
case_one:
    Func_08015080(0xc92, arg0, 0, 8);
    goto end;
block_3:
    effect = 0xc8b;
    Func_08015080(effect, arg0, 0, 8);
    effect -= 3;
    Func_080150b0(arg2, 5, arg0, 0x20, 8);
    Func_08015080(effect, arg0, 0x48, 8);
    goto end;
main_block:
    Func_08015270(arg0);
    Func_08015080(arg1 + (s32)&Value_00000182, arg0, 0, 0);
    if (arg2 != 0)
        goto block_3;
    if (arg3 == 1)
        goto case_one;
    if (arg3 != 2)
        goto block_3;
    Func_08015080(0xc93, arg0, 0, 8);
end:
    return;
}
