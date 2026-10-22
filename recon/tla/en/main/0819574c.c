// Near miss, differ 566 of 418 (draft 426 instructions, no device): an
// unused solid renderer, the filled twin of Func_08196248 (same setup,
// transform, corner copies and back-face test), written from the listing (no
// caller in the image; twins12 BattleEffect_RunMemberBeam 0.34). Corners are
// sorted by y with XOR swaps and each half of the triangle is filled along
// 16.16 edge slopes from IwramRatioMulQ14; a pixel keeps the larger colour
// (an unsigned compare, bcs, from the u8 pixel against the u8 colour).
// The same calls and control flow; as in the native, each corner field has
// its own pointer, so every swap reloads through it.
// Left: allocation. The native forms the six pointers into r6, r11, r10 and
// sp+28..36 and the face pointer into sp+8; here they land elsewhere.
// Three corner pointers (a, b, c) score differ 472 but leave the face loop
// 36 instructions short.
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
  s32 *ax;
  s32 *ay;
  s32 *bx;
  s32 *by;
  s32 *cx;
  s32 *cy;
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
  ax = &vertices[words];
  ay = ax + 1;
  bx = ax + 3;
  by = ax + 4;
  cx = ax + 6;
  cy = ax + 7;
  for (i = 0; i != faceCount; i++) {
    Dma_Set(&vertices[face->corner[0]], ax, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[face->corner[1]], bx, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[face->corner[2]], cx, 0x84000003, REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing =
        Iwram_MulQ16(*bx - *ax, *cy - *ay) - Iwram_MulQ16(*cx - *ax, *by - *ay);
    if (facing >= 0) {
      color = face->color;
      *ax += 64 << 16;
      *ay += 64 << 16;
      *bx += 64 << 16;
      *by += 64 << 16;
      *cx += 64 << 16;
      *cy += 64 << 16;

      // Sort the corners by y.
      if (*ay > *by) {
        XOR_SWAP(*ax, *bx);
        XOR_SWAP(*ay, *by);
      }
      if (*by > *cy) {
        XOR_SWAP(*bx, *cx);
        XOR_SWAP(*by, *cy);
        if (*ay > *by) {
          XOR_SWAP(*ax, *bx);
          XOR_SWAP(*ay, *by);
        }
      }

      top = *ay >> 16;
      middle = *by >> 16;
      bottom = HI(*cy);
      if (top != middle)
        slopeA = Iwram_RatioMulQ14(*by - *ay, *bx - *ax);
      if (top != bottom)
        slopeB = Iwram_RatioMulQ14(*cy - *ay, *cx - *ax);

      // The upper half, between the short edge and the long one.
      edgeB = *ax;
      edgeA = *ax;
      for (row = top; row != middle; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }

      // The lower half, from the middle corner.
      if (middle != bottom)
        slopeA = Iwram_RatioMulQ14(*cy - *by, *cx - *bx);
      edgeA = *bx;
      for (row = middle; row != bottom; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }
    }
    face++;
  }
}
