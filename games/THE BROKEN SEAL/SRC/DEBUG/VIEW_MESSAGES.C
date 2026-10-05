#include "PROTO.H"
#include "EDITION.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "BATTLE_UNIT.H"
#include "GAME_STATE.H"
#include "BATTLE_RUNTIME.H"
#include "text/MSG_IDS.H"

#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_FR)

#if EDITION_INTERNATIONAL
#define WINDOW_WORK_MESSAGE_BUSY 0x12f8
#else
#define WINDOW_WORK_MESSAGE_BUSY 0x1188
#endif

extern volatile s32 gKeysRepeat;
extern u8 *gWindowWork;

#if defined(TBS_EDITION_EN)
#define DEBUG_SECOND_FORM ((s32)MsgBecameCursed)
#else
#define DEBUG_SECOND_FORM ((s32)MsgDebugSecondForm)
#endif

#define DEBUG_VIEW_LIMIT \
    ((s32)MsgRariberoHowDidSearchForSheba - (s32)MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely + 5)

void UiText_DrawQuantity(s32, s32);
s32 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizeFar(s32, s32);
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
    s32 message;

    Ui_AdjustValueWithoutLimitFar((s32)MsgDebugSampleName, text);
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

            REG_DISPCNT = control;
        }
        gGameState.message_speed = 2;
        for (;;) {
            UiWork_ClearValueNameTablesFar();
            UiText_DrawQuantity(999, 5);
            UiText_DrawQuantity(0, 3);
            UiText_DrawQuantity(1, 1);
            UiText_DrawQuantity(1, 2);
            UiText_DrawQuantity(2, 4);
            if (second == 0) {
                message = (s32)MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely;
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
                if (index >= (u32)DEBUG_VIEW_LIMIT)
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
