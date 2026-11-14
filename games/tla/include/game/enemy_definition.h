#ifndef ALCHEMY_ENEMY_DEFINITION_H
#define ALCHEMY_ENEMY_DEFINITION_H

/// @file
/// Where an enemy's definition is found.

#include "edition.h"
#include "types.h"
#include "game/character.h"

const struct EnemyDefinition *Enemy_GetDefinition(s32 selector);

#endif
