// Measured 2026-11-05 (tla-en): differ 178 of 418.
// Near miss, differ 182 of 418 (tla-en, 2026-10-31; draft 418 instructions,
// no device): the native's shape, left with allocation. An unused solid
// renderer, the filled twin of Func_08196248 (same setup, transform, corner
// copies and back-face test), written from the listing (no caller in the
// image; twins12 BattleEffect_RunMemberBeam 0.34). Corners are sorted by y
// with XOR swaps and each half of the triangle is filled along 16.16 edge
// slopes from IwramRatioMulQ14; a pixel keeps the larger colour (an
// unsigned compare, bcs, from the u8 pixel against the u8 colour).
// The six corner pointers are formed inside the face loop, so loop.c hoists
// them past the loop's guard as the native places them; after the sort the
// corners are read from the first pointer at offsets, as the native reads
// them. Faces are indexed (faces[i]), so the face pointer is loop.c's copy
// at sp+8 and i keeps its own slot at sp+40, shared with the transform loop.
// The vertex count is scaled in place (the native keeps it in r6), the row
// offset is two shifts (multiplications fold into ((row >> 3) * 128 +
// (row & 7)) * 8) and the pixel index adds its shifted column first.
// The sort writes b's and c's y through bx[1] and cx[1], reading back the
// stored word through the same form, where the native stores through bx+4
// and cx+4; plain XOR_SWAP(*ay, *by) and XOR_SWAP(*by, *cy) score differ 343
// (draft 413) and give by a register in place of bx.
// Left: allocation. The native gives the first corner pointer r6 and edgeB
// r7; here they swap, because local-alloc gives the merged IwramMulQ16
// address r6 across the first product's call, where the native loads that
// address again before each call.
#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"

// The integer (pixel) half of a 16.16 value, read as the halfword it is.
#define HI(v) (((s16 *)&(v))[1])

// Swaps two integers without a temporary.
#define XOR_SWAP(a, b)                                                         \
  {                                                                            \
    (a) ^= (b);                                                                \
    (b) ^= (a);                                                                \
    (a) ^= (b);                                                                \
  }

// A point of the model, 16.16 fixed point.
struct SolidPoint {
  s32 x;
  s32 y;
  s32 z;
};

// A triangle of the model: where its three corners start in the vertex
// buffer (in words) and its colour.
struct SolidFace {
  s32 corner[3];
  u8 unk0c[12];
  u8 color;
  u8 unk19[3];
};

// Fills one scanline of the canvas (8x8 tiles, 16 to a row) from xa to xb,
// keeping the brighter of each pixel's colour and color.
#define FILL_SPAN(row, xa, xb)                                                 \
  {                                                                            \
    s32 left = (xa) >> 16;                                                     \
    s32 right = (xb) >> 16;                                                    \
    s32 rowOffset;                                                             \
    s32 x;                                                                     \
                                                                               \
    if (left > right)                                                          \
      XOR_SWAP(left, right);                                                   \
    rowOffset = (((row) & 7) << 3) + (((row) >> 3) << 10);                     \
    for (x = left; x != right; x++) {                                          \
      s32 pixel = ((x >> 3) << 6) + (x & 7) + rowOffset;                       \
                                                                               \
      if (canvas[pixel] < color)                                               \
        canvas[pixel] = color;                                                 \
    }                                                                          \
  }

// An unused solid renderer, the filled twin of Func_08196248. Transforms the
// vertexCount points in the sheet block by the current matrix, then fills
// every face of the effect work's list that faces the camera into the
// canvas, centred at (64, 64), keeping the brighter colour where faces
// overlap. Each triangle is sorted by y and filled in two halves, top to
// middle and middle to bottom, along 16.16 edge slopes.
void Func_0819574c(s32 vertexCount, s32 faceCount) {
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 *vertices = Ram_HeapSlots->unk64[0];
  struct SolidFace *faces = (struct SolidFace *)Ram_HeapSlots->battleFxWork[0];
  s32 i;
  s32 *ax, *bx, *cx, *cy, *ay, *by;
  s32 facing;
  s32 slopeA, slopeB, top, middle, bottom;
  u8 color;
  s32 edgeA, edgeB, row;

  // From here on, the count of words: three to a point.
  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  for (i = 0; i != faceCount; i++) {
    // The three corners of each face are copied after the vertices.
    ax = &vertices[vertexCount];
    bx = &vertices[vertexCount + 3];
    cx = &vertices[vertexCount + 6];
    cy = &vertices[vertexCount + 7];
    ay = &vertices[vertexCount + 1];
    by = &vertices[vertexCount + 4];
    Dma_Set(&vertices[faces[i].corner[0]], ax, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[1]], bx, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[2]], cx, 0x84000003, REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing =
        Iwram_MulQ16(*bx - *ax, *cy - *ay) - Iwram_MulQ16(*cx - *ax, *by - *ay);
    if (facing >= 0) {
      color = faces[i].color;
      *ax += 64 << 16;
      *ay += 64 << 16;
      *bx += 64 << 16;
      *by += 64 << 16;
      *cx += 64 << 16;
      *cy += 64 << 16;

      // Sort the corners by y. bx[1] is *by and cx[1] is *cy: the y swaps
      // store b's and c's y through their x pointers, as the native does.
      if (*ay > *by) {
        XOR_SWAP(*ax, *bx);
        *ay ^= *by;
        bx[1] = *by ^ *ay;
        *ay ^= bx[1];
      }
      if (*by > *cy) {
        XOR_SWAP(*bx, *cx);
        bx[1] = *by ^ *cy;
        cx[1] = *cy ^ bx[1];
        *by ^= cx[1];
        if (*ay > *by) {
          XOR_SWAP(*ax, *bx);
          *ay ^= *by;
          bx[1] = *by ^ *ay;
          *ay ^= bx[1];
        }
      }

      top = ax[1] >> 16;
      middle = ax[4] >> 16;
      bottom = HI(ax[7]);
      if (top != middle)
        slopeA = Iwram_RatioMulQ14(ax[4] - ax[1], ax[3] - ax[0]);
      if (top != bottom)
        slopeB = Iwram_RatioMulQ14(ax[7] - ax[1], ax[6] - ax[0]);

      // The upper half, between the short edge and the long one.
      edgeB = ax[0];
      edgeA = ax[0];
      for (row = top; row != middle; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }

      // The lower half, from the middle corner.
      if (middle != bottom)
        slopeA = Iwram_RatioMulQ14(ax[7] - ax[4], ax[6] - ax[3]);
      edgeA = ax[3];
      for (row = middle; row != bottom; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }
    }
  }
}
