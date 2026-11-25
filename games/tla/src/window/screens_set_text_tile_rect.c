/// @file
/// Switching a rectangle of a window's text to another palette.

// The main image and the Clear screen's overlay both compile this file.

/// Like Window_SetTextTileBitRect, but ORs all of `palette` into the palette
/// number of each entry after clearing only the bit in which WINDOW_PALETTE
/// and HIGHLIGHT_PALETTE differ.
void Window_SetTextTileRect(const struct Window *window, s32 x, s32 y,
                            s32 width, s32 height, u32 palette) {
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;

  x += window->x + 1;
  y += window->y + 1;
  palette = BG_TILE_PALETTE(palette);
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > DISPLAY_TILE_WIDTH - 1) {
    width = DISPLAY_TILE_WIDTH - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  // BUG: tests the bottom against the screen's width, so a window reaching
  // below the screen draws into the map's unseen rows.
#ifdef BUGFIX
  if (y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
  if (y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
    height = DISPLAY_TILE_HEIGHT - y;
  }
  if (width > 0 && height > 0) {
    do {
      u16 *cell = &state->tilemap[y][x];
      s32 remaining = width;
      while (remaining != 0) {
        u32 entry = *cell;
        entry &= ~BG_TILE_PALETTE(WINDOW_PALETTE ^ HIGHLIGHT_PALETTE);
        entry |= palette;
        remaining--;
        *cell = entry;
        cell++;
      }
      height--;
      y++;
    } while (height != 0);
    state->dirty = 1;
  }
}
