typedef signed int s32;
typedef unsigned char u8;

extern s32 Func_0200418e(s32, s32, s32, s32, s32, s32);
extern u8 *Func_020041fc(s32);

void Func_02001ca0(void)
{
    s32 width = 38;
    s32 height = 55;
    u8 *entry;

    Func_0200418e(40, 54, 1, 1, width, height);
    entry = Func_020041fc(10) + 85;
    *entry = 0;
}
