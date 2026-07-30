typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_0000256c;

void Func_0200a440(s32);
void Func_0200a458(s32, s32);
s32 Func_0200a36e(s32);
void Func_0200a376(s32, s32);

void Func_02004d50(void)
{
    u8 *t = &Value_0000256c;

    Func_0200a440((s32)t);
    Func_0200a458(0x800d, 0);
    if (Func_0200a36e(234) != -1) {
        Func_0200a376((s32)(t + 2), 1);
    }
}
