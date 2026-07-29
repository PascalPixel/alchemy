typedef signed int s32;

void Func_02007dc4(s32, s32);
void Func_02007dce(s32, s32);
s32 Func_02007cf8(s32, s32);
void Func_0200430e(s32);

void Func_020020dc(s32 a)
{
    if (*(s32 *)0x03001e40 & 2) {
        Func_02007dc4(a, 7);
    } else {
        Func_02007dce(a, 0);
    }
    if (Func_02007cf8(*(s32 *)0x03001e40, 15) == 0) {
        Func_0200430e(a);
    }
}
