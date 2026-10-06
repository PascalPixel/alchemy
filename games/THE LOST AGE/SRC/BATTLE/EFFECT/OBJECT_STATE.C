#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"


void ObjectMotion_SnapToTerrain(void *object)
{
    struct ObjectRuntime *state = object;
    s32 height;

#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    height = Func_08020190(0, state->x, state->z);
#else
    height = Map_GetTerrainHeightFar(0, state->x, state->z);
#endif
    state->y = height;
    state->terrain_height = height;
}
