typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_0000244f;
extern u8 Value_00002455;

void Func_02009dc8(void);
s32 Func_02009dae(s32);
void Func_02009e90(s32);
void Func_02009ea8(s32, s32);
void Func_02009dec(void);
s32 Func_02009dcc(s32);
void Func_02009eae(s32);
s32 Func_02009ebe();
void Func_02009e0a(void);
void Func_02009ef8(s32, s32, s32);
void Func_02009eba(s32, s32, s32);
void Func_02009eda(s32);
void Func_02009ef2(s32, s32);
void Func_02009ed4(s32, s32, s32);
void Func_02009f54(s32, s32);
void Func_02009f50(void);
void Func_02009e3e(s32);
void Func_02009f66(s32, s32);
void Func_02009e4c(s32);
void Func_02009f46(s32, s32, s32);
void Func_02009f1c(s32);
void Func_02009f34();
void Func_02009f5e();
void Func_02009f4c(s32, s32);
void Func_02009e82(s32);
void Func_02009f7e(s32, s32, s32);
void Func_02009f40(s32, s32, s32);
s32 Func_02009f6e();
s32 Func_02009ece(s32, s32);
void Func_02009f78(s32);
void Func_02009f88();
void Func_02009f98();
void Func_02009ed6(s32);
void Func_02009fd0(s32, s32, s32);
void Func_02009fa8(s32);
s32 Func_02009fb8();
void Func_02009fbe(s32);
s32 Func_02009fce();
void Func_02009fd6(s32);
s32 Func_02009fe6();
void Func_02009f14(s32);
void Func_02009f38(void);

void Func_02004794(void)
{
    Func_02009dc8();
    if (Func_02009dae(0x941)) {
        Func_02009e90(0x2566);
        Func_02009ea8(18, 0);
        Func_02009dec();
        return;
    }
    if (Func_02009dcc(0x313)) {
        Func_02009eae(0x2457);
        Func_02009ebe(25, 0);
        Func_02009e0a();
        return;
    }
    Func_02009ef8(25, 258, 30);
    Func_02009eba(25, 0, 0);
    {
        u8 *t = &Value_0000244f;

        Func_02009eda((s32)t);
        Func_02009ef2(25, 0);
        Func_02009ed4(25, 24, 0);
        Func_02009f54(24, 1);
        Func_02009f50();
        Func_02009e3e(60);
        Func_02009f66(0, 1);
        Func_02009e4c(20);
        Func_02009f46(25, 0x105, 60);
        Func_02009f1c((s32)(t + 1));
        Func_02009f34(25, 0);
        Func_02009f5e(25, 0x107, 60);
        Func_02009f34((s32)(t + 2));
        Func_02009f4c(25, 0);
        Func_02009e82(70);
        Func_02009f7e(25, 256, 60);
        Func_02009f40(25, 0, 0);
        Func_02009f5e((s32)(t + 3));
        Func_02009f6e(25, 0);
        if (Func_02009ece(0, 0) == 0) {
            Func_02009f78((s32)(t + 4));
            Func_02009f88(25, 0);
        } else {
            Func_02009f88((s32)(t + 5));
            Func_02009f98(25, 0);
        }
    }
    Func_02009ed6(60);
    Func_02009fd0(25, 0x105, 60);
    {
        u8 *t = &Value_00002455;

        Func_02009fa8((s32)t);
        Func_02009fb8(25, 0);
        Func_02009f98(25, 1);
        Func_02009fbe((s32)(t + 1));
        Func_02009fce(25, 0);
        t += 2;
        Func_02009f98(25, 3);
        Func_02009fd6((s32)t);
        Func_02009fe6(25, 0);
    }
    Func_02009f14(0x313);
    Func_02009f38();
}
