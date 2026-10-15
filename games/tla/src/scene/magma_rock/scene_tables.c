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

extern const u8 Data_02003ea8[];
extern const u8 Data_02003ed8[];
extern const u8 Data_02003f88[];

const struct SceneEntrance *Func_0200088c(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov691Room2)
        return (const struct SceneEntrance *)Data_02003ed8;
    if (gPartyState.scene == (s32)&SceneId_Ov691Room3)
        return (const struct SceneEntrance *)Data_02003f88;
    return (const struct SceneEntrance *)Data_02003ea8;
}
