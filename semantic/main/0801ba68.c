#include "types.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct SlidingNode_0801ba68 {
    struct SlidingNode_0801ba68 *previous;
    struct SlidingNode_0801ba68 *next;
    u16 field_08;
    u16 field_0a;
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
};

struct SlidingNode_0801ba68 *Func_0801a910(s32);
void Func_0801bd98(
    u16, u16, struct SlidingNode_0801ba68 *, s32);
void Func_080030f8(s32);
void Func_08003f3c(u32);

/*
 * Insert a newly constructed entry at one side of the animated list, slide
 * every entry by one sixteen-pixel slot, then retire the entry that leaves
 * the opposite side.
 */
void Func_0801ba68(void *state, u32 direction)
{
    struct SlidingNode_0801ba68 *node;
    struct SlidingNode_0801ba68 *cursor;
    u16 selection;
    u16 resource;
    u16 variant;
    u16 base_x;
    u16 base_y;

    base_x = M2C_FIELD(state, u16 *, 0x396);
    base_y = M2C_FIELD(state, u16 *, 0x398);

    if (direction != 0) {
        struct SlidingNode_0801ba68 *head =
            M2C_FIELD(state, struct SlidingNode_0801ba68 **, 0x348);

        selection = M2C_FIELD(state, u16 *, 0x39C) + 4;
        resource = M2C_FIELD(state, u16 *, 0x354 + selection * 2);
        variant = M2C_FIELD(state, u16 *, 0x374 + selection * 2);
        node = Func_0801a910(0);
        if (node == NULL) {
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
        while (cursor->next != NULL) {
            cursor = cursor->next;
            cursor->target_x = cursor->x - 0x10;
            cursor->motion_mode = -2;
        }

        cursor->next = node;
        node->next = NULL;
        node->previous = cursor;

        do {
            Func_080030f8(1);
        } while (head->motion != 0);

        M2C_FIELD(state, struct SlidingNode_0801ba68 **, 0x348) = head->next;
        Func_08003f3c(head->resource);
        head->field_0a = 0;
        head->next->previous = NULL;
    } else {
        struct SlidingNode_0801ba68 *old_head =
            M2C_FIELD(state, struct SlidingNode_0801ba68 **, 0x348);

        selection = M2C_FIELD(state, u16 *, 0x39C);
        resource = M2C_FIELD(state, u16 *, 0x354 + selection * 2);
        variant = M2C_FIELD(state, u16 *, 0x374 + selection * 2);
        node = Func_0801a910(0);
        if (node == NULL) {
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
        node->previous = NULL;
        M2C_FIELD(state, struct SlidingNode_0801ba68 **, 0x348) = node;

        cursor = node;
        do {
            cursor->target_x = cursor->x + 0x10;
            cursor->motion_mode = 2;
            cursor = cursor->next;
        } while (cursor != NULL);

        do {
            Func_080030f8(1);
        } while (node->motion != 0x100);

        cursor = node;
        while (cursor->next != NULL) {
            cursor = cursor->next;
        }
        Func_08003f3c(cursor->resource);
        cursor->field_0a = 0;
        cursor->previous->next = NULL;
    }
}
