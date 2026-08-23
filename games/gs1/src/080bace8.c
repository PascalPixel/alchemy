#include "types.h"

u8 *Runtime_GetObject(s32);
s32 *GetBattleObjectSlot(s32);
u8 *GetMotionRecord(s32, s32);
void Object_InitializeMode(void *, s32);
void WaitFrames(s32);
void Func_080bac6c(s32);
void Func_08009108(void **, s32);
void ActivateBattleObjectSlot(s32);

void Func_080bace8(s32 id)
{
    void *items[4];
    u8 *state;
    u8 *item;
    u8 *child;
    s32 index;

    state = Runtime_GetObject(id);
    index = 0;
    while ((item = GetMotionRecord(*GetBattleObjectSlot(id), index)) != 0) {
        if (state[0x12a] != 1)
            Object_InitializeMode(item, 4);
        else
            Object_InitializeMode(item, 5);
        index++;
    }

    if (state[0x12a] == 1) {
        index = 0;
        while ((item = GetMotionRecord(*GetBattleObjectSlot(id), index)) != 0) {
            child = *(u8 **)(item + 40);
            items[index] = item;
            child[5] = 6;
            child[22] = 0xff;
            index++;
        }
        WaitFrames(4);
        Func_080bac6c(id);
        Func_08009108(items, index);
        ActivateBattleObjectSlot(id);
    }
}
