#ifndef GUARD_LUNPA_H
#define GUARD_LUNPA_H

#include "types.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_effect.h"
#include "field_event.h"
#include "field_scene.h"
#include "flags.h"
#include "item_ids.h"
#include "songs.h"

// Lunpa's and the gates' entrances and exits are in scene_links.h.

/// The exit the gates' third road (TRIGGER_THIRD_ROAD) requests, which
/// gGateExits has no row for.
enum { GATE_EXIT_THIRD = 3 };

/// The first of the rooms in LunpaSuhalla that Lunpa's doors lead into.
enum { ROOM_ENTRANCE_FIRST = 1 };

#define ROOM_ENTRANCE(exit) ((exit) - LUNPA_TO_FIRST_HOME + ROOM_ENTRANCE_FIRST)

enum LunpaTrigger {
  TRIGGER_WORLD_MAP_ROAD = 1,
  TRIGGER_GATE_ROAD,
  TRIGGER_CAVE_ROAD,
  TRIGGER_FIRST_HOME_DOOR,
  TRIGGER_SECOND_HOME_DOOR,
  TRIGGER_THIRD_HOME_DOOR,
  TRIGGER_ARMS_SHOP_DOOR,
  TRIGGER_ITEM_SHOP_DOOR,
  TRIGGER_INN_DOOR,
  TRIGGER_TEMPLE_DOOR,
  TRIGGER_EIGHTH_ROOM_DOOR,
  TRIGGER_HIDDEN_PASSAGE,
  TRIGGER_HIDDEN_SWITCH = 20,
  TRIGGER_WELL = 90,
  TRIGGER_GRAVE_SLEEP_BOMB = 100,
  TRIGGER_BARREL_NUT,
  TRIGGER_PSYNERGY_STONE
};

enum GateTrigger {
  TRIGGER_FORTRESS_DOOR = 1,
  TRIGGER_LUNPA_ROAD,
  TRIGGER_THIRD_ROAD,
  TRIGGER_GUARDED_GROUND = 10,
  TRIGGER_FORTRESS_APPROACH = 20,
  TRIGGER_PARTY_SPOTTED = 91
};

enum {
  DOOR_COUNT = 8,
  DOOR_TEMPLE = TRIGGER_TEMPLE_DOOR - TRIGGER_FIRST_HOME_DOOR
};

/// Lunpa's placed actors, numbered in the order of gLunpaPlacements from
/// ACTOR_FIRST_PLACED. The villagers keep the letters of their messages'
/// names (MsgLunpaVillageVillagerBSealed and the rest).
enum LunpaActor {
  /// The puddle Reveal shows and Frost freezes into a pillar.
  /// ★ Named from what the player sees: its sprite, a small blue puddle (its
  /// render)
  ACTOR_HIDDEN_PUDDLE = ACTOR_FIRST_PLACED,
  ACTOR_VILLAGER_A, ///< "Have you been in Lunpa long?"
  /// "We've had no freedom ever since the caravan came to the village."
  ACTOR_VILLAGER_B,
  /// "Is it true that Dodonpa kidnapped Hammet and extorted ransom for him?"
  ACTOR_VILLAGER_C,
  /// "Donpa is too old to lead, so his men do whatever Dodonpa says."
  ACTOR_VILLAGER_D,
  /// "The righteous thief, Lunpa, and his son, Donpa" were great men.
  ACTOR_VILLAGER_E,
  ACTOR_VILLAGER_F, ///< "Donpa has no idea what Dodonpa is doing."
  ACTOR_VILLAGER_G, ///< "You want to see Donpa?"
  /// "The chef from the inn has been delivering meals to the fortress lately."
  ACTOR_VILLAGER_H,
  ACTOR_WEST_GUARD, ///< keeps the road in from the world map
  ACTOR_EAST_GUARD, ///< wants Dodonpa's authorization
  /// The hidden switch's sparkle, shown by Reveal.
  /// ★ Named from what the player sees: its sprite, a small white sparkle (its
  /// render)
  ACTOR_SWITCH_SPARKLE,
  ACTOR_FLOATING_NUT ///< the Nut that floats over the town, for Catch
};

enum { ACTOR_LAST_PLACED = 65 };

/// The Lunpa Fortress Gates' placed actors, numbered in the order of
/// gGatePlacements from ACTOR_FIRST_PLACED. Garet's row, the third, takes his
/// own actor, GARET, as every row of a party member's sprite does.
enum GateActor {
  /// ★ Named from the game: "We were on guard the whole time!"
  /// (MsgLunpaRightGuardReopenedThoughts)
  ACTOR_LEFT_GUARD = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "We were on guard the whole time!"
  /// (MsgLunpaRightGuardReopenedThoughts)
  ACTOR_RIGHT_GUARD
};

enum LunpaSprite {
  SPRITE_TOWNSPERSON_G = 0x66,
  SPRITE_TOWNSPERSON_B = 0x67,
  SPRITE_TOWNSPERSON_C = 0x6a,
  SPRITE_TOWNSPERSON_H = 0x6b,
  SPRITE_TOWNSPERSON_D = 0x6c,
  SPRITE_TOWNSPERSON_I = 0x6d,
  SPRITE_TOWNSPERSON_E = 0x6f,
  SPRITE_TOWNSPERSON_J = 0x73,
  SPRITE_GUARD = 0x96
};

enum { ACTOR_FLOAT = 7 };

enum {
  SPARKLE_MOTION_FLAGS = 8,
  SPARKLE_SCALE_X = 0x13333,
  SPARKLE_SCALE_Y = 0x18000
};

enum { GATE_RETREAT_ENTRANCE = 10 };

enum CatchLine {
  CATCH_LEFT_GUARD_CHALLENGES,
  CATCH_RIGHT_GUARD_WONDERS,
  CATCH_LEFT_GUARD_REFUSES_ENTRY,
  CATCH_RIGHT_GUARD_SENDS_PARTY_OFF
};

enum RecognitionLine { RECOGNITION_SEEN_THAT_MAN, RECOGNITION_IMPOSSIBLE };

enum { ANIM_SPRAWLED = 19 };

enum { PILLAR_TOP_HEIGHT = PIXELS(16), CELL_SIZE = PIXELS(16) };

enum { MAP_OBJECT_PSYNERGY_STONE = 102 };

enum {
  HEAP_ITEM_ICON = 17,
  ITEM_ICON_BUFFER_SIZE = 0x608,
  ITEM_ICON_TILES = 0x400,
  ITEM_ICON_TILE_BYTES = 128
};

extern struct MapCellAnim gLunpaDoors[];

#endif
