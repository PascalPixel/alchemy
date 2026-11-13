/// @file
/// The scene code of overlay 0x66a, in ROM order.

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

#include "alhafran_cave/local.h"
#include "version.h"

u8 *Scene_GetPlacements(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov66aRoom1)
    return gData_02001778;
  if (scene == (s32)&SceneId_Ov66aRoom2)
    return gData_02001868;
  return gData_02001778;
}

void Func_02000084(s32 mode, s32 frame) {
  Func_0200129c(gData_020016e4, mode, frame);
}

void Func_02000098(void) {}

void Func_0200009c(void) {}

void Func_020000a0(s32 id) {
  FieldEvent_SetMessageId((s32)&MsgOhStoppedBy);
  Event_OpenMessage(id, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId((s32)(&MsgOhStoppedBy + 1));
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId((s32)(&MsgOhStoppedBy + 2));
  }
  Event_ShowMessage(id, 0);
}

void Func_020000ec(void) { Func_02001580(gData_02001608); }

void Func_020000fc(void) {
  Event_SetFlag(0x204);
  Func_02001420_(0x1900000, 0x1300000, 0, 4);
  Func_02001420_(0x1a00000, 0x1300000, 0, 4);
  Func_02001420(0x1b00000, 0x1300000, 0, 4);
  Func_020015d0_(0x1800000, 0x1200000, 0, 255);
  Func_02001420_(0x1200000, 0x1800000, 0, 4);
  Func_02001420(0x1500000, 0x1800000, 0, 4);
}

void Func_02000168(void) {
  Event_ClearFlag(0x204);
  Func_02001420_(0x1900000, 0x1300000, 0, 0);
  Func_02001420_(0x1a00000, 0x1300000, 0, 0);
  Func_02001420(0x1b00000, 0x1300000, 0, 0);
  Func_020015d0_(0x1800000, 0x1200000, 0, 0);
  Func_02001420_(0x1200000, 0x1800000, 0, 0);
  Func_02001420(0x1500000, 0x1800000, 0, 0);
}

void Func_020001d4(void) {
  struct AlhafranCaveEventWork *work = Ram_HeapSlots->eventWork;

  work->valueD4a = 2;
}

u8 *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov66aRoom1)
    return gData_02001a18;
  if (scene == (s32)&SceneId_Ov66aRoom2)
    return gData_02001a90;
  return gData_02001a18;
}

void Func_02000220(void) {
  s32 y = Actor_Get(gMainRoutineWork.currentOwner)->y;

  if (y < 0x80000)
    gMainRoutineWork.unk2c3 = 0;
  else
    gMainRoutineWork.unk2c3 = 1;
}

s32 Func_02000258(void) {
  struct Actor *object = Actor_Get(8);

  Func_020015f0(gData_02001be0, object->x, object->y + 0x1c0000, object->z);
  return 0;
}

s32 Scene_Initialize(void) {
#if !TLA_PATCH_1 /* The Japanese edition does not check the entrance. */
  ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x201;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov66aRoom1)
    Func_020002f8();
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov66aRoom2)
    Func_0200051c();
  return 0;
#else
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov66aRoom1) {
    ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
        0x201;
    Func_020002f8();
  } else if (scene == (s32)&SceneId_Ov66aRoom2) {
    if (gMainRoutineWork.entrance == 8 || gMainRoutineWork.entrance == 9)
      ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
          0x209;
    else
      ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
          0x201;
    Func_0200051c();
  }
  return 0;
#endif
}

#include "task.h"

void Func_02001578(const void *data);
void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_020002f8(void) {
  struct AlhafranCaveMapWork *map =
      (struct AlhafranCaveMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object;

  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  map->mode17 = 1;
  Task_AddFrame(Func_02000220, TASK_PASS_VBLANK);
  if (Event_GetFlag(0x204))
    Func_020000fc();
  object = Actor_Get(8);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object = Actor_Get(9);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object = Actor_Get(10);
  object->motionFlags = 0;
  object->y = 0xc0000;
  Animation_ApplyChildValues(Actor_Get(8), 6);
  Animation_ApplyChildValues(Actor_Get(9), 6);
  Animation_ApplyChildValues(Actor_Get(10), 6);
  object = Actor_Get(12);
  object->motionFlags = 0;
  object->y = 0x100000;
  object = Actor_Get(11);
  object->motionFlags = 0;
  object->y = 0x100000;
  Actor_SetSpriteRotationById(13, 0x1000);
  Actor_SetAnimation(11, 2);
  Actor_SetAnimation(12, 3);
  Actor_SetAnimation(13, 2);
  FieldActor_SetSpritePriority(11, 2);
  FieldActor_SetSpritePriority(12, 2);
  FieldActor_SetSpritePriority(13, 3);
  Actor_Get(11)->priorityFlags |= 2;
  Actor_Get(12)->priorityFlags |= 2;
  Actor_Get(13)->priorityFlags |= 2;
  Animation_ApplyChildValues(Actor_Get(11), 6);
  Animation_ApplyChildValues(Actor_Get(12), 6);
  Animation_ApplyChildValues(Actor_Get(13), 6);
  if (Event_GetFlag(0x8a7)) {
    Actor_SetPosition(14, 0xc80000, 0x880000);
    Actor_SetPosition(15, 0x1880000, 0x1680000);
  }
  object = Actor_Get(14);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  Func_02001420(object->x, object->z, 0, 4);
  object = Actor_Get(15);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  Func_02001420(object->x, object->z, 0, 4);
  Func_02001578(gData_02001608);
  Actor_SetActionCallback(Actor_Get(16), 9);
  if (!Event_GetFlag(0x8a7)) {
    /* FAKEMATCH(wrapper): the three calls forwarded through Func_02001408_; as
     * any one a direct call, 4 of 223 instructions differ in tla-en. */
    Func_02001408_(0, 58, 12, 7, 13, 58);
    Func_02001408_(0, 58, 2, 9, 25, 66);
    Func_02001408_(0, 58, 10, 5, 15, 74);
  }
}

#include "call.h"

void Func_02001230(const u16 *table);
void Func_02001520(s32 value);
void Func_02001568(void);
void Func_02001570(s32 a, s32 b, s32 c, s32 d);
void Func_020015e0(s32 value);
void Func_020015e8(const void *data, s32 a, s32 b, s32 c);
void Func_02001220(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// The rooms' setup: sprite priorities, the scene callback, actors 12 to 14,
/// actors 15 to 24 by flags 0x8a7, 0x8ab and 0x8bd, and actor 8 stepping in
/// at entrance 9 until flag 0x109 is set.
void Func_0200051c(void) {
  /* FAKEMATCH(temporary): the data address held in a local, so it loads before
   * Func_020015e0 and is kept across it, as the ROM does; as loaded at its
   * call, 31 of 257 instructions differ in tla-en. */
  const void *data;
  struct AlhafranCaveEventWork *work;

  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Actor_Get(8)->priorityFlags |= 32;
  Func_02001520(0);
  data = gData_02001be0;
  Func_020015e0(0);
  Call4((void (*)())Func_020015e8, (s32)data, 8, 15, 0x40000000);
  FieldTask_AddFrame(Func_02000258, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 8) <= 1) {
    work = Ram_HeapSlots->eventWork;

    work->valueD4c = 3;
  }
  Func_02001568();
  Func_02001570(0, 0x200, 10, 11);
  Actor_Get(10)->priorityFlags |= 32;
  Actor_Get(11)->priorityFlags |= 32;
  Func_02001230(gData_020016e4);
  Actor_SetSingleChildFlags(Actor_Get(9), 0);
  Actor_SetSpriteRotationById(14, 0x1000);
  Actor_SetAnimation(12, 0);
  Actor_SetAnimation(13, 1);
  Actor_SetAnimation(14, 2);
  FieldActor_SetSpritePriority(12, 1);
  FieldActor_SetSpritePriority(13, 1);
  Actor_Get(12)->priorityFlags |= 2;
  Actor_Get(13)->priorityFlags |= 2;
  Actor_Get(14)->priorityFlags |= 2;
  Animation_ApplyChildValues(Actor_Get(12), 6);
  Animation_ApplyChildValues(Actor_Get(13), 6);
  Animation_ApplyChildValues(Actor_Get(14), 6);
  if (Event_GetFlag(0x8a7)) {
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(19, 0, 0);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_Get(23)->update = Func_02001220;
  } else if (Event_GetFlag(0x8ab)) {
    Actor_SetPosition(23, 0, 0);
    FieldActor_SetSpritePriority(18, 2);
    FieldActor_SetSpritePriority(16, 2);
    FieldActor_SetSpritePriority(17, 2);
    FieldActor_SetSpritePriority(19, 2);
    FieldActor_SetSpritePriority(20, 2);
    if (!Event_GetFlag(0x8bd)) {
      Actor_SetPosition(17, PIXELS(712), PIXELS(216));
      Actor_SetPosition(19, PIXELS(744), PIXELS(216));
    } else {
      Actor_SetPosition(22, 0, 0);
      Actor_SetPosition(21, 0, 0);
    }
  } else {
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(19, 0, 0);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_Get(23)->update = Func_02001220;
  }
  if (gMainRoutineWork.entrance == 9 && !Event_GetFlag(0x109)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Actor_SetSpeed(8, 0x4ccc, 0x2666);
    Actor_SetDestinationOffset(8, 0, 16);
    FieldActor_WaitForMoveById(8);
  }
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov66aRoom2 &&
      Event_GetFlag(0x8a7)) {
    Func_02001408_(23, 46, 4, 5, 21, 14);
    Func_02001408_(86, 44, 5, 6, 84, 12);
    Func_02001400_(22, 45, 5, 6, 20, 13);
  }
  return 0;
}

void Func_02000814(void) {}

extern u8 MsgYouHereTo[];

void Event_Start(void);
void Event_End(void);
void Func_02001508(void);
void Func_020015a0(s32 frames, s32 color);
void BattleEffect_StartArcEffect(void);
void BattleEffect_EndArcEffect(void);
void BattleEffect_WaitForArcGroups(void);
void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);
void Func_02001228(void);
extern s32 gData_02001610[];
extern u32 gData_02001674[];
extern s32 gData_02001c08;

void Func_02000818(void) {
  struct Actor *actor21;
  struct Actor *actor15;
  struct Actor *effect;
  s32 position[3];

  /* FAKEMATCH(wrapper): 79 calls forwarded through call.h's Call2, Call3,
   * Call4 and Value3; as direct calls, 221 of 993 instructions differ in
   * tla-en (any one alone, 2 to 71). */
  if (GameFlag_Test(0x8ab) == 0)
    return;
  if (GameFlag_Test(0x8a7) == 1)
    return;
  Event_SetFlag(0x8bd);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(22, 0x10000, 0x8000);
  Actor_SetSpeed(21, 0x10000, 0x8000);
  Actor_SetSpeed(17, 0x10000, 0x8000);
  Actor_SetSpeed(19, 0x10000, 0x8000);
  Actor_SetSpeed(20, 0x10000, 0x8000);
  FieldEvent_SetMessageId((s32)MsgYouHereTo);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_WalkToAndWait(4, 0x320, 0x120);
  Actor_FaceDirection(4, 0x8000, 0);
  FieldCamera_MoveTo(0x2e80000, -1, 0x1080000, 1);
  Func_02001508();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(18, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  FieldActor_Jump(21, 6, 15);
  FieldActor_Jump(21, 6, 23);
  FieldEvent_ShowMessageThenWait(21, 0, 10);
  Actor_ShowEmote(22, 0x102, 30);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_RunRepeatedMotion(16, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(16, 0, 10);
  Actor_ShowEmote(22, 0x100, 30);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_FaceDirection(22, 0x3000, 0);
  FieldEvent_Wait(30);
  (void *)Actor_ShowEmote(22, 0x107, 50);
  FieldCamera_MoveTo(0x2e80000, -1, 0x1380000, 1);
  Func_02001508();
  FieldEvent_Wait(20);
  Actor_SetAnimation(21, 3);
  FieldActor_StartRepeatedMotion(24, 3);
  Actor_SetAttachedEffect(24, 0x102);
  FieldEvent_Wait(40);
  FieldCamera_FollowActor(24, 1);
  Func_02001508();
  Actor_SetSpeed(24, 0x18000, 0xc000);
  Actor_SetScript(24, gData_02001610);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(21, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(21, 0x3000, 0);
  FieldActor_RefreshSelectorById(24);
  Actor_FaceDirection(22, 0, 0);
  FieldCamera_MoveTo(0x2e80000, -1, 0x1100000, 1);
  Func_02001508();
  FieldEvent_Wait(10);
  Actor_ShowEmote(24, 0x102, 30);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(24, 0xb000, 0);
  FieldEvent_Wait(20);
  (void *)Actor_ShowEmote(24, 0x103, 30);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  actor21 = Actor_Get(21);
  actor15 = Actor_Get(15);
  Call2((void (*)())Func_020015a0, 8, 0x1210);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(21), 2);
  Event_PlaySound(201);
  FieldEvent_Wait(40);
  effect = BattleEffect_StartItemBreak(actor21);
  position[0] = actor15->x;
  effect->watchedObject = actor15;
  position[1] = actor15->y + 0x100000;
  position[2] = actor15->z;
  FieldVector_AddPolarOffset(0x100000, 0xc000, position);
  FieldActor_SetMoveTarget(effect, position[0], position[1], position[2]);
  BattleEffect_SnapScaleToFull(effect);
  Func_020013f0(effect, 4);
  actor15->update = Func_02001228;
  Actor_FaceDirection(4, 0x6000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(17, 0, 0);
  Actor_FaceDirection(19, 0x8000, 0);
  Actor_ShowEmote(4, 0x100, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(19, 3);
  Actor_FaceDirection(20, 0x3000, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(0x3280000, -1, 0x1200000, 1);
  Func_02001508();
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(50);
  Actor_ShowEmote(20, 0x102, 60);
  FieldCamera_MoveTo(0x2e80000, -1, 0x1100000, 1);
  Func_02001508();
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(20, -16, 0);
  FieldEvent_Wait(20);
  (void *)Actor_ShowEmote(20, 0x105, 60);
  Actor_FaceDirection(18, 0x3000, 0);
  FieldEvent_Wait(40);
  FieldActor_StartRepeatedMotion(18, 3);
  Actor_SetAttachedEffect(18, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(18, 0x5000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(18, 0, 20);
  Actor_ShowEmote(21, 0x100, 30);
  actor15->update = 0;
  Animation_ApplyChildValues(actor15, 0);
  UpdateRisingParticleBurst(effect);
  BattleEffect_SetObjectUpdateMode(Actor_Get(21), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
#if TLA_PATCH_4
  Field_EndActorSpriteEffect(8);
#else
  Field_StartPaletteTransition(8);
#endif
  FieldEvent_Wait(30);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(16, 0, 0);
  Actor_FaceDirection(17, 0x3000, 0);
  Actor_FaceDirection(19, 0x5000, 0);
  Actor_FaceDirection(20, 0x8000, 0);
  Actor_FaceDirection(24, 0xb000, 0);
  Actor_FaceDirection(22, 0xb000, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(21, 3);
  Actor_SetAttachedEffect(21, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(21, 0xc000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(18, 4);
  FieldEvent_Wait(20);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_FaceDirection(17, 0x3000, 0);
  Actor_FaceDirection(19, 0x5000, 0);
  Actor_FaceDirection(20, 0x5000, 0);
  Actor_FaceDirection(18, 0x5000, 0);
  Actor_FaceDirection(22, 0x5000, 0);
  Actor_FaceDirection(24, 0x8000, 0);
  FieldEvent_Wait(40);
  Actor_ShowEmote(21, 0x100, 40);
  Actor_SetAnimationAndWait(21, 4);
  FieldEvent_Wait(10);
  Actor_FaceDirection(21, 0x3000, 0);
  FieldEvent_Wait(20);
  Func_020015a0(8, 0x1210);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(21), 2);
  Event_PlaySound(201);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  BattleEffect_SetObjectUpdateMode(Actor_Get(21), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
#if TLA_PATCH_4
  Field_EndActorSpriteEffect(8);
#else
  Field_StartPaletteTransition(8);
#endif
  Actor_FaceDirection(16, 0, 0);
  Actor_FaceDirection(17, 0x3000, 0);
  Actor_FaceDirection(19, 0x5000, 0);
  Actor_FaceDirection(20, 0x8000, 0);
  Actor_FaceDirection(24, 0xb000, 0);
  Actor_FaceDirection(22, 0xb000, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(21, 0xd000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(21, 0, 10);
  (void *)Actor_ShowEmote(21, 0x109, 50);
  Actor_FaceDirection(22, 0x5000, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(22, 0x102, 50);
  Actor_RunRepeatedMotion(21, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(21, 0, 10);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_FaceDirection(17, 0x3000, 0);
  Actor_FaceDirection(19, 0x5000, 0);
  Actor_FaceDirection(20, 0x5000, 0);
  Actor_FaceDirection(18, 0x5000, 0);
  Actor_FaceDirection(22, 0x5000, 0);
  Actor_FaceDirection(24, 0x8000, 0);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(21, 0x101);
  FieldEvent_ShowMessageThenWait(21, 0, 20);
  gData_02001c08 = 0;
  Task_AddFrame(Func_02000814, 0x480);
  Actor_ShowEmote(24, 0x102, 30);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(22, 0xb000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(22, 0x5000, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(22, 0, 8);
  Actor_FaceDirection(22, 0x8000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_RunRepeatedMotion(21, 2);
  FieldEvent_Wait(10);
  Actor_SetAttachedEffect(21, 0x100);
  FieldEvent_ShowMessageThenWait(21, 0, 10);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_FaceDirection(21, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(21, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(21, 0, 10);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_SetAnimationAndWait(21, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(21, 0, 10);
  Actor_FaceDirection(22, 0xf000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(22, 0xb000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2016, 0, 10);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(22, 0x8000, 0);
  Actor_FaceDirection(21, 0, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(22, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(21, 3);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(24, 21);
  Actor_WalkBy(22, 0, -16);
  Actor_WalkByAndWait(21, 0, -16);
  Actor_WalkBy(22, 52, 0);
  Actor_WalkByAndWait(21, 52, 0);
  FieldEvent_Wait(15);
  FieldActor_StartRepeatedMotion(22, 3);
  Actor_SetAttachedEffect(22, 0x102);
  FieldEvent_Wait(40);
  FieldActor_WatchAndSetScript(4, 21);
  FieldActor_SetSpritePriority(22, 2);
  FieldActor_SetSpritePriority(21, 2);
  Actor_SetScript(22, gData_02001674);
  FieldEvent_Wait(10);
  Actor_SetScript(21, gData_02001674);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(20, 16, 0);
  Actor_FaceDirection(20, 0x3000, 0);
  Actor_WalkBy(17, 0, 16);
  Actor_WalkByAndWait(19, 0, 16);
  FieldEvent_Wait(5);
  Actor_FaceDirection(17, 0x3000, 0);
  Actor_FaceDirection(19, 0x3000, 0);
  Actor_FaceDirection(18, 0x3000, 0);
  FieldActor_RefreshSelectorById(21);
  FieldActor_Stop(24);
  FieldActor_Stop(4);
  Actor_SetPosition(22, 0, 0);
  Actor_SetPosition(21, 0, 0);
  Actor_SetActionCallback(Actor_Get(24), 9);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

void Func_02001220(void) { Func_020015d8(); }

void Func_02001228(void) { Actor_ApplyRandomChildValues(); }

/// Walks a list of three-halfword entries (object id, kind, flag) ended by
/// 0xffff, clearing each object's collision bit and child flags before passing
/// the entry to Func_02001320. The twin of overlay 662's Func_02000b54.
void Func_02001230(const u16 *list) {
  const u16 *entry;

  if (list[0] == 0xffff)
    return;
  entry = list;
loop:
  {
    u32 id = entry[0];
    struct Actor *object = Actor_Get(id);
    s16 kind = entry[1];
    s16 flag = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSingleChildFlags(object, 0);
    Func_02001320(id, (u16)kind, (u16)flag, object->terrainId);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}

void Func_0200129c(const u16 *entry, s32 mode, s32 frame) {
  struct AlhafranCaveEffectWork *work = Ram_HeapSlots->effectWork;
  while (entry[0] != 0xffff) {
    u32 value = *entry;
    s32 id = work->objectId;

    if (id == value) {
      struct Actor *object = Actor_Get(id);
      s32 kind;
      s32 flag;

      entry++;
      kind = entry[0];
      flag = entry[1];

      if (frame <= 7)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Func_02001320(id, kind, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
    entry += 3;
  }
}

void Func_02001320(s32 id, u32 kind, s32 flag, s32 layer) {
  struct Actor *object = Actor_Get(id);
  struct AlhafranCaveMapCell *cell =
      ((struct AlhafranCaveMapWork *)Ram_HeapSlots->mapWork)
          ->layers[layer]
          .cells;

  cell += Func_02001588();
  if (Event_GetFlag(flag)) {
    cell->state = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_020013f0(object, 2);
      break;
    case 1:
      Func_020013f0(object, 4);
      break;
    case 2:
      Func_020013f0(object, 6);
      break;
    }
  } else {
    cell->state = 255;
    switch (kind) {
    case 0:
      Func_020013f0(object, 1);
      break;
    case 1:
      Func_020013f0(object, 3);
      break;
    case 2:
      Func_020013f0(object, 5);
      break;
    }
  }
}
