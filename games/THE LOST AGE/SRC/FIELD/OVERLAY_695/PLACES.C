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

void GameFlag_SetBit();

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}

void Func_02000358(void)
{
    Call1((void (*)())GameFlag_SetBit, 0x200);
    return;
}

void Func_02000364(void)
{
    Call1((void (*)())GameFlag_SetBit, 0x201);
    return;
}

void Func_02000374(void)
{
    Call1((void (*)())GameFlag_SetBit, 0x202);
    return;
}

void Func_02000384(void)
{
    Call1((void (*)())GameFlag_SetBit, 0x203);
    return;
}
