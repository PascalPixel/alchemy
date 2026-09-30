#include "TYPES.H"
#include "FIELD_SCENE.H"

void Scene_SetArrivalFlags(s32 unused);

/* The scene's last loader hook, run as its map is prepared. */
s32 Scene_PrepareMap(void)
{
    Scene_SetArrivalFlags(0);
    return 0;
}
