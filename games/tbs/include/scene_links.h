#ifndef ALCHEMY_SCENE_LINKS_H
#define ALCHEMY_SCENE_LINKS_H

/// @file
/// The ways between places: each scene's entrances, named by where the party
/// arrives from, and its exits, named by where they lead, in the exit tables'
/// numbers (SCENE_EXIT). A door is named the same from both sides. Links
/// between rooms of one scene are named by their rooms
/// (<SCENE>_<ROOM>_TO_<ROOM>).

/// The entrance a scene's table lists first: Scene_TakeExit sends the party
/// there when an exit has no row of its own, and the debug menu's scene select
/// starts there. A scene that gives it a place of its own names it below.
enum { ENTRANCE_DEFAULT = 0 };

/// SceneId_Title's entrances and exits.
enum {
  TITLE_FROM_VALE_GATE = 1,
  TITLE_AFTER_RUN = 2, ///< Title_Run
  TITLE_FROM_LEMURIAN_SHIP = 9,
  TITLE_FROM_WORLD_MAP = 10,
};

/// SceneId_Clear's entrances and exits.
enum {
  CLEAR_FROM_SANCTUM = 1,
  CLEAR_FROM_TITLE = 2,
};

/// SceneId_WorldMap's entrances.
enum {
  WORLD_MAP_FROM_VALE_GATE = 1,
  WORLD_MAP_FROM_TITLE = 2,
  WORLD_MAP_FROM_VAULT_SOUTH = 3,
  WORLD_MAP_FROM_GOMA_CAVE_ENTRANCE = 4,
  WORLD_MAP_FROM_BILIBIN = 5,
  WORLD_MAP_FROM_BILIBIN_BARRICADE_EAST = 6,
  WORLD_MAP_FROM_KOLIMA_WEST = 7,
  WORLD_MAP_FROM_KOLIMA_FOREST_ENTRANCE = 8,
  WORLD_MAP_FROM_IMIL_SOUTH = 9,
  WORLD_MAP_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST = 10,
  WORLD_MAP_FROM_KOLIMA_BRIDGE_EAST = 11,
  WORLD_MAP_FROM_FUCHIN_TEMPLE_GROUNDS = 12,
  WORLD_MAP_FROM_MOGALL_FOREST_SIGN = 13,
  WORLD_MAP_FROM_XIAN = 14,
  WORLD_MAP_FROM_ALPINE_CROSSING_EAST = 15,
  WORLD_MAP_FROM_ALTIN = 16,
  WORLD_MAP_FROM_LAMA_TEMPLE = 17,
  WORLD_MAP_FROM_LAMAKAN_DESERT_EAST = 18,
  WORLD_MAP_FROM_KALAY_WEST = 19,
  WORLD_MAP_FROM_KALAY_EAST = 20,
  WORLD_MAP_FROM_KALAY_DOCKS_EAST = 21,
  WORLD_MAP_FROM_TOLBI_DOCKS = 22,
  WORLD_MAP_FROM_TOLBI = 23,
  WORLD_MAP_FROM_ALTMILLER_CAVE_ENTRANCE = 24,
  WORLD_MAP_FROM_GONDOWAN_PASSAGE_EAST = 25,
  WORLD_MAP_FROM_GONDOWAN_CAVE_NORTH = 26,
  WORLD_MAP_FROM_SUHALLA = 27,
  WORLD_MAP_FROM_SUHALLA_DESERT_NORTH = 28,
  WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_SOUTH = 29,
  WORLD_MAP_FROM_LALIVERO_EAST = 30,
  WORLD_MAP_FROM_ROAD_TO_BABI_LIGHTHOUSE = 31,
  WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_EAST = 32,
  WORLD_MAP_FROM_GATEWAY_CAVE = 33,
  WORLD_MAP_FROM_LUNPA = 34,
  WORLD_MAP_FROM_BILIBIN_CAVE_ENTRANCE = 35,
  WORLD_MAP_FROM_KOLIMA_EAST = 38,
  WORLD_MAP_FROM_IMIL_NORTH = 39,
  WORLD_MAP_FROM_GOMA_CAVE_BRIDGE = 40,
  WORLD_MAP_FROM_GONDOWAN_CAVE_SOUTH = 41,
  WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_EAST = 42,
  WORLD_MAP_FROM_MOGALL_FOREST_KILLER_APE = 43,
  WORLD_MAP_FROM_VAULT_NORTH = 44,
  WORLD_MAP_FROM_KOLIMA_BRIDGE_WEST = 46,
  WORLD_MAP_FROM_BILIBIN_BARRICADE_WEST = 47,
  WORLD_MAP_FROM_BILIBIN_CAVE_ICE = 48,
  WORLD_MAP_FROM_WEST_LUNPA_CAVE = 49,
  WORLD_MAP_FROM_ALPINE_CROSSING_NORTH = 50,
  WORLD_MAP_FROM_ALPINE_CROSSING_WEST = 51,
  WORLD_MAP_FROM_ALTIN_PEAK_WEST_EXIT = 52,
  WORLD_MAP_FROM_LAMAKAN_DESERT_CAVE = 53,
  WORLD_MAP_FROM_GONDOWAN_PASSAGE_WEST = 54,
  WORLD_MAP_FROM_SUHALLA_DESERT_SOUTH = 55,
  WORLD_MAP_FROM_SUHALLA_NORTH = 56,
  WORLD_MAP_FROM_SUHALLA_EAST = 57,
  WORLD_MAP_FROM_KALAY_DOCKS_WEST = 58,
  WORLD_MAP_FROM_LALIVERO_WEST = 59,
  WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_NORTH = 60,
  WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_WEST = 61,
  WORLD_MAP_HAMMETS_CARAVAN_LEAVES = 64,
  WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK = 65,
  WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_SECOND_ATTACK = 66,
  WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_THIRD_ATTACK = 67,
  WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_KRAKEN = 68,
  WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_LAND = 69,
  WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI = 70,
  WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH = 71,
  WORLD_MAP_FROM_CLEAR = 72,
  /// After the look back at Venus Lighthouse, the lighthouse shakes in clouds
  /// of dust and WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE sends the party on to
  /// Sheba's fall (WorldMap_ShakeVenusLighthouse).
  WORLD_MAP_BEFORE_SHEBAS_FALL = 73,
  WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE = 74,
  WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS = 75,
  WORLD_MAP_FROM_CROSSBONE_ISLE = 76,
  /// Where one of the Suhalla Desert's storms sets the party down, still in
  /// the air, beside the world map's own storm.
  WORLD_MAP_FROM_SUHALLA_DESERT_STORM = 77,
  /// Where a defeat returns after Iodem hands over the Black Orb: the orb is
  /// discarded and WORLD_MAP_TO_LUNPA_SUHALLA leads on.
  WORLD_MAP_AFTER_BLACK_ORB_DEFEAT = 78,
  WORLD_MAP_JENNA_AND_KRADEN_ADRIFT = 80,
  WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT =
      90, ///< WorldMap_Initialize plays WorldMap_PlayLighthouseTransition
          ///< here in Mercury Lighthouse's colours, then sends the party to the
          ///< aerie
  WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT =
      91, ///< WorldMap_Initialize plays WorldMap_PlayLighthouseTransition
          ///< here in Venus Lighthouse's colours, then sends the party back to
          ///< the aerie
  /// The world map's debug entrance, which sets FLAG_NO_ENCOUNTERS,
  /// FLAG_NO_ENCOUNTERS_IN_PLACE and FLAG_DEBUG_STEPS_OFF.
  WORLD_MAP_FROM_DEBUG = 99,
};
/// SceneId_WorldMap's exits.
enum {
  WORLD_MAP_TO_VALE_GATE = 1,
  WORLD_MAP_TO_WORLD_MAP_FROM_TITLE = 2,
  WORLD_MAP_TO_VAULT_SOUTH = 3,
  WORLD_MAP_TO_GOMA_CAVE_ENTRANCE = 4,
  WORLD_MAP_TO_BILIBIN = 5,
  WORLD_MAP_TO_BILIBIN_BARRICADE_WEST = 6,
  WORLD_MAP_TO_KOLIMA_WEST = 7,
  WORLD_MAP_TO_KOLIMA_FOREST_ENTRANCE = 8,
  WORLD_MAP_TO_IMIL_SOUTH = 9,
  WORLD_MAP_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST = 10,
  WORLD_MAP_TO_KOLIMA_BRIDGE_EAST = 11,
  WORLD_MAP_TO_FUCHIN_TEMPLE_GROUNDS = 12,
  WORLD_MAP_TO_MOGALL_FOREST_SIGN = 13,
  WORLD_MAP_TO_XIAN = 14,
  WORLD_MAP_TO_ALPINE_CROSSING_EAST = 15,
  WORLD_MAP_TO_ALTIN = 16,
  WORLD_MAP_TO_LAMA_TEMPLE = 17,
  WORLD_MAP_TO_LAMAKAN_DESERT_EAST = 18,
  WORLD_MAP_TO_WORLD_MAP_FROM_KALAY_WEST = 19,
  WORLD_MAP_TO_KALAY_EAST = 20,
  WORLD_MAP_TO_KALAY_DOCKS_EAST = 21,
  WORLD_MAP_TO_TOLBI_DOCKS = 22,
  WORLD_MAP_TO_TOLBI = 23,
  WORLD_MAP_TO_ALTMILLER_CAVE_ENTRANCE = 24,
  WORLD_MAP_TO_GONDOWAN_PASSAGE_EAST = 25,
  WORLD_MAP_TO_GONDOWAN_CAVE_NORTH = 26,
  WORLD_MAP_TO_SUHALLA = 27,
  WORLD_MAP_TO_SUHALLA_DESERT_NORTH = 28,
  WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH = 29,
  WORLD_MAP_TO_LALIVERO_EAST = 30,
  WORLD_MAP_TO_ROAD_TO_BABI_LIGHTHOUSE = 31,
  WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST = 32,
  WORLD_MAP_TO_GATEWAY_CAVE = 33,
  WORLD_MAP_TO_LUNPA = 34,
  WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE = 35,
  WORLD_MAP_TO_GOMA_CAVE_BRIDGE = 40,
  WORLD_MAP_TO_GONDOWAN_CAVE_SOUTH = 41,
  WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST = 42,
  WORLD_MAP_TO_MOGALL_FOREST_KILLER_APE = 43,
  WORLD_MAP_TO_VAULT_NORTH = 44,
  WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE_NORTHEAST = 45,
  WORLD_MAP_TO_KOLIMA_BRIDGE_WEST = 46,
  WORLD_MAP_TO_BILIBIN_BARRICADE_EAST = 47,
  WORLD_MAP_TO_BILIBIN_CAVE_ICE = 48,
  WORLD_MAP_TO_WEST_LUNPA_CAVE = 49,
  WORLD_MAP_TO_ALPINE_CROSSING_NORTH = 50,
  WORLD_MAP_TO_ALPINE_CROSSING_WEST = 51,
  WORLD_MAP_TO_ALTIN_PEAK_WEST_EXIT = 52,
  WORLD_MAP_TO_LAMAKAN_DESERT_CAVE = 53,
  WORLD_MAP_TO_GONDOWAN_PASSAGE_WEST = 54,
  WORLD_MAP_TO_SUHALLA_DESERT_SOUTH = 55,
  WORLD_MAP_TO_KALAY_DOCKS_NORTH = 56,
  WORLD_MAP_TO_LALIVERO_WEST = 59,
  WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE = 60,
  WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST = 61,
  WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE =
      74, ///< ★ Named from its area name: Crossbone Isle
  WORLD_MAP_TO_CROSSBONE_ISLE =
      75, ///< ★ Named from its area name: Crossbone Isle
  WORLD_MAP_TO_WORLD_MAP_FROM_VALE_GATE = 100,
  WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES =
      101, ///< ★ Named from what the player sees: Hammet's caravan leaving
           ///< Vault for Kalay (its render)
  WORLD_MAP_TO_TOLBI_SHIP_MAST_FIRST_ATTACK = 102,
  WORLD_MAP_TO_TOLBI_SHIP_MAST_SECOND_ATTACK = 103,
  WORLD_MAP_TO_TOLBI_SHIP_MAST_THIRD_ATTACK = 104,
  WORLD_MAP_TO_TOLBI_SHIP_INTERIORS = 105,
  WORLD_MAP_TO_TOLBI_SHIP_MAST_LAND_HO =
      106, ///< ★ Named from the game: "Land ho!" (MsgTolbiShipLandHo)
  WORLD_MAP_TO_TOLBI_SHIP_DECK = 107,
  WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE_SOUTH = 108,
  WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE = 109,
  WORLD_MAP_TO_TOLBI_SHIP_INTERIORS_ROWING_DECK_ISLAND = 110,
  WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT =
      111, ///< ★ Named from what the player sees: Jenna and Kraden adrift on an
           ///< island in the ending (its render)
  WORLD_MAP_TO_LUNPA_SUHALLA = 112,
};

/// SceneId_ValeStorm's entrances.
enum {
  VALE_STORM_FROM_VALE_KRADEN_NORTH = 3,
  VALE_STORM_FROM_VALE_KRADEN_SOUTH = 4,
  VALE_STORM_FROM_VALE_GATE = 5,
  VALE_STORM_FROM_VALE_INTERIORS_SHOPS_EAST = 6,
  VALE_STORM_FROM_VALE_INTERIORS_SHOPS_SOUTH = 7,
  VALE_STORM_FROM_VALE_INTERIORS_INN_ISAACS_HOUSE = 8,
  VALE_STORM_FROM_VALE_INTERIORS_SHOPS_NORTH = 9,
  VALE_STORM_FROM_VALE_INTERIORS_SHOPS_WEST = 10,
  VALE_STORM_FROM_VALE_INTERIORS_INN_WEST = 12,
  VALE_STORM_FROM_VALE_INTERIORS_INN_EAST = 13,
  VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA = 15,
  VALE_STORM_FROM_BOULDER_PASSED = 16,
};
/// SceneId_ValeStorm's exits.
enum {
  VALE_STORM_TO_VALE_GATE = 2,
  VALE_STORM_TO_VALE_KRADEN_NORTH = 3,
  VALE_STORM_TO_VALE_KRADEN_SOUTH = 4,
  VALE_STORM_TO_VALE_INTERIORS_SHOPS_EAST = 5,
  VALE_STORM_TO_VALE_INTERIORS_SHOPS_SOUTH = 6,
  VALE_STORM_TO_VALE_INTERIORS_INN_ISAACS_HOUSE =
      7, ///< ★ Named from what the player sees: the door of Isaac's house on
         ///< the stormy night (its render)
  VALE_STORM_TO_VALE_INTERIORS_SHOPS_NORTH = 8,
  VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST = 9,
  VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SECOND_DOOR =
      10, ///< ★ Named from what the player sees: the second door of the house
          ///< by the river, leading where the first house door does (its
          ///< render)
  VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SANCTUM_DOOR =
      11, ///< ★ Named from what the player sees: the door where the Sanctum
          ///< stands by day (its render)
  VALE_STORM_TO_VALE_INTERIORS_INN_WEST = 12,
  VALE_STORM_TO_VALE_INTERIORS_INN_EAST = 13,
  VALE_STORM_TO_BOULDER_PASSED =
      20, ///< ★ Named from what the player sees: the riverbank where Isaac
          ///< stands once the Boulder has swept down the river (its render)
};

/// SceneId_Vale's entrances.
enum {
  VALE_FROM_SANCTUM = 1,
  VALE_FROM_SOL_ENTRANCE = 2,
  VALE_FROM_VALE_KRADEN_NORTH = 3,
  VALE_FROM_VALE_KRADEN_SOUTH = 4,
  VALE_FROM_VALE_GATE = 5,
  VALE_FROM_VALE_INTERIORS_SHOPS_EAST = 6,
  VALE_FROM_VALE_INTERIORS_SHOPS_SOUTH = 7,
  VALE_FROM_VALE_INTERIORS_INN = 8,
  VALE_FROM_VALE_INTERIORS_SHOPS_NORTH = 9,
  VALE_FROM_VALE_INTERIORS_SHOPS_WEST = 10,
  /// Beside the second door into the house of Jenna's grandparents, which
  /// VALE_TO_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR leaves by; no exit leads
  /// here.
  VALE_FROM_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR = 11,
  VALE_FROM_TITLE = 16,
  VALE_FROM_ROOF_MENDED = 17,
};
/// SceneId_Vale's exits.
enum {
  VALE_TO_SOL_ENTRANCE = 1,
  VALE_TO_VALE_GATE = 2,
  VALE_TO_VALE_KRADEN_NORTH = 3,
  VALE_TO_VALE_KRADEN_SOUTH = 4,
  VALE_TO_VALE_INTERIORS_SHOPS_EAST = 5,
  VALE_TO_VALE_INTERIORS_SHOPS_SOUTH = 6,
  VALE_TO_VALE_INTERIORS_INN = 7,
  VALE_TO_VALE_INTERIORS_SHOPS_NORTH = 8,
  VALE_TO_VALE_INTERIORS_SHOPS_WEST = 9,
  VALE_TO_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR =
      10, ///< ★ Named from what the player sees: the door of the house by the
          ///< river that leads into Jenna's grandparents' house (its render)
  VALE_TO_SANCTUM = 11,
  VALE_TO_ROOF_MENDED =
      12, ///< ★ Named from the game: "You patched it nicely." (MsgValeGoodJob)
};

/// SceneId_ValeGate's entrances.
enum {
  VALE_GATE_FROM_VALE_INTERIORS_SHOPS_NORTH = 1,
  VALE_GATE_FROM_VALE_INTERIORS_INN_NORTH = 2,
  VALE_GATE_FROM_VALE_INTERIORS_SHOPS_WEST = 3,
  VALE_GATE_FROM_VALE_INTERIORS_SHOPS_EAST = 4,
  VALE_GATE_FROM_VALE_INTERIORS_SHOPS_SOUTH = 5,
  VALE_GATE_FROM_VALE_INTERIORS_INN_WEST = 6,
  VALE_GATE_FROM_VALE_INTERIORS_INN_SOUTH = 7,
  VALE_GATE_FROM_VALE = 8,
  VALE_GATE_FROM_WORLD_MAP = 10,
  VALE_GATE_FROM_GOMA_CAVE_STREAM = 11,
  VALE_GATE_FROM_SANCTUM = 12,
  /// Beside VALE_GATE_FROM_GOMA_CAVE_STREAM, facing north; no exit leads here.
  VALE_GATE_GOMA_CAVE_STREAM_UNUSED = 15,
  /// Where the battle with the travelers on the stormy night returns, won or
  /// lost (ValeGate_FightTravelers).
  VALE_GATE_AFTER_TRAVELERS_BATTLE = 19,
  /// Entrances only the debug menu's scene select reaches: no table places
  /// them and no exit leads to them. The first replays the travelers leaving,
  /// the second sets FLAG_CHILDHOOD, the third a preset of story flags
  /// (ValeGate_SetProgressFlagsAndExit).
  VALE_GATE_DEBUG_BEFORE_TRAVELERS_LEAVE = 97,
  VALE_GATE_DEBUG_CHILDHOOD = 98,
  VALE_GATE_DEBUG_STORY_FLAGS = 99,
};
/// SceneId_ValeGate's exits.
enum {
  VALE_GATE_TO_VALE_INTERIORS_SHOPS_EAST = 1,
  VALE_GATE_TO_VALE_INTERIORS_INN_NORTH = 2,
  VALE_GATE_TO_VALE_INTERIORS_SHOPS_NORTH = 3,
  VALE_GATE_TO_VALE_INTERIORS_SHOPS_SOUTH = 4,
  VALE_GATE_TO_VALE_INTERIORS_SHOPS_WEST = 5,
  VALE_GATE_TO_VALE_INTERIORS_INN_NORTHWEST = 6,
  VALE_GATE_TO_VALE_INTERIORS_INN_SOUTH = 7,
  VALE_GATE_TO_VALE = 8,
  VALE_GATE_TO_TITLE = 9,
  VALE_GATE_TO_WORLD_MAP = 10,
  VALE_GATE_TO_GOMA_CAVE_STREAM = 11,
  VALE_GATE_TO_BILIBIN = 40,
  VALE_GATE_TO_VALE_INTERIORS_INN_SOUTHEAST = 50,
};

/// SceneId_ValeKraden's entrances.
enum {
  VALE_KRADEN_FROM_VALE_NORTH = 1,
  VALE_KRADEN_FROM_VALE_SOUTH = 2,
  VALE_KRADEN_FROM_VALE_INTERIORS_INN_NORTH = 3,
  VALE_KRADEN_FROM_VALE_CAVE_MOUTH_WEST = 4,
  /// Cleared of the place name with VALE_KRADEN_FROM_VALE_CAVE_MOUTH_EAST, but
  /// no exit leads here and the entrance table has no row for it.
  VALE_KRADEN_FROM_VALE_CAVE_MOUTH_UNUSED = 5,
  VALE_KRADEN_FROM_VALE_CAVE_MOUTH_EAST = 6,
  VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH = 10,
};
/// SceneId_ValeKraden's exits.
enum {
  VALE_KRADEN_TO_VALE_NORTH = 1,
  VALE_KRADEN_TO_VALE_SOUTH = 2,
  VALE_KRADEN_TO_VALE_INTERIORS_INN_NORTH = 3,
  VALE_KRADEN_TO_VALE_CAVE_MOUTH_WEST = 4,
  VALE_KRADEN_TO_VALE_CAVE_MOUTH_EAST = 6,
  VALE_KRADEN_TO_VALE_STORM_NORTH = 11,
  VALE_KRADEN_TO_VALE_STORM_SOUTH = 12,
  VALE_KRADEN_TO_VALE_INTERIORS_INN_MYTHRIL_BAG =
      13, ///< ★ Named from the game: "Mythril Bag" (MsgItemName)
};

/// SceneId_ValeInteriorsShops's entrances.
enum {
  VALE_INTERIORS_SHOPS_FROM_VALE_WEST = 1,
  VALE_INTERIORS_SHOPS_FROM_VALE_NORTH = 2,
  VALE_INTERIORS_SHOPS_FROM_VALE_GATE_NORTH = 3,
  VALE_INTERIORS_SHOPS_FROM_VALE_GATE_WEST = 4,
  VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH = 5,
  VALE_INTERIORS_SHOPS_FROM_VALE_EAST = 6,
  VALE_INTERIORS_SHOPS_FROM_VALE_GATE_SOUTH = 7,
  VALE_INTERIORS_SHOPS_FROM_VALE_GATE_EAST = 8,
};
/// SceneId_ValeInteriorsShops's exits.
enum {
  VALE_INTERIORS_SHOPS_TO_VALE_WEST = 1,
  VALE_INTERIORS_SHOPS_TO_VALE_NORTH = 2,
  VALE_INTERIORS_SHOPS_TO_VALE_GATE_WEST = 3,
  VALE_INTERIORS_SHOPS_TO_VALE_GATE_SOUTH = 4,
  VALE_INTERIORS_SHOPS_TO_VALE_SOUTH = 5,
  VALE_INTERIORS_SHOPS_TO_VALE_EAST = 6,
  VALE_INTERIORS_SHOPS_TO_VALE_GATE_EAST = 7,
  VALE_INTERIORS_SHOPS_TO_VALE_GATE_NORTH = 8,
};

/// SceneId_ValeInteriorsInn's entrances.
enum {
  VALE_INTERIORS_INN_FROM_VALE_GATE_NORTHWEST = 1,
  VALE_INTERIORS_INN_FROM_VALE_KRADEN_NORTH = 2,
  VALE_INTERIORS_INN_FROM_VALE_GATE_NORTHEAST = 3,
  VALE_INTERIORS_INN_FROM_VALE_GATE_WEST = 4,
  VALE_INTERIORS_INN_INN_FROM_UPSTAIRS =
      5, ///< ★ Named from what the player sees: Vale's inn, with its
         ///< innkeeper's counter and the stairs up to the guest room (its
         ///< render)
  VALE_INTERIORS_INN_FROM_VALE = 6,
  VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS =
      7, ///< ★ Named from what the player sees: Isaac's house, where Dora wakes
         ///< him on the stormy night (its render)
  VALE_INTERIORS_INN_INN_FROM_GROUND_FLOOR =
      8, ///< ★ Named from what the player sees: Vale's inn, with its
         ///< innkeeper's counter and the stairs up to the guest room (its
         ///< render)
  VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_GROUND_FLOOR =
      9, ///< ★ Named from what the player sees: Isaac's house, where Dora wakes
         ///< him on the stormy night (its render)
  VALE_INTERIORS_INN_FROM_VALE_STORM_WEST = 11,
  VALE_INTERIORS_INN_FROM_VALE_STORM_EAST = 12,
  VALE_INTERIORS_INN_FROM_BABI_PALACE = 19,
  VALE_INTERIORS_INN_FROM_VALE_GATE_SOUTHEAST = 20,
  VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA =
      21, ///< ★ Named from what the player sees: Isaac's house, where Dora
          ///< wakes him on the stormy night (its render)
  VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG = 22,
};
/// SceneId_ValeInteriorsInn's exits.
enum {
  VALE_INTERIORS_INN_TO_VALE_GATE_NORTHWEST = 1,
  VALE_INTERIORS_INN_TO_VALE_KRADEN_NORTH = 2,
  VALE_INTERIORS_INN_TO_VALE_GATE_NORTHEAST = 3,
  VALE_INTERIORS_INN_TO_VALE_GATE_WEST = 4,
  VALE_INTERIORS_INN_INN_TO_UPSTAIRS =
      5, ///< ★ Named from what the player sees: the upper floor the stairs
         ///< climb to, the inn's guest room or Isaac's bedroom (its render)
  VALE_INTERIORS_INN_TO_VALE = 6,
  VALE_INTERIORS_INN_ISAACS_HOUSE_TO_UPSTAIRS =
      7, ///< ★ Named from what the player sees: the upper floor the stairs
         ///< climb to, the inn's guest room or Isaac's bedroom (its render)
  VALE_INTERIORS_INN_INN_TO_GROUND_FLOOR =
      8, ///< ★ Named from what the player sees: the ground floor at the foot of
         ///< the stairs (its render)
  VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR =
      9, ///< ★ Named from what the player sees: the ground floor at the foot of
         ///< the stairs (its render)
  VALE_INTERIORS_INN_TO_VALE_STORM_WEST = 10,
  VALE_INTERIORS_INN_TO_VALE_STORM_EAST = 11,
  VALE_INTERIORS_INN_TO_BABI_PALACE = 19,
  VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA =
      20, ///< ★ Named from what the player sees: the ground floor at the foot
          ///< of the stairs (its render)
  VALE_INTERIORS_INN_TO_VALE_STORM_WITH_DORA =
      21, ///< ★ Named from what the player sees: Dora leading Isaac down and
          ///< out of the house on the stormy night (its render)
  VALE_INTERIORS_INN_TO_VALE_KRADEN_SOUTH = 22,
};

/// SceneId_Sanctum's entrances.
enum {
  SANCTUM_UNUSED = 0, ///< no exit leads here and it places nobody; the area
                      ///< names call it Sanctum
  SANCTUM_FROM_VALE = 1,
  /// Vale Sanctum, inside by the healers and facing south; no exit leads here.
  SANCTUM_VALE_UNUSED = 2,
  SANCTUM_FROM_VAULT = 10,
  SANCTUM_FROM_VAULT_REVISIT = 11,
  SANCTUM_FROM_VAULT_INTERIORS = 12,
  SANCTUM_FROM_VALE_ERUPTION = 20,
  SANCTUM_FROM_QUEST_RESTART = 21,
  SANCTUM_FROM_ALTIN = 29,
  SANCTUM_FROM_XIAN = 32,
  SANCTUM_FROM_BILIBIN = 35,
  /// Plays the quest's gathering like SANCTUM_FROM_VALE_ERUPTION; no exit leads
  /// here and the entrance table has no row for it.
  SANCTUM_QUEST_UNUSED = 50,
};
/// SceneId_Sanctum's exits.
enum {
  SANCTUM_TO_VALE = 1,
  SANCTUM_TO_VAULT = 10,
  SANCTUM_TO_VAULT_REVISIT = 11,
  SANCTUM_TO_QUEST_RESTART =
      20, ///< ★ Named from what the player sees: starting again before the
          ///< Great Healer after refusing the quest and leaving (its render)
  SANCTUM_TO_VALE_GATE = 30,
  SANCTUM_TO_ALTIN = 40,
  SANCTUM_TO_BILIBIN = 41,
  SANCTUM_TO_XIAN = 42,
  SANCTUM_TO_CLEAR = 50,
};

/// SceneId_ValeEruption's entrances.
enum {
  VALE_ERUPTION_FROM_SOL_ENTRANCE = 15,
};
/// SceneId_ValeEruption's exits.
enum {
  VALE_ERUPTION_TO_SANCTUM = 1,
};

/// SceneId_SolSanctumLunaSolRooms's entrances.
enum {
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_RELIEFS = 1,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE = 2,
  SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED = 3, ///< no exit leads here
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED = 4,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_TRIGGERED = 5,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN = 6,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_UNUSED = 7,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION = 8,
};
/// SceneId_SolSanctumLunaSolRooms's exits.
enum {
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS = 1,
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE = 2,
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED =
      3, ///< ★ Named from what the player sees: the quarter's light flashing
         ///< ever faster around Kraden once the trap is disarmed (its render)
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_TRIGGERED =
      4, ///< ★ Named from the game: "Stop!!! You're about to make a big
         ///< mistake!" (MsgSolSanctumWhatsHappening)
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN =
      5, ///< ★ Named from the game: "You found it"
         ///< (MsgSolSanctumKradenYouFoundIt)
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_UNUSED = 6,
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM = 7,
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_BATTLE = 10,
};

/// SceneId_SolSanctumStatue's entrances.
enum {
  SOL_SANCTUM_STATUE_FROM_VALE_GATE = 0,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS = 1,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_PSYNERGY_STONE = 2,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED = 3,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED = 4,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN = 5,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED = 6,
};
/// SceneId_SolSanctumStatue's exits.
enum {
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS = 1,
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE = 2,
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED =
      3, ///< ★ Named from what the player sees: Kraden explaining that the
         ///< centre statue disarmed the trap (its render)
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED =
      4, ///< ★ Named from the game: "Stop!!! You're about to make a big
         ///< mistake!" (MsgSolSanctumWhatsHappening)
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN =
      5, ///< ★ Named from what the player sees: the lit quarters of the
         ///< pictures flashing in the Luna room (its render)
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED = 6,
  SOL_SANCTUM_STATUE_TO_BATTLE = 10,
};

/// SceneId_SolSanctumPsynergyStone's entrances.
enum {
  SOL_SANCTUM_PSYNERGY_STONE_FROM_SOL_SANCTUM_STATUE = 1,
};
/// SceneId_SolSanctumPsynergyStone's exits.
enum {
  SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE = 1,
};

/// SceneId_SolSanctumMaze's entrances.
enum {
  SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_SOUTH = 1,
  SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_NORTH = 2,
};
/// SceneId_SolSanctumMaze's exits.
enum {
  SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_SOUTH = 1,
  SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_NORTH = 2,
};

/// SceneId_SolSanctumCorridor's entrances.
enum {
  SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_EAST = 1,
  SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_WEST = 2,
};
/// SceneId_SolSanctumCorridor's exits.
enum {
  SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST = 1,
  SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST = 2,
};

/// SceneId_SolSanctumReliefs's entrances.
enum {
  SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST = 1,
  SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_ROOM =
      2, ///< ★ Named from what the player sees: the room of three passages
         ///< above a stepping-stone pool, one passage ending at a chest (its
         ///< render)
  SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_ROOM =
      3, ///< ★ Named from what the player sees: the narrow dead end with a
         ///< statue at its head (its render)
  SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_FROM_ENTRANCE_ROOM =
      4, ///< ★ Named from what the player sees: the room off the entrance hall
         ///< with a pool crossed on stepping stones (its render)
  SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_EAST = 5,
  SOL_SANCTUM_RELIEFS_STEPS_ROOM_FROM_FIRST_RELIEF_ROOM =
      6, ///< ★ Named from what the player sees: the narrow room whose doorway
         ///< has stairs leading on into the sanctum (its render)
  SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_WEST = 7,
  SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE = 8,
  SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_FROM_STEPPING_STONE_ROOM =
      9, ///< ★ Named from what the player sees: the room inside Sol Sanctum's
         ///< door, where the party first comes in from the mountainside (its
         ///< render)
  SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_FROM_SECOND_RELIEF_ROOM =
      10, ///< ★ Named from what the player sees: the passage that runs in a
          ///< loop round a block of wall (its render)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM =
      11, ///< ★ Named from the game: "It's a relief"
          ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK =
      12, ///< ★ Named from the game: "It's a relief"
          ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM =
      13, ///< ★ Named from the game: "It's a relief"
          ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST = 14,
  SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE =
      15, ///< ★ Named from the game: "It's a relief"
          ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS = 16,
};
/// SceneId_SolSanctumReliefs's exits.
enum {
  SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST = 1,
  SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM =
      2, ///< ★ Named from the game: "It's a relief"
         ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM =
      3, ///< ★ Named from the game: "It's a relief"
         ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM =
      4, ///< ★ Named from what the player sees: the room inside Sol Sanctum's
         ///< door, where the party first comes in from the mountainside (its
         ///< render)
  SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST = 5,
  SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM =
      6, ///< ★ Named from the game: "It's a relief"
         ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST = 7,
  SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE = 8,
  SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM =
      9, ///< ★ Named from what the player sees: the room off the entrance hall
         ///< with a pool crossed on stepping stones (its render)
  SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM =
      10, ///< ★ Named from the game: "It's a relief"
          ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM =
      11, ///< ★ Named from what the player sees: the room of three passages
          ///< above a stepping-stone pool, one passage ending at a chest (its
          ///< render)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK =
      12, ///< ★ Named from what the player sees: the narrow dead end with a
          ///< statue at its head (its render)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM =
      13, ///< ★ Named from what the player sees: the narrow room whose doorway
          ///< has stairs leading on into the sanctum (its render)
  SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST = 14,
  SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE =
      15, ///< ★ Named from what the player sees: the passage that runs in a
          ///< loop round a block of wall (its render)
  SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS = 16,
};

/// SceneId_SolSanctumElementalStarRoom's entrances.
enum {
  SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS = 10,
};
/// SceneId_SolSanctumElementalStarRoom's exits.
enum {
  SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION = 2,
};

/// SceneId_SolSanctumEruption's entrances.
enum {
  SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM = 10,
};
/// SceneId_SolSanctumEruption's exits.
enum {
  SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS = 5,
};

/// SceneId_SolEntrance's entrances.
enum {
  SOL_ENTRANCE_FROM_VALE = 1,
  SOL_ENTRANCE_FROM_SOL_SANCTUM_RELIEFS = 2,
};
/// SceneId_SolEntrance's exits.
enum {
  SOL_ENTRANCE_TO_VALE = 1,
  SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS = 2,
};

/// SceneId_Vault's entrances.
enum {
  VAULT_FROM_WORLD_MAP_SOUTH = 1,
  VAULT_NORTH_UNUSED = 2, ///< inside the north gate; no exit leads here
  VAULT_FROM_WORLD_MAP_NORTH = 3,
  VAULT_FROM_VAULT_INTERIORS_EAST_SOUTHEAST = 5,
  VAULT_FROM_VAULT_INTERIORS_SOUTHEAST = 6,
  VAULT_FROM_VAULT_INTERIORS_SOUTHWEST = 7,
  VAULT_FROM_VAULT_INTERIORS_EAST = 8,
  VAULT_FROM_VAULT_INTERIORS_NORTH_NORTHEAST = 9,
  VAULT_FROM_VAULT_INTERIORS_NORTHWEST = 10,
  VAULT_FROM_SANCTUM = 11,
  VAULT_FROM_VAULT_JAIL = 12,
  VAULT_FROM_VAULT_INTERIORS_WEST = 13,
};
/// SceneId_Vault's exits.
enum {
  VAULT_TO_WORLD_MAP_SOUTH = 1,
  VAULT_TO_WORLD_MAP_NORTH = 2,
  VAULT_TO_VAULT_INTERIORS_EAST_SOUTHEAST = 5,
  VAULT_TO_VAULT_INTERIORS_SOUTHEAST = 6,
  VAULT_TO_VAULT_INTERIORS_SOUTHWEST = 7,
  VAULT_TO_VAULT_INTERIORS_EAST = 8,
  VAULT_TO_VAULT_INTERIORS_NORTH_NORTHEAST = 9,
  VAULT_TO_VAULT_INTERIORS_NORTHWEST = 10,
  VAULT_TO_SANCTUM = 11,
  VAULT_TO_VAULT_JAIL = 12,
  VAULT_TO_VAULT_INTERIORS_WEST = 13,
  VAULT_TO_VAULT_REVISIT = 14,
};

/// SceneId_VaultInteriors's entrances.
enum {
  VAULT_INTERIORS_FROM_VAULT_EAST_SOUTHEAST = 5,
  VAULT_INTERIORS_FROM_VAULT_SOUTHEAST = 6,
  VAULT_INTERIORS_FROM_VAULT_SOUTHWEST = 7,
  VAULT_INTERIORS_FROM_VAULT_EAST = 8,
  VAULT_INTERIORS_FROM_VAULT_NORTH_NORTHEAST = 9,
  VAULT_INTERIORS_FROM_VAULT_NORTHWEST = 10,
  VAULT_INTERIORS_INN_FROM_INN_UPSTAIRS =
      11, ///< ★ Named from what the player sees: Vault's inn, with the
          ///< innkeeper behind the counter and stairs up (its render)
  VAULT_INTERIORS_INN_UPSTAIRS_FROM_INN =
      12, ///< ★ Named from the game: "The guests upstairs"
          ///< (MsgVaultTheGuestsUpstairsWerentInTheir)
  VAULT_INTERIORS_FROM_VAULT_WEST = 13,
  VAULT_INTERIORS_CRATE_ROOM_FROM_LOFT =
      14, ///< ★ Named from what the player sees: the room with a crate that
          ///< Move pushes aside to reach the loft (its render)
  VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM =
      15, ///< ★ Named from the game: "I came into this loft"
          ///< (MsgVaultVaultCutFree)
  /// The inn's loft after the party loses the battle with the three thieves
  /// (VaultInteriors_LoseToThieves).
  /// ★ Named from the game: "I came into this loft" (MsgVaultVaultCutFree)
  VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES = 16,
  /// The inn's loft after the party beats the three thieves
  /// (VaultInteriors_DefeatThieves).
  /// ★ Named from the game: "I came into this loft" (MsgVaultVaultCutFree)
  VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED = 17,
};
/// SceneId_VaultInteriors's exits.
enum {
  VAULT_INTERIORS_TO_VAULT_EAST_SOUTHEAST = 5,
  VAULT_INTERIORS_TO_VAULT_SOUTHEAST = 6,
  VAULT_INTERIORS_TO_VAULT_SOUTHWEST = 7,
  VAULT_INTERIORS_TO_VAULT_EAST = 8,
  VAULT_INTERIORS_TO_VAULT_NORTH_NORTHEAST = 9,
  VAULT_INTERIORS_TO_VAULT_NORTHWEST = 10,
  VAULT_INTERIORS_INN_UPSTAIRS_TO_INN =
      11, ///< ★ Named from what the player sees: Vault's inn, with the
          ///< innkeeper behind the counter and stairs up (its render)
  VAULT_INTERIORS_INN_TO_INN_UPSTAIRS =
      12, ///< ★ Named from the game: "The guests upstairs"
          ///< (MsgVaultTheGuestsUpstairsWerentInTheir)
  VAULT_INTERIORS_TO_VAULT_WEST = 13,
  VAULT_INTERIORS_LOFT_TO_CRATE_ROOM =
      14, ///< ★ Named from what the player sees: the room with a crate that
          ///< Move pushes aside to reach the loft (its render)
  VAULT_INTERIORS_CRATE_ROOM_TO_LOFT =
      15, ///< ★ Named from the game: "I came into this loft"
          ///< (MsgVaultVaultCutFree)
  VAULT_INTERIORS_TO_SANCTUM = 16,
};

/// SceneId_VaultJail's entrances.
enum {
  VAULT_JAIL_FROM_VAULT = 1,
  VAULT_JAIL_FROM_LUNPA = 2,
  VAULT_JAIL_FROM_VAULT_REVISIT = 10,
};
/// SceneId_VaultJail's exits.
enum {
  VAULT_JAIL_TO_VAULT = 1,
  VAULT_JAIL_TO_VAULT_REVISIT = 2,
  VAULT_JAIL_TO_LUNPA = 3,
};

/// SceneId_VaultRevisit's entrances.
enum {
  VAULT_REVISIT_FROM_VAULT = 1,
  VAULT_REVISIT_NORTH_UNUSED = 2, ///< inside the north gate; no exit leads here
  VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTHEAST = 5,
  VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTH = 6,
  VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTHWEST = 7,
  VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_EAST = 8,
  VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_NORTH = 9,
  VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_NORTHWEST = 10,
  VAULT_REVISIT_FROM_SANCTUM = 11,
  VAULT_REVISIT_FROM_VAULT_JAIL = 12,
  VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE = 13,
  VAULT_REVISIT_FROM_VAULT_CAVE_MOUTH = 14,
};
/// SceneId_VaultRevisit's exits.
enum {
  VAULT_REVISIT_TO_WORLD_MAP_SOUTH = 1,
  VAULT_REVISIT_TO_WORLD_MAP_NORTH = 2,
  VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHEAST = 5,
  VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTH = 6,
  VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_SOUTHWEST = 7,
  VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_EAST = 8,
  VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTH = 9,
  VAULT_REVISIT_TO_VAULT_INTERIORS_REVISIT_NORTHWEST = 10,
  VAULT_REVISIT_TO_SANCTUM = 11,
  VAULT_REVISIT_TO_VAULT_JAIL = 12,
  VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE = 13,
  VAULT_REVISIT_TO_VAULT_CAVE_MOUTH = 14,
};

/// SceneId_VaultInteriorsRevisit's entrances.
enum {
  VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHEAST = 5,
  VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTH = 6,
  VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHWEST = 7,
  VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_EAST = 8,
  VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_NORTH = 9,
  VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_NORTHWEST = 10,
  VAULT_INTERIORS_REVISIT_INN_FROM_INN_UPSTAIRS =
      11, ///< ★ Named from what the player sees: Vault's inn, with the
          ///< innkeeper behind the counter and stairs up (its render)
  VAULT_INTERIORS_REVISIT_INN_UPSTAIRS_FROM_INN =
      12, ///< ★ Named from what the player sees: the room of beds up the inn's
          ///< stairs (its render)
};
/// SceneId_VaultInteriorsRevisit's exits.
enum {
  VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTHEAST = 5,
  VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTH = 6,
  VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTHWEST = 7,
  VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_EAST = 8,
  VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_NORTH = 9,
  VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_NORTHWEST = 10,
  VAULT_INTERIORS_REVISIT_INN_UPSTAIRS_TO_INN =
      11, ///< ★ Named from what the player sees: Vault's inn, with the
          ///< innkeeper behind the counter and stairs up (its render)
  VAULT_INTERIORS_REVISIT_INN_TO_INN_UPSTAIRS =
      12, ///< ★ Named from what the player sees: the room of beds up the inn's
          ///< stairs (its render)
};

/// SceneId_GomaCaveEntrance's entrances.
enum {
  GOMA_CAVE_ENTRANCE_FROM_WORLD_MAP = 1,
  GOMA_CAVE_ENTRANCE_FROM_GOMA_CAVE_STREAM = 2,
};
/// SceneId_GomaCaveEntrance's exits.
enum {
  GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM = 1,
  GOMA_CAVE_ENTRANCE_TO_WORLD_MAP = 2,
};

/// SceneId_GomaCavePond's entrances.
enum {
  GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_WEST = 1,
  GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_SOUTH = 2,
  GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_EAST = 3,
};
/// SceneId_GomaCavePond's exits.
enum {
  GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_WEST = 1,
  GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_SOUTH = 2,
  GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_EAST = 3,
};

/// SceneId_GomaCavePillar's entrances.
enum {
  GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_WEST = 1,
  GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST = 2,
  GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST = 3,
  GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST = 4,
  GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_SOUTH = 5,
};
/// SceneId_GomaCavePillar's exits.
enum {
  GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST = 1,
  GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST = 2,
  GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH = 3,
  GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST = 4,
  GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST = 5,
};

/// SceneId_GomaCaveStream's entrances.
enum {
  GOMA_CAVE_STREAM_FROM_GOMA_CAVE_ENTRANCE = 1,
  GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_WEST = 2,
  GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_EAST = 3,
  GOMA_CAVE_STREAM_FROM_GOMA_CAVE_BRIDGE = 4,
  GOMA_CAVE_STREAM_FROM_VALE_GATE = 5,
};
/// SceneId_GomaCaveStream's exits.
enum {
  GOMA_CAVE_STREAM_TO_GOMA_CAVE_ENTRANCE = 1,
  GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_WEST = 2,
  GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_EAST = 3,
  GOMA_CAVE_STREAM_TO_GOMA_CAVE_BRIDGE = 4,
  GOMA_CAVE_STREAM_TO_VALE_GATE = 5,
};

/// SceneId_GomaCaveBridge's entrances.
enum {
  GOMA_CAVE_BRIDGE_FROM_GOMA_CAVE_STREAM = 1,
  GOMA_CAVE_BRIDGE_FROM_WORLD_MAP = 2,
};
/// SceneId_GomaCaveBridge's exits.
enum {
  GOMA_CAVE_BRIDGE_TO_GOMA_CAVE_STREAM = 1,
  GOMA_CAVE_BRIDGE_TO_WORLD_MAP = 2,
};

/// SceneId_Bilibin's entrances.
enum {
  BILIBIN_FROM_WORLD_MAP = 1,
  BILIBIN_FROM_MCCOY_PALACE_GROUNDS = 2,
  BILIBIN_FROM_INN = 3,
  BILIBIN_FROM_HOUSE_EAST = 4,
  BILIBIN_FROM_HOUSE_WEST = 5,
  BILIBIN_FROM_HOUSE_NORTH = 6,
  BILIBIN_FROM_ITEM_SHOP = 7,
  BILIBIN_FROM_WEAPON_AND_ARMOR_SHOP = 8,
  BILIBIN_FROM_SANCTUM = 9,
  BILIBIN_FROM_BILIBIN_TREASURY_WEST = 10,
  BILIBIN_FROM_BILIBIN_TREASURY_EAST = 11,
};
/// SceneId_Bilibin's exits.
enum {
  BILIBIN_TO_WORLD_MAP = 1,
  BILIBIN_TO_MCCOY_PALACE_GROUNDS = 2,
  BILIBIN_TO_INN =
      3, ///< ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
  BILIBIN_TO_HOUSE_EAST = 4, ///< ★ Named from what the player sees: a house
                             ///< with beds, a table and a hearth (its render)
  BILIBIN_TO_HOUSE_WEST = 5,
  BILIBIN_TO_HOUSE_NORTH = 6,
  BILIBIN_TO_ITEM_SHOP =
      7, ///< ★ Named from what the player sees: the shop of shelves and crates
         ///< whose keeper sells items (its render)
  BILIBIN_TO_WEAPON_AND_ARMOR_SHOP =
      8, ///< ★ Named from what the player sees: the shop hung with swords and
         ///< shields, with a seller of each behind its counter (its render)
  BILIBIN_TO_SANCTUM = 9,
  BILIBIN_TO_BILIBIN_TREASURY_WEST = 10,
  BILIBIN_TO_BILIBIN_TREASURY_EAST = 11,
};

/// SceneId_BilibinInteriors's entrances.
enum {
  BILIBIN_INTERIORS_HOUSE_EAST = 1,  ///< the room behind Bilibin's house east
                                     ///< door; through BILIBIN_TO_HOUSE_EAST
  BILIBIN_INTERIORS_HOUSE_WEST = 2,  ///< the room behind Bilibin's house west
                                     ///< door; through BILIBIN_TO_HOUSE_WEST
  BILIBIN_INTERIORS_HOUSE_NORTH = 3, ///< the room behind Bilibin's house north
                                     ///< door; through BILIBIN_TO_HOUSE_NORTH
  BILIBIN_INTERIORS_WEAPON_AND_ARMOR_SHOP =
      4, ///< ★ Named from what the player sees: the shop hung with swords and
         ///< shields, with a seller of each behind its counter (its render);
         ///< through BILIBIN_TO_WEAPON_AND_ARMOR_SHOP
  BILIBIN_INTERIORS_ITEM_SHOP =
      5, ///< ★ Named from what the player sees: the shop of shelves and crates
         ///< whose keeper sells items (its render); through
         ///< BILIBIN_TO_ITEM_SHOP
  BILIBIN_INTERIORS_INN = 6, ///< the room behind Bilibin's inn door; the room
                             ///< holds the inn; through BILIBIN_TO_INN
  BILIBIN_INTERIORS_INN_UPSTAIRS_FROM_INN =
      7, ///< ★ Named from what the player sees: the rooms of beds and crates up
         ///< the inn's stairs (its render)
  BILIBIN_INTERIORS_INN_FROM_INN_UPSTAIRS =
      8, ///< ★ Named from what the player sees: Bilibin's inn, with the
         ///< restaurant beside its counter (its render)
};
/// SceneId_BilibinInteriors's exits.
enum {
  BILIBIN_INTERIORS_HOUSE_EAST_TO_BILIBIN = 1,
  BILIBIN_INTERIORS_HOUSE_WEST_TO_BILIBIN = 2,
  BILIBIN_INTERIORS_HOUSE_NORTH_TO_BILIBIN = 3,
  BILIBIN_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN = 4,
  BILIBIN_INTERIORS_ITEM_SHOP_TO_BILIBIN = 5,
  BILIBIN_INTERIORS_INN_TO_BILIBIN = 6,
  BILIBIN_INTERIORS_INN_UPSTAIRS_TO_INN =
      7, ///< ★ Named from what the player sees: Bilibin's inn, with the
         ///< restaurant beside its counter (its render)
  BILIBIN_INTERIORS_INN_TO_INN_UPSTAIRS =
      8, ///< ★ Named from what the player sees: the rooms of beds and crates up
         ///< the inn's stairs (its render)
};

/// SceneId_BilibinTreasury's entrances.
enum {
  BILIBIN_TREASURY_FROM_BILIBIN_SOUTH = 1,
  BILIBIN_TREASURY_LOWER_FROM_UPPER =
      2, ///< ★ Named from what the player sees: the small room at the foot of
         ///< the ladder, entered from Bilibin's west edge (its render)
  BILIBIN_TREASURY_UPPER_FROM_LOWER =
      3, ///< ★ Named from what the player sees: the cave up the ladder with the
         ///< golden statue and Gust (its render)
  BILIBIN_TREASURY_FROM_BILIBIN_NORTH = 4,
};
/// SceneId_BilibinTreasury's exits.
enum {
  BILIBIN_TREASURY_TO_BILIBIN_SOUTH = 1,
  BILIBIN_TREASURY_LOWER_TO_UPPER =
      2, ///< ★ Named from what the player sees: the cave up the ladder with the
         ///< golden statue and Gust (its render)
  BILIBIN_TREASURY_UPPER_TO_LOWER =
      3, ///< ★ Named from what the player sees: the small room at the foot of
         ///< the ladder, entered from Bilibin's west edge (its render)
  BILIBIN_TREASURY_TO_BILIBIN_NORTH = 4,
};

/// SceneId_McCoyPalace's entrances.
enum {
  MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST = 1,
  MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHWEST_LANDING =
      2, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHEAST_LANDING =
      3, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_WEST =
      4, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_EAST =
      5, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_FROM_TREASURE_ROOM =
      6, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHWEST_LANDING =
      7, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHEAST_LANDING =
      8, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM =
      9, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_WEST =
      10, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_EAST =
      11, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_WEST_BEDROOM =
      12, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_EAST_BEDROOM =
      13, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_SOUTHWEST_LANDING_FROM_ENTRANCE_ROOM =
      14, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_SOUTHWEST_LANDING_FROM_SOUTHWEST_TOWER =
      15, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_SOUTHEAST_LANDING_FROM_SOUTHEAST_TOWER =
      16, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_SOUTHEAST_LANDING_FROM_ENTRANCE_ROOM =
      17, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_WEST_BEDROOM_FROM_LADY_MCCOYS_ROOM =
      18, ///< ★ Named from what the player sees: the bedroom through the west
          ///< door of Lady McCoy's room, with a bookcase (its render)
  MCCOY_PALACE_EAST_BEDROOM_FROM_LADY_MCCOYS_ROOM =
      19, ///< ★ Named from what the player sees: the bedroom through the east
          ///< door, with a wardrobe and the woman with the silk nightgown (its
          ///< render)
  MCCOY_PALACE_NORTHEAST_LANDING_FROM_ENTRANCE_ROOM =
      20, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_NORTHEAST_LANDING_FROM_NORTHEAST_TOWER =
      21, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_NORTHWEST_LANDING_FROM_ENTRANCE_ROOM =
      22, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_NORTHWEST_LANDING_FROM_NORTHWEST_TOWER =
      23, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_NORTHWEST_TOWER_FROM_NORTHWEST_LANDING =
      24, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_NORTHEAST_TOWER_FROM_NORTHEAST_LANDING =
      25, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_SOUTHWEST_TOWER_FROM_SOUTHWEST_LANDING =
      26, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_SOUTHEAST_TOWER_FROM_SOUTHEAST_LANDING =
      27, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM =
      28, ///< ★ Named from the game: "Bilibin's great treasures"
          ///< (MsgBilibinDoThinkTheseBilibinsGreat)
  MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST = 29,
};
/// SceneId_McCoyPalace's exits.
enum {
  MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST = 1,
  MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHWEST_LANDING =
      2, ///< ★ Named from what the player sees: the small stair rooms between
         ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHEAST_LANDING =
      3, ///< ★ Named from what the player sees: the small stair rooms between
         ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_WEST =
      4, ///< ★ Named from the game: "This is Lady McCoy's room"
         ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_EAST =
      5, ///< ★ Named from the game: "This is Lady McCoy's room"
         ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM =
      6, ///< ★ Named from the game: "Bilibin's great treasures"
         ///< (MsgBilibinDoThinkTheseBilibinsGreat)
  MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHWEST_LANDING =
      7, ///< ★ Named from what the player sees: the small stair rooms between
         ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHEAST_LANDING =
      8, ///< ★ Named from what the player sees: the small stair rooms between
         ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM =
      9, ///< ★ Named from what the player sees: the room behind McCoy's door
         ///< where Lord McCoy sits (its render)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_WEST =
      10, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_EAST =
      11, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_WEST_BEDROOM =
      12, ///< ★ Named from what the player sees: the bedroom through the west
          ///< door of Lady McCoy's room, with a bookcase (its render)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_EAST_BEDROOM =
      13, ///< ★ Named from what the player sees: the bedroom through the east
          ///< door, with a wardrobe and the woman with the silk nightgown (its
          ///< render)
  MCCOY_PALACE_SOUTHWEST_LANDING_TO_ENTRANCE_ROOM =
      14, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_SOUTHWEST_LANDING_TO_SOUTHWEST_TOWER =
      15, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_SOUTHEAST_LANDING_TO_SOUTHEAST_TOWER =
      16, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_SOUTHEAST_LANDING_TO_ENTRANCE_ROOM =
      17, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_WEST_BEDROOM_TO_LADY_MCCOYS_ROOM =
      18, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_EAST_BEDROOM_TO_LADY_MCCOYS_ROOM =
      19, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_NORTHEAST_LANDING_TO_ENTRANCE_ROOM =
      20, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_NORTHEAST_LANDING_TO_NORTHEAST_TOWER =
      21, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_NORTHWEST_LANDING_TO_ENTRANCE_ROOM =
      22, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_NORTHWEST_LANDING_TO_NORTHWEST_TOWER =
      23, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_NORTHWEST_TOWER_TO_NORTHWEST_LANDING =
      24, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_NORTHEAST_TOWER_TO_NORTHEAST_LANDING =
      25, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_SOUTHWEST_TOWER_TO_SOUTHWEST_LANDING =
      26, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_SOUTHEAST_TOWER_TO_SOUTHEAST_LANDING =
      27, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_TREASURE_ROOM_TO_ENTRANCE_ROOM =
      28, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM =
      29, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
};

/// SceneId_McCoyPalaceGrounds's entrances.
enum {
  MCCOY_PALACE_GROUNDS_FROM_BILIBIN = 1,
  MCCOY_PALACE_GROUNDS_FROM_MCCOY_PALACE = 2,
};
/// SceneId_McCoyPalaceGrounds's exits.
enum {
  MCCOY_PALACE_GROUNDS_TO_BILIBIN = 1,
  MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE = 2,
  MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST = 10,
};

/// SceneId_BilibinBarricade's entrances.
enum {
  BILIBIN_BARRICADE_FROM_WORLD_MAP_WEST = 1,
  BILIBIN_BARRICADE_FROM_WORLD_MAP_EAST = 2,
};
/// SceneId_BilibinBarricade's exits.
enum {
  BILIBIN_BARRICADE_TO_WORLD_MAP_EAST = 1,
  BILIBIN_BARRICADE_TO_WORLD_MAP_WEST = 2,
};

/// SceneId_Kolima's entrances.
enum {
  KOLIMA_FROM_WORLD_MAP = 1,
  KOLIMA_FROM_INN = 3,
  KOLIMA_FROM_ITEM_SHOP = 4,
  KOLIMA_FROM_HOUSE_SOUTH = 5,
  KOLIMA_FROM_WEAPON_AND_ARMOR_SHOP = 6,
  KOLIMA_FROM_HOUSE_NORTH = 7,
  KOLIMA_FROM_KOLIMA_TUNNEL = 8,
};
/// SceneId_Kolima's exits.
enum {
  KOLIMA_TO_WORLD_MAP = 1,
  KOLIMA_TO_WORLD_MAP_EAST = 2,
  KOLIMA_TO_INN = 5,
  KOLIMA_TO_ITEM_SHOP = 6,
  KOLIMA_TO_HOUSE_SOUTH = 7,
  KOLIMA_TO_WEAPON_AND_ARMOR_SHOP = 8,
  KOLIMA_TO_HOUSE_NORTH = 9,
  KOLIMA_TO_KOLIMA_TUNNEL = 10,
};

/// SceneId_KolimaInteriors's entrances.
enum {
  KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP =
      1, ///< ★ Named from what the player sees: the room up the shop's stair, a
         ///< bookcase and two villagers (its render)
  KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR =
      2, ///< ★ Named from what the player sees: the room up the shop's stair, a
         ///< bookcase and two villagers (its render)
  KOLIMA_INTERIORS_ITEM_SHOP_FROM_KOLIMA =
      3, ///< ★ Named from what the player sees: the room behind the item sign,
         ///< the item seller at her counter (its render)
  KOLIMA_INTERIORS_INN_FROM_KOLIMA =
      4, ///< ★ Named from what the player sees: the room behind the INN sign on
         ///< the village's northwest tree, the innkeeper at his counter (its
         ///< render)
  KOLIMA_INTERIORS_INN_FROM_INN_SECOND_FLOOR =
      5, ///< ★ Named from what the player sees: the room behind the INN sign on
         ///< the village's northwest tree, the innkeeper at his counter (its
         ///< render)
  KOLIMA_INTERIORS_INN_THIRD_FLOOR_FROM_INN_SECOND_FLOOR =
      6, ///< ★ Named from what the player sees: the top room of the inn's tree,
         ///< a bed and a stranded merchant (its render)
  KOLIMA_INTERIORS_HOUSE_FROM_KOLIMA_SOUTH =
      7, ///< ★ Named from what the player sees: the house of the father with a
         ///< secret, a storeroom with a ladder behind it (its render)
  KOLIMA_INTERIORS_HOUSE_FROM_KOLIMA_NORTH =
      8, ///< ★ Named from what the player sees: the house of the father with a
         ///< secret, a storeroom with a ladder behind it (its render)
  KOLIMA_INTERIORS_FROM_KOLIMA_TUNNEL = 9,
  KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_FROM_KOLIMA =
      10, ///< ★ Named from what the player sees: the weapon and armor sellers
          ///< side by side at one counter (its render)
  KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_FROM_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR =
      11, ///< ★ Named from what the player sees: the weapon and armor sellers
          ///< side by side at one counter (its render)
  KOLIMA_INTERIORS_INN_SECOND_FLOOR_FROM_INN =
      12, ///< ★ Named from what the player sees: the room up the inn's stair, a
          ///< bed and a lumberjack staying the night (its render)
  KOLIMA_INTERIORS_INN_SECOND_FLOOR_FROM_INN_THIRD_FLOOR =
      13, ///< ★ Named from what the player sees: the room up the inn's stair, a
          ///< bed and a lumberjack staying the night (its render)
  KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR =
      14, ///< ★ Named from what the player sees: the top room of the shop's
          ///< tree, a lumberjack and his two children (its render)
};
/// SceneId_KolimaInteriors's exits.
enum {
  KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_TO_WEAPON_AND_ARMOR_SHOP =
      1, ///< ★ Named from what the player sees: the weapon and armor sellers
         ///< side by side at one counter (its render)
  KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_TO_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR =
      2, ///< ★ Named from what the player sees: the top room of the shop's
         ///< tree, a lumberjack and his two children (its render)
  KOLIMA_INTERIORS_ITEM_SHOP_TO_KOLIMA = 3,
  KOLIMA_INTERIORS_INN_TO_KOLIMA = 4,
  KOLIMA_INTERIORS_INN_TO_INN_SECOND_FLOOR =
      5, ///< ★ Named from what the player sees: the room up the inn's stair, a
         ///< bed and a lumberjack staying the night (its render)
  KOLIMA_INTERIORS_INN_THIRD_FLOOR_TO_INN_SECOND_FLOOR =
      6, ///< ★ Named from what the player sees: the room up the inn's stair, a
         ///< bed and a lumberjack staying the night (its render)
  KOLIMA_INTERIORS_HOUSE_TO_KOLIMA_SOUTH = 7,
  KOLIMA_INTERIORS_HOUSE_TO_KOLIMA_NORTH = 8,
  KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_KOLIMA = 9,
  KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR =
      10, ///< ★ Named from what the player sees: the room up the shop's stair,
          ///< a bookcase and two villagers (its render)
  KOLIMA_INTERIORS_INN_SECOND_FLOOR_TO_INN =
      11, ///< ★ Named from what the player sees: the room behind the INN sign
          ///< on the village's northwest tree, the innkeeper at his counter
          ///< (its render)
  KOLIMA_INTERIORS_INN_SECOND_FLOOR_TO_INN_THIRD_FLOOR =
      12, ///< ★ Named from what the player sees: the top room of the inn's
          ///< tree, a bed and a stranded merchant (its render)
  KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR_TO_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR =
      13, ///< ★ Named from what the player sees: the room up the shop's stair,
          ///< a bookcase and two villagers (its render)
  KOLIMA_INTERIORS_TO_KOLIMA_TUNNEL = 14,
};

/// SceneId_KolimaTunnel's entrances.
enum {
  KOLIMA_TUNNEL_FROM_KOLIMA_INTERIORS = 1,
  KOLIMA_TUNNEL_FROM_KOLIMA = 2,
};
/// SceneId_KolimaTunnel's exits.
enum {
  KOLIMA_TUNNEL_TO_KOLIMA_INTERIORS = 1,
  KOLIMA_TUNNEL_TO_KOLIMA = 2,
};

/// SceneId_KolimaBridge's entrances.
enum {
  KOLIMA_BRIDGE_FROM_WORLD_MAP_SOUTH = 1,
  KOLIMA_BRIDGE_FROM_WORLD_MAP_NORTH = 2,
};
/// SceneId_KolimaBridge's exits.
enum {
  KOLIMA_BRIDGE_TO_WORLD_MAP_SOUTH = 1,
  KOLIMA_BRIDGE_TO_WORLD_MAP_NORTH = 2,
};

/// SceneId_KolimaForestEntrance's entrances.
enum {
  /// Only the two unused exits lead here, and the entrance table has no row
  /// for it.
  KOLIMA_FOREST_ENTRANCE_UNUSED = 1,
  KOLIMA_FOREST_ENTRANCE_FROM_KOLIMA_FOREST_GLADE = 2,
  KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP = 4,
};
/// SceneId_KolimaForestEntrance's exits.
enum {
  KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE = 1,
  /// Two exits no event takes, back into this map
  /// (KOLIMA_FOREST_ENTRANCE_UNUSED).
  KOLIMA_FOREST_ENTRANCE_UNUSED_EXIT_A = 2,
  KOLIMA_FOREST_ENTRANCE_UNUSED_EXIT_B = 3,
  KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP = 4,
};

/// SceneId_KolimaForestGlade's entrances.
enum {
  KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_ENTRANCE = 1,
  KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_CLIFF = 2,
};
/// SceneId_KolimaForestGlade's exits.
enum {
  KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF = 1,
  KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE = 2,
};

/// SceneId_KolimaForestCliff's entrances.
enum {
  KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_GLADE = 1,
  KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_FLOODGATE = 2,
};
/// SceneId_KolimaForestCliff's exits.
enum {
  KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_FLOODGATE = 1,
  KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_GLADE = 2,
};

/// SceneId_KolimaForestFloodgate's entrances.
enum {
  KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_CLIFF = 1,
  KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE = 2,
};
/// SceneId_KolimaForestFloodgate's exits.
enum {
  KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_CLIFF = 1,
  KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE = 2,
};

/// SceneId_KolimaForestTree's entrances.
enum {
  KOLIMA_FOREST_TREE_FROM_KOLIMA_FOREST_FLOODGATE = 1,
  KOLIMA_FOREST_TREE_FROM_TRET_TREE_INTERIORS = 2,
};
/// SceneId_KolimaForestTree's exits.
enum {
  KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE = 1,
  KOLIMA_FOREST_TREE_TO_TRET_TREE_INTERIORS = 2,
};

/// SceneId_TretTreeInteriors's entrances.
enum {
  TRET_TREE_INTERIORS_FROM_KOLIMA_FOREST_TREE = 1,
  TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHWEST = 2,
  TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHEAST = 3,
  TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTH = 4,
  TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTHEAST = 5,
  TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTHWEST = 6,
  TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTH = 7,
  TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTH_NORTHWEST = 8,
  TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTH_NORTHEAST = 9,
  TRET_TREE_INTERIORS_ENTRANCE_FROM_SECOND_FLOOR =
      11, ///< ★ Named from what the player sees: the room entered from Kolima
          ///< Forest through the door at Tret's foot (its render)
  TRET_TREE_INTERIORS_SECOND_FLOOR_FROM_ENTRANCE =
      12, ///< ★ Named from what the player sees: the webbed room one floor
          ///< above the entrance, reached up a vine (its render)
  TRET_TREE_INTERIORS_THIRD_FLOOR_FROM_FOURTH_FLOOR =
      13, ///< ★ Named from what the player sees: the webbed room with a chest
          ///< and a Psynergy Stone, one floor higher (its render)
  TRET_TREE_INTERIORS_FOURTH_FLOOR_FROM_THIRD_FLOOR =
      14, ///< ★ Named from what the player sees: the bare room above, reached
          ///< up a vine (its render)
  TRET_TREE_INTERIORS_TRET_FROM_ENTRANCE =
      15, ///< ★ Named from what the player sees: the lowest room, where Tret's
          ///< face looks out of the wall (its render)
  TRET_TREE_INTERIORS_ENTRANCE_FROM_TRET =
      16, ///< ★ Named from what the player sees: the room entered from Kolima
          ///< Forest through the door at Tret's foot (its render)
  TRET_TREE_INTERIORS_AFTER_FIGHT_TRET = 19, ///< Event_FightTret
};
/// SceneId_TretTreeInteriors's exits.
enum {
  TRET_TREE_INTERIORS_TO_KOLIMA_FOREST_TREE = 1,
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST =
      2, ///< ★ Named from what the player sees: the lowest room, where Tret's
         ///< face looks out of the wall (its render)
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST =
      3, ///< ★ Named from what the player sees: the lowest room, where Tret's
         ///< face looks out of the wall (its render)
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH =
      4, ///< ★ Named from what the player sees: the lowest room, where Tret's
         ///< face looks out of the wall (its render)
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTHEAST =
      5, ///< ★ Named from what the player sees: the lowest room, where Tret's
         ///< face looks out of the wall (its render)
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTHWEST =
      6, ///< ★ Named from what the player sees: the lowest room, where Tret's
         ///< face looks out of the wall (its render)
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH =
      7, ///< ★ Named from what the player sees: the lowest room, where Tret's
         ///< face looks out of the wall (its render)
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST =
      8, ///< ★ Named from what the player sees: the lowest room, where Tret's
         ///< face looks out of the wall (its render)
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST =
      9, ///< ★ Named from what the player sees: the lowest room, where Tret's
         ///< face looks out of the wall (its render)
  TRET_TREE_INTERIORS_ENTRANCE_TO_SECOND_FLOOR =
      10, ///< ★ Named from what the player sees: the webbed room one floor
          ///< above the entrance, reached up a vine (its render)
  TRET_TREE_INTERIORS_SECOND_FLOOR_TO_ENTRANCE =
      11, ///< ★ Named from what the player sees: the room entered from Kolima
          ///< Forest through the door at Tret's foot (its render)
  TRET_TREE_INTERIORS_THIRD_FLOOR_TO_FOURTH_FLOOR =
      12, ///< ★ Named from what the player sees: the bare room above, reached
          ///< up a vine (its render)
  TRET_TREE_INTERIORS_FOURTH_FLOOR_TO_THIRD_FLOOR =
      13, ///< ★ Named from what the player sees: the webbed room with a chest
          ///< and a Psynergy Stone, one floor higher (its render)
  TRET_TREE_INTERIORS_TRET_TO_ENTRANCE =
      14, ///< ★ Named from what the player sees: the room entered from Kolima
          ///< Forest through the door at Tret's foot (its render)
  TRET_TREE_INTERIORS_ENTRANCE_TO_TRET =
      15, ///< ★ Named from what the player sees: the lowest room, where Tret's
          ///< face looks out of the wall (its render)
};

/// SceneId_TretTreeBranch's entrances.
enum {
  TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTH_SOUTHWEST = 1,
  TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTH_SOUTHEAST = 2,
  TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTH = 3,
  TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTHEAST = 4,
  TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTH = 5,
  TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTHWEST = 6,
  TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTH_NORTHWEST = 7,
  TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTH_NORTHEAST = 8,
};
/// SceneId_TretTreeBranch's exits.
enum {
  TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_SOUTH_SOUTHWEST = 1,
  TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_SOUTH_SOUTHEAST = 2,
  TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_SOUTH = 3,
  TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_SOUTHEAST = 4,
  TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_NORTH = 5,
  TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_NORTHWEST = 6,
  TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_NORTH_NORTHWEST = 7,
  TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_NORTH_NORTHEAST = 8,
};

/// SceneId_BilibinCaveIce's entrances.
enum {
  BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_WEST = 1,
  BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_EAST = 2,
  BILIBIN_CAVE_ICE_FROM_WORLD_MAP = 3,
  BILIBIN_CAVE_ICE_FROM_CHESTS = 4,
  BILIBIN_CAVE_ICE_CHESTS_FROM_BILIBIN_CAVE_ICE =
      5, ///< ★ Named from what the player sees: the small nook beyond the north
         ///< doorway that holds two chests (its render)
};
/// SceneId_BilibinCaveIce's exits.
enum {
  BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST = 1,
  BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST = 2,
  BILIBIN_CAVE_ICE_TO_WORLD_MAP = 3,
  BILIBIN_CAVE_ICE_TO_CHESTS =
      4, ///< ★ Named from what the player sees: the small nook beyond the north
         ///< doorway that holds two chests (its render)
  BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE = 5,
};

/// SceneId_BilibinCaveRapier's entrances.
enum {
  BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_WEST = 1,
  BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_EAST = 2,
  BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_WEST = 3,
  BILIBIN_CAVE_RAPIER_FROM_TORCHES = 4,
  BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_EAST = 5,
  BILIBIN_CAVE_RAPIER_TORCHES_FROM_BILIBIN_CAVE_RAPIER =
      6, ///< ★ Named from what the player sees: the nook where a fire burns
         ///< between two torches at the way on into the cave (its render)
};
/// SceneId_BilibinCaveRapier's exits.
enum {
  BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_WEST = 1,
  BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_EAST = 2,
  BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_WEST = 3,
  BILIBIN_CAVE_RAPIER_TO_TORCHES =
      4, ///< ★ Named from what the player sees: the nook where a fire burns
         ///< between two torches at the way on into the cave (its render)
  BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_EAST = 5,
  BILIBIN_CAVE_RAPIER_TORCHES_TO_BILIBIN_CAVE_RAPIER = 6,
};

/// SceneId_BilibinCaveEntrance's entrances.
enum {
  BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP = 1,
  BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_SOUTH = 2,
  BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_NORTH = 3,
};
/// SceneId_BilibinCaveEntrance's exits.
enum {
  BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP = 1,
  BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH = 2,
  BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH = 3,
};

/// SceneId_Imil's entrances.
enum {
  IMIL_FROM_HOUSE_SOUTHWEST = 1,
  IMIL_EAST_UNUSED =
      2, ///< at the village's east edge, facing west; no exit leads here
  IMIL_FROM_HOUSE_WEST = 3,
  IMIL_FROM_HOUSE_NORTHEAST = 4,
  IMIL_FROM_IMIL_INTERIORS_EAST = 5,
  IMIL_FROM_IMIL_INTERIORS_NORTHEAST = 6,
  IMIL_FROM_IMIL_INTERIORS_WEST = 7,
  IMIL_FROM_IMIL_INTERIORS_SOUTHWEST = 8,
  IMIL_FROM_IMIL_FALLS_CAVE = 9,
};
/// SceneId_Imil's exits.
enum {
  IMIL_TO_HOUSE_SOUTHWEST = 1, ///< ★ Named from what the player sees: a house
                               ///< with beds, a table and a hearth (its render)
  IMIL_TO_INN =
      2, ///< ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
  IMIL_TO_HOUSE_WEST = 3,
  IMIL_TO_HOUSE_NORTHEAST = 4,
  IMIL_TO_IMIL_INTERIORS_EAST = 5,
  IMIL_TO_IMIL_INTERIORS_NORTHEAST = 6,
  IMIL_TO_IMIL_FALLS_CAVE = 8,
  IMIL_TO_WORLD_MAP_SOUTH = 11,
  IMIL_TO_WORLD_MAP_NORTH = 12,
};

/// SceneId_ImilInteriors's entrances.
enum {
  IMIL_INTERIORS_HOUSE_SOUTHWEST =
      1, ///< the room behind Imil's house southwest door; through
         ///< IMIL_TO_HOUSE_SOUTHWEST
  IMIL_INTERIORS_FROM_IMIL_WEST_SOUTHWEST = 2,
  IMIL_INTERIORS_HOUSE_WEST =
      3, ///< the room behind Imil's house west door; through IMIL_TO_HOUSE_WEST
  IMIL_INTERIORS_HOUSE_NORTHEAST =
      4, ///< the room behind Imil's house northeast door; through
         ///< IMIL_TO_HOUSE_NORTHEAST
  IMIL_INTERIORS_INN = 5,
  IMIL_INTERIORS_INN_UPSTAIRS_FROM_INN =
      6, ///< ★ Named from what the player sees: the room up the inn's stairs,
         ///< with barrels (its render)
  IMIL_INTERIORS_FROM_IMIL_EAST_NORTHEAST = 7,
  IMIL_INTERIORS_INN_FROM_INN_UPSTAIRS =
      8, ///< ★ Named from what the player sees: Imil's inn, its innkeeper
         ///< behind the counter (its render)
};
/// SceneId_ImilInteriors's exits.
enum {
  IMIL_INTERIORS_HOUSE_WEST_TO_IMIL = 1,
  IMIL_INTERIORS_TO_IMIL_WEST_SOUTHWEST = 2,
  IMIL_INTERIORS_TO_IMIL_EAST = 3,
  IMIL_INTERIORS_TO_IMIL = 4,
  IMIL_INTERIORS_TO_IMIL_SOUTH_SOUTHWEST = 5,
  IMIL_INTERIORS_INN_UPSTAIRS_TO_INN =
      6, ///< ★ Named from what the player sees: Imil's inn, its innkeeper
         ///< behind the counter (its render)
  IMIL_INTERIORS_TO_IMIL_EAST_NORTHEAST = 7,
  IMIL_INTERIORS_INN_TO_INN_UPSTAIRS =
      8, ///< ★ Named from what the player sees: the room up the inn's stairs,
         ///< with barrels (its render)
};

/// SceneId_ImilFallsCave's entrances.
enum {
  IMIL_FALLS_CAVE_FROM_IMIL = 1,
};
/// SceneId_ImilFallsCave's exits.
enum {
  IMIL_FALLS_CAVE_TO_IMIL = 1,
};

/// SceneId_MercuryLighthouseEntrance's entrances.
enum {
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR = 1,
  MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_FROM_GODDESS =
      2, ///< ★ Named from what the player sees: the hall inside the door, lined
         ///< with six statues, where the Lizard Man blocks Mia (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES =
      3, ///< ★ Named from the game: "He who honors the heart of the goddess"
         ///< (MsgMercuryWhoHonorsHeart)
  MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_WEST =
      4, ///< ★ Named from the game: "He who honors the heart of the goddess"
         ///< (MsgMercuryWhoHonorsHeart)
  MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_EAST =
      5, ///< ★ Named from the game: "He who honors the heart of the goddess"
         ///< (MsgMercuryWhoHonorsHeart)
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE = 6,
  MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_WEST_WATERFALLS =
      7, ///< ★ Named from what the player sees: the long room of two pools fed
         ///< by spouts (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_EAST =
      8, ///< ★ Named from what the player sees: the long room of two pools fed
         ///< by spouts (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_WEST =
      9, ///< ★ Named from what the player sees: the long room of two pools fed
         ///< by spouts (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_EAST_WATERFALLS =
      10, ///< ★ Named from what the player sees: the long room of two pools fed
          ///< by spouts (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST = 11,
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST = 12,
  MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_CHEST_FOUNTAIN =
      13, ///< ★ Named from what the player sees: the room whose spout and pool
          ///< are dry until the pipes upstairs run (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST = 14,
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST = 15,
  MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_FROM_DRY_FOUNTAIN =
      16, ///< ★ Named from what the player sees: the pool room with a chest on
          ///< its ledge (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_FOUNTAINS =
      17, ///< ★ Named from what the player sees: the western of the two rooms
          ///< of three waterfalls (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS =
      18, ///< ★ Named from what the player sees: the western of the two rooms
          ///< of three waterfalls (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_FOUNTAINS =
      19, ///< ★ Named from what the player sees: the eastern of the two rooms
          ///< of three waterfalls, with a chest (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS =
      20, ///< ★ Named from what the player sees: the eastern of the two rooms
          ///< of three waterfalls, with a chest (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE = 25,
  /// Where Mia tries the doorway again and again while the Lizard Man
  /// blocks it.
  MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN = 30,
  /// Where the party comes back from the battle with the Lizard Man.
  MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN = 31,
  MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_UNUSED_NORTH =
      60, ///< no exit leads here
  MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_UNUSED_MIDDLE =
      99, ///< no exit leads here
};
/// SceneId_MercuryLighthouseEntrance's exits.
enum {
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR = 1,
  MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_TO_GODDESS =
      2, ///< ★ Named from the game: "He who honors the heart of the goddess"
         ///< (MsgMercuryWhoHonorsHeart)
  MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_SIX_STATUES =
      3, ///< ★ Named from what the player sees: the hall inside the door, lined
         ///< with six statues, where the Lizard Man blocks Mia (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_WEST =
      4, ///< ★ Named from what the player sees: the long room of two pools fed
         ///< by spouts (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_EAST =
      5, ///< ★ Named from what the player sees: the long room of two pools fed
         ///< by spouts (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_WEST_WATERFALLS =
      6, ///< ★ Named from what the player sees: the western of the two rooms of
         ///< three waterfalls (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_WEST =
      7, ///< ★ Named from the game: "He who honors the heart of the goddess"
         ///< (MsgMercuryWhoHonorsHeart)
  MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_EAST =
      8, ///< ★ Named from the game: "He who honors the heart of the goddess"
         ///< (MsgMercuryWhoHonorsHeart)
  MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_EAST_WATERFALLS =
      9, ///< ★ Named from what the player sees: the eastern of the two rooms of
         ///< three waterfalls, with a chest (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST = 10,
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST = 11,
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST = 12,
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST = 13,
  MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_WEST_WATERFALLS =
      14, ///< ★ Named from what the player sees: the western of the two rooms
          ///< of three waterfalls (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_EAST_WATERFALLS =
      15, ///< ★ Named from what the player sees: the eastern of the two rooms
          ///< of three waterfalls, with a chest (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_TWIN_FOUNTAINS =
      16, ///< ★ Named from what the player sees: the long room of two pools fed
          ///< by spouts (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_TO_DRY_FOUNTAIN =
      17, ///< ★ Named from what the player sees: the room whose spout and pool
          ///< are dry until the pipes upstairs run (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS =
      18, ///< ★ Named from what the player sees: the long room of two pools fed
          ///< by spouts (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_TO_CHEST_FOUNTAIN =
      19, ///< ★ Named from what the player sees: the pool room with a chest on
          ///< its ledge (its render)
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_BEFORE_LIZARD_MAN = 20,
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE = 50,
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_AFTER_LIZARD_MAN = 63,
};

/// SceneId_MercuryLighthouseLower's entrances.
enum {
  MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST = 1,
  MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST = 2,
  MERCURY_LIGHTHOUSE_LOWER_WATER_PILLARS_FROM_STATUE =
      3, ///< ★ Named from what the player sees: the large pool crossed on
         ///< rising water pillars (its render)
  MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST = 4,
  MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_FROM_STATUE =
      5, ///< ★ Named from what the player sees: the room with three waterfalls
         ///< over a grating (its render)
  MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH = 6,
  MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_SOUTHWEST = 7,
  MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_UPPER = 8,
  MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATER_PILLARS =
      9, ///< ★ Named from what the player sees: the room where a statue blocks
         ///< Mia until Move pushes it aside (its render)
  MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATERFALLS =
      10, ///< ★ Named from what the player sees: the room where a statue blocks
          ///< Mia until Move pushes it aside (its render)
  MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH = 11,
  MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE = 15,
};
/// SceneId_MercuryLighthouseLower's exits.
enum {
  MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST = 1,
  MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST = 2,
  MERCURY_LIGHTHOUSE_LOWER_WATER_PILLARS_TO_STATUE =
      3, ///< ★ Named from what the player sees: the room where a statue blocks
         ///< Mia until Move pushes it aside (its render)
  MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST =
      4, ///< ★ Named from what the player sees: the one-way way down to the
         ///< eastern doors of the room the pipe fills (its render)
  MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_NORTHEAST =
      5, ///< ★ Named from what the player sees: the one-way way down to the
         ///< eastern doors of the room the pipe fills (its render)
  MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_UPPER = 6,
  MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH = 7,
  MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE =
      8, ///< ★ Named from what the player sees: the hole the leader drops
         ///< through to the ground floor (its render)
  MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATER_PILLARS =
      9, ///< ★ Named from what the player sees: the large pool crossed on
         ///< rising water pillars (its render)
  MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATERFALLS =
      10, ///< ★ Named from what the player sees: the room with three waterfalls
          ///< over a grating (its render)
  MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_TO_STATUE =
      11, ///< ★ Named from what the player sees: the room where a statue blocks
          ///< Mia until Move pushes it aside (its render)
  MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH = 12,
  MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE =
      13, ///< ★ Named from what the player sees: the pipes whose water carries
          ///< the party down to the ground floor and back (its render)
};

/// SceneId_MercuryLighthouseMiddle's entrances.
enum {
  MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_SOUTH = 1,
  MERCURY_LIGHTHOUSE_MIDDLE_PIPES_FROM_GATE =
      2, ///< ★ Named from what the player sees: the large maze of pipes with a
         ///< chest (its render)
  MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTH = 3,
  MERCURY_LIGHTHOUSE_MIDDLE_GATE_FROM_PIPES =
      4, ///< ★ Named from what the player sees: the room whose gate a statue on
         ///< the floor switch holds open (its render)
  MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST = 5,
  MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR = 6,
  MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST = 7,
  MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST = 8,
  MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST = 9,
  MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST = 10,
};
/// SceneId_MercuryLighthouseMiddle's exits.
enum {
  MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_NORTH = 1,
  MERCURY_LIGHTHOUSE_MIDDLE_GATE_TO_PIPES =
      2, ///< ★ Named from what the player sees: the large maze of pipes with a
         ///< chest (its render)
  MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST = 3,
  MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR = 4,
  MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST = 5,
  MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTH = 6,
  MERCURY_LIGHTHOUSE_MIDDLE_PIPES_TO_GATE =
      7, ///< ★ Named from what the player sees: the room whose gate a statue on
         ///< the floor switch holds open (its render)
  MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST = 8,
  MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST = 9,
  MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST = 10,
};

/// SceneId_MercuryLighthouseUpper's entrances.
enum {
  MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST = 1,
  MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST = 2,
  MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST = 3,
  MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_FROM_GATE =
      4, ///< ★ Named from what the player sees: the room where five pillars
         ///< stand in a row along the north wall (its render)
  MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST = 5,
  MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST = 6,
  MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER = 7,
  MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_FIVE_PILLARS =
      8, ///< ★ Named from what the player sees: the room of three doors, a gate
         ///< and the floor switch the fallen statue is set on (its render)
  MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_STONE_DOORS =
      9, ///< ★ Named from what the player sees: the room of three doors, a gate
         ///< and the floor switch the fallen statue is set on (its render)
  MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST = 10,
  MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS =
      11, ///< ★ Named from what the player sees: the room with a gap in the
          ///< floor that one more pillar is pushed into (its render)
  MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST = 12,
  MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP =
      13, ///< ★ Named from what the player sees: the long hall with a wall of
          ///< waterfalls (its render)
  MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_GATE =
      14, ///< ★ Named from what the player sees: the room of three stone doors
          ///< and the statue that slides off its ledge (its render)
  MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_STONE_DOORS_STATUE_FALLS =
      15, ///< ★ Named from what the player sees: the room of three doors, a
          ///< gate and the floor switch the fallen statue is set on (its
          ///< render)
  MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_GATE_STATUE_FALLS =
      16, ///< ★ Named from what the player sees: the room of three stone doors
          ///< and the statue that slides off its ledge (its render)
};
/// SceneId_MercuryLighthouseUpper's exits.
enum {
  MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST = 1,
  MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST = 2,
  MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST = 3,
  MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_TO_GATE =
      4, ///< ★ Named from what the player sees: the room of three doors, a gate
         ///< and the floor switch the fallen statue is set on (its render)
  MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST = 5,
  MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST = 6,
  MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER = 7,
  MERCURY_LIGHTHOUSE_UPPER_GATE_TO_STONE_DOORS =
      8, ///< ★ Named from what the player sees: the room of three stone doors
         ///< and the statue that slides off its ledge (its render)
  MERCURY_LIGHTHOUSE_UPPER_GATE_TO_FIVE_PILLARS =
      9, ///< ★ Named from what the player sees: the room where five pillars
         ///< stand in a row along the north wall (its render)
  MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST = 10,
  MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS =
      11, ///< ★ Named from what the player sees: the long hall with a wall of
          ///< waterfalls (its render)
  MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST = 12,
  MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP =
      13, ///< ★ Named from what the player sees: the room with a gap in the
          ///< floor that one more pillar is pushed into (its render)
  MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_TO_GATE =
      14, ///< ★ Named from what the player sees: the room of three doors, a
          ///< gate and the floor switch the fallen statue is set on (its
          ///< render)
  MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_TO_GATE_STATUE_FALLS =
      15, ///< ★ Named from what the player sees: the view cutting to the room
          ///< below as the statue falls and lands (its render)
  MERCURY_LIGHTHOUSE_UPPER_GATE_TO_STONE_DOORS_STATUE_FALLS =
      16, ///< ★ Named from what the player sees: the view cutting to the room
          ///< below as the statue falls and lands (its render)
};

/// SceneId_MercuryLighthouseExterior's entrances.
enum {
  MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_MIDDLE = 1,
  MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST = 2,
  MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST = 3,
  MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_FROM_WATERFALL =
      4, ///< ★ Named from what the player sees: the higher of the two walkways
         ///< up the outside, leading to the waterfall (its render)
  MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH =
      10, ///< ★ Named from what the player sees: the waterfall whose statue Ply
          ///< stops to open the way up (its render)
  MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP = 11,
  MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE = 12,
  MERCURY_LIGHTHOUSE_EXTERIOR_UNUSED = 13, ///< no exit leads here
  MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE = 15,
};
/// SceneId_MercuryLighthouseExterior's exits.
enum {
  MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE = 1,
  MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST = 2,
  MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST = 3,
  MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_TO_WATERFALL =
      4, ///< ★ Named from what the player sees: the waterfall whose statue Ply
         ///< stops to open the way up (its render)
  MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_PATH =
      5, ///< ★ Named from what the player sees: the higher of the two walkways
         ///< up the outside, leading to the waterfall (its render)
  MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_EAST = 6,
  MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE = 7,
  MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT =
      8, ///< ★ Named from what the player sees: the first rise up the
         ///< waterfall, shown from the world map as the lighthouse is lit (its
         ///< render)
  MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE = 9,
};

/// SceneId_MercuryLighthouseAerie's entrances.
enum {
  MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT =
      1, ///< the world map sends the party here after its lighthouse cut
  MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE =
      2, ///< MercuryLighthouseAerie_ConfrontSaturos
  MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR = 5,
};
/// SceneId_MercuryLighthouseAerie's exits.
enum {
  MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR = 1,
};

/// SceneId_FuchinTempleGrounds's entrances.
enum {
  FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP = 1,
  FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_TEMPLE_INTERIORS = 2,
  FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE = 3,
};
/// SceneId_FuchinTempleGrounds's exits.
enum {
  FUCHIN_TEMPLE_GROUNDS_TO_WORLD_MAP = 1,
  FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_TEMPLE_INTERIORS = 2,
  FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE = 3,
};

/// SceneId_FuchinTempleInteriors's entrances.
enum {
  FUCHIN_TEMPLE_INTERIORS_FROM_FUCHIN_TEMPLE_GROUNDS = 1,
  FUCHIN_TEMPLE_INTERIORS_FROM_XIAN = 2,
  FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE = 3,
  FUCHIN_TEMPLE_INTERIORS_FROM_HOUSE = 4,
};
/// SceneId_FuchinTempleInteriors's exits.
enum {
  FUCHIN_TEMPLE_INTERIORS_TO_FUCHIN_TEMPLE_GROUNDS = 1,
  FUCHIN_TEMPLE_INTERIORS_TO_XIAN = 2,
  FUCHIN_TEMPLE_INTERIORS_TO_LAMA_TEMPLE = 3,
  FUCHIN_TEMPLE_INTERIORS_TO_HOUSE =
      4, ///< ★ Named from what the player sees: the small room with a bed
         ///< behind the door Master Feh and Feizhi come out of (its render)
};

/// SceneId_FuchinFallsCaveEntrance's entrances.
enum {
  FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_TEMPLE_GROUNDS = 1,
  FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR = 2,
  FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE = 3,
  FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_FROM_NORTH_HALF =
      4, ///< ★ Named from what the player sees: the cave's southern part by its
         ///< mouth, a pool crossed on floating logs (its render)
  FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_FROM_SOUTH_HALF =
      5, ///< ★ Named from what the player sees: the cave's northern part,
         ///< stumps standing in the water (its render)
  FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE = 6,
  FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST = 7,
  FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST = 8,
};
/// SceneId_FuchinFallsCaveEntrance's exits.
enum {
  FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS = 1,
  FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ZEPHYR = 2,
  FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE = 3,
  FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_TO_NORTH_HALF =
      4, ///< ★ Named from what the player sees: the cave's northern part,
         ///< stumps standing in the water (its render)
  FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_TO_SOUTH_HALF =
      5, ///< ★ Named from what the player sees: the cave's southern part by its
         ///< mouth, a pool crossed on floating logs (its render)
  FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE = 6,
  FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST = 7,
  FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST = 8,
};

/// SceneId_FuchinFallsCaveZephyr's entrances.
enum {
  FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE = 1,
  FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_ENTRANCE = 2,
};
/// SceneId_FuchinFallsCaveZephyr's exits.
enum {
  FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE = 1,
  FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_ENTRANCE = 2,
};

/// SceneId_FuchinFallsCaveDragonsEye's entrances.
enum {
  FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS =
      1, ///< ★ Named from what the player sees: the round chamber where the
         ///< Dragon's Eye sits on its pedestal (its render)
  FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE = 2,
  FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_FROM_CHAMBER =
      3, ///< ★ Named from what the player sees: the pool crossed on floating
         ///< logs before the eye's chamber (its render)
  FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE = 4,
  FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR = 5,
};
/// SceneId_FuchinFallsCaveDragonsEye's exits.
enum {
  FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_TO_LOGS =
      1, ///< ★ Named from what the player sees: the pool crossed on floating
         ///< logs before the eye's chamber (its render)
  FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE = 2,
  FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_TO_CHAMBER =
      3, ///< ★ Named from what the player sees: the round chamber where the
         ///< Dragon's Eye sits on its pedestal (its render)
  FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ENTRANCE = 4,
  FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ZEPHYR = 5,
};

/// SceneId_FuchinFallsCaveArcticBlade's entrances.
enum {
  FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE = 1,
  FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF =
      2, ///< ★ Named from what the player sees: the northern part, a log pool
         ///< with the chest on its ledge (its render)
  FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_FROM_NORTH_HALF =
      3, ///< ★ Named from what the player sees: the southern part, entered from
         ///< the cave's mouth (its render)
  FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE = 4,
};
/// SceneId_FuchinFallsCaveArcticBlade's exits.
enum {
  FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE = 1,
  FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_TO_SOUTH_HALF =
      2, ///< ★ Named from what the player sees: the southern part, entered from
         ///< the cave's mouth (its render)
  FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_TO_NORTH_HALF =
      3, ///< ★ Named from what the player sees: the northern part, a log pool
         ///< with the chest on its ledge (its render)
  FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_ENTRANCE = 4,
};

/// SceneId_FuchinFallsCaveOrbOfForce's entrances.
enum {
  FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_FROM_SOUTH_HALF =
      1, ///< ★ Named from what the player sees: the small chamber holding the
         ///< Orb of Force's chest (its render)
  FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_FROM_NORTH_HALF =
      2, ///< ★ Named from what the player sees: the winding passage with the Ki
         ///< inscription (its render)
  FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST = 3,
  FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST = 4,
};
/// SceneId_FuchinFallsCaveOrbOfForce's exits.
enum {
  FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_TO_SOUTH_HALF =
      1, ///< ★ Named from what the player sees: the winding passage with the Ki
         ///< inscription (its render)
  FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_TO_NORTH_HALF =
      2, ///< ★ Named from what the player sees: the small chamber holding the
         ///< Orb of Force's chest (its render)
  FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST = 3,
  FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST = 4,
};

/// SceneId_FuchinFallsCaveDragonStatue's entrances.
enum {
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST = 1,
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST = 2,
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE = 3,
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_WEST =
      4, ///< ★ Named from what the player sees: the dark northern half where
         ///< the dragon statue stands (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE =
      5, ///< ★ Named from what the player sees: the dark northern half where
         ///< the dragon statue stands (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST =
      6, ///< ★ Named from what the player sees: the dark northern half where
         ///< the dragon statue stands (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_WEST =
      7, ///< ★ Named from what the player sees: the southern half, below the
         ///< statue's ledge (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_MIDDLE =
      8, ///< ★ Named from what the player sees: the southern half, below the
         ///< statue's ledge (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_EAST =
      9, ///< ★ Named from what the player sees: the southern half, below the
         ///< statue's ledge (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE = 10,
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_EAST = 11,
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_WEST = 12,
};
/// SceneId_FuchinFallsCaveDragonStatue's exits.
enum {
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST = 1,
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST = 2,
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE = 3,
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_WEST =
      4, ///< ★ Named from what the player sees: the southern half, below the
         ///< statue's ledge (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_MIDDLE =
      5, ///< ★ Named from what the player sees: the southern half, below the
         ///< statue's ledge (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_EAST =
      6, ///< ★ Named from what the player sees: the southern half, below the
         ///< statue's ledge (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_WEST =
      7, ///< ★ Named from what the player sees: the dark northern half where
         ///< the dragon statue stands (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_MIDDLE =
      8, ///< ★ Named from what the player sees: the dark northern half where
         ///< the dragon statue stands (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_EAST =
      9, ///< ★ Named from what the player sees: the dark northern half where
         ///< the dragon statue stands (its render)
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE = 10,
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_EAST = 11,
  FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_WEST = 12,
};

/// SceneId_MogallForestSign's entrances.
enum {
  MOGALL_FOREST_SIGN_FROM_WORLD_MAP = 1,
  MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST = 2,
  MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH = 3,
  MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING =
      4, ///< ★ Named from what the player sees: the clearing entered from the
         ///< world map, with the broken sign and a hollow stump (its render)
  MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_ENTRANCE =
      5, ///< ★ Named from what the player sees: the clearing of only trees
         ///< south of the entrance, on the way to the river (its render)
  MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_RIVER =
      6, ///< ★ Named from what the player sees: the clearing of only trees
         ///< south of the entrance, on the way to the river (its render)
  MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING =
      7, ///< ★ Named from what the player sees: the large clearing with the
         ///< river, its logs and the Nut floating over the water (its render)
  MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST = 8,
  MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ = 9,
};
/// SceneId_MogallForestSign's exits.
enum {
  MOGALL_FOREST_SIGN_TO_WORLD_MAP = 1,
  MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_WEST = 2,
  MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_NORTH = 3,
  MOGALL_FOREST_SIGN_ENTRANCE_TO_SOUTHWEST_CLEARING =
      4, ///< ★ Named from what the player sees: the clearing of only trees
         ///< south of the entrance, on the way to the river (its render)
  MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_ENTRANCE =
      5, ///< ★ Named from what the player sees: the clearing entered from the
         ///< world map, with the broken sign and a hollow stump (its render)
  MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_RIVER =
      6, ///< ★ Named from what the player sees: the large clearing with the
         ///< river, its logs and the Nut floating over the water (its render)
  MOGALL_FOREST_SIGN_RIVER_TO_SOUTHWEST_CLEARING =
      7, ///< ★ Named from what the player sees: the clearing of only trees
         ///< south of the entrance, on the way to the river (its render)
  MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_EAST = 8,
  MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_QUARTZ = 9,
};

/// SceneId_MogallForestQuartz's entrances.
enum {
  MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIGN = 1,
  MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_FROM_STUMPS =
      2, ///< ★ Named from what the player sees: the clearing of only trees
         ///< entered from the forest's first area (its render)
  MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH = 3,
  MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHWEST_CLEARING =
      4, ///< ★ Named from what the player sees: the clearing with two great
         ///< hollow stumps (its render)
  MOGALL_FOREST_QUARTZ_STUMPS_FROM_POND =
      5, ///< ★ Named from what the player sees: the clearing with two great
         ///< hollow stumps (its render)
  MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHEAST_CLEARING =
      6, ///< ★ Named from what the player sees: the clearing with two great
         ///< hollow stumps (its render)
  MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_STUMPS =
      7, ///< ★ Named from what the player sees: the clearing of only trees east
         ///< of the stumps (its render)
  MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_RIVER =
      8, ///< ★ Named from what the player sees: the clearing of only trees east
         ///< of the stumps (its render)
  MOGALL_FOREST_QUARTZ_POND_FROM_STUMPS =
      9, ///< ★ Named from what the player sees: a pond round an island holding
         ///< a chest, reached on a log (its render)
  MOGALL_FOREST_QUARTZ_RIVER_FROM_NORTHEAST_CLEARING =
      10, ///< ★ Named from what the player sees: the large clearing with the
          ///< river, logs, a rock and Quartz (its render)
  MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_KILLER_APE = 11,
  MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH = 12,
};
/// SceneId_MogallForestQuartz's exits.
enum {
  MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIGN = 1,
  MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_TO_STUMPS =
      2, ///< ★ Named from what the player sees: the clearing with two great
         ///< hollow stumps (its render)
  MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_NORTH = 3,
  MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHWEST_CLEARING =
      4, ///< ★ Named from what the player sees: the clearing of only trees
         ///< entered from the forest's first area (its render)
  MOGALL_FOREST_QUARTZ_STUMPS_TO_POND =
      5, ///< ★ Named from what the player sees: a pond round an island holding
         ///< a chest, reached on a log (its render)
  MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHEAST_CLEARING =
      6, ///< ★ Named from what the player sees: the clearing of only trees east
         ///< of the stumps (its render)
  MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_STUMPS =
      7, ///< ★ Named from what the player sees: the clearing with two great
         ///< hollow stumps (its render)
  MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_RIVER =
      8, ///< ★ Named from what the player sees: the large clearing with the
         ///< river, logs, a rock and Quartz (its render)
  MOGALL_FOREST_QUARTZ_POND_TO_STUMPS =
      9, ///< ★ Named from what the player sees: the clearing with two great
         ///< hollow stumps (its render)
  MOGALL_FOREST_QUARTZ_RIVER_TO_NORTHEAST_CLEARING =
      10, ///< ★ Named from what the player sees: the clearing of only trees
          ///< east of the stumps (its render)
  MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_KILLER_APE = 11,
  MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH = 12,
};

/// SceneId_MogallForestKillerApe's entrances.
enum {
  MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_FROM_STUMPS =
      1, ///< ★ Named from what the player sees: the clearing of only trees
         ///< entered from the forest's second area (its render)
  MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_QUARTZ = 2,
  MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_WEST = 3,
  MOGALL_FOREST_KILLER_APE_STUMPS_FROM_CHEST =
      4, ///< ★ Named from what the player sees: the clearing with three great
         ///< hollow stumps (its render)
  MOGALL_FOREST_KILLER_APE_STUMPS_FROM_NORTHWEST_CLEARING =
      5, ///< ★ Named from what the player sees: the clearing with three great
         ///< hollow stumps (its render)
  MOGALL_FOREST_KILLER_APE_STUMPS_FROM_RIVER =
      6, ///< ★ Named from what the player sees: the clearing with three great
         ///< hollow stumps (its render)
  MOGALL_FOREST_KILLER_APE_CHEST_FROM_STUMPS =
      7, ///< ★ Named from what the player sees: the clearing whose chest lies
         ///< behind a stream, a log and a rock (its render)
  MOGALL_FOREST_KILLER_APE_RIVER_FROM_STUMPS =
      8, ///< ★ Named from what the player sees: the large clearing ringed by
         ///< the river, with logs to roll in and a rock (its render)
  MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_EAST = 9,
  MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH = 10,
  MOGALL_FOREST_KILLER_APE_RIVER_FROM_BRIDGES =
      11, ///< ★ Named from what the player sees: the large clearing ringed by
          ///< the river, with logs to roll in and a rock (its render)
  MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER =
      12, ///< ★ Named from what the player sees: the clearing whose river is
          ///< crossed by two log bridges, by the way out to the world map (its
          ///< render)
  MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP = 13,
  MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS =
      15, ///< MogallForest_KillerApeAttacks
};
/// SceneId_MogallForestKillerApe's exits.
enum {
  MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_TO_STUMPS =
      1, ///< ★ Named from what the player sees: the clearing with three great
         ///< hollow stumps (its render)
  MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_QUARTZ = 2,
  MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_WEST = 3,
  MOGALL_FOREST_KILLER_APE_STUMPS_TO_CHEST =
      4, ///< ★ Named from what the player sees: the clearing whose chest lies
         ///< behind a stream, a log and a rock (its render)
  MOGALL_FOREST_KILLER_APE_STUMPS_TO_NORTHWEST_CLEARING =
      5, ///< ★ Named from what the player sees: the clearing of only trees
         ///< entered from the forest's second area (its render)
  MOGALL_FOREST_KILLER_APE_STUMPS_TO_RIVER =
      6, ///< ★ Named from what the player sees: the large clearing ringed by
         ///< the river, with logs to roll in and a rock (its render)
  MOGALL_FOREST_KILLER_APE_CHEST_TO_STUMPS =
      7, ///< ★ Named from what the player sees: the clearing with three great
         ///< hollow stumps (its render)
  MOGALL_FOREST_KILLER_APE_RIVER_TO_STUMPS =
      8, ///< ★ Named from what the player sees: the clearing with three great
         ///< hollow stumps (its render)
  MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_EAST = 9,
  MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH = 10,
  MOGALL_FOREST_KILLER_APE_RIVER_TO_BRIDGES =
      11, ///< ★ Named from what the player sees: the clearing whose river is
          ///< crossed by two log bridges, by the way out to the world map (its
          ///< render)
  MOGALL_FOREST_KILLER_APE_BRIDGES_TO_RIVER =
      12, ///< ★ Named from what the player sees: the large clearing ringed by
          ///< the river, with logs to roll in and a rock (its render)
  MOGALL_FOREST_KILLER_APE_TO_WORLD_MAP = 13,
};

/// SceneId_MogallForestSidePaths's entrances.
enum {
  MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_NORTH = 1,
  MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_WEST = 2,
  MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_NORTH = 3,
  MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_SOUTH = 4,
  MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_WEST = 5,
  MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_EAST = 6,
  MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_SOUTH = 7,
  MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_EAST = 8,
  MOGALL_FOREST_SIDE_PATHS_NORTH_CLEARING_EAST = 9,
  MOGALL_FOREST_SIDE_PATHS_WEST_CLEARING_WEST = 10,
};
/// SceneId_MogallForestSidePaths's exits.
enum {
  MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_NORTH = 1,
  MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_WEST = 2,
  MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_NORTH = 3,
  MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_SOUTH = 4,
  MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_WEST = 5,
  MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_EAST = 6,
  MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_SOUTH = 7,
  MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_EAST = 8,
  MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_WEST =
      11, ///< ★ Named from what the player sees: the northernmost of the
          ///< side-path clearings (its render)
  MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_EAST =
      12, ///< ★ Named from what the player sees: the westernmost of the
          ///< side-path clearings (its render)
  MOGALL_FOREST_SIDE_PATHS_TO_CENTER_CLEARING =
      13, ///< ★ Named from what the player sees: the middle one of the
          ///< side-path clearings (its render)
  MOGALL_FOREST_SIDE_PATHS_TO_EAST_CLEARING =
      14, ///< ★ Named from what the player sees: the easternmost of the
          ///< side-path clearings (its render)
  MOGALL_FOREST_SIDE_PATHS_TO_SOUTHWEST_CLEARING =
      15, ///< ★ Named from what the player sees: the southwestern of the
          ///< side-path clearings (its render)
  MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_NORTHWEST =
      16, ///< ★ Named from what the player sees: the southern side-path
          ///< clearing, the one with two doors on its west side (its render)
  MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_SOUTHWEST =
      17, ///< ★ Named from what the player sees: the southern side-path
          ///< clearing, the one with two doors on its west side (its render)
  MOGALL_FOREST_SIDE_PATHS_TO_SOUTHEAST_CLEARING =
      18, ///< ★ Named from what the player sees: the southeastern of the
          ///< side-path clearings (its render)
  MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_EAST =
      19, ///< ★ Named from what the player sees: the northernmost of the
          ///< side-path clearings (its render)
  MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_WEST =
      20, ///< ★ Named from what the player sees: the westernmost of the
          ///< side-path clearings (its render)
};

/// SceneId_Xian's entrances.
enum {
  XIAN_FROM_HOUSE = 1,
  XIAN_FROM_WEAPON_AND_ARMOR_SHOP = 5,
  XIAN_FROM_HOUSE_NORTH = 6,
  XIAN_FROM_XIAN_INTERIORS_EAST = 7,
  XIAN_FROM_XIAN_INTERIORS_SOUTH = 8,
  XIAN_FROM_XIAN_INTERIORS_WEST = 9,
  XIAN_FROM_FUCHIN_TEMPLE_INTERIORS = 10,
  XIAN_FROM_SANCTUM_WEST = 11,
};
/// SceneId_Xian's exits.
enum {
  XIAN_TO_HOUSE = 1, ///< ★ Named from what the player sees: a house with beds,
                     ///< a table and a hearth (its render)
  XIAN_TO_HOUSE_NORTH = 2,
  XIAN_TO_XIAN_INTERIORS_EAST = 3,
  XIAN_TO_INN =
      4, ///< ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
  XIAN_TO_WEAPON_AND_ARMOR_SHOP =
      5, ///< ★ Named from what the player sees: the shop whose sellers stand
         ///< behind one long white counter (its render)
  XIAN_TO_SANCTUM = 6,
  XIAN_TO_FUCHIN_TEMPLE_INTERIORS = 7,
  XIAN_TO_WORLD_MAP = 8,
};

/// SceneId_XianInteriors's entrances.
enum {
  XIAN_INTERIORS_HOUSE =
      1, ///< the room behind Xian's house door; through XIAN_TO_HOUSE
  XIAN_INTERIORS_HOUSE_NORTH =
      2, ///< the room holds the hsus house; through XIAN_TO_HOUSE_NORTH
  XIAN_INTERIORS_FROM_XIAN_EAST = 3,
  XIAN_INTERIORS_INN_SOUTH = 4, ///< the room holds the inn; through XIAN_TO_INN
  XIAN_INTERIORS_INN_GUEST_ROOMS_FROM_INN =
      5, ///< ★ Named from what the player sees: the small rooms behind the
         ///< inn's front room, with a bed (its render)
  XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP =
      6, ///< ★ Named from what the player sees: the shop whose sellers stand
         ///< behind one long white counter (its render); through
         ///< XIAN_TO_WEAPON_AND_ARMOR_SHOP
  XIAN_INTERIORS_INN_FROM_INN_GUEST_ROOMS = 7, ///< the room holds the inn;
  XIAN_INTERIORS_FROM_FUCHIN_TEMPLE_INTERIORS =
      8, ///< the room behind FuchinTempleInteriors's house door; through
         ///< FUCHIN_TEMPLE_INTERIORS_TO_HOUSE
};
/// SceneId_XianInteriors's exits.
enum {
  XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_XIAN = 1,
  XIAN_INTERIORS_HOUSE_NORTH_TO_XIAN = 2,
  XIAN_INTERIORS_TO_XIAN_EAST = 3,
  XIAN_INTERIORS_TO_XIAN_SOUTH = 4,
  XIAN_INTERIORS_INN_GUEST_ROOMS_TO_INN = 5,
  XIAN_INTERIORS_TO_XIAN_WEST = 6,
  XIAN_INTERIORS_INN_TO_INN_GUEST_ROOMS =
      7, ///< ★ Named from what the player sees: the small rooms behind the
         ///< inn's front room, with a bed (its render)
  XIAN_INTERIORS_TO_FUCHIN_TEMPLE_INTERIORS = 8,
};

/// SceneId_AlpineCrossing's entrances.
enum {
  ALPINE_CROSSING_FROM_WORLD_MAP_EAST = 1,
  ALPINE_CROSSING_FROM_WORLD_MAP_NORTH = 2,
  ALPINE_CROSSING_FROM_WORLD_MAP_WEST = 3,
  ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST = 4,
  ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST = 5,
};
/// SceneId_AlpineCrossing's exits.
enum {
  ALPINE_CROSSING_TO_WORLD_MAP_EAST = 1,
  ALPINE_CROSSING_TO_WORLD_MAP_NORTH = 2,
  ALPINE_CROSSING_TO_WORLD_MAP_WEST = 3,
  ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST = 4,
  ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST = 5,
  ALPINE_CROSSING_TO_LAMA_TEMPLE = 6,
};

/// SceneId_Altin's entrances.
enum {
  ALTIN_FROM_WORLD_MAP = 1,
  ALTIN_FROM_ALTIN_INTERIORS_SOUTHWEST = 2,
  ALTIN_FROM_SANCTUM = 3,
  ALTIN_FROM_ALTIN_INTERIORS_NORTHWEST = 4,
  ALTIN_FROM_ALTIN_INTERIORS_NORTH = 5,
  ALTIN_FROM_ALTIN_INTERIORS_NORTHEAST = 6,
  ALTIN_FROM_ALTIN_INTERIORS_SOUTHEAST = 7,
  ALTIN_FROM_ALTIN_INTERIORS_EAST = 8,
  ALTIN_FROM_ALTIN_INTERIORS_WEST = 9,
  ALTIN_FROM_ALTIN_PEAK_FIRST_WATER_BEAST = 10,
  ALTIN_FROM_ALTIN_PEAK_SOUTH_ENTRANCE = 11,
  ALTIN_FROM_ALTIN_PEAK_JUNCTION = 12,
  ALTIN_FROM_ALTIN_PEAK_SPRITZ = 13,
};
/// SceneId_Altin's exits.
enum {
  ALTIN_TO_WORLD_MAP = 1,
  ALTIN_TO_ALTIN_INTERIORS_SOUTHWEST = 2,
  ALTIN_TO_SANCTUM = 3,
  ALTIN_TO_ALTIN_INTERIORS_NORTHWEST = 4,
  ALTIN_TO_ALTIN_INTERIORS_NORTH = 5,
  ALTIN_TO_ALTIN_INTERIORS_NORTHEAST = 6,
  ALTIN_TO_ALTIN_INTERIORS_SOUTHEAST = 7,
  ALTIN_TO_ALTIN_INTERIORS_EAST = 8,
  ALTIN_TO_ALTIN_INTERIORS_WEST = 9,
  ALTIN_TO_ALTIN_PEAK_FIRST_WATER_BEAST = 10,
  ALTIN_TO_ALTIN_PEAK_SOUTH_ENTRANCE = 11,
  ALTIN_TO_ALTIN_PEAK_JUNCTION = 12,
  ALTIN_TO_ALTIN_PEAK_SPRITZ = 13,
};

/// SceneId_AltinInteriors's entrances.
enum {
  ALTIN_INTERIORS_FROM_ALTIN_NORTHWEST = 1,
  ALTIN_INTERIORS_FROM_ALTIN_NORTH = 2,
  ALTIN_INTERIORS_FROM_ALTIN_NORTHEAST = 3,
  ALTIN_INTERIORS_FROM_ALTIN_WEST = 4,
  ALTIN_INTERIORS_FROM_ALTIN_SOUTHEAST = 5,
  ALTIN_INTERIORS_FROM_ALTIN_EAST = 6,
  ALTIN_INTERIORS_FROM_ALTIN_SOUTHWEST = 7,
  ALTIN_INTERIORS_INN_FROM_INN_UPSTAIRS =
      8, ///< ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
  ALTIN_INTERIORS_INN_UPSTAIRS_FROM_INN =
      9, ///< ★ Named from what the player sees: the inn's upper floor of guest
         ///< rooms, reached by the stairs beside its counter (its render)
};
/// SceneId_AltinInteriors's exits.
enum {
  ALTIN_INTERIORS_TO_ALTIN_NORTHWEST = 1,
  ALTIN_INTERIORS_TO_ALTIN_NORTH = 2,
  ALTIN_INTERIORS_TO_ALTIN_NORTHEAST = 3,
  ALTIN_INTERIORS_TO_ALTIN_WEST = 4,
  ALTIN_INTERIORS_TO_ALTIN_SOUTHEAST = 5,
  ALTIN_INTERIORS_TO_ALTIN_EAST = 6,
  ALTIN_INTERIORS_TO_ALTIN_SOUTHWEST = 7,
  ALTIN_INTERIORS_INN_TO_INN_UPSTAIRS =
      8, ///< ★ Named from what the player sees: the inn's upper floor of guest
         ///< rooms, reached by the stairs beside its counter (its render)
  ALTIN_INTERIORS_INN_UPSTAIRS_TO_INN =
      9, ///< ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
};

/// SceneId_AltinPeakFirstWaterBeast's entrances.
enum {
  ALTIN_PEAK_FIRST_WATER_BEAST_FROM_ALTIN = 1,
};
/// SceneId_AltinPeakFirstWaterBeast's exits.
enum {
  ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN = 1,
};

/// SceneId_AltinPeakRailTunnel's entrances.
enum {
  ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE = 1,
  ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SECOND_WATER_BEAST = 2,
};
/// SceneId_AltinPeakRailTunnel's exits.
enum {
  ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE = 1,
  ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST = 2,
};

/// SceneId_AltinPeakSecondWaterBeast's entrances.
enum {
  ALTIN_PEAK_SECOND_WATER_BEAST_FROM_ALTIN_PEAK_RAIL_TUNNEL = 1,
  ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_FROM_CHEST_ROOM =
      2, ///< ★ Named from what the player sees: the main cave, a lake ringed by
         ///< rails with an island where the water beast spits (its render)
  ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_FROM_LAKE =
      3, ///< ★ Named from what the player sees: the small chamber off the lake
         ///< that holds a chest and a boulder (its render)
};
/// SceneId_AltinPeakSecondWaterBeast's exits.
enum {
  ALTIN_PEAK_SECOND_WATER_BEAST_TO_ALTIN_PEAK_RAIL_TUNNEL = 1,
  ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_TO_CHEST_ROOM =
      2, ///< ★ Named from what the player sees: the small chamber off the lake
         ///< that holds a chest and a boulder (its render)
  ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE =
      3, ///< ★ Named from what the player sees: the main cave, a lake ringed by
         ///< rails with an island where the water beast spits (its render)
};

/// SceneId_AltinPeakSouthEntrance's entrances.
enum {
  ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN = 1,
  ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_FROM_FLOODED_CHAMBER =
      2, ///< ★ Named from what the player sees: the long rail tunnel that the
         ///< doorway from Altin opens into (its render)
  ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_RAIL_TUNNEL = 3,
  ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_FROM_TUNNEL =
      4, ///< ★ Named from what the player sees: the chamber off the tunnel that
         ///< is filled with water until the second drain (its render)
  ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_POOL = 5,
  ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST = 6,
  ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST = 7,
};
/// SceneId_AltinPeakSouthEntrance's exits.
enum {
  ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN = 1,
  ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER =
      2, ///< ★ Named from what the player sees: the chamber off the tunnel that
         ///< is filled with water until the second drain (its render)
  ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL =
      3, ///< ★ Named from what the player sees: the long rail tunnel that the
         ///< doorway from Altin opens into (its render)
  ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL =
      4, ///< ★ Named from what the player sees: the long rail tunnel that the
         ///< doorway from Altin opens into (its render)
  ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL =
      5, ///< ★ Named from what the player sees: the chamber off the tunnel that
         ///< is filled with water until the second drain (its render)
  ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST = 6,
  ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST = 7,
};

/// SceneId_AltinPeakThirdWaterBeast's entrances.
enum {
  ALTIN_PEAK_THIRD_WATER_BEAST_FROM_ALTIN_PEAK_POOL = 1,
};
/// SceneId_AltinPeakThirdWaterBeast's exits.
enum {
  ALTIN_PEAK_THIRD_WATER_BEAST_TO_ALTIN_PEAK_POOL = 1,
};

/// SceneId_AltinPeakPool's entrances.
enum {
  ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE = 1,
  ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST = 2,
  ALTIN_PEAK_POOL_SOUTHWEST_FROM_NORTH = 3,
  ALTIN_PEAK_POOL_SOUTHWEST_FROM_SOUTHEAST = 4,
  ALTIN_PEAK_POOL_SOUTHEAST_FROM_SOUTHWEST = 5,
  ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_THIRD_WATER_BEAST = 6,
};
/// SceneId_AltinPeakPool's exits.
enum {
  ALTIN_PEAK_POOL_TO_ALTIN_PEAK_SOUTH_ENTRANCE = 1,
  ALTIN_PEAK_POOL_NORTH_TO_SOUTHWEST = 2,
  ALTIN_PEAK_POOL_SOUTHWEST_TO_NORTH = 3,
  ALTIN_PEAK_POOL_SOUTHWEST_TO_SOUTHEAST = 4,
  ALTIN_PEAK_POOL_SOUTHEAST_TO_SOUTHWEST = 5,
  ALTIN_PEAK_POOL_TO_ALTIN_PEAK_THIRD_WATER_BEAST = 6,
};

/// SceneId_AltinPeakSpritz's entrances.
enum {
  ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_JUNCTION = 1,
  ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_FALLING_ROCKS = 2,
  ALTIN_PEAK_SPRITZ_FROM_ALTIN = 3,
};
/// SceneId_AltinPeakSpritz's exits.
enum {
  ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_JUNCTION = 1,
  ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_FALLING_ROCKS = 2,
  ALTIN_PEAK_SPRITZ_TO_ALTIN = 3,
};

/// SceneId_AltinPeakJunction's entrances.
enum {
  ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_SPRITZ = 1,
  ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_NORTH = 2,
  ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_WEST_EXIT = 3,
  ALTIN_PEAK_JUNCTION_FROM_ALTIN = 4,
  ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_SOUTH = 5,
};
/// SceneId_AltinPeakJunction's exits.
enum {
  ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_SPRITZ = 1,
  ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_NORTH = 2,
  ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_WEST_EXIT = 3,
  ALTIN_PEAK_JUNCTION_TO_ALTIN = 4,
  ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_SOUTH = 5,
};

/// SceneId_AltinPeakFallingRocks's entrances.
enum {
  ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_SPRITZ = 1,
  ALTIN_PEAK_FALLING_ROCKS_WEST_FROM_EAST = 2,
  ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_SOUTH = 3,
  ALTIN_PEAK_FALLING_ROCKS_EAST_FROM_WEST = 4,
  ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_NORTH = 5,
  ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE = 6,
};
/// SceneId_AltinPeakFallingRocks's exits.
enum {
  ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_SPRITZ = 1,
  ALTIN_PEAK_FALLING_ROCKS_WEST_TO_EAST = 2,
  ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_SOUTH = 3,
  ALTIN_PEAK_FALLING_ROCKS_EAST_TO_WEST = 4,
  ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_NORTH = 5,
  ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_HYDROS_STATUE = 6,
};

/// SceneId_AltinPeakHydrosStatue's entrances.
enum {
  ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS = 1,
};
/// SceneId_AltinPeakHydrosStatue's exits.
enum {
  ALTIN_PEAK_HYDROS_STATUE_TO_ALTIN_PEAK_FALLING_ROCKS = 1,
};

/// SceneId_AltinPeakWestExit's entrances.
enum {
  ALTIN_PEAK_WEST_EXIT_FROM_ALTIN_PEAK_JUNCTION = 1,
  ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM =
      2, ///< ★ Named from what the player sees: the winding tunnel with a stone
         ///< pillar and a boulder (its render)
  ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_FROM_TUNNEL =
      3, ///< ★ Named from what the player sees: the small chamber off the
         ///< tunnel that holds a chest (its render)
  ALTIN_PEAK_WEST_EXIT_FROM_WORLD_MAP = 4,
};
/// SceneId_AltinPeakWestExit's exits.
enum {
  ALTIN_PEAK_WEST_EXIT_TO_ALTIN_PEAK_JUNCTION = 1,
  ALTIN_PEAK_WEST_EXIT_TUNNEL_TO_CHEST_ROOM =
      2, ///< ★ Named from what the player sees: the small chamber off the
         ///< tunnel that holds a chest (its render)
  ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_TO_TUNNEL =
      3, ///< ★ Named from what the player sees: the winding tunnel with a stone
         ///< pillar and a boulder (its render)
  ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP = 4,
};

/// SceneId_LamaTemple's entrances.
enum {
  LAMA_TEMPLE_FROM_WORLD_MAP = 1,
  LAMA_TEMPLE_FROM_FUCHIN_TEMPLE_INTERIORS = 2,
  LAMA_TEMPLE_FROM_ALPINE_CROSSING = 3,
};
/// SceneId_LamaTemple's exits.
enum {
  LAMA_TEMPLE_TO_WORLD_MAP = 1,
  LAMA_TEMPLE_TO_FUCHIN_TEMPLE_INTERIORS = 2,
};

/// SceneId_LamakanDesertEast's entrances.
enum {
  LAMAKAN_DESERT_EAST_FROM_WORLD_MAP = 1,
  LAMAKAN_DESERT_EAST_FROM_LAMAKAN_DESERT_SMOG = 2,
};
/// SceneId_LamakanDesertEast's exits.
enum {
  LAMAKAN_DESERT_EAST_TO_WORLD_MAP = 1,
  LAMAKAN_DESERT_EAST_TO_LAMAKAN_DESERT_SMOG = 2,
};

/// SceneId_LamakanDesertSmog's entrances.
enum {
  LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_EAST = 1,
  LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_MANTICORE = 2,
};
/// SceneId_LamakanDesertSmog's exits.
enum {
  LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_EAST = 1,
  LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_MANTICORE = 2,
};

/// SceneId_LamakanDesertManticore's entrances.
enum {
  LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_SMOG = 1,
  LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_EAST = 2,
  LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_SOUTH = 3,
  LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_WEST = 4,
  LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE = 5, ///< FightManticore
};
/// SceneId_LamakanDesertManticore's exits.
enum {
  LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_SMOG = 1,
  LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_EAST = 2,
  LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_SOUTH = 3,
  LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_WEST = 4,
};

/// SceneId_LamakanDesertCave's entrances.
enum {
  LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_SOUTH = 1,
  LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_EAST = 2,
  LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_WEST = 3,
  LAMAKAN_DESERT_CAVE_FROM_WORLD_MAP = 4,
};
/// SceneId_LamakanDesertCave's exits.
enum {
  LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_EAST = 1,
  LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_SOUTH = 2,
  LAMAKAN_DESERT_CAVE_TO_WORLD_MAP = 3,
  LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_WEST = 4,
};

/// SceneId_ValeCaveMouth's entrances.
enum {
  VALE_CAVE_MOUTH_FROM_VALE_KRADEN_WEST = 1,
  VALE_CAVE_MOUTH_FROM_VALE_CAVE_KITE = 2,
  VALE_CAVE_MOUTH_FROM_VALE_KRADEN_EAST = 3,
};
/// SceneId_ValeCaveMouth's exits.
enum {
  VALE_CAVE_MOUTH_TO_VALE_KRADEN_WEST = 1,
  VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE = 2,
  VALE_CAVE_MOUTH_TO_VALE_KRADEN_EAST = 3,
};

/// SceneId_ValeCaveKite's entrances.
enum {
  VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH = 1,
  VALE_CAVE_KITE_WEST_FROM_EAST = 2,
  VALE_CAVE_KITE_WEST_FROM_SOUTH = 3,
  VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST = 4,
  VALE_CAVE_KITE_EAST_FROM_WEST = 5,
  VALE_CAVE_KITE_EAST_FROM_SOUTH = 6,
  VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST = 7,
  VALE_CAVE_KITE_SOUTH_FROM_WEST = 8,
  VALE_CAVE_KITE_SOUTH_FROM_EAST = 9,
  VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH = 10,
};
/// SceneId_ValeCaveKite's exits.
enum {
  VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH = 1,
  VALE_CAVE_KITE_WEST_TO_EAST = 2,
  VALE_CAVE_KITE_WEST_TO_SOUTH = 3,
  VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST = 4,
  VALE_CAVE_KITE_EAST_TO_WEST = 5,
  VALE_CAVE_KITE_EAST_TO_SOUTH = 6,
  VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST = 7,
  VALE_CAVE_KITE_SOUTH_TO_WEST = 8,
  VALE_CAVE_KITE_SOUTH_TO_EAST = 9,
  VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH = 10,
};

/// SceneId_ValeCaveLogs's entrances.
enum {
  VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST = 1,
  VALE_CAVE_LOGS_NORTH_FROM_SOUTH = 2,
  VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST = 3,
  VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_SOUTH = 4,
  VALE_CAVE_LOGS_SOUTH_FROM_NORTH = 5,
};
/// SceneId_ValeCaveLogs's exits.
enum {
  VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST = 1,
  VALE_CAVE_LOGS_NORTH_TO_SOUTH = 2,
  VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST = 3,
  VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH = 4,
  VALE_CAVE_LOGS_SOUTH_TO_NORTH = 5,
};

/// SceneId_VaultCavePassage's entrances.
enum {
  VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_MOUTH = 1,
  VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_NORTHWEST = 2,
  VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_WEST = 3,
  VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_EAST = 4,
  VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST =
      5, ///< ★ Named from what the player sees: the room of water where the
         ///< switch stands (its render)
  VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST = 6,
  VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST = 7,
  VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST =
      8, ///< ★ Named from what the player sees: the room of water where the
         ///< switch stands (its render)
  VAULT_CAVE_PASSAGE_CHEST_FROM_WATER_ROOM =
      9, ///< ★ Named from what the player sees: the room holding the cave's one
         ///< chest (its render)
  VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT = 10,
  VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST =
      11, ///< ★ Named from what the player sees: the ladder down from Vault's
          ///< hidden passage (its render)
  VAULT_CAVE_PASSAGE_CHEST_FROM_LADDER =
      12, ///< ★ Named from what the player sees: the room holding the cave's
          ///< one chest (its render)
  VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST =
      13, ///< ★ Named from what the player sees: the room of water where the
          ///< switch stands (its render)
};
/// SceneId_VaultCavePassage's exits.
enum {
  VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_MOUTH = 1,
  VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_NORTHWEST = 2,
  VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_WEST =
      3, ///< ★ Named from what the player sees: the room of water where the
         ///< switch stands (its render)
  VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_EAST =
      4, ///< ★ Named from what the player sees: the room of water where the
         ///< switch stands (its render)
  VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_WEST = 5,
  VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_EAST = 6,
  VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_SOUTHEAST = 7,
  VAULT_CAVE_PASSAGE_WATER_ROOM_TO_CHEST =
      8, ///< ★ Named from what the player sees: the room holding the cave's one
         ///< chest (its render)
  VAULT_CAVE_PASSAGE_CHEST_TO_WATER_ROOM =
      9, ///< ★ Named from what the player sees: the room of water where the
         ///< switch stands (its render)
  VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT = 10,
  VAULT_CAVE_PASSAGE_LADDER_TO_CHEST =
      11, ///< ★ Named from what the player sees: the room holding the cave's
          ///< one chest (its render)
  VAULT_CAVE_PASSAGE_CHEST_TO_LADDER =
      12, ///< ★ Named from what the player sees: the ladder down from Vault's
          ///< hidden passage (its render)
  VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_EAST = 13,
};

/// SceneId_VaultCaveVambrace's entrances.
enum {
  VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH = 1,
  VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH = 2,
  VAULT_CAVE_VAMBRACE_NORTH_FROM_FROST_ROOM = 3,
  VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST = 4,
  VAULT_CAVE_VAMBRACE_WEST_FROM_NORTH = 5,
  VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTHWEST = 6,
  VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_SOUTH = 7,
  VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_NORTH =
      8, ///< ★ Named from what the player sees: the room of six puddles that
         ///< Frost freezes, under falling objects (its render)
  VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_SOUTH =
      9, ///< ★ Named from what the player sees: the room of six puddles that
         ///< Frost freezes, under falling objects (its render)
  VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH = 10,
  VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM = 11,
  VAULT_CAVE_VAMBRACE_CHEST_FROM_FROST_ROOM =
      13, ///< ★ Named from what the player sees: the room behind the bush
          ///< holding the Vambrace chest (its render)
  VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_CHEST =
      14, ///< ★ Named from what the player sees: the room of six puddles that
          ///< Frost freezes, under falling objects (its render)
};
/// SceneId_VaultCaveVambrace's exits.
enum {
  VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_NORTH = 1,
  VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_NORTH = 2,
  VAULT_CAVE_VAMBRACE_NORTH_TO_FROST_ROOM =
      3, ///< ★ Named from what the player sees: the room of six puddles that
         ///< Frost freezes, under falling objects (its render)
  VAULT_CAVE_VAMBRACE_NORTH_TO_WEST = 4,
  VAULT_CAVE_VAMBRACE_WEST_TO_NORTH = 5,
  VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTHWEST = 6,
  VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_SOUTH = 7,
  VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_NORTH = 8,
  VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_SOUTH = 9,
  VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTH = 10,
  VAULT_CAVE_VAMBRACE_SOUTH_TO_FROST_ROOM =
      11, ///< ★ Named from what the player sees: the room of six puddles that
          ///< Frost freezes, under falling objects (its render)
  VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_CHEST =
      13, ///< ★ Named from what the player sees: the room behind the bush
          ///< holding the Vambrace chest (its render)
  VAULT_CAVE_VAMBRACE_CHEST_TO_FROST_ROOM =
      14, ///< ★ Named from what the player sees: the room of six puddles that
          ///< Frost freezes, under falling objects (its render)
};

/// SceneId_VaultCaveMouth's entrances.
enum {
  VAULT_CAVE_MOUTH_FROM_VAULT_REVISIT = 1,
  VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_PASSAGE = 2,
  VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_SOUTH = 3,
  VAULT_CAVE_MOUTH_SOUTH_FROM_NORTH = 4,
  VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH = 5,
  VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH = 6,
};
/// SceneId_VaultCaveMouth's exits.
enum {
  VAULT_CAVE_MOUTH_TO_VAULT_REVISIT = 1,
  VAULT_CAVE_MOUTH_TO_VAULT_CAVE_PASSAGE = 2,
  VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_SOUTH = 3,
  VAULT_CAVE_MOUTH_SOUTH_TO_NORTH = 4,
  VAULT_CAVE_MOUTH_NORTH_TO_SOUTH = 5,
  VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_NORTH = 6,
};

/// SceneId_Kalay's entrances.
enum {
  KALAY_FROM_HOUSE_NORTHWEST = 1,
  KALAY_FROM_HOUSE_NORTHEAST_UPPER = 2,
  KALAY_FROM_HOUSE_NORTHEAST_LOWER = 3,
  KALAY_FROM_HOUSE_SOUTHEAST = 4,
  KALAY_FROM_WEAPON_SHOP = 5,
  KALAY_FROM_ARMOR_SHOP = 6,
  KALAY_FROM_INN = 7,
  KALAY_FROM_KALAY_SANCTUM = 8,
  KALAY_FROM_WORLD_MAP = 9,
  KALAY_FROM_WORLD_MAP_WEST_UNUSED = 10, ///< no exit leads here
  KALAY_FROM_HAMMET_PALACE_GROUNDS = 11,
  KALAY_FROM_WEAPON_SHOP_COUNTER = 12,
  KALAY_FROM_ARMOR_SHOP_COUNTER = 13,
  KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM = 14,
};
/// SceneId_Kalay's exits.
enum {
  KALAY_TO_HOUSE_NORTHWEST = 1,
  KALAY_TO_HOUSE_NORTHEAST_UPPER =
      2, ///< ★ Named from what the player sees: the upper door of the
         ///< two-storey house in the town's north-east, on its terrace (its
         ///< render)
  KALAY_TO_HOUSE_NORTHEAST_LOWER =
      3, ///< ★ Named from what the player sees: the ground-floor door of the
         ///< two-storey house in the town's north-east (its render)
  KALAY_TO_HOUSE_SOUTHEAST = 4,
  KALAY_TO_WEAPON_SHOP =
      5, ///< ★ Named from what the player sees: the building with the sword
         ///< sign and the weapon seller's counter (its render)
  KALAY_TO_ARMOR_SHOP =
      6, ///< ★ Named from what the player sees: the building with the armor
         ///< seller's counter and shields on its inner walls (its render)
  KALAY_TO_INN = 7, ///< ★ Named from what the player sees: the building under
                    ///< the INN sign (its render)
  KALAY_TO_KALAY_SANCTUM = 8,
  KALAY_TO_WORLD_MAP = 9,
  KALAY_TO_WORLD_MAP_WEST = 10,
  KALAY_TO_HAMMET_PALACE_GROUNDS = 11,
  KALAY_TO_WEAPON_SHOP_COUNTER =
      12, ///< ★ Named from what the player sees: the arched doorway behind the
          ///< weapon seller's counter (its render)
  KALAY_TO_ARMOR_SHOP_COUNTER =
      13, ///< ★ Named from what the player sees: the arched doorway behind the
          ///< armor seller's counter (its render)
  KALAY_TO_KALAY_TUNNEL_TREASURE_ROOM = 14,
};

/// SceneId_KalayInteriors's entrances.
enum {
  KALAY_INTERIORS_HOUSE_NORTHWEST = 1,
  KALAY_INTERIORS_HOUSE_NORTHEAST_UPPER =
      2, ///< ★ Named from what the player sees: the upper floor of the
         ///< two-storey house in the town's north-east, entered from its
         ///< terrace (its render)
  KALAY_INTERIORS_HOUSE_NORTHEAST_LOWER =
      3, ///< ★ Named from what the player sees: the ground floor of the
         ///< two-storey house in the town's north-east (its render)
  KALAY_INTERIORS_HOUSE_SOUTHEAST = 4,
  KALAY_INTERIORS_WEAPON_SHOP_COUNTER =
      5, ///< ★ Named from what the player sees: behind the weapon seller's
         ///< counter, through the arched doorway at the back (its render)
  KALAY_INTERIORS_WEAPON_SHOP = 6,
  KALAY_INTERIORS_ARMOR_SHOP = 7,
  KALAY_INTERIORS_ARMOR_SHOP_COUNTER = 8,
  KALAY_INTERIORS_INN_FROM_INN_PASSAGE =
      9, ///< ★ Named from what the player sees: the inn's room with its
         ///< counter, dining tables and stairs (its render)
  KALAY_INTERIORS_INN = 10,
  KALAY_INTERIORS_INN_PASSAGE_FROM_INN =
      11, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST =
      12, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST =
      13, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST =
      14, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST =
      15, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  KALAY_INTERIORS_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE =
      16, ///< ★ Named from what the player sees: the guest room through the
          ///< east opening on the inn corridor's south side (its render)
  KALAY_INTERIORS_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE =
      17, ///< ★ Named from what the player sees: the guest room behind the west
          ///< door in the inn corridor's north wall (its render)
  KALAY_INTERIORS_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE =
      18, ///< ★ Named from what the player sees: the guest room behind the east
          ///< door in the inn corridor's north wall (its render)
  KALAY_INTERIORS_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE =
      19, ///< ★ Named from what the player sees: the guest room through the
          ///< west opening on the inn corridor's south side (its render)
};
/// SceneId_KalayInteriors's exits.
enum {
  KALAY_INTERIORS_HOUSE_NORTHWEST_TO_KALAY = 1,
  KALAY_INTERIORS_HOUSE_NORTHEAST_UPPER_TO_KALAY = 2,
  KALAY_INTERIORS_HOUSE_NORTHEAST_LOWER_TO_KALAY = 3,
  KALAY_INTERIORS_HOUSE_SOUTHEAST_TO_KALAY = 4,
  KALAY_INTERIORS_WEAPON_SHOP_COUNTER_TO_KALAY = 5,
  KALAY_INTERIORS_WEAPON_SHOP_TO_KALAY = 6,
  KALAY_INTERIORS_ARMOR_SHOP_TO_KALAY = 7,
  KALAY_INTERIORS_ARMOR_SHOP_COUNTER_TO_KALAY = 8,
  KALAY_INTERIORS_INN_TO_INN_PASSAGE =
      9, ///< ★ Named from what the player sees: the corridor at the foot of the
         ///< inn's stairs, lined with guest-room doors (its render)
  KALAY_INTERIORS_INN_TO_KALAY = 10,
  KALAY_INTERIORS_INN_PASSAGE_TO_INN =
      11, ///< ★ Named from what the player sees: the inn's room with its
          ///< counter, dining tables and stairs (its render)
  KALAY_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST =
      12, ///< ★ Named from what the player sees: the guest room behind the west
          ///< door in the inn corridor's north wall (its render)
  KALAY_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST =
      13, ///< ★ Named from what the player sees: the guest room behind the east
          ///< door in the inn corridor's north wall (its render)
  KALAY_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST =
      14, ///< ★ Named from what the player sees: the guest room through the
          ///< west opening on the inn corridor's south side (its render)
  KALAY_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST =
      15, ///< ★ Named from what the player sees: the guest room through the
          ///< east opening on the inn corridor's south side (its render)
  KALAY_INTERIORS_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE =
      16, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  KALAY_INTERIORS_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE =
      17, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  KALAY_INTERIORS_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE =
      18, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  KALAY_INTERIORS_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE =
      19, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
};

/// SceneId_KalaySanctum's entrances.
enum {
  KALAY_SANCTUM_FROM_KALAY = 1,
};
/// SceneId_KalaySanctum's exits.
enum {
  KALAY_SANCTUM_TO_KALAY = 1,
};

/// SceneId_HammetPalaceGrounds's entrances.
enum {
  HAMMET_PALACE_GROUNDS_FROM_KALAY = 1,
  HAMMET_PALACE_GROUNDS_FROM_HAMMET_PALACE = 2,
};
/// SceneId_HammetPalaceGrounds's exits.
enum {
  HAMMET_PALACE_GROUNDS_TO_KALAY = 1,
  HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE = 2,
  HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE_LAYANAS_ROOM = 5,
};

/// SceneId_HammetPalace's entrances.
enum {
  HAMMET_PALACE_FROM_HAMMET_PALACE_GROUNDS = 1,
  HAMMET_PALACE_ENTRANCE_ROOM_FROM_WEST_WING =
      2, ///< ★ Named from what the player sees: the hall inside the front door,
         ///< with a red carpet and two fountains (its render)
  HAMMET_PALACE_ENTRANCE_ROOM_FROM_EAST_WING =
      3, ///< ★ Named from what the player sees: the hall inside the front door,
         ///< with a red carpet and two fountains (its render)
  HAMMET_PALACE_ENTRANCE_ROOM_FROM_LAYANAS_ROOM =
      4, ///< ★ Named from what the player sees: the hall inside the front door,
         ///< with a red carpet and two fountains (its render)
  HAMMET_PALACE_ENTRANCE_ROOM_FROM_LANDING =
      5, ///< ★ Named from what the player sees: the hall inside the front door,
         ///< with a red carpet and two fountains (its render)
  HAMMET_PALACE_WEST_WING_FROM_ENTRANCE_ROOM =
      6, ///< ★ Named from what the player sees: the sitting room and bedroom
         ///< beyond the hall's west door (its render)
  HAMMET_PALACE_EAST_WING_FROM_ENTRANCE_ROOM =
      7, ///< ★ Named from what the player sees: the bedroom, kitchen and stairs
         ///< beyond the hall's east door (its render)
  HAMMET_PALACE_EAST_WING_FROM_PASSAGE_DOOR =
      8, ///< ★ Named from what the player sees: the bedroom, kitchen and stairs
         ///< beyond the hall's east door (its render)
  HAMMET_PALACE_LAYANAS_ROOM_FROM_ENTRANCE_ROOM =
      9, ///< ★ Named from what the player sees: the room with two thrones on a
         ///< red dais where Layana sits (its render)
  HAMMET_PALACE_PASSAGE_DOOR_FROM_EAST_WING =
      10, ///< ★ Named from the game: "underground passage"
          ///< (MsgHammetPalaceILeftTheDoorToThe)
  HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM = 11,
  HAMMET_PALACE_BEDROOM_FROM_LANDING =
      12, ///< ★ Named from what the player sees: the room of two beds and a
          ///< round table off the landing (its render)
  HAMMET_PALACE_LANDING_FROM_ENTRANCE_ROOM =
      13, ///< ★ Named from what the player sees: the small room at the other
          ///< end of the hall's stairs, with one door (its render)
  HAMMET_PALACE_LANDING_FROM_BEDROOM =
      14, ///< ★ Named from what the player sees: the small room at the other
          ///< end of the hall's stairs, with one door (its render)
  HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS =
      20, ///< ★ Named from what the player sees: the room with two thrones on a
          ///< red dais where Layana sits (its render)
};
/// SceneId_HammetPalace's exits.
enum {
  HAMMET_PALACE_TO_HAMMET_PALACE_GROUNDS = 1,
  HAMMET_PALACE_ENTRANCE_ROOM_TO_WEST_WING =
      2, ///< ★ Named from what the player sees: the sitting room and bedroom
         ///< beyond the hall's west door (its render)
  HAMMET_PALACE_ENTRANCE_ROOM_TO_EAST_WING =
      3, ///< ★ Named from what the player sees: the bedroom, kitchen and stairs
         ///< beyond the hall's east door (its render)
  HAMMET_PALACE_ENTRANCE_ROOM_TO_LAYANAS_ROOM =
      4, ///< ★ Named from what the player sees: the room with two thrones on a
         ///< red dais where Layana sits (its render)
  HAMMET_PALACE_ENTRANCE_ROOM_TO_LANDING =
      5, ///< ★ Named from what the player sees: the small room at the other end
         ///< of the hall's stairs, with one door (its render)
  HAMMET_PALACE_WEST_WING_TO_ENTRANCE_ROOM =
      6, ///< ★ Named from what the player sees: the hall inside the front door,
         ///< with a red carpet and two fountains (its render)
  HAMMET_PALACE_EAST_WING_TO_ENTRANCE_ROOM =
      7, ///< ★ Named from what the player sees: the hall inside the front door,
         ///< with a red carpet and two fountains (its render)
  HAMMET_PALACE_EAST_WING_TO_PASSAGE_DOOR =
      8, ///< ★ Named from the game: "underground passage"
         ///< (MsgHammetPalaceILeftTheDoorToThe)
  HAMMET_PALACE_LAYANAS_ROOM_TO_ENTRANCE_ROOM =
      9, ///< ★ Named from what the player sees: the hall inside the front door,
         ///< with a red carpet and two fountains (its render)
  HAMMET_PALACE_PASSAGE_DOOR_TO_EAST_WING =
      10, ///< ★ Named from what the player sees: the bedroom, kitchen and
          ///< stairs beyond the hall's east door (its render)
  HAMMET_PALACE_TO_KALAY_TUNNEL_TREASURE_ROOM = 11,
  HAMMET_PALACE_BEDROOM_TO_LANDING =
      12, ///< ★ Named from what the player sees: the small room at the other
          ///< end of the hall's stairs, with one door (its render)
  HAMMET_PALACE_LANDING_TO_ENTRANCE_ROOM =
      13, ///< ★ Named from what the player sees: the hall inside the front
          ///< door, with a red carpet and two fountains (its render)
  HAMMET_PALACE_LANDING_TO_BEDROOM =
      14, ///< ★ Named from what the player sees: the room of two beds and a
          ///< round table off the landing (its render)
};

/// SceneId_Lunpa's entrances.
enum {
  LUNPA_FROM_WORLD_MAP = 1,
  LUNPA_FROM_LUNPA_FORTRESS_GATES = 2,
  LUNPA_FROM_WEST_LUNPA_CAVE = 3,
  LUNPA_FROM_FIRST_HOME = 4,
  LUNPA_FROM_SECOND_HOME = 5,
  LUNPA_FROM_THIRD_HOME = 6,
  LUNPA_FROM_WEAPON_AND_ARMOR_SHOP = 7,
  LUNPA_FROM_ITEM_SHOP = 8,
  LUNPA_FROM_INN = 9,
  LUNPA_FROM_TEMPLE = 10,
  LUNPA_FROM_CRATE_ROOM = 11,
  LUNPA_FROM_VAULT_JAIL = 12,
};
/// SceneId_Lunpa's exits.
enum {
  LUNPA_TO_WORLD_MAP = 1,
  LUNPA_TO_LUNPA_FORTRESS_GATES = 2,
  LUNPA_TO_WEST_LUNPA_CAVE = 3,
  LUNPA_TO_FIRST_HOME =
      4, ///< ★ Named from what the player sees: the first of the town's three
         ///< homes, each with beds and a hearth (its render)
  LUNPA_TO_SECOND_HOME =
      5, ///< ★ Named from what the player sees: the second of the town's three
         ///< homes, each with beds and a hearth (its render)
  LUNPA_TO_THIRD_HOME =
      6, ///< ★ Named from what the player sees: the third of the town's three
         ///< homes, each with beds and a hearth (its render)
  LUNPA_TO_WEAPON_AND_ARMOR_SHOP =
      7, ///< ★ Named from what the player sees: the shop with two counters, a
         ///< weapon seller and an armor seller (its render)
  LUNPA_TO_ITEM_SHOP =
      8, ///< ★ Named from what the player sees: the shop stacked with goods
         ///< whose keeper sells items (its render)
  LUNPA_TO_INN =
      9, ///< ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
  LUNPA_TO_TEMPLE = 10, ///< ★ Named from its area name: Lunpa Temple
  LUNPA_TO_CRATE_ROOM =
      11, ///< ★ Named from what the player sees: the room piled with crates,
          ///< barrels and sacks (its render)
  LUNPA_TO_VAULT_JAIL = 12,
};

/// SceneId_LunpaSuhalla's entrances and exits.
enum {
  LUNPA_SUHALLA_FROM_SUHALLA_GATE_CHECKPOINT = 10,
};

/// SceneId_WestLunpaCave's entrances.
enum {
  WEST_LUNPA_CAVE_FROM_WORLD_MAP = 1,
  WEST_LUNPA_CAVE_FROM_LUNPA = 2,
  /// WEST_LUNPA_CAVE_FROM_LUNPA again with a shorter camera; no exit leads
  /// here.
  WEST_LUNPA_CAVE_FROM_LUNPA_UNUSED = 3,
};
/// SceneId_WestLunpaCave's exits.
enum {
  WEST_LUNPA_CAVE_TO_WORLD_MAP = 1,
  WEST_LUNPA_CAVE_TO_LUNPA = 2,
  WEST_LUNPA_CAVE_BY_WAGON =
      3, ///< ★ Named from the game: "Then let's take that wagon to Kalay!"
         ///< (MsgLunpaLetsTakeWagon)
};

/// SceneId_KalayDocks's entrances.
enum {
  KALAY_DOCKS_FROM_WORLD_MAP_SOUTH = 1,
  KALAY_DOCKS_FROM_TOLBI_SHIP_DECK = 2,
  KALAY_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS = 3,
  KALAY_DOCKS_FROM_WORLD_MAP_NORTH = 4,
};
/// SceneId_KalayDocks's exits.
enum {
  KALAY_DOCKS_TO_WORLD_MAP_SOUTH = 1,
  KALAY_DOCKS_TO_TOLBI_SHIP_DECK = 2,
  KALAY_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS = 3,
  KALAY_DOCKS_TO_WORLD_MAP_NORTH = 4,
};

/// SceneId_KalayTolbiDocksInteriors's entrances.
enum {
  KALAY_TOLBI_DOCKS_INTERIORS_FROM_KALAY_DOCKS = 1,
  KALAY_TOLBI_DOCKS_INTERIORS_FROM_TOLBI_DOCKS = 2,
};
/// SceneId_KalayTolbiDocksInteriors's exits.
enum {
  KALAY_TOLBI_DOCKS_INTERIORS_TO_KALAY_DOCKS = 1,
  KALAY_TOLBI_DOCKS_INTERIORS_TO_TOLBI_DOCKS = 2,
};

/// SceneId_TolbiShipDeck's entrances.
enum {
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD = 1,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_CRATE_HOLD = 2,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM = 3,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST = 4,
  TOLBI_SHIP_DECK_FROM_KALAY_DOCKS = 5,
  TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS = 6,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY = 10,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_SEA_MONSTERS = 11,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD = 12,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK = 13,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK = 14,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_KRAKEN = 15,
  TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE =
      16, ///< TolbiShipDeck_JumpIntoKrakenBattle
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED =
      17, ///< ★ Named from the game: "I can see land!" (MsgTolbiShipCanSeeLand)
  TOLBI_SHIP_DECK_FROM_WORLD_MAP = 18,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE = 19,
};
/// SceneId_TolbiShipDeck's exits.
enum {
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD = 1,
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CRATE_HOLD = 2,
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM = 3,
  TOLBI_SHIP_DECK_TO_KALAY_DOCKS = 4,
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST = 5,
  TOLBI_SHIP_DECK_TO_TOLBI_DOCKS = 6,
  TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE =
      7, ///< ★ Named from its area name: Crossbone Isle
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST_COURSE_CLEAR =
      10, ///< ★ Named from the game: "Ship's course... Clear!"
          ///< (MsgTolbiShipShipsCourseClear)
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_TURN = 11,
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_FIRST_ATTACK = 12,
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_SECOND_ATTACK = 13,
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_THIRD_ATTACK = 14,
  TOLBI_SHIP_DECK_TO_WORLD_MAP_REACH_TOLBI =
      15, ///< ★ Named from what the player sees: the ship coming in to Tolbi at
          ///< the end of the crossing (its render)
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_ISLAND = 16,
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_SWITCH_TEAMS = 17,
};

/// SceneId_TolbiShipMast's entrances.
enum {
  TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK = 1,
  TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR = 10,
  TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK = 11,
  TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK = 12,
  TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK = 13,
  TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO =
      14, ///< ★ Named from the game: "Land ho!" (MsgTolbiShipLandHo)
};
/// SceneId_TolbiShipMast's exits.
enum {
  TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK = 1,
  TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_CAST_OFF = 10,
  TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_MONSTERS = 11,
  TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_SECOND_ATTACK = 12,
  TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_THIRD_ATTACK = 13,
  TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED =
      14, ///< ★ Named from the game: "I can see land!" (MsgTolbiShipCanSeeLand)
  TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE = 15,
};

/// SceneId_TolbiShipInteriors's entrances.
enum {
  TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK =
      1, ///< ★ Named from what the player sees: the room below deck where the
         ///< tour's passengers sit round a green rug (its render)
  TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_ROWING_DECK =
      2, ///< ★ Named from what the player sees: the room below deck where the
         ///< tour's passengers sit round a green rug (its render)
  TOLBI_SHIP_INTERIORS_CRATE_HOLD_FROM_TOLBI_SHIP_DECK =
      3, ///< ★ Named from what the player sees: the room below deck stacked
         ///< with crates, barrels and fruit baskets (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD =
      4, ///< ★ Named from what the player sees: the oarsmen rowing in rows to a
         ///< drummer's beat (its render)
  TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK =
      5, ///< ★ Named from what the player sees: the room where the captain
         ///< stands by the ship's wheel (its render)
  TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF =
      10, ///< ★ Named from what the player sees: the room where the captain
          ///< stands by the ship's wheel (its render)
  TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS = 11,
  TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING =
      12, ///< ★ Named from what the player sees: the oarsmen rowing in rows to
          ///< a drummer's beat (its render)
  TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS =
      13, ///< ★ Named from what the player sees: the room where the captain
          ///< stands by the ship's wheel (its render)
  TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN =
      14, ///< ★ Named from what the player sees: the room where the captain
          ///< stands by the ship's wheel (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK =
      15, ///< ★ Named from what the player sees: the oarsmen rowing in rows to
          ///< a drummer's beat (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE =
      16, ///< TolbiShipInteriors_FightRowingDeckMonsters
  TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK =
      17, ///< ★ Named from what the player sees: the oarsmen rowing in rows to
          ///< a drummer's beat (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE =
      18, ///< TolbiShipInteriors_FightRowingDeckMonsters
  TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK =
      19, ///< ★ Named from what the player sees: the oarsmen rowing in rows to
          ///< a drummer's beat (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE =
      20, ///< TolbiShipInteriors_FightRowingDeckMonsters
  TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP = 21,
  TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND =
      22, ///< ★ Named from what the player sees: the room where the captain
          ///< stands by the ship's wheel (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND = 23,
  TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS =
      24, ///< ★ Named from what the player sees: the oarsmen rowing in rows to
          ///< a drummer's beat (its render)
  /// Where a defeat in the monster attacks or the Kraken battle returns the
  /// party (gGameState.savedEntrance): the voyage's flags are cleared and the
  /// party goes up on deck.
  TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT = 30,
};
/// SceneId_TolbiShipInteriors's exits.
enum {
  TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK = 1,
  TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_ROWING_DECK =
      2, ///< ★ Named from what the player sees: the oarsmen rowing in rows to a
         ///< drummer's beat (its render)
  TOLBI_SHIP_INTERIORS_CRATE_HOLD_TO_TOLBI_SHIP_DECK = 3,
  TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_PASSENGER_HOLD =
      4, ///< ★ Named from what the player sees: the room below deck where the
         ///< tour's passengers sit round a green rug (its render)
  TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK = 5,
  TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY =
      10, ///< ★ Named from the game: "Preparations... Ready!"
          ///< (MsgTolbiShipPreparationsReady)
  TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS =
      11, ///< ★ Named from what the player sees: the room below deck where the
          ///< tour's passengers sit round a green rug (its render)
  TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING =
      12, ///< ★ Named from what the player sees: the oarsmen rowing in rows to
          ///< a drummer's beat (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_FIRST_ATTACK =
      13, ///< ★ Named from what the player sees: the ship sailing across the
          ///< world map until the first monster attack (its render)
  TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_SEA_MONSTERS =
      14, ///< ★ Named from the game: "Sea monsters!"
          ///< (MsgTolbiShipAyeCaptainSeaMonsters)
  TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_MONSTERS_BOARD =
      15, ///< ★ Named from the game: "It's no use. It's too late!"
          ///< (MsgTolbiShipNoUseLate)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_SECOND_ATTACK =
      16, ///< ★ Named from what the player sees: the ship sailing across the
          ///< world map until the second monster attack (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_THIRD_ATTACK =
      17, ///< ★ Named from what the player sees: the ship sailing across the
          ///< world map until the third monster attack (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_KRAKEN =
      18, ///< ★ Named from what the player sees: the ship sailing across the
          ///< world map until the Kraken comes aboard (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_TOLBI_SHIP_DECK_KRAKEN =
      19, ///< ★ Named from what the player sees: the stairs up to the deck
          ///< while the Kraken is aboard, where the party jumps into the battle
          ///< (its render)
  TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_LAND =
      20, ///< ★ Named from what the player sees: the ship sailing across the
          ///< world map until land is sighted (its render)
  TOLBI_SHIP_INTERIORS_TO_WORLD_MAP = 21,
};

/// SceneId_TolbiDocks's entrances.
enum {
  TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK = 1,
  TOLBI_DOCKS_FROM_WORLD_MAP = 2,
  TOLBI_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS = 3,
};
/// SceneId_TolbiDocks's exits.
enum {
  TOLBI_DOCKS_TO_WORLD_MAP = 1,
  TOLBI_DOCKS_TO_TOLBI_SHIP_DECK = 2,
  TOLBI_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS = 3,
};

/// SceneId_CrossboneIsle's entrances.
enum {
  CROSSBONE_ISLE_FROM_WORLD_MAP = 1,
  CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE = 2,
};
/// SceneId_CrossboneIsle's exits.
enum {
  CROSSBONE_ISLE_TO_WORLD_MAP = 1,
  CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE = 2,
};

/// SceneId_CrossboneIsleCaveEntrance's entrances.
enum {
  CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE = 1,
  CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE_FIRST_FLOOR = 2,
};
/// SceneId_CrossboneIsleCaveEntrance's exits.
enum {
  CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE = 1,
  CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR = 2,
};

/// SceneId_CrossboneIsleSteppingStones's entrances.
enum {
  CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST = 1,
  CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_WEST = 2,
};
/// SceneId_CrossboneIsleSteppingStones's exits.
enum {
  CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_EAST = 1,
  CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_WEST = 2,
};

/// SceneId_CrossboneIsleIcePillars's entrances.
enum {
  CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_EAST = 1,
  CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_WEST = 2,
};
/// SceneId_CrossboneIsleIcePillars's exits.
enum {
  CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_EAST = 1,
  CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_WEST = 2,
};

/// SceneId_CrossboneIsleKeys's entrances.
enum {
  CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_EAST = 1,
  CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_WEST = 2,
};
/// SceneId_CrossboneIsleKeys's exits.
enum {
  CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_EAST = 1,
  CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_WEST = 2,
};

/// SceneId_CrossboneIsleStatues's entrances.
enum {
  CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_WEST = 2,
};
/// SceneId_CrossboneIsleStatues's exits.
enum {
  CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_WEST = 2,
};

/// SceneId_CrossboneIsleFloodgate's entrances.
enum {
  CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_WEST = 2,
};
/// SceneId_CrossboneIsleFloodgate's exits.
enum {
  CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_WEST = 2,
};

/// SceneId_CrossboneIsleBane's entrances.
enum {
  CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_WEST = 2,
};
/// SceneId_CrossboneIsleBane's exits.
enum {
  CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_WEST = 2,
};

/// SceneId_CrossboneIsleStackedPillars's entrances.
enum {
  CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST = 2,
};
/// SceneId_CrossboneIsleStackedPillars's exits.
enum {
  CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST = 2,
};

/// SceneId_CrossboneIsleDragonStatue's entrances.
enum {
  CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST = 2,
};
/// SceneId_CrossboneIsleDragonStatue's exits.
enum {
  CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST = 2,
};

/// SceneId_CrossboneIsleFloatingLogs's entrances.
enum {
  CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_WEST = 2,
  /// The middle of the room, facing south. No exit leads to it: only the debug
  /// scene selector, which warps to any entrance from 0 to 99, reaches it.
  CROSSBONE_ISLE_FLOATING_LOGS_MIDDLE = 99,
};
/// SceneId_CrossboneIsleFloatingLogs's exits.
enum {
  CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST = 1,
  CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST = 2,
};

/// SceneId_CrossboneIslePier's entrances.
enum {
  CROSSBONE_ISLE_PIER_FROM_CROSSBONE_ISLE_NINTH_FLOOR = 1,
  CROSSBONE_ISLE_PIER_FROM_PIRATE_SHIP = 2,
};
/// SceneId_CrossboneIslePier's exits.
enum {
  CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR = 1,
  CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP = 2,
};

/// SceneId_PirateShip's entrances.
enum {
  PIRATE_SHIP_FROM_CROSSBONE_ISLE_PIER = 1,
};
/// SceneId_PirateShip's exits.
enum {
  PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER = 1,
};

/// SceneId_CrossboneIsleFirstFloor's entrances.
enum {
  CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE = 1,
  CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_SECOND_FLOOR = 2,
  CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_EAST = 3,
  CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_WEST = 4,
};
/// SceneId_CrossboneIsleFirstFloor's exits.
enum {
  CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_CAVE_ENTRANCE = 1,
  CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR = 2,
  CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_STEPPING_STONES_EAST = 3,
  CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_STEPPING_STONES_WEST = 4,
};

/// SceneId_CrossboneIsleSecondFloor's entrances.
enum {
  CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_FIRST_FLOOR = 1,
  CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_THIRD_FLOOR = 2,
  CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_ICE_PILLARS_EAST = 3,
  CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_ICE_PILLARS_WEST = 4,
};
/// SceneId_CrossboneIsleSecondFloor's exits.
enum {
  CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_FIRST_FLOOR = 1,
  CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_THIRD_FLOOR = 2,
  CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_ICE_PILLARS_EAST = 3,
  CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_ICE_PILLARS_WEST = 4,
};

/// SceneId_CrossboneIsleThirdFloor's entrances.
enum {
  CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_SECOND_FLOOR = 1,
  CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_FOURTH_FLOOR = 2,
  CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_KEYS_EAST = 3,
  CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_KEYS_WEST = 4,
};
/// SceneId_CrossboneIsleThirdFloor's exits.
enum {
  CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR = 1,
  CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_FOURTH_FLOOR = 2,
  CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_KEYS_EAST = 3,
  CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_KEYS_WEST = 4,
};

/// SceneId_CrossboneIsleFourthFloor's entrances.
enum {
  CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_THIRD_FLOOR = 1,
  CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_FIFTH_FLOOR = 2,
  CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_STATUES_EAST = 3,
  CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_STATUES_WEST = 4,
};
/// SceneId_CrossboneIsleFourthFloor's exits.
enum {
  CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_THIRD_FLOOR = 1,
  CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_FIFTH_FLOOR = 2,
  CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_STATUES_EAST = 3,
  CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_STATUES_WEST = 4,
};

/// SceneId_CrossboneIsleFifthFloor's entrances.
enum {
  CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FOURTH_FLOOR = 1,
  CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_SIXTH_FLOOR = 2,
  CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FLOODGATE_EAST = 3,
  CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FLOODGATE_WEST = 4,
};
/// SceneId_CrossboneIsleFifthFloor's exits.
enum {
  CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FOURTH_FLOOR = 1,
  CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_SIXTH_FLOOR = 2,
  CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FLOODGATE_EAST = 3,
  CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FLOODGATE_WEST = 4,
};

/// SceneId_CrossboneIsleSixthFloor's entrances.
enum {
  CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_FIFTH_FLOOR = 1,
  CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR = 2,
  CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_BANE_EAST = 3,
  CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_BANE_WEST = 4,
};
/// SceneId_CrossboneIsleSixthFloor's exits.
enum {
  CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_FIFTH_FLOOR = 1,
  CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_SEVENTH_FLOOR = 2,
  CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_BANE_EAST = 3,
  CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_BANE_WEST = 4,
};

/// SceneId_CrossboneIsleSeventhFloor's entrances.
enum {
  CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_SIXTH_FLOOR = 1,
  CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR = 2,
  CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_STACKED_PILLARS_EAST = 3,
  CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_STACKED_PILLARS_WEST = 4,
};
/// SceneId_CrossboneIsleSeventhFloor's exits.
enum {
  CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_SIXTH_FLOOR = 1,
  CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_EIGHTH_FLOOR = 2,
  CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_STACKED_PILLARS_EAST = 3,
  CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_STACKED_PILLARS_WEST = 4,
};

/// SceneId_CrossboneIsleEighthFloor's entrances.
enum {
  CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR = 1,
  CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_NINTH_FLOOR = 2,
  CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_DRAGON_STATUE_EAST = 3,
  CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_DRAGON_STATUE_WEST = 4,
};
/// SceneId_CrossboneIsleEighthFloor's exits.
enum {
  CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_SEVENTH_FLOOR = 1,
  CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_NINTH_FLOOR = 2,
  CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_DRAGON_STATUE_EAST = 3,
  CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_DRAGON_STATUE_WEST = 4,
};

/// SceneId_CrossboneIsleNinthFloor's entrances.
enum {
  CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR = 1,
  CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_PIER = 2,
  CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_FLOATING_LOGS_EAST = 3,
  CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_FLOATING_LOGS_WEST = 4,
};
/// SceneId_CrossboneIsleNinthFloor's exits.
enum {
  CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_EIGHTH_FLOOR = 1,
  CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_PIER = 2,
  CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_FLOATING_LOGS_EAST = 3,
  CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_FLOATING_LOGS_WEST = 4,
};

/// SceneId_Tolbi's entrances.
enum {
  TOLBI_FROM_HOUSE_SOUTH = 1,
  TOLBI_FROM_HOUSE_NORTHWEST = 2,
  TOLBI_FROM_INN = 3,
  TOLBI_FROM_TOLBI_INTERIORS_EAST = 4,
  TOLBI_FROM_HOUSE_NORTHEAST = 5,
  TOLBI_FROM_HOUSE_EAST = 6,
  TOLBI_FROM_WEAPON_AND_ARMOR_SHOP = 7,
  TOLBI_FROM_HOUSE_SOUTHWEST = 8,
  TOLBI_FROM_SANCTUM = 9,
  TOLBI_FROM_TOLBI_INTERIORS_WEST = 10,
  TOLBI_FROM_BABI_PALACE_ENTRANCE = 11,
  TOLBI_FROM_COLOSSEUM_ENTRANCE = 12,
  TOLBI_FROM_LUCKY_DICE = 13,
  TOLBI_FROM_TOLBI_SPRING = 20,
};
/// SceneId_Tolbi's exits.
enum {
  TOLBI_TO_HOUSE_SOUTH = 1, ///< ★ Named from what the player sees: a house with
                            ///< beds, a table and a hearth (its render)
  TOLBI_TO_HOUSE_NORTHWEST = 2,
  TOLBI_TO_INN = 3,          ///< ★ Named from the game: "Inn Full"
                             ///< (MsgTolbiInteriorsIsaacCheckedTheSignInnFull)
  TOLBI_TO_LUCKY_WHEELS = 4, ///< ★ Named from the game: "Lucky Wheels"
                             ///< (MsgTolbiLuckyWheelsRulesPullLever)
  TOLBI_TO_HOUSE_NORTHEAST = 5,
  TOLBI_TO_HOUSE_EAST = 6,
  TOLBI_TO_WEAPON_AND_ARMOR_SHOP =
      7, ///< ★ Named from what the player sees: the shop of two rooms joined at
         ///< the front, a seller behind the counter of each (its render)
  TOLBI_TO_HOUSE_SOUTHWEST = 8,
  TOLBI_TO_SANCTUM = 9,
  TOLBI_TO_BABI_PALACE_ENTRANCE = 10,
  TOLBI_TO_COLOSSEUM_ENTRANCE = 11,
  TOLBI_TO_WORLD_MAP = 12,
  TOLBI_TO_LUCKY_DICE = 13, ///< ★ Named from the game: "Lucky Dice"
                            ///< (MsgTolbiSpringLuckyDiceThrowTheDiceOn)
  TOLBI_TO_TOLBI_SPRING = 30,
};

/// SceneId_TolbiInteriors's entrances.
enum {
  TOLBI_INTERIORS_HOUSE_NORTHEAST =
      1, ///< the room behind Tolbi's house northeast door; through
         ///< TOLBI_TO_HOUSE_NORTHEAST
  TOLBI_INTERIORS_HOUSE_NORTHWEST =
      2, ///< the room behind Tolbi's house northwest door; through
         ///< TOLBI_TO_HOUSE_NORTHWEST
  TOLBI_INTERIORS_HOUSE_EAST = 3,  ///< the room behind Tolbi's house east door;
                                   ///< through TOLBI_TO_HOUSE_EAST
  TOLBI_INTERIORS_HOUSE_SOUTH = 4, ///< the room behind Tolbi's house south
                                   ///< door; through TOLBI_TO_HOUSE_SOUTH
  TOLBI_INTERIORS_HOUSE_SOUTHWEST =
      5, ///< the room behind Tolbi's house southwest door; through
         ///< TOLBI_TO_HOUSE_SOUTHWEST
  TOLBI_INTERIORS_WEAPON_AND_ARMOR_SHOP =
      6, ///< ★ Named from what the player sees: the shop of two rooms joined at
         ///< the front, a seller behind the counter of each (its render);
         ///< through TOLBI_TO_WEAPON_AND_ARMOR_SHOP
  TOLBI_INTERIORS_SANCTUM =
      8, ///< the room behind Tolbi's sanctum door; the
         ///< room holds the sanctum; through TOLBI_TO_SANCTUM
  TOLBI_INTERIORS_INN =
      9, ///< the room behind Tolbi's inn door; through TOLBI_TO_INN
  TOLBI_INTERIORS_INN_FROM_INN_PASSAGE =
      10, ///< ★ Named from the game: "Inn Full"
          ///< (MsgTolbiInteriorsIsaacCheckedTheSignInnFull)
  TOLBI_INTERIORS_INN_PASSAGE_FROM_INN =
      11, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  TOLBI_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST =
      12, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  TOLBI_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST =
      13, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  TOLBI_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST =
      14, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  TOLBI_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST =
      15, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  TOLBI_INTERIORS_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE =
      16, ///< ★ Named from what the player sees: the guest room through the
          ///< east opening on the corridor's south side, where stranded
          ///< passengers argue (its render)
  TOLBI_INTERIORS_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE =
      17, ///< ★ Named from what the player sees: the guest room through the
          ///< west opening on the inn corridor's south side (its render)
  TOLBI_INTERIORS_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE =
      18, ///< ★ Named from what the player sees: the guest room behind the west
          ///< door in the inn corridor's north wall (its render)
  TOLBI_INTERIORS_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE =
      19, ///< ★ Named from what the player sees: the guest room behind the east
          ///< door in the corridor's north wall, where a woman hides a Lucky
          ///< Medal (its render)
  TOLBI_INTERIORS_SANCTUM_NORTH = 30, ///< the room holds the sanctum;
};
/// SceneId_TolbiInteriors's exits.
enum {
  TOLBI_INTERIORS_HOUSE_NORTHWEST_TO_TOLBI = 1,
  TOLBI_INTERIORS_INN_TO_TOLBI = 2,
  TOLBI_INTERIORS_TO_TOLBI_EAST = 3,
  TOLBI_INTERIORS_HOUSE_NORTHEAST_TO_TOLBI = 4,
  TOLBI_INTERIORS_HOUSE_EAST_TO_TOLBI = 5,
  TOLBI_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_TOLBI = 6,
  TOLBI_INTERIORS_HOUSE_SOUTHWEST_TO_TOLBI = 7,
  TOLBI_INTERIORS_SANCTUM_TO_TOLBI = 8,
  TOLBI_INTERIORS_TO_TOLBI_WEST = 9,
  TOLBI_INTERIORS_INN_TO_INN_PASSAGE =
      10, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  TOLBI_INTERIORS_INN_PASSAGE_TO_INN =
      11, ///< ★ Named from the game: "Inn Full"
          ///< (MsgTolbiInteriorsIsaacCheckedTheSignInnFull)
  TOLBI_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST =
      12, ///< ★ Named from what the player sees: the guest room through the
          ///< west opening on the inn corridor's south side (its render)
  TOLBI_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST =
      13, ///< ★ Named from what the player sees: the guest room through the
          ///< east opening on the corridor's south side, where stranded
          ///< passengers argue (its render)
  TOLBI_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST =
      14, ///< ★ Named from what the player sees: the guest room behind the west
          ///< door in the inn corridor's north wall (its render)
  TOLBI_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST =
      15, ///< ★ Named from what the player sees: the guest room behind the east
          ///< door in the corridor's north wall, where a woman hides a Lucky
          ///< Medal (its render)
  TOLBI_INTERIORS_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE =
      16, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  TOLBI_INTERIORS_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE =
      17, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  TOLBI_INTERIORS_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE =
      18, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
  TOLBI_INTERIORS_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE =
      19, ///< ★ Named from what the player sees: the corridor at the foot of
          ///< the inn's stairs, lined with guest-room doors (its render)
};

/// SceneId_TolbiGameRooms's entrances.
enum {
  TOLBI_GAME_ROOMS_LUCKY_DICE_FROM_TOLBI =
      10, ///< ★ Named from the game: "Throw the dice"
          ///< (MsgTolbiSpringLuckyDiceThrowTheDiceOn)
  TOLBI_GAME_ROOMS_LUCKY_WHEELS_FROM_TOLBI =
      11, ///< ★ Named from the game: "Lucky Wheels"
          ///< (MsgTolbiLuckyWheelsRulesPullLever)
  TOLBI_GAME_ROOMS_AFTER_OFFER_LUCKY_WHEELS =
      12, ///< TolbiSpring_OfferLuckyWheels
  TOLBI_GAME_ROOMS_AFTER_OFFER_LUCKY_DICE = 13, ///< TolbiSpring_OfferLuckyDice
};
/// SceneId_TolbiGameRooms's exits.
enum {
  TOLBI_GAME_ROOMS_LUCKY_DICE_TO_TOLBI = 1,
  TOLBI_GAME_ROOMS_LUCKY_WHEELS_TO_TOLBI = 2,
};

/// SceneId_BabiPalace's entrances.
enum {
  BABI_PALACE_FROM_BABI_PALACE_ENTRANCE = 1,
  BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_WEST =
      2, ///< ★ Named from what the player sees: the hall entered from outside,
         ///< with a winged statue in its middle (its render)
  BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_EAST =
      3, ///< ★ Named from what the player sees: the hall entered from outside,
         ///< with a winged statue in its middle (its render)
  BABI_PALACE_STATUE_ROOM_FROM_WEST_STEPS =
      4, ///< ★ Named from what the player sees: the hall entered from outside,
         ///< with a winged statue in its middle (its render)
  BABI_PALACE_STATUE_ROOM_FROM_EAST_STEPS =
      5, ///< ★ Named from what the player sees: the hall entered from outside,
         ///< with a winged statue in its middle (its render)
  BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_WEST =
      6, ///< ★ Named from what the player sees: the hall entered from outside,
         ///< with a winged statue in its middle (its render)
  BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_EAST =
      7, ///< ★ Named from what the player sees: the hall entered from outside,
         ///< with a winged statue in its middle (its render)
  BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST =
      8, ///< ★ Named from the game: "in this big bedroom"
         ///< (MsgBabiPalaceNoOnesGoingToSleepIn)
  BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_EAST =
      9, ///< ★ Named from the game: "in this big bedroom"
         ///< (MsgBabiPalaceNoOnesGoingToSleepIn)
  BABI_PALACE_SCHOLARS_LANDING_FROM_CARPET_ROOM =
      10, ///< ★ Named from what the player sees: the stair landing whose dark
          ///< doorway leads to the scholars' room (its render)
  BABI_PALACE_FROM_VALE_INTERIORS_INN = 11,
  BABI_PALACE_TUNNEL_LANDING_FROM_CARPET_ROOM =
      12, ///< ★ Named from what the player sees: the stair landing whose dark
          ///< doorway leads into the Colosseum Tunnel (its render)
  BABI_PALACE_FROM_COLOSSEUM_TUNNEL = 13,
  BABI_PALACE_BEDROOM_LANDING_FROM_MEETING_ROOM =
      16, ///< ★ Named from what the player sees: the small room of stairs and a
          ///< door outside Babi's bedroom (its render)
  BABI_PALACE_BEDROOM_LANDING_FROM_BABIS_BEDROOM =
      17, ///< ★ Named from what the player sees: the small room of stairs and a
          ///< door outside Babi's bedroom (its render)
  BABI_PALACE_BABIS_BEDROOM_FROM_BEDROOM_LANDING =
      18, ///< ★ Named from the game: "Lord Babi didn't use his bed"
          ///< (MsgBabiPalaceItLooksLikeLordBabiDidnt)
  BABI_PALACE_WEST_STEPS_FROM_STATUE_ROOM =
      20, ///< ★ Named from what the player sees: the small room that is only a
          ///< staircase, off the statue hall's west side (its render)
  BABI_PALACE_WEST_STEPS_FROM_WEST_ROUND_ROOM =
      21, ///< ★ Named from what the player sees: the small room that is only a
          ///< staircase, off the statue hall's west side (its render)
  BABI_PALACE_EAST_STEPS_FROM_STATUE_ROOM =
      22, ///< ★ Named from what the player sees: the small room that is only a
          ///< staircase, off the statue hall's east side (its render)
  BABI_PALACE_EAST_STEPS_FROM_EAST_ROUND_ROOM =
      23, ///< ★ Named from what the player sees: the small room that is only a
          ///< staircase, off the statue hall's east side (its render)
  BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_WEST =
      24, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_EAST =
      25, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_CARPET_ROOM_FROM_SCHOLARS_LANDING =
      26, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_CARPET_ROOM_FROM_TUNNEL_LANDING =
      27, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_CARPET_ROOM_FROM_NORTH_ROUND_ROOM =
      28, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_CARPET_ROOM_FROM_SHEBAS_LANDING =
      29, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_CARPET_ROOM_FROM_OVEN_ROOM =
      30, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_CARPET_ROOM_FROM_SOLDIERS_ROOM =
      31, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_CARPET_ROOM_FROM_MEETING_ROOM =
      32, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM =
      33, ///< ★ Named from what the player sees: the room with Babi's green
          ///< throne, where he meets the party after Colosso (its render)
  BABI_PALACE_MEETING_ROOM_FROM_BEDROOM_LANDING =
      34, ///< ★ Named from what the player sees: the room with Babi's green
          ///< throne, where he meets the party after Colosso (its render)
  BABI_PALACE_OVEN_ROOM_FROM_CARPET_ROOM =
      35, ///< ★ Named from what the player sees: the room with a table and a
          ///< lit stone stove (its render)
  BABI_PALACE_SOLDIERS_ROOM_FROM_CARPET_ROOM =
      36, ///< ★ Named from what the player sees: the room where four of Babi's
          ///< soldiers stand by a table and a bed (its render)
  BABI_PALACE_WEST_ROUND_ROOM_FROM_WEST_STEPS =
      40, ///< ★ Named from what the player sees: the small eight-sided room up
          ///< the west staircase, with two jars (its render)
  BABI_PALACE_EAST_ROUND_ROOM_FROM_EAST_STEPS =
      41, ///< ★ Named from what the player sees: the small eight-sided room up
          ///< the east staircase (its render)
  BABI_PALACE_NORTH_ROUND_ROOM_FROM_CARPET_ROOM =
      42, ///< ★ Named from what the player sees: the small eight-sided room up
          ///< the central hall's far west stairs (its render)
  BABI_PALACE_SHEBAS_LANDING_FROM_CARPET_ROOM =
      43, ///< ★ Named from what the player sees: the small room of stairs and a
          ///< door outside Sheba's room (its render)
  BABI_PALACE_SHEBAS_LANDING_FROM_SHEBAS_ROOM =
      44, ///< ★ Named from what the player sees: the small room of stairs and a
          ///< door outside Sheba's room (its render)
  BABI_PALACE_SHEBAS_ROOM_FROM_SHEBAS_LANDING =
      45, ///< ★ Named from the game: "Sheba" (MsgCharacterName)
  /// In no table row and led to by no exit: arriving by it sets
  /// FLAG_BABI_SAVED.
  /// XXX: likely a debug start.
  BABI_PALACE_AFTER_BABI_SAVED = 90,
  /// In no table row and led to by no exit: arriving by it sets FLAG_BABI_SAVED
  /// and FLAG_COLOSSO_OVER. XXX: likely a debug start.
  BABI_PALACE_AFTER_COLOSSO = 91,
  BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH = 98,
  BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH = 99,
};
/// SceneId_BabiPalace's exits.
enum {
  BABI_PALACE_TO_BABI_PALACE_ENTRANCE = 1,
  BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST =
      2, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
         ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST =
      3, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
         ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS =
      4, ///< ★ Named from what the player sees: the small room that is only a
         ///< staircase, off the statue hall's west side (its render)
  BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS =
      5, ///< ★ Named from what the player sees: the small room that is only a
         ///< staircase, off the statue hall's east side (its render)
  BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST =
      6, ///< ★ Named from the game: "in this big bedroom"
         ///< (MsgBabiPalaceNoOnesGoingToSleepIn)
  BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST =
      7, ///< ★ Named from the game: "in this big bedroom"
         ///< (MsgBabiPalaceNoOnesGoingToSleepIn)
  BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST =
      8, ///< ★ Named from what the player sees: the hall entered from outside,
         ///< with a winged statue in its middle (its render)
  BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST =
      9, ///< ★ Named from what the player sees: the hall entered from outside,
         ///< with a winged statue in its middle (its render)
  BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM =
      10, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_TO_VALE_INTERIORS_INN = 11,
  BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM =
      12, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_TO_COLOSSEUM_TUNNEL = 13,
  BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM =
      16, ///< ★ Named from what the player sees: the room with Babi's green
          ///< throne, where he meets the party after Colosso (its render)
  BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM =
      17, ///< ★ Named from the game: "Lord Babi didn't use his bed"
          ///< (MsgBabiPalaceItLooksLikeLordBabiDidnt)
  BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING =
      18, ///< ★ Named from what the player sees: the small room of stairs and a
          ///< door outside Babi's bedroom (its render)
  BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM =
      20, ///< ★ Named from what the player sees: the hall entered from outside,
          ///< with a winged statue in its middle (its render)
  BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM =
      21, ///< ★ Named from what the player sees: the small eight-sided room up
          ///< the west staircase, with two jars (its render)
  BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM =
      22, ///< ★ Named from what the player sees: the hall entered from outside,
          ///< with a winged statue in its middle (its render)
  BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM =
      23, ///< ★ Named from what the player sees: the small eight-sided room up
          ///< the east staircase (its render)
  BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST =
      24, ///< ★ Named from what the player sees: the hall entered from outside,
          ///< with a winged statue in its middle (its render)
  BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST =
      25, ///< ★ Named from what the player sees: the hall entered from outside,
          ///< with a winged statue in its middle (its render)
  BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING =
      26, ///< ★ Named from what the player sees: the stair landing whose dark
          ///< doorway leads to the scholars' room (its render)
  BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING =
      27, ///< ★ Named from what the player sees: the stair landing whose dark
          ///< doorway leads into the Colosseum Tunnel (its render)
  BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM =
      28, ///< ★ Named from what the player sees: the small eight-sided room up
          ///< the central hall's far west stairs (its render)
  BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING =
      29, ///< ★ Named from what the player sees: the small room of stairs and a
          ///< door outside Sheba's room (its render)
  BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM =
      30, ///< ★ Named from what the player sees: the room with a table and a
          ///< lit stone stove (its render)
  BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM =
      31, ///< ★ Named from what the player sees: the room where four of Babi's
          ///< soldiers stand by a table and a bed (its render)
  BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM =
      32, ///< ★ Named from what the player sees: the room with Babi's green
          ///< throne, where he meets the party after Colosso (its render)
  BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM =
      33, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING =
      34, ///< ★ Named from what the player sees: the small room of stairs and a
          ///< door outside Babi's bedroom (its render)
  BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM =
      35, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM =
      36, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS =
      40, ///< ★ Named from what the player sees: the small room that is only a
          ///< staircase, off the statue hall's west side (its render)
  BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS =
      41, ///< ★ Named from what the player sees: the small room that is only a
          ///< staircase, off the statue hall's east side (its render)
  BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM =
      42, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM =
      43, ///< ★ Named from what the player sees: the T-shaped red-carpeted hall
          ///< that joins the palace's stairs and doors (its render)
  BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM =
      44, ///< ★ Named from the game: "Sheba" (MsgCharacterName)
  BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING =
      45, ///< ★ Named from what the player sees: the small room of stairs and a
          ///< door outside Sheba's room (its render)
};

/// SceneId_BabiPalaceEntrance's entrances.
enum {
  BABI_PALACE_ENTRANCE_FROM_TOLBI = 1,
  BABI_PALACE_ENTRANCE_FROM_BABI_PALACE = 2,
};
/// SceneId_BabiPalaceEntrance's exits.
enum {
  BABI_PALACE_ENTRANCE_TO_TOLBI = 1,
  BABI_PALACE_ENTRANCE_TO_BABI_PALACE = 2,
};

/// SceneId_Colosseum's entrances.
enum {
  COLOSSEUM_STONE_PASSAGE_FROM_STANDS_NORTH =
      1, ///< ★ Named from what the player sees: the stone corridor the
         ///< Colosseum Tunnel opens into, with doorways to the stands (its
         ///< render)
  COLOSSEUM_STONE_PASSAGE_FROM_STANDS_WEST =
      2, ///< ★ Named from what the player sees: the stone corridor the
         ///< Colosseum Tunnel opens into, with doorways to the stands (its
         ///< render)
  COLOSSEUM_FROM_COLOSSEUM_TUNNEL = 4,
  COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST =
      5, ///< ★ Named from what the player sees: the crowded seats above the
         ///< trial grounds (its render)
  COLOSSEUM_STANDS_FROM_STONE_PASSAGE_NORTH =
      6, ///< ★ Named from what the player sees: the crowded seats above the
         ///< trial grounds (its render)
  COLOSSEUM_FAILED_TRIAL_FROM_STANDS =
      7, ///< ★ Named from what the player sees: a warrior strains at the trial,
         ///< gives up and runs (its render)
  COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST = 12,
  COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST = 21,
  COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH = 31,
  COLOSSEUM_FROM_COLOSSEUM_ENTRANCE = 64,
  COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST =
      65, ///< ★ Named from what the player sees: the long passage with arched
          ///< walls where an official leads Isaac to the finals (its render)
  COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH = 66,
  COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST =
      67, ///< ★ Named from what the player sees: the long passage with arched
          ///< walls where an official leads Isaac to the finals (its render)
  COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH = 68,
  COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS =
      69, ///< ★ Named from what the player sees: the crowded seats above the
          ///< trial grounds (its render)
  COLOSSEUM_CLEARED_TRIAL_FROM_STANDS =
      70, ///< ★ Named from what the player sees: a warrior heaves the trial's
          ///< load up and drops it, shaking the ground (its render)
};
/// SceneId_Colosseum's exits.
enum {
  COLOSSEUM_STONE_PASSAGE_TO_STANDS_NORTH =
      1, ///< ★ Named from what the player sees: the crowded seats above the
         ///< trial grounds (its render)
  COLOSSEUM_STONE_PASSAGE_TO_STANDS_WEST =
      2, ///< ★ Named from what the player sees: the crowded seats above the
         ///< trial grounds (its render)
  COLOSSEUM_STONE_PASSAGE_LOCKED_DOOR_TO_STONE_PASSAGE =
      3, ///< ★ Named from what the player sees: the stone corridor the
         ///< Colosseum Tunnel opens into, with doorways to the stands (its
         ///< render)
  COLOSSEUM_TO_COLOSSEUM_TUNNEL = 4,
  COLOSSEUM_STANDS_TO_STONE_PASSAGE_WEST =
      5, ///< ★ Named from what the player sees: the stone corridor the
         ///< Colosseum Tunnel opens into, with doorways to the stands (its
         ///< render)
  COLOSSEUM_STANDS_TO_STONE_PASSAGE_NORTH =
      6, ///< ★ Named from what the player sees: the stone corridor the
         ///< Colosseum Tunnel opens into, with doorways to the stands (its
         ///< render)
  COLOSSEUM_STANDS_TO_FAILED_TRIAL =
      7, ///< ★ Named from what the player sees: a warrior strains at the trial,
         ///< gives up and runs (its render)
  COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST = 12,
  COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_WEST = 21,
  COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST = 22,
  COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_NORTH = 31,
  COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH = 32,
  COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST =
      64, ///< ★ Named from what the player sees: the long passage with arched
          ///< walls where an official leads Isaac to the finals (its render)
  COLOSSEUM_TO_COLOSSEUM_AZART_MATCH = 65,
  COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST =
      66, ///< ★ Named from what the player sees: the long passage with pillars
          ///< before a plain wall that Isaac walks alone (its render)
  COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_WEST =
      67, ///< ★ Named from what the player sees: the long passage with arched
          ///< walls where an official leads Isaac to the finals (its render)
  COLOSSEUM_TRIAL_GROUNDS_TO_STANDS =
      69, ///< ★ Named from what the player sees: the crowded seats above the
          ///< trial grounds (its render)
  COLOSSEUM_STANDS_TO_CLEARED_TRIAL =
      70, ///< ★ Named from what the player sees: a warrior heaves the trial's
          ///< load up and drops it, shaking the ground (its render)
};

/// SceneId_ColosseumTunnel's entrances.
enum {
  COLOSSEUM_TUNNEL_FROM_BABI_PALACE = 1,
  COLOSSEUM_TUNNEL_FROM_COLOSSEUM = 2,
};
/// SceneId_ColosseumTunnel's exits.
enum {
  COLOSSEUM_TUNNEL_TO_BABI_PALACE = 1,
  COLOSSEUM_TUNNEL_TO_COLOSSEUM = 2,
};

/// SceneId_ColosseumEntrance's entrances.
enum {
  COLOSSEUM_ENTRANCE_FROM_TOLBI = 1,
};
/// SceneId_ColosseumEntrance's exits.
enum {
  COLOSSEUM_ENTRANCE_TO_TOLBI = 1,
  COLOSSEUM_ENTRANCE_TO_COLOSSEUM = 2,
};

/// SceneId_ColosseumAzartMatch's entrances.
enum {
  COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND =
      1, ///< ★ Named from what the player sees: Isaac racing Azart through the
         ///< stages after the lettering Match 1 (its render)
  COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST = 2,
  COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM = 3,
  /// The return from the battle with Azart when Isaac wins.
  COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON = 4,
  /// The return from the battle with Azart when Isaac loses.
  COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST = 5,
};
/// SceneId_ColosseumAzartMatch's exits.
enum {
  COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST = 4,
  COLOSSEUM_AZART_MATCH_TO_BABI_PALACE = 5,
  COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE =
      10, ///< ★ Named from what the player sees: Isaac racing Azart through the
          ///< stages after the lettering Match 1 (its render)
  COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST = 11,
};

/// SceneId_ColosseumSatrageMatch's entrances.
enum {
  COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND =
      1, ///< ★ Named from what the player sees: Isaac racing Satrage through
         ///< the stages after the lettering Match 2 (its render)
  COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST = 2,
  COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST = 3,
  /// The return from the battle with Satrage when Isaac wins.
  COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON = 4,
  /// The return from the battle with Satrage when Isaac loses.
  COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST = 5,
};
/// SceneId_ColosseumSatrageMatch's exits.
enum {
  COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH = 4,
  COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE = 5,
  COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE =
      10, ///< ★ Named from what the player sees: Isaac racing Satrage through
          ///< the stages after the lettering Match 2 (its render)
  COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH = 11,
};

/// SceneId_ColosseumNavampaMatch's entrances.
enum {
  COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND =
      1, ///< ★ Named from what the player sees: Isaac racing Navampa through
         ///< the stages after the lettering Match 3 (its render)
  COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH = 2,
  COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH = 3,
  /// The return from the battle with Navampa when Isaac wins.
  COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON = 4,
  /// The return from the battle with Navampa when Isaac loses.
  COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST = 5,
};
/// SceneId_ColosseumNavampaMatch's exits.
enum {
  COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE = 4,
  COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE_LOST =
      5, ///< ★ Named from what the player sees: the third match lost, after
         ///< which the party wakes in Babi's Palace (its render)
  COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE =
      10, ///< ★ Named from what the player sees: Isaac racing Navampa through
          ///< the stages after the lettering Match 3 (its render)
  COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM = 11,
};

/// SceneId_AltmillerCaveEntrance's entrances.
enum {
  ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP = 1,
  ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER = 2,
  ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_FROM_PASSAGE =
      3, ///< ★ Named from what the player sees: the small chamber through the
         ///< hole at the passage's end, with a torch-lit doorway (its render)
  ALTMILLER_CAVE_ENTRANCE_FROM_ALTMILLER_CAVE_BABI = 4,
};
/// SceneId_AltmillerCaveEntrance's exits.
enum {
  ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP = 1,
  ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER =
      2, ///< ★ Named from what the player sees: the small chamber through the
         ///< hole at the passage's end, with a torch-lit doorway (its render)
  ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE =
      3, ///< ★ Named from what the player sees: the dark winding passage in
         ///< from the cave mouth (its render)
  ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI = 4,
};

/// SceneId_AltmillerCaveBabi's entrances.
enum {
  ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE = 10,
  ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_VIAL_CHEST = 11,
};
/// SceneId_AltmillerCaveBabi's exits.
enum {
  ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE = 10,
  ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST = 11,
};

/// SceneId_AltmillerCaveVialChest's entrances.
enum {
  ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI = 20,
  ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_SQUALL = 21,
};
/// SceneId_AltmillerCaveVialChest's exits.
enum {
  ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI = 20,
  ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL = 21,
};

/// SceneId_AltmillerCaveSquall's entrances.
enum {
  ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST = 30,
  ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_COOKIE_CHEST = 31,
};
/// SceneId_AltmillerCaveSquall's exits.
enum {
  ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST = 30,
  ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST = 31,
};

/// SceneId_AltmillerCaveCookieChest's entrances.
enum {
  ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL = 40,
  ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_DRAUGHT = 41,
};
/// SceneId_AltmillerCaveCookieChest's exits.
enum {
  ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL = 40,
  ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT = 41,
};

/// SceneId_AltmillerCaveDraught's entrances.
enum {
  ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST = 50,
  ALTMILLER_CAVE_DRAUGHT_FROM_ROCKS = 51,
  ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE =
      52, ///< ★ Named from the game: "rotated the rock"
          ///< (MsgAltmillerRotatedRock)
  ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_CHEST =
      53, ///< ★ Named from the game: "rotated the rock"
          ///< (MsgAltmillerRotatedRock)
  ALTMILLER_CAVE_DRAUGHT_CHEST_FROM_ROCKS =
      54, ///< ★ Named from what the player sees: the chamber behind the burst
          ///< wall, with one chest on a stone (its render)
};
/// SceneId_AltmillerCaveDraught's exits.
enum {
  ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST =
      50, ///< ★ Named from what the player sees: the chamber behind the burst
          ///< wall, with one chest on a stone (its render)
  ALTMILLER_CAVE_DRAUGHT_TO_ROCKS = 51, ///< ★ Named from the game: "rotated the
                                        ///< rock" (MsgAltmillerRotatedRock)
  ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE =
      52, ///< ★ Named from what the player sees: the large cavern of stone
          ///< pillars before the rocks (its render)
  ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST =
      53, ///< ★ Named from what the player sees: the chamber behind the burst
          ///< wall, with one chest on a stone (its render)
  ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS =
      54, ///< ★ Named from the game: "rotated the rock"
          ///< (MsgAltmillerRotatedRock)
};

/// SceneId_GondowanPassage's entrances.
enum {
  GONDOWAN_PASSAGE_FROM_WORLD_MAP_NORTH = 1,
  GONDOWAN_PASSAGE_FROM_WORLD_MAP_SOUTH = 2,
};
/// SceneId_GondowanPassage's exits.
enum {
  GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH = 1,
  GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH = 2,
};

/// SceneId_KalayTunnelEntrance's entrances.
enum {
  KALAY_TUNNEL_ENTRANCE_FROM_KALAY_TUNNEL_CAVE = 1,
  KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_NORTH = 2,
  KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH = 10,
};
/// SceneId_KalayTunnelEntrance's exits.
enum {
  KALAY_TUNNEL_ENTRANCE_TO_KALAY_TUNNEL_CAVE = 1,
  KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_NORTH = 2,
  KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_SOUTH = 10,
};

/// SceneId_KalayTunnelCave's entrances.
enum {
  KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_PASSAGE = 1,
  KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_ENTRANCE = 2,
};
/// SceneId_KalayTunnelCave's exits.
enum {
  KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_PASSAGE = 1,
  KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_ENTRANCE = 2,
};

/// SceneId_KalayTunnelPassage's entrances.
enum {
  KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_WEST = 1,
  KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_EAST = 2,
  KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM =
      3, ///< ★ Named from what the player sees: the long brick corridor beside
         ///< the water channel (its render)
  KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_FROM_LONG_PATH =
      4, ///< ★ Named from what the player sees: the small room with two
         ///< stairways, apart from the corridor, where the cave comes in (its
         ///< render)
  KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_CAVE = 5,
};
/// SceneId_KalayTunnelPassage's exits.
enum {
  KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_WEST = 1,
  KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_EAST = 2,
  KALAY_TUNNEL_PASSAGE_LONG_PATH_TO_NORTHEAST_ROOM =
      3, ///< ★ Named from what the player sees: the small room with two
         ///< stairways, apart from the corridor, where the cave comes in (its
         ///< render)
  KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_TO_LONG_PATH =
      4, ///< ★ Named from what the player sees: the long brick corridor beside
         ///< the water channel (its render)
  KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_CAVE = 5,
};

/// SceneId_KalayTunnelTreasureRoom's entrances.
enum {
  KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE = 1,
  KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_WEST = 2,
  KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_EAST = 3,
  KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY = 4,
};
/// SceneId_KalayTunnelTreasureRoom's exits.
enum {
  KALAY_TUNNEL_TREASURE_ROOM_TO_HAMMET_PALACE = 1,
  KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_WEST = 2,
  KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_EAST = 3,
  KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY = 4,
};

/// SceneId_GondowanCaveRiver's entrances.
enum {
  GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_NORTH = 1,
  GONDOWAN_CAVE_RIVER_FROM_BOULDER_PASSAGE = 2,
  GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_FROM_RIVER =
      3, ///< ★ Named from what the player sees: the passage with a boulder that
         ///< Lift raises (its render)
  GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_SOUTH = 4,
};
/// SceneId_GondowanCaveRiver's exits.
enum {
  GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH = 1,
  GONDOWAN_CAVE_RIVER_TO_BOULDER_PASSAGE =
      2, ///< ★ Named from what the player sees: the passage with a boulder that
         ///< Lift raises (its render)
  GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_TO_RIVER =
      3, ///< ★ Named from what the player sees: the underground river crossed
         ///< on stepping pads (its render)
  GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_SOUTH = 4,
};

/// SceneId_GondowanCave's entrances.
enum {
  GONDOWAN_CAVE_FROM_WORLD_MAP_EAST = 1,
  GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_NORTH = 2,
  GONDOWAN_CAVE_FROM_WEST_PASSAGE = 3,
  GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVE =
      4, ///< ★ Named from what the player sees: the narrow passage with a pool
         ///< and a boulder that leads to the west way in (its render)
  GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_SOUTH = 5,
  GONDOWAN_CAVE_FROM_WORLD_MAP_WEST = 6,
};
/// SceneId_GondowanCave's exits.
enum {
  GONDOWAN_CAVE_TO_WORLD_MAP_EAST = 1,
  GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH = 2,
  GONDOWAN_CAVE_TO_WEST_PASSAGE =
      3, ///< ★ Named from what the player sees: the narrow passage with a pool
         ///< and a boulder that leads to the west way in (its render)
  GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVE =
      4, ///< ★ Named from what the player sees: the cavern of the east way in,
         ///< with the stump and the pillar (its render)
  GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH = 5,
  GONDOWAN_CAVE_TO_WORLD_MAP_WEST = 6,
};

/// SceneId_LunpaFortressGates's entrances.
enum {
  LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_GROUND_FLOOR = 1,
  LUNPA_FORTRESS_GATES_FROM_LUNPA = 2,
  LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA = 3,
  LUNPA_FORTRESS_GATES_AFTER_CAUGHT =
      4, ///< where a guard who catches the party in the fortress sends it:
         ///< "You're on your way out now!" (LunpaFortress_GuardThrowsPartyOut)
};
/// SceneId_LunpaFortressGates's exits.
enum {
  LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR = 1,
  LUNPA_FORTRESS_GATES_TO_LUNPA = 2,
};

/// SceneId_LunpaFortressGroundFloor's entrances.
enum {
  LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_GATES = 1,
  LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_PASSAGE =
      2, ///< ★ Named from what the player sees: the small room just inside the
         ///< fortress door (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_ENTRANCE =
      3, ///< ★ Named from what the player sees: the long winding hall kept by
         ///< the entrance guard and two sentries (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTH_PASSAGE =
      4, ///< ★ Named from what the player sees: the long winding hall kept by
         ///< the entrance guard and two sentries (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_FEAST_ROOM =
      5, ///< ★ Named from what the player sees: the long winding hall kept by
         ///< the entrance guard and two sentries (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTHEAST_PASSAGE =
      6, ///< ★ Named from what the player sees: the long winding hall kept by
         ///< the entrance guard and two sentries (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_EAST_PASSAGE =
      7, ///< ★ Named from what the player sees: the long winding hall kept by
         ///< the entrance guard and two sentries (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH = 8,
  LUNPA_FORTRESS_GROUND_FLOOR_NORTH_PASSAGE_FROM_MAIN_PASSAGE =
      9, ///< ★ Named from what the player sees: the hall at the top of the map
         ///< with a cell door, the stairs up and the way down to the cave (its
         ///< render)
  LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_NORTH = 10,
  LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_CAVE = 11,
  LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_FROM_MAIN_PASSAGE =
      12, ///< ★ Named from the game: "Whee! Thanks to Hammet we can enjoy food
          ///< and drink all day long!" (MsgLunpaHammetBuysDrink)
  LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_MAIN_PASSAGE =
      13, ///< ★ Named from what the player sees: the winding hall in the top
          ///< right of the map, with one guard (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_EAST_PASSAGE =
      14, ///< ★ Named from what the player sees: the winding hall in the top
          ///< right of the map, with one guard (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_BARREL_ROOM =
      15, ///< ★ Named from what the player sees: the winding hall in the top
          ///< right of the map, with one guard (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_MAIN_PASSAGE =
      16, ///< ★ Named from what the player sees: the hall on the right of the
          ///< map with a patrolling guard and a cell door (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_NORTHEAST_PASSAGE =
      17, ///< ★ Named from what the player sees: the hall on the right of the
          ///< map with a patrolling guard and a cell door (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_TREASURE_ROOM =
      18, ///< ★ Named from what the player sees: the hall on the right of the
          ///< map with a patrolling guard and a cell door (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_FROM_NORTHEAST_PASSAGE =
      19, ///< ★ Named from what the player sees: a small room of barrels where
          ///< a startled resident hides (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_FROM_EAST_PASSAGE =
      20, ///< ★ Named from what the player sees: the room of four chests (its
          ///< render)
};
/// SceneId_LunpaFortressGroundFloor's exits.
enum {
  LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_GATES = 1,
  LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_TO_MAIN_PASSAGE =
      2, ///< ★ Named from what the player sees: the long winding hall kept by
         ///< the entrance guard and two sentries (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_ENTRANCE =
      3, ///< ★ Named from what the player sees: the small room just inside the
         ///< fortress door (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_NORTH_PASSAGE =
      4, ///< ★ Named from what the player sees: the hall at the top of the map
         ///< with a cell door, the stairs up and the way down to the cave (its
         ///< render)
  LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_FEAST_ROOM =
      5, ///< ★ Named from the game: "Whee! Thanks to Hammet we can enjoy food
         ///< and drink all day long!" (MsgLunpaHammetBuysDrink)
  LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_NORTHEAST_PASSAGE =
      6, ///< ★ Named from what the player sees: the winding hall in the top
         ///< right of the map, with one guard (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_EAST_PASSAGE =
      7, ///< ★ Named from what the player sees: the hall on the right of the
         ///< map with a patrolling guard and a cell door (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH = 8,
  LUNPA_FORTRESS_GROUND_FLOOR_NORTH_PASSAGE_TO_MAIN_PASSAGE =
      9, ///< ★ Named from what the player sees: the long winding hall kept by
         ///< the entrance guard and two sentries (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_NORTH = 10,
  LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_CAVE = 11,
  LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_TO_MAIN_PASSAGE =
      12, ///< ★ Named from what the player sees: the long winding hall kept by
          ///< the entrance guard and two sentries (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_MAIN_PASSAGE =
      13, ///< ★ Named from what the player sees: the long winding hall kept by
          ///< the entrance guard and two sentries (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_EAST_PASSAGE =
      14, ///< ★ Named from what the player sees: the hall on the right of the
          ///< map with a patrolling guard and a cell door (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_BARREL_ROOM =
      15, ///< ★ Named from what the player sees: a small room of barrels where
          ///< a startled resident hides (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_MAIN_PASSAGE =
      16, ///< ★ Named from what the player sees: the long winding hall kept by
          ///< the entrance guard and two sentries (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_NORTHEAST_PASSAGE =
      17, ///< ★ Named from what the player sees: the winding hall in the top
          ///< right of the map, with one guard (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_TREASURE_ROOM =
      18, ///< ★ Named from what the player sees: the room of four chests (its
          ///< render)
  LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_TO_NORTHEAST_PASSAGE =
      19, ///< ★ Named from what the player sees: the winding hall in the top
          ///< right of the map, with one guard (its render)
  LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_TO_EAST_PASSAGE =
      20, ///< ★ Named from what the player sees: the hall on the right of the
          ///< map with a patrolling guard and a cell door (its render)
};

/// SceneId_LunpaFortressUpperFloor's entrances.
enum {
  LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH = 1,
  LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_NORTH_PASSAGE =
      2, ///< ★ Named from what the player sees: the tall hall on the left of
         ///< the map where two guards patrol (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_MIDDLE_PASSAGE =
      3, ///< ★ Named from what the player sees: the tall hall on the left of
         ///< the map where two guards patrol (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_WEST_PASSAGE =
      4, ///< ★ Named from what the player sees: the empty winding hall at the
         ///< top of the map (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_GUARD_ROOM =
      5, ///< ★ Named from what the player sees: the empty winding hall at the
         ///< top of the map (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_WOMENS_ROOM =
      6, ///< ★ Named from what the player sees: the empty winding hall at the
         ///< top of the map (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_WEST_PASSAGE =
      7, ///< ★ Named from what the player sees: the hall in the middle of the
         ///< map, one guard under three arches (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_FAMILY_ROOM =
      8, ///< ★ Named from what the player sees: the hall in the middle of the
         ///< map, one guard under three arches (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_CELL_KEY_ROOM =
      9, ///< ★ Named from what the player sees: the hall in the middle of the
         ///< map, one guard under three arches (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_MIDDLE_PASSAGE =
      10, ///< ★ Named from what the player sees: the bedroom of Dodonpa's wife
          ///< and children (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_PASSAGE =
      11, ///< ★ Named from what the player sees: the room where three guards
          ///< stand together and fight together (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_PASSAGE =
      12, ///< ★ Named from what the player sees: the room where three guards
          ///< stand together and fight together (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_PASSAGE =
      13, ///< ★ Named from the game: "Nearly all of these women were brought
          ///< here against their will" (MsgLunpaWomenWantHome)
  LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_MIDDLE_PASSAGE =
      14, ///< ★ Named from the game: "Cell Key" (MsgItemName)
  LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_PASSAGE =
      15, ///< ★ Named from the game: "Cell Key" (MsgItemName)
  LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH = 16,
  LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_GUARD_ROOM =
      17, ///< ★ Named from what the player sees: the winding hall at the bottom
          ///< of the map, with one guard (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_DONPAS_ROOM =
      18, ///< ★ Named from what the player sees: the winding hall at the bottom
          ///< of the map, with one guard (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_EAST_PASSAGE =
      19, ///< ★ Named from what the player sees: the winding hall at the bottom
          ///< of the map, with one guard (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_PASSAGE =
      20, ///< ★ Named from the game: "Who is disrupting my sleep???"
          ///< (MsgLunpaWhoDisruptingSleep)
  LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_CELL_KEY_ROOM =
      21, ///< ★ Named from what the player sees: the hall on the right of the
          ///< map with a patrolling guard (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_SOUTH_PASSAGE =
      22, ///< ★ Named from what the player sees: the hall on the right of the
          ///< map with a patrolling guard (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM =
      23, ///< ★ Named from the game: "Tonic" (MsgDjinnName)
  LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM =
      24, ///< ★ Named from the game: "Who is disrupting my sleep???"
          ///< (MsgLunpaWhoDisruptingSleep)
  LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_UNUSED =
      30, ///< no exit or event leads here
  /// Where the party comes back after the battle with the three guards in the
  /// guard room.
  LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE = 31,
};
/// SceneId_LunpaFortressUpperFloor's exits.
enum {
  LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH = 1,
  LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_TO_NORTH_PASSAGE =
      2, ///< ★ Named from what the player sees: the empty winding hall at the
         ///< top of the map (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_TO_MIDDLE_PASSAGE =
      3, ///< ★ Named from what the player sees: the hall in the middle of the
         ///< map, one guard under three arches (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_WEST_PASSAGE =
      4, ///< ★ Named from what the player sees: the tall hall on the left of
         ///< the map where two guards patrol (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_GUARD_ROOM =
      5, ///< ★ Named from what the player sees: the room where three guards
         ///< stand together and fight together (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_WOMENS_ROOM =
      6, ///< ★ Named from the game: "Nearly all of these women were brought
         ///< here against their will" (MsgLunpaWomenWantHome)
  LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_WEST_PASSAGE =
      7, ///< ★ Named from what the player sees: the tall hall on the left of
         ///< the map where two guards patrol (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_FAMILY_ROOM =
      8, ///< ★ Named from what the player sees: the bedroom of Dodonpa's wife
         ///< and children (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_CELL_KEY_ROOM =
      9, ///< ★ Named from the game: "Cell Key" (MsgItemName)
  LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_TO_MIDDLE_PASSAGE =
      10, ///< ★ Named from what the player sees: the hall in the middle of the
          ///< map, one guard under three arches (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_NORTH_PASSAGE =
      11, ///< ★ Named from what the player sees: the empty winding hall at the
          ///< top of the map (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_SOUTH_PASSAGE =
      12, ///< ★ Named from what the player sees: the winding hall at the bottom
          ///< of the map, with one guard (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_TO_NORTH_PASSAGE =
      13, ///< ★ Named from what the player sees: the empty winding hall at the
          ///< top of the map (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_MIDDLE_PASSAGE =
      14, ///< ★ Named from what the player sees: the hall in the middle of the
          ///< map, one guard under three arches (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_EAST_PASSAGE =
      15, ///< ★ Named from what the player sees: the hall on the right of the
          ///< map with a patrolling guard (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_NORTH = 16,
  LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_GUARD_ROOM =
      17, ///< ★ Named from what the player sees: the room where three guards
          ///< stand together and fight together (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_DONPAS_ROOM =
      18, ///< ★ Named from the game: "Who is disrupting my sleep???"
          ///< (MsgLunpaWhoDisruptingSleep)
  LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_EAST_PASSAGE =
      19, ///< ★ Named from what the player sees: the hall on the right of the
          ///< map with a patrolling guard (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_SOUTH_PASSAGE =
      20, ///< ★ Named from what the player sees: the winding hall at the bottom
          ///< of the map, with one guard (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_TO_CELL_KEY_ROOM =
      21, ///< ★ Named from the game: "Cell Key" (MsgItemName)
  LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_TO_SOUTH_PASSAGE =
      22, ///< ★ Named from what the player sees: the winding hall at the bottom
          ///< of the map, with one guard (its render)
  LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_TO_DONPAS_ROOM =
      23, ///< ★ Named from the game: "Who is disrupting my sleep???"
          ///< (MsgLunpaWhoDisruptingSleep)
  LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_TONIC_ROOM =
      24, ///< ★ Named from the game: "Tonic" (MsgDjinnName)
};

/// SceneId_LunpaFortressCave's entrances.
enum {
  LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_GROUND_FLOOR = 1,
  LUNPA_FORTRESS_CAVE_ENTRANCE_FROM_LAKE =
      2, ///< ★ Named from what the player sees: the small cave at the foot of
         ///< the stairs from the fortress (its render)
  LUNPA_FORTRESS_CAVE_LAKE_FROM_ENTRANCE =
      3, ///< ★ Named from what the player sees: the big cave round an
         ///< underground pool (its render)
  LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHEAST = 4,
  LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTH = 5,
  LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHWEST = 6,
};
/// SceneId_LunpaFortressCave's exits.
enum {
  LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_GROUND_FLOOR = 1,
  LUNPA_FORTRESS_CAVE_ENTRANCE_TO_LAKE =
      2, ///< ★ Named from what the player sees: the big cave round an
         ///< underground pool (its render)
  LUNPA_FORTRESS_CAVE_LAKE_TO_ENTRANCE =
      3, ///< ★ Named from what the player sees: the small cave at the foot of
         ///< the stairs from the fortress (its render)
  LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHEAST = 4,
  LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTH = 5,
  LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHWEST = 6,
};

/// SceneId_LunpaFortressToadonpa's entrances.
enum {
  LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTH = 1,
  LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHEAST = 2,
  LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST = 3,
  /// Where the party comes back after the battle with Toadonpa.
  LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE = 4,
};
/// SceneId_LunpaFortressToadonpa's exits.
enum {
  LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTH = 1,
  LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHEAST = 2,
  LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_GATES = 4,
  LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHWEST = 5,
};

/// SceneId_SuhallaDesertNorth's entrances.
enum {
  SUHALLA_DESERT_NORTH_FROM_WORLD_MAP = 1,
  SUHALLA_DESERT_NORTH_FROM_SUHALLA_DESERT_SOUTH = 2,
};
/// SceneId_SuhallaDesertNorth's exits.
enum {
  SUHALLA_DESERT_NORTH_TO_WORLD_MAP = 1,
  SUHALLA_DESERT_NORTH_TO_SUHALLA_DESERT_SOUTH = 2,
};

/// SceneId_SuhallaDesertSouth's entrances.
enum {
  SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_NORTH = 1,
  SUHALLA_DESERT_SOUTH_FROM_WORLD_MAP = 2,
  SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_EAST = 3,
  SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_WEST = 4,
};
/// SceneId_SuhallaDesertSouth's exits.
enum {
  SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_NORTH = 1,
  SUHALLA_DESERT_SOUTH_TO_WORLD_MAP = 2,
  SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_EAST = 3,
  SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_WEST = 4,
};

/// SceneId_SuhallaDesertCaves's entrances.
enum {
  SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_WEST = 1,
  SUHALLA_DESERT_CAVES_WEST_FROM_EAST =
      2, ///< ★ Named from what the player sees: the long winding cave on the
         ///< left, under the desert's western cave mouth (its render)
  SUHALLA_DESERT_CAVES_EAST_FROM_WEST =
      3, ///< ★ Named from what the player sees: the short cave on the right,
         ///< under the desert's eastern cave mouth (its render)
  SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_EAST = 4,
};
/// SceneId_SuhallaDesertCaves's exits.
enum {
  SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST =
      1, ///< ★ Named from what the player sees: the long winding cave on the
         ///< left, under the desert's western cave mouth (its render)
  SUHALLA_DESERT_CAVES_WEST_TO_EAST =
      2, ///< ★ Named from what the player sees: the short cave on the right,
         ///< under the desert's eastern cave mouth (its render)
  SUHALLA_DESERT_CAVES_EAST_TO_WEST =
      3, ///< ★ Named from what the player sees: the long winding cave on the
         ///< left, under the desert's western cave mouth (its render)
  SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST =
      4, ///< ★ Named from what the player sees: the short cave on the right,
         ///< under the desert's eastern cave mouth (its render)
};

/// SceneId_Suhalla's entrances.
enum {
  SUHALLA_FROM_INN = 1,
  SUHALLA_FROM_SANCTUM = 2,
  SUHALLA_FROM_HOUSE_WEST = 3,
  SUHALLA_FROM_HOUSE_NORTHEAST = 4,
  SUHALLA_FROM_HOUSE_EAST = 5,
  /// In the table at (120, 272), facing east, but led to by no exit.
  SUHALLA_SIXTH_UNUSED = 6,
  SUHALLA_FROM_WORLD_MAP = 10,
  /// In no table row and led to by no exit: arriving by it sets
  /// FLAG_BABI_SPOKE_AFTER_COLOSSO, marking Babi's audience as held. XXX:
  /// likely a debug start into the story after the audience, because Babi's
  /// Palace (BABI_PALACE_AFTER_COLOSSO) and Suhalla's buildings
  /// (SUHALLA_INTERIORS_AFTER_BABI_SPOKE) have entrances that set story
  /// flags the same way.
  SUHALLA_AFTER_BABI_SPOKE = 90,
};
/// SceneId_Suhalla's exits.
enum {
  SUHALLA_TO_INN =
      1, ///< ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
  SUHALLA_TO_SANCTUM = 2,
  SUHALLA_TO_HOUSE_WEST = 3, ///< ★ Named from what the player sees: a house
                             ///< with beds, a table and a hearth (its render)
  SUHALLA_TO_HOUSE_NORTHEAST = 4,
  SUHALLA_TO_HOUSE_EAST = 5,
  SUHALLA_TO_WORLD_MAP = 10,
  SUHALLA_TO_WORLD_MAP_NORTH = 11,
  SUHALLA_TO_WORLD_MAP_EAST = 12,
};

/// SceneId_SuhallaInteriors's entrances.
enum {
  SUHALLA_INTERIORS_INN = 1,
  SUHALLA_INTERIORS_SANCTUM =
      2, ///< the room behind Suhalla's sanctum door;
         ///< the room holds the sanctum; through SUHALLA_TO_SANCTUM
  SUHALLA_INTERIORS_HOUSE_WEST = 3, ///< the room behind Suhalla's house west
                                    ///< door; through SUHALLA_TO_HOUSE_WEST
  SUHALLA_INTERIORS_HOUSE_NORTHEAST =
      4, ///< the room behind Suhalla's house northeast door; through
         ///< SUHALLA_TO_HOUSE_NORTHEAST
  SUHALLA_INTERIORS_HOUSE_EAST = 5, ///< the room behind Suhalla's house east
                                    ///< door; through SUHALLA_TO_HOUSE_EAST
  SUHALLA_INTERIORS_SANCTUM_NORTH =
      6, ///< before the priest in Suhalla Sanctum, where a party defeated in
         ///< Suhalla wakes
  SUHALLA_INTERIORS_HOUSE_WEST_UNUSED = 7, ///< no exit leads here
  SUHALLA_INTERIORS_INN_FROM_GUEST_ROOM_WEST =
      10, ///< ★ Named from what the player sees: Suhalla's inn and its counter
          ///< (its render)
  SUHALLA_INTERIORS_INN_FROM_GUEST_ROOM_EAST =
      11, ///< ★ Named from what the player sees: Suhalla's inn and its counter
          ///< (its render)
  SUHALLA_INTERIORS_GUEST_ROOM_WEST_FROM_INN =
      12, ///< ★ Named from what the player sees: the inn's guest room with one
          ///< bed, behind its western doorway (its render)
  SUHALLA_INTERIORS_GUEST_ROOM_EAST_FROM_INN =
      13, ///< ★ Named from what the player sees: the inn's guest room with two
          ///< beds, behind its eastern doorway (its render)
  SUHALLA_INTERIORS_INN_FROM_LOFT =
      14, ///< ★ Named from what the player sees: Suhalla's inn and its counter
          ///< (its render)
  SUHALLA_INTERIORS_LOFT_FROM_INN =
      15, ///< ★ Named from what the player sees: the room with beds up the
          ///< inn's ladder (its render)
  /// In no table row and led to by no exit: arriving by it also marks Babi's
  /// audience as held, as SUHALLA_AFTER_BABI_SPOKE does. XXX: likely a
  /// debug start into the story after the audience.
  SUHALLA_INTERIORS_AFTER_BABI_SPOKE = 90,
};
/// SceneId_SuhallaInteriors's exits.
enum {
  SUHALLA_INTERIORS_INN_TO_SUHALLA = 1,
  SUHALLA_INTERIORS_SANCTUM_TO_SUHALLA = 2,
  SUHALLA_INTERIORS_HOUSE_WEST_TO_SUHALLA = 3,
  SUHALLA_INTERIORS_HOUSE_NORTHEAST_TO_SUHALLA = 4,
  SUHALLA_INTERIORS_HOUSE_EAST_TO_SUHALLA = 5,
  SUHALLA_INTERIORS_INN_TO_GUEST_ROOM_WEST =
      10, ///< ★ Named from what the player sees: the inn's guest room with one
          ///< bed, behind its western doorway (its render)
  SUHALLA_INTERIORS_INN_TO_GUEST_ROOM_EAST =
      11, ///< ★ Named from what the player sees: the inn's guest room with two
          ///< beds, behind its eastern doorway (its render)
  SUHALLA_INTERIORS_GUEST_ROOM_WEST_TO_INN =
      12, ///< ★ Named from what the player sees: Suhalla's inn and its counter
          ///< (its render)
  SUHALLA_INTERIORS_GUEST_ROOM_EAST_TO_INN =
      13, ///< ★ Named from what the player sees: Suhalla's inn and its counter
          ///< (its render)
  SUHALLA_INTERIORS_INN_TO_LOFT =
      14, ///< ★ Named from what the player sees: the room with beds up the
          ///< inn's ladder (its render)
  SUHALLA_INTERIORS_LOFT_TO_INN =
      15, ///< ★ Named from what the player sees: Suhalla's inn and its counter
          ///< (its render)
};

/// SceneId_SuhallaGateCheckpoint's entrances.
enum {
  SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH = 1,
  SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_WEST = 2,
  SUHALLA_GATE_CHECKPOINT_FROM_LUNPA_SUHALLA = 3,
  SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_EAST = 4,
  SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST = 5,
};
/// SceneId_SuhallaGateCheckpoint's exits.
enum {
  SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_SOUTH = 1,
  SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_WEST = 2,
  SUHALLA_GATE_CHECKPOINT_TO_LUNPA_SUHALLA = 3,
  SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_EAST = 4,
  SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_EAST = 5,
};

/// SceneId_SuhallaGateDew's entrances.
enum {
  SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_WEST = 1,
  SUHALLA_GATE_DEW_WEST_FROM_EAST =
      2, ///< ★ Named from what the player sees: the cliffs reached by the
         ///< gate's western way, with a chest (its render)
  SUHALLA_GATE_DEW_EAST_FROM_WEST =
      3, ///< ★ Named from what the player sees: the cliffs reached by the
         ///< gate's eastern way, where Dew waits by the cave mouth (its render)
  SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_EAST = 4,
  SUHALLA_GATE_DEW_FROM_GATEWAY_CAVE = 5,
};
/// SceneId_SuhallaGateDew's exits.
enum {
  SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_WEST =
      1, ///< ★ Named from what the player sees: the cliffs reached by the
         ///< gate's western way, with a chest (its render)
  SUHALLA_GATE_DEW_WEST_TO_EAST =
      2, ///< ★ Named from what the player sees: the cliffs reached by the
         ///< gate's eastern way, where Dew waits by the cave mouth (its render)
  SUHALLA_GATE_DEW_EAST_TO_WEST =
      3, ///< ★ Named from what the player sees: the cliffs reached by the
         ///< gate's western way, with a chest (its render)
  SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_EAST =
      4, ///< ★ Named from what the player sees: the cliffs reached by the
         ///< gate's eastern way, where Dew waits by the cave mouth (its render)
  SUHALLA_GATE_DEW_TO_GATEWAY_CAVE = 5,
};

/// SceneId_GatewayCave's entrances.
enum {
  GATEWAY_CAVE_FROM_SUHALLA_GATE_DEW = 1,
  GATEWAY_CAVE_NORTH_FROM_SOUTH =
      2, ///< ★ Named from what the player sees: the small cave entered from the
         ///< cliffs (its render)
  GATEWAY_CAVE_SOUTH_FROM_NORTH =
      3, ///< ★ Named from what the player sees: the larger cave of pools and
         ///< purple crystals that leads out (its render)
  GATEWAY_CAVE_FROM_WORLD_MAP = 4,
};
/// SceneId_GatewayCave's exits.
enum {
  GATEWAY_CAVE_TO_SUHALLA_GATE_DEW = 1,
  GATEWAY_CAVE_NORTH_TO_SOUTH =
      2, ///< ★ Named from what the player sees: the larger cave of pools and
         ///< purple crystals that leads out (its render)
  GATEWAY_CAVE_SOUTH_TO_NORTH =
      3, ///< ★ Named from what the player sees: the small cave entered from the
         ///< cliffs (its render)
  GATEWAY_CAVE_TO_WORLD_MAP = 4,
};

/// SceneId_TunnelRuinsGreatStatue's entrances.
enum {
  TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR = 1,
  TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS =
      2, ///< ★ Named from the game: "The statue seems to speak to your soul..."
         ///< (MsgTunnelRuinsStatueSeemsSpeakYourSoul)
  TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE =
      3, ///< ★ Named from what the player sees: the first room on the way with
         ///< a pair of stone pillars to push (its render)
  TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH =
      4, ///< ★ Named from what the player sees: the first room on the way with
         ///< a pair of stone pillars to push (its render)
  TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS =
      5, ///< ★ Named from what the player sees: the upper room of columns
         ///< standing in a pit, hopped across (its render)
  TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS =
      6, ///< ★ Named from what the player sees: the upper room of columns
         ///< standing in a pit, hopped across (its render)
  TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS =
      7, ///< ★ Named from what the player sees: the upper room of columns
         ///< standing in a pit, hopped across (its render)
  TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH =
      8, ///< ★ Named from what the player sees: the second room with a pair of
         ///< stone pillars to push (its render)
  TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM =
      9, ///< ★ Named from what the player sees: the second room with a pair of
         ///< stone pillars to push (its render)
  TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH =
      10, ///< ★ Named from what the player sees: two stone pillars that sink
          ///< into the floor once moved onto their columns (its render)
  TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH =
      11, ///< ★ Named from what the player sees: two stone pillars that sink
          ///< into the floor once moved onto their columns (its render)
  TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS =
      12, ///< ★ Named from what the player sees: the room of bushes that
          ///< Whirlwind blows away (its render)
  TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE =
      13, ///< ★ Named from what the player sees: the room of bushes that
          ///< Whirlwind blows away (its render)
  TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_FROM_WHIRLWIND_ROOM =
      14, ///< ★ Named from what the player sees: a long narrow corridor (its
          ///< render)
  TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH = 15,
  TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_FROM_SINKING_PILLARS =
      16, ///< ★ Named from what the player sees: the lower room of columns
          ///< standing in a pit (its render)
  TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST = 17,
  TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST = 18,
};
/// SceneId_TunnelRuinsGreatStatue's exits.
enum {
  TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR = 1,
  TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS =
      2, ///< ★ Named from what the player sees: the first room on the way with
         ///< a pair of stone pillars to push (its render)
  TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE =
      3, ///< ★ Named from the game: "The statue seems to speak to your soul..."
         ///< (MsgTunnelRuinsStatueSeemsSpeakYourSoul)
  TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH =
      4, ///< ★ Named from what the player sees: the upper room of columns
         ///< standing in a pit, hopped across (its render)
  TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS =
      5, ///< ★ Named from what the player sees: the first room on the way with
         ///< a pair of stone pillars to push (its render)
  TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS =
      6, ///< ★ Named from what the player sees: the second room with a pair of
         ///< stone pillars to push (its render)
  TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS =
      7, ///< ★ Named from what the player sees: two stone pillars that sink
         ///< into the floor once moved onto their columns (its render)
  TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH =
      8, ///< ★ Named from what the player sees: the upper room of columns
         ///< standing in a pit, hopped across (its render)
  TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM =
      9, ///< ★ Named from what the player sees: the room of bushes that
         ///< Whirlwind blows away (its render)
  TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH =
      10, ///< ★ Named from what the player sees: the upper room of columns
          ///< standing in a pit, hopped across (its render)
  TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH =
      11, ///< ★ Named from what the player sees: the lower room of columns
          ///< standing in a pit (its render)
  TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS =
      12, ///< ★ Named from what the player sees: the second room with a pair of
          ///< stone pillars to push (its render)
  TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE =
      13, ///< ★ Named from what the player sees: a long narrow corridor (its
          ///< render)
  TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM =
      14, ///< ★ Named from what the player sees: the room of bushes that
          ///< Whirlwind blows away (its render)
  TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH = 15,
  TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS =
      16, ///< ★ Named from what the player sees: two stone pillars that sink
          ///< into the floor once moved onto their columns (its render)
  TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST = 17,
  TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST = 18,
};

/// SceneId_TunnelRuinsLadyStatues's entrances.
enum {
  TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST = 1,
  TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH = 2,
  TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH = 3,
  TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH = 4,
  TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH = 5,
  TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST = 6,
  TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH = 7,
  TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST = 8,
  TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHWEST = 9,
  TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHEAST = 10,
  TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHEAST = 11,
  TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST = 12,
  TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH = 13,
  TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST = 14,
  TUNNEL_RUINS_LADY_STATUES_TREE_DOOR_SOUTH = 15,
  TUNNEL_RUINS_LADY_STATUES_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR = 16,
  TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST = 17,
};
/// SceneId_TunnelRuinsLadyStatues's exits.
enum {
  TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH = 1,
  TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST =
      2, ///< ★ Named from what the player sees: four raised walkways over a pit
         ///< (its render)
  TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST = 3,
  TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST =
      4, ///< ★ Named from what the player sees: four raised walkways over a pit
         ///< (its render)
  TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH =
      5, ///< ★ Named from what the player sees: two platforms that sink and
         ///< rise like a balance under what Carry sets on them (its render)
  TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH =
      6, ///< ★ Named from what the player sees: the room of a stone pillar that
         ///< Douse soaks (its render)
  TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST =
      7, ///< ★ Named from what the player sees: the western room of a statue
         ///< that vanishes under Reveal (its render)
  TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH =
      8, ///< ★ Named from what the player sees: the room of five coloured
         ///< statues of a woman before a door (its render)
  TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST =
      9, ///< ★ Named from what the player sees: four raised walkways over a pit
         ///< (its render)
  TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST =
      10, ///< ★ Named from what the player sees: four raised walkways over a
          ///< pit (its render)
  TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH =
      11, ///< ★ Named from what the player sees: the great door with a tree
          ///< carved over it, the way on to Venus Lighthouse (its render)
  TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST = 12,
  TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST =
      13, ///< ★ Named from what the player sees: the room of five coloured
          ///< statues of a woman before a door (its render)
  TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR = 14,
  TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_WEST = 15,
  TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_SOUTH = 16,
  TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_NORTH = 17,
};

/// SceneId_BabiLighthouseStoneDoor's entrances.
enum {
  BABI_LIGHTHOUSE_STONE_DOOR_FROM_TUNNEL_RUINS_GREAT_STATUE = 1,
  BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_WEST = 2,
  BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_EAST = 3,
};
/// SceneId_BabiLighthouseStoneDoor's exits.
enum {
  BABI_LIGHTHOUSE_STONE_DOOR_TO_TUNNEL_RUINS_GREAT_STATUE = 1,
  BABI_LIGHTHOUSE_STONE_DOOR_TO_BABI_LIGHTHOUSE_LUFF_WEST = 2,
};

/// SceneId_BabiLighthouseLuff's entrances.
enum {
  BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_WEST_STEPS =
      1, ///< ★ Named from what the player sees: the tall room of two arches and
         ///< a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_EAST_STEPS =
      2, ///< ★ Named from what the player sees: the tall room of two arches and
         ///< a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_LOCKED_DOOR_ROOM =
      3, ///< ★ Named from what the player sees: the tall room of two arches and
         ///< a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_STEPS =
      4, ///< ★ Named from what the player sees: the brick room entered by the
         ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_SOUTH_ROOM_WEST_STEPS =
      5, ///< ★ Named from what the player sees: the brick room entered by the
         ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_SOUTH_ROOM_EAST_STEPS =
      6, ///< ★ Named from what the player sees: the brick room entered by the
         ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_ENTRY_EAST = 7,
  BABI_LIGHTHOUSE_LUFF_WEST_ROOM_FROM_EAST_ROOM_STEPS =
      8, ///< ★ Named from what the player sees: the brick room entered by the
         ///< Entry's west door, with three stones in its pits (its render)
  BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_ENTRY_WEST = 9,
  BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_STONE_DOOR_WEST = 10,
  BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_FROM_SOUTH_ROOM =
      11, ///< ★ Named from what the player sees: the small room with a locked
          ///< door and the arch on to the stone door (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_EAST =
      12, ///< ★ Named from what the player sees: the brick room entered by the
          ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_PIT =
      13, ///< ★ Named from what the player sees: the tall room of two arches
          ///< and a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_SOUTHWEST =
      14, ///< ★ Named from what the player sees: the brick room entered by the
          ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_SOUTH =
      15, ///< ★ Named from what the player sees: the tall room of two arches
          ///< and a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_NORTHWEST =
      16, ///< ★ Named from what the player sees: the brick room entered by the
          ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_NORTH =
      17, ///< ★ Named from what the player sees: the tall room of two arches
          ///< and a narrow passage south (its render)
};
/// SceneId_BabiLighthouseLuff's exits.
enum {
  BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_STEPS =
      1, ///< ★ Named from what the player sees: the brick room entered by the
         ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_EAST = 2,
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_WEST_ROOM_STEPS =
      3, ///< ★ Named from what the player sees: the brick room entered by the
         ///< Entry's west door, with three stones in its pits (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_WEST_STEPS =
      4, ///< ★ Named from what the player sees: the tall room of two arches and
         ///< a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_EAST_STEPS =
      5, ///< ★ Named from what the player sees: the tall room of two arches and
         ///< a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_WEST = 6,
  BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_WEST_STEPS =
      7, ///< ★ Named from what the player sees: the brick room entered by the
         ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_EAST_STEPS =
      8, ///< ★ Named from what the player sees: the brick room entered by the
         ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_LOCKED_DOOR_ROOM =
      9, ///< ★ Named from what the player sees: the small room with a locked
         ///< door and the arch on to the stone door (its render)
  BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_TO_SOUTH_ROOM =
      10, ///< ★ Named from what the player sees: the tall room of two arches
          ///< and a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_EAST =
      11, ///< ★ Named from what the player sees: the brick room entered by the
          ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_PIT =
      12, ///< ★ Named from what the player sees: the tall room of two arches
          ///< and a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_SOUTHWEST =
      13, ///< ★ Named from what the player sees: the brick room entered by the
          ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_SOUTH =
      14, ///< ★ Named from what the player sees: the tall room of two arches
          ///< and a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_NORTHWEST =
      15, ///< ★ Named from what the player sees: the brick room entered by the
          ///< Entry's east door, where Luff waits on a ledge (its render)
  BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_NORTH =
      16, ///< ★ Named from what the player sees: the tall room of two arches
          ///< and a narrow passage south (its render)
  BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_EAST = 17,
  BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_WEST = 18,
};

/// SceneId_BabiLighthouseEntry's entrances.
enum {
  BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE = 1,
  BABI_LIGHTHOUSE_ENTRY_FROM_BABI_LIGHTHOUSE_LUFF_WEST = 2,
  BABI_LIGHTHOUSE_ENTRY_FROM_BABI_LIGHTHOUSE_LUFF_EAST = 3,
  BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_WEST = 4,
  BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTH = 5,
  BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH = 6,
};
/// SceneId_BabiLighthouseEntry's exits.
enum {
  BABI_LIGHTHOUSE_ENTRY_TO_ROAD_TO_BABI_LIGHTHOUSE = 1,
  BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_EAST = 2,
  BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_WEST = 3,
  BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_WEST = 4,
  BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_SOUTH = 5,
  BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_NORTH = 6,
};

/// SceneId_RoadToBabiLighthouse's entrances.
enum {
  ROAD_TO_BABI_LIGHTHOUSE_FROM_WORLD_MAP = 1,
  ROAD_TO_BABI_LIGHTHOUSE_FROM_BABI_LIGHTHOUSE_ENTRY = 2,
};
/// SceneId_RoadToBabiLighthouse's exits.
enum {
  ROAD_TO_BABI_LIGHTHOUSE_TO_WORLD_MAP = 1,
  ROAD_TO_BABI_LIGHTHOUSE_TO_BABI_LIGHTHOUSE_ENTRY = 2,
};

/// SceneId_Lalivero's entrances.
enum {
  LALIVERO_FROM_LALIVERO_FARANS_HOUSE_NORTHWEST = 1,
  LALIVERO_FROM_LALIVERO_SHOPS_WEST = 2,
  LALIVERO_FROM_LALIVERO_FARANS_HOUSE_NORTHEAST = 3,
  LALIVERO_FROM_LALIVERO_SHOPS_NORTHWEST = 4,
  LALIVERO_FROM_LALIVERO_SHOPS_NORTH = 5,
  LALIVERO_FROM_LALIVERO_SHOPS_NORTHEAST = 6,
  LALIVERO_FROM_LALIVERO_FARANS_HOUSE_SOUTHWEST = 7,
  LALIVERO_FROM_LALIVERO_FARANS_HOUSE_SOUTH = 8,
  LALIVERO_FROM_LALIVERO_SHOPS_SOUTH = 9,
  LALIVERO_FROM_LALIVERO_SHOPS_SOUTHEAST = 10,
  LALIVERO_FROM_LALIVERO_SHOPS_EAST_SOUTHEAST = 11,
  LALIVERO_FROM_WORLD_MAP_EAST = 20,
  LALIVERO_FROM_WORLD_MAP_WEST = 21,
};
/// SceneId_Lalivero's exits.
enum {
  LALIVERO_TO_LALIVERO_FARANS_HOUSE_NORTHWEST = 1,
  LALIVERO_TO_LALIVERO_SHOPS_WEST = 2,
  LALIVERO_TO_LALIVERO_FARANS_HOUSE_NORTHEAST = 3,
  LALIVERO_TO_LALIVERO_SHOPS_NORTHWEST = 4,
  LALIVERO_TO_LALIVERO_SHOPS_NORTH = 5,
  LALIVERO_TO_LALIVERO_SHOPS_NORTHEAST = 6,
  LALIVERO_TO_LALIVERO_FARANS_HOUSE_SOUTHWEST = 7,
  LALIVERO_TO_LALIVERO_FARANS_HOUSE_SOUTH = 8,
  LALIVERO_TO_LALIVERO_SHOPS_SOUTH = 9,
  LALIVERO_TO_LALIVERO_SHOPS_SOUTHEAST = 10,
  LALIVERO_TO_LALIVERO_SHOPS_EAST_SOUTHEAST = 11,
  LALIVERO_TO_WORLD_MAP_EAST = 20,
  LALIVERO_TO_WORLD_MAP_WEST = 21,
};

/// SceneId_LaliveroShops's entrances.
enum {
  LALIVERO_SHOPS_FROM_LALIVERO_WEST = 1,
  LALIVERO_SHOPS_FROM_LALIVERO_SOUTHEAST = 2,
  LALIVERO_SHOPS_ITEM_SHOP_UPSTAIRS_FROM_ITEM_SHOP =
      3, ///< ★ Named from what the player sees: the room up the item shop's
         ///< stairs (its render)
  LALIVERO_SHOPS_FROM_LALIVERO_NORTH = 4,
  LALIVERO_SHOPS_ITEM_SHOP_FROM_ITEM_SHOP_UPSTAIRS =
      5, ///< ★ Named from what the player sees: the room behind Lalivero's
         ///< north door where a merchant sells items across a counter (its
         ///< render)
  LALIVERO_SHOPS_FROM_LALIVERO_NORTHWEST = 6,
  LALIVERO_SHOPS_NORTHWEST_HOUSE_FROM_NORTHWEST_HOUSE_UPSTAIRS =
      7, ///< ★ Named from what the player sees: the house behind Lalivero's
         ///< northwest door (its render)
  LALIVERO_SHOPS_NORTHWEST_HOUSE_UPSTAIRS_FROM_NORTHWEST_HOUSE =
      8, ///< ★ Named from what the player sees: the room up that house's
         ///< stairs, where a worker naps (its render)
  LALIVERO_SHOPS_FROM_LALIVERO_SOUTH = 9,
  LALIVERO_SHOPS_FROM_LALIVERO_EAST_SOUTHEAST = 10,
  LALIVERO_SHOPS_FROM_LALIVERO_NORTHEAST = 11,
  LALIVERO_SHOPS_SANCTUM =
      12, ///< stepping in makes the sanctum where a defeated party wakes
};
/// SceneId_LaliveroShops's exits.
enum {
  LALIVERO_SHOPS_TO_LALIVERO_WEST = 1,
  LALIVERO_SHOPS_TO_LALIVERO_SOUTHEAST = 2,
  LALIVERO_SHOPS_ITEM_SHOP_UPSTAIRS_TO_ITEM_SHOP =
      3, ///< ★ Named from what the player sees: the room behind Lalivero's
         ///< north door where a merchant sells items across a counter (its
         ///< render)
  LALIVERO_SHOPS_TO_LALIVERO_NORTH = 4,
  LALIVERO_SHOPS_ITEM_SHOP_TO_ITEM_SHOP_UPSTAIRS =
      5, ///< ★ Named from what the player sees: the room up the item shop's
         ///< stairs (its render)
  LALIVERO_SHOPS_TO_LALIVERO_NORTHWEST = 6,
  LALIVERO_SHOPS_NORTHWEST_HOUSE_TO_NORTHWEST_HOUSE_UPSTAIRS =
      7, ///< ★ Named from what the player sees: the room up that house's
         ///< stairs, where a worker naps (its render)
  LALIVERO_SHOPS_NORTHWEST_HOUSE_UPSTAIRS_TO_NORTHWEST_HOUSE =
      8, ///< ★ Named from what the player sees: the house behind Lalivero's
         ///< northwest door (its render)
  LALIVERO_SHOPS_TO_LALIVERO_SOUTH = 9,
  LALIVERO_SHOPS_TO_LALIVERO_EAST_SOUTHEAST = 10,
  LALIVERO_SHOPS_TO_LALIVERO_NORTHEAST = 11,
};

/// SceneId_LaliveroFaransHouse's entrances.
enum {
  LALIVERO_FARANS_HOUSE_FROM_LALIVERO_SOUTHWEST = 21,
  LALIVERO_FARANS_HOUSE_FROM_LALIVERO_SOUTH = 22,
  LALIVERO_FARANS_HOUSE_FROM_LALIVERO_NORTHEAST = 23,
  LALIVERO_FARANS_HOUSE_INN_FROM_INN_UPSTAIRS =
      24, ///< ★ Named from what the player sees: the room where an innkeeper
          ///< offers a room for the night across a counter (its render)
  LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_FROM_INN =
      25, ///< ★ Named from what the player sees: the hall at the top of the
          ///< inn's stairs, with two bedroom doors (its render)
  LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_FROM_GUEST_ROOM_WEST =
      26, ///< ★ Named from what the player sees: the hall at the top of the
          ///< inn's stairs, with two bedroom doors (its render)
  LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_FROM_GUEST_ROOM_EAST =
      27, ///< ★ Named from what the player sees: the hall at the top of the
          ///< inn's stairs, with two bedroom doors (its render)
  LALIVERO_FARANS_HOUSE_GUEST_ROOM_WEST_FROM_INN_UPSTAIRS =
      28, ///< ★ Named from what the player sees: a bedroom off the inn's
          ///< upstairs hall (its render)
  LALIVERO_FARANS_HOUSE_GUEST_ROOM_EAST_FROM_INN_UPSTAIRS =
      29, ///< ★ Named from what the player sees: a bedroom off the inn's
          ///< upstairs hall (its render)
  LALIVERO_FARANS_HOUSE_FROM_LALIVERO_NORTHWEST = 30,
  LALIVERO_FARANS_HOUSE_FROM_LEMURIAN_SHIP = 31,
  LALIVERO_FARANS_HOUSE_FROM_VENUS_LIGHTHOUSE_AERIE =
      99, ///< back from Venus Lighthouse, the party is led straight to Faran
};
/// SceneId_LaliveroFaransHouse's exits.
enum {
  LALIVERO_FARANS_HOUSE_TO_LALIVERO_SOUTHWEST = 21,
  LALIVERO_FARANS_HOUSE_TO_LALIVERO_SOUTH = 22,
  LALIVERO_FARANS_HOUSE_TO_LALIVERO_NORTHEAST = 23,
  LALIVERO_FARANS_HOUSE_INN_TO_INN_UPSTAIRS =
      24, ///< ★ Named from what the player sees: the hall at the top of the
          ///< inn's stairs, with two bedroom doors (its render)
  LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_TO_INN =
      25, ///< ★ Named from what the player sees: the room where an innkeeper
          ///< offers a room for the night across a counter (its render)
  LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_TO_GUEST_ROOM_WEST =
      26, ///< ★ Named from what the player sees: a bedroom off the inn's
          ///< upstairs hall (its render)
  LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_TO_GUEST_ROOM_EAST =
      27, ///< ★ Named from what the player sees: a bedroom off the inn's
          ///< upstairs hall (its render)
  LALIVERO_FARANS_HOUSE_GUEST_ROOM_WEST_TO_INN_UPSTAIRS =
      28, ///< ★ Named from what the player sees: the hall at the top of the
          ///< inn's stairs, with two bedroom doors (its render)
  LALIVERO_FARANS_HOUSE_GUEST_ROOM_EAST_TO_INN_UPSTAIRS =
      29, ///< ★ Named from what the player sees: the hall at the top of the
          ///< inn's stairs, with two bedroom doors (its render)
  LALIVERO_FARANS_HOUSE_TO_LALIVERO_NORTHWEST = 30,
  LALIVERO_FARANS_HOUSE_TO_LEMURIAN_SHIP = 31,
};

/// SceneId_VenusLighthouseEntry's entrances.
enum {
  VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_WEST = 1,
  VENUS_LIGHTHOUSE_ENTRY_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR = 2,
  VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_EAST = 3,
};
/// SceneId_VenusLighthouseEntry's exits.
enum {
  VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST = 1,
  VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR = 2,
  VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST = 3,
};

/// SceneId_VenusLighthouseGroundFloor's entrances.
enum {
  VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_ENTRY = 1,
  VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_FROM_RELIEFS =
      2, ///< ★ Named from what the player sees: the hall of four statues behind
         ///< the front door (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_FRONT_ROOM =
      3, ///< ★ Named from what the player sees: the room of golden reliefs and
         ///< a tree emblem on the floor (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_EAST =
      4, ///< ★ Named from what the player sees: the room of golden reliefs and
         ///< a tree emblem on the floor (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_WEST =
      5, ///< ★ Named from what the player sees: the room of golden reliefs and
         ///< a tree emblem on the floor (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_PILLARS =
      6, ///< ★ Named from what the player sees: the room of golden reliefs and
         ///< a tree emblem on the floor (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_RELIEFS =
      7, ///< ★ Named from what the player sees: the room of spiral-marked stone
         ///< pillars (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_CARRY_STONE =
      8, ///< ★ Named from what the player sees: the room of spiral-marked stone
         ///< pillars (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_FLOOR_SWITCH =
      9, ///< ★ Named from what the player sees: the room of spiral-marked stone
         ///< pillars (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_NORTH =
      10, ///< ★ Named from what the player sees: the room of spiral-marked
          ///< stone pillars (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_WEST =
      11, ///< ★ Named from what the player sees: the room of spiral-marked
          ///< stone pillars (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_PILLARS_NORTH =
      12, ///< ★ Named from what the player sees: the room with five statues in
          ///< a row (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_PILLARS_WEST =
      13, ///< ★ Named from what the player sees: the room with five statues in
          ///< a row (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_SMALL_ROOM =
      14, ///< ★ Named from what the player sees: the room with five statues in
          ///< a row (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_FLOOR_SWITCH =
      20, ///< ★ Named from what the player sees: the room of the great statue
          ///< that speaks to Isaac (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_SMALL_ROOM =
      21, ///< ★ Named from what the player sees: the room of the great statue
          ///< that speaks to Isaac (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS =
      22, ///< ★ Named from what the player sees: the room with a statue, a
          ///< floor switch and a ledge (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR = 23,
  VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_FROM_STATUE_ROW =
      24, ///< ★ Named from what the player sees: the narrow room of two stacked
          ///< arches (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_FROM_GREAT_STATUE =
      25, ///< ★ Named from what the player sees: the narrow room of two stacked
          ///< arches (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_GREAT_STATUE =
      26, ///< ★ Named from what the player sees: the room with a statue, a
          ///< floor switch and a ledge (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_RELIEFS_WEST =
      30, ///< ★ Named from the game: "Carry Stone" (MsgItemName)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_RELIEFS_EAST =
      31, ///< ★ Named from the game: "Carry Stone" (MsgItemName)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_PILLARS =
      32, ///< ★ Named from the game: "Carry Stone" (MsgItemName)
};
/// SceneId_VenusLighthouseGroundFloor's exits.
enum {
  VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_ENTRY = 1,
  VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_TO_RELIEFS =
      2, ///< ★ Named from what the player sees: the room of golden reliefs and
         ///< a tree emblem on the floor (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_FRONT_ROOM =
      3, ///< ★ Named from what the player sees: the hall of four statues behind
         ///< the front door (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_EAST =
      4, ///< ★ Named from the game: "Carry Stone" (MsgItemName)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_WEST =
      5, ///< ★ Named from the game: "Carry Stone" (MsgItemName)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_PILLARS =
      6, ///< ★ Named from what the player sees: the room of spiral-marked stone
         ///< pillars (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_RELIEFS =
      7, ///< ★ Named from what the player sees: the room of golden reliefs and
         ///< a tree emblem on the floor (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_CARRY_STONE =
      8, ///< ★ Named from the game: "Carry Stone" (MsgItemName)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_FLOOR_SWITCH =
      9, ///< ★ Named from what the player sees: the room with a statue, a floor
         ///< switch and a ledge (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_NORTH =
      10, ///< ★ Named from what the player sees: the room with five statues in
          ///< a row (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_WEST =
      11, ///< ★ Named from what the player sees: the room with five statues in
          ///< a row (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_NORTH =
      12, ///< ★ Named from what the player sees: the room of spiral-marked
          ///< stone pillars (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_WEST =
      13, ///< ★ Named from what the player sees: the room of spiral-marked
          ///< stone pillars (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_SMALL_ROOM =
      14, ///< ★ Named from what the player sees: the narrow room of two stacked
          ///< arches (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_FLOOR_SWITCH =
      20, ///< ★ Named from what the player sees: the room with a statue, a
          ///< floor switch and a ledge (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_SMALL_ROOM =
      21, ///< ★ Named from what the player sees: the narrow room of two stacked
          ///< arches (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_PILLARS =
      22, ///< ★ Named from what the player sees: the room of spiral-marked
          ///< stone pillars (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR = 23,
  VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_STATUE_ROW =
      24, ///< ★ Named from what the player sees: the room with five statues in
          ///< a row (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_GREAT_STATUE =
      25, ///< ★ Named from what the player sees: the room of the great statue
          ///< that speaks to Isaac (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_GREAT_STATUE =
      26, ///< ★ Named from what the player sees: the room of the great statue
          ///< that speaks to Isaac (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_WEST =
      30, ///< ★ Named from what the player sees: the room of golden reliefs and
          ///< a tree emblem on the floor (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_EAST =
      31, ///< ★ Named from what the player sees: the room of golden reliefs and
          ///< a tree emblem on the floor (its render)
  VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_PILLARS =
      32, ///< ★ Named from what the player sees: the room of spiral-marked
          ///< stone pillars (its render)
};

/// SceneId_VenusLighthouseFirstFloor's entrances.
enum {
  VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_LOWER_ROOM =
      1, ///< ★ Named from what the player sees: the room of a golden floor that
         ///< carries the party south, and a hole (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_GAP =
      2, ///< ★ Named from what the player sees: the room of a golden floor that
         ///< carries the party south, and a hole (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH = 3,
  VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_THUNDER_CROWN =
      4, ///< ★ Named from what the player sees: the room of a golden floor that
         ///< carries the party south, and a hole (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_FROM_DRIFTING_FLOOR =
      5, ///< ★ Named from the game: "Thunder Crown" (MsgItemName)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_FROM_LADDERS =
      6, ///< ★ Named from the game: "Thunder Crown" (MsgItemName)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_FROM_LADDERS =
      7, ///< ★ Named from what the player sees: the room of three stone pillars
         ///< in a row (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST = 8,
  VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_DRIFTING_FLOOR =
      9, ///< ★ Named from what the player sees: the room where two pillars
         ///< stand by a gap (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_LADDERS =
      10, ///< ★ Named from what the player sees: the room where two pillars
          ///< stand by a gap (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_GAP =
      11, ///< ★ Named from what the player sees: the large room of ladders and
          ///< grates (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH = 12,
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_THUNDER_CROWN =
      13, ///< ★ Named from what the player sees: the large room of ladders and
          ///< grates (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PILLARS =
      14, ///< ★ Named from what the player sees: the large room of ladders and
          ///< grates (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PSYNERGY_STONE =
      15, ///< ★ Named from what the player sees: the large room of ladders and
          ///< grates (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR = 16,
  VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_FROM_LADDERS =
      17, ///< ★ Named from the game: "Psynergy Stone" (MsgGotPsynergyStone)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_TUNNEL_RUINS_LADY_STATUES = 18,
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR =
      19, ///< ★ Named from what the player sees: the small room below the hole
          ///< (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR_HOLE =
      20, ///< ★ Named from what the player sees: the small room below the hole
          ///< (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST = 21,
};
/// SceneId_VenusLighthouseFirstFloor's exits.
enum {
  VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM =
      1, ///< ★ Named from what the player sees: the small room below the hole
         ///< (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_GAP =
      2, ///< ★ Named from what the player sees: the room where two pillars
         ///< stand by a gap (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH = 3,
  VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_THUNDER_CROWN =
      4, ///< ★ Named from the game: "Thunder Crown" (MsgItemName)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_DRIFTING_FLOOR =
      5, ///< ★ Named from what the player sees: the room of a golden floor that
         ///< carries the party south, and a hole (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_LADDERS =
      6, ///< ★ Named from what the player sees: the large room of ladders and
         ///< grates (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_TO_LADDERS =
      7, ///< ★ Named from what the player sees: the large room of ladders and
         ///< grates (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST = 8,
  VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_DRIFTING_FLOOR =
      9, ///< ★ Named from what the player sees: the room of a golden floor that
         ///< carries the party south, and a hole (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_LADDERS =
      10, ///< ★ Named from what the player sees: the large room of ladders and
          ///< grates (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_GAP =
      11, ///< ★ Named from what the player sees: the room where two pillars
          ///< stand by a gap (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST = 12,
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_THUNDER_CROWN =
      13, ///< ★ Named from the game: "Thunder Crown" (MsgItemName)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PILLARS =
      14, ///< ★ Named from what the player sees: the room of three stone
          ///< pillars in a row (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PSYNERGY_STONE =
      15, ///< ★ Named from the game: "Psynergy Stone" (MsgGotPsynergyStone)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR = 16,
  VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_TO_LADDERS =
      17, ///< ★ Named from what the player sees: the large room of ladders and
          ///< grates (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES = 18,
  VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_TO_DRIFTING_FLOOR =
      19, ///< ★ Named from what the player sees: the room of a golden floor
          ///< that carries the party south, and a hole (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM_HOLE =
      20, ///< ★ Named from what the player sees: the small room below the hole
          ///< (its render)
  VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH = 21,
};

/// SceneId_VenusLighthouseSecondFloor's entrances.
enum {
  VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH = 1,
  VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_FROM_PILLAR_PIT =
      2, ///< ★ Named from what the player sees: the passage the party walks
         ///< into and leaves in a trail of sparks (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_FROM_SPARKLING_PASSAGE =
      3, ///< ★ Named from what the player sees: the room of stone posts over a
         ///< pit, with ladders (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST = 4,
  VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_FROM_GATE =
      5, ///< ★ Named from what the player sees: the room of stone posts over a
         ///< pit, with ladders (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR = 6,
  VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST = 7,
  VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_FROM_GATE =
      8, ///< ★ Named from what the player sees: the room of the floor switch
         ///< that slides a wall aside (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_WALL_SWITCH =
      9, ///< ★ Named from what the player sees: the room of the gate a pillar
         ///< in its socket opens, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_PILLAR_PIT =
      10, ///< ★ Named from what the player sees: the room of the gate a pillar
          ///< in its socket opens, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH = 11,
};
/// SceneId_VenusLighthouseSecondFloor's exits.
enum {
  VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH = 1,
  VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_TO_PILLAR_PIT =
      2, ///< ★ Named from what the player sees: the room of stone posts over a
         ///< pit, with ladders (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_SPARKLING_PASSAGE =
      3, ///< ★ Named from what the player sees: the passage the party walks
         ///< into and leaves in a trail of sparks (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST = 4,
  VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_GATE =
      5, ///< ★ Named from what the player sees: the room of the gate a pillar
         ///< in its socket opens, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR = 6,
  VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH = 7,
  VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_TO_GATE =
      8, ///< ★ Named from what the player sees: the room of the gate a pillar
         ///< in its socket opens, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_WALL_SWITCH =
      9, ///< ★ Named from what the player sees: the room of the floor switch
         ///< that slides a wall aside (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_PILLAR_PIT =
      10, ///< ★ Named from what the player sees: the room of stone posts over a
          ///< pit, with ladders (its render)
  VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST = 11,
};

/// SceneId_VenusLighthouseThirdFloor's entrances.
enum {
  VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST =
      1,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_FROM_SPARKLING_PASSAGES =
      2, ///< ★ Named from what the player sees: the small room at the top of
         ///< the floor's west side (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_FROM_SPARKLING_PASSAGES =
      3, ///< ★ Named from the game: "Dragon Scales" (MsgItemName)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_NORTHWEST_ROOM =
      4, ///< ★ Named from what the player sees: the room of golden falls hiding
         ///< two sparkling passages (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRAGON_SCALES =
      5, ///< ★ Named from what the player sees: the room of golden falls hiding
         ///< two sparkling passages (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRIFTING_FLOOR =
      6, ///< ★ Named from what the player sees: the room of golden falls hiding
         ///< two sparkling passages (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST =
      7,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_FROM_DRIFTING_FLOOR =
      8, ///< ★ Named from what the player sees: the narrow room at the bottom
         ///< of the floor's west side (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SPARKLING_PASSAGES =
      9, ///< ★ Named from what the player sees: the large room of a golden
         ///< floor that carries the party south (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST =
      10,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM =
      11, ///< ★ Named from what the player sees: the large room of a golden
          ///< floor that carries the party south (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST =
      12,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST =
      13,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_PILLAR_SOCKETS =
      14, ///< ★ Named from what the player sees: the large room of a golden
          ///< floor that carries the party south (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_DRIFTING_FLOOR =
      15, ///< ★ Named from what the player sees: the room where three pillars
          ///< sink into sockets, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_WEST_ROOM =
      16, ///< ★ Named from what the player sees: the room where three pillars
          ///< sink into sockets, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR = 17,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_FROM_PILLAR_SOCKETS =
      18, ///< ★ Named from what the player sees: the small room where the
          ///< stairs from below arrive (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST =
      19,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST =
      20,
};
/// SceneId_VenusLighthouseThirdFloor's exits.
enum {
  VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST =
      1,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_TO_SPARKLING_PASSAGES =
      2, ///< ★ Named from what the player sees: the room of golden falls hiding
         ///< two sparkling passages (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_TO_SPARKLING_PASSAGES =
      3, ///< ★ Named from what the player sees: the room of golden falls hiding
         ///< two sparkling passages (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_NORTHWEST_ROOM =
      4, ///< ★ Named from what the player sees: the small room at the top of
         ///< the floor's west side (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRAGON_SCALES =
      5, ///< ★ Named from the game: "Dragon Scales" (MsgItemName)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRIFTING_FLOOR =
      6, ///< ★ Named from what the player sees: the large room of a golden
         ///< floor that carries the party south (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST =
      7,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_TO_DRIFTING_FLOOR =
      8, ///< ★ Named from what the player sees: the large room of a golden
         ///< floor that carries the party south (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SPARKLING_PASSAGES =
      9, ///< ★ Named from what the player sees: the room of golden falls hiding
         ///< two sparkling passages (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST =
      10,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SOUTHWEST_ROOM =
      11, ///< ★ Named from what the player sees: the narrow room at the bottom
          ///< of the floor's west side (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST = 12,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST = 13,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_PILLAR_SOCKETS =
      14, ///< ★ Named from what the player sees: the room where three pillars
          ///< sink into sockets, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_DRIFTING_FLOOR =
      15, ///< ★ Named from what the player sees: the large room of a golden
          ///< floor that carries the party south (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_WEST_ROOM =
      16, ///< ★ Named from what the player sees: the small room where the
          ///< stairs from below arrive (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR = 17,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_TO_PILLAR_SOCKETS =
      18, ///< ★ Named from what the player sees: the room where three pillars
          ///< sink into sockets, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST = 19,
  VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST =
      20,
};

/// SceneId_VenusLighthouseFourthFloor's entrances.
enum {
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST =
      1,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_FROM_WALL_SWITCHES =
      2, ///< ★ Named from what the player sees: the room of spiral pillars set
         ///< on platforms that rise or sink (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLARS =
      3, ///< ★ Named from what the player sees: the large room of the west and
         ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST =
      4,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST =
      5,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE =
      6, ///< ★ Named from what the player sees: the large room of the west and
         ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST = 7,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_EAST =
      8, ///< ★ Named from what the player sees: the large room of the west and
         ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST = 9,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_WEST =
      10, ///< ★ Named from what the player sees: the large room of the west and
          ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_EAST =
      11, ///< ★ Named from what the player sees: the large room of the west and
          ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_SOUTH =
      12, ///< ★ Named from what the player sees: the large room of the west and
          ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_WEST =
      13, ///< ★ Named from the game: "Gaia Blade" (MsgItemName)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_EAST =
      14, ///< ★ Named from the game: "Gaia Blade" (MsgItemName)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_SOUTH =
      15, ///< ★ Named from what the player sees: the room where four pillars
          ///< sink into sockets, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_EAST =
      16, ///< ★ Named from what the player sees: the room where four pillars
          ///< sink into sockets, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_FROM_WALL_SWITCHES =
      17, ///< ★ Named from the game: "Psynergy Stone" (MsgGotPsynergyStone)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST =
      18,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST =
      19,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING =
      20, ///< ★ Named from what the player sees: the large room of the west and
          ///< east floor switches and the rising platform (its render)
};
/// SceneId_VenusLighthouseFourthFloor's exits.
enum {
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST =
      1,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_TO_WALL_SWITCHES =
      2, ///< ★ Named from what the player sees: the large room of the west and
         ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLARS =
      3, ///< ★ Named from what the player sees: the room of spiral pillars set
         ///< on platforms that rise or sink (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST =
      4,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST =
      5,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PSYNERGY_STONE =
      6, ///< ★ Named from the game: "Psynergy Stone" (MsgGotPsynergyStone)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST = 7,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_EAST =
      8, ///< ★ Named from what the player sees: the room where four pillars
         ///< sink into sockets, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST = 9,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_WEST =
      10, ///< ★ Named from the game: "Gaia Blade" (MsgItemName)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_EAST =
      11, ///< ★ Named from the game: "Gaia Blade" (MsgItemName)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_SOUTH =
      12, ///< ★ Named from what the player sees: the room where four pillars
          ///< sink into sockets, with a statue and a locked door (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_WEST =
      13, ///< ★ Named from what the player sees: the large room of the west and
          ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_EAST =
      14, ///< ★ Named from what the player sees: the large room of the west and
          ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_SOUTH =
      15, ///< ★ Named from what the player sees: the large room of the west and
          ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_EAST =
      16, ///< ★ Named from what the player sees: the large room of the west and
          ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES =
      17, ///< ★ Named from what the player sees: the large room of the west and
          ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST = 18,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST =
      19,
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES_RISING =
      20, ///< ★ Named from what the player sees: the large room of the west and
          ///< east floor switches and the rising platform (its render)
  VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE = 21,
};

/// SceneId_VenusLighthouseAerie's entrances and exits.
enum {
  VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR = 1,
  VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE = 2,  ///< after Saturos and Menardi
  VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE = 3, ///< after their fused form
  VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP = 4,
  VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT = 9, ///< after either battle is lost
  VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT =
      93, ///< the world map sends the party back here after its lighthouse cut
  VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP = 1,
  VENUS_LIGHTHOUSE_AERIE_TO_LALIVERO_FARANS_HOUSE = 2,
  VENUS_LIGHTHOUSE_AERIE_TO_LALIVERO_SHOPS = 9,
};

/// SceneId_LemurianShip's entrances.
enum {
  LEMURIAN_SHIP_FROM_LALIVERO_FARANS_HOUSE = 1,
  /// In no table row and led to by no exit: only the debug scene selector,
  /// which warps to any entrance from 0 to 99, reaches it. Arriving by it hands
  /// the leader the Black Orb.
  LEMURIAN_SHIP_WITH_BLACK_ORB = 99,
};
/// SceneId_LemurianShip's exits.
enum {
  LEMURIAN_SHIP_TO_LALIVERO_FARANS_HOUSE = 1,
};

/// SceneId_TolbiSpring's entrances.
enum {
  TOLBI_SPRING_FROM_TOLBI = 1,
};
/// SceneId_TolbiSpring's exits.
enum {
  TOLBI_SPRING_TO_TOLBI = 1,
};

/// SceneId_LinkLobby's entrances and exits.
enum {
  LINK_LOBBY_FROM_CLEAR = 1,
};

#endif
