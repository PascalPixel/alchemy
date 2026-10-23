#include "unit/overworld/zonedata.h"
#include "types.h"

/// The world map's encounter zones (struct WorldMapEncounterRule): {encounter
/// region, battle environment (-1 for any), flag (-1, or a flag that turns the
/// rule off once set), zone}, ended by region -1. The first matching rule wins
/// (Encounter_ResolveWorldMapZoneAndBackdrop), and ground no rule matches
/// has no random battles.
///
/// Each encounter region is an area of the map, whose zones are made for
/// party levels from 3 (region 1) to 22 (region 6). Each region has two zones:
/// BATTLE_ENVIRONMENT_FOREST (BATTLE_ENVIRONMENT_SNOW_FOREST in
/// WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD) takes the second, with a rate of 9
/// against the first's 20 or 25. No rule uses a flag or any environment.
const s16 gWorldMap_EncounterRules[132] = {
    1,  4,  -1, 7,  1,  7,  -1, 7,  1,  1,  -1, 7,  1,  2,  -1, 8,  2,  4,  -1,
    13, 2,  7,  -1, 13, 2,  1,  -1, 13, 2,  2,  -1, 14, 21, 5,  -1, 29, 21, 6,
    -1, 30, 31, 1,  -1, 37, 31, 2,  -1, 38, 3,  3,  -1, 45, 3,  4,  -1, 45, 3,
    7,  -1, 45, 3,  1,  -1, 45, 3,  2,  -1, 46, 4,  3,  -1, 54, 4,  4,  -1, 54,
    4,  7,  -1, 54, 4,  1,  -1, 54, 4,  2,  -1, 55, 5,  3,  -1, 64, 5,  4,  -1,
    64, 5,  7,  -1, 64, 5,  1,  -1, 64, 5,  2,  -1, 65, 6,  3,  -1, 80, 6,  4,
    -1, 80, 6,  7,  -1, 80, 6,  1,  -1, 80, 6,  2,  -1, 81, -1, 0,  0,  0,
};

/// The encounter of each Djinni battle (Encounter_GetDjinnFormation): {zone,
/// slot}, by Djinni number (element * 20 + index). The 20 rows of an element
/// all use its zone (ENCOUNTER_ZONE_VENUS_DJINN to
/// ENCOUNTER_ZONE_JUPITER_DJINN), slot
/// index % 8, so Djinn 8 apart fight the same formation.
const u16 gEncounter_AreaEntryTable[160] = {
    100, 0, 100, 1, 100, 2, 100, 3, 100, 4, 100, 5, 100, 6, 100, 7, 100, 0,
    100, 1, 100, 2, 100, 3, 100, 4, 100, 5, 100, 6, 100, 7, 100, 0, 100, 1,
    100, 2, 100, 3, 101, 0, 101, 1, 101, 2, 101, 3, 101, 4, 101, 5, 101, 6,
    101, 7, 101, 0, 101, 1, 101, 2, 101, 3, 101, 4, 101, 5, 101, 6, 101, 7,
    101, 0, 101, 1, 101, 2, 101, 3, 102, 0, 102, 1, 102, 2, 102, 3, 102, 4,
    102, 5, 102, 6, 102, 7, 102, 0, 102, 1, 102, 2, 102, 3, 102, 4, 102, 5,
    102, 6, 102, 7, 102, 0, 102, 1, 102, 2, 102, 3, 103, 0, 103, 1, 103, 2,
    103, 3, 103, 4, 103, 5, 103, 6, 103, 7, 103, 0, 103, 1, 103, 2, 103, 3,
    103, 4, 103, 5, 103, 6, 103, 7, 103, 0, 103, 1, 103, 2, 103, 3,
};
