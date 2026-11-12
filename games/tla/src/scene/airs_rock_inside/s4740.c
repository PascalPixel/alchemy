/// @file
/// Scene code of overlay 0x664, in ROM order, from s4740's offset up to the next object the overlay links.

#include "airs_rock_inside.h"

#include "scene_tables.h"
#include "airs_rock_inside.h"
#include "call.h"
#include "text/msg_ids.h"

void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02004534(void);
#include "event_work.h"
#include "actor.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

s32 Func_02005ea4();
s32 Func_02005e74();
s32 Func_02005eac();
s32 Func_02005e6c();
s32 Func_02005dcc();
s32 Func_02005dd4();
s32 Func_02004138();
s32 Party_LearnAbility();
s32 Actor_TurnToFaceEachOther();
s32 Actor_SetSpeedParameters();
s32 Actor_WaitForMoveById();
s32 Actor_ResetAndSetPosition();
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

static inline void Call0(void (*f)()) {
  /* FAKEMATCH(wrapper): the call layer without arguments, through which the two
   * -1s before the call are negated in this order; as a direct call, 2 of 202
   * instructions differ in tla-en. */
  f();
}

/// Checking the tablet shows its message; the first time (flag 0x8d4 clear)
/// the scene with actor 6 follows, as Func_020030cc of Magma Rock does with
/// the Blaze tablet.
void Func_02004534(void) {
  struct FieldActor *object;

  Func_02005e14();
  Func_02005f2c(0);
  FieldMessage_Show(MsgCheckedTheTablet, 1);
  if (Value1(GameFlag_Test, 0x8d4) != 0) {
    Func_02005e1c();
  } else {
    Func_02005ea4((s32)MsgCheckedTheTablet);
    EVENT_WORK->messageId++;
    Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0xb333,
          0x5999);
    Call3(Actor_SetSpeedParameters, 6, 0xb333, 0x5999);
    Func_02005e74(6, gMainRoutineWork.currentOwner);
    FieldEvent_Wait(1);
    Call3(Actor_ResetAndSetPositionInMode2, gMainRoutineWork.currentOwner, 256,
          136);
    Actor_ResetAndSetPositionInMode2(6, 272, 136);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Actor_WaitForMoveById(6);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_ArmCallback(6, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Call2(Func_02005eac, 6, 0);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(30);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 256, 152);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_ArmCallback(6, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetModeById(6, 3);
    FieldEvent_Wait(30);
    Call3(Actor_ArmCallback, 6, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Call3(Actor_SetPositionAndReset, 6, 264, 137);
    Actor_ArmCallback(6, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    FieldMessage_Show(MsgCheckedTheTablet + 2, 1);
    Call3(Func_02005dcc, 0x50000, 0x70000, 0x10000);
    Call3(Func_02005dcc, -1, -1, 0xe666);
    /* FAKEMATCH(wrapper): the call through Call0's pointer, which negates the
     * two -1s before it in this order; as a direct call, 2 of 202 instructions
     * differ in tla-en. */
    Call0(Func_02005dd4);
    Call2(Func_02004138, 8, 6);
    Party_LearnAbility(6, 0x90);
    GameFlag_SetBit(0x8d4);
    Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 6, 0);
    FieldEvent_Wait(10);
    Actor_SetModeById(6, 3);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Call3(Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
    Actor_SetModeById(6, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_ResetAndSetPosition(6, FIELD_AT(object, s16, 10),
                                FIELD_AT(object, s16, 18));
    Actor_WaitForMoveById(6);
    Func_02005e6c(6, 0, 0);
    Actor_SetActionCallback((struct Actor *)Actor_GetById(8), 9);
    Func_02005e1c();
  }
}

void Func_02004740(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 107);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_020047b0(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 280, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_02004818(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_02004880(void) {}

void Func_020007b4(void);

void Func_02004884(void) {
  Func_02005e14();
  Func_02005f2c(0);
  if (Actor_GetById(8)->x >> 19 == 65)
    GameFlag_SetBit(771);
  else
    GameFlag_ClearBit(771);
  Func_02005e1c();
}

void Func_020048bc(void) { Func_020007b4(); }

#include "types.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"

extern u16 gData_0200621c[];
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_020006cc(struct Actor *object);

void Func_020048c4(void) {
  struct Actor *first = 0, *second = 0, *leader;
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramBlockCacheEntry *entry;
  u16 *pair;
  u32 id, i;
  s32 busy;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  pair = gData_0200621c;
  if (*pair == 0xffff)
    goto found;
search:
  id = *pair;
  first = Actor_GetById(id);
  if (first->x >> 20 == leader->x >> 20 && first->z >> 20 == leader->z >> 20) {
    second = Actor_GetById(id - ((id & 1) << 1) + 1);
    goto found;
  }
  pair += 3;
  if (*pair != 0xffff)
    goto search;
found:
  busy = ((u8 *)first)[0x63] | ((u8 *)second)[0x63];
  if (busy != 0)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  firstSprite = first->animation;
  secondSprite = second->animation;
  second->motionFlags = busy;
  second->y += -0x1c0000;
  wait(1);
  entry = &gResourceTableEntries[secondSprite->vramBlock];
  second->watchedObject = (void *)(entry->offset + 0x6010000);
  entry = &gResourceTableEntries[firstSprite->vramBlock];
  first->watchedObject = (void *)(entry->offset + 0x6010000);
  Dma_Fill32(busy, second->watchedObject, entry->size);
  Dma_Set(first->watchedObject, second->watchedObject, 0x840000c0,
          (volatile u32 *)0x040000d4);
  first->watchedObject = (void *)((u8 *)first->watchedObject + 0x300);
  second->watchedObject = (void *)((u8 *)second->watchedObject + 0x300);
  leader->motionFlags = busy;
  i = 0;
step:
  if (i <= 4) {
    second->y += 0x53333;
    Dma_Set(first->watchedObject, second->watchedObject, 0x84000030,
            (volatile u32 *)0x040000d4);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x100);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x100);
  }
  leader->y += -0x38000;
  leader->terrainHeight = leader->y;
  first->scaleY += -0x1f00;
  wait(1);
  i++;
  if (i != 8)
    goto step;
  leader->motionFlags = 3;
  first->scaleY = 0x10000;
  second->y = second->terrainHeight;
  GameFlag_SetBit(first->action);
  GameFlag_ClearBit(second->action);
  Func_02005d5c(first, ((struct FieldSprite *)first->animation)->frame + 1);
  Func_02005d5c(second, ((struct FieldSprite *)second->animation)->frame - 1);
  Func_020006cc(first);
  Func_020006cc(second);
  FieldEvent_Wait(10);
  Func_02005e1c();
}

