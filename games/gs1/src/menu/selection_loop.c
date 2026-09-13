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
void Menu_Apply(void *state, u32 index);
void Menu_Apply2(void *state, u32 mode);
void Menu_Apply3(void *state, u32 index);
void Menu_Apply4(u16 type, u32 value);

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

/* menu/selection/move_forward.c */
void Menu_MoveSelectionForward(u8 *state)
{
    u16 *selection = (u16 *)(state + 0x39c);
    u16 *index = (u16 *)(state + 0x39e);
    u16 *count;
    u32 end = *selection + *index + 1;

    count = (u16 *)(state + 0x394);
    if (end != *count) {
        Menu_Apply(state, *index);
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 33;

            *status = value;
        }
        WaitFrames(1);
        *index += 1;
        if (*index == 4 && end + 1 < *count) {
            *index += 0xffff;
            {
                u32 value = 8;

                *(u16 *)(state + 60) = value;
            }
            *selection += 1;
            Menu_Apply2(state, 1);
            if (*selection + *index + 2 == *count) {
                u32 value = 0;

                *(u16 *)(state + 62) = value;
            }
            {
                u32 value = 1;

                *(u16 *)(state + 10) = value;
            }
        }
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 1;

            *status = value;
            Menu_Apply3(state, *(u16 *)(state + 0x39e));
        }
        WaitFrames(1);
        Menu_Apply4(*(u16 *)(*(u8 **)(state + 0x348) + 10), 0);
        WaitFrames(1);
    }
}

/* menu/selection/move_backward.c */
void Menu_MoveSelectionBackward(u8 *state)
{
    u32 *selection = (u32 *)(state + 0x39c);

    if (*selection != 0) {
        u32 no;

        Menu_Apply(state, *(u16 *)(state + 0x39e));
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 33;

            *status = value;
        }
        WaitFrames(1);
        no = *(u16 *)(state + 0x39e);
        if (no == 1 && *(u16 *)selection != 0) {
            {
                u32 value = 8;

                *(u16 *)(state + 8) = value;
            }
            *(u16 *)selection += 0xffff;
            Menu_Apply2(state, 0);
            if (*(u16 *)selection == 0) {
                *(u16 *)(state + 10) = 0;
            }
            *(u16 *)(state + 62) = no;
        } else {
            *(u16 *)(state + 0x39e) += 0xffff;
        }
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 1;

            *status = value;
            Menu_Apply3(state, *(u16 *)(state + 0x39e));
        }
        WaitFrames(1);
        Menu_Apply4(*(u16 *)(*(u8 **)(state + 0x348) + 10), 0);
        WaitFrames(1);
    }
}
