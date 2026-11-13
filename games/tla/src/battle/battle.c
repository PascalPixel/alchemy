/// @file
/// The battle object, the battle bank's C as The Broken Seal's battle.c
/// begins: the debug routines, a battle's setup, link battles, the whole
/// battle, the battle sprites and objects, the units' movements, auto battle,
/// a turn's actions and the start of an action's staging. battle_internal.h
/// holds what the parts share; the parts under battle/ are included here in
/// the order their code has in the ROM. Camelot's own files are not
/// recoverable: the ROM shows one object, and these parts are ours, cut where
/// The Broken Seal's are. The bank's code after them is in objects of its
/// own, from presentation.c on: its Func_0811e3ac calls
/// BattleObject_Reset (placement.c) with the object alone, leaving the second
/// argument register as BattleObject_Get left it, a call this object's
/// prototype of the routine cannot compile.

#include "battle_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "battle/debug.c"
#include "battle/setup.c"
#include "battle/link.c"
#include "battle/run.c"
#include "battle/sprite_slots.c"
#include "battle/placement.c"
#include "battle/objects.c"
#include "battle/motion.c"
#include "battle/auto_battle.c"
#include "battle/turn.c"
#include "battle/staging.c"
