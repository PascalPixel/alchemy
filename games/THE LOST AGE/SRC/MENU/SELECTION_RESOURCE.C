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
    BattlePres_SetActorModesFar(data, 0);
}
