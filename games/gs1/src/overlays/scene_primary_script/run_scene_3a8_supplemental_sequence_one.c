#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02007056();
void Func_0200709e();
s32 Func_02007386();
s32 Func_0200738a();
s32 Func_02007392();
s32 Func_0200739e();
s32 Func_020073a4();
s32 Func_020073b0();
void Func_020073c4();
s32 Func_020073cc();
s32 Func_020073ea();
s32 Func_020073ec();
s32 Func_020073f8();
s32 Func_020073fc();
void Func_0200740c();
void Func_0200744c();
void Func_0200745c();
void Func_0200746e();
void Func_02007540();
void Func_0200755e();
void Func_02007566();
void Func_02007588();
void Func_020075a6();

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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 mask;
    s32 c12;
    s32 old;
    u8 *rp;
    u8 *p4;

    rec7 = Value1(Func_0200738a, 8);
    rec8 = Func_02007392(9);
    if ((u32)(*(s16 *)(rec7 + 10) + -0x17d) <= 12) {
        if (*(s16 *)(rec7 + 18) <= 0x309) {
            goto L_020037ae;
        }
        record = Value1(Func_020073b0, 0);
        p4 = (u8 *)*(s32 *)(rec7 + 80);
        rp = (u8 *)*(s32 *)(record + 80);
        c12 = 12 & rp[9];
        old = p4[9];
        mask = -13;
        mask &= old;
        p4[9] = (mask | c12);
    } else {
        L_020037ae:;
        if (Value1(Func_02007386, 0x302) == 0) {
            if (*(s16 *)(rec7 + 10) <= 245) {
                if ((*(volatile s32 *)0x03001e40 & 1) == 0) {
                    if (Value1(Func_020073a4, 0x202) == 0) {
                        Call1(Func_02007540, -1);
                        Func_0200755e(230);
                        Call1(Func_020073c4, 0x202);
                    }
                    Func_02007056(*(volatile s32 *)(rec7 + 8), *(volatile s32 *)(rec7 + 12), *(volatile s32 *)(rec7 + 16));
                }
            }
        }
    }
    if (Value1(Func_020073cc, 0x303) == 0) {
        if (*(s16 *)(rec8 + 10) <= 0x2c5) {
            if ((*(volatile s32 *)0x03001e40 & 1) == 0) {
                if (Value1(Func_020073ec, 0x203) == 0) {
                    Call1(Func_02007588, -1);
                    Func_020075a6(230);
                    Call1(Func_0200740c, 0x203);
                }
                Func_0200709e(*(volatile s32 *)(rec8 + 8), *(volatile s32 *)(rec8 + 12), *(volatile s32 *)(rec8 + 16));
            }
        }
    }
}

