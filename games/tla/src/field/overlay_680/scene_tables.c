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

extern const u8 Data_02000f5c[];
extern const u8 Data_02000f8c[];

const struct ScenePlacement *Func_02000040(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov680Room1)
        return (const struct ScenePlacement *)Data_02000f5c;
    if (gPartyState.scene == (s32)&SceneId_Ov680Room2)
        return (const struct ScenePlacement *)Data_02000f8c;
    return 0;
}
