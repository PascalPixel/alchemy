/// @file
/// Gondowan Passage and Gondowan Cave, the guarded bridge into Gondowan and the
/// cave east of it, which the party crosses on the way from Tolbi to Babi
/// Lighthouse.
///
/// Three scenes share this overlay:
///
/// - Gondowan Passage (SceneId_GondowanPassage, area name Gondowan Passage):
///   three soldiers guard the bridge. Iodem waits there to go on to Babi
///   Lighthouse with the party once Babi has spoken to it after Colosso
///   (FLAG_BABI_SPOKE_AFTER_COLOSSO).
/// - Gondowan Cave (SceneId_GondowanCave, area name Gondowan Cave): both ways
///   in from the World Map, a tree stump that Move sends leaping into the
///   water, a stone pillar to push or move in after it, and a boulder for
///   Lift.
/// - GondowanCaveRiver (SceneId_GondowanCaveRiver): the river between the
///   cave's two halves, with a boulder for Lift and two chests.
///
/// One exit table serves every scene. The other getters choose by
/// gMainRoutineWork.next, and Gondowan Passage's actor placements and
/// triggers also by FLAG_BABI_SPOKE_AFTER_COLOSSO. An empty set answers any
/// other scene. Every scene's trigger table carries GondowanCaveRiver's two
/// chests.
///
/// Gondowan Passage sets FLAG_RETREAT_BLOCKED on every entry. While Iodem
/// travels with the party (FLAG_IODEM_FOLLOWING_PARTY) his actor is kept
/// out of sight.
///
/// Talking to him or walking up to him while he waits (step trigger 11) makes
/// him come along (GondowanCave_IodemRejoins). Until he has reached Babi
/// Lighthouse (FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED), turning back
/// (step trigger 10) brings him out to ask why
/// (GondowanCave_IodemAsksWhyTurningBack). If he stays to wait, the saved place
/// becomes the Tolbi Sanctum, so a defeat returns the party there.
///
/// GondowanCave's way across the water is two things that float in it, and
/// step triggers 10 and 11 jump the leader across them
/// (GondowanCave_JumpAcross).
///
/// Both are kept in story flags, so the cave stays solved and
/// Scene_Initialize puts them back. FLAG_GONDOWAN_CAVE_STUMP_MOVED is set
/// once the stump has leapt in. FLAG_GONDOWAN_CAVE_PILLAR_DROPPED is set once
/// the pillar has gone in past column 42, shown from then on by
/// ACTOR_DROPPED_PILLAR.
///
/// Cell (107, 17) of the map grid is cell (43, 17) of layout layer 2,
/// whose window starts 64 columns in. It is the crevice east of the pillar,
/// where it drops.
///
/// The layout loads that cell with ground 6, as the two cells west of it.
/// Copying (108, 17) over it gives it ground 0, as its east neighbour. The
/// drop makes that copy, and so does every arrival by
/// GONDOWAN_CAVE_FROM_WORLD_MAP_EAST, solved or not.
///
/// Nothing the player sees depends on it. In tbs-en the screen is the same
/// either way. The leader jumps the crevice at row 17 (step triggers 10 and
/// 11) from either side either way, and the pillar settles at the same height
/// in each of columns 41 to 44 either way.

#include "main_routine_work.h"
#include "heap_slots.h"
#include "scene_ids.h"
#include "module/import.h"
#include "module/scene/gondowan_cave.h"
#include "module/scene/shared/field_effect_spawn.h"
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
#include "event.h"
#include "scene_tables.h"
#include "pushed_actor.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "fixed_math.h"
#include "flags.h"
#include "sounds.h"

/// Gondowan Passage's placed actors, numbered in the order of
/// gGondowanPassageActorPlacements and
/// gGondowanPassageActorPlacementsWithIodem from ACTOR_FIRST_PLACED.
///
/// They are the soldiers who guard the bridge, and in the second table Iodem
/// after them.
enum GondowanPassageActorId {
  /// Asks whether the party will cross into Gondowan.
  ACTOR_FIRST_SOLDIER = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_SOLDIER,
  /// Warns that the cave to the east leads nowhere.
  ACTOR_THIRD_SOLDIER,
  /// Waits for the party once Babi has spoken after Colosso.
  ACTOR_IODEM
};

/// GondowanCaveRiver's placed actor, the one row of
/// gGondowanCaveRiverActorPlacements that a trigger names.
enum GondowanCaveRiverActorId {
  /// Raised by Lift.
  ACTOR_RIVER_BOULDER = ACTOR_FIRST_PLACED
};

/// GondowanCave's placed actors, numbered in the order of
/// gGondowanCaveActorPlacements from ACTOR_FIRST_PLACED.
enum GondowanCaveActorId {
  /// Moved with Move, in the sprite of the Goma Cave stump.
  ACTOR_STUMP = ACTOR_FIRST_PLACED,
  /// Pushed or moved with Move, in the sprite the stone pillars of other scenes
  /// use (crossbone_isle_stepping_stones.c).
  ACTOR_PILLAR,
  /// Stands in for the pillar once it has dropped in.
  ///
  /// It is out of sight at first. It is drawn in the stump's sprite, splashing
  /// in (STUMP_ANIMATION_SPLASH) and on later visits afloat
  /// (STUMP_ANIMATION_SINK).
  ACTOR_DROPPED_PILLAR,
  /// Raised by Lift.
  ACTOR_BOULDER,
  /// An invisible actor (SPRITE_BLANK, its shadow turned off) 16 pixels west
  /// of the boulder.
  ///
  /// It fills the gap between the boulder and the passage's west wall, so the
  /// boulder blocks the passage until Lift takes both away
  /// (https://alchemy.pascalpixel.com/places/gondowan-cave/map-4.png shows
  /// the gap).
  ACTOR_BOULDER_GAP
};

// The leap script also runs after the shared pushed actor movement.
static void GondowanCave_MovePillar(void);
static void GondowanCave_RunSpinningLeap(s32 actorId);

#include "shared/field_effect_create.h"

#include "shared/field_effect_spawn.h"
#include "shared/staged_actor.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_GondowanPassage) {
    return gGondowanPassageEntrances;
  }
  if (sceneId == (s32)&SceneId_GondowanCaveRiver) {
    return gGondowanCaveRiverEntrances;
  }
  if (sceneId == (s32)&SceneId_GondowanCave) {
    return gGondowanCaveEntrances;
  }
  return gGondowanCaveEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// Every scene of the overlay shares one exit table.
const u32 *Scene_GetExits(void) { return gGondowanCaveExits; }

/// Iodem waits in Gondowan Passage once Babi has spoken to the party after
/// Colosso.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_GondowanPassage) {
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanPassageActorPlacementsWithIodem;
    }
    return gGondowanPassageActorPlacements;
  }
  if (sceneId == (s32)&SceneId_GondowanCaveRiver) {
    return gGondowanCaveRiverActorPlacements;
  }
  if (sceneId == (s32)&SceneId_GondowanCave) {
    return gGondowanCaveActorPlacements;
  }
  return gGondowanCaveActorPlacementsOther;
}

/// The triggers of each scene, with Iodem's in Gondowan Passage when he is
/// there.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_GondowanPassage) {
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanPassageTriggersWithIodem;
    }
    return gGondowanPassageTriggers;
  }
  if (sceneId == (s32)&SceneId_GondowanCaveRiver) {
    return gGondowanCaveRiverTriggers;
  }
  if (sceneId == (s32)&SceneId_GondowanCave) {
    return gGondowanCaveTriggers;
  }
  return gGondowanCaveTriggersOther;
}

/// Turning back in Gondowan Passage (step trigger 10) with Iodem in the party,
/// until he has reached Babi Lighthouse: he steps out and asks why Isaac is
/// going back.
static void GondowanCave_IodemAsksWhyTurningBack(void) {
  struct Actor *leader;

  if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED) == 0 &&
      Event_GetFlag(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
    Event_Start();
    // Bring Iodem out of the party before opening his question.
    Actor_SetSpeed(ACTOR_IODEM, 1);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPositionFixed(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetDestinationOffset(ACTOR_IODEM, -8, 16);
    Actor_WaitForMoveById(ACTOR_IODEM);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
    Event_SetMessageId(MsgKaragolWhyGoingBackIsaacDo);
    // "Why are you going back, Isaac? Do you need to get something..."
    Event_OpenMessage(ACTOR_IODEM, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      // "I see... In that case, I shall wait here for you to return." The
      // saved place becomes Tolbi's.
      // "I see... In that case, I shall wait here for you to return."
      Event_ShowMessage(ACTOR_IODEM, 0);
      Actor_WalkTo(ACTOR_IODEM, 152, 232);
      Event_ClearFlag(FLAG_IODEM_FOLLOWING_PARTY);
      Actor_WaitForMoveById(ACTOR_IODEM);
      Actor_SetAnimation(ACTOR_IODEM, ANIM_STAND);
      gMainRoutineWork.restartScene = (s32)&SceneId_StuccoRoomsB;
      gMainRoutineWork.restartEntrance = STUCCO_ROOMS_B_SANCTUM_NORTH;
    } else {
      // "Then we should continue on our way to the lighthouse." He rejoins
      // the party, and Isaac steps away from the step trigger.
      gFieldWork->message += 1;
      // "Then we should continue on our way to the lighthouse."
      Event_ShowMessage(ACTOR_IODEM, 0);
      Actor_SetAnimation(ACTOR_IODEM, ANIM_WALK);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                             leader->z.part.pixel);
      }
      Actor_WaitForMoveById(ACTOR_IODEM);
      Actor_SetPosition(ACTOR_IODEM, 0, 0);
      Event_Wait(30);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
      Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    }
    Event_End();
  }
}

/// Iodem comes along with the party from Gondowan Passage, where he waits.
static void GondowanCave_IodemRejoins(void) {
  Event_Start();
  Actor_FaceActor(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  Event_SetMessageId(MsgKaragolIveBeenWaitingForIsaac);
  // "I've been waiting for you, Isaac! We should go to Babi..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Actor_SetAnimation(ACTOR_IODEM, ANIM_WALK);
  {
    struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

    if (leader != 0)
      Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                           leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IODEM);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Event_Wait(20);
  GameFlag_Set(FLAG_IODEM_FOLLOWING_PARTY);
  Event_End();
}

/// Talking to the first soldier in Gondowan Passage before Babi has spoken
/// after Colosso: he asks whether the party will cross into Gondowan.
static void GondowanCave_SoldierAsksAboutCrossing(void) {
  s32 firstMessage;

  firstMessage = MsgKaragolDoYouWishCrossInto;
  Event_SetMessageId(firstMessage);
  // "Do you wish to cross into Gondowan?"
  Event_OpenMessage(ACTOR_FIRST_SOLDIER, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0) {
        // The shorter answer, without the advice to watch Colosso.
        Event_SetMessageId(firstMessage + 8);
      }
    }
    Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  } else {
    gFieldWork->message++;
    // "That's a good idea. They wouldn't let you into Babi..."
    Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  }
}

/// Mind Read on the second soldier in Gondowan Passage, before Babi has
/// spoken after Colosso: he grumbles about guard duty.
static void GondowanCave_ReadSoldierMind(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0 &&
      GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0)
    Event_SetMessageId(MsgKaragolWeMissedColossoBecauseWe);
  else
    Event_SetMessageId(MsgKaragolWarriorsHaveBeenFightingWhile);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
}

/// Leaving GondowanCave by GONDOWAN_CAVE_TO_WORLD_MAP_EAST (exit 1),
/// out to the World Map.
static void GondowanCave_LeaveToWorldMapEast(void) {
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  // XXX (no evidence): what flags 0x9a3, 0x9a4 and 0x9a6 hold; only this
  // block, which nothing reaches, touches them.
  if (GameFlag_Get(FLAG_SCENE_TEMP_04) != 0) {
    GameFlag_Clear(0x9a3);
    Event_ClearFlag(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    Event_ClearFlag(0x9a4);
    GameFlag_Clear(0x9a6);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    GameFlag_Set(0x9a4);
  }
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Walking west or east into step triggers 10 and 11 in GondowanCave: the
/// leader jumps two cells forward where the map allows (Field_TryJumpForward).
static void GondowanCave_JumpAcross(void) { Field_TryJumpForward(); }

/// Move on the stump in GondowanCave, until it has leapt.
static void GondowanCave_MoveStump(void) {
  Actor_Get(ACTOR_STUMP);
  Event_Start();
  {
    struct Actor *actor = Actor_Get(ACTOR_STUMP);

    if (FIELD_CELL(actor->x.fixed) <= 30) {
      GondowanCave_RunSpinningLeap(ACTOR_STUMP);
      MapGrid_CopyCellAttributes(29, 19, 1, 1, 27, 19);
      GameFlag_Set(FLAG_GONDOWAN_CAVE_STUMP_MOVED);
    }
  }
  Event_End();
}

/// Pushing the pillar in GondowanCave (step trigger 50): it moves a
/// step, then may have reached its place.
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

/// Move on the pillar in GondowanCave, and after each push: past column 42 it
/// drops into the water with a splash, for good.
static void GondowanCave_MovePillar(void) {
  Event_Start();
  if (FIELD_CELL(Actor_Get(ACTOR_PILLAR)->x.fixed) > 42) {
    s32 destinationCellX = 107;
    s32 destinationCellZ = 17;

    MapGrid_CopyCellAttributes(108, 17, 1, 1, destinationCellX,
                               destinationCellZ);
    Event_Wait(8);
    GondowanCave_PlaceDroppedPillar(STUMP_ANIMATION_SPLASH);
    Sound_Play(SE_CAST);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
  }
  Event_End();
}

/// Unused: an empty function nothing refers to.
static UNUSED void GondowanCave_UnusedNoOpA(void) {}

/// Rotates the sprite during the airborne part of the leap.
static void GondowanCave_RotateLeapingActor(struct Actor *actor) {
  actor->sprite->rotation -= TRIG_TURN / 64;
}

/// Waits at most forty frames for the actor to descend to the height
/// limit.
static void GondowanCave_WaitForDescent(struct Actor *actor, s32 heightLimit) {
  s32 framesRemaining = 40;

  while (framesRemaining != 0) {
    wait(1);
    framesRemaining--;
    if (actor->y.fixed <= heightLimit) {
      break;
    }
  }
}

/// The actor spins in place, leaps and lands with a shake and a ring of dust,
/// then comes to rest a little further on.
static void GondowanCave_RunSpinningLeap(s32 actorId) {
  struct Actor *actor;
  u32 stepIndex;
  struct Vector3 particleVelocity;
  s32 angle;

  actor = Actor_Get(actorId);
  // Wind up the spin while horizontal motion is disabled.
  actor->motionFlags = 0;
  for (stepIndex = 0; stepIndex < 9; stepIndex++) {
    wait(1);
    actor->sprite->rotation -= TRIG_TURN / 256;
    actor->x.fixed -= Trig_Cos(actor->sprite->rotation) / 2;
    actor->targetX = ACTOR_NO_TARGET;
  }
  actor->update = GondowanCave_RotateLeapingActor;
  // Continue spinning through the first jump.
  Sound_Play(SE_WHOOSH);
  Actor_SetSpeed(actorId, 2);
  Actor_SetDestination(actorId, 472, 288);
  actor->gravity = px(4) / 5;
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->layer = 0;
  Actor_WaitForMoveById(actorId);
  GondowanCave_WaitForDescent(actor, px(32));
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  // Emit a complete particle fan, including its closing direction.
  for (stepIndex = 0; stepIndex < 17; stepIndex++) {
    angle = stepIndex << 12;
    particleVelocity.x = Trig_Cos(angle);
    particleVelocity.y = 0;
    particleVelocity.z = Trig_Sin(angle);
    particleVelocity.x -= particleVelocity.x / 4;
    particleVelocity.z -= particleVelocity.z / 2;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                      particleVelocity.x, particleVelocity.y,
                      particleVelocity.z, 0, 0);
  }
  Actor_SetDestination(actorId, 440, 308);
  // Finish the second movement before restoring the resting animation.
  Actor_WaitForMoveById(actorId);
  GondowanCave_WaitForDescent(actor, px(32));
  actor->update = 0;
  actor->sprite->rotation = TRIG_TURN / 16;
  Sound_Play(SE_CAST);
  Actor_SetAnimation(actorId, STUMP_ANIMATION_SPLASH);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(10);
  Actor_SetAnimation(actorId, STUMP_ANIMATION_SINK);
}

/// Unused: an empty function nothing refers to.
static UNUSED void GondowanCave_UnusedNoOpB(void) {}

/// Lift on the boulder in GondowanCave also takes away the
/// invisible actor beside it.
static void GondowanCave_LiftBoulder(void) {
  Actor_SetPosition(ACTOR_BOULDER_GAP, 0, 0);
}

/// Sets up the scene as the party enters.
s32 Scene_Initialize(void) {

  if (gMainRoutineWork.next == (s32)&SceneId_GondowanPassage) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    if (GameFlag_Get(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
      Actor_SetPosition(ACTOR_IODEM, 0, 0);
    }
  }
  if (gMainRoutineWork.next == (s32)&SceneId_GondowanCave) {
    // XXX (no evidence): why this arrival changes the drop's cell; the
    // change has no effect that play shows.
    if (gMainRoutineWork.entrance == GONDOWAN_CAVE_FROM_WORLD_MAP_EAST) {
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

#include "shared/field_effect_duration_scripts.h"

#include "shared/pushed_actor_tables.h"
// clang-format off

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gGondowanCaveEntrancesOther[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

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
    SCENE_EXIT(GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_PASSAGE_EAST),
    SCENE_EXIT(GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_PASSAGE_WEST),
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
    // Iodem
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
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(GondowanCave_IodemAsksWhyTurningBack) },
    { TRIGGER_STEP, 11, FLAG_IODEM_FOLLOWING_PARTY, EVENT(GondowanCave_IodemRejoins) },
    { TRIGGER_TALK, ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWhyWouldIodemChooseToWait) },
    { TRIGGER_TALK, ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaventSeenAnybodyUnusualCrossing) },
    { TRIGGER_TALK, ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouGoEastFrom) },
    { TRIGGER_TALK, ACTOR_IODEM, CONDITION_ALWAYS, EVENT(GondowanCave_IodemRejoins) },
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

/// FLAG_SCENE_TEMP_05 ends Move on the pillar.
struct Trigger gGondowanCaveTriggers[] = {
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(GondowanCave_LeaveToWorldMapEast) },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WEST_PASSAGE },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVE },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WORLD_MAP_WEST },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, CONDITION_ALWAYS, EVENT(GondowanCave_LiftBoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GONDOWAN_CAVE_STUMP_MOVED, EVENT(GondowanCave_MoveStump) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, FLAG_SCENE_TEMP_05, EVENT(GondowanCave_MovePillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT(GondowanCave_JumpAcross) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT(GondowanCave_JumpAcross) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 50, CONDITION_ALWAYS, EVENT(GondowanCave_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};
// clang-format on
