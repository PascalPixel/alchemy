/// @file
/// The map screen's markers (MapScreen_Show).

#include "battle/battle.h"
#include "flags.h"
#include "scene_ids.h"

/// A marker row: the marker's tile (1 or 2, the two kinds of area), the area
/// group whose FLAG_AREA_VISITED shows it, and the scene whose area name
/// (Area_GetNameIndex) the cursor shows near it.
#define AREA_MARKER(tile, group, scene)                                        \
  (tile) << 16 | FLAG_AREA_VISITED(group), (scene)

/// A row for each World Map actor from WORLD_MAP_FIRST_AREA_ACTOR, whose
/// marker is drawn where that actor stands, so a place's spot on the image
/// is its actor's on the World Map. A row of zeros is an actor with no
/// marker, and -1 ends the table.
// clang-format off
const s32 gMapScreen_AreaMarkers[] = {
    AREA_MARKER(1, 3, SCENE_VALE_PROLOGUE),
    AREA_MARKER(1, 5, SCENE_VAULT),
    AREA_MARKER(1, 25, SCENE_ALTIN),
    AREA_MARKER(1, 51, SCENE_SUHALLA),
    AREA_MARKER(1, 34, SCENE_LUNPA),
    AREA_MARKER(1, 36, SCENE_KALAY_DOCKS),
    AREA_MARKER(1, 38, SCENE_TOLBI_DOCKS),
    AREA_MARKER(1, 17, SCENE_IMIL),
    AREA_MARKER(1, 11, SCENE_KOLIMA),
    AREA_MARKER(1, 23, SCENE_XIAN),
    AREA_MARKER(1, 27, SCENE_LAMA_TEMPLE),
    AREA_MARKER(1, 20, SCENE_FUCHIN_TEMPLE_GROUNDS),
    AREA_MARKER(1, 8, SCENE_BILIBIN),
    AREA_MARKER(1, 32, SCENE_KALAY),
    AREA_MARKER(1, 55, SCENE_LALIVERO),
    AREA_MARKER(1, 40, SCENE_TOLBI),
    AREA_MARKER(2, 19, SCENE_MERCURY_LIGHTHOUSE_ENTRANCE),
    AREA_MARKER(2, 4, SCENE_SOL_SANCTUM_LUNA_SOL_ROOMS),
    AREA_MARKER(2, 57, SCENE_VENUS_LIGHTHOUSE_FIRST_FLOOR),
    AREA_MARKER(1, 10, SCENE_BILIBIN_BARRICADE),
    AREA_MARKER(2, 54, SCENE_BABI_LIGHTHOUSE_STONE_DOOR),
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    AREA_MARKER(1, 12, SCENE_KOLIMA_BRIDGE),
    AREA_MARKER(1, 44, SCENE_GONDOWAN_PASSAGE),
    -1, 0,
};
// clang-format on
