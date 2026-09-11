#include "types.h"
#include "scene.h"
#include "abi/menu/selection/reload_node_resource.h"

struct Node_0801b9a8 {
    u8 filler0[4];
    struct Node_0801b9a8 *next;
    u8 filler8[2];
    u16 type;
    u16 value;
    u8 filler14[18];
    u16 base;
};

struct State_0801b9a8 {
    u8 filler0[0x348];
    struct Node_0801b9a8 *head;
};

extern u8 gVal;

void Menu_ReloadNodeResource(struct State_0801b9a8 *state, u32 index)
{
    struct Node_0801b9a8 *node = state->head;
    u32 output;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 first = node->base - (u32)&gVal;

        value = node->value;
        Menu_SetRange(first, 0, &value, &output, 1);
    }
}
