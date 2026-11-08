// A textured triangle rasterizer that draws a mesh
// into the 8x8-tiled effect canvas, keeping the brighter of the canvas and
// texture colour indices: the textured member of the family of Func_0819574c
// (filled) and Func_08196248 (wireframe). No Broken Seal twin found.
// Shape, as in Func_0819574c: the corner pointers are formed inside the face
// loop so loop.c hoists them past its guard, faces are indexed so the face
// pointer is loop.c's copy at sp+8, and after the sort the corners are read
// from ax at offsets. The declaration order gives the native's stack slots.
// ax is spelled as an integer sum: &vertices[vertexCount] adds its operands
// the other way round (one more instruction differs).
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "iwram_call.h"
#include "battle_effect_work.h"

// One mesh face: its three corners as word offsets into the vertex buffer
// (vertex index * 3), padded to an effect step's 28 bytes.
struct MeshFace {
  s32 vertex[3];
  s32 unk0c[4];
};

#define XOR_SWAP(a, b)                                                         \
  {                                                                            \
    (a) ^= (b);                                                                \
    (b) ^= (a);                                                                \
    (a) ^= (b);                                                                \
  }
#define DIFF_ABS(v) ((v) >= 0 ? (v) : -(v))

// One textured scanline at canvas row y, from xA to xB, keeping the brighter
// of the canvas and texture colour.
#define DRAW_SPAN(y)                                                           \
  {                                                                            \
    u8 *row = work->sheet;                                                     \
    s32 left, right, u, v;                                                     \
    s32 rowOffset;                                                             \
    s32 x;                                                                     \
                                                                               \
    row += ((vA >> 16) << textureShift) + (uA >> 16);                          \
    left = xA >> 16;                                                           \
    right = xB >> 16;                                                          \
    u = 0;                                                                     \
    v = 0;                                                                     \
    if (left > right)                                                          \
      XOR_SWAP(left, right);                                                   \
    rowOffset = (((y) & 7) << 3) + (((y) >> 3) << 10);                         \
    for (x = left; x != right; x++) {                                          \
      s32 texel = (u >> 16) + ((v >> 16) << textureShift);                     \
      s32 at = ((x >> 3) << 6) + (x & 7) + rowOffset;                          \
                                                                               \
      if (canvas[at] < row[texel])                                             \
        canvas[at] = row[texel];                                               \
      u += dudx;                                                               \
      v += dvdx;                                                               \
    }                                                                          \
  }

// The texture steps along a scanline, from the middle corner's row.
#define STEP_RIGHTWARD()                                                       \
  {                                                                            \
    width = DIFF_ABS(ax[3] - ax[0] - Iwram_MulQ16(dxB, ax[4] - ax[1]));        \
    dudx = Iwram_RatioMulQ14(width, uv[1][0] - uv[0][0] -                      \
                                        Iwram_MulQ16(duB, ax[4] - ax[1]));     \
    dvdx = Iwram_RatioMulQ14(width, uv[1][1] - uv[0][1] -                      \
                                        Iwram_MulQ16(dvB, ax[4] - ax[1]));     \
    XOR_SWAP(duA, duB);                                                        \
    XOR_SWAP(dvA, dvB);                                                        \
  }
#define STEP_LEFTWARD()                                                        \
  {                                                                            \
    width = DIFF_ABS(Iwram_MulQ16(dxB, ax[4] - ax[1]) - (ax[3] - ax[0]));      \
    dudx = Iwram_RatioMulQ14(width, Iwram_MulQ16(duB, ax[4] - ax[1]) -         \
                                        (uv[1][0] - uv[0][0]));                \
    dvdx = Iwram_RatioMulQ14(width, Iwram_MulQ16(dvB, ax[4] - ax[1]) -         \
                                        (uv[1][1] - uv[0][1]));                \
  }

// Transforms vertexCount vertices (16.16 x, y, z in the heap's vertex
// buffer) in place, then draws each front-facing face of the mesh at the
// start of the effect work as an affine-textured triangle: uvs gives each
// face's three texture corners (16.16), the texture is the effect sheet,
// textureShift its row length as a power of two. Coordinates are offset by
// 64 pixels into the canvas.
void Func_08195a9c(s32 vertexCount, s32 faceCount, s32 (*uvs)[3][2],
                   s32 textureShift) {
  s32 i;
  s32 *vertices;
  u8 *canvas;
  struct MeshFace *faces;
  s32 *ax, *bx, *cx, *cy, *ay, *by;
  s32 uv[3][2];
  s32 width;
  s32 dxA, dxB, duA, dvA, duB, dvB;
  s32 xA, xB, uA, vA, uB, vB;
  s32 top, middle, bottom;
  s32 dudx, dvdx;
  struct BattleEffectWork *work;
  u32 y;

  vertices = Ram_HeapSlots->unk64[0];
  faces = (struct MeshFace *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  for (i = 0; i != faceCount; i++) {
    // As an integer sum: the native adds the scaled count first.
    ax = (s32 *)(vertexCount * 4 + (s32)vertices);
    bx = &vertices[vertexCount + 3];
    cx = &vertices[vertexCount + 6];
    cy = &vertices[vertexCount + 7];
    ay = &vertices[vertexCount + 1];
    by = &vertices[vertexCount + 4];
    Dma_Set(&vertices[faces[i].vertex[0]], ax,
            (DMA_ENABLE | DMA_32BIT) << 16 | 3, REG_DMA3);
    Dma_Set(&vertices[faces[i].vertex[1]], bx,
            (DMA_ENABLE | DMA_32BIT) << 16 | 3, REG_DMA3);
    Dma_Set(&vertices[faces[i].vertex[2]], cx,
            (DMA_ENABLE | DMA_32BIT) << 16 | 3, REG_DMA3);

    // Back faces are skipped.
    if (Iwram_MulQ16(*bx - *ax, *cy - *ay) -
            Iwram_MulQ16(*cx - *ax, *by - *ay) >=
        0) {
      work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
      Dma_Set(uvs[i], uv, (DMA_ENABLE | DMA_32BIT) << 16 | 6, SPLIT_CONSTANT(REG_DMA3));
      *ax += 64 << 16;
      *ay += 64 << 16;
      *bx += 64 << 16;
      *by += 64 << 16;
      *cx += 64 << 16;
      *cy += 64 << 16;

      // Sort the corners from top to bottom. bx[1] is *by and cx[1] is *cy.
      if (*ay > *by) {
        XOR_SWAP(*ax, *bx);
        *ay ^= *by;
        bx[1] = *by ^ *ay;
        *ay ^= bx[1];
        XOR_SWAP(uv[0][0], uv[1][0]);
        XOR_SWAP(uv[0][1], uv[1][1]);
      }
      if (*by > *cy) {
        XOR_SWAP(*bx, *cx);
        bx[1] = *by ^ *cy;
        cx[1] = *cy ^ bx[1];
        *by ^= cx[1];
        XOR_SWAP(uv[1][0], uv[2][0]);
        XOR_SWAP(uv[1][1], uv[2][1]);
        if (*ay > *by) {
          XOR_SWAP(*ax, *bx);
          *ay ^= *by;
          bx[1] = *by ^ *ay;
          *ay ^= bx[1];
          XOR_SWAP(uv[0][0], uv[1][0]);
          XOR_SWAP(uv[0][1], uv[1][1]);
        }
      }

      top = ax[1] >> 16;
      middle = ax[4] >> 16;
      bottom = ax[7] >> 16;
      if (top != middle) {
        s32 height = ax[4] - ax[1];

        dxA = Iwram_RatioMulQ14(height, ax[3] - ax[0]);
        duA = Iwram_RatioMulQ14(height, uv[1][0] - uv[0][0]);
        dvA = Iwram_RatioMulQ14(height, uv[1][1] - uv[0][1]);
      }
      if (top != bottom) {
        s32 height = ax[7] - ax[1];

        dxB = Iwram_RatioMulQ14(height, ax[6] - ax[0]);
        duB = Iwram_RatioMulQ14(height, uv[2][0] - uv[0][0]);
        dvB = Iwram_RatioMulQ14(height, uv[2][1] - uv[0][1]);
      }
      xB = xA = ax[0];
      uA = uB = uv[0][0];
      vA = vB = uv[0][1];

      if (ax[3] >= ax[6])
        STEP_RIGHTWARD()
      else
        STEP_LEFTWARD()

      // The upper half.
      for (y = top; y != middle; y++) {
        DRAW_SPAN(y);
        xA += dxA;
        xB += dxB;
        uA += duA;
        vA += dvA;
        uB += duB;
        vB += dvB;
      }

      // The lower half: edge A now runs from the middle corner to the bottom.
      if (middle != bottom) {
        s32 height = ax[7] - ax[4];

        dxA = Iwram_RatioMulQ14(height, ax[6] - ax[3]);
        if (ax[3] >= ax[6]) {
          duB = duA;
          dvB = dvA;
          uB = uA;
          vB = vA;
        }
        duA = Iwram_RatioMulQ14(height, uv[2][0] - uv[1][0]);
        dvA = Iwram_RatioMulQ14(height, uv[2][1] - uv[1][1]);
        xA = ax[3];
        uA = uv[1][0];
        vA = uv[1][1];
        if (ax[0] <= xA) {
          STEP_RIGHTWARD();
          uA = uB;
          vA = vB;
        } else
          STEP_LEFTWARD()
        for (y = middle; y != bottom; y++) {
          DRAW_SPAN(y);
          xA += dxA;
          xB += dxB;
          uA += duA;
          vA += dvA;
          /* FAKEMATCH(dead): edge B's values stepped on though nothing reads
           * them again; flow deletes both, and cse2 then stores uv[0] to uB
           * ahead of uA, as the native does; as no steps, 4 of 966 instructions
           * differ in tla-en. */
          uB += duB;
          vB += dvB;
        }
      }
    }
  }
}
