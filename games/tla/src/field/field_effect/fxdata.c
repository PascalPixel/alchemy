#include "objcmd.h"
#include "types.h"
#include "module/field/field.h"

//--------------------------------------------------------------------------
// Effect tables after the particle pictures.
//--------------------------------------------------------------------------

// Six words of zero. The functions that read them also write to them (wave
// phases and two flags), but they lie in the cartridge, so the writes are
// lost and each stays zero.
const s32 gData_080f393c = 0;
const s32 gData_080f3940 = 0;
const s32 gData_080f3944 = 0;
const s32 gData_080f3948 = 0;
const s32 gData_080f394c = 0;
const s32 gData_080f3950 = 0;

/// Eight colours (BGR555).
// As The Broken Seal's gBattleEffect_TargetColors are.
// clang-format off
const u32 gFieldEffect_TargetColors[8] = {
    0x4084, 0x1204, 0x1090, 0x2942, 0x304c, 0x098c, 0x2108, 0x4210,
};
// clang-format on

const s32 gBattleEffect_UntargetedObjectScript[] = {
    SC_FREE,
};

const s32 *const gFieldEffect_DurationScripts[3] = {
    &gFieldEffect_HeavyImpactScript[8],
    &gFieldEffect_HeavyImpactScript[11],
    &gFieldEffect_HeavyImpactScript[14],
};
