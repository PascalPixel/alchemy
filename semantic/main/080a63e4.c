#include "layout_guard.h"
#include "types.h"

typedef struct MenuDefinition_080a63e4 {
    u8 padding00[0x0c];
    u16 first_row;
} MenuDefinition_080a63e4;

typedef struct Cursor_080a63e4 {
    u8 padding00[5];
    u8 kind;
    u16 position;
    u8 padding08[0x0e];
    u16 attributes;
} Cursor_080a63e4;

typedef struct MenuState_080a63e4 {
    u8 padding000[8];
    s32 selected_id;
    u8 padding00c[4];
    MenuDefinition_080a63e4 *definition;
    u8 padding014[4];
    Cursor_080a63e4 *cursor;
    s8 initial_index;
    s8 active_index;
    u8 padding01e[6];
    s32 action_window;
    u8 padding028[4];
    s32 preview_window;
    u8 padding030[0x148];
    u16 preview_resource;
    u8 padding17a[0x8e];
    u16 actor_ids[8];
    u8 padding218;
    u8 actor_count;
    u8 padding21a;
    u8 selected_actor_byte;
} MenuState_080a63e4;

LAYOUT_OFFSET_GUARD(
    MenuState080a63e4_Definition,
    MenuState_080a63e4,
    definition,
    0x10);
LAYOUT_OFFSET_GUARD(
    MenuState080a63e4_ActiveIndex,
    MenuState_080a63e4,
    active_index,
    0x1d);
LAYOUT_OFFSET_GUARD(
    MenuState080a63e4_PreviewResource,
    MenuState_080a63e4,
    preview_resource,
    0x178);
LAYOUT_OFFSET_GUARD(
    MenuState080a63e4_ActorIds,
    MenuState_080a63e4,
    actor_ids,
    0x208);
LAYOUT_OFFSET_GUARD(
    MenuState080a63e4_ActorCount,
    MenuState_080a63e4,
    actor_count,
    0x219);

extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;

s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015270(s32);
void *Func_08077008(u16);
s32 Func_080770c0(s32);
void Func_080770d0(s32);
void Func_080a112c(s32, u16, s32, s32);
void Func_080a17c4(Cursor_080a63e4 *);
void Func_080a1804(MenuState_080a63e4 *, u16);
void Func_080a1a40(s32, s32);
void Func_080a1ac0(s32, s32);
void Func_080f9010(s32);

/*
 * Select a party member with wrapped vertical navigation, optionally updating
 * the action and preview panes as the active member changes.
 */
s32 Func_080a63e4(s32 suppress_preview)
{
    MenuState_080a63e4 *state =
        *(MenuState_080a63e4 **)0x03001f2c;
    s32 selection = state->active_index;
    s32 result = 0;
    s32 preview_open = 0;
    s32 redraw = 1;
    s32 selection_bytes = 0;

    Func_08077008(state->actor_ids[state->initial_index]);
    Func_080a1ac0(selection * 24 - 10, 16);

    for (;;) {
        if (Func_080770c0(0x150) != 0) {
            selection_bytes = selection * 2;
            break;
        }

        if (redraw != 0) {
            s32 coordinate;
            u16 actor_id;

            redraw = 0;
            selection = Func_080022fc(
                selection + state->actor_count,
                state->actor_count);
            selection_bytes = selection * 2;
            actor_id = state->actor_ids[selection];
            Func_08077008(actor_id);

            coordinate =
                ((state->definition->first_row + selection * 3) * 8) - 2;
            state->cursor->position = (u16)coordinate;
            state->cursor->attributes =
                (state->cursor->attributes & 0xfe00) |
                ((u16)coordinate & 0x01ff);

            if (suppress_preview == 0) {
                Func_080a112c(state->action_window, actor_id, 0, 0);
                Func_080a1804(state, actor_id);
                if (Func_080770c0(0x151) == 0 && preview_open == 0) {
                    Func_08015270(state->preview_window);
                    Func_08015080(
                        (state->preview_resource & 0x3fff) + 0x53a,
                        state->preview_window,
                        0,
                        0);
                    preview_open = 1;
                } else {
                    Func_080770d0(0x151);
                }
            }
        } else {
            selection_bytes = selection * 2;
        }

        Func_080a1a40((selection_bytes + selection) * 8 - 10, 16);
        Func_080030f8(1);

        if ((Data_03001c94 & 1) != 0) {
            Func_080f9010(0x70);
            result = state->actor_ids[selection];
            break;
        }
        if ((Data_03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            result = -1;
            break;
        }
        if ((Data_03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            selection--;
            redraw = 1;
        }
        if ((Data_03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            selection++;
            redraw = 1;
        }
    }

    state->active_index = (s8)selection;
    Func_080a17c4(state->cursor);
    state->cursor->kind = 13;
    Func_080030f8(1);
    state->active_index = (s8)selection;
    state->selected_id = state->actor_ids[selection];
    state->selected_actor_byte = (u8)state->actor_ids[selection];
    return result;
}
