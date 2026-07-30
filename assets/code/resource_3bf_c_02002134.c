typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_0000240d;

void Func_02007768(void);
void Func_020077d2(s32, s32, s32);
void Func_020077aa(s32, s32);
void Func_020077fa(s32, s32);
void Func_0200782c(s32, s32, s32);
void Func_0200790a(s32);
void Func_02007886(s32, s32, s32);
void Func_0200785e(s32);
void Func_02007876(s32, s32);
void Func_020078a4(s32, s32, s32);
void Func_0200787a(s32);
void Func_02007892(s32, s32);
void Func_02007916(void);
void Func_020077cc(s32);
void Func_02007902(s32);
void Func_020077e6(void);
void Func_020077ce(s32);

void Func_02002134(void)
{
    Func_02007768();
    Func_020077d2(0, 0, 0);
    Func_020077aa(0, 1);
    Func_020077fa(0, 1);
    Func_0200782c(12, 0, 0);
    Func_0200790a(113);
    Func_02007886(12, 256, 60);
    {
        u8 *t = &Value_0000240d;

        Func_0200785e((s32)t);
        Func_02007876(12, 0);
        Func_020078a4(0, 258, 50);
        Func_0200787a((s32)(t + 1));
    }
    Func_02007892(12, 0);
    Func_02007916();
    Func_020077cc(60);
    Func_02007902(60);
    Func_020077e6();
    Func_020077ce(548);
}
