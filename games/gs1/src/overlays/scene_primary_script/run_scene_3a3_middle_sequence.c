#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02001792();
s32 Func_0200179c();
void Func_020017ac();
void Func_020017c4();
s32 Func_020017ce();
s32 Func_020017d4();
s32 Func_020017dc();
void Func_020017fa();
s32 Func_02001806();
s32 Func_02001812();
void Func_02001832();
void Func_02001836();
void Func_02001842();
s32 Func_02001866();
void Func_02001878();
void Func_0200187e();
void Func_02001884();
void Func_0200189c();
void Func_020018a2();
void Func_020018a4();
void Func_020018a8();
void Func_020018b8();
s32 Func_020018c2();
void Func_020018d0();
void Func_02001900();
void Func_02001906();
void Func_02001918();
u8 *Func_0200191a();
void Func_02001924();
s32 Func_0200193e();
void Func_02001944();
s32 Func_0200194e();
void Func_02001952();
u8 *Func_0200195c();
s32 Func_0200196a();
s32 Func_0200197a();
void Func_02001980();
u8 *Func_02001988();
s32 Func_02001990();
void Func_02001992();
void Func_02001998();
void Func_020019ae();
s32 Func_02001a30();
void Func_02001a38();
s32 Func_02001a40();
void Func_02001a6e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunMiddleSequence(void)
{
    s32 scene;
    s32 rec5;
    s32 rec6;
    s32 rec0;
    s32 kind;
    s32 bit;

    scene = Value1(Func_020017d4, 0);
    rec5 = Value1(Func_0200179c, 0x242);
    if (rec5 != 0) {
        Func_02001792(64, 32, 0, 32, 32, 32);
        Call6(Func_020017ac, 64, 32, 32, 32, 0, 0);
        kind = 20;
    } else {
        rec6 = Value1(Func_020017ce, 0x241);
        if (rec6 != 0) {
            Func_020017c4(64, 0, 0, 32, 32, 32);
            Value6(Func_020017dc, 64, 0, 32, 32, rec5, rec5);
            Func_02001842(17);
            kind = 20;
        } else {
            rec0 = Value1(Func_02001806, 0x240);
            if (rec0 == 0) {
                goto L_020009b8;
            }
            Func_020017fa(0, 64, 0, 32, 32, 32);
            Value6(Func_02001812, 0, 64, 32, 32, rec6, rec6);
            Func_02001878(16);
            kind = 17;
        }
    }
    Func_0200187e(kind);
    Func_02001884(21);
    goto L_020009da;
    L_020009b8:;
    Call6(Func_02001836, 0, 32, 32, 32, rec0, rec0);
    Func_0200189c(15);
    Func_020018a2(16);
    Func_020018a8(17);
    L_020009da:;
    if (Value1(Func_02001866, 0x8ff) != 0) {
        Func_020018b8(18);
    } else {
        Func_02001998(170);
        Func_02001918(18, 2);
        Func_02001900(18, 3);
        Call2(Func_02001832, 0x2008d09, 0xc80);
    }
    if (Data_02000240_t[225][0] == 3) {
        Call1(Func_020018b8, 0x12f);
    }
    Call6(Func_020018a4, 0, 33, 4, 3, 20, 41);
    if (Value1(Func_020018c2, 0x906) != 0) {
        Call3(Func_02001944, 19, 0x1680000, 0xa80000);
    }
    Func_020018d0((s32)Func_0200191a(19), 0);
    Func_02001980(22, 15);
    Call2((void (*)())Func_02001988, 23, 15);
    bit = 8;
    Func_02001990(24, 15);
    *(u8 *)(Func_0200193e(22) + 89) |= bit;
    *(u8 *)(Func_0200194e(23) + 89) |= bit;
    {
        u8 *record = Func_0200195c(24);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | bit);
    }
    bit = 2;
    *(u8 *)(Func_0200196a(22) + 35) |= bit;
    *(u8 *)(Func_0200197a(23) + 35) |= bit;
    {
        u8 *record = Func_02001988(24);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | bit);
    }
    Func_02001a30(22, 1);
    Func_02001a38(23, 1);
    Func_02001a40(24, 1);
    Func_02001906(1);
    Func_02001992();
    Func_02001a6e(*(s32 *)(scene + 8), *(s32 *)(scene + 12), *(s32 *)(scene + 16), 0);
    Func_02001952();
    Func_020019ae();
    Func_02001924(1);
}
