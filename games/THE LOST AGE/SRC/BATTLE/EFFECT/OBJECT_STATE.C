#include "TYPES.H"

s32 Func_08020190(s32, s32, s32);

void ObjectMotion_SnapToTerrain(void *object)
{
    s32 height;

#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    /* These localizations reach the terrain query through this entry. */
    height = Func_08020190(0, *(s32 *)((u8 *)object + 8),
#else
    height = Map_GetTerrainHeightFar(0, *(s32 *)((u8 *)object + 8),
#endif
        *(s32 *)((u8 *)object + 0x10));
    *(s32 *)((u8 *)object + 0x0c) = height;
    *(s32 *)((u8 *)object + 0x14) = height;
}
