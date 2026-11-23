/// @file
/// The fade of the Lucky Dice and Lucky Wheels screens, which darken and
/// brighten by scaling their palettes; each game's object includes it.

/// Copies count RGB555 colours from source to destination with each channel
/// multiplied by scale (16.16: 0 is black, FIXED_ONE unchanged), for fades.
static s32 Palette_ScaleRgb555(u16 *source, u16 *destination, FIXED scale,
                               s32 count) {
  s32 remaining;
  u32 color;
  u32 red;
  u32 green;
  u32 blue;

  if (count > 0) {
    u32 redMask = RGB(31, 0, 0);
    u32 greenMask = RGB(0, 31, 0);
    u32 blueMask = RGB(0, 0, 31);

    remaining = count;
    do {
      color = *source;
      red = color & redMask;
      green = color & greenMask;
      blue = blueMask & color;
      red *= scale;
      green *= scale;
      blue *= scale;
      color = (FIXEDtoINT(red) & redMask) | (FIXEDtoINT(green) & greenMask);
      color |= FIXEDtoINT(blue) & blueMask;
      *destination = color;
      source++;
      destination++;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}
