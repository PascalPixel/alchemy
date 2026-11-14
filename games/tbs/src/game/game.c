/// @file
/// The game object: the rules of the party and its world. Units and their
/// stats, a new game, items and inventories, Psynergy and levels, the game
/// flags, enemy units and the party's counters, element levels and classes,
/// battle arithmetic and the Djinn.

#include "game_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "game/units.c"
#include "game/inventory.c"
#include "game/levels.c"
#include "game/flags.c"
#include "game/party.c"
#include "game/classes.c"
#include "game/battle_arithmetic.c"
#include "game/djinn.c"
#include "game/snapshot.c"
