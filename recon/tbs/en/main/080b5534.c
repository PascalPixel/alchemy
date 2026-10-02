/* DRAFT (score 240), rewritten 2026-10-02 in plain C: 7 of 187 instructions
 * differ, in two places.
 * 1. The ROM clears the message index (movs r5, #0) straight after the sound
 *    call, before the display-control store; here the scheduler lets it sink
 *    to the end of that block, wherever the statement is written.
 * 2. The ROM loads each window's first message into r3 and adds the index
 *    into r0; here the message is loaded into r0 itself.
 * Settled: the two loops are plain for (;;) loops (the compiler moves the
 * part after the last exit test to the top); the range is written out in
 * the clamp, not kept in a variable; the name is stored through the unit
 * structure, which is what lets the copy loop test the value it loaded; the
 * display-control value goes through an int; the flag is a pointer and a
 * value set before the loop and again at its end.
 * The sample text 0x903 still needs its message name. */
#include "TYPES.H"
#include "BATTLE_UNIT.H"

extern volatile s32 gKeysRepeat;
extern u8 *gWindowWork;
extern u8 gCell[];
extern u8 MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely;
extern u8 MsgRariberoHowDidSearchForSheba;
extern u8 MsgBecameCursed;

void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
void FarCall_WindowTable(void);
void UiWork_ClearValueNameTablesFar(void);
void UiText_DrawQuantity(s32, s32);
s32 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void UiWork_DrainPendingFar(s32);
void UiWork_FinalizeFar(s32, s32);
struct BattleUnit *Owner_GetStateFar(s32);
void AudioCommand_PlayFar(s32);
void WaitFrames(s32);

void DebugBattle_ViewMessages(void)
{
    u16 text[64];
    s32 second = 0;
    struct BattleUnit *unit = Owner_GetStateFar(0);
    s32 i;
    s32 index;
    s32 window;
    u8 *flag;
    s32 value;

    Ui_AdjustValueWithoutLimitFar(0x903, text);
    for (i = 0; i <= 13; i++) {
        unit->name[i] = text[i];
        if (text[i] == 0)
            break;
    }
    unit->name[14] = 0;
    FarCall_WindowTable();
    AudioCommand_PlayFar(71);
    index = 0;
    {
        s32 control = 0x1341;

        *(volatile u16 *)0x04000000 = control;
    }
    flag = gCell;
    flag += 0x20c;
    value = 2;
    for (;;) {
        *flag = value;
        UiWork_ClearValueNameTablesFar();
        UiText_DrawQuantity(999, 5);
        UiText_DrawQuantity(0, 3);
        UiText_DrawQuantity(1, 1);
        UiText_DrawQuantity(1, 2);
        UiText_DrawQuantity(2, 4);
        if (second == 0)
            window = UiText_OpenMessageWindowFar(index + (s32)&MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely, 2, 10, 4);
        else
            window = UiText_OpenMessageWindowFar(index + (s32)&MsgBecameCursed, 2, 2, 4);
        WaitFrames(10);
        for (;;) {
            if (gKeysRepeat & 2) {
                if (second != 0) {
                    second = 0;
                } else {
                    index++;
                    second = 1;
                }
            }
            if (gKeysRepeat & 0x10)
                index++;
            if (gKeysRepeat & 0x20)
                index -= 2;
            if (gKeysRepeat & 0x40)
                second = 1;
            if (gKeysRepeat & 0x80)
                second = 0;
            if (gKeysRepeat & 0x100)
                index += 10;
            if (gKeysRepeat & 0x200)
                index -= 10;
            if (index < 0)
                index = 0;
            if ((u32)index >= (u32)((s32)&MsgRariberoHowDidSearchForSheba - (s32)&MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely + 5))
                index = (s32)&MsgRariberoHowDidSearchForSheba - (s32)&MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely + 5;
            if (gKeysRepeat & 0x3f2)
                break;
            if (UiWork_IsCompleteFar() != 0 && (gKeysRepeat & 1))
                break;
            WaitFrames(1);
        }
        UiWork_DrainPendingFar(1);
        UiWork_FinalizeFar(window, 1);
        flag = &gWindowWork[0x12f8];
        value = 0;
    }
}
