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
#include "scene_ids.h"


const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

extern const u8 Data_0200116c[];
extern const u8 Data_020011b8[];

const void *Scene_GetExits(void)
{
    if (GameFlag_Test(0x873))
        return Data_020011b8;
    return Data_0200116c;
}

extern const u8 Data_02001204[];
extern const u8 Data_020013fc[];
extern const u8 Data_020014a4[];

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gPartyState.scene == (s32)&SceneId_DailaInteriors2)
        return (const struct ScenePlacement *)Data_020013fc;
    if (GameFlag_Test(0x873))
        return (const struct ScenePlacement *)Data_020014a4;
    return (const struct ScenePlacement *)Data_02001204;
}

void DailaInteriors_EmptyHook(void)
{
}
