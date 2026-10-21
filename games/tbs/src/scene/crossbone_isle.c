/// @file
/// Crossbone Isle, an island reached from the world map, and the main line of
/// the cave beneath it, in three places by their area names:
/// Crossbone Isle, Crossbone Isle Cave and the Pirate Ship.
///
/// No one on the island speaks: it is a run of Psynergy puzzles and battles
/// that ends with Deadbeard on the Pirate Ship. Its places are met in this
/// order:
///
/// - Crossbone Isle (SceneId_CrossboneIsle), the island itself, reached
///   from the world map (WORLD_MAP_TO_CROSSBONE_ISLE;
///   CROSSBONE_ISLE_TO_WORLD_MAP leads back to it). Whirlwind blows four ivies
///   away for good (four flags from FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY,
///   the first also opening a cell), Growth grows four plants so they can be
///   crossed, for this visit only (FLAG_SCENE_TEMP_40 to _43), two pillars can
///   be pushed, and a Nut circles until Catch takes it (0xfd7).
///   CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE leads down into the cave.
/// - Crossbone Isle Cave (SceneId_CrossboneIsleCaveEntrance), the cave's
///   mouth, with only its two exits.
/// - Crossbone Isle Cave (SceneId_CrossboneIsleFirstFloor to
///   SceneId_CrossboneIsleNinthFloor), the nine floors the cave descends
///   through, all on one map, each entered from the east
///   (CORRIDOR_FROM_PREVIOUS_ROOM) and left to the west
///   (CORRIDOR_TO_NEXT_ROOM). Every room numbers its doors alike, and
///   scene_links.h names them room by room
///   (CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE and
///   CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR in the first).
///   Each has two doors in its north wall into a side room of its own. The
///   first time the party walks into the east door (Event_OpenEastDoor), the
///   monsters behind it attack: encounter table 63 for the first four rooms and
///   84 for the next five, one group per room. Back from the battle
///   (ENTRANCE_AFTER_BATTLE) the door opens and the party walks through, and
///   from then on it simply opens (one flag per room from
///   FLAG_CROSSBONE_ISLE_FIRST_GUARDS_DEFEATED); the west door always opens.
///   Between the doors stands a pillar that, pushed or moved with Move to
///   column 40, sinks into the floor and opens the corridor (one flag per room
///   from FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED). In the first floor a chest
///   is a mimic (0xef4). The side rooms of the first eight floors are in
///   crossbone_isle_stepping_stones.c and crossbone_isle_keys.c.
/// - Crossbone Isle Cave (SceneId_CrossboneIsleFloatingLogs), the ninth
///   floor's side room: twelve logs floating on green water, which drift with
///   the leader standing on them until another log or the shore stops them, and
///   hops from log to log, lead to its four chests.
/// - Crossbone Isle Cave (SceneId_CrossboneIslePier), the ledge past the
///   ninth floor where a wooden pier leads out to the Pirate Ship.
/// - Pirate Ship (SceneId_PirateShip), at the end. Stepping on trigger
///   10 (Event_FightDeadbeard) sets off the battle with Deadbeard, encounter
///   table 84's sixth group; when the party comes back
///   (ENTRANCE_AFTER_BATTLE) FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED is set and
///   a chest with the Demon Mail appears. A second chest is a mimic (0xef7).
///
/// Scene_Initialize restores each scene as its flags say: the ivy and plants on
/// the island, the sliding pillars' footprints, the corridor's sunk pillars and
/// the chests the mimics leave.
#include "game_state.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/crossbone_isle.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "field_effect.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "minigame.h"
#include "sound.h"
#include "edition.h"
#include "field_service.h"
#include "call.h"
#include "scene_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "event.h"

/// Crossbone Isle's placed actors, numbered in the order of
/// gCrossboneIslePlacements1.
enum CrossboneIsleActor {
  /// The Nut that circles over the island until Catch takes it (0xfd7).
  ACTOR_ISLE_NUT = ACTOR_FIRST_PLACED,
  /// The two pillars a push moves (Event_PushIslePillar).
  ACTOR_FIRST_ISLE_PILLAR,
  /// The four plants Growth grows, crossable once grown for the
  /// rest of the visit (FLAG_SCENE_TEMP_40 to _43).
  ACTOR_FIRST_PLANT = ACTOR_FIRST_ISLE_PILLAR + 2,
  /// The ivy Whirlwind blows away, one flag each from
  /// FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY. Only three rows place one: the
  /// fourth ivy's event and CrossboneIsle_RestoreIvy name ACTOR_FIRST_IVY + 3,
  /// which the table leaves unplaced.
  ACTOR_FIRST_IVY = ACTOR_FIRST_PLANT + 4
};

/// The twelve floating logs of the ninth floor's side room
/// (SceneId_CrossboneIsleFloatingLogs), numbered in the order of
/// gCrossboneIslePlacements3, from the first to the twelfth, with their
/// shapes: a log three cells wide slides north and south, and one three cells
/// tall slides east and west.
enum CrossboneIsleFloatingLogsActor {
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED, ///< three cells wide
  ACTOR_SECOND_PILLAR,                     ///< three cells wide
  ACTOR_THIRD_PILLAR,                      ///< three cells wide
  ACTOR_FOURTH_PILLAR,                     ///< three cells tall
  ACTOR_FIFTH_PILLAR,                      ///< three cells tall
  ACTOR_SIXTH_PILLAR,                      ///< three cells tall
  ACTOR_SEVENTH_PILLAR,                    ///< three cells tall
  ACTOR_EIGHTH_PILLAR,                     ///< three cells tall
  ACTOR_NINTH_PILLAR,                      ///< three cells tall
  ACTOR_TENTH_PILLAR,                      ///< three cells wide
  ACTOR_ELEVENTH_PILLAR,                   ///< three cells wide
  ACTOR_TWELFTH_PILLAR                     ///< three cells tall
};

/// The corridor rooms' one placed actor (gCrossboneIslePlacements6To14): the
/// pillar between the doors that sinks into the floor at column 40.
enum CrossboneIsleCorridorActor { ACTOR_CORRIDOR_PILLAR = ACTOR_FIRST_PLACED };

/// The corridor rooms' entrances, which every room numbers alike, so the code
/// they share names them here; scene_links.h names each room's own
/// (CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE to
/// CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_WEST in the
/// first).
enum {
  /// At the east end, from the room before (the cave's mouth,
  /// CrossboneIsleCaveEntrance,
  /// for the first).
  CORRIDOR_FROM_PREVIOUS_ROOM = 1,
  /// At the west end, from the room after (CrossboneIslePier for the last).
  CORRIDOR_FROM_NEXT_ROOM = 2,
  CORRIDOR_FROM_SIDE_ROOM_EAST = 3, ///< out of the side room by the east door
  CORRIDOR_FROM_SIDE_ROOM_WEST = 4  ///< out of the side room by the west door
};

/// The corridor rooms' exits, numbered as their entrances.
enum {
  CORRIDOR_TO_PREVIOUS_ROOM = 1,
  CORRIDOR_TO_NEXT_ROOM = 2,
  CORRIDOR_TO_SIDE_ROOM_EAST = 3, ///< the east door (Event_OpenEastDoor)
  CORRIDOR_TO_SIDE_ROOM_WEST = 4  ///< the west door (Event_OpenWestDoor)
};

/// The entrance a battle returns the party to: in a corridor room after the
/// monsters behind its east door (Event_OpenEastDoor), and on the Pirate Ship
/// after Deadbeard (Event_FightDeadbeard).
enum { ENTRANCE_AFTER_BATTLE = 5 };

static void CrossboneIsle_RestoreSunkPillar(void);
static void CrossboneIsle_OpenEastDoorAfterBattle(void);
static void CrossboneIsle_RestoreIvy(void);
static void CrossboneIsle_DrawPillarPuzzleFootprints(void);

#include "common/object_staged_actor.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Unused: an empty function nothing calls.
UNUSED static void CrossboneIsle_UnusedCallback(void) {}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsle) {
    return gCrossboneIsleEntrances1;
  }
  if (scene == (s32)&SceneId_CrossboneIsleCaveEntrance) {
    return gCrossboneIsleEntrances2;
  }
  if (scene == (s32)&SceneId_CrossboneIsleFloatingLogs) {
    return gCrossboneIsleEntrances3;
  }
  if (scene == (s32)&SceneId_CrossboneIslePier) {
    return gCrossboneIsleEntrances4;
  }
  if (scene == (s32)&SceneId_PirateShip) {
    return gCrossboneIsleEntrances5;
  }
  return &gCrossboneIsleEntrances5[2];
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gCrossboneIsleExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsle) {
    return gCrossboneIslePlacements1;
  }
  if (scene == (s32)&SceneId_CrossboneIsleFloatingLogs) {
    return gCrossboneIslePlacements3;
  }
  if (scene <= (s32)&SceneId_CrossboneIsleNinthFloor &&
      scene >= (s32)&SceneId_CrossboneIsleFirstFloor) {
    return gCrossboneIslePlacements6To14;
  }
  return gCrossboneIslePlacementsOther;
}

#include "common/effect_advance.h"

/// Emits a ring of dust around a corridor room's pillar as it sinks.
static void CrossboneIsle_SpawnPillarMovedBurst(s32 objectId) {
  struct FieldActor *actor;
  struct Vector3 velocity;
  struct EffectOptions options;
  u32 angleStep;
  s32 angle;
  s32 velocityX;
  s32 velocityZ;

  actor = Object_GetById(objectId);
  options.update = Effect_AdvanceMotion;
  for (angleStep = 0; angleStep <= 16; angleStep += 2) {
    angle = angleStep << 12;
    velocity.x = Engine_MathCos(angle);
    velocity.y = 0;
    velocityZ = Engine_MathSin(angle);
    velocityX = velocity.x;
    velocity.z = velocityZ;
    velocityX += Math_Divide(velocityX, 3);
    velocity.x = velocityX;
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocityX,
                 velocity.y, velocityZ, 0x01000001, &options);
  }
}

/// Stepping on trigger 10 on the Pirate Ship: Deadbeard attacks (encounter
/// table 84's sixth group), and the party comes back to the ship at
/// ENTRANCE_AFTER_BATTLE.
static void Event_FightDeadbeard(void) {

  EV_BEGIN();
  Event_SetNextDestination(gGameState.scene, ENTRANCE_AFTER_BATTLE);
  Battle_SetStart(BATTLE_START_SCENE);
  Field_RequestBattleFromEncounterTable(84, 5);
  EV_END();
}

const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsle) {
    return gCrossboneIsleEvents1;
  }
  if (scene == (s32)&SceneId_CrossboneIsleCaveEntrance) {
    return gCrossboneIsleEvents2;
  }
  if (scene == (s32)&SceneId_CrossboneIsleFloatingLogs) {
    return gCrossboneIsleEvents3;
  }
  if (scene == (s32)&SceneId_CrossboneIslePier) {
    return gCrossboneIsleEvents4;
  }
  if (scene == (s32)&SceneId_PirateShip) {
    return gCrossboneIsleEvents5;
  }
  return gCrossboneIsleEventsOther;
}

/// Catch on the Nut circling on Crossbone Isle: the party gets it (0xfd7).
static void Event_CatchNut(void) {
  EV_BEGIN();
  Engine_ActorSetPosition(ACTOR_ISLE_NUT, 0, 0);
  Engine_GameFlagSet(0xfd7);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  EV_END();
}

/// Restores each scene's puzzles from its flags: the sliding pillars'
/// footprints in CrossboneIsleFloatingLogs; on the Pirate Ship the mimic's
/// chest and, after Deadbeard, the Demon Mail's; on Crossbone Isle the ivy,
/// plants and Nut; in the corridor rooms the mimic's chest, the sunk pillars
/// and the door opened after a battle.
s32 Scene_Initialize(void) {
  struct FieldActor *actor;
  s32 scene;
  s32 current;
  s32 first;

  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  // CrossboneIsleFloatingLogs's sliding pillars restore their map footprints
  // alone.
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_CrossboneIsleFloatingLogs) {
    CrossboneIsle_DrawPillarPuzzleFootprints();
    return 0;
  }
  if (scene == (s32)&SceneId_PirateShip) {
    if (FLAG_CHECK(0xef7) == 0) {
      Map_CopyCellAttributes(0, 3, 1, 1, 13, 40);
      Map_CopyCellAttributes(0, 2, 1, 1, 15, 40);
      MapObject_SetPosition(101, PIXELS(216), PIXELS(648));
    }
    if (gGameState.scene == scene) {
      if (gGameState.entrance != ENTRANCE_AFTER_BATTLE) {
        if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED) == 0) {
          return 0;
        }
      }
      FLAG_SET(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED);
      Map_CopyCellAttributes(0, 1, 1, 1, 13, 30);
      MapObject_SetPosition(100, PIXELS(216), PIXELS(488));
      return 0;
    }
  }
  current = gGameState.scene;
  if (current == (s32)&SceneId_CrossboneIsle) {
    CrossboneIsle_RestoreIvy();
    Actor_Get(ACTOR_ISLE_NUT)->targetX = PIXELS(129);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_ISLE_PILLAR);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_ISLE_PILLAR + 1);
    if (FLAG_CHECK(FLAG_SCENE_TEMP_40) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(304), PIXELS(368), 253);
    }
    if (FLAG_CHECK(FLAG_SCENE_TEMP_41) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT + 1);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(80), PIXELS(368), 253);
    }
    if (FLAG_CHECK(FLAG_SCENE_TEMP_42) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT + 2);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(96), PIXELS(336), 253);
    }
    if (FLAG_CHECK(FLAG_SCENE_TEMP_43) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT + 3);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(144), PIXELS(320), 253);
      Field_SetMapCellCollision(0, PIXELS(752), PIXELS(320), 253);
    }
    if (FLAG_CHECK(0xfd7) != 0) {
      return 0;
    }
    Scene_FloatNut(ACTOR_ISLE_NUT);
    return 0;
  }
  // The nine corridor rooms (CrossboneIsleFirstFloor to
  // CrossboneIsleNinthFloor) share their door battle, sunk pillar and return
  // entrance.
  first = (s32)&SceneId_CrossboneIsleFirstFloor;
  if (current == first) {
    if (FLAG_CHECK(0xef4) == 0) {
      Map_CopyCellAttributes(0, 0, 1, 1, 37, 10);
      MapObject_SetPosition(100, PIXELS(600), PIXELS(168));
    }
  }
  current = gGameState.scene;
  if (current >= first) {
    if (current <= (s32)&SceneId_CrossboneIsleNinthFloor) {
      CrossboneIsle_RestoreSunkPillar();
      if (gGameState.entrance == ENTRANCE_AFTER_BATTLE) {
        CrossboneIsle_OpenEastDoorAfterBattle();
      }
    }
  }
  return 0;
}

/// In a corridor room of Crossbone Isle Cave whose pillar has been sunk, it
/// stays sunk in its place.
static void CrossboneIsle_RestoreSunkPillar(void) {
  struct FieldActor *actor;
  s32 cleared;

  actor = Actor_Get(ACTOR_CORRIDOR_PILLAR);
  cleared =
      FLAG_CHECK((gGameState.scene + (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                                      (s32)&SceneId_CrossboneIsleFirstFloor)));
  if (cleared != 0) {
    OBJ_POS_SET(ACTOR_CORRIDOR_PILLAR, 650, 168);
    actor->y.fixed = -PIXELS(32);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_CORRIDOR_PILLAR), 0);
    Engine_ActorSetSpritePriority(ACTOR_CORRIDOR_PILLAR, 3);
    actor->motionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Map_CopyCellAttributes(42, 10, 1, 1, 40, 10);
  } else {
    Object_GetById(ACTOR_CORRIDOR_PILLAR)->motionFlags = cleared;
  }
}

/// Move on a corridor room's pillar in Crossbone Isle Cave, and after each
/// push: once it reaches column 40 it sinks into the floor in a ring of dust,
/// and the corridor is open (a flag per room from
/// FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED).
static void Event_MovePillar(void) {
  struct FieldActor *pillar;
  s16 *sceneNumber;
  struct GameState *state;
  s32 column;

  pillar = Object_GetById(ACTOR_CORRIDOR_PILLAR);
  column = pillar->x.fixed >> 20;
  if (column != 40) {
    return;
  }

#if TBS_PATCH_1
  // Through state, the scene's address is built as the game state's plus an
  // offset, as the bytes have it; &gGameState.scene folds into one literal.
  state = &gGameState;
  sceneNumber = &state->scene;
  if (Engine_GameFlagIsSet(*sceneNumber +
                           (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                            (s32)&SceneId_CrossboneIsleFirstFloor)) != 0) {
    return;
  }
#endif

  // Play the moved-pillar reaction before hiding it and opening the passage.
  pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;

  WAIT(8);
  CrossboneIsle_SpawnPillarMovedBurst(ACTOR_CORRIDOR_PILLAR);
  Engine_AudioPlayCue(SE_136);
  WAIT(40);

  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_CORRIDOR_PILLAR), 0);
  Engine_ActorSetSpritePriority(ACTOR_CORRIDOR_PILLAR, 3);

  pillar->motionFlags = 0;
  pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;

  // Replace the blocked cell and remember that the room's pillar is sunk.
  Map_CopyCellAttributes(42, 10, 1, 1, column, 10);

#if !TBS_PATCH_1
  state = &gGameState;
  sceneNumber = &state->scene;
#endif
  Engine_GameFlagSet(*sceneNumber + (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                                     (s32)&SceneId_CrossboneIsleFirstFloor));
}

/// Walking into a corridor room's pillar in Crossbone Isle Cave (trigger 20):
/// push it a step.
static void Event_PushPillar(void) {
  EV_BEGIN();
  StagedActor_AdvancePair();
  EV_END();
  Event_MovePillar();
}

/// Walking north into a corridor room's west door in Crossbone Isle Cave
/// (trigger 4): it opens, and the party goes through to the side room.
static void Event_OpenWestDoor(void) {
  Engine_MapAnimateCells(gCrossboneIsle_EntranceCells, 37, 7);
  Engine_AudioPlayCue(SE_183);
  Engine_EventRequestExit(CORRIDOR_TO_SIDE_ROOM_WEST);
}

/// Walking north into a corridor room's east door in Crossbone Isle Cave
/// (trigger 3): the first time, the monsters behind it attack, and the party
/// comes back at ENTRANCE_AFTER_BATTLE; once they are beaten, the door opens
/// onto the side room.
static void Event_OpenEastDoor(void) {
  if (Engine_GameFlagIsSet(gGameState.scene +
                           (FLAG_CROSSBONE_ISLE_FIRST_GUARDS_DEFEATED -
                            (s32)&SceneId_CrossboneIsleFirstFloor)) == 0) {
    EV_BEGIN();
    Event_SetNextDestination(gGameState.scene, ENTRANCE_AFTER_BATTLE);
    Battle_SetStart(BATTLE_START_SCENE);
    // The first four rooms use encounter table 63; the next five use 84.
    switch (gGameState.scene - (s32)&SceneId_CrossboneIsleFirstFloor) {
    case 0:
      Field_RequestBattleFromEncounterTable(63, 0);
      break;
    case 1:
      Field_RequestBattleFromEncounterTable(63, 1);
      break;
    case 2:
      Field_RequestBattleFromEncounterTable(63, 2);
      break;
    case 3:
      Field_RequestBattleFromEncounterTable(63, 3);
      break;
    case 4:
      Field_RequestBattleFromEncounterTable(84, 0);
      break;
    case 5:
      Field_RequestBattleFromEncounterTable(84, 1);
      break;
    case 6:
      Field_RequestBattleFromEncounterTable(84, 2);
      break;
    case 7:
      Field_RequestBattleFromEncounterTable(84, 3);
      break;
    case 8:
      Field_RequestBattleFromEncounterTable(84, 4);
      break;
    }
    EV_END();
  } else {
    Engine_MapAnimateCells(gCrossboneIsle_EntranceCells, 44, 7);
    Engine_AudioPlayCue(SE_183);
    Engine_EventRequestExit(CORRIDOR_TO_SIDE_ROOM_EAST);
  }
}

/// Back from the battle at a corridor room's east door
/// (ENTRANCE_AFTER_BATTLE): the door opens, and the party walks through.
static void CrossboneIsle_OpenEastDoorAfterBattle(void) {
  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  FLAG_SET((gGameState.scene + (FLAG_CROSSBONE_ISLE_FIRST_GUARDS_DEFEATED -
                                (s32)&SceneId_CrossboneIsleFirstFloor)));
  WAIT(30);
  Map_AnimateCells(gCrossboneIsle_EntranceCells, 44, 7);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Engine_EventRequestExit(CORRIDOR_TO_SIDE_ROOM_EAST);
  EV_END();
}

/// Growth on Crossbone Isle's first plant: it grows, and its cell can be
/// crossed.
static void Event_GrowFirstPlant(s32 actorId) {
  s32 fixedX = PIXELS(304);
  s32 fixedZ = PIXELS(368);
  struct FieldActor *actor = Object_GetById(ACTOR_FIRST_PLANT);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_40);
}

/// Growth on Crossbone Isle's second plant.
static void Event_GrowSecondPlant(s32 actorId) {
  s32 fixedX = PIXELS(80);
  s32 fixedZ = PIXELS(368);
  struct FieldActor *actor = Object_GetById(ACTOR_FIRST_PLANT + 1);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_41);
}

/// Growth on Crossbone Isle's third plant.
static void Event_GrowThirdPlant(s32 actorId) {
  s32 fixedX = PIXELS(96);
  s32 fixedZ = PIXELS(336);
  struct FieldActor *actor = Object_GetById(ACTOR_FIRST_PLANT + 2);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_42);
}

/// Growth on Crossbone Isle's fourth plant, which opens two cells.
static void Event_GrowFourthPlant(s32 actorId) {
  s32 fixedX = PIXELS(144);
  s32 fixedZ = PIXELS(320);
  s32 secondFixedX = PIXELS(752);
  s32 secondFixedZ = PIXELS(320);
  struct FieldActor *actor = Object_GetById(ACTOR_FIRST_PLANT + 3);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  SetMapCellCollision(0, secondFixedX, secondFixedZ, 253);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_43);
}

/// Whirlwind on Crossbone Isle's first ivy: it is blown away for good, and
/// its cell opens.
static void Event_BlowAwayFirstIvy(void) {
  Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY);
  {

    Map_CopyCellAttributes(0, 0, 1, 1, 8, 21);
  }
}

/// Whirlwind on Crossbone Isle's second ivy.
static void Event_BlowAwaySecondIvy(void) {
  Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1);
}

/// Whirlwind on Crossbone Isle's third ivy.
static void Event_BlowAwayThirdIvy(void) {
  Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2);
}

/// Whirlwind on Crossbone Isle's fourth ivy.
static void Event_BlowAwayFourthIvy(void) {
  Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3);
}

/// On Crossbone Isle, ivy blown away stays gone; the rest stands tall.
static void CrossboneIsle_RestoreIvy(void) {
  struct FieldActor *actor;

  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY) != 0) {
    Map_CopyCellAttributes(0, 0, 1, 1, 8, 21);
    OBJ_POS_SET(ACTOR_FIRST_IVY, 968, 968);
  } else {
    actor = Actor_Get(ACTOR_FIRST_IVY);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1) != 0) {
    OBJ_POS_SET(ACTOR_FIRST_IVY + 1, 968, 968);
  } else {
    actor = Actor_Get(ACTOR_FIRST_IVY + 1);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2) != 0) {
    OBJ_POS_SET(ACTOR_FIRST_IVY + 2, 968, 968);
  } else {
    actor = Actor_Get(ACTOR_FIRST_IVY + 2);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3) != 0) {
    OBJ_POS_SET(ACTOR_FIRST_IVY + 3, 968, 968);
  } else {
    actor = Actor_Get(ACTOR_FIRST_IVY + 3);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
}

/// Walking into one of Crossbone Isle's two pillars: push it if the cell beyond
/// is clear.
static void Event_PushIslePillar(void) {
  struct StagedActorProbe probe;

  EV_BEGIN();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
  }
  EV_END();
}

/// Moves the selected actor and puzzle pillar by the same pixel offset.
static void CrossboneIsle_MoveSelectedActorAndPillar(s32 actorId, s32 xOffset,
                                                     s32 zOffset) {
  struct FieldActor *selectedActor;
  struct FieldActor *pillar;
  s32 fixedX;
  s32 fixedZ;

  selectedActor = Object_GetById(gGameState.selectedActor);
  pillar = Object_GetById(actorId);
  EV_BEGIN();
  {
    fixedX = ((selectedActor->x.fixed + (xOffset * PIXELS(1))) & 0xFFF00000) +
             PIXELS(8);
    fixedZ = ((selectedActor->z.fixed + (zOffset * PIXELS(1))) & 0xFFF00000) +
             PIXELS(8);

    selectedActor->speed = PIXELS(1);
    selectedActor->acceleration = PIXELS(1) / 2;
    Engine_ObjectSetPosition(selectedActor, fixedX, selectedActor->y.fixed,
                             fixedZ);
  }
  // Position the pillar on the same cell-center grid as the selected actor.
  Object_SetMode(selectedActor, 27);
  {
    fixedX =
        ((pillar->x.fixed + (xOffset * PIXELS(1))) & 0xFFF00000) + PIXELS(8);
    fixedZ =
        ((pillar->z.fixed + (zOffset * PIXELS(1))) & 0xFFF00000) + PIXELS(8);

    pillar->speed = PIXELS(1);
    pillar->acceleration = PIXELS(1) / 2;
    Engine_ObjectSetPosition(pillar, fixedX, pillar->y.fixed, fixedZ);
  }
  // The pillar's mode depends on which side of the grid it moves toward.
  if (xOffset < 0 || zOffset < 0) {
    Object_SetMode(pillar, 4);
  } else {
    Object_SetMode(pillar, 3);
  }
  Engine_AudioPlayCue(SE_226);
  FieldObject_WaitForMove(selectedActor);
  Engine_AudioPlayCue(SE_288);
  EV_END();
}

/// Draws the three-cell horizontal footprint centered on this pillar.
static void CrossboneIsle_DrawHorizontalPillarFootprint(s32 actorId,
                                                        s32 sourceColumn,
                                                        s32 sourceRow,
                                                        s32 unused) {
  struct FieldActor *actor = Object_GetById(actorId);

  if (actor != 0) {
    s32 priorityFlags;

    Engine_ActorSetSpritePriority(actorId, 3);
    priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
    {
      s32 row = actor->z.fixed >> 20;
      s32 column = (actor->x.fixed >> 20) - 1;

      Map_CopyCellAttributes(sourceColumn, sourceRow, 3, 1, column, row);
    }
  }
}

/// Draws the three-cell vertical footprint centered on this pillar.
static void CrossboneIsle_DrawVerticalPillarFootprint(s32 actorId,
                                                      s32 sourceColumn,
                                                      s32 sourceRow,
                                                      s32 unused) {
  struct FieldActor *actor = Object_GetById(actorId);

  if (actor != 0) {
    s32 priorityFlags;

    Engine_ActorSetSpritePriority(actorId, 3);
    priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
    {
      s32 column = actor->x.fixed >> 20;
      s32 row = (actor->z.fixed >> 20) - 1;

      Map_CopyCellAttributes(sourceColumn, sourceRow, 1, 3, column, row);
    }
  }
}

/// Runs the two animation changes used when touching a puzzle pillar.
static void CrossboneIsle_AnimateTouchedPillar(int actorId) {
  OBJ_MODE(actorId, 1);
  OBJ_MODE(actorId, 2);
}

/// Draws all twelve pillar footprints in Crossbone Isle Cave
/// (CrossboneIsleFloatingLogs).
static void CrossboneIsle_DrawPillarPuzzleFootprints(void) {
  CrossboneIsle_DrawHorizontalPillarFootprint(ACTOR_FIRST_PILLAR, 17, 30, 21);
  CrossboneIsle_DrawHorizontalPillarFootprint(ACTOR_THIRD_PILLAR, 17, 31, 22);
  CrossboneIsle_DrawVerticalPillarFootprint(ACTOR_FOURTH_PILLAR, 20, 30, 23);
  CrossboneIsle_DrawVerticalPillarFootprint(ACTOR_FIFTH_PILLAR, 21, 30, 24);
  CrossboneIsle_DrawVerticalPillarFootprint(ACTOR_SIXTH_PILLAR, 22, 30, 25);
  CrossboneIsle_DrawVerticalPillarFootprint(ACTOR_EIGHTH_PILLAR, 23, 30, 26);
  CrossboneIsle_DrawHorizontalPillarFootprint(ACTOR_TENTH_PILLAR, 0, 30, 31);
  CrossboneIsle_DrawHorizontalPillarFootprint(ACTOR_ELEVENTH_PILLAR, 0, 31, 32);
  CrossboneIsle_DrawHorizontalPillarFootprint(ACTOR_SECOND_PILLAR, 0, 32, 33);
  CrossboneIsle_DrawVerticalPillarFootprint(ACTOR_TWELFTH_PILLAR, 4, 30, 34);
  CrossboneIsle_DrawVerticalPillarFootprint(ACTOR_SEVENTH_PILLAR, 5, 30, 35);
  CrossboneIsle_DrawVerticalPillarFootprint(ACTOR_NINTH_PILLAR, 6, 30, 36);
}

// Crossbone Isle Cave (CrossboneIsleFloatingLogs), the ninth floor's side room:
// its twelve floating logs. Stepping onto a log plays its animation from the
// start; walking onto one in a direction it moves slides it that way with the
// leader on it until another pillar or the wall stops it; walking against a
// pillar's edge hops the leader 32 pixels if the landing is clear.

/// Stepping onto the first pillar.
static void Event_StepOnFirstPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_FIRST_PILLAR);
}

/// Stepping onto the third pillar.
static void Event_StepOnThirdPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_THIRD_PILLAR);
}

/// Stepping onto the fourth pillar.
static void Event_StepOnFourthPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_FOURTH_PILLAR);
}

/// Stepping onto the fifth pillar.
static void Event_StepOnFifthPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_FIFTH_PILLAR);
}

/// Stepping onto the sixth pillar.
static void Event_StepOnSixthPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_SIXTH_PILLAR);
}

/// Stepping onto the eighth pillar.
static void Event_StepOnEighthPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_EIGHTH_PILLAR);
}

/// Stepping onto the tenth pillar.
static void Event_StepOnTenthPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_TENTH_PILLAR);
}

/// Stepping onto the eleventh pillar.
static void Event_StepOnEleventhPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_ELEVENTH_PILLAR);
}

/// Stepping onto the second pillar.
static void Event_StepOnSecondPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_SECOND_PILLAR);
}

/// Stepping onto the twelfth pillar.
static void Event_StepOnTwelfthPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_TWELFTH_PILLAR);
}

/// Stepping onto the seventh pillar.
static void Event_StepOnSeventhPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_SEVENTH_PILLAR);
}

/// Stepping onto the ninth pillar.
static void Event_StepOnNinthPillar(void) {
  CrossboneIsle_AnimateTouchedPillar(ACTOR_NINTH_PILLAR);
}

/// Tests a step, then moves the leader with the pillar-pushing animation.
static s32 CrossboneIsle_TryHop(struct FieldActor *actor,
                                struct FieldPosition *requested) {
  u8 *motionFlags = &actor->motionFlags;
  s32 savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  struct FieldPosition destination;

  destination.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
  destination.y = actor->y.fixed;
  destination.z = (actor->z.fixed & 0xfff00000) + PIXELS(8);
  {
    s32 direction = (actor->facing + 0x2000) & 0xc000;

    Vector_AddPolarOffset(PIXELS(32), direction, &destination);
  }
  // Keep the original motion flags while playing the leader's push step.
  if (FieldObject_CheckMovementCollision(actor, &destination) == 0) {
#else
  if (FieldObject_CheckMovementCollision(actor, requested) == 0) {
#endif
    s32 movingFlags;

    EV_BEGIN();
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(actor, 7);
    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    movingFlags = 126;
    movingFlags &= *motionFlags;
    *motionFlags = (u8)movingFlags;
    Engine_ActorSetSpriteFlags(actor, 0);
    {
#if TBS_PATCH_1
      s16 *pixelCoordinates = (s16 *)&destination;
#else
      s16 *pixelCoordinates = (s16 *)requested;
#endif

      Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, pixelCoordinates[1],
                                pixelCoordinates[5]);
    }
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    *motionFlags = (u8)savedMotionFlags;
    EV_END();
    return 1;
  }
  return 0;
}

/// Walking north against a pillar's edge.
static s32 Event_HopNorth(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + -PIXELS(32);
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking south against a pillar's edge, or at the Pirate Ship's trigger 20.
static s32 Event_HopSouth(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + PIXELS(32);
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking west against a pillar's edge.
static s32 Event_HopWest(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + -PIXELS(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking east against a pillar's edge.
static s32 Event_HopEast(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + PIXELS(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking north onto the first pillar.
static void Event_SlideFirstPillarNorth(void) {
  s32 actorX = Object_GetById(ACTOR_FIRST_PILLAR)->x.fixed >> 20;
  s32 actorZ = Object_GetById(ACTOR_FIRST_PILLAR)->z.fixed >> 20;
  s32 fifthPillarColumn = Object_GetById(ACTOR_FIFTH_PILLAR)->x.fixed >> 20;
  s32 eighthPillarColumn = Object_GetById(ACTOR_EIGHTH_PILLAR)->x.fixed >> 20;

  if (actorZ == 19) {
    if (fifthPillarColumn == 24) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, -80);
    } else if (eighthPillarColumn == 24) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, -112);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, -80);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, -112);
    }
  } else if (actorZ == 14) {
    if (fifthPillarColumn == 24) {
      return;
    }
    if (eighthPillarColumn == 24) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, -64);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, -112);
    }
  } else if (actorZ == 10) {
    if (eighthPillarColumn == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, -48);
  } else {
    Event_HopNorth();
    return;
  }
  Engine_TaskWait(2);
  {
    s32 routeEndZ = Object_GetById(ACTOR_FIRST_PILLAR)->z.fixed >> 20;
    s32 routeX = actorX - 1;

    Map_CopyCellAttributes(routeX, actorZ, 3, 1, routeX, routeEndZ);
  }
  Map_CopyCellAttributes(0, 0, 3, 1, actorX - 1, actorZ);
}

/// Copies a horizontal pillar's old footprint to its new row, then clears the
/// old row.
static inline void
CrossboneIsle_RedrawHorizontalPillarRoute(s32 actorId, s32 column, s32 row) {
  Engine_TaskWait(2);
  {
    s32 destinationRow = Object_GetById(actorId)->z.fixed >> 20;
    s32 footprintColumn = column - 1;

    Map_CopyCellAttributes(footprintColumn, row, 3, 1, footprintColumn,
                           destinationRow);
  }
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Copies a vertical pillar's old footprint to its new column, then clears the
/// old one.
static inline void
CrossboneIsle_RedrawVerticalPillarRoute(s32 actorId, s32 column, s32 row) {
  Engine_TaskWait(2);
  {
    s32 destinationColumn = Object_GetById(actorId)->x.fixed >> 20;
    s32 footprintRow = row - 1;

    Map_CopyCellAttributes(column, footprintRow, 1, 3, destinationColumn,
                           footprintRow);
  }
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking south onto the first pillar.
static void Event_SlideFirstPillarSouth(void) {
  s32 column = Object_GetById(ACTOR_FIRST_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_FIRST_PILLAR)->z.fixed >> 20;
  s32 fifthPillarColumn = Object_GetById(ACTOR_FIFTH_PILLAR)->x.fixed >> 20;

  if (row == 7) {
    if (fifthPillarColumn == 24) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, 80);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, 112);
    }
  } else if (row == 10) {
    if (fifthPillarColumn == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, 144);
  } else if (row == 14) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIRST_PILLAR, 0, 80);
  } else {
    return;
  }
  CrossboneIsle_RedrawHorizontalPillarRoute(ACTOR_FIRST_PILLAR, column, row);
}

/// Walking north onto the third pillar.
static void Event_SlideThirdPillarNorth(void) {
  s32 column = Object_GetById(ACTOR_THIRD_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_THIRD_PILLAR)->z.fixed >> 20;
  s32 sixthPillarColumn = Object_GetById(ACTOR_SIXTH_PILLAR)->x.fixed >> 20;
  s32 eighthPillarColumn = Object_GetById(ACTOR_EIGHTH_PILLAR)->x.fixed >> 20;

  if (row == 18) {
    if (eighthPillarColumn >= 31 && eighthPillarColumn <= 33) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_THIRD_PILLAR, 0, -128);
    } else if (sixthPillarColumn >= 31 && sixthPillarColumn <= 33) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_THIRD_PILLAR, 0, -128);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_THIRD_PILLAR, 0, -112);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_THIRD_PILLAR, 0, -64);
    }
  } else if (row == 10) {
    if (eighthPillarColumn >= 31 && eighthPillarColumn <= 33) {
      return;
    }
    if (sixthPillarColumn >= 31 && sixthPillarColumn <= 33) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_THIRD_PILLAR, 0, -48);
  } else if (row == 7) {
    return;
  }
  CrossboneIsle_RedrawHorizontalPillarRoute(ACTOR_THIRD_PILLAR, column, row);
}

/// Walking south onto the third pillar.
static void Event_SlideThirdPillarSouth(void) {
  s32 column = Object_GetById(ACTOR_THIRD_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_THIRD_PILLAR)->z.fixed >> 20;

  if (row != 18) {
    if (row == 10) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_THIRD_PILLAR, 0, 128);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_THIRD_PILLAR, 0, 112);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_THIRD_PILLAR, 0, 64);
    }
    CrossboneIsle_RedrawHorizontalPillarRoute(ACTOR_THIRD_PILLAR, column, row);
  }
}

/// Walking west onto the fourth pillar.
static void Event_SlideFourthPillarWest(void) {
  s32 column = Object_GetById(ACTOR_FOURTH_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_FOURTH_PILLAR)->z.fixed >> 20;

  if (column != 30) {
    if (column == 34) {
      if ((Object_GetById(ACTOR_THIRD_PILLAR)->z.fixed >> 20) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FOURTH_PILLAR, -64, 0);
    } else if (column == 36) {
      if ((Object_GetById(ACTOR_THIRD_PILLAR)->z.fixed >> 20) == 18) {
        CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FOURTH_PILLAR, -32, 0);
      } else {
        CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FOURTH_PILLAR, -96, 0);
      }
    }
    CrossboneIsle_RedrawVerticalPillarRoute(ACTOR_FOURTH_PILLAR, column, row);
  }
}

/// Walking east onto the fourth pillar.
static void Event_SlideFourthPillarEast(void) {
  s32 column = Object_GetById(ACTOR_FOURTH_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_FOURTH_PILLAR)->z.fixed >> 20;

  if (column != 36) {
    if (column == 30) {
      if ((Object_GetById(ACTOR_THIRD_PILLAR)->z.fixed >> 20) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FOURTH_PILLAR, 96, 0);
    } else if (column == 34) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FOURTH_PILLAR, 32, 0);
    }
    CrossboneIsle_RedrawVerticalPillarRoute(ACTOR_FOURTH_PILLAR, column, row);
  }
}

/// Walking west onto the fifth pillar.
static void Event_SlideFifthPillarWest(void) {
  s32 column = Object_GetById(ACTOR_FIFTH_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_FIFTH_PILLAR)->z.fixed >> 20;

  if (column == 36) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIFTH_PILLAR, -96, 0);
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIFTH_PILLAR, -96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIFTH_PILLAR, -96, 0);
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIFTH_PILLAR, -64, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(ACTOR_FIFTH_PILLAR, column, row);
}

/// Walking east onto the fifth pillar.
static void Event_SlideFifthPillarEast(void) {
  s32 column = Object_GetById(ACTOR_FIFTH_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_FIFTH_PILLAR)->z.fixed >> 20;

  if (column == 24) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIFTH_PILLAR, 96, 0);
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIFTH_PILLAR, 96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_FIFTH_PILLAR, 32, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(ACTOR_FIFTH_PILLAR, column, row);
}

/// Walking west onto the sixth pillar.
static void Event_SlideSixthPillarWest(void) {
  s32 column = Object_GetById(ACTOR_SIXTH_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_SIXTH_PILLAR)->z.fixed >> 20;
  s32 thirdPillarRow = Object_GetById(ACTOR_THIRD_PILLAR)->z.fixed >> 20;
  s32 eighthPillarColumn = Object_GetById(ACTOR_EIGHTH_PILLAR)->x.fixed >> 20;

  if (column == 36) {
    if (eighthPillarColumn == 34) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -16, 0);
    } else if (thirdPillarRow == 7) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -32, 0);
    } else if (eighthPillarColumn == 30) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -96, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -80, 0);
    }
  } else if (column == 35) {
    if (eighthPillarColumn == 34) {
      return;
    }
    if (thirdPillarRow == 7) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -16, 0);
    } else if (eighthPillarColumn == 30) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -80, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -80, 0);
    }
  } else if (column == 34) {
    if (thirdPillarRow == 7) {
      return;
    }
    if (eighthPillarColumn == 30) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -144, 0);
    }
  } else if (column == 31) {
    if (eighthPillarColumn == 30) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, -96, 0);
  } else if (column == 25) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(ACTOR_SIXTH_PILLAR, column, row);
}

/// Walking east onto the sixth pillar.
static void Event_SlideSixthPillarEast(void) {
  s32 column = Object_GetById(ACTOR_SIXTH_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_SIXTH_PILLAR)->z.fixed >> 20;

  Object_GetById(ACTOR_EIGHTH_PILLAR);
  if (column == 25) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, 96, 0);
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, 80, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, 80, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, 32, 0);
  } else if (column == 35) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SIXTH_PILLAR, 16, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(ACTOR_SIXTH_PILLAR, column, row);
}

/// Walking west onto the eighth pillar.
static void Event_SlideEighthPillarWest(void) {
  s32 column = Object_GetById(ACTOR_EIGHTH_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_EIGHTH_PILLAR)->z.fixed >> 20;
  s32 firstPillarRow = Object_GetById(ACTOR_FIRST_PILLAR)->z.fixed >> 20;
  s32 thirdPillarRow = Object_GetById(ACTOR_THIRD_PILLAR)->z.fixed >> 20;

  if (column == 35) {
    if (thirdPillarRow == 7) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, -16, 0);
    } else if (firstPillarRow == 7) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, -96, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, -80, 0);
    }
  } else if (column == 34) {
    if (thirdPillarRow == 7) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, -96, 0);
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, -64, 0);
  } else if (column == 33) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, -144, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, -80, 0);
  } else if (column == 30) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, -96, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(ACTOR_EIGHTH_PILLAR, column, row);
}

/// Walking east onto the eighth pillar.
static void Event_SlideEighthPillarEast(void) {
  s32 column = Object_GetById(ACTOR_EIGHTH_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_EIGHTH_PILLAR)->z.fixed >> 20;
  s32 thirdPillarRow = Object_GetById(ACTOR_THIRD_PILLAR)->z.fixed >> 20;
  s32 sixthPillarColumn = Object_GetById(ACTOR_SIXTH_PILLAR)->x.fixed >> 20;

  if (column == 24) {
    if (thirdPillarRow == 7 || sixthPillarColumn == 31) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 96, 0);
    } else if (sixthPillarColumn == 34) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 64, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 80, 0);
    } else if (sixthPillarColumn == 35) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 80, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 80, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 96, 0);
    }
  } else if (column == 30 || sixthPillarColumn == 31) {
    if (thirdPillarRow == 7) {
      return;
    }
    if (sixthPillarColumn == 34) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 48, 0);
    } else if (sixthPillarColumn == 35) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 80, 0);
    }
  } else if (column == 33) {
    if (sixthPillarColumn == 34) {
      return;
    }
    if (sixthPillarColumn == 35) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 16, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 32, 0);
    }
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_EIGHTH_PILLAR, 16, 0);
  } else if (column == 35) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(ACTOR_EIGHTH_PILLAR, column, row);
}

/// Walking north onto the tenth pillar.
static void Event_SlideTenthPillarNorth(void) {
  s32 column = Object_GetById(ACTOR_TENTH_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_TENTH_PILLAR)->z.fixed >> 20;
  s32 twelfthPillarColumn = Object_GetById(ACTOR_TWELFTH_PILLAR)->x.fixed >> 20;

  if (row == 19) {
    if (twelfthPillarColumn >= 3 && twelfthPillarColumn <= 5) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TENTH_PILLAR, 0, -16);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TENTH_PILLAR, 0, -64);
    }
  } else if (row == 18) {
    if (twelfthPillarColumn >= 3 && twelfthPillarColumn <= 5) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TENTH_PILLAR, 0, -48);
  } else if (row == 15) {
    return;
  }
  CrossboneIsle_RedrawHorizontalPillarRoute(ACTOR_TENTH_PILLAR, column, row);
}

/// Walking south onto the tenth pillar.
static void Event_SlideTenthPillarSouth(void) {
  s32 column = Object_GetById(ACTOR_TENTH_PILLAR)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_TENTH_PILLAR)->z.fixed >> 20;

  if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TENTH_PILLAR, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TENTH_PILLAR, 0, 16);
  } else if (row == 19) {
    return;
  }
  CrossboneIsle_RedrawHorizontalPillarRoute(ACTOR_TENTH_PILLAR, column, row);
}

/// These three columns form the crossing lane for the vertical pillars.
#define IS_IN_PILLAR_CROSSING_COLUMNS(x) ((x) >= 6 && (x) <= 8)

/// Walking north onto the eleventh pillar.
static void Event_SlideEleventhPillarNorth(void) {
  s32 column;
  s32 row;
  s32 twelfthPillarColumn;
  s32 seventhPillarColumn;
  s32 ninthPillarColumn;

  column = Object_GetById(ACTOR_ELEVENTH_PILLAR)->x.fixed >> 20;
  row = Object_GetById(ACTOR_ELEVENTH_PILLAR)->z.fixed >> 20;
  twelfthPillarColumn = Object_GetById(ACTOR_TWELFTH_PILLAR)->x.fixed >> 20;
  seventhPillarColumn = Object_GetById(ACTOR_SEVENTH_PILLAR)->x.fixed >> 20;
  ninthPillarColumn = Object_GetById(ACTOR_NINTH_PILLAR)->x.fixed >> 20;
  if (row == 19) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(twelfthPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -16);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(seventhPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -64);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -64);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(twelfthPillarColumn)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(seventhPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -48);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -144);
    }
  } else if (row == 15) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(seventhPillarColumn)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -96);
    }
  } else if (row == 14) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(seventhPillarColumn)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -80);
    }
  } else if (row == 12) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -48);
  } else if (row == 11) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, -32);
  } else if (row == 9) {
    return;
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(ACTOR_ELEVENTH_PILLAR)->z.fixed >>
                                   20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_PILLAR_CROSSING_COLUMNS

/// Walking south onto the eleventh pillar.
static void Event_SlideEleventhPillarSouth(void) {
  s32 column;
  s32 row;
  s32 twelfthPillarColumn;
  s32 seventhPillarColumn;

  column = Object_GetById(ACTOR_ELEVENTH_PILLAR)->x.fixed >> 20;
  row = Object_GetById(ACTOR_ELEVENTH_PILLAR)->z.fixed >> 20;
  twelfthPillarColumn = Object_GetById(ACTOR_TWELFTH_PILLAR)->x.fixed >> 20;
  seventhPillarColumn = Object_GetById(ACTOR_SEVENTH_PILLAR)->x.fixed >> 20;

  if (row == 9) {
    if ((u32)(seventhPillarColumn - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 32);
    } else if ((u32)(twelfthPillarColumn - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 80);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 64);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 96);
    }
  } else if (row == 11) {
    if ((u32)(seventhPillarColumn - 6) <= 2) {
      return;
    }
    if ((u32)(twelfthPillarColumn - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 128);
    }
  } else if (row == 12) {
    if ((u32)(twelfthPillarColumn - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 112);
    }
  } else if (row == 14) {
    if ((u32)(twelfthPillarColumn - 6) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_ELEVENTH_PILLAR, 0, 16);
  } else if (row == 19) {
    return;
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(ACTOR_ELEVENTH_PILLAR)->z.fixed >>
                                   20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_PILLAR_CROSSING_COLUMNS
#define IS_IN_PILLAR_CROSSING_COLUMNS(x) ((x) >= 9 && (x) <= 11)

/// Walking north onto the second pillar.
static void Event_SlideSecondPillarNorth(void) {
  s32 column;
  s32 row;
  s32 twelfthPillarColumn;
  s32 seventhPillarColumn;
  s32 ninthPillarColumn;

  column = Object_GetById(ACTOR_SECOND_PILLAR)->x.fixed >> 20;
  row = Object_GetById(ACTOR_SECOND_PILLAR)->z.fixed >> 20;
  twelfthPillarColumn = Object_GetById(ACTOR_TWELFTH_PILLAR)->x.fixed >> 20;
  seventhPillarColumn = Object_GetById(ACTOR_SEVENTH_PILLAR)->x.fixed >> 20;
  ninthPillarColumn = Object_GetById(ACTOR_NINTH_PILLAR)->x.fixed >> 20;
  if (row == 19) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(twelfthPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -16);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(seventhPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -64);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -80);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(twelfthPillarColumn)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(seventhPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -48);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -96);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -64);
    }
  } else if (row == 15) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(seventhPillarColumn)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -112);
    }
  } else if (row == 14) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(seventhPillarColumn)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -96);
    }
  } else if (row == 12) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -64);
  } else if (row == 11) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(ninthPillarColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, -48);
  } else if ((u32)row <= 9) {
    return;
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(ACTOR_SECOND_PILLAR)->z.fixed >>
                                   20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_PILLAR_CROSSING_COLUMNS

/// Walking south onto the second pillar.
static void Event_SlideSecondPillarSouth(void) {
  s32 column;
  s32 row;
  s32 twelfthPillarColumn;
  s32 seventhPillarColumn;
  s32 ninthPillarColumn;

  column = Object_GetById(ACTOR_SECOND_PILLAR)->x.fixed >> 20;
  row = Object_GetById(ACTOR_SECOND_PILLAR)->z.fixed >> 20;
  twelfthPillarColumn = Object_GetById(ACTOR_TWELFTH_PILLAR)->x.fixed >> 20;
  seventhPillarColumn = Object_GetById(ACTOR_SEVENTH_PILLAR)->x.fixed >> 20;
  ninthPillarColumn = Object_GetById(ACTOR_NINTH_PILLAR)->x.fixed >> 20;

  if (row == 8) {
    if ((u32)(ninthPillarColumn - 9) <= 2) {
      return;
    }
    if ((u32)(seventhPillarColumn - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, 48);
    } else {
      if ((u32)(twelfthPillarColumn - 9) > 2) {
        CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, 80);
      }
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, 96);
    }
  } else if (row == 11) {
    if ((u32)(seventhPillarColumn - 9) <= 2) {
      return;
    }
    if ((u32)(twelfthPillarColumn - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, 128);
    }
  } else if (row == 12) {
    if ((u32)(seventhPillarColumn - 9) <= 2) {
      return;
    }
    if ((u32)(twelfthPillarColumn - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, 112);
    }
  } else if (row == 14) {
    if ((u32)(twelfthPillarColumn - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SECOND_PILLAR, 0, 16);
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(ACTOR_SECOND_PILLAR)->z.fixed >>
                                   20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Walking west onto the twelfth pillar.
static void Event_SlideTwelfthPillarWest(void) {
  s32 column;
  s32 row;
  s32 tenthPillarRow;
  s32 eleventhPillarRow;
  s32 secondPillarRow;

  column = Object_GetById(ACTOR_TWELFTH_PILLAR)->x.fixed >> 20;
  row = Object_GetById(ACTOR_TWELFTH_PILLAR)->z.fixed >> 20;
  tenthPillarRow = Object_GetById(ACTOR_TENTH_PILLAR)->z.fixed >> 20;
  eleventhPillarRow = Object_GetById(ACTOR_ELEVENTH_PILLAR)->z.fixed >> 20;
  secondPillarRow = Object_GetById(ACTOR_SECOND_PILLAR)->z.fixed >> 20;
  if (column == 3) {
    return;
  }
  if (column == 13) {
    if (secondPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -16, 0);
    } else if (eleventhPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -64, 0);
    } else if (tenthPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -112, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -48, 0);
    }
  } else if (column == 6) {
    if (tenthPillarRow == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -48, 0);
  } else if (column == 5) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -32, 0);
  } else if (column == 8) {
    if (eleventhPillarRow == 15) {
      return;
    }
    if (tenthPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -80, 0);
    }
  } else if (column == 9) {
    if (eleventhPillarRow == 15) {
      return;
    }
    if (tenthPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -96, 0);
    }
  } else if (column == 12) {
    if (secondPillarRow == 15) {
      return;
    }
    if (eleventhPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -48, 0);
    } else if (tenthPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -96, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, -144, 0);
    }
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(
      column, row - 1, 1, 3,
      Object_GetById(ACTOR_TWELFTH_PILLAR)->x.fixed >> 20, row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking east onto the twelfth pillar.
static void Event_SlideTwelfthPillarEast(void) {
  s32 column;
  s32 row;
  s32 eleventhPillarRow;
  s32 secondPillarRow;

  column = Object_GetById(ACTOR_TWELFTH_PILLAR)->x.fixed >> 20;
  row = Object_GetById(ACTOR_TWELFTH_PILLAR)->z.fixed >> 20;
  eleventhPillarRow = Object_GetById(ACTOR_ELEVENTH_PILLAR)->z.fixed >> 20;
  secondPillarRow = Object_GetById(ACTOR_SECOND_PILLAR)->z.fixed >> 20;
  if (column == 3) {
    if (eleventhPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 32, 0);
    } else if (secondPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 112, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 48, 0);
    }
  } else if (column == 5) {
    if (eleventhPillarRow == 15) {
      return;
    }
    if (secondPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 128, 0);
    }
  } else if (column == 6) {
    if (secondPillarRow == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 112, 0);
    }
  } else if (column == 8) {
    if (secondPillarRow == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_TWELFTH_PILLAR, 16, 0);
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(
      column, row - 1, 1, 3,
      Object_GetById(ACTOR_TWELFTH_PILLAR)->x.fixed >> 20, row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking west onto the seventh pillar.
static void Event_SlideSeventhPillarWest(void) {
  s32 column;
  s32 row;
  s32 eleventhPillarRow;
  s32 secondPillarRow;
  s32 footprintRow;

  column = Object_GetById(ACTOR_SEVENTH_PILLAR)->x.fixed >> 20;
  row = Object_GetById(ACTOR_SEVENTH_PILLAR)->z.fixed >> 20;
  eleventhPillarRow = Object_GetById(ACTOR_ELEVENTH_PILLAR)->z.fixed >> 20;
  secondPillarRow = Object_GetById(ACTOR_SECOND_PILLAR)->z.fixed >> 20;

  if (column == 13) {
    if ((u32)(secondPillarRow - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, -16, 0);
    } else if ((u32)(eleventhPillarRow - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, -112, 0);
    }
  } else if (column == 12) {
    if ((u32)(secondPillarRow - 12) <= 2) {
      return;
    }
    if ((u32)(eleventhPillarRow - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, -96, 0);
    }
  } else if (column == 9) {
    if ((u32)(eleventhPillarRow - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, -48, 0);
  } else if (column == 8) {
    if ((u32)(eleventhPillarRow - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, -32, 0);
  } else if (column == 6) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(
      column, footprintRow, 1, 3,
      Object_GetById(ACTOR_SEVENTH_PILLAR)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east onto the seventh pillar.
static void Event_SlideSeventhPillarEast(void) {
  s32 column;
  s32 row;
  s32 eleventhPillarRow;
  s32 secondPillarRow;
  s32 footprintRow;

  column = Object_GetById(ACTOR_SEVENTH_PILLAR)->x.fixed >> 20;
  row = Object_GetById(ACTOR_SEVENTH_PILLAR)->z.fixed >> 20;
  eleventhPillarRow = Object_GetById(ACTOR_ELEVENTH_PILLAR)->z.fixed >> 20;
  secondPillarRow = Object_GetById(ACTOR_SECOND_PILLAR)->z.fixed >> 20;

  if (column == 6) {
    if ((u32)(secondPillarRow - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, 32, 0);
    } else if ((u32)(eleventhPillarRow - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, 112, 0);
    }
  } else if (column == 8) {
    if ((u32)(secondPillarRow - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, 80, 0);
  } else if (column == 9) {
    if ((u32)(secondPillarRow - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_SEVENTH_PILLAR, 16, 0);
  } else if (column == 13) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(
      column, footprintRow, 1, 3,
      Object_GetById(ACTOR_SEVENTH_PILLAR)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking west onto the ninth pillar.
static void Event_SlideNinthPillarWest(void) {
  s32 column;
  s32 row;
  s32 eleventhPillarRow;
  s32 secondPillarRow;
  s32 footprintRow;

  column = Object_GetById(ACTOR_NINTH_PILLAR)->x.fixed >> 20;
  row = Object_GetById(ACTOR_NINTH_PILLAR)->z.fixed >> 20;
  eleventhPillarRow = Object_GetById(ACTOR_ELEVENTH_PILLAR)->z.fixed >> 20;
  secondPillarRow = Object_GetById(ACTOR_SECOND_PILLAR)->z.fixed >> 20;

  if (column == 13) {
    if ((u32)(secondPillarRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, -16, 0);
    } else if ((u32)(eleventhPillarRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, -112, 0);
    }
  } else if (column == 12) {
    if ((u32)(secondPillarRow - 9) <= 2) {
      return;
    }
    if ((u32)(eleventhPillarRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, -96, 0);
    }
  } else if (column == 9) {
    if ((u32)(eleventhPillarRow - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, -48, 0);
  } else if (column == 8) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, -32, 0);
  } else if (column == 6) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(
      column, footprintRow, 1, 3,
      Object_GetById(ACTOR_NINTH_PILLAR)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east onto the ninth pillar.
static void Event_SlideNinthPillarEast(void) {
  s32 column;
  s32 row;
  s32 companionRow;

  column = Object_GetById(ACTOR_NINTH_PILLAR)->x.fixed >> 20;
  row = Object_GetById(ACTOR_NINTH_PILLAR)->z.fixed >> 20;
  companionRow = Object_GetById(ACTOR_SECOND_PILLAR)->z.fixed >> 20;

  if (column == 6) {
    if ((u32)(companionRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, 112, 0);
    }
  } else if (column == 8) {
    if ((u32)(companionRow - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndPillar(ACTOR_NINTH_PILLAR, 16, 0);
  } else if (column == 13) {
    return;
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(
      column, row - 1, 1, 3, Object_GetById(ACTOR_NINTH_PILLAR)->x.fixed >> 20,
      row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

#include "common/object_orbit.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
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

s32 gEffectScript0[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u16 gCrossboneIsle_EntranceCells[] = {
    MAP_CELL_STEP(58, 23, 1, 2, 5), MAP_CELL_STEP(59, 23, 1, 2, 5), MAP_CELL_STEPS_END,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
    (const s32 *)0xffff0000,
    (const s32 *)120,
    (const s32 *)0x40000098,
    (const s32 *)0xffff0000,
    (const s32 *)-1,
    (const s32 *)0xffff,
    (const s32 *)0xffff,
    0,
    0,
    0,
    0,
    0,
};

struct SceneEntrance gCrossboneIsleEntrances1[] = {
    { CROSSBONE_ISLE_FROM_WORLD_MAP, CONDITION_ALWAYS, 280, 0, 544, FACING_NORTH, 0, 0, 0, 512, 560, 0 },
    { CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE, CONDITION_ALWAYS, 136, 0, 360, FACING_SOUTH, 0, 0, 0, 512, 560, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances2[] = {
    { CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE, CONDITION_ALWAYS, 312, 0, 520, FACING_NORTH, 0, 16, 64, 416, 544, 0 },
    { CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE_FIRST_FLOOR, CONDITION_ALWAYS, 280, 0, 144, FACING_EAST, 0, 16, 64, 416, 544, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances3[] = {
    { CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_EAST, CONDITION_ALWAYS, 312, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_WEST, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { 99, CONDITION_ALWAYS, 272, 0, 232, FACING_SOUTH, 0, 0, 16, 640, 464, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances4[] = {
    { CROSSBONE_ISLE_PIER_FROM_CROSSBONE_ISLE_NINTH_FLOOR, CONDITION_ALWAYS, 184, 0, 136, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { CROSSBONE_ISLE_PIER_FROM_PIRATE_SHIP, CONDITION_ALWAYS, 40, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances5[] = {
    { PIRATE_SHIP_FROM_CROSSBONE_ISLE_PIER, CONDITION_ALWAYS, 312, 0, 656, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 216, 0, 544, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    // The nine corridor rooms (CrossboneIsleFirstFloor to CrossboneIsleNinthFloor) enter through this
    // shared tail, since every room numbers its entrances alike.

    { CORRIDOR_FROM_PREVIOUS_ROOM, CONDITION_ALWAYS, 840, 0, 176, FACING_WEST, 0, 432, 32, 880, 352, 0 },
    { CORRIDOR_FROM_NEXT_ROOM, CONDITION_ALWAYS, 488, 0, 176, FACING_EAST, 0, 432, 32, 880, 352, 0 },
    { CORRIDOR_FROM_SIDE_ROOM_EAST, CONDITION_ALWAYS, 712, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { CORRIDOR_FROM_SIDE_ROOM_WEST, CONDITION_ALWAYS, 600, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 712, 0, 152, FACING_NORTH, 0, 432, 32, 880, 352, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossboneIsleExits[] = {
    SCENE_EXITS(CrossboneIsle),
    SCENE_EXIT(CROSSBONE_ISLE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CROSSBONE_ISLE),
    SCENE_EXIT(CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE, CrossboneIsleCaveEntrance, CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE),
    SCENE_EXITS(CrossboneIsleCaveEntrance),
    SCENE_EXIT(CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE, CrossboneIsle, CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE),
    SCENE_EXIT(CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE),
    SCENE_EXITS(CrossboneIsleFloatingLogs),
    SCENE_EXIT(CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_FLOATING_LOGS_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_FLOATING_LOGS_WEST),
    SCENE_EXITS(CrossboneIslePier),
    SCENE_EXIT(CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_PIER),
    SCENE_EXIT(CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP, PirateShip, PIRATE_SHIP_FROM_CROSSBONE_ISLE_PIER),
    SCENE_EXITS(PirateShip),
    SCENE_EXIT(PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER, CrossboneIslePier, CROSSBONE_ISLE_PIER_FROM_PIRATE_SHIP),
    SCENE_EXITS(CrossboneIsleFirstFloor),
    SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_CAVE_ENTRANCE, CrossboneIsleCaveEntrance, CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE_FIRST_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_FIRST_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_STEPPING_STONES_EAST, CrossboneIsleSteppingStones, CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_STEPPING_STONES_WEST, CrossboneIsleSteppingStones, CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleSecondFloor),
    SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_FIRST_FLOOR, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_SECOND_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_THIRD_FLOOR, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_SECOND_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_ICE_PILLARS_EAST, CrossboneIsleIcePillars, CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_ICE_PILLARS_WEST, CrossboneIsleIcePillars, CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleThirdFloor),
    SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_THIRD_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_FOURTH_FLOOR, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_THIRD_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_KEYS_EAST, CrossboneIsleKeys, CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_KEYS_WEST, CrossboneIsleKeys, CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleFourthFloor),
    SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_THIRD_FLOOR, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_FOURTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_FIFTH_FLOOR, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FOURTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_STATUES_EAST, CrossboneIsleStatues, CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_STATUES_WEST, CrossboneIsleStatues, CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleFifthFloor),
    SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FOURTH_FLOOR, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_FIFTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_SIXTH_FLOOR, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_FIFTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FLOODGATE_EAST, CrossboneIsleFloodgate, CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FLOODGATE_WEST, CrossboneIsleFloodgate, CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleSixthFloor),
    SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_FIFTH_FLOOR, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_SIXTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_SEVENTH_FLOOR, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_SIXTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_BANE_EAST, CrossboneIsleBane, CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_BANE_WEST, CrossboneIsleBane, CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleSeventhFloor),
    SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_SIXTH_FLOOR, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_EIGHTH_FLOOR, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_STACKED_PILLARS_EAST, CrossboneIsleStackedPillars, CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_STACKED_PILLARS_WEST, CrossboneIsleStackedPillars, CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleEighthFloor),
    SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_SEVENTH_FLOOR, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_NINTH_FLOOR, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_DRAGON_STATUE_EAST, CrossboneIsleDragonStatue, CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_DRAGON_STATUE_WEST, CrossboneIsleDragonStatue, CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleNinthFloor),
    SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_EIGHTH_FLOOR, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_NINTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_PIER, CrossboneIslePier, CROSSBONE_ISLE_PIER_FROM_CROSSBONE_ISLE_NINTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_FLOATING_LOGS_EAST, CrossboneIsleFloatingLogs, CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_FLOATING_LOGS_WEST, CrossboneIsleFloatingLogs, CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_WEST),
    SCENE_EXITS_END,
};

struct ScenePlacement gCrossboneIslePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlacements1[] = {
    { 0x16, 0xfd7, 7,
        PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(144), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(352), 0, PIXELS(376), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(368), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(336), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(320), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(352), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlacements3[] = {
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(408), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(520), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(584), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(552), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(584), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(392), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(72), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(120), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlacements6To14[] = {
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(632), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gCrossboneIsleEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, CORRIDOR_TO_PREVIOUS_ROOM },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, CORRIDOR_TO_NEXT_ROOM },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenWestDoor) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CORRIDOR_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MovePillar) },
    { SEARCH(SEARCH_CHEST), 100, 0xef4, 0x500004 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleEvents1[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_ISLE_NUT, 0xfd7, EVENT_SCRIPT(Event_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT, FLAG_SCENE_TEMP_40, EVENT_SCRIPT(Event_GrowFirstPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT + 1, FLAG_SCENE_TEMP_41, EVENT_SCRIPT(Event_GrowSecondPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT + 2, FLAG_SCENE_TEMP_42, EVENT_SCRIPT(Event_GrowThirdPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT + 3, FLAG_SCENE_TEMP_43, EVENT_SCRIPT(Event_GrowFourthPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY, EVENT_SCRIPT(Event_BlowAwayFirstIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY + 1, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1, EVENT_SCRIPT(Event_BlowAwaySecondIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY + 2, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2, EVENT_SCRIPT(Event_BlowAwayThirdIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY + 3, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3, EVENT_SCRIPT(Event_BlowAwayFourthIvy) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_TO_WORLD_MAP },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushIslePillar) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushIslePillar) },
    { 0x8413, 100, 0xec0, FIND_COINS(1) },
    { 0xc413, 100, 0xec0, FIND_COINS(1) },
    { 0xa413, 100, 0xec0, FIND_COINS(1) },
    { 0x8413, 101, 0xec1, FIND_ITEM(ITEM_MINT) },
    { 0xc413, 101, 0xec1, FIND_ITEM(ITEM_MINT) },
    { 0xa413, 101, 0xec1, FIND_ITEM(ITEM_MINT) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST },
    { TOUCH_WALKING_INTO(FACING_WEST), 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 42, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 43, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideFirstPillarNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideFirstPillarSouth) },
    { TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideThirdPillarNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideThirdPillarSouth) },
    { TOUCH_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideFourthPillarWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideFourthPillarEast) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideFifthPillarWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideFifthPillarEast) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideSixthPillarWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideSixthPillarEast) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideEighthPillarWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideEighthPillarEast) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideTenthPillarNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideTenthPillarSouth) },
    { TOUCH_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideEleventhPillarNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideEleventhPillarSouth) },
    { TOUCH_WALKING_INTO(FACING_EAST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideSecondPillarNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideSecondPillarSouth) },
    { TOUCH_WALKING_INTO(FACING_EAST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_WEST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideTwelfthPillarWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideTwelfthPillarEast) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideSeventhPillarWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideSeventhPillarEast) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideNinthPillarWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideNinthPillarEast) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnFirstPillar) },
    { EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnThirdPillar) },
    { EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnFourthPillar) },
    { EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnFifthPillar) },
    { EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnSixthPillar) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnEighthPillar) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnTenthPillar) },
    { EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnEleventhPillar) },
    { EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnSecondPillar) },
    { EVENT_TOUCH, 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnTwelfthPillar) },
    { EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnSeventhPillar) },
    { EVENT_TOUCH, 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnNinthPillar) },
    { SEARCH(SEARCH_CHEST), 100, 0xee2, FIND_COINS(999) },
    { SEARCH(SEARCH_CHEST), 101, 0xee3, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { SEARCH(SEARCH_CHEST), 102, 0xee4, FIND_ITEM(ITEM_MURAMASA) },
    { SEARCH(SEARCH_CHEST), 103, 0xee5, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gCrossboneIsleEvents4[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP},
    {SCENE_EVENTS_END},
};

struct SceneEvent gCrossboneIsleEvents5[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER},
    {EVENT_TOUCH, 10, FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
     EVENT_SCRIPT(Event_FightDeadbeard)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_HopSouth)},
    {SEARCH(SEARCH_CHEST), 100, 0xee6, FIND_ITEM(ITEM_DEMON_MAIL)},
    {SEARCH(SEARCH_CHEST), 101, 0xef7, 0x500007},
    {SCENE_EVENTS_END},
};
