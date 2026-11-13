#ifndef ALCHEMY_LUNPA_H
#define ALCHEMY_LUNPA_H

#include "types.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_effect.h"
#include "event.h"
#include "scene_tables.h"
#include "flags.h"
#include "item_ids.h"
#include "sounds.h"

// Lunpa's and the gates' entrances and exits are in scene_exits.h.

/// The exit the gates' third road
/// (CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD) requests, which gGateExits
/// has no row for. Scene_TakeExit, reaching the end of the gates' exits, falls
/// back to the event of their first exit at entrance 0, so the road leads to
/// LunpaFortressGroundFloor's default entrance.
enum { GATE_EXIT_THIRD = 3 };

/// The first of the rooms in RockRooms that Lunpa's doors lead into.
enum { ROOM_ENTRANCE_FIRST = 1 };

#define ROOM_ENTRANCE(exit) ((exit) - LUNPA_TO_FIRST_HOME + ROOM_ENTRANCE_FIRST)

/// The cell behaviors Lunpa's triggers answer.
enum LunpaCellBehavior {
  CELL_BEHAVIOR_LUNPA_WORLD_MAP_ROAD = 1,
  CELL_BEHAVIOR_LUNPA_GATE_ROAD,
  CELL_BEHAVIOR_LUNPA_CAVE_ROAD,
  CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR,
  CELL_BEHAVIOR_LUNPA_SECOND_HOME_DOOR,
  CELL_BEHAVIOR_LUNPA_THIRD_HOME_DOOR,
  CELL_BEHAVIOR_LUNPA_ARMS_SHOP_DOOR,
  CELL_BEHAVIOR_LUNPA_ITEM_SHOP_DOOR,
  CELL_BEHAVIOR_LUNPA_INN_DOOR,
  CELL_BEHAVIOR_LUNPA_TEMPLE_DOOR,
  CELL_BEHAVIOR_LUNPA_EIGHTH_ROOM_DOOR,
  CELL_BEHAVIOR_LUNPA_HIDDEN_PASSAGE,
  CELL_BEHAVIOR_LUNPA_HIDDEN_SWITCH = 20,
  CELL_BEHAVIOR_LUNPA_WELL = 90,
  CELL_BEHAVIOR_LUNPA_GRAVE_SLEEP_BOMB = 100,
  CELL_BEHAVIOR_LUNPA_BARREL_NUT,
  CELL_BEHAVIOR_LUNPA_PSYNERGY_STONE
};

/// The cell behaviors the Lunpa Fortress Gates' triggers answer.
enum LunpaFortressGatesCellBehavior {
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_DOOR = 1,
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_LUNPA_ROAD,
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD,
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_GUARDED_GROUND = 10,
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH = 20,
  CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_WELL = 90
};

/// The event the gates' guards raise on spotting the uncloaked party.
enum LunpaFortressGatesRaisedTrigger { RAISED_PARTY_SPOTTED = 91 };

enum {
  DOOR_COUNT = 8,
  DOOR_TEMPLE = CELL_BEHAVIOR_LUNPA_TEMPLE_DOOR -
      CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR
};

/// Lunpa's placed actors, numbered in the order of gLunpaActorPlacements
/// from ACTOR_FIRST_PLACED. The townspeople are named by their sprites and by
/// what they say before Donpa's judgment.
enum LunpaActorId {
  /// The puddle Reveal shows and Frost freezes into a pillar.
  ACTOR_HIDDEN_PUDDLE = ACTOR_FIRST_PLACED,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  ACTOR_WOMAN_WITH_NO_FREEDOM,
  /// A stout man with brown hair (SPRITE_STOUT_MAN).
  ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who wanders.
  ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
  /// An old woman with white hair (SPRITE_OLD_WOMAN).
  ACTOR_OLD_WOMAN_ON_DONPA,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR).
  ACTOR_BOY_ASKING_TO_SEE_DONPA,
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), who
  /// wanders.
  ACTOR_GIRL_ON_THE_CHEFS_MEALS,
  /// The two soldiers in spiked helmets (SPRITE_SOLDIER_IN_SPIKED_HELMET);
  /// this one keeps the road in from the World Map.
  ACTOR_WEST_GUARD,
  ACTOR_EAST_GUARD, ///< wants Dodonpa's authorization
  /// The hidden switch's sparkle, shown by Reveal.
  ACTOR_SWITCH_SPARKLE,
  /// The Nut that floats over the town, for Catch.
  ACTOR_FLOATING_NUT
};

/// The Lunpa Fortress Gates' placed actors, numbered in the order of
/// gGateActorPlacements from ACTOR_FIRST_PLACED. Garet's row, the third,
/// takes his own actor, GARET, as every row of a party member's sprite does.
enum GateActorId { ACTOR_LEFT_GUARD = ACTOR_FIRST_PLACED, ACTOR_RIGHT_GUARD };

/// How the switch's sparkle is stretched while Reveal shows it.
enum { SPARKLE_SCALE_X = 6 * Q16_ONE / 5, SPARKLE_SCALE_Y = 3 * Q16_ONE / 2 };

/// Where Retreat at the gates sends the party
/// (MainRoutineWork.retreatEntrance).
enum { GATE_RETREAT_ENTRANCE = 10 };

enum CatchMessage {
  CATCH_LEFT_GUARD_CHALLENGES,
  CATCH_RIGHT_GUARD_WONDERS,
  CATCH_LEFT_GUARD_REFUSES_ENTRY,
  CATCH_RIGHT_GUARD_SENDS_PARTY_OFF
};

enum RecognitionMessage { RECOGNITION_SEEN_THAT_MAN, RECOGNITION_IMPOSSIBLE };

enum { ANIM_SPRAWLED = 19 };

enum { PILLAR_TOP_HEIGHT = px(16), CELL_SIZE = px(16) };

enum { TREASURE_SPOT_PSYNERGY_STONE = 102 };

extern struct MapGridCellAnimation gLunpaDoors[];

#endif
