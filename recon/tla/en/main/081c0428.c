// Differ 0 of 44 through its own route, SOUND_C (tla-en, 2026-11-07); `alchemy-devtools diff`
// measures the game route, which gives 61: no interworking return, ldrb [rX, rY] for add-then-load.
// Tried 7 November: wave read before the inner call (64), local table pointers and *(p + n) (61), s32 key (63),
// s32 *wave (61); a 7-minute permute found nothing under 61. The sound route still emits all 44 (see below).
// Through SOUND_C's old_agbcc -O2 -mthumb-interwork the same C emits all 44 instructions (rechecked
// 6 November 2026). Blocked, 5 November 2026: the work copy of the PCM key-to-frequency
// interpolation, as ☀️'s MidiKeyToFreq (games/tbs/src/sound/audio.c). Through
// the sound route (old_agbcc -O2 -mthumb-interwork, SOUND_C) this C emits the
// listing's 44 instructions, all of them; the game route differs by 61. What
// stops it is not the code: both pool words are 0 and both calls reach
// 0x081bf80c, in the zero fill before the sound bank. C reaches those only by
// a name at that address, which O2 forbids.
// ☀️'s route for code that runs from a RAM copy does not reach it (checked 6
// November 2026): ☀️ links such code into an output section run in IWRAM and
// stored in ROM (`> IWRAM AT>ROM` in games/tbs/main.ld, its load address from
// LOADADDR), and the linker resolves every relocation, so its calls and pool
// words hold real addresses. These bytes are a copy of an older sound library
// kept in ROM, as player_work_copy.c and the leftover driver objects beside it
// (games/tla/src/sound): pool words 0 and calls into the zero fill are what an
// object whose relocations were never applied, or applied for another layout,
// holds. No load or run address from the object list's layout gives a word of
// 0 for gSound_PcmPitchCodes; only a name at 0 would, an equate O2 forbids. The
// leftover assembly reaches the same targets as `Symbol + offset` (S1 lets it,
// for r11 and the like), which C cannot write. It stays a draft.
#include "types.h"

u32 umul3232H32(u32 left, u32 right);
extern u8 gSound_PcmPitchCodes[];
extern u32 gSound_PcmFrequencySteps[];

s32 Pcm_KeyToFrequencyWorkCopy(void *wave, u8 key, u8 pitch) {
  u32 keyIndex;
  u32 pitchScale;
  u32 lowerFrequency;
  u32 upperFrequency;

  keyIndex = key;
  pitchScale = (u32)pitch << 24;
  if (keyIndex > 178) {
    keyIndex = 178;
    pitchScale = 255U << 24;
  }

  lowerFrequency = gSound_PcmPitchCodes[keyIndex];
  lowerFrequency =
      gSound_PcmFrequencySteps[lowerFrequency & 15] >> (lowerFrequency >> 4);
  upperFrequency = gSound_PcmPitchCodes[keyIndex + 1];
  upperFrequency =
      gSound_PcmFrequencySteps[upperFrequency & 15] >> (upperFrequency >> 4);

  return umul3232H32(
      *(s32 *)((u8 *)wave + 4),
      lowerFrequency +
          umul3232H32(upperFrequency - lowerFrequency, pitchScale));
}
