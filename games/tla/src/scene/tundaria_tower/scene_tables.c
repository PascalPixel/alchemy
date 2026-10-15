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


const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

#include "scene_ids.h"

extern const u8 gData_020033d0[];
extern const u8 gData_020033f0[];

const struct ScenePlacement *Func_02000488(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov68fRoom6)
        return (const struct ScenePlacement *)gData_020033d0;
    if (gPartyState.scene == (s32)&SceneId_Ov68fRoom7)
        return (const struct ScenePlacement *)gData_020033f0;
    return 0;
}
