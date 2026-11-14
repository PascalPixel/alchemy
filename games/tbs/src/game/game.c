/// @file
/// The game object: the rules of the party and its world. Units and their
/// stats, a new game, items and inventories, Psynergy and levels, the game
/// flags, enemy units and the party's counters, element levels and classes,
/// battle arithmetic and the Djinn.

#include "game_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "game_units.c"
#include "game_inventory.c"
#include "game_levels.c"
#include "game_flags.c"
#include "game_party.c"
#include "game_classes.c"
#include "game_battle_arithmetic.c"
#include "game_djinn.c"
#include "game_snapshot.c"
