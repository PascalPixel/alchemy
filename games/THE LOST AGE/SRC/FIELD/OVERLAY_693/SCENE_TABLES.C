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
