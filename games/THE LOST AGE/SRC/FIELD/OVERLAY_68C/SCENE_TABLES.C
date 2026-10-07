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

void Battle_WaitMode0();
void Func_0200031c();
void Func_0200282c();
void Func_02002834();
void Func_020028ac();
void Func_020028c4();
void Func_020028fc();
void Func_0200291c();
void GameFlag_Test();
void Inventory_PromptAndSetObjectMode();
void ObjectMotion_ArmCallback();
void Object_LinkObjectAndSetCallback();
void UiText_OpenMessageAtObject();

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return gSceneRegions;
}

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}

void Func_02000058(s32 arg0)
{
    s32 result0;
    s32 result1;
    s32 result2;
    s32 result3;

    result0 = Value1((s32 (*)())GameFlag_Test, 0x966);
    if (result0 == 0) goto L_02008082;
    Call1((void (*)())Func_020028ac, (s32)MsgGoToLunpa);
    Call2((void (*)())Func_020028c4, arg0, 0);
    Call2((void (*)())Object_LinkObjectAndSetCallback, 10, 4);
    goto L_020080f6;
L_02008082:
    result1 = Value1((s32 (*)())GameFlag_Test, 0x965);
    if (result1 == 0) goto L_020080ba;
    ((void (*)())Func_0200282c)();
    Call1((void (*)())Func_020028fc, 0);
    Call1((void (*)())Func_020028ac, (s32)MsgIAmSo);
    Call2((void (*)())Func_020028c4, arg0, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, arg0, 0x3000, 0);
    ((void (*)())Func_02002834)();
    goto L_020080f6;
L_020080ba:
    Call1((void (*)())Func_020028ac, (s32)MsgAreYouTheTravelers);
    Value2((s32 (*)())UiText_OpenMessageAtObject, arg0, 0);
    result2 = Value0((s32 (*)())Func_0200291c);
    result3 = Value2((s32 (*)())Inventory_PromptAndSetObjectMode, result2, 0);
    if (result3 != 0) goto L_020080dc;
    ((void (*)())Func_0200031c)();
    goto L_020080f6;
L_020080dc:
    Value1((s32 (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Func_020028c4, arg0, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, arg0, 0x8000, 0);
L_020080f6:
    return;
}
