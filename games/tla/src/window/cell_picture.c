/// @file
/// The window transition's scanline bounds, built from the UI tilemap by ARM
/// code that ScanlineWindow_FillSpans copies into RAM for each call.

#include "edition.h"
#include "types.h"
#include "system/io_reg.h"
#include "window/window.h"

/// Builds the hardware window's horizontal extent for each of the screen's 160
/// scanlines from the UI tilemap at tilemap (BG_SCREEN_WIDTH tiles a row,
/// DISPLAY_TILE_WIDTH shown), and returns the end of what it wrote.
u16 *Tile_BuildCellPictures(u16 *scanlineBounds, u16 *tilemap) {
  u32 tileRow;
  u32 tileColumn;
  s32 left;
  s32 right;
  s32 topLeft;
  s32 bottomLeft;
  s32 topRight;
  s32 bottomRight;
  s32 foundLeftEdge;
  s32 horizontalBounds;

  for (tileRow = 0; tileRow < DISPLAY_TILE_HEIGHT; tileRow++) {
    bottomLeft = 0;
    left = 0;
    topLeft = 0;
    bottomRight = 0;
    right = 0;
    topRight = 0;
    foundLeftEdge = 0;
    tileColumn = 0;
    scanlineBounds++;
    for (; tileColumn < DISPLAY_TILE_WIDTH; tileColumn++) {
      u16 tile = *tilemap++;

      if (tile == 0)
        continue;
      // The first border tile establishes the left edge and a one-tile span.
      // A later right border tile extends that span.
      if (foundLeftEdge == 0) {
        switch (tile) {
        case WINDOW_TILE_BORDER_TOP_LEFT:
          left = tileColumn * 8;
          right = left + 8;
          topLeft = left + 1;
          bottomLeft = left;
          topRight = left + 7;
          bottomRight = right;
          foundLeftEdge++;
          break;
        case WINDOW_TILE_BORDER_BOTTOM_LEFT:
          left = tileColumn * 8;
          right = left + 8;
          topLeft = left;
          bottomLeft = left + 1;
          topRight = right;
          bottomRight = left + 7;
          foundLeftEdge++;
          break;
        // The left edge meeting a divider counts as a side too.
        case WINDOW_TILE(7):
        case WINDOW_TILE_BORDER_LEFT:
        case WINDOW_TILE_BORDER_LEFT_TEE:
          left = tileColumn * 8;
          right = left + 8;
          bottomLeft = left;
          topLeft = left;
          bottomRight = right;
          topRight = right;
          foundLeftEdge++;
        }
      } else {
        switch (tile) {
        case WINDOW_TILE_BORDER_TOP_RIGHT:
          right = tileColumn * 8 + 8;
          topRight = tileColumn * 8 + 7;
          bottomRight = right;
          break;
        case WINDOW_TILE_BORDER_BOTTOM_RIGHT:
          right = tileColumn * 8 + 8;
          topRight = right;
          bottomRight = tileColumn * 8 + 7;
          break;
        case WINDOW_TILE(7):
        case WINDOW_TILE_BORDER_RIGHT:
        case WINDOW_TILE_BORDER_RIGHT_TEE:
          bottomRight = tileColumn * 8 + 8;
          right = bottomRight;
          topRight = bottomRight;
          break;
        }
      }
    }
    // The tile row's eight scanlines: its top line, six middle lines and its
    // bottom line.
    horizontalBounds = topRight + (topLeft << 8);
    *scanlineBounds++ = horizontalBounds;
    horizontalBounds = right + (left << 8);
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
    scanlineBounds++;
    tilemap += BG_SCREEN_WIDTH - DISPLAY_TILE_WIDTH;
    *scanlineBounds++ = horizontalBounds;
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
    horizontalBounds = bottomRight + (bottomLeft << 8);
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
  }
  return scanlineBounds;
}
