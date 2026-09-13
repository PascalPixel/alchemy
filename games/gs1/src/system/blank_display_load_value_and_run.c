#include "types.h"

/* runtime/blank_display_load_value_and_run.c */
s32 Audio_PlayCue(s32);
s32 Sys_Check(void);

s32 Runtime_BlankDisplayLoadValueAndRun(void)
{
  s32 *p;
  u8 *src;
  if (1)
  {
    *((s16 *) 0x04000000) = 0x40;
    src = (u8 *)((void *) 0x02000240);
    p = (s32 *)ADDR_03001CB4;
    *p = *((s32 *)(src + 4));
  }
  Audio_PlayCue(9);
  Sys_Check();
  return 0;
}

/* graphics/color/scale_rgb555.c */
s32 Graphics_ScaleRgb555(
    u16 *source,
    u16 *destination,
    s32 scale,
    s32 count)
{
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
