/// @file
/// The battle object, the battle bank's C as The Broken Seal's battle.c is:
/// the debug routines, a battle's setup, link battles, the whole battle, the
/// battle sprites and objects, the units' movements, auto battle, a turn's
/// actions and the start of an action's staging, then a unit's motion
/// graphics, the strongest unit, applying an ability to its targets, playback,
/// the end of a turn, the screen transitions, the background, camera and
/// highlights, enemy formations and the spoils. battle_internal.h holds what
/// the parts share; the parts under battle/ are included here in the order
/// their code has in the ROM. Camelot's own files are not recoverable: the ROM
/// shows one object, and these parts are ours, cut where The Broken Seal's
/// are. BattleMotion_CallBeast (motion_graphics.c) calls BattleObject_Reset
/// (placement.c) with the object alone, leaving the second argument register
/// as BattleObject_Get left it, so it calls the routine through a view.

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

// The views the parts above keep end here; the parts below see the names as
// their headers declare them.
#undef EncounterWork
#undef Data_02018000
#undef GameFlag_GetByte
#undef BattleSprite_GetEffectHeight
#undef Unit_CalculateFar

#include "battle/motion_graphics.c"
#include "battle/strongest_unit.c"
#include "battle/targeting.c"
#include "battle/playback.c"
#include "battle/action.c"
#include "battle/turn_end.c"
#include "battle/transition.c"
#include "battle/background.c"
#include "battle/formation.c"
#include "battle/spoils.c"

#undef BattleObject_Reset
#undef Battle_GetUnitAtPlace
