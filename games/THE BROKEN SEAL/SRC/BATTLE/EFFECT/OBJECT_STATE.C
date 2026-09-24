#include "TYPES.H"

s32 BattleFx_FindMatchingEvent(s32, s32, void *);

s32 BattleFx_HasMatchingEvent5(s32 effectId)
{
    s32 local;
    s32 result = BattleFx_FindMatchingEvent(0x70000005, (u16)effectId, &local);
    return (u32)((-result) | result) >> 31;
}

s32 Event_FindFacingTrigger(u16);

u32 BattleFx_HasTrigger(u16 effectId)
{
    s32 result;

    result = Event_FindFacingTrigger(effectId);
    return (u32)((0 - result) | result) >> 0x1F;
}

s32 Map_GetTerrainHeightFar(s32, s32, s32);

void ObjectMotion_SnapToTerrain(void *object)
{
    s32 height;

    height = Map_GetTerrainHeightFar(0, *(s32 *)((u8 *)object + 8),
        *(s32 *)((u8 *)object + 0x10));
    *(s32 *)((u8 *)object + 0x0c) = height;
    *(s32 *)((u8 *)object + 0x14) = height;
}
