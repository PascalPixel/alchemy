// A field Psynergy's cast colours, and two script tables of the field effects.

#include "field/field_effect.h"
#include "graphics/palette_buffers.h"

// The cast colours: blue, green, red, cyan, magenta, yellow, grey and white.
const Sint32 FieldPsynergyCastColors[] = {
    PALETTE_TINT_CUSTOM | RGB(4, 4, 16),
    PALETTE_TINT_CUSTOM | RGB(4, 16, 4),
    PALETTE_TINT_CUSTOM | RGB(16, 4, 4),
    PALETTE_TINT_CUSTOM | RGB(4, 16, 16),
    PALETTE_TINT_CUSTOM | RGB(16, 4, 16),
    PALETTE_TINT_CUSTOM | RGB(16, 16, 4),
    PALETTE_TINT_CUSTOM | RGB(8, 8, 8),
    PALETTE_TINT_CUSTOM | RGB(16, 16, 16),
};

const Sint32        FieldEffectUntargetedActorScript[] = {SC_RELEASE};

// The resident copy of the scripts EFFECT_SCRIPT_MASK picks, by enum EFFECT_DURATION.
const Sint32 *const FieldEffectDurationScripts[]       = {
    [EFFECT_SCRIPT_SHORT]  = FieldEffectShortEffectScript,
    [EFFECT_SCRIPT_MEDIUM] = FieldEffectMediumEffectScript,
    [EFFECT_SCRIPT_LONG]   = FieldEffectLongEffectScript,
};
