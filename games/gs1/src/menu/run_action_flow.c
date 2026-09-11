#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "a8_state.h"

/* menu/run_action_flow.c */
/* menu/sel/run_action_flow.c */
struct MenuActionObject {
    u8 padding0[5];
    u8 mode;
};

struct MenuActionState {
    u8 padding0[0x14];
    struct MenuActionObject *object;
    u8 padding18[0x15C];
    u16 selection;
};

extern struct MenuState_080a76d0 *gIw;

s32 GameFlag_IsSet(s32 flag);

s32 Menu_RunActionFlow(void)
{
    struct MenuActionState *state = gIw;
    s32 step = 0;
    s32 finished = step;
    s32 result = 0;
    u32 changed;

    while (!finished && !GameFlag_IsSet(0x150)) {
        switch (step) {
        case 0:
            state->selection = finished;
            if (Menu_Check(0) == -1) {
                result = -1;
                finished = 1;
            }
            step = 1;
            break;
        case 1:
            state->object->mode = 13;
            result = Menu_unk2();
            step = result == -1 ? 0 : 2;
            break;
        case 2:
            state->object->mode = 13;
            result = Menu_unk3();
            step = 0;
            if (result != -1)
                step = 3;
            break;
        case 3:
            state->object->mode = 13;
            result = Menu_unk4();
            /* Collapse every non-cancellation result to one. */
            changed = (u32)~result;
            step = (-changed | changed) >> 31;
            break;
        default:
            finished = 1;
            break;
        }
    }

    if (GameFlag_IsSet(0x150))
        result = -1;

    return result;
}

/* menu/character_selector/move_entry.c */
struct CharacterSelectorOrderState {
    u8 unknown_000[0x208];
    u16 character_ids[8];
    u8 unknown_218;
    volatile u8 character_count;
};

s32 CharacterSelector_MoveEntry(s32 selected_index, s32 direction)
{
    struct CharacterSelectorOrderState *state =
        *(struct CharacterSelectorOrderState **)0x03001f2c;
    u32 reordered[14];
    s32 index;

    if (state->character_count <= 1) {
        return 0;
    }
    if (direction == 1) {
        if (selected_index == state->character_count - 1) {
            return 0;
        }
    } else {
        if (selected_index == 0) {
            return 0;
        }
    }

    {
        u32 zero = 0;
        u32 *clear = &reordered[13];

        do {
            *clear = zero;
            clear--;
        } while ((s32)clear >= (s32)reordered);
    }
    for (index = 0; index < state->character_count; index++) {
        reordered[index] = state->character_ids[index];
    }

    if (direction == 1) {
        index = reordered[selected_index];
        reordered[selected_index] = reordered[selected_index + 1];
        reordered[selected_index + 1] = index;
    } else {
        index = reordered[selected_index];
        reordered[selected_index] = reordered[selected_index - 1];
        reordered[selected_index - 1] = index;
    }

    for (index = 0; index < state->character_count; index++) {
        Menu_Check(state->character_ids[index]);
    }
    for (index = 0; index < state->character_count; index++) {
        Menu_unk2(reordered[index]);
    }
    *(u8 *)&state->character_count = Menu_unk3(state->character_ids);
    return 1;
}

/* menu/character_selector/init_selector_entries.c */
struct State080a8034 {
    u8 padding_00[0x20];
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2c;
    u8 padding_30[0xe0];
    s8 field_110;
    s8 field_111;
    s8 field_112;
    s8 field_113;
};

s32 InitializeEntryObjects(s32, s32, s32, s32, s32);

void Menu_InitSelectorCursorAndEntries(void)
{
    struct State080a8034 *state;

    state = *(struct State080a8034 **)ADDR_03001F2C;
    InitializeEntryObjects(Menu_Check(state), 2, 2, 8, 0);
    state->field_28 = 0;
    state->field_24 = 0;
    state->field_2c = 0;
    state->field_20 = 0;
    state->field_110 = 0;
    state->field_111 = 0;
    state->field_112 = 8;
    state->field_113 = 2;
}

/* menu/entry/create_window_and_entry_objects.c */
typedef struct {
    u8 padding[15];
    u8 field_0f;
} Object0f;

void *Menu_Run(s32, s32, s32, s32, s32, s32);

void Menu_CreateWindowAndEntryObjects(s32 resource)
{
    s32 created;
    s32 handle;
    void *object;
    struct State080a8088 *state;

    state = *(struct State080a8088 **)ADDR_03001F2C;
    created = 0;
    handle = state->handle;
    if (handle == 0) {
        created = UiWindow_UpdateOrCreate(&state->handle, 0, 5, 0x1E, 0xF, 2);
        handle = state->handle;
    }
    if (created != 0) {
        object = Menu_Run(resource, 0, 0, handle, 0, 0);
        state->object = object;
        if ((((Object0f *)object)->field_0f = 0xF0, state->mode) == 3) {
            Menu_SpawnIconEntries(state, handle);
        }
        Menu_Check(handle);
        Menu_Place(handle, resource, 0x100);
        return;
    }
    Menu_Place(handle, resource, 0);
}

/* character_menu/draw_selection_cursor.c */
struct State_080a847c {
    u8 padding[36];
    u8 *object;
};

extern struct State_080a847c *volatile gIw;
extern u8 gRom[];

void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected,
    u8 *entries, s32 invert)
{
    struct State_080a847c *state = gIw;
    u32 different;
    s32 count;
    s32 index;
    s32 x;
    s32 y;
    s32 last;
    u8 width;

    if (mode == 0) {
        y = selected * 2 + 5;
        x = 0;
        width = 5;
        count = 0;
        index = 0;
        while (index <= 4) {
            if (entries[index] != 0) {
                if (selected == count) {
                    width = gRom[index];
                    break;
                }
                count++;
            }
            index++;
        }
    } else if (selected <= 3) {
        y = selected;
        x = 5;
        width = 13;
    } else {
        y = selected + 4;
        x = 8;
        width = 20;
    }

    different = 1 ^ (u32)invert;
    last = 15 - (((0u - different) | different) >> 31);
    Sys_SetRect(state->object, x, y, width, 1, last);
}

/* character_menu/draw_selection_labels.c */
void UiText_DrawMessageAt(s32, s32, s32, s32);
extern u8 gVal[][2];
extern u8 gVal2[];

void CharacterMenu_DrawSelectionLabels(s32 target, s32 selected,
    const u8 *entries)
{
    s32 count = 0;
    s32 index = 0;

    do {
        if (entries[index] != 0) {
            if (selected == count) {
                s32 message = (s32)gVal[index];
                UiText_DrawMessageAt(message, target, 0, -1);
                UiText_DrawMessageAt(message + 1, target, 0, 15);
            }
            count++;
        }
        index++;
    } while (index <= 4);

    if (count == 0)
        UiText_DrawMessageAt((s32)gVal2, target, 0, 0);
}
