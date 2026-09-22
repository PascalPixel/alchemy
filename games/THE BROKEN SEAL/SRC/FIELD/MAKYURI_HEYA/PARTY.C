#include "MAKYURI_HEYA.H"

extern u8 LinkedMessage_MercuryDialogue;

/* Mia thanks the party, asks why they entered the lighthouse, and joins them. */

#define FieldScene_RunActorCoordinateDialogueSequence Func_02004a08

void Func_0200a4da();
void Func_0200a4e0();
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
void *Func_0200a58e_a();
void *Func_0200a59c();
void *Func_0200a5a8();
void *Func_0200a5a8_a();
void Func_0200a5aa();
void Func_0200a5b6();
void Func_0200a5b6_a();
s32 Func_0200a5be();
void Func_0200a5c0();
void Func_0200a5ce();
void Func_0200a5f8();
void Func_0200a5f8_a();
s32 Func_0200a604();
void Func_0200a60c();
void Func_0200a610();
s32 Func_0200a616();
s32 Func_0200a638();
void Func_0200a648();
void Func_0200a648_a();
s32 Func_0200a650();
void Func_0200a65c();
void Func_0200a660();
void Func_0200a662();
void Func_0200a666();
void Func_0200a66a();
s32 Func_0200a66e();
s32 Func_0200a67c();
void Func_0200a68e();
void Func_0200a694();
void Func_0200a69e();
void Func_0200a6a4();
void Func_0200a6b0();
s32 Func_0200a6b4();
void Func_0200a6be();
void Func_0200a6c6();
void Func_0200a6ca();
u8 *Func_0200a6d2();
void Func_0200a6ea();
void Func_0200a6ec();
void Func_0200a6f6();
void Func_0200a6fa();
void Func_0200a704();
void Func_0200a708();
void Func_0200a724();
void Func_0200a732();
void Func_0200a732_a();
void Func_0200a744();
void Func_0200a754_a();

void Func_02004a08(void)
{
    s32 base;
    s32 position;
    s32 event;
    u8 *actor;

    position = *(s32 *)((u8 *)Actor_Get(8) + 8) / 0x100000;
    if (position != 48) {
        return;
    }
    Func_0200a4da();
    base = (s32)&LinkedMessage_MercuryDialogue;
    Func_0200a5aa(base);
    Func_0200a4e0(20);
    Func_0200a588(3, 1);
    Actor_TurnToAngle(0, 32768, 20);
    Event_ShowMessageAndWait(3, 0, 20);
    Func_0200a58e(3, 3);
    Func_0200a50c(20);
    Func_0200a59c(0, 3);
    Func_0200a51a(20);
    Func_0200a520(60);
    Func_0200a5a8(3, 16);
    Func_0200a52e(50);
    Func_0200a5b6(3, 1);
    Func_0200a616(3, 0);
    if (Func_0200a566(0, 0) == 1) {
        Func_0200a550(20);
        Func_0200a5f8(3, 2);
        Func_0200a55e(20);
        Func_0200a648(3, 0, 20);
        Func_0200a5f8_a(3, 4);
        Func_0200a576(20);
        Func_0200a660(3, 0, 20);
        Func_0200a610(3, 3);
        Func_0200a58e_a(20);
        Func_0200a66e(3, 0);
        if (Func_0200a5be(0, 0) == 1) {
            Func_0200a5a8_a(20);
            Func_0200a638(3, 4);
            Func_0200a5b6_a(20);
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
    Actor_SetSpeed(3, 52428, 26214);
    Func_0200a666(3, 728, 632);
    Func_0200a60c(20);
    Func_0200a6f6(3, 0, 20);
    Func_0200a69e(3, 16);
    Func_0200a708(3, 0, 20);
    Func_0200a6b0(3, 1);
    Func_0200a6ea(3, 0, 20);
    Func_0200a6ca(3, 4);
    Func_0200a648_a(20);
    Func_0200a732(3, 0, 20);
    Actor_ShowEmote(3, 261, 90);
    Func_0200a6ec(3, 3);
    Func_0200a66a(20);
    Func_0200a754_a(3, 0, 20);
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
    Func_0200a732_a(3);
    Func_0200a744(3, 0, 0);
    Map_CopyCellAttributes(110, 39, 5, 1, 46, 39);
    Func_0200a6c6(2163);
    Func_0200a6fa();
}
