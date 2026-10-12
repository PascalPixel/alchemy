#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "input.h"
#include "object_runtime.h"
#include "party_state.h"
#include "text/msg_ids.h"

void Func_020019e4(s32 value, s32 digits);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
s32 Func_02001a74(s32 actor, s32 flags);
void Func_02001ab4(s32 skip);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
s32 UiWindow_Create();
void RenderOutput_PrepareForRedraw();
void UiText_DrawNumber();
s32 DebugParty_LoadPreset();
void UiWork_ClearValueNameTables(void);

s32 Func_02000e4c(s32 id)
{
    s32 window;
    s32 choice;
    s32 shown;

    Func_02001a14();
    Func_02001ab4(0);
    ObjectMotion_SetAngleToward(id, gPartyState.current_owner, 0);
    Func_02001a64((s32)MsgChooseYourParameters);
    Func_02001a74(id, 0);
    window = UiWindow_Create(0, 0, 6, 4, 2);
    choice = 0;
    shown = -1;
    for (;;) {
        if (choice != shown) {
            RenderOutput_PrepareForRedraw(window);
            UiText_DrawNumber(choice, 3, window, 0, 0);
            shown = choice;
        }
        if (gInput.repeat & 0x20) {
            choice--;
        }
        if (gInput.repeat & 0x10) {
            choice++;
        }
        if (choice < 0) {
            choice = 0;
        }
        if (((volatile struct InputState *)&gInput)->pressed & 1) {
            break;
        }
        if (((volatile struct InputState *)&gInput)->pressed & 2) {
            choice = -1;
            break;
        }
        WaitFrames(1);
    }
    UiWork_Finalize(window, 1);
    {
        s32 ok;
        s32 message;

        if (choice >= 0) {
            ok = DebugParty_LoadPreset(choice);
        } else {
            message = (s32)MsgWhatIsThis;
            goto show;
        }
        /* FAKEMATCH: the refusal jumps into the unknown-preset branch so the two share one message call, as the reference lays them out. */
        if (ok != 0) {
            message = (s32)MsgThereSNoSuch;
        show:
            Func_02001a64(message);
            Func_02001a74(9, 0);
        } else {
            Func_02001a64((s32)MsgTheSettingsAre);
            Func_02001a74(9, 0);
        }
    }
    WaitFrames(10);
    Func_02001a1c();
}
