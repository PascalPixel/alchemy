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

void Func_02001110(s32 a, s32 b, s32 c);
void GameFlag_ClearBit(s32 flag);

void Func_02000038(void)
{
    Func_02001110(14, 0, 15);
    GameFlag_ClearBit(0x300);
}

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
