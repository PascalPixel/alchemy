#include "layout_guard.h"
#include "types.h"

typedef struct SlidingNode_0801b664 {
    struct SlidingNode_0801b664 *previous;
    struct SlidingNode_0801b664 *next;
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
} SlidingNode_0801b664;

typedef struct SelectionState_0801b664 {
    u8 padding000[8];
    u16 backward_scroll_motion;
    u16 previous_marker_visible;
    u8 padding00c[0x30];
    u16 forward_scroll_motion;
    u16 next_marker_visible;
    u8 padding040[0x308];
    SlidingNode_0801b664 *head;
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
} SelectionState_0801b664;

LAYOUT_OFFSET_GUARD(
    SlidingNode0801b664_EntryKind,
    SlidingNode_0801b664,
    entry_kind,
    0x0a);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801b664_X,
    SlidingNode_0801b664,
    x,
    0x10);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801b664_MotionMode,
    SlidingNode_0801b664,
    motion_mode,
    0x14);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801b664_TargetX,
    SlidingNode_0801b664,
    target_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b664_Head,
    SelectionState_0801b664,
    head,
    0x348);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b664_ResourceIds,
    SelectionState_0801b664,
    resource_ids,
    0x354);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b664_PageSize,
    SelectionState_0801b664,
    page_size,
    0x394);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b664_FirstVisible,
    SelectionState_0801b664,
    first_visible,
    0x39c);
LAYOUT_OFFSET_GUARD(
    SelectionState0801b664_CursorState,
    SelectionState_0801b664,
    cursor_state,
    0x3a2);

void Func_0801b9a8(SelectionState_0801b664 *, u16);
void Func_0801b9ec(SelectionState_0801b664 *, u16);
void Func_0801ba68(SelectionState_0801b664 *, u32);
void Func_0801bd98(u16, u16, SlidingNode_0801b664 *, s32);
void Func_0801b010(u16, u32);
void Func_080030f8(s32);

static void RepopulateFromStart_0801b664(SelectionState_0801b664 *state)
{
    SlidingNode_0801b664 *node = state->head;
    u16 entry = 0;

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

/* Move the selector forward, wrapping and rebuilding a long five-row page. */
void Func_0801b664(SelectionState_0801b664 *state)
{
    SlidingNode_0801b664 *node;
    u16 position = state->cursor_position;

    Func_0801b9a8(state, position);
    state->cursor_state = 0x21;
    Func_080030f8(1);

    position++;
    state->cursor_position = position;

    if (state->page_size <= 5) {
        if (position == state->page_size)
            state->cursor_position = 0;
    } else if (state->first_visible + position == state->page_size) {
        SlidingNode_0801b664 *head = state->head;

        state->previous_marker_visible = 0;
        node = head;
        for (;;) {
            node->target_x = state->base_x;
            node->target_y = state->base_y;
            node->motion_mode = -0x0c;
            if (node->next == 0)
                break;
            node = node->next;
        }

        while (node->target_x != node->x)
            Func_080030f8(1);

        RepopulateFromStart_0801b664(state);
        state->cursor_position = 0;
        state->first_visible = 0;

        node = head->next;
        position = head->x + 0x10;
        while (node != 0) {
            node->target_x = (s16)position;
            node->motion_mode = 0x0c;
            node = node->next;
            position += 0x10;
        }
        state->next_marker_visible = 1;
    } else if (position == 4 &&
               state->first_visible + position + 1 < state->page_size) {
        state->cursor_position = position - 1;
        state->forward_scroll_motion = 8;
        state->first_visible++;
        Func_0801ba68(state, 1);

        if (state->first_visible + state->cursor_position + 2 ==
            state->page_size) {
            state->next_marker_visible = 0;
        }
        state->previous_marker_visible = 1;
    }

    state->cursor_state = 1;
    Func_0801b9ec(state, state->cursor_position);
    Func_080030f8(1);
    Func_0801b010(state->head->entry_kind, 0);
    Func_080030f8(1);
}
