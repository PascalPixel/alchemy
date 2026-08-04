typedef signed int s32;
extern s32 Func_080770c0(s32);
extern void Func_0808a170(s32);
extern void Func_0808a180(s32, s32);
void Func_020007f4(void)
{
    if (Func_080770c0(0x941) == 0) {
        s32 message;
        if (Func_080770c0(0x85a) == 0)
            message = 0x1be2;
        else
            message = 0x1ba5;
        Func_0808a170(message);
        Func_0808a180(18, 0);
    } else {
        Func_0808a170(0x250c);
        Func_0808a180(18, 0);
    }
}
