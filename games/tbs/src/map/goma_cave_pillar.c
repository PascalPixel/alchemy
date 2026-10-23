/// @file
/// Goma Cave (MapId_GomaCavePillar), the room of two pillars and a stump,
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
/// floor underfoot, with the cells along it changed. All three are map-section
/// temp flags, so the room is as it started again once the party leaves Goma
/// Cave's four maps. Walking west into a gap (metatile behavior 20) jumps the
/// leader across it, and a chest holds a Lucky Medal.
///
/// Forge, a Mars Djinni, waits here beside a man who once fought it: `Did you
/// see something strange over there? I fought it, but it fled.` Talking to
/// Forge makes him jump in surprise, and the Djinni's battle begins. After a
/// win the room reloads with the Djinni's script still pending, so the man
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
/// WARP_DEST_STUMP_FALL, which no warp names and only the debug map selector
/// reaches, plays the stump being pushed to the edge and falling.
///
/// Open questions:
/// - XXX (no evidence): why the mask that hides the falling stump is drawn
///   black and translucent rather than in SPRITE_COLOR_HIDDEN, when either
///   would hide it the same way behind the floor; nothing in the ROM says,
///   so only the original source could.
#include "action_script.h"
#include "version.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/map/goma_cave_pillar.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "script_layer.h"
#include "map_tables.h"
#include "djinn_ids.h"
#include "pushed_object.h"
#include "edition.h"
#include "tbs_edition.h"
#include "object_event.h"
#include "save_block.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "fixed_point_position.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"

#include "flags.h"
#include "songs.h"
#include "script.h"

// Map-section temp flags: Map_ResetFlagsOnEnter clears 0x300 to 0x3ff when the
// party enters another map section, and other map sections use the numbers for
// their own.
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

/// The room's object events, numbered in the order of
/// gGomaCavePillar_ObjectEventTemplates.
enum GomaCavePillarLocalId {
  /// Slides between two places.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  LOCALID_WEST_PILLAR = LOCALID_FIRST_PLACED,
  /// Pushed over the edge into the room below.
  /// ★ Named from the game: "You want to move the stump, right?"
  /// (MsgGomaNoUsePsynergy)
  LOCALID_STUMP,
  /// Sinks into the floor in its place.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  LOCALID_EAST_PILLAR,
  /// Hides the stump where it drops behind the floor in front of the ledge: a
  /// crate drawn solid black (SPRITE_COLOR_BLACK) and translucent at sprite
  /// priority 2, behind the floor, and ahead of the stump in the sprite list,
  /// so where it covers the stump the player sees the floor. Moved away in a
  /// render of the fall, the stump shows over the floor instead. It waits in
  /// the room's corner until the stump teeters at the edge, and goes back
  /// there once the stump has fallen.
  LOCALID_MASK_FOR_STUMP,
  /// Forge, a Mars Djinni, until it joins the party.
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  LOCALID_FORGE,
  /// The man who fought Forge and lost it, a man with golden-brown hair in
  /// orange and red.
  /// ★ Named from the game: "Did you see something strange over there? I
  /// fought it, but it fled." (MsgVaultDidYouSeeSomethingStrangeOver)
  LOCALID_WITNESS
};

/// The warp destination no warp names, which plays the stump being pushed to
/// the edge and falling into the room below; only the debug map selector
/// reaches it.
enum { WARP_DEST_STUMP_FALL = 99 };

/// The map event the man's timer raises once Forge has joined the party
/// (GomaCavePillar_FidgetWhileForgeFights).
enum GomaCavePillarRaisedEvent { RAISED_FORGE_JOINED = 99 };

// Shared movement routines stay in their linked order.
#include "common/object_staged_object_event.h"

/// Talking to Forge, a Mars Djinni, in Goma Cave: the man beside it jumps in
/// surprise, and the battle with the Djinni begins.
static void EventScript_FightForge(void) {
  OBJECT_EMOTE(LOCALID_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_Launch(LOCALID_WITNESS, 2, 0);
  Field_RequestDjinnBattle(LOCALID_FORGE, DJINNI_FORGE);
}

/// Advances the fragment's selected coordinate plane and keeps its targets in
/// step.
static void GomaCavePillar_UpdateFragment(union FieldObject *fieldObject) {
  struct ObjectEvent *fragment = (struct ObjectEvent *)fieldObject;
  // The object event's speed and acceleration slots carry the two selected axis
  // steps.
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
static s32 GomaCavePillar_FinishFragment(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  ObjectGroup_SetChildValue(objectEvent, 15);
  return 0;
}

/// Creates a fragment with two axis rates, a script and packed plane/priority.
static void GomaCavePillar_SpawnFragment(s32 x, s32 y, s32 z, s32 firstRate,
                                         s32 secondRate,
                                         s32 scriptAndChildValue,
                                         s32 planeAndPriority) {
  // A struct copy of the named table, which sits in the middle of .data; a
  // local array of the three scripts compiles to the same code.
  struct EffectScripts table;
  struct ObjectEvent *leader;
  struct ObjectEvent *fragment;
  struct Sprite *sprite;
  u32 priority;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  table = gGomaCavePillar_FragmentScripts;
  fragment = ObjectEvent_Create(SPRITE_DUST_PUFF, x, y, z);
  if (fragment == 0)
    return;
  sprite = fragment->sprite;
  ObjectDispatch_SelectAnimation(fragment, (scriptAndChildValue + 1) & 15);
  ObjectEvent_SetScript(fragment, table.script[scriptAndChildValue & 15]);
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
    fragment->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
    sprite->image.priority = priority;
    break;
  }
}

const struct MapWarpDest *Map_GetWarpDests(void) {
  return gGomaCavePillar_WarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gGomaCavePillar_Warps; }

const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  return gGomaCavePillar_ObjectEventTemplates;
}

/// Copies the map grid cells for the west pillar on either side of its track.
static inline void GomaCavePillar_CopyWestPillarCells(s32 sourceColumn) {
  MapGrid_CopyCellAttributes(sourceColumn, 40, 6, 3, 18, 6);
}

/// Move on the west pillar in Goma Cave: it slides between its two places,
/// and the map grid cells follow it.
static void EventScript_MoveWestPillar(void) {
  struct ObjectEvent *objectEvent;
  s32 x;

  objectEvent = ObjectEvent_GetById(LOCALID_WEST_PILLAR);
  x = objectEvent->x.fixed;
  if (x < 0)
    x += 0xfffff;
  x >>= 20;
  SCRIPT_START();
  if (x == 20) {
    GomaCavePillar_CopyWestPillarCells(18);
    GameFlag_Clear(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
  } else {
    GomaCavePillar_CopyWestPillarCells(24);
    GameFlag_Set(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
  }
  SCRIPT_END();
}

/// While the stump teeters, dust falls from it: one fragment every eighth
/// frame, two on every sixteenth.
static void GomaCavePillar_SpawnStumpDust(void) {
  s32 framePhase;
  s32 fixedX;
  s32 fixedZ;
  struct ObjectEvent *heightSource;
  struct ObjectEvent *firstXSource, *firstZSource;
  s32 firstRandomX, firstRandomSpeed, firstStyle, firstSpeedBand,
      firstBandScaled, firstFallingSpeed;
  struct ObjectEvent *secondXSource, *secondZSource;
  s32 secondRandomX, secondRandomSpeed, secondStyle, secondSpeedBand,
      secondBandScaled, secondFallingSpeed;

  framePhase = gFrameCount & 7;
  if (framePhase == 0) {
    firstXSource = ObjectEvent_GetById(LOCALID_STUMP);
    firstRandomX = Engine_RandomNext();
    fixedX = firstXSource->x.fixed;
    fixedX +=
        ((((u32)(((firstRandomX << 1) + firstRandomX) << 2)) >> 16) << 16);
    heightSource = ObjectEvent_GetById(LOCALID_STUMP);
    firstZSource = ObjectEvent_GetById(LOCALID_STUMP);
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
      secondXSource = ObjectEvent_GetById(LOCALID_STUMP);
      secondRandomX = Engine_RandomNext();
      fixedX = secondXSource->x.fixed;
      fixedX +=
          ((((u32)(((secondRandomX << 1) + secondRandomX) << 2)) >> 16) << 16);
      heightSource = ObjectEvent_GetById(LOCALID_STUMP);
      secondZSource = ObjectEvent_GetById(LOCALID_STUMP);
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
static void EventScript_PushStumpOver(void) {
  s32 column;
  s32 zero;
  void (*fragmentCallback)(void);
  s32 fixedX;
  s32 fixedZ;
  struct ObjectEvent *heightSource;
  struct ObjectEvent *stump;
  struct ObjectEvent *spriteObjectEvent;
  struct ObjectEvent *blendObjectEvent;
  struct ObjectEvent *bounceObjectEvent, *gravityObjectEvent;
  struct ObjectEvent *movedObjectEvent;
  struct ObjectEvent *zSource, *xSource;
  s32 randomX, randomSpeed, style, speedBand, bandScaled, fallingSpeed;
  struct ObjectEvent *overlayObjectEvent;

  stump = ObjectEvent_GetById(LOCALID_STUMP);
  column = stump->x.fixed / PIXELS(16);
  SCRIPT_START();
  if (column == 25) {
    overlayObjectEvent = ObjectEvent_GetById(LOCALID_MASK_FOR_STUMP);
    zero = 0;
    overlayObjectEvent->layer = 1;
    spriteObjectEvent = ObjectEvent_GetById(LOCALID_MASK_FOR_STUMP);
    ObjectEvent_SetSpriteFlags(spriteObjectEvent, 0);
    ObjectGroup_ConfigureChildValue(LOCALID_MASK_FOR_STUMP, SPRITE_COLOR_BLACK);
    blendObjectEvent = ObjectEvent_GetById(LOCALID_MASK_FOR_STUMP);
    ObjectEvent_SetBlendMode(blendObjectEvent, OBJECT_BLEND_TRANSLUCENT);
    Engine_ObjectEventSetPosition(LOCALID_MASK_FOR_STUMP, PIXELS(414),
                                  PIXELS(240));
    WAIT(10);
    // Dust falls while the stump rocks on the edge.
    fragmentCallback = GomaCavePillar_SpawnStumpDust;
    FieldScheduler_AddCallback(fragmentCallback, 3200);
    Sound_PlaySong(SE_RUMBLE);
    ObjectMotion_OffsetPositionAndResetMotion(LOCALID_STUMP, 1, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_STUMP);
    WAIT(10);
    ObjectMotion_OffsetPositionAndResetMotion(LOCALID_STUMP, 2, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_STUMP);
    bounceObjectEvent = ObjectEvent_GetById(LOCALID_STUMP);
    bounceObjectEvent->bounce = zero;
    gravityObjectEvent = ObjectEvent_GetById(LOCALID_STUMP);
    gravityObjectEvent->gravity = PIXELS(3) / 5;
    WAIT(3);
    // It tips over the edge.
    ObjectMotion_SetSpeedParameters(LOCALID_STUMP, 5 * PIXELS(1) / 2,
                                    PIXELS(1) / 4);
    Sound_PlaySong(SE_STOP_SLOT3);
    ObjectMotion_ResetAndSetPosition(LOCALID_STUMP, 416, 200);
    movedObjectEvent = ObjectEvent_GetById(LOCALID_STUMP);
    ObjectEvent_SetSpriteFlags(movedObjectEvent, 0);
    Task_Remove(fragmentCallback);
    WAIT(12);
    Sound_PlaySong(SE_189);
    xSource = ObjectEvent_GetById(LOCALID_STUMP);
    randomX = Engine_RandomNext();
    fixedX = xSource->x.fixed;
    fixedX += ((((u32)(((randomX << 1) + randomX) << 2)) >> 16) << 16);
    heightSource = ObjectEvent_GetById(LOCALID_STUMP);
    zSource = ObjectEvent_GetById(LOCALID_STUMP);
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
    // It lands below; the shake dies down before the map grid cells change.
    Sound_PlaySong(SE_CAST);
    MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    MapGrid_WaitForShakeToSettle();
    Engine_ObjectEventSetPosition(LOCALID_STUMP, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_MASK_FOR_STUMP, 0, 0);
    GameFlag_Set(FLAG_GOMA_CAVE_STUMP_FELL);
    MapGrid_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  SCRIPT_END();
}

/// Once the east pillar reaches its place (cell 38, 14), it sinks two pixels
/// into the floor and six fragments fly out around it.
static void GomaCavePillar_SettleEastPillar(void) {
  s32 x;
  s32 z;

  x = ObjectEvent_GetById(LOCALID_EAST_PILLAR)->x.fixed / PIXELS(16);
  z = ObjectEvent_GetById(LOCALID_EAST_PILLAR)->z.fixed / PIXELS(16);
  if (x == 38 && z == 14) {
    ObjectEvent_GetById(LOCALID_EAST_PILLAR)->y.fixed = -PIXELS(2);
    ObjectEvent_GetById(LOCALID_EAST_PILLAR)->targetY =
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->y.fixed;
    Sound_PlaySong(SE_188);
    GomaCavePillar_SpawnFragment(
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->x.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->y.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->z.fixed, PIXELS(1) / 2, 0, 0,
        PLANE_XZ);
    GomaCavePillar_SpawnFragment(
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->x.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->y.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->z.fixed, PIXELS(2) / 5,
        PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->x.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->y.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->z.fixed, -PIXELS(2) / 5,
        PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->x.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->y.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->z.fixed, -PIXELS(1) / 2, 0, 0,
        PLANE_XZ);
    GomaCavePillar_SpawnFragment(
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->x.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->y.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->z.fixed, PIXELS(2) / 5,
        -PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->x.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->y.fixed,
        ObjectEvent_GetById(LOCALID_EAST_PILLAR)->z.fixed, -PIXELS(2) / 5,
        -PIXELS(2) / 5, 0, PLANE_XZ);
    GameFlag_Set(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE);
  }
}

/// Move on the east pillar in Goma Cave, until it is in its place
/// (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE).
static void EventScript_MoveEastPillar(void) {
  SCRIPT_START();
  GomaCavePillar_SettleEastPillar();
  SCRIPT_END();
}

/// Walking into the east pillar in Goma Cave pushes it a step, until it is in
/// its place.
static void EventScript_PushEastPillar(void) {
  SCRIPT_START();
  PushedObject_AdvancePair();
  GomaCavePillar_SettleEastPillar();
  SCRIPT_END();
}

/// Walking west into the gap in Goma Cave (metatile behavior 20): the leader
/// jumps across it. Later patches use the shared forward jump.
static void EventScript_JumpGapWest(void) {
#if TBS_PATCH_1
  Field_TryJumpForward();
#else
  struct ObjectEvent *leader;
  struct FieldPosition target;
  struct FieldPosition *destination;
  u8 *motionFlags;
  u8 savedMotionFlags;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;
  target.x = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed - PIXELS(32);
  destination = &target;
  destination->y = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed;
  destination->z = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed;
  if (FieldObject_CheckMovementCollision(leader, destination) == 0) {
    SCRIPT_START();
    ObjectDispatch_SelectAnimation(leader, ANIM_LEADER_SPRING);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    ObjectDispatch_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *motionFlags &= 0x7e;
    ObjectEvent_SetSpriteFlags(leader, 0);
    ObjectMotion_SetPositionAndCommit(LOCALID_PARTY_LEADER,
                                      *(s16 *)((u8 *)destination + 2),
                                      *(s16 *)((u8 *)destination + 10));
    ObjectDispatch_SelectAnimation(leader, ANIM_LEADER_SPRING);
    ObjectEvent_SetSpriteFlags(leader, 1);
    *motionFlags = savedMotionFlags;
    SCRIPT_END();
  }
#endif
}

const struct MapEvent *Map_GetEvents(void) { return gGomaCavePillar_Events; }

/// Back from a won battle with Forge, the man reacts on a timer while the
/// Djinni joins the party; once the Djinni's script is over, it raises
/// RAISED_FORGE_JOINED.
static void GomaCavePillar_FidgetWhileForgeFights(void) {
  struct FieldWork *work;
  s32 frame;

  work = gFieldWork;
  frame = ++gGomaCavePillar_WitnessFrames;
  switch (frame) {
  case 60:
    OBJECT_DIR(LOCALID_WITNESS, FACING_SOUTHEAST, 0);
    FieldEffect_SpawnLinked(LOCALID_WITNESS, EMOTE_WINCE, 0);
    break;
  case 180:
    ObjectMotion_SetVariantCallback(LOCALID_WITNESS, 3);
    break;
  case 240:
  case 270:
    ObjectMotion_Launch(LOCALID_WITNESS, 4, 0);
    break;
  case 480:
    OBJECT_ANIMATION(LOCALID_WITNESS, ANIM_SHAKE_HEAD);
    break;
  }
  if (gSaveBlock.pendingDjinnEvent == 0)
    work->raisedEvent = RAISED_FORGE_JOINED;
}

/// Raised in Goma Cave once Forge has joined the party: the man who fought it
/// is amazed, `You got it? Wow, that's pretty impressive!`, then walks off for
/// good.
static void EventScript_WitnessCheersForgeCaught(void) {
  Task_Remove(GomaCavePillar_FidgetWhileForgeFights);
  SCRIPT_START();
  OBJECT_EMOTE(LOCALID_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WITNESS, 2);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  MESSAGE_SET(MsgGomaGotWowThatsPrettyImpressive);
  Script_ShowMessage(LOCALID_WITNESS, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_WITNESS, ANIM_NOD);
  WAIT(30);
  ObjectEvent_GetById(LOCALID_EAST_PILLAR)->priorityFlags &=
      ~OBJ_EVENT_PRIORITY_UNDERFOOT;
  OBJECT_SPEED(LOCALID_WITNESS, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkToAndWait(LOCALID_WITNESS, 600, 216);
  ObjectEvent_WalkToAndWait(LOCALID_WITNESS, 600, 248);
  ObjectEvent_WalkToAndWait(LOCALID_WITNESS, 568, 296);
  OBJECT_POSITION_SET(LOCALID_WITNESS, 0, 0);
  ObjectEvent_GetById(LOCALID_EAST_PILLAR)->priorityFlags |=
      OBJ_EVENT_PRIORITY_UNDERFOOT;
  GameFlag_Set(FLAG_GOMA_CAVE_WITNESS_LEFT);
  SCRIPT_END();
}

/// Sets up the room as the party enters: the pillars and the stump where the
/// party left them. WARP_DEST_STUMP_FALL plays the stump's fall; after a won
/// battle with Forge, the man's reaction starts.
s32 Map_Initialize(void) {
  struct ObjectEvent *objectEvent;
  struct FieldWork *work;

  work = gFieldWork;
  work->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  work->transitionFrames = 24;
  ObjectEvent_GetById(LOCALID_STUMP)->collisionFlags |= OBJ_EVENT_NEAR_PSYNERGY;
  if (GameFlag_Get(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED) != 0) {
    OBJECT_POSITION_SET(LOCALID_WEST_PILLAR, 344, 104);
    GomaCavePillar_CopyWestPillarCells(24);
  } else {
    GomaCavePillar_CopyWestPillarCells(18);
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
    OBJECT_POSITION_SET(LOCALID_STUMP, 0, 0);
    MapGrid_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE) != 0) {
    OBJECT_POSITION_SET(LOCALID_EAST_PILLAR, 616, 232);
    if (gSaveBlock.warpDest == GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST ||
        gSaveBlock.warpDest == GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST) {
      ObjectEvent_GetById(LOCALID_EAST_PILLAR)->layer = 2;
      objectEvent = ObjectEvent_GetById(LOCALID_EAST_PILLAR);
      objectEvent->y.fixed--;
      ObjectEvent_GetById(LOCALID_EAST_PILLAR)->priorityFlags |=
          OBJ_EVENT_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(36, 48, 5, 1, 36, 14);
    }
  }
  if (gSaveBlock.warpDest == WARP_DEST_STUMP_FALL) {
    FieldScript_StartScreenTransition();
    FieldScript_WaitTransitionFrames();
    OBJECT_POSITION_SET(LOCALID_STUMP, 384, 192);
    WAIT(60);
    ObjectEvent_GetById(LOCALID_STUMP)->layer = 2;
    ObjectEvent_MoveToAndWait(LOCALID_STUMP, 408, 192);
    WAIT(60);
    EventScript_PushStumpOver();
  }
  // Coming back from a Djinni battle: the man reacts until it is over.
  if (gSaveBlock.pendingDjinnEvent != 0) {
    gGomaCavePillar_WitnessFrames = 0;
    Task_AddCallback(GomaCavePillar_FidgetWhileForgeFights, 3200);
  }
  return 0;
}

/// One step per facing: X pixels in the high halfword, Z pixels in the low
/// half.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
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

struct MapWarpDest gGomaCavePillar_WarpDests[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_WEST, CONDITION_ALWAYS, 88, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST, CONDITION_ALWAYS, 584, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST, CONDITION_ALWAYS, 232, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_SOUTH, CONDITION_ALWAYS, 536, 0, 408, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { WARP_DEST_STUMP_FALL, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { MAP_TABLE_END },
};

u32 gGomaCavePillar_Warps[] = {
    MAP_WARPS(GomaCavePillar),
    MAP_WARP(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST, GomaCaveStream, GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_WEST),
    MAP_WARP(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_WEST),
    MAP_WARP(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_SOUTH),
    MAP_WARP(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_EAST),
    MAP_WARP(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST, GomaCaveStream, GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_EAST),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gGomaCavePillar_ObjectEventTemplates[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(384), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(584), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {MARS_DJINNI, FLAG_DJINNI(DJINNI_FORGE), MOVEMENT_TYPE_STAND, PIXELS(664), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_MAN_IN_ORANGE, FLAG_GOMA_CAVE_WITNESS_LEFT, MOVEMENT_TYPE_STAND, PIXELS(628), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0},
    {MAP_TABLE_END},
};

struct MapEvent gGomaCavePillar_Events[] = {
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, LOCALID_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT_SCRIPT(EventScript_PushEastPillar) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpGapWest) },
    { MAP_EVENT_TALK, LOCALID_FORGE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_FightForge) },
    { MAP_EVENT_TALK, LOCALID_WITNESS, FLAG_DJINNI_HIDING(DJINNI_FORGE), MSGNO(MsgVaultDidYouSeeSomethingStrangeOver) },
    { MAP_EVENT_TALK, LOCALID_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultHmmmItMustBeHidingSomewhere) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WITNESS, FLAG_DJINNI_HIDING(DJINNI_FORGE), MSGNO(MsgVaultItJustWhackedMeInThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultIKnowitsKindItllBeBack) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_WEST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MoveWestPillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_STUMP, FLAG_GOMA_CAVE_STUMP_FELL, EVENT_SCRIPT(EventScript_PushStumpOver) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT_SCRIPT(EventScript_MoveEastPillar) },
    { BG_EVENT(BG_EVENT_CHEST), 101, 0xf4f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENT_RAISED, RAISED_FORGE_JOINED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WitnessCheersForgeCaught) },
    { MAP_EVENTS_END },
};
// clang-format on
