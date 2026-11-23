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
///   Units           game_units.c: Unit_Calculate, Unit_SetHp, Unit_AddPp
///   Items           game_inventory.c: Inventory_AddItem, Inventory_Equip,
///                   PartyInventory_Add
///   Levels          game_levels.c: Unit_LearnPsynergy, Unit_LevelUp
///   Enemies, party  game_party.c: Party_AddMember, Party_AdjustCoins
///   Classes         game_classes.c: Class_Determine, Unit_UpdateClass
///   Battle maths    game_battle_arithmetic.c: Battle_GetElementMultiplier,
///                   Battle_Random
///   Djinn           game_djinn.c: Djinn_Give, Djinn_Set
///   Put aside       game_snapshot.c: InventorySnapshot_Save

#include "game_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "game_units.c"
#include "game_inventory.c"
#include "game_levels.c"
#include "game_party.c"
#include "game_classes.c"
#include "game_battle_arithmetic.c"
#include "game_djinn.c"
#include "game_snapshot.c"

// The tables, in ROM order.
#include "experience_table.c"
#include "artifact_slots.c"
#include "item_table.c"
#include "ability_table.c"
#include "enemy_table.c"
#include "character_table.c"
#include "summon_table.c"
#include "class_table.c"
#include "element_table.c"
#include "djinn_table.c"
