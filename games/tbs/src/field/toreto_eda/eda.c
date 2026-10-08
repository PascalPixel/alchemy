#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "resource_397.h"
#include "io_reg.h"
#include "irq.h"

extern struct EventWork *gEventWork;
extern u8 gDisplayTransition[];

struct SceneObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 settled_y;
    u8 unknown_18[10];
    u8 layer;
};

union SceneCell {
    s32 w;
    s16 h[2];
};

extern s32 gGameState[];
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif
extern s32 gKeysHeld;
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern s32 gKeysRepeat;
#endif

static u16 sTransferHeader[16];
static u8 sTransferData[0x60];
static s32 sSplitLine;
static u16 sHofsAbove;
static u16 sHofsBelow;

void *Object_GetById(u32);
void BattleFx_SetPhaseRequest(s32, s32);


void State_SetActorEightValue3d(void)
{
    BattleFx_SetPhaseRequest(8, 0x3D);
}

u8 *ToretoEda_GetEntrances(void)
{
    return ToretoEda_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *ToretoEda_GetExits(void)
{
    return ToretoEda_SceneTable1;
}

u8 *ToretoEda_GetPlacements(void)
{
    return ToretoEda_SceneTable2;
}

void Actor_ShiftObjectsByBlock(s32 bx, s32 bz)
{
    u8 *work = *(u8 **)&gEventWork;
    struct SceneObject *obj;
    s32 dx = bx;
    s32 dz = bz;
    s32 h;

    obj = (struct SceneObject *)Object_GetById(gGameState[125]);
    dx <<= 20;
    dz <<= 20;

    if (obj != 0) {
        obj->x += dx;
        obj->z += dz;
        h = Map_GetTerrainHeight((s32)obj->layer, obj->x, obj->z);
        obj->y = h;
        obj->settled_y = h;
    }

    obj = *(struct SceneObject **)(work + 480);
    if (obj != 0) {
        obj->x += dx;
        obj->z += dz;
        h = Map_GetTerrainHeight((s32)obj->layer, obj->x, obj->z);
        obj->y = h;
        obj->settled_y = h;
    }
}

void Scene_ApplyOffset0Pos5(void)
{
    Actor_ShiftObjectsByBlock(0, 5);
}

void Scene_ApplyOffset0Neg5(void)
{
    Actor_ShiftObjectsByBlock(0, -5);
}

void Scene_ApplyOffset0Pos5Second(void)
{
    Actor_ShiftObjectsByBlock(0, 5);
}

void Scene_ApplyOffset0Neg5Second(void)
{
    Actor_ShiftObjectsByBlock(0, -5);
}

void Scene_ApplyOffset0Pos6(void)
{
    Actor_ShiftObjectsByBlock(0, 6);
}

void Scene_ApplyOffset0Neg6(void)
{
    Actor_ShiftObjectsByBlock(0, -6);
}

void ToretoEda_PlayCueAndSelectTouchedExit(void)
{
    u8 *state = (u8 *)gEventWork;
    s16 *cnt;

    Engine_AudioPlayCue(0x7B);
    cnt = (s16 *)(state + 0x16C);
    Engine_EventRequestExit(*cnt);
}

void Effect_SetAlphaBlendForScene9(void)
{
    u8 *disp;

    DisplayTransition_InitializeBattleEffectState(9);

    REG_BLDCNT = 0x3f42;
    REG_BLDALPHA = 0x0c04;

    disp = *(u8 **)gDisplayTransition;
    {
        u16 *slot = (u16 *)(disp + 0x534);
        int value = 0x3f3f;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x536);
        int value = 31;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x52a);
        int value = 10;
        *slot = value;
    }
}

u8 *ToretoEda_GetEvents(void)
{
    return ToretoEda_SceneTable3;
}

void Scene_RunTwoCallSequence(void)
{
    Engine_EventBegin();
    Engine_EventEnd();
}

void State_RunEmptyHookFirst(void)
{
}

void State_RunEmptyHook(void)
{
}

void ToretoEda_ExchangeInputPacket(void)
{
    u16 *hdr = sTransferHeader;

    hdr[0] = gKeysHeld;
    hdr[1] = gKeysRepeat;
    SerialRuntime_PollAndTransfer(hdr, sTransferData);
}

s32 ToretoEda_InitializeBackdropTransition(void)
{
    u8 **base = (u8 **)&gEventWork;
    u8 *work;
    u8 *disp;
    s32 off = 224;
    s32 *scene;

    off <<= 1;
    work = *base;
    scene = (s32 *)(work + off);
    off -= 192;
    *scene = off;

    DisplayTransition_InitializeBattleEffectState(9);

    REG_BLDCNT = 0x3f42;
    REG_BLDALPHA = 0x0c04;

    disp = base[4];
    {
        u16 *slot = (u16 *)(disp + 0x534);
        int value = 0x3f3f;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x536);
        int value = 31;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x52a);
        int value = 10;
        *slot = value;
    }

    ToretoEda_StartBg3Split();
    return 0;
}

void Effect_UpdateBg3HofsByVcount(void)
{
    u16 *src;
    u32 value;

    if (REG_VCOUNT >= sSplitLine) {
        src = &sHofsAbove;
    } else {
        src = &sHofsBelow;
    }
    value = *src;
    REG_BG3HOFS = value;
}

void Effect_SetBg3HofsSplit(void)
{
    union SceneCell *work = (union SceneCell *)(((u8 *)gMapWork[0]) + 260);
    s32 hofs;

    sSplitLine = 192 - work[1].h[1];
    sHofsAbove = hofs = work[0].h[1];
    sHofsBelow = hofs - (gFrameCount >> 2);
}

void ToretoEda_StartBg3Split(void)
{
    Runtime_SetIrqHandler(1, 0, Effect_UpdateBg3HofsByVcount);
    Engine_TaskAddCallback((s32)(Effect_SetBg3HofsSplit), 0xc80);
}
