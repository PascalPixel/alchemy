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

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

extern s16 Data_02002c5c[];
extern s16 Data_02002efc[];
s32 GameFlag_Test(s32 flag);

const s16 *Func_0200004c(void)
{
    if (GameFlag_Test(0x97f))
        return Data_02002efc;
    return Data_02002c5c;
}
