#include "layout_guard.h"
#include "types.h"

typedef struct SlidingNode_0801ba68 {
    struct SlidingNode_0801ba68 *previous;
    struct SlidingNode_0801ba68 *next;
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
    u8 field_1c[6];
    s16 motion;
    s16 step;
    s16 scale;
} SlidingNode_0801ba68;

typedef struct SelectionState_0801ba68 {
    u8 padding000[0x348];
    SlidingNode_0801ba68 *head;
    u8 padding34c[8];
    u16 resource_ids[16];
    u16 variants[16];
    u16 page_size;
    u16 base_x;
    u16 base_y;
    u16 padding39a;
    u16 first_visible;
} SelectionState_0801ba68;

LAYOUT_OFFSET_GUARD(
    SlidingNode0801ba68_EntryKind,
    SlidingNode_0801ba68,
    entry_kind,
    0x0a);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801ba68_X,
    SlidingNode_0801ba68,
    x,
    0x10);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801ba68_MotionMode,
    SlidingNode_0801ba68,
    motion_mode,
    0x14);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801ba68_TargetX,
    SlidingNode_0801ba68,
    target_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    SlidingNode0801ba68_Motion,
    SlidingNode_0801ba68,
    motion,
    0x22);
LAYOUT_OFFSET_GUARD(
    SelectionState0801ba68_Head,
    SelectionState_0801ba68,
    head,
    0x348);
LAYOUT_OFFSET_GUARD(
    SelectionState0801ba68_ResourceIds,
    SelectionState_0801ba68,
    resource_ids,
    0x354);
LAYOUT_OFFSET_GUARD(
    SelectionState0801ba68_BaseX,
    SelectionState_0801ba68,
    base_x,
    0x396);
LAYOUT_OFFSET_GUARD(
    SelectionState0801ba68_FirstVisible,
    SelectionState_0801ba68,
    first_visible,
    0x39c);

SlidingNode_0801ba68 *Func_0801a910(s32);
void Func_0801bd98(
    u16, u16, SlidingNode_0801ba68 *, s32);
void Func_080030f8(s32);
void Func_08003f3c(u32);

/*
 * Insert a newly constructed entry at one side of the animated list, slide
 * every entry by one sixteen-pixel slot, then retire the entry that leaves
 * the opposite side.
 */
void Func_0801ba68(SelectionState_0801ba68 *state, u32 direction)
{
    SlidingNode_0801ba68 *node;
    SlidingNode_0801ba68 *cursor;
    u16 selection;
    u16 resource;
    u16 variant;
    u16 base_x;
    u16 base_y;

    base_x = state->base_x;
    base_y = state->base_y;

    if (direction != 0) {
        SlidingNode_0801ba68 *head = state->head;

        selection = state->first_visible + 4;
        resource = state->resource_ids[selection];
        variant = state->variants[selection];
        node = Func_0801a910(0);
        if (node == 0) {
            return;
        }
        Func_0801bd98(resource, variant, node, 0);

        node->x = base_x + 0x50;
        node->target_x = base_x + 0x40;
        node->y = base_y;
        node->target_y = base_y;
        node->motion = 0x20;
        node->step = 0x20;
        node->scale = 0x100;
        node->motion_mode = -2;

        cursor = head;
        cursor->step = -0x20;
        cursor->target_x = cursor->x - 0x10;
        cursor->scale = 0;
        cursor->motion_mode = -2;
        while (cursor->next != 0) {
            cursor = cursor->next;
            cursor->target_x = cursor->x - 0x10;
            cursor->motion_mode = -2;
        }

        cursor->next = node;
        node->next = 0;
        node->previous = cursor;

        do {
            Func_080030f8(1);
        } while (head->motion != 0);

        state->head = head->next;
        Func_08003f3c(head->resource);
        head->entry_kind = 0;
        head->next->previous = 0;
    } else {
        SlidingNode_0801ba68 *old_head = state->head;

        selection = state->first_visible;
        resource = state->resource_ids[selection];
        variant = state->variants[selection];
        node = Func_0801a910(0);
        if (node == 0) {
            return;
        }
        Func_0801bd98(resource, variant, node, 0);

        node->x = base_x - 0x10;
        node->target_x = base_x;
        node->y = base_y;
        node->target_y = base_y;
        node->motion_mode = 2;
        node->motion = 0x20;
        node->step = 0x20;
        node->scale = 0x100;

        old_head->previous = node;
        node->next = old_head;
        node->previous = 0;
        state->head = node;

        cursor = node;
        do {
            cursor->target_x = cursor->x + 0x10;
            cursor->motion_mode = 2;
            cursor = cursor->next;
        } while (cursor != 0);

        do {
            Func_080030f8(1);
        } while (node->motion != 0x100);

        cursor = node;
        while (cursor->next != 0) {
            cursor = cursor->next;
        }
        Func_08003f3c(cursor->resource);
        cursor->entry_kind = 0;
        cursor->previous->next = 0;
    }
}
