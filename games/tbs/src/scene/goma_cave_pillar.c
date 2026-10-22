/// @file
/// Goma Cave (SceneId_GomaCavePillar), the room of two pillars and a stump,
/// in the cave in the Goma Range that the mayor of Vault tells the party to
/// pass through on the way to Bilibin.
///
/// The party comes in from the cave's first room (goma_cave_waterway.c), and
/// the room's puzzle is Move on three things. The west pillar slides between
/// two places a step apart, and the map cells along its track follow it
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
/// floor underfoot, with the cells along it changed. All three are place flags,
/// so the room is as it started again once the party leaves Goma Cave's four
/// scenes. Walking west into a gap (collision code 20) jumps the leader across
/// it, and a chest holds a Lucky Medal.
///
/// Forge, a Mars Djinni, waits here beside a man who once fought it: `Did you
/// see something strange over there? I fought it, but it fled.` Talking to
/// Forge makes him jump in surprise, and the Djinni's battle begins. After a
/// win the room reloads with the Djinni's event still pending, so the man
/// fidgets on a timer while Forge joins the party; once it has, the timer
/// raises TRIGGER_FORGE_JOINED, and the man is amazed and walks away for good
/// (FLAG_GOMA_CAVE_WITNESS_LEFT). After a loss or an escape Forge hides until
/// the room is entered again (FLAG_DJINNI_HIDING_0 + DJINNI_FORGE), and the man
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
#include "script.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/goma_cave_pillar.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "mapcopy.h"
#include "types.h"
#include "animspr.h"
#include "field_event.h"
#include "field_scene.h"
#include "djinn_ids.h"
#include "staged_actor.h"
#include "b5_context.h"
#include "edition.h"
#include "tbs_edition.h"
#include "field_obj.h"
#include "game_state.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "fixed_point_position.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"

#include "flags.h"
#include "songs.h"
#include "event.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.
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

/// The room's actors, numbered in the order of gGomaCavePillar_Placements.
enum GomaCavePillarActor {
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
  /// The stump's shadow while it teeters at the edge: a crate's outline drawn
  /// solid black (SPRITE_COLOR_BLACK) and translucent, kept in the room's
  /// corner until then and put back there once the stump has fallen.
  /// ★ Named from what the player sees: SPRITE_CRATE drawn as a
  /// translucent black shape under the stump, spr_223.png (its render)
  ACTOR_STUMP_SHADOW,
  /// Forge, a Mars Djinni, until it joins the party.
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  ACTOR_FORGE,
  /// The man who fought Forge and lost it, a man with golden-brown hair in
  /// orange and red.
  /// ★ Named from the game: "Did you see something strange over there? I
  /// fought it, but it fled." (MsgVaultDidYouSeeSomethingStrangeOver)
  ACTOR_WITNESS
};

/// The entrance no exit names, which plays the stump being pushed to the edge
/// and falling into the room below; only the debug scene selector reaches it.
enum { ENTRANCE_STUMP_FALL = 99 };

/// The event the man's timer raises once Forge has joined the party
/// (GomaCavePillar_FidgetWhileForgeFights).
enum { TRIGGER_FORGE_JOINED = 99 };

// Shared movement routines stay in their linked order.
#include "common/object_staged_actor.h"

/// Talking to Forge, a Mars Djinni, in Goma Cave: the man beside it jumps in
/// surprise, and the battle with the Djinni begins.
static void Event_FightForge(void) {
  OBJ_EMOTE(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorJump(ACTOR_WITNESS, 2, 0);
  Field_RequestDjinnBattle(ACTOR_FORGE, DJINNI_FORGE);
}

/// Advances the fragment's selected coordinate plane and keeps its targets in
/// step.
static void GomaCavePillar_UpdateFragment(union FieldObject *fieldObject) {
  struct FieldActor *fragment = (struct FieldActor *)fieldObject;
  // The actor's speed and acceleration slots carry the two selected axis steps.
  s16 plane = (s16)fragment->work;

  switch (plane) {
  case PLANE_XY:
    fragment->x.fixed += fragment->speed;
    fragment->targetX = fragment->x.fixed;
    fragment->y.fixed += fragment->acceleration;
    fragment->targetY = fragment->y.fixed;
    break;
  case PLANE_XZ:
    fragment->x.fixed += fragment->speed;
    fragment->targetX = fragment->x.fixed;
    fragment->z.fixed += fragment->acceleration;
    fragment->targetZ = fragment->z.fixed;
    break;
  case PLANE_YZ:
    fragment->y.fixed += fragment->speed;
    fragment->targetY = fragment->y.fixed;
    fragment->z.fixed += fragment->acceleration;
    fragment->targetZ = fragment->z.fixed;
    break;
  }
}

/// Applies child value 15 at the end of a fragment's lifetime.
static s32
GomaCavePillar_FinishFragment(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, 15);
  return 0;
}

/// Creates a fragment with two axis rates, a script and packed plane/priority.
static void GomaCavePillar_SpawnFragment(s32 x, s32 y, s32 z, s32 firstRate,
                                         s32 secondRate,
                                         s32 scriptAndChildValue,
                                         s32 planeAndPriority) {
  struct EffectScripts table;
  struct FieldActor *leader;
  struct FieldActor *fragment;
  struct AnimationObject *sprite;
  u32 priority;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  table = gGomaCavePillar_FragmentScripts;
  fragment = Engine_ObjectCreate(SPRITE_DUST_PUFF, x, y, z);
  if (fragment == 0)
    return;
  sprite = fragment->sprite;
  Object_SetMode(fragment, (scriptAndChildValue + 1) & 15);
  Engine_ObjectSetScript(fragment, table.script[scriptAndChildValue & 15]);
  ObjectGroup_SetChildValue(fragment, ((u32)scriptAndChildValue >> 16) & 15);
  fragment->motionFlags = 0;
  sprite->flags = 0;
  fragment->update = GomaCavePillar_UpdateFragment;
  fragment->speed = firstRate;
  fragment->acceleration = secondRate;
  fragment->work = (u16)planeAndPriority;
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

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gGomaCavePillar_Exits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gGomaCavePillar_Placements;
}

/// Copies the map cells for the west pillar on either side of its track.
static inline void GomaCavePillar_CopyWestPillarCells(s32 sourceColumn) {
  Map_CopyCellAttributes(sourceColumn, 40, 6, 3, 18, 6);
}

/// Move on the west pillar in Goma Cave: it slides between its two places,
/// and the map cells follow it.
static void Event_MoveWestPillar(void) {
  struct FieldActor *actor;
  s32 x;

  actor = Object_GetById(ACTOR_WEST_PILLAR);
  x = actor->x.fixed;
  if (x < 0)
    x += 0xfffff;
  x >>= 20;
  EV_BEGIN();
  if (x == 20) {
    GomaCavePillar_CopyWestPillarCells(18);
    Engine_GameFlagClear(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
  } else {
    GomaCavePillar_CopyWestPillarCells(24);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
  }
  EV_END();
}

/// While the stump teeters, dust falls from it: one fragment every eighth
/// frame, two on every sixteenth.
static void GomaCavePillar_SpawnStumpDust(void) {
  s32 framePhase;
  s32 fixedX;
  s32 fixedZ;
  struct FieldActor *heightSource;
  struct FieldActor *firstXSource, *firstZSource;
  s32 firstRandomX, firstRandomSpeed, firstStyle, firstSpeedBand,
      firstBandScaled, firstFallingSpeed;
  struct FieldActor *secondXSource, *secondZSource;
  s32 secondRandomX, secondRandomSpeed, secondStyle, secondSpeedBand,
      secondBandScaled, secondFallingSpeed;

  framePhase = gFrameCount & 7;
  if (framePhase == 0) {
    firstXSource = Object_GetById(ACTOR_STUMP);
    firstRandomX = Engine_RandomNext();
    fixedX = firstXSource->x.fixed;
    fixedX +=
        ((((u32)(((firstRandomX << 1) + firstRandomX) << 2)) >> 16) << 16);
    heightSource = Object_GetById(ACTOR_STUMP);
    firstZSource = Object_GetById(ACTOR_STUMP);
    fixedZ = firstZSource->z.fixed;
    fixedZ += PIXELS(6);
    firstRandomSpeed = Engine_RandomNext();
    firstSpeedBand = ((u32)((firstRandomSpeed << 2) + firstRandomSpeed) >> 16);
    // These shifts produce speedBand * 6553: a tenth-pixel falling step.
    firstBandScaled =
        (((firstSpeedBand << 1) + firstSpeedBand) << 2) + firstSpeedBand;
    firstFallingSpeed = firstBandScaled << 6;
    firstFallingSpeed -= firstBandScaled;
    firstFallingSpeed <<= 3;
    firstFallingSpeed += firstSpeedBand;
    firstStyle = (u32)(Engine_RandomNext() << 1) >> 16;
    firstFallingSpeed = -firstFallingSpeed;
    GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
                                 firstFallingSpeed, firstStyle, framePhase);
    framePhase = gFrameCount & 15;
    if (framePhase == 0) {
      secondXSource = Object_GetById(ACTOR_STUMP);
      secondRandomX = Engine_RandomNext();
      fixedX = secondXSource->x.fixed;
      fixedX +=
          ((((u32)(((secondRandomX << 1) + secondRandomX) << 2)) >> 16) << 16);
      heightSource = Object_GetById(ACTOR_STUMP);
      secondZSource = Object_GetById(ACTOR_STUMP);
      fixedZ = secondZSource->z.fixed;
      fixedZ += PIXELS(6);
      secondRandomSpeed = Engine_RandomNext();
      secondSpeedBand =
          ((u32)((secondRandomSpeed << 2) + secondRandomSpeed) >> 16);
      // Keep the same five falling speeds for the second fragment.
      secondBandScaled =
          (((secondSpeedBand << 1) + secondSpeedBand) << 2) + secondSpeedBand;
      secondFallingSpeed = secondBandScaled << 6;
      secondFallingSpeed -= secondBandScaled;
      secondFallingSpeed <<= 3;
      secondFallingSpeed += secondSpeedBand;
      secondStyle = (u32)(Engine_RandomNext() << 1) >> 16;
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
static void Event_PushStumpOver(void) {
  s32 column;
  s32 zero;
  void (*fragmentCallback)(void);
  s32 fixedX;
  s32 fixedZ;
  struct FieldActor *heightSource;
  struct FieldActor *stump;
  struct FieldActor *spriteActor;
  struct FieldActor *blendActor;
  struct FieldActor *bounceActor, *gravityActor;
  struct FieldActor *movedActor;
  struct FieldActor *zSource, *xSource;
  s32 randomX, randomSpeed, style, speedBand, bandScaled, fallingSpeed;
  struct FieldActor *overlayActor;

  stump = Object_GetById(ACTOR_STUMP);
  column = stump->x.fixed / PIXELS(16);
  EV_BEGIN();
  if (column == 25) {
    overlayActor = Object_GetById(ACTOR_STUMP_SHADOW);
    zero = 0;
    overlayActor->terrainId = 1;
    spriteActor = Object_GetById(ACTOR_STUMP_SHADOW);
    Engine_ActorSetSpriteFlags(spriteActor, 0);
    Engine_ActorSetChildValue(ACTOR_STUMP_SHADOW, SPRITE_COLOR_BLACK);
    blendActor = Object_GetById(ACTOR_STUMP_SHADOW);
    Engine_ObjectSetBlendMode(blendActor, OBJECT_BLEND_TRANSLUCENT);
    Engine_ActorSetPosition(ACTOR_STUMP_SHADOW, PIXELS(414), PIXELS(240));
    WAIT(10);
    // Dust falls while the stump rocks on the edge.
    fragmentCallback = GomaCavePillar_SpawnStumpDust;
    FieldScheduler_AddCallback(fragmentCallback, 3200);
    Engine_AudioPlayCue(SE_141);
    Engine_ActorSetDestinationOffset(ACTOR_STUMP, 1, 0);
    Engine_ActorWaitForMove(ACTOR_STUMP);
    WAIT(10);
    Engine_ActorSetDestinationOffset(ACTOR_STUMP, 2, 0);
    Engine_ActorWaitForMove(ACTOR_STUMP);
    bounceActor = Object_GetById(ACTOR_STUMP);
    bounceActor->bounce = zero;
    gravityActor = Object_GetById(ACTOR_STUMP);
    gravityActor->gravity = PIXELS(3) / 5;
    WAIT(3);
    // It tips over the edge.
    Engine_ActorSetSpeed(ACTOR_STUMP, 5 * PIXELS(1) / 2, PIXELS(1) / 4);
    Engine_AudioPlayCue(SE_STOP_SLOT3);
    Engine_ActorSetDestination(ACTOR_STUMP, 416, 200);
    movedActor = Object_GetById(ACTOR_STUMP);
    Engine_ActorSetSpriteFlags(movedActor, 0);
    Engine_TaskRemoveCallback(fragmentCallback);
    WAIT(12);
    Engine_AudioPlayCue(SE_189);
    xSource = Object_GetById(ACTOR_STUMP);
    randomX = Engine_RandomNext();
    fixedX = xSource->x.fixed;
    fixedX += ((((u32)(((randomX << 1) + randomX) << 2)) >> 16) << 16);
    heightSource = Object_GetById(ACTOR_STUMP);
    zSource = Object_GetById(ACTOR_STUMP);
    fixedZ = zSource->z.fixed;
    fixedZ += PIXELS(6);
    randomSpeed = Engine_RandomNext();
    speedBand = ((u32)((randomSpeed << 2) + randomSpeed) >> 16);
    // A last piece of dust, with the same tenth-pixel speed bands.
    bandScaled = (((speedBand << 1) + speedBand) << 2) + speedBand;
    fallingSpeed = bandScaled << 6;
    fallingSpeed -= bandScaled;
    fallingSpeed <<= 3;
    fallingSpeed += speedBand;
    style = (u32)(Engine_RandomNext() << 1) >> 16;
    fallingSpeed = -fallingSpeed;
    GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, zero,
                                 fallingSpeed, style, zero);
    WAIT(20);
    // It lands below; the shake dies down before the map cells change.
    Engine_AudioPlayCue(SE_CAST);
    Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Engine_MapRenderWaitForValues();
    Engine_ActorSetPosition(ACTOR_STUMP, 0, 0);
    Engine_ActorSetPosition(ACTOR_STUMP_SHADOW, 0, 0);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_STUMP_FELL);
    Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  EV_END();
}

/// Once the east pillar reaches its place (cell 38, 14), it sinks two pixels
/// into the floor and six fragments fly out around it.
static void GomaCavePillar_SettleEastPillar(void) {
  s32 x;
  s32 z;

  x = Object_GetById(ACTOR_EAST_PILLAR)->x.fixed / PIXELS(16);
  z = Object_GetById(ACTOR_EAST_PILLAR)->z.fixed / PIXELS(16);
  if (x == 38 && z == 14) {
    Object_GetById(ACTOR_EAST_PILLAR)->y.fixed = -PIXELS(2);
    Object_GetById(ACTOR_EAST_PILLAR)->targetY =
        Object_GetById(ACTOR_EAST_PILLAR)->y.fixed;
    Engine_AudioPlayCue(SE_188);
    GomaCavePillar_SpawnFragment(Object_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 PIXELS(1) / 2, 0, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Object_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 PIXELS(2) / 5, PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Object_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 -PIXELS(2) / 5, PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Object_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 -PIXELS(1) / 2, 0, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Object_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 PIXELS(2) / 5, -PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Object_GetById(ACTOR_EAST_PILLAR)->x.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->y.fixed,
                                 Object_GetById(ACTOR_EAST_PILLAR)->z.fixed,
                                 -PIXELS(2) / 5, -PIXELS(2) / 5, 0, PLANE_XZ);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE);
  }
}

/// Move on the east pillar in Goma Cave, until it is in its place
/// (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE).
static void Event_MoveEastPillar(void) {
  EV_BEGIN();
  GomaCavePillar_SettleEastPillar();
  EV_END();
}

/// Walking into the east pillar in Goma Cave pushes it a step, until it is in
/// its place.
static void Event_PushEastPillar(void) {
  EV_BEGIN();
  StagedActor_AdvancePair();
  GomaCavePillar_SettleEastPillar();
  EV_END();
}

/// Walking west into the gap in Goma Cave (collision code 20): the leader
/// jumps across it. Later patches use the shared forward jump.
static void Event_JumpGapWest(void) {
#if TBS_PATCH_1
  Field_TryJumpForward();
#else
  struct FieldActor *leader;
  struct FieldPosition target;
  struct FieldPosition *destination;
  u8 *motionFlags;
  u8 savedMotionFlags;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;
  target.x = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed - PIXELS(32);
  destination = &target;
  destination->y = Object_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  destination->z = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed;
  if (FieldObject_CheckMovementCollision(leader, destination) == 0) {
    EV_BEGIN();
    Object_SetMode(leader, ANIM_LEADER_SPRING);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(leader, ANIM_LEADER_LEAP);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *motionFlags &= 0x7e;
    Engine_ActorSetSpriteFlags(leader, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER,
                              *(s16 *)((u8 *)destination + 2),
                              *(s16 *)((u8 *)destination + 10));
    Object_SetMode(leader, ANIM_LEADER_SPRING);
    Engine_ActorSetSpriteFlags(leader, 1);
    *motionFlags = savedMotionFlags;
    EV_END();
  }
#endif
}

const struct SceneEvent *Scene_GetEvents(void) {
  return gGomaCavePillar_Events;
}

/// Back from a won battle with Forge, the man reacts on a timer while the
/// Djinni joins the party; once the Djinni event is over, it raises
/// TRIGGER_FORGE_JOINED.
static void GomaCavePillar_FidgetWhileForgeFights(void) {
  struct EventWork *work;
  s32 frame;

  work = gEventWork;
  frame = ++gGomaCavePillar_WitnessFrames;
  switch (frame) {
  case 60:
    OBJ_DIR(ACTOR_WITNESS, FACING_SOUTHEAST, 0);
    Engine_ActorShowEmote(ACTOR_WITNESS, EMOTE_WINCE, 0);
    break;
  case 180:
    Engine_ActorStartRepeatedMotion(ACTOR_WITNESS, 3);
    break;
  case 240:
  case 270:
    Engine_ActorJump(ACTOR_WITNESS, 4, 0);
    break;
  case 480:
    OBJ_MODE(ACTOR_WITNESS, ANIM_SHAKE_HEAD);
    break;
  }
  if (gGameState.pendingDjinnEvent == 0)
    work->raisedTrigger = TRIGGER_FORGE_JOINED;
}

/// Raised in Goma Cave once Forge has joined the party: the man who fought it
/// is amazed, `You got it? Wow, that's pretty impressive!`, then walks off for
/// good.
static void Event_WitnessCheersForgeCaught(void) {
  Engine_TaskRemoveCallback(GomaCavePillar_FidgetWhileForgeFights);
  EV_BEGIN();
  OBJ_EMOTE(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_WITNESS, 2);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  MESSAGE_SET(MsgGomaGotWowThatsPrettyImpressive);
  Event_ShowMessage(ACTOR_WITNESS, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_WITNESS, ANIM_NOD);
  WAIT(30);
  Object_GetById(ACTOR_EAST_PILLAR)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  OBJ_SPEED(ACTOR_WITNESS, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_WITNESS, 600, 216);
  Actor_WalkToAndWait(ACTOR_WITNESS, 600, 248);
  Actor_WalkToAndWait(ACTOR_WITNESS, 568, 296);
  OBJ_POS_SET(ACTOR_WITNESS, 0, 0);
  Object_GetById(ACTOR_EAST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  FLAG_SET(FLAG_GOMA_CAVE_WITNESS_LEFT);
  EV_END();
}

/// Sets up the room as the party enters: the pillars and the stump where the
/// party left them. ENTRANCE_STUMP_FALL plays the stump's fall; after a won
/// battle with Forge, the man's reaction starts.
s32 Scene_Initialize(void) {
  struct FieldActor *actor;
  struct EventWork *work;

  work = gEventWork;
  work->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  work->transitionFrames = 24;
  Object_GetById(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
  if (FLAG_CHECK(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED) != 0) {
    OBJ_POS_SET(ACTOR_WEST_PILLAR, 344, 104);
    GomaCavePillar_CopyWestPillarCells(24);
  } else {
    GomaCavePillar_CopyWestPillarCells(18);
  }
  if (FLAG_CHECK(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
    OBJ_POS_SET(ACTOR_STUMP, 0, 0);
    Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  if (FLAG_CHECK(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE) != 0) {
    OBJ_POS_SET(ACTOR_EAST_PILLAR, 616, 232);
    if (gGameState.entrance == GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST ||
        gGameState.entrance == GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST) {
      Object_GetById(ACTOR_EAST_PILLAR)->terrainId = 2;
      actor = Object_GetById(ACTOR_EAST_PILLAR);
      actor->y.fixed--;
      Object_GetById(ACTOR_EAST_PILLAR)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Map_CopyCellAttributes(36, 48, 5, 1, 36, 14);
    }
  }
  if (gGameState.entrance == ENTRANCE_STUMP_FALL) {
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    OBJ_POS_SET(ACTOR_STUMP, 384, 192);
    WAIT(60);
    Object_GetById(ACTOR_STUMP)->terrainId = 2;
    Actor_MoveToAndWait(ACTOR_STUMP, 408, 192);
    WAIT(60);
    Event_PushStumpOver();
  }
  // Coming back from a Djinni battle: the man reacts until it is over.
  if (gGameState.pendingDjinnEvent != 0) {
    gGomaCavePillar_WitnessFrames = 0;
    Task_AddCallback(GomaCavePillar_FidgetWhileForgeFights, 3200);
  }
  return 0;
}

/// One step per facing: X pixels in the high halfword, Z pixels in the low
/// half.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

/// The scripts a pillar fragment can run, chosen by the low bits of its spawn
/// value.
struct EffectScripts gGomaCavePillar_FragmentScripts = {{
    gGomaCavePillar_FragmentScriptShort,
    gGomaCavePillar_FragmentScriptMedium,
    gGomaCavePillar_FragmentScriptLong,
}};

s32 gGomaCavePillar_FragmentScriptShort[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_FREE,
};

s32 gGomaCavePillar_FragmentScriptMedium[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_FREE,
};

s32 gGomaCavePillar_FragmentScriptLong[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_FREE,
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

struct ScenePlacement gGomaCavePillar_Placements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_STUMP, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(384), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(584), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_CRATE, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {MARS_DJINNI, FLAG_DJINNI_0 + DJINNI_FORGE, ACTOR_STAND, PIXELS(664), 0, PIXELS(216), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_MAN_IN_ORANGE, FLAG_GOMA_CAVE_WITNESS_LEFT, ACTOR_STAND, PIXELS(628), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0},
    {SCENE_TABLE_END},
};

struct SceneEvent gGomaCavePillar_Events[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 5, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 6, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 7, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 8, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 9, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT_SCRIPT(Event_PushEastPillar) },
    { TOUCH_WALKING_INTO(FACING_WEST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpGapWest) },
    { EVENT_TALK, ACTOR_FORGE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FightForge) },
    { EVENT_TALK, ACTOR_WITNESS, FLAG_DJINNI_HIDING_0 + DJINNI_FORGE, MSGNO(MsgVaultDidYouSeeSomethingStrangeOver) },
    { EVENT_TALK, ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultHmmmItMustBeHidingSomewhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, FLAG_DJINNI_HIDING_0 + DJINNI_FORGE, MSGNO(MsgVaultItJustWhackedMeInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultIKnowitsKindItllBeBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_STUMP_FELL, EVENT_SCRIPT(Event_PushStumpOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT_SCRIPT(Event_MoveEastPillar) },
    { SEARCH(SEARCH_CHEST), 101, 0xf4f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EVENT_RAISED, TRIGGER_FORGE_JOINED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WitnessCheersForgeCaught) },
    { SCENE_EVENTS_END },
};
// clang-format on
