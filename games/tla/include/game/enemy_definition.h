#ifndef ALCHEMY_ENEMY_DEFINITION_H
#define ALCHEMY_ENEMY_DEFINITION_H

/// @file
/// The lookup of an enemy's definition by its enemy id.

#include "edition.h"
#include "types.h"
#include "game/character.h"

/// Returns the row of enemy_table.c for `enemyId` (see ENEMY_ROW).
const struct EnemyDefinition *Enemy_GetDefinition(s32 enemyId);

#endif
