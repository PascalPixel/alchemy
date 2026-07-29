#include "types.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct SlidingNode_0801b664 {
    struct SlidingNode_0801b664 *previous;
    struct SlidingNode_0801b664 *next;
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

void Func_0801b9a8(void *, u16);
void Func_0801b9ec(void *, u16);
void Func_0801ba68(void *, u32);
void Func_0801bd98(
    u16, u16, struct SlidingNode_0801b664 *, s32);
void Func_0801b010(u16, u32);
void Func_080030f8(s32);

/*
 * Move the selection window one step forward. At the end of a long page,
 * animate the whole list back to its origin and repopulate its records;
 * within the sliding range, insert the newly exposed trailing entry.
 */
void Func_0801b664(void *state)
{
    struct SlidingNode_0801b664 *node;
    u16 page_size;
    u16 selection;
    u16 position;

    position = M2C_FIELD(state, u16 *, 0x39E);
    Func_0801b9a8(state, position);
    M2C_FIELD(state, u16 *, 0x3A2) = 0x21;
    Func_080030f8(1);

    position++;
    M2C_FIELD(state, u16 *, 0x39E) = position;
    page_size = M2C_FIELD(state, u16 *, 0x394);

    if (page_size <= 5) {
        if (position == page_size) {
            M2C_FIELD(state, u16 *, 0x39E) = 0;
        }
    } else {
        selection = M2C_FIELD(state, u16 *, 0x39C);
        if (selection + position == page_size) {
            struct SlidingNode_0801b664 *head =
                M2C_FIELD(state, struct SlidingNode_0801b664 **, 0x348);
            u16 base_x = M2C_FIELD(state, u16 *, 0x396);
            u16 base_y = M2C_FIELD(state, u16 *, 0x398);

            M2C_FIELD(state, u16 *, 0xA) = 0;
            node = head;
            if (node->next != NULL) {
                do {
                    node->target_x = base_x;
                    node->target_y = base_y;
                    node->motion_mode = -0xC;
                    node = node->next;
                } while (node->next != NULL);
            }
            node->target_x = base_x;
            node->target_y = base_y;
            node->motion_mode = -0xC;

            while (node->target_x != node->x) {
                Func_080030f8(1);
            }

            node = head;
            if (node != NULL) {
                u16 *resource = (u16 *)((u8 *)state + 0x354);
                do {
                    Func_0801bd98(
                        resource[0], resource[16], node, 1);
                    node = node->next;
                    resource++;
                } while (node != NULL);
            }

            M2C_FIELD(state, u16 *, 0x39E) = 0;
            M2C_FIELD(state, u16 *, 0x39C) = 0;
            node = head->next;
            base_x = head->x + 0x10;
            while (node != NULL) {
                node->target_x = base_x;
                node->motion_mode = 0xC;
                node = node->next;
                base_x += 0x10;
            }
            M2C_FIELD(state, u16 *, 0x3E) = 1;
        } else if (position == 4 &&
                   selection + position + 1 < page_size) {
            M2C_FIELD(state, u16 *, 0x39E) = position - 1;
            M2C_FIELD(state, u16 *, 0x3C) = 8;
            selection++;
            M2C_FIELD(state, u16 *, 0x39C) = selection;
            Func_0801ba68(state, 1);
            position = M2C_FIELD(state, u16 *, 0x39E);
            if (selection + position + 2 == page_size) {
                M2C_FIELD(state, u16 *, 0x3E) = 0;
            }
            M2C_FIELD(state, u16 *, 0xA) = 1;
        }
    }

    M2C_FIELD(state, u16 *, 0x3A2) = 1;
    Func_0801b9ec(state, M2C_FIELD(state, u16 *, 0x39E));
    Func_080030f8(1);
    node = M2C_FIELD(state, struct SlidingNode_0801b664 **, 0x348);
    Func_0801b010(node->field_0a, 0);
    Func_080030f8(1);
}
