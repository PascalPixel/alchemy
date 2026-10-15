#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "party_state.h"
#include "scene_ids.h"


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

extern const u8 Data_02000840[];
extern const u8 Data_02000960[];

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov65ERoom2)
        return (const struct ScenePlacement *)Data_02000960;
    return (const struct ScenePlacement *)Data_02000840;
}
