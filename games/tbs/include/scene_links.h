#ifndef ALCHEMY_SCENE_LINKS_H
#define ALCHEMY_SCENE_LINKS_H

/// @file
/// The ways between places: each scene's entrances, named by where the party
/// arrives from, and its exits, named by where they lead, in the exit tables'
/// numbers (SCENE_EXIT). A door is named the same from both sides. Links
/// between rooms of one scene are named by their rooms
/// (<SCENE>_<ROOM>_TO_<ROOM>).

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

/// SceneId_WorldMap's entrances and exits.
enum {
  WORLD_MAP_FROM_VALE_GATE = 1,
  WORLD_MAP_FROM_TITLE = 2,
  WORLD_MAP_FROM_VAULT_SOUTH = 3,
  WORLD_MAP_FROM_GOMA_CAVE_ENTRANCE = 4,
  WORLD_MAP_FROM_BILIBIN = 5,
  WORLD_MAP_FROM_BILIBIN_BARRICADE_EAST = 6,
  WORLD_MAP_FROM_KOLIMA_WEST = 7,
  WORLD_MAP_FROM_KOLIMA_FOREST_1 = 8,
  WORLD_MAP_FROM_IMIL_SOUTH = 9,
  WORLD_MAP_FROM_MERCURY_LIGHTHOUSE_4_EAST = 10,
  WORLD_MAP_FROM_KOLIMA_BRIDGE_EAST = 11,
  WORLD_MAP_FROM_FUCHIN_TEMPLE_1 = 12,
  WORLD_MAP_FROM_MOGALL_FOREST_1 = 13,
  WORLD_MAP_FROM_XIAN = 14,
  WORLD_MAP_FROM_ALPINE_CROSSING_EAST = 15,
  WORLD_MAP_FROM_ALTIN = 16,
  WORLD_MAP_FROM_LAMA_TEMPLE = 17,
  WORLD_MAP_FROM_LAMAKAN_DESERT_1 = 18,
  WORLD_MAP_FROM_KALAY_1_WEST = 19,
  WORLD_MAP_FROM_KALAY_1_EAST = 20,
  WORLD_MAP_FROM_KALAY_DOCKS_EAST = 21,
  WORLD_MAP_FROM_TOLBI_DOCKS = 22,
  WORLD_MAP_FROM_TOLBI = 23,
  WORLD_MAP_FROM_ALTMILLER_CAVE_1 = 24,
  WORLD_MAP_FROM_GONDOWAN_PASSAGE_EAST = 25,
  WORLD_MAP_FROM_GONDOWAN_CAVE_2_NORTH = 26,
  WORLD_MAP_FROM_SUHALLA = 27,
  WORLD_MAP_FROM_SUHALLA_DESERT_1 = 28,
  WORLD_MAP_FROM_SUHALLA_GATE_1_EAST = 29,
  WORLD_MAP_FROM_LALIVERO_EAST = 30,
  WORLD_MAP_FROM_ROAD_TO_BABI_LIGHTHOUSE = 31,
  WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_EAST = 32,
  WORLD_MAP_FROM_GATEWAY_CAVE = 33,
  WORLD_MAP_FROM_LUNPA = 34,
  WORLD_MAP_FROM_BILIBIN_CAVE_ENTRANCE = 35,
  WORLD_MAP_FROM_KOLIMA_EAST = 38,
  WORLD_MAP_FROM_IMIL_NORTH = 39,
  WORLD_MAP_FROM_GOMA_CAVE_BRIDGE = 40,
  WORLD_MAP_FROM_GONDOWAN_CAVE_2_SOUTH = 41,
  WORLD_MAP_FROM_SUHALLA_GATE_1_WEST = 42,
  WORLD_MAP_FROM_MOGALL_FOREST_3 = 43,
  WORLD_MAP_FROM_VAULT_NORTH = 44,
  WORLD_MAP_FROM_KOLIMA_BRIDGE_WEST = 46,
  WORLD_MAP_FROM_BILIBIN_BARRICADE_WEST = 47,
  WORLD_MAP_FROM_BILIBIN_CAVE_ICE = 48,
  WORLD_MAP_FROM_WEST_LUNPA_CAVE = 49,
  WORLD_MAP_FROM_ALPINE_CROSSING_NORTH = 50,
  WORLD_MAP_FROM_ALPINE_CROSSING_WEST = 51,
  WORLD_MAP_FROM_ALTIN_PEAK_11 = 52,
  WORLD_MAP_FROM_LAMAKAN_DESERT_4 = 53,
  WORLD_MAP_FROM_GONDOWAN_PASSAGE_WEST = 54,
  WORLD_MAP_FROM_SUHALLA_DESERT_2 = 55,
  WORLD_MAP_FROM_KALAY_DOCKS_WEST = 58,
  WORLD_MAP_FROM_LALIVERO_WEST = 59,
  WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_NORTH = 60,
  WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_WEST = 61,
  WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH = 71,
  WORLD_MAP_FROM_CLEAR = 72,
  WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS = 75,
  WORLD_MAP_FROM_CROSSBONE_ISLE = 76,
  WORLD_MAP_MERCURY_LIGHTHOUSE_LIT =
      90, ///< WorldMap_Initialize plays WorldMap_PlayLighthouseTransition
          ///< here in Mercury Lighthouse's colours, then sends the party to the
          ///< aerie
  WORLD_MAP_VENUS_LIGHTHOUSE_LIT =
      91, ///< WorldMap_Initialize plays WorldMap_PlayLighthouseTransition
          ///< here in Venus Lighthouse's colours, then sends the party back to
          ///< the aerie
};
enum {
  WORLD_MAP_TO_VALE_GATE = 1,
  WORLD_MAP_TO_VAULT_SOUTH = 3,
  WORLD_MAP_TO_GOMA_CAVE_ENTRANCE = 4,
  WORLD_MAP_TO_BILIBIN = 5,
  WORLD_MAP_TO_BILIBIN_BARRICADE_WEST = 6,
  WORLD_MAP_TO_KOLIMA_WEST = 7,
  WORLD_MAP_TO_KOLIMA_FOREST_1 = 8,
  WORLD_MAP_TO_IMIL_SOUTH = 9,
  WORLD_MAP_TO_MERCURY_LIGHTHOUSE_4_EAST = 10,
  WORLD_MAP_TO_KOLIMA_BRIDGE_EAST = 11,
  WORLD_MAP_TO_FUCHIN_TEMPLE_1 = 12,
  WORLD_MAP_TO_MOGALL_FOREST_1 = 13,
  WORLD_MAP_TO_XIAN = 14,
  WORLD_MAP_TO_ALPINE_CROSSING_EAST = 15,
  WORLD_MAP_TO_ALTIN = 16,
  WORLD_MAP_TO_LAMA_TEMPLE = 17,
  WORLD_MAP_TO_LAMAKAN_DESERT_1 = 18,
  WORLD_MAP_TO_KALAY_1_EAST = 20,
  WORLD_MAP_TO_KALAY_DOCKS_EAST = 21,
  WORLD_MAP_TO_TOLBI_DOCKS = 22,
  WORLD_MAP_TO_TOLBI = 23,
  WORLD_MAP_TO_ALTMILLER_CAVE_1 = 24,
  WORLD_MAP_TO_GONDOWAN_PASSAGE_EAST = 25,
  WORLD_MAP_TO_GONDOWAN_CAVE_2_NORTH = 26,
  WORLD_MAP_TO_SUHALLA = 27,
  WORLD_MAP_TO_SUHALLA_DESERT_1 = 28,
  WORLD_MAP_TO_SUHALLA_GATE_1_WEST = 29,
  WORLD_MAP_TO_LALIVERO_EAST = 30,
  WORLD_MAP_TO_ROAD_TO_BABI_LIGHTHOUSE = 31,
  WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST = 32,
  WORLD_MAP_TO_GATEWAY_CAVE = 33,
  WORLD_MAP_TO_LUNPA = 34,
  WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE = 35,
  WORLD_MAP_TO_GOMA_CAVE_BRIDGE = 40,
  WORLD_MAP_TO_GONDOWAN_CAVE_2_SOUTH = 41,
  WORLD_MAP_TO_SUHALLA_GATE_1_EAST = 42,
  WORLD_MAP_TO_MOGALL_FOREST_3 = 43,
  WORLD_MAP_TO_VAULT_NORTH = 44,
  WORLD_MAP_TO_KOLIMA_BRIDGE_WEST = 46,
  WORLD_MAP_TO_BILIBIN_BARRICADE_EAST = 47,
  WORLD_MAP_TO_BILIBIN_CAVE_ICE = 48,
  WORLD_MAP_TO_WEST_LUNPA_CAVE = 49,
  WORLD_MAP_TO_ALPINE_CROSSING_NORTH = 50,
  WORLD_MAP_TO_ALPINE_CROSSING_WEST = 51,
  WORLD_MAP_TO_ALTIN_PEAK_11 = 52,
  WORLD_MAP_TO_LAMAKAN_DESERT_4 = 53,
  WORLD_MAP_TO_GONDOWAN_PASSAGE_WEST = 54,
  WORLD_MAP_TO_SUHALLA_DESERT_2 = 55,
  WORLD_MAP_TO_KALAY_DOCKS_NORTH = 56,
  WORLD_MAP_TO_LALIVERO_WEST = 59,
  WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE = 60,
  WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST = 61,
  WORLD_MAP_TO_CROSSBONE_ISLE = 75,
  WORLD_MAP_TO_TOLBI_SHIP_INTERIORS = 105,
  WORLD_MAP_TO_TOLBI_SHIP_DECK = 107,
  WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE_SOUTH = 108,
  WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE = 109,
  WORLD_MAP_TO_LUNPA_SUHALLA = 112,
};

/// SceneId_ValeStorm's entrances and exits.
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

/// SceneId_Vale's entrances and exits.
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
  VALE_FROM_TITLE = 16,
  VALE_FROM_ROOF_MENDED = 17,
};
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

/// SceneId_ValeGate's entrances and exits.
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
};
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

/// SceneId_ValeKraden's entrances and exits.
enum {
  VALE_KRADEN_FROM_VALE_NORTH = 1,
  VALE_KRADEN_FROM_VALE_SOUTH = 2,
  VALE_KRADEN_FROM_VALE_INTERIORS_INN_NORTH = 3,
  VALE_KRADEN_FROM_VALE_CAVE_MOUTH_WEST = 4,
  VALE_KRADEN_FROM_VALE_CAVE_MOUTH_EAST = 6,
  VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH = 10,
};
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

/// SceneId_ValeInteriorsShops's entrances and exits.
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

/// SceneId_ValeInteriorsInn's entrances and exits.
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
  VALE_INTERIORS_INN_INN_FROM_DOWNSTAIRS =
      8, ///< ★ Named from what the player sees: Vale's inn, with its
         ///< innkeeper's counter and the stairs up to the guest room (its
         ///< render)
  VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_DOWNSTAIRS =
      9, ///< ★ Named from what the player sees: Isaac's house, where Dora wakes
         ///< him on the stormy night (its render)
  VALE_INTERIORS_INN_FROM_VALE_STORM_WEST = 11,
  VALE_INTERIORS_INN_FROM_VALE_STORM_EAST = 12,
  VALE_INTERIORS_INN_FROM_BABI_PALACE_1 = 19,
  VALE_INTERIORS_INN_FROM_VALE_GATE_SOUTHEAST = 20,
  VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA =
      21, ///< ★ Named from what the player sees: Isaac's house, where Dora
          ///< wakes him on the stormy night (its render)
  VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG = 22,
};
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
  VALE_INTERIORS_INN_INN_TO_DOWNSTAIRS =
      8, ///< ★ Named from what the player sees: the ground floor at the foot of
         ///< the stairs (its render)
  VALE_INTERIORS_INN_ISAACS_HOUSE_TO_DOWNSTAIRS =
      9, ///< ★ Named from what the player sees: the ground floor at the foot of
         ///< the stairs (its render)
  VALE_INTERIORS_INN_TO_VALE_STORM_WEST = 10,
  VALE_INTERIORS_INN_TO_VALE_STORM_EAST = 11,
  VALE_INTERIORS_INN_TO_BABI_PALACE_1 = 19,
  VALE_INTERIORS_INN_ISAACS_HOUSE_TO_DOWNSTAIRS_WITH_DORA =
      20, ///< ★ Named from what the player sees: the ground floor at the foot
          ///< of the stairs (its render)
  VALE_INTERIORS_INN_TO_VALE_STORM_WITH_DORA =
      21, ///< ★ Named from what the player sees: Dora leading Isaac down and
          ///< out of the house on the stormy night (its render)
  VALE_INTERIORS_INN_TO_VALE_KRADEN_SOUTH = 22,
};

/// SceneId_Sanctum's entrances and exits.
enum {
  SANCTUM_DJINNIS_HOUSE = 0, ///< the room holds the djinnis house;
  SANCTUM_FROM_VALE = 1,
  SANCTUM_FROM_VAULT = 10,
  SANCTUM_FROM_VAULT_REVISIT = 11,
  SANCTUM_FROM_VAULT_INTERIORS = 12,
  SANCTUM_FROM_VALE_ERUPTION = 20,
  SANCTUM_FROM_QUEST_RESTART = 21,
  SANCTUM_FROM_ALTIN = 29,
  SANCTUM_FROM_XIAN = 32,
  SANCTUM_FROM_BILIBIN = 35,
};
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

/// SceneId_ValeEruption's entrances and exits.
enum {
  VALE_ERUPTION_FROM_SOL_ENTRANCE = 15,
};
enum {
  VALE_ERUPTION_TO_SANCTUM = 1,
};

/// SceneId_SolSanctumLunaSolRooms's entrances and exits.
enum {
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_RELIEFS = 1,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE = 2,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED = 4,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_SPRUNG = 5,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN = 6,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_UNUSED = 7,
  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION = 8,
};
enum {
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS = 1,
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE = 2,
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED =
      3, ///< ★ Named from what the player sees: the quarter's light flashing
         ///< ever faster around Kraden once the trap is disarmed (its render)
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_SPRUNG =
      4, ///< ★ Named from the game: "Stop!!! You're about to make a big
         ///< mistake!" (MsgSolSanctumWhatsHappening)
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN =
      5, ///< ★ Named from the game: "You found it"
         ///< (MsgSolSanctumKradenYouFoundIt)
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_UNUSED = 6,
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM = 7,
  SOL_SANCTUM_LUNA_SOL_ROOMS_TO_BATTLE = 10,
};

/// SceneId_SolSanctumStatue's entrances and exits.
enum {
  SOL_SANCTUM_STATUE_FROM_VALE_GATE = 0,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS = 1,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_PSYNERGY_STONE = 2,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED = 3,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_SPRUNG = 4,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN = 5,
  SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED = 6,
};
enum {
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS = 1,
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE = 2,
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED =
      3, ///< ★ Named from what the player sees: Kraden explaining that the
         ///< centre statue disarmed the trap (its render)
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_SPRUNG =
      4, ///< ★ Named from the game: "Stop!!! You're about to make a big
         ///< mistake!" (MsgSolSanctumWhatsHappening)
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN =
      5, ///< ★ Named from what the player sees: the lit quarters of the
         ///< pictures flashing in the Luna room (its render)
  SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED = 6,
  SOL_SANCTUM_STATUE_TO_BATTLE = 10,
};

/// SceneId_SolSanctumPsynergyStone's entrances and exits.
enum {
  SOL_SANCTUM_PSYNERGY_STONE_FROM_SOL_SANCTUM_STATUE = 1,
};
enum {
  SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE = 1,
};

/// SceneId_SolSanctumMaze's entrances and exits.
enum {
  SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_SOUTH = 1,
  SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_NORTH = 2,
};
enum {
  SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_SOUTH = 1,
  SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_NORTH = 2,
};

/// SceneId_SolSanctumCorridor's entrances and exits.
enum {
  SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_EAST = 1,
  SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_WEST = 2,
};
enum {
  SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST = 1,
  SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST = 2,
};

/// SceneId_SolSanctumReliefs's entrances and exits.
enum {
  SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST = 1,
  SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_HALL =
      2, ///< ★ Named from what the player sees: the room of three passages
         ///< above a stepping-stone pool, one passage ending at a chest (its
         ///< render)
  SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_HALL =
      3, ///< ★ Named from what the player sees: the narrow dead end with a
         ///< statue at its head (its render)
  SOL_SANCTUM_RELIEFS_POOL_ROOM_FROM_ENTRANCE_HALL =
      4, ///< ★ Named from what the player sees: the room off the entrance hall
         ///< with a pool crossed on stepping stones (its render)
  SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_EAST = 5,
  SOL_SANCTUM_RELIEFS_STAIRWAY_ROOM_FROM_FIRST_RELIEF_HALL =
      6, ///< ★ Named from what the player sees: the narrow room whose doorway
         ///< has stairs leading on into the sanctum (its render)
  SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_WEST = 7,
  SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE = 8,
  SOL_SANCTUM_RELIEFS_ENTRANCE_HALL_FROM_POOL_ROOM =
      9, ///< ★ Named from what the player sees: the room inside Sol Sanctum's
         ///< door, where the party first comes in from the mountainside (its
         ///< render)
  SOL_SANCTUM_RELIEFS_LOOP_CORRIDOR_FROM_SECOND_RELIEF_HALL =
      10, ///< ★ Named from what the player sees: the passage that runs in a
          ///< loop round a block of wall (its render)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_HALL_FROM_CHEST_ROOM =
      11, ///< ★ Named from the game: "It's a relief"
          ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_HALL_FROM_STATUE_NOOK =
      12, ///< ★ Named from the game: "It's a relief"
          ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_HALL_FROM_STAIRWAY_ROOM =
      13, ///< ★ Named from the game: "It's a relief"
          ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST = 14,
  SOL_SANCTUM_RELIEFS_SECOND_RELIEF_HALL_FROM_LOOP_CORRIDOR =
      15, ///< ★ Named from the game: "It's a relief"
          ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS = 16,
};
enum {
  SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST = 1,
  SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_HALL =
      2, ///< ★ Named from the game: "It's a relief"
         ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_HALL =
      3, ///< ★ Named from the game: "It's a relief"
         ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_POOL_ROOM_TO_ENTRANCE_HALL =
      4, ///< ★ Named from what the player sees: the room inside Sol Sanctum's
         ///< door, where the party first comes in from the mountainside (its
         ///< render)
  SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST = 5,
  SOL_SANCTUM_RELIEFS_STAIRWAY_ROOM_TO_FIRST_RELIEF_HALL =
      6, ///< ★ Named from the game: "It's a relief"
         ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST = 7,
  SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE = 8,
  SOL_SANCTUM_RELIEFS_ENTRANCE_HALL_TO_POOL_ROOM =
      9, ///< ★ Named from what the player sees: the room off the entrance hall
         ///< with a pool crossed on stepping stones (its render)
  SOL_SANCTUM_RELIEFS_LOOP_CORRIDOR_TO_SECOND_RELIEF_HALL =
      10, ///< ★ Named from the game: "It's a relief"
          ///< (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_HALL_TO_CHEST_ROOM =
      11, ///< ★ Named from what the player sees: the room of three passages
          ///< above a stepping-stone pool, one passage ending at a chest (its
          ///< render)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_HALL_TO_STATUE_NOOK =
      12, ///< ★ Named from what the player sees: the narrow dead end with a
          ///< statue at its head (its render)
  SOL_SANCTUM_RELIEFS_FIRST_RELIEF_HALL_TO_STAIRWAY_ROOM =
      13, ///< ★ Named from what the player sees: the narrow room whose doorway
          ///< has stairs leading on into the sanctum (its render)
  SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST = 14,
  SOL_SANCTUM_RELIEFS_SECOND_RELIEF_HALL_TO_LOOP_CORRIDOR =
      15, ///< ★ Named from what the player sees: the passage that runs in a
          ///< loop round a block of wall (its render)
  SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS = 16,
};

/// SceneId_SolSanctumElementalStarRoom's entrances and exits.
enum {
  SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS = 10,
};
enum {
  SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION = 2,
};

/// SceneId_SolSanctumEruption's entrances and exits.
enum {
  SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM = 10,
};
enum {
  SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS = 5,
};

/// SceneId_SolEntrance's entrances and exits.
enum {
  SOL_ENTRANCE_FROM_VALE = 1,
  SOL_ENTRANCE_FROM_SOL_SANCTUM_RELIEFS = 2,
};
enum {
  SOL_ENTRANCE_TO_VALE = 1,
  SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS = 2,
};

/// SceneId_Vault's entrances and exits.
enum {
  VAULT_FROM_WORLD_MAP_SOUTH = 1,
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

/// SceneId_VaultInteriors's entrances and exits.
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
};
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

/// SceneId_VaultJail's entrances and exits.
enum {
  VAULT_JAIL_FROM_VAULT = 1,
  VAULT_JAIL_FROM_LUNPA = 2,
  VAULT_JAIL_FROM_VAULT_REVISIT = 10,
};
enum {
  VAULT_JAIL_TO_VAULT = 1,
  VAULT_JAIL_TO_VAULT_REVISIT = 2,
  VAULT_JAIL_TO_LUNPA = 3,
};

/// SceneId_VaultRevisit's entrances and exits.
enum {
  VAULT_REVISIT_FROM_VAULT = 1,
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

/// SceneId_VaultInteriorsRevisit's entrances and exits.
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

/// SceneId_GomaCaveEntrance's entrances and exits.
enum {
  GOMA_CAVE_ENTRANCE_FROM_WORLD_MAP = 1,
  GOMA_CAVE_ENTRANCE_FROM_GOMA_CAVE_STREAM = 2,
};
enum {
  GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM = 1,
  GOMA_CAVE_ENTRANCE_TO_WORLD_MAP = 2,
};

/// SceneId_GomaCavePond's entrances and exits.
enum {
  GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_WEST = 1,
  GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_SOUTH = 2,
  GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_EAST = 3,
};
enum {
  GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_WEST = 1,
  GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_SOUTH = 2,
  GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_EAST = 3,
};

/// SceneId_GomaCavePillar's entrances and exits.
enum {
  GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_WEST = 1,
  GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST = 2,
  GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST = 3,
  GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST = 4,
  GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_SOUTH = 5,
};
enum {
  GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST = 1,
  GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST = 2,
  GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH = 3,
  GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST = 4,
  GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST = 5,
};

/// SceneId_GomaCaveStream's entrances and exits.
enum {
  GOMA_CAVE_STREAM_FROM_GOMA_CAVE_ENTRANCE = 1,
  GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_WEST = 2,
  GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_EAST = 3,
  GOMA_CAVE_STREAM_FROM_GOMA_CAVE_BRIDGE = 4,
  GOMA_CAVE_STREAM_FROM_VALE_GATE = 5,
};
enum {
  GOMA_CAVE_STREAM_TO_GOMA_CAVE_ENTRANCE = 1,
  GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_WEST = 2,
  GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_EAST = 3,
  GOMA_CAVE_STREAM_TO_GOMA_CAVE_BRIDGE = 4,
  GOMA_CAVE_STREAM_TO_VALE_GATE = 5,
};

/// SceneId_GomaCaveBridge's entrances and exits.
enum {
  GOMA_CAVE_BRIDGE_FROM_GOMA_CAVE_STREAM = 1,
  GOMA_CAVE_BRIDGE_FROM_WORLD_MAP = 2,
};
enum {
  GOMA_CAVE_BRIDGE_TO_GOMA_CAVE_STREAM = 1,
  GOMA_CAVE_BRIDGE_TO_WORLD_MAP = 2,
};

/// SceneId_Bilibin's entrances and exits.
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
enum {
  BILIBIN_TO_WORLD_MAP = 1,
  BILIBIN_TO_MCCOY_PALACE_GROUNDS = 2,
  BILIBIN_TO_INN = 3,
  BILIBIN_TO_HOUSE_EAST = 4,
  BILIBIN_TO_HOUSE_WEST = 5,
  BILIBIN_TO_HOUSE_NORTH = 6,
  BILIBIN_TO_ITEM_SHOP = 7,
  BILIBIN_TO_WEAPON_AND_ARMOR_SHOP = 8,
  BILIBIN_TO_SANCTUM = 9,
  BILIBIN_TO_BILIBIN_TREASURY_WEST = 10,
  BILIBIN_TO_BILIBIN_TREASURY_EAST = 11,
};

/// SceneId_BilibinInteriors's entrances and exits.
enum {
  BILIBIN_INTERIORS_HOUSE_EAST = 1,  ///< the room behind Bilibin's house east
                                     ///< door; through BILIBIN_TO_HOUSE_EAST
  BILIBIN_INTERIORS_HOUSE_WEST = 2,  ///< the room behind Bilibin's house west
                                     ///< door; through BILIBIN_TO_HOUSE_WEST
  BILIBIN_INTERIORS_HOUSE_NORTH = 3, ///< the room behind Bilibin's house north
                                     ///< door; through BILIBIN_TO_HOUSE_NORTH
  BILIBIN_INTERIORS_WEAPON_AND_ARMOR_SHOP =
      4, ///< the room behind Bilibin's weapon and armor shop door; the room
         ///< holds the weapon and armor shop; through
         ///< BILIBIN_TO_WEAPON_AND_ARMOR_SHOP
  BILIBIN_INTERIORS_ITEM_SHOP =
      5, ///< the room behind Bilibin's item shop door; the room holds the item
         ///< shop; through BILIBIN_TO_ITEM_SHOP
  BILIBIN_INTERIORS_INN = 6, ///< the room behind Bilibin's inn door; the room
                             ///< holds the inn; through BILIBIN_TO_INN
  BILIBIN_INTERIORS_INN_UPSTAIRS_FROM_INN =
      7, ///< ★ Named from what the player sees: the rooms of beds and crates up
         ///< the inn's stairs (its render)
  BILIBIN_INTERIORS_INN_FROM_INN_UPSTAIRS =
      8, ///< ★ Named from what the player sees: Bilibin's inn, with the
         ///< restaurant beside its counter (its render)
};
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

/// SceneId_BilibinTreasury's entrances and exits.
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

/// SceneId_McCoyPalace's entrances and exits.
enum {
  MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST = 1,
  MCCOY_PALACE_HALL_FROM_SOUTHWEST_LANDING =
      2, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_HALL_FROM_SOUTHEAST_LANDING =
      3, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_HALL_FROM_LADY_MCCOYS_ROOM_WEST =
      4, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_HALL_FROM_LADY_MCCOYS_ROOM_EAST =
      5, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_HALL_FROM_TREASURE_ROOM =
      6, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_HALL_FROM_NORTHWEST_LANDING =
      7, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_HALL_FROM_NORTHEAST_LANDING =
      8, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_HALL_FROM_MCCOYS_ROOM =
      9, ///< ★ Named from what the player sees: the entrance hall whose red
         ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_HALL_WEST =
      10, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_HALL_EAST =
      11, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_WEST_BEDROOM =
      12, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_EAST_BEDROOM =
      13, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_SOUTHWEST_LANDING_FROM_HALL =
      14, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_SOUTHWEST_LANDING_FROM_SOUTHWEST_TOWER =
      15, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_SOUTHEAST_LANDING_FROM_SOUTHEAST_TOWER =
      16, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_SOUTHEAST_LANDING_FROM_HALL =
      17, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_WEST_BEDROOM_FROM_LADY_MCCOYS_ROOM =
      18, ///< ★ Named from what the player sees: the bedroom through the west
          ///< door of Lady McCoy's room, with a bookcase (its render)
  MCCOY_PALACE_EAST_BEDROOM_FROM_LADY_MCCOYS_ROOM =
      19, ///< ★ Named from what the player sees: the bedroom through the east
          ///< door, with a wardrobe and the woman with the silk nightgown (its
          ///< render)
  MCCOY_PALACE_NORTHEAST_LANDING_FROM_HALL =
      20, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_NORTHEAST_LANDING_FROM_NORTHEAST_TOWER =
      21, ///< ★ Named from what the player sees: the small stair rooms between
          ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_NORTHWEST_LANDING_FROM_HALL =
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
  MCCOY_PALACE_TREASURE_ROOM_FROM_HALL =
      28, ///< ★ Named from the game: "Bilibin's great treasures"
          ///< (MsgBilibinDoThinkTheseBilibinsGreat)
  MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST = 29,
};
enum {
  MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST = 1,
  MCCOY_PALACE_HALL_TO_SOUTHWEST_LANDING =
      2, ///< ★ Named from what the player sees: the small stair rooms between
         ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_HALL_TO_SOUTHEAST_LANDING =
      3, ///< ★ Named from what the player sees: the small stair rooms between
         ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_HALL_TO_LADY_MCCOYS_ROOM_WEST =
      4, ///< ★ Named from the game: "This is Lady McCoy's room"
         ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_HALL_TO_LADY_MCCOYS_ROOM_EAST =
      5, ///< ★ Named from the game: "This is Lady McCoy's room"
         ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_HALL_TO_TREASURE_ROOM =
      6, ///< ★ Named from the game: "Bilibin's great treasures"
         ///< (MsgBilibinDoThinkTheseBilibinsGreat)
  MCCOY_PALACE_HALL_TO_NORTHWEST_LANDING =
      7, ///< ★ Named from what the player sees: the small stair rooms between
         ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_HALL_TO_NORTHEAST_LANDING =
      8, ///< ★ Named from what the player sees: the small stair rooms between
         ///< the hall's corner stairs and the rooms above (its render)
  MCCOY_PALACE_HALL_TO_MCCOYS_ROOM =
      9, ///< ★ Named from what the player sees: the room behind McCoy's door
         ///< where Lord McCoy sits (its render)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_HALL_WEST =
      10, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_HALL_EAST =
      11, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_WEST_BEDROOM =
      12, ///< ★ Named from what the player sees: the bedroom through the west
          ///< door of Lady McCoy's room, with a bookcase (its render)
  MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_EAST_BEDROOM =
      13, ///< ★ Named from what the player sees: the bedroom through the east
          ///< door, with a wardrobe and the woman with the silk nightgown (its
          ///< render)
  MCCOY_PALACE_SOUTHWEST_LANDING_TO_HALL =
      14, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_SOUTHWEST_LANDING_TO_SOUTHWEST_TOWER =
      15, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_SOUTHEAST_LANDING_TO_SOUTHEAST_TOWER =
      16, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_SOUTHEAST_LANDING_TO_HALL =
      17, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_WEST_BEDROOM_TO_LADY_MCCOYS_ROOM =
      18, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_EAST_BEDROOM_TO_LADY_MCCOYS_ROOM =
      19, ///< ★ Named from the game: "This is Lady McCoy's room"
          ///< (MsgBilibinThisIsLadyMccoysRoomSo)
  MCCOY_PALACE_NORTHEAST_LANDING_TO_HALL =
      20, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_NORTHEAST_LANDING_TO_NORTHEAST_TOWER =
      21, ///< ★ Named from what the player sees: the small eight-sided rooms up
          ///< two flights from each corner of the hall (its render)
  MCCOY_PALACE_NORTHWEST_LANDING_TO_HALL =
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
  MCCOY_PALACE_TREASURE_ROOM_TO_HALL =
      28, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
  MCCOY_PALACE_MCCOYS_ROOM_TO_HALL =
      29, ///< ★ Named from what the player sees: the entrance hall whose red
          ///< carpet leads to McCoy's door (its render)
};

/// SceneId_McCoyPalaceGrounds's entrances and exits.
enum {
  MCCOY_PALACE_GROUNDS_FROM_BILIBIN = 1,
  MCCOY_PALACE_GROUNDS_FROM_MCCOY_PALACE = 2,
};
enum {
  MCCOY_PALACE_GROUNDS_TO_BILIBIN = 1,
  MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE = 2,
  MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST = 10,
};

/// SceneId_BilibinBarricade's entrances and exits.
enum {
  BILIBIN_BARRICADE_FROM_WORLD_MAP_WEST = 1,
  BILIBIN_BARRICADE_FROM_WORLD_MAP_EAST = 2,
};
enum {
  BILIBIN_BARRICADE_TO_WORLD_MAP_EAST = 1,
  BILIBIN_BARRICADE_TO_WORLD_MAP_WEST = 2,
};

/// SceneId_Kolima's entrances and exits.
enum {
  KOLIMA_FROM_WORLD_MAP = 1,
  KOLIMA_FROM_KOLIMA_INTERIORS_NORTHWEST = 3,
  KOLIMA_FROM_KOLIMA_INTERIORS_SOUTHWEST = 4,
  KOLIMA_FROM_KOLIMA_INTERIORS_SOUTH = 5,
  KOLIMA_FROM_KOLIMA_INTERIORS_EAST = 6,
  KOLIMA_FROM_KOLIMA_INTERIORS_SOUTHEAST = 7,
  KOLIMA_FROM_KOLIMA_TUNNEL = 8,
};
enum {
  KOLIMA_TO_WORLD_MAP = 1,
  KOLIMA_TO_WORLD_MAP_EAST = 2,
  KOLIMA_TO_KOLIMA_INTERIORS_SOUTH = 5,
  KOLIMA_TO_KOLIMA_INTERIORS_EAST = 6,
  KOLIMA_TO_KOLIMA_INTERIORS_SOUTHEAST = 7,
  KOLIMA_TO_KOLIMA_INTERIORS_4 = 8,
  KOLIMA_TO_KOLIMA_INTERIORS_WEST = 9,
  KOLIMA_TO_KOLIMA_TUNNEL = 10,
};

/// SceneId_KolimaInteriors's entrances and exits.
enum {
  KOLIMA_INTERIORS_FROM_KOLIMA_NORTHWEST = 3,
  KOLIMA_INTERIORS_FROM_KOLIMA_NORTHEAST = 4,
  KOLIMA_INTERIORS_FROM_KOLIMA_SOUTHWEST = 7,
  KOLIMA_INTERIORS_FROM_KOLIMA_WEST = 8,
  KOLIMA_INTERIORS_FROM_KOLIMA_TUNNEL = 9,
  KOLIMA_INTERIORS_FROM_KOLIMA_SOUTHEAST = 10,
};
enum {
  KOLIMA_INTERIORS_TO_KOLIMA_NORTHWEST = 3,
  KOLIMA_INTERIORS_TO_KOLIMA_NORTHEAST = 4,
  KOLIMA_INTERIORS_TO_KOLIMA_SOUTHWEST = 7,
  KOLIMA_INTERIORS_TO_KOLIMA_WEST = 8,
  KOLIMA_INTERIORS_TO_KOLIMA_EAST = 9,
  KOLIMA_INTERIORS_TO_KOLIMA_TUNNEL = 14,
};

/// SceneId_KolimaTunnel's entrances and exits.
enum {
  KOLIMA_TUNNEL_FROM_KOLIMA_INTERIORS = 1,
  KOLIMA_TUNNEL_FROM_KOLIMA = 2,
};
enum {
  KOLIMA_TUNNEL_TO_KOLIMA_INTERIORS = 1,
  KOLIMA_TUNNEL_TO_KOLIMA = 2,
};

/// SceneId_KolimaBridge's entrances and exits.
enum {
  KOLIMA_BRIDGE_FROM_WORLD_MAP_SOUTH = 1,
  KOLIMA_BRIDGE_FROM_WORLD_MAP_NORTH = 2,
};
enum {
  KOLIMA_BRIDGE_TO_WORLD_MAP_SOUTH = 1,
  KOLIMA_BRIDGE_TO_WORLD_MAP_NORTH = 2,
};

/// SceneId_KolimaForest1's entrances and exits.
enum {
  KOLIMA_FOREST_1_FROM_KOLIMA_FOREST_2 = 2,
  KOLIMA_FOREST_1_FROM_WORLD_MAP = 4,
};
enum {
  KOLIMA_FOREST_1_TO_KOLIMA_FOREST_2 = 1,
  KOLIMA_FOREST_1_TO_WORLD_MAP = 4,
};

/// SceneId_KolimaForest2's entrances and exits.
enum {
  KOLIMA_FOREST_2_FROM_KOLIMA_FOREST_1 = 1,
  KOLIMA_FOREST_2_FROM_KOLIMA_FOREST_3 = 2,
};
enum {
  KOLIMA_FOREST_2_TO_KOLIMA_FOREST_3 = 1,
  KOLIMA_FOREST_2_TO_KOLIMA_FOREST_1 = 2,
};

/// SceneId_KolimaForest3's entrances and exits.
enum {
  KOLIMA_FOREST_3_FROM_KOLIMA_FOREST_2 = 1,
  KOLIMA_FOREST_3_FROM_KOLIMA_FOREST_FLOODGATE = 2,
};
enum {
  KOLIMA_FOREST_3_TO_KOLIMA_FOREST_FLOODGATE = 1,
  KOLIMA_FOREST_3_TO_KOLIMA_FOREST_2 = 2,
};

/// SceneId_KolimaForestFloodgate's entrances and exits.
enum {
  KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_3 = 1,
  KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE = 2,
};
enum {
  KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_3 = 1,
  KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE = 2,
};

/// SceneId_KolimaForestTree's entrances and exits.
enum {
  KOLIMA_FOREST_TREE_FROM_KOLIMA_FOREST_FLOODGATE = 1,
  KOLIMA_FOREST_TREE_FROM_TRET_TREE_INTERIORS = 2,
};
enum {
  KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE = 1,
  KOLIMA_FOREST_TREE_TO_TRET_TREE_INTERIORS = 2,
};

/// SceneId_TretTreeInteriors's entrances and exits.
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
  TRET_TREE_INTERIORS_AFTER_FIGHT_TRET = 19, ///< Event_FightTret
};
enum {
  TRET_TREE_INTERIORS_TO_KOLIMA_FOREST_TREE = 1,
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST = 2,
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST = 3,
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH = 4,
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTHEAST = 5,
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTHWEST = 6,
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH = 7,
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST = 8,
  TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST = 9,
};

/// SceneId_TretTreeBranch's entrances and exits.
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

/// SceneId_BilibinCaveIce's entrances and exits.
enum {
  BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_WEST = 1,
  BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_EAST = 2,
  BILIBIN_CAVE_ICE_FROM_WORLD_MAP = 3,
  BILIBIN_CAVE_ICE_FROM_CHESTS = 4,
  BILIBIN_CAVE_ICE_CHESTS_FROM_BILIBIN_CAVE_ICE =
      5, ///< ★ Named from what the player sees: the small nook beyond the north
         ///< doorway that holds two chests (its render)
};
enum {
  BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST = 1,
  BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST = 2,
  BILIBIN_CAVE_ICE_TO_WORLD_MAP = 3,
  BILIBIN_CAVE_ICE_TO_CHESTS =
      4, ///< ★ Named from what the player sees: the small nook beyond the north
         ///< doorway that holds two chests (its render)
  BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE = 5,
};

/// SceneId_BilibinCaveRapier's entrances and exits.
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

/// SceneId_BilibinCaveEntrance's entrances and exits.
enum {
  BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP = 1,
  BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_SOUTH = 2,
  BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_NORTH = 3,
};
enum {
  BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP = 1,
  BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH = 2,
  BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH = 3,
};

/// SceneId_Imil's entrances and exits.
enum {
  IMIL_FROM_HOUSE_SOUTHWEST = 1,
  IMIL_FROM_HOUSE_WEST = 3,
  IMIL_FROM_HOUSE_NORTHEAST = 4,
  IMIL_FROM_IMIL_INTERIORS_EAST = 5,
  IMIL_FROM_IMIL_INTERIORS_NORTHEAST = 6,
  IMIL_FROM_IMIL_INTERIORS_WEST = 7,
  IMIL_FROM_IMIL_INTERIORS_SOUTHWEST = 8,
  IMIL_FROM_IMIL_FALLS_CAVE = 9,
};
enum {
  IMIL_TO_HOUSE_SOUTHWEST = 1,
  IMIL_TO_IMIL_INTERIORS_2 = 2,
  IMIL_TO_HOUSE_WEST = 3,
  IMIL_TO_HOUSE_NORTHEAST = 4,
  IMIL_TO_IMIL_INTERIORS_EAST = 5,
  IMIL_TO_IMIL_INTERIORS_NORTHEAST = 6,
  IMIL_TO_IMIL_FALLS_CAVE = 8,
  IMIL_TO_WORLD_MAP_SOUTH = 11,
  IMIL_TO_WORLD_MAP_NORTH = 12,
};

/// SceneId_ImilInteriors's entrances and exits.
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
  IMIL_INTERIORS_FROM_IMIL_SOUTH_SOUTHWEST = 5,
  IMIL_INTERIORS_FROM_IMIL_EAST_NORTHEAST = 7,
};
enum {
  IMIL_INTERIORS_HOUSE_WEST_TO_IMIL = 1,
  IMIL_INTERIORS_TO_IMIL_WEST_SOUTHWEST = 2,
  IMIL_INTERIORS_TO_IMIL_EAST = 3,
  IMIL_INTERIORS_TO_IMIL = 4,
  IMIL_INTERIORS_TO_IMIL_SOUTH_SOUTHWEST = 5,
  IMIL_INTERIORS_TO_IMIL_EAST_NORTHEAST = 7,
};

/// SceneId_ImilFallsCave's entrances and exits.
enum {
  IMIL_FALLS_CAVE_FROM_IMIL = 1,
};
enum {
  IMIL_FALLS_CAVE_TO_IMIL = 1,
};

/// SceneId_MercuryLighthouseEntrance's entrances and exits.
enum {
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_4 = 1,
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_1_5 = 6,
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_1_WEST = 11,
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_1_WEST_NORTHWEST = 12,
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_1_4 = 14,
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_1_EAST_NORTHEAST = 15,
  MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_1_6 = 25,
};
enum {
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_4 = 1,
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_1_WEST = 10,
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_1_WEST_NORTHWEST = 11,
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_1_SOUTHWEST = 12,
  MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_1_EAST_NORTHEAST = 13,
};

/// SceneId_MercuryLighthouse1's entrances and exits.
enum {
  MERCURY_LIGHTHOUSE_1_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST = 1,
  MERCURY_LIGHTHOUSE_1_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST = 2,
  MERCURY_LIGHTHOUSE_1_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST = 4,
  MERCURY_LIGHTHOUSE_1_FROM_MERCURY_LIGHTHOUSE_2_SOUTH = 6,
  MERCURY_LIGHTHOUSE_1_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_SOUTHWEST = 7,
  MERCURY_LIGHTHOUSE_1_FROM_MERCURY_LIGHTHOUSE_3 = 8,
  MERCURY_LIGHTHOUSE_1_FROM_MERCURY_LIGHTHOUSE_2_NORTH = 11,
};
enum {
  MERCURY_LIGHTHOUSE_1_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST = 1,
  MERCURY_LIGHTHOUSE_1_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST = 2,
  MERCURY_LIGHTHOUSE_1_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST = 4,
  MERCURY_LIGHTHOUSE_1_TO_MERCURY_LIGHTHOUSE_ENTRANCE_4 = 5,
  MERCURY_LIGHTHOUSE_1_TO_MERCURY_LIGHTHOUSE_3 = 6,
  MERCURY_LIGHTHOUSE_1_TO_MERCURY_LIGHTHOUSE_2_NORTH = 7,
  MERCURY_LIGHTHOUSE_1_TO_MERCURY_LIGHTHOUSE_ENTRANCE_5 = 8,
  MERCURY_LIGHTHOUSE_1_TO_MERCURY_LIGHTHOUSE_2_SOUTH = 12,
  MERCURY_LIGHTHOUSE_1_TO_MERCURY_LIGHTHOUSE_ENTRANCE_6 = 13,
};

/// SceneId_MercuryLighthouse2's entrances and exits.
enum {
  MERCURY_LIGHTHOUSE_2_FROM_MERCURY_LIGHTHOUSE_1_SOUTH = 1,
  MERCURY_LIGHTHOUSE_2_FROM_MERCURY_LIGHTHOUSE_1_NORTH = 3,
  MERCURY_LIGHTHOUSE_2_FROM_MERCURY_LIGHTHOUSE_3_WEST_NORTHWEST = 5,
  MERCURY_LIGHTHOUSE_2_FROM_MERCURY_LIGHTHOUSE_4 = 6,
  MERCURY_LIGHTHOUSE_2_FROM_MERCURY_LIGHTHOUSE_3_NORTHEAST = 7,
  MERCURY_LIGHTHOUSE_2_FROM_MERCURY_LIGHTHOUSE_3_EAST_NORTHEAST = 8,
  MERCURY_LIGHTHOUSE_2_FROM_MERCURY_LIGHTHOUSE_3_NORTH_NORTHEAST = 9,
  MERCURY_LIGHTHOUSE_2_FROM_MERCURY_LIGHTHOUSE_3_NORTHWEST = 10,
};
enum {
  MERCURY_LIGHTHOUSE_2_TO_MERCURY_LIGHTHOUSE_1_NORTH = 1,
  MERCURY_LIGHTHOUSE_2_TO_MERCURY_LIGHTHOUSE_3_WEST_NORTHWEST = 3,
  MERCURY_LIGHTHOUSE_2_TO_MERCURY_LIGHTHOUSE_4 = 4,
  MERCURY_LIGHTHOUSE_2_TO_MERCURY_LIGHTHOUSE_3_NORTHWEST = 5,
  MERCURY_LIGHTHOUSE_2_TO_MERCURY_LIGHTHOUSE_1_SOUTH = 6,
  MERCURY_LIGHTHOUSE_2_TO_MERCURY_LIGHTHOUSE_3_NORTHEAST = 8,
  MERCURY_LIGHTHOUSE_2_TO_MERCURY_LIGHTHOUSE_3_EAST_NORTHEAST = 9,
  MERCURY_LIGHTHOUSE_2_TO_MERCURY_LIGHTHOUSE_3_NORTH_NORTHEAST = 10,
};

/// SceneId_MercuryLighthouse3's entrances and exits.
enum {
  MERCURY_LIGHTHOUSE_3_FROM_MERCURY_LIGHTHOUSE_2_WEST_NORTHWEST = 1,
  MERCURY_LIGHTHOUSE_3_FROM_MERCURY_LIGHTHOUSE_2_NORTH_NORTHEAST = 2,
  MERCURY_LIGHTHOUSE_3_FROM_MERCURY_LIGHTHOUSE_2_NORTHWEST = 3,
  MERCURY_LIGHTHOUSE_3_FROM_MERCURY_LIGHTHOUSE_2_NORTHEAST = 5,
  MERCURY_LIGHTHOUSE_3_FROM_MERCURY_LIGHTHOUSE_2_EAST_NORTHEAST = 6,
  MERCURY_LIGHTHOUSE_3_FROM_MERCURY_LIGHTHOUSE_1 = 7,
  MERCURY_LIGHTHOUSE_3_FROM_MERCURY_LIGHTHOUSE_4_EAST = 10,
  MERCURY_LIGHTHOUSE_3_FROM_MERCURY_LIGHTHOUSE_4_WEST = 12,
};
enum {
  MERCURY_LIGHTHOUSE_3_TO_MERCURY_LIGHTHOUSE_2_WEST_NORTHWEST = 1,
  MERCURY_LIGHTHOUSE_3_TO_MERCURY_LIGHTHOUSE_2_NORTH_NORTHEAST = 2,
  MERCURY_LIGHTHOUSE_3_TO_MERCURY_LIGHTHOUSE_2_NORTHWEST = 3,
  MERCURY_LIGHTHOUSE_3_TO_MERCURY_LIGHTHOUSE_2_NORTHEAST = 5,
  MERCURY_LIGHTHOUSE_3_TO_MERCURY_LIGHTHOUSE_2_EAST_NORTHEAST = 6,
  MERCURY_LIGHTHOUSE_3_TO_MERCURY_LIGHTHOUSE_1 = 7,
  MERCURY_LIGHTHOUSE_3_TO_MERCURY_LIGHTHOUSE_4_EAST = 10,
  MERCURY_LIGHTHOUSE_3_TO_MERCURY_LIGHTHOUSE_4_WEST = 12,
};

/// SceneId_MercuryLighthouse4's entrances and exits.
enum {
  MERCURY_LIGHTHOUSE_4_FROM_MERCURY_LIGHTHOUSE_2 = 1,
  MERCURY_LIGHTHOUSE_4_FROM_MERCURY_LIGHTHOUSE_3_SOUTH = 2,
  MERCURY_LIGHTHOUSE_4_FROM_MERCURY_LIGHTHOUSE_3_NORTH = 3,
  MERCURY_LIGHTHOUSE_4_FROM_WORLD_MAP = 11,
  MERCURY_LIGHTHOUSE_4_FROM_MERCURY_LIGHTHOUSE_ENTRANCE = 12,
  MERCURY_LIGHTHOUSE_4_FROM_MERCURY_LIGHTHOUSE_AERIE_1 = 15,
};
enum {
  MERCURY_LIGHTHOUSE_4_TO_MERCURY_LIGHTHOUSE_2 = 1,
  MERCURY_LIGHTHOUSE_4_TO_MERCURY_LIGHTHOUSE_3_SOUTH = 2,
  MERCURY_LIGHTHOUSE_4_TO_MERCURY_LIGHTHOUSE_3_NORTH = 3,
  MERCURY_LIGHTHOUSE_4_TO_WORLD_MAP_EAST = 6,
  MERCURY_LIGHTHOUSE_4_TO_MERCURY_LIGHTHOUSE_ENTRANCE = 7,
  MERCURY_LIGHTHOUSE_4_TO_WORLD_MAP_WEST = 8,
  MERCURY_LIGHTHOUSE_4_TO_MERCURY_LIGHTHOUSE_AERIE_1 = 9,
};

/// SceneId_MercuryLighthouseAerie1's entrances and exits.
enum {
  MERCURY_LIGHTHOUSE_AERIE_1_AFTER_BEACON_LIT =
      1, ///< the world map sends the party here after its lighthouse cut
  MERCURY_LIGHTHOUSE_AERIE_1_AFTER_CONFRONT_SATUROS =
      2, ///< MercuryLighthouseAerie_ConfrontSaturos
  MERCURY_LIGHTHOUSE_AERIE_1_FROM_MERCURY_LIGHTHOUSE_4 = 5,
};
enum {
  MERCURY_LIGHTHOUSE_AERIE_1_TO_MERCURY_LIGHTHOUSE_4 = 1,
};

/// SceneId_FuchinTemple1's entrances and exits.
enum {
  FUCHIN_TEMPLE_1_FROM_WORLD_MAP = 1,
  FUCHIN_TEMPLE_1_FROM_FUCHIN_TEMPLE_2 = 2,
  FUCHIN_TEMPLE_1_FROM_FUCHIN_FALLS_CAVE_1 = 3,
};
enum {
  FUCHIN_TEMPLE_1_TO_WORLD_MAP = 1,
  FUCHIN_TEMPLE_1_TO_FUCHIN_TEMPLE_2 = 2,
  FUCHIN_TEMPLE_1_TO_FUCHIN_FALLS_CAVE_1 = 3,
};

/// SceneId_FuchinTemple2's entrances and exits.
enum {
  FUCHIN_TEMPLE_2_FROM_FUCHIN_TEMPLE_1 = 1,
  FUCHIN_TEMPLE_2_FROM_XIAN = 2,
  FUCHIN_TEMPLE_2_FROM_LAMA_TEMPLE = 3,
  FUCHIN_TEMPLE_2_FROM_HOUSE = 4,
};
enum {
  FUCHIN_TEMPLE_2_TO_FUCHIN_TEMPLE_1 = 1,
  FUCHIN_TEMPLE_2_TO_XIAN = 2,
  FUCHIN_TEMPLE_2_TO_LAMA_TEMPLE = 3,
  FUCHIN_TEMPLE_2_TO_HOUSE = 4,
};

/// SceneId_FuchinFallsCave1's entrances and exits.
enum {
  FUCHIN_FALLS_CAVE_1_FROM_FUCHIN_TEMPLE_1 = 1,
  FUCHIN_FALLS_CAVE_1_FROM_FUCHIN_FALLS_CAVE_2 = 2,
  FUCHIN_FALLS_CAVE_1_FROM_FUCHIN_FALLS_CAVE_4 = 3,
  FUCHIN_FALLS_CAVE_1_FROM_FUCHIN_FALLS_CAVE_3 = 6,
  FUCHIN_FALLS_CAVE_1_FROM_FUCHIN_FALLS_CAVE_6_WEST = 7,
  FUCHIN_FALLS_CAVE_1_FROM_FUCHIN_FALLS_CAVE_6_EAST = 8,
};
enum {
  FUCHIN_FALLS_CAVE_1_TO_FUCHIN_TEMPLE_1 = 1,
  FUCHIN_FALLS_CAVE_1_TO_FUCHIN_FALLS_CAVE_2 = 2,
  FUCHIN_FALLS_CAVE_1_TO_FUCHIN_FALLS_CAVE_4 = 3,
  FUCHIN_FALLS_CAVE_1_TO_FUCHIN_FALLS_CAVE_3 = 6,
  FUCHIN_FALLS_CAVE_1_TO_FUCHIN_FALLS_CAVE_6_WEST = 7,
  FUCHIN_FALLS_CAVE_1_TO_FUCHIN_FALLS_CAVE_6_EAST = 8,
};

/// SceneId_FuchinFallsCave2's entrances and exits.
enum {
  FUCHIN_FALLS_CAVE_2_FROM_FUCHIN_FALLS_CAVE_3 = 1,
  FUCHIN_FALLS_CAVE_2_FROM_FUCHIN_FALLS_CAVE_1 = 2,
};
enum {
  FUCHIN_FALLS_CAVE_2_TO_FUCHIN_FALLS_CAVE_3 = 1,
  FUCHIN_FALLS_CAVE_2_TO_FUCHIN_FALLS_CAVE_1 = 2,
};

/// SceneId_FuchinFallsCave3's entrances and exits.
enum {
  FUCHIN_FALLS_CAVE_3_FROM_FUCHIN_FALLS_CAVE_6 = 2,
  FUCHIN_FALLS_CAVE_3_FROM_FUCHIN_FALLS_CAVE_1 = 4,
  FUCHIN_FALLS_CAVE_3_FROM_FUCHIN_FALLS_CAVE_2 = 5,
};
enum {
  FUCHIN_FALLS_CAVE_3_TO_FUCHIN_FALLS_CAVE_6 = 2,
  FUCHIN_FALLS_CAVE_3_TO_FUCHIN_FALLS_CAVE_1 = 4,
  FUCHIN_FALLS_CAVE_3_TO_FUCHIN_FALLS_CAVE_2 = 5,
};

/// SceneId_FuchinFallsCave4's entrances and exits.
enum {
  FUCHIN_FALLS_CAVE_4_FROM_FUCHIN_FALLS_CAVE_6 = 1,
  FUCHIN_FALLS_CAVE_4_FROM_FUCHIN_FALLS_CAVE_1 = 4,
};
enum {
  FUCHIN_FALLS_CAVE_4_TO_FUCHIN_FALLS_CAVE_6 = 1,
  FUCHIN_FALLS_CAVE_4_TO_FUCHIN_FALLS_CAVE_1 = 4,
};

/// SceneId_FuchinFallsCave5's entrances and exits.
enum {
  FUCHIN_FALLS_CAVE_5_FROM_FUCHIN_FALLS_CAVE_6_WEST = 3,
  FUCHIN_FALLS_CAVE_5_FROM_FUCHIN_FALLS_CAVE_6_EAST = 4,
};
enum {
  FUCHIN_FALLS_CAVE_5_TO_FUCHIN_FALLS_CAVE_6_WEST = 3,
  FUCHIN_FALLS_CAVE_5_TO_FUCHIN_FALLS_CAVE_6_EAST = 4,
};

/// SceneId_FuchinFallsCave6's entrances and exits.
enum {
  FUCHIN_FALLS_CAVE_6_FROM_FUCHIN_FALLS_CAVE_5_EAST = 1,
  FUCHIN_FALLS_CAVE_6_FROM_FUCHIN_FALLS_CAVE_5_WEST = 2,
  FUCHIN_FALLS_CAVE_6_FROM_FUCHIN_FALLS_CAVE_4 = 3,
  FUCHIN_FALLS_CAVE_6_FROM_FUCHIN_FALLS_CAVE_3 = 10,
  FUCHIN_FALLS_CAVE_6_FROM_FUCHIN_FALLS_CAVE_1_NORTH = 11,
  FUCHIN_FALLS_CAVE_6_FROM_FUCHIN_FALLS_CAVE_1_SOUTH = 12,
};
enum {
  FUCHIN_FALLS_CAVE_6_TO_FUCHIN_FALLS_CAVE_5_WEST = 1,
  FUCHIN_FALLS_CAVE_6_TO_FUCHIN_FALLS_CAVE_5_EAST = 2,
  FUCHIN_FALLS_CAVE_6_TO_FUCHIN_FALLS_CAVE_4 = 3,
  FUCHIN_FALLS_CAVE_6_TO_FUCHIN_FALLS_CAVE_3 = 10,
  FUCHIN_FALLS_CAVE_6_TO_FUCHIN_FALLS_CAVE_1_NORTH = 11,
  FUCHIN_FALLS_CAVE_6_TO_FUCHIN_FALLS_CAVE_1_SOUTH = 12,
};

/// SceneId_MogallForest1's entrances and exits.
enum {
  MOGALL_FOREST_1_FROM_WORLD_MAP = 1,
  MOGALL_FOREST_1_FROM_MOGALL_FOREST_4_WEST = 2,
  MOGALL_FOREST_1_FROM_MOGALL_FOREST_4_NORTH = 3,
  MOGALL_FOREST_1_FROM_MOGALL_FOREST_4_EAST = 8,
  MOGALL_FOREST_1_FROM_MOGALL_FOREST_2 = 9,
};
enum {
  MOGALL_FOREST_1_TO_WORLD_MAP = 1,
  MOGALL_FOREST_1_TO_MOGALL_FOREST_4_WEST = 2,
  MOGALL_FOREST_1_TO_MOGALL_FOREST_4_NORTH = 3,
  MOGALL_FOREST_1_TO_MOGALL_FOREST_4_EAST = 8,
  MOGALL_FOREST_1_TO_MOGALL_FOREST_2 = 9,
};

/// SceneId_MogallForest2's entrances and exits.
enum {
  MOGALL_FOREST_2_FROM_MOGALL_FOREST_1 = 1,
  MOGALL_FOREST_2_FROM_MOGALL_FOREST_4_NORTH = 3,
  MOGALL_FOREST_2_FROM_MOGALL_FOREST_3 = 11,
  MOGALL_FOREST_2_FROM_MOGALL_FOREST_4_SOUTH = 12,
};
enum {
  MOGALL_FOREST_2_TO_MOGALL_FOREST_1 = 1,
  MOGALL_FOREST_2_TO_MOGALL_FOREST_4_NORTH = 3,
  MOGALL_FOREST_2_TO_MOGALL_FOREST_3 = 11,
  MOGALL_FOREST_2_TO_MOGALL_FOREST_4_SOUTH = 12,
};

/// SceneId_MogallForest3's entrances and exits.
enum {
  MOGALL_FOREST_3_FROM_MOGALL_FOREST_2 = 2,
  MOGALL_FOREST_3_FROM_MOGALL_FOREST_4_NORTH = 3,
  MOGALL_FOREST_3_FROM_MOGALL_FOREST_4_EAST = 9,
  MOGALL_FOREST_3_FROM_MOGALL_FOREST_4_SOUTH = 10,
  MOGALL_FOREST_3_FROM_WORLD_MAP = 13,
  MOGALL_FOREST_3_AFTER_KILLER_APE_ATTACKS =
      15, ///< MogallForest_KillerApeAttacks
};
enum {
  MOGALL_FOREST_3_TO_MOGALL_FOREST_2 = 2,
  MOGALL_FOREST_3_TO_MOGALL_FOREST_4_WEST = 3,
  MOGALL_FOREST_3_TO_MOGALL_FOREST_4_NORTH = 9,
  MOGALL_FOREST_3_TO_MOGALL_FOREST_4_SOUTH = 10,
  MOGALL_FOREST_3_TO_WORLD_MAP = 13,
};

/// SceneId_MogallForest4's entrances and exits.
enum {
  MOGALL_FOREST_4_FROM_MOGALL_FOREST_1_NORTH = 1,
  MOGALL_FOREST_4_FROM_MOGALL_FOREST_1_WEST = 2,
  MOGALL_FOREST_4_FROM_MOGALL_FOREST_2_WEST = 3,
  MOGALL_FOREST_4_FROM_MOGALL_FOREST_2_EAST = 4,
  MOGALL_FOREST_4_FROM_MOGALL_FOREST_3_WEST = 5,
  MOGALL_FOREST_4_FROM_MOGALL_FOREST_3_NORTH = 6,
  MOGALL_FOREST_4_FROM_MOGALL_FOREST_3_EAST = 7,
  MOGALL_FOREST_4_FROM_MOGALL_FOREST_1_SOUTH = 8,
};
enum {
  MOGALL_FOREST_4_TO_MOGALL_FOREST_1_NORTH = 1,
  MOGALL_FOREST_4_TO_MOGALL_FOREST_1_WEST = 2,
  MOGALL_FOREST_4_TO_MOGALL_FOREST_2_WEST = 3,
  MOGALL_FOREST_4_TO_MOGALL_FOREST_2_EAST = 4,
  MOGALL_FOREST_4_TO_MOGALL_FOREST_3_WEST = 5,
  MOGALL_FOREST_4_TO_MOGALL_FOREST_3_NORTH = 6,
  MOGALL_FOREST_4_TO_MOGALL_FOREST_3_EAST = 7,
  MOGALL_FOREST_4_TO_MOGALL_FOREST_1_SOUTH = 8,
};

/// SceneId_Xian's entrances and exits.
enum {
  XIAN_FROM_HOUSE = 1,
  XIAN_FROM_WEAPON_AND_ARMOR_SHOP = 5,
  XIAN_FROM_SANCTUM_EAST = 6,
  XIAN_FROM_XIAN_INTERIORS_EAST = 7,
  XIAN_FROM_XIAN_INTERIORS_SOUTH = 8,
  XIAN_FROM_XIAN_INTERIORS_WEST = 9,
  XIAN_FROM_FUCHIN_TEMPLE_2 = 10,
  XIAN_FROM_SANCTUM_WEST = 11,
};
enum {
  XIAN_TO_HOUSE = 1,
  XIAN_TO_HSUS_HOUSE = 2,
  XIAN_TO_XIAN_INTERIORS_EAST = 3,
  XIAN_TO_INN = 4,
  XIAN_TO_WEAPON_AND_ARMOR_SHOP = 5,
  XIAN_TO_SANCTUM = 6,
  XIAN_TO_FUCHIN_TEMPLE_2 = 7,
  XIAN_TO_WORLD_MAP = 8,
};

/// SceneId_XianInteriors's entrances and exits.
enum {
  XIAN_INTERIORS_HOUSE =
      1, ///< the room behind Xian's house door; through XIAN_TO_HOUSE
  XIAN_INTERIORS_HSUS_HOUSE =
      2, ///< the room holds the hsus house; through XIAN_TO_HSUS_HOUSE
  XIAN_INTERIORS_FROM_XIAN_EAST = 3,
  XIAN_INTERIORS_INN_SOUTH = 4, ///< the room holds the inn; through XIAN_TO_INN
  XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP =
      6, ///< the room behind Xian's weapon and armor shop door; the room holds
         ///< the weapon and armor shop; through XIAN_TO_WEAPON_AND_ARMOR_SHOP
  XIAN_INTERIORS_INN_NORTH = 7, ///< the room holds the inn;
  XIAN_INTERIORS_FROM_FUCHIN_TEMPLE_2 =
      8, ///< the room behind FuchinTemple2's house door; through
         ///< FUCHIN_TEMPLE_2_TO_HOUSE
};
enum {
  XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_XIAN = 1,
  XIAN_INTERIORS_TO_XIAN_EAST = 3,
  XIAN_INTERIORS_TO_XIAN_SOUTH = 4,
  XIAN_INTERIORS_TO_XIAN_WEST = 6,
  XIAN_INTERIORS_HOUSE_TO_FUCHIN_TEMPLE_2 = 8,
};

/// SceneId_AlpineCrossing's entrances and exits.
enum {
  ALPINE_CROSSING_FROM_WORLD_MAP_EAST = 1,
  ALPINE_CROSSING_FROM_WORLD_MAP_NORTH = 2,
  ALPINE_CROSSING_FROM_WORLD_MAP_WEST = 3,
  ALPINE_CROSSING_FROM_ALTIN_PEAK_4_WEST = 4,
  ALPINE_CROSSING_FROM_ALTIN_PEAK_4_EAST = 5,
};
enum {
  ALPINE_CROSSING_TO_WORLD_MAP_EAST = 1,
  ALPINE_CROSSING_TO_WORLD_MAP_NORTH = 2,
  ALPINE_CROSSING_TO_WORLD_MAP_WEST = 3,
  ALPINE_CROSSING_TO_ALTIN_PEAK_4_WEST = 4,
  ALPINE_CROSSING_TO_ALTIN_PEAK_4_EAST = 5,
  ALPINE_CROSSING_TO_LAMA_TEMPLE = 6,
};

/// SceneId_Altin's entrances and exits.
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
  ALTIN_FROM_ALTIN_PEAK_1 = 10,
  ALTIN_FROM_ALTIN_PEAK_4 = 11,
  ALTIN_FROM_ALTIN_PEAK_8 = 12,
  ALTIN_FROM_ALTIN_PEAK_7 = 13,
};
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
  ALTIN_TO_ALTIN_PEAK_1 = 10,
  ALTIN_TO_ALTIN_PEAK_4 = 11,
  ALTIN_TO_ALTIN_PEAK_8 = 12,
  ALTIN_TO_ALTIN_PEAK_7 = 13,
};

/// SceneId_AltinInteriors's entrances and exits.
enum {
  ALTIN_INTERIORS_FROM_ALTIN_NORTHWEST = 1,
  ALTIN_INTERIORS_FROM_ALTIN_NORTH = 2,
  ALTIN_INTERIORS_FROM_ALTIN_NORTHEAST = 3,
  ALTIN_INTERIORS_FROM_ALTIN_WEST = 4,
  ALTIN_INTERIORS_FROM_ALTIN_SOUTHEAST = 5,
  ALTIN_INTERIORS_FROM_ALTIN_EAST = 6,
  ALTIN_INTERIORS_FROM_ALTIN_SOUTHWEST = 7,
};
enum {
  ALTIN_INTERIORS_TO_ALTIN_NORTHWEST = 1,
  ALTIN_INTERIORS_TO_ALTIN_NORTH = 2,
  ALTIN_INTERIORS_TO_ALTIN_NORTHEAST = 3,
  ALTIN_INTERIORS_TO_ALTIN_WEST = 4,
  ALTIN_INTERIORS_TO_ALTIN_SOUTHEAST = 5,
  ALTIN_INTERIORS_TO_ALTIN_EAST = 6,
  ALTIN_INTERIORS_TO_ALTIN_SOUTHWEST = 7,
};

/// SceneId_AltinPeak1's entrances and exits.
enum {
  ALTIN_PEAK_1_FROM_ALTIN = 1,
};
enum {
  ALTIN_PEAK_1_TO_ALTIN = 1,
};

/// SceneId_AltinPeak2's entrances and exits.
enum {
  ALTIN_PEAK_2_FROM_ALTIN_PEAK_4 = 1,
  ALTIN_PEAK_2_FROM_ALTIN_PEAK_3 = 2,
};
enum {
  ALTIN_PEAK_2_TO_ALTIN_PEAK_4 = 1,
  ALTIN_PEAK_2_TO_ALTIN_PEAK_3 = 2,
};

/// SceneId_AltinPeak3's entrances and exits.
enum {
  ALTIN_PEAK_3_FROM_ALTIN_PEAK_2 = 1,
};
enum {
  ALTIN_PEAK_3_TO_ALTIN_PEAK_2 = 1,
};

/// SceneId_AltinPeak4's entrances and exits.
enum {
  ALTIN_PEAK_4_FROM_ALTIN = 1,
  ALTIN_PEAK_4_FROM_ALTIN_PEAK_2 = 3,
  ALTIN_PEAK_4_FROM_ALTIN_PEAK_6 = 5,
  ALTIN_PEAK_4_FROM_ALPINE_CROSSING_WEST = 6,
  ALTIN_PEAK_4_FROM_ALPINE_CROSSING_EAST = 7,
};
enum {
  ALTIN_PEAK_4_TO_ALTIN = 1,
  ALTIN_PEAK_4_TO_ALTIN_PEAK_2 = 3,
  ALTIN_PEAK_4_TO_ALTIN_PEAK_6 = 5,
  ALTIN_PEAK_4_TO_ALPINE_CROSSING_WEST = 6,
  ALTIN_PEAK_4_TO_ALPINE_CROSSING_EAST = 7,
};

/// SceneId_AltinPeak5's entrances and exits.
enum {
  ALTIN_PEAK_5_FROM_ALTIN_PEAK_6 = 1,
};
enum {
  ALTIN_PEAK_5_TO_ALTIN_PEAK_6 = 1,
};

/// SceneId_AltinPeak6's entrances and exits.
enum {
  ALTIN_PEAK_6_FROM_ALTIN_PEAK_4 = 1,
  ALTIN_PEAK_6_FROM_ALTIN_PEAK_5 = 6,
};
enum {
  ALTIN_PEAK_6_TO_ALTIN_PEAK_4 = 1,
  ALTIN_PEAK_6_TO_ALTIN_PEAK_5 = 6,
};

/// SceneId_AltinPeak7's entrances and exits.
enum {
  ALTIN_PEAK_7_FROM_ALTIN_PEAK_8 = 1,
  ALTIN_PEAK_7_FROM_ALTIN_PEAK_9 = 2,
  ALTIN_PEAK_7_FROM_ALTIN = 3,
};
enum {
  ALTIN_PEAK_7_TO_ALTIN_PEAK_8 = 1,
  ALTIN_PEAK_7_TO_ALTIN_PEAK_9 = 2,
  ALTIN_PEAK_7_TO_ALTIN = 3,
};

/// SceneId_AltinPeak8's entrances and exits.
enum {
  ALTIN_PEAK_8_FROM_ALTIN_PEAK_7 = 1,
  ALTIN_PEAK_8_FROM_ALTIN_PEAK_9_WEST = 2,
  ALTIN_PEAK_8_FROM_ALTIN_PEAK_11 = 3,
  ALTIN_PEAK_8_FROM_ALTIN = 4,
  ALTIN_PEAK_8_FROM_ALTIN_PEAK_9_EAST = 5,
};
enum {
  ALTIN_PEAK_8_TO_ALTIN_PEAK_7 = 1,
  ALTIN_PEAK_8_TO_ALTIN_PEAK_9_WEST = 2,
  ALTIN_PEAK_8_TO_ALTIN_PEAK_11 = 3,
  ALTIN_PEAK_8_TO_ALTIN = 4,
  ALTIN_PEAK_8_TO_ALTIN_PEAK_9_EAST = 5,
};

/// SceneId_AltinPeak9's entrances and exits.
enum {
  ALTIN_PEAK_9_FROM_ALTIN_PEAK_7 = 1,
  ALTIN_PEAK_9_FROM_ALTIN_PEAK_8_SOUTH = 3,
  ALTIN_PEAK_9_FROM_ALTIN_PEAK_8_NORTH = 5,
  ALTIN_PEAK_9_FROM_ALTIN_PEAK_10 = 6,
};
enum {
  ALTIN_PEAK_9_TO_ALTIN_PEAK_7 = 1,
  ALTIN_PEAK_9_TO_ALTIN_PEAK_8_SOUTH = 3,
  ALTIN_PEAK_9_TO_ALTIN_PEAK_8_NORTH = 5,
  ALTIN_PEAK_9_TO_ALTIN_PEAK_10 = 6,
};

/// SceneId_AltinPeak10's entrances and exits.
enum {
  ALTIN_PEAK_10_FROM_ALTIN_PEAK_9 = 1,
};
enum {
  ALTIN_PEAK_10_TO_ALTIN_PEAK_9 = 1,
};

/// SceneId_AltinPeak11's entrances and exits.
enum {
  ALTIN_PEAK_11_FROM_ALTIN_PEAK_8 = 1,
  ALTIN_PEAK_11_FROM_WORLD_MAP = 4,
};
enum {
  ALTIN_PEAK_11_TO_ALTIN_PEAK_8 = 1,
  ALTIN_PEAK_11_TO_WORLD_MAP = 4,
};

/// SceneId_LamaTemple's entrances and exits.
enum {
  LAMA_TEMPLE_FROM_WORLD_MAP = 1,
  LAMA_TEMPLE_FROM_FUCHIN_TEMPLE_2 = 2,
  LAMA_TEMPLE_FROM_ALPINE_CROSSING = 3,
};
enum {
  LAMA_TEMPLE_TO_WORLD_MAP = 1,
  LAMA_TEMPLE_TO_FUCHIN_TEMPLE_2 = 2,
};

/// SceneId_LamakanDesert1's entrances and exits.
enum {
  LAMAKAN_DESERT_1_FROM_WORLD_MAP = 1,
  LAMAKAN_DESERT_1_FROM_LAMAKAN_DESERT_2 = 2,
};
enum {
  LAMAKAN_DESERT_1_TO_WORLD_MAP = 1,
  LAMAKAN_DESERT_1_TO_LAMAKAN_DESERT_2 = 2,
};

/// SceneId_LamakanDesert2's entrances and exits.
enum {
  LAMAKAN_DESERT_2_FROM_LAMAKAN_DESERT_1 = 1,
  LAMAKAN_DESERT_2_FROM_LAMAKAN_DESERT_3 = 2,
};
enum {
  LAMAKAN_DESERT_2_TO_LAMAKAN_DESERT_1 = 1,
  LAMAKAN_DESERT_2_TO_LAMAKAN_DESERT_3 = 2,
};

/// SceneId_LamakanDesert3's entrances and exits.
enum {
  LAMAKAN_DESERT_3_FROM_LAMAKAN_DESERT_2 = 1,
  LAMAKAN_DESERT_3_FROM_LAMAKAN_DESERT_4_EAST = 2,
  LAMAKAN_DESERT_3_FROM_LAMAKAN_DESERT_4_SOUTH = 3,
  LAMAKAN_DESERT_3_FROM_LAMAKAN_DESERT_4_WEST = 4,
  LAMAKAN_DESERT_3_AFTER_FIGHT_MANTICORE = 5, ///< FightManticore
};
enum {
  LAMAKAN_DESERT_3_TO_LAMAKAN_DESERT_2 = 1,
  LAMAKAN_DESERT_3_TO_LAMAKAN_DESERT_4_EAST = 2,
  LAMAKAN_DESERT_3_TO_LAMAKAN_DESERT_4_SOUTH = 3,
  LAMAKAN_DESERT_3_TO_LAMAKAN_DESERT_4_WEST = 4,
};

/// SceneId_LamakanDesert4's entrances and exits.
enum {
  LAMAKAN_DESERT_4_FROM_LAMAKAN_DESERT_3_SOUTH = 1,
  LAMAKAN_DESERT_4_FROM_LAMAKAN_DESERT_3_SOUTHEAST = 2,
  LAMAKAN_DESERT_4_FROM_LAMAKAN_DESERT_3_NORTH = 3,
  LAMAKAN_DESERT_4_FROM_WORLD_MAP = 4,
};
enum {
  LAMAKAN_DESERT_4_TO_LAMAKAN_DESERT_3_EAST = 1,
  LAMAKAN_DESERT_4_TO_LAMAKAN_DESERT_3_SOUTHEAST = 2,
  LAMAKAN_DESERT_4_TO_WORLD_MAP = 3,
  LAMAKAN_DESERT_4_TO_LAMAKAN_DESERT_3_WEST = 4,
};

/// SceneId_ValeCaveMouth's entrances and exits.
enum {
  VALE_CAVE_MOUTH_FROM_VALE_KRADEN_WEST = 1,
  VALE_CAVE_MOUTH_FROM_VALE_CAVE_KITE = 2,
  VALE_CAVE_MOUTH_FROM_VALE_KRADEN_EAST = 3,
};
enum {
  VALE_CAVE_MOUTH_TO_VALE_KRADEN_WEST = 1,
  VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE = 2,
  VALE_CAVE_MOUTH_TO_VALE_KRADEN_EAST = 3,
};

/// SceneId_ValeCaveKite's entrances and exits.
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

/// SceneId_ValeCaveLogs's entrances and exits.
enum {
  VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST = 1,
  VALE_CAVE_LOGS_NORTH_FROM_SOUTH = 2,
  VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST = 3,
  VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_SOUTH = 4,
  VALE_CAVE_LOGS_SOUTH_FROM_NORTH = 5,
};
enum {
  VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST = 1,
  VALE_CAVE_LOGS_NORTH_TO_SOUTH = 2,
  VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST = 3,
  VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH = 4,
  VALE_CAVE_LOGS_SOUTH_TO_NORTH = 5,
};

/// SceneId_VaultCavePassage's entrances and exits.
enum {
  VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_MOUTH = 1,
  VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_NORTHWEST = 2,
  VAULT_CAVE_PASSAGE_NORTH_FROM_POOL_WEST = 3,
  VAULT_CAVE_PASSAGE_NORTH_FROM_POOL_EAST = 4,
  VAULT_CAVE_PASSAGE_POOL_FROM_NORTH_WEST =
      5, ///< ★ Named from what the player sees: the room of water where the
         ///< switch stands (its render)
  VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST = 6,
  VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST = 7,
  VAULT_CAVE_PASSAGE_POOL_FROM_CHEST =
      8, ///< ★ Named from what the player sees: the room of water where the
         ///< switch stands (its render)
  VAULT_CAVE_PASSAGE_CHEST_FROM_POOL =
      9, ///< ★ Named from what the player sees: the room holding the cave's one
         ///< chest (its render)
  VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT = 10,
  VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST =
      11, ///< ★ Named from what the player sees: the ladder down from Vault's
          ///< hidden passage (its render)
  VAULT_CAVE_PASSAGE_CHEST_FROM_LADDER =
      12, ///< ★ Named from what the player sees: the room holding the cave's
          ///< one chest (its render)
  VAULT_CAVE_PASSAGE_POOL_FROM_NORTH_EAST =
      13, ///< ★ Named from what the player sees: the room of water where the
          ///< switch stands (its render)
};
enum {
  VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_MOUTH = 1,
  VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_NORTHWEST = 2,
  VAULT_CAVE_PASSAGE_NORTH_TO_POOL_WEST =
      3, ///< ★ Named from what the player sees: the room of water where the
         ///< switch stands (its render)
  VAULT_CAVE_PASSAGE_NORTH_TO_POOL_EAST =
      4, ///< ★ Named from what the player sees: the room of water where the
         ///< switch stands (its render)
  VAULT_CAVE_PASSAGE_POOL_TO_NORTH_WEST = 5,
  VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_EAST = 6,
  VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_SOUTHEAST = 7,
  VAULT_CAVE_PASSAGE_POOL_TO_CHEST =
      8, ///< ★ Named from what the player sees: the room holding the cave's one
         ///< chest (its render)
  VAULT_CAVE_PASSAGE_CHEST_TO_POOL =
      9, ///< ★ Named from what the player sees: the room of water where the
         ///< switch stands (its render)
  VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT = 10,
  VAULT_CAVE_PASSAGE_LADDER_TO_CHEST =
      11, ///< ★ Named from what the player sees: the room holding the cave's
          ///< one chest (its render)
  VAULT_CAVE_PASSAGE_CHEST_TO_LADDER =
      12, ///< ★ Named from what the player sees: the ladder down from Vault's
          ///< hidden passage (its render)
  VAULT_CAVE_PASSAGE_POOL_TO_NORTH_EAST = 13,
};

/// SceneId_VaultCaveVambrace's entrances and exits.
enum {
  VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH = 1,
  VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH = 2,
  VAULT_CAVE_VAMBRACE_NORTH_FROM_PUDDLES = 3,
  VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST = 4,
  VAULT_CAVE_VAMBRACE_WEST_FROM_NORTH = 5,
  VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTHWEST = 6,
  VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_SOUTH = 7,
  VAULT_CAVE_VAMBRACE_PUDDLES_FROM_NORTH =
      8, ///< ★ Named from what the player sees: the room of six puddles that
         ///< Frost freezes, under falling objects (its render)
  VAULT_CAVE_VAMBRACE_PUDDLES_FROM_SOUTH =
      9, ///< ★ Named from what the player sees: the room of six puddles that
         ///< Frost freezes, under falling objects (its render)
  VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH = 10,
  VAULT_CAVE_VAMBRACE_SOUTH_FROM_PUDDLES = 11,
  VAULT_CAVE_VAMBRACE_CHEST_FROM_PUDDLES =
      13, ///< ★ Named from what the player sees: the room behind the bush
          ///< holding the Vambrace chest (its render)
  VAULT_CAVE_VAMBRACE_PUDDLES_FROM_CHEST =
      14, ///< ★ Named from what the player sees: the room of six puddles that
          ///< Frost freezes, under falling objects (its render)
};
enum {
  VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_NORTH = 1,
  VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_NORTH = 2,
  VAULT_CAVE_VAMBRACE_NORTH_TO_PUDDLES =
      3, ///< ★ Named from what the player sees: the room of six puddles that
         ///< Frost freezes, under falling objects (its render)
  VAULT_CAVE_VAMBRACE_NORTH_TO_WEST = 4,
  VAULT_CAVE_VAMBRACE_WEST_TO_NORTH = 5,
  VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTHWEST = 6,
  VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_SOUTH = 7,
  VAULT_CAVE_VAMBRACE_PUDDLES_TO_NORTH = 8,
  VAULT_CAVE_VAMBRACE_PUDDLES_TO_SOUTH = 9,
  VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTH = 10,
  VAULT_CAVE_VAMBRACE_SOUTH_TO_PUDDLES =
      11, ///< ★ Named from what the player sees: the room of six puddles that
          ///< Frost freezes, under falling objects (its render)
  VAULT_CAVE_VAMBRACE_PUDDLES_TO_CHEST =
      13, ///< ★ Named from what the player sees: the room behind the bush
          ///< holding the Vambrace chest (its render)
  VAULT_CAVE_VAMBRACE_CHEST_TO_PUDDLES =
      14, ///< ★ Named from what the player sees: the room of six puddles that
          ///< Frost freezes, under falling objects (its render)
};

/// SceneId_VaultCaveMouth's entrances and exits.
enum {
  VAULT_CAVE_MOUTH_FROM_VAULT_REVISIT = 1,
  VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_PASSAGE = 2,
  VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_SOUTH = 3,
  VAULT_CAVE_MOUTH_SOUTH_FROM_NORTH = 4,
  VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH = 5,
  VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH = 6,
};
enum {
  VAULT_CAVE_MOUTH_TO_VAULT_REVISIT = 1,
  VAULT_CAVE_MOUTH_TO_VAULT_CAVE_PASSAGE = 2,
  VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_SOUTH = 3,
  VAULT_CAVE_MOUTH_SOUTH_TO_NORTH = 4,
  VAULT_CAVE_MOUTH_NORTH_TO_SOUTH = 5,
  VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_NORTH = 6,
};

/// SceneId_Kalay1's entrances and exits.
enum {
  KALAY_1_FROM_KALAY_INTERIORS_1_NORTHWEST = 1,
  KALAY_1_FROM_KALAY_INTERIORS_1_NORTH = 2,
  KALAY_1_FROM_KALAY_INTERIORS_1_NORTHEAST = 3,
  KALAY_1_FROM_KALAY_INTERIORS_1_WEST = 4,
  KALAY_1_FROM_KALAY_INTERIORS_1_SOUTH_SOUTHEAST = 5,
  KALAY_1_FROM_KALAY_INTERIORS_1_EAST_SOUTHEAST = 6,
  KALAY_1_FROM_KALAY_INTERIORS_1_SOUTH_SOUTHWEST = 7,
  KALAY_1_FROM_KALAY_SANCTUM = 8,
  KALAY_1_FROM_WORLD_MAP = 9,
  KALAY_1_FROM_KALAY_2 = 11,
  KALAY_1_FROM_KALAY_INTERIORS_1_WEST_NORTHWEST = 12,
  KALAY_1_FROM_KALAY_INTERIORS_1_EAST = 13,
  KALAY_1_FROM_KALAY_TUNNEL_3 = 14,
};
enum {
  KALAY_1_TO_KALAY_INTERIORS_1_NORTHWEST = 1,
  KALAY_1_TO_KALAY_INTERIORS_1_NORTH = 2,
  KALAY_1_TO_KALAY_INTERIORS_1_NORTHEAST = 3,
  KALAY_1_TO_KALAY_INTERIORS_1_WEST = 4,
  KALAY_1_TO_KALAY_INTERIORS_1_SOUTH_SOUTHEAST = 5,
  KALAY_1_TO_KALAY_INTERIORS_1_EAST_SOUTHEAST = 6,
  KALAY_1_TO_KALAY_INTERIORS_1_SOUTH_SOUTHWEST = 7,
  KALAY_1_TO_KALAY_SANCTUM = 8,
  KALAY_1_TO_WORLD_MAP = 9,
  KALAY_1_TO_WORLD_MAP_WEST = 10,
  KALAY_1_TO_KALAY_2 = 11,
  KALAY_1_TO_KALAY_INTERIORS_1_WEST_NORTHWEST = 12,
  KALAY_1_TO_KALAY_INTERIORS_1_EAST = 13,
  KALAY_1_TO_KALAY_TUNNEL_3 = 14,
};

/// SceneId_KalayInteriors1's entrances and exits.
enum {
  KALAY_INTERIORS_1_FROM_KALAY_1_NORTHWEST = 1,
  KALAY_INTERIORS_1_FROM_KALAY_1_NORTH = 2,
  KALAY_INTERIORS_1_FROM_KALAY_1_NORTHEAST = 3,
  KALAY_INTERIORS_1_FROM_KALAY_1_WEST = 4,
  KALAY_INTERIORS_1_FROM_KALAY_1_WEST_NORTHWEST = 5,
  KALAY_INTERIORS_1_FROM_KALAY_1_SOUTH_SOUTHEAST = 6,
  KALAY_INTERIORS_1_FROM_KALAY_1_EAST_SOUTHEAST = 7,
  KALAY_INTERIORS_1_FROM_KALAY_1_EAST = 8,
  KALAY_INTERIORS_1_FROM_KALAY_1_SOUTH_SOUTHWEST = 10,
};
enum {
  KALAY_INTERIORS_1_TO_KALAY_1_NORTHWEST = 1,
  KALAY_INTERIORS_1_TO_KALAY_1_NORTH = 2,
  KALAY_INTERIORS_1_TO_KALAY_1_NORTHEAST = 3,
  KALAY_INTERIORS_1_TO_KALAY_1_WEST = 4,
  KALAY_INTERIORS_1_TO_KALAY_1_WEST_NORTHWEST = 5,
  KALAY_INTERIORS_1_TO_KALAY_1_SOUTH_SOUTHEAST = 6,
  KALAY_INTERIORS_1_TO_KALAY_1_EAST_SOUTHEAST = 7,
  KALAY_INTERIORS_1_TO_KALAY_1_EAST = 8,
  KALAY_INTERIORS_1_TO_KALAY_1_SOUTH_SOUTHWEST = 10,
};

/// SceneId_KalaySanctum's entrances and exits.
enum {
  KALAY_SANCTUM_FROM_KALAY_1 = 1,
};
enum {
  KALAY_SANCTUM_TO_KALAY_1 = 1,
};

/// SceneId_Kalay2's entrances and exits.
enum {
  KALAY_2_FROM_KALAY_1 = 1,
  KALAY_2_FROM_HAMMET_PALACE = 2,
};
enum {
  KALAY_2_TO_KALAY_1 = 1,
  KALAY_2_TO_HAMMET_PALACE = 2,
  KALAY_2_TO_HAMMET_PALACE_EAST = 5,
};

/// SceneId_HammetPalace's entrances and exits.
enum {
  HAMMET_PALACE_FROM_KALAY_2_WEST = 1,
  HAMMET_PALACE_FROM_KALAY_TUNNEL_3 = 11,
  HAMMET_PALACE_FROM_KALAY_2_EAST = 20,
};
enum {
  HAMMET_PALACE_TO_KALAY_2_WEST = 1,
  HAMMET_PALACE_TO_KALAY_TUNNEL_3 = 11,
};

/// SceneId_Lunpa's entrances and exits.
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
  LUNPA_FROM_EIGHTH_ROOM = 11,
  LUNPA_FROM_VAULT_JAIL = 12,
};
enum {
  LUNPA_TO_WORLD_MAP = 1,
  LUNPA_TO_LUNPA_FORTRESS_GATES = 2,
  LUNPA_TO_WEST_LUNPA_CAVE = 3,
  LUNPA_TO_FIRST_HOME = 4,
  LUNPA_TO_SECOND_HOME = 5,
  LUNPA_TO_THIRD_HOME = 6,
  LUNPA_TO_WEAPON_AND_ARMOR_SHOP = 7,
  LUNPA_TO_ITEM_SHOP = 8,
  LUNPA_TO_INN = 9,
  LUNPA_TO_TEMPLE = 10,
  LUNPA_TO_EIGHTH_ROOM = 11,
  LUNPA_TO_VAULT_JAIL = 12,
};

/// SceneId_LunpaSuhalla's entrances and exits.
enum {
  LUNPA_SUHALLA_FROM_SUHALLA_GATE_1 = 10,
};

/// SceneId_WestLunpaCave's entrances and exits.
enum {
  WEST_LUNPA_CAVE_FROM_WORLD_MAP = 1,
  WEST_LUNPA_CAVE_FROM_LUNPA = 2,
};
enum {
  WEST_LUNPA_CAVE_TO_WORLD_MAP = 1,
  WEST_LUNPA_CAVE_TO_LUNPA = 2,
  WEST_LUNPA_CAVE_BY_WAGON = 3,
};

/// SceneId_KalayDocks's entrances and exits.
enum {
  KALAY_DOCKS_FROM_WORLD_MAP_SOUTH = 1,
  KALAY_DOCKS_FROM_TOLBI_SHIP_DECK = 2,
  KALAY_DOCKS_FROM_KALAY_TOLBI_DOCKS_2 = 3,
  KALAY_DOCKS_FROM_WORLD_MAP_NORTH = 4,
};
enum {
  KALAY_DOCKS_TO_WORLD_MAP_SOUTH = 1,
  KALAY_DOCKS_TO_TOLBI_SHIP_DECK = 2,
  KALAY_DOCKS_TO_KALAY_TOLBI_DOCKS_2 = 3,
  KALAY_DOCKS_TO_WORLD_MAP_NORTH = 4,
};

/// SceneId_KalayTolbiDocks2's entrances and exits.
enum {
  KALAY_TOLBI_DOCKS_2_FROM_KALAY_DOCKS = 1,
  KALAY_TOLBI_DOCKS_2_FROM_TOLBI_DOCKS = 2,
};
enum {
  KALAY_TOLBI_DOCKS_2_TO_KALAY_DOCKS = 1,
  KALAY_TOLBI_DOCKS_2_TO_TOLBI_DOCKS = 2,
};

/// SceneId_TolbiShipDeck's entrances and exits.
enum {
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_2 = 2,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SOUTH = 4,
  TOLBI_SHIP_DECK_FROM_KALAY_DOCKS = 5,
  TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS = 6,
  TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE =
      16, ///< TolbiShipDeck_JumpIntoKrakenBattle
  TOLBI_SHIP_DECK_FROM_WORLD_MAP = 18,
  TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_NORTH = 19,
};
enum {
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS = 2,
  TOLBI_SHIP_DECK_TO_KALAY_DOCKS = 4,
  TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST = 5,
  TOLBI_SHIP_DECK_TO_TOLBI_DOCKS = 6,
};

/// SceneId_TolbiShipMast's entrances and exits.
enum {
  TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK = 1,
};
enum {
  TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_SOUTH = 1,
  TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_NORTH = 15,
};

/// SceneId_TolbiShipInteriors's entrances and exits.
enum {
  TOLBI_SHIP_INTERIORS_FROM_TOLBI_SHIP_DECK_2 = 3,
  TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP = 21,
};
enum {
  TOLBI_SHIP_INTERIORS_HOUSE_TO_TOLBI_SHIP_DECK = 1,
  TOLBI_SHIP_INTERIORS_TO_TOLBI_SHIP_DECK_2 = 3,
  TOLBI_SHIP_INTERIORS_HOUSE_2_TO_TOLBI_SHIP_DECK = 5,
  TOLBI_SHIP_INTERIORS_HOUSE_3_TO_TOLBI_SHIP_DECK = 14,
  TOLBI_SHIP_INTERIORS_HOUSE_4_TO_TOLBI_SHIP_DECK = 15,
  TOLBI_SHIP_INTERIORS_TO_WORLD_MAP = 21,
};

/// SceneId_TolbiDocks's entrances and exits.
enum {
  TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK = 1,
  TOLBI_DOCKS_FROM_WORLD_MAP = 2,
  TOLBI_DOCKS_FROM_KALAY_TOLBI_DOCKS_2 = 3,
};
enum {
  TOLBI_DOCKS_TO_WORLD_MAP = 1,
  TOLBI_DOCKS_TO_TOLBI_SHIP_DECK = 2,
  TOLBI_DOCKS_TO_KALAY_TOLBI_DOCKS_2 = 3,
};

/// SceneId_CrossboneIsle's entrances and exits.
enum {
  CROSSBONE_ISLE_FROM_WORLD_MAP = 1,
  CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_2 = 2,
};
enum {
  CROSSBONE_ISLE_TO_WORLD_MAP = 1,
  CROSSBONE_ISLE_TO_CROSSBONE_ISLE_2 = 2,
};

/// SceneId_CrossboneIsle2's entrances and exits.
enum {
  CROSSBONE_ISLE_2_FROM_CROSSBONE_ISLE = 1,
  CROSSBONE_ISLE_2_FROM_CROSSBONE_ISLE_6 = 2,
};
enum {
  CROSSBONE_ISLE_2_TO_CROSSBONE_ISLE = 1,
  CROSSBONE_ISLE_2_TO_CROSSBONE_ISLE_6 = 2,
};

/// SceneId_CrossboneIslePillarRooms1's entrances and exits.
enum {
  CROSSBONE_ISLE_PILLAR_ROOMS_1_FROM_CROSSBONE_ISLE_6_EAST = 1,
  CROSSBONE_ISLE_PILLAR_ROOMS_1_FROM_CROSSBONE_ISLE_6_WEST = 2,
};
enum {
  CROSSBONE_ISLE_PILLAR_ROOMS_1_TO_CROSSBONE_ISLE_6_EAST = 1,
  CROSSBONE_ISLE_PILLAR_ROOMS_1_TO_CROSSBONE_ISLE_6_WEST = 2,
};

/// SceneId_CrossboneIslePillarRooms2's entrances and exits.
enum {
  CROSSBONE_ISLE_PILLAR_ROOMS_2_FROM_CROSSBONE_ISLE_7_EAST = 1,
  CROSSBONE_ISLE_PILLAR_ROOMS_2_FROM_CROSSBONE_ISLE_7_WEST = 2,
};
enum {
  CROSSBONE_ISLE_PILLAR_ROOMS_2_TO_CROSSBONE_ISLE_7_EAST = 1,
  CROSSBONE_ISLE_PILLAR_ROOMS_2_TO_CROSSBONE_ISLE_7_WEST = 2,
};

/// SceneId_CrossboneIsleKeys's entrances and exits.
enum {
  CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_8_EAST = 1,
  CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_8_WEST = 2,
};
enum {
  CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_8_EAST = 1,
  CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_8_WEST = 2,
};

/// SceneId_CrossboneIslePlatformRooms2's entrances and exits.
enum {
  CROSSBONE_ISLE_PLATFORM_ROOMS_2_FROM_CROSSBONE_ISLE_9_EAST = 1,
  CROSSBONE_ISLE_PLATFORM_ROOMS_2_FROM_CROSSBONE_ISLE_9_WEST = 2,
};
enum {
  CROSSBONE_ISLE_PLATFORM_ROOMS_2_TO_CROSSBONE_ISLE_9_EAST = 1,
  CROSSBONE_ISLE_PLATFORM_ROOMS_2_TO_CROSSBONE_ISLE_9_WEST = 2,
};

/// SceneId_CrossboneIsleFloodgate's entrances and exits.
enum {
  CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_10_EAST = 1,
  CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_10_WEST = 2,
};
enum {
  CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_10_EAST = 1,
  CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_10_WEST = 2,
};

/// SceneId_CrossboneIsleBane's entrances and exits.
enum {
  CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_11_EAST = 1,
  CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_11_WEST = 2,
};
enum {
  CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_11_EAST = 1,
  CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_11_WEST = 2,
};

/// SceneId_CrossboneIslePillarRooms4's entrances and exits.
enum {
  CROSSBONE_ISLE_PILLAR_ROOMS_4_FROM_CROSSBONE_ISLE_12_EAST = 1,
  CROSSBONE_ISLE_PILLAR_ROOMS_4_FROM_CROSSBONE_ISLE_12_WEST = 2,
};
enum {
  CROSSBONE_ISLE_PILLAR_ROOMS_4_TO_CROSSBONE_ISLE_12_EAST = 1,
  CROSSBONE_ISLE_PILLAR_ROOMS_4_TO_CROSSBONE_ISLE_12_WEST = 2,
};

/// SceneId_CrossboneIslePillarRooms5's entrances and exits.
enum {
  CROSSBONE_ISLE_PILLAR_ROOMS_5_FROM_CROSSBONE_ISLE_13_EAST = 1,
  CROSSBONE_ISLE_PILLAR_ROOMS_5_FROM_CROSSBONE_ISLE_13_WEST = 2,
};
enum {
  CROSSBONE_ISLE_PILLAR_ROOMS_5_TO_CROSSBONE_ISLE_13_EAST = 1,
  CROSSBONE_ISLE_PILLAR_ROOMS_5_TO_CROSSBONE_ISLE_13_WEST = 2,
};

/// SceneId_CrossboneIsle3's entrances and exits.
enum {
  CROSSBONE_ISLE_3_FROM_CROSSBONE_ISLE_14_EAST = 1,
  CROSSBONE_ISLE_3_FROM_CROSSBONE_ISLE_14_WEST = 2,
};
enum {
  CROSSBONE_ISLE_3_TO_CROSSBONE_ISLE_14_EAST = 1,
  CROSSBONE_ISLE_3_TO_CROSSBONE_ISLE_14_WEST = 2,
};

/// SceneId_CrossboneIsle4's entrances and exits.
enum {
  CROSSBONE_ISLE_4_FROM_CROSSBONE_ISLE_14 = 1,
  CROSSBONE_ISLE_4_FROM_PIRATE_SHIP = 2,
};
enum {
  CROSSBONE_ISLE_4_TO_CROSSBONE_ISLE_14 = 1,
  CROSSBONE_ISLE_4_TO_PIRATE_SHIP = 2,
};

/// SceneId_PirateShip's entrances and exits.
enum {
  PIRATE_SHIP_FROM_CROSSBONE_ISLE_4 = 1,
};
enum {
  PIRATE_SHIP_TO_CROSSBONE_ISLE_4 = 1,
};

/// SceneId_CrossboneIsle6's entrances and exits.
enum {
  CROSSBONE_ISLE_6_FROM_CROSSBONE_ISLE_2 = 1,
  CROSSBONE_ISLE_6_FROM_CROSSBONE_ISLE_7 = 2,
  CROSSBONE_ISLE_6_FROM_CROSSBONE_ISLE_PILLAR_ROOMS_1_EAST = 3,
  CROSSBONE_ISLE_6_FROM_CROSSBONE_ISLE_PILLAR_ROOMS_1_WEST = 4,
};
enum {
  CROSSBONE_ISLE_6_TO_CROSSBONE_ISLE_2 = 1,
  CROSSBONE_ISLE_6_TO_CROSSBONE_ISLE_7 = 2,
  CROSSBONE_ISLE_6_TO_CROSSBONE_ISLE_PILLAR_ROOMS_1_EAST = 3,
  CROSSBONE_ISLE_6_TO_CROSSBONE_ISLE_PILLAR_ROOMS_1_WEST = 4,
};

/// SceneId_CrossboneIsle7's entrances and exits.
enum {
  CROSSBONE_ISLE_7_FROM_CROSSBONE_ISLE_6 = 1,
  CROSSBONE_ISLE_7_FROM_CROSSBONE_ISLE_8 = 2,
  CROSSBONE_ISLE_7_FROM_CROSSBONE_ISLE_PILLAR_ROOMS_2_EAST = 3,
  CROSSBONE_ISLE_7_FROM_CROSSBONE_ISLE_PILLAR_ROOMS_2_WEST = 4,
};
enum {
  CROSSBONE_ISLE_7_TO_CROSSBONE_ISLE_6 = 1,
  CROSSBONE_ISLE_7_TO_CROSSBONE_ISLE_8 = 2,
  CROSSBONE_ISLE_7_TO_CROSSBONE_ISLE_PILLAR_ROOMS_2_EAST = 3,
  CROSSBONE_ISLE_7_TO_CROSSBONE_ISLE_PILLAR_ROOMS_2_WEST = 4,
};

/// SceneId_CrossboneIsle8's entrances and exits.
enum {
  CROSSBONE_ISLE_8_FROM_CROSSBONE_ISLE_7 = 1,
  CROSSBONE_ISLE_8_FROM_CROSSBONE_ISLE_9 = 2,
  CROSSBONE_ISLE_8_FROM_CROSSBONE_ISLE_KEYS_EAST = 3,
  CROSSBONE_ISLE_8_FROM_CROSSBONE_ISLE_KEYS_WEST = 4,
};
enum {
  CROSSBONE_ISLE_8_TO_CROSSBONE_ISLE_7 = 1,
  CROSSBONE_ISLE_8_TO_CROSSBONE_ISLE_9 = 2,
  CROSSBONE_ISLE_8_TO_CROSSBONE_ISLE_KEYS_EAST = 3,
  CROSSBONE_ISLE_8_TO_CROSSBONE_ISLE_KEYS_WEST = 4,
};

/// SceneId_CrossboneIsle9's entrances and exits.
enum {
  CROSSBONE_ISLE_9_FROM_CROSSBONE_ISLE_8 = 1,
  CROSSBONE_ISLE_9_FROM_CROSSBONE_ISLE_10 = 2,
  CROSSBONE_ISLE_9_FROM_CROSSBONE_ISLE_PLATFORM_ROOMS_2_EAST = 3,
  CROSSBONE_ISLE_9_FROM_CROSSBONE_ISLE_PLATFORM_ROOMS_2_WEST = 4,
};
enum {
  CROSSBONE_ISLE_9_TO_CROSSBONE_ISLE_8 = 1,
  CROSSBONE_ISLE_9_TO_CROSSBONE_ISLE_10 = 2,
  CROSSBONE_ISLE_9_TO_CROSSBONE_ISLE_PLATFORM_ROOMS_2_EAST = 3,
  CROSSBONE_ISLE_9_TO_CROSSBONE_ISLE_PLATFORM_ROOMS_2_WEST = 4,
};

/// SceneId_CrossboneIsle10's entrances and exits.
enum {
  CROSSBONE_ISLE_10_FROM_CROSSBONE_ISLE_9 = 1,
  CROSSBONE_ISLE_10_FROM_CROSSBONE_ISLE_11 = 2,
  CROSSBONE_ISLE_10_FROM_CROSSBONE_ISLE_FLOODGATE_EAST = 3,
  CROSSBONE_ISLE_10_FROM_CROSSBONE_ISLE_FLOODGATE_WEST = 4,
};
enum {
  CROSSBONE_ISLE_10_TO_CROSSBONE_ISLE_9 = 1,
  CROSSBONE_ISLE_10_TO_CROSSBONE_ISLE_11 = 2,
  CROSSBONE_ISLE_10_TO_CROSSBONE_ISLE_FLOODGATE_EAST = 3,
  CROSSBONE_ISLE_10_TO_CROSSBONE_ISLE_FLOODGATE_WEST = 4,
};

/// SceneId_CrossboneIsle11's entrances and exits.
enum {
  CROSSBONE_ISLE_11_FROM_CROSSBONE_ISLE_10 = 1,
  CROSSBONE_ISLE_11_FROM_CROSSBONE_ISLE_12 = 2,
  CROSSBONE_ISLE_11_FROM_CROSSBONE_ISLE_BANE_EAST = 3,
  CROSSBONE_ISLE_11_FROM_CROSSBONE_ISLE_BANE_WEST = 4,
};
enum {
  CROSSBONE_ISLE_11_TO_CROSSBONE_ISLE_10 = 1,
  CROSSBONE_ISLE_11_TO_CROSSBONE_ISLE_12 = 2,
  CROSSBONE_ISLE_11_TO_CROSSBONE_ISLE_BANE_EAST = 3,
  CROSSBONE_ISLE_11_TO_CROSSBONE_ISLE_BANE_WEST = 4,
};

/// SceneId_CrossboneIsle12's entrances and exits.
enum {
  CROSSBONE_ISLE_12_FROM_CROSSBONE_ISLE_11 = 1,
  CROSSBONE_ISLE_12_FROM_CROSSBONE_ISLE_13 = 2,
  CROSSBONE_ISLE_12_FROM_CROSSBONE_ISLE_PILLAR_ROOMS_4_EAST = 3,
  CROSSBONE_ISLE_12_FROM_CROSSBONE_ISLE_PILLAR_ROOMS_4_WEST = 4,
};
enum {
  CROSSBONE_ISLE_12_TO_CROSSBONE_ISLE_11 = 1,
  CROSSBONE_ISLE_12_TO_CROSSBONE_ISLE_13 = 2,
  CROSSBONE_ISLE_12_TO_CROSSBONE_ISLE_PILLAR_ROOMS_4_EAST = 3,
  CROSSBONE_ISLE_12_TO_CROSSBONE_ISLE_PILLAR_ROOMS_4_WEST = 4,
};

/// SceneId_CrossboneIsle13's entrances and exits.
enum {
  CROSSBONE_ISLE_13_FROM_CROSSBONE_ISLE_12 = 1,
  CROSSBONE_ISLE_13_FROM_CROSSBONE_ISLE_14 = 2,
  CROSSBONE_ISLE_13_FROM_CROSSBONE_ISLE_PILLAR_ROOMS_5_EAST = 3,
  CROSSBONE_ISLE_13_FROM_CROSSBONE_ISLE_PILLAR_ROOMS_5_WEST = 4,
};
enum {
  CROSSBONE_ISLE_13_TO_CROSSBONE_ISLE_12 = 1,
  CROSSBONE_ISLE_13_TO_CROSSBONE_ISLE_14 = 2,
  CROSSBONE_ISLE_13_TO_CROSSBONE_ISLE_PILLAR_ROOMS_5_EAST = 3,
  CROSSBONE_ISLE_13_TO_CROSSBONE_ISLE_PILLAR_ROOMS_5_WEST = 4,
};

/// SceneId_CrossboneIsle14's entrances and exits.
enum {
  CROSSBONE_ISLE_14_FROM_CROSSBONE_ISLE_13 = 1,
  CROSSBONE_ISLE_14_FROM_CROSSBONE_ISLE_4 = 2,
  CROSSBONE_ISLE_14_FROM_CROSSBONE_ISLE_3_EAST = 3,
  CROSSBONE_ISLE_14_FROM_CROSSBONE_ISLE_3_WEST = 4,
};
enum {
  CROSSBONE_ISLE_14_TO_CROSSBONE_ISLE_13 = 1,
  CROSSBONE_ISLE_14_TO_CROSSBONE_ISLE_4 = 2,
  CROSSBONE_ISLE_14_TO_CROSSBONE_ISLE_3_EAST = 3,
  CROSSBONE_ISLE_14_TO_CROSSBONE_ISLE_3_WEST = 4,
};

/// SceneId_Tolbi's entrances and exits.
enum {
  TOLBI_FROM_HOUSE_SOUTH = 1,
  TOLBI_FROM_HOUSE_NORTHWEST = 2,
  TOLBI_FROM_RESTAURANT = 3,
  TOLBI_FROM_TOLBI_INTERIORS_EAST = 4,
  TOLBI_FROM_HOUSE_NORTHEAST = 5,
  TOLBI_FROM_HOUSE_EAST = 6,
  TOLBI_FROM_WEAPON_AND_ARMOR_SHOP = 7,
  TOLBI_FROM_HOUSE_SOUTHWEST = 8,
  TOLBI_FROM_SANCTUM = 9,
  TOLBI_FROM_TOLBI_INTERIORS_WEST = 10,
  TOLBI_FROM_BABI_PALACE_2 = 11,
  TOLBI_FROM_COLOSSEUM_ENTRANCE = 12,
  TOLBI_FROM_TOLBI_SPRING_1 = 13,
  TOLBI_FROM_TOLBI_SPRING_2 = 20,
};
enum {
  TOLBI_TO_HOUSE_SOUTH = 1,
  TOLBI_TO_HOUSE_NORTHWEST = 2,
  TOLBI_TO_RESTAURANT = 3,
  TOLBI_TO_HOUSE_NORTHEAST = 5,
  TOLBI_TO_HOUSE_EAST = 6,
  TOLBI_TO_WEAPON_AND_ARMOR_SHOP = 7,
  TOLBI_TO_HOUSE_SOUTHWEST = 8,
  TOLBI_TO_SANCTUM = 9,
  TOLBI_TO_BABI_PALACE_2 = 10,
  TOLBI_TO_COLOSSEUM_ENTRANCE = 11,
  TOLBI_TO_WORLD_MAP = 12,
  TOLBI_TO_TOLBI_SPRING_2 = 30,
};

/// SceneId_TolbiInteriors's entrances and exits.
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
      6, ///< the room behind Tolbi's weapon and armor shop door; the room holds
         ///< the weapon and armor shop; through TOLBI_TO_WEAPON_AND_ARMOR_SHOP
  TOLBI_INTERIORS_SANCTUM =
      8, ///< the room behind Tolbi's sanctum door; the
         ///< room holds the sanctum; through TOLBI_TO_SANCTUM
  TOLBI_INTERIORS_RESTAURANT =
      9, ///< the room behind Tolbi's restaurant door; the room holds the
         ///< restaurant; through TOLBI_TO_RESTAURANT
  TOLBI_INTERIORS_RESTAURANT_NORTH = 10, ///< the room holds the restaurant;
  TOLBI_INTERIORS_SANCTUM_NORTH = 30,    ///< the room holds the sanctum;
};
enum {
  TOLBI_INTERIORS_HOUSE_NORTHWEST_TO_TOLBI = 1,
  TOLBI_INTERIORS_RESTAURANT_TO_TOLBI = 2,
  TOLBI_INTERIORS_TO_TOLBI_EAST = 3,
  TOLBI_INTERIORS_HOUSE_NORTHEAST_TO_TOLBI = 4,
  TOLBI_INTERIORS_HOUSE_EAST_TO_TOLBI = 5,
  TOLBI_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_TOLBI = 6,
  TOLBI_INTERIORS_HOUSE_SOUTHWEST_TO_TOLBI = 7,
  TOLBI_INTERIORS_SANCTUM_TO_TOLBI = 8,
  TOLBI_INTERIORS_TO_TOLBI_WEST = 9,
};

/// SceneId_TolbiSpring1's entrances and exits.
enum {
  TOLBI_SPRING_1_AFTER_OFFER_LUCKY_WHEELS =
      12,                                     ///< TolbiSpring_OfferLuckyWheels
  TOLBI_SPRING_1_AFTER_OFFER_LUCKY_DICE = 13, ///< TolbiSpring_OfferLuckyDice
};
enum {
  TOLBI_SPRING_1_TO_TOLBI = 1,
};

/// SceneId_BabiPalace1's entrances and exits.
enum {
  BABI_PALACE_1_FROM_BABI_PALACE_2 = 1,
  BABI_PALACE_1_FROM_VALE_INTERIORS_INN = 11,
  BABI_PALACE_1_FROM_COLOSSEUM_TUNNEL = 13,
  BABI_PALACE_1_FROM_COLOSSEUM_FINAL_1 = 98,
  BABI_PALACE_1_FROM_COLOSSEUM_FINAL_3 = 99,
};
enum {
  BABI_PALACE_1_TO_BABI_PALACE_2 = 1,
  BABI_PALACE_1_TO_VALE_INTERIORS_INN = 11,
  BABI_PALACE_1_TO_COLOSSEUM_TUNNEL = 13,
};

/// SceneId_BabiPalace2's entrances and exits.
enum {
  BABI_PALACE_2_FROM_TOLBI = 1,
  BABI_PALACE_2_FROM_BABI_PALACE_1 = 2,
};
enum {
  BABI_PALACE_2_TO_TOLBI = 1,
  BABI_PALACE_2_TO_BABI_PALACE_1 = 2,
};

/// SceneId_Colosseum's entrances and exits.
enum {
  COLOSSEUM_FROM_COLOSSEUM_TUNNEL = 4,
  COLOSSEUM_FROM_COLOSSEUM_FINAL_1_WEST = 12,
  COLOSSEUM_FROM_COLOSSEUM_FINAL_1_EAST = 21,
  COLOSSEUM_FROM_COLOSSEUM_FINAL_2_NORTH = 31,
  COLOSSEUM_FROM_COLOSSEUM_ENTRANCE = 64,
  COLOSSEUM_FROM_COLOSSEUM_FINAL_3 = 66,
  COLOSSEUM_FROM_COLOSSEUM_FINAL_2_SOUTH = 68,
};
enum {
  COLOSSEUM_TO_COLOSSEUM_TUNNEL = 4,
  COLOSSEUM_TO_COLOSSEUM_FINAL_1_WEST = 12,
  COLOSSEUM_TO_COLOSSEUM_FINAL_2_WEST = 21,
  COLOSSEUM_TO_COLOSSEUM_FINAL_2_EAST = 22,
  COLOSSEUM_TO_COLOSSEUM_FINAL_3_NORTH = 31,
  COLOSSEUM_TO_COLOSSEUM_FINAL_3_SOUTH = 32,
  COLOSSEUM_TO_COLOSSEUM_FINAL_1 = 65,
};

/// SceneId_ColosseumTunnel's entrances and exits.
enum {
  COLOSSEUM_TUNNEL_FROM_BABI_PALACE_1 = 1,
  COLOSSEUM_TUNNEL_FROM_COLOSSEUM = 2,
};
enum {
  COLOSSEUM_TUNNEL_TO_BABI_PALACE_1 = 1,
  COLOSSEUM_TUNNEL_TO_COLOSSEUM = 2,
};

/// SceneId_ColosseumEntrance's entrances and exits.
enum {
  COLOSSEUM_ENTRANCE_FROM_TOLBI = 1,
};
enum {
  COLOSSEUM_ENTRANCE_TO_TOLBI = 1,
  COLOSSEUM_ENTRANCE_TO_COLOSSEUM = 2,
};

/// SceneId_ColosseumFinal1's entrances and exits.
enum {
  COLOSSEUM_FINAL_1_FROM_COLOSSEUM_WEST = 2,
  COLOSSEUM_FINAL_1_FROM_COLOSSEUM = 3,
};
enum {
  COLOSSEUM_FINAL_1_TO_COLOSSEUM_EAST = 4,
  COLOSSEUM_FINAL_1_TO_BABI_PALACE_1 = 5,
  COLOSSEUM_FINAL_1_TO_COLOSSEUM_WEST = 11,
};

/// SceneId_ColosseumFinal2's entrances and exits.
enum {
  COLOSSEUM_FINAL_2_FROM_COLOSSEUM_EAST = 2,
  COLOSSEUM_FINAL_2_FROM_COLOSSEUM_WEST = 3,
};
enum {
  COLOSSEUM_FINAL_2_TO_COLOSSEUM_NORTH = 4,
  COLOSSEUM_FINAL_2_TO_BABI_PALACE_1 = 5,
  COLOSSEUM_FINAL_2_TO_COLOSSEUM_SOUTH = 11,
};

/// SceneId_ColosseumFinal3's entrances and exits.
enum {
  COLOSSEUM_FINAL_3_FROM_COLOSSEUM_SOUTH = 2,
  COLOSSEUM_FINAL_3_FROM_COLOSSEUM_NORTH = 3,
};
enum {
  COLOSSEUM_FINAL_3_TO_BABI_PALACE_1 = 4,
  COLOSSEUM_FINAL_3_TO_COLOSSEUM = 11,
};

/// SceneId_AltmillerCave1's entrances and exits.
enum {
  ALTMILLER_CAVE_1_FROM_WORLD_MAP = 1,
  ALTMILLER_CAVE_1_FROM_ALTMILLER_CAVE_2 = 4,
};
enum {
  ALTMILLER_CAVE_1_TO_WORLD_MAP = 1,
  ALTMILLER_CAVE_1_TO_ALTMILLER_CAVE_2 = 4,
};

/// SceneId_AltmillerCave2's entrances and exits.
enum {
  ALTMILLER_CAVE_2_FROM_ALTMILLER_CAVE_1 = 10,
  ALTMILLER_CAVE_2_FROM_ALTMILLER_CAVE_3 = 11,
};
enum {
  ALTMILLER_CAVE_2_TO_ALTMILLER_CAVE_1 = 10,
  ALTMILLER_CAVE_2_TO_ALTMILLER_CAVE_3 = 11,
};

/// SceneId_AltmillerCave3's entrances and exits.
enum {
  ALTMILLER_CAVE_3_FROM_ALTMILLER_CAVE_2 = 20,
  ALTMILLER_CAVE_3_FROM_ALTMILLER_CAVE_4 = 21,
};
enum {
  ALTMILLER_CAVE_3_TO_ALTMILLER_CAVE_2 = 20,
  ALTMILLER_CAVE_3_TO_ALTMILLER_CAVE_4 = 21,
};

/// SceneId_AltmillerCave4's entrances and exits.
enum {
  ALTMILLER_CAVE_4_FROM_ALTMILLER_CAVE_3 = 30,
  ALTMILLER_CAVE_4_FROM_ALTMILLER_CAVE_5 = 31,
};
enum {
  ALTMILLER_CAVE_4_TO_ALTMILLER_CAVE_3 = 30,
  ALTMILLER_CAVE_4_TO_ALTMILLER_CAVE_5 = 31,
};

/// SceneId_AltmillerCave5's entrances and exits.
enum {
  ALTMILLER_CAVE_5_FROM_ALTMILLER_CAVE_4 = 40,
  ALTMILLER_CAVE_5_FROM_ALTMILLER_CAVE_6 = 41,
};
enum {
  ALTMILLER_CAVE_5_TO_ALTMILLER_CAVE_4 = 40,
  ALTMILLER_CAVE_5_TO_ALTMILLER_CAVE_6 = 41,
};

/// SceneId_AltmillerCave6's entrances and exits.
enum {
  ALTMILLER_CAVE_6_FROM_ALTMILLER_CAVE_5 = 50,
};
enum {
  ALTMILLER_CAVE_6_TO_ALTMILLER_CAVE_5 = 50,
};

/// SceneId_GondowanPassage's entrances and exits.
enum {
  GONDOWAN_PASSAGE_FROM_WORLD_MAP_NORTH = 1,
  GONDOWAN_PASSAGE_FROM_WORLD_MAP_SOUTH = 2,
};
enum {
  GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH = 1,
  GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH = 2,
};

/// SceneId_KalayTunnelEntrance's entrances and exits.
enum {
  KALAY_TUNNEL_ENTRANCE_FROM_KALAY_TUNNEL_1 = 1,
  KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_NORTH = 2,
  KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH = 10,
};
enum {
  KALAY_TUNNEL_ENTRANCE_TO_KALAY_TUNNEL_1 = 1,
  KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_NORTH = 2,
  KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_SOUTH = 10,
};

/// SceneId_KalayTunnel1's entrances and exits.
enum {
  KALAY_TUNNEL_1_FROM_KALAY_TUNNEL_2 = 1,
  KALAY_TUNNEL_1_FROM_KALAY_TUNNEL_ENTRANCE = 2,
};
enum {
  KALAY_TUNNEL_1_TO_KALAY_TUNNEL_2 = 1,
  KALAY_TUNNEL_1_TO_KALAY_TUNNEL_ENTRANCE = 2,
};

/// SceneId_KalayTunnel2's entrances and exits.
enum {
  KALAY_TUNNEL_2_FROM_KALAY_TUNNEL_3_WEST = 1,
  KALAY_TUNNEL_2_FROM_KALAY_TUNNEL_3_EAST = 2,
  KALAY_TUNNEL_2_FROM_KALAY_TUNNEL_1 = 5,
};
enum {
  KALAY_TUNNEL_2_TO_KALAY_TUNNEL_3_WEST = 1,
  KALAY_TUNNEL_2_TO_KALAY_TUNNEL_3_EAST = 2,
  KALAY_TUNNEL_2_TO_KALAY_TUNNEL_1 = 5,
};

/// SceneId_KalayTunnel3's entrances and exits.
enum {
  KALAY_TUNNEL_3_FROM_HAMMET_PALACE = 1,
  KALAY_TUNNEL_3_FROM_KALAY_TUNNEL_2_WEST = 2,
  KALAY_TUNNEL_3_FROM_KALAY_TUNNEL_2_EAST = 3,
  KALAY_TUNNEL_3_FROM_KALAY_1 = 4,
};
enum {
  KALAY_TUNNEL_3_TO_HAMMET_PALACE = 1,
  KALAY_TUNNEL_3_TO_KALAY_TUNNEL_2_WEST = 2,
  KALAY_TUNNEL_3_TO_KALAY_TUNNEL_2_EAST = 3,
  KALAY_TUNNEL_3_TO_KALAY_1 = 4,
};

/// SceneId_GondowanCave1's entrances and exits.
enum {
  GONDOWAN_CAVE_1_FROM_GONDOWAN_CAVE_2_NORTH = 1,
  GONDOWAN_CAVE_1_FROM_GONDOWAN_CAVE_2_SOUTH = 4,
};
enum {
  GONDOWAN_CAVE_1_TO_GONDOWAN_CAVE_2_NORTH = 1,
  GONDOWAN_CAVE_1_TO_GONDOWAN_CAVE_2_SOUTH = 4,
};

/// SceneId_GondowanCave2's entrances and exits.
enum {
  GONDOWAN_CAVE_2_FROM_WORLD_MAP_EAST = 1,
  GONDOWAN_CAVE_2_FROM_GONDOWAN_CAVE_1_NORTH = 2,
  GONDOWAN_CAVE_2_FROM_GONDOWAN_CAVE_1_SOUTH = 5,
  GONDOWAN_CAVE_2_FROM_WORLD_MAP_WEST = 6,
};
enum {
  GONDOWAN_CAVE_2_TO_WORLD_MAP_EAST = 1,
  GONDOWAN_CAVE_2_TO_GONDOWAN_CAVE_1_NORTH = 2,
  GONDOWAN_CAVE_2_TO_GONDOWAN_CAVE_1_SOUTH = 5,
  GONDOWAN_CAVE_2_TO_WORLD_MAP_WEST = 6,
};

/// SceneId_LunpaFortressGates's entrances and exits.
enum {
  LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_1 = 1,
  LUNPA_FORTRESS_GATES_FROM_LUNPA = 2,
  LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_4 = 3,
  LUNPA_FORTRESS_GATES_AFTER_THROWN_OUT = 4,
};
enum {
  LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_1 = 1,
  LUNPA_FORTRESS_GATES_TO_LUNPA = 2,
};

/// SceneId_LunpaFortress1's entrances and exits.
enum {
  LUNPA_FORTRESS_1_FROM_LUNPA_FORTRESS_GATES = 1,
  LUNPA_FORTRESS_1_FROM_LUNPA_FORTRESS_2_SOUTH = 8,
  LUNPA_FORTRESS_1_FROM_LUNPA_FORTRESS_2_NORTH = 10,
  LUNPA_FORTRESS_1_FROM_LUNPA_FORTRESS_3 = 11,
};
enum {
  LUNPA_FORTRESS_1_TO_LUNPA_FORTRESS_GATES = 1,
  LUNPA_FORTRESS_1_TO_LUNPA_FORTRESS_2_SOUTH = 8,
  LUNPA_FORTRESS_1_TO_LUNPA_FORTRESS_2_NORTH = 10,
  LUNPA_FORTRESS_1_TO_LUNPA_FORTRESS_3 = 11,
};

/// SceneId_LunpaFortress2's entrances and exits.
enum {
  LUNPA_FORTRESS_2_FROM_LUNPA_FORTRESS_1_SOUTH = 1,
  LUNPA_FORTRESS_2_FROM_LUNPA_FORTRESS_1_NORTH = 16,
};
enum {
  LUNPA_FORTRESS_2_TO_LUNPA_FORTRESS_1_SOUTH = 1,
  LUNPA_FORTRESS_2_TO_LUNPA_FORTRESS_1_NORTH = 16,
};

/// SceneId_LunpaFortress3's entrances and exits.
enum {
  LUNPA_FORTRESS_3_FROM_LUNPA_FORTRESS_1 = 1,
  LUNPA_FORTRESS_3_FROM_LUNPA_FORTRESS_4_NORTHEAST = 4,
  LUNPA_FORTRESS_3_FROM_LUNPA_FORTRESS_4_NORTH = 5,
  LUNPA_FORTRESS_3_FROM_LUNPA_FORTRESS_4_NORTHWEST = 6,
};
enum {
  LUNPA_FORTRESS_3_TO_LUNPA_FORTRESS_1 = 1,
  LUNPA_FORTRESS_3_TO_LUNPA_FORTRESS_4_NORTHEAST = 4,
  LUNPA_FORTRESS_3_TO_LUNPA_FORTRESS_4_NORTH = 5,
  LUNPA_FORTRESS_3_TO_LUNPA_FORTRESS_4_NORTHWEST = 6,
};

/// SceneId_LunpaFortress4's entrances and exits.
enum {
  LUNPA_FORTRESS_4_FROM_LUNPA_FORTRESS_3_WEST = 1,
  LUNPA_FORTRESS_4_FROM_LUNPA_FORTRESS_3_SOUTH = 2,
  LUNPA_FORTRESS_4_FROM_LUNPA_FORTRESS_3_EAST = 3,
};
enum {
  LUNPA_FORTRESS_4_TO_LUNPA_FORTRESS_3_WEST = 1,
  LUNPA_FORTRESS_4_TO_LUNPA_FORTRESS_3_SOUTH = 2,
  LUNPA_FORTRESS_4_TO_LUNPA_FORTRESS_GATES = 4,
  LUNPA_FORTRESS_4_TO_LUNPA_FORTRESS_3_2 = 5,
};

/// SceneId_SuhallaDesert1's entrances and exits.
enum {
  SUHALLA_DESERT_1_FROM_WORLD_MAP = 1,
  SUHALLA_DESERT_1_FROM_SUHALLA_DESERT_2 = 2,
};
enum {
  SUHALLA_DESERT_1_TO_WORLD_MAP = 1,
  SUHALLA_DESERT_1_TO_SUHALLA_DESERT_2 = 2,
};

/// SceneId_SuhallaDesert2's entrances and exits.
enum {
  SUHALLA_DESERT_2_FROM_SUHALLA_DESERT_1 = 1,
  SUHALLA_DESERT_2_FROM_WORLD_MAP = 2,
  SUHALLA_DESERT_2_FROM_SUHALLA_DESERT_3_EAST = 3,
  SUHALLA_DESERT_2_FROM_SUHALLA_DESERT_3_WEST = 4,
};
enum {
  SUHALLA_DESERT_2_TO_SUHALLA_DESERT_1 = 1,
  SUHALLA_DESERT_2_TO_WORLD_MAP = 2,
  SUHALLA_DESERT_2_TO_SUHALLA_DESERT_3_EAST = 3,
  SUHALLA_DESERT_2_TO_SUHALLA_DESERT_3_WEST = 4,
};

/// SceneId_SuhallaDesert3's entrances and exits.
enum {
  SUHALLA_DESERT_3_FROM_SUHALLA_DESERT_2_WEST = 1,
  SUHALLA_DESERT_3_FROM_SUHALLA_DESERT_2_EAST = 4,
};
enum {
  SUHALLA_DESERT_3_TO_SUHALLA_DESERT_2_WEST = 1,
  SUHALLA_DESERT_3_TO_SUHALLA_DESERT_2_EAST = 4,
};

/// SceneId_Suhalla's entrances and exits.
enum {
  SUHALLA_FROM_INN = 1,
  SUHALLA_FROM_SANCTUM = 2,
  SUHALLA_FROM_HOUSE_WEST = 3,
  SUHALLA_FROM_HOUSE_NORTHEAST = 4,
  SUHALLA_FROM_HOUSE_EAST = 5,
  SUHALLA_FROM_WORLD_MAP = 10,
};
enum {
  SUHALLA_TO_INN = 1,
  SUHALLA_TO_SANCTUM = 2,
  SUHALLA_TO_HOUSE_WEST = 3,
  SUHALLA_TO_HOUSE_NORTHEAST = 4,
  SUHALLA_TO_HOUSE_EAST = 5,
  SUHALLA_TO_WORLD_MAP = 10,
};

/// SceneId_SuhallaInteriors's entrances and exits.
enum {
  SUHALLA_INTERIORS_SANCTUM =
      2, ///< the room behind Suhalla's sanctum door;
         ///< the room holds the sanctum; through SUHALLA_TO_SANCTUM
  SUHALLA_INTERIORS_HOUSE_WEST = 3, ///< the room behind Suhalla's house west
                                    ///< door; through SUHALLA_TO_HOUSE_WEST
  SUHALLA_INTERIORS_HOUSE_NORTHEAST =
      4, ///< the room behind Suhalla's house northeast door; through
         ///< SUHALLA_TO_HOUSE_NORTHEAST
  SUHALLA_INTERIORS_HOUSE_EAST = 5,    ///< the room behind Suhalla's house east
                                       ///< door; through SUHALLA_TO_HOUSE_EAST
  SUHALLA_INTERIORS_SANCTUM_NORTH = 6, ///< the room holds the sanctum;
};
enum {
  SUHALLA_INTERIORS_INN_TO_SUHALLA = 1,
  SUHALLA_INTERIORS_SANCTUM_TO_SUHALLA = 2,
  SUHALLA_INTERIORS_HOUSE_WEST_TO_SUHALLA = 3,
  SUHALLA_INTERIORS_HOUSE_NORTHEAST_TO_SUHALLA = 4,
  SUHALLA_INTERIORS_HOUSE_EAST_TO_SUHALLA = 5,
};

/// SceneId_SuhallaGate1's entrances and exits.
enum {
  SUHALLA_GATE_1_FROM_WORLD_MAP_WEST = 1,
  SUHALLA_GATE_1_FROM_SUHALLA_GATE_2_WEST = 2,
  SUHALLA_GATE_1_FROM_LUNPA_SUHALLA = 3,
  SUHALLA_GATE_1_FROM_SUHALLA_GATE_2_EAST = 4,
  SUHALLA_GATE_1_FROM_WORLD_MAP_EAST = 5,
};
enum {
  SUHALLA_GATE_1_TO_WORLD_MAP_EAST = 1,
  SUHALLA_GATE_1_TO_SUHALLA_GATE_2_WEST = 2,
  SUHALLA_GATE_1_TO_LUNPA_SUHALLA = 3,
  SUHALLA_GATE_1_TO_SUHALLA_GATE_2_EAST = 4,
  SUHALLA_GATE_1_TO_WORLD_MAP_WEST = 5,
};

/// SceneId_SuhallaGate2's entrances and exits.
enum {
  SUHALLA_GATE_2_FROM_SUHALLA_GATE_1_WEST = 1,
  SUHALLA_GATE_2_FROM_SUHALLA_GATE_1_EAST = 4,
  SUHALLA_GATE_2_FROM_GATEWAY_CAVE = 5,
};
enum {
  SUHALLA_GATE_2_TO_SUHALLA_GATE_1_WEST = 1,
  SUHALLA_GATE_2_TO_SUHALLA_GATE_1_EAST = 4,
  SUHALLA_GATE_2_TO_GATEWAY_CAVE = 5,
};

/// SceneId_GatewayCave's entrances and exits.
enum {
  GATEWAY_CAVE_FROM_SUHALLA_GATE_2 = 1,
  GATEWAY_CAVE_FROM_WORLD_MAP = 4,
};
enum {
  GATEWAY_CAVE_TO_SUHALLA_GATE_2 = 1,
  GATEWAY_CAVE_TO_WORLD_MAP = 4,
};

/// SceneId_TunnelRuins1's entrances and exits.
enum {
  TUNNEL_RUINS_1_FROM_BABI_LIGHTHOUSE_1 = 1,
  TUNNEL_RUINS_1_FROM_TUNNEL_RUINS_2_NORTH = 15,
  TUNNEL_RUINS_1_FROM_TUNNEL_RUINS_2_SOUTHWEST = 17,
  TUNNEL_RUINS_1_FROM_TUNNEL_RUINS_2_SOUTHEAST = 18,
};
enum {
  TUNNEL_RUINS_1_TO_BABI_LIGHTHOUSE_1 = 1,
  TUNNEL_RUINS_1_TO_TUNNEL_RUINS_2_WEST = 15,
  TUNNEL_RUINS_1_TO_TUNNEL_RUINS_2_SOUTHWEST = 17,
  TUNNEL_RUINS_1_TO_TUNNEL_RUINS_2_SOUTHEAST = 18,
};

/// SceneId_TunnelRuins2's entrances and exits.
enum {
  TUNNEL_RUINS_2_FROM_BABI_LIGHTHOUSE_ENTRY_WEST = 1,
  TUNNEL_RUINS_2_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH = 2,
  TUNNEL_RUINS_2_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH = 3,
  TUNNEL_RUINS_2_FROM_TUNNEL_RUINS_1_WEST = 4,
  TUNNEL_RUINS_2_FROM_TUNNEL_RUINS_1_NORTH = 6,
  TUNNEL_RUINS_2_FROM_VENUS_LIGHTHOUSE_2 = 16,
  TUNNEL_RUINS_2_FROM_TUNNEL_RUINS_1_SOUTH = 17,
};
enum {
  TUNNEL_RUINS_2_TO_TUNNEL_RUINS_1_NORTH = 1,
  TUNNEL_RUINS_2_TO_TUNNEL_RUINS_1_SOUTHWEST = 3,
  TUNNEL_RUINS_2_TO_TUNNEL_RUINS_1_SOUTHEAST = 12,
  TUNNEL_RUINS_2_TO_VENUS_LIGHTHOUSE_2 = 14,
  TUNNEL_RUINS_2_TO_BABI_LIGHTHOUSE_ENTRY_WEST = 15,
  TUNNEL_RUINS_2_TO_BABI_LIGHTHOUSE_ENTRY_SOUTH = 16,
  TUNNEL_RUINS_2_TO_BABI_LIGHTHOUSE_ENTRY_NORTH = 17,
};

/// SceneId_BabiLighthouse1's entrances and exits.
enum {
  BABI_LIGHTHOUSE_1_FROM_TUNNEL_RUINS_1 = 1,
  BABI_LIGHTHOUSE_1_FROM_BABI_LIGHTHOUSE_2_WEST = 2,
  BABI_LIGHTHOUSE_1_FROM_BABI_LIGHTHOUSE_2_EAST = 3,
};
enum {
  BABI_LIGHTHOUSE_1_TO_TUNNEL_RUINS_1 = 1,
  BABI_LIGHTHOUSE_1_TO_BABI_LIGHTHOUSE_2_WEST = 2,
};

/// SceneId_BabiLighthouse2's entrances and exits.
enum {
  BABI_LIGHTHOUSE_2_FROM_BABI_LIGHTHOUSE_ENTRY_EAST = 7,
  BABI_LIGHTHOUSE_2_FROM_BABI_LIGHTHOUSE_ENTRY_WEST = 9,
  BABI_LIGHTHOUSE_2_FROM_BABI_LIGHTHOUSE_1 = 10,
};
enum {
  BABI_LIGHTHOUSE_2_TO_BABI_LIGHTHOUSE_ENTRY_EAST = 2,
  BABI_LIGHTHOUSE_2_TO_BABI_LIGHTHOUSE_ENTRY_WEST = 6,
  BABI_LIGHTHOUSE_2_TO_BABI_LIGHTHOUSE_1_EAST = 17,
  BABI_LIGHTHOUSE_2_TO_BABI_LIGHTHOUSE_1_WEST = 18,
};

/// SceneId_BabiLighthouseEntry's entrances and exits.
enum {
  BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE = 1,
  BABI_LIGHTHOUSE_ENTRY_FROM_BABI_LIGHTHOUSE_2_WEST = 2,
  BABI_LIGHTHOUSE_ENTRY_FROM_BABI_LIGHTHOUSE_2_EAST = 3,
  BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_2_WEST = 4,
  BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_2_SOUTH = 5,
  BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_2_NORTH = 6,
};
enum {
  BABI_LIGHTHOUSE_ENTRY_TO_ROAD_TO_BABI_LIGHTHOUSE = 1,
  BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_2_EAST = 2,
  BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_2_WEST = 3,
  BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_2_WEST = 4,
  BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_2_SOUTH = 5,
  BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_2_NORTH = 6,
};

/// SceneId_RoadToBabiLighthouse's entrances and exits.
enum {
  ROAD_TO_BABI_LIGHTHOUSE_FROM_WORLD_MAP = 1,
  ROAD_TO_BABI_LIGHTHOUSE_FROM_BABI_LIGHTHOUSE_ENTRY = 2,
};
enum {
  ROAD_TO_BABI_LIGHTHOUSE_TO_WORLD_MAP = 1,
  ROAD_TO_BABI_LIGHTHOUSE_TO_BABI_LIGHTHOUSE_ENTRY = 2,
};

/// SceneId_Lalivero's entrances and exits.
enum {
  LALIVERO_FROM_LALIVERO_INTERIORS_2_NORTHWEST = 1,
  LALIVERO_FROM_LALIVERO_INTERIORS_1_WEST = 2,
  LALIVERO_FROM_LALIVERO_INTERIORS_2_NORTHEAST = 3,
  LALIVERO_FROM_LALIVERO_INTERIORS_1_NORTHWEST = 4,
  LALIVERO_FROM_LALIVERO_INTERIORS_1_NORTH = 5,
  LALIVERO_FROM_LALIVERO_INTERIORS_1_NORTHEAST = 6,
  LALIVERO_FROM_LALIVERO_INTERIORS_2_SOUTHWEST = 7,
  LALIVERO_FROM_LALIVERO_INTERIORS_2_SOUTH = 8,
  LALIVERO_FROM_LALIVERO_INTERIORS_1_SOUTH = 9,
  LALIVERO_FROM_LALIVERO_INTERIORS_1_SOUTHEAST = 10,
  LALIVERO_FROM_LALIVERO_INTERIORS_1_EAST_SOUTHEAST = 11,
  LALIVERO_FROM_WORLD_MAP_EAST = 20,
  LALIVERO_FROM_WORLD_MAP_WEST = 21,
};
enum {
  LALIVERO_TO_LALIVERO_INTERIORS_2_NORTHWEST = 1,
  LALIVERO_TO_LALIVERO_INTERIORS_1_WEST = 2,
  LALIVERO_TO_LALIVERO_INTERIORS_2_NORTHEAST = 3,
  LALIVERO_TO_LALIVERO_INTERIORS_1_NORTHWEST = 4,
  LALIVERO_TO_LALIVERO_INTERIORS_1_NORTH = 5,
  LALIVERO_TO_LALIVERO_INTERIORS_1_NORTHEAST = 6,
  LALIVERO_TO_LALIVERO_INTERIORS_2_SOUTHWEST = 7,
  LALIVERO_TO_LALIVERO_INTERIORS_2_SOUTH = 8,
  LALIVERO_TO_LALIVERO_INTERIORS_1_SOUTH = 9,
  LALIVERO_TO_LALIVERO_INTERIORS_1_SOUTHEAST = 10,
  LALIVERO_TO_LALIVERO_INTERIORS_1_EAST_SOUTHEAST = 11,
  LALIVERO_TO_WORLD_MAP_EAST = 20,
  LALIVERO_TO_WORLD_MAP_WEST = 21,
};

/// SceneId_LaliveroInteriors1's entrances and exits.
enum {
  LALIVERO_INTERIORS_1_FROM_LALIVERO_WEST = 1,
  LALIVERO_INTERIORS_1_FROM_LALIVERO_SOUTHEAST = 2,
  LALIVERO_INTERIORS_1_FROM_LALIVERO_NORTH = 4,
  LALIVERO_INTERIORS_1_FROM_LALIVERO_NORTHWEST = 6,
  LALIVERO_INTERIORS_1_FROM_LALIVERO_SOUTH = 9,
  LALIVERO_INTERIORS_1_FROM_LALIVERO_EAST_SOUTHEAST = 10,
  LALIVERO_INTERIORS_1_FROM_LALIVERO_NORTHEAST = 11,
};
enum {
  LALIVERO_INTERIORS_1_TO_LALIVERO_WEST = 1,
  LALIVERO_INTERIORS_1_TO_LALIVERO_SOUTHEAST = 2,
  LALIVERO_INTERIORS_1_TO_LALIVERO_NORTH = 4,
  LALIVERO_INTERIORS_1_TO_LALIVERO_NORTHWEST = 6,
  LALIVERO_INTERIORS_1_TO_LALIVERO_SOUTH = 9,
  LALIVERO_INTERIORS_1_TO_LALIVERO_EAST_SOUTHEAST = 10,
  LALIVERO_INTERIORS_1_TO_LALIVERO_NORTHEAST = 11,
};

/// SceneId_LaliveroInteriors2's entrances and exits.
enum {
  LALIVERO_INTERIORS_2_FROM_LALIVERO_SOUTHWEST = 21,
  LALIVERO_INTERIORS_2_FROM_LALIVERO_SOUTH = 22,
  LALIVERO_INTERIORS_2_FROM_LALIVERO_NORTHEAST = 23,
  LALIVERO_INTERIORS_2_FROM_LALIVERO_NORTHWEST = 30,
  LALIVERO_INTERIORS_2_FROM_LEMURIAN_SHIP = 31,
};
enum {
  LALIVERO_INTERIORS_2_TO_LALIVERO_SOUTHWEST = 21,
  LALIVERO_INTERIORS_2_TO_LALIVERO_SOUTH = 22,
  LALIVERO_INTERIORS_2_TO_LALIVERO_NORTHEAST = 23,
  LALIVERO_INTERIORS_2_TO_LALIVERO_NORTHWEST = 30,
  LALIVERO_INTERIORS_2_TO_LEMURIAN_SHIP = 31,
};

/// SceneId_VenusLighthouseEntry's entrances and exits.
enum {
  VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_WEST = 1,
  VENUS_LIGHTHOUSE_ENTRY_FROM_VENUS_LIGHTHOUSE_1 = 2,
  VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_EAST = 3,
};
enum {
  VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST = 1,
  VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_1 = 2,
  VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST = 3,
};

/// SceneId_VenusLighthouse1's entrances and exits.
enum {
  VENUS_LIGHTHOUSE_1_FROM_VENUS_LIGHTHOUSE_ENTRY = 1,
  VENUS_LIGHTHOUSE_1_FROM_VENUS_LIGHTHOUSE_2 = 23,
};
enum {
  VENUS_LIGHTHOUSE_1_TO_VENUS_LIGHTHOUSE_ENTRY = 1,
  VENUS_LIGHTHOUSE_1_TO_VENUS_LIGHTHOUSE_2 = 23,
};

/// SceneId_VenusLighthouse2's entrances and exits.
enum {
  VENUS_LIGHTHOUSE_2_FROM_VENUS_LIGHTHOUSE_3_SOUTH = 3,
  VENUS_LIGHTHOUSE_2_FROM_VENUS_LIGHTHOUSE_3_WEST = 8,
  VENUS_LIGHTHOUSE_2_FROM_VENUS_LIGHTHOUSE_3_NORTH = 12,
  VENUS_LIGHTHOUSE_2_FROM_VENUS_LIGHTHOUSE_1 = 16,
  VENUS_LIGHTHOUSE_2_FROM_TUNNEL_RUINS_2 = 18,
  VENUS_LIGHTHOUSE_2_FROM_VENUS_LIGHTHOUSE_3_EAST = 21,
};
enum {
  VENUS_LIGHTHOUSE_2_TO_VENUS_LIGHTHOUSE_3_NORTH = 3,
  VENUS_LIGHTHOUSE_2_TO_VENUS_LIGHTHOUSE_3_EAST = 8,
  VENUS_LIGHTHOUSE_2_TO_VENUS_LIGHTHOUSE_3_WEST = 12,
  VENUS_LIGHTHOUSE_2_TO_VENUS_LIGHTHOUSE_1 = 16,
  VENUS_LIGHTHOUSE_2_TO_TUNNEL_RUINS_2 = 18,
  VENUS_LIGHTHOUSE_2_TO_VENUS_LIGHTHOUSE_3_SOUTH = 21,
};

/// SceneId_VenusLighthouse3's entrances and exits.
enum {
  VENUS_LIGHTHOUSE_3_FROM_VENUS_LIGHTHOUSE_2_NORTH = 1,
  VENUS_LIGHTHOUSE_3_FROM_VENUS_LIGHTHOUSE_2_EAST = 4,
  VENUS_LIGHTHOUSE_3_FROM_VENUS_LIGHTHOUSE_4 = 6,
  VENUS_LIGHTHOUSE_3_FROM_VENUS_LIGHTHOUSE_2_WEST = 7,
  VENUS_LIGHTHOUSE_3_FROM_VENUS_LIGHTHOUSE_2_SOUTH = 11,
};
enum {
  VENUS_LIGHTHOUSE_3_TO_VENUS_LIGHTHOUSE_2_SOUTH = 1,
  VENUS_LIGHTHOUSE_3_TO_VENUS_LIGHTHOUSE_2_WEST = 4,
  VENUS_LIGHTHOUSE_3_TO_VENUS_LIGHTHOUSE_4 = 6,
  VENUS_LIGHTHOUSE_3_TO_VENUS_LIGHTHOUSE_2_NORTH = 7,
  VENUS_LIGHTHOUSE_3_TO_VENUS_LIGHTHOUSE_2_EAST = 11,
};

/// SceneId_VenusLighthouse4's entrances and exits.
enum {
  VENUS_LIGHTHOUSE_4_FROM_VENUS_LIGHTHOUSE_5_WEST_NORTHWEST = 1,
  VENUS_LIGHTHOUSE_4_FROM_VENUS_LIGHTHOUSE_5_NORTH_NORTHEAST = 7,
  VENUS_LIGHTHOUSE_4_FROM_VENUS_LIGHTHOUSE_5_NORTH_NORTHWEST = 10,
  VENUS_LIGHTHOUSE_4_FROM_VENUS_LIGHTHOUSE_5_NORTHEAST = 12,
  VENUS_LIGHTHOUSE_4_FROM_VENUS_LIGHTHOUSE_5_SOUTHWEST = 13,
  VENUS_LIGHTHOUSE_4_FROM_VENUS_LIGHTHOUSE_3 = 17,
  VENUS_LIGHTHOUSE_4_FROM_VENUS_LIGHTHOUSE_5_NORTHWEST = 19,
  VENUS_LIGHTHOUSE_4_FROM_VENUS_LIGHTHOUSE_5_EAST_NORTHEAST = 20,
};
enum {
  VENUS_LIGHTHOUSE_4_TO_VENUS_LIGHTHOUSE_5_WEST_NORTHWEST = 1,
  VENUS_LIGHTHOUSE_4_TO_VENUS_LIGHTHOUSE_5_NORTH_NORTHEAST = 7,
  VENUS_LIGHTHOUSE_4_TO_VENUS_LIGHTHOUSE_5_NORTH_NORTHWEST = 10,
  VENUS_LIGHTHOUSE_4_TO_VENUS_LIGHTHOUSE_5_NORTHEAST = 12,
  VENUS_LIGHTHOUSE_4_TO_VENUS_LIGHTHOUSE_5_SOUTHWEST = 13,
  VENUS_LIGHTHOUSE_4_TO_VENUS_LIGHTHOUSE_3 = 17,
  VENUS_LIGHTHOUSE_4_TO_VENUS_LIGHTHOUSE_5_NORTHWEST = 19,
  VENUS_LIGHTHOUSE_4_TO_VENUS_LIGHTHOUSE_5_EAST_NORTHEAST = 20,
};

/// SceneId_VenusLighthouse5's entrances and exits.
enum {
  VENUS_LIGHTHOUSE_5_FROM_VENUS_LIGHTHOUSE_4_WEST_NORTHWEST = 1,
  VENUS_LIGHTHOUSE_5_FROM_VENUS_LIGHTHOUSE_4_NORTH_NORTHWEST = 4,
  VENUS_LIGHTHOUSE_5_FROM_VENUS_LIGHTHOUSE_4_NORTH_NORTHEAST = 5,
  VENUS_LIGHTHOUSE_5_FROM_VENUS_LIGHTHOUSE_4_NORTHEAST = 7,
  VENUS_LIGHTHOUSE_5_FROM_VENUS_LIGHTHOUSE_4_SOUTHWEST = 9,
  VENUS_LIGHTHOUSE_5_FROM_VENUS_LIGHTHOUSE_4_NORTHWEST = 18,
  VENUS_LIGHTHOUSE_5_FROM_VENUS_LIGHTHOUSE_4_EAST_NORTHEAST = 19,
};
enum {
  VENUS_LIGHTHOUSE_5_TO_VENUS_LIGHTHOUSE_4_WEST_NORTHWEST = 1,
  VENUS_LIGHTHOUSE_5_TO_VENUS_LIGHTHOUSE_4_NORTH_NORTHWEST = 4,
  VENUS_LIGHTHOUSE_5_TO_VENUS_LIGHTHOUSE_4_NORTH_NORTHEAST = 5,
  VENUS_LIGHTHOUSE_5_TO_VENUS_LIGHTHOUSE_4_NORTHEAST = 7,
  VENUS_LIGHTHOUSE_5_TO_VENUS_LIGHTHOUSE_4_SOUTHWEST = 9,
  VENUS_LIGHTHOUSE_5_TO_VENUS_LIGHTHOUSE_4_NORTHWEST = 18,
  VENUS_LIGHTHOUSE_5_TO_VENUS_LIGHTHOUSE_4_EAST_NORTHEAST = 19,
  VENUS_LIGHTHOUSE_5_TO_VENUS_LIGHTHOUSE_AERIE = 21,
};

/// SceneId_VenusLighthouseAerie's entrances and exits.
enum {
  VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_5 = 1,
  VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP = 4,
  VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT =
      93, ///< the world map sends the party back here after its lighthouse cut
};

/// SceneId_LemurianShip's entrances and exits.
enum {
  LEMURIAN_SHIP_FROM_LALIVERO_INTERIORS_2 = 1,
};
enum {
  LEMURIAN_SHIP_TO_LALIVERO_INTERIORS_2 = 1,
};

/// SceneId_TolbiSpring2's entrances and exits.
enum {
  TOLBI_SPRING_2_FROM_TOLBI = 1,
};
enum {
  TOLBI_SPRING_2_TO_TOLBI = 1,
};

/// SceneId_LinkLobby's entrances and exits.
enum {
  LINK_LOBBY_FROM_CLEAR = 1,
};

#endif
