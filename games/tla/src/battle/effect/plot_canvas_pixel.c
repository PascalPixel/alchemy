#include "types.h"
#include "ram_buffer.h"

void BattleFx_PlotHalfCanvasPixel(s32 x, s32 y, s32 color) {
  u16 *map = Ram_HeapSlots->canvasMap;
  u8 *canvas;

  /* FAKEMATCH: the barrier keeps the map's load ahead of the row shift; without
   * it the shift comes first (one reordered pair, differ 2). */
  asm volatile("");
  canvas = Ram_HeapSlots->battleFxWork[1];
  canvas[map[(y << 6) + x]] = color;
}

void BattleFx_PlotCanvasPixel(s32 x, s32 y, s32 color) {
  u16 *map = Ram_HeapSlots->canvasMap;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];

  if (x > 63) {
    canvas += 0x1000;
    x -= 64;
  }
  canvas[map[(y << 6) + x]] = color;
}

// Draws a line of half canvas pixels from x0, y0 to x1, y1, Bresenham's
// way: one pixel per step along the longer axis.
void BattleFx_DrawHalfCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy;
  s32 sx;
  s32 sy;
  s32 err = 0;
  s32 x;
  s32 y;
  s32 i;

  if (dx < 0) {
    sx = -1;
    dx = -dx;
  } else {
    sx = 1;
  }
  dy = y1 - y0;
  if (dy < 0) {
    sy = -1;
    dy = -dy;
  } else {
    sy = 1;
  }
  x = x0;
  y = y0;
  if (dx > dy) {
    for (i = 0; i < dx; i++) {
      BattleFx_PlotHalfCanvasPixel(x, y, color);
      err += dy;
      if (err >= dx) {
        err -= dx;
        y += sy;
      }
      x += sx;
    }
  } else {
    for (i = 0; i < dy; i++) {
      BattleFx_PlotHalfCanvasPixel(x, y, color);
      err += dx;
      if (err >= dy) {
        err -= dy;
        x += sx;
      }
      y += sy;
    }
  }
}

// Draws a circle of the given radius round (cx, cy) on the half canvas, by
// the midpoint method: each step plots the eight symmetric points.
void BattleFx_DrawHalfCanvasCircle(s32 cx, s32 cy, s32 radius, s32 color) {
  s32 x = radius;
  s32 y = 0;

  while (x >= y) {
    BattleFx_PlotHalfCanvasPixel(cx + x, cy + y, color);
    BattleFx_PlotHalfCanvasPixel(cx + x, cy - y, color);
    BattleFx_PlotHalfCanvasPixel(cx - x, cy + y, color);
    BattleFx_PlotHalfCanvasPixel(cx - x, cy - y, color);
    BattleFx_PlotHalfCanvasPixel(cx + y, cy + x, color);
    BattleFx_PlotHalfCanvasPixel(cx + y, cy - x, color);
    BattleFx_PlotHalfCanvasPixel(cx - y, cy + x, color);
    BattleFx_PlotHalfCanvasPixel(cx - y, cy - x, color);
    radius = radius - y * 2 + 1;
    if (radius < 0) {
      s32 next = x - 1;

      radius += next * 2;
      x = next;
    }
    y++;
  }
}
