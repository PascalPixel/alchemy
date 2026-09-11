#include "types.h"
#include "scene.h"
#include "abi/menu/selection/load_node_resource.h"

struct Node_0801b9ec {
    u8 filler0[4];
    struct Node_0801b9ec *next;
    u8 filler8[2];
    u16 type;
    u16 value;
    u8 filler14[18];
    u16 base;
};

struct State_0801b9ec {
    u8 filler0[0x348];
    struct Node_0801b9ec *head;
};

extern u8 gVal;

void Menu_LoadSelectedResource(void);

void Menu_LoadSelectionNodeResource(struct State_0801b9ec *state, u32 index)
{
    struct Node_0801b9ec *node = state->head;
    u32 res;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 id = node->base - (u32)&gVal;

        value = node->value;
        Menu_SetRange(id, 0, &value, &res, 1);
        Menu_LoadSelectedResource();
    }
}
