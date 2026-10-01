/* Near miss: score 80. ⚓️ loads the record's child pointer (ldr r2, [r0,
   #40]) before scaling the index for items[] in the second loop, and one
   bne differs. 45 s of permuting found nothing. */
#include "TYPES.H"
#include "SYSTEM.H"

u8 *Owner_GetState(s32);
s32 *GetBattleObjectSlot(s32);
u8 *GetMotionRecord(s32, s32);
void Animation_ApplyChildArgumentFar(void *, s32);
void BattleActor_RemoveFromLists(s32);
void Map_RenderAllAnimatedTileFramesFar(void **, s32);
void ActivateBattleObjectSlot(s32);

void BattleMotion_InitializeActorRecords(s32 id)
{
    void *items[4];
    u8 *state;
    u8 *item;
    u8 *child;
    s32 index;

    state = Owner_GetState(id);
    index = 0;
    while ((item = GetMotionRecord(*GetBattleObjectSlot(id), index)) != 0) {
        if (state[0x12a] != 1)
            Animation_ApplyChildArgumentFar(item, 4);
        else
            Animation_ApplyChildArgumentFar(item, 5);
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
        BattleActor_RemoveFromLists(id);
        Map_RenderAllAnimatedTileFramesFar(items, index);
        ActivateBattleObjectSlot(id);
    }
}
