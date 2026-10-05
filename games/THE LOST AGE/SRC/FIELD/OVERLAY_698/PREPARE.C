#include "TYPES.H"
#include "FIELD_SCENE.H"

void Scene_SetArrivalFlags(s32 unused);

s32 Scene_PrepareMap(void)
{
    Scene_SetArrivalFlags(0);
    return 0;
}
