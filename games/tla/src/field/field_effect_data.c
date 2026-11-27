/* The ropes' phases, the tints a field Psynergy fades toward, and the scripts
   that hold and release a field effect. */

#include "field/actor_script.h"
#include "field/field.h"

/* --------------------------------------------------------------------------
   Effect tables after the particle pictures.
   -------------------------------------------------------------------------- */

/* Six words of zero. The functions that read them also write to them (wave
   phases and two flags), but they lie in the cartridge, so the writes are lost
   and each stays zero. */
const Sint32 Rope_LashPhase = 0;
const Sint32 Rope_UntiePhase = 0;
const Sint32 Rope_DebugSwinging = 0;
const Sint32 Tightrope_LashPhase = 0;
const Sint32 Tightrope_UntiePhase = 0;
const Sint32 Tightrope_DebugSwinging = 0;

/* The eight tints the screen fades toward while a field Psynergy is cast:
   picked by the Psynergy, by a FLAG_PSYNERGY_TINT flag, or at a summon tablet
   by the frame counter. */
/* clang-format off */
const Uint32 FieldPsynergy_CastColors[8] = {
	RGB(4, 4, 16),  RGB(4, 16, 4),  RGB(16, 4, 4), RGB(2, 10, 10),
	RGB(12, 2, 12), RGB(12, 12, 2), RGB(8, 8, 8),  RGB(16, 16, 16),
};
/* clang-format on */

/* A script that releases its actor at once: a Mind Read crescent's end, once it
   reaches the caster (fdMindReadUpdateCrescent). */
const Sint32 MindRead_CrescentEndScript[] = {
	SC_RELEASE,
};

/* The field effects' three scripts by how long an effect lasts: short, medium
   and long. */
const Sint32 *const FieldEffect_DurationScripts[3] = {
	FieldEffect_ShortEffectScript,
	FieldEffect_MediumEffectScript,
	FieldEffect_LongEffectScript,
};
