#ifndef ALCHEMY_ENEMY_IDS_H
#define ALCHEMY_ENEMY_IDS_H

/// @file
/// Enemy ids, as The Broken Seal's enemy_ids.h numbers them.

/// The first enemy id: enemy id `ENEMY_ID_FIRST + n` is the enemy in row n of
/// enemy_table.c (Enemy_GetDefinition); the ids below it are the party's
/// characters.
#define ENEMY_ID_FIRST 8

/// How many enemies enemy_table.c defines.
#define ENEMY_COUNT 380

#endif
