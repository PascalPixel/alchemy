/* The debug menus' tables: the warp menu's presets and the abilities the
   shortcut grid offers. */

#include "ability_ids.h"
#include "debug/debug.h"

extern const Uint8 DebugWarp_JupiterName[];
extern const Uint8 DebugWarp_Arafura2Name[];
extern const Uint8 DebugWarp_EnergySampleName[];
extern const Uint8 DebugWarp_Arafura3Name[];
extern const Uint8 DebugWarp_Champa2Name[];
extern const Uint8 DebugWarp_KaoruSpecial4Name[];
extern const Uint8 DebugWarp_KaoruSpecial3Name[];
extern const Uint8 DebugWarp_KaoruSpecial2Name[];
extern const Uint8 DebugWarp_KaoruSpecial1Name[];

/* A row's handler, cast to the row's type unless UBFIX gives the row the
   handler's own. */
#if FIX_FUNCTION_SIGNATURES
#define WARP(handler) handler
#else
#define WARP(handler) (Sint32 (*)(void)) handler
#endif

/* The debug warp menu's rows, "0:Jupiter" to "8:Kaoru Special1". */
const DEBUG_WARP_PRESET DebugWarp_Presets[] = {
	{DebugWarp_JupiterName, WARP(mnDebugWarpJupiter)},
	{DebugWarp_Arafura2Name, WARP(mnDebugWarpArafura2)},
	{DebugWarp_EnergySampleName, WARP(mnDebugWarpEnergySample)},
	{DebugWarp_Arafura3Name, WARP(mnDebugWarpArafura3)},
	{DebugWarp_Champa2Name, WARP(mnDebugWarpChampa2)},
	{DebugWarp_KaoruSpecial4Name, WARP(mnDebugWarpKaoruSpecial4)},
	{DebugWarp_KaoruSpecial3Name, WARP(mnDebugWarpKaoruSpecial3)},
	{DebugWarp_KaoruSpecial2Name, WARP(mnDebugWarpKaoruSpecial2)},
	{DebugWarp_KaoruSpecial1Name, WARP(mnDebugWarpKaoruSpecial1)},
};

#undef WARP

/* The field Psynergy of the debug shortcut grid, a column of nine at a time. */
const Sint16 DebugShortcut_Abilities[4][9] = {
	{ABILITY_SAND, ABILITY_BLAZE, ABILITY_POUND, ABILITY_ARROW, ABILITY_BURST,
		ABILITY_CYCLONE, ABILITY_LASH, ABILITY_SCOOP, ABILITY_PARCH},
	{ABILITY_TELEPORT, ABILITY_TREMOR, ABILITY_HOVER, ABILITY_GRIND,
		ABILITY_MAGNET, ABILITY_NOTHING, ABILITY_NOTHING, ABILITY_NOTHING,
		ABILITY_NOTHING},
	{ABILITY_MOVE, ABILITY_LIFT, ABILITY_CARRY, ABILITY_CATCH, ABILITY_FORCE,
		ABILITY_NOTHING, ABILITY_RETREAT, ABILITY_REVEAL, ABILITY_NOTHING},
	{ABILITY_AVOID, ABILITY_CLOAK, ABILITY_HALT, ABILITY_WHIRLWIND,
		ABILITY_GROWTH, ABILITY_DOUSE, ABILITY_PLY, ABILITY_FROST,
		ABILITY_MIND_READ},
};
