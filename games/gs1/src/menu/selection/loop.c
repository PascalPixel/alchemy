#include "types.h"
#include "scene.h"
#include "sound_ids.h"

/* menu/selection/loop.c */
extern u8 *gIw;
extern u32 gIw2;
extern volatile u32 gIw3;
void WaitFrames(s32);
void Audio_PlayCue(u32);
void FunctionHead_0801b9ec(void *state, u32 index);
void Menu_StepRight(void *state);
void Menu_StepLeft(void *state);
s32 Menu_ConfirmSelection(void *state);
void Menu_Do(void *state);
void Menu_Do2(void *state);

s32 Menu_SelectionLoop(s32 mode)
{
    u8 *state = gIw;

    FunctionHead_0801b9ec(state, 0);
    for (;;) {
        WaitFrames(1);
        if (*(u16 *)(state + 0x3a0) != 0) {
            continue;
        }
        if (mode != 0x3e7) {
            if (gIw3 & 0x10) {
                Menu_StepRight(state);
            } else if (gIw3 & 0x20) {
                Menu_StepLeft(state);
            } else if (gIw2 & 1) {
                return Menu_ConfirmSelection(state);
            }
        }
        if (mode != 0 && (gIw2 & 2)) {
            return -1;
        }
    }
}

/* menu/selection/wait_for_input.c */
u32 Menu_WaitForSelectionInput(u32 value)
{
    u8 *state = *(u8 **)ADDR_03001E98;
    volatile u32 *input;
    u32 result;

again:
    WaitFrames(1);
    if (*(u16 *)(state + 0x3a0) != 0)
        goto again;

    if (value != 999) {
        input = (u32 *)ADDR_03001B04;
        if (*input & 0x10) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            Menu_Do(state);
        } else if (*input & 0x20) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            Menu_Do2(state);
        }

        if (*(u32 *)ADDR_03001C94 & 1) {
            result = *(u16 *)(state + 0x39c)
                   + *(u16 *)(state + 0x39e);
            if (*(u16 *)(*(u8 **)(state + 0x348) + 10) == 6) {
                if (result == 0)
                    Audio_PlayCue(SOUND_MENU_CONFIRM);
                else
                    Audio_PlayCue(SOUND_MENU_CANCEL);
            } else {
                Audio_PlayCue(SOUND_MENU_CONFIRM);
            }
            return result;
        }
    }

    if (value != 0 && (*(u32 *)ADDR_03001C94 & 2)) {
        Audio_PlayCue(SOUND_MENU_CANCEL);
        return -1;
    }
    goto again;
}
