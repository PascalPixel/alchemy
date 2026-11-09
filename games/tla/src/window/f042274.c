#include "types.h"
#include "edition.h"
#include "ram_buffer.h"
#include "window.h"
#include "render_input.h"
#include "text_render.h"

u8 *String_FormatNumber(u8 *output, s32 value, s32 width);

/// Draws a four-digit number after a two-tile label as tiles in window work at
/// pixel (offsetX, offsetY): the HP label for variant 0, the party status
/// window's HP; the PP label for variant 1, its PP.
void Text_DrawPrefixedNumberAtOffset(s32 value, struct RenderInput *work,
                                     s32 offsetX, s32 offsetY, s32 variant) {
  u8 formatted[16];
  u16 output[8];
  u8 *text;
  s32 index;
  u32 cell;
  struct WindowWork *canvas;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  text = String_FormatNumber(formatted, value, 4);
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
  if (variant == 0) {
    output[0] = WINDOW_TILE_HP_LABEL;
    output[1] = WINDOW_TILE_HP_LABEL_END;
  } else {
    output[0] = WINDOW_TILE_PP_LABEL;
    output[1] = WINDOW_TILE_PP_LABEL_END;
  }
#else
  if (variant == 0) {
    output[0] = WINDOW_TILE_HP_LABEL;
  } else {
    output[0] = WINDOW_TILE_PP_LABEL;
  }
  // XXX: English, German and Japanese link this function from their listings:
  // their ROM loads the end tile from the literal pool 90 bytes away, which no
  // other constant in the game's C does; a C form that pools exactly that word
  // would settle it (recon/tla/en/main/08042274.c lists the forms tried).
  {
    // Held in a u16 local, the end tile is loaded; stored directly, it is
    // built.
    u16 second = WINDOW_TILE_HP_LABEL_END;

    output[1] = second;
  }
#endif
  for (index = 0; index <= 4; index++) {
    output[index + 2] = text[index];
  }
  output[6] = 0;
  cell = (((s16)work->y + ((u32)offsetY >> 3) + 1) << 5) +
         ((s16)work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 0x280) {
    vram = 0x06002000;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
  }
}
