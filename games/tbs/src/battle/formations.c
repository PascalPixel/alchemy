/* @file */
/* The 380 enemy formations, by formation id, and the enemies a monster battle
 */
/* never draws. */

#include "battle/formations.h"
#include "enemy_ids.h"

/* clang-format off */
/* Each formation by its id, under the areas whose encounter zones */
/* (encounter_zones.c) list it or the event battle that starts it; an id left */
/* out has no enemies. The Battle Arena's monster battles may draw any */
/* formation besides (btFormationSelectLevelMatchedCandidate). */
FORMATION Formations[FORMATION_COUNT] = {
    /* Vale, the stormy night */
    [1] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Vale, the stormy night */
    [2] = {
        .memberIds = {ENEMY_ROW(ENEMY_BAT)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Vale, the stormy night */
    [3] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILD_MUSHROOM)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Vale, the stormy night */
    [4] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN), ENEMY_ROW(ENEMY_BAT)},
        .minCnts = {1, 1},
        .maxCnts = {1, 1},
    },
    /* Vale's plaza, the stormy night */
    [5] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_MYSTERY_WOMAN), ENEMY_ROW(ENEMY_MYSTERY_MAN)},
        .minCnts = {1, 1},
        .maxCnts = {1, 1},
    },
    /* Sol Sanctum */
    [7] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Sol Sanctum */
    [8] = {
        .memberIds = {ENEMY_ROW(ENEMY_BAT)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Sol Sanctum */
    [9] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILD_MUSHROOM)},
        .minCnts = {1},
        .maxCnts = {3},
    },
    /* Sol Sanctum */
    [10] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN), ENEMY_ROW(ENEMY_WILD_MUSHROOM), ENEMY_ROW(ENEMY_BAT)},
        .minCnts = {0, 0, 1},
        .maxCnts = {1, 1, 2},
    },
    /* Sol Sanctum */
    [11] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_SOL_SANCTUM)},
        .minCnts = {1},
        .maxCnts = {3},
    },
    /* Sol Sanctum */
    [12] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_SOL_SANCTUM), ENEMY_ROW(ENEMY_BAT)},
        .minCnts = {1},
        .maxCnts = {2, 3},
    },
    /* Sol Sanctum */
    [13] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_SOL_SANCTUM), ENEMY_ROW(ENEMY_WILD_MUSHROOM)},
        .minCnts = {1},
        .maxCnts = {2, 3},
    },
    /* Sol Sanctum */
    [14] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_SOL_SANCTUM), ENEMY_ROW(ENEMY_VERMIN), ENEMY_ROW(ENEMY_WILD_MUSHROOM), ENEMY_ROW(ENEMY_BAT)},
        .minCnts = {1},
        .maxCnts = {2, 1, 1, 1},
    },
    /* Sol Sanctum */
    [15] = {
        .memberIds = {ENEMY_ROW(ENEMY_AMAZE_SOL_SANCTUM)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Sol Sanctum */
    [16] = {
        .memberIds = {ENEMY_ROW(ENEMY_AMAZE_SOL_SANCTUM), ENEMY_ROW(ENEMY_SLIME_SOL_SANCTUM), ENEMY_ROW(ENEMY_VERMIN)},
        .minCnts = {1},
        .maxCnts = {1, 2, 2},
    },
    /* Sol Sanctum */
    [17] = {
        .memberIds = {ENEMY_ROW(ENEMY_AMAZE_SOL_SANCTUM), ENEMY_ROW(ENEMY_BAT), ENEMY_ROW(ENEMY_WILD_MUSHROOM)},
        .minCnts = {1},
        .maxCnts = {1, 2, 2},
    },
    /* Sol Sanctum */
    [18] = {
        .memberIds = {ENEMY_ROW(ENEMY_AMAZE_SOL_SANCTUM)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* World Map, encounter region 1 */
    [20] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN_WORLD_MAP)},
        .minCnts = {1},
        .maxCnts = {3},
    },
    /* World Map, encounter region 1 */
    [21] = {
        .memberIds = {ENEMY_ROW(ENEMY_BAT_WORLD_MAP)},
        .minCnts = {1},
        .maxCnts = {3},
    },
    /* World Map, encounter region 1 */
    [22] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILD_MUSHROOM_WORLD_MAP)},
        .minCnts = {1},
        .maxCnts = {3},
    },
    /* World Map, encounter region 1 */
    [23] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_WORLD_MAP)},
        .minCnts = {1},
        .maxCnts = {3},
    },
    /* World Map, encounter region 1 */
    [24] = {
        .memberIds = {ENEMY_ROW(ENEMY_AMAZE_WORLD_MAP)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* World Map, encounter region 1 */
    [25] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* World Map, encounter region 1 */
    [26] = {
        .memberIds = {ENEMY_ROW(ENEMY_VERMIN_WORLD_MAP), ENEMY_ROW(ENEMY_BAT_WORLD_MAP), ENEMY_ROW(ENEMY_WILD_MUSHROOM_WORLD_MAP)},
        .minCnts = {1},
        .maxCnts = {2, 1, 1},
    },
    /* World Map, encounter region 1 */
    [27] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_WORLD_MAP), ENEMY_ROW(ENEMY_AMAZE_WORLD_MAP), ENEMY_ROW(ENEMY_BAT_WORLD_MAP)},
        .minCnts = {0, 1},
        .maxCnts = {1, 2, 1},
    },
    /* the World Map's Djinn tutorial */
    [28] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE_DJINN_LESSON)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Vault */
    [29] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_BANDIT), ENEMY_ROW(ENEMY_THIEF), ENEMY_ROW(ENEMY_THIEF)},
        .minCnts = {1, 1, 1},
        .maxCnts = {1, 1, 1},
    },
    /* Goma Cave */
    [31] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST), ENEMY_ROW(ENEMY_VERMIN_WORLD_MAP), ENEMY_ROW(ENEMY_WILD_MUSHROOM_WORLD_MAP)},
        .minCnts = {1},
        .maxCnts = {1, 2, 2},
    },
    /* Goma Cave */
    [32] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE), ENEMY_ROW(ENEMY_BAT_WORLD_MAP), ENEMY_ROW(ENEMY_SLIME_WORLD_MAP)},
        .minCnts = {1},
        .maxCnts = {2, 2, 2},
    },
    /* Goma Cave */
    [33] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Goma Cave */
    [34] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE), ENEMY_ROW(ENEMY_GHOST)},
        .minCnts = {0, 1},
        .maxCnts = {2, 2},
    },
    /* Goma Cave */
    [35] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Goma Cave */
    [36] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILL_HEAD)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* Goma Cave */
    [37] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON), ENEMY_ROW(ENEMY_GHOST)},
        .minCnts = {1, 1},
        .maxCnts = {1, 2},
    },
    /* in no encounter zone */
    [46] = {
        .memberIds = {ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* in no encounter zone */
    [47] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* in no encounter zone */
    [48] = {
        .memberIds = {ENEMY_ROW(ENEMY_WORM), ENEMY_ROW(ENEMY_VERMIN), ENEMY_ROW(ENEMY_BAT)},
        .minCnts = {1},
        .maxCnts = {1, 1, 1},
    },
    /* World Map, encounter region 2; Kolima Tunnel */
    [50] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* World Map, encounter region 2; Kolima Tunnel */
    [51] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE), ENEMY_ROW(ENEMY_SKELETON)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* World Map, encounter region 2; Kolima Tunnel */
    [52] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST), ENEMY_ROW(ENEMY_BAT_WORLD_MAP), ENEMY_ROW(ENEMY_ZOMBIE)},
        .minCnts = {1},
        .maxCnts = {1, 2, 1},
    },
    /* World Map, encounter region 2; Kolima Tunnel */
    [53] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON), ENEMY_ROW(ENEMY_BAT_WORLD_MAP), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minCnts = {0, 0, 1},
        .maxCnts = {1, 3, 2},
    },
    /* World Map, encounter region 2; Kolima Tunnel */
    [54] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* World Map, encounter region 2; Kolima Tunnel */
    [55] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_SOLDIER), ENEMY_ROW(ENEMY_RAT), ENEMY_ROW(ENEMY_BAT_WORLD_MAP)},
        .minCnts = {1},
        .maxCnts = {2, 2, 1},
    },
    /* World Map, encounter region 2; Kolima Tunnel */
    [56] = {
        .memberIds = {ENEMY_ROW(ENEMY_ZOMBIE), ENEMY_ROW(ENEMY_GHOST)},
        .minCnts = {1},
        .maxCnts = {3, 2},
    },
    /* World Map, encounter region 2; Kolima Tunnel */
    [57] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON), ENEMY_ROW(ENEMY_GHOST), ENEMY_ROW(ENEMY_WILL_HEAD)},
        .minCnts = {1},
        .maxCnts = {3, 2, 1},
    },
    /* Kolima Forest */
    [59] = {
        .memberIds = {ENEMY_ROW(ENEMY_DRONE_BEE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Kolima Forest */
    [60] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minCnts = {1, 1},
        .maxCnts = {2, 2},
    },
    /* Kolima Forest */
    [61] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILL_HEAD), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Kolima Forest */
    [62] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Kolima Forest */
    [63] = {
        .memberIds = {ENEMY_ROW(ENEMY_DRONE_BEE)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Kolima Forest */
    [64] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON), ENEMY_ROW(ENEMY_WILL_HEAD)},
        .minCnts = {1},
        .maxCnts = {3, 3},
    },
    /* Kolima Forest */
    [65] = {
        .memberIds = {ENEMY_ROW(ENEMY_DRONE_BEE), ENEMY_ROW(ENEMY_RAT_SOLDIER), ENEMY_ROW(ENEMY_RAT)},
        .minCnts = {1},
        .maxCnts = {2, 2, 2},
    },
    /* Kolima Forest */
    [66] = {
        .memberIds = {ENEMY_ROW(ENEMY_TROLL)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Tret Tree */
    [68] = {
        .memberIds = {ENEMY_ROW(ENEMY_SPIDER)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* Tret Tree */
    [69] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_SOLDIER), ENEMY_ROW(ENEMY_RAT), ENEMY_ROW(ENEMY_DRONE_BEE)},
        .minCnts = {0, 0, 1},
        .maxCnts = {2, 2, 2},
    },
    /* Tret Tree */
    [70] = {
        .memberIds = {ENEMY_ROW(ENEMY_SPIDER), ENEMY_ROW(ENEMY_DRONE_BEE)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Tret Tree */
    [71] = {
        .memberIds = {ENEMY_ROW(ENEMY_TROLL), ENEMY_ROW(ENEMY_RAT), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minCnts = {1, 1, 1},
        .maxCnts = {1, 1, 1},
    },
    /* Tret Tree */
    [72] = {
        .memberIds = {ENEMY_ROW(ENEMY_CREEPER)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Tret Tree */
    [73] = {
        .memberIds = {ENEMY_ROW(ENEMY_SPIDER), ENEMY_ROW(ENEMY_TROLL)},
        .minCnts = {1},
        .maxCnts = {3, 1},
    },
    /* Tret Tree */
    [74] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_CREEPER), ENEMY_ROW(ENEMY_CREEPER)},
        .minCnts = {1},
        .maxCnts = {2, 1, 1},
    },
    /* Tret Tree */
    [75] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_TROLL), ENEMY_ROW(ENEMY_TROLL)},
        .minCnts = {1},
        .maxCnts = {1, 1, 1},
    },
    /* Bilibin Cave */
    [77] = {
        .memberIds = {ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Bilibin Cave */
    [78] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOUL)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Bilibin Cave */
    [79] = {
        .memberIds = {ENEMY_ROW(ENEMY_TROLL), ENEMY_ROW(ENEMY_DRONE_BEE), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
        .minCnts = {1, 1, 1},
        .maxCnts = {1, 1, 1},
    },
    /* Bilibin Cave */
    [80] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOUL), ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Bilibin Cave */
    [81] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_TROLL)},
        .minCnts = {1, 1},
        .maxCnts = {2, 1},
    },
    /* Bilibin Cave */
    [82] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Bilibin Cave */
    [83] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOUL), ENEMY_ROW(ENEMY_GNOME)},
        .minCnts = {1},
        .maxCnts = {3, 2},
    },
    /* Bilibin Cave */
    [84] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOUL)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD */
    [86] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME)},
        .minCnts = {1},
        .maxCnts = {3},
    },
    /* World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD */
    [87] = {
        .memberIds = {ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {2},
        .maxCnts = {4},
    },
    /* World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD */
    [88] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAULER), ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {1, 1},
        .maxCnts = {1, 3},
    },
    /* World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD */
    [89] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAULER), ENEMY_ROW(ENEMY_GNOME)},
        .minCnts = {1, 1},
        .maxCnts = {1, 3},
    },
    /* World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD */
    [90] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAULER)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* Mercury Lighthouse */
    [92] = {
        .memberIds = {ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {3},
        .maxCnts = {5},
    },
    /* Mercury Lighthouse */
    [93] = {
        .memberIds = {ENEMY_ROW(ENEMY_CUTTLE), ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Mercury Lighthouse */
    [94] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Mercury Lighthouse */
    [95] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_MAN)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Mercury Lighthouse */
    [96] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAULER)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Mercury Lighthouse */
    [97] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_MAN), ENEMY_ROW(ENEMY_CUTTLE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Mercury Lighthouse */
    [98] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOUL)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Mercury Lighthouse */
    [99] = {
        .memberIds = {ENEMY_ROW(ENEMY_SIREN)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Mercury Lighthouse */
    [100] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_MAN), ENEMY_ROW(ENEMY_HARPY)},
        .minCnts = {1, 1},
        .maxCnts = {2, 1},
    },
    /* Mercury Lighthouse */
    [101] = {
        .memberIds = {ENEMY_ROW(ENEMY_CUTTLE)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Mercury Lighthouse */
    [102] = {
        .memberIds = {ENEMY_ROW(ENEMY_HARPY)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Mercury Lighthouse */
    [103] = {
        .memberIds = {ENEMY_ROW(ENEMY_SIREN), ENEMY_ROW(ENEMY_HARPY)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Mercury Lighthouse */
    [104] = {
        .memberIds = {ENEMY_ROW(ENEMY_SIREN)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Tret Tree */
    [106] = {
        .memberIds = {ENEMY_ROW(ENEMY_TRET)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Mercury Lighthouse */
    [107] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_MAN)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Mercury Lighthouse Aerie */
    [108] = {
        .memberIds = {ENEMY_ROW(ENEMY_SATUROS_MERCURY_LIGHTHOUSE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* World Map, encounter region 31; Fuchin Falls Cave */
    [120] = {
        .memberIds = {ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {3},
        .maxCnts = {4},
    },
    /* World Map, encounter region 31; Fuchin Falls Cave */
    [121] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAULER)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* World Map, encounter region 31; Fuchin Falls Cave */
    [122] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_MAN)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* World Map, encounter region 31; Fuchin Falls Cave */
    [123] = {
        .memberIds = {ENEMY_ROW(ENEMY_MOLE), ENEMY_ROW(ENEMY_OOZE)},
        .minCnts = {1, 1},
        .maxCnts = {2, 2},
    },
    /* Fuchin Falls Cave */
    [124] = {
        .memberIds = {ENEMY_ROW(ENEMY_SPIRIT)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* Fuchin Falls Cave */
    [125] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRGE), ENEMY_ROW(ENEMY_MOLE)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Fuchin Falls Cave */
    [126] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKELETON)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Fuchin Falls Cave */
    [127] = {
        .memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER), ENEMY_ROW(ENEMY_SPIRIT)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Mogall Forest */
    [129] = {
        .memberIds = {ENEMY_ROW(ENEMY_MOLE)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Mogall Forest */
    [130] = {
        .memberIds = {ENEMY_ROW(ENEMY_MOLE), ENEMY_ROW(ENEMY_DIRGE)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Mogall Forest */
    [131] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRGE), ENEMY_ROW(ENEMY_SPIRIT)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Mogall Forest */
    [132] = {
        .memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER), ENEMY_ROW(ENEMY_SPIRIT)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Mogall Forest */
    [133] = {
        .memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Mogall Forest */
    [134] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Mogall Forest */
    [135] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD), ENEMY_ROW(ENEMY_SPIRIT)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Mogall Forest */
    [136] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Mogall Forest */
    [138] = {
        .memberIds = {ENEMY_ROW(ENEMY_APE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Mogall Forest */
    [139] = {
        .memberIds = {ENEMY_ROW(ENEMY_KILLER_APE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* World Map, encounter region 3 */
    [141] = {
        .memberIds = {ENEMY_ROW(ENEMY_MOLE), ENEMY_ROW(ENEMY_DIRGE)},
        .minCnts = {1, 1},
        .maxCnts = {2, 2},
    },
    /* World Map, encounter region 3 */
    [142] = {
        .memberIds = {ENEMY_ROW(ENEMY_APE), ENEMY_ROW(ENEMY_MOLE)},
        .minCnts = {1, 1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 3 */
    [143] = {
        .memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER), ENEMY_ROW(ENEMY_SPIRIT)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* World Map, encounter region 3 */
    [144] = {
        .memberIds = {ENEMY_ROW(ENEMY_APE), ENEMY_ROW(ENEMY_DIRGE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 3 */
    [145] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD), ENEMY_ROW(ENEMY_SPIRIT)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* World Map, encounter region 3 */
    [146] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Altin Peak */
    [148] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRGE), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Altin Peak */
    [149] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_BEAST), ENEMY_ROW(ENEMY_APE)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* Altin Peak */
    [150] = {
        .memberIds = {ENEMY_ROW(ENEMY_TARANTULA), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Altin Peak */
    [151] = {
        .memberIds = {ENEMY_ROW(ENEMY_SLIME_BEAST), ENEMY_ROW(ENEMY_CALAMAR)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* Altin Peak */
    [152] = {
        .memberIds = {ENEMY_ROW(ENEMY_TARANTULA), ENEMY_ROW(ENEMY_SLIME_BEAST)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Altin Peak */
    [153] = {
        .memberIds = {ENEMY_ROW(ENEMY_CALAMAR)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Altin Peak */
    [154] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_FIGHTER)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Altin Peak */
    [155] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_FIGHTER), ENEMY_ROW(ENEMY_APE)},
        .minCnts = {1},
        .maxCnts = {3, 2},
    },
    /* Altin Peak */
    [157] = {
        .memberIds = {ENEMY_ROW(ENEMY_LIVING_STATUE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Altin Peak */
    [158] = {
        .memberIds = {ENEMY_ROW(ENEMY_HYDROS_STATUE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Lamakan Desert */
    [160] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* Lamakan Desert */
    [161] = {
        .memberIds = {ENEMY_ROW(ENEMY_GRUB), ENEMY_ROW(ENEMY_RAT_FIGHTER)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Lamakan Desert */
    [162] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC), ENEMY_ROW(ENEMY_RAT_FIGHTER)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Lamakan Desert */
    [163] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE), ENEMY_ROW(ENEMY_ORC)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Lamakan Desert */
    [164] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE), ENEMY_ROW(ENEMY_GRUB)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Lamakan Desert */
    [165] = {
        .memberIds = {ENEMY_ROW(ENEMY_SALAMANDER), ENEMY_ROW(ENEMY_GRUB)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Lamakan Desert */
    [166] = {
        .memberIds = {ENEMY_ROW(ENEMY_SALAMANDER), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Lamakan Desert */
    [167] = {
        .memberIds = {ENEMY_ROW(ENEMY_SALAMANDER)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Lamakan Desert */
    [169] = {
        .memberIds = {ENEMY_ROW(ENEMY_ANT_LION)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Lamakan Desert */
    [170] = {
        .memberIds = {ENEMY_ROW(ENEMY_MANTICORE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* World Map, encounter region 4 */
    [180] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* World Map, encounter region 4 */
    [181] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* World Map, encounter region 4 */
    [182] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* World Map, encounter region 4 */
    [183] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 4 */
    [184] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 4 */
    [185] = {
        .memberIds = {ENEMY_ROW(ENEMY_UNDEAD)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* World Map, encounter region 4 */
    [186] = {
        .memberIds = {ENEMY_ROW(ENEMY_UNDEAD), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 4 */
    [187] = {
        .memberIds = {ENEMY_ROW(ENEMY_UNDEAD), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Vale Cave */
    [189] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Vale Cave */
    [190] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Vale Cave */
    [191] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Vale Cave */
    [192] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAVAGER)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Vale Cave */
    [193] = {
        .memberIds = {ENEMY_ROW(ENEMY_UNDEAD), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Vale Cave */
    [194] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAVAGER), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Vale Cave */
    [195] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST_MAGE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Vale Cave */
    [196] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST_MAGE), ENEMY_ROW(ENEMY_UNDEAD)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Vault Cave */
    [201] = {
        .memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Vault Cave */
    [202] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAVAGER), ENEMY_ROW(ENEMY_DEATH_CAP)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Vault Cave */
    [203] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAVAGER), ENEMY_ROW(ENEMY_FIGHTER_BEE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Vault Cave */
    [204] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST_MAGE), ENEMY_ROW(ENEMY_UNDEAD)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Vault Cave */
    [205] = {
        .memberIds = {ENEMY_ROW(ENEMY_GHOST_MAGE)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Vault Cave */
    [206] = {
        .memberIds = {ENEMY_ROW(ENEMY_GARGOYLE)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Vault Cave */
    [207] = {
        .memberIds = {ENEMY_ROW(ENEMY_CAVE_TROLL), ENEMY_ROW(ENEMY_GARGOYLE)},
        .minCnts = {1, 1},
        .maxCnts = {1, 1},
    },
    /* Vault Cave */
    [208] = {
        .memberIds = {ENEMY_ROW(ENEMY_CAVE_TROLL)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* the Tolbi-bound ship */
    [210] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAN_O_WAR)},
        .minCnts = {3},
        .maxCnts = {3},
    },
    /* the Tolbi-bound ship */
    [211] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_FIGHTER), ENEMY_ROW(ENEMY_RABID_BAT)},
        .minCnts = {1, 2},
        .maxCnts = {1, 2},
    },
    /* the Tolbi-bound ship */
    [212] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_VIRAGO), ENEMY_ROW(ENEMY_MAN_O_WAR)},
        .minCnts = {1, 2},
        .maxCnts = {1, 2},
    },
    /* the Tolbi-bound ship */
    [213] = {
        .memberIds = {ENEMY_ROW(ENEMY_KRAKEN)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* World Map, encounter region 5 */
    [220] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_MOLE)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* World Map, encounter region 5 */
    [221] = {
        .memberIds = {ENEMY_ROW(ENEMY_ARMORED_RAT), ENEMY_ROW(ENEMY_RABID_BAT)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 5 */
    [222] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_MOLE), ENEMY_ROW(ENEMY_RABID_BAT)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 5 */
    [223] = {
        .memberIds = {ENEMY_ROW(ENEMY_ARMORED_RAT), ENEMY_ROW(ENEMY_FOUL_DIRGE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 5 */
    [224] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_MOLE), ENEMY_ROW(ENEMY_FOUL_DIRGE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 5 */
    [225] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* World Map, encounter region 5 */
    [226] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE), ENEMY_ROW(ENEMY_MAD_MOLE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 5 */
    [227] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE), ENEMY_ROW(ENEMY_FOUL_DIRGE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Altmiller Cave */
    [229] = {
        .memberIds = {ENEMY_ROW(ENEMY_ARMORED_RAT), ENEMY_ROW(ENEMY_FOUL_DIRGE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Altmiller Cave */
    [230] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAD_MOLE), ENEMY_ROW(ENEMY_WORM)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Altmiller Cave */
    [231] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE), ENEMY_ROW(ENEMY_WIGHT)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Altmiller Cave */
    [232] = {
        .memberIds = {ENEMY_ROW(ENEMY_GOLEM), ENEMY_ROW(ENEMY_WIGHT)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Altmiller Cave */
    [233] = {
        .memberIds = {ENEMY_ROW(ENEMY_CLAY_GARGOYLE), ENEMY_ROW(ENEMY_MAD_MOLE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Altmiller Cave */
    [234] = {
        .memberIds = {ENEMY_ROW(ENEMY_GOLEM), ENEMY_ROW(ENEMY_CLAY_GARGOYLE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Altmiller Cave */
    [235] = {
        .memberIds = {ENEMY_ROW(ENEMY_DREAD_HOUND)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* Altmiller Cave */
    [236] = {
        .memberIds = {ENEMY_ROW(ENEMY_DREAD_HOUND), ENEMY_ROW(ENEMY_GOLEM)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Colosso */
    [238] = {
        .memberIds = {ENEMY_ROW(ENEMY_AZART_IRON_SHIELD)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Colosso */
    [239] = {
        .memberIds = {ENEMY_ROW(ENEMY_AZART_PADDED_GLOVES)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Colosso */
    [240] = {
        .memberIds = {ENEMY_ROW(ENEMY_SATRAGE_CHAIN_MAIL)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Colosso */
    [241] = {
        .memberIds = {ENEMY_ROW(ENEMY_SATRAGE_COTTON_SHIRT)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Colosso */
    [242] = {
        .memberIds = {ENEMY_ROW(ENEMY_NAVAMPA_CLAYMORE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Colosso */
    [243] = {
        .memberIds = {ENEMY_ROW(ENEMY_NAVAMPA_HUNTERS_SWORD)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Gondowan Cave */
    [250] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* Gondowan Cave */
    [251] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_WARRIOR), ENEMY_ROW(ENEMY_WARRIOR_BEE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Gondowan Cave */
    [252] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRTY_APE)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* Gondowan Cave */
    [253] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD)},
        .minCnts = {1},
        .maxCnts = {2},
    },
    /* Gondowan Cave */
    [254] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRTY_APE), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Gondowan Cave */
    [255] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Gondowan Cave */
    [256] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRTY_APE), ENEMY_ROW(ENEMY_WARRIOR_BEE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Gondowan Cave */
    [257] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD)},
        .minCnts = {2},
        .maxCnts = {3},
    },
    /* Lunpa Fortress */
    [259] = {
        .memberIds = {ENEMY_ROW(ENEMY_RAT_WARRIOR), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* Lunpa Fortress */
    [260] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* Lunpa Fortress */
    [261] = {
        .memberIds = {ENEMY_ROW(ENEMY_VILE_DIRGE), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* Lunpa Fortress */
    [262] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Lunpa Fortress */
    [264] = {
        .memberIds = {ENEMY_ROW(ENEMY_BRIGAND)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Lunpa Fortress */
    [265] = {
        .memberIds = {ENEMY_ROW(ENEMY_BRIGAND)},
        .minCnts = {3},
        .maxCnts = {3},
    },
    /* Lunpa Fortress */
    [266] = {
        .memberIds = {ENEMY_ROW(ENEMY_TOADONPA)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* World Map, encounter region 6 */
    [268] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC_CAPTAIN), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minCnts = {1},
        .maxCnts = {1, 1},
    },
    /* World Map, encounter region 6 */
    [269] = {
        .memberIds = {ENEMY_ROW(ENEMY_WARRIOR_BEE), ENEMY_ROW(ENEMY_KOBOLD)},
        .minCnts = {1},
        .maxCnts = {1, 1},
    },
    /* World Map, encounter region 6 */
    [270] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRTY_APE), ENEMY_ROW(ENEMY_RAT_WARRIOR)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 6 */
    [271] = {
        .memberIds = {ENEMY_ROW(ENEMY_DIRTY_APE), ENEMY_ROW(ENEMY_WARRIOR_BEE)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* World Map, encounter region 6 */
    [272] = {
        .memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* World Map, encounter region 6 */
    [273] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC_CAPTAIN), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* World Map, encounter region 6 */
    [274] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC_CAPTAIN)},
        .minCnts = {1},
        .maxCnts = {3},
    },
    /* World Map, encounter region 6 */
    [275] = {
        .memberIds = {ENEMY_ROW(ENEMY_BRUTAL_TROLL)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Suhalla Desert */
    [277] = {
        .memberIds = {ENEMY_ROW(ENEMY_ACID_MAGGOT), ENEMY_ROW(ENEMY_KOBOLD)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Suhalla Desert */
    [278] = {
        .memberIds = {ENEMY_ROW(ENEMY_ROACH), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Suhalla Desert */
    [279] = {
        .memberIds = {ENEMY_ROW(ENEMY_BRUTAL_TROLL)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Suhalla Desert */
    [280] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAGICORE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Suhalla Desert */
    [281] = {
        .memberIds = {ENEMY_ROW(ENEMY_HARRIDAN), ENEMY_ROW(ENEMY_ORC_CAPTAIN)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Suhalla Desert */
    [282] = {
        .memberIds = {ENEMY_ROW(ENEMY_STONE_SOLDIER)},
        .minCnts = {1},
        .maxCnts = {3},
    },
    /* in no encounter zone */
    [283] = {
        .memberIds = {ENEMY_ROW(ENEMY_STONE_SOLDIER), ENEMY_ROW(ENEMY_ACID_MAGGOT)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Suhalla Desert */
    [284] = {
        .memberIds = {ENEMY_ROW(ENEMY_HARRIDAN), ENEMY_ROW(ENEMY_ROACH)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Suhalla Desert */
    [285] = {
        .memberIds = {ENEMY_ROW(ENEMY_MAGICORE), ENEMY_ROW(ENEMY_VILE_DIRGE)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* Suhalla Desert and the World Map */
    [287] = {
        .memberIds = {ENEMY_ROW(ENEMY_TORNADO_LIZARD)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Suhalla Desert */
    [288] = {
        .memberIds = {ENEMY_ROW(ENEMY_STORM_LIZARD)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Suhalla Desert and the World Map */
    [289] = {
        .memberIds = {ENEMY_ROW(ENEMY_TEMPEST_LIZARD)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Crossbone Isle */
    [291] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_HOBGOBLIN), ENEMY_ROW(ENEMY_VIRAGO)},
        .minCnts = {1, 2},
        .maxCnts = {1, 2},
    },
    /* Crossbone Isle */
    [292] = {
        .memberIds = {ENEMY_ROW(ENEMY_GRISLY), ENEMY_ROW(ENEMY_SUCCUBUS)},
        .minCnts = {2, 1},
        .maxCnts = {2, 1},
    },
    /* Crossbone Isle */
    [293] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_LICH), ENEMY_ROW(ENEMY_FIENDISH_GHOUL)},
        .minCnts = {1, 2},
        .maxCnts = {1, 2},
    },
    /* Crossbone Isle */
    [294] = {
        .memberIds = {ENEMY_ROW(ENEMY_GRYPHON)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Crossbone Isle */
    [296] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_LIZARD_KING), ENEMY_ROW(ENEMY_HARRIDAN), ENEMY_ROW(ENEMY_STONE_SOLDIER)},
        .minCnts = {1, 1, 2},
        .maxCnts = {1, 1, 2},
    },
    /* Crossbone Isle */
    [297] = {
        .memberIds = {ENEMY_ROW(ENEMY_CHIMERA)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Crossbone Isle */
    [298] = {
        .memberIds = {ENEMY_ROW(ENEMY_EARTH_LIZARD)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Crossbone Isle */
    [300] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_POISON_TOAD), ENEMY_ROW(ENEMY_THUNDER_LIZARD)},
        .minCnts = {1, 2},
        .maxCnts = {1, 2},
    },
    /* Crossbone Isle */
    [301] = {
        .memberIds = {ENEMY_ROW(ENEMY_CEREBUS)},
        .minCnts = {2},
        .maxCnts = {2},
    },
    /* Crossbone Isle */
    [302] = {
        .memberIds = {ENEMY_ROW(ENEMY_DEADBEARD)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Suhalla Gate; Gateway Cave */
    [310] = {
        .memberIds = {ENEMY_ROW(ENEMY_CANNIBAL_GHOUL), ENEMY_ROW(ENEMY_HORNED_GHOST)},
        .minCnts = {1},
        .maxCnts = {1, 1},
    },
    /* Suhalla Gate; Gateway Cave */
    [311] = {
        .memberIds = {ENEMY_ROW(ENEMY_HORNED_GHOST), ENEMY_ROW(ENEMY_GNOME_WIZARD)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Suhalla Gate; Gateway Cave */
    [312] = {
        .memberIds = {ENEMY_ROW(ENEMY_EARTH_GOLEM), ENEMY_ROW(ENEMY_GOBLIN)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Suhalla Gate; Gateway Cave */
    [313] = {
        .memberIds = {ENEMY_ROW(ENEMY_MOLE_MAGE), ENEMY_ROW(ENEMY_EARTH_GOLEM)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* Suhalla Gate; Gateway Cave */
    [314] = {
        .memberIds = {ENEMY_ROW(ENEMY_NIGHTMARE), ENEMY_ROW(ENEMY_HORNED_GHOST)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Suhalla Gate; Gateway Cave */
    [315] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_WIZARD), ENEMY_ROW(ENEMY_CANNIBAL_GHOUL)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Suhalla Gate; Gateway Cave */
    [316] = {
        .memberIds = {ENEMY_ROW(ENEMY_GOBLIN), ENEMY_ROW(ENEMY_MOLE_MAGE)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Suhalla Gate; Gateway Cave */
    [317] = {
        .memberIds = {ENEMY_ROW(ENEMY_NIGHTMARE), ENEMY_ROW(ENEMY_EARTH_GOLEM)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Venus Lighthouse */
    [319] = {
        .memberIds = {ENEMY_ROW(ENEMY_HORNED_GHOST), ENEMY_ROW(ENEMY_NIGHTMARE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Venus Lighthouse */
    [320] = {
        .memberIds = {ENEMY_ROW(ENEMY_EARTH_GOLEM), ENEMY_ROW(ENEMY_HORNED_GHOST)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Venus Lighthouse */
    [321] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_WIZARD), ENEMY_ROW(ENEMY_EARTH_GOLEM)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Venus Lighthouse */
    [322] = {
        .memberIds = {ENEMY_ROW(ENEMY_NIGHTMARE), ENEMY_ROW(ENEMY_GOBLIN)},
        .minCnts = {2},
        .maxCnts = {2, 1},
    },
    /* Venus Lighthouse */
    [323] = {
        .memberIds = {ENEMY_ROW(ENEMY_NIGHTMARE), ENEMY_ROW(ENEMY_EARTH_GOLEM), ENEMY_ROW(ENEMY_HORNED_GHOST)},
        .minCnts = {1, 1, 1},
        .maxCnts = {1, 1, 1},
    },
    /* Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins */
    [328] = {
        .memberIds = {ENEMY_ROW(ENEMY_GNOME_WIZARD), ENEMY_ROW(ENEMY_PLATED_RAT)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins */
    [329] = {
        .memberIds = {ENEMY_ROW(ENEMY_SKULL_WARRIOR), ENEMY_ROW(ENEMY_WILLOWISP)},
        .minCnts = {1},
        .maxCnts = {2, 2},
    },
    /* Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins */
    [330] = {
        .memberIds = {ENEMY_ROW(ENEMY_ORC_LORD), ENEMY_ROW(ENEMY_WILD_GRYPHON)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins */
    [331] = {
        .memberIds = {ENEMY_ROW(ENEMY_ICE_GARGOYLE), ENEMY_ROW(ENEMY_NIGHTMARE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins */
    [332] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILD_GRYPHON), ENEMY_ROW(ENEMY_MANTICORE_KING)},
        .minCnts = {1},
        .maxCnts = {1, 1},
    },
    /* Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins */
    [333] = {
        .memberIds = {ENEMY_ROW(ENEMY_MANTICORE_KING), ENEMY_ROW(ENEMY_WILLOWISP)},
        .minCnts = {1},
        .maxCnts = {1, 3},
    },
    /* Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins */
    [334] = {
        .memberIds = {ENEMY_ROW(ENEMY_CHIMERA_MAGE), ENEMY_ROW(ENEMY_ICE_GARGOYLE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Babi Lighthouse Entry and Babi Lighthouse; Tunnel Ruins */
    [335] = {
        .memberIds = {ENEMY_ROW(ENEMY_CHIMERA_MAGE), ENEMY_ROW(ENEMY_MANTICORE_KING)},
        .minCnts = {1},
        .maxCnts = {1, 1},
    },
    /* Venus Lighthouse */
    [337] = {
        .memberIds = {ENEMY_ROW(ENEMY_WILD_GRYPHON), ENEMY_ROW(ENEMY_GRAND_GOLEM)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* Venus Lighthouse */
    [338] = {
        .memberIds = {ENEMY_ROW(ENEMY_MANTICORE_KING), ENEMY_ROW(ENEMY_SKULL_WARRIOR)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Venus Lighthouse */
    [339] = {
        .memberIds = {ENEMY_ROW(ENEMY_CHIMERA_MAGE), ENEMY_ROW(ENEMY_FENRIR)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Venus Lighthouse */
    [340] = {
        .memberIds = {ENEMY_ROW(ENEMY_RECLUSE), ENEMY_ROW(ENEMY_WILLOWISP)},
        .minCnts = {1},
        .maxCnts = {2, 3},
    },
    /* Venus Lighthouse */
    [341] = {
        .memberIds = {ENEMY_ROW(ENEMY_BOULDER_BEAST), ENEMY_ROW(ENEMY_THUNDER_LIZARD)},
        .minCnts = {1},
        .maxCnts = {2, 1},
    },
    /* Venus Lighthouse */
    [342] = {
        .memberIds = {ENEMY_ROW(ENEMY_GRAND_GOLEM), ENEMY_ROW(ENEMY_BOULDER_BEAST)},
        .minCnts = {1},
        .maxCnts = {1, 4},
    },
    /* Venus Lighthouse */
    [343] = {
        .memberIds = {ENEMY_ROW(ENEMY_FENRIR), ENEMY_ROW(ENEMY_ICE_GARGOYLE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Venus Lighthouse */
    [344] = {
        .memberIds = {ENEMY_ROW(ENEMY_THUNDER_LIZARD), ENEMY_ROW(ENEMY_CHIMERA_MAGE)},
        .minCnts = {1},
        .maxCnts = {1, 2},
    },
    /* Venus Lighthouse Aerie */
    [346] = {
        .layout = FORMATION_LAYOUT_CENTERED,
        .memberIds = {ENEMY_ROW(ENEMY_MENARDI), ENEMY_ROW(ENEMY_SATUROS_VENUS_LIGHTHOUSE)},
        .minCnts = {1, 1},
        .maxCnts = {1, 1},
    },
    /* Venus Lighthouse Aerie */
    [FORMATION_FUSION_DRAGON] = {
        .memberIds = {ENEMY_ROW(ENEMY_FUSION_DRAGON)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* a mimic chest */
    [350] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_MERCURY_LIGHTHOUSE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* a mimic chest */
    [351] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_FUCHIN_FALLS_CAVE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* a mimic chest */
    [352] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_ALTIN_PEAK)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* a mimic chest */
    [353] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_VAULT_CAVE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* a mimic chest */
    [354] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_CROSSBONE_ISLE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* a mimic chest */
    [355] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_ALTMILLER_CAVE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* a mimic chest */
    [356] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_SUHALLA_DESERT)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* a mimic chest */
    [357] = {
        .memberIds = {ENEMY_ROW(ENEMY_MIMIC_PIRATE_SHIP)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Quartz */
    [360] = {
        .memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_QUARTZ)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Vine */
    [361] = {
        .memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_VINE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* Bane's row of Encounter_DjinnBattles; no event starts it */
    [362] = {
        .memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_UNUSED_BANE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Sleet */
    [365] = {
        .memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_SLEET)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Spritz */
    [366] = {
        .memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_SPRITZ)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Hail */
    [367] = {
        .memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_HAIL)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Dew */
    [368] = {
        .memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_DEW)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Forge */
    [371] = {
        .memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_FORGE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Corona */
    [372] = {
        .memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_CORONA)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Flash */
    [373] = {
        .memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_FLASH)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Breeze */
    [376] = {
        .memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_BREEZE)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Zephyr */
    [377] = {
        .memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_ZEPHYR)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Squall */
    [378] = {
        .memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_SQUALL)},
        .minCnts = {1},
        .maxCnts = {1},
    },
    /* the battle with the Djinni Luff */
    [379] = {
        .memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_LUFF)},
        .minCnts = {1},
        .maxCnts = {1},
    },
};

/* The 20 enemies whose defeated flags a monster battle clears before it */
/* picks a formation (btFormationSelectLevelMatchedCandidate). */
u16 BattleFormation_ExcludedEnemies[] = {
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
/* clang-format on */
