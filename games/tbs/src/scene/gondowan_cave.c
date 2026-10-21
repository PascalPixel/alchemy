/// @file
/// Gondowan Passage, the guarded bridge into Gondowan, and Gondowan Cave, the
/// cave east of it.
///
/// - Gondowan Passage (SceneId_GondowanPassage), between two points of the
///   world map (GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH and
///   GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH). Three soldiers guard the bridge to
///   Gondowan, and Retreat does not work here (FLAG_RETREAT_BLOCKED, set on
///   every entry). At first the first soldier asks `Do you wish to cross into
///   Gondowan?` and lets the party pass either way, sending them to watch
///   Colosso in Tolbi until it is over (FLAG_COLOSSO_OVER); the second
///   grumbles in his mind about missing it, and the third warns that no one
///   gets through the cave to the east to Kalay: those who tried never
///   returned.
///
///   After Colosso, Lord Babi of Tolbi asks the party to go to Venus
///   Lighthouse with Iodem, who commands his soldiers, and Iodem goes ahead
///   to wait for them at the passage. Once Babi has spoken
///   (FLAG_BABI_SPOKE_AFTER_COLOSSO) the scene takes other placements and
///   events: Iodem stands in the passage, and the soldiers talk of him.
///   Talking to him, or walking up to him while he is not with the party
///   (trigger 11), makes him join it (FLAG_IODEM_FOLLOWING_PARTY): `I've been
///   waiting for you, Isaac! We should go to Babi Lighthouse soon.` While he
///   travels with them, entering keeps his actor out of sight, and until he
///   has reached Babi Lighthouse (FLAG_IODEM_AT_BABI_LIGHTHOUSE) turning back
///   (trigger 10) brings him out to ask `Why are you going back, Isaac?` If
///   the party has something to fetch, he stays to wait, and a defeat now
///   returns them to the Tolbi Sanctum (TOLBI_INTERIORS_SANCTUM_NORTH); if
///   not, he rejoins them.
/// - Gondowan Cave (SceneId_GondowanCaveRiver and SceneId_GondowanCave), the
///   cave that, the soldiers say, no one gets through. Someone in Tolbi's
///   houses thinks the same (tolbi_interiors.c, Mind Read,
///   MsgTolbiInteriorsYoudThinkYouCouldGetThrough): `Not unless you can lift
///   boulders, at any rate.` Both ways in are in GondowanCave
///   (GONDOWAN_CAVE_TO_WORLD_MAP_EAST and GONDOWAN_CAVE_TO_WORLD_MAP_WEST),
///   and GondowanCaveRiver lies between two parts of it
///   (GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH and _SOUTH lead in,
///   GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH and _SOUTH back).
///
///   In GondowanCave a stump answers Move from column 30 or less: it spins,
///   leaps across and lies where it lands, freeing the cell where it stood
///   (FLAG_GONDOWAN_CAVE_STUMP_MOVED). A pillar is pushed by walking
///   into it, east or west, or moved with Move; once it is past column 42 it
///   drops into its place for good, shown from then on by a second actor
///   (FLAG_GONDOWAN_CAVE_PILLAR_DROPPED). Both are story flags, so the
///   cave stays solved. Lift raises a boulder in each scene; in GondowanCave it
///   also takes away the invisible actor beside it (ACTOR_BOULDER_SIDE).
///   Two chests in GondowanCaveRiver hold an Apple and a Lucky Medal (each
///   scene's table carries their search events). Arriving by
///   GONDOWAN_CAVE_FROM_WORLD_MAP_EAST always gives cell (107, 17) the
///   attributes it takes when the pillar drops, and leaving by
///   GONDOWAN_CAVE_TO_WORLD_MAP_EAST runs an event: with FLAG_SCENE_TEMP_04
///   set, which no code in this overlay sets, it first puts the puzzle's flags
///   into one arrangement.
///
/// Every scene shares one exit table; the getters choose each scene's other
/// tables, and an empty set serves any other scene number.
#include "game_state.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/gondowan_cave.h"
#include "unit/scene/common/effect_spawn.h"
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
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "field_obj.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "event.h"

/// Gondowan Passage's placed actors, numbered in the order of
/// gGondowanPassagePlacements and gGondowanPassagePlacementsWithIodem from
/// ACTOR_FIRST_PLACED: the soldiers who guard the bridge, and in the second
/// table Iodem after them.
enum GondowanPassageActor {
  /// Asks `Do you wish to cross into Gondowan?`
  ACTOR_FIRST_SOLDIER = ACTOR_FIRST_PLACED,
  /// "Us? We're the soldiers who guard the bridge to Gondowan."
  ACTOR_SECOND_SOLDIER,
  /// "Even if you head east, you can't get to Kalay from here, even!"
  ACTOR_THIRD_SOLDIER,
  /// Waits for the party once Babi has spoken after Colosso.
  /// ★ Named from the game: "Why would Iodem choose to wait for us"
  /// (MsgGondowanCaveWhyWouldIodemChooseToWait)
  ACTOR_IODEM
};

/// GondowanCaveRiver's placed actor, the one row of
/// gGondowanCaveRiverPlacements that an event names.
enum GondowanCaveRiverActor {
  ACTOR_RIVER_BOULDER = ACTOR_FIRST_PLACED ///< raised by Lift
};

/// GondowanCave's placed actors, numbered in the order of
/// gGondowanCavePlacements from ACTOR_FIRST_PLACED.
enum GondowanCaveActor {
  /// Moved with Move, in the sprite of the Goma Cave stump the game calls "that
  /// stump" (MsgGomaNoUsePsynergy).
  ACTOR_STUMP = ACTOR_FIRST_PLACED,
  /// Pushed or moved with Move, in the sprite the stone pillars of other scenes
  /// use (crossbone_isle_stepping_stones.c).
  /// ★ Named from what the player sees: a tall tan stone pillar standing on
  /// the cave floor (its render)
  ACTOR_PILLAR,
  /// Stands in for the pillar once it has dropped into its place; out of sight
  /// at first. It is drawn in the stump's sprite, and on later visits in the
  /// animation the stump takes once it has landed.
  ACTOR_DROPPED_PILLAR,
  /// Raised by Lift.
  /// ★ Named from what the player sees: a round boulder in the cave passage,
  /// which floats up under Lift (its render)
  ACTOR_BOULDER,
  /// An invisible actor 16 pixels west of the boulder, which Lift takes away
  /// with it. XXX: likely holds the cell beside the boulder, because it
  /// stands one cell from it and goes when the boulder does.
  ACTOR_BOULDER_SIDE
};

// The leap event also runs after the shared staged-actor movement.
static void Event_MovePillar(void);
static void GondowanCave_RunSpinningLeap(s32 actorId);

// Shared effect routines retain their position before the scene handlers.
#include "common/effect_create.h"

// Particle and movable-object routines precede this scene's entry points.
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_GondowanPassage) {
    return gGondowanPassageEntrances;
  }
  if (scene == (s32)&SceneId_GondowanCaveRiver) {
    return gGondowanCaveRiverEntrances;
  }
  if (scene == (s32)&SceneId_GondowanCave) {
    return gGondowanCaveEntrances;
  }
  return gGondowanCaveEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Every scene of the overlay shares one exit table.
const u32 *Scene_GetExits(void) { return gGondowanCaveExits; }

/// Iodem waits in Gondowan Passage once Babi has spoken to the party after
/// Colosso.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_GondowanPassage) {
    if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanPassagePlacementsWithIodem;
    }
    return gGondowanPassagePlacements;
  }
  if (scene == (s32)&SceneId_GondowanCaveRiver) {
    return gGondowanCaveRiverPlacements;
  }
  if (scene == (s32)&SceneId_GondowanCave) {
    return gGondowanCavePlacements;
  }
  return gGondowanCavePlacementsOther;
}

/// Each scene's events, with Iodem's in Gondowan Passage when he is there.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_GondowanPassage) {
    if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanPassageEventsWithIodem;
    }
    return gGondowanPassageEvents;
  }
  if (scene == (s32)&SceneId_GondowanCaveRiver) {
    return gGondowanCaveRiverEvents;
  }
  if (scene == (s32)&SceneId_GondowanCave) {
    return gGondowanCaveEvents;
  }
  return gGondowanCaveEventsOther;
}

/// Turning back in Gondowan Passage (trigger 10) with Iodem in the party,
/// until he has reached Babi Lighthouse: he steps out and asks why Isaac is
/// going back. Yes: he waits here for the party's return. No: he rejoins the
/// party and Isaac steps away from the trigger.
static void Event_IodemAsksWhyTurningBack(void) {
  struct FieldActor *leader;

  if (FLAG_CHECK(FLAG_IODEM_AT_BABI_LIGHTHOUSE) == 0 &&
      FLAG_CHECK(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
    EV_BEGIN();
    // Bring Iodem out of the party before opening his question.
    Engine_ActorSetSpeed(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
    }
    Engine_ActorSetDestinationOffset(ACTOR_IODEM, -8, 16);
    Engine_ActorWaitForMove(ACTOR_IODEM);
    OBJ_DIR(ACTOR_IODEM, FACING_NORTH + FACING_STEP, 0);
    WAIT(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
    MESSAGE_SET(MsgKaragolWhyGoingBackIsaacDo);
    Event_OpenMessage(ACTOR_IODEM, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      // "I see... In that case, I shall wait here for you to return." The
      // saved place becomes Tolbi's.
      Event_ShowMessage(ACTOR_IODEM, 0);
      OBJ_WALK(ACTOR_IODEM, 152, 232);
      GameFlag_Clear(FLAG_IODEM_FOLLOWING_PARTY);
      Engine_ActorWaitForMove(ACTOR_IODEM);
      OBJ_MODE(ACTOR_IODEM, ANIM_STAND);
      gGameState.savedScene = (s32)&SceneId_TolbiInteriors;
      gGameState.savedEntrance = TOLBI_INTERIORS_SANCTUM_NORTH;
    } else {
      // "Then we should continue on our way to the lighthouse." He rejoins
      // the party, and Isaac steps away from the trigger.
      gEventWork->message += 1;
      Event_ShowMessage(ACTOR_IODEM, 0);
      OBJ_MODE(ACTOR_IODEM, ANIM_WALK);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                             leader->z.part.pixel);
      }
      Engine_ActorWaitForMove(ACTOR_IODEM);
      OBJ_POS_SET(ACTOR_IODEM, 0, 0);
      WAIT(30);
      OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_WALK);
      Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
      Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
      OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
    }
    EV_END();
  }
}

/// Talking to Iodem where he waits in Gondowan Passage, or walking up to him
/// (trigger 11) while he is not with the party: he joins it. `I've been
/// waiting for you, Isaac! We should go to Babi Lighthouse soon.`
static void Event_IodemRejoins(void) {
  EV_BEGIN();
  OBJ_FACE(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  WAIT(10);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  MESSAGE_SET(MsgKaragolIveBeenWaitingForIsaac);
  MESSAGE_PUT(ACTOR_IODEM, 0);
  OBJ_MODE(ACTOR_IODEM, ANIM_WALK);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0)
      Engine_ActorSetDestination(ACTOR_IODEM, leader->x.part.pixel,
                                 leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_IODEM);
  Engine_ActorSetPosition(ACTOR_IODEM, 0, 0);
  WAIT(20);
  Engine_GameFlagSet(FLAG_IODEM_FOLLOWING_PARTY);
  EV_END();
}

/// Talking to the first soldier in Gondowan Passage before Babi has spoken
/// after Colosso: he asks whether the party will cross into Gondowan. `Do you
/// wish to cross into Gondowan?` Yes: `I won't stop you, but I wouldn't go
/// unless I had business there.`, with advice to watch Colosso until it is
/// over. No: `That's a good idea. They wouldn't let you into Babi Lighthouse,
/// anyway.`
static void Event_SoldierAsksAboutCrossing(void) {
  s32 firstMessage;

  firstMessage = MsgKaragolDoYouWishCrossInto;
  MESSAGE_SET(firstMessage);
  MESSAGE_OPEN(ACTOR_FIRST_SOLDIER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0) {
        // The shorter answer, without the advice to watch Colosso.
        MESSAGE_SET(firstMessage + 8);
      }
    }
    MESSAGE_PUT(ACTOR_FIRST_SOLDIER, 0);
  } else {
    gEventWork->message++;
    MESSAGE_PUT(ACTOR_FIRST_SOLDIER, 0);
  }
}

/// Mind Read on the second soldier in Gondowan Passage, before Babi has
/// spoken after Colosso: once Colosso is over (FLAG_COLOSSO_OVER) he grumbles
/// that guard duty made him miss all of it; before, that the warriors fight
/// while he guards the bridge.
static void Event_ReadSoldierMind(void) {
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0 &&
      Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0)
    MESSAGE_SET(MsgKaragolWeMissedColossoBecauseWe);
  else
    MESSAGE_SET(MsgKaragolWarriorsHaveBeenFightingWhile);
  MESSAGE_PUT(ACTOR_SECOND_SOLDIER, 0);
}

/// Leaving GondowanCave by GONDOWAN_CAVE_TO_WORLD_MAP_EAST
/// (collision code 1), out to the world map. With FLAG_SCENE_TEMP_04 set, the
/// puzzle's saved flags return to one arrangement first.
static void Event_LeaveToWorldMapEast(void) {
  struct EventWork *event;

  event = gEventWork;
  EV_BEGIN();
  if (FLAG_CHECK(FLAG_SCENE_TEMP_04) != 0) {
    GameFlag_Clear(0x9a3);
    GameFlag_Clear(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    GameFlag_Clear(0x9a4);
    GameFlag_Clear(0x9a6);
    FLAG_SET(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    FLAG_SET(0x9a4);
  }
  Engine_EventRequestExit(event->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  EV_END();
}

/// Walking into the pillar in GondowanCave from the west or the
/// east (triggers 10 and 11): look at what is ahead, which starts the push.
static void Event_CheckAhead(void) { Engine_LeaderCheckAhead(); }

/// Move on the stump in GondowanCave, until it has leapt: from
/// column 30 or less it spins and leaps across, freeing the cell where it
/// stood.
static void Event_MoveStump(void) {
  Object_GetById(ACTOR_STUMP);
  EV_BEGIN();
  {
    struct FieldActor *actor = Object_GetById(ACTOR_STUMP);

    if ((actor->x.fixed >> 20) <= 30) {
      GondowanCave_RunSpinningLeap(ACTOR_STUMP);
      Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
      Engine_GameFlagSet(FLAG_GONDOWAN_CAVE_STUMP_MOVED);
    }
  }
  EV_END();
}

/// Pushing the pillar in GondowanCave (trigger 50): it moves a
/// step, then may have reached its place.
static void Event_PushPillar(void) {
  StagedActor_AdvancePair();
  Event_MovePillar();
}

/// Hides the pillar and shows the actor that stands in for it in its place.
static inline void GondowanCave_PlaceDroppedPillar(s32 animation) {
  OBJ_POS_SET(ACTOR_PILLAR, 0, 0);
  OBJ_POS_SET(ACTOR_DROPPED_PILLAR, 696, 288);
  OBJ_MODE(ACTOR_DROPPED_PILLAR, animation);
}

/// Move on the pillar in GondowanCave, and after each push: past
/// column 42 it drops into its place for good.
static void Event_MovePillar(void) {
  EV_BEGIN();
  if ((Object_GetById(ACTOR_PILLAR)->x.fixed >> 20) > 42) {
    s32 destinationCellX = 107;
    s32 destinationCellZ = 17;

    Map_CopyCellAttributes(108, 17, 1, 1, destinationCellX, destinationCellZ);
    WAIT(8);
    GondowanCave_PlaceDroppedPillar(3);
    SE(SE_CAST);
    FLAG_SET(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
  }
  EV_END();
}

/// Unused: an empty function nothing calls.
static UNUSED void GondowanCave_UnusedActorCallback(void) {}

/// Rotates the sprite during the airborne part of the leap.
static void GondowanCave_RotateLeapingObject(union FieldObject *fieldObject) {
  fieldObject->actor.sprite->rotation -= 0x400;
}

/// Waits at most forty frames for the actor to descend to the height limit.
static void GondowanCave_WaitForDescent(struct FieldActor *actor,
                                        s32 heightLimit) {
  s32 framesRemaining = 40;

  while (framesRemaining != 0) {
    Engine_TaskWait(1);
    framesRemaining--;
    if (actor->y.fixed <= heightLimit) {
      break;
    }
  }
}

/// The object spins in place, leaps and lands with a shake and a ring of dust,
/// then comes to rest a little further on.
static void GondowanCave_RunSpinningLeap(s32 actorId) {
  struct FieldActor *actor;
  u32 stepIndex;
  struct Vector3 particleVelocity;
  s32 angle;

  actor = Object_GetById(actorId);
  // Wind up the spin while horizontal motion is disabled.
  actor->motionFlags = 0;
  for (stepIndex = 0; stepIndex < 9; stepIndex++) {
    Engine_TaskWait(1);
    actor->sprite->rotation -= 0x100;
    actor->x.fixed -= Engine_MathCos(actor->sprite->rotation) / 2;
    actor->targetX = ACTOR_NO_TARGET;
  }
  actor->update = GondowanCave_RotateLeapingObject;
  // Continue spinning through the first jump.
  Engine_AudioPlayCue(SE_136);
  OBJ_SPEED(actorId, PIXELS(2), PIXELS(1));
  Actor_SetDestination(actorId, 472, 288);
  actor->gravity = 4 * PIXELS(1) / 5;
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->terrainId = 0;
  Engine_ActorWaitForMove(actorId);
  GondowanCave_WaitForDescent(actor, PIXELS(32));
  Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  // Emit a complete particle fan, including its closing direction.
  for (stepIndex = 0; stepIndex < 17; stepIndex++) {
    angle = stepIndex << 12;
    particleVelocity.x = Engine_MathCos(angle);
    particleVelocity.y = 0;
    particleVelocity.z = Engine_MathSin(angle);
    particleVelocity.x -= particleVelocity.x / 4;
    particleVelocity.z -= particleVelocity.z / 2;
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                 particleVelocity.x, particleVelocity.y, particleVelocity.z, 0,
                 0);
  }
  Actor_SetDestination(actorId, 440, 308);
  // Finish the second movement before restoring the resting animation.
  Engine_ActorWaitForMove(actorId);
  GondowanCave_WaitForDescent(actor, PIXELS(32));
  actor->update = 0;
  actor->sprite->rotation = 0x1000;
  Engine_AudioPlayCue(SE_CAST);
  OBJ_MODE(actorId, 3);
  Map_WaitForShakeToSettle();
  WAIT(10);
  OBJ_MODE(actorId, 2);
}

/// Unused: an empty function nothing calls.
static UNUSED void GondowanCave_UnusedSceneCallback(void) {}

/// Lift on the boulder in GondowanCave also takes away the
/// invisible actor beside it.
static void Event_LiftBoulder(void) {
  Engine_ActorSetPosition(ACTOR_BOULDER_SIDE, 0, 0);
}

/// Sets up the scene as the party enters. Gondowan Passage blocks Retreat and
/// keeps Iodem out of sight while he travels with the party; Gondowan Cave's
/// GondowanCave puts the stump and the pillar where the party left them.
s32 Scene_Initialize(void) {
  struct FieldActor *actor;

  if (gGameState.scene == (s32)&SceneId_GondowanPassage) {
    FLAG_SET(FLAG_RETREAT_BLOCKED);
    if (FLAG_CHECK(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
      OBJ_POS_SET(ACTOR_IODEM, 0, 0);
    }
  }
  if (gGameState.scene == (s32)&SceneId_GondowanCave) {
    if (gGameState.entrance == GONDOWAN_CAVE_FROM_WORLD_MAP_EAST) {
      Map_CopyCellAttributes(108, 17, 1, 1, 107, 17);
    }
    if (FLAG_CHECK(FLAG_GONDOWAN_CAVE_STUMP_MOVED) != 0) {
      OBJ_POS_SET(ACTOR_STUMP, 440, 308);
      OBJ_MODE(ACTOR_STUMP, 2);
      Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
    }
    if (FLAG_CHECK(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED) != 0) {
      GondowanCave_PlaceDroppedPillar(2);
    }
    actor = Actor_Get(ACTOR_BOULDER_SIDE);
    Engine_ActorSetSpriteFlags(actor, 0);
  }
  return 0;
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

/// Packs signed pixel steps with X in the upper halfword and Z in the lower.
/// Shift the unsigned representation so westward steps remain defined C.
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

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
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
    { GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVERN, CONDITION_ALWAYS, 344, 0, 584, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
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
    SCENE_EXIT(GONDOWAN_CAVE_TO_WEST_PASSAGE, GondowanCave, GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVERN),
    SCENE_EXIT(GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVERN, GondowanCave, GONDOWAN_CAVE_FROM_WEST_PASSAGE),
    SCENE_EXIT(GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_SOUTH),
    SCENE_EXIT(GONDOWAN_CAVE_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_GONDOWAN_CAVE_SOUTH),
    SCENE_EXITS_END,
};

struct ScenePlacement gGondowanCavePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gGondowanPassagePlacements[] = {
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(144), 0, PIXELS(168), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gGondowanPassagePlacementsWithIodem[] = {
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    // Iodem
    { SPRITE_IODEM, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gGondowanCaveRiverPlacements[] = {
    { 0x127, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(624), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gGondowanCavePlacements[] = {
    { SPRITE_STUMP, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(496), 0, PIXELS(304), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STUMP, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x127, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(636), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(640), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gGondowanCaveEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gGondowanPassageEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH },
    { EVENT_TALK, ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SoldierAsksAboutCrossing) },
    { EVENT_TALK, ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveUsWereTheSoldiersWhoGuard) },
    { EVENT_TALK, ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouHeadEastYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveThoseWhoGoToGondowanRarely) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadSoldierMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveYoudThinkYouCouldGetThrough) },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(ITEM_APPLE) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gGondowanPassageEventsWithIodem[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IodemAsksWhyTurningBack) },
    { EVENT_TOUCH, 11, FLAG_IODEM_FOLLOWING_PARTY, EVENT_SCRIPT(Event_IodemRejoins) },
    { EVENT_TALK, ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWhyWouldIodemChooseToWait) },
    { EVENT_TALK, ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaventSeenAnybodyUnusualCrossing) },
    { EVENT_TALK, ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouGoEastFrom) },
    { EVENT_TALK, ACTOR_IODEM, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IodemRejoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveIodemIsGoingToBabiLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveItsALotOfStressBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEveryoneComesDownThisWayWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaveToHeadForThe) },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(ITEM_APPLE) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gGondowanCaveRiverEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_BOULDER_PASSAGE },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_TO_RIVER },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_SOUTH },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_RIVER_BOULDER, CONDITION_ALWAYS, 0 },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(ITEM_APPLE) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

/// FLAG_SCENE_TEMP_05 ends Move on the pillar.
struct SceneEvent gGondowanCaveEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToWorldMapEast) },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WEST_PASSAGE },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVERN },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WORLD_MAP_WEST },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftBoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GONDOWAN_CAVE_STUMP_MOVED, EVENT_SCRIPT(Event_MoveStump) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, FLAG_SCENE_TEMP_05, EVENT_SCRIPT(Event_MovePillar) },
    { TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 50, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(ITEM_APPLE) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};
// clang-format on
