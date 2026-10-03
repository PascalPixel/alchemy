#include "EDITION.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "BATTLE_UNIT.H"

#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN) || defined(TBS_EDITION_IT)

/* The byte in the window work the viewer clears after each message; the
 * localised window work is larger. */
#if EDITION_INTERNATIONAL
#define WINDOW_WORK_MESSAGE_BUSY 0x12f8
#else
#define WINDOW_WORK_MESSAGE_BUSY 0x1188
#endif

extern volatile s32 gKeysRepeat;
extern u8 *gWindowWork;
extern u8 gCell[];
extern u8 MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely;
extern u8 MsgRariberoHowDidSearchForSheba;

/* Where the second form starts: the first message after the menu texts in
 * the Japanese catalogue, which the English catalogue names, and in the
 * Italian one the empty message after the sample name. */
#if defined(TBS_EDITION_EN)
extern u8 MsgBecameCursed;
#define DEBUG_SECOND_FORM ((s32)&MsgBecameCursed)
#else
extern u8 MsgDebugSecondForm;
#define DEBUG_SECOND_FORM ((s32)&MsgDebugSecondForm)
#endif
extern u8 MsgDebugSampleName;

/* The furthest the viewer steps from a form's first message. */
#define DEBUG_VIEW_LIMIT \
    ((s32)&MsgRariberoHowDidSearchForSheba - (s32)&MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely + 5)

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

/* The debug message viewer: names the first party member with the sample
 * text, then shows one message at a time in a window, the first form at the
 * right and the second at the left. Right and left step through the
 * messages, L and R jump ten, up and down choose the form, B steps through
 * both forms. It never returns. */
void DebugBattle_ViewMessages(void)
{
    u16 text[64];
    s32 second = 0;
    struct BattleUnit *unit = Owner_GetStateFar(0);
    s32 i;
    s32 index;
    s32 window;
    s32 message;

    Ui_AdjustValueWithoutLimitFar((s32)&MsgDebugSampleName, text);
    for (i = 0; i <= 13; i++) {
        unit->name[i] = text[i];
        if (text[i] == 0)
            break;
    }
    unit->name[14] = 0;
    FarCall_WindowTable();
    AudioCommand_PlayFar(71);
    for (;;) {
        index = 0;
        {
            s32 control = 0x1341;

            *(volatile u16 *)0x04000000 = control;
        }
        gCell[0x20c] = 2;
        for (;;) {
            UiWork_ClearValueNameTablesFar();
            UiText_DrawQuantity(999, 5);
            UiText_DrawQuantity(0, 3);
            UiText_DrawQuantity(1, 1);
            UiText_DrawQuantity(1, 2);
            UiText_DrawQuantity(2, 4);
            if (second == 0) {
                message = (s32)&MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely;
                window = UiText_OpenMessageWindowFar(index + message, 2, 10, 4);
            } else {
                message = DEBUG_SECOND_FORM;
                window = UiText_OpenMessageWindowFar(index + message, 2, 2, 4);
            }
            WaitFrames(10);
            for (;;) {
                if (gKeysRepeat & KEY_B) {
                    if (second != 0) {
                        second = 0;
                    } else {
                        index++;
                        second = 1;
                    }
                }
                if (gKeysRepeat & KEY_RIGHT)
                    index++;
                if (gKeysRepeat & KEY_LEFT)
                    index -= 2;
                if (gKeysRepeat & KEY_UP)
                    second = 1;
                if (gKeysRepeat & KEY_DOWN)
                    second = 0;
                if (gKeysRepeat & KEY_R)
                    index += 10;
                if (gKeysRepeat & KEY_L)
                    index -= 10;
                if (index < 0)
                    index = 0;
                if ((u32)index >= (u32)DEBUG_VIEW_LIMIT)
                    index = DEBUG_VIEW_LIMIT;
                if (gKeysRepeat & (KEY_B | KEYS_DPAD | KEYS_SHOULDERS))
                    break;
                if (UiWork_IsCompleteFar() != 0 && (gKeysRepeat & KEY_A))
                    break;
                WaitFrames(1);
            }
            UiWork_DrainPendingFar(1);
            UiWork_FinalizeFar(window, 1);
            gWindowWork[WINDOW_WORK_MESSAGE_BUSY] = 0;
        }
    }
}

#endif
