// Draft, differ 61 of 44: the work copy of the PCM key-to-frequency
// interpolation, as ☀️'s Pcm_KeyToFrequency (games/common/src/sound/sound.c);
// its tables' pool words are zero in the copy. A sound-library function: the
// draft compiles with the game profile, where the build routes the library
// through old_agbcc. Other: register choice (r8 for the wave, where the
// listing has ip), and the game compiler indexes the tables with ldrb/ldr
// [rN, rM] where the listing adds first.
#include "types.h"

u32 Audio_UmulHigh32(u32 left, u32 right);
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

  return Audio_UmulHigh32(
      *(s32 *)((u8 *)wave + 4),
      lowerFrequency +
          Audio_UmulHigh32(upperFrequency - lowerFrequency, pitchScale));
}
