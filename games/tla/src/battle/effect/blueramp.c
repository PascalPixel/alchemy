#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"

#define PIXEL_BUFFER Ram_MapCellBuffer

static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  /* FAKEMATCH: forwarding through the inline loads the routine's entry before
   * the arguments, as the native does at all three copies; called directly the
   * entry loads last (differ 10). */
  Iwram_CopyWords(destination, source, size);
}

/// Turns the background canvas into a blue ramp, row by row from the top, each
/// column starting after its own random delay.
void Graphics_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *p, *pixel;
  u16 *palette;
  u32 delayMask;

  palette = (u16 *)0x05000040;
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  delays = Ram_HeapSlots->unk64[0];
  end = 0;
  speed = 16;
  CopyWords_(PIXEL_BUFFER, (void *)0x06008000, 0x7800);
  delayMask = 63;
  p = delays;
  do {
    *p++ = Random16() & delayMask;
  } while (p != delays + 256);
  row = 0;
  i = 0;
  do {
    end += speed / 4;
    speed++;
    while (row != end) {
      x = 0;
      do {
        y = row - delays[x];
        if (y >= 0) {
          if (y <= 119) {
            pixel = PIXEL_BUFFER + ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) +
                                    ((y / 8) << 11));
            color = ((s16 *)0x05000000)[*pixel];
            red = color & 31;
            green = (color >> 5) & 31;
            blue = (color >> 10) & 31;
            if (red < green)
              red = green;
            if (red < blue)
              red = blue;
            *pixel = 63 - red;
          }
        }
        x++;
      } while (x != 256);
      row++;
    }
    CopyWords_((void *)0x06008000, PIXEL_BUFFER, 0x7800);
    wait(1);
    if (end > 248)
      break;
    i++;
  } while (i != 27);
  palette = (u16 *)0x050000c0;
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  {
    u8 *buffer = PIXEL_BUFFER;

    /* FAKEMATCH: the native loads the buffer's address before it starts the
     * 0x7800 count; without the barrier the count's two instructions come first
     * (differ 2). */
    __asm__ volatile("");
    i = 0;
    do {
      *buffer++ += 64;
      i++;
    } while (i != 0x7800);
  }
  CopyWords_((void *)0x06008000, PIXEL_BUFFER, 0x7800);
  wait(1);
}
