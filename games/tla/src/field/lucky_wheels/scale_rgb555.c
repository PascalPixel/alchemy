#include "types.h"
#include "io_reg.h"
#include "main_routine_work.h"
#include "system.h"

void Sound_PlayFar(s32 cue);
void LuckyWheels_Run(void);

/// Blanks the display but for 1D objects, seeds the random state from the
/// play time, plays sound 709 and runs Lucky Wheels.
// As The Broken Seal's.
// XXX: sound 709's name; The Lost Age has no sounds.h yet, and The Broken
// Seal plays MUS_DJINN_LESSON here.
s32 LuckyWheels_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  gRandomState = gMainRoutineWork.playTime;
  Sound_PlayFar(709);
  LuckyWheels_Run();
  return 0;
}

s32 Graphics_ScaleRgb555(u16 *source, u16 *destination, s32 scale, s32 count) {
  s32 remaining;
  u32 red_mask;
  u32 green_mask;
  u32 blue_mask;
  u32 pixel;
  u32 red;
  u32 green;
  u32 blue;

  if (count > 0) {
    red_mask = 0x1f;
    green_mask = 0x3e0;
    blue_mask = 0x7c00;
    remaining = count;
    do {
      pixel = *source;
      red = pixel & red_mask;
      green = pixel & green_mask;
      blue = blue_mask & pixel;
      red *= scale;
      green *= scale;
      blue *= scale;
      pixel = ((red >> 16) & red_mask) | ((green >> 16) & green_mask);
      pixel |= (blue >> 16) & blue_mask;
      *destination = pixel;
      source++;
      destination++;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}
