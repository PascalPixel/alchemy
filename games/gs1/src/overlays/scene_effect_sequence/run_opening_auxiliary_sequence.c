#include "types.h"

#define RunOpeningAuxiliarySequence Func_02000108

s32 Func_020044d0();
void Func_020044e4();
void Func_020044f2();
void Func_02004504();
u8 *Func_02004510();
void Func_0200453c();
void Func_0200456c();
void Func_0200456e();
void Func_0200459c();
void Func_020045b4();
void Func_020045c4();
void Func_020045e0();
void Func_020045f8();
s32 Func_020045fc();
void Func_02004604();
void Func_02004612();
s32 Func_02004614();
void Func_02004626();
void Func_0200462c();
void Func_02004630();
void Func_02004636();
void Func_0200463c();
void Func_0200464a();
void Func_02004664();
void Func_02004680();
void Func_02004684();
void Func_0200468c();
void Func_02004696();
s32 Func_0200469c();
void Func_020046a6();
void Func_020046b6();
s32 Func_020046bc();
void Func_020046be();
void Func_020046c2();
void Func_020046ce();
void Func_020046d0();
void Func_020046d2();
void Func_020046da();
void Func_0200470e();
void Func_02004718();
void Func_02004740();
void Func_0200474c();
void Func_02004756();
void Func_0200475a();
void Func_0200475c();
void Func_0200476c();
void Func_02004774();
void Func_02004776();
void Func_02004780();
void Func_0200478c();
void Func_020047bc();
void Func_020047cc();
void Func_02004842();
void Func_0200484c();

extern u8 Value_00001ff8;

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void RunOpeningAuxiliarySequence(s32 a)
{
    u8 *ret;
    s16 v;
    s32 c;
    s32 t;

    ret = Func_02004510(0);
    v = (*(u16 *)(ret + 6) + 0x2000) & 0xc000;
    Func_02004504();
    Func_02004630();
    if (Value1(Func_020044d0, 512) == 0) {
        Call1(Func_020044e4, 512);
        Func_020044f2(0x969);
        Func_020045e0(0x1ff7);
        Func_020045f8(a, 0);
        Func_0200453c(10);
        t = v << 16;
        c = 0x4000;
        if (t == (0x4000 << 16)) {
            Func_020045b4(0, 40, 104);
            Func_02004636(0, 0, 0);
        }
        Call3(Func_0200459c, a, 0x10000, 0x8000);
        Call3(Func_020046d0, a, 0, -48);
        Func_020046da(a, 64, 0);
        Func_02004664(a, c, 0);
    } else {
        Call1(Func_0200456e, 512);
        Func_0200456c(0x969);
        Call3(Func_02004680, a, 0x4000, 0);
        Func_02004612(0, 120, 96);
        Func_02004696(0, 0xc000, 0);
        Func_020045c4(20);
        c = (s32)&Value_00001ff8;
        Func_0200468c(c);
        Func_0200469c(a, 0);
        if (Func_020045fc(0, 0) == 0) {
            Func_020046a6(c + 1);
            Func_020046be(a, 0);
        } else {
            Func_020046b6(c + 2);
            Func_020046ce(a, 0);
        }
        Func_02004604(10);
        Func_0200469c(a, 3);
        Func_02004612(20);
        Call3(Func_02004776, a, -64, 0);
        Func_02004780(a, 0, 48);
    }
    Func_0200463c();
}
