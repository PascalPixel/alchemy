#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "event_inline.h"

struct ContigoEventCount {
  u8 unk000[0x1c4];
  u16 refusals;
};

extern u8 gSceneState[];

void Field_SetDestination(s32 scene, s32 entrance);
void Func_0200403c(s32 value, s32 digits);
u32 Func_0200404c(void);
void Func_0200406c(void);
void Func_02004074(void);
void Func_0200422c(void);

/// The dice game attendant: the fee is ten coins a level.
void Func_02001abc(void) {
  u32 price = Func_0200404c() * 10;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  if ((u32)gMainRoutineWork.coins < price) {
    Event_SetMessageId((s32)MsgContigo03);
    FieldMessage_ShowAtActor(14, 0);
    return;
  }
  Event_SetMessageId((s32)MsgContigo01);
  Func_0200403c(price, 5);
  FieldMessage_ShowAtActor(14, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    /* FAKEMATCH(pin): the scene state's address pinned to r3, so that it
     * loads before the coins; as a plain store, 6 of 72 instructions differ in
     * tla-en. */
    register u32 *state __asm__("r3") = (u32 *)gSceneState;

    *state = gMainRoutineWork.coins;
    Func_0200422c();
    Event_ShowMessageAndWait(14, 0);
    GameFlag_SetBit(0x109);
    GameFlag_SetBit(0x9e5);
    wait(1);
    Field_SetDestination(0x1fd, 0);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov694Room3, 19);
  } else {
    ((struct ContigoEventCount *)Ram_HeapSlots->eventWork)->refusals++;
    Event_ShowMessageAndWait(14, 0);
  }
  Func_02004074();
}

/// The Super Lucky Dice attendant: the same check without the fee shown.
void Func_02001b84(void) {
  u32 price = Func_0200404c() * 10;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  if ((u32)gMainRoutineWork.coins < price) {
    Event_SetMessageId((s32)MsgContigo34);
    FieldMessage_ShowAtActor(14, 0);
    return;
  }
  Event_SetMessageId((s32)MsgContigo23);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    /* FAKEMATCH(pin): the scene state's address pinned to r3, so that it
     * loads before the coins; as a plain store, 6 of 69 instructions differ in
     * tla-en. */
    register u32 *state __asm__("r3") = (u32 *)gSceneState;

    *state = gMainRoutineWork.coins;
    Func_0200422c();
    Event_ShowMessageAndWait(8, 0);
    GameFlag_SetBit(0x109);
    GameFlag_SetBit(0x9e5);
    wait(1);
    Field_SetDestination(0x1fb, 0);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov694Room3, 18);
  } else {
    ((struct ContigoEventCount *)Ram_HeapSlots->eventWork)->refusals++;
    Event_ShowMessageAndWait(8, 0);
  }
  Func_02004074();
}
