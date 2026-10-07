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
