#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
void *ResourceMetadata_RegisterFar(s32, s32);
s32 Animation_SetWorkEntryFar(void *, s32);
void **GetBattleObjectSlot();

void BattleMotion_SpawnSlotEffectAndWait(void)
{
    void *object;
    void *effect;

    object = *GetBattleObjectSlot();
    if ((object != NULL) && ((0xF & FIELD_AT_OFFSET(object, u8 *, 0x54)) == 1)) {
        effect = ResourceMetadata_RegisterFar(FIELD_AT_OFFSET(object, s32 *, 0x50), 0x11B);
        if (effect != NULL) {
            Animation_SetWorkEntryFar(effect, 1);
            FIELD_AT_OFFSET(effect, s8 *, 6) = 3;
        }
        WaitFrames(0xA);
    }
}
