/// @file
/// The 380 enemy formations, by formation id, and the enemies a monster battle
/// never draws.

#include "battle/formations.h"
#include "enemy_ids.h"
#include "battle/battle.h"

// clang-format off
/// Each formation by its id, under the areas whose encounter zones
/// (encounter_zones.c) list it or the event battle that starts it; an id left
/// out has no enemies. The Battle Arena's monster battles may draw any
/// formation besides (BattleFormation_SelectLevelMatchedCandidate).
struct Formation gFormations[FORMATION_COUNT] = {
    // Vale, the stormy night
    [1] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Vale, the stormy night
    [2] = {
        .memberIds = {ENEMY_ROW(ENEMY_BAT)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Vale, the stormy night
    [3] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILD_MUSHROOM)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Vale, the stormy night
    [4] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN), ENEMY_ROW(ENEMY_BAT)},
        .minimumCounts = {1, 1},
        .maximumCounts = {1, 1},
    },
    // Vale's plaza, the stormy night
    [5] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_MYSTERY_WOMAN), ENEMY_ROW(ENEMY_MYSTERY_MAN)},
        .minimumCounts = {1, 1},
        .maximumCounts = {1, 1},
    },
    // Sol Sanctum
    [7] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Sol Sanctum
    [8] = {
        .memberIds = {ENEMY_ROW(ENEMY_BAT)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Sol Sanctum
    [9] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILD_MUSHROOM)},
        .minimumCounts = {1},
        .maximumCounts = {3},
    },
    // Sol Sanctum
    [10] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN), ENEMY_ROW(ENEMY_WILD_MUSHROOM), ENEMY_ROW(ENEMY_BAT)},
        .minimumCounts = {0, 0, 1},
        .maximumCounts = {1, 1, 2},
    },
    // Sol Sanctum
    [11] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_SOL_SANCTUM)},
        .minimumCounts = {1},
        .maximumCounts = {3},
    },
    // Sol Sanctum
    [12] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_SOL_SANCTUM), ENEMY_ROW(ENEMY_BAT)},
        .minimumCounts = {1},
        .maximumCounts = {2, 3},
    },
    // Sol Sanctum
    [13] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_SOL_SANCTUM), ENEMY_ROW(ENEMY_WILD_MUSHROOM)},
        .minimumCounts = {1},
        .maximumCounts = {2, 3},
    },
    // Sol Sanctum
    [14] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_SOL_SANCTUM), ENEMY_ROW(ENEMY_VERMIN), ENEMY_ROW(ENEMY_WILD_MUSHROOM), ENEMY_ROW(ENEMY_BAT)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1, 1, 1},
    },
    // Sol Sanctum
    [15] = {
        .memberIds = {ENEMY_ROW(ENEMY_AMAZE_SOL_SANCTUM)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Sol Sanctum
    [16] = {
        .memberIds = {ENEMY_ROW(ENEMY_AMAZE_SOL_SANCTUM), ENEMY_ROW(ENEMY_SLIME_SOL_SANCTUM), ENEMY_ROW(ENEMY_VERMIN)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2, 2},
    },
    // Sol Sanctum
    [17] = {
        .memberIds = {ENEMY_ROW(ENEMY_AMAZE_SOL_SANCTUM), ENEMY_ROW(ENEMY_BAT), ENEMY_ROW(ENEMY_WILD_MUSHROOM)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2, 2},
    },
    // Sol Sanctum
    [18] = {
        .memberIds = {ENEMY_ROW(ENEMY_AMAZE_SOL_SANCTUM)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // World Map, encounter region 1
    [20] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN_WORLD_MAP)},
        .minimumCounts = {1},
        .maximumCounts = {3},
    },
    // World Map, encounter region 1
    [21] = {
        .memberIds = {ENEMY_ROW(ENEMY_BAT_WORLD_MAP)},
        .minimumCounts = {1},
        .maximumCounts = {3},
    },
    // World Map, encounter region 1
    [22] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILD_MUSHROOM_WORLD_MAP)},
        .minimumCounts = {1},
        .maximumCounts = {3},
    },
    // World Map, encounter region 1
    [23] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_WORLD_MAP)},
        .minimumCounts = {1},
        .maximumCounts = {3},
    },
    // World Map, encounter region 1
    [24] = {
        .memberIds = {ENEMY_ROW(ENEMY_AMAZE_WORLD_MAP)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // World Map, encounter region 1
    [25] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // World Map, encounter region 1
    [26] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN_WORLD_MAP), ENEMY_ROW(ENEMY_BAT_WORLD_MAP), ENEMY_ROW(ENEMY_WILD_MUSHROOM_WORLD_MAP)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1, 1},
    },
    // World Map, encounter region 1
    [27] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_WORLD_MAP), ENEMY_ROW(ENEMY_AMAZE_WORLD_MAP), ENEMY_ROW(ENEMY_BAT_WORLD_MAP)},
        .minimumCounts = {0, 1},
        .maximumCounts = {1, 2, 1},
    },
    // the World Map's Djinn tutorial
    [28] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE_DJINN_LESSON)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Vault
    [29] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_BANDIT), ENEMY_ROW(ENEMY_THIEF), ENEMY_ROW(ENEMY_THIEF)},
        .minimumCounts = {1, 1, 1},
        .maximumCounts = {1, 1, 1},
    },
    // Goma Cave
    [31] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST), ENEMY_ROW(ENEMY_VERMIN_WORLD_MAP), ENEMY_ROW(ENEMY_WILD_MUSHROOM_WORLD_MAP)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2, 2},
    },
    // Goma Cave
    [32] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE), ENEMY_ROW(ENEMY_BAT_WORLD_MAP), ENEMY_ROW(ENEMY_SLIME_WORLD_MAP)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2, 2},
    },
    // Goma Cave
    [33] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Goma Cave
    [34] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE), ENEMY_ROW(ENEMY_GHOST)},
        .minimumCounts = {0, 1},
        .maximumCounts = {2, 2},
    },
    // Goma Cave
    [35] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Goma Cave
    [36] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILL_HEAD)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // Goma Cave
    [37] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON), ENEMY_ROW(ENEMY_GHOST)},
        .minimumCounts = {1, 1},
        .maximumCounts = {1, 2},
    },
    // in no encounter zone
    [46] = {
        .memberIds = {ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // in no encounter zone
    [47] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // in no encounter zone
    [48] = {
        .memberIds = {ENEMY_ROW(ENEMY_WORM), ENEMY_ROW(ENEMY_VERMIN), ENEMY_ROW(ENEMY_BAT)},
        .minimumCounts = {1},
        .maximumCounts = {1, 1, 1},
    },
    // World Map, encounter region 2; Kolima Tunnel
    [50] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // World Map, encounter region 2; Kolima Tunnel
    [51] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE), ENEMY_ROW(ENEMY_SKELETON)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // World Map, encounter region 2; Kolima Tunnel
    [52] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST), ENEMY_ROW(ENEMY_BAT_WORLD_MAP), ENEMY_ROW(ENEMY_ZOMBIE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2, 1},
    },
    // World Map, encounter region 2; Kolima Tunnel
    [53] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON), ENEMY_ROW(ENEMY_BAT_WORLD_MAP), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minimumCounts = {0, 0, 1},
        .maximumCounts = {1, 3, 2},
    },
    // World Map, encounter region 2; Kolima Tunnel
    [54] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // World Map, encounter region 2; Kolima Tunnel
    [55] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_SOLDIER), ENEMY_ROW(ENEMY_RAT), ENEMY_ROW(ENEMY_BAT_WORLD_MAP)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2, 1},
    },
    // World Map, encounter region 2; Kolima Tunnel
    [56] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE), ENEMY_ROW(ENEMY_GHOST)},
        .minimumCounts = {1},
        .maximumCounts = {3, 2},
    },
    // World Map, encounter region 2; Kolima Tunnel
    [57] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON), ENEMY_ROW(ENEMY_GHOST), ENEMY_ROW(ENEMY_WILL_HEAD)},
        .minimumCounts = {1},
        .maximumCounts = {3, 2, 1},
    },
    // Kolima Forest
    [59] = {
        .memberIds = {ENEMY_ROW(ENEMY_DRONE_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Kolima Forest
    [60] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minimumCounts = {1, 1},
        .maximumCounts = {2, 2},
    },
    // Kolima Forest
    [61] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILL_HEAD), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Kolima Forest
    [62] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Kolima Forest
    [63] = {
        .memberIds = {ENEMY_ROW(ENEMY_DRONE_BEE)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Kolima Forest
    [64] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON), ENEMY_ROW(ENEMY_WILL_HEAD)},
        .minimumCounts = {1},
        .maximumCounts = {3, 3},
    },
    // Kolima Forest
    [65] = {
        .memberIds = {ENEMY_ROW(ENEMY_DRONE_BEE), ENEMY_ROW(ENEMY_RAT_SOLDIER), ENEMY_ROW(ENEMY_RAT)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2, 2},
    },
    // Kolima Forest
    [66] = {
        .memberIds = {ENEMY_ROW(ENEMY_TROLL)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Tret Tree
    [68] = {
        .memberIds = {ENEMY_ROW(ENEMY_SPIDER)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // Tret Tree
    [69] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_SOLDIER), ENEMY_ROW(ENEMY_RAT), ENEMY_ROW(ENEMY_DRONE_BEE)},
        .minimumCounts = {0, 0, 1},
        .maximumCounts = {2, 2, 2},
    },
    // Tret Tree
    [70] = {
        .memberIds = {ENEMY_ROW(ENEMY_SPIDER), ENEMY_ROW(ENEMY_DRONE_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Tret Tree
    [71] = {
        .memberIds = {ENEMY_ROW(ENEMY_TROLL), ENEMY_ROW(ENEMY_RAT), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minimumCounts = {1, 1, 1},
        .maximumCounts = {1, 1, 1},
    },
    // Tret Tree
    [72] = {
        .memberIds = {ENEMY_ROW(ENEMY_CREEPER)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Tret Tree
    [73] = {
        .memberIds = {ENEMY_ROW(ENEMY_SPIDER), ENEMY_ROW(ENEMY_TROLL)},
        .minimumCounts = {1},
        .maximumCounts = {3, 1},
    },
    // Tret Tree
    [74] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_CREEPER), ENEMY_ROW(ENEMY_CREEPER)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1, 1},
    },
    // Tret Tree
    [75] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_TROLL), ENEMY_ROW(ENEMY_TROLL)},
        .minimumCounts = {1},
        .maximumCounts = {1, 1, 1},
    },
    // Bilibin Cave
    [77] = {
        .memberIds = {ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Bilibin Cave
    [78] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOUL)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Bilibin Cave
    [79] = {
        .memberIds = {ENEMY_ROW(ENEMY_TROLL), ENEMY_ROW(ENEMY_DRONE_BEE), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minimumCounts = {1, 1, 1},
        .maximumCounts = {1, 1, 1},
    },
    // Bilibin Cave
    [80] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOUL), ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Bilibin Cave
    [81] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_TROLL)},
        .minimumCounts = {1, 1},
        .maximumCounts = {2, 1},
    },
    // Bilibin Cave
    [82] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Bilibin Cave
    [83] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOUL), ENEMY_ROW(ENEMY_GNOME)},
        .minimumCounts = {1},
        .maximumCounts = {3, 2},
    },
    // Bilibin Cave
    [84] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOUL)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD
    [86] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME)},
        .minimumCounts = {1},
        .maximumCounts = {3},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD
    [87] = {
        .memberIds = {ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {2},
        .maximumCounts = {4},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD
    [88] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAULER), ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {1, 1},
        .maximumCounts = {1, 3},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD
    [89] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAULER), ENEMY_ROW(ENEMY_GNOME)},
        .minimumCounts = {1, 1},
        .maximumCounts = {1, 3},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD
    [90] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAULER)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // Mercury Lighthouse
    [92] = {
        .memberIds = {ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {3},
        .maximumCounts = {5},
    },
    // Mercury Lighthouse
    [93] = {
        .memberIds = {ENEMY_ROW(ENEMY_CUTTLE), ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Mercury Lighthouse
    [94] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Mercury Lighthouse
    [95] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_MAN)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Mercury Lighthouse
    [96] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAULER)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Mercury Lighthouse
    [97] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_MAN), ENEMY_ROW(ENEMY_CUTTLE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Mercury Lighthouse
    [98] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOUL)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Mercury Lighthouse
    [99] = {
        .memberIds = {ENEMY_ROW(ENEMY_SIREN)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Mercury Lighthouse
    [100] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_MAN), ENEMY_ROW(ENEMY_HARPY)},
        .minimumCounts = {1, 1},
        .maximumCounts = {2, 1},
    },
    // Mercury Lighthouse
    [101] = {
        .memberIds = {ENEMY_ROW(ENEMY_CUTTLE)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Mercury Lighthouse
    [102] = {
        .memberIds = {ENEMY_ROW(ENEMY_HARPY)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Mercury Lighthouse
    [103] = {
        .memberIds = {ENEMY_ROW(ENEMY_SIREN), ENEMY_ROW(ENEMY_HARPY)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Mercury Lighthouse
    [104] = {
        .memberIds = {ENEMY_ROW(ENEMY_SIREN)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Tret Tree
    [106] = {
        .memberIds = {ENEMY_ROW(ENEMY_TRET)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Mercury Lighthouse
    [107] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_MAN)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Mercury Lighthouse Aerie
    [108] = {
        .memberIds = {ENEMY_ROW(ENEMY_SATUROS_MERCURY_LIGHTHOUSE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // World Map, encounter region 31; Fuchin Falls Cave
    [120] = {
        .memberIds = {ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {3},
        .maximumCounts = {4},
    },
    // World Map, encounter region 31; Fuchin Falls Cave
    [121] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAULER)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // World Map, encounter region 31; Fuchin Falls Cave
    [122] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_MAN)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // World Map, encounter region 31; Fuchin Falls Cave
    [123] = {
        .memberIds = {ENEMY_ROW(ENEMY_MOLE), ENEMY_ROW(ENEMY_OOZE)},
        .minimumCounts = {1, 1},
        .maximumCounts = {2, 2},
    },
    // Fuchin Falls Cave
    [124] = {
        .memberIds = {ENEMY_ROW(ENEMY_SPIRIT)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // Fuchin Falls Cave
    [125] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRGE), ENEMY_ROW(ENEMY_MOLE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Fuchin Falls Cave
    [126] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Fuchin Falls Cave
    [127] = {
        .memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER), ENEMY_ROW(ENEMY_SPIRIT)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Mogall Forest
    [129] = {
        .memberIds = {ENEMY_ROW(ENEMY_MOLE)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Mogall Forest
    [130] = {
        .memberIds = {ENEMY_ROW(ENEMY_MOLE), ENEMY_ROW(ENEMY_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Mogall Forest
    [131] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRGE), ENEMY_ROW(ENEMY_SPIRIT)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Mogall Forest
    [132] = {
        .memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER), ENEMY_ROW(ENEMY_SPIRIT)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Mogall Forest
    [133] = {
        .memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Mogall Forest
    [134] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Mogall Forest
    [135] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD), ENEMY_ROW(ENEMY_SPIRIT)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Mogall Forest
    [136] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Mogall Forest
    [138] = {
        .memberIds = {ENEMY_ROW(ENEMY_APE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Mogall Forest
    [139] = {
        .memberIds = {ENEMY_ROW(ENEMY_KILLER_APE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // World Map, encounter region 3
    [141] = {
        .memberIds = {ENEMY_ROW(ENEMY_MOLE), ENEMY_ROW(ENEMY_DIRGE)},
        .minimumCounts = {1, 1},
        .maximumCounts = {2, 2},
    },
    // World Map, encounter region 3
    [142] = {
        .memberIds = {ENEMY_ROW(ENEMY_APE), ENEMY_ROW(ENEMY_MOLE)},
        .minimumCounts = {1, 1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 3
    [143] = {
        .memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER), ENEMY_ROW(ENEMY_SPIRIT)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // World Map, encounter region 3
    [144] = {
        .memberIds = {ENEMY_ROW(ENEMY_APE), ENEMY_ROW(ENEMY_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 3
    [145] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD), ENEMY_ROW(ENEMY_SPIRIT)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // World Map, encounter region 3
    [146] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Altin Peak
    [148] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRGE), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Altin Peak
    [149] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_BEAST), ENEMY_ROW(ENEMY_APE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // Altin Peak
    [150] = {
        .memberIds = {ENEMY_ROW(ENEMY_TARANTULA), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Altin Peak
    [151] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_BEAST), ENEMY_ROW(ENEMY_CALAMAR)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // Altin Peak
    [152] = {
        .memberIds = {ENEMY_ROW(ENEMY_TARANTULA), ENEMY_ROW(ENEMY_SLIME_BEAST)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Altin Peak
    [153] = {
        .memberIds = {ENEMY_ROW(ENEMY_CALAMAR)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Altin Peak
    [154] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_FIGHTER)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Altin Peak
    [155] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_FIGHTER), ENEMY_ROW(ENEMY_APE)},
        .minimumCounts = {1},
        .maximumCounts = {3, 2},
    },
    // Altin Peak
    [157] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIVING_STATUE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Altin Peak
    [158] = {
        .memberIds = {ENEMY_ROW(ENEMY_HYDROS_STATUE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Lamakan Desert
    [160] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // Lamakan Desert
    [161] = {
        .memberIds = {ENEMY_ROW(ENEMY_GRUB), ENEMY_ROW(ENEMY_RAT_FIGHTER)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Lamakan Desert
    [162] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC), ENEMY_ROW(ENEMY_RAT_FIGHTER)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Lamakan Desert
    [163] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE), ENEMY_ROW(ENEMY_ORC)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Lamakan Desert
    [164] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE), ENEMY_ROW(ENEMY_GRUB)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Lamakan Desert
    [165] = {
        .memberIds = {ENEMY_ROW(ENEMY_SALAMANDER), ENEMY_ROW(ENEMY_GRUB)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Lamakan Desert
    [166] = {
        .memberIds = {ENEMY_ROW(ENEMY_SALAMANDER), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Lamakan Desert
    [167] = {
        .memberIds = {ENEMY_ROW(ENEMY_SALAMANDER)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Lamakan Desert
    [169] = {
        .memberIds = {ENEMY_ROW(ENEMY_ANT_LION)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Lamakan Desert
    [170] = {
        .memberIds = {ENEMY_ROW(ENEMY_MANTICORE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // World Map, encounter region 4
    [180] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // World Map, encounter region 4
    [181] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // World Map, encounter region 4
    [182] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // World Map, encounter region 4
    [183] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 4
    [184] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 4
    [185] = {
        .memberIds = {ENEMY_ROW(ENEMY_UNDEAD)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // World Map, encounter region 4
    [186] = {
        .memberIds = {ENEMY_ROW(ENEMY_UNDEAD), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 4
    [187] = {
        .memberIds = {ENEMY_ROW(ENEMY_UNDEAD), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Vale Cave
    [189] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Vale Cave
    [190] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Vale Cave
    [191] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Vale Cave
    [192] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAVAGER)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Vale Cave
    [193] = {
        .memberIds = {ENEMY_ROW(ENEMY_UNDEAD), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Vale Cave
    [194] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAVAGER), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Vale Cave
    [195] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST_MAGE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Vale Cave
    [196] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST_MAGE), ENEMY_ROW(ENEMY_UNDEAD)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Vault Cave
    [201] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Vault Cave
    [202] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAVAGER), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Vault Cave
    [203] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAVAGER), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Vault Cave
    [204] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST_MAGE), ENEMY_ROW(ENEMY_UNDEAD)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Vault Cave
    [205] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST_MAGE)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Vault Cave
    [206] = {
        .memberIds = {ENEMY_ROW(ENEMY_GARGOYLE)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Vault Cave
    [207] = {
        .memberIds = {ENEMY_ROW(ENEMY_CAVE_TROLL), ENEMY_ROW(ENEMY_GARGOYLE)},
        .minimumCounts = {1, 1},
        .maximumCounts = {1, 1},
    },
    // Vault Cave
    [208] = {
        .memberIds = {ENEMY_ROW(ENEMY_CAVE_TROLL)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // the Tolbi-bound ship
    [210] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAN_O_WAR)},
        .minimumCounts = {3},
        .maximumCounts = {3},
    },
    // the Tolbi-bound ship
    [211] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_FIGHTER), ENEMY_ROW(ENEMY_RABID_BAT)},
        .minimumCounts = {1, 2},
        .maximumCounts = {1, 2},
    },
    // the Tolbi-bound ship
    [212] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_VIRAGO), ENEMY_ROW(ENEMY_MAN_O_WAR)},
        .minimumCounts = {1, 2},
        .maximumCounts = {1, 2},
    },
    // the Tolbi-bound ship
    [213] = {
        .memberIds = {ENEMY_ROW(ENEMY_KRAKEN)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // World Map, encounter region 5
    [220] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_MOLE)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // World Map, encounter region 5
    [221] = {
        .memberIds = {ENEMY_ROW(ENEMY_ARMORED_RAT), ENEMY_ROW(ENEMY_RABID_BAT)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 5
    [222] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_MOLE), ENEMY_ROW(ENEMY_RABID_BAT)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 5
    [223] = {
        .memberIds = {ENEMY_ROW(ENEMY_ARMORED_RAT), ENEMY_ROW(ENEMY_FOUL_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 5
    [224] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_MOLE), ENEMY_ROW(ENEMY_FOUL_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 5
    [225] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // World Map, encounter region 5
    [226] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE), ENEMY_ROW(ENEMY_MAD_MOLE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 5
    [227] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE), ENEMY_ROW(ENEMY_FOUL_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Altmiller Cave
    [229] = {
        .memberIds = {ENEMY_ROW(ENEMY_ARMORED_RAT), ENEMY_ROW(ENEMY_FOUL_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Altmiller Cave
    [230] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_MOLE), ENEMY_ROW(ENEMY_WORM)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Altmiller Cave
    [231] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE), ENEMY_ROW(ENEMY_WIGHT)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Altmiller Cave
    [232] = {
        .memberIds = {ENEMY_ROW(ENEMY_GOLEM), ENEMY_ROW(ENEMY_WIGHT)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Altmiller Cave
    [233] = {
        .memberIds = {ENEMY_ROW(ENEMY_CLAY_GARGOYLE), ENEMY_ROW(ENEMY_MAD_MOLE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Altmiller Cave
    [234] = {
        .memberIds = {ENEMY_ROW(ENEMY_GOLEM), ENEMY_ROW(ENEMY_CLAY_GARGOYLE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Altmiller Cave
    [235] = {
        .memberIds = {ENEMY_ROW(ENEMY_DREAD_HOUND)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // Altmiller Cave
    [236] = {
        .memberIds = {ENEMY_ROW(ENEMY_DREAD_HOUND), ENEMY_ROW(ENEMY_GOLEM)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Colosso
    [238] = {
        .memberIds = {ENEMY_ROW(ENEMY_AZART_IRON_SHIELD)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Colosso
    [239] = {
        .memberIds = {ENEMY_ROW(ENEMY_AZART_PADDED_GLOVES)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Colosso
    [240] = {
        .memberIds = {ENEMY_ROW(ENEMY_SATRAGE_CHAIN_MAIL)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Colosso
    [241] = {
        .memberIds = {ENEMY_ROW(ENEMY_SATRAGE_COTTON_SHIRT)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Colosso
    [242] = {
        .memberIds = {ENEMY_ROW(ENEMY_NAVAMPA_CLAYMORE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Colosso
    [243] = {
        .memberIds = {ENEMY_ROW(ENEMY_NAVAMPA_HUNTERS_SWORD)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Gondowan Cave
    [250] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // Gondowan Cave
    [251] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_WARRIOR), ENEMY_ROW(ENEMY_WARRIOR_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Gondowan Cave
    [252] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRTY_APE)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // Gondowan Cave
    [253] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD)},
        .minimumCounts = {1},
        .maximumCounts = {2},
    },
    // Gondowan Cave
    [254] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRTY_APE), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Gondowan Cave
    [255] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Gondowan Cave
    [256] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRTY_APE), ENEMY_ROW(ENEMY_WARRIOR_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Gondowan Cave
    [257] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD)},
        .minimumCounts = {2},
        .maximumCounts = {3},
    },
    // Lunpa Fortress
    [259] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_WARRIOR), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // Lunpa Fortress
    [260] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // Lunpa Fortress
    [261] = {
        .memberIds = {ENEMY_ROW(ENEMY_VILE_DIRGE), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // Lunpa Fortress
    [262] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Lunpa Fortress
    [264] = {
        .memberIds = {ENEMY_ROW(ENEMY_BRIGAND)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Lunpa Fortress
    [265] = {
        .memberIds = {ENEMY_ROW(ENEMY_BRIGAND)},
        .minimumCounts = {3},
        .maximumCounts = {3},
    },
    // Lunpa Fortress
    [266] = {
        .memberIds = {ENEMY_ROW(ENEMY_TOADONPA)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // World Map, encounter region 6
    [268] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC_CAPTAIN), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minimumCounts = {1},
        .maximumCounts = {1, 1},
    },
    // World Map, encounter region 6
    [269] = {
        .memberIds = {ENEMY_ROW(ENEMY_WARRIOR_BEE), ENEMY_ROW(ENEMY_KOBOLD)},
        .minimumCounts = {1},
        .maximumCounts = {1, 1},
    },
    // World Map, encounter region 6
    [270] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRTY_APE), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 6
    [271] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRTY_APE), ENEMY_ROW(ENEMY_WARRIOR_BEE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // World Map, encounter region 6
    [272] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // World Map, encounter region 6
    [273] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC_CAPTAIN), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // World Map, encounter region 6
    [274] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC_CAPTAIN)},
        .minimumCounts = {1},
        .maximumCounts = {3},
    },
    // World Map, encounter region 6
    [275] = {
        .memberIds = {ENEMY_ROW(ENEMY_BRUTAL_TROLL)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Suhalla Desert
    [277] = {
        .memberIds = {ENEMY_ROW(ENEMY_ACID_MAGGOT), ENEMY_ROW(ENEMY_KOBOLD)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Suhalla Desert
    [278] = {
        .memberIds = {ENEMY_ROW(ENEMY_ROACH), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Suhalla Desert
    [279] = {
        .memberIds = {ENEMY_ROW(ENEMY_BRUTAL_TROLL)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Suhalla Desert
    [280] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAGICORE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Suhalla Desert
    [281] = {
        .memberIds = {ENEMY_ROW(ENEMY_HARRIDAN), ENEMY_ROW(ENEMY_ORC_CAPTAIN)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Suhalla Desert
    [282] = {
        .memberIds = {ENEMY_ROW(ENEMY_STONE_SOLDIER)},
        .minimumCounts = {1},
        .maximumCounts = {3},
    },
    // in no encounter zone
    [283] = {
        .memberIds = {ENEMY_ROW(ENEMY_STONE_SOLDIER), ENEMY_ROW(ENEMY_ACID_MAGGOT)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Suhalla Desert
    [284] = {
        .memberIds = {ENEMY_ROW(ENEMY_HARRIDAN), ENEMY_ROW(ENEMY_ROACH)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Suhalla Desert
    [285] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAGICORE), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // Suhalla Desert and the World Map
    [287] = {
        .memberIds = {ENEMY_ROW(ENEMY_TORNADO_LIZARD)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Suhalla Desert
    [288] = {
        .memberIds = {ENEMY_ROW(ENEMY_STORM_LIZARD)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Suhalla Desert and the World Map
    [289] = {
        .memberIds = {ENEMY_ROW(ENEMY_TEMPEST_LIZARD)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Crossbone Isle
    [291] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_HOBGOBLIN), ENEMY_ROW(ENEMY_VIRAGO)},
        .minimumCounts = {1, 2},
        .maximumCounts = {1, 2},
    },
    // Crossbone Isle
    [292] = {
        .memberIds = {ENEMY_ROW(ENEMY_GRISLY), ENEMY_ROW(ENEMY_SUCCUBUS)},
        .minimumCounts = {2, 1},
        .maximumCounts = {2, 1},
    },
    // Crossbone Isle
    [293] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_LICH), ENEMY_ROW(ENEMY_FIENDISH_GHOUL)},
        .minimumCounts = {1, 2},
        .maximumCounts = {1, 2},
    },
    // Crossbone Isle
    [294] = {
        .memberIds = {ENEMY_ROW(ENEMY_GRYPHON)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Crossbone Isle
    [296] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_KING), ENEMY_ROW(ENEMY_HARRIDAN), ENEMY_ROW(ENEMY_STONE_SOLDIER)},
        .minimumCounts = {1, 1, 2},
        .maximumCounts = {1, 1, 2},
    },
    // Crossbone Isle
    [297] = {
        .memberIds = {ENEMY_ROW(ENEMY_CHIMERA)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Crossbone Isle
    [298] = {
        .memberIds = {ENEMY_ROW(ENEMY_EARTH_LIZARD)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Crossbone Isle
    [300] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_POISON_TOAD), ENEMY_ROW(ENEMY_THUNDER_LIZARD)},
        .minimumCounts = {1, 2},
        .maximumCounts = {1, 2},
    },
    // Crossbone Isle
    [301] = {
        .memberIds = {ENEMY_ROW(ENEMY_CEREBUS)},
        .minimumCounts = {2},
        .maximumCounts = {2},
    },
    // Crossbone Isle
    [302] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEADBEARD)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Suhalla Gate; Gateway Cave
    [310] = {
        .memberIds = {ENEMY_ROW(ENEMY_CANNIBAL_GHOUL), ENEMY_ROW(ENEMY_HORNED_GHOST)},
        .minimumCounts = {1},
        .maximumCounts = {1, 1},
    },
    // Suhalla Gate; Gateway Cave
    [311] = {
        .memberIds = {ENEMY_ROW(ENEMY_HORNED_GHOST), ENEMY_ROW(ENEMY_GNOME_WIZARD)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Suhalla Gate; Gateway Cave
    [312] = {
        .memberIds = {ENEMY_ROW(ENEMY_EARTH_GOLEM), ENEMY_ROW(ENEMY_GOBLIN)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Suhalla Gate; Gateway Cave
    [313] = {
        .memberIds = {ENEMY_ROW(ENEMY_MOLE_MAGE), ENEMY_ROW(ENEMY_EARTH_GOLEM)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // Suhalla Gate; Gateway Cave
    [314] = {
        .memberIds = {ENEMY_ROW(ENEMY_NIGHTMARE), ENEMY_ROW(ENEMY_HORNED_GHOST)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Suhalla Gate; Gateway Cave
    [315] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_WIZARD), ENEMY_ROW(ENEMY_CANNIBAL_GHOUL)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Suhalla Gate; Gateway Cave
    [316] = {
        .memberIds = {ENEMY_ROW(ENEMY_GOBLIN), ENEMY_ROW(ENEMY_MOLE_MAGE)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Suhalla Gate; Gateway Cave
    [317] = {
        .memberIds = {ENEMY_ROW(ENEMY_NIGHTMARE), ENEMY_ROW(ENEMY_EARTH_GOLEM)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Venus Lighthouse
    [319] = {
        .memberIds = {ENEMY_ROW(ENEMY_HORNED_GHOST), ENEMY_ROW(ENEMY_NIGHTMARE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Venus Lighthouse
    [320] = {
        .memberIds = {ENEMY_ROW(ENEMY_EARTH_GOLEM), ENEMY_ROW(ENEMY_HORNED_GHOST)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Venus Lighthouse
    [321] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_WIZARD), ENEMY_ROW(ENEMY_EARTH_GOLEM)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Venus Lighthouse
    [322] = {
        .memberIds = {ENEMY_ROW(ENEMY_NIGHTMARE), ENEMY_ROW(ENEMY_GOBLIN)},
        .minimumCounts = {2},
        .maximumCounts = {2, 1},
    },
    // Venus Lighthouse
    [323] = {
        .memberIds = {ENEMY_ROW(ENEMY_NIGHTMARE), ENEMY_ROW(ENEMY_EARTH_GOLEM), ENEMY_ROW(ENEMY_HORNED_GHOST)},
        .minimumCounts = {1, 1, 1},
        .maximumCounts = {1, 1, 1},
    },
    // Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins
    [328] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_WIZARD), ENEMY_ROW(ENEMY_PLATED_RAT)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins
    [329] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKULL_WARRIOR), ENEMY_ROW(ENEMY_WILLOWISP)},
        .minimumCounts = {1},
        .maximumCounts = {2, 2},
    },
    // Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins
    [330] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC_LORD), ENEMY_ROW(ENEMY_WILD_GRYPHON)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins
    [331] = {
        .memberIds = {ENEMY_ROW(ENEMY_ICE_GARGOYLE), ENEMY_ROW(ENEMY_NIGHTMARE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins
    [332] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILD_GRYPHON), ENEMY_ROW(ENEMY_MANTICORE_KING)},
        .minimumCounts = {1},
        .maximumCounts = {1, 1},
    },
    // Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins
    [333] = {
        .memberIds = {ENEMY_ROW(ENEMY_MANTICORE_KING), ENEMY_ROW(ENEMY_WILLOWISP)},
        .minimumCounts = {1},
        .maximumCounts = {1, 3},
    },
    // Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins
    [334] = {
        .memberIds = {ENEMY_ROW(ENEMY_CHIMERA_MAGE), ENEMY_ROW(ENEMY_ICE_GARGOYLE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins
    [335] = {
        .memberIds = {ENEMY_ROW(ENEMY_CHIMERA_MAGE), ENEMY_ROW(ENEMY_MANTICORE_KING)},
        .minimumCounts = {1},
        .maximumCounts = {1, 1},
    },
    // Venus Lighthouse
    [337] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILD_GRYPHON), ENEMY_ROW(ENEMY_GRAND_GOLEM)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // Venus Lighthouse
    [338] = {
        .memberIds = {ENEMY_ROW(ENEMY_MANTICORE_KING), ENEMY_ROW(ENEMY_SKULL_WARRIOR)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Venus Lighthouse
    [339] = {
        .memberIds = {ENEMY_ROW(ENEMY_CHIMERA_MAGE), ENEMY_ROW(ENEMY_FENRIR)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Venus Lighthouse
    [340] = {
        .memberIds = {ENEMY_ROW(ENEMY_RECLUSE), ENEMY_ROW(ENEMY_WILLOWISP)},
        .minimumCounts = {1},
        .maximumCounts = {2, 3},
    },
    // Venus Lighthouse
    [341] = {
        .memberIds = {ENEMY_ROW(ENEMY_BOULDER_BEAST), ENEMY_ROW(ENEMY_THUNDER_LIZARD)},
        .minimumCounts = {1},
        .maximumCounts = {2, 1},
    },
    // Venus Lighthouse
    [342] = {
        .memberIds = {ENEMY_ROW(ENEMY_GRAND_GOLEM), ENEMY_ROW(ENEMY_BOULDER_BEAST)},
        .minimumCounts = {1},
        .maximumCounts = {1, 4},
    },
    // Venus Lighthouse
    [343] = {
        .memberIds = {ENEMY_ROW(ENEMY_FENRIR), ENEMY_ROW(ENEMY_ICE_GARGOYLE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Venus Lighthouse
    [344] = {
        .memberIds = {ENEMY_ROW(ENEMY_THUNDER_LIZARD), ENEMY_ROW(ENEMY_CHIMERA_MAGE)},
        .minimumCounts = {1},
        .maximumCounts = {1, 2},
    },
    // Venus Lighthouse Aerie
    [346] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_MENARDI), ENEMY_ROW(ENEMY_SATUROS_VENUS_LIGHTHOUSE)},
        .minimumCounts = {1, 1},
        .maximumCounts = {1, 1},
    },
    // Venus Lighthouse Aerie
    [FORMATION_FUSION_DRAGON] = {
        .memberIds = {ENEMY_ROW(ENEMY_FUSION_DRAGON)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // a mimic chest
    [350] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_MERCURY_LIGHTHOUSE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // a mimic chest
    [351] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_FUCHIN_FALLS_CAVE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // a mimic chest
    [352] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_ALTIN_PEAK)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // a mimic chest
    [353] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_VAULT_CAVE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // a mimic chest
    [354] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_CROSSBONE_ISLE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // a mimic chest
    [355] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_ALTMILLER_CAVE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // a mimic chest
    [356] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_SUHALLA_DESERT)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // a mimic chest
    [357] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_PIRATE_SHIP)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Quartz
    [360] = {
        .memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_QUARTZ)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Vine
    [361] = {
        .memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_VINE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // Bane's row of gEncounter_DjinnBattles; no event starts it
    [362] = {
        .memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_UNUSED_BANE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Sleet
    [365] = {
        .memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_SLEET)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Spritz
    [366] = {
        .memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_SPRITZ)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Hail
    [367] = {
        .memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_HAIL)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Dew
    [368] = {
        .memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_DEW)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Forge
    [371] = {
        .memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_FORGE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Corona
    [372] = {
        .memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_CORONA)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Flash
    [373] = {
        .memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_FLASH)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Breeze
    [376] = {
        .memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_BREEZE)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Zephyr
    [377] = {
        .memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_ZEPHYR)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Squall
    [378] = {
        .memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_SQUALL)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
    // the battle with the Djinni Luff
    [379] = {
        .memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_LUFF)},
        .minimumCounts = {1},
        .maximumCounts = {1},
    },
};

/// The 20 enemies whose defeated flags a monster battle clears before it
/// picks a formation (BattleFormation_SelectLevelMatchedCandidate).
u16 gBattleFormation_ExcludedEnemies[] = {
    ENEMY_ZOMBIE_DJINN_LESSON,
    ENEMY_MYSTERY_MAN,
    ENEMY_MYSTERY_WOMAN,
    ENEMY_AZART_PADDED_GLOVES,
    ENEMY_SATRAGE_COTTON_SHIRT,
    ENEMY_NAVAMPA_HUNTERS_SWORD,
    ENEMY_VENUS_DJINNI_QUARTZ,
    ENEMY_VENUS_DJINNI_VINE,
    ENEMY_VENUS_DJINNI_UNUSED_BANE,
    ENEMY_MERCURY_DJINNI_SLEET,
    ENEMY_MERCURY_DJINNI_SPRITZ,
    ENEMY_MERCURY_DJINNI_HAIL,
    ENEMY_MERCURY_DJINNI_DEW,
    ENEMY_MARS_DJINNI_FORGE,
    ENEMY_MARS_DJINNI_CORONA,
    ENEMY_MARS_DJINNI_FLASH,
    ENEMY_JUPITER_DJINNI_BREEZE,
    ENEMY_JUPITER_DJINNI_ZEPHYR,
    ENEMY_JUPITER_DJINNI_SQUALL,
    ENEMY_JUPITER_DJINNI_LUFF,
};
// clang-format on
