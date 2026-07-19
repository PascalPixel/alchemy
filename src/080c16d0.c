typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

void Func_080072f0(u32, s32, s32, u32);
void Func_08002dd8(s32);
s32 Func_08004278(u32);

s32 Func_080c16d0(s32 unused0, s32 unused1, s32 mode)
{
    Func_080072f0(0x06004000, 0x4000, mode, 0x03000164);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08002dd8(40);
    Func_08002dd8(39);
    *(u16 *)0x04000000 = 0x1341;
    Func_08004278(0x080c1439);
    return Func_08004278(0x080c11ed);
}
