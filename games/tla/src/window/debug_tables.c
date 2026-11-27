// The debug menus' tables: the warp menu's presets and the abilities the shortcut grid offers.

#include "ability_ids.h"
#include "debug/debug.h"

extern const Uint8 DebugWarpJupiterName[];
extern const Uint8 DebugWarpArafura2Name[];
extern const Uint8 DebugWarpEnergySampleName[];
extern const Uint8 DebugWarpArafura3Name[];
extern const Uint8 DebugWarpChampa2Name[];
extern const Uint8 DebugWarpKaoruSpecial4Name[];
extern const Uint8 DebugWarpKaoruSpecial3Name[];
extern const Uint8 DebugWarpKaoruSpecial2Name[];
extern const Uint8 DebugWarpKaoruSpecial1Name[];

// A row's handler, cast to the row's type unless UBFIX gives the row the handler's own.
#if FIX_FUNCTION_SIGNATURES
#    define WARP(handler) handler
#else
#    define WARP(handler) (Sint32 (*)(void)) handler
#endif

// The debug warp menu's rows, "0:Jupiter" to "8:Kaoru Special1".
const DEBUG_WARP_PRESET DebugWarpPresets[] = {
    {DebugWarpJupiterName,       WARP(mnDebugWarpJupiter)      },
    {DebugWarpArafura2Name,      WARP(mnDebugWarpArafura2)     },
    {DebugWarpEnergySampleName,  WARP(mnDebugWarpEnergySample) },
    {DebugWarpArafura3Name,      WARP(mnDebugWarpArafura3)     },
    {DebugWarpChampa2Name,       WARP(mnDebugWarpChampa2)      },
    {DebugWarpKaoruSpecial4Name, WARP(mnDebugWarpKaoruSpecial4)},
    {DebugWarpKaoruSpecial3Name, WARP(mnDebugWarpKaoruSpecial3)},
    {DebugWarpKaoruSpecial2Name, WARP(mnDebugWarpKaoruSpecial2)},
    {DebugWarpKaoruSpecial1Name, WARP(mnDebugWarpKaoruSpecial1)},
};

#undef WARP

// The field Psynergy of the debug shortcut grid, a column of nine at a time.
const Sint16 DebugShortcutAbilities[4][9] = {
    {ABILITY_SAND,
     ABILITY_BLAZE,  ABILITY_POUND,
     ABILITY_ARROW,     ABILITY_BURST,
     ABILITY_CYCLONE, ABILITY_LASH,
     ABILITY_SCOOP,   ABILITY_PARCH    },
    {ABILITY_TELEPORT,
     ABILITY_TREMOR, ABILITY_HOVER,
     ABILITY_GRIND,     ABILITY_MAGNET,
     ABILITY_NOTHING, ABILITY_NOTHING,
     ABILITY_NOTHING, ABILITY_NOTHING  },
    {ABILITY_MOVE,
     ABILITY_LIFT,   ABILITY_CARRY,
     ABILITY_CATCH,     ABILITY_FORCE,
     ABILITY_NOTHING, ABILITY_RETREAT,
     ABILITY_REVEAL,  ABILITY_NOTHING  },
    {ABILITY_AVOID,
     ABILITY_CLOAK,  ABILITY_HALT,
     ABILITY_WHIRLWIND, ABILITY_GROWTH,
     ABILITY_DOUSE,   ABILITY_PLY,
     ABILITY_FROST,   ABILITY_MIND_READ},
};
