#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunActorCoordinateDialogueSequence:
 * 66 calls, one bounded dialogue loop, the actor-coordinate gate, and the
 * complete presentation sequence across the callable owner. */

#define FieldScene_RunActorCoordinateDialogueSequence Func_02004a08

void Func_02004d70();
void Func_02004d78();
void Func_02004e6a();
void Func_0200a4da();
void Func_0200a4e0();
void *Func_0200a4e4();
void *Func_0200a50c();
void *Func_0200a51a();
void *Func_0200a520();
void Func_0200a52e();
void Func_0200a550();
void Func_0200a55e();
s32 Func_0200a566();
void Func_0200a576();
void Func_0200a588();
void *Func_0200a58e();
void *Func_0200a59c();
void *Func_0200a5a8();
void Func_0200a5aa();
void Func_0200a5b6();
s32 Func_0200a5be();
void Func_0200a5c0();
void Func_0200a5ce();
void *Func_0200a5dc();
void *Func_0200a5de();
void Func_0200a5f8();
s32 Func_0200a604();
void Func_0200a60c();
void Func_0200a610();
void Func_0200a616();
void Func_0200a628();
void Func_0200a638();
void Func_0200a648();
void Func_0200a650();
void Func_0200a65c();
void Func_0200a660();
void Func_0200a662();
void Func_0200a666();
void Func_0200a66a();
void Func_0200a66e();
void Func_0200a67c();
void Func_0200a68e();
void Func_0200a694();
void Func_0200a69e();
void Func_0200a6a4();
void Func_0200a6b0();
void Func_0200a6b4();
void Func_0200a6be();
void Func_0200a6c6();
void Func_0200a6ca();
void Func_0200a6d0();
u8 *Func_0200a6d2();
void Func_0200a6ea();
void Func_0200a6ec();
void Func_0200a6f0();
void Func_0200a6f6();
void Func_0200a6fa();
void Func_0200a702();
void Func_0200a704();
void Func_0200a708();
void Func_0200a718();
void Func_0200a724();
void Func_0200a732();
void Func_0200a744();
void Func_0200a754();
void *Func_0200a760();
void Func_0200a764();
void *Func_0200a76a();
void *Func_0200a77a();
void *Func_0200a782();
void *Func_0200a78c();
void *Func_0200a7a4();
void Func_0200a7ac();
void *Func_0200a7b0();
void *Func_0200a7b8();
void *Func_0200a7ec();
void Func_0200a7ee();
void *Func_0200a7f8();
void *Func_0200a800();
void *Func_0200a80a();
void *Func_0200a816();
void *Func_0200a81e();
void Func_0200a846();
void *Func_0200a860();
void Func_0200a862();
void Func_0200a866();
void *Func_0200a86c();
void *Func_0200a878();
void *Func_0200a880();
void Func_0200a888();
void *Func_0200a89e();
void *Func_0200a8be();
void Func_0200a8c8();
void *Func_0200a8ca();
void Func_0200a8d0();
void *Func_0200a8dc();
void Func_0200a8e8();
void Func_0200a8ea();
void Func_0200a8fc();
void Func_0200a8fe();
void Func_0200a900();
void Func_0200a902();
void Func_0200a908();
void Func_0200a910();
void *Func_0200a914();
void Func_0200a91c();
void Func_0200a94a();
void *Func_0200a964();
void Func_0200a96e();
void *Func_0200a972();
void *Func_0200a97a();
void Func_0200a9a6();
void *Func_0200a9d4();
void Func_0200a9f4();
void Func_0200aa18();
void Func_0200aa66();
void Func_0200aaca();

void Func_02004a08(void)
{
    s32 base;
    s32 position;
    s32 event;
    u8 *actor;
    void *p10;
    void *p100;
    void *p104;
    void *p105;
    void *p107;
    void *p11;
    void *p114;
    void *p12;
    void *p120;
    void *p121;
    void *p122;
    void *p13;
    void *p130;
    void *p6;
    void *p67;
    void *p68;
    void *p69;
    void *p7;
    void *p70;
    void *p71;
    void *p73;
    void *p74;
    void *p75;
    void *p8;
    void *p83;
    void *p84;
    void *p85;
    void *p86;
    void *p87;
    void *p88;
    void *p9;
    void *p94;
    void *p95;
    void *p96;
    void *p97;

    actor = Func_0200a4e4(8);
    position = *(s32 *)(actor + 8);
    if (position < 0) {
        position += 0xfffff;
    }
    if ((position >> 20) != 48) {
        return;
    }
    Func_0200a4da();
    base = 5521;
    Func_0200a5aa(base);
    Func_0200a4e0(20);
    Func_0200a588(3, 1);
    p6 = Func_0200a5dc(0, 32768, 20);
    p7 = Func_0200a5de(3, 0, 20);
    p8 = Func_0200a58e(3, 3);
    p9 = Func_0200a50c(20);
    p10 = Func_0200a59c(0, 3);
    p11 = Func_0200a51a(20);
    p12 = Func_0200a520(60);
    p13 = Func_0200a5a8(3, 16);
    Func_0200a52e(50);
    Func_0200a5b6(3, 1);
    Func_0200a616(3, 0);
    if (Func_0200a566(0, 0) == 1) {
        Func_0200a550(20);
        Func_0200a5f8(3, 2);
        Func_0200a55e(20);
        Func_0200a648(3, 0, 20);
        Func_0200a5f8(3, 4);
        Func_0200a576(20);
        Func_0200a660(3, 0, 20);
        Func_0200a610(3, 3);
        Func_0200a58e(20);
        Func_0200a66e(3, 0);
        if (Func_0200a5be(0, 0) == 1) {
            Func_0200a5a8(20);
            Func_0200a638(3, 4);
            Func_0200a5b6(20);
            event = base + 5;
            for (;;) {
                Func_0200a6a4(event);
                Func_0200a6b4(3, 0);
                if (Func_0200a604(0, 0) != 1) {
                    break;
                }
                Func_0200a5c0(20);
                Func_0200a650(3, 4);
                Func_0200a5ce(20);
                event = 5689;
            }
        }
    }
    Func_0200a6be(5527);
    Func_0200a628(3, 52428, 26214);
    Func_0200a666(3, 728, 632);
    Func_0200a60c(20);
    Func_0200a6f6(3, 0, 20);
    Func_0200a69e(3, 16);
    Func_0200a708(3, 0, 20);
    Func_0200a6b0(3, 1);
    Func_0200a6ea(3, 0, 20);
    Func_0200a6ca(3, 4);
    Func_0200a648(20);
    Func_0200a732(3, 0, 20);
    Func_0200a754(3, 261, 90);
    Func_0200a6ec(3, 3);
    Func_0200a66a(20);
    Func_0200a754(3, 0, 20);
    Func_0200a694(3, 1);
    Func_0200a662(68);
    Func_0200a67c(3, 1, 0);
    Func_0200a68e(3, 1, 0);
    Func_0200a65c(3);
    Func_0200a724(3, 2);
    actor = Func_0200a6d2(0);
    if (actor != 0) {
        Func_0200a704(3, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_0200a732(3);
    Func_0200a744(3, 0, 0);
    Func_0200a650(110, 39, 5, 1, 46, 39);
    Func_0200a6c6(2163);
    Func_0200a6fa();
}
