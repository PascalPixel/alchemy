#include "TYPES.H"
#include "FIELD_SCENE.H"

extern const u32 gSceneExits[];

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}
