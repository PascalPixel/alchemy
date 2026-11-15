/// @file
/// The Venus Lighthouse, as the prologue opens on it: Felix moves a statue
/// with Psynergy and goes on alone, and Jenna and Kraden, doubting they should
/// leave, meet Alex and set off for Idejima (SceneId_VenusLighthouse).

#include "../field/object/set_sprite_priority.c"
#include "battle/battle.h"
#include "flags.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field_event.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "system/scene.h"
#include "scene_tables.h"
#include "system/frame.h"
#include "shared/tables.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/event.h"
#include "flag_call.h"
#include "module/field/event_commands.h"
#include "module/game/game.h"
#include "tla_edition.h"
#include "scene_ids.h"
#include "version.h"
#include "objcmd.h"
#include "fixed_math.h"

/// The scene's placed actors. At ENTRANCE_OPENING Felix and Jenna are placed as
/// the first two rows of gActorPlacementsEntrance99 and keep their own ids.
/// Set once the statue Felix moved has sunk into the floor; the statue by it no
/// longer throws the leader back.
#define FLAG_VENUS_LIGHTHOUSE_STATUE_SUNK FLAG_SCENE_TEMP_00
/// Set while the leader stands on the floor switch, pressed down.
#define FLAG_VENUS_LIGHTHOUSE_SWITCH_PRESSED FLAG_SCENE_TEMP_01

enum VenusLighthouseActorId {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  ACTOR_ALEX,
  /// The statue Felix moves aside with Move; it sinks into the floor.
  ACTOR_MOVED_STATUE,
  /// The first of the nine statues that glow (gVenusLighthouse_StatueGlows),
  /// tinted as the party walks in.
  ACTOR_FIRST_GLOWING_STATUE = ACTOR_FIRST_PLACED + 9
};

#define Actor_SetSpriteFlags                                                   \
  ((void (*)(struct Actor *, s32))Actor_SetSpriteFlags)

#define Actor_SetSpriteColor                                                   \
  ((void (*)(struct Actor *, s32))Actor_SetSpriteColor)

void ConfigureOverlayObject(struct Actor *object, s32 parameter) {
  object->motionFlags = 0;
  object->collisionFlags = 8;
  Actor_SetSpriteFlags(object, 0);
  FieldActor_SetSpriteColor(object, parameter);
}

struct Actor *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = FieldActor_Create(kind, x, y, z);

  if (object != 0) {
    Scene_SetActorSpritePriority(object, 0);
    ConfigureOverlayObject(object, 14);
    Actor_SetBlendMode(object, 1);
    return object;
  }
  return 0;
}

struct Actor *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z,
                                                       s32 kind) {
  struct Actor *object = FieldActor_Create(kind, x, y, z);

  if (object != 0) {
    Scene_SetActorSpritePriority(object, 1);
    ConfigureOverlayObject(object, 15);
    object->priorityFlags |= 2;
    return object;
  }
  return 0;
}

#include "../field/field_effect/move.c"

#undef Actor_SetSpriteFlags

extern void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);
extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
extern const s32 *gFieldEffect_DurationScripts[];
void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      (struct FieldEffect *)Actor_GetById(gMainRoutineWork.leaderActorId);
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

#include "shared/distance.c"

#include "../field/object/find_in_cell.c"

extern s32 gPushLeaderSteps[];

s32 MapGrid_CheckCollision(struct StagedActor *object,
                           struct FieldPosition *position);

void VenusLighthouse_PushBlockOnto(void) {
  struct FieldPosition dst;
  struct StagedActor *lead;
  struct StagedActor *next;
  struct StagedActor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = (struct StagedActor *)Actor_GetById(gMainRoutineWork.leaderActorId);
  dir = lead->facing >> 12;
  step = gPushLeaderSteps[dir];
  dst.x = lead->x.fixed + (step & 0xffff0000);
  dst.y = lead->y.fixed;
  step <<= 16;
  dst.z = lead->z.fixed + step;
  next = Actor_FindInCell(&dst, lead);
  if (next == 0)
    return;

  step = gPushLeaderSteps[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst.x = next->x.fixed;
  dst.y = next->y.fixed + px(16);
  dst.z = next->z.fixed;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->transitionMode = 2;
  step = gPushLeaderSteps[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  if (MapGrid_CheckCollision(next, &dst) > 0)
    return;

  if (next->transitionBusy != 0)
    return;

  Import_ActorSelectAnimation(lead, 8);
  rate = 0x3333;
  System_Wait(15);
  Event_PlaySound(SE_PUSH);
  next->moveRateX = rate;
  next->moveRateZ = rate;
  FieldActor_SetMoveTargetFixed(next, dst.x, dst.y, dst.z);
  lead->moveRateX = rate;
  lead->moveRateZ = rate;
  FieldActor_SetMoveTargetFixed(lead, dst.x, dst.y, dst.z);
  FieldActor_WaitForMove(next);
  Event_NoOp();
  next->x.fixed = dst.x;
  {
    /* FAKEMATCH(temporary): z read ahead of velocityX's store. */
    s32 z = dst.z;

    next->velocityX = 0;
    next->z.fixed = z;
    next->velocityZ = 0;
  }
  lead->targetX = 0x80000000;
  lead->targetZ = 0x80000000;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x.fixed = lead->x.part[0].pixel << 16;
  lead->z.fixed = lead->z.part[0].pixel << 16;
  Import_ActorSelectAnimation(lead, 1);
}

#include "../field/map/cell_fill_byte.c"

/// Unused: does nothing; nothing calls it.
UNUSED void VenusLighthouse_Nothing(void) {}

#define FieldActor_SetSpriteColor ((void (*)(void *, s32))Actor_SetSpriteColor)

s32 OverlayObject_UpdateEveryFourFrames(void *obj) {
  if ((gFrameCount & 3) == 0)
    FieldActor_SetSpriteColor(obj, 7);
  else
    FieldActor_SetSpriteColor(obj, 0);

  if ((gFrameCount & 7) == 0)
    Event_PlaySound(SE_ITEM_BREAK);
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
                    0xb0000, &options);
done:
  return 0;
}

#define Actor_SetSpriteFlags ((void (*)(void *, s32))Actor_SetSpriteFlags)

extern struct ActorPlacement gActorPlacementsEntrance99[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

s32 SceneActor_ApplyCounterLowBitsAsMode(struct Actor *actor) {
  FieldActor_SetSpriteColor(actor, actor->timer & 15);
  return 0;
}

s32 SceneActor_ClearSingleChildField(void *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.entrance == ENTRANCE_ALTERNATE_PLACEMENTS)
    return gActorPlacementsEntrance99;
  return gActorPlacements;
}

#undef FieldActor_SetSpriteColor

// The Venus Lighthouse's floor events and data: Felix moving the statue and
// leaving Jenna and Kraden, their talk of leaving, the floor switch, the
// statue that repels the leader, the reliefs and the paths Reveal shows.

enum { ENTRANCE_OPENING = 99 };

struct SetupWork {
  u8 unk00[0x24];
  s32 (*idle)(void);
};

extern s32 gPushLeaderSteps[];

void VenusLighthouse_FelixLeaves(void);

void Actor_VanishInSparks(s32 actor);

void VenusLighthouse_RepelLeader(void);

void VenusLighthouse_StepOnFloorSwitch(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern s32 gVenusLighthouse_StatueGlows[];
extern s32 gVenusLighthouse_ShrinkAndHide[];

void Exit_PrepareActorAnimation(s32 actor, s32 value);

extern s32 gVenusLighthouse_LeaderFlashes[];

void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 actor, s32 x, s32 z);
void Actor_SetAnimationAndWait(s32 actor, s32 mode);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
void Actor_WaitForMove(struct Actor *actor);
/* VIEW(floor): routine Actor_ShowEmoteById as void Actor_ShowEmoteById(s32
 * actor, s32 height, s32 frames); event.h has s32 Actor_ShowEmoteById(s32
 * actorId, s32 emote, s32 frames); tried as the definition's struct
 * BattleEffectWatchedObject *Actor_ShowEmoteById(s32 resourceId, s32 flags, s32
 * state), it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse.c:388: conflicting types for
 * `Actor_ShowEmoteById'. */
extern void
Actor_ShowEmoteById__floor(s32 actor, s32 emote,
                           s32 frames) __asm__("Actor_ShowEmoteById");

/// Until the statue has sunk, stepping by it throws the leader
/// back in a flash of sparks.
void VenusLighthouse_RepelLeader(void) {
  s32 leader = Party_GetLeaderActor();
  struct Actor *actor = Actor_GetById(leader);
  s32 z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetMovementTypeById(leader, gVenusLighthouse_LeaderFlashes);
  Actor_WaitForScriptEndById(leader);
  Actor_SetSpriteColor(Actor_GetById(leader), 6);
  actor->velocityY = px(4);
  Actor_SetSpeed(leader, 4);
  if (actor->z.fixed >> 20 <= 54) {
    Actor_GetById(leader)->steeringFlags &= ~1;
    z = 210;
  } else {
    Actor_GetById(leader)->steeringFlags &= ~1;
    z = 238;
  }
  Actor_WalkToAndWaitById(leader, actor->x.part[0].pixel, z * 4);
  Event_Wait(1);
  Actor_GetById(leader)->steeringFlags |= 1;
  Event_Wait(20);
  Actor_SetSpriteColor(actor, 0);
  actor->update = VenusLighthouse_EmitRisingParticles;
  Actor_ShowEmoteById__floor(leader, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetAnimationAndWait(leader, 4);
  Actor_SetSpriteColor(Actor_GetById(leader), 0);
  Actor_SetAnimationAndWait(leader, 4);

  /* FAKEMATCH(dead): an empty once-only block. */
  do {
  } while (0);

  actor->update = 0;
  Event_End();
}

void VenusLighthouse_StepOnFloorSwitch(void) {
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  s32 z = leader->z.part[0].pixel;

  if ((u32)(leader->x.part[0].pixel - 676) > 7 || z < 788 || z >= 796) {
    MapGrid_CopyCellPictureIndicesRect(53, 50, 42, 49, 1, 1);
    MapGrid_CopyCellsTo(55, 117, 41, 117, 3, 5);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_SWITCH_PRESSED);
    leader->motionFlags |= 1;
    leader->groundHeight = 0;
    leader->y.fixed = 0;
  } else if (!Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_SWITCH_PRESSED)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_Wait(5);
    MapGrid_CopyCellPictureIndicesRect(52, 50, 42, 49, 1, 1);
    MapGrid_CopyCellsTo(52, 117, 41, 117, 3, 5);
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_SWITCH_PRESSED);
    Sound_Play(SE_RATTLE);
    leader->motionFlags &= ~1;
    leader->groundHeight = -px(2);
    leader->y.fixed = -px(2);
    Event_End();
  }
}

#include "text/msg_ids.h"

/// The statue Felix moved sinks into the floor in column 42, and
/// the way past it opens.
void VenusLighthouse_SinkStatue(void) {
  struct Actor *obj;
  s32 val;
  s32 a;
  s32 b;

  obj = Actor_Get(ACTOR_MOVED_STATUE);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (obj->x.fixed >> 20 == 42) {
    FieldEvent_Wait(30);
    Event_PlaySound(SE_THUMP);
    obj->motionFlags = 0;
    val = -px(2);
    obj->groundHeight = val;
    obj->y.fixed = val;
    Event_SetFlag(FLAG_VENUS_LIGHTHOUSE_STATUE_SUNK);
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
  // "There are words carved into the relief... If ye seek to climb Venus..."
  Message_ShowCentered((s32)MsgVenusLighthouseReliefWords, 1);
  Event_End();
}

void VenusLighthouse_ReadTrueHeart(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(FieldParty_GetLeaderActor(), 1);
  // "The true road can only be known by he who bears a true heart."
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

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 VenusLighthouse_Idle(void) { return 0; }

/// Felix casts Move on the statue.
void VenusLighthouse_FelixMovesStatue(void) {
  FieldPsynergy_LoadResources(ABILITY_MOVE, 1);
  FieldPsynergy_SetupActorPair(4, ACTOR_MOVED_STATUE);
  ((struct SetupWork *)Ram_HeapSlots->fieldEffectWork)->idle =
      VenusLighthouse_Idle;
  FieldPsynergy_StartCast();
  FieldEvent_Wait(10);
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
}

// The Japanese form; from patch 1, the form below also saves this scene and
// entrance 22.
#if !TLA_PATCH_1
s32 VenusLighthouse_Start(void) {
  gFieldWork->transition = 0x204;
  if (gMainRoutineWork.entrance == ENTRANCE_OPENING) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    VenusLighthouse_FelixLeaves();
    VenusLighthouse_DoubtLeaving();
    Event_End();
  } else {
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_MOVED_STATUE), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 1), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 2), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 3), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 4), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 5), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 6), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 7), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 8), 6);
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
  gFieldWork->transition = 0x204;
  if (routine->entrance == ENTRANCE_OPENING) {
    // From patch 1 it also saves this scene and entrance 22.
    routine->restartScene = SCENE_VENUS_LIGHTHOUSE;
    routine->restartEntrance = 22;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    VenusLighthouse_FelixLeaves();
    VenusLighthouse_DoubtLeaving();
    Event_End();
  } else {
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_MOVED_STATUE), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 1), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 2), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 3), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 4), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 5), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 6), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 7), 6);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GLOWING_STATUE + 8), 6);
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
  struct FieldWork *work;

  Actor_SetPosition(ACTOR_FELIX, 712, 816);
  Actor_SetPosition(ACTOR_JENNA, 696, 840);
  Actor_SetPosition(ACTOR_KRADEN, 728, 840);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_SetSpeed(ACTOR_ALEX, 1);
  FieldCamera_MoveTo(664, CAMERA_KEEP, 824, 0);
  work = Ram_HeapSlots->fieldWork;
  work->transition = 256;
  work->transitionFrames = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_FELIX, 664, 824);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 712, 824);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  VenusLighthouse_FelixMovesStatue();
  VenusLighthouse_SinkStatue();
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(ACTOR_KRADEN, 728, 840);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 712, 816);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_FELIX);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_FELIX);
  Actor_WalkToAndWait(ACTOR_FELIX, 744, 816);
  Actor_WalkToAndWait(ACTOR_FELIX, 744, 780);
  Event_PlaySound(SE_STAIRS_DOWN);
  Exit_PrepareActorAnimation(4, -1);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_SetPosition(ACTOR_FELIX, 0, 0);
}

void VenusLighthouse_DoubtLeaving(void) {
  struct Actor *actor;
  struct FieldWork *work;

  FieldEvent_SetMessageId((s32)MsgVenusLighthouseAreYouSureWeShouldBeLeaving);
  EventCamera_FollowActor(ACTOR_KRADEN, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(ACTOR_JENNA, 664, 840);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 696, 840);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_WalkTo(ACTOR_KRADEN, 696, 920);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_JENNA, 664, 888);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(ACTOR_KRADEN, 696, 888);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(696, CAMERA_KEEP, 920, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  // "Are you sure we should be leaving like this?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  // "Once we go down those stairs, we can't get back into the lighthouse."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  // "Maybe I should have stopped my brother..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Jenna, is something troubling you?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  // "I just..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "What is it? What's wrong?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "I have a bad feeling... Like something terrible is going to happen."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_KOLIMA_BRIDGE_POST);
  Actor_SetPosition(ACTOR_ALEX, 680, 920);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  Actor_VanishInSparks(ACTOR_ALEX);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "How unlike you, Jenna."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(20);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_KRADEN, 40);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Surprised to see me?"
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "A-Alex... Were you listening?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH, 0);
  FieldEvent_Wait(30);
  // "Where's Felix? Why isn't he here with you?"
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_JENNA, 0.5);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_JENNA);
  Actor_WalkByAndWait(ACTOR_JENNA, 0, -32);
  FieldEvent_Wait(10);
  // "My brother? He left us."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Actor_WalkByAndWait(ACTOR_ALEX, -8, -8);
  // "What can Felix possibly be thinking?"
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "He was worried about Sheba. He went to check on her."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  // "He was supposed to be leading you out of here! I thought you would..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Why are you still here?"
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "We tried to stop Felix from going back up to the aerie."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(30);
  // "How like him. Once Felix gets an idea into his head, he rarely..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  // "Alex, what were you talking about just now?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  // "What did you mean when you said... "How unlike you"?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "I was merely surprised to hear you expressing such concern, Jenna."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "Nevertheless, I'm impressed Felix went back..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_WalkByAndWait(ACTOR_JENNA, 0, 16);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(10);
  // "Don't change the subject, Alex! What did you mean!?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Are you saying that I'm insensitive!?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "If that's what you heard, then I must have misspoken. Accept my..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 7);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Well, Jenna, I think Alex has said his piece on the matter..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "I'm not sure he has, Kraden."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_JENNA, 0, 16);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(10);
  // "Who do you think you are, talking to me like that!?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "I retract my statement, my dear. Forget all about it, Jenna."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "So tell me about this "feeling," Jenna..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "I'm just... I'm not sure we should be leaving Venus Lighthouse yet..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "What are you saying?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "You can't really mean to remain here..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Alex is right. It's far too dangerous, Jenna. We would only hinder..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "The others? Did Isaac follow us to the lighthouse?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "It would seem so."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  // "He must be trying to stop Saturos and Menardi!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "If my brother goes back and finds Isaac, they're going to end up..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "If the Venus Lighthouse has not yet been lit, yes. They will fight."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "Felix is a terribly rash young man, is he not?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Why are boys such fools?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  // "He may be rash, but Felix is no fool. His good qualities outweigh..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  // "That's why I think Isaac would understand if we just talked to him."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_WalkByAndWait(ACTOR_JENNA, 0, -16);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  // "Why didn't I think of this before now?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  // "Please... Can't we go back up and talk to Isaac?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "I'm afraid that's not possible."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "Alex, why?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "He is an enemy."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(20);
  // "Isaac? An enemy?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(25);
  // "Our methods may differ, but you and I ultimately want the same thing..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  // "To light the elemental lighthouses..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "Isaac and his friends would prevent this from happening."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "So that makes them enemies?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "But fear not... They won't be able to defeat Saturos and Menardi."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_KRADEN, 50);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "So... Felix went back to the top of the lighthouse?"
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 6);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "Why do you seem so pleased, Alex?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(20);
  // "Why shouldn't I be? After all, the lighthouse will soon shine..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "Alex, why do you want to see the beacon lit so badly?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(30);
  // "Oh, so it's my turn to answer questions, is it?"
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "Once, Alchemy was commonplace throughout this world..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(10);
  // "With its power, mankind worked wonders across the land..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  // "The lost age of man..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "I want to see that world restored once again, and..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "And what, Alex?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "We've spoken long enough already. Let us continue this another time."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "We should leave the lighthouse now, before the beacon is fired..."
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "I concur... Let's meet at the location we agreed on and wait for Felix."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "All right, Kraden, let's go."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_ALEX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  actor = (struct Actor *)Actor_Get(ACTOR_JENNA);
  if (actor != NULL)
    Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].pixel,
                         actor->z.part[0].pixel);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetSpeed(ACTOR_ALEX, 1.2);
  Actor_SetAnimation(ACTOR_ALEX, ANIM_WALK);
  actor = (struct Actor *)Actor_Get(ACTOR_JENNA);
  if (actor != NULL)
    Actor_SetDestination(ACTOR_ALEX, actor->x.part[0].pixel,
                         actor->z.part[0].pixel);
  FieldActor_WaitForMoveById(ACTOR_ALEX);
  Actor_SetPosition(ACTOR_ALEX, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_JENNA, 16, 0);
  Actor_WalkToAndWait(ACTOR_JENNA, 680, 944);
  FieldEvent_Wait(15);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldEvent_Wait(15);
  Actor_WalkToAndWait(ACTOR_JENNA, 728, 944);
  Actor_WalkToAndWait(ACTOR_JENNA, 728, 928);
  Actor_SetPosition(ACTOR_JENNA, 728, 912);
  Event_PlaySound(SE_STAIRS_DOWN);
  Venus_ObjEffectSet(5, -1);
  FieldEvent_Wait(40);
  MapGrid_CopyCellsTo(53, 50, 42, 49, 1, 1);
  MapGrid_CopyCellsTo(55, 117, 41, 117, 3, 5);
  Event_ClearFlag(FLAG_VENUS_LIGHTHOUSE_SWITCH_PRESSED);
  Actor_SetPosition(ACTOR_MOVED_STATUE, 664, 792);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  gMainRoutineWork.leaderActorId = 5;
  Party_RemoveMember(4);
  FieldEvent_Wait(50);
  work = Ram_HeapSlots->fieldWork;
  work->transitionFrames = 60;
  work->transition = 256;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(22);
}

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gPushLeaderSteps[] = {                                                               
      PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),                  
      PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),                  
      PUSH_STEP(0, 16),  PUSH_STEP(-16, 0), PUSH_STEP(-16, 0),                 
      PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),                 
      PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16),                 
      PUSH_STEP(16, 0),                                                        
  };
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
    SCENE_EXITS(VenusLighthouse),
    SCENE_EXIT(1, VenusLighthouseEntrance, 9),
    SCENE_EXIT(2, VenusLighthouse, 3),
    SCENE_EXIT(3, VenusLighthouse, 2),
    SCENE_EXIT(4, VenusLighthouse, 31),
    SCENE_EXIT(5, VenusLighthouse, 30),
    SCENE_EXIT(6, VenusLighthouse, 7),
    SCENE_EXIT(7, VenusLighthouse, 6),
    SCENE_EXIT(8, VenusLighthouse, 32),
    SCENE_EXIT(9, VenusLighthouse, 22),
    SCENE_EXIT(10, VenusLighthouse, 12),
    SCENE_EXIT(11, VenusLighthouse, 13),
    SCENE_EXIT(12, VenusLighthouse, 10),
    SCENE_EXIT(13, VenusLighthouse, 11),
    SCENE_EXIT(14, VenusLighthouse, 24),
    SCENE_EXIT(20, VenusLighthouse, 26),
    SCENE_EXIT(21, VenusLighthouse, 25),
    SCENE_EXIT(22, VenusLighthouse, 9),
    SCENE_EXIT(24, VenusLighthouse, 14),
    SCENE_EXIT(25, VenusLighthouse, 21),
    SCENE_EXIT(26, VenusLighthouse, 20),
    SCENE_EXIT(30, VenusLighthouse, 5),
    SCENE_EXIT(31, VenusLighthouse, 4),
    SCENE_EXIT(32, VenusLighthouse, 8),
    SCENE_EXITS_END,
};

s32 gVenusLighthouse_StatueGlows[] = {
    22, 35, 0x2126, 22, 39, 1, 46, (s32)SceneActor_ApplyCounterLowBitsAsMode,
    17,
};

s32 gVenusLighthouse_ShrinkAndHide[] = {
    SC_SET(SW_SCALE_X, 0x9999),
    SC_SET(SW_SCALE_Y, 0x9999),
    SC_SET(SW_COLLISION, 0),
    SC_CALL((s32)SceneActor_ClearSingleChildField),
    SC_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    664, 0, 792, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(229, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 12,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(230, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 12,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(229, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(315, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 120,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(315, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 104,
                    FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(452, CONDITION_ALWAYS, (s32)gVenusLighthouse_ShrinkAndHide,
                    488, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    72, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    72, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    168, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    168, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    72, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    104, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    136, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    168, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    200, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gActorPlacementsEntrance99[] = {
    ACTOR_PLACEMENT(4, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(336, CONDITION_ALWAYS, (s32)gVenusLighthouse_StatueGlows,
                    664, 0, 792, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 24, CONDITION_ALWAYS, 24},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 25, CONDITION_ALWAYS, 25},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 26, CONDITION_ALWAYS, 26},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 30, CONDITION_ALWAYS, 30},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 31, CONDITION_ALWAYS, 31},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 32, CONDITION_ALWAYS, 32},
    {TRIGGER_STEP, 45, FLAG_VENUS_LIGHTHOUSE_STATUE_SUNK,
     EVENT(VenusLighthouse_RepelLeader)},
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
#undef FieldActor_SetSpriteColor
#undef Actor_SetSpriteFlags
