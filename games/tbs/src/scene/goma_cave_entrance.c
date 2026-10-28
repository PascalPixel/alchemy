/// @file
/// Goma Cave Entrance (SceneId_GomaCaveEntrance), the mouth of the cave in
/// the Goma Range between Vault and Bilibin, where Ivan rejoins the party.
///
/// A stump caught in ivy (ACTOR_STUMP, held by ACTOR_STUMP_IVY) blocks the
/// way on. Until FLAG_GOMA_CAVE_IVAN_JOINED is set, Move on it runs
/// GomaCaveEntrance_IvanRejoins: Garet says their Psynergy can't free it, and
/// only once the thieves in Vault are jailed (FLAG_VAULT_THIEVES_JAILED) does
/// Ivan walk up, blow the ivy off with Whirlwind and rejoin with the Shaman's
/// Rod equipped. The ivy is never the player's target (ACTOR_NOT_TARGETED), so
/// that event is the only way past it. After it, Move runs
/// GomaCaveEntrance_MoveStump, which sends the stump leaping out of the way
/// once it stands on column 11: it lands in the pool under the waterfall and
/// sinks until only its top shows, a stepping stone beside ACTOR_SUNKEN_STUMP
/// (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED; its render,
/// places/goma-cave-entrance_goma-cave-entrance-stump-moved/map-0.png).
///
/// The two stone pillars and a second ivy are puzzles on the way. The west
/// pillar is pushed by walking into it or moved with Move, and stands on one of
/// three columns, which two flags record
/// (FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP and _TWO_STEPS). The east
/// pillar, moved with Move to its last column, settles into the floor so it can
/// be walked over (FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE). Whirlwind
/// blows the second ivy away for good (FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY);
/// its actor placement row is then skipped, though it keeps its actor id.
///
/// Scene_Initialize blocks Retreat and puts everything back as these flags say:
/// the stump where it landed, the pillars on their columns and the map grid
/// cells that go with them. GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM leads into
/// Goma Cave (SceneId_GomaCaveStream) and GOMA_CAVE_ENTRANCE_TO_WORLD_MAP out
/// to the World Map.
#include "module/scene/goma_cave_entrance.h"
#include "version.h"
#include "module/scene/shared/effect_spawn.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
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
#include "minigame.h"
#include "sound.h"
#include "edition.h"
#include "call.h"
#include "battle_system.h"
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
#include "fx_work.h"
#include "fixed_math.h"

/// The room's actors. Garet's and Ivan's rows go to their party slots,
/// and the rows after them number from ACTOR_FIRST_PLACED.
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
  /// outside the camera's bounds. Nothing in the overlay addresses them, and
  /// Move spawns its own hand (FieldEffect_StartItemBreak), so the player never
  /// sees them.
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
#include "shared/effect_create.h"
#include "shared/effect_spawn.h"

/// Shows the next message over the speaker, then pauses.
static void GomaCaveEntrance_SayThenWait(s32 actorId, s32 frames) {
  Event_ShowMessageAndWait(actorId, 0);
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
/// pillar stands on one of three columns, and the map grid cells and flags
/// follow it.
static void GomaCaveEntrance_MoveWestPillar(void) {
  struct Actor *pillar;
  s32 column;

  pillar = Actor_GetById(ACTOR_WEST_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
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

  pillar = Actor_GetById(ACTOR_EAST_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  if (column == 23) {
    Event_Wait(10);
    Actor_GetById(ACTOR_EAST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_GetById(ACTOR_EAST_PILLAR)->motionFlags = 0;
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_EAST_PILLAR), 0);
    MapGrid_CopyCellAttributes(54, 17, 1, 1, column, 17);
    GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE);
  }
}

/// Walking into the west pillar at Goma Cave Entrance from the west (
/// step trigger 5) or the east (step trigger 6): the leader leans on it and
/// pushes it one step.
static void GomaCaveEntrance_PushWestPillar(void) {
  struct FieldWork *work;
  s32 offsetX;

  work = gFieldWork;
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_SetSpeed(ACTOR_WEST_PILLAR, PIXELS(1) / 5, PIXELS(1) / 10);
  Event_PlaySound(SE_PUSH);
  // 16 pixels east from step trigger 5, west from step trigger 6.
  offsetX = (11 - (work->stepTrigger << 1)) << 4;
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, offsetX, 0);
  Actor_SetDestinationOffset(ACTOR_WEST_PILLAR, offsetX, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WaitForMoveById(ACTOR_WEST_PILLAR);
  Event_Wait(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  GomaCaveEntrance_MoveWestPillar();
  Scene_PlayQueuedMusic();
  Event_End();
}

UNUSED static void GomaCaveEntrance_NoOpA(void) {}

/// Unused: a second empty function nothing calls.
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

/// Sets up Goma Cave Entrance: Retreat is blocked, and the stump and pillars
/// stand where the party left them, the stump's ivy gone once Ivan has blown
/// it off.
s32 Scene_Initialize(void) {
  struct Actor *movedActor;

  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  Event_Wait(10);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Import_ActorSetAnimation(ACTOR_SUNKEN_STUMP, STUMP_ANIMATION_SINK);
  Actor_GetById(ACTOR_SUNKEN_STUMP)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  {
    struct Actor *record = Actor_GetById(ACTOR_STUMP);

    record->collisionFlags = ACTOR_NEAR_PSYNERGY | record->collisionFlags;
  }
  // The player never targets the stump's ivy; only Ivan's Whirlwind in
  // GomaCaveEntrance_IvanRejoins removes it.
  {
    struct Actor *record = Actor_GetById(ACTOR_STUMP_IVY);

    record->collisionFlags = ACTOR_NOT_TARGETED | record->collisionFlags;
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_IVAN_JOINED) != 0) {
    MapGrid_CopyCellAttributes(74, 11, 1, 1, 73, 11);
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED) != 0) {
    Import_ActorSetPosition(ACTOR_STUMP, PIXELS(136), PIXELS(196));
    Actor_GetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Import_ActorSetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
    MapGrid_CopyCellAttributes(39, 12, 3, 1, 8, 12);
    MapGrid_CopyCellAttributes(43, 11, 3, 1, 12, 11);
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP) != 0) {
    Actor_SetPosition(ACTOR_WEST_PILLAR, PIXELS(264), PIXELS(312));
    MapGrid_CopyCellAttributes(48, 18, 1, 2, 16, 18);
  } else {
    if (GameFlag_Get(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS) != 0) {
      Actor_SetPosition(ACTOR_WEST_PILLAR, PIXELS(280), PIXELS(312));
      MapGrid_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    }
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE) != 0) {
    Import_ActorSetPosition(ACTOR_EAST_PILLAR, PIXELS(376), PIXELS(280));
    Actor_GetById(ACTOR_EAST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_GetById(ACTOR_EAST_PILLAR)->motionFlags = 0;
    movedActor = Actor_GetById(ACTOR_EAST_PILLAR);
    Actor_SetSpriteFlags(movedActor, 0);
    MapGrid_CopyCellAttributes(54, 17, 1, 1, 23, 17);
  }
  return 0;
}

/// Move on the stump at Goma Cave Entrance while the ivy holds it: Garet says
/// their Psynergy is not strong enough. Once the thieves in Vault are jailed
/// (FLAG_VAULT_THIEVES_JAILED), Ivan turns up, blows the ivy away with
/// Whirlwind, says he could not get into Lunpa and asks to join the party; he
/// rejoins with the Shaman's Rod equipped (FLAG_GOMA_CAVE_IVAN_JOINED).
static void GomaCaveEntrance_IvanRejoins(void) {
  struct Actor *leader;

#if TBS_PATCH_1
  Scene_PlayQueuedMusic();
#endif
  Event_Start();
  Event_Wait(30);
  Event_SetMessageId(MsgGomaNoUsePsynergy);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_SetSpeed(ACTOR_GARET, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkToAndWait(ACTOR_GARET, 264, 168);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    // Garet falls back into line.
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, PIXELS(0), PIXELS(0));
    Event_End();
  } else {
    // Ivan comes up from the south: "You want to move the stump, right?"
    Actor_SetPosition(ACTOR_IVAN, PIXELS(360), PIXELS(248));
    Actor_SetSpeed(ACTOR_IVAN, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
    Actor_WalkToAndWait(ACTOR_IVAN, 272, 248);
    Actor_WalkToAndWait(ACTOR_IVAN, 272, 208);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
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
    FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 120);
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
    FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 30);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    // "I have a great idea. Leave it to me!": Whirlwind.
    GomaCaveEntrance_WhirlwindOnStumpIvy();
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
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
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    // "Please, allow me to join your quest.": Ivan rejoins with his rod.
    Event_PrepareActorAndApplyValue(IVAN, 1);
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
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(50);
    Actor_SetSpeed(ACTOR_GARET, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
    Actor_SetSpeed(ACTOR_IVAN, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
    Actor_WalkTo(ACTOR_GARET, 248, 168);
    Actor_WalkToAndWait(ACTOR_IVAN, 248, 168);
    Actor_SetPosition(ACTOR_IVAN, PIXELS(0), PIXELS(0));
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, PIXELS(0), PIXELS(0));
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
  column = Actor_GetById(ACTOR_STUMP)->x.fixed >> 20;
  if (column == 11) {
    GomaCaveEntrance_LeapStump(ACTOR_STUMP);
    stump = Actor_GetById(ACTOR_STUMP);
    stump->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(39, 12, 3, 1, 8, 12);
    MapGrid_CopyCellAttributes(43, 11, 3, 1, 12, column);
    GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED);
  }
  Event_End();
}

/// Ivan casts Whirlwind on the stump's ivy, and the event waits for it. The
/// announcement is lowered to a fixed place (FIELD_MESSAGE_LOWER), since
/// it would otherwise be placed by where the leader, not Ivan, stands.
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
static void GomaCaveEntrance_SpinStump(struct Actor *updated) {
  struct Actor *actor = updated;
  struct Sprite *state = actor->sprite;
  s32 v = state->rotation - 0x800;

  state->rotation = v;
}

// Shared height-wait code stays at its native position in the overlay.
#include "shared/actor_waitdown.h"

/// The stump spins in place, leaps and lands with a shake and a ring of dust,
/// then hops on into the pool under the waterfall with a splash and sinks
/// (STUMP_ANIMATION_SPLASH).
static void GomaCaveEntrance_LeapStump(s32 stumpId) {
  struct Vector3 dir;
  struct EffectOptions params;
  struct Vector3 *v;
  struct EffectOptions *p;
  struct Actor *stump;
  u32 i;

  stump = Actor_GetById(stumpId);
  stump->motionFlags = 0;
  for (i = 0; i <= 17; i++) {
    wait(1);
    // The corresponding signed halfword view retains the negative literal.
    *(s16 *)&stump->sprite->rotation += -256;
    stump->x.fixed -= Trig_Cos(stump->sprite->rotation) / 2;
    stump->targetX = ACTOR_NO_TARGET;
  }
  stump->update = GomaCaveEntrance_SpinStump;
  Actor_SetSpeed(stumpId, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_ResetAndSetPosition(stumpId, 160, 192);
  stump->gravity = 4 * PIXELS(1) / 5;
  stump->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  stump->layer = 0;
  Actor_WaitForMoveById(stumpId);
  Actor_WaitBelowHeight(stump, PIXELS(32));
  Sound_Play(SE_THUMP);
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  for (i = 0; i <= 16; i++) {
    v = &dir;
    SET_POSITION(v, Trig_Cos(i << 12), 0, Trig_Sin(i << 12));
    v->x -= v->x / 4;
    v->z -= v->z / 2;
    FieldEffect_Spawn(stump->x.fixed, stump->y.fixed, stump->z.fixed, v->x,
                      v->y, v->z, 0, NULL);
  }
  stump->velocityY = PIXELS(5);
  Actor_ResetAndSetPosition(stumpId, 139, 196);
  Actor_WaitForMoveById(stumpId);
  Actor_WaitBelowHeight(stump, PIXELS(32));
  // UB: the native integer store aliases the update pointer. A typed clear
  // swaps this store with the following sprite-pointer load.
#ifdef UBFIX
  stump->update = NULL;
#else
  *(s32 *)&stump->update = 0;
#endif
  stump->sprite->rotation = 0x1000;
  p = &params;
  p->type = SPRITE_SPLASH;
  p->startScaleX = Q16_ONE / 2;
  p->startScaleY = 4 * Q16_ONE / 5;
  p->targetScaleX = 3 * Q16_ONE / 2;
  p->targetScaleY = 6 * Q16_ONE / 5;
  FieldEffect_Spawn(
      stump->x.fixed, stump->y.fixed, stump->z.fixed, 0, 0, 0,
      EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET, p);
  Sound_Play(SE_CAST);
  Import_ActorSetAnimation(stumpId, STUMP_ANIMATION_SPLASH);
  MapGrid_WaitForShakeToSettle();
}

// Shared inventory equipment code stays at its native overlay position.
#include "shared/item_equip.h"

/// Gives Ivan the Shaman's Rod and equips it. With his items full, he drops
/// another to make room; later patches skip items marked only indispensable.
static void GomaCaveEntrance_GiveShamansRod(void) {
  struct Unit *member;
  struct ItemDefinition *item;
  s32 tries;
  s32 i;
  s32 id;

  id = ITEM_SHAMANS_ROD;
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

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gGomaCaveEntranceTallIvyScript[] = {
    SC_SET(SW_SCALE_Y, 0x19999),
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
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MOVE_HAND, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_MOVE_HAND, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_IVY, FLAG_GOMA_CAVE_IVAN_JOINED, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(191), 0, PIXELS(194), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_IVY, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, (s32)gGomaCaveEntranceTallIvyScript,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH, TALK_FACING_PARTY, 0 },
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
    { TRIGGERS_END },
};
// clang-format on
