#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "sound_ids.h"

/* menu/selection/selection_loop.c */
/* menu/selection/loop.c */
extern u8 *gIw;
extern u32 gIw2;
extern volatile u32 gIw3;

void WaitFrames(u32 value);

s32 Menu_SelectionLoop(s32 mode)
{
    u8 *state = gIw;

    Menu_Apply(state, 0);
    for (;;) {
        WaitFrames(1);
        if (*(u16 *)(state + 0x3a0) != 0) {
            continue;
        }
        if (mode != 0x3e7) {
            if (gIw3 & 0x10) {
                Menu_Do(state);
            } else if (gIw3 & 0x20) {
                Menu_unk2_2(state);
            } else if (gIw2 & 1) {
                return Menu_unk3_2(state);
            }
        }
        if (mode != 0 && (gIw2 & 2)) {
            return -1;
        }
    }
}

/* menu/selection/wait_for_input.c */
extern void WaitFrames(u32);
extern void Audio_PlayCue(u32);

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
            Menu_unk2_2(state);
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
struct State_0801b9a8;
struct State_0801b9ec;

void WaitFrames(u32 value);

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
void WaitFrames(u32 value);

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

/* menu/selection/reload_node_resource.c */
struct Node_0801b9a8 {
    u8 filler0[4];
    struct Node_0801b9a8 *next;
    u8 filler8[2];
    u16 type;
    u16 value;
    u8 filler14[18];
    u16 base;
};

struct State_0801b9a8 {
    u8 filler0[0x348];
    struct Node_0801b9a8 *head;
};

extern u8 gVal;

void Menu_ReloadNodeResource(struct State_0801b9a8 *state, u32 index)
{
    struct Node_0801b9a8 *node = state->head;
    u32 output;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 first = node->base - (u32)&gVal;

        value = node->value;
        Menu_SetRange(first, 0, &value, &output, 1);
    }
}

/* menu/selection/load_node_resource.c */
struct Node_0801b9ec {
    u8 filler0[4];
    struct Node_0801b9ec *next;
    u8 filler8[2];
    u16 type;
    u16 value;
    u8 filler14[18];
    u16 base;
};

struct State_0801b9ec {
    u8 filler0[0x348];
    struct Node_0801b9ec *head;
};

extern u8 gVal;

void Menu_LoadSelectedResource(void);

void Menu_LoadSelectionNodeResource(struct State_0801b9ec *state, u32 index)
{
    struct Node_0801b9ec *node = state->head;
    u32 res;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 id = node->base - (u32)&gVal;

        value = node->value;
        Menu_SetRange(id, 0, &value, &res, 1);
        Menu_LoadSelectedResource();
    }
}

/* menu/selection/send_node_count_list.c */
void Menu_SendNodeCountList(u8 *arg0)
{
    u16 data[6];
    u8 *node = *(u8 **)(arg0 + 0x348);
    s32 count = 0;

    while (node != 0) {
        node = *(u8 **)(node + 4);
        count++;
    }
    data[count] = 0xff;
    Menu_Apply(data, 0);
}

/* resource/load_by_mode.c */
/* resource/load_by_mode.c */
extern s32 Resource_FindFreeSlot(void);

s32 Resource_LoadByMode(s32 mode, s32 value)
{
    s32 output;
    s32 result = -1;

    switch (mode) {
    case 1:
    case 6:
        Sys_SetRange(value, 0, &result, &output, 0);
        break;
    case 2:
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return -1;
        Sys_Place(value, 0x1a, result);
        break;
    case 9:
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return -1;
        Ui_BuildPatternToSlot(value, 0, result);
        break;
    case 4:
        Ability_LoadGlyph(value, 1, &result, &output, 0);
        break;
    }
    return result;
}

/* resource/load_by_mode_into_slot.c */
extern s32 Resource_FindFreeSlot(void);

s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option)
{
    s32 output;
    s32 original = result;

    if (result == -1) {
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return original;
    }

    switch (mode) {
    case 1:
    case 6:
        Sys_SetRange(value, option, &result, &output, 1);
        break;
    case 2:
        Sys_Place(value, 58, result);
        break;
    case 7:
        Sys_Place(value, 42, result);
        break;
    case 4:
        Ability_LoadGlyph(value, option, (s32)&result, (s32)&output, 1);
        break;
    case 8:
        Ui_BuildPatternToSlot(value, 0, result);
        break;
    case 9:
        load_table_entry(value, 0, (void *)result);
        break;
    }

    return result;
}
