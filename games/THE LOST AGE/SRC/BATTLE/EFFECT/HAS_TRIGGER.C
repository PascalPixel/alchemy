#include "TYPES.H"

s32 BattleFx_FindMatchingEvent(s32, s32, void *);

s32 BattleFx_HasMatchingEvent5(s32 effectId);
void ObjectMotion_SnapToTerrain(void *object);

u32 BattleFx_HasTrigger(u16 effectId)
{
    s32 result;

    result = Event_FindFacingTrigger(effectId);
    return (u32)((0 - result) | result) >> 0x1F;
}
