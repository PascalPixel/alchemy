/// @file
/// Goma Cave (SceneId_GomaCavePillar), the room of two pillars and a stump,
/// in the cave in the Goma Range that the mayor of Vault tells the party to
/// pass through on the way to Bilibin.
///
/// The party comes in from the cave's first room (goma_cave_stream.c), and
/// the room's puzzle is Move on three things. The west pillar slides between
/// two places a step apart, and the map grid cells along its track follow it
/// (FLAG_GOMA_CAVE_WEST_PILLAR_MOVED says which). The stump is pushed a step
/// at a time; once it reaches the edge at column 25 it teeters, shedding
/// dust, tips over and falls into the room below (goma_cave_pond.c) with a
/// crash that shakes the screen, and the cells at the edge change
/// (FLAG_GOMA_CAVE_STUMP_FELL). The east pillar can also be pushed by walking
/// into it; when it reaches cell (38, 14) it sinks two pixels into the floor
/// with a burst of fragments and stays there
/// (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE). Coming back in on the east side, from
/// the first room or from the room below
/// (GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST,
/// GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST), the party finds it set into the
/// floor underfoot, with the cells along it changed. All three are area
/// temp flags, so the room is as it started again once the party leaves Goma
/// Cave's four scenes. Walking west into a gap (metatile behavior 20) jumps the
/// leader across it, and a chest holds a Lucky Medal.
///
/// Forge, a Mars Djinni, waits here beside a man who once fought it: `Did you
/// see something strange over there? I fought it, but it fled.` Talking to
/// Forge makes him jump in surprise, and the Djinni's battle begins. After a
/// win the room reloads with the Djinni's event still pending, so the man
/// fidgets on a timer while Forge joins the party; once it has, the timer
/// raises RAISED_FORGE_JOINED, and the man is amazed and walks away for good
/// (FLAG_GOMA_CAVE_WITNESS_LEFT). After a loss or an escape Forge hides until
/// the room is entered again (FLAG_DJINNI_HIDING(DJINNI_FORGE)), and the man
/// says `Hmmm... It must be hiding somewhere again...`.
///
/// Two ways lead back to the first room
/// (GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST and
/// GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST) and three down to the room
/// below (GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST,
/// GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH and
/// GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST).
/// ENTRANCE_STUMP_FALL, which no exit names and only the debug scene selector
/// reaches, plays the stump being pushed to the edge and falling.
#include "script_operands.h"
#include "version.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/scene/goma_cave_pillar.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "map_grid_copy.h"
#include "types.h"
#include "animspr.h"
#include "event_layer.h"
#include "scene_tables.h"
#include "djinn_ids.h"
#include "pushed_actor.h"
#include "edition.h"
#include "tbs_edition.h"
#include "actor.h"
#include "common.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "fixed_point_position.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "fixed_math.h"

#include "flags.h"
#include "songs.h"

// Map-section temp flags: Scene_Arrive clears 0x300 to 0x3ff when
// the party enters another area, and other areas use the numbers
// for their own.
/// The stump has been pushed over into the room below.
/// ★ Named from what the player sees: a tree stump standing on a ledge (its
/// render)
#define FLAG_GOMA_CAVE_STUMP_FELL 0x300
/// The east stone pillar has sunk into its place.
/// ★ Named from what the player sees: a stone pillar on the room's east side
/// (its render)
#define FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE 0x301
/// The west stone pillar stands one step east of its start.
/// ★ Named from what the player sees: a stone pillar at the head of the room's
/// west passage (its render)
#define FLAG_GOMA_CAVE_WEST_PILLAR_MOVED 0x302

enum GomaCavePillarFragmentPlane { PLANE_XY = 0, PLANE_XZ = 1, PLANE_YZ = 2 };

/// The room's actors, numbered in the order of
/// gGomaCavePillar_ActorTemplates.
enum GomaCavePillarActorId {
  /// Slides between two places.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  ACTOR_WEST_PILLAR = ACTOR_FIRST_PLACED,
  /// Pushed over the edge into the room below.
  /// ★ Named from the game: "You want to move the stump, right?"
  /// (MsgGomaNoUsePsynergy)
  ACTOR_STUMP,
  /// Sinks into the floor in its place.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  ACTOR_EAST_PILLAR,
  /// Hides the stump where it drops behind the floor in front of the ledge: a
  /// crate drawn solid black (SPRITE_COLOR_BLACK) and translucent at sprite
  /// priority 2, behind the floor, and ahead of the stump in the sprite list,
  /// so where it covers the stump the player sees the floor. Moved away in a
  /// render of the fall, the stump shows over the floor instead. It waits in
  /// the room's corner until the stump teeters at the edge, and goes back
  /// there once the stump has fallen.
  ACTOR_MASK_FOR_STUMP,
  /// Forge, a Mars Djinni, until it joins the party.
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  ACTOR_FORGE,
  /// The man who fought Forge and lost it, a man with golden-brown hair in
  /// orange and red.
  /// ★ Named from the game: "Did you see something strange over there? I
  /// fought it, but it fled." (MsgVaultDidYouSeeSomethingStrangeOver)
  ACTOR_WITNESS
};

/// The entrance no exit names, which plays the stump being pushed to
/// the edge and falling into the room below; only the debug scene selector
/// reaches it.
enum { ENTRANCE_STUMP_FALL = 99 };

/// The trigger the man's timer raises once Forge has joined the party
/// (GomaCavePillar_FidgetWhileForgeFights).
enum GomaCavePillarRaisedTrigger { RAISED_FORGE_JOINED = 99 };

// Shared movement routines stay in their linked order.
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
static void GomaCavePillar_UpdateFragment(struct Actor *updated) {
  struct Actor *fragment = (struct Actor *)updated;
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
static s32 GomaCavePillar_FinishFragment(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
  return 0;
}

/// Creates a fragment with two axis rates, a script and packed plane/priority.
static void GomaCavePillar_SpawnFragment(s32 x, s32 y, s32 z, s32 firstRate,
                                         s32 secondRate,
                                         s32 scriptAndChildValue,
                                         s32 planeAndPriority) {
  // A struct copy of the named table, which sits in the middle of .data; a
  // local array of the three scripts compiles to the same code.
  struct EffectDurationScripts table;
  struct Actor *leader;
  struct Actor *fragment;
  struct Sprite *sprite;
  u32 priority;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
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

const struct ActorTemplate *Scene_GetActorTemplates(void) {
  return gGomaCavePillar_ActorTemplates;
}

/// Copies the map grid cells for the west pillar on either side of its track.
static inline void GomaCavePillar_CopyWestPillarCells(s32 sourceColumn) {
  MapGrid_CopyCellAttributes(sourceColumn, 40, 6, 3, 18, 6);
}

/// Move on the west pillar in Goma Cave: it slides between its two places,
/// and the map grid cells follow it.
static void GomaCavePillar_MoveWestPillar(void) {
  struct Actor *actor;
  s32 x;

  actor = Actor_GetById(ACTOR_WEST_PILLAR);
  x = actor->x.fixed;
  if (x < 0)
    x += 0xfffff;
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
    firstXSource = Actor_GetById(ACTOR_STUMP);
    firstRandomX = Import_RandomNext();
    fixedX = firstXSource->x.fixed;
    fixedX +=
        ((((u32)(((firstRandomX << 1) + firstRandomX) << 2)) >> 16) << 16);
    heightSource = Actor_GetById(ACTOR_STUMP);
    firstZSource = Actor_GetById(ACTOR_STUMP);
    fixedZ = firstZSource->z.fixed;
    fixedZ += PIXELS(6);
    firstRandomSpeed = Import_RandomNext();
    firstSpeedBand = ((u32)((firstRandomSpeed << 2) + firstRandomSpeed) >> 16);
    // These shifts produce speedBand * 6553: a tenth-pixel falling step.
    firstBandScaled =
        (((firstSpeedBand << 1) + firstSpeedBand) << 2) + firstSpeedBand;
    firstFallingSpeed = firstBandScaled << 6;
    firstFallingSpeed -= firstBandScaled;
    firstFallingSpeed <<= 3;
    firstFallingSpeed += firstSpeedBand;
    firstStyle = (u32)(Import_RandomNext() << 1) >> 16;
    firstFallingSpeed = -firstFallingSpeed;
    GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
                                 firstFallingSpeed, firstStyle, framePhase);
    framePhase = gFrameCount & 15;
    if (framePhase == 0) {
      secondXSource = Actor_GetById(ACTOR_STUMP);
      secondRandomX = Import_RandomNext();
      fixedX = secondXSource->x.fixed;
      fixedX +=
          ((((u32)(((secondRandomX << 1) + secondRandomX) << 2)) >> 16) << 16);
      heightSource = Actor_GetById(ACTOR_STUMP);
      secondZSource = Actor_GetById(ACTOR_STUMP);
      fixedZ = secondZSource->z.fixed;
      fixedZ += PIXELS(6);
      secondRandomSpeed = Import_RandomNext();
      secondSpeedBand =
          ((u32)((secondRandomSpeed << 2) + secondRandomSpeed) >> 16);
      // Keep the same five falling speeds for the second fragment.
      secondBandScaled =
          (((secondSpeedBand << 1) + secondSpeedBand) << 2) + secondSpeedBand;
      secondFallingSpeed = secondBandScaled << 6;
      secondFallingSpeed -= secondBandScaled;
      secondFallingSpeed <<= 3;
      secondFallingSpeed += secondSpeedBand;
      secondStyle = (u32)(Import_RandomNext() << 1) >> 16;
      secondFallingSpeed = -secondFallingSpeed;
      GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
                                   secondFallingSpeed, secondStyle, framePhase);
    }
  }
}

s32 gGomaCavePillar_WitnessFrames;

/// Move on the stump in Goma Cave, until it falls (FLAG_GOMA_CAVE_STUMP_FELL):
/// once it is pushed to the edge (column 25), it teeters shedding dust, tips
/// over and falls into the room below with a crash that shakes the screen.
static void GomaCavePillar_PushStumpOver(void) {
  s32 column;
  s32 zero;
  void (*fragmentCallback)(void);
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

  stump = Actor_GetById(ACTOR_STUMP);
  column = stump->x.fixed / PIXELS(16);
  Event_Start();
  if (column == 25) {
    overlayActor = Actor_GetById(ACTOR_MASK_FOR_STUMP);
    zero = 0;
    overlayActor->layer = 1;
    spriteActor = Actor_GetById(ACTOR_MASK_FOR_STUMP);
    Actor_SetSpriteFlags(spriteActor, 0);
    // XXX (no evidence): why the mask is drawn black and translucent rather
    // than in SPRITE_COLOR_HIDDEN, which would hide the stump the same way.
    Actor_ConfigureSpriteColor(ACTOR_MASK_FOR_STUMP, SPRITE_COLOR_BLACK);
    blendActor = Actor_GetById(ACTOR_MASK_FOR_STUMP);
    Actor_SetBlendMode(blendActor, ACTOR_BLEND_TRANSLUCENT);
    Import_ActorSetPosition(ACTOR_MASK_FOR_STUMP, PIXELS(414), PIXELS(240));
    Event_WaitFrames(10);
    // Dust falls while the stump rocks on the edge.
    fragmentCallback = GomaCavePillar_SpawnStumpDust;
    Task_AddFrame(fragmentCallback, TASK_PASS_FRAME);
    Sound_PlaySong(SE_RUMBLE);
    Actor_OffsetPositionAndResetMotion(ACTOR_STUMP, 1, 0);
    Actor_WaitForMoveById(ACTOR_STUMP);
    Event_WaitFrames(10);
    Actor_OffsetPositionAndResetMotion(ACTOR_STUMP, 2, 0);
    Actor_WaitForMoveById(ACTOR_STUMP);
    bounceActor = Actor_GetById(ACTOR_STUMP);
    bounceActor->bounce = zero;
    gravityActor = Actor_GetById(ACTOR_STUMP);
    gravityActor->gravity = PIXELS(3) / 5;
    Event_WaitFrames(3);
    // It tips over the edge.
    Actor_SetSpeedParameters(ACTOR_STUMP, 5 * PIXELS(1) / 2, PIXELS(1) / 4);
    Sound_PlaySong(SE_STOP_ACTION_SE);
    Actor_ResetAndSetPosition(ACTOR_STUMP, 416, 200);
    movedActor = Actor_GetById(ACTOR_STUMP);
    Actor_SetSpriteFlags(movedActor, 0);
    Task_Remove(fragmentCallback);
    Event_WaitFrames(12);
    Sound_PlaySong(SE_STONE_GRIND);
    xSource = Actor_GetById(ACTOR_STUMP);
    randomX = Import_RandomNext();
    fixedX = xSource->x.fixed;
    fixedX += ((((u32)(((randomX << 1) + randomX) << 2)) >> 16) << 16);
    heightSource = Actor_GetById(ACTOR_STUMP);
    zSource = Actor_GetById(ACTOR_STUMP);
    fixedZ = zSource->z.fixed;
    fixedZ += PIXELS(6);
    randomSpeed = Import_RandomNext();
    speedBand = ((u32)((randomSpeed << 2) + randomSpeed) >> 16);
    // A last piece of dust, with the same tenth-pixel speed bands.
    bandScaled = (((speedBand << 1) + speedBand) << 2) + speedBand;
    fallingSpeed = bandScaled << 6;
    fallingSpeed -= bandScaled;
    fallingSpeed <<= 3;
    fallingSpeed += speedBand;
    style = (u32)(Import_RandomNext() << 1) >> 16;
    fallingSpeed = -fallingSpeed;
    GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, zero,
                                 fallingSpeed, style, zero);
    Event_WaitFrames(20);
    // It lands below; the shake dies down before the map grid cells change.
    Sound_PlaySong(SE_CAST);
    MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    MapGrid_WaitForShakeToSettle();
    Import_ActorSetPosition(ACTOR_STUMP, 0, 0);
    Import_ActorSetPosition(ACTOR_MASK_FOR_STUMP, 0, 0);
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

  x = Actor_GetById(ACTOR_EAST_PILLAR)->x.fixed / PIXELS(16);
  z = Actor_GetById(ACTOR_EAST_PILLAR)->z.fixed / PIXELS(16);
  if (x == 38 && z == 14) {
    Actor_GetById(ACTOR_EAST_PILLAR)->y.fixed = -PIXELS(2);
    Actor_GetById(ACTOR_EAST_PILLAR)->targetY =
        Actor_GetById(ACTOR_EAST_PILLAR)->y.fixed;
    Sound_PlaySong(SE_THUMP);
    GomaCavePillar_SpawnFragment(Actor_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 PIXELS(1) / 2, 0, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 PIXELS(2) / 5, PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 -PIXELS(2) / 5, PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 -PIXELS(1) / 2, 0, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 PIXELS(2) / 5, -PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Actor_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Actor_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 -PIXELS(2) / 5, -PIXELS(2) / 5, 0, PLANE_XZ);
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

/// Walking west into the gap in Goma Cave (metatile behavior 20): the leader
/// jumps across it. Later patches use the shared forward jump.
static void GomaCavePillar_JumpGapWest(void) {
#if TBS_PATCH_1
  Field_TryJumpForward();
#else
  struct Actor *leader;
  struct FieldPosition target;
  struct FieldPosition *destination;
  u8 *motionFlags;
  u8 savedMotionFlags;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;
  target.x = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed - PIXELS(32);
  destination = &target;
  destination->y = Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  destination->z = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed;
  if (FieldActor_CheckMovementCollision(leader, destination) == 0) {
    Event_Start();
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *motionFlags &= 0x7e;
    Actor_SetSpriteFlags(leader, 0);
    Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER,
                               *(s16 *)((u8 *)destination + 2),
                               *(s16 *)((u8 *)destination + 10));
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(leader, 1);
    *motionFlags = savedMotionFlags;
    Event_End();
  }
#endif
}

const struct Trigger *Scene_GetTriggers(void) {
  return gGomaCavePillar_Triggers;
}

/// Back from a won battle with Forge, the man reacts on a timer while the
/// Djinni joins the party; once the Djinni's event is over, it raises
/// RAISED_FORGE_JOINED.
static void GomaCavePillar_FidgetWhileForgeFights(void) {
  struct FieldWork *work;
  s32 frame;

  work = gFieldWork;
  frame = ++gGomaCavePillar_WitnessFrames;
  switch (frame) {
  case 60:
    Actor_FaceDirection(ACTOR_WITNESS, FACING_SOUTHEAST, 0);
    FieldEffect_SpawnAttached(ACTOR_WITNESS, EMOTE_WINCE, 0);
    break;
  case 180:
    Actor_StartRepeatedMotion(ACTOR_WITNESS, 3);
    break;
  case 240:
  case 270:
    Actor_Jump(ACTOR_WITNESS, 4, 0);
    break;
  case 480:
    Import_ActorSetAnimation(ACTOR_WITNESS, ANIM_SHAKE_HEAD);
    break;
  }
  if (gCommon.pendingDjinni == 0)
    work->raisedTrigger = RAISED_FORGE_JOINED;
}

/// Raised in Goma Cave once Forge has joined the party: the man who fought it
/// is amazed, `You got it? Wow, that's pretty impressive!`, then walks off for
/// good.
static void GomaCavePillar_WitnessCheersForgeCaught(void) {
  Task_Remove(GomaCavePillar_FidgetWhileForgeFights);
  Event_Start();
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_RunRepeatedMotion(ACTOR_WITNESS, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Event_SetMessageId(MsgGomaGotWowThatsPrettyImpressive);
  Event_ShowMessage(ACTOR_WITNESS, 0);
  Actor_SetAnimationAndWait(ACTOR_WITNESS, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_GetById(ACTOR_EAST_PILLAR)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpeed(ACTOR_WITNESS, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_WITNESS, 600, 216);
  Actor_WalkToAndWait(ACTOR_WITNESS, 600, 248);
  Actor_WalkToAndWait(ACTOR_WITNESS, 568, 296);
  Actor_SetPosition(ACTOR_WITNESS, PIXELS(0), PIXELS(0));
  Actor_GetById(ACTOR_EAST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  GameFlag_Set(FLAG_GOMA_CAVE_WITNESS_LEFT);
  Event_End();
}

/// Sets up the room as the party enters: the pillars and the stump where the
/// party left them. ENTRANCE_STUMP_FALL plays the stump's fall; after a won
/// battle with Forge, the man's reaction starts.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  struct FieldWork *work;

  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  work->transitionFrames = 24;
  Actor_GetById(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
  if (GameFlag_Get(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED) != 0) {
    Actor_SetPosition(ACTOR_WEST_PILLAR, PIXELS(344), PIXELS(104));
    GomaCavePillar_CopyWestPillarCells(24);
  } else {
    GomaCavePillar_CopyWestPillarCells(18);
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
    Actor_SetPosition(ACTOR_STUMP, PIXELS(0), PIXELS(0));
    MapGrid_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE) != 0) {
    Actor_SetPosition(ACTOR_EAST_PILLAR, PIXELS(616), PIXELS(232));
    if (gCommon.entrance == GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST ||
        gCommon.entrance == GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST) {
      Actor_GetById(ACTOR_EAST_PILLAR)->layer = 2;
      actor = Actor_GetById(ACTOR_EAST_PILLAR);
      actor->y.fixed--;
      Actor_GetById(ACTOR_EAST_PILLAR)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(36, 48, 5, 1, 36, 14);
    }
  }
  if (gCommon.entrance == ENTRANCE_STUMP_FALL) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Actor_SetPosition(ACTOR_STUMP, PIXELS(384), PIXELS(192));
    Event_WaitFrames(60);
    Actor_GetById(ACTOR_STUMP)->layer = 2;
    Actor_MoveToAndWait(ACTOR_STUMP, 408, 192);
    Event_WaitFrames(60);
    GomaCavePillar_PushStumpOver();
  }
  // Coming back from a Djinni battle: the man reacts until it is over.
  if (gCommon.pendingDjinni != 0) {
    gGomaCavePillar_WitnessFrames = 0;
    Task_AddFrame(GomaCavePillar_FidgetWhileForgeFights, TASK_PASS_FRAME);
  }
  return 0;
}

/// One step per facing: X pixels in the high halfword, Z pixels in the low
/// half.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

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

struct ActorTemplate gGomaCavePillar_ActorTemplates[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(384), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(584), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {MARS_DJINNI, FLAG_DJINNI(DJINNI_FORGE), MOVEMENT_TYPE_STAND, PIXELS(664), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_MAN_IN_ORANGE, FLAG_GOMA_CAVE_WITNESS_LEFT, MOVEMENT_TYPE_STAND, PIXELS(628), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0},
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
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, 0xf4f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_RAISED, RAISED_FORGE_JOINED, CONDITION_ALWAYS, EVENT(GomaCavePillar_WitnessCheersForgeCaught) },
    { TRIGGERS_END },
};
// clang-format on
