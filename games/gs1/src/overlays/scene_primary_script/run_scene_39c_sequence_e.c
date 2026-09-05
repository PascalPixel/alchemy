#include "types.h"

#define Lifted_020017a8 Func_020017a8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001998();
void Func_02001aa6();
s32 Func_02002122();
void Func_020022e0();
void Func_020023fc();
void Func_02002424();
void Func_0200248c();
void Func_02002e8e();
void Func_02002ea4();
void Func_0200326a();
void Func_02003316();
void Func_02003324();
void Func_02005264();
void Func_020053a2();
s32 Func_020071a6();
s32 Func_020071be();
void Func_020071ce();
void Func_020071de();
s32 Func_02007258();
void Func_020072a4();
s32 Func_020072ce();
void Func_02007310();
void Func_02007316();
void Func_0200731c();
void Func_02007326();
void Func_02007366();
void Func_02007394();
void Func_0200741c();
void Func_0200741e();
void Func_02007430();
void Func_02007442();
void Func_02007452();
void Func_02007462();
void Func_02007472();
void Func_02007476();
void Func_02007490();
void Func_020074a0();
void Func_020074a6();
void Func_020074b0();
void Func_020074c0();
void Func_020074d0();
void Func_020074e0();
void Func_020074f0();
void Func_02007500();
void Func_0200752e();
void Func_020075aa();
s32 Func_020075c2();
void Func_020075f4();
void Func_0200760e();
void Func_02007620();
void Func_02007640();
void Func_02007664();
void Func_0200766c();
void Func_020076fa();
void Func_02007712();
void Func_0200771e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}



typedef struct {
    s32 f0;
    s32 f1;
    s32 f2;
    s32 f3;
    s32 t0;
    s32 t1;
} SceneEvent;

void Func_020019b8(void)
{
    SceneEvent ev;
    s32 kind;
    s32 shape;
    s32 three;

    Func_02007476();
    if (Func_02002122(&ev) != 0) {
        kind = ev.f1;
        if (kind == 8) {
            shape = ev.f2;
            if ((shape >> 20) == 11) {
                Func_020022e0(ev);
                Func_020074a6(30);
                Func_02007664(211);
                Func_02005264();
                three = 3;
                Func_0200741e(76, 60, 74, 38, three, 1);
                Func_02007430(77, 60, 76, 38, 2, 1);
                Func_02007442(75, 58, 86, 41, 1, three);
                Func_02007452(75, 59, 86, 43, 1, 2);
                Func_02007462(76, 59, 80, 49, 2, 1);
                Func_02007472(77, 59, 82, 49, 2, 1);
                Func_02007500(0x302);
            } else {
                ev.t1 = 0x200b949;
                Func_02007490(75, 57, 86, 41, 1, 1);
                Func_020074a0(71, 59, 86, 42, 1, 1);
                Func_020074b0(71, 59, 86, 43, 1, 1);
                Func_020074c0(71, 59, 86, 44, 1, 1);
                Func_020074d0(71, 59, 80, 49, 1, 1);
                Func_020074e0(71, 59, 81, 49, 1, 1);
                Func_020074f0(71, 59, 82, 49, 1, 1);
                Func_02007500(78, 58, 83, 49, 1, 1);
                Func_020023fc(ev);
                Func_020075aa(0x302);
            }
        } else if (kind == 10) {
            if ((ev.t0 >> 20) == 40) {
                Func_02002424(ev);
                if (Value1(Func_020075c2, 0x307) == 0) {
                    Func_020076fa(0x18000, 0x3000);
                    Func_02007712(0x2ca0000, -1, 0x2500000, 1);
                    Func_0200771e();
                    Call1(Func_020075f4, 0x307);
                    Func_02003316(5);
                    Func_02007620(50);
                } else {
                    Func_02003324(5);
                }
                Func_0200760e(0x306);
            } else if ((ev.t0 >> 20) == 42) {
                ev.t1 = 0x20098f9;
                Func_0200248c(ev);
                Func_0200326a(5);
                Func_02007640(0x306);
            }
        }
    }
    Func_0200766c();
}
