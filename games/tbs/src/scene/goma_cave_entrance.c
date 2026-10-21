/// @file
/// Goma Cave Entrance (SceneId_GomaCaveEntrance), the mouth of the cave in
/// the Goma Range that leads from Vault to Bilibin, where Ivan rejoins the
/// party.
///
/// Vault's mayor sent the party here: `Look for a cave in the mountains of the
/// Goma Range, northeast of us. Once you've passed it, you'll reach Bilibin.`
/// In Vault, Ivan, Master Hammet's servant, had joined the party to find the
/// Shaman's Rod that thieves stole from Hammet, reading the townspeople's
/// minds; once the thieves were caught he took the rod and set out for Lunpa,
/// where Hammet was said to be heading.
///
/// A stump caught in ivy blocks the way on, and Move on it runs
/// Event_IvanRejoins while FLAG_GOMA_CAVE_IVAN_REJOINED is clear. Garet says
/// their Psynergy is not strong enough to free it. If the thieves in Vault are
/// jailed (FLAG_VAULT_THIEVES_JAILED), Ivan walks up, blows the ivy off with
/// Whirlwind, tells them he could not get into Lunpa and asks to join their
/// quest; he rejoins with the Shaman's Rod equipped, and the flag is set.
/// Otherwise Garet only falls back into line. Once Ivan is back, Move on the
/// stump runs Event_MoveStump instead: when the stump has been moved to the
/// right column it spins, leaps and lands out of the way
/// (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED).
///
/// Two pillars and a second ivy are puzzles on the way. The west pillar is
/// pushed by walking into it or moved with Move, and stands on one of three
/// columns, which two flags record
/// (FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP and _TWO_STEPS). The east
/// pillar, moved with Move to its last column, settles into the floor so it can
/// be walked over (FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE). Whirlwind
/// blows the second ivy away for good (FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY),
/// and its placement row is skipped from then on.
///
/// Scene_Initialize blocks Retreat and puts everything back as these flags say:
/// the stump where it landed, the pillars on their columns and the map cells
/// that go with them. GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM leads into Goma
/// Cave (SceneId_GomaCaveStream) and GOMA_CAVE_ENTRANCE_TO_WORLD_MAP out to the
/// world map.
#include "unit/scene/goma_cave_entrance.h"
#include "version.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "create_configured_overlay_object.h"
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
#include "objcmd.h"
#include "global_cells.h"
#include "ability_ids.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "event.h"
#include "item_ids.h"
#include "vector3.h"

// The room's actors: Garet and Ivan take their party slots, the rest follow
// the placement table from 8.
#define STUMP 8
#define WEST_PILLAR 9
#define EAST_PILLAR 10
#define STUMP_IVY 15 ///< the ivy holding the stump
#define IVY 16

// Private scene actions used by the reunion sequence.
static void GomaCaveEntrance_WhirlwindOnStumpIvy(void);
static void GomaCaveEntrance_LeapStump(s32 id);
static void GomaCaveEntrance_GiveShamansRod(void);

// Shared effects stay at their native positions in the overlay.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Shows the next message over the speaker, then pauses.
static void GomaCaveEntrance_SayThenWait(s32 actorId, s32 frames) {
  MESSAGE_PUT(actorId, 0);
  WAIT(frames);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gGomaCaveEntranceEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gGomaCaveEntranceExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gGomaCaveEntrancePlacements;
}

/// Move on the west pillar at Goma Cave Entrance, and after each push: the
/// pillar stands on one of three columns, and the map cells and flags follow
/// it.
static void Event_MoveWestPillar(void) {
  struct FieldActor *pillar;
  s32 column;

  pillar = Object_GetById(WEST_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  GameFlag_Clear(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP);
  GameFlag_Clear(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS);
  if (column == 15) {
    Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
  } else if (column == 16) {
    Map_CopyCellAttributes(48, 18, 1, 2, column, 18);
    FLAG_SET(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP);
  } else {
    Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    FLAG_SET(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS);
  }
}

/// Move on the east pillar at Goma Cave Entrance: once it reaches column 23 it
/// settles into the floor and can be walked over.
static void Event_MoveEastPillar(void) {
  struct FieldActor *pillar;
  s32 column;

  pillar = Object_GetById(EAST_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  if (column == 23) {
    WAIT(10);
    Object_GetById(EAST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(EAST_PILLAR)->motionFlags = 0;
    Engine_ActorSetSpriteFlags(Object_GetById(EAST_PILLAR), 0);
    Map_CopyCellAttributes(54, 17, 1, 1, column, 17);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE);
  }
}

/// Walking into the west pillar at Goma Cave Entrance from the west (trigger 5)
/// or the east (trigger 6): the leader leans on it and pushes it one step.
static void Event_PushWestPillar(void) {
  struct EventWork *work;
  s32 offsetX;

  work = gEventWork;
  EV_BEGIN();
  OBJ_MODE(ACTOR_PARTY_LEADER, 8);
  WAIT(20);
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  OBJ_SPEED(WEST_PILLAR, PIXELS(1) / 5, PIXELS(1) / 10);
  SE(SE_185);
  // 16 pixels east from trigger 5, west from trigger 6.
  offsetX = (11 - (work->touchedTrigger << 1)) << 4;
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, offsetX, 0);
  Actor_SetDestinationOffset(WEST_PILLAR, offsetX, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorWaitForMove(WEST_PILLAR);
  WAIT(20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_MoveWestPillar();
  BattleFx_PlayQueuedSound();
  EV_END();
}

/// Unused: an empty function nothing calls.
UNUSED static void GomaCaveEntrance_NoOpA(void) {}

/// Unused: a second empty function nothing calls.
UNUSED static void GomaCaveEntrance_NoOpB(void) {}

/// Whirlwind on the second ivy at Goma Cave Entrance: it is blown away for
/// good.
static void Event_BlowAwayIvy(void) {
  EV_BEGIN();
  Engine_GameFlagSet(FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY);
  EV_END();
}

const struct SceneEvent *Scene_GetEvents(void) {
  return gGomaCaveEntranceEvents;
}

/// Sets up Goma Cave Entrance: Retreat is blocked, and the stump and pillars
/// stand where the party left them, the stump's ivy gone once Ivan has blown
/// it off.
s32 Scene_Initialize(void) {
  struct FieldActor *movedObject;
  s32 v5;

  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  WAIT(10);
  BattleFx_SetQueuedSoundAndPlay(SE_170);
  OBJ_MODE(11, 2);
  Object_GetById(11)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  {
    struct FieldActor *record = Object_GetById(STUMP);

    record->collisionFlags = 16 | record->collisionFlags;
  }
  {
    struct FieldActor *record = Object_GetById(STUMP_IVY);

    record->collisionFlags = 8 | record->collisionFlags;
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_IVAN_REJOINED) != 0) {
    Map_CopyCellAttributes(74, 11, 1, 1, 73, 11);
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED) != 0) {
    Engine_ActorSetPosition(STUMP, PIXELS(136), PIXELS(196));
    Object_GetById(STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    v5 = 12;
    OBJ_MODE(STUMP, 2);
    Map_CopyCellAttributes(39, 12, 3, 1, 8, v5);
    Map_CopyCellAttributes(43, 11, 3, 1, v5, 11);
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP) != 0) {
    OBJ_POS_SET(WEST_PILLAR, 264, 312);
    Map_CopyCellAttributes(48, 18, 1, 2, 16, 18);
  } else {
    if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS) !=
        0) {
      OBJ_POS_SET(WEST_PILLAR, 280, 312);
      Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    }
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE) != 0) {
    Engine_ActorSetPosition(EAST_PILLAR, PIXELS(376), PIXELS(280));
    Object_GetById(EAST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    v5 = 0;
    Object_GetById(EAST_PILLAR)->motionFlags = v5;
    movedObject = Object_GetById(EAST_PILLAR);
    Engine_ActorSetSpriteFlags(movedObject, 0);
    Map_CopyCellAttributes(54, 17, 1, 1, 23, 17);
  }
  return 0;
}

/// Move on the stump at Goma Cave Entrance while the ivy holds it: Garet says
/// their Psynergy is not strong enough. Once the thieves in Vault are jailed
/// (FLAG_VAULT_THIEVES_JAILED), Ivan turns up, blows the ivy away with
/// Whirlwind, says he could not get into Lunpa and asks to join the party; he
/// rejoins with the Shaman's Rod equipped (FLAG_GOMA_CAVE_IVAN_REJOINED).
static void Event_IvanRejoins(void) {
  struct FieldActor *leader;

#if TBS_PATCH_1
  BattleFx_PlayQueuedSound();
#endif
  EV_BEGIN();
  WAIT(30);
  MESSAGE_SET(MsgGomaNoUsePsynergy);
  OBJ_SPEED(ACTOR_PARTY_LEADER, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_SPEED(GARET, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkToAndWait(GARET, 264, 168);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 20);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(20);
  if (FLAG_CHECK(FLAG_VAULT_THIEVES_JAILED) == 0) {
    // Garet falls back into line.
    OBJ_MODE(GARET, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(GARET);
    OBJ_POS_SET(GARET, 0, 0);
    EV_END();
  } else {
    // Ivan comes up from the south: "You want to move the stump, right?"
    OBJ_POS_SET(IVAN, 360, 248);
    OBJ_SPEED(IVAN, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
    Actor_WalkToAndWait(IVAN, 272, 248);
    Actor_WalkToAndWait(IVAN, 272, 208);
    OBJ_DIR(IVAN, FACING_NORTHWEST, 20);
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    WAIT(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    OBJ_DIR(GARET, FACING_SOUTH, 0);
    WAIT(20);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT, 60);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Actor_WalkToAndWait(IVAN, 264, 200);
    OBJ_WALK(ACTOR_PARTY_LEADER, 248, 168);
    Actor_WalkToAndWait(IVAN, 248, 184);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
    Actor_WalkToAndWait(IVAN, 232, 184);
    WAIT(20);
    OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 5, 60);
    OBJ_DIR(IVAN, FACING_NORTHEAST, 20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    WAIT(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    WAIT(20);
    OBJ_DIR(IVAN, FACING_WEST, 20);
    Event_ShowMessageAndWait(IVAN, 0, 120);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 5, 60);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    WAIT(60);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
    WAIT(60);
    OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 6, 0);
    Engine_ActorStartRepeatedMotion(IVAN, 1);
    WAIT(30);
    Event_ShowMessageAndWait(IVAN, 0, 30);
    OBJ_DIR(IVAN, FACING_NORTHEAST, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    WAIT(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    WAIT(20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(20);
    OBJ_DIR(IVAN, FACING_WEST, 20);
    // "I have a great idea. Leave it to me!": Whirlwind.
    GomaCaveEntrance_WhirlwindOnStumpIvy();
    OBJ_MODE(IVAN, ANIM_STAND);
    WAIT(20);
#if TBS_PATCH_1
    BattleFx_PlayQueuedSound();
#endif
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT, 60);
    Engine_ActorJump(GARET, 2, 0);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(GARET, 20);
    Actor_WalkToAndWait(IVAN, 264, 184);
    WAIT(10);
    Actor_FaceActor(IVAN, GARET, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    WAIT(20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 60);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 5, 60);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 1, 0);
    WAIT(60);
    OBJ_DIR(GARET, FACING_WEST, 0);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    WAIT(60);
    OBJ_DIR(GARET, FACING_SOUTH, 0);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    WAIT(10);
    GomaCaveEntrance_SayThenWait(GARET, 20);
    OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 5, 0);
    WAIT(60);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 2, 0);
    WAIT(60);
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 30);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 1, 0);
    WAIT(80);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GARET, 1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    WAIT(60);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    OBJ_DIR(GARET, FACING_WEST, 0);
    WAIT(80);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    WAIT(30);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 30);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    WAIT(20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 40);
    OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    WAIT(20);
    // "Please, allow me to join your quest.": Ivan rejoins with his rod.
    Event_PrepareObjectAndApplyValue(IVAN, 1);
    WAIT(60);
    GomaCaveEntrance_GiveShamansRod();
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    WAIT(20);
    Actor_WalkToAndWait(IVAN, 248, 184);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    OBJ_DIR(GARET, FACING_WEST, 0);
    WAIT(120);
    GomaCaveEntrance_SayThenWait(IVAN, 30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
    WAIT(20);
    OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
    OBJ_MODE(GARET, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(50);
    OBJ_SPEED(GARET, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
    OBJ_SPEED(IVAN, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
    OBJ_WALK(GARET, 248, 168);
    Actor_WalkToAndWait(IVAN, 248, 168);
    OBJ_POS_SET(IVAN, 0, 0);
    Engine_ActorWaitForMove(GARET);
    OBJ_POS_SET(GARET, 0, 0);
    Map_CopyCellAttributes(74, 11, 1, 1, 73, 11);
    FLAG_SET(FLAG_GOMA_CAVE_IVAN_REJOINED);
    EV_END();
  }
}

/// Move on the freed stump at Goma Cave Entrance: from column 11 it spins,
/// leaps away and lies where it lands (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED).
static void Event_MoveStump(void) {
  s32 column;
  struct FieldActor *stump;

  EV_BEGIN();
  column = Object_GetById(STUMP)->x.fixed >> 20;
  if (column == 11) {
    GomaCaveEntrance_LeapStump(STUMP);
    stump = Object_GetById(STUMP);
    stump->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Map_CopyCellAttributes(39, 12, 3, 1, 8, 12);
    Map_CopyCellAttributes(43, 11, 3, 1, 12, column);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED);
  }
  EV_END();
}

/// Ivan casts Whirlwind on the stump's ivy, and the scene waits for it.
static void GomaCaveEntrance_WhirlwindOnStumpIvy(void) {
  u8 *effectWork;

  effectWork = gEffectWork;
  BattleFx_LoadActionEffectResources(ABILITY_WHIRLWIND, 1);
  BattleFx_SetupObjectPair(IVAN, STUMP_IVY);
  {
    u8 *f = (u8 *)((s32)effectWork + 0x71c);
    s32 t = 8;

    t |= *f;
    *f = t;
  }
  EventObject_Initialize();
  Field_DispatchTypeHandler(PSYNERGY_STEP_ON_TARGET);
  EffectRuntime_StopCurrentObject();
}

/// The leaping stump's update hook turns its sprite a fixed amount per frame.
static void GomaCaveEntrance_SpinStump(union FieldObject *fieldObject) {
  struct FieldActor *obj = &fieldObject->actor;
  struct AnimationObject *state = obj->sprite;
  s32 v = state->rotation - 0x800;

  state->rotation = v;
}

// Shared height-wait code stays at its native position in the overlay.
#include "common/object_waitdown.h"

/// The stump spins in place, leaps and lands with a shake and a ring of dust,
/// then comes to rest a little further on.
static void GomaCaveEntrance_LeapStump(s32 id) {
  struct Vector3 dir;
  struct EffectOptions params;
  struct Vector3 *v;
  struct EffectOptions *p;
  struct FieldActor *actor;
  u32 i;

  actor = Object_GetById(id);
  actor->motionFlags = 0;
  for (i = 0; i <= 17; i++) {
    Engine_TaskWait(1);
    // The corresponding signed halfword view retains the negative literal.
    *(s16 *)&actor->sprite->rotation += -256;
    actor->x.fixed -= Engine_MathCos(actor->sprite->rotation) / 2;
    actor->targetX = ACTOR_NO_TARGET;
  }
  actor->update = GomaCaveEntrance_SpinStump;
  OBJ_SPEED(id, PIXELS(3), 3 * PIXELS(1) / 2);
  Engine_ActorSetDestination(id, 160, 192);
  actor->gravity = 0xcccc;
  actor->motionFlags = 3;
  actor->terrainId = 0;
  Engine_ActorWaitForMove(id);
  SceneActor_WaitObjectBelowHeight(actor, 0x200000);
  Engine_AudioPlayCue(SE_188);
  Map_SetShake(0x50000, 0x50000, 0x10000);
  Engine_AudioPlayCue(SE_141);
  Map_SetShake(-1, -1, 0xe666);
  for (i = 0; i <= 16; i++) {
    v = &dir;
    SET_POSITION(v, Engine_MathCos(i << 12), 0, Engine_MathSin(i << 12));
    v->x -= v->x / 4;
    v->z -= v->z / 2;
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, v->x, v->y,
                 v->z, 0, NULL);
  }
  actor->velocityY = 0x50000;
  Engine_ActorSetDestination(id, 139, 196);
  Engine_ActorWaitForMove(id);
  SceneActor_WaitObjectBelowHeight(actor, 0x200000);
  // UB: the native integer store aliases the update pointer. A typed clear
  // swaps this store with the following sprite-pointer load.
#ifdef UBFIX
  actor->update = NULL;
#else
  *(s32 *)&actor->update = 0;
#endif
  actor->sprite->rotation = 0x1000;
  p = &params;
  p->type = 214;
  p->startScaleX = 0x8000;
  p->startScaleY = 0xcccc;
  p->targetScaleX = 0x18000;
  p->targetScaleY = 0x13333;
  Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0,
               0x1c0000, p);
  Engine_AudioPlayCue(SE_CAST);
  OBJ_MODE(id, 3);
  Engine_MapWaitWorkValuesBelow256();
}

// Shared inventory equipment code stays at its native overlay position.
#include "common/item_equip.h"

/// Gives Ivan the Shaman's Rod and equips it. With his items full, he drops
/// another to make room; later patches skip items marked only indispensable.
static void GomaCaveEntrance_GiveShamansRod(void) {
  struct BattleUnit *member;
  struct ItemDefinition *item;
  s32 tries;
  s32 i;
  s32 id;

  id = ITEM_SHAMANS_ROD;
  member = UnitGet(IVAN);
  tries = 0;
retry:
  if (++tries > 1000) {
#if TBS_PATCH_3
    for (i = 0; i <= INVENTORY_SLOTS - 1; i++) {
      item = Item_Get(member->inventory[i]);
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
      item = Item_Get(member->inventory[i]);
      if (item->type == ITEM_TYPE_WEAPON) {
        Inventory_Discard(IVAN, i);
        goto retry;
      }
    }
    for (i = 0; i <= INVENTORY_SLOTS - 1; i++) {
      item = Item_Get(member->inventory[i]);
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

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
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
struct ScenePlacement gGomaCaveEntrancePlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STUMP, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STUMP, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd7, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xd7, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STUMP, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, FLAG_GOMA_CAVE_IVAN_REJOINED, 7,
        PIXELS(191), 0, PIXELS(194), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xf8, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, (s32)gGomaCaveEntranceTallIvyScript,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gGomaCaveEntranceEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, GOMA_CAVE_ENTRANCE_TO_WORLD_MAP },
    { TOUCH_WALKING_INTO(FACING_EAST), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushWestPillar) },
    { TOUCH_WALKING_INTO(FACING_WEST), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), STUMP, FLAG_GOMA_CAVE_IVAN_REJOINED, EVENT_SCRIPT(Event_IvanRejoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), STUMP, FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED, EVENT_SCRIPT(Event_MoveStump) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), WEST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), EAST_PILLAR, FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE, EVENT_SCRIPT(Event_MoveEastPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), IVY, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, EVENT_SCRIPT(Event_BlowAwayIvy) },
    { SCENE_EVENTS_END },
};
// clang-format on
