typedef signed short s16;
typedef signed int s32;

void Func_020011e4(s32, s32, s32, s32, s32, s32);
void Func_020011fa(s32, s32, s32, s32, s32, s32);

void Func_02000150(void)
{
    if (**(s16 **)0x020092c4 == 1) {
        s32 fifth = 4;
        s32 sixth = 9;
        Func_020011e4(0, 0, 1, 4, fifth, sixth);
    } else {
        s32 fifth = 6;
        s32 sixth = 9;
        Func_020011fa(0, 0, 1, 4, fifth, sixth);
    }
}
