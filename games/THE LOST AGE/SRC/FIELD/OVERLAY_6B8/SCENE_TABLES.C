#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FIELD_SCENE.H"


const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}
