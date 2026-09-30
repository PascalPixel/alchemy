#include "TYPES.H"

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

extern u8 MsgCommandName;
void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);

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
        u32 id = node->base - (u32)&MsgCommandName;

        value = node->value;
        Ui_BuildPairedPatternsToSlot(id, 0, &value, &res, 1);
        Menu_LoadSelectedResource();
    }
}
