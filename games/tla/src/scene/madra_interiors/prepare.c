#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "madra_interiors.h"

extern u8 gData_0200349c[];
extern u8 gData_02003418[];
extern u8 gData_020033c8[];
void Battle_WaitMode0();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Field_DispatchTypeHandler();
void Func_02001d50();
void Func_02001d58();
void Func_02001db8();
void Func_02001e08();
void Func_02001e18();
void Func_02001e28();
void Func_02001e40();
void Func_02001e48();
void Func_02001e68();
void Func_02001e90();
void Func_02001e98();
void Func_02001ea0();
void Func_02001ea8();
void Func_02001eb0();
void Func_02001eb8();
void Func_02001ec8();
void Func_02001ef0();
void Func_02001f10();
void GameFlag_SetBit();
void Motion_CamBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void ObjectDispatch_SetSingleChildFlags();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_CommitPositionAndActivate();
void ObjectMotion_EnableActionAndResetMotion();
void ObjectMotion_EnableActionAndSetCallback();
void ObjectMotion_Launch();
void ObjectMotion_OffsetPositionAndResetMotion();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_SetAngleToward();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_WaitForAnimationChange();
void Object_AttachWorkTargetToObject();
struct ObjectRuntime *Object_GetById();
void Object_LinkObjectAndSetCallback();
void Object_LinkPair();
void Object_RefreshSelectorById();
void Object_SetModeById();
void UiText_OpenMessageAtObject();

void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_02001e80(s32 value);
void Func_02001d20(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001d28(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000534(void);
void Func_02000570(void);

// The rooms' setup: the leader's sprite priority, the two rooms' actors and
// the sprite of actor 10, by the story flags.
s32 Func_02000624(void)
{
    struct FieldSprite *sprite;
    void *event;

    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
    Func_02001e80(0);
    if (gPartyState.scene == (s32)&SceneId_Ov659Room1) {
        /* FAKEMATCH: the one-pass block ends the scheduling block: ⚓️ loads the event work before it builds the offset 0xd4c (differ 2 without it; through a plain pointer 20). */
        do {
            event = Ram_HeapSlots->eventWork;
        } while (0);
        FIELD_AT(event, u16, 0xd4c) = 3;
        if (!FlagCheck(0x821)) {
            ActorPlace(29, 0, 0);
            ActorPlace(30, 0, 0);
        } else {
            Object_SetPartAttribute(Object_GetById(29), 3);
            Object_SetPartAttribute(Object_GetById(30), 5);
        }
        if (FlagCheck(0x8ab)) {
            Func_02001d28(2, 110, 16, 10, 16, 64);
            Func_02001d20(2, 46, 16, 10, 16, 0);
        }
    }
    if (gPartyState.entrance == 20)
        Func_02000570();
    if (gPartyState.scene == (s32)&SceneId_Ov659Room2) {
        if (FlagCheck(0x8ab)) {
            ActorPlace(10, 0, 0);
        } else {
            Func_02001ef0(10, 0xc000);
            ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
            ((struct FieldSprite *)Object_GetById(10)->animation)->nextFrame |= 8;
            sprite = Object_GetById(10)->animation;
            sprite->unk11 = (sprite->unk11 & 3) | 32;
            ((struct FieldSprite *)Object_GetById(10)->animation)->unk19 = 1;
        }
    }
    Func_02000534();
    if (FlagCheck(0x8ff))
        Object_SetPartAttribute(Object_GetById(33), 2);
    else
        Object_SetPartAttribute(Object_GetById(28), 2);
    return 0;
}

s32 Scene_PrepareMap(void)
{
    return 0;
}

s32 Func_020007ac(struct ObjectRuntime *object)
{
    struct FieldSprite *sprite;

    sprite = object->animation;
    if (object != NULL && sprite != NULL)
        sprite->rotation -= 0x800;
    return 0;
}

s32 Func_020007c8(struct ObjectRuntime *object)
{
    struct FieldSprite *sprite;

    sprite = object->animation;
    if (object != NULL && sprite != NULL)
        sprite->rotation = 0;
    return 0;
}

void Func_020007dc(void)
{
    s32 i;
    struct ObjectRuntime *object0;
    struct ObjectRuntime *object1;
    struct ObjectRuntime *object2;
    struct ObjectRuntime *object3;
    struct ObjectRuntime *object4;
    struct ObjectRuntime *object5;
    struct ObjectRuntime *object6;
    struct ObjectRuntime *object7;
    struct ObjectRuntime *object8;
    struct ObjectRuntime *object9;
    struct ObjectRuntime *object10;
    struct ObjectRuntime *object11;
    struct ObjectRuntime *object12;
    s32 result13;
    struct ObjectRuntime *object14;

    ((void (*)())Func_02001d50)();
    Call1((void (*)())Func_02001eb8, 0);
    Call2((void (*)())Object_SetModeById, 8, 1);
    Call3((void (*)())Func_02001db8, 4, 0x1900000, 0x3780000);
    Call4((void (*)())Motion_CamBounds, 0x1980000, -1, 0x3500000, 1);
    ((void (*)())Event_StartScreenTransition)();
    ((void (*)())Event_WaitTransitionFrames)();
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 7, 0xcccc, 0x6666);
    Call1((void (*)())Func_02001e18, (s32)MsgMadraInteriors28);
    Call3((void (*)())Func_02001e28, 10, 0, 10);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 0x18c, 0x365);
    Call4((void (*)())Func_02001ec8, 11, 25, 19, 0xc000);
    Call1((void (*)())Object_RefreshSelectorById, 11);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 11, 0x8000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0xe000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 11, 0xe000, 0);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call4((void (*)())Motion_CamBounds, 0x1e80000, -1, 0x3340000, 1);
    ((void (*)())Func_02001e68)();
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value3((s32 (*)())Func_02001e48, 9, 0x107, 30);
    Call3((void (*)())Func_02001e28, 9, 0, 10);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
    Call3((void (*)())Func_02001e28, 9, 0, 10);
    Call2((void (*)())Func_02001e40, 7, 0xc000);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 7, 0, -16);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Value3((s32 (*)())Func_02001e48, 9, 0x103, 0);
    object0 = Object_GetById(9);
    object0->actionFlags &= 254;
    for (i = 2; i >= 0; i--) {
        Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 9, 0, -8);
        Call1((void (*)())Func_02001f10, 134);
        Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 9, 0, 8);
    }
    object1 = Object_GetById(9);
    object1->actionFlags |= 1;
    Call1((void (*)())Battle_WaitMode0, 30);
    Value3((s32 (*)())Func_02001e48, 7, 0x102, 30);
    Call3((void (*)())Func_02001e28, 9, 0, 10);
    Value3((s32 (*)())Func_02001e48, 7, 0x107, 30);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Value3((s32 (*)())Func_02001e48, 9, 0x108, 30);
    Call3((void (*)())Func_02001e28, 9, 0, 10);
    Call3((void (*)())ObjectMotion_SetAngleToward, 10, 9, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
    Call3((void (*)())Func_02001e28, 10, 0, 10);
    Call3((void (*)())ObjectMotion_SetAngleToward, 9, 10, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
    Call3((void (*)())Func_02001e28, 9, 0, 10);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Value3((s32 (*)())Func_02001e48, 9, 0x107, 20);
    Call2((void (*)())Func_02001e40, 9, 0xc000);
    Call3((void (*)())Func_02001e28, 9, 0, 10);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x4000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 9, 0, 10);
    Value3((s32 (*)())Func_02001e48, 10, 0x102, 30);
    Call3((void (*)())Func_02001e28, 10, 0, 10);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 7, 0, 16);
    Call2((void (*)())Func_02001e90, 24, 1);
    Call2((void (*)())Func_02001e98, 7, 8);
    ((void (*)())Func_02001eb0)();
    Value3((s32 (*)())Func_02001e48, 9, 0x101, 30);
    Call3((void (*)())ObjectMotion_SetAngleToward, 9, 7, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 10, 7, 0);
    object2 = Object_GetById(9);
    Call2((void (*)())ObjectDispatch_SetSingleChildFlags, (s32)object2, 0);
    Value2((s32 (*)())ObjectMotion_EnableActionAndSetCallback, 9, gData_0200349c);
    Call1((void (*)())Field_DispatchTypeHandler, 1);
    ((void (*)())Func_02001ea0)();
    ((void (*)())Func_02001ea8)();
    Call3((void (*)())Func_02001e28, 11, 0, 10);
    Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 9);
    object3 = Object_GetById(9);
    object3->scaleX = 0x10000;
    object4 = Object_GetById(9);
    object4->scaleY = 0x10000;
    Call1((void (*)())Battle_WaitMode0, 4);
    object5 = Object_GetById(9);
    object5->motionFlags = 2;
    object6 = Object_GetById(9);
    object6->actionFlags &= 254;
    Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 9, -8, 0);
    Value2((s32 (*)())ObjectMotion_EnableActionAndSetCallback, 9, gData_02003418);
    Call1((void (*)())Object_RefreshSelectorById, 9);
    Call1((void (*)())Func_02001f10, 127);
    Call1((void (*)())Battle_WaitMode0, 60);
    object7 = Object_GetById(9);
    object7->actionFlags = (object7->actionFlags | 1);
    Call3((void (*)())ObjectMotion_Launch, 9, 6, 0);
    Call1((void (*)())Battle_WaitMode0, 3);
    Call2((void (*)())Func_02001ef0, 9, 0);
    object8 = Object_GetById(9);
    Call2((void (*)())ObjectDispatch_SetSingleChildFlags, (s32)object8, 1);
    Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x4000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 10, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 10, 0xc000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value3((s32 (*)())Func_02001e48, 10, 0x100, 30);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())ObjectMotion_Launch, 9, 6, 20);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_SetAngleToward, 9, 7, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    object9 = Object_GetById(10);
    object9->actionFlags &= 254;
    object10 = Object_GetById(9);
    object10->actionFlags &= 254;
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 9, -16, 16);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x4000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())ObjectMotion_Launch, 9, 6, 20);
    Call1((void (*)())Battle_WaitMode0, 20);
    object11 = Object_GetById(9);
    object11->actionFlags = (object11->actionFlags | 1);
    Value2((s32 (*)())ObjectMotion_EnableActionAndSetCallback, 9, gData_020033c8);
    Call2((void (*)())Object_LinkObjectAndSetCallback, 4, 9);
    Call2((void (*)())Object_LinkObjectAndSetCallback, 11, 9);
    Call3((void (*)())Func_02001e28, 9, 0, 10);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_SetAngleToward, 10, 7, 0);
    Call2((void (*)())Object_LinkObjectAndSetCallback, 4, 10);
    Call2((void (*)())Object_LinkObjectAndSetCallback, 11, 10);
    Call3((void (*)())Func_02001e28, 10, 0, 10);
    Value3((s32 (*)())Func_02001e48, 7, 0x105, 30);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Value3((s32 (*)())Func_02001e48, 10, 0x102, 30);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 10, -16, 16);
    Call3((void (*)())Func_02001e28, 10, 0, 1);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 10, -16, 8);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 10, 0, 10);
    object12 = Object_GetById(10);
    object12->actionFlags |= 1;
    Value2((s32 (*)())ObjectMotion_EnableActionAndSetCallback, 10, gData_020033c8);
    Call1((void (*)())Battle_WaitMode0, 40);
    Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 4);
    Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 11);
    Call1((void (*)())Object_RefreshSelectorById, 10);
    Call3((void (*)())Func_02001db8, 9, 0, 0);
    Call3((void (*)())Func_02001db8, 10, 0, 0);
    Call2((void (*)())Object_SetModeById, 8, 6);
    Call2((void (*)())Func_02001e08, 8, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xc000, 0);
    Call1((void (*)())ObjectMotion_WaitForAnimationChange, 8);
    Call2((void (*)())Object_AttachWorkTargetToObject, 4, 1);
    ((void (*)())Func_02001e68)();
    Call3((void (*)())Object_LinkPair, 11, 4, 0);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value2((s32 (*)())UiText_OpenMessageAtObject, 11, 0);
    result13 = Value2((s32 (*)())Inventory_PromptAndSetObjectMode, 4, 0);
    if (result13 != 0) goto L_02008e40;
    Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
    Call3((void (*)())Func_02001e28, 11, 0, 10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    goto L_02008e64;
L_02008e40:
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 4);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Call3((void (*)())Func_02001e28, 11, 0, 10);
L_02008e64:
    Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Func_02001e28, 11, 0, 10);
    Call2((void (*)())Object_SetModeById, 4, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 3);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Object_SetModeById, 11, 2);
    object14 = Object_GetById(4);
    if (object14 == 0) goto L_02008eb2;
    Value3((s32 (*)())ObjectMotion_ResetAndSetPosition, 11, *(s16 *)((u8 *)object14 + 10), *(s16 *)((u8 *)object14 + 18));
L_02008eb2:
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 11);
    Call3((void (*)())Func_02001db8, 11, 0, 0);
    Call1((void (*)())GameFlag_SetBit, 0x911);
    ((void (*)())Func_02001d58)();
    return;
}

void Func_02000ed8(void)
{
    ((void (*)())Func_02001d50)();
    Call1((void (*)())Func_02001eb8, 0);
    Call1((void (*)())Func_02001e18, (s32)MsgMadraInteriors29);
    Call3((void (*)())ObjectMotion_ArmCallback, 11, 0x4000, 0);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    ((void (*)())Func_02001d58)();
    return;
}

void Func_02000f0c(void)
{
    ((void (*)())Func_02001d50)();
    Call1((void (*)())Func_02001eb8, 0);
    Call1((void (*)())Func_02001e18, (s32)MsgMadraInteriors30);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x8000, 0);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xc000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 7, 0, 10);
    ((void (*)())Func_02001d58)();
    return;
}

void Func_02000fe0(void)
{
    Call2((void (*)())Func_02001e90, 24, 1);
    Call2((void (*)())Func_02001e98, 7, 8);
    ((void (*)())Func_02001eb0)();
    Call1((void (*)())Field_DispatchTypeHandler, 1);
    ((void (*)())Func_02001ea0)();
    ((void (*)())Func_02001ea8)();
    return;
}
