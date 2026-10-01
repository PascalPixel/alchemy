/* TLA-EN Idejima departure attempt: 2088 emitted bytes versus the
   complete 2088-byte native owner; 493 differing byte positions
   excluding 186 external relocation words at native offsets.
   The English message number is the ordinary immediate trial.
   Calls and address pools have not been linked in this draft comparison;
   this result makes no complete-match or credit claim. */

#include "TYPES.H"

#include "PARTY_STATE.H"
#include "FIELD_EVENT.H"

struct SceneEventWork {
    u8 unknown_000[0x1c4];
    u16 message_id;
};

#include "RAM_BUFFER.H"

extern const u8 Idejima_ActorDepartureScript[];
void GameFlag_SetBit();
void Battle_WaitMode0();
s32 Inventory_PromptAndSetObjectMode();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_EnableActionAndSetCallback();
void Object_RefreshSelectorById();
void ObjectMotion_EnableActionAndResetMotion();
void Object_SetActionCallbackAndRefreshById();
void ObjectMotion_ResetAndSetPositionInMode2();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Func_02003238();
void Motion_SetModeAndWaitAnimation();
void ObjectMotion_Launch();
s32 UiText_OpenMessageAtObject();
void ObjectMotion_ArmCallback();
s32 Func_020032c0();
void Func_020032c8();
void Func_020032d0();
void Motion_CamBounds();
void Func_02003310();
void Object_LinkObjectAndSetCallback();
void Func_02003320();

#include "CALL.H"

void Idejima_RunAlexDeparture(void)
{
    const u8 *script;

    Engine_EventBegin();
    Func_02003310(0);
    Call2(Func_020032d0, 0x9999, 0x1333);
    Call4(Motion_CamBounds, 0x6c0000, -1, 0x2d00000, 1);
    Call3(ObjectMotion_SetSpeedParameters, gPartyState.current_owner, 0xcccc, 0x6666);
    Call3(ObjectMotion_SetPositionAndReset, gPartyState.current_owner, 86, 0x2c8);
    Func_02003238(8, gPartyState.current_owner);
    Task_Wait(1);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0xcccc, 0x6666);
    Call3(ObjectMotion_SetPositionAndReset, 8, 104, 0x2c8);
    ObjectMotion_ArmCallback(gPartyState.current_owner, 0, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x8000, 20);
    Call1(Engine_EventSetMessage, 0x16de);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    ObjectMotion_ArmCallback(8, 0, 40);
    Call3(ObjectMotion_ArmCallback, 8, 0x5000, 20);
    ObjectMotion_ArmCallback(8, 0, 0);
    Call3(ObjectMotion_SetPositionAndReset, gPartyState.current_owner, 104, 0x2d8);
    ObjectMotion_ArmCallback(gPartyState.current_owner, 0, 20);
    Call3(ObjectMotion_ArmCallback, gPartyState.current_owner, 0x2000, 20);
    ObjectMotion_ArmCallback(gPartyState.current_owner, 0, 0);
    Engine_EventShowMessage(8, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x3000, 0);
    Call3(ObjectMotion_ArmCallback, gPartyState.current_owner, 0xc000, 20);
    Engine_ActorSetAnimation(8, 3);
    Value2(Engine_EventChooseYesNo, 8, 0);
    Value3(Func_020032c0, 8, 0x106, 40);
    Engine_EventShowMessage(8, 0);
    Call2(Func_02003238, 5, gPartyState.current_owner);
    Func_02003238(6, gPartyState.current_owner);
    Task_Wait(1);
    Call3(ObjectMotion_SetSpeedParameters, 6, 0xcccc, 0x6666);
    Call3(ObjectMotion_SetSpeedParameters, 5, 0xcccc, 0x6666);
    Call3(ObjectMotion_ResetAndSetPositionInMode2, 5, 86, 0x2c8);
    Call3(ObjectMotion_SetPositionAndReset, 6, 86, 0x2d8);
    Call3(ObjectMotion_ArmCallback, 6, 0xe000, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(5);
    Engine_ActorSetAnimation(5, 1);
    Call3(ObjectMotion_ArmCallback, 5, 0x4000, 0);
    Engine_EventShowMessage(5, 0);
    Call3(ObjectMotion_ArmCallback, gPartyState.current_owner, 0x8000, 0);
    ObjectMotion_ArmCallback(6, 0, 40);
    Call3(ObjectMotion_ArmCallback, 6, 0xe000, 0);
    Call3(ObjectMotion_ArmCallback, gPartyState.current_owner, 0xc000, 0);
    Engine_ActorSetAnimation(8, 4);
    Value2(UiText_OpenMessageAtObject, 0x2008, 0);
    ObjectMotion_ArmCallback(6, 0, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x2000, 0);
    if (Value2(Inventory_PromptAndSetObjectMode, gPartyState.current_owner, 0) == 0) {
        Motion_SetModeAndWaitAnimation(8, 4);
        Call2(Engine_EventShowMessage, 0x2008, 0);
        ((struct SceneEventWork *)Ram_HeapSlots->event_work)->message_id += 1;
    } else {
        ((struct SceneEventWork *)Ram_HeapSlots->event_work)->message_id += 1;
        Motion_SetModeAndWaitAnimation(8, 3);
        Call2(Engine_EventShowMessage, 0x2008, 0);
    }
    ObjectMotion_ArmCallback(5, 0, 0);
    Engine_EventShowMessage(5, 0);
    Value3(Func_020032c0, 6, 0x101, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xc000, 0);
    Engine_EventShowMessage(6, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    Call3(Func_020032c0, gPartyState.current_owner, 0x101, 40);
    Call3(ObjectMotion_ArmCallback, 8, 0x3000, 0);
    Engine_ActorSetAnimation(8, 4);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    Value3(Func_020032c0, 5, 0x100, 20);
    Engine_EventShowMessage(5, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x8000, 20);
    Motion_SetModeAndWaitAnimation(8, 3);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    ObjectMotion_ArmCallback(6, 0, 20);
    Engine_EventShowMessage(6, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x2000, 0);
    Value2(UiText_OpenMessageAtObject, 5, 0);
    if (Value2(Inventory_PromptAndSetObjectMode, 8, 0) == 0) {
        Call3(ObjectMotion_ArmCallback, 8, 0x3000, 20);
        Motion_SetModeAndWaitAnimation(8, 3);
        Call2(Engine_EventShowMessage, 0x2008, 0);
        ((struct SceneEventWork *)Ram_HeapSlots->event_work)->message_id += 1;
    } else {
        ((struct SceneEventWork *)Ram_HeapSlots->event_work)->message_id += 1;
        Value3(Func_020032c0, 8, 0x103, 0);
        Call3(ObjectMotion_ArmCallback, 8, 0x4000, 20);
        Call2(Engine_EventShowMessage, 0x2008, 0);
        Func_02003320();
    }
    Value3(Func_020032c0, 5, 0x100, 0);
    Engine_EventShowMessage(5, 0);
    Value3(Func_020032c0, 8, 0x100, 40);
    Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    Value3(Func_020032c0, 6, 0x101, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xe000, 40);
    Engine_EventShowMessage(6, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x4000, 20);
    Engine_EventShowMessage(5, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x3000, 0);
    Call3(ObjectMotion_ArmCallback, gPartyState.current_owner, 0xc000, 20);
    Value2(UiText_OpenMessageAtObject, 0x2008, 0);
    if (Value2(Inventory_PromptAndSetObjectMode, gPartyState.current_owner, 0) == 0) {
        Value3(Func_020032c0, 6, 0x103, 0);
        ObjectMotion_ArmCallback(6, 0, 20);
        Engine_EventShowMessage(6, 0);
        ((struct SceneEventWork *)Ram_HeapSlots->event_work)->message_id += 1;
    } else {
        ((struct SceneEventWork *)Ram_HeapSlots->event_work)->message_id += 1;
        Value3(Func_020032c0, 6, 0x108, 0);
        ObjectMotion_ArmCallback(6, 0, 20);
        Engine_EventShowMessage(6, 0);
    }
    Value3(Func_020032c0, 8, 0x100, 0);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xe000, 0);
    Motion_SetModeAndWaitAnimation(6, 3);
    Engine_EventShowMessage(6, 0);
    Value3(Func_020032c0, 5, 0x101, 40);
    Engine_EventShowMessage(5, 0);
    Object_LinkObjectAndSetCallback(gPartyState.current_owner, 6);
    Object_LinkObjectAndSetCallback(8, 6);
    Object_LinkObjectAndSetCallback(5, 6);
    Call3(ObjectMotion_SetSpeedParameters, 6, 0x6666, 0x3333);
    Call3(ObjectMotion_SetPositionAndReset, 6, 86, 0x2ec);
    Battle_WaitMode0(40);
    Value3(Func_020032c0, 6, 0x109, 80);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
    ObjectMotion_EnableActionAndResetMotion(8);
    ObjectMotion_EnableActionAndResetMotion(5);
    Motion_SetModeAndWaitAnimation(6, 3);
    Battle_WaitMode0(40);
    Engine_ActorSetAnimation(5, 4);
    Engine_EventShowMessage(5, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xc000, 0);
    Object_RefreshSelectorById(6);
    Engine_ActorSetAnimation(6, 4);
    Engine_EventShowMessage(6, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x3000, 0);
    Call3(ObjectMotion_ArmCallback, gPartyState.current_owner, 0xc000, 40);
    Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
    Call3(ObjectMotion_ArmCallback, gPartyState.current_owner, 0x6000, 20);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    Motion_SetModeAndWaitAnimation(6, 3);
    Engine_EventShowMessage(6, 0);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(5, 3);
    Motion_SetModeAndWaitAnimation(gPartyState.current_owner, 3);
    Call3(ObjectMotion_ArmCallback, 6, 0xe000, 0);
    Engine_EventShowMessage(6, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x3000, 0);
    Value2(Engine_EventChooseYesNo, 0x2008, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xa000, 40);
    Engine_EventShowMessage(6, 0);
    ObjectMotion_ArmCallback(5, 0, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x8000, 20);
    Engine_ActorSetAnimation(6, 4);
    Engine_EventShowMessage(6, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x4000, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
    Value3(Func_020032c0, 8, 0x105, 0);
    Value3(Func_020032c0, 5, 0x105, 40);
    Engine_EventShowMessage(5, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xc000, 0);
    Engine_EventShowMessage(6, 0);
    Call2(Func_020032c8, 8, 0x102);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    Value3(Func_020032c0, 6, 0x108, 20);
    Call3(ObjectMotion_ArmCallback, 6, 0xe000, 20);
    Engine_EventShowMessage(6, 0);
    Motion_SetModeAndWaitAnimation(8, 3);
    Engine_EventShowMessage(5, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x2000, 0);
    Value2(UiText_OpenMessageAtObject, 5, 0);
    if (Value2(Inventory_PromptAndSetObjectMode, gPartyState.current_owner, 0) == 0) {
        Engine_ActorSetAnimation(5, 3);
        Engine_EventShowMessage(5, 0);
        ((struct SceneEventWork *)Ram_HeapSlots->event_work)->message_id += 1;
    } else {
        ((struct SceneEventWork *)Ram_HeapSlots->event_work)->message_id += 1;
        Engine_ActorSetAnimation(5, 4);
        Engine_EventShowMessage(5, 0);
    }
    Value3(Func_020032c0, 8, 0x109, 40);
    Motion_SetModeAndWaitAnimation(6, 4);
    Battle_WaitMode0(20);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    Motion_SetModeAndWaitAnimation(6, 3);
    Call3(ObjectMotion_ArmCallback, 8, 0x8000, 20);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    ObjectMotion_Launch(8, 4, 20);
    Call2(Engine_EventShowMessage, 0x2008, 0);
    Call3(ObjectMotion_SetSpeedParameters, 6, 0xcccc, 0x6666);
    script = Idejima_ActorDepartureScript;
    ObjectMotion_EnableActionAndSetCallback(8, script);
    Call2(ObjectMotion_EnableActionAndSetCallback, 5, script);
    Object_SetActionCallbackAndRefreshById(6, script);
    Call1(GameFlag_SetBit, 0x867);
    Engine_EventEnd();
}
