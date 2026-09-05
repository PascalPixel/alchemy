#include "types.h"

#define FieldScene_RunSecondArrivalSequence Func_02001df8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_0200469a();
s32 Func_020048b0();
void Func_02004aaa();
s32 Func_02004b24();
s32 Func_020054a8();
void Func_02005766();
s32 Func_02005960();
void Func_020059ce();
void Func_020059de();
void Func_020059ee();
s32 Func_02005a00();
void Func_02005a0c();
void Func_02005a5c();
void Func_02006814();
void Func_0200684c();
void Func_0200688a();
void Func_0200689e();
void Func_020068da();
void Func_02006910();
void Func_0200691e();
void Func_02006920();
void Func_0200692e();
void Func_02006930();
void Func_02006948();
void Func_02006956();
s32 Func_02006984();
s32 Func_020069a4();
void Func_020069a8();
void Func_020069be();
void Func_020069c6();
void Func_020069ca();
void Func_020069e8();
void Func_020069ea();
void Func_02006a00();
void Func_02006a08();
void Func_02006a16();
void Func_02006a2e();
void Func_02006a42();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSecondArrivalSequence(s32 a0)
{
    u32 i;
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_0200469a();
    } else {
        Func_02006814();
        rec = Value2(Func_020048b0, a0, 2);
        if (rec != 0) {
        } else {
            Call1(Func_020068da, 0x20b7);
            Call2(Func_0200692e, 0x30000, 0x6000);
            Call4(Func_02006948, 0x1780000, -1, 0x980000, 1);
            Func_02006956();
            Func_0200684c(30);
            Func_0200691e(a0, 0);
            Func_020054a8(0, 0x118, 200);
            Call3(Func_0200689e, 0, 0x18000, 0xc000);
            Value3(Func_02005a00, 0, 0x118, 152);
            Call3(Func_02005a0c, 0, 0x128, 152);
            Func_0200688a(10);
            Func_020069c6();
            Call4(Func_020069a8, -1, -1, -1, 0);
            Value3(Func_02006984, 0, 0xc000, 15);
            Func_020069e8();
            Call4(Func_020069ca, -1, -1, -1, 0);
            Value3(Func_020069a4, 0, 0, 15);
            Func_02006a08();
            Call4(Func_020069ea, -1, -1, -1, 0);
            Call3(Func_020069c6, 0, 0x4000, 15);
            Func_020069be(a0, 0);
            Value3(Func_02005960, 96, 40, 0);
            Func_020059ce(128, 40, 10);
            Func_02006910(30);
            Func_020059de(160, 40, 10);
            Func_02006920(30);
            Func_020059ee(160, 72, 10);
            Func_02006930(30);
            Func_02006a00(a0, 0);
            Func_02005a5c();
            Func_02005766(0);
            Func_02006a42(0, 0);
            Func_02004aaa(a0, 2);
            goto L_02001f70;
        }
        if (rec == 1) {
            Call1(Func_02006a16, 0x20b6);
            Func_02006a2e(a0, 0);
        }
        L_02001f70:;
        Value3(Func_02004b24, rec, a0, 2);
        ((void (*)())Func_02006984)();
    }
}
