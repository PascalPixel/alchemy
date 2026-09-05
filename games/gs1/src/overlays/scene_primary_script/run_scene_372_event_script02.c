#include "types.h"

#define RunEventScript02 Func_02002dec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02006304();
s32 Func_020074d0();
s32 Func_020074dc();
void Func_02007506();
void Func_02007554();
void Func_02007562();
void Func_02007570();
s32 Func_0200757e();
s32 Func_020075a8();
void Func_020075c8();
void Func_020075e6();
s32 Func_020075ea();
void Func_020075ec();
s32 Func_02007610();
void Func_02007616();
s32 Func_0200762c();
void Func_02007638();
void Func_02007672();
void Func_0200768e();
void Func_02007694();
void Func_0200769a();
void Func_0200769c();
void Func_020076b2();
void Func_020076b4();
void Func_020076c8();
void Func_020076d4();
void Func_020076d8();
void Func_020076e0();
void Func_02007706();
void Func_02007720();
void Func_02007728();
void Func_02007740();
void Func_0200774a();
void Func_0200774e();
void Func_02007750();
void Func_02007766();
void Func_0200777a();
void Func_0200778c();
void Func_0200778e();
void Func_020077a6();
void Func_020077b8();
void Func_020077bc();
void Func_020077be();
void Func_020077c4();
void Func_020077ce();
void Func_020077d0();
void Func_020077d8();
void Func_020077e4();
void Func_020077f0();
void Func_02007802();
void Func_02007806();
void Func_0200780a();
void Func_0200781a();
void Func_0200781c();
void Func_02007820();
void Func_02007822();
void Func_0200782a();
void Func_02007834();
void Func_02007842();
void Func_02007844();
void Func_02007854();
void Func_02007856();
void Func_02007866();
void Func_0200786c();
s32 Func_02007876();
void Func_02007886();
void Func_02007890();
void Func_0200789a();
void Func_020078a2();
void Func_020078a6();
void Func_020078ac();
void Func_020078ae();
void Func_020078b6();
void Func_020078b8();
void Func_020078c0();
void Func_020078c8();
void Func_020078d8();
void Func_02007908();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Byte 9 of the linked record is a packed pair of two-bit fields. A
 * bitfield store emits movs #13 / negs; mask arithmetic narrows to
 * movs #243. */
struct Rec9 {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;
    u8 hi : 4;
};

struct Work80 {
    u8 pad00[35];
    u8 flags;
    u8 pad36[44];
    struct Rec9 *child;
};

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunEventScript02(void)
{
    u32 i;
    u8 *rec;
    u8 *record;
    struct Rec9 *p4;

    if (Value1(Func_020074d0, 0x840) == 0) {
    } else {
        if (Value1(Func_020074dc, 0x841) != 0) {
        } else {
            Func_02007506();
            Call3(Func_02007554, 0, 0x10000, 0x8000);
            Call3(Func_02007562, 22, 0x10000, 0x8000);
            Call3(Func_02007570, 26, 0x10000, 0x8000);
            Call3(Func_0200757e, 8, 0x10000, 0x8000);
            Call3(Func_020075c8, 0, 217, 0x557);
            record = Value1(Func_0200757e, 0);
            if (record != 0) {
                Func_020075ec(22, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
            Call3(Func_020075e6, 22, 235, 0x557);
            Call3(Func_02007672, 22, 0xb000, 0);
            record = Value1(Func_020075a8, 0);
            if (record != 0) {
                Func_02007616(26, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
            Call3(Func_02007610, 26, 199, 0x557);
            Call3(Func_0200769c, 26, 0xd000, 0);
            Call3(Func_02007638, 25, 0xf70000, 0x4ba0000);
            Call3(Func_020076b4, 25, 0x6000, 0);
            record = Value1(Func_020075ea, 8);
            p4 = ((struct Work80 *)record)->child;
            ((struct Work80 *)record)->flags &= 254;
            p4->mode = 1;
            rec = Value1(Func_02007610, 0);
            p4 = ((struct Work80 *)rec)->child;
            ((struct Work80 *)rec)->flags &= 254;
            p4->mode = 2;
            record = Value1(Func_0200762c, 0);
            if (record != 0) {
                Func_0200769a(8, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
            Call3(Func_02007694, 8, 221, 0x569);
            Call3(Func_02007720, 8, 0xb000, 60);
            Func_020076e0(26, 2);
            Call1(Func_02007706, 0xec6);
            Func_02007728(26, 0, 40);
            Call3(Func_020076d4, 9, 0x650000, 0x4ad0000);
            Call3(Func_02007750, 9, 0x2000, 0);
            Call3(Func_0200774a, 0x1009, 0, 10);
            Call3(Func_02007766, 26, 0xa000, 0);
            Call2(Func_0200778e, 0x13333, 0x2666);
            Call4(Func_020077a6, 0x650000, -1, 0x4ad0000, 1);
            Call3(Func_020076c8, 9, 0x16666, 0xb333);
            Call2(Func_020076d8, 9, 0x200cd1c);
            Func_0200768e(60);
            Call2(Func_020077be, 0x9999, 0x1333);
            Call4(Func_020077d8, 0xbb0000, -1, 0x5300000, 1);
            Func_020077e4();
            Func_020076b2(40);
            Func_0200777a(26, 2);
            Func_020077bc(26, 0, 20);
            Func_0200778c(9, 2);
            Call3(Func_020077ce, 0x4009, 0, 20);
            Call2(Func_0200780a, 0x20000, 0x4000);
            Call4(Func_02007822, 0xdd0000, -1, 0x5690000, 1);
            Func_020077c4(0, 8, 0);
            Func_020077ce(22, 8, 0);
            Call3(Func_0200781a, 26, 0x3000, 80);
            Call4(Func_02007854, 0xb60000, -1, 0x5500000, 1);
            Call3(Func_020077b8, 8, 182, 0x568);
            Func_02007802(8, 9, 0);
            Func_02007740(30);
            Func_020077f0(8, 3);
            Func_0200774e(10);
            Func_02007820(0, 9, 0);
            Func_0200782a(22, 9, 0);
            Func_02007834(26, 9, 0);
            Func_0200781c(9, 3);
            Func_0200786c(9, 0);
            Func_02007844(26, 2);
            Func_02007886(26, 0, 10);
            Call3(Func_020078a2, 9, 0xe000, 40);
            Call3(Func_020078ae, 9, 0x3000, 20);
            Func_02007856(9, 3);
            Func_020078a6(9, 0);
            Func_02007890(26, 8, 0);
            Func_0200789a(22, 0, 0);
            Func_020077d0(40);
            Func_020078a2(0, 9, 0);
            Func_020078ac(22, 9, 0);
            Func_020078b6(26, 9, 0);
            Func_020078c0(8, 9, 0);
            Func_020078c0(9, 2);
            Func_02007806(20);
            Func_02007908(9, 0, 10);
            Func_020078b8(0, 3);
            Func_020078c0(26, 3);
            Func_020078c8(22, 3);
            Func_020078d8(8, 3);
            rec[35] |= 1;
            record = Value1(Func_02007876, 8);
            record[35] |= 1;
            Func_02006304();
            Call1(Func_02007842, 0x841);
            Func_02007866();
        }
    }
}
