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
/// Broken Seal keeps here, are in the system bank. Its tables, which The
/// Broken Seal links as objects of their own, are parts here too, after the
/// code. Where to start:
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

#undef Data_080b1290
#undef gCharacter_LevelExpTable
#undef gData_080b1f40
#undef gData_080b2340
#undef gCharacter_DefinitionTable
#undef gSummon_OrderList
#undef gElement_PowerResistByLevel
#undef gEnemy_ElementPresetTable

// The tables, in ROM order.
#include "game/experience_table.c"
#include "game/artifact_slots.c"
#include "game/item_table.c"
#include "game/ability_table.c"
#include "game/enemy_table.c"
#include "game/character_table.c"
#include "game/summon_table.c"
#include "game/class_table.c"
#include "game/element_table.c"
#include "game/djinn_table.c"
