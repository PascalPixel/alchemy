/// @file
/// Gondowan Passage and Gondowan Cave, south and south-east of Tolbi.
///
/// Gondowan Passage (SceneId_GondowanPassage) is the bridge from Tolbi into
/// Gondowan: a gatehouse at its north end, three soldiers in spiked helmets
/// at the gate, then a drawbridge and a long stone bridge running south over
/// the water. The soldiers swear nobody gets through the cave to the east to
/// Kalay. Once Babi has sent the party after Colosso
/// (FLAG_BABI_SPOKE_AFTER_COLOSSO), Iodem waits at the north end of the
/// drawbridge and comes along to Babi Lighthouse. While he travels with the
/// party (FLAG_IODEM_FOLLOWING_PARTY) his actor is kept out of sight, and
/// heading back north through the gate with him makes him step out and ask
/// why. Retreat does not work on the bridge (FLAG_RETREAT_BLOCKED).
///
/// Gondowan Cave (SceneId_GondowanCave) is the way the soldiers say is
/// impossible: a cave from the world map east of the bridge through to the
/// Kalay side. Its west mouth opens on a passage that forks. The right fork,
/// the short way to the east cave, is blocked by a boulder that only Lift
/// clears. The left fork leads to the river room (SceneId_GondowanCaveRiver),
/// where Isaac hops along a line of posts in the water to the chest on its
/// islet, and from there into the east cave. In the east cave Move sends a
/// stump off its ledge into the water, where it floats as one more post to
/// hop on, and the tall pillar by the narrow crevice is moved into the
/// crevice to clear the way to it. Isaac jumps the crevice and walks out of
/// the cave's east mouth.
///
/// One exit table serves every scene. The other getters choose by
/// gMainRoutineState.next, and Gondowan Passage's actor placements and
/// triggers also by FLAG_BABI_SPOKE_AFTER_COLOSSO. An empty set answers any
/// other scene. Every scene's trigger table carries the river room's two
/// chests.
///
/// The stump and the pillar are kept in story flags, so the cave stays solved
/// and Scene_Initialize puts them back: FLAG_GONDOWAN_CAVE_STUMP_MOVED once the
/// stump has leapt in, FLAG_GONDOWAN_CAVE_PILLAR_DROPPED once the pillar has
/// dropped into the crevice, shown from then on by ACTOR_DROPPED_PILLAR.
///
/// Dropping the pillar also copies map cell (108, 17) over (107, 17), the
/// crevice's cell (43, 17) as the second layer of the map holds it, 64
/// columns over, and every arrival by the east mouth makes the same copy,
/// solved or not. Nothing on screen depends on it: the crevice looks the
/// same, Isaac jumps it from either side, and the pillar settles at the same
/// height wherever it drops.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern struct SceneEntrance gScene_FallbackEntrances[];
extern struct SceneEntrance gGondowanPassageEntrances[];
extern struct SceneEntrance gGondowanCaveRiverEntrances[];
extern struct SceneEntrance gGondowanCaveEntrances[];
extern u32 gGondowanCaveExits[];
extern struct ActorPlacement gGondowanCaveActorPlacementsOther[];
extern struct ActorPlacement gGondowanPassageActorPlacements[];
extern struct ActorPlacement gGondowanPassageActorPlacementsWithIodem[];
extern struct ActorPlacement gGondowanCaveRiverActorPlacements[];
extern struct ActorPlacement gGondowanCaveActorPlacements[];
extern struct Trigger gGondowanCaveTriggersOther[];
extern struct Trigger gGondowanPassageTriggers[];
extern struct Trigger gGondowanPassageTriggersWithIodem[];
extern struct Trigger gGondowanCaveRiverTriggers[];
extern struct Trigger gGondowanCaveTriggers[];

/// The cells of the bridge and the cave that run events. The cave's way out
/// east is a step trigger whose cell is its exit's number
/// (GONDOWAN_CAVE_TO_WORLD_MAP_EAST).
enum {
  /// Gondowan Passage: the gate through the gatehouse, at its north end.
  CELL_BEHAVIOR_GONDOWAN_PASSAGE_GATE = 10,
  /// Gondowan Passage: the north end of the drawbridge, where Iodem waits.
  CELL_BEHAVIOR_GONDOWAN_PASSAGE_IODEM = 11,
  /// Gondowan Cave: the west edge of the crevice beside the tall pillar.
  CELL_BEHAVIOR_GONDOWAN_CAVE_CREVICE_WEST_EDGE = 10,
  /// Gondowan Cave: the east edge of the crevice.
  CELL_BEHAVIOR_GONDOWAN_CAVE_CREVICE_EAST_EDGE = 11,
  /// Gondowan Cave: the tall pillar's first cell, west of the crevice.
  CELL_BEHAVIOR_GONDOWAN_CAVE_PILLAR = 50
};

/// Gondowan Passage's placed actors, numbered in the order of
/// gGondowanPassageActorPlacements and
/// gGondowanPassageActorPlacementsWithIodem from ACTOR_FIRST_PLACED.
///
/// They are the soldiers who guard the bridge, and in the second table Iodem
/// after them.
enum GondowanPassageActorId {
  /// On the road south of the gatehouse; asks whether the party will cross
  /// into Gondowan.
  ACTOR_FIRST_SOLDIER = ACTOR_FIRST_PLACED,
  /// West of the gate.
  ACTOR_SECOND_SOLDIER,
  /// East of the gate; says nobody gets through the cave to the east to
  /// Kalay.
  ACTOR_THIRD_SOLDIER,
  /// Waits at the north end of the drawbridge once Babi has sent the party
  /// after Colosso.
  ACTOR_IODEM
};

/// The river room's placed actor, the one row of
/// gGondowanCaveRiverActorPlacements that a trigger names.
enum GondowanCaveRiverActorId {
  /// The boulder in the passage south of the river, raised by Lift.
  ACTOR_RIVER_BOULDER = ACTOR_FIRST_PLACED
};

/// Gondowan Cave's placed actors, numbered in the order of
/// gGondowanCaveActorPlacements from ACTOR_FIRST_PLACED.
enum GondowanCaveActorId {
  /// The squat stump on the ledge in the middle of the east cave, beside the
  /// torch; Move sends it into the water west of the ledge. It is drawn in
  /// the sprite of the Goma Cave stump.
  ACTOR_STUMP = ACTOR_FIRST_PLACED,
  /// The tall pillar on the west side of the crevice, pushed or moved with
  /// Move, in the sprite the stone pillars of other scenes use
  /// (crossbone_isle_stepping_stones.c).
  ACTOR_PILLAR,
  /// Stands in for the pillar once it has dropped in.
  ///
  /// It is out of sight at first. It is drawn in the stump's sprite, splashing
  /// in (STUMP_ANIMATION_SPLASH) and on later visits afloat
  /// (STUMP_ANIMATION_SINK).
  ACTOR_DROPPED_PILLAR,
  /// The boulder in the right fork north of the west mouth, raised by Lift.
  ACTOR_BOULDER,
  /// An invisible actor (SPRITE_BLANK, its shadow turned off) 16 pixels west
  /// of the boulder.
  ///
  /// It fills the gap between the boulder and the passage's west wall, so the
  /// boulder blocks the fork until Lift takes both away
  /// (https://alchemy.pascalpixel.com/places/gondowan-cave/map-4.png shows
  /// the gap).
  ACTOR_BOULDER_GAP
};

/// The stump's column at the edge of its ledge (its placement, x 496). Move
/// west takes it off this column, and it leaps into the water.
#define STUMP_COLUMN 31

/// The crevice's column, two east of the pillar's first cell (x 696, where
/// ACTOR_DROPPED_PILLAR stands). Pushed or moved this far, the pillar drops
/// in.
#define PILLAR_DROP_COLUMN 43

/// Move on the tall pillar beside the crevice in the east cave, and each push
/// of it: once it reaches the crevice (cell (43, 17)) it drops into the water
/// at its foot with a splash and stays there, clearing the way to the edge.
static void GondowanCave_MovePillar(void);
/// The actor spins in place, leaps off the ledge and lands with a shake and a
/// ring of dust, then hops on into the water with a splash and floats there.
static void GondowanCave_LeapIntoWater(s32 actorId);

#include "shared/field_effect_create.c"

#include "shared/field_effect_spawn.c"
#include "shared/pushed_actor.c"

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_GONDOWAN_PASSAGE) {
    return gGondowanPassageEntrances;
  }
  if (sceneId == SCENE_GONDOWAN_CAVE_RIVER) {
    return gGondowanCaveRiverEntrances;
  }
  if (sceneId == SCENE_GONDOWAN_CAVE) {
    return gGondowanCaveEntrances;
  }
  return gScene_FallbackEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// Every scene of the overlay shares one exit table.
const u32 *Scene_GetExits(void) { return gGondowanCaveExits; }

/// Iodem waits on Gondowan Passage once Babi has sent the party after
/// Colosso.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_GONDOWAN_PASSAGE) {
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanPassageActorPlacementsWithIodem;
    }
    return gGondowanPassageActorPlacements;
  }
  if (sceneId == SCENE_GONDOWAN_CAVE_RIVER) {
    return gGondowanCaveRiverActorPlacements;
  }
  if (sceneId == SCENE_GONDOWAN_CAVE) {
    return gGondowanCaveActorPlacements;
  }
  return gGondowanCaveActorPlacementsOther;
}

/// The triggers of each scene, with Iodem's in Gondowan Passage when he is
/// there.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_GONDOWAN_PASSAGE) {
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanPassageTriggersWithIodem;
    }
    return gGondowanPassageTriggers;
  }
  if (sceneId == SCENE_GONDOWAN_CAVE_RIVER) {
    return gGondowanCaveRiverTriggers;
  }
  if (sceneId == SCENE_GONDOWAN_CAVE) {
    return gGondowanCaveTriggers;
  }
  return gGondowanCaveTriggersOther;
}

/// Isaac heads back north through the gatehouse's gate on Gondowan Passage
/// (cells (8, 7) to (10, 7)) with Iodem in the party, before Iodem has reached
/// Babi Lighthouse: Iodem steps out beside him and asks whether he needs
/// something he left behind.
///
/// Answered yes, Iodem walks back to the north end of the drawbridge to wait
/// there, and a defeat from then on returns the party to the Tolbi Sanctum.
/// Answered no, he says they should go on to the lighthouse and falls in
/// again, and Isaac steps back south off the gate.
static void GondowanCave_IodemAsksWhyTurningBack(void) {
  struct Actor *leader;

  if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED) == 0 &&
      Event_GetFlag(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
    Event_Start();
    Actor_SetSpeed(ACTOR_IODEM, 1);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPositionFX(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetDestinationOffset(ACTOR_IODEM, -8, 16);
    Actor_WaitForMoveById(ACTOR_IODEM);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
    Event_SetMessageId(MsgKaragolWhyGoingBackIsaacDo);
    // "Why are you going back, Isaac?"
    // "Do you need to get something you left behind?"
    Event_OpenMessage(ACTOR_IODEM, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      // "I see... In that case, I shall wait here for you to return."
      Event_ShowMessage(ACTOR_IODEM, 0);
      Actor_WalkTo(ACTOR_IODEM, 152, 232);
      Event_ClearFlag(FLAG_IODEM_FOLLOWING_PARTY);
      Actor_WaitForMoveById(ACTOR_IODEM);
      Actor_SetAnimation(ACTOR_IODEM, ANIM_STAND);
      // A defeat now returns the party to the Tolbi Sanctum.
      gMainRoutineState.restartScene = SCENE_STUCCO_ROOMS_B;
      gMainRoutineState.restartEntrance = STUCCO_ROOMS_B_SANCTUM_NORTH;
    } else {
      gFieldState->message += 1;
      // "Then we should continue on our way to the lighthouse."
      Event_ShowMessage(ACTOR_IODEM, 0);
      Actor_SetAnimation(ACTOR_IODEM, ANIM_WALK);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      if (leader != NULL) {
        Actor_SetDestination(ACTOR_IODEM, leader->x.part[0].integer,
                             leader->z.part[0].integer);
      }
      Actor_WaitForMoveById(ACTOR_IODEM);
      Actor_SetPosition(ACTOR_IODEM, 0, 0);
      Event_Wait(30);
      // Isaac steps back south off the gate.
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
      Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    }
    Event_End();
  }
}

/// Isaac talks to Iodem, waiting at the north end of the drawbridge on
/// Gondowan Passage, or walks up to him (cells (8, 14) to (10, 14)): Iodem
/// says he has been waiting and they should go to Babi Lighthouse, walks into
/// Isaac and travels with the party from then on.
static void GondowanCave_IodemComesAlong(void) {
  Event_Start();
  Actor_FaceActor(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  Event_SetMessageId(MsgKaragolIveBeenWaitingForIsaac);
  // "I've been waiting for you, Isaac! We should go to Babi Lighthouse soon."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Actor_SetAnimation(ACTOR_IODEM, ANIM_WALK);
  {
    struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

    if (leader != NULL)
      Actor_SetDestination(ACTOR_IODEM, leader->x.part[0].integer,
                           leader->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_IODEM);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Event_Wait(20);
  GameFlag_Set(FLAG_IODEM_FOLLOWING_PARTY);
  Event_End();
}

/// Isaac talks to the soldier on the road south of the gatehouse on Gondowan
/// Passage, before Babi has sent the party on: he asks whether they wish to
/// cross into Gondowan.
///
/// Answered yes, he won't stop them but would sooner watch Colosso in Tolbi,
/// or, once Colosso is over, simply wouldn't go without business there.
/// Answered no, he agrees: they wouldn't be let into Babi Lighthouse anyway.
static void GondowanCave_SoldierAsksAboutCrossing(void) {
  s32 firstMessage;

  firstMessage = MsgKaragolDoYouWishCrossInto;
  Event_SetMessageId(firstMessage);
  // "Do you wish to cross into Gondowan?"
  Event_OpenMessage(ACTOR_FIRST_SOLDIER, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    // Yes: "I won't stop you, but I wouldn't go unless I had business there."
    // "I'd go to Tolbi and watch Colosso. That's what I'd do."
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0) {
        // Once Colosso is over, the shorter answer eight messages on: "I
        // won't stop you, but I wouldn't go unless I had business there!"
        Event_SetMessageId(firstMessage + 8);
      }
    }
    Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  } else {
    gFieldState->message++;
    // No: "That's a good idea. They wouldn't let you into Babi Lighthouse,
    // anyway."
    Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  }
}

/// Mind Read on the soldier west of the gate on Gondowan Passage, before Babi
/// has sent the party on: he grumbles about being stuck on guard duty, while
/// the warriors fight or, once it is over, through all of Colosso.
static void GondowanCave_ReadSoldierMind(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0 &&
      GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0)
    Event_SetMessageId(MsgKaragolWeMissedColossoBecauseWe);
  else
    Event_SetMessageId(MsgKaragolWarriorsHaveBeenFightingWhile);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
}

/// Isaac walks out of Gondowan Cave by its east mouth, at the foot of the path
/// in the south-east corner of the east cave (cell (49, 29)), and comes out on
/// the world map.
static void GondowanCave_LeaveToWorldMapEast(void) {
  struct FieldState *state;

  state = gFieldState;
  Event_Start();
  // XXX (no evidence): what story flags 0x9a3, 0x9a4 and 0x9a6 hold, among
  // FLAG_GONDOWAN_CAVE_STUMP_MOVED and FLAG_GONDOWAN_CAVE_PILLAR_DROPPED; only
  // this block touches them, nothing in either game reads them, and nothing
  // sets FLAG_SCENE_TEMP_04, so no play or render shows them.
  if (GameFlag_Get(FLAG_SCENE_TEMP_04) != 0) {
    GameFlag_Clear(0x9a3);
    Event_ClearFlag(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    Event_ClearFlag(0x9a4);
    GameFlag_Clear(0x9a6);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    GameFlag_Set(0x9a4);
  }
  Import_EventSetExitRequest(state->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Isaac walks to the edge of the narrow crevice in the east cave (cell
/// (42, 17) on its west side, (44, 17) on its east) and keeps walking: he
/// jumps across it.
///
/// Field_TryJumpForward carries him two cells on where the map allows.
static void GondowanCave_JumpCrevice(void) { Field_TryJumpForward(); }

/// Move on the squat stump on the ledge in the middle of the east cave, steered
/// west: it spins, hops off the ledge onto the ground below in a ring of
/// dust, and on into the water with a splash, where it floats (cell (27, 19))
/// for Isaac to hop on.
///
/// Its cell takes the attributes of cell (29, 19), a square Isaac hops from,
/// so the water there becomes one too. Once it has leapt Move on it no longer
/// runs this.
static void GondowanCave_MoveStump(void) {
  // DEAD: the stump looked up and dropped; the ROM keeps its call.
  Actor_Get(ACTOR_STUMP);
  Event_Start();
  if (FIELD_CELL(Actor_Get(ACTOR_STUMP)->x.fixed) < STUMP_COLUMN) {
    GondowanCave_LeapIntoWater(ACTOR_STUMP);
    MapGrid_CopyCellAttributes(29, 19, 1, 1, 27, 19);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_STUMP_MOVED);
  }
  Event_End();
}

/// Isaac, standing where the tall pillar first stood beside the crevice (cell
/// (41, 17)), walks into it: he pushes it a cell east, and at the crevice it
/// drops in.
static void GondowanCave_PushPillar(void) {
  PushedActor_AdvancePair();
  GondowanCave_MovePillar();
}

/// Hides the pillar and shows the actor that stands in for it in its
/// place.
static inline void GondowanCave_PlaceDroppedPillar(s32 animation) {
  Actor_SetPosition(ACTOR_PILLAR, 0, 0);
  Actor_SetPosition(ACTOR_DROPPED_PILLAR, 696, 288);
  Actor_SetAnimation(ACTOR_DROPPED_PILLAR, animation);
}

static void GondowanCave_MovePillar(void) {
  Event_Start();
  if (FIELD_CELL(Actor_Get(ACTOR_PILLAR)->x.fixed) >= PILLAR_DROP_COLUMN) {
    MapGrid_CopyCellAttributes(108, 17, 1, 1, 107, 17);
    Event_Wait(8);
    GondowanCave_PlaceDroppedPillar(STUMP_ANIMATION_SPLASH);
    Sound_Play(SE_CAST);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
  }
  Event_End();
}

/// Unused: an empty function nothing refers to.
static UNUSED void GondowanCave_UnusedNoOpA(void) {}

/// Spins the leaping actor a sixty-fourth of a turn each frame.
static void GondowanCave_RotateLeapingActor(struct Actor *actor) {
  actor->sprite->rotation -= TRIG_TURN / 64;
}

/// Waits until the actor has come down to the height, forty frames at most.
static void GondowanCave_WaitToLand(struct Actor *actor, s32 height) {
  s32 framesRemaining = 40;

  while (framesRemaining != 0) {
    wait(1);
    framesRemaining--;
    if (actor->y.fixed <= height) {
      break;
    }
  }
}

static void GondowanCave_LeapIntoWater(s32 actorId) {
  struct Actor *actor;
  u32 stepIndex;
  struct Vector3 particleVelocity;
  s32 angle;

  actor = Actor_Get(actorId);
  // It winds up its spin in place.
  actor->motionFlags = 0;
  for (stepIndex = 0; stepIndex < 9; stepIndex++) {
    wait(1);
    actor->sprite->rotation -= TRIG_TURN / 256;
    actor->x.fixed -= Trig_Cos(actor->sprite->rotation) / 2;
    actor->targetX = ACTOR_NO_TARGET;
  }
  actor->update = GondowanCave_RotateLeapingActor;
  Sound_Play(SE_WHOOSH);
  Actor_SetSpeed(actorId, 2);
  Actor_SetDestination(actorId, 472, 288);
  actor->gravity = toFIXED(4) / 5;
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->layer = 0;
  Actor_WaitForMoveById(actorId);
  GondowanCave_WaitToLand(actor, toFIXED(32));
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  for (stepIndex = 0; stepIndex <= TRIG_TURN / FACING_STEP; stepIndex++) {
    angle = stepIndex * FACING_STEP;
    particleVelocity.x = Trig_Cos(angle);
    particleVelocity.y = 0;
    particleVelocity.z = Trig_Sin(angle);
    particleVelocity.x -= particleVelocity.x / 4;
    particleVelocity.z -= particleVelocity.z / 2;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                      particleVelocity.x, particleVelocity.y,
                      particleVelocity.z, 0, NULL);
  }
  Actor_SetDestination(actorId, 440, 308);
  Actor_WaitForMoveById(actorId);
  GondowanCave_WaitToLand(actor, toFIXED(32));
  actor->update = NULL;
  actor->sprite->rotation = TRIG_TURN / 16;
  Sound_Play(SE_CAST);
  Actor_SetAnimation(actorId, STUMP_ANIMATION_SPLASH);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(10);
  Actor_SetAnimation(actorId, STUMP_ANIMATION_SINK);
}

/// Unused: an empty function nothing refers to.
static UNUSED void GondowanCave_UnusedNoOpB(void) {}

/// Lift on the boulder in the right fork north of the cave's west mouth raises
/// it away, and clears the short way north to the east cave.
///
/// Lift removes the boulder itself; the event takes away ACTOR_BOULDER_GAP,
/// which stood in the gap beside it.
static void GondowanCave_LiftBoulder(void) {
  Actor_SetPosition(ACTOR_BOULDER_GAP, 0, 0);
}

/// Sets up the scene as the party enters.
s32 Scene_Initialize(void) {
  if (gMainRoutineState.next == SCENE_GONDOWAN_PASSAGE) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    if (GameFlag_Get(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
      Actor_SetPosition(ACTOR_IODEM, 0, 0);
    }
  }
  if (gMainRoutineState.next == SCENE_GONDOWAN_CAVE) {
    // XXX (no evidence): why this arrival changes the drop's cell; the
    // change has no effect that play shows.
    if (gMainRoutineState.entrance == GONDOWAN_CAVE_FROM_WORLD_MAP_EAST) {
      MapGrid_CopyCellAttributes(108, 17, 1, 1, 107, 17);
    }
    if (GameFlag_Get(FLAG_GONDOWAN_CAVE_STUMP_MOVED) != 0) {
      Actor_SetPosition(ACTOR_STUMP, 440, 308);
      Actor_SetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
      MapGrid_CopyCellAttributes(29, 19, 1, 1, 27, 19);
    }
    if (GameFlag_Get(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED) != 0) {
      GondowanCave_PlaceDroppedPillar(STUMP_ANIMATION_SINK);
    }
    Actor_SetSpriteFlags(Actor_Get(ACTOR_BOULDER_GAP), 0);
  }
  return 0;
}

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "shared/fallback_entrances.c"

struct SceneEntrance gGondowanPassageEntrances[] = {
    { GONDOWAN_PASSAGE_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 152, 0, 120, FACING_SOUTH, 0, 32, 32, 272, 480, 0 },
    { GONDOWAN_PASSAGE_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 152, 0, 440, FACING_NORTH, 0, 32, 32, 272, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gGondowanCaveRiverEntrances[] = {
    { GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_NORTH, CONDITION_ALWAYS, 728, 0, 312, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
    { GONDOWAN_CAVE_RIVER_FROM_BOULDER_PASSAGE, CONDITION_ALWAYS, 168, 0, 408, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
    { GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_FROM_RIVER, CONDITION_ALWAYS, 168, 0, 520, FACING_SOUTH, 0, 112, 464, 576, 704, 0 },
    { GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_SOUTH, CONDITION_ALWAYS, 504, 0, 664, FACING_NORTH, 0, 112, 464, 576, 704, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gGondowanCaveEntrances[] = {
    { GONDOWAN_CAVE_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 792, 0, 456, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_NORTH, CONDITION_ALWAYS, 360, 0, 152, FACING_SOUTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_FROM_WEST_PASSAGE, CONDITION_ALWAYS, 344, 0, 440, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVE, CONDITION_ALWAYS, 344, 0, 584, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
    { GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_SOUTH, CONDITION_ALWAYS, 136, 0, 504, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
    { GONDOWAN_CAVE_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 216, 0, 888, FACING_NORTH, 0, 48, 432, 408, 896, 0 },
    { SCENE_TABLE_END },
};

u32 gGondowanCaveExits[] = {
    SCENE_EXITS(GondowanPassage),
    SCENE_EXIT(GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_PASSAGE_NORTH),
    SCENE_EXIT(GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_PASSAGE_SOUTH),
    SCENE_EXITS(GondowanCaveRiver),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH, GondowanCave, GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_NORTH),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_TO_BOULDER_PASSAGE, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_FROM_RIVER),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_TO_RIVER, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_BOULDER_PASSAGE),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_SOUTH, GondowanCave, GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_SOUTH),
    SCENE_EXITS(GondowanCave),
    SCENE_EXIT(GONDOWAN_CAVE_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_GONDOWAN_CAVE_NORTH),
    SCENE_EXIT(GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_NORTH),
    SCENE_EXIT(GONDOWAN_CAVE_TO_WEST_PASSAGE, GondowanCave, GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVE),
    SCENE_EXIT(GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVE, GondowanCave, GONDOWAN_CAVE_FROM_WEST_PASSAGE),
    SCENE_EXIT(GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_SOUTH),
    SCENE_EXIT(GONDOWAN_CAVE_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_GONDOWAN_CAVE_SOUTH),
    SCENE_EXITS_END,
};

struct ActorPlacement gGondowanCaveActorPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gGondowanPassageActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        144, 0, 168, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        120, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 120, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gGondowanPassageActorPlacementsWithIodem[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 136, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        120, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 120, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 232, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gGondowanCaveRiverActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 624, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        440, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gGondowanCaveActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        496, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        664, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 636, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 640, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

struct Trigger gGondowanCaveTriggersOther[] = {
    { TRIGGER_TABLE_END },
};

struct Trigger gGondowanPassageTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH },
    { TRIGGER_TALK, ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, EVENT(GondowanCave_SoldierAsksAboutCrossing) },
    { TRIGGER_TALK, ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveUsWereTheSoldiersWhoGuard) },
    { TRIGGER_TALK, ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouHeadEastYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveThoseWhoGoToGondowanRarely) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, EVENT(GondowanCave_ReadSoldierMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveYoudThinkYouCouldGetThrough) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};

struct Trigger gGondowanPassageTriggersWithIodem[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH },
    { TRIGGER_STEP, CELL_BEHAVIOR_GONDOWAN_PASSAGE_GATE, CONDITION_ALWAYS, EVENT(GondowanCave_IodemAsksWhyTurningBack) },
    { TRIGGER_STEP, CELL_BEHAVIOR_GONDOWAN_PASSAGE_IODEM, FLAG_IODEM_FOLLOWING_PARTY, EVENT(GondowanCave_IodemComesAlong) },
    { TRIGGER_TALK, ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWhyWouldIodemChooseToWait) },
    { TRIGGER_TALK, ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaventSeenAnybodyUnusualCrossing) },
    { TRIGGER_TALK, ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouGoEastFrom) },
    { TRIGGER_TALK, ACTOR_IODEM, CONDITION_ALWAYS, EVENT(GondowanCave_IodemComesAlong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveIodemIsGoingToBabiLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveItsALotOfStressBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEveryoneComesDownThisWayWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaveToHeadForThe) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};

struct Trigger gGondowanCaveRiverTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_BOULDER_PASSAGE },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_TO_RIVER },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_SOUTH },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_RIVER_BOULDER, CONDITION_ALWAYS, 0 },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};

/// Move on the tall pillar runs while FLAG_SCENE_TEMP_05 is clear, which is
/// always, since nothing sets it; once the pillar has dropped it stands at
/// (0, 0), out of reach.
struct Trigger gGondowanCaveTriggers[] = {
    { TRIGGER_STEP, GONDOWAN_CAVE_TO_WORLD_MAP_EAST, CONDITION_ALWAYS, EVENT(GondowanCave_LeaveToWorldMapEast) },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WEST_PASSAGE },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVE },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WORLD_MAP_WEST },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, CONDITION_ALWAYS, EVENT(GondowanCave_LiftBoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GONDOWAN_CAVE_STUMP_MOVED, EVENT(GondowanCave_MoveStump) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, FLAG_SCENE_TEMP_05, EVENT(GondowanCave_MovePillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_GONDOWAN_CAVE_CREVICE_EAST_EDGE, CONDITION_ALWAYS, EVENT(GondowanCave_JumpCrevice) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_GONDOWAN_CAVE_CREVICE_WEST_EDGE, CONDITION_ALWAYS, EVENT(GondowanCave_JumpCrevice) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_GONDOWAN_CAVE_PILLAR, CONDITION_ALWAYS, EVENT(GondowanCave_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};
// clang-format on
