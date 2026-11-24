/// @file
/// Where the party returns to after a defeat (Scene_SetRestartPoint).

#include "types.h"
#include "field/scene_tables.h"

/// Each row: the scene and entrance (-1 for any) it matches, then the scene
/// and entrance the party restarts at; scene -1 ends the table. A scene's
/// number cannot be a symbol in a halfword, so each row names its scenes in a
/// comment.
// clang-format off
const s16 gScene_RestartRules[] = {
    3, 15, 8, 16, // SCENE_VALE_PROLOGUE to SCENE_COTTAGE_ROOMS_B
    4, 16, 9, 2, // SCENE_VALE to SCENE_SANCTUM
    20, 1, 9, 12, // SCENE_VAULT to SCENE_SANCTUM
    20, 3, 9, 12, // SCENE_VAULT to SCENE_SANCTUM
    30, -1, 9, 35, // SCENE_BILIBIN to SCENE_SANCTUM
    36, -1, 36, 12, // SCENE_KOLIMA to SCENE_KOLIMA
    50, -1, 51, 7, // SCENE_IMIL to SCENE_ICE_ROOMS
    60, -1, 60, 5, // SCENE_FUCHIN_TEMPLE_GROUNDS to SCENE_FUCHIN_TEMPLE_GROUNDS
    72, -1, 9, 32, // SCENE_XIAN to SCENE_SANCTUM
    75, -1, 9, 29, // SCENE_ALTIN to SCENE_SANCTUM
    88, -1, 88, 5, // SCENE_LAMA_TEMPLE to SCENE_LAMA_TEMPLE
    23, -1, 9, 11, // SCENE_VAULT_REVISIT to SCENE_SANCTUM
    99, -1, 101, 2, // SCENE_KALAY to SCENE_KALAY_SANCTUM
    104, 3, 105, 11, // SCENE_LUNPA to SCENE_ROCK_ROOMS
    135, -1, 136, 30, // SCENE_TOLBI to SCENE_STUCCO_ROOMS_B
    167, -1, 168, 6, // SCENE_SUHALLA to SCENE_TIMBER_ROOMS_A
    178, -1, 179, 12, // SCENE_LALIVERO to SCENE_TIMBER_ROOMS_B
    190, -1, 190, 1, // SCENE_BATTLE_ARENA to SCENE_BATTLE_ARENA
    -1, -1, -1, -1, // end
};
