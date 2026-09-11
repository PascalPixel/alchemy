#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* menu/selection/set_node_coordinates.c */
struct Node_0801b1ec {
    u8 filler0[4];
    struct Node_0801b1ec *next;
    u8 filler8[8];
    u16 first1;
    u16 second1;
    u8 filler14[4];
    u16 first2;
    u16 second2;
};

struct State_0801b1ec {
    u8 filler0[0x348];
    struct Node_0801b1ec *head;
    u8 filler34c[0x4a];
    u16 first;
    u16 second;
};

extern struct State_0801b1ec *gIw;

void Menu_SetNodeCoordinates(u32 first, u32 second)
{
    struct State_0801b1ec *state = gIw;
    struct Node_0801b1ec *node;

    state->first = first;
    state->second = second;
    node = state->head;
    while (node != 0) {
        node->first1 = first;
        node->first2 = first;
        node->second1 = second;
        node->second2 = second;
        node = node->next;
        first += 16;
    }
}

/* menu/selection/setup_both_sides.c */
void Menu_SetupSelectionBothSides(void)
{
    s32 state;

    state = *(s32 *)ADDR_03001E98;
    Menu_Apply(state, 0);
    Menu_Apply(state, 1);
}
