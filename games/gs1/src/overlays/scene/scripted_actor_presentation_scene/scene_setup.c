#include "types.h"

#define FieldScene_InitActorsAndDispatchBySubstate Func_0200071c
#define FieldScene_RunSetupSequence35c4 Func_020035c4
#define FieldScene_RunThreeStepsInBracket Func_020038c0
#define FieldScene_RunBracketedSceneWithFlag282 Func_020038dc
typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[8];
    s32 unk18;
    u8 filler1c[7];
    u8 unk23;
    u8 filler24[0x31];
    u8 unk55;
} Spr;

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

void Func_02006438(s32);
void Func_02006438_a(Spr *, s32);
void Func_02006326(s32);
void Func_02006446(s32);
Spr *Func_02006484(s32);
void Func_020063ea(Spr *, s32);
Spr *Func_02006490(s32);
void Func_020063f6(Spr *, s32);
Spr *Func_0200649c(s32);
void Func_02006402(Spr *, s32);
Spr *Func_020064a8(s32);
void Func_0200640e(Spr *, s32);
Spr *Func_020064b4(s32);
Spr *Func_020064be(s32);
Spr *Func_020064ca(s32);
Spr *Func_020064d2(s32);
void Func_020065b0(s32, s32);
s32 Func_020064f8(s32);
void Func_020010a8(void);
void Func_02002b5a(void);
void Func_020040c0(void);
void Func_020040e2(void);
void Func_02004130(void);
void Func_02006546(void);
s32 Func_02006524(s32);
s32 Func_02006532(s32);
s32 Func_02006540(s32);
void Func_02006534(s32, s32);
void Func_0200653e(s32, s32);
void Func_020066ac(s32);
s32 Func_02006562(s32);
s32 Func_02000d1a(void);
Spr *Func_020065b2(void);
void Func_02009496(s32);
void Func_0200944e(s32, s32);
void Func_02009464(s32);
void Func_020091da(s32);
void Func_02009462(s32, s32);
void Func_02009478(s32);
void Func_020091ee(s32);
void Func_020095f8(void);
void Func_02009610(void);
void Func_02007490();
void Func_02007768();
void Func_02007b0c();

/*
 * resource_3c9: a scripted-scene bracket around three setup steps.
 */

/* The three inner steps are old-style: their signatures are not established. */

/*
 * One symbol per call site, spelled as the raw pc-relative decode of the
 * reference halfwords rather than the semantic import address. Listed in
 * call order.
 */

void Func_02009614();  /* Func_0808a018 */

void Func_020081bc();  /* Func_020048d8, this overlay's own */

void Func_02008410();  /* Func_02004b28, this overlay's own */

void Func_02009604();  /* GameFlag_Set */

void Func_020097a6();  /* Func_0808a368 */

void Func_020097b2();  /* Func_0808a370 */

void Func_02009770();  /* Func_0808a248 */

void Func_02009654();  /* Func_0808a020 */

s32 FieldScene_InitActorsAndDispatchBySubstate(void)
{
    s32 v;
    u32 i;
    s32 z;
    s16 mode;
    Spr *obj;

    Func_02006438(324);
    Func_02006326(1);
    Func_02006446(272);
    Func_020063ea(Func_02006484(8), 0);
    Func_020063f6(Func_02006490(9), 0);
    Func_02006402(Func_0200649c(10), 0);
    Func_0200640e(Func_020064a8(11), 0);
    v = (s32)0xffff0000;
    Func_020064b4(10)->unk18 = v;
    Func_020064be(11)->unk18 = v;
    i = 12;
    z = 0;
    do {
        obj = Func_020064ca(i);
        Func_02006438_a(Func_020064d2(i), z);
        Func_020065b0(i, 1);
        obj->unk55 = 4;
        obj->unk23 |= 2;
        obj->unk0c = 0x8000;
        i++;
    } while (i <= 17);

    mode = Data_02000240[225];

    switch (mode) {
    case 1:
        if (Func_020064f8(0x109) == 0) {
            Func_020010a8();
        }
        break;
    case 2:
        Func_02002b5a();
        break;
    case 3:
        Func_020040c0();
        break;
    case 0x5d:
        Func_020040e2();
        break;
    case 4:
        Func_02004130();
        break;
    case 9:
        Func_02006546();
        if (Func_02006524(0x345) != 0) {
            Func_02006534(0, 65);
        } else if (Func_02006532(0x346) != 0) {
            Func_02006534(1, 65);
        } else if (Func_02006540(0x347) != 0) {
            Func_02006534(2, 65);
        } else {
            Func_0200653e(3, 65);
        }
        Func_020066ac(9);
        break;
    }

    if (Func_02006562(0x109) != 0) {
        if (Func_02000d1a() != 0) {
            Spr *p = Func_020065b2();
            if (p != 0) {
                p->unk55 = 0;
            }
        }
    }
    return z;
}

void FieldScene_RunSetupSequence35c4(void)
{
    Func_02009496(187);
    Func_0200944e(0x7fff, 1);
    Func_02009464(1);
    Func_020091da(4);
    Func_02009462(0x40250d, 1);
    Func_02009478(1);
    Func_020091ee(1);
}

void FieldScene_RunThreeStepsInBracket(void)
{
    Func_020095f8();
    Func_02007490();
    Func_02007768();
    Func_02007b0c();
    Func_02009610();
}

/*
 * Brackets a scripted scene: opens it, runs two of this overlay's own
 * steps, sets the story flag, writes the workspace phase and timer, then
 * closes. The 72-byte owner includes its alignment halfword and one pool
 * word. No incoming argument is read before being overwritten, so this is
 * void.
 */
void FieldScene_RunBracketedSceneWithFlag282(void)
{
    u8 *workspace;

    Func_02009614();
    Func_020081bc();
    Func_02008410();
    /* The flag id is built as 141 << 1 rather than folded. */
    Func_02009604(141 << 1);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 512;
    *(s32 *)(workspace + 456) = 24;

    Func_020097a6();
    Func_020097b2();
    Func_02009770(1);
    Func_02009654();
}
