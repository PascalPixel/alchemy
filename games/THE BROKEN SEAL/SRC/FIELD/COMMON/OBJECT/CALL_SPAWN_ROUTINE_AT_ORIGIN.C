#include "TYPES.H"
#include "SCENE.H"
s32 Menu_RunConfirmSelectionFar(s32 value, s32 x, s32 y, s32 z);

s32 Object_CallSpawnRoutineAtOrigin(s32 value)
{
    return Menu_RunConfirmSelectionFar(value, 0, 0, 0);
}
