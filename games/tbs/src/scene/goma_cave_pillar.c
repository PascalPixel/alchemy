/// @file
/// Goma Cave (SceneId_GomaCavePillar), the room of two pillars and a stump,
/// in the cave in the Goma Range that the mayor of Vault tells the party to
/// pass through on the way to Bilibin.
///
/// The party comes in from the cave's first room (goma_cave_stream.c), and
/// the room's puzzle is Move on three things.
///
/// - The west pillar slides between two places a step apart, and the map
///   grid cells along its track follow it (FLAG_GOMA_CAVE_WEST_PILLAR_MOVED
///   says which).
/// - The stump is pushed a step at a time. Once it reaches the edge at column
///   25 it teeters, shedding dust, tips over and falls into the room below
///   (goma_cave_pond.c) with a crash that shakes the screen. The cells at the
///   edge change (FLAG_GOMA_CAVE_STUMP_FELL).
/// - The east pillar can also be pushed by walking into it. When it reaches
///   cell (38, 14) it sinks two pixels into the floor with a burst of
///   fragments and stays there (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE).
///
/// Coming back in on the east side, from the first room or from the room
/// below (GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST,
/// GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST), the party finds the east pillar
/// set into the floor underfoot, with the cells along it changed.
///
/// All three are area temp flags, so the room is as it started again once the
/// party leaves Goma Cave's four scenes.
///
/// Walking west into a gap (cell behavior 20) jumps the leader across it, and
/// a chest holds a Lucky Medal.
///
/// Forge, a Mars Djinni, waits here beside a man who once fought it: `Did you
/// see something strange over there? I fought it, but it fled.` Talking to
/// Forge makes him jump in surprise, and the Djinni's battle begins.
///
/// After a win the room reloads with the Djinni's event still pending, so the
/// man fidgets on a timer while Forge joins the party. Once it has, the timer
/// raises RAISED_FORGE_JOINED, and the man is amazed and walks away for good
/// (FLAG_GOMA_CAVE_WITNESS_LEFT).
///
/// After a loss or an escape Forge hides until the room is entered again
/// (FLAG_DJINNI_HIDING(DJINNI_FORGE)), and the man says `Hmmm... It must be
/// hiding somewhere again...`.
///
/// Two ways lead back to the first room
/// (GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST and
/// GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST) and three down to the room
/// below (GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST,
/// GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH and
/// GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST).
///
/// ENTRANCE_STUMP_FALL, which no exit names and only the debug scene selector
/// reaches, plays the stump being pushed to the edge and falling.
#include "script_operands.h"
#include "version.h"
#include "heap_slots.h"
#include "module/import.h"
#include "module/scene/goma_cave_pillar.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "sprite.h"
#include "event.h"
#include "scene_tables.h"
#include "djinn_ids.h"
#include "pushed_actor.h"
#include "tbs_edition.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "fixed_point_position.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "fixed_math.h"

#include "flags.h"
#include "sounds.h"

// Map-section temp flags: Scene_Arrive clears FLAG_AREA_TEMP_FIRST to
// FLAG_AREA_TEMP_LAST when the party enters another area, and other areas use
// the numbers for their own.
/// The stump has been pushed over into the room below.
#define FLAG_GOMA_CAVE_STUMP_FELL FLAG_AREA_TEMP_00
/// The east stone pillar has sunk into its place.
#define FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE FLAG_AREA_TEMP_01
/// The west stone pillar stands one step east of its start.
#define FLAG_GOMA_CAVE_WEST_PILLAR_MOVED FLAG_AREA_TEMP_02

enum GomaCavePillarFragmentPlane { PLANE_XY = 0, PLANE_XZ = 1, PLANE_YZ = 2 };

/// The room's actors, numbered in the order of
/// gGomaCavePillar_ActorPlacements.
enum GomaCavePillarActorId {
  /// Slides between two places.
  ACTOR_WEST_PILLAR = ACTOR_FIRST_PLACED,
  /// Pushed over the edge into the room below.
  ACTOR_STUMP,
  /// Sinks into the floor in its place.
  ACTOR_EAST_PILLAR,
  /// Hides the stump where it drops behind the floor in front of the ledge.
  ///
  /// It is a crate drawn solid black (SPRITE_COLOR_BLACK) and translucent at
  /// sprite priority 2, behind the floor, and ahead of the stump in the sprite
  /// list. So where it covers the stump the player sees the floor. Moved away
  /// in a render of the fall, the stump shows over the floor instead.
  ///
  /// It waits in the room's corner until the stump teeters at the edge, and
  /// goes back there once the stump has fallen.
  ACTOR_MASK_FOR_STUMP,
  /// Forge, a Mars Djinni, until it joins the party.
  ACTOR_FORGE,
  /// The man who fought Forge and lost it, a man with golden-brown hair in
  /// orange and red.
  ACTOR_WITNESS
};

/// The entrance no exit names, which plays the stump being pushed to
/// the edge and falling into the room below.
///
/// Only the debug scene selector reaches it.
enum { ENTRANCE_STUMP_FALL = 99 };

/// The trigger the man's timer raises once Forge has joined the party
/// (GomaCavePillar_FidgetWhileForgeFights).
enum GomaCavePillarRaisedTrigger { RAISED_FORGE_JOINED = 99 };

#include "shared/staged_actor.h"

/// Talking to Forge, a Mars Djinni, in Goma Cave: the man beside it jumps in
/// surprise, and the battle with the Djinni begins.
static void GomaCavePillar_FightForge(void) {
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_WITNESS, 2, 0);
  Field_RequestDjinnBattle(ACTOR_FORGE, DJINNI_FORGE);
}

/// Advances the fragment's selected coordinate plane and keeps its targets in
/// step.
static void GomaCavePillar_UpdateFragment(struct Actor *fragment) {
  // The actor's speed and acceleration slots carry the two selected axis
  // steps.
  s16 plane = (s16)fragment->userValue;

  switch (plane) {
  case PLANE_XY:
    fragment->x.fixed += fragment->speedLimit;
    fragment->targetX = fragment->x.fixed;
    fragment->y.fixed += fragment->acceleration;
    fragment->targetY = fragment->y.fixed;
    break;
  case PLANE_XZ:
    fragment->x.fixed += fragment->speedLimit;
    fragment->targetX = fragment->x.fixed;
    fragment->z.fixed += fragment->acceleration;
    fragment->targetZ = fragment->z.fixed;
    break;
  case PLANE_YZ:
    fragment->y.fixed += fragment->speedLimit;
    fragment->targetY = fragment->y.fixed;
    fragment->z.fixed += fragment->acceleration;
    fragment->targetZ = fragment->z.fixed;
    break;
  }
}

/// Applies child value 15 at the end of a fragment's lifetime.
static s32 GomaCavePillar_FinishFragment(struct Actor *actor) {
  Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
  return 0;
}

/// Creates a fragment with two axis rates, a script and packed plane/priority.
static void GomaCavePillar_SpawnFragment(s32 x, s32 y, s32 z, s32 firstRate,
                                         s32 secondRate,
                                         s32 scriptAndChildValue,
                                         s32 planeAndPriority) {
  struct EffectDurationScripts table;
  struct Actor *leader;
  struct Actor *fragment;
  struct Sprite *sprite;
  u32 priority;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  table = gGomaCavePillar_FragmentScripts;
  fragment = FieldActor_Create(SPRITE_DUST_PUFF, x, y, z);
  if (fragment == 0)
    return;
  sprite = fragment->sprite;
  Actor_SelectAnimation(fragment, (scriptAndChildValue + 1) & 15);
  Actor_StartScript(fragment, table.script[scriptAndChildValue & 15]);
  Actor_SetSpriteColor(fragment, ((u32)scriptAndChildValue >> 16) & 15);
  fragment->motionFlags = 0;
  sprite->flags = 0;
  fragment->update = GomaCavePillar_UpdateFragment;
  fragment->speedLimit = firstRate;
  fragment->acceleration = secondRate;
  fragment->userValue = (u16)planeAndPriority;
  priority = (u32)planeAndPriority >> 16;
  switch (priority) {
  case 0:
    sprite->image.priority = leader->sprite->image.priority;
    break;
  case 1:
  case 2:
  case 3:
    fragment->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->image.priority = priority;
    break;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gGomaCavePillar_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gGomaCavePillar_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gGomaCavePillar_ActorPlacements;
}

/// Copies the map grid cells for the west pillar on either side of its track.
static inline void GomaCavePillar_CopyWestPillarCells(s32 sourceColumn) {
  MapGrid_CopyCellAttributes(sourceColumn, 40, 6, 3, 18, 6);
}

/// Move on the west pillar in Goma Cave: it slides between its two places,
/// and the map grid cells follow it.
static void GomaCavePillar_MoveWestPillar(void) {
  s32 x;

  x = Actor_Get(ACTOR_WEST_PILLAR)->x.fixed;
  if (x < 0)
    x += px(16) - 1;
  x >>= 20;
  Event_Start();
  if (x == 20) {
    GomaCavePillar_CopyWestPillarCells(18);
    GameFlag_Clear(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
  } else {
    GomaCavePillar_CopyWestPillarCells(24);
    GameFlag_Set(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
  }
  Event_End();
}

/// While the stump teeters, dust falls from it: one fragment every eighth
/// frame, two on every sixteenth.
static void GomaCavePillar_SpawnStumpDust(void) {
  s32 framePhase;
  s32 fixedX;
  s32 fixedZ;
  struct Actor *heightSource;
  struct Actor *firstXSource, *firstZSource;
  s32 firstRandomX, firstRandomSpeed, firstStyle, firstSpeedBand,
      firstBandScaled, firstFallingSpeed;
  struct Actor *secondXSource, *secondZSource;
  s32 secondRandomX, secondRandomSpeed, secondStyle, secondSpeedBand,
      secondBandScaled, secondFallingSpeed;

  framePhase = gFrameCount & 7;
  if (framePhase == 0) {
    firstXSource = Actor_Get(ACTOR_STUMP);
    firstRandomX = Random_Next();
    fixedX = firstXSource->x.fixed;
    fixedX +=
        ((((u32)(((firstRandomX << 1) + firstRandomX) << 2)) >> 16) << 16);
    heightSource = Actor_GetById(ACTOR_STUMP);
    firstZSource = Actor_Get(ACTOR_STUMP);
    fixedZ = firstZSource->z.fixed;
    fixedZ += px(6);
    firstRandomSpeed = Random_Next();
    firstSpeedBand = ((u32)((firstRandomSpeed << 2) + firstRandomSpeed) >> 16);
    // These shifts produce speedBand * 6553: a tenth-pixel falling step.
    firstBandScaled =
        (((firstSpeedBand << 1) + firstSpeedBand) << 2) + firstSpeedBand;
    firstFallingSpeed = firstBandScaled << 6;
    firstFallingSpeed -= firstBandScaled;
    firstFallingSpeed <<= 3;
    firstFallingSpeed += firstSpeedBand;
    firstStyle = Random_Below(2);
    firstFallingSpeed = -firstFallingSpeed;
    GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
                                 firstFallingSpeed, firstStyle, framePhase);
    framePhase = gFrameCount & 15;
    if (framePhase == 0) {
      secondXSource = Actor_Get(ACTOR_STUMP);
      secondRandomX = Random_Next();
      fixedX = secondXSource->x.fixed;
      fixedX +=
          ((((u32)(((secondRandomX << 1) + secondRandomX) << 2)) >> 16) << 16);
      heightSource = Actor_GetById(ACTOR_STUMP);
      secondZSource = Actor_Get(ACTOR_STUMP);
      fixedZ = secondZSource->z.fixed;
      fixedZ += px(6);
      secondRandomSpeed = Random_Next();
      secondSpeedBand =
          ((u32)((secondRandomSpeed << 2) + secondRandomSpeed) >> 16);
      // Keep the same five falling speeds for the second fragment.
      secondBandScaled =
          (((secondSpeedBand << 1) + secondSpeedBand) << 2) + secondSpeedBand;
      secondFallingSpeed = secondBandScaled << 6;
      secondFallingSpeed -= secondBandScaled;
      secondFallingSpeed <<= 3;
      secondFallingSpeed += secondSpeedBand;
      secondStyle = Random_Below(2);
      secondFallingSpeed = -secondFallingSpeed;
      GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
                                   secondFallingSpeed, secondStyle, framePhase);
    }
  }
}

s32 gGomaCavePillar_WitnessFrames;

/// Move on the stump in Goma Cave, until it falls (FLAG_GOMA_CAVE_STUMP_FELL).
///
/// Once it is pushed to the edge (column 25), it teeters shedding dust, tips
/// over and falls into the room below with a crash that shakes the screen.
static void GomaCavePillar_PushStumpOver(void) {
  s32 column;
  void (*fragmentFunc)(void);
  s32 fixedX;
  s32 fixedZ;
  struct Actor *heightSource;
  struct Actor *stump;
  struct Actor *spriteActor;
  struct Actor *blendActor;
  struct Actor *bounceActor, *gravityActor;
  struct Actor *movedActor;
  struct Actor *zSource, *xSource;
  s32 randomX, randomSpeed, style, speedBand, bandScaled, fallingSpeed;
  struct Actor *overlayActor;

  stump = Actor_Get(ACTOR_STUMP);
  column = stump->x.fixed / px(16);
  Event_Start();
  if (column == 25) {
    overlayActor = Actor_Get(ACTOR_MASK_FOR_STUMP);
    overlayActor->layer = 1;
    spriteActor = Actor_Get(ACTOR_MASK_FOR_STUMP);
    Actor_SetSpriteFlags(spriteActor, 0);
    // XXX (no evidence): why the mask is drawn black and translucent rather
    // than in SPRITE_COLOR_HIDDEN, which would hide the stump the same way.
    Actor_SetSpriteColorById(ACTOR_MASK_FOR_STUMP, SPRITE_COLOR_BLACK);
    blendActor = Actor_Get(ACTOR_MASK_FOR_STUMP);
    Actor_SetBlendMode(blendActor, ACTOR_BLEND_TRANSLUCENT);
    Actor_SetPosition(ACTOR_MASK_FOR_STUMP, 414, 240);
    Event_Wait(10);
    // Dust falls while the stump rocks on the edge.
    fragmentFunc = GomaCavePillar_SpawnStumpDust;
    Task_AddFrame(fragmentFunc, TASK_PASS_FRAME);
    Sound_Play(SE_RUMBLE);
    Actor_SetDestinationOffset(ACTOR_STUMP, 1, 0);
    Actor_WaitForMoveById(ACTOR_STUMP);
    Event_Wait(10);
    Actor_SetDestinationOffset(ACTOR_STUMP, 2, 0);
    Actor_WaitForMoveById(ACTOR_STUMP);
    bounceActor = Actor_Get(ACTOR_STUMP);
    bounceActor->bounce = 0;
    gravityActor = Actor_Get(ACTOR_STUMP);
    gravityActor->gravity = px(3) / 5;
    Event_Wait(3);
    // It tips over the edge.
    Actor_SetSpeedAndAcceleration(ACTOR_STUMP, 2.5, 0.25);
    Sound_Play(SE_STOP_ACTION_SE);
    Actor_SetDestination(ACTOR_STUMP, 416, 200);
    movedActor = Actor_Get(ACTOR_STUMP);
    Actor_SetSpriteFlags(movedActor, 0);
    Task_Remove(fragmentFunc);
    Event_Wait(12);
    Sound_Play(SE_STONE_GRIND);
    xSource = Actor_Get(ACTOR_STUMP);
    randomX = Random_Next();
    fixedX = xSource->x.fixed;
    fixedX += ((((u32)(((randomX << 1) + randomX) << 2)) >> 16) << 16);
    heightSource = Actor_Get(ACTOR_STUMP);
    zSource = Actor_Get(ACTOR_STUMP);
    fixedZ = zSource->z.fixed;
    fixedZ += px(6);
    randomSpeed = Random_Next();
    speedBand = ((u32)((randomSpeed << 2) + randomSpeed) >> 16);
    // A last piece of dust, with the same tenth-pixel speed bands.
    bandScaled = (((speedBand << 1) + speedBand) << 2) + speedBand;
    fallingSpeed = bandScaled << 6;
    fallingSpeed -= bandScaled;
    fallingSpeed <<= 3;
    fallingSpeed += speedBand;
    style = Random_Below(2);
    fallingSpeed = -fallingSpeed;
    GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
                                 fallingSpeed, style, 0);
    Event_Wait(20);
    // It lands below; the shake dies down before the map grid cells change.
    Sound_Play(SE_CAST);
    MapGrid_SetShake(5, 5, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    MapGrid_WaitForShakeToSettle();
    Actor_SetPosition(ACTOR_STUMP, 0, 0);
    Actor_SetPosition(ACTOR_MASK_FOR_STUMP, 0, 0);
    GameFlag_Set(FLAG_GOMA_CAVE_STUMP_FELL);
    MapGrid_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  Event_End();
}

/// Once the east pillar reaches its place (cell 38, 14), it sinks two pixels
/// into the floor and six fragments fly out around it.
static void GomaCavePillar_SettleEastPillar(void) {
  s32 x;
  s32 z;

  x = Actor_Get(ACTOR_EAST_PILLAR)->x.fixed / px(16);
  z = Actor_Get(ACTOR_EAST_PILLAR)->z.fixed / px(16);
  if (x == 38 && z == 14) {
    Actor_Get(ACTOR_EAST_PILLAR)->y.fixed = -px(2);
    Actor_Get(ACTOR_EAST_PILLAR)->targetY =
        Actor_Get(ACTOR_EAST_PILLAR)->y.fixed;
    Sound_Play(SE_THUMP);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 px(1) / 2, 0, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 px(2) / 5, px(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 -px(2) / 5, px(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 -px(1) / 2, 0, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 px(2) / 5, -px(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_Get(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_Get(ACTOR_EAST_PILLAR)->z.fixed,
                                 -px(2) / 5, -px(2) / 5, 0, PLANE_XZ);
    GameFlag_Set(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE);
  }
}

/// Move on the east pillar in Goma Cave, until it is in its place
/// (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE).
static void GomaCavePillar_MoveEastPillar(void) {
  Event_Start();
  GomaCavePillar_SettleEastPillar();
  Event_End();
}

/// Walking into the east pillar in Goma Cave pushes it a step, until it is in
/// its place.
static void GomaCavePillar_PushEastPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  GomaCavePillar_SettleEastPillar();
  Event_End();
}

/// Walking west into the gap in Goma Cave (cell behavior 20): the leader
/// jumps across it.
///
/// Later patches use the shared forward jump.
static void GomaCavePillar_JumpGapWest(void) {
#if TBS_PATCH_1
  Field_TryJumpForward();
#else
  struct Actor *leader;
  struct FieldPosition target;
  struct FieldPosition *destination;
  u8 *motionFlags;
  u8 savedMotionFlags;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;
  target.x = Actor_Get(ACTOR_PARTY_LEADER)->x.fixed - px(32);
  /* LEFTOVER(template): destination holds &target, the landing reached
   * through a pointer; Camelot's template: the same leap body in the sibling
   * leap routines MogallForest_TryJumpLeaderTo, LogRoomsA_TryJumpTo,
   * TunnelRuins_HopLeaderTo and shared/kolima_jump.h takes its landing as a
   * struct FieldPosition *; as target.y and &target, 51 of 79 instructions
   * differ in tbs-ja. */
  destination = &target;
  destination->y = Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
  destination->z = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed;
  if (FieldActor_CheckMovementCollision(leader, destination) == 0) {
    Event_Start();
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = px(3);
    leader->acceleration = px(2);
    leader->velocityY = px(4);
    *motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(leader, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, (s16)((u32)destination->x >> 16),
                        (s16)((u32)destination->z >> 16));
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    *motionFlags = savedMotionFlags;
    Event_End();
  }
#endif
}

const struct Trigger *Scene_GetTriggers(void) {
  return gGomaCavePillar_Triggers;
}

/// Back from a won battle with Forge, the man reacts on a timer while the
/// Djinni joins the party.
///
/// Once the Djinni's event is over, it raises RAISED_FORGE_JOINED.
static void GomaCavePillar_FidgetWhileForgeFights(void) {
  struct FieldWork *work;
  s32 frame;

  work = gFieldWork;
  frame = ++gGomaCavePillar_WitnessFrames;
  switch (frame) {
  case 60:
    Actor_FaceDirection(ACTOR_WITNESS, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(ACTOR_WITNESS, EMOTE_WINCE, 0);
    break;
  case 180:
    Actor_StartRepeatedMotion(ACTOR_WITNESS, 3);
    break;
  case 240:
  case 270:
    Actor_Jump(ACTOR_WITNESS, 4, 0);
    break;
  case 480:
    Actor_SetAnimation(ACTOR_WITNESS, ANIM_SHAKE_HEAD);
    break;
  }
  if (gMainRoutineWork.pendingDjinni == 0)
    work->raisedTrigger = RAISED_FORGE_JOINED;
}

/// Raised in Goma Cave once Forge has joined the party: the man who fought it
/// is amazed.
///
/// He says `You got it? Wow, that's pretty impressive!`, then walks off for
/// good.
static void GomaCavePillar_WitnessCheersForgeCaught(void) {
  Task_Remove(GomaCavePillar_FidgetWhileForgeFights);
  Event_Start();
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_RunRepeatedMotion(ACTOR_WITNESS, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Event_SetMessageId(MsgGomaGotWowThatsPrettyImpressive);
  // "You got it? Wow, that's pretty impressive! Keep your eyes..."
  Event_ShowMessage(ACTOR_WITNESS, 0);
  Actor_SetAnimationAndWait(ACTOR_WITNESS, ANIM_NOD);
  Event_Wait(30);
  Actor_Get(ACTOR_EAST_PILLAR)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpeed(ACTOR_WITNESS, 2);
  Actor_WalkToAndWait(ACTOR_WITNESS, 600, 216);
  Actor_WalkToAndWait(ACTOR_WITNESS, 600, 248);
  Actor_WalkToAndWait(ACTOR_WITNESS, 568, 296);
  Actor_SetPosition(ACTOR_WITNESS, 0, 0);
  Actor_Get(ACTOR_EAST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  GameFlag_Set(FLAG_GOMA_CAVE_WITNESS_LEFT);
  Event_End();
}

/// Sets up the room as the party enters: the pillars and the stump where the
/// party left them.
///
/// ENTRANCE_STUMP_FALL plays the stump's fall. After a won battle with Forge,
/// the man's reaction starts.
s32 Scene_Initialize(void) {
  struct FieldWork *work;

  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  work->transitionFrames = 24;
  Actor_Get(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
  if (GameFlag_Get(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED) != 0) {
    Actor_SetPosition(ACTOR_WEST_PILLAR, 344, 104);
    GomaCavePillar_CopyWestPillarCells(24);
  } else {
    GomaCavePillar_CopyWestPillarCells(18);
  }
  // The cells GomaCavePillar_PushStumpOver copies as it sets the flag, redrawn
  // on arrival.
  if (GameFlag_Get(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
    Actor_SetPosition(ACTOR_STUMP, 0, 0);
    MapGrid_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE) != 0) {
    Actor_SetPosition(ACTOR_EAST_PILLAR, 616, 232);
    if (gMainRoutineWork.entrance ==
            GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST ||
        gMainRoutineWork.entrance ==
            GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST) {
      Actor_Get(ACTOR_EAST_PILLAR)->layer = 2;
      Actor_Get(ACTOR_EAST_PILLAR)->y.fixed--;
      Actor_Get(ACTOR_EAST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(36, 48, 5, 1, 36, 14);
    }
  }
  if (gMainRoutineWork.entrance == ENTRANCE_STUMP_FALL) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Actor_SetPosition(ACTOR_STUMP, 384, 192);
    Event_Wait(60);
    Actor_Get(ACTOR_STUMP)->layer = 2;
    Actor_MoveToAndWait(ACTOR_STUMP, 408, 192);
    Event_Wait(60);
    GomaCavePillar_PushStumpOver();
  }
  // Coming back from a Djinni battle: the man reacts until it is over.
  if (gMainRoutineWork.pendingDjinni != 0) {
    gGomaCavePillar_WitnessFrames = 0;
    Task_AddFrame(GomaCavePillar_FidgetWhileForgeFights, TASK_PASS_FRAME);
  }
  return 0;
}

#include "shared/pushed_actor_tables.h"
// clang-format off

/// The scripts a pillar fragment can run, chosen by the low bits of its spawn
/// value.
struct EffectDurationScripts gGomaCavePillar_FragmentScripts = {{
    gGomaCavePillar_FragmentScriptShort,
    gGomaCavePillar_FragmentScriptMedium,
    gGomaCavePillar_FragmentScriptLong,
}};

s32 gGomaCavePillar_FragmentScriptShort[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gGomaCavePillar_FragmentScriptMedium[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gGomaCavePillar_FragmentScriptLong[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_RELEASE,
};

struct SceneEntrance gGomaCavePillar_Entrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_WEST, CONDITION_ALWAYS, 88, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST, CONDITION_ALWAYS, 584, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST, CONDITION_ALWAYS, 232, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_SOUTH, CONDITION_ALWAYS, 536, 0, 408, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { ENTRANCE_STUMP_FALL, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { SCENE_TABLE_END },
};

u32 gGomaCavePillar_Exits[] = {
    SCENE_EXITS(GomaCavePillar),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST, GomaCaveStream, GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_WEST),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_WEST),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_SOUTH),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_EAST),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST, GomaCaveStream, GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_EAST),
    SCENE_EXITS_END,
};

struct ActorPlacement gGomaCavePillar_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 192, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FORGE), MOVEMENT_TYPE_STAND, 664, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_ORANGE, FLAG_GOMA_CAVE_WITNESS_LEFT, MOVEMENT_TYPE_STAND, 628, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gGomaCavePillar_Triggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT(GomaCavePillar_PushEastPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 20, CONDITION_ALWAYS, EVENT(GomaCavePillar_JumpGapWest) },
    { TRIGGER_TALK, ACTOR_FORGE, CONDITION_ALWAYS, EVENT(GomaCavePillar_FightForge) },
    { TRIGGER_TALK, ACTOR_WITNESS, FLAG_DJINNI_HIDING(DJINNI_FORGE), MSGNO(MsgVaultDidYouSeeSomethingStrangeOver) },
    { TRIGGER_TALK, ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultHmmmItMustBeHidingSomewhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, FLAG_DJINNI_HIDING(DJINNI_FORGE), MSGNO(MsgVaultItJustWhackedMeInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultIKnowitsKindItllBeBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR, CONDITION_ALWAYS, EVENT(GomaCavePillar_MoveWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_STUMP_FELL, EVENT(GomaCavePillar_PushStumpOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT(GomaCavePillar_MoveEastPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GOMA_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_RAISED, RAISED_FORGE_JOINED, CONDITION_ALWAYS, EVENT(GomaCavePillar_WitnessCheersForgeCaught) },
    { TRIGGER_TABLE_END },
};
// clang-format on
