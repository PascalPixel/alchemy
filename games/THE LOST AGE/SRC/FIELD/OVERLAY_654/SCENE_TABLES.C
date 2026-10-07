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
#include "SCENE_IDS.H"

extern const u8 Data_02003df4[];
extern const u8 Data_02003e14[];
extern const u8 Data_02003e44[];


const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Kabe9)
        return (const struct SceneRegion *)Data_02003df4;
    if (scene == (s32)&SceneId_Kabe10)
        return (const struct SceneRegion *)Data_02003e14;
    if (scene == (s32)&SceneId_Kabe11)
        return (const struct SceneRegion *)Data_02003e44;
    return 0;
}
