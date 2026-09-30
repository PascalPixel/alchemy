#include "TYPES.H"
#include "SYSTEM.H"

u8 *Owner_GetStateFar(s32);
s32 *GetBattleObjectSlot(s32);
u8 *GetMotionRecord(s32, s32);
void AnimationObjects_SelectAnimationFar(void *, s32);
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

    state = Owner_GetStateFar(id);
    index = 0;
    while ((item = GetMotionRecord(*GetBattleObjectSlot(id), index)) != 0) {
        if (state[0x12a] != 1)
            AnimationObjects_SelectAnimationFar(item, 4);
        else
            AnimationObjects_SelectAnimationFar(item, 5);
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
