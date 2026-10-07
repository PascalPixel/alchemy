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

void Func_02001060();
void GameFlag_SetBit();

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

void Func_0200004c(s32 arg0, s32 arg1)
{
    if (arg1 != 8) goto L_02008072;
    Call1((void (*)())GameFlag_SetBit, 0x9a9);
    Call6((void (*)())Func_02001060, 0, 2, 1, 1, 2, arg1);
L_02008072:
    if (arg1 != 10) goto L_02008094;
    Call1((void (*)())GameFlag_SetBit, 0x9aa);
    Call6((void (*)())Func_02001060, 0, 32, 1, 1, 14, 38);
L_02008094:
    return;
}
