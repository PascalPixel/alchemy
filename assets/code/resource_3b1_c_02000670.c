typedef unsigned char u8;
typedef signed int s32;

void Func_02006b3c(void);
void Func_02006cc0(void);
s32 Func_02006b1e(s32 n);
s32 Func_02006b38(s32 n);
void Func_02006c30(s32 n);
void Func_02006c48(s32 a, s32 b);
void Func_02006c4a(s32 n);
s32 Func_02006c5a(s32 a, s32 b);
s32 Func_02006b92(s32 a, s32 b);
void Func_020025f6(void);
void Func_02006c3c(s32 a, s32 b);
void Func_02006c84(s32 a, s32 b);
void Func_02006ca8(s32 a, s32 b, s32 c);
void Func_02006c88(s32 n);
void Func_02006ca0(s32 a, s32 b);
void Func_02006bbc(void);

extern u8 Value_00000921;
extern u8 Value_00001dd4;
extern u8 Value_00000922;
extern u8 Value_00001d91;
extern u8 Value_00001d31;

void Func_02000670(void) {
    Func_02006b3c();
    Func_02006cc0();
    if (Func_02006b1e((s32)&Value_00000921) != 0) {
        Func_02006c30((s32)&Value_00001dd4);
        Func_02006c48(10, 0);
    } else if (Func_02006b38((s32)&Value_00000922) != 0) {
        Func_02006c4a((s32)&Value_00001d91);
        Func_02006c5a(10, 0);
        if (Func_02006b92(0, 0) == 0) {
            Func_020025f6();
        } else {
            Func_02006c3c(10, 2);
            Func_02006c84(10, 0);
            {
                s32 v = 0xD000;
                Func_02006ca8(10, v, 0);
            }
        }
    } else {
        Func_02006c88((s32)&Value_00001d31);
        Func_02006ca0(10, 0);
    }
    Func_02006bbc();
}
