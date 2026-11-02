#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"

// The integer (pixel) half of a 16.16 value, read as the halfword it is.
#define HI(v) (((s16 *)&(v))[1])

// A point of the wireframe model, 16.16 fixed point.
struct WirePoint {
  s32 x;
  s32 y;
  s32 z;
};

// A triangle of the wireframe model: where its three corners start in the
// vertex buffer (in words) and the colour its edges are drawn in.
struct WireFace {
  s32 corner[3];
  u8 unk0c[12];
  u8 color;
  u8 unk19[3];
};

void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, u8 color);

// An unused wireframe renderer. Transforms the vertexCount points in the
// sheet block by the current matrix, then draws the three edges of every
// face of the effect work's list that faces the camera (its corners run
// counter-clockwise on screen) as clipped lines into the canvas, centred at
// (64, 64).
void Func_08196248(s32 vertexCount, s32 faceCount) {
  s32 *vertices = Ram_HeapSlots->unk64[0];
  struct WireFace *faces = (struct WireFace *)Ram_HeapSlots->battleFxWork[0];
  s32 i;
  s32 facing;
  s32 color;

  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  // The three corners of each face are copied after the vertices.
  for (i = 0; i != faceCount; i++) {
    // Spelled through integers: the index comes first in the addition.
    s32 *x0 = (s32 *)(vertexCount * 4 + (s32)vertices);
    s32 *x1 = x0 + 3;
    s32 *x2 = x0 + 6;
    s32 *y2 = x0 + 7;
    s32 *y0 = x0 + 1;
    s32 *y1 = x0 + 4;

    Dma_Set(&vertices[faces[i].corner[0]], x0, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[1]], x1, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[2]], x2, 0x84000003, REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing =
        Iwram_MulQ16(*x1 - *x0, *y2 - *y0) - Iwram_MulQ16(*x2 - *x0, *y1 - *y0);
    if (facing >= 0) {
      color = faces[i].color;
      *x0 += 64 << 16;
      *y0 += 64 << 16;
      *x1 += 64 << 16;
      *y1 += 64 << 16;
      *x2 += 64 << 16;
      *y2 += 64 << 16;
      Func_08143eb4(HI(*x0), HI(*y0), HI(*x1), HI(*y1), color);
      Func_08143eb4(HI(*x1), HI(*y1), HI(*x2), HI(*y2), color);
      Func_08143eb4(HI(*x0), HI(*y0), HI(*x2), HI(*y2), color);
    }
  }
}
