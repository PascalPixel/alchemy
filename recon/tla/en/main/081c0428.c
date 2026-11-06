// Near miss, differ 61 of 44 (tla-en, 2026-11-06): game route; through SOUND_C's
// old_agbcc -O2 -mthumb-interwork the same C emits all 44 instructions (rechecked
// 6 November 2026). Blocked, 5 November 2026: the work copy of the PCM key-to-frequency
// interpolation, as ☀️'s MidiKeyToFreq (games/tbs/src/sound/audio.c). Through
// the sound route (old_agbcc -O2 -mthumb-interwork, SOUND_C) this C emits the
// listing's 44 instructions, all of them; the game route differs by 61. What
// stops it is not the code: both pool words are 0 and both calls reach
// 0x081bf80c, in the zero fill before the sound bank. C reaches those only by
// a name at that address, which O2 forbids.
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
