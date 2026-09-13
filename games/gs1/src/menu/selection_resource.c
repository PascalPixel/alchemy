#include "types.h"

struct MenuResourceNode {
    u8 unknown0[4];
    struct MenuResourceNode *next;
    u8 unknown8[2];
    u16 type;
    u16 value;
    u8 unknown14[18];
    u16 base;
};

struct MenuResourceList {
    u8 unknown0[0x348];
    struct MenuResourceNode *head;
};

extern u8 Data_0000001f;
void Func_08019ee4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);

void Menu_ReloadNodeResource(struct MenuResourceList *state, u32 index)
{
    struct MenuResourceNode *node = state->head;
    u32 output;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 first = node->base - (u32)&Data_0000001f;

        value = node->value;
        Func_08019ee4(first, 0, &value, &output, 1);
    }
}





void Menu_LoadSelectedResource(void);

void Menu_LoadSelectionNodeResource(struct MenuResourceList *state, u32 index)
{
    struct MenuResourceNode *node = state->head;
    u32 res;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 id = node->base - (u32)&Data_0000001f;

        value = node->value;
        Func_08019ee4(id, 0, &value, &res, 1);
        Menu_LoadSelectedResource();
    }
}

void Func_080b50e0(u16 *, s32);

void Menu_SendNodeCountList(u8 *arg0)
{
    u16 data[6];
    u8 *node = *(u8 **)(arg0 + 0x348);
    s32 count = 0;

    while (node != 0) {
        node = *(u8 **)(node + 4);
        count++;
    }
    data[count] = 0xff;
    Func_080b50e0(data, 0);
}
