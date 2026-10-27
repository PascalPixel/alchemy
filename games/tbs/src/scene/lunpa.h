#ifndef ALCHEMY_LUNPA_H
#define ALCHEMY_LUNPA_H

#include "types.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_effect.h"
#include "event_layer.h"
#include "scene_tables.h"
#include "flags.h"
#include "item_ids.h"
#include "songs.h"

// Lunpa's and the gates' entrances and exits are in scene_links.h.

/// The exit the gates' third road (MB_LUNPA_FORTRESS_GATES_THIRD_ROAD)
/// requests, which gGateExits has no row for. Scene_TakeExit, reaching the end
/// of the gates' exits, falls back to the event of their first exit at entrance
/// 0, so the road leads to LunpaFortressGroundFloor's default entrance.
enum { GATE_EXIT_THIRD = 3 };

/// The first of the rooms in RockRooms that Lunpa's doors lead into.
enum { ROOM_ENTRANCE_FIRST = 1 };

#define ROOM_ENTRANCE(exit) ((exit) - LUNPA_TO_FIRST_HOME + ROOM_ENTRANCE_FIRST)

/// The metatile behaviors Lunpa's triggers answer.
enum LunpaMetatileBehavior {
  MB_LUNPA_WORLD_MAP_ROAD = 1,
  MB_LUNPA_GATE_ROAD,
  MB_LUNPA_CAVE_ROAD,
  MB_LUNPA_FIRST_HOME_DOOR,
  MB_LUNPA_SECOND_HOME_DOOR,
  MB_LUNPA_THIRD_HOME_DOOR,
  MB_LUNPA_ARMS_SHOP_DOOR,
  MB_LUNPA_ITEM_SHOP_DOOR,
  MB_LUNPA_INN_DOOR,
  MB_LUNPA_TEMPLE_DOOR,
  MB_LUNPA_EIGHTH_ROOM_DOOR,
  MB_LUNPA_HIDDEN_PASSAGE,
  MB_LUNPA_HIDDEN_SWITCH = 20,
  /// ★ Named from the game: "peered into the well" (MsgFieldPeeredWell)
  MB_LUNPA_WELL = 90,
  MB_LUNPA_GRAVE_SLEEP_BOMB = 100,
  /// ★ Named from the game: "checked the barrel" (MsgVaultIsaacCheckedBarrel)
  MB_LUNPA_BARREL_NUT,
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  MB_LUNPA_PSYNERGY_STONE
};

/// The metatile behaviors the Lunpa Fortress Gates' triggers answer.
enum LunpaFortressGatesMetatileBehavior {
  MB_LUNPA_FORTRESS_GATES_FORTRESS_DOOR = 1,
  MB_LUNPA_FORTRESS_GATES_LUNPA_ROAD,
  MB_LUNPA_FORTRESS_GATES_THIRD_ROAD,
  MB_LUNPA_FORTRESS_GATES_GUARDED_GROUND = 10,
  MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH = 20,
  /// ★ Named from the game: "peered into the well" (MsgFieldPeeredWell)
  MB_LUNPA_FORTRESS_GATES_WELL = 90
};

/// The event the gates' guards raise on spotting the uncloaked party.
enum LunpaFortressGatesRaisedTrigger { RAISED_PARTY_SPOTTED = 91 };

enum {
  DOOR_COUNT = 8,
  DOOR_TEMPLE = MB_LUNPA_TEMPLE_DOOR - MB_LUNPA_FIRST_HOME_DOOR
};

/// Lunpa's placed actors, numbered in the order of gLunpaActorTemplates
/// from ACTOR_FIRST_PLACED. The townspeople are named by their sprites and by
/// what they say before Donpa's judgment.
enum LunpaActorId {
  /// The puddle Reveal shows and Frost freezes into a pillar.
  /// ★ Named from what the player sees: its sprite, a small blue puddle (its
  /// render)
  ACTOR_HIDDEN_PUDDLE = ACTOR_FIRST_PLACED,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "Have you been in Lunpa long?"
  /// (MsgLunpaVillagerAAsksHowLong)
  ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "We've had no freedom ever since the caravan
  /// came to the village." (MsgLunpaVillageVillagerBSealed)
  ACTOR_WOMAN_WITH_NO_FREEDOM,
  /// A stout man with brown hair (SPRITE_STOUT_MAN).
  /// ★ Named from the game: "Is it true that Dodonpa kidnapped Hammet"
  /// (MsgLunpaVillagerCAsksAboutKidnapping)
  ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "Donpa is too old to lead"
  /// (MsgLunpaVillageVillagerDSealed)
  ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who wanders.
  /// ★ Named from the game: "The righteous thief, Lunpa, and his son, Donpa"
  /// (MsgLunpaVillageVillagerESealed)
  ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
  /// An old woman with white hair (SPRITE_OLD_WOMAN).
  /// ★ Named from the game: "Donpa has no idea what Dodonpa is doing."
  /// (MsgLunpaVillageVillagerFSealed)
  ACTOR_OLD_WOMAN_ON_DONPA,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR).
  /// ★ Named from the game: "You want to see Donpa?"
  /// (MsgLunpaVillagerGAsksAboutDonpa)
  ACTOR_BOY_ASKING_TO_SEE_DONPA,
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), who
  /// wanders.
  /// ★ Named from the game: "The chef from the inn has been delivering meals
  /// to the fortress lately." (MsgLunpaVillageVillagerHSealed)
  ACTOR_GIRL_ON_THE_CHEFS_MEALS,
  /// The two soldiers in spiked helmets (SPRITE_SOLDIER_IN_SPIKED_HELMET);
  /// this one keeps the road in from the World Map.
  /// ★ Named from what the player sees: a soldier with a spear guarding the
  /// road (its render)
  ACTOR_WEST_GUARD,
  /// ★ Named from what the player sees: a second soldier with a spear (its
  /// render)
  ACTOR_EAST_GUARD, ///< wants Dodonpa's authorization
  /// The hidden switch's sparkle, shown by Reveal.
  /// ★ Named from what the player sees: its sprite, a small white sparkle (its
  /// render)
  ACTOR_SWITCH_SPARKLE,
  /// The Nut that floats over the town, for Catch.
  /// ★ Named from the game: "Nut" (MsgItemName)
  ACTOR_FLOATING_NUT
};

/// The Lunpa Fortress Gates' placed actors, numbered in the order of
/// gGateActorTemplates from ACTOR_FIRST_PLACED. Garet's row, the third,
/// takes his own actor, GARET, as every row of a party member's sprite does.
enum GateActorId {
  /// ★ Named from the game: "We were on guard the whole time!"
  /// (MsgLunpaRightGuardReopenedThoughts)
  ACTOR_LEFT_GUARD = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "We were on guard the whole time!"
  /// (MsgLunpaRightGuardReopenedThoughts)
  ACTOR_RIGHT_GUARD
};

/// How the switch's sparkle is stretched while Reveal shows it.
enum { SPARKLE_SCALE_X = 0x13333, SPARKLE_SCALE_Y = 0x18000 };

/// Where Retreat at the gates sends the party
/// (Common.retreatEntrance).
enum { GATE_RETREAT_ENTRANCE = 10 };

enum CatchMessage {
  CATCH_LEFT_GUARD_CHALLENGES,
  CATCH_RIGHT_GUARD_WONDERS,
  CATCH_LEFT_GUARD_REFUSES_ENTRY,
  CATCH_RIGHT_GUARD_SENDS_PARTY_OFF
};

enum RecognitionMessage { RECOGNITION_SEEN_THAT_MAN, RECOGNITION_IMPOSSIBLE };

enum { ANIM_SPRAWLED = 19 };

enum { PILLAR_TOP_HEIGHT = PIXELS(16), CELL_SIZE = PIXELS(16) };

enum { TREASURE_SPOT_PSYNERGY_STONE = 102 };

extern struct MapGridCellAnimation gLunpaDoors[];

#endif
