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
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"



s32 Func_02000038(s32 arg0)
{
    *(u8 *)((u8 *)*(s32 *)((u8 *)arg0 + 80) + 26) = 0;
    *(u8 *)((u8 *)(arg0 + 34) + 0) = 1;
    return 1;
}

void Battle_WaitMode0();
void Func_02000a30();
void Func_02000a38();
void Func_02000a40();
void Func_02000a68();
void Func_02000a70();
void Func_02000ab0();
void Func_02000ac0();
void Func_02000ad0();
void Func_02000ad8();
void Func_02000af0();
void Func_02000b30();
void GameFlag_SetBit();
void GameFlag_Test();
void Motion_CamBounds();
struct ObjectRuntime *Object_GetById();

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

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}

void Func_02000064(void)
{
    Call6((void (*)())Func_02000a40, 47, 9, 9, 4, 25, 9);
    Call6((void (*)())Func_02000a40, 47, 69, 9, 10, 25, 69);
    Call6((void (*)())Func_02000a38, 111, 8, 9, 5, 89, 8);
    return;
}

void Func_020000a8(void)
{
    s32 result0;
    struct ObjectRuntime *object1;

    result0 = Value1((s32 (*)())GameFlag_Test, 0x200);
    if (result0 != 0) goto L_02008100;
    Call1((void (*)())GameFlag_SetBit, 0x200);
    ((void (*)())Func_02000a68)();
    Call1((void (*)())Func_02000b30, 0);
    Call1((void (*)())Func_02000ac0, (s32)MsgShipIceWallFirstApproach);
    object1 = Object_GetById(8);
    Call1((void (*)())Func_02000a30, (s32)object1);
    Call2((void (*)())Func_02000ad8, 8, 0xc000);
    Call1((void (*)())Battle_WaitMode0, 5);
    Call3((void (*)())Func_02000ad0, 5, 0, 5);
    Call3((void (*)())Func_02000ad0, 11, 0, 5);
    ((void (*)())Func_02000a70)();
L_02008100:
    return;
}

void Func_02000108(void)
{
    s32 result0;
    struct ObjectRuntime *object1;

    result0 = Value1((s32 (*)())GameFlag_Test, 0x90c);
    if (result0 != 0) goto L_020081d2;
    Call1((void (*)())GameFlag_SetBit, 0x90c);
    ((void (*)())Func_02000a68)();
    Call1((void (*)())Func_02000b30, 0);
    Call1((void (*)())Func_02000ac0, (s32)MsgShipIceWallCrewReposition);
    object1 = Object_GetById(8);
    Call1((void (*)())Func_02000a30, (s32)object1);
    Call1((void (*)())Battle_WaitMode0, 5);
    Call3((void (*)())Func_02000ad0, 5, 0, 5);
    Call4((void (*)())Motion_CamBounds, 0x1d00000, -1, 0xb00000, 1);
    ((void (*)())Func_02000af0)();
    Call1((void (*)())Battle_WaitMode0, 5);
    Call3((void (*)())Func_02000ab0, 11, 0x1980000, 0x1200000);
    Call3((void (*)())Func_02000ab0, 5, 0x1980000, 0x1200000);
    Call3((void (*)())Func_02000ab0, 6, 0x1980000, 0x1200000);
    Call3((void (*)())Func_02000ad0, 11, 0, 5);
    Call3((void (*)())Func_02000ad0, 6, 0, 5);
    Call3((void (*)())Func_02000ad0, 11, 0, 5);
    Call3((void (*)())Func_02000ab0, 11, 0, 0);
    Call3((void (*)())Func_02000ab0, 5, 0, 0);
    Call3((void (*)())Func_02000ab0, 6, 0, 0);
    ((void (*)())Func_02000a70)();
L_020081d2:
    return;
}
