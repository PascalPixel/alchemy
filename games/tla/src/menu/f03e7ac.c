#include "types.h"
#include "ram_buffer.h"

struct SelectionNode {
    u8 unknown_00[4];
    struct SelectionNode *next;
    u8 unknown_08[8];
    u16 x;
    u16 y;
    u8 unknown_14[4];
    u16 target_x;
    u16 target_y;
};

struct SelectionScreen {
    u8 unknown_000[0x348];
    struct SelectionNode *head;
    u8 unknown_34c[0x4a];
    u16 base_x;
    u16 base_y;
};

void Func_0803e7c8(void *menu, s32 side);

void Menu_SetNodeCoordinates(u32 first, u32 second)
{
    struct SelectionScreen *state;
    struct SelectionNode *node;

    /* FAKEMATCH: the one-pass block ends the scheduling block: the native loads the menu work before it builds the offset of the base coordinates (one reordered row, differ 2 without). */
    do {
        state = Ram_HeapSlots->menu_work;
    } while (0);
    state->base_x = first;
    state->base_y = second;
    node = state->head;
    while (node != 0) {
        node->x = first;
        node->target_x = first;
        node->y = second;
        node->target_y = second;
        node = node->next;
        first += 16;
    }
}

void Func_0803e7ac(void)
{
    void *menu = Ram_HeapSlots->menu_work;

    Func_0803e7c8(menu, 0);
    Func_0803e7c8(menu, 1);
}
