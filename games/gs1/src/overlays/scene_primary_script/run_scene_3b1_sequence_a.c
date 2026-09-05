#include "types.h"

#define FieldScene_RunScene3b1SequenceA Func_02000b84

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001d2e();
s32 Func_02001e74();
void Func_02005416();
void Func_02005476();
void Func_02005504();
s32 Func_02007030();
void Func_02007050();
s32 Func_020070a0();
void Func_020070aa();
s32 Func_020070e0();
void Func_020070e2();
s32 Func_020070e8();
void Func_020070ea();
s32 Func_020070fa();
s32 Func_02007100();
void Func_02007102();
s32 Func_0200710c();
void Func_02007110();
void Func_0200711e();
void Func_02007122();
void Func_02007128();
void Func_02007142();
void Func_0200714c();
void Func_02007162();
void Func_02007164();
void Func_0200716c();
void Func_02007170();
void Func_02007182();
void Func_02007198();
s32 Func_020071a8();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b1SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Func_02007050();
    if (Value1(Func_02007030, 0x300) != 0) {
        rec7 = Value0(Func_02001e74);
        Func_02001d2e();
        Call1(Func_0200714c, 0x1ea1);
        Func_02005416(12);
        Func_02007102(rec7, 2);
        record = Value1(Func_020070a0, 0);
        if (record != 0) {
            Func_020070e2(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02007110(rec7);
        Func_02007122(rec7, 0, 0);
    } else {
        Func_02007164(12, 2);
        Func_020070aa(20);
        Call1(Func_02007198, 0x1e81);
        Value2(Func_020071a8, 12, 0);
        if (Value2(Func_020070e0, 0, 0) == 0) {
            Func_02005476(12);
            Func_02007162(12, 2);
            record = Value1(Func_02007100, 0);
            if (record != 0) {
                Func_02007142(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_02007170(12);
            Func_02007182(12, 0, 0);
            Call1(Func_020070ea, 0x300);
            if (Value1(Func_020070e8, 0x92b) != 0) {
                Call1(Func_020070fa, 0x994);
                goto L_02000c9a;
            }
            if (Value1(Func_020070fa, 0x92a) != 0) {
                Call1(Func_0200710c, 0x91b);
                goto L_02000c9a;
            }
            if (Value1(Func_0200710c, 0x929) != 0) {
                Call1(Func_0200711e, 0x939);
                goto L_02000c9a;
            }
            Call1(Func_02007128, 0x930);
        } else {
            bump_step(1);
            Func_02005504(12);
        }
    }
    L_02000c9a:;
    Func_0200716c();
}
