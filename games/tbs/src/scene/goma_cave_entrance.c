/// @file
/// Goma Cave Entrance (SceneId_GomaCaveEntrance), the foot of the cliffs in
/// the Goma Range between Vault and Bilibin, where Ivan joins the party. The
/// cave's mouth is high on a column of rock beside a waterfall, hidden behind
/// tall ivy, with a stump caught in ivy in front of it.
///
/// The party comes in from the World Map at the bottom of the map, crosses the
/// river on a stump sunk in it (ACTOR_SUNKEN_STUMP) and climbs the cliffs. On
/// the way a stone pillar stands by a ladder at the foot of a cliff: walked
/// into or moved with Move, it slides a step at a time, blocking the ladder
/// while it stands in front of it (FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP
/// and _TWO_STEPS). Higher up, Move pulls the second pillar off its ledge
/// into the gap beside it, where its top makes a bridge
/// (FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE).
///
/// On the column by the cave, Move on the ivy-bound stump makes Garet say
/// their Psynergy can't free it. Once the thieves in Vault are jailed
/// (FLAG_VAULT_THIEVES_JAILED), Ivan comes up instead, blows the ivy off with
/// Whirlwind and joins the party with the Shaman's Rod
/// (FLAG_GOMA_CAVE_IVAN_JOINED). Move then sends the stump leaping off the
/// column into the pool under the waterfall, where it sinks until only its top
/// shows and can be jumped across (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED).
/// Whirlwind on the tall ivy blows it away for good and opens the cave's mouth
/// (FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY).
///
/// Retreat does not work here. GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM leads
/// into Goma Cave (SceneId_GomaCaveStream) and GOMA_CAVE_ENTRANCE_TO_WORLD_MAP
/// out to the World Map.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/create_configured_actor.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_psynergy_state.h"
#include "field/map_grid.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "game/inventory.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/goma_cave.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern struct SceneEntrance gGomaCaveEntranceEntrances[];
extern u32 gGomaCaveEntranceExits[];
extern struct ActorPlacement gGomaCaveEntranceActorPlacements[];
extern struct Trigger gGomaCaveEntranceTriggers[];

/// The room's actors.
///
/// Garet's and Ivan's rows go to their party slots, and the rows after them
/// number from ACTOR_FIRST_PLACED.
enum GomaCaveEntranceActorId {
  /// The stump caught in ivy in front of the cave's mouth.
  ACTOR_STUMP = ACTOR_FIRST_PLACED,
  /// The pillar by the ladder at the foot of a cliff.
  ACTOR_WEST_PILLAR,
  /// The pillar on the ledge that Move pulls into the gap beside it.
  ACTOR_EAST_PILLAR,
  /// A second stump, sunk in the river until only its top shows, that the
  /// party crosses on.
  ACTOR_SUNKEN_STUMP,
  /// Unused: two Move hands and a third stump, placed at the map's origin
  /// outside the camera's bounds.
  ///
  /// Nothing in the overlay addresses them, and Move spawns its own hand
  /// (FieldEffect_SpawnMoveHand), so the player never sees them.
  ACTOR_UNUSED_FIRST_MOVE_HAND,
  ACTOR_UNUSED_SECOND_MOVE_HAND,
  ACTOR_UNUSED_STUMP,
  /// The ivy holding the stump, until Ivan blows it away.
  ACTOR_STUMP_IVY,
  /// The tall ivy over the cave's mouth.
  ACTOR_IVY
};

static void GomaCaveEntrance_WhirlwindOnStumpIvy(void);
static void Scene_LeapStump(s32 stumpId);
static void GomaCaveEntrance_GiveShamansRod(void);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

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

/// Isaac casts Move on the pillar by the ladder at the foot of the cliff, and
/// each push of it ends here: in front of the ladder, cell (16, 18), it blocks
/// the climb, and to either side of it the ladder climbs the cliff again.
///
/// The ladder's two cells are copied from a spare stretch of the map: climbing
/// cells, or flat ground while the pillar stands there. The flags keep the
/// pillar's place for the next visit.
static void GomaCaveEntrance_MoveWestPillar(void) {
  s32 column;

  column = Actor_Get(ACTOR_WEST_PILLAR)->x.fixed / toFIXED(16);
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

/// Isaac casts Move on the pillar on the ledge across the gap: pulled west
/// into the gap, cell (23, 17), it drops in until its top is level with the
/// ground, and Isaac can walk across it.
static void GomaCaveEntrance_MoveEastPillar(void) {
  s32 column;

  column = Actor_Get(ACTOR_EAST_PILLAR)->x.fixed / toFIXED(16);
  if (column == 23) {
    Event_Wait(10);
    Actor_Get(ACTOR_EAST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_Get(ACTOR_EAST_PILLAR)->motionFlags = 0;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_EAST_PILLAR), 0);
    MapGrid_CopyCellAttributes(54, 17, 1, 1, column, 17);
    GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE);
  }
}

/// The cells beside the pillar by the ladder from which walking into it
/// pushes it east, from its west side, and west, from its east side.
enum { TRIGGER_PUSH_WEST_PILLAR_EAST = 5, TRIGGER_PUSH_WEST_PILLAR_WEST = 6 };

/// Isaac walks into the pillar by the ladder from the west or the east, leans
/// on it and pushes it one step.
static void GomaCaveEntrance_PushWestPillar(void) {
  struct FieldState *state;
  s32 offsetX;

  state = gFieldState;
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
  Actor_SetSpeed(ACTOR_WEST_PILLAR, 0.2);
  Sound_Play(SE_PUSH);
  // A cell east from TRIGGER_PUSH_WEST_PILLAR_EAST, west from
  // TRIGGER_PUSH_WEST_PILLAR_WEST.
  offsetX = TRIGGER_PUSH_WEST_PILLAR_EAST + TRIGGER_PUSH_WEST_PILLAR_WEST -
            state->stepTrigger * 2;
  offsetX *= MAPGRID_CELL_PIXELS;
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

/// Isaac casts Whirlwind on the tall ivy over the cave's mouth: it is blown
/// away for good, and the way into the cave is open.
static void GomaCaveEntrance_BlowAwayIvy(void) {
  Event_Start();
  GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  return gGomaCaveEntranceTriggers;
}

/// Sets up Goma Cave Entrance as the party arrives: Retreat is barred, the
/// waterfall's sound plays, and the stump, the pillars and the cells that go
/// with them are where the party left them.
s32 Scene_Initialize(void) {
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  Event_Wait(10);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Actor_SetAnimation(ACTOR_SUNKEN_STUMP, STUMP_ANIMATION_SINK);
  Actor_Get(ACTOR_SUNKEN_STUMP)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  {
    struct Actor *stump = Actor_Get(ACTOR_STUMP);

    stump->collisionFlags = ACTOR_NEAR_PSYNERGY | stump->collisionFlags;
  }
  // The player never targets the stump's ivy; only Ivan's Whirlwind in
  // GomaCaveEntrance_IvanJoins removes it.
  {
    struct Actor *ivy = Actor_Get(ACTOR_STUMP_IVY);

    ivy->collisionFlags = ACTOR_NOT_TARGETED | ivy->collisionFlags;
  }
  // The cell GomaCaveEntrance_IvanJoins copies once the stump's ivy is gone,
  // copied again on each arrival.
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
    // copied again on each arrival.
    if (GameFlag_Get(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS) != 0) {
      Actor_SetPosition(ACTOR_WEST_PILLAR, 280, 312);
      MapGrid_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    }
  }
  if (GameFlag_Get(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE) != 0) {
    Actor_SetPosition(ACTOR_EAST_PILLAR, 376, 280);
    Actor_Get(ACTOR_EAST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_Get(ACTOR_EAST_PILLAR)->motionFlags = 0;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_EAST_PILLAR), 0);
    MapGrid_CopyCellAttributes(54, 17, 1, 1, 23, 17);
  }
  return 0;
}

/// Isaac casts Move on the ivy-bound stump in front of the cave's mouth. Garet
/// shakes his head: "It's no use. Our Psynergy isn't strong enough to free
/// that stump." Once the thieves in Vault are jailed, Ivan comes up from
/// the east instead, blows the ivy off with Whirlwind and joins the party.
static void GomaCaveEntrance_IvanJoins(void) {
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
  if (leader != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkToAndWait(ACTOR_GARET, 264, 168);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "It's no use. Our Psynergy isn't strong enough to free that stump."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    // Garet falls back into line.
    Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetDestination(ACTOR_GARET, leader->x.part[0].integer,
                           leader->z.part[0].integer);
    }
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
    Event_End();
  } else {
    // Ivan walks up from the east.
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
    // He blows the ivy off the stump with Whirlwind.
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
    // "All right, Ivan!"
    GomaCaveEntrance_SayThenWait(ACTOR_GARET, 20);
    Actor_WalkToAndWait(ACTOR_IVAN, 264, 184);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    // "I did it!"
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
    // "...Thanks for the help. But why are you here, Ivan?"
    GomaCaveEntrance_SayThenWait(ACTOR_GARET, 20);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "I couldn't get into Lunpa..."
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    // "Don't get me wrong! I didn't come here to ask for your help."
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    // "Your quest has been on my mind ever since I left Vault."
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
    // "Remember? I read everything that happened in your minds."
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Event_Wait(80);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    // "I couldn't just leave, not with all these terrible things happening."
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 30);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "If I can't rescue Master Hammet, then I want to help you..."
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    // "Please, allow me to join your quest."
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    // Ivan joins the party, and his Shaman's Rod with him.
    Event_AddPartyMember(IVAN, 1);
    Event_Wait(60);
    GomaCaveEntrance_GiveShamansRod();
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_IVAN, 248, 184);
    Event_Wait(20);
    // "Are you trying to get to Bilibin through this cave?"
    GomaCaveEntrance_SayThenWait(ACTOR_IVAN, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Event_Wait(120);
    // "Then let's go!"
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

/// Isaac casts Move on the freed stump in front of the cave's mouth: pushed
/// west to the column's edge, it tips, leaps spinning into the pool under the
/// waterfall with a crash and a splash, and sinks until only its top shows
/// (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED).
///
/// The cells by the pool change so that Isaac can jump across on it.
static void GomaCaveEntrance_MoveStump(void) {
  s32 column;

  Event_Start();
  column = FIELD_CELL(Actor_Get(ACTOR_STUMP)->x.fixed);
  if (column == 11) {
    Scene_LeapStump(ACTOR_STUMP);
    Actor_Get(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(39, 12, 3, 1, 8, 12);
    MapGrid_CopyCellAttributes(43, 11, 3, 1, 12, 11);
    GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED);
  }
  Event_End();
}

/// Ivan casts Whirlwind on the stump's ivy, and the event waits for it.
static void GomaCaveEntrance_WhirlwindOnStumpIvy(void) {
  struct FieldEffectState *effectState;
  u8 flags;

  effectState = gFieldEffectState;
  FieldPsynergy_LoadResources(ABILITY_WHIRLWIND, 1);
  FieldPsynergy_SetupActorPair(ACTOR_IVAN, ACTOR_STUMP_IVY);
  flags = (u8)effectState->messageMode | FIELD_MESSAGE_LOWER;
  effectState->messageMode = flags;
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET);
  FieldPsynergy_StopCasterUpdate();
}

/// Where the stump lands from the ledge and where it bounces on to and sinks,
/// in pixels, and the ground's height there.
#define STUMP_LANDING_X 160
#define STUMP_LANDING_Z 192
#define STUMP_RESTING_X 139
#define STUMP_RESTING_Z 196
#define STUMP_FLOOR_HEIGHT toFIXED(32)
#include "shared/stump_leap.c"

#include "shared/item_equip.c"

/// The tries at making room for the Shaman's Rod before a slot is cleared by
/// force.
#define SHAMANS_ROD_TRIES 1000

/// Gives Ivan the Shaman's Rod and equips it.
///
/// While his pack is full, a weapon of his is thrown away to make room, or
/// failing that a consumable item; after SHAMANS_ROD_TRIES tries a slot is
/// emptied by force.
static void GomaCaveEntrance_GiveShamansRod(void) {
  struct Unit *member;
  struct ItemDefinition *item;
  s32 tries;
  s32 i;
  s32 id = ITEM_SHAMANS_ROD;
  member = Unit_Get(IVAN);
  tries = 0;
retry:
  if (++tries > SHAMANS_ROD_TRIES) {
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

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// Stretches the tall ivy over the cave's mouth to 1.6 times its height.
s32 gGomaCaveEntranceTallIvyScript[] = {
    SC_SET(SW_SCALE_Y, 8 * FIXED_ONE / 5),
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
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), TRIGGER_PUSH_WEST_PILLAR_EAST, CONDITION_ALWAYS, EVENT(GomaCaveEntrance_PushWestPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), TRIGGER_PUSH_WEST_PILLAR_WEST, CONDITION_ALWAYS, EVENT(GomaCaveEntrance_PushWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_IVAN_JOINED, EVENT(GomaCaveEntrance_IvanJoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED, EVENT(GomaCaveEntrance_MoveStump) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR, CONDITION_ALWAYS, EVENT(GomaCaveEntrance_MoveWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE, EVENT(GomaCaveEntrance_MoveEastPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, EVENT(GomaCaveEntrance_BlowAwayIvy) },
    { TRIGGER_TABLE_END },
};
// clang-format on
