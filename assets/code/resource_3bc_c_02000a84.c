typedef signed int s32;

extern void Func_0200561a(s32);
extern void Func_02005448(s32);
extern void Func_020052d6(s32);
extern void Func_020052ea(s32);
extern void Func_020052fa(s32);
extern void Func_02005310(s32);

void Func_02000a84(void)
{
    s32 *status;
    s32 value;

    Func_0200561a(28);
    Func_02005448(0x361);
    Func_020052d6(10);
    value = *(s32 *)0x0200D480;
    if (value != 1 && value != 3) {
        status = (s32 *)0x0200D480;
        do {
            Func_020052ea(1);
            value = *status;
        } while (value != 1 && value != 3);
    }
    Func_020052fa(1);
    Func_02005310(0x0200804D);
}
