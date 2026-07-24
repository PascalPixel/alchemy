typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern s32 Func_08018038(s32, s32);
extern void Func_0801868c(s32, s32, s32, s32, s32, s32, s32);

s32 Func_080187fc(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u16 *base;
    s32 temp;
    s32 offset;

    base = *(u16 **)0x03001E8C;
    temp = Func_08018038(arg0, 0);
    offset = temp * 2 + 3760;
    if (*(u16 *)((u8 *)base + offset) == 0)
    {
        return 0;
    }
    Func_0801868c(temp, arg1, arg2, arg3, arg4, 0, 1);
    return 1;
}
