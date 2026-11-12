/// @file
/// The scene code of overlay 0x685 that its scene tables belong to, in ROM
/// order; the rest of the overlay is in champa_cliff_caves/.

#include "event_inline.h"
#include "champa_cliff_caves/scene.h"

void Func_02000038(struct RiseObject *object) {
  object->scale += 0x1000;
  if (object->scale >= 0x10000) {
    object->scale = 0x10000;
    object->update = 0;
  }
  object->scaleCopy = object->scale;
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

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "scene_ids.h"

extern u32 gData_0200375c[];
extern u32 gData_020037a4[];
extern u32 gData_0200381c[];
extern u32 gData_02003864[];
extern u32 gData_020038ac[];
extern u32 gData_0200390c[];
extern u32 gData_02003984[];
extern u32 gData_020039cc[];
extern u32 gData_02003aec[];
extern u32 gData_02003b94[];
extern u32 gData_02003c0c[];
extern u32 gData_02003c84[];

u32 *Func_02000070(void) {
  s32 scene;

  if (GameFlag_Test(0x97e)) {
    scene = gMainRoutineWork.scene;
    if (scene == (s32)&SceneId_Ov685Room1)
      return gData_02003aec;
    if (scene == (s32)&SceneId_Ov685Room2)
      return gData_02003b94;
    if (scene == (s32)&SceneId_Ov685Room3)
      return gData_02003c0c;
    if (scene == (s32)&SceneId_Ov685Room4)
      return gData_02003c84;
  } else {
    if (GameFlag_Test(0x97f)) {
      scene = gMainRoutineWork.scene;
      if (scene == (s32)&SceneId_Ov685Room1)
        return gData_020038ac;
      if (scene == (s32)&SceneId_Ov685Room2)
        return gData_0200390c;
      if (scene == (s32)&SceneId_Ov685Room3)
        return gData_02003984;
      if (scene == (s32)&SceneId_Ov685Room4)
        return gData_020039cc;
    } else {
      scene = gMainRoutineWork.scene;
      if (scene == (s32)&SceneId_Ov685Room1)
        return gData_0200375c;
      if (scene == (s32)&SceneId_Ov685Room2)
        return gData_020037a4;
      if (scene == (s32)&SceneId_Ov685Room3)
        return gData_0200381c;
      if (scene == (s32)&SceneId_Ov685Room4)
        return gData_02003864;
    }
  }
  return gData_0200375c;
}

void Func_0200016c(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Func_0200345c();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterOnColumn_(id, 2, -8);
  else
    Actor_OffsetPositionAndResetMotion_(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003464();
}

void Func_020001f4(s32 door) { Func_0200016c(&gData_02003d10, door, 0); }

extern const struct WarpTarget gData_02003d18;

static __inline__ void Actor_CommitPositionAndActivate_(s32 actor, s32 dx,
                                                        s32 dz) {
  Actor_CommitPositionAndActivate(actor, dx, dz);
}

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                 s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as a direct call, 4 of 112
   * instructions differ in tla-en (Func_020004d4). */
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

void Func_02000208(void) {
  const struct WarpTarget *warp;
  s32 msg;

  warp = &gData_02003d10;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 168, 424);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_EAST);
  Sound_Play(158);
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Actor_GetById(8)->motionFlags = 0;
  Func_020034d4_(8, 184 << 16, 204 << 17);
  Actor_SetSpeedParameters_(8, 0x1cccc, 0xe666);
  Actor_CommitPositionAndActivate(8, 0, 16);
  Actor_ArmCallbackAndRefresh(8, FACING_WEST);
  msg = (s32)MsgHasMyGrandson;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(8, 0);
  Actor_SetSpeedParameters_(8, 0xcccc, 0x6666);
  Actor_CommitPositionAndActivate_(8, 0, -14);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  warp = &gData_02003d18;
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Sound_Play(158);
}

void Func_02000304(s32 actor) {
  s32 msg = (s32)MsgWhatYouWant;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_02000350(s32 actor) {
  struct Actor *object;

  Event_SetMessageId((s32)MsgAllRightI);
  Event_ShowMessageAndWait(actor, 0);
  object = Actor_GetById(actor);
  Func_02003624(object, Actor_GetById(gMainRoutineWork.currentOwner));
}

void Func_0200038c(s32 actor) {
  Event_SetMessageId((s32)MsgAwComeOn);
  Event_ShowMessageAndWait(actor, 0);
  Func_02003534_(actor, 0xd000);
  Actor_SetActionCallback(Actor_GetById(actor), 8);
}

void Func_020003bc(void) {
  Func_0802b450(41, 97, 1, 4, 41, 77);
  Map_CopyCellAttributeRect(41, 33, 1, 4, 41, 13);
}

void Func_020003ec(void) {
  struct Actor *object;
  s32 x;
  s32 z;

  Func_0802b450(50, 97, 1, 4, 41, 77);
  Map_CopyCellAttributeRect(46, 33, 1, 4, 41, 13);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  x = object->x >> 20;
  z = object->z >> 20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov685Room4 && x == 41 &&
      (u32)(z - 13) <= 4)
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                           PIXELS(664), PIXELS(280));
}

extern s32 gData_02003d20[];
extern s32 gData_02003e7c[];
extern s32 gData_02003ffc[];
extern s32 gData_02004104[];

void Func_02000464(s32 a) { Func_02003474(a, 326, 0x978); }

s32 *Func_02000478(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov685Room1)
    return gData_02003d20;
  if (scene == (s32)&SceneId_Ov685Room2)
    return gData_02003e7c;
  if (scene == (s32)&SceneId_Ov685Room3)
    return gData_02003ffc;
  if (scene == (s32)&SceneId_Ov685Room4)
    return gData_02004104;
  return gData_02003d20;
}

#include "version.h"

struct EventTransition {
  u8 unk000[0x1ac];
  s32 type;
};

void Func_02001774(void);
void Func_02001868(void);
void Func_020035ac(s32 a);

static __inline__ void Func_0200361c_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as a direct call, 2 of 112
   * instructions differ in tla-en (Func_020004d4). */
  Func_080d3b28(a, b);
}

s32 Func_020004d4(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

#if !TLA_PATCH_1
  ((struct EventTransition *)Ram_HeapSlots->eventWork)->type = 516;
#else
  ((struct EventTransition *)Ram_HeapSlots->eventWork)->type = 513;
#endif
  if (party->scene != (s32)&SceneId_Ov685Room1)
    Actor_GetById(party->currentOwner)->priorityFlags |= 32;
  Func_020035ac(0);
  if (party->scene == (s32)&SceneId_Ov685Room4) {
    if (party->entrance == 98)
      Func_02001774();
    if (party->entrance == 99) {
      party->secondary = 23;
      party->entrance = 23;
      Func_02001868();
    }
    if (GameFlag_Test(0x977) && !GameFlag_Test(0x978)) {
      Func_0200361c_(10, 326);
      Func_020034d4_(10, 0x2980000, 0xe80000);
      Actor_GetById(10)->motionFlags = 4;
    }
    if (GameFlag_Test(0x977))
      Func_020034d4_(8, 0x2980000, 0xc80000);
  } else if (party->scene == (s32)&SceneId_Ov685Room3) {
    if (GameFlag_Test(0x97e)) {
      Actor_SetModeById(8, 11);
      Actor_SetSingleChildFlags(Actor_GetById(8), 0);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.
struct ActorSolid {
  u8 unk00[0x5a];
  u8 solid : 8;
};

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_020005e8(s32 actor) {
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 648, 200);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_EAST);
  Event_Wait(5);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 6);
  Event_Wait(7);
  Func_080d3b28(10, actor);
  Actor_SetHorizontalPositionWithTerrain(10, 648 << 16, 196 << 16);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Actor_CommitPositionAndActivate(gMainRoutineWork.currentOwner, -16, -8);
  Event_Wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_EAST);
  Event_Wait(10);
}

void Func_0200067c(s32 value) {
  Actor_SetSpeedParameters(8, 0xcccc, 0x6666);
  Actor_GetById(8)->actionFlags &= 0xfe;
  Actor_CommitPositionAndActivate(8, 0, 6);
  Event_Wait(1);
  ((struct ActorSolid *)Actor_GetById(8))->solid |= 1;
  Event_Wait(10);
  Actor_SetModeById(8, 8);
  Event_Wait(15);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  Event_Wait(15);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  if (value == 0) {
    Actor_GetById(8)->actionFlags &= 0xfe;
    Actor_CommitPositionAndActivate_(8, 0, -6);
    Event_Wait(1);
    ((struct ActorSolid *)Actor_GetById(8))->solid |= 1;
    Event_Wait(10);
  }
}

/// Fades five background colours, from palette entry 114, from one set to
/// another over the given number of frames, one step a frame.
void Func_02000734(u16 *from, u16 *to, s32 frames) {
  s32 step;

  for (step = 0; step <= frames; step++) {
    s32 offset;
    s32 left;

    for (offset = 0, left = 4; left >= 0; left--) {
      s32 red, green, blue, toRed, toGreen, toBlue;

      red = *(u16 *)(offset + (s32)from) & 31;
      green = *(u16 *)(offset + (s32)from) >> 5 & 31;
      blue = *(u16 *)(offset + (s32)from) >> 10 & 31;
      toRed = *(u16 *)(offset + (s32)to) & 31;
      toGreen = *(u16 *)(offset + (s32)to) >> 5 & 31;
      toBlue = *(u16 *)(offset + (s32)to) >> 10 & 31;
      red += step * (toRed - red) / frames;
      green += step * (toGreen - green) / frames;
      blue += step * (toBlue - blue) / frames;
      // A once-only block: its end keeps the step after the colour's store.
      do {
        s32 color = blue << 10 | green << 5 | red;

        *(u16 *)(0x050000e4 + offset) = color;
      } while (0);
      offset += 2;
    }
    wait(1);
  }
}

struct EventCounters {
  u8 unk000[0x1c4];
  u16 count;
};

void Func_02000804(s32 actor) {
  Func_0200345c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgThisLooksLike);
  Func_020005e8(actor);
  Func_02003534_(8, 0x3000);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Func_080d47b4(8, 257, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Func_080d47b4(8, 261, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Func_080d47b4(8, 256, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(8);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_0200067c(0);
  Actor_ArmCallbackAndRefresh(8, FACING_WEST);
  Event_Wait(10);
  Func_080d47b4(8, 258, 30);
  /* FAKEMATCH(signature): called through a type returning s32, which gives the
   * native's argument order (r1 before r0); as the void call scene_call.h
   * declares, which the other scenes match with, 2 of 144 instructions differ
   * in tla-en. */
  ((s32 (*)(s32, s32))FieldMessage_ShowAtActor)(8, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Event_ShowMessageThenWait(8, 0, 5);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
  } else {
    Event_Wait(35);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    Event_ShowMessageThenWait(8, 0, 5);
  }
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_02003534_(8, 0x5000);
  Func_02003464();
}

void Func_02000978(s32 actor) {
  Func_0200345c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAhHaveYou);
  Func_020005e8(actor);
  Func_02003534_(8, 0x3000);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Func_080d47b4(8, 258, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Func_080d47b4(8, 264, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_0200067c(0);
  Actor_ArmCallbackAndRefresh(8, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_02003534_(8, 0x5000);
  Func_02003464();
}
