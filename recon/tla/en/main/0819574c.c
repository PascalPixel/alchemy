/* Draft, differ 472 of 418 (draft 382 instructions, no device): an unused
 * solid renderer, the filled twin of Func_08196248 (same setup, transform,
 * corner copies and back-face test), written from the listing (no caller in
 * the image; twins12 BattleFx_RunMemberBeam 0.34). Corners are sorted by y
 * with XOR swaps and each half of the triangle is filled along 16.16 edge
 * slopes from IwramRatioMulQ14; a pixel keeps the larger colour (an
 * unsigned compare, bcs, from the u8 pixel against the u8 colour).
 * Left: as in Func_08196248, the native holds each corner field it touches
 * through its own address register or spill (a.y at r11, b.x at r10, b.y
 * at sp+28, c.x at sp+36, c.y at sp+32), and it reloads after every store of
 * a swap (the corners may alias); with three corner pointers (a, b, c) the
 * swaps reload as the native's do, but the addresses stay offsets, so the
 * face loop is still 36 instructions short.
 * Tried: corner[k] fields directly (499), six pointers to the six fields
 * (538). */
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
    rowOffset = ((row) & 7) * 8 + ((row) >> 3) * 1024;                         \
    for (x = left; x != right; x++) {                                          \
      s32 pixel = (x & 7) + (x >> 3) * 64 + rowOffset;                         \
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
  struct SolidFace *face = (struct SolidFace *)Ram_HeapSlots->battleFxWork[0];
  struct SolidPoint *corner;
  struct SolidPoint *a;
  struct SolidPoint *b;
  struct SolidPoint *c;
  u8 color;
  s32 words;
  s32 facing;
  s32 top;
  s32 middle;
  s32 bottom;
  s32 slopeA;
  s32 slopeB;
  s32 edgeA;
  s32 edgeB;
  s32 row;
  s32 i;

  words = vertexCount * 3;
  for (i = 0; i != words; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  // The three corners of each face are copied after the vertices.
  corner = (struct SolidPoint *)&vertices[words];
  a = &corner[0];
  b = &corner[1];
  c = &corner[2];
  for (i = 0; i != faceCount; i++) {
    Dma_Set(&vertices[face->corner[0]], a, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[face->corner[1]], b, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[face->corner[2]], c, 0x84000003, REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing = Iwram_MulQ16(b->x - a->x, c->y - a->y) -
             Iwram_MulQ16(c->x - a->x, b->y - a->y);
    if (facing >= 0) {
      color = face->color;
      a->x += 64 << 16;
      a->y += 64 << 16;
      b->x += 64 << 16;
      b->y += 64 << 16;
      c->x += 64 << 16;
      c->y += 64 << 16;

      // Sort the corners by y.
      if (a->y > b->y) {
        XOR_SWAP(a->x, b->x);
        XOR_SWAP(a->y, b->y);
      }
      if (b->y > c->y) {
        XOR_SWAP(b->x, c->x);
        XOR_SWAP(b->y, c->y);
        if (a->y > b->y) {
          XOR_SWAP(a->x, b->x);
          XOR_SWAP(a->y, b->y);
        }
      }

      top = a->y >> 16;
      middle = b->y >> 16;
      bottom = HI(c->y);
      if (top != middle)
        slopeA = Iwram_RatioMulQ14(b->y - a->y,
                                   b->x - a->x);
      if (top != bottom)
        slopeB = Iwram_RatioMulQ14(c->y - a->y,
                                   c->x - a->x);

      // The upper half, between the short edge and the long one.
      edgeB = a->x;
      edgeA = a->x;
      for (row = top; row != middle; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }

      // The lower half, from the middle corner.
      if (middle != bottom)
        slopeA = Iwram_RatioMulQ14(c->y - b->y,
                                   c->x - b->x);
      edgeA = b->x;
      for (row = middle; row != bottom; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }
    }
    face++;
  }
}
