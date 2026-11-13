#include "types.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"

extern void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);
extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
extern const s32 *gFieldEffect_DurationScripts[];
void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      (struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Import_ActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  sprite->nextFrame = 0;
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Import_ActorSelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#include "venus_lighthouse/distance.c"

#include "../field/object/find_in_cell.c"

// ---- scene/venus_lighthouse.c ----

/// The Venus Lighthouse, as the prologue opens on it: Felix moves a statue
/// with Psynergy and goes on alone, and Jenna and Kraden, doubting they should
/// leave, meet Alex and set off for Idejima. This file holds the scene's entry
/// routines and the block push; the rest of the overlay is in
/// venus_lighthouse/.

#include "shared/push.h"
#include "field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "scene.h"

extern s32 gPushLeaderSteps[];

s32 MapGrid_CheckCollision(struct StagedActor *object,
                           struct FieldPosition *position);

DEFINE_BLOCK_PUSH_ONTO(VenusLighthouse_PushBlockOnto, gPushLeaderSteps,
                       MapGrid_CheckCollision, Import_ActorSelectAnimation)

#include "../field/map/cell_fill_byte.c"

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "field_event.h"
#include "scene_tables.h"
#include "frame.h"

/// Unused: does nothing; nothing calls it.
UNUSED void VenusLighthouse_Nothing(void) {}

#define FieldActor_SetSpriteColor ((void (*)(void *, s32))Actor_SetSpriteColor)

s32 OverlayObject_UpdateEveryFourFrames(void *obj) {
  if ((gFrameCount & 3) == 0)
    FieldActor_SetSpriteColor(obj, 7);
  else
    FieldActor_SetSpriteColor(obj, 0);

  if ((gFrameCount & 7) == 0)
    Event_PlaySound(138);
  return 0;
}

/// Emits a rising particle near the actor every eight frames.
s32 VenusLighthouse_EmitRisingParticles(struct SceneEffectSource *actor) {
  struct SceneEffectParams options;
  s32 framePhase, particleX, particleY, riseSpeed;
  framePhase = gFrameCount & 7;
  if (framePhase != 0)
    goto done;
  options.priority = 3 - (s32)((u32)(Random16() * 2) >> 16);
  options.startScaleX = 0x6666;
  options.startScaleY = 0x6666;
  options.palette = 14;
  particleX = actor->x + (((s32)((u32)(Random16() * 9) >> 16) - 4) << 16);
  particleY = actor->y + ((32 - (s32)((u32)(Random16() * 32) >> 16)) << 16);
  riseSpeed = ((((s32)((u32)(Random16() * 5) >> 16)) << 16) + 0x00050000) / 10;
  FieldEffect_Spawn(particleX, particleY, actor->z, 0, riseSpeed, framePhase,
                    0x000b0000, &options);
done:
  return 0;
}

#define Actor_SetSpriteFlags ((void (*)(void *, s32))Actor_SetSpriteFlags)

extern struct ActorPlacement gActorPlacementsEntrance99[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

s32 SceneActor_ApplyCounterLowBitsAsMode(struct Actor *actor) {
  FieldActor_SetSpriteColor(actor, actor->timer & 15);
  return 0;
}

s32 SceneActor_ClearSingleChildField(void *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.entrance == ENTRANCE_ALTERNATE_PLACEMENTS)
    return gActorPlacementsEntrance99;
  return gActorPlacements;
}

#undef FieldActor_SetSpriteColor

// ---- scene/venus_lighthouse/floor.c ----

/// The Venus Lighthouse's floor events and data: Felix moving the statue and
/// leaving Jenna and Kraden, their talk of leaving, the floor switch, the
/// statue that repels the leader, the reliefs and the paths Reveal shows.

#include "shared/tables.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "event.h"
#include "flag_call.h"
#include "module/field/event_commands.h"
#include "module/scene/venus_lighthouse.h"
#include "module/game/game.h"

void Exit_PrepareActorAnimation(s32 actor, s32 value);

extern s32 gVenusLighthouse_LeaderFlashes[];

/* VIEW(floor): routine Actor_GetById as struct VenusActor *Actor_GetById(s32
 * id); event.h has struct Actor *Actor_GetById(u32 objectId); tried as the
 * definition's struct Actor *Actor_GetById(u32 objectId), which the object
 * already declares, it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse.c:271: invalid type argument of `->'. */
extern struct VenusActor *Actor_GetById__floor(s32 id) __asm__("Actor_GetById");
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
/* VIEW(floor): routine Actor_RefreshSelectorById as void
 * Actor_RefreshSelectorById(s32 actor); actor.h has void
 * Actor_RefreshSelectorById(u32); tried as the definition's void
 * Actor_RefreshSelectorById(u32 objectId), VenusLighthouse_RepelLeader moves: 1
 * of 85 instructions in tla-ja. */
extern void Actor_RefreshSelectorById__floor(s32 actor) __asm__(
    "Actor_RefreshSelectorById");
/* VIEW(floor): routine Actor_SetSpriteColor as void Actor_SetSpriteColor(struct
 * VenusActor *object, s32 value); event.h has void Actor_SetSpriteColor(struct
 * Actor *object, s32 attribute); not tried: Actor_SetSpriteColor's bound
 * declaration was not found. */
extern void
Actor_SetPartAttribute__floor(struct VenusActor *object,
                              s32 value) __asm__("Actor_SetSpriteColor");
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 actor, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
void Actor_WaitForMove(struct Actor *actor);
/* VIEW(floor): routine Actor_ShowEmoteById as void Actor_ShowEmoteById(s32
 * actor, s32 height, s32 frames); event.h has s32 Actor_ShowEmoteById(s32
 * actorId, s32 emote, s32 frames); tried as the definition's struct
 * BattleEffectWatchedObject *Actor_ShowEmoteById(s32 resourceId, s32 flags, s32
 * state), it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse.c:238: conflicting types for
 * `Actor_ShowEmoteById'. */
extern void
Actor_ShowEmoteById__floor(s32 actor, s32 height,
                           s32 frames) __asm__("Actor_ShowEmoteById");
/* VIEW(floor): routine VenusLighthouse_EmitRisingParticles as s32
 * VenusLighthouse_EmitRisingParticles(struct VenusActor *object);
 * scene/venus_lighthouse has s32 VenusLighthouse_EmitRisingParticles(struct
 * SceneEffectSource *actor) {; tried as the definition's s32
 * VenusLighthouse_EmitRisingParticles(struct SceneEffectSource *actor), which
 * the object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse.c:278:
 * `VenusLighthouse_EmitRisingParticles__floor' undeclared (first use in this
 * function). */
s32 VenusLighthouse_EmitRisingParticles__floor(
    struct VenusActor *object) __asm__("VenusLighthouse_EmitRisingParticles");

/// Until the statue has sunk (flag 0x200), stepping by it throws the leader
/// back in a flash of sparks.
void VenusLighthouse_RepelLeader(void) {
  s32 leader = Party_GetLeaderActor();
  struct VenusActor *actor = Actor_GetById__floor(leader);
  s32 z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetMovementTypeById(leader, gVenusLighthouse_LeaderFlashes);
  Actor_RefreshSelectorById__floor(leader);
  Actor_SetPartAttribute__floor(Actor_GetById__floor(leader), 6);
  {
    /* FAKEMATCH(pin): the leader pinned to r0, so it is copied there before the
     * acceleration is built, as the native does; as
     * Actor_SetSpeedParameters(leader, 0x40000, 0x20000), 2 of 85 instructions
     * differ in tla-en. */
    register s32 who __asm__("r0");

    actor->speed = 0x40000;
    who = leader;
    Actor_SetSpeedParameters(who, 0x40000, 0x20000);
  }
  if (*(s32 *)&actor->zLow >> 20 <= 54) {
    Actor_GetById__floor(leader)->selectFlags &= ~1;
    z = 210;
  } else {
    Actor_GetById__floor(leader)->selectFlags &= ~1;
    z = 238;
  }
  Actor_WalkToAndWaitById(leader, actor->xPixel, z * 4);
  Event_Wait(1);
  Actor_GetById__floor(leader)->selectFlags |= 1;
  Event_Wait(20);
  Actor_SetPartAttribute__floor(actor, 0);
  actor->update = VenusLighthouse_EmitRisingParticles__floor;
  Actor_ShowEmoteById__floor(leader, 258, 60);
  Motion_SetModeAndWaitAnimation(leader, 4);
  Actor_SetPartAttribute__floor(Actor_GetById__floor(leader), 0);
  Motion_SetModeAndWaitAnimation(leader, 4);

  /* FAKEMATCH(dead): an empty once-only block, so the null callback is built
   * after the wait; as no block, 46 of 85 instructions differ in tla-en. */
  do {
  } while (0);

  actor->update = 0;
  Event_End();
}

void VenusLighthouse_StepOnFloorSwitch(void) {
  struct VenusActor *leader = Actor_GetById__floor(Party_GetLeaderActor());
  s32 z = leader->zPixel;

  if ((u32)(leader->xPixel - 676) > 7 || z < 788 || z >= 796) {
    MapGrid_CopyCellPictureIndicesRect(53, 50, 42, 49, 1, 1);
    MapGrid_CopyCellsTo(55, 117, 41, 117, 3, 5);
    GameFlag_Clear(0x201);
    leader->motionFlags |= 1;
    leader->terrainHeight = 0;
    leader->y = 0;
  } else if (!Event_GetFlag(0x201)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_Wait(5);
    MapGrid_CopyCellPictureIndicesRect(52, 50, 42, 49, 1, 1);
    MapGrid_CopyCellsTo(52, 117, 41, 117, 3, 5);
    GameFlag_Set(0x201);
    Sound_Play(161);
    leader->motionFlags &= ~1;
    leader->terrainHeight = -0x20000;
    leader->y = -0x20000;
    Event_End();
  }
}

#include "field_event.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "tla_edition.h"
#include "scene_ids.h"
#include "version.h"

/// The statue Felix moved sinks into the floor in column 42 (flag 0x200), and
/// the way past it opens.
void VenusLighthouse_SinkStatue(void) {
  struct Actor *obj;
  s32 val;
  s32 a;
  s32 b;

  obj = Actor_Get(10);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (obj->x >> 20 == 42) {
    FieldEvent_Wait(30);
    Event_PlaySound(188);
    obj->motionFlags = 0;
    val = 0xfffe0000;
    obj->terrainHeight = val;
    obj->y = val;
    Event_SetFlag(0x200);
    a = 3;
    b = 5;
    MapGrid_CopyCellsTo(44, 117, 41, 117, a, b);
  }
  Event_End();
}

void VenusLighthouse_ReadRelief(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(FieldParty_GetLeaderActor(), 1);
  Message_ShowCentered((s32)MsgVenusLighthouseReliefWords, 1);
  Event_End();
}

void VenusLighthouse_ReadTrueHeart(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(FieldParty_GetLeaderActor(), 1);
  Message_ShowCentered((s32)MsgVenusLighthouseTrueHeart, 1);
  Event_End();
}

/// Reveal fades: the paths it showed hide again.
void VenusLighthouse_HidePaths(void) {
  s32 x;
  s32 y;
  s32 a;
  s32 b;

  x = 23;
  y = 8;
  MapGrid_CopyCellAttributes(35, 8, 1, 3, x, y);
  a = 3;
  b = 1;
  MapGrid_CopyCellsTo(35, 8, 23, 8, b, a);
  MapGrid_CopyCellsTo(99, 8, 87, 8, b, a);
  x = 46;
  y = 55;
  MapGrid_CopyCellAttributes(57, 55, 3, 3, x, y);
  MapGrid_CopyCellsTo(57, 55, 46, 55, a, a);
  MapGrid_CopyCellsTo(121, 55, 110, 55, a, a);
}

/// Reveal shows the hidden paths.
void VenusLighthouse_RevealPaths(void) {
  s32 x;
  s32 y;
  s32 a;
  s32 b;

  x = 23;
  y = 8;
  MapGrid_CopyCellAttributes(36, 8, 1, 3, x, y);
  a = 3;
  b = 1;
  MapGrid_CopyCellsTo(36, 8, 23, 8, b, a);
  MapGrid_CopyCellsTo(100, 8, 87, 8, b, a);
  x = 46;
  y = 55;
  MapGrid_CopyCellAttributes(53, 55, 3, 3, x, y);
  MapGrid_CopyCellsTo(53, 55, 46, 55, a, a);
  MapGrid_CopyCellsTo(117, 55, 110, 55, a, a);
}

#define Actor_SetSpriteColor                                                   \
  ((void (*)(struct Actor *, s32))Actor_SetSpriteColor)

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 VenusLighthouse_Idle(void) { return 0; }

/// Felix casts Move on the statue.
void VenusLighthouse_FelixMovesStatue(void) {
  BattleEffect_LoadActionEffectResources(140, 1);
  FieldPsynergy_SetupActorPair(4, 10);
  ((struct SetupWork *)Ram_HeapSlots->effectWork)->idle = VenusLighthouse_Idle;
  EventObject_Initialize();
  FieldEvent_Wait(10);
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
}

/* VIEW(floor): tag struct AerieEventWork as struct AerieEventWork__floor;
 * module/scene/scene.h has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse.c:447: structure has no member named
 * `mode'. */
struct AerieEventWork__floor {
  u8 unk000[0x1ac];
  s32 mode;
};

// The Japanese form; from patch 1, the form below also saves this scene and
// entrance 22.
#if !TLA_PATCH_1
s32 VenusLighthouse_Start(void) {
  ((struct AerieEventWork__floor *)Ram_HeapSlots->eventWork)->mode = 0x204;
  if (gMainRoutineWork.entrance == ENTRANCE_OPENING) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    VenusLighthouse_FelixLeaves();
    VenusLighthouse_DoubtLeaving();
    Event_End();
  } else {
    FieldActor_SetSpriteColor(Actor_Get(10), 6);
    FieldActor_SetSpriteColor(Actor_Get(17), 6);
    FieldActor_SetSpriteColor(Actor_Get(18), 6);
    FieldActor_SetSpriteColor(Actor_Get(19), 6);
    FieldActor_SetSpriteColor(Actor_Get(20), 6);
    FieldActor_SetSpriteColor(Actor_Get(21), 6);
    FieldActor_SetSpriteColor(Actor_Get(22), 6);
    FieldActor_SetSpriteColor(Actor_Get(23), 6);
    FieldActor_SetSpriteColor(Actor_Get(24), 6);
    FieldActor_SetSpriteColor(Actor_Get(25), 6);
  }
  return 0;
}
#endif

void VenusLighthouse_DoubtLeaving(void);

// The Japanese edition has no form of this function here.
#if TLA_PATCH_1
s32 VenusLighthouse_Start(void) {
  struct MainRoutineWork *routine;

  routine = &gMainRoutineWork;
  ((struct AerieEventWork__floor *)Ram_HeapSlots->eventWork)->mode = 0x204;
  if (routine->entrance == ENTRANCE_OPENING) {
    // From patch 1 it also saves this scene and entrance 22.
    routine->savedScene = (s32)&SceneId_VenusLighthouse;
    routine->savedEntrance = 22;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    VenusLighthouse_FelixLeaves();
    VenusLighthouse_DoubtLeaving();
    Event_End();
  } else {
    FieldActor_SetSpriteColor(Actor_Get(10), 6);
    FieldActor_SetSpriteColor(Actor_Get(17), 6);
    FieldActor_SetSpriteColor(Actor_Get(18), 6);
    FieldActor_SetSpriteColor(Actor_Get(19), 6);
    FieldActor_SetSpriteColor(Actor_Get(20), 6);
    FieldActor_SetSpriteColor(Actor_Get(21), 6);
    FieldActor_SetSpriteColor(Actor_Get(22), 6);
    FieldActor_SetSpriteColor(Actor_Get(23), 6);
    FieldActor_SetSpriteColor(Actor_Get(24), 6);
    FieldActor_SetSpriteColor(Actor_Get(25), 6);
  }
  return 0;
}
#endif

s32 Scene_PrepareMap(void) { return 0; }

static inline void Venus_ObjEffectSet(s32 actor, s32 value) {
  Exit_PrepareActorAnimation(actor, value);
}

/// The prologue's opening: Felix moves the statue aside with Psynergy and
/// climbs off alone, leaving Jenna and Kraden.
void VenusLighthouse_FelixLeaves(void) {
  struct EventWork *work;

  Actor_SetPosition(4, PIXELS(712), PIXELS(816));
  Actor_SetPosition(5, PIXELS(696), PIXELS(840));
  Actor_SetPosition(8, PIXELS(728), PIXELS(840));
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_SetSpeed(9, 0x10000, 0x8000);
  FieldCamera_MoveTo(PIXELS(664), -1, PIXELS(824), 0);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 256;
  work->delay = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(4, 664, 824);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(8, 712, 824);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  VenusLighthouse_FelixMovesStatue();
  VenusLighthouse_SinkStatue();
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(8, 728, 840);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_WalkToAndWait(4, 712, 816);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  FieldActor_WatchAndSetScript(5, 4);
  FieldActor_WatchAndSetScript(8, 4);
  Actor_WalkToAndWait(4, 744, 816);
  Actor_WalkToAndWait(4, 744, 780);
  Event_PlaySound(129);
  Exit_PrepareActorAnimation(4, -1);
  FieldEvent_Wait(50);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Actor_SetPosition(4, 0, 0);
}

/// ★ Named from the game: "Are you sure we should be leaving like this?"
/// (MsgVenusLighthouseAreYouSureWeShouldBeLeaving)
void VenusLighthouse_DoubtLeaving(void) {
  struct VenusActor *actor;
  struct EventWork *work;

  FieldEvent_SetMessageId((s32)MsgVenusLighthouseAreYouSureWeShouldBeLeaving);
  EventCamera_FollowActor(8, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(5, 664, 840);
  Actor_FaceDirection(5, 0, 0);
  Actor_WalkToAndWait(8, 696, 840);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(10);
  Actor_WalkTo(8, 696, 920);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(5, 664, 888);
  FieldActor_WaitForMoveById(8);
  FieldActor_ShowEmote(8, 256, 30);
  Actor_FaceDirection(8, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(8, 696, 888);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(PIXELS(696), -1, PIXELS(920), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(8, 257, 50);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(8, 257, 30);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(8, 258, 40);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(30);
  Event_PlaySound(198);
  Actor_SetPosition(9, PIXELS(680), PIXELS(920));
  Actor_FaceDirection(9, 0xb000, 0);
  Actor_VanishInSparks(9);
  Actor_FaceDirection(9, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(5, 2);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  Actor_FaceEachOther(5, 8, 40);
  FieldActor_ShowEmote(9, 256, 30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(8, 258, 40);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(9, 257, 30);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(9, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(20);
  Actor_SetSpeed(5, 0x8000, 0x4000);
  FieldActor_WatchAndSetScript(8, 5);
  Actor_WalkByAndWait(5, 0, -32);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(9, 261, 30);
  Actor_WalkByAndWait(9, -8, -8);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, 0x5000, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(9, 256, 30);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(9, 257, 30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(8, 258, 0);
  FieldActor_ShowEmote(5, 258, 40);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(9, 264, 30);
  Actor_FaceDirection(9, 0xf000, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(5, 262, 40);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(5, 0x3000, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(9, 265, 30);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_Wait(10);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_WalkByAndWait(5, 0, 16);
  Actor_FaceDirection(5, 0x3000, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(5, 263, 30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 7);
  FieldEvent_Wait(30);
  Actor_FaceDirection(8, 0x9000, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(8, 258, 40);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, 0x1000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(5, 0, 16);
  Actor_FaceDirection(5, 0x3000, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(5, 261, 50);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, 0x3000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(8, 258, 40);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(8, 265, 30);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(5, 258, 40);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(8, 261, 30);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(5, 263, 30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_WalkByAndWait(5, 0, -16);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  Actor_FaceDirection(8, 0x5000, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(8, 0x9000, 0);
  Actor_FaceDirection(9, 0xd000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(5, 0x1000, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(5, 256, 30);
  Actor_FaceDirection(5, 0x3000, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0xf000, 0);
  FieldEvent_Wait(25);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(5, 258, 40);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_FaceEachOther(5, 8, 50);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(9, 264, 30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 6);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(8, 257, 30);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, 0x5000, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(9, 256, 30);
  Actor_FaceDirection(9, 0xf000, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(5, 257, 30);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(9, 258, 40);
  Actor_FaceDirection(9, 0xb000, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0xf000, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0xb000, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, 0x1000, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(5, 261, 30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimation(9, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_SetAnimation(8, 2);
  actor = (struct VenusActor *)Actor_Get(5);
  if (actor != NULL)
    Actor_SetDestination(8, actor->xPixel, actor->zPixel);
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  Actor_SetAnimation(9, 2);
  actor = (struct VenusActor *)Actor_Get(5);
  if (actor != NULL)
    Actor_SetDestination(9, actor->xPixel, actor->zPixel);
  FieldActor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(5, 16, 0);
  Actor_WalkToAndWait(5, 680, 944);
  FieldEvent_Wait(15);
  Actor_FaceDirection(5, 0xd000, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(15);
  Actor_WalkToAndWait(5, 728, 944);
  Actor_WalkToAndWait(5, 728, 928);
  Actor_SetPosition(5, PIXELS(728), PIXELS(912));
  Event_PlaySound(129);
  Venus_ObjEffectSet(5, -1);
  FieldEvent_Wait(40);
  MapGrid_CopyCellsTo(53, 50, 42, 49, 1, 1);
  MapGrid_CopyCellsTo(55, 117, 41, 117, 3, 5);
  Event_ClearFlag(513);
  Actor_SetPosition(10, PIXELS(664), PIXELS(792));
  FieldCamera_MoveTo(-1, -1, -1, 0);
  gMainRoutineWork.currentOwner = 5;
  Party_RemoveMember(4);
  FieldEvent_Wait(50);
  work = Ram_HeapSlots->eventWork;
  work->delay = 60;
  work->screenEffect = 256;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(22);
}

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38,
};

DEFINE_PUSH_LEADER_STEPS(gPushLeaderSteps)
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gVenusLighthouse_LeaderFlashes[] = {
    39,
    12,
    22,
    38,
    (s32)OverlayObject_UpdateEveryFourFrames,
    (s32)0x80010000,
    22,
    9,
    0x11999,
    22,
    10,
    0x11999,
    0,
    2,
    22,
    9,
    0xe666,
    22,
    10,
    0xe666,
    0,
    2,
    13,
    10,
    (s32)0xc0010000,
    22,
    9,
    0x10000,
    22,
    10,
    0x10000,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 120, 0, 200, FACING_NORTH, 0, 0, 56, 240, 216, 0},
    {2, CONDITION_ALWAYS, 120, 0, 104, FACING_SOUTH, 0, 0, 56, 240, 216, 0},
    {3, CONDITION_ALWAYS, 344, 0, 232, FACING_NORTH, 0, 256, 48, 512, 248, 0},
    {4, CONDITION_ALWAYS, 472, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0},
    {5, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0},
    {6, CONDITION_ALWAYS, 344, 0, 104, FACING_SOUTH, 0, 256, 48, 512, 248, 0},
    {7, CONDITION_ALWAYS, 840, 0, 248, FACING_NORTH, 0, 672, 32, 1008, 256, 0},
    {8, CONDITION_ALWAYS, 968, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0},
    {9, CONDITION_ALWAYS, 856, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0},
    {10, CONDITION_ALWAYS, 808, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0},
    {11, CONDITION_ALWAYS, 712, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0},
    {12, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 16, 288, 256, 520, 0},
    {13, CONDITION_ALWAYS, 104, 0, 504, FACING_NORTH, 0, 16, 288, 256, 520, 0},
    {14, CONDITION_ALWAYS, 104, 0, 472, FACING_SOUTH, 0, 16, 288, 256, 520, 0},
    {20, CONDITION_ALWAYS, 520, 0, 696, FACING_SOUTH, 0, 304, 344, 576, 728, 0},
    {21, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 304, 344, 576, 728, 0},
    {22, CONDITION_ALWAYS, 728, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0},
    {23, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 600, 736, 840, 952, 0},
    {24, CONDITION_ALWAYS, 392, 0, 952, FACING_SOUTH, 0, 272, 808, 512, 984, 0},
    {25, CONDITION_ALWAYS, 392, 0, 856, FACING_SOUTH, 0, 272, 808, 512, 984, 0},
    {26, CONDITION_ALWAYS, 760, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0},
    {30, CONDITION_ALWAYS, 88, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0},
    {31, CONDITION_ALWAYS, 184, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0},
    {32, CONDITION_ALWAYS, 184, 0, 648, FACING_SOUTH, 0, 16, 600, 256, 920, 0},
    {98, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 600, 736, 840, 952, 0},
    {99, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 576, 736, -1, 968, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(5),        SCENE_EXIT(1, 4, 9),   SCENE_EXIT(2, 5, 3),
    SCENE_EXIT(3, 5, 2),   SCENE_EXIT(4, 5, 31),  SCENE_EXIT(5, 5, 30),
    SCENE_EXIT(6, 5, 7),   SCENE_EXIT(7, 5, 6),   SCENE_EXIT(8, 5, 32),
    SCENE_EXIT(9, 5, 22),  SCENE_EXIT(10, 5, 12), SCENE_EXIT(11, 5, 13),
    SCENE_EXIT(12, 5, 10), SCENE_EXIT(13, 5, 11), SCENE_EXIT(14, 5, 24),
    SCENE_EXIT(20, 5, 26), SCENE_EXIT(21, 5, 25), SCENE_EXIT(22, 5, 9),
    SCENE_EXIT(24, 5, 14), SCENE_EXIT(25, 5, 21), SCENE_EXIT(26, 5, 20),
    SCENE_EXIT(30, 5, 5),  SCENE_EXIT(31, 5, 4),  SCENE_EXIT(32, 5, 8),
    SCENE_EXITS_END,
};

s32 gVenusLighthouse_StatueGlows[] = {
    22, 35, 0x2126, 22, 39, 1, 46, (s32)SceneActor_ApplyCounterLowBitsAsMode,
    17,
};

s32 gVenusLighthouse_ShrinkAndHide[] = {
    22,
    9,
    0x9999,
    22,
    10,
    0x9999,
    22,
    30,
    0,
    46,
    (s32)SceneActor_ClearSingleChildField,
    17,
};

struct ActorPlacement gActorPlacements[] = {
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0},
    {57, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(664), 0,
     PIXELS(792), FACING_SOUTH, 2, 0},
    {229, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(12), FACING_SOUTH, 2, 0},
    {230, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(12), FACING_SOUTH, 2, 0},
    {229, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_EAST, 0, 0},
    {315, CONDITION_ALWAYS, 1, PIXELS(872), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {315, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(104), FACING_NORTH, 2, 1},
    {452, CONDITION_ALWAYS, (s32)gVenusLighthouse_ShrinkAndHide, PIXELS(488), 0,
     PIXELS(536), FACING_SOUTH, 2, 0},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(72), 0,
     PIXELS(104), FACING_SOUTH, 2, 0},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(72), 0,
     PIXELS(168), FACING_SOUTH, 2, 1},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(168), 0,
     PIXELS(104), FACING_SOUTH, 2, 1},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(168), 0,
     PIXELS(168), FACING_SOUTH, 2, 1},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(72), 0,
     PIXELS(344), FACING_SOUTH, 2, 0},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(104), 0,
     PIXELS(344), FACING_SOUTH, 2, 1},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(136), 0,
     PIXELS(344), FACING_SOUTH, 2, 1},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(168), 0,
     PIXELS(344), FACING_SOUTH, 2, 1},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(200), 0,
     PIXELS(344), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gActorPlacementsEntrance99[] = {
    {4, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0},
    {57, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0},
    {336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows, PIXELS(664), 0,
     PIXELS(792), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {49, 4, CONDITION_ALWAYS, 4},
    {49, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {49, 8, CONDITION_ALWAYS, 8},
    {33, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {33, 14, CONDITION_ALWAYS, 14},
    {49, 20, CONDITION_ALWAYS, 20},
    {49, 21, CONDITION_ALWAYS, 21},
    {49, 22, CONDITION_ALWAYS, 22},
    {49, 23, CONDITION_ALWAYS, 23},
    {49, 24, CONDITION_ALWAYS, 24},
    {33, 25, CONDITION_ALWAYS, 25},
    {33, 26, CONDITION_ALWAYS, 26},
    {33, 30, CONDITION_ALWAYS, 30},
    {33, 31, CONDITION_ALWAYS, 31},
    {33, 32, CONDITION_ALWAYS, 32},
    {TRIGGER_STEP, 45, 0x200, EVENT(VenusLighthouse_RepelLeader)},
    {TRIGGER_STEP, 46, CONDITION_ALWAYS,
     EVENT(VenusLighthouse_StepOnFloorSwitch)},
    {TRIGGER_EXAMINE, 35, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief)},
    {TRIGGER_EXAMINE, 36, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief)},
    {TRIGGER_EXAMINE, 37, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief)},
    {TRIGGER_EXAMINE, 38, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief)},
    {TRIGGER_EXAMINE, 39, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief)},
    {TRIGGER_EXAMINE, 40, CONDITION_ALWAYS,
     EVENT(VenusLighthouse_ReadTrueHeart)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_RevealPaths)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_HidePaths)},
    {TRIGGER_TABLE_END},
};
#undef Actor_SetSpriteColor
