#include "types.h"
#include "ram_buffer.h"

struct SelectionNode
{
    u8                    unk00[4];
    struct SelectionNode *next;
    u8                    unk08[8];
    u16                   x;
    u16                   y;
    u8                    unk14[4];
    u16                   targetX;
    u16                   targetY;
};

struct SelectionScreen
{
    u8                    unk000[0x348];
    struct SelectionNode *head;
    u8                    unk34c[0x4a];
    u16                   baseX;
    u16                   baseY;
};

void Func_0803e7c8(void *menu, s32 side);

void Menu_SetNodeCoordinates(u32 first, u32 second)
{
    struct SelectionScreen *state;
    struct SelectionNode   *node;

    /* FAKEMATCH: the one-pass block ends the scheduling block: the native loads the menu work before it builds the offset of the base coordinates (one reordered row, differ 2 without). */
    do
    {
        state = Ram_HeapSlots->menuWork;
    } while (0);
    state->baseX = first;
    state->baseY = second;
    node = state->head;
    while (node != 0)
    {
        node->x = first;
        node->targetX = first;
        node->y = second;
        node->targetY = second;
        node = node->next;
        first += 16;
    }
}

void Func_0803e7ac(void)
{
    void *menu = Ram_HeapSlots->menuWork;

    Func_0803e7c8(menu, 0);
    Func_0803e7c8(menu, 1);
}
