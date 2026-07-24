typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

u8 *Func_08077008(s32);
u8 *Func_08077018(u16);
void Func_080b1f4c(s32, s32, s32);

s32 Func_080b196c(s32 arg0, s32 arg1)
{
    u8 *state;
    s32 offset;
    s32 val;

    state = Func_08077008(arg0);
    if (arg1 == -1)
    {
        return 0;
    }
    offset = arg1 * 2 + 216;
    val = 0x1FF & *(u16 *)(state + offset);
    if (Func_08077018(val)[2] == 6)
    {
        return 0;
    }
    if (Func_08077018(val)[3] & 8)
    {
        return 0;
    }
    Func_080b1f4c(arg0, arg1, -1);
    return 1;
}
