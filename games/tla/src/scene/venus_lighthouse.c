/// @file
/// The Venus Lighthouse, as the prologue opens on it: Felix moves a statue onto
/// the floor switch with Psynergy and goes on alone, and Jenna and Kraden,
/// doubting they should leave, meet Alex and set off for Idejima
/// (SceneId_VenusLighthouse).
#include "fixed_math.h"
#include "flags.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "tla_edition.h"
#include "types.h"
#include "version.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_coordinate.h"
#include "field/field_event.h"
#include "field/field_psynergy_state.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/flag_call.h"
#include "field/scene_tables.h"
#include "game/character_ids.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/main_routine_state.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/scene.h"
#include "system/system.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"
#include "shared/tables.h"

/// Set once the statue Felix moved onto the floor switch has sunk into it and
/// holds it down; the barrier no longer throws the leader back.
#define FLAG_VENUS_LIGHTHOUSE_SWITCH_HELD FLAG_SCENE_TEMP_00
/// Set while the leader stands on the floor switch, pressed down.
#define FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH FLAG_SCENE_TEMP_01

/// The cell behaviors the scene's events answer, as The Broken Seal numbers
/// them on this floor.
enum VenusLighthouseCellBehavior {
  /// The first of the five reliefs VenusLighthouse_ReadRelief shows the words
  /// of.
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF = 35,
  /// The relief by the stairs down from the floor switch's room
  /// (VenusLighthouse_ReadTrueHeart).
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_TRUE_HEART_RELIEF = 40,
  /// The floor of yellow lightning south of the floor switch, which throws the
  /// leader back until the switch is held (VenusLighthouse_RepelLeader).
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_BARRIER = 45,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOOR_SWITCH = 46
};

/// The stairs down at the south of the floor switch's room, where the prologue
/// opens, to the room of the spiral pillars, as The Broken Seal names them:
/// Jenna, Kraden and Alex leave by its exit, and from patch 1 the game restarts
/// at its entrance.
enum {
  VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_PILLARS = 22,
  VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_PILLARS = 22
};

/// The scene's actors, numbering the rows of gActorPlacements: The Broken
/// Seal's ground floor's, with Kraden and Alex in place of Garet and its three
/// scholars set aside in the corner. At VENUS_LIGHTHOUSE_FROM_TITLE Felix and
/// Jenna are placed as the first two rows of
/// gVenusLighthouseOpeningActorPlacements and keep their own ids.
enum VenusLighthouseActorId {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  ACTOR_ALEX,
  /// The lady statue Felix moves onto the floor switch with Move; it sinks in
  /// and holds the switch down.
  ACTOR_FLOOR_SWITCH_STATUE,
  /// The Broken Seal's three scholars of this floor (SPRITE_OLD_MAN_IN_BLUE_CAP
  /// and SPRITE_MAN_WITH_GLASSES), under its names, kept at (8, 8) out of
  /// sight; they never speak here.
  ///@{
  ACTOR_SCHOLAR_ATTACKED_ANYWAY,
  ACTOR_SCHOLAR_NOT_THE_ENTRANCE,
  ACTOR_DOOR_SCHOLAR,
  ///@}
  /// The two spiral pillars below the stairs of entrance 9.
  ///@{
  ACTOR_SPIRAL_PILLAR_A,
  ACTOR_SPIRAL_PILLAR_B,
  ///@}
  /// The small model of the lighthouse (gVenusMiniatureLighthouseScript).
  ACTOR_MINIATURE_LIGHTHOUSE,
  /// The first of the nine lady statues drawn grey (gVenusGreyStatueScript),
  /// in the two rooms by the entrance.
  ACTOR_FIRST_GREY_STATUE
};

/// An actor's timer as gVenusGreyStatueScript sets it. It holds a tag in the
/// upper bits and a colour mode (enum SpriteColorMode) in the low four.
#define VENUS_COLOR_MARK(mode) (VENUS_COLOR_MARK_TAG << 4 | (mode))
enum { VENUS_COLOR_MARK_TAG = 0x212 }; ///< the tag that marks the timer

/// The scene's name for the byte Actor keeps for scene code. A statue with
/// SW_VENUS_IMMOVABLE set cannot be pushed.
enum { SW_VENUS_IMMOVABLE = SW_RISE_COUNTER };

#define Actor_SetSpriteFlags                                                   \
  ((void (*)(struct Actor *, s32))Actor_SetSpriteFlags)

#define Actor_SetSpriteColor                                                   \
  ((void (*)(struct Actor *, s32))Actor_SetSpriteColor)

/// Sets up a passage effect's motion, collision and colour mode.
void VenusLighthouse_ConfigurePassageEffect(struct Actor *actor,
                                            s32 colorMode) {
  actor->motionFlags = 0;
  actor->collisionFlags = ACTOR_NOT_TARGETED;
  Actor_SetSpriteFlags(actor, 0);
  FieldActor_SetSpriteColor(actor, colorMode);
}

/// Unused: creates a mask over a hole, an actor of sprite type drawn solid
/// black and translucent, as The Broken Seal's Venus Lighthouse drops the
/// leader or a pillar through its floor; nothing here calls it.
UNUSED struct Actor *
VenusLighthouse_CreateBlendedPassageEffect(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *effect = FieldActor_Create(type, x, y, z);

  if (effect != 0) {
    Scene_SetActorSpritePriority(effect, 0);
    VenusLighthouse_ConfigurePassageEffect(effect, SPRITE_COLOR_BLACK);
    Actor_SetBlendMode(effect, 1);
    return effect;
  }
  return 0;
}

/// Unused: creates an actor drawn hidden that keeps the leader on screen
/// while a pillar sinks by him, as The Broken Seal's Venus Lighthouse does;
/// nothing here calls it.
UNUSED struct Actor *
VenusLighthouse_CreateOccludingPassageEffect(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *actor = FieldActor_Create(type, x, y, z);

  if (actor != 0) {
    Scene_SetActorSpritePriority(actor, 1);
    VenusLighthouse_ConfigurePassageEffect(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    return actor;
  }
  return 0;
}

#include "../field/field_effect_move.c"

#undef Actor_SetSpriteFlags

extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
extern const s32 *gFieldEffect_DurationScripts[];

/// Puts a short-lived effect sprite, dust unless `options` names another, at
/// a point, moving off at the velocity given and drawn at the leader's
/// priority: this overlay's own copy of field_effect_spawn.c.
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      (struct FieldEffect *)Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect =
        (struct FieldEffect *)Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation((void *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  sprite->flags = 0;
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    FIXED targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - FIXED_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation((void *)effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#include "shared/position_distance.h"

#include "../field/actor_find_in_cell.c"

extern s32 gPushLeaderSteps[];

s32 MapGrid_CheckCollision(struct Actor *actor, struct FieldPosition *position);

/// Unused: the leader pushes what stands in front of him a cell on, unless
/// something solid is beyond it or on top of it, the ground beyond refuses it
/// or it is still moving, as The Broken Seal's pushing scenes do; nothing here
/// calls it.
UNUSED void PushedActor_AdvancePair(void) {
  struct FieldPosition targetPosition;
  struct Actor *leader;
  struct Actor *pillar;
  struct Actor *blockingActor;
  s32 directionIndex;
  u32 step;
  FIXED moveRate;
  s32 transitionBusy;

  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  directionIndex = FACING_INDEX(leader->facing);
  step = gPushLeaderSteps[directionIndex];
  targetPosition.x = leader->x.fixed + (step & FIELD_PIXEL_MASK);
  targetPosition.y = leader->y.fixed;
  step = INTtoFIXED(step);
  targetPosition.z = leader->z.fixed + step;
  pillar = Actor_FindInCell((s32 *)&targetPosition, leader);
  if (pillar == 0)
    return;

  step = gPushLeaderSteps[directionIndex];
  targetPosition.x = pillar->x.fixed + (step & FIELD_PIXEL_MASK);
  targetPosition.y = pillar->y.fixed;
  step = INTtoFIXED(step);
  targetPosition.z = pillar->z.fixed + step;
  blockingActor = Actor_FindInCell((s32 *)&targetPosition, pillar);
  if (blockingActor != 0 && (blockingActor->collisionFlags & ACTOR_SOLID) != 0)
    return;

  targetPosition.x = pillar->x.fixed;
  targetPosition.y = pillar->y.fixed + toFIXED(16);
  targetPosition.z = pillar->z.fixed;
  blockingActor = Actor_FindInCell((s32 *)&targetPosition, pillar);
  if (blockingActor != 0 && (blockingActor->collisionFlags & ACTOR_SOLID) != 0)
    return;

  pillar->layer = 2;
  step = gPushLeaderSteps[directionIndex];
  targetPosition.x = pillar->x.fixed + (step & FIELD_PIXEL_MASK);
  targetPosition.y = pillar->y.fixed;
  step = INTtoFIXED(step);
  targetPosition.z = pillar->z.fixed + step;
  if (MapGrid_CheckCollision(pillar, &targetPosition) > 0)
    return;

  transitionBusy = pillar->riseCounter;
  if (transitionBusy != 0)
    return;

  Actor_SelectAnimation(leader, ANIM_PUSH);
  moveRate = toFIXED(1) / 5;
  System_Wait(15);
  Event_PlaySound(SE_PUSH);
  pillar->speedLimit = moveRate;
  pillar->acceleration = moveRate;
  FieldActor_SetMoveTargetFX(pillar, targetPosition.x, targetPosition.y,
                             targetPosition.z);
  leader->speedLimit = moveRate;
  leader->acceleration = moveRate;
  FieldActor_SetMoveTargetFX(leader, targetPosition.x, targetPosition.y,
                             targetPosition.z);
  FieldActor_WaitForMove(pillar);
  Event_NoOp();
  pillar->x.fixed = targetPosition.x;
  pillar->z.fixed = targetPosition.z;
  pillar->velocityX = transitionBusy;
  pillar->velocityZ = transitionBusy;
  leader->targetX = ACTOR_NO_TARGET;
  leader->targetZ = ACTOR_NO_TARGET;
  leader->velocityX = transitionBusy;
  leader->velocityZ = transitionBusy;
  leader->x.fixed = INTtoFIXED(leader->x.part[0].integer);
  leader->z.fixed = INTtoFIXED(leader->z.part[0].integer);
  Actor_SelectAnimation(leader, ANIM_STAND);
}

#include "../field/map_cell_fill_byte.c"

/// Unused: does nothing; nothing calls it.
UNUSED void VenusLighthouse_Nothing(void) {}

#define FieldActor_SetSpriteColor ((void (*)(void *, s32))Actor_SetSpriteColor)

/// Flashes the leader white every fourth frame, with the sound of a breaking
/// item every eighth.
s32 VenusLighthouse_PulseChildEffect(void *actor) {
  if ((gFrameCount & 3) == 0)
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
  else
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);

  if ((gFrameCount & 7) == 0)
    Event_PlaySound(SE_ITEM_BREAK);
  return 0;
}

/// Emits a rising particle near the actor every eight frames.
s32 VenusLighthouse_EmitRisingParticles(struct Actor *actor) {
  struct EffectOptions options;
  s32 framePhase;
  FIXED particleX, particleY, riseSpeed;
  framePhase = gFrameCount & 7;
  if (framePhase == 0) {
    options.priority = 3 - (s32)(Random_Below(2));
    options.startScaleX = 2 * FIXED_ONE / 5;
    options.startScaleY = 2 * FIXED_ONE / 5;
    options.palette = SPRITE_COLOR_BLACK;
    particleX = actor->x.fixed + INTtoFIXED((s32)(Random_Below(9)) - 4);
    particleY = actor->y.fixed + INTtoFIXED(32 - (s32)(Random_Below(32)));
    riseSpeed = (INTtoFIXED((s32)(Random_Below(5))) + toFIXED(5)) / 10;
    FieldEffect_Spawn(
        particleX, particleY, actor->z.fixed, 0, riseSpeed, framePhase,
        EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
        &options);
  }
  return 0;
}

#define Actor_SetSpriteFlags ((void (*)(void *, s32))Actor_SetSpriteFlags)

extern struct ActorPlacement gVenusLighthouseOpeningActorPlacements[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// Draws the actor in the colour mode its VENUS_COLOR_MARK holds.
s32 VenusLighthouse_UpdateChildFromTimer(struct Actor *actor) {
  FieldActor_SetSpriteColor(actor, actor->timer & 15);
  return 0;
}

/// Clears the scripted actor's sprite flags and advances its script.
s32 VenusLighthouse_ClearScriptedSpriteFlags(void *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.entrance == VENUS_LIGHTHOUSE_FROM_TITLE)
    return gVenusLighthouseOpeningActorPlacements;
  return gActorPlacements;
}

#undef FieldActor_SetSpriteColor

// The Venus Lighthouse's floor events and data: Felix moving the statue and
// leaving Jenna and Kraden, their talk of leaving, the floor switch, the
// statue that repels the leader, the reliefs and the paths Reveal shows.

void Exit_PrepareActorAnimation(s32 id, s32 side);
void Actor_AppearInSparks(s32 objectId);
void VenusLighthouse_FelixLeaves(void);

void VenusLighthouse_RepelLeader(void);

void VenusLighthouse_StepOnFloorSwitch(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern s32 gVenusGreyStatueScript[];
extern s32 gVenusMiniatureLighthouseScript[];

extern s32 gVenusLeaderDazedScript[];

/// Sets bits in a byte of flags.
static inline void SetFlagBits(u8 *flags, u8 bits) { *flags |= bits; }

/// Until the statue holds the floor switch down, stepping onto the barrier
/// throws the leader back, dazed and flashing.
void VenusLighthouse_RepelLeader(void) {
  s32 leader = Party_GetLeaderActor();
  struct Actor *actor = Actor_GetById(leader);
  s32 z;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_SetMovementTypeById(leader, gVenusLeaderDazedScript);
  Actor_WaitForScriptEndById(leader);
  Actor_SetSpriteColor(Actor_GetById(leader), SPRITE_COLOR_GREY);
  actor->velocityY = toFIXED(4);
  Actor_SetSpeed(leader, 4);
  // Thrown back off the barrier the way he came, north to z 840 or south to
  // z 952.
  if (FIELD_CELL(actor->z.fixed) <= 54) {
    Actor_GetById(leader)->steeringFlags &= ~ACTOR_FACES_MOTION;
    z = 210;
  } else {
    Actor_GetById(leader)->steeringFlags &= ~ACTOR_FACES_MOTION;
    z = 238;
  }
  Actor_WalkToAndWaitById(leader, actor->x.part[0].integer, z * 4);
  Event_Wait(1);
  SetFlagBits(&Actor_GetById(leader)->steeringFlags, ACTOR_FACES_MOTION);
  Event_Wait(20);
  Actor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
  actor->update = VenusLighthouse_EmitRisingParticles;
  FieldActor_ShowEmote(leader, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetAnimationAndWait(leader, ANIM_SHAKE_HEAD);
  Actor_SetSpriteColor(Actor_GetById(leader), SPRITE_COLOR_NORMAL);
  Actor_SetAnimationAndWait(leader, ANIM_SHAKE_HEAD);

  actor->update = 0;
  Event_End();
}

/// Standing on the floor switch presses it
/// (FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH) and lowers the leader into
/// it; stepping off releases it.
void VenusLighthouse_StepOnFloorSwitch(void) {
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  s32 z = leader->z.part[0].integer;

  // Off the switch's middle, 8 pixels square from (676, 788).
  if ((u32)(leader->x.part[0].integer - 676) > 7 || z < 788 || z >= 796) {
    MapGrid_CopyCellTilesIndicesRect(53, 50, 42, 49, 1, 1);
    MapGrid_CopyCellsTo(55, 117, 41, 117, 3, 5);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH);
    leader->motionFlags |= ACTOR_FOLLOWS_TERRAIN;
    leader->groundHeight = 0;
    leader->y.fixed = 0;
  } else if (!Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    Event_Wait(5);
    MapGrid_CopyCellTilesIndicesRect(52, 50, 42, 49, 1, 1);
    MapGrid_CopyCellsTo(52, 117, 41, 117, 3, 5);
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH);
    Sound_Play(SE_RATTLE);
    leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
    leader->groundHeight = -toFIXED(2);
    leader->y.fixed = -toFIXED(2);
    Event_End();
  }
}

/// The statue Felix moved onto the floor switch, in column 42, sinks in and
/// holds it down, and the barrier lets the leader by.
void VenusLighthouse_SinkStatue(void) {
  struct Actor *statue;
  FIXED sunk;
  s32 width;
  s32 height;

  statue = Actor_Get(ACTOR_FLOOR_SWITCH_STATUE);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (FIELD_CELL(statue->x.fixed) == 42) {
    FieldEvent_Wait(30);
    Event_PlaySound(SE_THUMP);
    statue->motionFlags = 0;
    sunk = -toFIXED(2);
    statue->groundHeight = sunk;
    statue->y.fixed = sunk;
    Event_SetFlag(FLAG_VENUS_LIGHTHOUSE_SWITCH_HELD);
    width = 3;
    height = 5;
    MapGrid_CopyCellsTo(44, 117, 41, 117, width, height);
  }
  Event_End();
}

/// Searching the relief shows its words.
void VenusLighthouse_ReadRelief(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetAnimation(FieldParty_GetLeaderActor(), ANIM_STAND);
  // "There are words carved into the relief... If ye seek to climb Venus..."
  Message_ShowCentered((s32)MsgVenusLighthouseReliefWords, 1);
  Event_End();
}

/// Searching the other relief shows that only a true heart knows the road.
void VenusLighthouse_ReadTrueHeart(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetAnimation(FieldParty_GetLeaderActor(), ANIM_STAND);
  // "The true road can only be known by he who bears a true heart."
  Message_ShowCentered((s32)MsgVenusLighthouseTrueHeart, 1);
  Event_End();
}

/// Reveal fades: the paths it showed hide again.
void VenusLighthouse_HidePaths(void) {
  s32 x;
  s32 y;
  s32 wide;
  s32 narrow;

  x = 23;
  y = 8;
  MapGrid_CopyCellAttributes(35, 8, 1, 3, x, y);
  wide = 3;
  narrow = 1;
  MapGrid_CopyCellsTo(35, 8, 23, 8, narrow, wide);
  MapGrid_CopyCellsTo(99, 8, 87, 8, narrow, wide);
  x = 46;
  y = 55;
  MapGrid_CopyCellAttributes(57, 55, 3, 3, x, y);
  MapGrid_CopyCellsTo(57, 55, 46, 55, wide, wide);
  MapGrid_CopyCellsTo(121, 55, 110, 55, wide, wide);
}

/// Reveal shows the hidden paths.
void VenusLighthouse_RevealPaths(void) {
  s32 x;
  s32 y;
  s32 wide;
  s32 narrow;

  x = 23;
  y = 8;
  MapGrid_CopyCellAttributes(36, 8, 1, 3, x, y);
  wide = 3;
  narrow = 1;
  MapGrid_CopyCellsTo(36, 8, 23, 8, narrow, wide);
  MapGrid_CopyCellsTo(100, 8, 87, 8, narrow, wide);
  x = 46;
  y = 55;
  MapGrid_CopyCellAttributes(53, 55, 3, 3, x, y);
  MapGrid_CopyCellsTo(53, 55, 46, 55, wide, wide);
  MapGrid_CopyCellsTo(117, 55, 110, 55, wide, wide);
}

#define Actor_SetSpriteColor                                                   \
  ((void (*)(struct Actor *, s32))Actor_SetSpriteColor)

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Does nothing: the end of Felix's Move, which leaves the rest to the event.
s32 VenusLighthouse_Idle(void) { return 0; }

/// Felix casts Move on the statue.
void VenusLighthouse_FelixMovesStatue(void) {
  FieldPsynergy_LoadResources(ABILITY_MOVE, 1);
  FieldPsynergy_SetupActorPair(ACTOR_FELIX, ACTOR_FLOOR_SWITCH_STATUE);
  ((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->finishCallback =
      (void (*)(void))VenusLighthouse_Idle;
  FieldPsynergy_StartCast();
  FieldEvent_Wait(10);
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
}

// The Japanese form; from patch 1, the form below also saves this scene and
// VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_PILLARS as where the game restarts.
#if !TLA_PATCH_1
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineState.entrance == VENUS_LIGHTHOUSE_FROM_TITLE) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    VenusLighthouse_FelixLeaves();
    VenusLighthouse_DoubtLeaving();
    Event_End();
  } else {
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FLOOR_SWITCH_STATUE),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 1),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 2),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 3),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 4),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 5),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 6),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 7),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 8),
                              SPRITE_COLOR_GREY);
  }
  return 0;
}
#endif

void VenusLighthouse_DoubtLeaving(void);

// The Japanese edition has no form of this function here.
#if TLA_PATCH_1
s32 Scene_Initialize(void) {
  struct MainRoutineState *state;

  state = &gMainRoutineState;
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (state->entrance == VENUS_LIGHTHOUSE_FROM_TITLE) {
    state->restartScene = SCENE_VENUS_LIGHTHOUSE;
    state->restartEntrance = VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_PILLARS;
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    VenusLighthouse_FelixLeaves();
    VenusLighthouse_DoubtLeaving();
    Event_End();
  } else {
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FLOOR_SWITCH_STATUE),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 1),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 2),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 3),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 4),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 5),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 6),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 7),
                              SPRITE_COLOR_GREY);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_FIRST_GREY_STATUE + 8),
                              SPRITE_COLOR_GREY);
  }
  return 0;
}
#endif

s32 Scene_PrepareMap(void) { return 0; }

/// Has the actor go down the stairs (Exit_PrepareActorAnimation).
static inline void VenusLighthouse_GoDownStairs(s32 actor, s32 side) {
  Exit_PrepareActorAnimation(actor, side);
}

/// The prologue's opening: Felix moves the statue aside with Psynergy and
/// climbs off alone, leaving Jenna and Kraden.
void VenusLighthouse_FelixLeaves(void) {
  struct FieldState *state;

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
  state = Ram_HeapSlots->fieldState;
  state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  state->transitionFrames = 60;
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
  Exit_PrepareActorAnimation(ACTOR_FELIX, -1);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_SetPosition(ACTOR_FELIX, 0, 0);
}

/// Jenna doubts they should have left Felix; Alex appears in sparks and asks
/// after him, and the three go down the stairs to leave the lighthouse.
void VenusLighthouse_DoubtLeaving(void) {
  struct Actor *actor;
  struct FieldState *state;

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
  Actor_AppearInSparks(ACTOR_ALEX);
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
    Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetSpeed(ACTOR_ALEX, 1.2);
  Actor_SetAnimation(ACTOR_ALEX, ANIM_WALK);
  actor = (struct Actor *)Actor_Get(ACTOR_JENNA);
  if (actor != NULL)
    Actor_SetDestination(ACTOR_ALEX, actor->x.part[0].integer,
                         actor->z.part[0].integer);
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
  VenusLighthouse_GoDownStairs(ACTOR_JENNA, -1);
  FieldEvent_Wait(40);
  // Behind them the floor switch springs back up and the statue stands where
  // it was.
  MapGrid_CopyCellsTo(53, 50, 42, 49, 1, 1);
  MapGrid_CopyCellsTo(55, 117, 41, 117, 3, 5);
  Event_ClearFlag(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH);
  Actor_SetPosition(ACTOR_FLOOR_SWITCH_STATUE, 664, 792);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  gMainRoutineState.leaderActorId = ACTOR_JENNA;
  Party_RemoveMember(FELIX);
  FieldEvent_Wait(50);
  state = Ram_HeapSlots->fieldState;
  state->transitionFrames = 60;
  state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_PILLARS);
}

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

#include "shared/push_leader_steps.c"
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The leader's actor script as the statue throws him back
/// (VenusLighthouse_RepelLeader). He swells and shrinks ten times, flashing
/// (VenusLighthouse_PulseChildEffect).
s32 gVenusLeaderDazedScript[] = {
    SC_ANIMATION(12),
    SC_SET(SW_HOOK, (s32)VenusLighthouse_PulseChildEffect),
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, 11 * FIXED_ONE / 10),
    SC_SET(SW_SCALE_Y, 11 * FIXED_ONE / 10),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * FIXED_ONE / 10),
    SC_SET(SW_SCALE_Y, 9 * FIXED_ONE / 10),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_END,
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
    SCENE_EXIT(VENUS_LIGHTHOUSE_TO_VENUS_LIGHTHOUSE_ENTRANCE,
               VenusLighthouseEntrance,
               VENUS_LIGHTHOUSE_ENTRANCE_FROM_VENUS_LIGHTHOUSE),
    SCENE_EXIT(2, VenusLighthouse, 3),
    SCENE_EXIT(3, VenusLighthouse, 2),
    SCENE_EXIT(4, VenusLighthouse, 31),
    SCENE_EXIT(5, VenusLighthouse, 30),
    SCENE_EXIT(6, VenusLighthouse, 7),
    SCENE_EXIT(7, VenusLighthouse, 6),
    SCENE_EXIT(8, VenusLighthouse, 32),
    SCENE_EXIT(9, VenusLighthouse, VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_PILLARS),
    SCENE_EXIT(10, VenusLighthouse, 12),
    SCENE_EXIT(11, VenusLighthouse, 13),
    SCENE_EXIT(12, VenusLighthouse, 10),
    SCENE_EXIT(13, VenusLighthouse, 11),
    SCENE_EXIT(14, VenusLighthouse, 24),
    SCENE_EXIT(20, VenusLighthouse, 26),
    SCENE_EXIT(21, VenusLighthouse, 25),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_PILLARS, VenusLighthouse, 9),
    SCENE_EXIT(24, VenusLighthouse, 14),
    SCENE_EXIT(25, VenusLighthouse, 21),
    SCENE_EXIT(26, VenusLighthouse, 20),
    SCENE_EXIT(30, VenusLighthouse, 5),
    SCENE_EXIT(31, VenusLighthouse, 4),
    SCENE_EXIT(32, VenusLighthouse, 8),
    SCENE_EXITS_END,
};

/// The statues' actor script. They are drawn grey (VENUS_COLOR_MARK), and
/// never pushed (SW_VENUS_IMMOVABLE).
s32 gVenusGreyStatueScript[] = {
    SC_SET(SW_TIMER, VENUS_COLOR_MARK(SPRITE_COLOR_GREY)),
    SC_SET(SW_VENUS_IMMOVABLE, 1),
    SC_CALL(VenusLighthouse_UpdateChildFromTimer),
    SC_END,
};

/// The actor script of the World Map's picture of Venus Lighthouse, placed
/// at three fifths of its size, with no collision.
s32 gVenusMiniatureLighthouseScript[] = {
    SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 5),
    SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 5),
    SC_SET(SW_COLLISION, 0),
    SC_CALL(VenusLighthouse_ClearScriptedSpriteFlags),
    SC_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0,
                    8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 664, 0, 792, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 8, 0, 12, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_GLASSES, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 8, 0, 12, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_EAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    872, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    840, 0, 104, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_ALWAYS,
                    (s32)gVenusMiniatureLighthouseScript, 488, 0, 536,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 72, 0, 104, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 72, 0, 168, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 168, 0, 104, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 168, 0, 168, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 72, 0, 344, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 104, 0, 344, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 136, 0, 344, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 168, 0, 344, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 200, 0, 344, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gVenusLighthouseOpeningActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0,
                    8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
                    (s32)gVenusGreyStatueScript, 664, 0, 792, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
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
    {TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_BARRIER,
     FLAG_VENUS_LIGHTHOUSE_SWITCH_HELD, EVENT(VenusLighthouse_RepelLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOOR_SWITCH,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_StepOnFloorSwitch)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 1,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 2,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 3,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 4,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_TRUE_HEART_RELIEF,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadTrueHeart)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_RevealPaths)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(VenusLighthouse_HidePaths)},
    {TRIGGER_TABLE_END},
};
#undef FieldActor_SetSpriteColor
#undef Actor_SetSpriteFlags
