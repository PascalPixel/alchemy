typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_0000240d;

void Func_020070e4(void);
void Func_0200714e();
void Func_02007126(s32, s32);
void Func_0200713c();
void Func_0200717c(s32, s32);
void Func_020071cc(s32);
void Func_020071e4(s32, s32);
void Func_02007212(s32, s32, s32);
void Func_020071e8(s32);
void Func_02007200(s32, s32);
void Func_02007266(s32);
void Func_0200728a(void);

void Func_02001ab0(void)
{
    Func_020070e4();
    Func_0200714e(9, 0, 0);
    Func_02007126(9, 1);
    Func_0200713c(9);
    Func_0200717c(9, 0);
    Func_0200713c(0, 1);
    {
        u8 *t = &Value_0000240d;

        Func_020071cc((s32)t);
        Func_020071e4(9, 0);
        Func_02007212(0, 258, 60);
        Func_020071e8((s32)(t + 1));
    }
    Func_02007200(9, 0);
    Func_02007266(60);
    Func_0200728a();
    Func_0200714e();
}
