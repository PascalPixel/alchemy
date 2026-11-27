/* The game object: the rules of the party and its world. Units and their stats,
   a new game, items and inventories, Psynergy and levels, the active party and
   its counters, element levels and classes, battle arithmetic, the Djinn, and
   the inventories put aside and given back.

   game_internal.h holds what every part shares (its includes, the declarations
   the parts disagreed on and one inventory helper); the code is in the files
   under game/, included here in the order it has in the ROM. Camelot's own
   files are not recoverable: the ROM shows one object, and these parts are
   ours, cut where The Broken Seal's are. The game flags, which The Broken Seal
   keeps here, are in the system library. Its tables, which The Broken Seal
   links as objects of their own, are parts here too, after the code. Where to
   start:

	 Units           game_units.c: gmUnitCalc, gmUnitSetHp, gmUnitAddPp
	 Items           game_inventory.c: gmInventoryAddItem, gmInventoryEquip,
					 gmPartyInventoryAdd
	 Levels          game_levels.c: gmUnitLearnPsynergy, gmUnitLevelUp
	 Enemies, party  game_party.c: gmPartyAddMember, gmPartyAdjustCoins
	 Classes         game_classes.c: gmClassDetermine, gmUnitUpdateClass
	 Battle maths    game_battle_arithmetic.c: gmBattleGetElementMultiplier,
					 gmBattleRandom
	 Djinn           game_djinn.c: gmDjinnGive, gmDjinnSet
	 Put aside       game_snapshot.c: gmInventorySnapshotSave */

#include "game_internal.h"

/* The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
   parts"). */
#include "game_units.c"
#include "game_inventory.c"
#include "game_levels.c"
#include "game_party.c"
#include "game_classes.c"
#include "game_battle_arithmetic.c"
#include "game_djinn.c"
#include "game_snapshot.c"

/* The tables, in ROM order. */
#include "experience_table.c"
#include "rare_item_counters.c"
#include "item_table.c"
#include "ability_table.c"
#include "enemy_table.c"
#include "character_table.c"
#include "summon_table.c"
#include "class_table.c"
#include "element_table.c"
#include "djinn_table.c"
