/// @file
/// Outside the Venus Lighthouse, at the start of the prologue: Jenna and Kraden
/// come out of the lighthouse, Tolbi's soldiers and the Ruffians close in,
/// and Psynergy hurls two of them skyward on columns of rising earth before the
/// party heads west for the meeting spot.

#include "battle.h"
#include "debug.h"
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
#include "event_inline.h"
#include "event_work.h"
#include "gamepak/far_call/system.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/scene/scene.h"
#include "module/scene/venus_lighthouse_entrance.h"
#include "module/tla.h"
#include "resource.h"
#include "scene_ids.h"
#include "module/game/game.h"
#include "text/msg_ids.h"

/// The dust puffs' work, kept in Ram_HeapSlots' menu runtime while they play.
struct DustPuffState {
  u8 unk00[24];
  s32 frame;
};

struct DustPuffSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[30];
};

struct DustPuffWork {
  u8 unk0000[0x800];
  struct DustPuffState states[96];
  struct DustPuffSprite sprites[96];
  u16 unk2180;
  u16 unk2182;
};

s32 Actor_FallOverWithAnimationById();
void Func_02002a0c(struct GlowSprite *sprite, s32 width, s32 height, s32 attr,
                   s32 tile);
void Actor_SelectAnimation(struct Actor *actor, s32 mode);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void VenusLighthouseEntrance_BlastActorWithPsynergy(void *object) {
  Field_StartColorTransition(8, 0x4084);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(5), 2);
  FieldEvent_Wait(20);
  VenusLighthouseEntrance_BlastActorSkyward(object);
  BattleEffect_SetObjectUpdateMode(Actor_Get(5), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
}

void VenusLighthouseEntrance_TrailDust(struct Actor *object) {
  VenusLighthouseEntrance_EmitDust(object, object->x, object->y, object->z);
  if (++object->timer % 8 == 0)
    Event_PlaySound(125);
}

/// Talked to, a soldier runs 96 pixels out and back trailing dust.
void VenusLighthouseEntrance_RunWithDustTrail(s32 actor) {
  struct Actor *object = Actor_Get(actor);

  VenusLighthouseEntrance_StartDust();
  object->update = VenusLighthouseEntrance_TrailDust;
  object->priorityFlags = 0;
  {
    s32 a = actor;
    /* FAKEMATCH(pin): the 96 pinned to r1; as
     * Actor_WalkByAndWaitById(a, 96, dz), the pin below kept, 2 of 35
     * instructions differ in tla-en. */
    register s32 dx asm("r1") = 96;
    /* FAKEMATCH(pin): the -96 pinned to r2, the same call's second; as
     * Actor_WalkByAndWaitById(actor, 96, -96) with neither pin nor the
     * copy, 23 of 35 instructions differ in tla-en. */
    register s32 dz asm("r2") = -96;

    Actor_WalkByAndWaitById(a, dx, dz);
  }
  {
    /* FAKEMATCH(pin): the arguments pinned in order, so the -96 is negated
     * after the actor is copied; as Actor_WalkByAndWaitById(
     * actor, -96, 96), 2 of 35 instructions differ in tla-en. */
    register s32 a asm("r0") = actor;
    s32 dx = -96;

    Actor_WalkByAndWaitById(a, dx, 96);
  }
  object->update = 0;
  object->priorityFlags = 1;
  FieldEvent_Wait(30);
  VenusLighthouseEntrance_StopDust();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "ram_buffer.h"
#include "main_routine_work.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void VenusLighthouseEntrance_BirdsLand(void);
void VenusLighthouseEntrance_WakeUp(void);

s32 Scene_Initialize(void) {
  s32 t37;
  s32 t0;
  s32 t7;
  s32 t10;
  s32 t21;
  s32 t33;

  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x100;
  t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FIELD_AT((t0 + 0x23), u8, 0) = (0x20 | FIELD_AT((t0 + 0x23), u8, 0));
  Actor_SetAnimation(10, 5);
  FieldActor_SetSpritePriority(10, 1);
  Actor_SetAnimation(11, 5);
  FieldActor_SetSpritePriority(11, 1);
  FieldActor_SetSpritePriority(14, 2);
  FieldActor_SetSpritePriority(19, 2);
  t7 = Event_GetFlag(0x892);
  if (t7 != 0) {
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
  }
  t10 = Event_GetFlag(0x890);
  if (t10 != 0) {
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
  }
  if (gMainRoutineWork.entrance == 9) {
    t21 = Event_GetFlag(0x891);
    if (t21 == 0) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      VenusLighthouseEntrance_BirdsLand();
      Event_End();
    }
  }
  if (gMainRoutineWork.entrance == 0x62) {
    gMainRoutineWork.secondary = 1;
    gMainRoutineWork.entrance = 1;
    Party_RestoreHpPp(1);
    VenusLighthouseEntrance_WakeUp();
  }
  if (gMainRoutineWork.entrance == 0x63) {
    Event_SetFlag(0x893);
  }
  if (gMainRoutineWork.entrance == 0x5a) {
    gMainRoutineWork.currentOwner = 5;
    Party_RemoveMember(4);
    Party_RemoveMember(6);
    Party_RemoveMember(7);
    Event_SetDestination((s32)&SceneId_VenusLighthouseEntrance, 1);
  }
  t33 = Event_GetFlag(0x893);
  if (t33 == 0) {
    Actor_SetAnimation(22, 5);
    FieldActor_SetSpritePriority(22, 1);
  } else {
    Actor_SetPosition(22, 40 << 16, 248 << 16);
    t37 = ((s32)Actor_Get(22));
    FIELD_AT(t37, u16, 6) = 0x4000;
    Actor_FallOverWithAnimationById(0x16, 0x4000, 0);
    FieldActor_SetSpritePriority(22, 3);
    Actor_SetActionCallback(Actor_Get(22), 8);
  }
  return 0;
}

#include "call.h"

s32 Scene_PrepareMap(void) { return 0; }

s32 Actor_StartScript();
void FieldCamera_WaitForArrival(void);

/// Two birds (actors 10 and 11) fly down and land as the party comes out of the
/// lighthouse, the first time (flag 0x891).
// XXX: birds, read from their flight and landing; a render of entrance 9
// would settle it.
void VenusLighthouseEntrance_BirdsLand(void) {
  s32 t1;
  s32 t3;
  s32 t9;
  s32 t16;

  Event_SetFlag(0x891);
  t1 = ((s32)Actor_Get(10));
  FIELD_AT((t1 + 0x55), u8, 0) = 0;
  FIELD_AT(t1, s32, 72) = 0x28f;
  FIELD_AT(t1, s32, 48) = 0x18000;
  FIELD_AT(t1, s32, 52) = 0x8000;
  FieldActor_SetPositionAndResetMotion(t1, PIXELS(216), PIXELS(96),
                                       PIXELS(176));
  t3 = ((s32)Actor_Get(11));
  FIELD_AT((t3 + 0x55), u8, 0) = 0;
  FIELD_AT(t3, s32, 72) = 0x7ae;
  FIELD_AT(t3, s32, 48) = 0x18000;
  FIELD_AT(t3, s32, 52) = 0x8000;
  FieldActor_SetPositionAndResetMotion(t3, PIXELS(616), PIXELS(96),
                                       PIXELS(152));
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(376), -1, PIXELS(248), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  t9 = ((s32)Actor_Get(10));
  Actor_SetAnimation(10, 4);
  Event_PlaySound(165);
  FieldActor_SetMoveTarget(t9, PIXELS(352), PIXELS(8), PIXELS(176));
  Actor_StartScript(t9, (s32)gVenusLighthouseEntrance_BirdLands);
  FieldEvent_Wait(25);
  FieldCamera_MoveTo(PIXELS(376), -1, PIXELS(104), 1);
  t16 = ((s32)Actor_Get(11));
  Actor_SetAnimation(11, 4);
  FieldActor_SetMoveTarget(t16, PIXELS(416), PIXELS(8), PIXELS(152));
  Actor_StartScript(t16, (s32)gVenusLighthouseEntrance_BirdLands);
  FieldEvent_Wait(60);
  Event_PlaySound(165);
  FieldActor_WaitForMoveById(10);
  FieldActor_WaitForMoveById(11);
  FieldCamera_WaitForArrival();
}

extern u32 gVenusLighthouseEntrance_BirdFlies[];

/// The birds take off as the party walks past them (flag 0x892).
void VenusLighthouseEntrance_BirdsFlyOff(void) {
  s32 t7;
  s32 t11;

  Event_SetFlag(0x892);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(165);
  Actor_SetAnimation(10, 3);
  FieldActor_SetAnimation(11, 3);
  FieldEvent_Wait(21);
  t7 = Actor_Get(10);
  FIELD_AT((t7 + 0x55), u8, 0) = 0;
  FIELD_AT(t7, s32, 48) = 0x20000;
  FIELD_AT(t7, s32, 52) = 0x8000;
  Actor_SetAnimation(10, 4);
  FieldActor_SetMoveTarget(t7, PIXELS(616), PIXELS(96), PIXELS(176));
  Actor_StartScript(t7, (s32)gVenusLighthouseEntrance_BirdFlies);
  t11 = Actor_Get(11);
  FIELD_AT((t11 + 0x55), u8, 0) = 0;
  FIELD_AT(t11, s32, 48) = 0x20000;
  FIELD_AT(t11, s32, 52) = 0x8000;
  Actor_SetAnimation(11, 4);
  FieldActor_SetMoveTarget(t11, PIXELS(120), PIXELS(96), PIXELS(176));
  Call2(Actor_StartScript, t11, (s32)gVenusLighthouseEntrance_BirdFlies);
  Event_End();
}

/// "There they are! Get 'em!"
void VenusLighthouseEntrance_ThievesAttack(void) {
  u32 i;
  s32 p10;
  s32 p8;
  s32 rec;
  u8 *record;
  s32 v5;
  s32 callback;

  Event_SetFlag(0x890);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgThereTheyAre);
  Actor_SetSpeed(12, 0x10000, 0x8000);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  FieldEvent_ShowMessageThenWait(12, 0, 0);
  Actor_WalkToAndWait(5, 0x178, 0x118);
  FieldEvent_Wait(10);
  Actor_ShowEmote(5, 0x101, 40);
  Actor_FaceDirection(5, 0x6000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(5, 0x2000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(5, 0x4000, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(20);
  Actor_WalkTo(5, 0x178, 0x168);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(0x1780000, -1, 0x1a80000, 1);
  FieldCamera_WaitForArrival();
  FieldActor_WaitForMoveById(5);
  FieldEvent_Wait(10);
  Actor_ShowEmote(12, 0x102, 40);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  Actor_ShowEmote(17, 0x107, 30);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  FieldActor_Jump(18, 6, 15);
  FieldActor_Jump(18, 6, 25);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirection(14, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(14, 0xa000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(14, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(14, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_RunRepeatedMotion(15, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_FaceDirection(19, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(19, 0, 10);
  Actor_SetAnimationAndWait(20, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(20, 0, 10);
  Actor_SetAnimationAndWait(19, 3);
  FieldEvent_Wait(10);
  Actor_FaceDirection(15, 0xe000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(16, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_SetAnimationAndWait(16, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(16, 0, 10);
  Actor_WalkByAndWait(16, 64, 0);
  Actor_SetPosition(16, 0, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(20, 0xa000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(21, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(20, 0, 10);
  Actor_SetAnimationAndWait(21, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(21, 0, 10);
  Actor_WalkByAndWait(21, -64, 64);
  Actor_SetPosition(21, 0, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(14, 0xa000, 0);
  Actor_FaceDirection(15, 0xa000, 0);
  Actor_FaceDirection(19, 0xe000, 0);
  Actor_FaceDirection(20, 0xe000, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(0x1780000, -1, 0x1680000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldActor_LaunchFromLeader(8, -16, -16, 0x4000);
  FieldActor_LaunchFromLeader(9, 16, -16, 0x4000);
  FieldActor_WaitForMoveById(8);
  FieldEvent_Wait(20);
  Actor_SetSpeed(9, 0x10000, 0x8000);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(8, 0, 16);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirection(5, 0x8000, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(5, 0x4000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x2005, 0, 10);
  Actor_ShowEmote(9, 0x101, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_WalkByAndWait(9, 0, 16);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0x8000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(5, 0x102, 60);
  Actor_FaceDirection(5, 0x4000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpeed(9, 0x8000, 0x4000);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(9, 0, 16);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(5, 0x2000, 0);
  Actor_FaceDirection(8, 0x2000, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(8, 0x101, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirection(9, 0xa000, 0);
  FieldEvent_Wait(20);
  ((void (*)())FieldMessage_ShowAtActor)(9, 0);
  FieldEvent_Wait(10);
  if (FieldEvent_AskYesNoWithActorAnimation(5, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_FaceDirection(5, 0x8000, 0);
    Actor_FaceDirection(8, 0, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(5, 3);
    FieldEvent_Wait(30);
    Actor_FaceDirection(5, 0x2000, 0);
    Actor_FaceDirection(8, 0x2000, 0);
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(8, 0, 10);
    *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) += 1;
  } else {
    FieldEvent_Wait(30);
    Actor_FaceDirection(8, 0, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationAndWait(8, 4);
    *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) += 1;
    FieldEvent_ShowMessageThenWait(8, 0, 10);
    Actor_FaceDirection(8, 0x2000, 0);
  }
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, 0x5000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(5, 0x6000, 0);
  Actor_FaceDirection(8, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(0x1380000, -1, 0x1a80000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirection(17, 0x8000, 0);
  Actor_FaceDirection(18, 0, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(17, 0xe000, 0);
  Actor_FaceDirection(18, 0xe000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_ShowEmote(18, 0x107, 30);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  FieldCamera_MoveTo(0x1780000, -1, 0x1a80000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, 0x2000, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  Actor_FaceDirection(8, 0x2000, 0);
  Actor_FaceDirection(5, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  FieldCamera_MoveTo(0x1780000, -1, 0x1680000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0x8000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(5, 0x102, 40);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldCamera_MoveTo(0x1780000, -1, 0x1980000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, 0x3000, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(9, 0x8000, 0x4000);
  Actor_WalkByAndWait(9, 0, 16);
  Actor_FaceDirection(9, 0x2000, 0);
  Actor_FaceDirection(17, 0xf000, 0);
  Actor_FaceDirection(18, 0xf000, 0);
  Actor_FaceDirection(19, 0xf000, 0);
  Actor_FaceDirection(20, 0xf000, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(12, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimationAndWait(9, 4);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  Actor_FaceDirection(9, 0x9000, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(9, 0x2000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(12, 0, 0);
  Actor_FaceDirection(13, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(12, 3);
  Actor_SetAnimationAndWait(13, 3);
  FieldEvent_Wait(10);
  Actor_FaceDirection(12, 0xa000, 0);
  Actor_FaceDirection(13, 0xa000, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_SetSpeed(13, 0x13333, 0x9999);
  Actor_WalkByAndWait(12, -26, 8);
  Actor_FaceDirection(12, 0xb000, 0);
  Actor_WalkByAndWait(13, -16, -12);
  Actor_FaceDirection(13, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimation(9, 8);
  FieldEvent_Wait(50);
  BattleEffect_StartArcEffect();
  record = ((u8 *)Actor_GetById(9));
  BattleEffect_SetObjectUpdateMode((s32)record, 2);
  Event_PlaySound(178);
  FieldEvent_Wait(20);
  Field_StartColorTransition(8, 0x4084);
  FieldEvent_Wait(60);
  Event_PlaySound(0x186);
  FieldActor_SetSpritePriority(12, 0);
  VenusLighthouseEntrance_BlastActorSkyward(12);
  Actor_ShowEmote(17, 0x100, 0);
  Actor_ShowEmote(18, 0x100, 0);
  Actor_ShowEmote(19, 0x100, 0);
  Actor_ShowEmote(20, 0x100, 0);
  Actor_ShowEmote(13, 0x100, 0);
  Actor_ShowEmote(14, 0x100, 0);
  Actor_ShowEmote(15, 0x100, 50);
  FieldActor_SetSpritePriority(13, 1);
  VenusLighthouseEntrance_BlastActorSkyward(13);
  FieldEvent_Wait(30);
  record = ((u8 *)Actor_GetById(9));
  BattleEffect_SetObjectUpdateMode((s32)record, 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(16);
  FieldEvent_Wait(10);
  Actor_SetAnimation(9, 9);
  FieldEvent_Wait(30);
  Actor_ShowEmote(14, 0x102, 40);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_RunRepeatedMotion(15, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  FieldActor_StartRepeatedMotion(14, 3);
  Actor_SetAttachedEffect(14, 0x102);
  FieldActor_StartRepeatedMotion(15, 3);
  Actor_SetAttachedEffect(15, 0x102);
  FieldEvent_Wait(50);
  Actor_FaceDirection(14, 0x8000, 0);
  Actor_FaceDirection(15, 0x8000, 0);
  FieldActor_Jump(14, 6, 15);
  FieldActor_Jump(14, 6, 25);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_FaceDirection(5, 0x6000, 0);
  Actor_FaceDirection(8, 0x5000, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(9, 0x108, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkByAndWait(9, -16, 16);
  Actor_FaceDirection(9, 0x6000, 0);
  FieldCamera_MoveTo(0x1680000, -1, 0x1980000, 1);
  FieldCamera_WaitForArrival();
  FieldActor_StartRepeatedMotion(17, 3);
  Actor_SetAttachedEffect(17, 0x102);
  FieldActor_StartRepeatedMotion(18, 3);
  Actor_SetAttachedEffect(18, 0x102);
  FieldActor_StartRepeatedMotion(19, 3);
  Actor_SetAttachedEffect(19, 0x102);
  FieldActor_StartRepeatedMotion(20, 3);
  Actor_SetAttachedEffect(20, 0x102);
  FieldActor_StartRepeatedMotion(21, 3);
  Actor_SetAttachedEffect(21, 0x102);
  FieldEvent_Wait(50);
  Actor_SetAnimationAndWait(17, 4);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_ShowEmote(18, 0x102, 40);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimationAndWait(19, 4);
  FieldEvent_ShowMessageThenWait(19, 0, 10);
  FieldActor_StartRepeatedMotion(20, 3);
  Actor_SetAttachedEffect(20, 0x102);
  FieldEvent_Wait(55);
  Actor_FaceDirection(17, 0x8000, 0);
  Actor_FaceDirection(18, 0, 0);
  Actor_FaceDirection(19, 0x5000, 0);
  Actor_FaceDirection(20, 0xe000, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(20, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(20, 0, 10);
  FieldActor_Jump(18, 6, 0);
  FieldActor_Jump(19, 6, 0);
  FieldActor_Jump(20, 6, 25);
  VenusLighthouseEntrance_StartDust();
  Actor_SetSpeed(20, 0x1cccc, 0xe666);
  Actor_SetSpeed(19, 0x1cccc, 0xe666);
  Actor_SetSpeed(18, 0x1cccc, 0xe666);
  Actor_SetSpeed(17, 0x1cccc, 0xe666);
  v5 = 0;
  rec = ((u8 *)Actor_GetById(20));
  callback = (s32)VenusLighthouseEntrance_TrailDust;
  ((struct SparkRecord *)rec)->delay = v5;
  {
    s32 next = 19;

    ((struct SparkRecord *)rec)->update = callback;
    rec = (s32)((u8 *)Actor_GetById(next));
  }
  ((struct SparkRecord *)rec)->delay = v5;
  {
    s32 next = 18;

    ((struct SparkRecord *)rec)->update = callback;
    rec = (s32)((u8 *)Actor_GetById(next));
  }
  ((struct SparkRecord *)rec)->delay = v5;
  {
    s32 next = 20;

    ((struct SparkRecord *)rec)->update = callback;
    Actor_SetAnimation(next, 10);
  }
  Actor_SetAnimation(19, 10);
  Actor_SetAnimation(18, 10);
  Actor_SetDestinationOffset(20, -24, 24);
  Actor_SetDestinationOffset(19, -24, 24);
  Actor_SetDestinationOffset(18, -24, 24);
  FieldActor_WaitForMoveById(18);
  Actor_SetDestination(20, 224, 0x1e0);
  Actor_SetDestination(19, 224, 0x1e0);
  Actor_MoveToAndWait(18, 224, 0x1e0);
  rec = (s32)((u8 *)Actor_GetById(20));
  ((struct SparkRecord *)rec)->update = v5;
  rec = (s32)((u8 *)Actor_GetById(19));
  ((struct SparkRecord *)rec)->update = v5;
  rec = (s32)((u8 *)Actor_GetById(18));
  ((struct SparkRecord *)rec)->update = v5;
  FieldEvent_Wait(1);
  FieldActor_Jump(17, 6, 15);
  FieldActor_Jump(17, 6, 25);
  rec = ((u8 *)Actor_GetById(17));
  p10 = rec + 100;
  ((struct SparkRecord *)rec)->delay = v5;
  {
    s32 next = 17;

    ((struct SparkRecord *)rec)->update = callback;
    Actor_SetAnimation(next, 10);
  }
  Actor_SetDestinationOffset(17, -20, 20);
  FieldActor_WaitForMoveById(17);
  FieldActor_Jump(17, 4, 0);
  Actor_SetDestinationOffset(17, -20, 20);
  FieldActor_WaitForMoveById(17);
  Actor_SetAnimation(17, 1);
  Event_PlaySound(133);
  *(s32 *)(rec + 8) += 0x80000;
  ((struct SparkRecord *)rec)->update = v5;
  *(s32 *)(rec + 16) += -0x80000;
  {
    s32 shown = 0xc000;

    ((struct {
      u8 pad[6];
      u16 v;
    } *)rec)
        ->v = shown;
  }
  {
    s32 target = *(s32 *)(rec + 80);
    s32 shown = 0xa000;

    *(u16 *)(target + 18) = shown;
  }
  FieldEvent_Wait(55);
  *(s32 *)(rec + 8) += -0x40000;
  *(s32 *)(rec + 16) += 0x40000;
  {
    s32 shown = 0x4000;
    s32 target = *(s32 *)(rec + 80);

    ((struct {
      u8 pad[18];
      u16 v;
    } *)target)
        ->v = v5;
    ((struct {
      u8 pad[6];
      u16 v;
    } *)rec)
        ->v = shown;
  }
  FieldActor_Jump(17, 6, 40);
  Actor_SetAnimation(17, 11);
  Event_PlaySound(135);
  FieldEvent_Wait(14);
  Event_PlaySound(135);
  FieldEvent_Wait(14);
  Event_PlaySound(135);
  FieldEvent_Wait(14);
  Event_PlaySound(135);
  FieldEvent_Wait(14);
  FieldEvent_Wait(20);
  ((struct SparkRecord *)rec)->delay = v5;
  {
    s32 next = 17;

    ((struct SparkRecord *)rec)->update = callback;
    Actor_SetAnimation(next, 10);
  }
  Actor_MoveToAndWait(17, 224, 0x1d8);
  FieldEvent_Wait(30);
  ((struct SparkRecord *)rec)->update = v5;
  Actor_SetPosition(20, 0, 0);
  Actor_SetPosition(19, 0, 0);
  Actor_SetPosition(18, 0, 0);
  Actor_SetPosition(17, 0, 0);
  VenusLighthouseEntrance_StopDust();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 4);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldCamera_MoveTo(0x1780000, -1, 0x1980000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0xb000, 0);
  {
    s32 a0 = 5;

    /* FAKEMATCH(dead): an empty once-only block before the angle's load; as the
     * plain call, 2 of 2142 instructions differ in tla-en. */
    do {
    } while (0);
    Actor_FaceDirection(a0, 0x4000, 0);
  }
  Actor_FaceDirection(8, 0x3000, 0);
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(14, 0x2000, 0);
  Actor_FaceDirection(15, 0xa000, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(14, 0x8000, 0);
  Actor_FaceDirection(15, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(14, 0x18000, 0xc000);
  Actor_WalkByAndWait(14, -32, 0);
  record = ((u8 *)Actor_GetById(9));
  {
    s32 shown = 0x2000;

    *(u16 *)((s32)record + 6) = shown;
  }
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  Actor_SetSpeed(9, 0x18000, 0xc000);
  Actor_WalkByAndWait(9, 0, 16);
  Actor_FaceDirection(9, 0, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  FieldEvent_Wait(10);
  FieldActor_SetSpritePriority(14, 3);
  Actor_SetSpeed(14, 0x20000, 0x10000);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) &= 254;
  Actor_WalkToAndWait(14, 0x1c8, 0x1a8);
  FieldEvent_Wait(1);
  /* FAKEMATCH(dead): an empty once-only block ends CSE's block, so the 1 of
   * these six sets stays one register; as no block, 80 of 2142 instructions
   * differ in tla-en. */
  do {
  } while (0);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) |= 1;
  Actor_FaceDirection(14, 0x6000, 0);
  Actor_SetSpeed(14, 0x10000, 0x8000);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  Actor_SetSpeed(9, 0xb333, 0x5999);
  Actor_WalkByAndWait(9, 48, 0);
  Actor_FaceDirection(9, 0x1000, 0);
  Actor_FaceDirection(5, 0x2000, 0);
  Actor_FaceDirection(8, 0x2000, 0);
  Actor_SetSpeed(15, 0x10000, 0x8000);
  *(u8 *)(((u8 *)Actor_GetById(15)) + 90) &= 254;
  Actor_WalkToAndWait(15, 0x1c8, 0x1b8);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(15)) + 90) |= 1;
  FieldEvent_Wait(10);
  Actor_ShowEmote(9, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  Actor_RunRepeatedMotion(14, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_WalkByAndWait(9, 16, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(9, 0x107, 30);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  FieldActor_StartRepeatedMotion(14, 2);
  Actor_RunRepeatedMotion(15, 2);
  FieldEvent_Wait(20);
  Actor_SetSpeed(14, 0x8000, 0x4000);
  Actor_SetSpeed(15, 0x8000, 0x4000);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) &= 254;
  Actor_WalkToAndWait(14, 0x1e8, 0x1a8);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) |= 1;
  *(u8 *)(((u8 *)Actor_GetById(15)) + 90) &= 254;
  Actor_WalkToAndWait(15, 0x1e8, 0x1b8);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(15)) + 90) |= 1;
  Actor_WalkToAndWait(9, 0x1c8, 0x1b0);
  Actor_FaceDirection(9, 0, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  FieldActor_StartRepeatedMotion(14, 3);
  Actor_SetAttachedEffect(14, 0x102);
  FieldActor_StartRepeatedMotion(15, 3);
  Actor_SetAttachedEffect(15, 0x102);
  FieldEvent_Wait(50);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) &= 254;
  Actor_WalkToAndWait(14, 0x1f8, 0x1a8);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) |= 1;
  Actor_SetPosition(14, 0, 0);
  *(u8 *)(((u8 *)Actor_GetById(15)) + 90) &= 254;
  Actor_WalkToAndWait(15, 0x1f8, 0x1b8);
  FieldEvent_Wait(1);
  {
    u8 *flags = ((u8 *)Actor_GetById(15)) + 90;
    u8 v = *flags | 1;

    *flags = v;
  }
  Actor_SetPosition(15, 0, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkByAndWait(9, 64, 0);
  Actor_SetPosition(9, 0, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveToActorById(5, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_ShowEmote(8, 0x105, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  FieldEvent_Wait(10);
  FieldActor_Jump(8, 4, 0);
  Actor_FaceDirection(8, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmote(5, 0x105, 50);
  Actor_FaceDirection(5, 0x8000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2005, 0, 10);
  Actor_ShowEmote(8, 0x101, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmote(5, 0x109, 80);
  FieldEvent_Wait(10);
  {
    s32 a1 = 0x106;
    s32 a2 = 40;
    s32 a0 = 8;

    Actor_ShowEmote(a0, a1, a2);
  }
  Event_OpenMessage(8, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(5, 0) == 0) {
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(8, 0, 10);
    *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) += 1;
  } else {
    FieldEvent_Wait(40);
    *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) += 1;
    FieldEvent_ShowMessageThenWait(8, 0, 10);
  }
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, 0x5000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirection(8, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_SetAnimation(8, 2);
  record = (s32)Actor_Get(5);
  if ((s32)record != 0) {
    Actor_SetDestination(8, *(s16 *)((s32)record + 10),
                         *(s16 *)((s32)record + 18));
  }
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  EventCamera_FollowActor(5, 1);
  Event_End();
}

/// "Why do I gotta be the one to wait for backup!?"
void VenusLighthouseEntrance_ThiefWaitsForBackup(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgWhyDoI);
  Actor_WalkToAndWait(5, 72, 266);
  Actor_FaceDirection(5, 0x8000, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 0xa);
  FieldActor_SetAnimation(22, 1);
  FieldEvent_Wait(10);
  Actor_ShowEmote(22, 0x101, 50);
  FieldEvent_Wait(10);
  Actor_FaceDirection(22, 0, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 35);
  Actor_FaceDirection(22, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(22, 0xc);
  FieldEvent_ShowMessageThenWait(22, 0, 0);
  Actor_SetAnimation(22, 1);
  Actor_ShowEmote(22, 0x100, 50);
  Actor_FaceDirection(22, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(22, 0, 0xa);
  FieldActor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(22, 0x10000, 0x8000);
  Actor_WalkByAndWait(22, 0x10, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 0xa);
  Actor_SetSpeed(22, 0x20000, 0x10000);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 25);
  FieldActor_Jump(22, 6, 0);
  Actor_WalkBy(22, 0x1a, 0);
  FieldEvent_Wait(8);
  Actor_SetAnimation(22, 1);
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_VenusLighthouseEntrance,
                                       0x63);
  Event_SetDefeatDestination((s32)&SceneId_VenusLighthouseEntrance, 0x62);
  gMainRoutineWork.battleRule24b = 2;
  Call2(Field_RequestBattleFromEncounterTable, 9, 0);
  Event_End();
}

/// "Not that way, Jenna! The meeting spot is west of here."
void VenusLighthouseEntrance_PointToMeetingSpot(void) {
  s32 t7;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgNotThatWayThe);
  FieldActor_LaunchFromLeader(8, 0x10, 0, 0x8000);
  FieldActor_WaitForMoveById(8);
  FieldEvent_ShowMessageThenWait(0x2008, 0, 0xa);
  Actor_SetAnimation(8, 2);
  t7 = ((s32)Actor_Get(5));
  if (t7 != 0) {
    Actor_SetDestination(8, FIELD_AT(t7, s16, 10), FIELD_AT(t7, s16, 18));
  }
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(5, -16, 0);
  Event_End();
}

#include "iwram_call.h"

/// ★ Named from the game: " ... Are... Are you all right?" (MsgAreAreYou)
void VenusLighthouseEntrance_WakeUp(void) {
  s32 t96;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(5, PIXELS(364), PIXELS(156));
  Actor_SetPosition(8, PIXELS(362), PIXELS(142));
  Actor_SetPosition(9, PIXELS(390), PIXELS(144));
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(8, 0x3000, 0);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldActor_SetSpritePriority(8, 2);
  Actor_SetSpriteRotationById(5, 0x4000);
  Actor_SetSpriteFlags(((s32)Actor_Get(5)), 0);
  FieldCamera_MoveToActorById(5, 0);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1b4), s32, 0) = 0x3c;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgAreAreYou);
  Actor_ShowEmote(8, 0x102, 40);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(30);
  Actor_SetPosition(5, PIXELS(376), PIXELS(164));
  Actor_SetSpriteRotationById(5, 0);
  Actor_SetSpriteFlags(((s32)Actor_Get(5)), 1);
  FieldActor_Jump(5, 6, 50);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(5, -14, -2);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, 0, 0);
  Actor_FaceDirection(5, 0xf000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_SetAnimationAndWait(9, 4);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkByAndWait(9, 0, 0x10);
  Actor_FaceDirection(9, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(5, 9);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkBy(9, 0, 0x78);
  FieldEvent_Wait(40);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkByAndWait(8, 0x18, 0);
  Actor_WalkByAndWait(8, 0, 0x14);
  Actor_FaceDirection(5, 0x3000, 0);
  FieldActor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(8, 0x105, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  FieldActor_Jump(8, 4, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_SetAnimation(8, 2);
  t96 = ((s32)Actor_Get(5));
  if (t96 != 0) {
    Actor_SetDestination(8, FIELD_AT(t96, s16, 10), FIELD_AT(t96, s16, 18));
  }
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1b4), s32, 0) = 8;
  Event_End();
}

struct GlowWork {
  u8 unk000[0x400];
  struct GlowSprite sprites[16];
  struct GlowState states[16];
  struct Actor *object;
  struct Actor *effect;
  s32 x;
  s32 y;
  s32 z;
  s16 stage;
  s16 timer;
  u16 tileTop;
  u16 tile;
  s16 height;
  s8 follow;
};

void VenusLighthouseEntrance_UpdateBlastColumn(void) {
  struct GlowWork *work = Ram_HeapSlots->menuRuntime;
  struct Actor *object = work->object;
  struct Actor *effect = work->effect;
  s32 i, y;
  s32 scale;
  s32 frameStep;
  s32 mask;
  struct GlowState *state;
  struct GlowSprite *sprite;

  switch (work->stage) {
  case 0:
    if (work->timer == 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 1:
    work->height += 4;
    if (work->height > 31) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 2:
    if (work->timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 3:
    if (work->timer == 0) {
      object->motionFlags = 3;
      object->velocityY = 0x180000;
      work->follow = 0;
      Sound_Play(134);
    }
    work->height += 8;
    if (work->height > 79) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 4: {
    s16 *timer = &work->timer;

    if (work->timer == 10) {
      object->z = 0;
      object->x = 0;
    }
    if (*timer == 30) {
      work->stage++;
      work->timer = -1;
    }
    break;
  }
  case 5:
    work->height -= 8;
    if (work->height <= 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 6: {
    s16 *timer;

    scale = effect->scaleX;
    if (scale > 0)
      scale -= 0x51e;
    else
      scale = 0;
    effect->scaleX = scale;
    effect->scaleY = scale;
    /* FAKEMATCH(dead): an empty once-only block after the scale's stores, which
     * the timer's address follows; as no block, 2 of 449 instructions differ in
     * tla-en. */
    do {
    } while (0);
    timer = &work->timer;
    if (work->timer == 10)
      Sound_Play(310);
    if (*timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  }
  case 7: {
    s16 *timer = &work->timer;

    if (work->timer == 0) {
      Sound_Play(144);
      MapGrid_SetShake(0x30000, 0x30000, 0x10000);
    }
    if (*timer == 30) {
      MapGrid_SetShake(-1, -1, 0xe666);
      if (work->timer == 30) {
        work->stage = 999;
        work->timer = -1;
      }
    }
    break;
  }
  }
  object->facing += 0x1000;
  if (work->height > 0) {
    for (i = 0, y = 0; i < 16 && y < work->height - 8; i++, y += 16) {
      state = &work->states[i];
      state->x = work->x;
      state->y = work->y + (y << 16) + 0x80000;
      state->z = work->z;
      sprite = &work->sprites[i];
      state->frame++;
      frameStep = state->frame >> 2;
      mask = 3;
      sprite->tile = work->tileTop + (frameStep & mask) * 8;
      EffectSprite_DrawAtWorldPosition(sprite, state);
    }
    if (i <= 15) {
      state = &work->states[i];
      state->x = work->x;
      state->y = work->y + (work->height << 16);
      state->z = work->z;
      state->frame++;
      state->y += Value2((s32 (*)())Iwram_MulQ16, 0xfffe0000,
                         Math_Sin(state->frame << 12)) +
                  0xfffe0000;
      sprite = &work->sprites[i];
      sprite->tile = work->tile + ((state->frame >> 2) & 3) * 8;
      EffectSprite_DrawAtWorldPosition(sprite, state);
      if (work->follow != 0)
        object->y = state->y;
    }
  }
  work->timer++;
}
#undef FIELD_AT

// object the overlay links.

s32 BattleEffect_GetObjectDepth(struct Actor *actor);

/// A column of rising earth lifts an actor, holds it, then hurls it off the
/// top of the screen, and the ground shakes as the column sinks.
void VenusLighthouseEntrance_BlastActorSkyward(s32 id) {
  struct GlowWork *work = HeapSlot_Alloc(220, 0x860);
  struct Actor *actor = Actor_Get(id);
  struct Actor *effect;
  struct GlowSprite *sprite;
  struct GlowState *state;
  s32 resource, tile, i;
  u8 zero = 0; // Holds 0 for its whole life.

  actor->movementState = zero;
  work->object = actor;
  work->x = actor->x;
  work->y = actor->y;
  work->z = actor->z + 0x20000;
  Decompress_TaggedLz(gField_Tiles64D, work);
  resource = VramCache_FindFreeEntry();
  tile = Vram_Load(resource, 0x800, work);
  work->tile = tile;
  work->tileTop = tile + 32;
  state = work->states;
  sprite = work->sprites;
  for (i = 15; i >= 0;) {
    Func_02002a0c(sprite, 8, 8, 0x40000000, tile);
    sprite->unk1e = BattleEffect_GetObjectDepth(actor) - 2;
    sprite->priority = BattleEffect_GetObjectPriority(actor);
    sprite->fullColor = 1;
    sprite->palette = 0;
    state->frame = -1;
    i--;
    sprite++;
    state++;
  }
  effect = (FieldActor_Create(290, actor->x, actor->y, actor->z));
  work->effect = effect;
  Actor_SelectAnimation(effect, 1);
  Actor_SetSpriteFlags(effect, 0);
  effect->priorityFlags = 2;
  Actor_SetSpriteFlags(actor, 0);
  ((struct GlowAnimation *)actor->sprite)->angle = 0x4000;
  actor->x += -0xc0000;
  actor->motionFlags = 0;
  work->stage = 0;
  work->timer = 0;
  work->height = 0;
  work->follow = 1;
  Event_PlaySound(221);
  Task_AddFrame(VenusLighthouseEntrance_UpdateBlastColumn, 0x480);
  if (work->stage != 999) {
  wait:
    System_Wait(1);
    if (work->stage != 999)
      goto wait;
  }
  FieldTask_Remove(VenusLighthouseEntrance_UpdateBlastColumn);
  Actor_Release(effect);
  VramCache_FreeEntry(resource);
  HeapSlot_Release(220);
}

void BattleEffect_StepVector3(struct DustPuffState *state, s32 a, s32 b);

void VenusLighthouseEntrance_UpdateDust(void) {
  struct DustPuffWork *work = Ram_HeapSlots->menuRuntime;
  struct DustPuffState *state = work->states;
  struct DustPuffSprite *sprite = work->sprites;
  s32 value;
  s32 i;

  for (i = 0; i < 96; i++, sprite++, state++) {
    value = state->frame;
    if ((u32)value < 16) {
      sprite->tile = work->unk2182 + ((value >> 1) & 7) * 8;
      EffectSprite_DrawAtWorldPosition(sprite, state);
      BattleEffect_StepVector3(state, 64, 0);
      state->frame++;
    }
  }
}

/* VIEW(venus_lighthouse_entrance): tag struct SparkSprite as struct
 * SparkSprite__venus_lighthouse_entrance; module/tla.h has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/venus_lighthouse_entrance.c:1368: structure has
 * no member named `unk1e'. */
struct SparkSprite__venus_lighthouse_entrance {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkWork {
  u8 unk0000[0x800];
  struct SparkState states[96];
  struct SparkSprite__venus_lighthouse_entrance sprites[96];
  s16 next;
  u16 tileBase;
};

void VenusLighthouseEntrance_EmitDust(struct Actor *source, s32 x, s32 y,
                                      s32 z) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  s32 angle = Random16();
  s16 *next = &work->next;
  struct SparkSprite__venus_lighthouse_entrance *sprite = &work->sprites[*next];
  struct SparkState *state = &work->states[*next];

  sprite->priority = BattleEffect_GetObjectPriority(source);
  sprite->unk1e = BattleEffect_GetObjectDepth(source);
  state->frame = 0;
  state->x = x;
  state->y = y;
  state->z = z;
  FieldVector_AddPolarOffset(Random16() * 2 + 0x20000, angle, &state->x);
  state->velocityX = 0;
  state->velocityY = (Random16() >> 1) + 0xc000;
  state->velocityZ = 0;
  FieldVector_AddPolarOffset(Random16() >> 2, angle, &state->velocityX);
  *next = *next + 1;
  *next = *next % 96;
}

/* VIEW(s26f4): tag struct SparkSprite as struct SparkSprite__s26f4;
 * venus_lighthouse_entrance.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse_entrance.c:1423: structure has no member
 * named `unk1e'. */
struct SparkSprite__s26f4 {
  u8 unk00[4];
  u16 attr0 : 13;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

/* VIEW(s26f4): tag struct SparkWork as struct SparkWork__s26f4;
 * venus_lighthouse_entrance.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse_entrance.c:1420: structure has no member
 * named `resource'. */
struct SparkWork__s26f4 {
  u8 unk0000[0x800];
  struct SparkState states[96];
  struct SparkSprite__s26f4 sprites[96];
  u16 unk2180;
  u16 tileBase;
  u16 resource;
};

void VenusLighthouseEntrance_StartDust(void) {
  struct SparkWork__s26f4 *work = HeapSlot_Alloc(220, 0x2188);
  s32 resource;
  s32 tileBase;
  s32 i;
  struct SparkState *state;
  struct SparkSprite__s26f4 *sprite;

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DustPuffA), work);
  resource = VramCache_FindFreeEntry();
  tileBase = Vram_Load(resource, 0x800, work);
  work->tileBase = tileBase;
  work->resource = resource;
  state = work->states;
  sprite = work->sprites;
  for (i = 95; i >= 0; i--) {
    Func_02002a0c(sprite, 8, 8, 0x40000000, tileBase);
    sprite->fullColor = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    state->frame = -1;
    state++;
    sprite++;
  }
  {
    s32 zero = 0;
    work->unk2180 = zero;
  }
  Task_AddFrame(VenusLighthouseEntrance_UpdateDust, (s32)(0x480));
}

#include "resource.h"
#include "heap_release.h"

/* VIEW(sparks): tag struct SparkWork as struct SparkWork__sparks;
 * venus_lighthouse_entrance.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse_entrance.c:1455: structure has no member
 * named `resource'. */
struct SparkWork__sparks {
  u8 unk0000[0x2102];
  u16 tileBase;
  u8 unk2104[0x80];
  s16 resource;
};

void VenusLighthouseEntrance_StopDust(void) {
  struct SparkWork__sparks *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(VenusLighthouseEntrance_UpdateDust);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
}

#include "text/msg_ids.h"

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 376, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 376, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 384, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 112, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {90, CONDITION_ALWAYS, 376, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {98, CONDITION_ALWAYS, 384, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 72, 0, 264, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(4),
    SCENE_EXIT(1, 6, 5),
    SCENE_EXIT(2, 5, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0},
    {57, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0},
    {92, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(176), FACING_EAST, 2, 0},
    {92, CONDITION_ALWAYS, 1, PIXELS(416), 0, PIXELS(152), FACING_WEST, 2, 0},
    {232, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(408), FACING_NORTHWEST, 2,
     0},
    {232, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(408), FACING_NORTHWEST, 2,
     0},
    {232, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(424), FACING_NORTHWEST, 2,
     0},
    {232, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(440), FACING_NORTHWEST, 2,
     0},
    {232, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(408), FACING_NORTHWEST, 2,
     0},
    {187, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(408), FACING_NORTHEAST, 2,
     0},
    {195, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(408), FACING_NORTHEAST, 2,
     0},
    {187, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(424), FACING_NORTHEAST, 2,
     0},
    {195, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(440), FACING_NORTHEAST, 2,
     0},
    {187, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(408), FACING_NORTHEAST, 2,
     0},
    {93, CONDITION_ALWAYS, 1, PIXELS(24), 0, PIXELS(264), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 9, 0x892, EVENT(VenusLighthouseEntrance_BirdsFlyOff)},
    {TRIGGER_STEP, 10, 0x890, EVENT(VenusLighthouseEntrance_ThievesAttack)},
    {TRIGGER_STEP, 11, 0x893,
     EVENT(VenusLighthouseEntrance_ThiefWaitsForBackup)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS,
     EVENT(VenusLighthouseEntrance_PointToMeetingSpot)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgUggghICan)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     EVENT(VenusLighthouseEntrance_BlastActorWithPsynergy)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS,
     EVENT(VenusLighthouseEntrance_RunWithDustTrail)},
    {TRIGGER_TABLE_END},
};

// clang-format off
u32 gVenusLighthouseEntrance_BirdLands[] = {
    1, 0, 1, 39, 2, 0,
    4, 22, 26, 3, 22, 13,
    0, 0, 1, 17,
};

u32 gVenusLighthouseEntrance_BirdFlies[] = {
    1, 0, 1, 38,
};
// clang-format on
