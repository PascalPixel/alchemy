// Draft, differ 215 of 203 (draft 170 instructions, no device): an unused
// wireframe renderer, written from the listing (no caller in the image; no
// Broken Seal twin, twins12 0.43).
// Left: the native holds every corner field it reads through its own
// address register, formed once before the face loop: the three corners'
// x at r6, r6 + 12 (r9) and r6 + 24 (r10), their y at r6 + 4 (r7), r6 + 16
// (r11) and r6 + 28 (sp+4); the halfword loads then index those with 2.
// Here only corners 1 and 2 get registers and the y fields are immediate
// offsets, so the face loop is 33 instructions shorter. The vertex loop's
// counter lives at sp+8 in the native (reused as the face counter), here
// in r9. Next to try: separate pointers for the six fields, a halfword
// union for the 16.16 values.
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
  struct WireFace *face = (struct WireFace *)Ram_HeapSlots->battleFxWork[0];
  s32 *x0;
  s32 *y0;
  s32 *x1;
  s32 *y1;
  s32 *x2;
  s32 *y2;
  s32 words;
  s32 facing;
  s32 color;
  s32 i;

  words = vertexCount * 3;
  for (i = 0; i != words; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  // The three corners of each face are copied after the vertices.
  x0 = &vertices[words];
  y0 = x0 + 1;
  x1 = x0 + 3;
  y1 = x0 + 4;
  x2 = x0 + 6;
  y2 = x0 + 7;
  for (i = 0; i != faceCount; i++) {
    Dma_Set(&vertices[face->corner[0]], x0, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[face->corner[1]], x1, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[face->corner[2]], x2, 0x84000003, REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing =
        Iwram_MulQ16(*x1 - *x0, *y2 - *y0) - Iwram_MulQ16(*x2 - *x0, *y1 - *y0);
    if (facing >= 0) {
      color = face->color;
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
    face++;
  }
}
