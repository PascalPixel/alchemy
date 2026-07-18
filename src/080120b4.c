typedef unsigned char u8;
typedef signed int s32;

s32 Func_080120b4(s32 arg0, s32 arg1)
{
    s32 x = arg0 / 16;
    s32 y = arg1 / 16;
    u8 *entry = (u8 *)0x02010000 + (x + y * 128) * 4;

    return entry[1] >> 6;
}
