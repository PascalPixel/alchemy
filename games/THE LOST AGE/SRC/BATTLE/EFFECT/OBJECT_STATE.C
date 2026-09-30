#include "TYPES.H"

void ObjectMotion_SnapToTerrain(void *object)
{
    s32 height;

    height = Map_GetTerrainHeightFar(0, *(s32 *)((u8 *)object + 8),
        *(s32 *)((u8 *)object + 0x10));
    *(s32 *)((u8 *)object + 0x0c) = height;
    *(s32 *)((u8 *)object + 0x14) = height;
}
