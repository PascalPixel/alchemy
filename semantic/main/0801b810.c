#include "layout_guard.h"
#include "types.h"

typedef struct SlidingNode_0801b810 {
    struct SlidingNode_0801b810 *previous;
    struct SlidingNode_0801b810 *next;
    u16 field_08;
    u16 entry_kind;
    u16 resource;
    u16 field_0e;
    s16 x;
    s16 y;
    s16 motion_mode;
    u16 field_16;
    s16 target_x;
    s16 target_y;
    u8 padding1c[6];
    s16 motion;
    s16 step;
    s16 scale;
} SlidingNode_0801b810;

typedef struct SelectionState_0801b810 {
    u8 padding000[8];
    u16 backward_scroll_motion;
    u16 previous_marker_visible;
    u8 padding00c[0x30];
    u16 forward_scroll_motion;
    u16 next_marker_visible;
    u8 padding040[0x308];
    SlidingNode_0801b810 *head;
    u8 padding34c[8];
    u16 resource_ids[16];
    u16 variants[16];
    u16 page_size;
    u16 base_x;
    u16 base_y;
    u16 padding39a;
    u16 first_visible;
    u16 cursor_position;
    u16 padding3a0;
    u16 cursor_state;
} SelectionState_0801b810;

LAYOUT_OFFSET_GUARD(
    SlidingNode0801b810_EntryKind,
    SlidingNode_0801b810,
    entry_kind,
    0x0a);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801b810_X,
    SlidingNode_0801b810,
    x,
    0x10);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801b810_MotionMode,
    SlidingNode_0801b810,
    motion_mode,
    0x14);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801b810_TargetX,
    SlidingNode_0801b810,
    target_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b810_Head,
    SelectionState_0801b810,
    head,
    0x348);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b810_ResourceIds,
    SelectionState_0801b810,
    resource_ids,
    0x354);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b810_PageSize,
    SelectionState_0801b810,
    page_size,
    0x394);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b810_FirstVisible,
    SelectionState_0801b810,
    first_visible,
    0x39c);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b810_CursorState,
    SelectionState_0801b810,
    cursor_state,
    0x3a2);

void Func_0801b9a8(SelectionState_0801b810 *, u16);
void Func_0801b9ec(SelectionState_0801b810 *, u16);
void Func_0801ba68(SelectionState_0801b810 *, u32);
void Func_0801bd98(u16, u16, SlidingNode_0801b810 *, s32);
void Func_0801b010(u16, u32);
void Func_080030f8(s32);

static void RepopulateNodes_0801b810(
    SelectionState_0801b810 *state,
    u16 first_entry)
{
    SlidingNode_0801b810 *node = state->head;
    u16 entry = first_entry;

    while (node != 0) {
        Func_0801bd98(
            state->resource_ids[entry],
            state->variants[entry],
            node,
            1);
        node = node->next;
        entry++;
    }
}

/* Move the selector backward, wrapping and rebuilding a long five-row page. */
void Func_0801b810(SelectionState_0801b810 *state)
{
    SlidingNode_0801b810 *node;
    u16 position = state->cursor_position;

    Func_0801b9a8(state, position);
    state->cursor_state = 0x21;
    Func_080030f8(1);

    if (state->page_size <= 5) {
        if (position != 0)
            position--;
        else
            position = state->page_size - 1;
        state->cursor_position = position;
    } else if ((state->first_visible | position) != 0) {
        if (position == 1 && state->first_visible != 0) {
            state->backward_scroll_motion = 8;
            state->first_visible--;
            Func_0801ba68(state, 0);
            if (state->first_visible == 0)
                state->previous_marker_visible = 0;
            state->next_marker_visible = 1;
        } else {
            state->cursor_position = position - 1;
        }
    } else {
        s32 offset = 0x40;
        s32 target_x;

        state->next_marker_visible = 0;
        node = state->head;
        while (node->next != 0) {
            node->target_x = node->x + offset;
            node->motion_mode = 0x0c;
            node = node->next;
            offset -= 0x10;
        }

        while (state->head->x != state->head->target_x)
            Func_080030f8(1);

        state->first_visible = state->page_size - 5;
        state->cursor_position = 4;
        RepopulateNodes_0801b810(state, state->first_visible);

        node = state->head;
        target_x = state->base_x;
        while (node->next != 0) {
            node->target_x = (s16)target_x;
            node->motion_mode = -0x0c;
            node = node->next;
            target_x += 0x10;
        }
        state->previous_marker_visible = 1;
    }

    state->cursor_state = 1;
    Func_0801b9ec(state, state->cursor_position);
    Func_080030f8(1);
    Func_0801b010(state->head->entry_kind, 0);
    Func_080030f8(1);
}
