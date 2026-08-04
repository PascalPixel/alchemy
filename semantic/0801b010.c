#include "layout_guard.h"
#include "types.h"

struct SelectionNode_0801b010 {
    u8 padding00[0x20];
    u16 description_message;
};

struct Window_0801b010 {
    u8 padding00[8];
    u16 width;
};

struct SelectionState_0801b010 {
    u8 padding000[0x350];
    struct Window_0801b010 *description_window;
    u8 padding354[0x40];
    u16 use_entry_description;
    u8 padding396[0x0a];
    u16 special_selection;
    u8 padding3a2[0x16];
    u16 special_anchor;
};

LAYOUT_OFFSET_GUARD(
    SelectionNode0801b010_Description,
    struct SelectionNode_0801b010,
    description_message,
    0x20);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b010_Window,
    struct SelectionState_0801b010,
    description_window,
    0x350);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b010_EntryDescription,
    struct SelectionState_0801b010,
    use_entry_description,
    0x394);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b010_SpecialSelection,
    struct SelectionState_0801b010,
    special_selection,
    0x3a0);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b010_SpecialAnchor,
    struct SelectionState_0801b010,
    special_anchor,
    0x3b8);

extern struct SelectionState_0801b010 *Data_03001e98;

struct SelectionNode_0801b010 *Func_0801b36c(void *);
struct Window_0801b010 *Func_080162d4(u32, u32, u32, u32, u32);
void Func_08016418(struct Window_0801b010 *, s32);
void Func_08016478(struct Window_0801b010 *);
void Func_0801e7c0(u32, struct Window_0801b010 *, s32, s32);

/*
 * Ensure that the active selection's description window exists at the right
 * width, present it, and draw either the entry-specific message or the fixed
 * type-2/type-4 fallback.  Type 6 uses its own five-tile window and records a
 * sentinel anchor for the surrounding special selector.
 */
void Func_0801b010(u16 type, u32 content_width)
{
    struct SelectionState_0801b010 *state = Data_03001e98;
    struct SelectionNode_0801b010 *selection = Func_0801b36c(state);
    struct Window_0801b010 *window = state->description_window;

    if (window == 0) {
        if (type == 6) {
            u32 y = state->special_anchor != 0 ? 17 : 0;

            window = Func_080162d4(17, y, 5, 3, type);
            state->description_window = window;
            state->special_selection = 0;
            state->special_anchor = 999;
        } else {
            window = Func_080162d4(
                ((9 - content_width) >> 1) + 19,
                17,
                content_width + 2,
                3,
                6);
            state->description_window = window;
        }
        Func_08016478(window);
    } else {
        if (content_width != 0 && window->width != content_width + 2) {
            Func_08016418(window, 2);
            window = Func_080162d4(
                ((9 - content_width) >> 1) + 19,
                17,
                content_width + 2,
                3,
                6);
            state->description_window = window;
        }
        Func_08016478(state->description_window);
    }

    if (state->use_entry_description != 0) {
        Func_0801e7c0(
            selection->description_message,
            state->description_window,
            0,
            0);
    } else if (type == 2) {
        Func_0801e7c0(0x50, state->description_window, 0, 0);
    } else if (type == 4) {
        Func_0801e7c0(0x51, state->description_window, 0, 0);
    }
}
