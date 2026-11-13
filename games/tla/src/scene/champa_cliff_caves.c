/// @file
/// The scene code of overlay 0x685, in ROM order.

// ---- games/tla/src/scene/champa_cliff_caves.c ----

#include "field/event_inline.h"
#include "types.h"
#include "flags.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "scene_call.h"
#include "text/msg_ids.h"

struct WarpTarget {
  const void *scene;
  u16 x;
  u16 z;
};

struct RiseObject {
  u8 unk00[0x18];
  s32 scale;
  s32 scaleCopy;
  u8 unk20[0x4c];
  s32 update;
};

/* VIEW(champa_cliff_caves): routine gData_02003d10 as this part declared it;
 * the joined file has another. */
extern const struct WarpTarget
    gData_02003d10__champa_cliff_caves __asm__("gData_02003d10");
#define gData_02003d10 gData_02003d10__champa_cliff_caves

void ChampaCliffCaves_OpenDoorAndEnter(const struct WarpTarget *warp, s32 door,
                                       s32 mode);
void Event_PickUpItem(s32 a, s32 b, s32 c);
void Actor_WatchActor(struct Actor *object, struct Actor *target);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column, s32 offset) {
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                           s32 dz) {
  Actor_SetDestinationOffsetById(id, dx, dz);
}

static __inline__ void Func_02003534_(s32 actor, s32 angle) {
  Actor_FaceDirectionById(actor, angle);
}
#include "field/event.h"
#include "event_work.h"
#include "field/field.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "module/field/event_commands.h"

/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern struct ActorPlacement gData_0200375c[];
extern struct ActorPlacement gData_020037a4[];
extern struct ActorPlacement gData_020038ac[];
extern struct ActorPlacement gData_0200390c[];
extern struct ActorPlacement gData_02003984[];
extern struct ActorPlacement gData_02003aec[];
extern struct ActorPlacement gData_02003b94[];
extern struct ActorPlacement gData_02003c0c[];
extern struct Trigger gData_02003e7c[];
extern struct Trigger gData_02003ffc[];
extern struct Trigger gData_02004104[];

void Func_02001774(void);
void Func_02001868(void);
void Func_020005e8(s32 value);
void Func_0200067c(s32 mode);

void FieldCamera_WaitForArrival(void);

void Func_020032a4(void);
void Func_02000804(s32 item);
void Func_02000978(s32 item);
void Func_020029f8(s32 id, s32 mode);
void Func_02002c88(void);
void Func_02002de4(void);
void Func_02002d34(void);

void Func_02000208(void);

void Func_020003bc(void);

void Func_020003ec(void);
void Func_0200115c(void);

extern s16 gData_02003ce4[];
extern s16 gData_02003cfa[];

struct FlashSprite;

void FieldEffect_StartBlankOverlay(s32 a);
void Field_StartColorTransition(s32 a, s32 b);
extern char ResourceId_FireColumn;

#include "module/scene/scene.h"
#include "module/system/system.h"
#include "graphics/resource.h"
#include "system/system.h"

#define DEFINE_CHAMPA_CLIFF_CAVES_GROW(name, n1)                               \
  void name(struct RiseObject *object) {                                       \
    object->scale += n1;                                                       \
    if (object->scale >= 0x10000) {                                            \
      object->scale = 0x10000;                                                 \
      object->update = 0;                                                      \
    }                                                                          \
    object->scaleCopy = object->scale;                                         \
  }

DEFINE_CHAMPA_CLIFF_CAVES_GROW(Func_02000038, 0x1000)

#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "scene_ids.h"

extern struct ActorPlacement gData_0200381c[];
extern struct ActorPlacement gData_02003864[];
/* VIEW(champa_cliff_caves): routine gData_020039cc as this part declared it;
 * the joined file has another. */
extern u32 gData_020039cc__champa_cliff_caves[] __asm__("gData_020039cc");
#define gData_020039cc gData_020039cc__champa_cliff_caves
/* VIEW(champa_cliff_caves): routine gData_02003c84 as this part declared it;
 * the joined file has another. */
extern u32 gData_02003c84__champa_cliff_caves[] __asm__("gData_02003c84");
#define gData_02003c84 gData_02003c84__champa_cliff_caves

const struct ActorPlacement *Scene_GetPlacements(void) {
  s32 scene;

  if (GameFlag_Get(0x97e)) {
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
    if (GameFlag_Get(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
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

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void ChampaCliffCaves_OpenDoorAndEnter(const struct WarpTarget *warp, s32 door,
                                       s32 mode) {
  s32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  MapGrid_PlayMapEdit(warp->scene, warp->x, warp->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterOnColumn_(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Func_020001f4(s32 door) {
  ChampaCliffCaves_OpenDoorAndEnter(&gData_02003d10, door, 0);
}

/* VIEW(champa_cliff_caves): routine gData_02003d18 as this part declared it;
 * the joined file has another. */
extern const struct WarpTarget
    gData_02003d18__champa_cliff_caves __asm__("gData_02003d18");
#define gData_02003d18 gData_02003d18__champa_cliff_caves

static __inline__ void Actor_CommitPositionAndActivate_(s32 actor, s32 dx,
                                                        s32 dz) {
  Actor_WalkByAndWaitById(actor, dx, dz);
}

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                 s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

void Func_02000208(void) {
  const struct WarpTarget *warp;
  s32 msg;

  warp = &gData_02003d10;
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 168, 424);
  Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_EAST);
  Sound_Play(158);
  MapGrid_PlayMapEdit(warp->scene, warp->x, warp->z);
  Actor_GetById(8)->motionFlags = 0;
  Actor_SetPosition(8, 184 << 16, 204 << 17);
  Actor_SetSpeedParameters_(8, 0x1cccc, 0xe666);
  Actor_WalkByAndWaitById(8, 0, 16);
  Actor_FaceDirectionById(8, FACING_WEST);
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
  MapGrid_PlayMapEdit(warp->scene, warp->x, warp->z);
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
  Actor_WatchActor(object, Actor_GetById(gMainRoutineWork.currentOwner));
}

void Func_0200038c(s32 actor) {
  Event_SetMessageId((s32)MsgAwComeOn);
  Event_ShowMessageAndWait(actor, 0);
  Func_02003534_(actor, 0xd000);
  Actor_SetActionCallback(Actor_GetById(actor), 8);
}

void Func_020003bc(void) {
  MapGrid_CopyCellPictureCellsRect(41, 97, 1, 4, 41, 77);
  Map_CopyCellAttributeRect(41, 33, 1, 4, 41, 13);
}

void Func_020003ec(void) {
  struct Actor *object;
  s32 x;
  s32 z;

  MapGrid_CopyCellPictureCellsRect(50, 97, 1, 4, 41, 77);
  Map_CopyCellAttributeRect(46, 33, 1, 4, 41, 13);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  x = object->x >> 20;
  z = object->z >> 20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov685Room4 && x == 41 &&
      (u32)(z - 13) <= 4)
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                           PIXELS(664), PIXELS(280));
}

extern struct Trigger gData_02003d20[];

void Func_02000464(s32 a) { Event_PickUpItem(a, 326, 0x978); }

const struct Trigger *Scene_GetEvents(void) {
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

s32 Scene_Initialize(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

#if !TLA_PATCH_1
  ((struct EventTransition *)Ram_HeapSlots->eventWork)->type = 516;
#else
  ((struct EventTransition *)Ram_HeapSlots->eventWork)->type = 513;
#endif
  if (party->scene != (s32)&SceneId_Ov685Room1)
    Actor_GetById(party->currentOwner)->priorityFlags |= 32;
  FieldEffect_StartBlankOverlay(0);
  if (party->scene == (s32)&SceneId_Ov685Room4) {
    if (party->entrance == 98)
      Func_02001774();
    if (party->entrance == 99) {
      party->secondary = 23;
      party->entrance = 23;
      Func_02001868();
    }
    if (GameFlag_Get(0x977) && !GameFlag_Get(0x978)) {
      FieldActor_ShowItemIconById(10, 326);
      Actor_SetPosition(10, 0x2980000, 0xe80000);
      Actor_GetById(10)->motionFlags = 4;
    }
    if (GameFlag_Get(0x977))
      Actor_SetPosition(8, 0x2980000, 0xc80000);
  } else if (party->scene == (s32)&SceneId_Ov685Room3) {
    if (GameFlag_Get(0x97e)) {
      Actor_SetModeById(8, 11);
      Actor_SetSpriteFlags(Actor_GetById(8), 0);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_020005e8(s32 actor) {
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 648, 200);
  Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_EAST);
  Event_Wait(5);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 6);
  Event_Wait(7);
  Actor_ShowItemIconById(10, actor);
  Actor_SetHorizontalPositionWithTerrain(10, 648 << 16, 196 << 16);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Actor_WalkByAndWaitById(gMainRoutineWork.currentOwner, -16, -8);
  Event_Wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_EAST);
  Event_Wait(10);
}

void Func_0200067c(s32 value) {
  Actor_SetSpeedParameters(8, 0xcccc, 0x6666);
  Actor_GetById(8)->actionFlags &= 0xfe;
  Actor_WalkByAndWaitById(8, 0, 6);
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

void Func_02000804(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgThisLooksLike);
  Func_020005e8(actor);
  Func_02003534_(8, 0x3000);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(8, 257, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(8, 261, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(8, 256, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(8);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_0200067c(0);
  Actor_FaceDirectionById(8, FACING_WEST);
  Event_Wait(10);
  Actor_ShowEmoteById(8, 258, 30);
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
  Event_End();
}

void Func_02000978(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAhHaveYou);
  Func_020005e8(actor);
  Func_02003534_(8, 0x3000);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(8, 258, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(8, 264, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_0200067c(0);
  Actor_FaceDirectionById(8, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_02003534_(8, 0x5000);
  Event_End();
}

// ---- games/tla/src/scene/champa_cliff_caves/s0a34.c ----
#include "call.h"
#include "actor.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/* VIEW(s0a34): variable gData_02003634 as extern const struct ThrowAnimations
 * gData_02003634; the definition has u32 gData_02003634[] = { 439, 440, 441, };
 * tried as the definition's extern u32 gData_02003634[], it does not compile in
 * tla-ja: games/tla/src/scene/champa_cliff_caves.c:550: incompatible types in
 * assignment. */
extern const struct ThrowAnimations
    gData_02003634__s0a34 __asm__("gData_02003634");
void Actor_ShowItemIconById(s32 id, s32 animation);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);

void Func_02000a34(s32 value) {
  struct Actor *thrown = Actor_GetById(10);
  u16 bright[5];
  u16 dim[5];
  s32 glow[3];
  s32 i;
  struct Actor *object;
  u16 *palette;

  glow[0] = 0x2960000;
  glow[1] = 0;
  glow[2] = 0xee0000;
  palette = (u16 *)0x050000e4;
  bright[0] = *palette++;
  bright[1] = *palette++;
  bright[2] = *palette++;
  bright[3] = *palette++;
  bright[4] = *palette;
  palette = (u16 *)0x050000c4;
  dim[0] = *palette++;
  dim[1] = *palette++;
  dim[2] = *palette++;
  dim[3] = *palette++;
  dim[4] = *palette;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWellDoneYou);
  Func_020005e8(value);
  Value3((s32 (*)())Actor_ShowEmoteById, 8, 0x100, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Value3((s32 (*)())Actor_ShowEmoteById, 8, 0x108, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_0200067c(1);
  Call3((void (*)())Actor_WalkToAndWaitById, 8, 0x298, 200);
  Call2((void (*)())Actor_FaceDirectionById, 8, 0x3000);
  Event_Wait(10);
  Call2((void (*)())Actor_FaceDirectionById, 8, 0x8000);
  Event_Wait(10);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 8, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Event_ShowMessageThenWait(8, 0, 5);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Event_Wait(40);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessageThenWait(8, 0, 5);
  }
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0x2000,
        0);
  Call2((void (*)())Actor_FaceDirectionById, 8, 0x5000);
  Event_Wait(10);
  FieldCamera_MoveWithinMapBounds(0x2980000, -1, 0xe80000, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Value3((s32 (*)())Actor_ShowEmoteById, 8, 0x107, 40);
  Event_ShowMessageThenWait(0x2008, 0, 5);
  for (i = 0; i <= 2; i++) {
    struct ThrowAnimations animations;
    s32 landing[3];
    animations = gData_02003634__s0a34;
    Actor_SetModeById(8, 9);
    Event_Wait(3);
    Actor_ShowItemIconById(10, animations.animations[i]);
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 10, 0x29c0000,
          0xc80000);
    thrown->gravity = 0x4000;
    Actor_Jump(10, 4, 2);
    Actor_SetSpritePriority(10, 1);
    Call3((void (*)())Actor_SetSpeedParameters, 10, 0x10000, 0x8000);
    Actor_SetDestinationOffsetById(10, 0, 40);
    Sound_Play(0x136);
    Event_Wait(34);
    landing[0] = thrown->x;
    landing[1] = thrown->y;
    landing[2] = thrown->z;
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Event_Wait(20);
    Sound_Play(134);
    Func_02000734(bright, dim, 12);
    Func_02000734(dim, bright, 48);
    Event_Wait(10);
  }
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  object->actionFlags &= ~1;
  Actor_WalkByAndWaitById(gMainRoutineWork.currentOwner, 12, 12);
  Event_Wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Actor_FaceDirectionById(gMainRoutineWork.currentOwner, 0x2000);
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Event_Wait(20);
  Actor_SetModeById(8, 7);
  Event_Wait(60);
  Sound_Play(78);
  Value3((s32 (*)())Actor_ShowEmoteById, 8, 0x101, 60);
  Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
  Event_Wait(20);
  Sound_Play(220);
  Func_02000734(bright, dim, 48);
  Actor_StartRepeatedMotion(gMainRoutineWork.currentOwner, 3);
  Call2((void (*)())FieldEffect_SetAttached, gMainRoutineWork.currentOwner,
        0x102);
  Event_Wait(50);
  Actor_ShowEmoteById(8, 0x101, 60);
  Call2((void (*)())Field_StartColorTransition, 16, 0x1090);
  Call3((void (*)())MapGrid_SetScreenShake, 0x20000, 0x20000, 0x10000);
  Func_02003164(glow);
  Actor_SetModeById(8, 1);
  Actor_Jump(4, 6, 0);
  Sound_Play(194);
  Actor_GetById(4)->actionFlags &= ~1;
  Call3((void (*)())Actor_WalkByAndWaitById, 4, -12, -12);
  Event_Wait(1);
  Actor_GetById(4)->actionFlags |= 1;
  Event_Wait(10);
  Actor_StartRepeatedMotion(8, 3);
  FieldEffect_SetAttached(8, 0x102);
  Event_Wait(30);
  Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
  Call3((void (*)())MapGrid_SetScreenShake, 0x40000, 0x40000, 0x10000);
  Func_0200320c(glow);
  Sound_Play(194);
  Event_Wait(10);
  Actor_Jump(8, 6, 15);
  Actor_Jump(8, 6, 23);
  Sound_Play(194);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x2008, 0, 5);
  Call2((void (*)())Actor_ShowItemIconById, 10, 0x146);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 10, 0x2980000,
        0xe80000);
  Actor_SetSpritePriority(10, 1);
  thrown->motionFlags = 2;
  thrown->y = 0x180000;
  thrown->update = Func_02000038;
  thrown->terrainHeight = 0;
  thrown->scaleX = 0;
  thrown->scaleY = 0;
  Actor_Jump(10, 5, 0);
  Field_StartPaletteTransition(8);
  Func_020032a4();
  Call3((void (*)())MapGrid_SetScreenShake, -1, -1, 0xe666);
  while (thrown->y > 0)
    wait(1);
  thrown->gravity = 0x10000;
  thrown->y = 0;
  thrown->motionFlags = 4;
  Scene_PlayMusic();
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(5);
  Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
  Actor_FaceDirectionById(8, 0x8000);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x2008, 0, 5);
  Actor_FaceDirectionById(8, 0x5000);
  Event_Wait(10);
  Actor_SetSpritePriority(10, 2);
  Event_End();
}

void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Get(flag); }

static __inline__ void Func_0200343c_(s32 x, s32 z, s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 32 of
   * 613 instructions differ in tla-en. */
  MapGrid_SetCellHeight(x, z, a, b);
}

static __inline__ void Actor_ArmCallback_(s32 actor, s32 angle, s32 frames) {
  Actor_ArmCallback(actor, angle, frames);
}

void Func_02000f48(s32 actor) {
  s32 item;
  /* FAKEMATCH(dead): each owner stored and never read; as the bare tests,
   * 68 of 205 instructions differ in tla-en. */
  s32 owner;

  if ((Actor_GetById(gMainRoutineWork.currentOwner)->x >> 20) <= 41)
    goto first;
  ((struct EventFlags685 *)Ram_HeapSlots->eventWork)->field15a = 1;
  return;
remove:
  PartyInventory_Remove(item);
  goto chosen;
last:
  item = 441;
  if ((owner = PartyInventory_FindMember(item)) != -1)
    goto remove;
  item = -1;
  goto chosen;
first:
  item = 439;
  if ((owner = PartyInventory_FindMember(item)) != -1)
    goto remove;
  item++;
  if ((owner = PartyInventory_FindMember(item)) == -1)
    goto last;
  PartyInventory_Remove(item);
chosen:
  if (item != -1) {
    if (!GameFlag_Test_(0x975)) {
      GameFlag_Set(0x975);
      Func_02000804(item);
    } else if (!GameFlag_Test_(0x976)) {
      GameFlag_Set(0x976);
      Func_02000978(item);
    } else if (!GameFlag_Test_(0x977)) {
      GameFlag_Set(0x977);
      Func_0200343c_(164 << 18, 208 << 16, 0, -4);
      Func_0200343c_(164 << 18, 224 << 16, 0, -4);
      Func_0200343c_(164 << 18, 240 << 16, 0, -4);
      Func_0200343c_(164 << 18, 128 << 17, 0, -4);
      Func_02000a34(item);
      Func_0200343c_(164 << 18, 208 << 16, 0, 0);
      Func_0200343c_(164 << 18, 224 << 16, 0, 0);
      Func_0200343c_(164 << 18, 240 << 16, 0, 0);
      Func_0200343c_(164 << 18, 128 << 17, 0, 0);
    }
  } else {
    Actor_FaceActorById(actor, gMainRoutineWork.currentOwner, 5);
    if (!GameFlag_Test_(0x975))
      Event_SetMessageId((s32)MsgIMeanIt);
    else if (!GameFlag_Test_(0x976))
      Event_SetMessageId((s32)MsgTheLegendSaysThat);
    else if (!GameFlag_Test_(0x977))
      Event_SetMessageId((s32)MsgYesThereS);
    else
      Event_SetMessageId((s32)MsgTheTridentSeems);
    Event_ShowMessageAndWait(actor, 0);
    Actor_ArmCallback_(actor, 0x5000, 0);
  }
}

DEFINE_CHAMPA_CLIFF_CAVES_GROW(Func_02001138, 0xc00)

#include "flag_call.h"

#define PALETTE ((u16 *)0x05000000)
#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct Position {
  s32 x;
  s32 y;
  s32 z;
};

static __inline__ void FieldMessage_ShowAtActor_(s32 speaker, s32 flags) {
  /* FAKEMATCH(wrapper): forwarded through this inline; as a direct call, 35 of
   * 613 instructions differ in tla-en. */
  ((s32 (*)(s32, s32))FieldMessage_ShowAtActor)(speaker, flags);
}

void Func_0200115c(void) {
  u16 current[5];
  u16 target[5];
  struct Position position;
  const u16 *colour;
  struct MainRoutineWork *party;
  struct Actor *object = Actor_GetById(13);

  if (Flag_IsSet(0x97e))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 584, 248);
  if (Flag_IsSet(0x974)) {
    Func_02003534_(gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId((s32)MsgBackForMore);
    FieldEvent_ShowMessageThenWait(11, 0, 5);
    Sound_Play(36);
    FieldCamera_MoveWithinMapBounds(664 << 16, -1, 0x1000000, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(5);
    Motion_SetModeAndWaitAnimation(8, 4);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    FieldActor_ShowEmote(8, 265, 30);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
  } else {
    Flag_Set(0x974);
    Event_SetMessageId((s32)MsgHoldItRight);
    FieldEvent_ShowMessageThenWait(11, 0, 5);
    Sound_Play(36);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
    FieldCamera_MoveWithinMapBounds(664 << 16, -1, 0x1000000, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(5);
    Actor_Jump(11, 6, 15);
    Actor_Jump(11, 6, 23);
    FieldEvent_ShowMessageThenWait(11, 0, 5);
    FieldActor_ShowEmote(11, 263, 30);
    FieldEvent_ShowMessageThenWait(11, 0, 5);
    FieldActor_ShowEmote(8, 257, 30);
    Func_02003534_(8, 0x3000);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(8, 0, 5);
    FieldActor_ShowEmote(11, 256, 30);
    Func_02003534_(11, 0xb000);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(11, 0, 5);
    FieldActor_ShowEmote(11, 263, 30);
    FieldEvent_ShowMessageThenWait(11, 0, 5);
    Motion_SetModeAndWaitAnimation(8, 3);
    FieldEvent_ShowMessageThenWait(8, 0, 5);
    Func_02003534_(8, 0x8000);
    Event_Wait(5);
    Motion_SetModeAndWaitAnimation(8, 4);
    FieldEvent_ShowMessageThenWait(8, 0, 5);
    Func_02003534_(11, 0x8000);
    Event_Wait(30);
    Func_02003534_(11, 0xb000);
    Event_Wait(10);
    FieldActor_ShowEmote(11, 258, 30);
    FieldEvent_ShowMessageThenWait(11, 0, 5);
    Func_02003534_(11, 0x8000);
    Event_Wait(20);
    Motion_SetVarCbAndRefresh(11, 2);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(11, 0, 5);
    FieldActor_ShowEmote(8, 257, 30);
    FieldEvent_ShowMessageThenWait(8, 0, 5);
    Func_02003534_(11, 0xb000);
    Event_Wait(10);
    FieldActor_ShowEmote(11, 265, 45);
    Func_02003534_(11, 0x300a);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(11, 0, 5);
    FieldActor_ShowEmote(8, 258, 30);
    Func_02003534_(8, 0x3000);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(8, 0, 5);
    Func_02003534_(11, 0xb000);
    Event_Wait(10);
    Motion_SetModeAndWaitAnimation(8, 4);
    Event_Wait(5);
    Func_02003534_(8, 0x8000);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(8, 0, 5);
    Motion_SetModeAndWaitAnimation(11, 3);
    Event_Wait(10);
    Motion_SetModeAndWaitAnimation(11, 3);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(11, 0, 5);
    Func_02003534_(11, 0x8000);
    Event_Wait(5);
  }
  Event_SetMessageId((s32)MsgOhAllRightListen);
  Func_02003534_(8, 0xb000);
  Event_Wait(20);
  Actor_GetById(8)->facing = 0x5000;
  Actor_SetModeById(8, 5);
  Event_Wait(30);
  Actor_SetModeById(8, 6);
  Actor_SetPosition(12, 724 << 16, 252 << 16);
  WORD_AT(Actor_GetById(12), 24) = 0x13333;
  WORD_AT(Actor_GetById(12), 28) = 0x13333;
  Actor_SetSpriteFlags(Actor_GetById(12), 0);
  FieldMessage_ShowAtActor_(0x2008, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Scene_PlayMusic();
    Event_Wait(15);
    Actor_SetPosition(12, 0, 0);
    Actor_SetModeById(8, 1);
    Actor_GetById(8)->facing = 0x8000;
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    Actor_WalkByAndWaitById(4, -16, 0);
    Event_Wait(5);
  } else {
    Event_Wait(25);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    FieldActor_ShowEmote(8, 263, 30);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    Sound_Play(78);
    Func_0200343c_(656 << 16, 208 << 16, 0, -4);
    Func_0200343c_(656 << 16, 224 << 16, 0, -4);
    Func_0200343c_(656 << 16, 240 << 16, 0, -4);
    Func_0200343c_(656 << 16, 256 << 16, 0, -4);
    WORD_AT(object, 72) = 0x4000;
    object->motionFlags = 2;
    object->terrainHeight = 0xfc190000;
    Actor_Jump(12, 10, 0);
    Sound_Play(146);
    Actor_SetSpritePriority(12, 1);
    Actor_SetSpeed(12, 0x30000, 0x18000);
    Actor_WalkToById(12, 644, 248);
    Event_Wait(21);
    Actor_SetPosition(12, 0, 0);
    Event_Wait(50);
    MapGrid_SetShake(0x20000, 0x20000, 0x10000);
    Event_Wait(70);
    Sound_Play(148);
    Event_Wait(25);
    colour = (const u16 *)0x050000e4;
    current[0] = *colour++;
    current[1] = *colour++;
    current[2] = *colour++;
    current[3] = *colour++;
    current[4] = *colour++;
    colour = (const u16 *)0x050001a4;
    target[0] = *colour++;
    target[1] = *colour++;
    target[2] = *colour++;
    target[3] = *colour++;
    target[4] = *colour++;
    Func_02000734(current, target, 48);
    Event_Wait(20);
    Sound_Play(194);
    position.x = 0x02960000;
    position.y = 0;
    position.z = 0xee0000;
    Func_02002f58(&position);
    Sound_Play(147);
    Actor_SetPosition(13, 664 << 16, 248 << 16);
    WORD_AT(object, 72) = 0x10000;
    object->motionFlags = 2;
    object->terrainHeight = 0;
    object->y = 0x200000;
    WORD_AT(object, 24) = 0x4000;
    WORD_AT(object, 108) = (s32)Func_02001138;
    WORD_AT(object, 28) = 0x4000;
    Actor_Jump(13, 11, 0);
    Sound_Play(147);
    Actor_SetSpeed(13, 0x33333, 0x19999);
    Actor_WalkToAndWait(13, 584, 248);
    Party_SetFields1f2And1f4((s32)&SceneId_Ov685Room4, 98);
    Event_SetAfterBattleDestination((s32)&SceneId_Ov685Room4, 99);
    party = &gMainRoutineWork;
    /* FAKEMATCH(dead): holds the 2's store after the party pointer; as no
     * block, 10 of 613 instructions differ in tla-en. */
    do {
    } while (0);
    party->battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(12, 5);
  }
  Event_End();
}

/* VIEW(s0a34): tag struct EventTransition as struct EventTransition__s0a34;
 * champa_cliff_caves.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/champa_cliff_caves.c:965: structure has no member named
 * `frames'. */
struct EventTransition__s0a34 {
  u8 unk000[0x1ac];
  s32 type;
  u8 unk1b0[4];
  s32 frames;
};

// The scene's own event inline: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02001774(void) {
  struct EventTransition__s0a34 *work;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgHaveYouLearned);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                         568 << 16, 248 << 16);
  Actor_SetPosition(8, 584 << 16, 224 << 16);
  Actor_SetPosition(13, 600 << 16, 240 << 16);
  Actor_SetPosition(11, 584 << 16, 264 << 16);
  Actor_ArmCallback_(8, 0x5000, 0);
  Actor_ArmCallback_(11, 0xb000, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 41);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Actor_ShowEmoteById(11, 264, 30);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 20);
  work = Ram_HeapSlots->eventWork;
  work->frames = 40;
  work->type = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetDestination((s16)gMainRoutineWork.savedScene,
                       (s16)gMainRoutineWork.savedEntrance);
}

/* VIEW(s0a34): routine gData_020041e8 as this part declared it; the joined file
 * has another. */
extern const u8 gData_020041e8__s0a34[] __asm__("gData_020041e8");
#define gData_020041e8 gData_020041e8__s0a34
/* VIEW(s0a34): routine gData_02004254 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02004254__s0a34[] __asm__("gData_02004254");
#define gData_02004254 gData_02004254__s0a34
/* VIEW(s0a34): routine gData_0200432c as this part declared it; the joined file
 * has another. */
extern const u8 gData_0200432c__s0a34[] __asm__("gData_0200432c");
#define gData_0200432c gData_0200432c__s0a34
/* VIEW(s0a34): routine gData_020042d4 as this part declared it; the joined file
 * has another. */
extern const u8 gData_020042d4__s0a34[] __asm__("gData_020042d4");
#define gData_020042d4 gData_020042d4__s0a34

/// The scene that opens with MsgMySalamanderYou: persons 8 and 11 talk and
/// move with the party through three yes-or-no answers (each counted in the
/// event work), person 14 is brought in and the others follow it, then
/// persons 5, 6, 7 and 15 join the leader and flag 0x97e is set.
void Func_02001868(void) {
  const u8 *scriptA, *scriptB, *scriptC, *scriptD;
  s32 *owner;
  u8 *party;
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMySalamanderYou);
  owner = (s32 *)&gMainRoutineWork;
  owner = (s32 *)((u8 *)owner + 532);
  Actor_SetHorizontalPositionWithTerrain(*owner, PIXELS(568), PIXELS(256));
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 13, PIXELS(592),
        PIXELS(240));
  Actor_SetModeById(13, 3);
  Actor_ArmCallback(*owner, FACING_EAST, 0);
  Call4((void (*)())FieldCamera_MoveWithinMapBounds, PIXELS(656), -1,
        PIXELS(248), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetVarCbAndRefresh(13, 2);
  Event_Wait(5);
  Func_020029f8(13, 1);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x10000, 0x8000);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -12, -18);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -10, -14);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -18, -12);
  Call3((void (*)())Actor_WalkToAndWaitById, 8, 664, 200);
  Call2((void (*)())Actor_FaceDirectionById, 8, 0x5000);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 5);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -16, 0);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -16, 8);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -16, 16);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -8, 16);
  Call3((void (*)())Actor_WalkToAndWaitById, 8, 608, 248);
  Actor_FaceDirectionById(8, FACING_WEST);
  Event_Wait(5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
  FieldActor_ShowEmote(11, 0x102, 40);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x1b333, 0xd999);
  Call3((void (*)())Actor_WalkByAndWaitById, 11, -16, 0);
  Event_Wait(5);
  Event_ShowMessageThenWait(11, 0, 5);
  Actor_FaceDirectionById(8, FACING_EAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_Jump(11, 6, 15);
  Actor_Jump(11, 6, 23);
  Event_ShowMessageThenWait(11, 0, 5);
  Actor_ShowEmoteById(8, 0x107, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())FieldEffect_SetAttached, 11, 0x102);
  Event_Wait(38);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())FieldEffect_SetAttached, 11, 0x102);
  Event_Wait(37);
  Event_ShowMessageThenWait(11, 0, 5);
  Call2((void (*)())Actor_FaceDirectionById, 8, 0x7ff6);
  Event_Wait(25);
  Actor_FaceDirectionById(8, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetVarCbAndRefresh(11, 2);
  Event_Wait(30);
  Actor_ShowEmoteById(11, 0x106, 30);
  Event_ShowMessageThenWait(11, 0, 10);
  FieldActor_ShowEmote(8, 0x102, 40);
  Event_ShowMessageThenWait(8, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionById(8, 0x7ff6);
  Event_Wait(10);
  Actor_ShowEmoteById(8, 0x101, 40);
  Call4((void (*)())Actor_LaunchFromLeader, 7, 0, -16, 0);
  Call4((void (*)())Actor_LaunchFromLeader, 5, -16, 0, 0);
  Call4((void (*)())Actor_LaunchFromLeader, 6, -16, -16, 0);
  Call4((void (*)())Actor_LaunchFromLeader, 15, 16, -16, 0);
  Actor_WaitForMoveById(15);
  Event_Wait(10);
  FieldCamera_MoveWithinMapBounds(PIXELS(640), -1, PIXELS(248), 1);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(6, 4);
  Call3((void (*)())Event_ShowMessageThenWait, 0x1006, 0, 5);
  Actor_StartRepeatedMotion(8, 3);
  Call2((void (*)())FieldEffect_SetAttached, 8, 0x102);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())FieldEffect_SetAttached, 11, 0x102);
  Event_Wait(40);
  Event_ShowMessageThenWait(0x2008, 0, 5);
  Motion_SetModeAndWaitAnimation(5, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1005, 0, 5);
  Actor_FaceDirectionById(8, FACING_EAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_StartRepeatedMotion(11, 3);
  FieldEffect_SetAttached(11, 0x102);
  Event_Wait(35);
  Event_ShowMessageThenWait(11, 0, 0);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirectionById(15, 0x5000);
  Event_Wait(5);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Actor_FaceDirectionById(6, FACING_SOUTHEAST);
    Event_Wait(5);
    Motion_SetModeAndWaitAnimation(6, 3);
    Event_Wait(10);
    Event_ShowMessageThenWait(0x1006, 0, 5);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Event_Wait(35);
    Actor_FaceDirectionById(6, FACING_SOUTHEAST);
    Event_Wait(5);
    Motion_SetModeAndWaitAnimation(6, 4);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessageThenWait(0x1006, 0, 5);
  }
  Actor_ArmCallback(15, FACING_EAST, 0);
  Actor_FaceDirectionById(6, FACING_EAST);
  Event_ShowMessageThenWait(8, 0, 5);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x2cccc, 0x16666);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x36666, 0x1b333);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())FieldEffect_SetAttached, 11, 0x102);
  scriptA = gData_020041e8;
  Actor_SetMovementTypeById(8, scriptA);
  Event_Wait(20);
  Actor_WalkToById(11, 664, 290);
  Actor_RefreshSelectorById(8);
  scriptB = gData_02004254;
  Actor_SetMovementTypeById(8, scriptB);
  Actor_WaitForMoveById(11);
  scriptC = gData_0200432c;
  Actor_SetMovementTypeById(11, scriptC);
  Actor_RefreshSelectorById(8);
  scriptD = gData_020042d4;
  Actor_SetMovementTypeById(8, scriptD);
  Actor_RefreshSelectorById(11);
  Actor_SetMovementTypeById(11, scriptA);
  Actor_RefreshSelectorById(8);
  Call3((void (*)())Actor_ArmCallback, 8, 0xb000, 0);
  Actor_RefreshSelectorById(11);
  Call3((void (*)())Actor_ArmCallback, 11, 0x3000, 0);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 5);
  FieldActor_ShowEmote(11, 0x102, 30);
  Event_ShowMessageThenWait(11, 0, 5);
  Actor_ShowEmoteById(8, 0x103, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())FieldEffect_SetAttached, 11, 0x102);
  Actor_SetMovementTypeById(8, scriptC);
  Event_Wait(20);
  Actor_WaitForMoveById(11);
  Actor_SetMovementTypeById(11, scriptB);
  Actor_RefreshSelectorById(8);
  Actor_SetMovementTypeById(8, scriptA);
  Actor_RefreshSelectorById(11);
  Actor_SetMovementTypeById(11, scriptD);
  Actor_RefreshSelectorById(8);
  Call3((void (*)())Actor_ArmCallback, 8, 0x3000, 0);
  Actor_RefreshSelectorById(11);
  Call3((void (*)())Actor_ArmCallback, 11, 0xb000, 0);
  Motion_SetModeAndWaitAnimation(11, 4);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 5);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_SetSpritePriority(4, 3);
  Actor_SetSpritePriority(5, 3);
  Actor_SetSpritePriority(6, 3);
  Actor_SetSpritePriority(7, 3);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 15, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 11, FACING_WEST, 0);
  Call2((void (*)())Actor_FaceDirectionById, 8, FACING_WEST);
  Event_Wait(10);
  Call4((void (*)())FieldCamera_MoveWithinMapBounds, PIXELS(568), -1,
        PIXELS(248), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_SetSpritePriority(14, 2);
  Actor_GetById(14)->facing = 0x3000;
  Actor_GetById(14)->motionFlags = 0;
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 14, PIXELS(520),
        PIXELS(200));
  Call3((void (*)())Actor_SetSpeedParameters, 14, 0x10000, 0x8000);
  Actor_WalkByAndWaitById(14, 0, 16);
  Actor_ArmCallback(14, 0x3000, 0);
  Event_ShowMessageThenWait(11, 0, 5);
  Actor_WatchAndSetScript(4, 14);
  Actor_WatchAndSetScript(7, 14);
  Actor_WatchAndSetScript(5, 14);
  Actor_WatchAndSetScript(6, 14);
  Actor_WatchAndSetScript(15, 14);
  Actor_WatchAndSetScript(8, 14);
  Actor_WalkByAndWaitById(14, 0, 16);
  Actor_SetSpritePriority(14, 3);
  Actor_WalkByAndWaitById(14, 0, 32);
  Actor_WalkByAndWaitById(14, 8, 8);
  Actor_WalkByAndWaitById(14, 64, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0x5000, 0);
  FieldCamera_MoveWithinMapBounds(PIXELS(600), -1, PIXELS(256), 1);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x10000, 0x8000);
  Call3((void (*)())Actor_WalkByAndWaitById, 11, -32, 0);
  Call3((void (*)())Actor_WalkByAndWaitById, 11, -16, -16);
  Call2((void (*)())Actor_FaceDirectionById, 11, FACING_WEST);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 5);
  Call2((void (*)())Actor_FaceDirectionById, 14, 0xb000);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x200e, 0, 5);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x10000, 0x8000);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -16, 0);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -16, 8);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -16, 16);
  Call3((void (*)())Actor_WalkByAndWaitById, 8, -8, 8);
  Call2((void (*)())Actor_FaceDirectionById, 8, 0x5000);
  Event_ShowMessageThenWait(8, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 11, 0xb000, 0);
  Actor_FaceDirectionById(14, 0xd000);
  Event_Wait(15);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetModeAndWaitAnimation(14, 4);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_ShowEmoteById(8, 0x109, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetModeAndWaitAnimation(11, 4);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 11, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHEAST, 0);
  Call2((void (*)())Actor_FaceDirectionById, 8, 0x5000);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Motion_SetVarCbAndRefresh(8, 2);
    Event_Wait(5);
    Event_ShowMessageThenWait(8, 0, 5);
    Actor_FaceDirectionById(11, FACING_WEST);
    Event_Wait(10);
    Actor_ShowEmoteById(11, 0x108, 30);
    Event_ShowMessageThenWait(11, 0, 5);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  } else {
    Event_Wait(35);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
    Event_ShowMessageThenWait(8, 0, 5);
    Actor_FaceDirectionById(11, FACING_WEST);
    Event_Wait(5);
    Actor_ShowEmoteById(11, 0x107, 30);
    Event_ShowMessageThenWait(11, 0, 5);
    Actor_Jump(11, 6, 15);
    Actor_Jump(11, 6, 23);
  }
  Call2((void (*)())Actor_FaceDirectionById, 4, FACING_SOUTHEAST);
  Event_Wait(15);
  Actor_ShowEmoteById(14, 0x109, 30);
  Event_ShowMessageThenWait(14, 0, 5);
  Call2((void (*)())Actor_FaceDirectionById, 14, 0xb000);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(14, 2);
  Event_Wait(5);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200e, 0, 5);
  FieldActor_ShowEmote(8, 0x102, 30);
  Event_ShowMessageThenWait(8, 0, 5);
  Call2((void (*)())Actor_FaceDirectionById, 11, 0xb000);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 5);
  Call2((void (*)())Actor_FaceDirectionById, 14, 0xd000);
  Event_Wait(5);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirectionById(14, FACING_EAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(14, 0, 5);
  FieldActor_ShowEmote(11, 0x105, 60);
  Actor_ShowEmoteById(14, 0x101, 30);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_StartRepeatedMotion(11, 3);
  FieldEffect_SetAttached(11, 0x102);
  Event_Wait(40);
  Call2((void (*)())Actor_FaceDirectionById, 11, FACING_WEST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Actor_FaceDirectionById(14, 0xd000);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_ShowEmoteById(8, 0x105, 50);
  Event_ShowMessageThenWait(8, 0, 5);
  Call2((void (*)())Actor_FaceDirectionById, 14, 0xb000);
  Event_Wait(10);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x200e, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 11, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 14, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Call2((void (*)())Actor_FaceDirectionById, 15, 0x5000);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(15);
    Motion_SetModeAndWaitAnimation(15, 3);
    Event_Wait(10);
    Event_ShowMessageThenWait(0x200f, 0, 5);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Event_Wait(35);
    Motion_SetModeAndWaitAnimation(15, 3);
    Event_Wait(10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessageThenWait(0x200f, 0, 5);
  }
  Call3((void (*)())Actor_ArmCallback, 8, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 15, 0x3000, 0);
  Actor_FaceDirectionById(14, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 10);
  Actor_StartRepeatedMotion(11, 3);
  Call2((void (*)())FieldEffect_SetAttached, 11, 0x102);
  Event_Wait(50);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 5);
  FieldActor_ShowEmote(14, 0x101, 30);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_ShowEmoteById(11, 0x105, 30);
  Call2((void (*)())Actor_FaceDirectionById, 11, 0x5000);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 15);
  Actor_ShowEmoteById(14, 0x106, 30);
  Event_ShowMessageThenWait(14, 0, 5);
  FieldActor_ShowEmote(11, 0x101, 35);
  Call2((void (*)())Actor_FaceDirectionById, 11, FACING_WEST);
  Event_Wait(5);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(14, 4);
  Event_ShowMessageThenWait(14, 0, 5);
  Call2((void (*)())Actor_FaceDirectionById, 11, 0xb000);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 5);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_ShowMessageThenWait(14, 0, 5);
  Call3((void (*)())Actor_WalkByAndWaitById, 14, -8, 0);
  Call3((void (*)())Actor_ArmCallback, 11, FACING_WEST, 0);
  Call3((void (*)())Actor_WalkByAndWaitById, 14, -8, 0);
  Actor_ShowEmoteById(11, 0x102, 0);
  Call3((void (*)())Actor_WalkByAndWaitById, 14, -56, 0);
  Call2((void (*)())Actor_FaceDirectionById, 14, 0xb000);
  FieldActor_ShowEmote(14, 0x101, 30);
  Actor_FaceDirectionById(14, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Motion_SetVarCbAndRefresh(11, 2);
  Event_Wait(5);
  Actor_Jump(11, 6, 20);
  Actor_WatchAndSetScript(4, 11);
  Actor_WatchAndSetScript(7, 11);
  Actor_WatchAndSetScript(5, 11);
  Actor_WatchAndSetScript(6, 11);
  Actor_WatchAndSetScript(15, 11);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x20000, 0x10000);
  Actor_WalkByAndWaitById(11, -80, 0);
  Event_Wait(10);
  Actor_SetSpritePriority(14, 3);
  Actor_SetSpritePriority(11, 3);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x10000, 0x8000);
  Call3((void (*)())Actor_WalkByById, 14, 0, -64);
  Call3((void (*)())Actor_WalkByAndWaitById, 11, -16, 0);
  Actor_WalkByById(11, 0, -64);
  Event_Wait(15);
  Actor_SetSpritePriority(14, 2);
  Actor_SetSpritePriority(11, 2);
  Actor_WaitForMoveById(14);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_WaitForMoveById(11);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Actor_ArmCallback(4, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Event_Wait(60);
  Actor_WalkByAndWaitById(8, 0, 16);
  Call2((void (*)())Actor_FaceDirectionById, 8, FACING_WEST);
  Event_ShowMessageThenWait(8, 0, 5);
  FieldActor_ShowEmote(4, 0x101, 0);
  FieldActor_ShowEmote(7, 0x101, 0);
  FieldActor_ShowEmote(5, 0x101, 0);
  FieldActor_ShowEmote(6, 0x101, 0);
  Actor_ShowEmoteById(15, 0x101, 30);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_FaceDirectionById(15, FACING_EAST);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTH, 0);
  Call2((void (*)())Actor_FaceDirectionById, 15, 0x5000);
  Event_Wait(40);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_FaceDirectionById(15, FACING_EAST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(8, 0, 5);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0xb333, 0x5999);
  Actor_SetMovementTypeById(8, gData_020041e8);
  Event_Wait(30);
  Actor_Stop(8);
  Call3((void (*)())Actor_WalkToAndWaitById, 8, 648, 188);
  Call2((void (*)())Actor_FaceDirectionById, 8, 0x5000);
  Event_Wait(10);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionById(15, 0x5000);
  Event_Wait(20);
  Actor_SetSpritePriority(4, 2);
  Actor_GetById(4)->priorityFlags |= 1;
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Actor_SetModeById(5, 2);
  party = (u8 *)&gMainRoutineWork;
  owner = (s32 *)(party + 532);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_SetDestinationById(5, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
  Actor_SetModeById(6, 2);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_SetDestinationById(6, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x13333, 0x9999);
  Actor_SetModeById(7, 2);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_SetDestinationById(7, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 15, 0x13333, 0x9999);
  Actor_SetModeById(15, 2);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_SetDestinationById(15, ((s16 *)&leader->x)[1],
                             ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(15);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  GameFlag_Set(0x97e);
  Event_End();
}

#include "system/dma.h"

/* VIEW(champa_cliff_caves): routine Decompress_TaggedLz as void
 * Decompress_TaggedLz(void *entry, void *destination); resource.h has s32
 * Decompress_TaggedLz(const void *source, void *destination); not tried:
 * Decompress_TaggedLz's bound declaration was not found. */
extern void Resource_DecodeType01__champa_cliff_caves(
    void *entry, void *destination) __asm__("Decompress_TaggedLz");
/* VIEW(champa_cliff_caves): routine Camera_WorldToScreen as void
 * Camera_WorldToScreen(s32 *position); module/field/event_commands.h has void
 * Camera_WorldToScreen(struct EffectVector *position); tried as the
 * definition's void Camera_WorldToScreen(struct EffectVector *position), which
 * the object already declares, Func_02002de4 moves: 2 of 175 instructions in
 * tla-ja. */
extern void Camera_WorldToScreen__champa_cliff_caves(s32 *position) __asm__(
    "Camera_WorldToScreen");

static __inline__ void Place(void *a, s32 b, s32 c, u32 d, s32 e) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 4 of 181
   * instructions differ in tla-en. */
  EffectSprite_Init(a, b, c, d, e);
}

void Func_020029f8(s32 id, s32 mode) {
  struct FlashWork *work = HeapSlot_Alloc(220, 0x428);
  struct Actor *object = Actor_GetById(id);
  s32 slot;
  s32 tile;
  struct FlashSprite *sprite;
  s32 position[3];
  s32 i;
  s32 z;

  if (mode == 0) {
    for (i = 15; i >= 0; i--) {
      s32 angle = object->facing;
      angle += 0x2000;
      object->facing = angle;
      wait(1);
    }
  }
  wait(2);
  object->scaleX = 0;
  Actor_SetSpriteFlags(object, 0);
  Dma_Set(Field_Palette685, (void *)0x050003e0, 0x80000010,
          (volatile u32 *)0x040000d4);
  Resource_DecodeType01__champa_cliff_caves(
      Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion), work);
  slot = VramCache_FindFreeEntry();
  tile = Vram_Load(slot, 0x400, work);
  sprite = &work->sprite;
  Place(sprite, 8, 16, 0x40000000, tile);
  sprite->unk1e = 240;
  sprite->priority = 1;
  sprite->fullColor = 0;
  sprite->palette = 15;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Camera_WorldToScreen__champa_cliff_caves(position);
  sprite->x = position[0];
  sprite->z = z = position[2];
  if (mode == 1) {
    sprite->scaleX = 0x20000;
    sprite->scaleY = 0x20000;
    sprite->z = z + 0xa0000;
  }
  Sound_Play(145);
  for (i = 0; i <= 71; i++) {
    sprite = &work->sprite;
    switch (i) {
    case 6:
      sprite->tile = tile + 4;
      break;
    case 12:
      sprite->tile = tile + 8;
      break;
    case 22:
      sprite->tile = tile + 12;
      break;
    case 32:
      sprite->tile = tile + 16;
      break;
    case 42:
      sprite->tile = tile + 20;
      break;
    case 52:
      sprite->tile = tile + 24;
      break;
    case 62:
      sprite->tile = tile + 28;
      break;
    }
    EffectSprite_Draw(sprite);
    wait(1);
  }
  Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  VramCache_FreeEntry(slot);
  HeapSlot_Release(220);
}
#undef FIELD_AT
#undef PALETTE
#undef BYTE_AT
#undef WORD_AT

void Func_02002c88(void) {
  struct ChampaCliffCavesWork *work = Ram_HeapSlots->menuRuntime;
  struct ChampaCliffCavesSprite *sprite;
  s32 i;
  u8 *tile;
  s32 entry;
  s32 vram;

  Resource_DecodeType01__champa_cliff_caves(
      Resource_GetTableEntry((s32)&ResourceId_GlowDisc), work);
  i = 0;
  tile = work->tiles;
next:
  if ((*tile & 0xf0) == 0xf0)
    *tile = (*tile & 0xf) + 0x60;
  tile++;
  i++;
  if (i < 0x1000)
    goto next;
  entry = VramCache_FindFreeEntry();
  vram = VramCache_Load(entry, 0x1000, work);
  work->entryA = entry;
  work->vramA = vram;
  sprite = &work->spriteA;
  Place(sprite, 31, 31, 0xc0000000, vram);
  sprite->fullColor = 1;
  sprite->palette = 0;
  sprite->priority = 2;
  sprite->unk1e = 1;
  work->field1064.value = 0;
  work->field1068.value = 0;
}

// ---- games/tla/src/scene/champa_cliff_caves/s2d34.c ----

/* VIEW(s2d34): tag struct ChampaCliffCavesWork as struct
 * ChampaCliffCavesWork__s2d34; champa_cliff_caves.c has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/champa_cliff_caves.c:1654: request for member
 * `value' in something not a structure or union. */
struct ChampaCliffCavesWork__s2d34 {
  u8 tiles[0x1000];
  s32 originX;
  s32 originY;
  s32 originZ;
  struct ChampaCliffCavesSprite spriteA;
  struct ChampaCliffCavesSprite spriteB;
  u16 entryA;
  u16 vramA;
  u16 entryB;
  u16 vramB;
  s32 field1064;
  s32 field1068;
  // A union: the two stores below keep their place after the sprite's.
  union {
    s32 value;
    u16 half;
  } field106c, field1070;
  u16 field1074;
  u8 unk1076[2];
};

void Func_02002d34(void) {
  struct ChampaCliffCavesWork__s2d34 *work = Ram_HeapSlots->menuRuntime;
  struct ChampaCliffCavesSprite *sprite;
  s32 i;
  u8 *tile;
  s32 entry;
  s32 vram;

  Resource_DecodeType01__champa_cliff_caves(
      Resource_GetTableEntry((s32)&ResourceId_FireColumn), work);
  i = 0;
  tile = work->tiles;
next:
  if ((*tile & 0xf0) == 0xf0)
    *tile = (*tile & 0xf) + 0x60;
  i++;
  tile++;
  if (i < 0x1000)
    goto next;
  entry = VramCache_FindFreeEntry();
  vram = VramCache_Load(entry, 0x800, work);
  work->entryB = entry;
  work->vramB = vram;
  sprite = &work->spriteB;
  Place(sprite, 15, 47, 0xc0008000, vram);
  sprite->fullColor = 1;
  sprite->palette = 1;
  sprite->priority = 1;
  sprite->unk1e = 1;
  work->field106c.value = 0;
  work->field1070.value = 0;
}

// The glow's frame callback.
/* VIEW(champa_cliff_caves): tag struct GlowSprite as struct
 * GlowSprite__champa_cliff_caves; module/scene/scene.h has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/champa_cliff_caves.c:1731: structure has no
 * member named `x'. */
struct GlowSprite__champa_cliff_caves {
  u8 unk_0[12];
  s32 x;
  s32 z;
  s32 scale;
  s32 trailingScale;
  u8 unk_1C[12];
};

struct Glow {
  u8 unk_0[4096];
  s32 position[3];
  struct GlowSprite__champa_cliff_caves sprites[2];
  u8 unk_105C[8];
  s32 scale;
  s32 trailingScale;
  s32 scale2;
  s32 trailingScale2;
  s16 frame;
};

/// Cycles thirty-two colours of the object palettes through a wave of greens
/// and blues, places the glow's two sprites at its spot with their scales, and
/// lets each sprite's second scale trail its first at 31/40.
void Func_02002de4(void) {
  struct Glow *work = (struct Glow *)Ram_HeapSlots->menuRuntime;
  s32 position[3];
  struct GlowSprite__champa_cliff_caves *sprite;
  s32 i;

  i = 0;
loop:
  {
    s32 red = 31;
    s16 *frame = &work->frame;
    s32 wave = Trig_Sin((*frame + i) << 11) * 6;
    s32 blue = Trig_Sin(((*frame / 2 + i) << 11) + 0x4800) * 3;
    s32 colour;
    wave >>= 16;
    blue >>= 16;
    wave += 20;
    blue += 3;
    if (i <= 15) {
      colour = blue << 10 | wave << 5 | red;
      ((u16 *)0x05000200)[111 - i] = colour;
    } else {
      colour = blue << 10 | wave << 5 | red;
      ((u16 *)0x05000200)[255 - i] = colour;
    }
    i++;
  }
  if (i <= 31)
    goto loop;
  position[0] = work->position[0];
  position[1] = work->position[1];
  position[2] = work->position[2];
  sprite = &work->sprites[0];
  Camera_WorldToScreen__champa_cliff_caves(position);
  sprite->x = position[0];
  sprite->z = position[2];
  sprite->scale = work->scale;
  sprite->trailingScale = work->trailingScale;
  EffectSprite_Draw(sprite);
  position[0] = work->position[0];
  position[1] = work->position[1];
  position[2] = work->position[2];
  sprite = &work->sprites[1];
  Camera_WorldToScreen__champa_cliff_caves(position);
  sprite->x = position[0];
  sprite->z = position[2];
  sprite->scale = work->scale2;
  sprite->trailingScale = work->trailingScale2;
  EffectSprite_Draw(sprite);
  work->trailingScale = work->scale * 31 / 40;
  work->trailingScale2 = work->scale2 * 31 / 40;
  work->frame++;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02002f58(const s32 *position) {
  u8 *work = HeapSlot_Alloc(220, 0x1078);
  s32 time;
  s32 state;

  FIELD_AT(work, s32, 4096) = position[0];
  FIELD_AT(work, s32, 4100) = position[1];
  FIELD_AT(work, s32, 4104) = position[2];
  Func_02002c88();
  Func_02002d34();
  FIELD_AT(work, u16, 4212) = 0;
  time = 0;
  {
    s32 priority = 0x480;

    Task_Add(Func_02002de4, priority);
  }
  MapGrid_SetShake(0x40000, 0x40000, 0x10000);
  state = 0;
  do {
    switch (state) {
    case 0:
      if ((FIELD_AT(work, s32, 4196) += 0x600) > 0x14ccb) {
        FIELD_AT(work, s32, 4196) = 0x14ccc;
        state = 1;
        time = -1;
      }
      break;
    case 1:
      if (time == 10) {
        state = 2;
        time = -1;
      }
      break;
    case 2:
      if ((FIELD_AT(work, s32, 4204) += 0x400) > 0x6665) {
        FIELD_AT(work, s32, 4204) = 0x6666;
        state = 3;
        time = -1;
      }
      break;
    case 3:
      if ((FIELD_AT(work, s32, 4204) += 0x3000) > 0x14ccb) {
        FIELD_AT(work, s32, 4204) = 0x14ccc;
        state = 999;
      }
      break;
    }
    System_Wait(1);
    time++;
  } while (state != 999);
}

void Func_02003078(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  s32 position[3];
  s32 state;
  s32 time;

  position[0] = FIELD_AT(work, s32, 4096);
  position[1] = FIELD_AT(work, s32, 4100) - 0x200000;
  state = 0;
  time = 0;
  position[2] = FIELD_AT(work, s32, 4104);
  do {
    switch (state) {
    case 0:
      if ((FIELD_AT(work, s32, 4204) -= 0x1000) < 0) {
        FIELD_AT(work, s32, 4204) = 0;
        time = -1;
        state = 1;
      }
      break;
    case 1:
      if ((FIELD_AT(work, s32, 4196) -= 0x800) < 0) {
        FIELD_AT(work, s32, 4196) = 0;
        state = 2;
        time = -1;
      }
      break;
    case 2:
      if (time == 1)
        state = 999;
      break;
    }
    wait(1);
    time++;
  } while (state != 999);
  Task_Remove(Func_02002de4);
  MapGrid_SetScreenShake(0, 0, 0);
  VramCache_FreeEntry(FIELD_AT(work, s16, 4188));
  VramCache_FreeEntry(FIELD_AT(work, s16, 4192));
  HeapSlot_Release(220);
}
#undef FIELD_AT

// ---- games/tla/src/scene/champa_cliff_caves/s3164.c ----
#include "system/task.h"

/* VIEW(s3164): tag struct ChampaCliffCavesWork as struct
 * ChampaCliffCavesWork__s3164; champa_cliff_caves.c has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/champa_cliff_caves.c:1894: invalid operands to
 * binary +. */
struct ChampaCliffCavesWork__s3164 {
  u8 tiles[0x1000];
  s32 originX;
  s32 originY;
  s32 originZ;
  u8 spriteA[40];
  u8 spriteB[40];
  u16 entryA;
  u16 vramA;
  u16 entryB;
  u16 vramB;
  s32 field1064;
  s32 field1068;
  s32 field106c;
  s32 field1070;
  u16 field1074;
  u8 unk1076[2];
};

struct Vec3 {
  s32 x;
  s32 y;
  s32 z;
};

void Func_02003164(const struct Vec3 *origin) {
  struct ChampaCliffCavesWork__s3164 *work =
      HeapSlot_Alloc(220, sizeof(struct ChampaCliffCavesWork__s3164));
  s32 state;

  work->originX = origin->x;
  work->originY = origin->y;
  work->originZ = origin->z;
  Func_02002c88();
  work->field106c = 0;
  work->field1070 = 0;
  work->field1074 = 0;
  {
    s32 priority = 0x480;

    Task_Add(Func_02002de4, priority);
  }
  Sound_Play(139);
  state = 0;
  do {
    if (state == 0) {
      work->field1064 += 0x1000;
      if (work->field1064 > 0x14ccb) {
        work->field1064 = 0x14ccc;
        state = 999;
      }
    }
    wait(1);
  } while (state != 999);
}

// ---- games/tla/src/scene/champa_cliff_caves/s320c.c ----

void Func_0200320c(void) {
  struct ChampaCliffCavesWork *work =
      HeapSlot_Alloc(220, sizeof(struct ChampaCliffCavesWork));
  s32 state;

  Func_02002d34();
  {
    s32 priority = 0x480;

    Task_Add(Func_02002de4, priority);
  }
  Sound_Play(144);
  state = 0;
  do {
    switch (state) {
    case 0:
      work->field106c += 0x1000;
      if (work->field106c > 0x6665) {
        work->field106c = 0x6666;
        state = 1;
      }
      break;
    case 1:
      work->field106c += 0x3000;
      if (work->field106c > 0x14ccb) {
        work->field106c = 0x14ccc;
        state = 999;
      }
      break;
    }
    wait(1);
  } while (state != 999);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020032a4(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  s32 state = 0;
  s32 time = 0;

  do {
    switch (state) {
    case 0:
      if ((FIELD_AT(work, s32, 4204) += 0x1000) > 0x1ffff) {
        FIELD_AT(work, s32, 4204) = 0x20000;
        state = 1;
        time = -1;
      }
      break;
    case 1:
      if ((FIELD_AT(work, s32, 4204) -= 0x1000) <= 0x14ccb) {
        FIELD_AT(work, s32, 4204) = 0x14ccc;
        state = 2;
        time = -1;
      }
      /* fall through */
    case 2:
      if ((FIELD_AT(work, s32, 4204) -= 0x1000) < 0)
        FIELD_AT(work, s32, 4204) = 0;
      if ((FIELD_AT(work, s32, 4196) -= 0x1000) < 0) {
        FIELD_AT(work, s32, 4196) = 0;
        state++;
        time = -1;
      }
      break;
    case 3:
      if (time == 1)
        state = 999;
      break;
    }
    wait(1);
    time++;
  } while (state != 999);
  Task_Remove(Func_02002de4);
  VramCache_FreeEntry(FIELD_AT(work, s16, 4188));
  VramCache_FreeEntry(FIELD_AT(work, s16, 4192));
  HeapSlot_Release(220);
}

u32 gData_02003634[] = {
    439,
    440,
    441,
};
#undef FIELD_AT

#undef FIELD_AT
#undef DEFINE_CHAMPA_CLIFF_CAVES_GROW
#undef FIELD_AT
#undef PALETTE
#undef BYTE_AT
#undef WORD_AT
#undef FIELD_AT
#undef gData_02003d10
#undef gData_020039cc
#undef gData_02003c84
#undef gData_02003d18
#undef gData_020041e8
#undef gData_02004254
#undef gData_0200432c
#undef gData_020042d4
// ---- from scene/data/champa_cliff_caves.c ----

#include "macros.h"
#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "text/msg_ids.h"

/* VIEW(data): routine Func_020001f4 as this part declared it; the joined file
 * has another. */
void Func_020001f4__data(void) __asm__("Func_020001f4");
#define Func_020001f4 Func_020001f4__data
/* VIEW(data): routine Func_02000304 as this part declared it; the joined file
 * has another. */
void Func_02000304__data(void) __asm__("Func_02000304");
#define Func_02000304 Func_02000304__data
/* VIEW(data): routine Func_02000350 as this part declared it; the joined file
 * has another. */
void Func_02000350__data(void) __asm__("Func_02000350");
#define Func_02000350 Func_02000350__data
/* VIEW(data): routine Func_0200038c as this part declared it; the joined file
 * has another. */
void Func_0200038c__data(void) __asm__("Func_0200038c");
#define Func_0200038c Func_0200038c__data
/* VIEW(data): routine Func_02000464 as this part declared it; the joined file
 * has another. */
void Func_02000464__data(void) __asm__("Func_02000464");
#define Func_02000464 Func_02000464__data
/* VIEW(data): routine Func_02000f48 as this part declared it; the joined file
 * has another. */
void Func_02000f48__data(void) __asm__("Func_02000f48");
#define Func_02000f48 Func_02000f48__data

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(185),
    SCENE_EXIT_IF(1, 182, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 186, 12, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 185, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 185, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 185, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 185, 5, CONDITION_ALWAYS),
    SCENE_EXITS(186),
    SCENE_EXIT_IF(7, 182, 10, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 186, 9, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 186, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(10, 186, 11, CONDITION_ALWAYS),
    SCENE_EXIT_IF(11, 186, 10, CONDITION_ALWAYS),
    SCENE_EXIT_IF(12, 185, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(13, 187, 14, CONDITION_ALWAYS),
    SCENE_EXITS(187),
    SCENE_EXIT_IF(14, 186, 13, CONDITION_ALWAYS),
    SCENE_EXIT_IF(15, 187, 16, CONDITION_ALWAYS),
    SCENE_EXIT_IF(16, 187, 15, CONDITION_ALWAYS),
    SCENE_EXIT_IF(17, 187, 18, CONDITION_ALWAYS),
    SCENE_EXIT_IF(18, 187, 17, CONDITION_ALWAYS),
    SCENE_EXIT_IF(19, 182, 11, CONDITION_ALWAYS),
    SCENE_EXIT_IF(20, 188, 21, CONDITION_ALWAYS),
    SCENE_EXITS(188),
    SCENE_EXIT_IF(21, 187, 20, CONDITION_ALWAYS),
    SCENE_EXIT_IF(22, 188, 23, CONDITION_ALWAYS),
    SCENE_EXIT_IF(23, 188, 22, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gData_0200375c[] = {
    {116, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(568),
     FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {116, CONDITION_ALWAYS, 2, PIXELS(216), 0, PIXELS(224), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_020037a4[] = {
    {115, CONDITION_ALWAYS, 1, PIXELS(192), 0, PIXELS(184),
     FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(216),
     FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(248), FACING_WEST, 0, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(248),
     FACING_NORTH + FACING_STEP, 0, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_0200381c[] = {
    {36, CONDITION_ALWAYS, 1, PIXELS(-56), 0, PIXELS(-56),
     FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(424),
     FACING_SOUTH + FACING_STEP, 1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_02003864[] = {
    {36, CONDITION_ALWAYS, 1, PIXELS(-56), 0, PIXELS(-56),
     FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(616), 0, PIXELS(216),
     FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_020038ac[] = {
    {39, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(552), FACING_WEST, 3, 0},
    {39, CONDITION_ALWAYS, 8, PIXELS(344), 0, PIXELS(568), FACING_WEST, 0, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(504),
     FACING_SOUTHEAST + FACING_STEP, 3, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_0200390c[] = {
    {39, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(728),
     FACING_NORTH + FACING_STEP, 3, 0},
    {39, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(728),
     FACING_NORTHWEST + FACING_STEP, 3, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(248), FACING_WEST, 3, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(248),
     FACING_NORTH + FACING_STEP, 3, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_02003984[] = {
    {39, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(392),
     FACING_SOUTH + FACING_STEP, 3, 0},
    {39, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(424),
     FACING_SOUTHEAST + FACING_STEP, 3, 0},
    {SCENE_TABLE_END},
};

LATE_DATA s16 gData_020039cc[] = {
    36,
    -1,
    1,
    0,
    0,
    736,
    0,
    0,
    0,
    248,
    FACING_WEST,
    2,
    115,
    -1,
    8,
    0,
    0,
    216,
    0,
    0,
    0,
    464,
    0xd000,
    1,
    489,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_SOUTH,
    2,
    16,
    -1,
    1,
    0,
    0,
    752,
    0,
    0,
    0,
    264,
    FACING_WEST,
    2,
    338,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_WEST,
    2,
    158,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_WEST,
    2,
    37,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_WEST,
    1,
    56,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_NORTH,
    2,
    5,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_SOUTH,
    2,
    6,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_SOUTH,
    2,
    7,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_NORTH,
    2,
    -1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

LATE_DATA struct ActorPlacement gData_02003aec[] = {
    {116, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(568),
     FACING_SOUTHEAST + FACING_STEP, 3, 0},
    {116, CONDITION_ALWAYS, 2, PIXELS(216), 0, PIXELS(224), FACING_SOUTH, 0, 0},
    {39, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(208),
     FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {39, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(216),
     FACING_SOUTH + FACING_STEP, 0, 0},
    {39, CONDITION_ALWAYS, 2, PIXELS(576), 0, PIXELS(248), FACING_SOUTH, 0, 0},
    {39, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(264), FACING_EAST, 0, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_02003b94[] = {
    {115, CONDITION_ALWAYS, 1, PIXELS(192), 0, PIXELS(184),
     FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(216),
     FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(248), FACING_WEST, 0, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(248), FACING_EAST, 0, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_02003c0c[] = {
    {16, CONDITION_ALWAYS, 1, PIXELS(560), 0, PIXELS(704), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_WEST, 1, 0},
    {38, CONDITION_ALWAYS, 1, PIXELS(480), 0, PIXELS(704), FACING_EAST, 3, 0},
    {37, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(712), FACING_WEST, 1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA s16 gData_02003c84[] = {
    36,  -1,     1,   0,      0,  648, 0,
    0,   0,      188, 0x5000, 2,  115, -1,
    1,   0,      0,   216,    0,  0,   0,
    464, 0x5000, 3,   489,    -1, 1,   0,
    0,   -56,    0,   0,      0,  -56, FACING_SOUTH,
    2,   -1,     0,   0,      0,  0,   0,
    0,   0,      0,   0,      0,  0,
};

LATE_DATA s16 gData_02003ce4[] = {
    1, 64, 1, 2, 6, 2, 64, 1, 2, 6, -1,
};

LATE_DATA s16 gData_02003cfa[] = {
    1, 64, 1, 2, 6, 0, 64, 1, 2, 6, -1,
};

LATE_DATA s32 gData_02003d10[] = {
    (s32)gData_02003ce4,
    0x58000b,
};

LATE_DATA s32 gData_02003d18[] = {
    (s32)gData_02003cfa,
    0x58000b,
};

LATE_DATA struct Trigger gData_02003d20[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 8, IF_FLAG_SET(0x97e), MSGNO(MsgArenTYou)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(0x97e), MSGNO(MsgIThinkThose)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(0x97e), MSGNO(MsgWeReGoing)},
    {TRIGGER_TALK, 11, IF_FLAG_SET(0x97e), MSGNO(MsgThereWerePlenty)},
    {TRIGGER_TALK, 12, IF_FLAG_SET(0x97e), MSGNO(MsgWeReallyWant)},
    {TRIGGER_TALK, 13, IF_FLAG_SET(0x97e), MSGNO(MsgItSAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x97e),
     MSGNO(MsgThatSureComes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x97e),
     MSGNO(MsgAllThoseSailors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x97e),
     MSGNO(MsgSupposeWeWent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x97e),
     MSGNO(MsgBriggsIsJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(0x97e),
     MSGNO(MsgIWasAs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(0x97e),
     MSGNO(MsgAtLeastWe)},
    {TRIGGER_TALK, 8, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgWhyDoYou)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(Func_02000350)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(Func_02000304)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgMaybeIfI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgHeSAfter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgToughOrNot)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAllOfOurMen)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTheChampaSailors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgBriggsIsThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIfYouLive)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gData_02003e7c[] = {
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x31, 12, CONDITION_ALWAYS, 12},
    {0x21, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TALK, 8, IF_FLAG_SET(0x97e), MSGNO(MsgWhyDidYou)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(0x97e), MSGNO(MsgWeHaveSo)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(0x97e), MSGNO(MsgBriggsCameDown)},
    {TRIGGER_TALK, 11, IF_FLAG_SET(0x97e), MSGNO(MsgEoleoStillWon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x97e),
     MSGNO(MsgIJustKnow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x97e),
     MSGNO(MsgBriggsSaysHe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x97e),
     MSGNO(MsgBriggsSNever)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x97e),
     MSGNO(MsgEoleoIsYounger)},
    {TRIGGER_TALK, 8, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgIDRatherFight)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgYouReAren)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgWhySEveryone)},
    {TRIGGER_TALK, 11, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgIfYouSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgItWouldBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgBriggsSaidThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgHeSNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhatSHe)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBeforeHeLeft)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMyHusbandWants)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSomedayIWant)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgMomSHad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgBriggsMayBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgWhenIMarried)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgHasMomForgotten)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgEoleoIsThe)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gData_02003ffc[] = {
    {0xc602, 15, IF_FLAG_SET(0x97e), EVENT(Func_020001f4)},
    {0x31, 14, CONDITION_ALWAYS, 14},
    {0xc602, 15, CONDITION_ALWAYS, EVENT(Func_02000208)},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 19, CONDITION_ALWAYS, 19},
    {0x21, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_TALK, 8, IF_FLAG_SET(0x97e), MSGNO(MsgOnceIM)},
    {0xc400, 11, IF_FLAG_SET(0x97e), MSGNO(MsgIWonderWhatS)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(0x97e), MSGNO(MsgBabuBaBubabu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x97e),
     MSGNO(MsgIMCertain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x97e),
     MSGNO(MsgThereIsAbsolutely)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x97e),
     MSGNO(MsgOneOfPapa)},
    {TRIGGER_TALK, 8, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgLeaveTheWomen)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgDidYouReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhatAreThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhyWouldBriggs)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgObabaSNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIVeNeverSeen)},
    {TRIGGER_EXAMINE, 40, 0x97e, EVENT(Func_02000208)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gData_02004104[] = {
    {0x31, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_STEP, 30, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(Func_0200115c)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000f48)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x975, MSGNO(MsgNowIVe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x976, MSGNO(MsgAllOfMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x977, MSGNO(MsgSoonILl)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgAllOfMyTraining)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Func_02000464)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(0x97e), MSGNO(MsgYouGuysDon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x97e),
     MSGNO(MsgMustBeIncredibly)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(Func_0200038c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgBriggsIsPretty)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgThatPitIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIWonderHow)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Func_020003bc)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Func_020003ec)},
    {TRIGGER_TABLE_END},
};

LATE_DATA u32 gData_020041e8[] = {
    39,       2, 4,  0x2680000, 0, 0xdc0000, 1,  4, 0x2780000, 0,
    0xce0000, 1, 4,  0x2880000, 0, 0xc80000, 1,  4, 0x2980000, 0,
    0xc60000, 1, 39, 1,         0, 1,        17,
};

LATE_DATA u32 gData_02004254[] = {
    39,       2,  4, 0x2a80000, 0, 0xc80000, 1, 4,  0x2b60000, 0,
    0xce0000, 1,  4, 0x2c60000, 0, 0xd80000, 1, 4,  0x2d00000, 0,
    0xe80000, 1,  4, 0x2d00000, 0, 0xf60000, 1, 39, 1,         0,
    1,        17,
};

LATE_DATA u32 gData_020042d4[] = {
    39,        2,  4,         0x2c60000, 0, 0x1080000, 1, 4,
    0x2ba0000, 0,  0x1160000, 1,         4, 0x2980000, 0, 0x1220000,
    1,         39, 1,         0,         1, 17,
};

LATE_DATA u32 gData_0200432c[] = {
    39,        2, 4,  0x2780000, 0, 0x1180000, 1,  4, 0x2680000, 0,
    0x10a0000, 1, 4,  0x2600000, 0, 0xf80000,  1,  4, 0x2600000, 0,
    0xe60000,  1, 39, 1,         0, 1,         17,
};
// clang-format on
#undef Func_020001f4
#undef Func_02000304
#undef Func_02000350
#undef Func_0200038c
#undef Func_02000464
#undef Func_02000f48
