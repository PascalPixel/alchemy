/// @file
/// Goma Cave Entrance (SceneId_GomaCaveEntrance), the mouth of the cave in
/// the Goma Range between Vault and Bilibin, where Ivan rejoins the party.
///
/// A stump caught in ivy (ACTOR_STUMP, held by ACTOR_STUMP_IVY) blocks the
/// way on. Until FLAG_GOMA_CAVE_IVAN_JOINED is set, Move on it runs
/// GomaCaveEntrance_IvanRejoins. Garet says their Psynergy can't free it.
/// Only once the thieves in Vault are jailed (FLAG_VAULT_THIEVES_JAILED) does
/// Ivan walk up, blow the ivy off with Whirlwind and rejoin with the Shaman's
/// Rod equipped.
///
/// The ivy is never the player's target (ACTOR_NOT_TARGETED), so that event
/// is the only way past it.
///
/// After it, Move runs GomaCaveEntrance_MoveStump, which sends the stump
/// leaping out of the way once it stands on column 11. It lands in the pool
/// under the waterfall and sinks until only its top shows, a stepping stone
/// beside ACTOR_SUNKEN_STUMP (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED; its render,
/// places/goma-cave-entrance_goma-cave-entrance-stump-moved/map-0.png).
///
/// The two stone pillars and a second ivy are puzzles on the way.
///
/// - The west pillar is pushed by walking into it or moved with Move. It
///   stands on one of three columns, which two flags record
///   (FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP and _TWO_STEPS).
/// - The east pillar, moved with Move to its last column, settles into the
///   floor so it can be walked over
///   (FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE).
/// - Whirlwind blows the second ivy away for good
///   (FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY). Its actor placement row is
///   then skipped, though it keeps its actor id.
///
/// Scene_Initialize blocks Retreat and puts everything back as these flags
/// say: the stump where it landed, the pillars on their columns and the map
/// grid cells that go with them.
///
/// GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM leads into Goma Cave
/// (SceneId_GomaCaveStream) and GOMA_CAVE_ENTRANCE_TO_WORLD_MAP out to the
/// World Map.
#include "module/scene/goma_cave_entrance.h"
#include "version.h"
#include "module/scene/shared/field_effect_spawn.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/game/game.h"
#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "actor.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "create_configured_actor.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "edition.h"
#include "playback_queue.h"
#include "battle_action_run.h"
#include "battle_unit.h"
#include "item.h"
#include "inventory.h"
#include "text/msg_ids.h"
#include "actor_script.h"
#include "heap_slots.h"
#include "ability_ids.h"
#include "macros.h"
#include "flags.h"
#include "sounds.h"
#include "item_ids.h"
#include "vector3.h"
#include "field_psynergy_work.h"
#include "fixed_math.h"

/// The room's actors.
///
/// Garet's and Ivan's rows go to their party slots, and the rows after them
/// number from ACTOR_FIRST_PLACED.
enum GomaCaveEntranceActorId {
  /// The stump caught in ivy that blocks the way on.
  /// ★ Named from the game: "It's no use. Our Psynergy isn't strong enough to
  /// free that stump." (MsgGomaNoUsePsynergy)
  ACTOR_STUMP = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  ACTOR_WEST_PILLAR,
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  ACTOR_EAST_PILLAR,
  /// A second stump, always sunk until only its top shows, that the party
  /// walks over.
  /// ★ Named from what the player sees: a stump sunk to its cut top, the last
  /// frame of STUMP_ANIMATION_SINK in spr_212.png (its render)
  ACTOR_SUNKEN_STUMP,
  /// Unused: two Move hands and a third stump, placed at the map's origin
  /// outside the camera's bounds.
  ///
  /// Nothing in the overlay addresses them, and Move spawns its own hand
  /// (FieldEffect_SpawnMoveHand), so the player never sees them.
  /// ★ Named from what the player sees: a large open hand, spr_215.png (its
  /// render)
  ACTOR_UNUSED_FIRST_MOVE_HAND,
  /// ★ Named from what the player sees: a large open hand, spr_215.png (its
  /// render)
  ACTOR_UNUSED_SECOND_MOVE_HAND,
  /// ★ Named from what the player sees: a tree stump, spr_212.png (its render)
  ACTOR_UNUSED_STUMP,
  /// The ivy holding the stump, until Ivan blows it away.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  ACTOR_STUMP_IVY,
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  ACTOR_IVY
};

// Private event routines used by the reunion event.
static void GomaCaveEntrance_WhirlwindOnStumpIvy(void);
static void GomaCaveEntrance_LeapStump(s32 stumpId);
static void GomaCaveEntrance_GiveShamansRod(void);

// Shared effects stay at their native positions in the overlay.
#include "shared/field_effect_create.h"
#include "shared/field_effect_spawn.h"

/// Shows the next message over the speaker, then pauses.
static void GomaCaveEntrance_SayThenWait(s32 actorId, s32 frames) {
  Event_ShowMessage(actorId, 0);
  Event_Wait(frames);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gGomaCaveEntranceEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gGomaCaveEntranceExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gGomaCaveEntranceActorPlacements;
}

/// Move on the west pillar at Goma Cave Entrance, and after each push: the
/// pillar stands on one of three columns.
///
/// The map grid cells and flags follow it.
static void GomaCaveEntrance_MoveWestPillar(void) {
  struct Actor *pillar;
  s32 column;

  pillar = Actor_Get(ACTOR_WEST_PILLAR);
  column = pillar->x.fixed / px(16);
  Event_ClearFlag(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP);
  Event_ClearFlag(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS);
  if (column == 15) {
    MapGrid_CopyCellAttributes(47, 18, 1, 2, 16, 18);
  } else if (column == 16) {
    MapGrid_CopyCellAttributes(48, 18, 1, 2, column, 18);
    GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP);
  } else {
    MapGrid_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS);
  }
}

/// Move on the east pillar at Goma Cave Entrance: once it reaches column 23 it
/// settles into the floor and can be walked over.
static void GomaCaveEntrance_MoveEastPillar(void) {
  struct Actor *pillar;
  s32 column;

  pillar = Actor_Get(ACTOR_EAST_PILLAR);
  column = pillar->x.fixed / px(16);
  if (column == 23) {
    Event_Wait(10);
    Actor_Get(ACTOR_EAST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_Get(ACTOR_EAST_PILLAR)->motionFlags = 0;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_EAST_PILLAR), 0);
    MapGrid_CopyCellAttributes(54, 17, 1, 1, column, 17);
    GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE);
  }
}

/// Walking into the west pillar at Goma Cave Entrance from the west
/// (step trigger 5) or the east (step trigger 6): the leader leans on it and
/// pushes it one step.
static void GomaCaveEntrance_PushWestPillar(void) {
  struct FieldWork *work;
  s32 offsetX;

  work = gFieldWork;
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
  Actor_SetSpeed(ACTOR_WEST_PILLAR, 0.2);
  Sound_Play(SE_PUSH);
  // 16 pixels east from step trigger 5, west from step trigger 6.
  offsetX = (11 - (work->stepTrigger << 1)) << 4;
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, offsetX, 0);
  Actor_SetDestinationOffset(ACTOR_WEST_PILLAR, offsetX, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WaitForMoveById(ACTOR_WEST_PILLAR);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  GomaCaveEntrance_MoveWestPillar();
  Scene_PlayQueuedMusic();
  Event_End();
}

UNUSED static void GomaCaveEntrance_NoOpA(void) {}

UNUSED static void GomaCaveEntrance_NoOpB(void) {}

/// Whirlwind on the second ivy at Goma Cave Entrance: it is blown away for
/// good.
static void GomaCaveEntrance_BlowAwayIvy(void) {
  Event_Start();
  GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  return gGomaCaveEntranceTriggers;
}

/// Sets up Goma Cave Entrance.
///
/// Retreat is blocked, and the stump and pillars stand where the party left
/// them. The stump's ivy is gone once Ivan has blown it off.
s32 Scene_Initialize(void) {
  struct Actor *movedActor;

  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  Event_Wait(10);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Actor_SetAnimation(ACTOR_SUNKEN_STUMP, STUMP_ANIMATION_SINK);
  Actor_Get(ACTOR_SUNKEN_STUMP)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  {
    struct Actor *record = Actor_Get(ACTOR_STUMP);

    record->collisionFlags = ACTOR_NEAR_PSYNERGY | record->collisionFlags;
  }
  // The player never targets the stump's ivy; only Ivan's Whirlwind in
  // GomaCaveEntrance_IvanRejoins removes it.
  {
    struct Actor *record = Actor_Get(ACTOR_STUMP_IVY);

    record->collisionFlags = ACTOR_NOT_TARGETED | record->collisionFlags;
  }
  // The cells GomaCaveEntrance_IvanRejoins copies as it sets the flag, redrawn
  // on arrival.
  if (GameFlag_Get(FLAG_GOMA_CAVE_IVAN_JOINED) != 0) {
    MapGrid_CopyCellAttributes(74, 11, 1, 1, 73, 11);
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED) != 0) {
    Actor_SetPosition(ACTOR_STUMP, 136, 196);
    Actor_Get(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
    MapGrid_CopyCellAttributes(39, 12, 3, 1, 8, 12);
    MapGrid_CopyCellAttributes(43, 11, 3, 1, 12, 11);
  }
  // One step along, the west pillar stands on column 16, and the two cells
  // there are plain ground where they are otherwise a climb; two steps along
  // it has cleared them and the climb is back.
  if (GameFlag_Get(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP) != 0) {
    Actor_SetPosition(ACTOR_WEST_PILLAR, 264, 312);
    MapGrid_CopyCellAttributes(48, 18, 1, 2, 16, 18);
  } else {
    // The cells GomaCaveEntrance_MoveWestPillar copies as it sets the flag,
    // redrawn on arrival.
    if (GameFlag_Get(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS) != 0) {
      Actor_SetPosition(ACTOR_WEST_PILLAR, 280, 312);
      MapGrid_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    }
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE) != 0) {
    Actor_SetPosition(ACTOR_EAST_PILLAR, 376, 280);
    Actor_Get(ACTOR_EAST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_Get(ACTOR_EAST_PILLAR)->motionFlags = 0;
    movedActor = Actor_Get(ACTOR_EAST_PILLAR);
    Actor_SetSpriteFlags(movedActor, 0);
    MapGrid_CopyCellAttributes(54, 17, 1, 1, 23, 17);
  }
  return 0;
}

/// Move on the stump at Goma Cave Entrance while the ivy holds it: Garet says
/// their Psynergy is not strong enough.
///
/// Once the thieves in Vault are jailed (FLAG_VAULT_THIEVES_JAILED), Ivan
/// turns up and blows the ivy away with Whirlwind. He says he could not get
/// into Lunpa and asks to join the party.
///
/// He rejoins with the Shaman's Rod equipped (FLAG_GOMA_CAVE_IVAN_JOINED).
static void GomaCaveEntrance_IvanRejoins(void) {
  struct Actor *leader;

#if TBS_PATCH_1
  Scene_PlayQueuedMusic();
#endif
  Event_Start();
  Event_Wait(30);
  Event_SetMessageId(MsgGomaNoUsePsynergy);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPositionFixed(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkToAndWait(ACTOR_GARET, 264, 168);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "It's no use. Our Psynergy isn't strong enough to free that..."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    // Garet falls back into line.
    Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
    Event_End();
  } else {
    // Ivan comes up from the south: "You want to move the stump, right?"
    Actor_SetPosition(ACTOR_IVAN, 360, 248);
    Actor_SetSpeed(ACTOR_IVAN, 0.8);
    Actor_WalkToAndWait(ACTOR_IVAN, 272, 248);
    Actor_WalkToAndWait(ACTOR_IVAN, 272, 208);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    // "You want to move the stump, right?"
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Actor_WalkToAndWait(ACTOR_IVAN, 264, 200);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 168);
    Actor_WalkToAndWait(ACTOR_IVAN, 248, 184);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 232, 184);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "It looks like it's trapped in the ivy."
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    // "Isn't there anything we can do?"
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 120);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(30);
    // "I know!"
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 30);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    // "I have a great idea. Leave it to me!"
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    // "I have a great idea. Leave it to me!": Whirlwind.
    GomaCaveEntrance_WhirlwindOnStumpIvy();
    Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
    Event_Wait(20);
#if TBS_PATCH_1
    Scene_PlayQueuedMusic();
#endif
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Actor_Jump(ACTOR_GARET, 2, 0);
    Event_Wait(20);
    GomaCaveEntrance_SayThenWait(ACTOR_GARET, 20);
    Actor_WalkToAndWait(ACTOR_IVAN, 264, 184);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Event_Wait(10);
    GomaCaveEntrance_SayThenWait(ACTOR_GARET, 20);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GARET, 1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Event_Wait(80);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 30);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    // "Please, allow me to join your quest.": Ivan rejoins with his rod.
    Event_AddPartyMember(IVAN, 1);
    Event_Wait(60);
    GomaCaveEntrance_GiveShamansRod();
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_IVAN, 248, 184);
    Event_Wait(20);
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Event_Wait(120);
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(50);
    Actor_SetSpeed(ACTOR_GARET, 0.8);
    Actor_SetSpeed(ACTOR_IVAN, 0.8);
    Actor_WalkTo(ACTOR_GARET, 248, 168);
    Actor_WalkToAndWait(ACTOR_IVAN, 248, 168);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
    MapGrid_CopyCellAttributes(74, 11, 1, 1, 73, 11);
    GameFlag_Set(FLAG_GOMA_CAVE_IVAN_JOINED);
    Event_End();
  }
}

/// Move on the freed stump at Goma Cave Entrance: from column 11 it spins,
/// leaps away and lies where it lands (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED).
static void GomaCaveEntrance_MoveStump(void) {
  s32 column;
  struct Actor *stump;

  Event_Start();
  column = FIELD_CELL(Actor_Get(ACTOR_STUMP)->x.fixed);
  if (column == 11) {
    GomaCaveEntrance_LeapStump(ACTOR_STUMP);
    stump = Actor_Get(ACTOR_STUMP);
    stump->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(39, 12, 3, 1, 8, 12);
    MapGrid_CopyCellAttributes(43, 11, 3, 1, 12, column);
    GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED);
  }
  Event_End();
}

/// Ivan casts Whirlwind on the stump's ivy, and the event waits for it.
///
/// The announcement is lowered to a fixed place (FIELD_MESSAGE_LOWER).
/// Otherwise it would be placed by where the leader, not Ivan, stands.
static void GomaCaveEntrance_WhirlwindOnStumpIvy(void) {
  struct FieldEffectWork *effectWork;
  u8 flags;

  effectWork = gFieldEffectWork;
  FieldPsynergy_LoadResources(ABILITY_WHIRLWIND, 1);
  FieldPsynergy_SetupActorPair(ACTOR_IVAN, ACTOR_STUMP_IVY);
  flags = (u8)effectWork->messageMode | FIELD_MESSAGE_LOWER;
  effectWork->messageMode = flags;
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET);
  FieldPsynergy_StopCasterUpdate();
}

/// The leaping stump's update routine turns its sprite a fixed amount per
/// frame.
static void GomaCaveEntrance_SpinStump(struct Actor *actor) {
  struct Sprite *state = actor->sprite;
  s32 v = state->rotation - TRIG_TURN / 32;

  state->rotation = v;
}

// Shared height-wait code stays at its native position in the overlay.
#include "shared/actor_wait_below_height.h"

/// The stump leaps out of the way and sinks in the pool.
///
/// It spins in place, leaps and lands with a shake and a ring of dust. Then
/// it hops on into the pool under the waterfall with a splash and sinks
/// (STUMP_ANIMATION_SPLASH).
static void GomaCaveEntrance_LeapStump(s32 stumpId) {
  struct Vector3 dir;
  struct EffectOptions params;
  struct Actor *stump;
  u32 i;

  stump = Actor_Get(stumpId);
  stump->motionFlags = 0;
  for (i = 0; i <= 17; i++) {
    wait(1);
    stump->sprite->rotation -= TRIG_TURN / 256;
    stump->x.fixed -= Trig_Cos(stump->sprite->rotation) / 2;
    stump->targetX = ACTOR_NO_TARGET;
  }
  stump->update = GomaCaveEntrance_SpinStump;
  Actor_SetSpeed(stumpId, 3);
  Actor_SetDestination(stumpId, 160, 192);
  stump->gravity = px(4) / 5;
  stump->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  stump->layer = 0;
  Actor_WaitForMoveById(stumpId);
  Actor_WaitBelowHeight(stump, px(32));
  Sound_Play(SE_THUMP);
  MapGrid_SetShake(5, 5, 1);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  for (i = 0; i <= 16; i++) {
    SET_POSITION(&dir, Trig_Cos(i << 12), 0, Trig_Sin(i << 12));
    dir.x -= dir.x / 4;
    dir.z -= dir.z / 2;
    FieldEffect_Spawn(stump->x.fixed, stump->y.fixed, stump->z.fixed, dir.x,
                      dir.y, dir.z, 0, NULL);
  }
  stump->velocityY = px(5);
  Actor_SetDestination(stumpId, 139, 196);
  Actor_WaitForMoveById(stumpId);
  Actor_WaitBelowHeight(stump, px(32));
  // UB: the update routine is cleared as a word.
#ifdef UBFIX
  stump->update = NULL;
#else
  *(u32 *)&stump->update = 0;
#endif
  stump->sprite->rotation = TRIG_TURN / 16;
  params.type = SPRITE_SPLASH;
  params.startScaleX = Q16_ONE / 2;
  params.startScaleY = 4 * Q16_ONE / 5;
  params.targetScaleX = 3 * Q16_ONE / 2;
  params.targetScaleY = 6 * Q16_ONE / 5;
  FieldEffect_Spawn(stump->x.fixed, stump->y.fixed, stump->z.fixed, 0, 0, 0,
                    EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                        EFFECT_SCALE_TO_TARGET,
                    &params);
  Sound_Play(SE_CAST);
  Actor_SetAnimation(stumpId, STUMP_ANIMATION_SPLASH);
  MapGrid_WaitForShakeToSettle();
}

// Shared inventory equipment code stays at its native overlay position.
#include "shared/item_equip.h"

/// Gives Ivan the Shaman's Rod and equips it.
///
/// With his items full, he drops another to make room. Later patches skip
/// items marked only indispensable.
static void GomaCaveEntrance_GiveShamansRod(void) {
  struct Unit *member;
  struct ItemDefinition *item;
  s32 tries;
  s32 i;
  s32 id = ITEM_SHAMANS_ROD;
  member = Unit_Get(IVAN);
  tries = 0;
retry:
  if (++tries > 1000) {
#if TBS_PATCH_3
    for (i = 0; i <= INVENTORY_SLOTS - 1; i++) {
      item = Item_GetDirect(member->inventory[i]);
      if (item->flags != ITEM_INDISPENSABLE) {
        Inventory_Discard(IVAN, i);
        break;
      }
    }
    if (i == INVENTORY_SLOTS)
#endif
      member->inventory[INVENTORY_SLOTS - 1] = 0;
  }
  if (Inventory_AddItem(IVAN, id) == -1) {
    for (i = 0; i <= INVENTORY_SLOTS - 1; i++) {
      item = Item_GetDirect(member->inventory[i]);
      if (item->type == ITEM_TYPE_WEAPON) {
        Inventory_Discard(IVAN, i);
        goto retry;
      }
    }
    for (i = 0; i <= INVENTORY_SLOTS - 1; i++) {
      item = Item_GetDirect(member->inventory[i]);
      if (item->type == ITEM_TYPE_NONE &&
          (item->flags & ITEM_INDISPENSABLE) == 0 &&
          item->useType == ITEM_USE_CONSUMED) {
        Inventory_Discard(IVAN, i);
        goto retry;
      }
    }
    goto retry;
  }
  for (i = 0; i <= INVENTORY_SLOTS - 1; i++) {
    if (member->inventory[i] == id)
      Inventory_Equip(IVAN, i);
  }
}

#include "shared/field_effect_duration_scripts.h"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gGomaCaveEntranceTallIvyScript[] = {
    SC_SET(SW_SCALE_Y, 8 * Q16_ONE / 5),
    SC_END,
};

struct SceneEntrance gGomaCaveEntranceEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 192, 0, 197, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {GOMA_CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 96, 0, 440,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {GOMA_CAVE_ENTRANCE_FROM_GOMA_CAVE_STREAM, CONDITION_ALWAYS, 200, 0, 184,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gGomaCaveEntranceExits[] = {
    SCENE_EXITS(GomaCaveEntrance),
    SCENE_EXIT(GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM, GomaCaveStream,
               GOMA_CAVE_STREAM_FROM_GOMA_CAVE_ENTRANCE),
    SCENE_EXIT(GOMA_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_GOMA_CAVE_ENTRANCE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gGomaCaveEntranceActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOVE_HAND, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOVE_HAND, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, FLAG_GOMA_CAVE_IVAN_JOINED, MOVEMENT_TYPE_RESET_ACTION,
        191, 0, 194, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, (s32)gGomaCaveEntranceTallIvyScript,
        200, 0, 168, FACING_NORTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gGomaCaveEntranceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, GOMA_CAVE_ENTRANCE_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 5, CONDITION_ALWAYS, EVENT(GomaCaveEntrance_PushWestPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 6, CONDITION_ALWAYS, EVENT(GomaCaveEntrance_PushWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_IVAN_JOINED, EVENT(GomaCaveEntrance_IvanRejoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED, EVENT(GomaCaveEntrance_MoveStump) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR, CONDITION_ALWAYS, EVENT(GomaCaveEntrance_MoveWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE, EVENT(GomaCaveEntrance_MoveEastPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, EVENT(GomaCaveEntrance_BlowAwayIvy) },
    { TRIGGER_TABLE_END },
};
// clang-format on
