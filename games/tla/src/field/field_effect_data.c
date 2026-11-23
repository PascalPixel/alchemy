/// @file
/// The ropes' phases and the field effects' tables.

#include "field/actor_script.h"
#include "types.h"
#include "field/field.h"
#include "system/io_reg.h"

//--------------------------------------------------------------------------
// Effect tables after the particle pictures.
//--------------------------------------------------------------------------

/// Six words of zero. The functions that read them also write to them (wave
/// phases and two flags), but they lie in the cartridge, so the writes are
/// lost and each stays zero.
const s32 gRope_LashPhase = 0;
const s32 gRope_UntiePhase = 0;
const s32 gRope_DebugSwinging = 0;
const s32 gTightrope_LashPhase = 0;
const s32 gTightrope_UntiePhase = 0;
const s32 gTightrope_DebugSwinging = 0;

/// Eight colours (BGR555).
// As The Broken Seal's gBattleEffect_TargetColors are.
// clang-format off
const u32 gFieldEffect_TargetColors[8] = {
    RGB(4, 4, 16),  RGB(4, 16, 4),  RGB(16, 4, 4), RGB(2, 10, 10),
    RGB(12, 2, 12), RGB(12, 12, 2), RGB(8, 8, 8),  RGB(16, 16, 16),
};
// clang-format on

const s32 gBattleEffect_UntargetedObjectScript[] = {
    SC_RELEASE,
};

/// The field effects' three scripts by how long an effect lasts: short, medium
/// and long.
const s32 *const gFieldEffect_DurationScripts[3] = {
    gFieldEffect_ShortEffectScript,
    gFieldEffect_MediumEffectScript,
    gFieldEffect_LongEffectScript,
};
