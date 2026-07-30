typedef signed int s32;
typedef unsigned char u8;

extern s32 Func_02003f8a(s32, s32, s32, s32, s32, s32);
extern u8 *Func_02003ff8(s32);

void Func_02001a9c(void)
{
    s32 width = 40;
    s32 height = 42;
    u8 *entry;

    Func_02003f8a(39, 42, 1, 1, width, height);
    entry = Func_02003ff8(8) + 35;
    *entry = 2;
}
