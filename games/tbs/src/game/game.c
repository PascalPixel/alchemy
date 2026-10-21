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
/// next begins. Its tables are the .tsv files in this folder, built by the .s
/// files beside them; README.md gives the words and prefixes. Where to start:
///
///   Units           units.c: UnitGet, UnitCalc, GameState_InitDefaults
///   Items           inventory.c: Inventory_AddItem, Inventory_Equip,
///                   PartyInventory_Add
///   Levels          levels.c: Unit_RefreshPsynergy, Unit_LevelUp
///   Flags           flags.c: GameFlag_Test, GameFlag_SetBit
///   Enemies, party  party.c: BattleUnitSet, Party_AddActiveOwner,
///                   Party_AdjustCoins
///   Classes         classes.c: Class_Determine, Unit_UpdateClass
///   Battle maths    battle_calc.c: Battle_GetElementMultiplier,
///                   BattleRandom
///   Djinn           djinn.c: Djinn_AddToOwner, Djinn_Activate

#include "game_internal.h"

// The game object's code, in ROM order. Each part is compiled here, as part
// of this one object, never on its own: the order of these includes is the
// order of the code in the ROM.
#include "game/units.c"
#include "game/inventory.c"
#include "game/levels.c"
#include "game/flags.c"
#include "game/party.c"
#include "game/classes.c"
#include "game/battle_calc.c"
#include "game/djinn.c"
#include "game/snapshot.c"
