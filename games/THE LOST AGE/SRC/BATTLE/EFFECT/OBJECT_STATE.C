#include "TYPES.H"
#include "OBJECT_RUNTIME.H"

s32 Func_08020190(s32, s32, s32);

/* Sets the object on the terrain under it. */
void ObjectMotion_SnapToTerrain(void *object)
{
    struct ObjectRuntime *state = object;
    s32 height;

#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    /* These localizations reach the terrain query through this entry. */
    height = Func_08020190(0, state->x, state->z);
#else
    height = Map_GetTerrainHeightFar(0, state->x, state->z);
#endif
    state->y = height;
    state->terrain_height = height;
}
