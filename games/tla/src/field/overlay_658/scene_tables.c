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

extern const u8 Data_02002d38[];
extern const u8 Data_02002ff0[];

const u8 *Func_0200004c(void)
{
    if (GameFlag_Test(0x8ff))
        return Data_02002ff0;
    return Data_02002d38;
}
