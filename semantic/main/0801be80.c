#include "layout_guard.h"
#include "types.h"

typedef struct SpriteConfig_0801be80 {
    u8 padding00[5];
    u8 attribute0_high;
    u8 padding06;
    u8 attribute1_high;
    u16 attribute2;
} SpriteConfig_0801be80;

typedef struct SlidingNode_0801be80 {
    struct SlidingNode_0801be80 *previous;
    struct SlidingNode_0801be80 *next;
    u16 field08;
    u16 entry_kind;
    u16 resource;
    u16 tile;
    s16 x;
    s16 y;
    s16 motion_mode;
    s16 field16;
    s16 target_x;
    s16 target_y;
    s16 display_x;
    s16 display_y;
    u16 field20;
    s16 motion;
    s16 step;
    s16 scale;
    SpriteConfig_0801be80 sprite;
} SlidingNode_0801be80;

typedef struct SelectionState_0801be80 {
    u8 padding000[0x0a];
    s16 left_marker;
    u8 padding00c[0x32];
    s16 right_marker;
    u8 padding040[0x2a2];
    s16 cursor_upload;
    u8 padding2e4[0x16];
    s16 cursor_animation;
    u8 padding2fc[0x4c];
    SlidingNode_0801be80 *head;
    SlidingNode_0801be80 *collapsed_head;
    u8 padding350[0x4a];
    s16 cursor_offset;
    u16 first_visible;
    u16 cursor_position;
    u8 padding3a0[2];
    s16 cursor_state;
    u16 saved_first_visible[5];
    u16 saved_cursor_position[5];
} SelectionState_0801be80;

LAYOUT_OFFSET_GUARD(
    SlidingNode0801be80_EntryKind,
    SlidingNode_0801be80,
    entry_kind,
    0x0a);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801be80_X,
    SlidingNode_0801be80,
    x,
    0x10);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801be80_TargetX,
    SlidingNode_0801be80,
    target_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801be80_Sprite,
    SlidingNode_0801be80,
    sprite,
    0x28);
LAYOUT_OFFSET_GUARD(
    SelectionState0801be80_Head,
    SelectionState_0801be80,
    head,
    0x348);
LAYOUT_OFFSET_GUARD(
    SelectionState0801be80_CursorOffset,
    SelectionState_0801be80,
    cursor_offset,
    0x39a);
LAYOUT_OFFSET_GUARD(
    SelectionState0801be80_FirstVisible,
    SelectionState_0801be80,
    first_visible,
    0x39c);
LAYOUT_OFFSET_GUARD(
    SelectionState0801be80_CursorState,
    SelectionState_0801be80,
    cursor_state,
    0x3a2);
LAYOUT_OFFSET_GUARD(
    SelectionState0801be80_SavedFirstVisible,
    SelectionState_0801be80,
    saved_first_visible,
    0x3a4);

void Func_080030f8(u32);
void Func_08003f3c(u32);
SlidingNode_0801be80 *Func_0801a910(s32);
void Func_0801b9a8(SelectionState_0801be80 *, u16);
void Func_0801ba34(SelectionState_0801be80 *);
void Func_0801c21c(void);

static SlidingNode_0801be80 *NodeAt_0801be80(
    SlidingNode_0801be80 *node,
    u16 index)
{
    while (node != 0 && index != 0) {
        node = node->next;
        index--;
    }
    return node;
}

static void AppendCollapsedNode_0801be80(
    SelectionState_0801be80 *state,
    SlidingNode_0801be80 *node)
{
    SlidingNode_0801be80 *tail = state->collapsed_head;

    if (tail == 0) {
        state->collapsed_head = node;
        node->previous = 0;
    } else {
        while (tail->next != 0)
            tail = tail->next;
        tail->next = node;
        node->previous = tail;
    }
    node->next = 0;
}

/* Collapse the active page to its selected entry and append its display copy. */
u32 Func_0801be80(SelectionState_0801be80 *state)
{
    u32 selected_index = state->first_visible + state->cursor_position;
    SlidingNode_0801be80 *selected;
    SlidingNode_0801be80 *node;
    SlidingNode_0801be80 *copy;
    s32 collapsed_count = 0;

    Func_0801ba34(state);
    Func_0801b9a8(state, state->cursor_position);
    state->cursor_state = 0x21;
    Func_080030f8(1);

    state->left_marker = 0;
    state->right_marker = 0;
    state->cursor_upload = 0;
    state->cursor_animation = 0;
    Func_0801c21c();

    selected = NodeAt_0801be80(state->head, state->cursor_position);
    selected->display_x = selected->x;
    selected->display_y = selected->y;

    for (node = state->head; node != 0; node = node->next) {
        if (node != selected) {
            node->target_x = selected->x;
            node->motion_mode = (selected->x - node->x) >> 1;
        }
    }
    Func_080030f8(2);

    for (node = state->head; node != 0; node = node->next) {
        if (node != selected) {
            Func_08003f3c(node->resource);
            node->entry_kind = 0;
        }
    }

    state->head = selected;
    selected->previous = 0;
    selected->next = 0;
    selected->target_x = 4;
    for (node = state->collapsed_head; node != 0; node = node->next) {
        selected->target_x += 0x10;
        collapsed_count++;
    }

    state->saved_first_visible[collapsed_count] = state->first_visible;
    state->saved_cursor_position[collapsed_count] = state->cursor_position;
    selected->motion_mode = (selected->target_x - selected->x) >> 1;
    state->cursor_offset = 0;
    state->cursor_position |= 0x80;
    Func_080030f8(2);

    copy = Func_0801a910(1);
    copy->entry_kind = selected->entry_kind;
    copy->field20 = selected->field20;
    copy->field08 = selected->field08;
    copy->resource = selected->resource;
    copy->tile = selected->tile;
    copy->x = selected->x;
    copy->y = selected->y;
    copy->target_x = selected->x;
    copy->target_y = selected->y;
    copy->display_x = selected->display_x;
    copy->display_y = selected->display_y;
    copy->motion_mode = 0;
    copy->field16 = 0;
    copy->motion = 0x100;
    copy->scale = 0x100;

    copy->sprite.attribute0_high &= 0x03;
    copy->sprite.attribute1_high =
        (copy->sprite.attribute1_high & 0x3f) | 0x40;
    ((u8 *)&copy->sprite.attribute2)[1] &= 0x0f;
    copy->sprite.attribute2 =
        (copy->sprite.attribute2 & 0xfc00) | (copy->tile & 0x03ff);

    selected->entry_kind = 0;
    state->head = 0;
    AppendCollapsedNode_0801be80(state, copy);
    return selected_index;
}
