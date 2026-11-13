/// @file
/// The game object: the rules of the party and its world. Units and their
/// stats, a new game, items and inventories, Psynergy and levels, the active
/// party and its counters, element levels and classes, battle arithmetic, the
/// Djinn, and the inventories put aside and given back.
///
/// game_internal.h holds what every part shares (its includes, the
/// declarations the parts disagreed on and one inventory helper); the code is
/// in the files under game/, included here in the order it has in the ROM.
/// Camelot's own files are not recoverable: the ROM shows one object, and these
/// parts are ours, cut where The Broken Seal's are. The game flags, which The
/// Broken Seal keeps here, are in the system bank. Its tables are the other
/// objects in this folder. Where to start:
///
///   Units           units.c: Unit_Calculate, Unit_SetHp, Unit_AddPp
///   Items           inventory.c: Inventory_AddItem, Inventory_Equip,
///                   PartyInventory_Add
///   Levels          levels.c: Unit_LearnPsynergy, Unit_LevelUp
///   Enemies, party  party.c: Party_AddMember, Party_AdjustCoins
///   Classes         classes.c: Class_Determine, Unit_UpdateClass
///   Battle maths    battle_arithmetic.c: Battle_GetElementMultiplier,
///                   Battle_Random
///   Djinn           djinn.c: Djinn_Give, Djinn_Set

#include "game_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "game/units.c"
#include "game/inventory.c"
#include "game/levels.c"
#include "game/party.c"
#include "game/classes.c"
#include "game/battle_arithmetic.c"
#include "game/djinn.c"
#include "game/snapshot.c"
