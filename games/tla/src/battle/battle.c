/// @file
/// The battle object, the battle bank's C as The Broken Seal's battle.c
/// begins: the debug routines, a battle's setup, link battles, the whole
/// battle, the battle sprites and objects, the units' movements, auto battle
/// and a turn's actions. battle_internal.h holds what the parts share; the
/// parts under battle/ are included here in the order their code has in the
/// ROM. Camelot's own files are not recoverable: the ROM shows one object, and
/// these parts are ours, cut where The Broken Seal's are.

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
