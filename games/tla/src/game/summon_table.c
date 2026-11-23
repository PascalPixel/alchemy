/// @file
/// The summons: the order the Summon command lists them in, and each one's
/// ability and the Djinn it needs.
// Part of game.c.

#include "game_internal.h"

/// The order the Summon command lists the summons in: by the number of Djinn
/// they need, fewest first.
const u8 gSummon_OrderList[SUMMON_COUNT] = {
    SUMMON_VENUS,    SUMMON_MERCURY, SUMMON_MARS,        SUMMON_JUPITER,
    SUMMON_RAMSES,   SUMMON_NEREID,  SUMMON_KIRIN,       SUMMON_ATALANTA,
    SUMMON_ZAGAN,    SUMMON_MEGAERA, SUMMON_CYBELE,      SUMMON_NEPTUNE,
    SUMMON_TIAMAT,   SUMMON_PROCNE,  SUMMON_FLORA,       SUMMON_MOLOCH,
    SUMMON_JUDGMENT, SUMMON_BOREAS,  SUMMON_METEOR,      SUMMON_THOR,
    SUMMON_ULYSSES,  SUMMON_HAURES,  SUMMON_ECLIPSE,     SUMMON_COATLICUE,
    SUMMON_DAEDALUS, SUMMON_AZUL,    SUMMON_CATASTROPHE, SUMMON_CHARON,
    SUMMON_IRIS,
};

// clang-format off
/// The 29 summons, by summon id, read through SummonDefinition_Get. Each
/// needs this many standby Djinn of each element.
const struct SummonDefinition gSummon_DefinitionTable[] = {
    [SUMMON_VENUS] = {
        .abilityId = ABILITY_VENUS,
        .djinnRequired = {[ELEMENT_VENUS] = 1},
    },
    [SUMMON_MERCURY] = {
        .abilityId = ABILITY_MERCURY,
        .djinnRequired = {[ELEMENT_MERCURY] = 1},
    },
    [SUMMON_MARS] = {
        .abilityId = ABILITY_MARS,
        .djinnRequired = {[ELEMENT_MARS] = 1},
    },
    [SUMMON_JUPITER] = {
        .abilityId = ABILITY_JUPITER,
        .djinnRequired = {[ELEMENT_JUPITER] = 1},
    },
    [SUMMON_RAMSES] = {
        .abilityId = ABILITY_RAMSES,
        .djinnRequired = {[ELEMENT_VENUS] = 2},
    },
    [SUMMON_NEREID] = {
        .abilityId = ABILITY_NEREID,
        .djinnRequired = {[ELEMENT_MERCURY] = 2},
    },
    [SUMMON_KIRIN] = {
        .abilityId = ABILITY_KIRIN,
        .djinnRequired = {[ELEMENT_MARS] = 2},
    },
    [SUMMON_ATALANTA] = {
        .abilityId = ABILITY_ATALANTA,
        .djinnRequired = {[ELEMENT_JUPITER] = 2},
    },
    [SUMMON_CYBELE] = {
        .abilityId = ABILITY_CYBELE,
        .djinnRequired = {[ELEMENT_VENUS] = 3},
    },
    [SUMMON_NEPTUNE] = {
        .abilityId = ABILITY_NEPTUNE,
        .djinnRequired = {[ELEMENT_MERCURY] = 3},
    },
    [SUMMON_TIAMAT] = {
        .abilityId = ABILITY_TIAMAT,
        .djinnRequired = {[ELEMENT_MARS] = 3},
    },
    [SUMMON_PROCNE] = {
        .abilityId = ABILITY_PROCNE,
        .djinnRequired = {[ELEMENT_JUPITER] = 3},
    },
    [SUMMON_JUDGMENT] = {
        .abilityId = ABILITY_JUDGMENT,
        .djinnRequired = {[ELEMENT_VENUS] = 4},
    },
    [SUMMON_BOREAS] = {
        .abilityId = ABILITY_BOREAS,
        .djinnRequired = {[ELEMENT_MERCURY] = 4},
    },
    [SUMMON_METEOR] = {
        .abilityId = ABILITY_METEOR,
        .djinnRequired = {[ELEMENT_MARS] = 4},
    },
    [SUMMON_THOR] = {
        .abilityId = ABILITY_THOR,
        .djinnRequired = {[ELEMENT_JUPITER] = 4},
    },
    [SUMMON_ZAGAN] = {
        .abilityId = ABILITY_ZAGAN,
        .djinnRequired = {[ELEMENT_VENUS] = 1, [ELEMENT_MARS] = 1},
    },
    [SUMMON_MEGAERA] = {
        .abilityId = ABILITY_MEGAERA,
        .djinnRequired = {[ELEMENT_MARS] = 1, [ELEMENT_JUPITER] = 1},
    },
    [SUMMON_FLORA] = {
        .abilityId = ABILITY_FLORA,
        .djinnRequired = {[ELEMENT_VENUS] = 1, [ELEMENT_JUPITER] = 2},
    },
    [SUMMON_MOLOCH] = {
        .abilityId = ABILITY_MOLOCH,
        .djinnRequired = {[ELEMENT_MERCURY] = 2, [ELEMENT_JUPITER] = 1},
    },
    [SUMMON_ULYSSES] = {
        .abilityId = ABILITY_ULYSSES,
        .djinnRequired = {[ELEMENT_MERCURY] = 2, [ELEMENT_MARS] = 2},
    },
    [SUMMON_HAURES] = {
        .abilityId = ABILITY_HAURES,
        .djinnRequired = {[ELEMENT_VENUS] = 3, [ELEMENT_MARS] = 2},
    },
    [SUMMON_ECLIPSE] = {
        .abilityId = ABILITY_ECLIPSE,
        .djinnRequired = {[ELEMENT_MERCURY] = 2, [ELEMENT_JUPITER] = 3},
    },
    [SUMMON_COATLICUE] = {
        .abilityId = ABILITY_COATLICUE,
        .djinnRequired = {[ELEMENT_MERCURY] = 3, [ELEMENT_JUPITER] = 3},
    },
    [SUMMON_DAEDALUS] = {
        .abilityId = ABILITY_DAEDALUS,
        .djinnRequired = {[ELEMENT_VENUS] = 3, [ELEMENT_MARS] = 4},
    },
    [SUMMON_AZUL] = {
        .abilityId = ABILITY_AZUL,
        .djinnRequired = {[ELEMENT_VENUS] = 3, [ELEMENT_MERCURY] = 4},
    },
    [SUMMON_CATASTROPHE] = {
        .abilityId = ABILITY_CATASTROPHE,
        .djinnRequired = {[ELEMENT_MARS] = 3, [ELEMENT_JUPITER] = 5},
    },
    [SUMMON_CHARON] = {
        .abilityId = ABILITY_CHARON,
        .djinnRequired = {[ELEMENT_VENUS] = 8, [ELEMENT_JUPITER] = 2},
    },
    [SUMMON_IRIS] = {
        .abilityId = ABILITY_IRIS,
        .djinnRequired = {[ELEMENT_MERCURY] = 4, [ELEMENT_MARS] = 9},
    },
};
// clang-format on
