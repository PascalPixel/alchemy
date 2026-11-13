/// @file
/// The scene code of overlay 0x68a, in ROM order.

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "field_event.h"
#include "event_inline.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

void Func_02000080(s32 id);
void Func_02000838(void);
void Func_020019f4(void);
void Func_02001a24(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY, s32 layer);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void MapCopy(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                           s32 destY, s32 layer) {
  Func_02001a24(srcX, srcY, width, height, destX, destY, layer);
}
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "module/scene/ancient_lemuria.h"
#include "module/scene/scene.h"

void Func_02000038(void) { Func_080d7788(12, 1, 13); }

void Func_02000048(struct Actor *object) {
  struct FieldSprite *sprite = object->sprite;
  s16 time = object->timer++;

  sprite->rotation = Iwram_MulQ16(Math_Sine(time << 12), 0x100);
  if ((time & 15) == 0)
    Sound_Play(155);
}

void Func_02000080(s32 id) {
  struct Actor *object = Actor_GetById(id);
  struct FieldSprite *sprite = object->sprite;

  WORD_AT(object, 108) = 0;
  object->timer = 0;
  sprite->rotation = 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_020000b0(void) { WORD_AT(Ram_HeapSlots->mapWork, 240) = 0x02ee0000; }

void Func_020000c4(void) { WORD_AT(Ram_HeapSlots->mapWork, 240) = 0x01600000; }

void Func_020000d4(void) {}

void Func_020000d8(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 8 && z == 36)
    GameFlag_SetBit(0x961);
  if (GameFlag_Test(0x961))
    Func_02000838();
}

/// One hop of object 12 by (dx, dz).
static inline void Hop(s32 dx, s32 dz) {
  Actor_Jump(12, 5, 0);
  Actor_CommitPositionAndActivate(12, dx, dz);
  Event_Wait(3);
}

/// Once (flag 0x96c): object 12 spins for 30 frames while it grows from half
/// scale, then hops thirteen times while the leader follows, and object 13
/// takes Func_02000048 as its update.
void Func_02000110(void) {
  struct Actor *object;
  s32 i;

  /* FAKEMATCH(wrapper): scene.h's MapCopy, whose inline parameters keep each
   * stack argument in its own register; as a direct call, 8 of 273
   * instructions differ in tla-en. */
  MapCopy(7, 13, 1, 1, 5, 13, 0);
  if (Event_GetFlag(0x96c))
    return;
  object = Actor_GetById(12);
  Event_SetFlag(0x96c);
  Event_SetFlag(0x96e);
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Actor_SetPosition(12, 176 << 15, 216 << 16);
  Actor_Jump(12, 9, 0);
  object->scaleY = 0x8000;
  object->scaleX = 0x8000;
  for (i = 29; i >= 0; i--) {
    object->facing += 0x1000;
    {
      /* FAKEMATCH(pin): the limit's first half stays in r2; as a plain
       * local, 30 of 273 instructions differ in tla-en. */
      register s32 limit asm("r2") = 255;

      /* FAKEMATCH(barrier): the barrier starts the 0xffff limit before the
       * scale's load and finishes it after; without it, 13 of 273
       * instructions differ in tla-en. */
      asm("" : "+r"(limit) : "r"(object->scaleX));
      if (object->scaleX <= (limit << 8) + 255)
        object->scaleX += 0xa00;
    }
    object->scaleY = object->scaleX;
    wait(1);
  }
  object->facing = 0x3000;
  object->scaleY = 0x10000;
  object->scaleX = 0x10000;
  Actor_SetSingleChildFlags(Actor_GetById(12), 1);
  Actor_SetSpritePriority(12, 1);
  Event_Wait(8);
  Actor_Jump(12, 9, 0);
  Actor_SetPositionAndReset(12, 120, 200);
  Actor_GetById(12)->priorityFlags |= 1;
  Actor_SetPositionAndReset(12, 136, 200);
  Event_Wait(3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(16, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(16, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Hop(0, 16);
  Hop(16, 0);
  Hop(16, 0);
  Hop(16, 0);
  Hop(16, 0);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_GetById(13)->update = Func_02000048;
  Actor_GetById(13)->timer = 0;
}

void Func_020003bc(void) { MapCopy(9, 9, 2, 3, 8, 9, 0); }

void Func_020003dc(void) { MapCopy(38, 0, 5, 2, 12, 7, 0); }

void Func_020003fc(void) { MapCopy(39, 2, 4, 3, 14, 11, 0); }

void Func_0200041c(void) { MapCopy(39, 2, 3, 3, 21, 9, 0); }

void Func_0200043c(void) { MapCopy(39, 0, 4, 3, 25, 8, 0); }

void Func_0200045c(void) { Func_02000080(13); }

/// Once flag 0x96c is set and 0x96d is not: sets 0x96d, clears 0x96e, launches
/// object 12 from 22 pixels up at half size and turns it an eighth of a turn a
/// frame for 24 frames while it grows back to full size, then leaves it facing
/// 0x3000.
void Func_02000468(void) {
  struct Actor *object;
  s32 i;
  struct Actor *p;

  if (Event_GetFlag(0x96d))
    return;
  if (!GameFlag_Test(0x96c))
    return;
  object = Actor_GetById(12);
  GameFlag_SetBit(0x96d);
  GameFlag_ClearBit(0x96e);
  Func_02000080(13);
  Actor_SetPosition(12, 270 << 16, 184 << 16);
  object->y = 22 << 16;
  Actor_Jump(12, 11, 0);
  object->scaleY = 0x8000;
  object->scaleX = 0x8000;
  Actor_SetSpeed(12, 0x23333, 0x11999);
  Actor_WalkTo(12, 312, 184);
  for (i = 23; i >= 0; i--) {
    {
      s32 turn = 0x2000;

      object->facing += turn;
    }
    /* FAKEMATCH(temporary): the object read again through p after the
     * turn, so the scale is reloaded after its test; as object throughout, 13
     * of 85 instructions differ in tla-en. */
    p = object;
    if (p->scaleX <= 0xffff)
      p->scaleX += 0xa00;
    p->scaleY = p->scaleX;
    wait(1);
  }
  p->facing = 0x3000;
  p->scaleY = 0x10000;
  p->scaleX = 0x10000;
  Actor_WaitForMoveById(12);
}

void Func_02000534(void) {
  struct Actor *object;

  if (!GameFlag_Test(0x961) || GameFlag_Test(0x96a)) {
    Sound_Play(123);
    Import_EventSetExitRequest(2);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldActor_LaunchFromLeader(11, 0, -16, 0x5000);
    Actor_ArmCallback(4, FACING_NORTH, 0);
    Actor_WaitForMoveById(11);
    Event_Wait(20);
    if (GameFlag_Test(7)) {
      Event_SetMessageId(MsgLemuriaStayExploring);
      Event_ShowMessageThenWait(11, 0, 10);
    } else {
      Event_SetMessageId(MsgLemuriaStayExploringAlternate);
      Event_ShowMessageThenWait(11, 0, 10);
    }
    Actor_SetSpeed(11, 0x13333, 0x9999);
    Actor_SetModeById(11, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != NULL)
      Actor_ResetAndSetPosition(11, *(s16 *)((u8 *)object + 10),
                                *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(11);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_WalkToAndWait(4, 168, 832);
    Event_End();
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// Rotates colours 2-14 of palettes 7 and 8 by one entry every four frames,
/// and selects colour 0x7f from the table.
s32 Func_02000630(void) {
  if ((*Ram_FrameCount & 3) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x050000fc;
    index = 14;
  first:
    palette = (u16 *)0x050000e0;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    *(volatile u16 *)0x050000e2 = saved;
    saved = *(volatile u16 *)0x0500011c;
    index = 14;
  second:
    palette = (u16 *)0x05000100;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)0x05000102 = saved;
  }
  {
    s16 color = gData_02001c30[(*Ram_FrameCount >> 2) % 12];

    *(volatile u16 *)0x050000fe = color;
    return color;
  }
}

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 513;
  Task_Add(Func_02000630, 0xc80);
  if (gMainRoutineWork.entrance != 1)
    GameFlag_ClearBit(0x12f);
  else
    GameFlag_SetBit(0x12f);
  if (GameFlag_Test(0x961)) {
    Actor_SetPosition(8, 136 << 16, 584 << 16);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  }
  if (Event_GetFlag(0x96d))
    Actor_SetPosition(12, 312 << 16, 184 << 16);
  if (gMainRoutineWork.entrance == 1 && !GameFlag_Test(0x96d) &&
      GameFlag_Test(0x96c)) {
    *(void (**)(void))(((u8 *)Actor_GetById(13)) + 108) = Func_02000048;
    *(u16 *)(((u8 *)Actor_GetById(13)) + 100) = 0;
  }
  Actor_SetSingleChildFlags(Actor_GetById(13), 0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 entrance = gMainRoutineWork.entrance;

  if (entrance == 1)
    GameFlag_SetBit(0x960);
  else if (entrance == 2)
    GameFlag_ClearBit(0x960);
  else if (entrance == 3)
    GameFlag_ClearBit(0x960);
  else if (entrance == 4)
    GameFlag_SetBit(0x960);
  else if (entrance == 5)
    GameFlag_SetBit(0x960);
  if (GameFlag_Test(0x960)) {
    Func_0802b450(38, 47, 26, 12, 8, 47);
    Func_0802b450(102, 47, 26, 12, 72, 47);
    Map_CopyCellAttributeRect(38, 47, 26, 12, 8, 47);
  }
  return 0;
}

void Func_02000830(void) { Func_020019f4(); }

void Actor_MoveToObject();

void Func_02000838(void) {
  struct Actor *leader;
  s32 result;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaRecognizePsynergy);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(4, 0x101, 40);
  Event_ShowMessageThenWait(10, 0, 10);
  Func_020019f4();
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_Wait(20);
  Actor_SetModeById(4, 1);
  Event_Wait(20);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_ArmCallback(4, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_ArmCallback(4, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_WalkByAndWait(4, 0, -16);
  Event_Wait(10);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(536), 1);
  Func_020019e4();
  Event_Wait(20);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_CommitPositionAndActivate(9, 24, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_WalkByAndWait(10, -24, 0);
  Actor_ArmCallback(10, 0x5000, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(10);
  Event_OpenMessage(10, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    Event_Wait(30);
    Actor_StartRepeatedMotion(10, 3);
    FieldEffect_SetAttached(10, 0x102);
    Event_Wait(40);
    Event_ShowMessageThenWait(10, 0, 10);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
  } else {
    Event_Wait(40);
    Actor_StartRepeatedMotion(10, 3);
    FieldEffect_SetAttached(10, 0x102);
    Event_Wait(40);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    Event_ShowMessageThenWait(10, 0, 0);
    Actor_Jump(10, 6, 15);
    Actor_Jump(10, 6, 23);
    Event_Wait(10);
  }
  Actor_ShowEmote(9, 0x102, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(10, 0x102, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  FieldActor_LaunchFromLeader(11, 16, -16, 0xb000);
  Actor_WaitForMoveById(11);
  Event_Wait(20);
  Actor_ShowEmote(11, 0x101, 30);
  FieldEvent_ShowMessageThenWait(0x200b, 0, 10);
  Actor_FaceDirection(9, 0x3000, 0);
  Event_Wait(10);
  Actor_ShowEmote(9, 0x102, 40);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(11, 0x5000, 0);
  Event_Wait(50);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(11, 0xb000, 0);
  Event_Wait(30);
  Actor_SetModeById(4, 4);
  Motion_SetModeAndWaitAnimation(11, 4);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_MoveToObject(7, 4);
  Actor_GetById(7)->scaleX = 0;
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ArmCallback(9, FACING_EAST, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Event_Wait(45);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Event_Wait(25);
  Actor_GetById(7)->scaleX = 0x10000;
  FieldActor_LaunchFromLeader(7, 0, -16, 0xc000);
  Actor_WaitForMoveById(7);
  Event_Wait(20);
  Actor_FaceDirection(9, 0x5000, 0);
  Actor_ShowEmote(9, 0x100, 0);
  Actor_ShowEmote(10, 0x100, 30);
  FieldMessage_ShowAtActor(9, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(0x800a, 0, 0);
  Message_CloseFinishedWindows();
  Event_Wait(10);
  Actor_ShowEmote(7, 0x108, 40);
  Actor_WalkByAndWait(9, -8, 8);
  Actor_FaceDirection(9, 0x3000, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(10, -8, 8);
  Actor_FaceDirection(10, 0x5000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(7, 0x107, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(7, 0x3000, 0);
  Event_Wait(35);
  Actor_FaceDirection(11, 0x5000, 0);
  Actor_ArmCallback(4, FACING_NORTHEAST, 0);
  Event_Wait(50);
  Actor_FaceDirection(11, 0xb000, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetModeById(11, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(25);
  Actor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 0x102);
  Actor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  Event_Wait(40);
  Actor_ArmCallback(9, FACING_EAST, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Event_Wait(40);
  Actor_FaceDirection(7, 0x3000, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_WalkByAndWait(7, 0, -16);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_StartRepeatedMotion(9, 2);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(10);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Event_Wait(30);
  Actor_ShowEmote(9, 0x102, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(10, 0x102, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(7, 0x109, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(11, 0x101, 30);
  FieldEvent_ShowMessageThenWait(0x200b, 0, 10);
  Actor_Jump(9, 6, 15);
  Actor_Jump(9, 6, 23);
  Actor_ShowEmote(9, 0x107, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpritePriority(11, 2);
  Actor_Jump(11, 6, 15);
  Actor_Jump(11, 6, 23);
  Actor_ShowEmote(11, 0x103, 30);
  FieldEvent_ShowMessageThenWait(0x200b, 0, 10);
  Actor_ShowEmote(10, 0x107, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(11, 0x102, 30);
  FieldEvent_ShowMessageThenWait(0x200b, 0, 10);
  Actor_ArmCallback(7, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_StartRepeatedMotion(11, 3);
  Actor_SetAttachedEffect(11, 0x102);
  Event_Wait(50);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(11, 0x100, 30);
  FieldEvent_ShowMessageThenWait(0x200b, 0, 10);
  Actor_WalkByAndWait(11, 0, -16);
  Actor_FaceDirection(11, 0xb000, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200b, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ArmCallback(9, FACING_EAST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(20);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Event_Wait(30);
  Actor_ShowEmote(9, 0x108, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(10, 0x108, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(11, 0x108, 0);
  Actor_ShowEmote(7, 0x108, 0);
  Actor_ShowEmote(4, 0x108, 55);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(9, 0xd000, 0);
  Actor_FaceDirection(10, 0xb000, 0);
  Event_Wait(20);
  Actor_WalkBy(9, 0, -16);
  Actor_WalkByAndWait(10, 0, -16);
  Actor_FaceDirection(11, FACING_WEST, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(11, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200b, 0, 10);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 0x102);
  Actor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Event_Wait(30);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(512), 1);
  Func_020019e4();
  Event_Wait(10);
  Actor_ShowEmote(9, 0x109, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(11, 0xb000, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_ShowEmote(11, 0x102, 55);
  Actor_ShowEmote(10, 0x109, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ArmCallback(9, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_ArmCallback(11, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetSpritePriority(4, 2);
  Actor_WalkByAndWait(4, 8, -20);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_ShowEmote(7, 0x108, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(20);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_ArmCallback(11, 0xb000, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(20);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  FieldMessage_ShowAtActor(9, 0);
  if ((s8)gMainRoutineWork.counter274 <= 4) {
    result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
    if (result == 0) {
      Event_Wait(25);
      Actor_ArmCallback(11, 0x5000, 0);
      Event_Wait(30);
      Motion_SetModeAndWaitAnimation(11, 3);
      Event_Wait(10);
      Event_ShowMessageThenWait(0x200b, 0, 10);
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    } else {
      Event_Wait(35);
      Actor_ShowEmote(9, 0x102, 0);
      Actor_ShowEmote(10, 0x102, 50);
      Actor_ArmCallback(11, 0x5000, 0);
      Event_Wait(30);
      Actor_ShowEmote(11, 0x103, 30);
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
      Event_ShowMessageThenWait(0x200b, 0, 10);
    }
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count += 2;
  } else {
    /* FAKEMATCH(pin): the count pinned to r2, as the ROM keeps it, with the
     * work pointer in r3; as a plain local, local-alloc gives the count r3,
     * 14 of 1671 instructions differ in tla-en. */
    register u32 n asm("r2");

    n = ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count;
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count = n + 2;
    result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
    if (result == 0) {
      Event_Wait(25);
      Actor_FaceDirection(11, 0x5000, 0);
      Actor_ArmCallbackAndRefresh(7, 0x3000);
      Event_Wait(10);
      Func_080d47b4(7, 0x102, 50);
      Motion_SetVarCbAndRefresh(11, 2);
      Event_Wait(10);
      Event_ShowMessageThenWait(0x200b, 0, 10);
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    } else {
      Event_Wait(35);
      Actor_FaceDirection(11, 0x5000, 0);
      Actor_ArmCallbackAndRefresh(7, 0x3000);
      Event_Wait(10);
      Actor_ShowEmote(11, 0x103, 40);
      Actor_Jump(11, 6, 15);
      Actor_Jump(11, 6, 23);
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
      Event_ShowMessageThenWait(0x200b, 0, 10);
    }
    Actor_ArmCallbackAndRefresh(7, FACING_NORTH);
    Event_Wait(10);
  }
  Actor_WalkByAndWait(7, 0, -10);
  Event_Wait(15);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(11, 0xb000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(7, 0x102, 40);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(25);
  Camera_FollowActor(10, 1);
  Actor_WalkBy(10, 0, -32);
  Event_Wait(10);
  Actor_CommitPositionAndActivate(9, 16, 0);
  Actor_WalkBy(9, 0, -32);
  Actor_WaitForMoveById(10);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  Actor_WaitForMoveById(9);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Event_Wait(30);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(536), 1);
  Func_020019e4();
  Event_Wait(20);
  Actor_WalkByAndWait(11, 0, -10);
  Actor_ArmCallback(11, FACING_WEST, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x200b, 0, 10);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(7, 0x109, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(7, 0x102, 40);
  Event_ShowMessageThenWait(7, 0, 10);
  Motion_SetVarCbAndRefresh(7, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_ShowEmote(11, 0x101, 30);
  Event_ShowMessageThenWait(0x200b, 0, 10);
  Actor_ShowEmote(7, 0x105, 50);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ArmCallback(7, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(7, 0, -32);
  Actor_FaceDirection(11, 0xb000, 0);
  Event_Wait(10);
  Actor_ArmCallback(7, 0x3000, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(7, 0x108, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_ArmCallback(11, 0x5000, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_SetSpeed(4, 0xcccc, 0x6666);
  Actor_SetSpeed(11, 0xcccc, 0x6666);
  Actor_WalkByAndWait(4, -8, -16);
  Actor_WalkBy(11, 0, -10);
  Actor_WalkByAndWait(4, 0, -16);
  Actor_WaitForMoveById(11);
  Actor_ArmCallback(11, 0xb000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(20);
  Actor_SetModeById(4, 3);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(20);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetModeById(7, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(7, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Actor_SetSpeed(11, 0x13333, 0x9999);
  Actor_SetModeById(11, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(11, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(11);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Event_End();
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 100, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(201),
    SCENE_EXIT_IF(1, 201, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 200, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 202, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 201, 1, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {289, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(584), FACING_SOUTH, 2, 0},
    {157, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(504), 0x3000, 0, 0},
    {157, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(504), 0x5000, 0, 0},
    {56, CONDITION_ALWAYS, 1, -PIXELS(56), 0, -PIXELS(56), FACING_NORTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, -PIXELS(56), 0, -PIXELS(56), FACING_NORTH, 2, 0},
    {244, 0x51, 1, -PIXELS(56), 0, -PIXELS(56), FACING_SOUTH, 0, 0},
    {289, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Func_02000534)},
    {33, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Func_020000b0)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(Func_020000c4)},
    {0x602, 22, CONDITION_ALWAYS, EVENT(Func_02000830)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Func_02000038)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x961, (u32)(Func_020000d4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x961, EVENT(Func_020000d8)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_02000110)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_020003bc)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(Func_020003dc)},
    {0x50008905, 33, CONDITION_ALWAYS, EVENT(Func_020003fc)},
    {0x50008905, 34, CONDITION_ALWAYS, EVENT(Func_0200041c)},
    {0x50008905, 35, CONDITION_ALWAYS, EVENT(Func_0200043c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_TREMOR, PSYNERGY_PHASE_BEFORE_CAST), 13, 0x196e, EVENT(Func_0200045c)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 13, 0x196e, EVENT(Func_02000468)},
    {TRIGGERS_END},
};

// clang-format off
s16 gData_02001c30[] = {
    0x5f0c, 0x62c8, 0x6684, 0x6a40, 0x6a00, 0x69c0,
    0x6980, 0x69c0, 0x6a00, 0x6a40, 0x6684, 0x62c8,
};
// clang-format on
