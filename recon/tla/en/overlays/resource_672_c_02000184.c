// 26 October 2026, under -mthumb-slow-loads: differ 10 of 895: the rise loop's
// count, the palette address register and, at the end, the native issues a load
// one slot later after a store, where alchemy-gcc issues it at once; an empty
// asm after the rise loop's wait() leaves 6; alchemy permute (5 to 6
// minutes) and a second asm or block does not move it.
// Draft, differ 14 of 895: the long scene in the Gabomba Statue's chamber
// (message MsgOhYoungWitch on): the statue object rises 40 pixels, actors 9,
// 10, 12, 13 and 14 move, emote and speak, the screen flashes white, the party
// gets item 242 (Func_02001d48 lifts the statue object above the leader), two
// yes-or-no questions count up the event work's halfword 0x1c4, the screen
// fades out to MsgCarriedTheMessage, flags 0x8ff, 0x86b and 0x8f2 change and
// the retreat and saved places become Kibombo. Left: the native counts up in
// the rise loop after building wait()'s argument, keeps the palette address
// in r5, and reloads two literal pool values in another order.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "call.h"
#include "scene_ids.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern struct Actor *gData_02002058;
extern const u8 MsgOhYoungWitch[];
extern const u8 gData_02001f78[];
extern const u8 gData_02001f9c[];
extern const u8 gData_02001fe4[];
extern const u8 gData_0200200c[];
extern const u8 MsgCarriedTheMessage[];
void Func_02001da0(void);
void Func_02001ee8();
void Func_02001f10();
void Actor_SetModeById();
void Func_02001ec0();
void Func_02001eb8();
s32 Func_02001ec8();
void wait();
s32 Func_02001e88();
void Actor_SetSpeedParameters();
void Actor_SetPositionAndReset();
void Actor_ArmCallback();
void Func_02001e20();
void Func_02001e48();
void Func_02000100(void);
void Func_02001e60();
void Func_02001e90();
void Func_02001e70();
void Actor_Jump();
s32 Actor_GetById();
#define OBJECT(id) ((struct Actor *)Value1(Actor_GetById, (id)))
void Func_02001e10();
void FieldEvent_Wait();
void Actor_SetMovementTypeAndWaitById();
void Func_02001d88();
void Func_02001e58();
void Motion_SetModeAndWaitAnimation();
void Actor_SetPartAttribute();
void Func_02001094(void);
void Func_02001d48();
void Func_02001d50();
void PartyInventory_GiveItem();
void Func_02001d30();
s32 FieldMessage_ShowAtActor();
s32 Event_AskYesNoWithActorAnimation();
void Actor_SetMovementTypeById();
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001d78(void);
void Func_02001d80(void);
void GameFlag_SetBit();
void GameFlag_ClearBit();
void Func_02001eb0();

void Func_02000184(void) {
  struct Actor *object, *leader;
  u32 i;

  Func_02001da0();
  Call1(Func_02001ee8, 0);
  Call1(Func_02001f10, 26);
  Call1(Func_02001f10, 212);
  Call2(Actor_SetModeById, 8, 3);
  Call2(Func_02001ec0, 0x10000, 2);
  Call2(Func_02001eb8, 0x10000, 2);
  Call2(Func_02001ec0, 0x10000, 1);
  Call2(Func_02001eb8, 0x403108, 1);
  Value1(Func_02001ec8, 40);
  Call1(wait, 40);
  Call1(Func_02001f10, 197);
  Call1(wait, 1);
  if (gData_02002058 != 0) {
    i = 0;
  rise:
    if (i == 20)
      Value3(Func_02001e88, 10, 0x100, 0);
    gData_02002058->y += 0x8000;
    i++;
    wait(1);
    if (i <= 79)
      goto rise;
    gData_02002058->motionFlags = 4;
  }
  Call2(Actor_SetModeById, 8, 1);
  Call3(Actor_SetSpeedParameters, gAdventure.currentOwner, 0xcccc,
        0x6666);
  Call3(Actor_SetPositionAndReset, gAdventure.currentOwner, 358, 108);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
  Call2(Func_02001e20, 9, gAdventure.currentOwner);
  Call1(wait, 1);
  Call3(Actor_SetSpeedParameters, 9, 0xcccc, 0x6666);
  Call3(Actor_SetPositionAndReset, 9, 364, 124);
  Call3(Actor_ArmCallback, 9, 0xd000, 0);
  Call1(Func_02001e48, MsgOhYoungWitch);
  Func_02000100();
  Call3(Actor_ArmCallback, 10, 0, 20);
  Call3(Actor_ArmCallback, 10, 0x8000, 40);
  Call3(Actor_ArmCallback, 10, 0, 20);
  Call2(Func_02001e60, 0x200a, 0);
  Call2(Func_02001e90, 9, 258);
  Call2(Func_02001e60, 9, 0);
  Call2(Func_02001e70, 10, 0xb000);
  Call3(Actor_Jump, 10, 4, 20);
  Call2(Func_02001e60, 0x200a, 0);
  Func_02000100();
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x4000, 0);
  Call3(Actor_ArmCallback, 9, 0xb000, 40);
  Func_02000100();
  Value3(Func_02001e88, gAdventure.currentOwner, 0x100, 0);
  Value3(Func_02001e88, 9, 0x100, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x2000, 0);
  Call3(Actor_ArmCallback, 9, 0x3000, 0);
  Call2(Actor_SetModeById, 8, 3);
  Call3(Actor_ArmCallback, 10, 0x3000, 20);
  object = OBJECT(14);
  object->angle = OBJECT(10)->angle;
  Call1(wait, 1);
  Call2(Func_02001e20, 14, 10);
  Call3(Func_02001e10, 10, 0, 0);
  Call1(Func_02001f10, 110);
  Call2(Actor_SetModeById, 14, 5);
  Call1(FieldEvent_Wait, 60);
  Call1(FieldEvent_Wait, 20);
  Call3(Actor_ArmCallback, 14, 0xb000, 0);
  Call2(Actor_SetModeById, 8, 1);
  Call1(FieldEvent_Wait, 40);
  Call2(Actor_SetModeById, 8, 3);
  Call1(Func_02001f10, 110);
  Call2(Actor_SetModeById, 14, 6);
  Call1(FieldEvent_Wait, 60);
  Call2(Actor_SetModeById, 14, 1);
  Call1(FieldEvent_Wait, 20);
  Call2(Actor_SetModeById, 8, 1);
  Call1(FieldEvent_Wait, 40);
  Func_02000100();
  Call3(Actor_Jump, 14, 2, 10);
  Call3(Actor_Jump, 14, 4, 10);
  Call2(Func_02001e60, 0x200e, 0);
  Call2(Actor_SetMovementTypeAndWaitById, 14, gData_02001f78);
  Call2(Func_02001e60, 0x200e, 0);
  Call3(Actor_SetSpeedParameters, 14, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 14, 392, 164);
  Value3(Func_02001e88, 14, 264, 0);
  Call3(Actor_Jump, 14, 6, 20);
  Call2(Func_02001e60, 0x600e, 0);
  Call3(Actor_SetPositionAndReset, 14, 392, 220);
  Call3(Func_02001e10, 14, 0, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0, 0);
  Call3(Actor_ArmCallback, 9, 0, 0);
  OBJECT(12)->motionFlags = 0;
  Call2(Actor_SetMovementTypeAndWaitById, 12, gData_02001f9c);
  Call1(FieldEvent_Wait, 20);
  Func_02000100();
  Value3(Func_02001e88, gAdventure.currentOwner, 257, 0);
  Value3(Func_02001e88, 9, 257, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
  Call3(Actor_ArmCallback, 9, 0xd000, 0x50);
  Func_02000100();
  Call1(FieldEvent_Wait, 20);
  Func_02000100();
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x2000, 20);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x8000, 40);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x2000, 20);
  Call2(Actor_SetModeById, 8, 3);
  Call2(Func_02001ec0, 0x10000, 2);
  Call2(Func_02001eb8, 0x10003, 2);
  Value1(Func_02001ec8, 10);
  Call1(wait, 10);
  *(u16 *)0x0500021e = 0x7fff;
  Call1(wait, 1);
  Call3(Func_02001d88, (s16)(*(u16 *)(EVENT_WORK + 0x1c4))++, 0, 2);
  Call2(Func_02001eb8, 0x10000, 2);
  Value1(Func_02001ec8, 20);
  Call1(wait, 20);
  Call2(Actor_SetModeById, 8, 1);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
  Value3(Func_02001e88, gAdventure.currentOwner, 0x100, 40);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x4000, 0);
  Call3(Actor_ArmCallback, 9, 0xb000, 20);
  Func_02000100();
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
  Call3(Actor_ArmCallback, 9, 0xd000, 0);
  Func_02000100();
  Value3(Func_02001e88, gAdventure.currentOwner, 257, 0);
  Value3(Func_02001e88, 9, 257, 40);
  Func_02000100();
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0, 0);
  Call3(Actor_ArmCallback, 9, 0, 0);
  Call2(Actor_SetMovementTypeAndWaitById, 12, gData_02001fe4);
  Func_02000100();
  Call2(Func_02001e90, gAdventure.currentOwner, 258);
  Call2(Func_02001e90, 9, 258);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
  Call3(Actor_ArmCallback, 9, 0xd000, 20);
  Func_02000100();
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x4000, 0);
  Call3(Actor_ArmCallback, 9, 0xb000, 20);
  Func_02000100();
  Call2(Func_02001e70, 9, 0xd000);
  Call2(Func_02001e60, 9, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
  Value3(Func_02001e88, 13, 257, 40);
  Func_02000100();
  Call3(Func_02001e58, 9, 0, 40);
  Func_02000100();
  Call3(Actor_Jump, 9, 4, 10);
  Call2(Func_02001e60, 9, 0);
  Func_02000100();
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x4000, 0);
  Call2(Func_02001e70, 9, 0xb000);
  Call2(Motion_SetModeAndWaitAnimation, 9, 3);
  Func_02000100();
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
  Call2(Func_02001e70, 9, 0xd000);
  Call2(Actor_SetModeById, 9, 4);
  Call2(Func_02001e60, 9, 0);
  Func_02000100();
  Call1(Func_02001f10, 0);
  Call1(Func_02001f10, 220);
  Call2(Actor_SetModeById, 8, 3);
  Call2(Actor_SetPartAttribute, Value1(Actor_GetById, 13), 7);
  Call3(Func_02001e10, 13, 0x1a80000, 0x680000);
  Call2(Func_02001eb8, 0x7fff, 0);
  Value1(Func_02001ec8, 40);
  Call1(wait, 40);
  Func_02001094();
  Call3(Func_02001e10, 13, 0, 0);
  Call1(FieldEvent_Wait, 1);
  Call2(Func_02001eb8, 0x10000, 2);
  Call2(Func_02001eb8, 0x403108, 1);
  Value1(Func_02001ec8, 60);
  Call1(wait, 60);
  Func_02000100();
  Call2(Func_02001eb8, 0x10000, 1);
  Value1(Func_02001ec8, 20);
  Call1(wait, 20);
  gData_02002058->motionFlags = 3;
  Call1(FieldEvent_Wait, 10);
  Call1(Func_02001f10, 309);
  Call1(FieldEvent_Wait, 40);
  Call1(Func_02001f10, 25);
  Call1(FieldEvent_Wait, 40);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x2000, 0);
  Call2(Func_02001e70, 9, 0xb000);
  Call2(Func_02001e60, 9, 0);
  Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
  Call3(Actor_ArmCallback, 9, 0xd000, 0);
  Call3(Actor_SetPositionAndReset, gAdventure.currentOwner, 392, 104);
  Call2(Func_02001e70, gAdventure.currentOwner, 0xc000);
  Call2(Actor_SetModeById, gAdventure.currentOwner, 28);
  if (gData_02002058 != 0) {
    leader = OBJECT(gAdventure.currentOwner);
    object = gData_02002058;
    object->motionFlags = 0;
    Call4(Func_02001d48, (s32)object, leader->x, leader->y + 0x240000,
          leader->z);
    Call1(Func_02001d50, (s32)gData_02002058);
  }
  Call2(PartyInventory_GiveItem, 242, 0);
  if (gData_02002058 != 0)
    Call1(Func_02001d30, (s32)gData_02002058);
  Call2(Actor_SetModeById, gAdventure.currentOwner, 1);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x6000, 0);
  Call3(Actor_ArmCallback, 9, 0xd000, 40);
  Value2(FieldMessage_ShowAtActor, 9, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, gAdventure.currentOwner, 0) ==
      0) {
    Call2(Actor_SetModeById, 9, 4);
    Call2(Func_02001e60, 9, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Call2(Actor_SetModeById, 9, 3);
    Call2(Func_02001e60, 9, 0);
  }
  Call3(Actor_SetPositionAndReset, 9, 376, 124);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_02001e60, 9, 0);
  Call2(Func_02001e70, 9, 0xd000);
  Value2(FieldMessage_ShowAtActor, 9, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, gAdventure.currentOwner, 0) ==
      0) {
    Call2(Func_02001e60, 9, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Call2(Func_02001e60, 9, 0);
  }
  Call2(Actor_SetModeById, 9, 4);
  Call2(Func_02001e60, 9, 0);
  Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
  Call2(Func_02001e60, 9, 0);
  Call2(Motion_SetModeAndWaitAnimation, 9, 3);
  Call2(Func_02001e60, 9, 0);
  Call2(Func_02001e60, 9, 0);
  Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
  Call2(Actor_SetMovementTypeById, 9, gData_0200200c);
  Call1(FieldEvent_Wait, 20);
  Call2(Actor_SetMovementTypeById, gAdventure.currentOwner,
        gData_0200200c);
  Call1(FieldEvent_Wait, 40);
  Call2(Func_02001eb8, 0, 0);
  Value1(Func_02001ec8, 60);
  Call1(wait, 0x40);
  *(s32 *)(EVENT_WORK + 0x1ac) = 513;
  *(s32 *)(EVENT_WORK + 0x1b4) = 10;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call2(Func_02001eb8, 0x10000, 0);
  *(s32 *)(EVENT_WORK + 0x1b4) = 1;
  Value1(Func_02001ec8, 1);
  Func_02001d78();
  Call3(Func_02001d88, MsgCarriedTheMessage, 0, 3);
  Func_02001d80();
  Call1(GameFlag_SetBit, 0x8ff);
  Call1(GameFlag_ClearBit, 0x86b);
  Call1(GameFlag_ClearBit, 0x8f2);
  gAdventure.retreatScene = (s32)&SceneId_Kibombo1;
  gAdventure.retreatEntrance = 1;
  gAdventure.savedScene = (s32)&SceneId_Kibombo3;
  gAdventure.savedEntrance = 6;
  Call1(Func_02001eb0, 3);
}
