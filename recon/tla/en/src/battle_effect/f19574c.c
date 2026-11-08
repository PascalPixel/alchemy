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
  s32 (*mul)(s32, s32);
  s32 slopeA, slopeB, top, middle, bottom;
  u8 color;
  s32 edgeA, edgeB, row;

  // From here on, the count of words: three to a point.
  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  for (i = 0; i != faceCount; i++) {
    // The three corners of each face are copied after the vertices.
    ax = (s32 *)(vertexCount * 4 + (s32)vertices);
    bx = ax + 3;
    cx = ax + 6;
    cy = ax + 7;
    ay = ax + 1;
    by = ax + 4;
    // A local holding the multiply routine's address (one value throughout).
    mul = Iwram_MulQ16Entry;
    Dma_Set(&vertices[faces[i].corner[0]], ax, SPLIT_CONSTANT(0x84000003), REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[1]], bx, SPLIT_CONSTANT(0x84000003), REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[2]], cx, SPLIT_CONSTANT(0x84000003), REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing = mul(*bx - *ax, *cy - *ay) - mul(*cx - *ax, *by - *ay);
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
