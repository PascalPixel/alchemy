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
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

void Func_02001870();
void Func_02001878();
void Func_020018d0();
void Func_02001930();

void Func_02000d04(void)
{
    ((void (*)())Func_02001870)();
    Call1((void (*)())Func_02001930, 0);
    Call3((void (*)())Func_020018d0, gPartyState.current_owner, 0x101, 32);
    ((void (*)())Func_02001878)();
    return;
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
