// Draft, differ 874 of 966 (draft 918, no device), written from the
// listing: a textured triangle rasterizer that draws a mesh into the
// 8x8-tiled effect canvas, keeping the brighter of the canvas and texture
// colour indices. No Broken Seal twin found; the textured member of the family
// of Func_0819574c (filled) and Func_08196248 (wireframe), whose drafts give
// each corner field its own pointer. The slope and texture-step
// setup between the sort and the scanline loops matches row for row apart
// from stack slots.
// Native frame: 144 bytes, uv at sp+120, faceCount, uvs and textureShift
// spilled at sp+116/112/108, the one counter for the transform and face
// loops at sp+104 (shared here too), vertices and canvas at sp+100/96, and
// five hoisted pointers to the triangle's x1, x2, y2, y0 and y1 at
// sp+92..76 (the cull, the 64-pixel offset and the sort go through them,
// so the sort reloads each coordinate after every store); then the slopes
// dxA, dxB, duA, dvA, duB, dvB at sp+72..52, the edges xA, xB, uA, vA, uB,
// vB at sp+48..28, yMid, yBottom, dudx, work and the face pointer at
// sp+24..8. Here the stack frame is 132 and the triangle is addressed as r6 plus
// offsets, so the sort works in registers and every slot shifts.
// Tried: six coordinate pointers (assigned before the face loop: differ
// 1284; inside it, which hoists them as the native does: 1274; with the
// triangle base as an expression in the loop: 1292, weighted 46195 against
// 25930 here); the swaps as an inline taking pointers (1345); the
// declarations in the native's slot order (890, weighted 25840).
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

struct MeshVertex {
  s32 x;
  s32 y;
  s32 z;
};

#define XOR_SWAP(a, b)                                                         \
  {                                                                            \
    (a) ^= (b);                                                                \
    (b) ^= (a);                                                                \
    (a) ^= (b);                                                                \
  }
#define DIFF_ABS(v) ((v) >= 0 ? (v) : -(v))

// Transforms vertexCount vertices (16.16 x, y, z in the heap's vertex
// buffer) in place, then draws each front-facing face of the mesh at the
// start of the effect work as an affine-textured triangle: uvs gives each
// face's three texture corners (16.16), the texture is the effect sheet,
// textureShift its row length as a power of two. Coordinates are offset by
// 64 pixels into the canvas.
void Func_08195a9c(s32 vertexCount, s32 faceCount, s32 (*uvs)[3][2],
                   s32 textureShift) {
  s32 *vertices;
  u8 *canvas;
  struct MeshFace *face;
  struct MeshVertex *tri;
  struct BattleEffectWork *work;
  s32 uv[3][2];
  s32 i;
  s32 height;
  s32 width;
  s32 yTop;
  s32 yMid;
  s32 yBottom;
  s32 y;
  s32 xA, xB, uA, uB, vA, vB;
  s32 dxA, dxB, duA, duB, dvA, dvB;
  s32 dudx, dvdx;

  vertices = Ram_HeapSlots->unk64[0];
  face = (struct MeshFace *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  tri = (struct MeshVertex *)&vertices[vertexCount];
  for (i = 0; i != faceCount; i++, face++) {
    Dma_Set(&vertices[face->vertex[0]], &tri[0],
            (DMA_ENABLE | DMA_32BIT) << 16 | 3, REG_DMA3);
    Dma_Set(&vertices[face->vertex[1]], &tri[1],
            (DMA_ENABLE | DMA_32BIT) << 16 | 3, REG_DMA3);
    Dma_Set(&vertices[face->vertex[2]], &tri[2],
            (DMA_ENABLE | DMA_32BIT) << 16 | 3, REG_DMA3);

    // Back faces are skipped.
    if (Iwram_MulQ16(tri[1].x - tri[0].x, tri[2].y - tri[0].y) -
            Iwram_MulQ16(tri[2].x - tri[0].x, tri[1].y - tri[0].y) <
        0)
      continue;

    work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    Dma_Set(uvs[i], uv, (DMA_ENABLE | DMA_32BIT) << 16 | 6, REG_DMA3);
    tri[0].x += 64 << 16;
    tri[0].y += 64 << 16;
    tri[1].x += 64 << 16;
    tri[1].y += 64 << 16;
    tri[2].x += 64 << 16;
    tri[2].y += 64 << 16;

    // Sort the corners from top to bottom.
    if (tri[0].y > tri[1].y) {
      XOR_SWAP(tri[0].x, tri[1].x);
      XOR_SWAP(tri[0].y, tri[1].y);
      XOR_SWAP(uv[0][0], uv[1][0]);
      XOR_SWAP(uv[0][1], uv[1][1]);
    }
    if (tri[1].y > tri[2].y) {
      XOR_SWAP(tri[1].x, tri[2].x);
      XOR_SWAP(tri[1].y, tri[2].y);
      XOR_SWAP(uv[1][0], uv[2][0]);
      XOR_SWAP(uv[1][1], uv[2][1]);
      if (tri[0].y > tri[1].y) {
        XOR_SWAP(tri[0].x, tri[1].x);
        XOR_SWAP(tri[0].y, tri[1].y);
        XOR_SWAP(uv[0][0], uv[1][0]);
        XOR_SWAP(uv[0][1], uv[1][1]);
      }
    }

    yMid = tri[1].y >> 16;
    yTop = tri[0].y >> 16;
    yBottom = tri[2].y >> 16;
    if (yTop != yMid) {
      height = tri[1].y - tri[0].y;
      dxA = Iwram_RatioMulQ14(height, tri[1].x - tri[0].x);
      duA = Iwram_RatioMulQ14(height, uv[1][0] - uv[0][0]);
      dvA = Iwram_RatioMulQ14(height, uv[1][1] - uv[0][1]);
    }
    if (yTop != yBottom) {
      height = tri[2].y - tri[0].y;
      dxB = Iwram_RatioMulQ14(height, tri[2].x - tri[0].x);
      duB = Iwram_RatioMulQ14(height, uv[2][0] - uv[0][0]);
      dvB = Iwram_RatioMulQ14(height, uv[2][1] - uv[0][1]);
    }
    xB = xA = tri[0].x;
    uB = uA = uv[0][0];
    vB = vA = uv[0][1];

    // The texture steps along a scanline, from the middle corner's row.
    if (tri[1].x >= tri[2].x) {
      width = DIFF_ABS(tri[1].x - tri[0].x -
                       Iwram_MulQ16(dxB, tri[1].y - tri[0].y));
      dudx = Iwram_RatioMulQ14(
          width, uv[1][0] - uv[0][0] - Iwram_MulQ16(duB, tri[1].y - tri[0].y));
      dvdx = Iwram_RatioMulQ14(
          width, uv[1][1] - uv[0][1] - Iwram_MulQ16(dvB, tri[1].y - tri[0].y));
      XOR_SWAP(duA, duB);
      XOR_SWAP(dvA, dvB);
    } else {
      width = DIFF_ABS(Iwram_MulQ16(dxB, tri[1].y - tri[0].y) -
                       (tri[1].x - tri[0].x));
      dudx = Iwram_RatioMulQ14(width, Iwram_MulQ16(duB, tri[1].y - tri[0].y) -
                                          (uv[1][0] - uv[0][0]));
      dvdx = Iwram_RatioMulQ14(width, Iwram_MulQ16(dvB, tri[1].y - tri[0].y) -
                                          (uv[1][1] - uv[0][1]));
    }

    // The upper half.
    for (y = yTop; y != yMid; y++) {
      u8 *row = work->sheet + ((vA >> 16) << textureShift) + (uA >> 16);
      s32 left = xA >> 16;
      s32 right = xB >> 16;
      s32 u = 0;
      s32 v = 0;
      s32 rowOffset;
      s32 x;

      if (left > right)
        XOR_SWAP(left, right);
      rowOffset = ((y & 7) << 3) + ((y >> 3) << 10);
      for (x = left; x != right; x++) {
        u8 texel = row[(u >> 16) + ((v >> 16) << textureShift)];
        s32 at = ((x >> 3) << 6) + (x & 7) + rowOffset;

        if (canvas[at] < texel)
          canvas[at] = texel;
        u += dudx;
        v += dvdx;
      }
      xA += dxA;
      xB += dxB;
      uA += duA;
      vA += dvA;
      uB += duB;
      vB += dvB;
    }
    if (yMid == yBottom)
      continue;

    // The lower half: edge A now runs from the middle corner to the bottom.
    height = tri[2].y - tri[1].y;
    dxA = Iwram_RatioMulQ14(height, tri[2].x - tri[1].x);
    if (tri[1].x >= tri[2].x) {
      duB = duA;
      dvB = dvA;
      uB = uA;
      vB = vA;
    }
    duA = Iwram_RatioMulQ14(height, uv[2][0] - uv[1][0]);
    dvA = Iwram_RatioMulQ14(height, uv[2][1] - uv[1][1]);
    xA = tri[1].x;
    uA = uv[1][0];
    vA = uv[1][1];
    if (tri[0].x <= xA) {
      width = DIFF_ABS(tri[1].x - tri[0].x -
                       Iwram_MulQ16(dxB, tri[1].y - tri[0].y));
      dudx = Iwram_RatioMulQ14(
          width, uv[1][0] - uv[0][0] - Iwram_MulQ16(duB, tri[1].y - tri[0].y));
      dvdx = Iwram_RatioMulQ14(
          width, uv[1][1] - uv[0][1] - Iwram_MulQ16(dvB, tri[1].y - tri[0].y));
      XOR_SWAP(duA, duB);
      XOR_SWAP(dvA, dvB);
      uA = uB;
      vA = vB;
    } else {
      width = DIFF_ABS(Iwram_MulQ16(dxB, tri[1].y - tri[0].y) -
                       (tri[1].x - tri[0].x));
      dudx = Iwram_RatioMulQ14(width, Iwram_MulQ16(duB, tri[1].y - tri[0].y) -
                                          (uv[1][0] - uv[0][0]));
      dvdx = Iwram_RatioMulQ14(width, Iwram_MulQ16(dvB, tri[1].y - tri[0].y) -
                                          (uv[1][1] - uv[0][1]));
    }
    for (y = yMid; y != yBottom; y++) {
      u8 *row = work->sheet + ((vA >> 16) << textureShift) + (uA >> 16);
      s32 left = xA >> 16;
      s32 right = xB >> 16;
      s32 u = 0;
      s32 v = 0;
      s32 rowOffset;
      s32 x;

      if (left > right)
        XOR_SWAP(left, right);
      rowOffset = ((y & 7) << 3) + ((y >> 3) << 10);
      for (x = left; x != right; x++) {
        u8 texel = row[(u >> 16) + ((v >> 16) << textureShift)];
        s32 at = ((x >> 3) << 6) + (x & 7) + rowOffset;

        if (canvas[at] < texel)
          canvas[at] = texel;
        u += dudx;
        v += dvdx;
      }
      xA += dxA;
      xB += dxB;
      uA += duA;
      vA += dvA;
    }
  }
}
