/// @file
/// A field Psynergy's cast colours, and two script tables of the field
/// effects.

#include "field/actor_script.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "graphics/palette_buffers.h"
#include "system/io_reg.h"

/// The cast colours: blue, green, red, cyan, magenta, yellow, grey and white.
const s32 gFieldPsynergy_CastColors[] = {
    PALETTE_TINT_CUSTOM | RGB(4, 4, 16),  PALETTE_TINT_CUSTOM | RGB(4, 16, 4),
    PALETTE_TINT_CUSTOM | RGB(16, 4, 4),  PALETTE_TINT_CUSTOM | RGB(4, 16, 16),
    PALETTE_TINT_CUSTOM | RGB(16, 4, 16), PALETTE_TINT_CUSTOM | RGB(16, 16, 4),
    PALETTE_TINT_CUSTOM | RGB(8, 8, 8),   PALETTE_TINT_CUSTOM | RGB(16, 16, 16),
};

const s32 gFieldEffect_UntargetedActorScript[] = {SC_RELEASE};

/// The resident copy of the scripts EFFECT_SCRIPT_MASK picks, by enum
/// EffectDuration.
const s32 *const gFieldEffect_DurationScripts[] = {
    [EFFECT_SCRIPT_SHORT] = gFieldEffect_ShortEffectScript,
    [EFFECT_SCRIPT_MEDIUM] = gFieldEffect_MediumEffectScript,
    [EFFECT_SCRIPT_LONG] = gFieldEffect_LongEffectScript,
};
