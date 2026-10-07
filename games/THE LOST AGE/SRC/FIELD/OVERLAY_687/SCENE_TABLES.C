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

extern const u8 Data_020021e4[];
extern const u8 Data_02002214[];

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov687Room2)
        return (const struct ScenePlacement *)Data_02002214;
    return (const struct ScenePlacement *)Data_020021e4;
}
