/// @file
/// The game object: the rules of the party and its world. Units and their
/// stats, a new game, items and inventories, Psynergy and levels, the game
/// flags, enemy units and the party's counters, element levels and classes,
/// battle arithmetic and the Djinn.
///
/// game_internal.h holds what every part shares (its includes and one inline
/// helper); the code is in the files under game/, included here in the order
/// it has in the ROM. Camelot's own files are not recoverable: the ROM shows
/// one object, and these parts are ours, cut where one family ends and the
/// next begins. Its tables are the other .c files in this folder, each its own
/// object; README.md gives the words and prefixes. Where to start: units.c
/// (Unit_Get, Unit_Calculate, MainRoutineWork_InitializeDefaults) for units;
/// inventory.c (Inventory_AddItem, Inventory_Equip, PartyInventory_Add) for
/// items; levels.c (Unit_RefreshPsynergy, Unit_LevelUp) for levels; flags.c
/// (GameFlag_Get, GameFlag_Set) for flags; party.c (BattleUnit_Set,
/// Party_AddMember, Party_AdjustCoins) for enemies and the party; classes.c
/// (Class_Determine, Unit_UpdateClass) for classes; battle_arithmetic.c
/// (Battle_GetElementMultiplier, Battle_Random) for battle maths; djinn.c
/// (Djinn_Give, Djinn_Set) for the Djinn.

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
