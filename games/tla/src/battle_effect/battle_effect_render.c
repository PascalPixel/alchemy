/// @file
/// The model renderers and projections the battle effects draw with, which
/// follow Iris in the battle effect code.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

// The first three renderers draw a mesh straight into the canvas in C, filled,
// textured or as a wireframe, and nothing calls them; the effects draw through
// Render_DrawModelList and the ARM kernels it copies into IWRAM.

/// Swaps two integers without a temporary.
#define XOR_SWAP(a, b)                                                         \
  {                                                                            \
    (a) ^= (b);                                                                \
    (b) ^= (a);                                                                \
    (a) ^= (b);                                                                \
  }

/// A triangle of the model: where its three corners start in the vertex
/// buffer (in words) and its colour.
struct SolidFace {
  s32 corner[3];
  u8 unread[12]; ///< the renderer skips them
  u8 color;
  u8 padding[3];
};

/// Fills one scanline of the canvas (8x8 tiles, 16 to a row) from xa to xb,
/// keeping the brighter of each pixel's colour and color.
#define FILL_SPAN(row, xa, xb)                                                 \
  {                                                                            \
    s32 left = FIXEDtoINT(xa);                                                 \
    s32 right = FIXEDtoINT(xb);                                                \
    s32 rowOffset;                                                             \
    s32 x;                                                                     \
                                                                               \
    if (left > right)                                                          \
      XOR_SWAP(left, right);                                                   \
    rowOffset = CANVAS_ROW_OFFSET(row);                                        \
    for (x = left; x != right; x++) {                                          \
      s32 pixel = ((x >> 3) << 6) + (x & 7) + rowOffset;                       \
                                                                               \
      if (canvas[pixel] < color)                                               \
        canvas[pixel] = color;                                                 \
    }                                                                          \
  }

/// Unused: nothing calls it. A solid renderer, the filled twin of
/// Render_DrawMeshWireframe. Transforms the vertexCount points in the sheet
/// block by the current matrix, then fills every face of the effect state's
/// list that faces the camera into the canvas, centred at (64, 64), keeping the
/// brighter colour where faces overlap. Each triangle is sorted by y and filled
/// in two halves, top to middle and middle to bottom, along 16.16 edge slopes.
UNUSED void Render_FillMesh(s32 vertexCount, s32 faceCount) {
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  s32 *vertices = Ram_HeapSlots->battleSheet;
  struct SolidFace *faces = (struct SolidFace *)Ram_HeapSlots->battleEffect;
  s32 i;
  s32 *ax, *bx, *cx, *cy, *ay, *by;
  s32 facing;
  s32 (*mul)(s32, s32);
  FIXED slopeA, slopeB;
  s32 top, middle, bottom;
  u8 color;
  FIXED edgeA, edgeB;
  s32 row;

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
    mul = Iwram_MulFX;
    Dma_Set(&vertices[faces[i].corner[0]], ax, DMA_COPY_WORDS(3), REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[1]], bx, DMA_COPY_WORDS(3), REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[2]], cx, DMA_COPY_WORDS(3), REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing = mul(*bx - *ax, *cy - *ay) - mul(*cx - *ax, *by - *ay);
    if (facing >= 0) {
      color = faces[i].color;
      *ax += toFIXED(64);
      *ay += toFIXED(64);
      *bx += toFIXED(64);
      *by += toFIXED(64);
      *cx += toFIXED(64);
      *cy += toFIXED(64);

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

      top = FIXEDtoINT(ax[1]);
      middle = FIXEDtoINT(ax[4]);
      bottom = FIXED_INTEGER_PART(ax[7]);
      if (top != middle)
        slopeA = Iwram_DivFX(ax[4] - ax[1], ax[3] - ax[0]);
      if (top != bottom)
        slopeB = Iwram_DivFX(ax[7] - ax[1], ax[6] - ax[0]);

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
        slopeA = Iwram_DivFX(ax[7] - ax[4], ax[6] - ax[3]);
      edgeA = ax[3];
      for (row = middle; row != bottom; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }
    }
  }
}

/// One mesh face: its three corners as word offsets into the vertex buffer
/// (vertex index * 3), padded to an effect step's 28 bytes.
struct MeshFace {
  s32 vertex[3];
  s32 padding[4];
};

/// The size of v.
#define DIFF_ABS(v) ((v) >= 0 ? (v) : -(v))

/// One textured scanline at canvas row y, from xA to xB, keeping the brighter
/// of the canvas and texture colour.
#define DRAW_SPAN(y)                                                           \
  {                                                                            \
    u8 *row = state->sheet;                                                    \
    s32 left, right, u, v;                                                     \
    s32 rowOffset;                                                             \
    s32 x;                                                                     \
                                                                               \
    row += ((FIXEDtoINT(vA)) << textureShift) + (FIXEDtoINT(uA));              \
    left = FIXEDtoINT(xA);                                                     \
    right = FIXEDtoINT(xB);                                                    \
    u = 0;                                                                     \
    v = 0;                                                                     \
    if (left > right)                                                          \
      XOR_SWAP(left, right);                                                   \
    rowOffset = CANVAS_ROW_OFFSET(y);                                          \
    for (x = left; x != right; x++) {                                          \
      s32 texel = (FIXEDtoINT(u)) + ((FIXEDtoINT(v)) << textureShift);         \
      s32 at = ((x >> 3) << 6) + (x & 7) + rowOffset;                          \
                                                                               \
      if (canvas[at] < row[texel])                                             \
        canvas[at] = row[texel];                                               \
      u += dudx;                                                               \
      v += dvdx;                                                               \
    }                                                                          \
  }

/// The texture steps along a scanline, from the middle corner's row.
#define STEP_RIGHTWARD()                                                       \
  {                                                                            \
    width = DIFF_ABS(ax[3] - ax[0] - Iwram_MulFX(dxB, ax[4] - ax[1]));         \
    dudx = Iwram_DivFX(width,                                                  \
                       uv[1][0] - uv[0][0] - Iwram_MulFX(duB, ax[4] - ax[1])); \
    dvdx = Iwram_DivFX(width,                                                  \
                       uv[1][1] - uv[0][1] - Iwram_MulFX(dvB, ax[4] - ax[1])); \
    XOR_SWAP(duA, duB);                                                        \
    XOR_SWAP(dvA, dvB);                                                        \
  }
#define STEP_LEFTWARD()                                                        \
  {                                                                            \
    width = DIFF_ABS(Iwram_MulFX(dxB, ax[4] - ax[1]) - (ax[3] - ax[0]));       \
    dudx = Iwram_DivFX(width, Iwram_MulFX(duB, ax[4] - ax[1]) -                \
                                  (uv[1][0] - uv[0][0]));                      \
    dvdx = Iwram_DivFX(width, Iwram_MulFX(dvB, ax[4] - ax[1]) -                \
                                  (uv[1][1] - uv[0][1]));                      \
  }

/// Unused: nothing calls it. Transforms vertexCount vertices (16.16 x, y, z in
/// the heap's vertex buffer) in place, then draws each front-facing face of the
/// mesh at the start of the effect state as an affine-textured triangle: uvs
/// gives each face's three texture corners (16.16), the texture is the effect
/// sprite sheet, textureShift its row length as a power of two. Coordinates are
/// offset by 64 pixels into the canvas.
UNUSED void Render_DrawTexturedMesh(s32 vertexCount, s32 faceCount,
                                    s32 (*uvs)[3][2], s32 textureShift) {
  s32 i;
  s32 *vertices;
  u8 *canvas;
  struct MeshFace *faces;
  s32 *ax, *bx, *cx, *cy, *ay, *by;
  s32 uv[3][2];
  s32 width;
  FIXED dxA, dxB, duA, dvA, duB, dvB;
  FIXED xA, xB, uA, vA, uB, vB;
  s32 top, middle, bottom;
  s32 dudx, dvdx;
  struct BattleEffectState *state;
  u32 y;

  vertices = Ram_HeapSlots->battleSheet;
  faces = (struct MeshFace *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  for (i = 0; i != faceCount; i++) {
    // The three corners of each face are copied after the vertices.
    ax = (s32 *)(vertexCount * 4 + (s32)vertices);
    bx = &vertices[vertexCount + 3];
    cx = &vertices[vertexCount + 6];
    cy = &vertices[vertexCount + 7];
    ay = &vertices[vertexCount + 1];
    by = &vertices[vertexCount + 4];
    Dma_Set(&vertices[faces[i].vertex[0]], ax, DMA_COPY_WORDS(3), REG_DMA3);
    Dma_Set(&vertices[faces[i].vertex[1]], bx, DMA_COPY_WORDS(3), REG_DMA3);
    Dma_Set(&vertices[faces[i].vertex[2]], cx, DMA_COPY_WORDS(3), REG_DMA3);

    // Back faces are skipped.
    if (Iwram_MulFX(*bx - *ax, *cy - *ay) - Iwram_MulFX(*cx - *ax, *by - *ay) >=
        0) {
      state = Ram_HeapSlots->battleEffect;
      Dma_Set(uvs[i], uv, DMA_COPY_WORDS(6), REG_DMA3);
      *ax += toFIXED(64);
      *ay += toFIXED(64);
      *bx += toFIXED(64);
      *by += toFIXED(64);
      *cx += toFIXED(64);
      *cy += toFIXED(64);

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

      top = FIXEDtoINT(ax[1]);
      middle = FIXEDtoINT(ax[4]);
      bottom = FIXEDtoINT(ax[7]);
      if (top != middle) {
        s32 height = ax[4] - ax[1];

        dxA = Iwram_DivFX(height, ax[3] - ax[0]);
        duA = Iwram_DivFX(height, uv[1][0] - uv[0][0]);
        dvA = Iwram_DivFX(height, uv[1][1] - uv[0][1]);
      }
      if (top != bottom) {
        s32 height = ax[7] - ax[1];

        dxB = Iwram_DivFX(height, ax[6] - ax[0]);
        duB = Iwram_DivFX(height, uv[2][0] - uv[0][0]);
        dvB = Iwram_DivFX(height, uv[2][1] - uv[0][1]);
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

        dxA = Iwram_DivFX(height, ax[6] - ax[3]);
        if (ax[3] >= ax[6]) {
          duB = duA;
          dvB = dvA;
          uB = uA;
          vB = vA;
        }
        duA = Iwram_DivFX(height, uv[2][0] - uv[1][0]);
        dvA = Iwram_DivFX(height, uv[2][1] - uv[1][1]);
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
          /* LEFTOVER(template): edge B's steps, as Render_FillMesh's lower half
           * steps both its edges and this routine's upper half steps all six
           * values, idle here; as the loop without them. */
          uB += duB;
          vB += dvB;
        }
      }
    }
  }
}

/// A triangle of the wireframe model: where its three corners start in the
/// vertex buffer (in words) and the colour its edges are drawn in.
struct WireFace {
  s32 corner[3];
  u8 unread[12]; ///< the renderer skips them
  u8 color;
  u8 padding[3];
};

/// Unused: nothing calls it. A wireframe renderer. Transforms the vertexCount
/// points in the sheet block by the current matrix, then draws the three edges
/// of every face of the effect state's list that faces the camera (its corners
/// run counter-clockwise on screen) as clipped lines into the canvas, centred
/// at (64, 64).
UNUSED void Render_DrawMeshWireframe(s32 vertexCount, s32 faceCount) {
  s32 *vertices = Ram_HeapSlots->battleSheet;
  struct WireFace *faces = (struct WireFace *)Ram_HeapSlots->battleEffect;
  s32 i;
  s32 facing;
  s32 color;

  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  // The three corners of each face are copied after the vertices.
  for (i = 0; i != faceCount; i++) {
    s32 *x0 = (s32 *)(vertexCount * 4 + (s32)vertices);
    s32 *x1 = x0 + 3;
    s32 *x2 = x0 + 6;
    s32 *y2 = x0 + 7;
    s32 *y0 = x0 + 1;
    s32 *y1 = x0 + 4;

    Dma_Set(&vertices[faces[i].corner[0]], x0, DMA_COPY_WORDS(3), REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[1]], x1, DMA_COPY_WORDS(3), REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[2]], x2, DMA_COPY_WORDS(3), REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing =
        Iwram_MulFX(*x1 - *x0, *y2 - *y0) - Iwram_MulFX(*x2 - *x0, *y1 - *y0);
    if (facing >= 0) {
      color = faces[i].color;
      *x0 += toFIXED(64);
      *y0 += toFIXED(64);
      *x1 += toFIXED(64);
      *y1 += toFIXED(64);
      *x2 += toFIXED(64);
      *y2 += toFIXED(64);
      BattleEffect_DrawClippedCanvasLine(
          FIXED_INTEGER_PART(*x0), FIXED_INTEGER_PART(*y0),
          FIXED_INTEGER_PART(*x1), FIXED_INTEGER_PART(*y1), color);
      BattleEffect_DrawClippedCanvasLine(
          FIXED_INTEGER_PART(*x1), FIXED_INTEGER_PART(*y1),
          FIXED_INTEGER_PART(*x2), FIXED_INTEGER_PART(*y2), color);
      BattleEffect_DrawClippedCanvasLine(
          FIXED_INTEGER_PART(*x0), FIXED_INTEGER_PART(*y0),
          FIXED_INTEGER_PART(*x2), FIXED_INTEGER_PART(*y2), color);
    }
  }
}

/// BattleEffect_BuildBlitter(slot, 7, 7, flags, mode) for a 128 by 128 canvas,
/// with the flags and the mode packed into one word: mode << 4 | flags.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags) {
  return BattleEffect_BuildBlitter(slot, 7, 7, modeAndFlags & 0xf,
                                   (u32)modeAndFlags >> 4);
}

// BattleEffect_BuildBlitter walks The Lost Age's blitter template once and
// writes the blitter's ARM code into a heap slot: a list of steps with short
// copies as block moves and longer ones by DMA, its runs those of
// BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT or a plain copy, and the
// second run copied from the first by its measured length.

#include "system/asm.h"

/// Copies two words from source to destination with one block load and store.
static __inline__ void CopyWords2(const void *source, void *destination) {
  ASM_REGISTER(const void *from, "r0") = source;
  ASM_REGISTER(void *to, "r1") = destination;
  ASM_COPY_WORDS_2(from, to);
}

/// Copies three words from source to destination with one block load and store.
static __inline__ void CopyWords3(const void *source, void *destination) {
  ASM_REGISTER(const void *from, "r0") = source;
  ASM_REGISTER(void *to, "r1") = destination;
  ASM_COPY_WORDS_3(from, to);
}

/// Copies four words from source to destination with one block load and store.
static __inline__ void CopyWords4(const void *source, void *destination) {
  ASM_REGISTER(const void *from, "r0") = source;
  ASM_REGISTER(void *to, "r1") = destination;
  ASM_COPY_WORDS_4(from, to);
}

/// The most bytes a built blitter takes.
#define BLITTER_BYTES 436
/// The run fragments' lengths in words: eight pixels copied, eight copied
/// where not 0, and four kept where brighter or added, each laid twice.
#define COPY8_WORDS 16
#define TRANSPARENT8_WORDS 25
#define MAX4_WORDS 16
#define COLOR_WORDS 14

/// An ARM branch's 24-bit word offset from `site` to `target`.
#define BRANCH_FIELD(site, target)                                             \
  ((((u32)(target) - (u32)(site) - 8) >> 2) & 0xffffff)

/// Writing a blitter at dst: a word; words copied from a fragment, by CPU or
/// DMA, once or twice; a branch to target; or the branch at site aimed here.
#define PUT(word)                                                              \
  do {                                                                         \
    *dst++ = (word);                                                           \
  } while (0)
#define COPY_FROM(from, n)                                                     \
  CopyWords##n(from, dst);                                                     \
  dst += (n)
#define COPY(n)                                                                \
  do {                                                                         \
    COPY_FROM(src, n);                                                         \
    src += (n);                                                                \
  } while (0)
#define COPY_LONG(n)                                                           \
  do {                                                                         \
    Dma_Set(src, dst, control | (n), REG_DMA3);                                \
    dst += (n);                                                                \
    src += (n);                                                                \
  } while (0)
#define COPY_DMA(from, n)                                                      \
  Dma_Set(from, dst, DMA_COPY_WORDS(n), REG_DMA3);                             \
  dst += (n)
#define COPY_TWICE(from, n)                                                    \
  {                                                                            \
    const u32 *part = (from);                                                  \
    u32 control = DMA_COPY_WORDS(n);                                           \
    Dma_Set(part, dst, control, REG_DMA3);                                     \
    dst += (n);                                                                \
    Dma_Set(part, dst, control, REG_DMA3);                                     \
    dst += (n);                                                                \
  }
#define PUT_BRANCH(target)                                                     \
  *dst = *src++ + BRANCH_FIELD(dst, target);                                   \
  dst++
#define PATCH(site)                                                            \
  do {                                                                         \
    *(site) |= BRANCH_FIELD(site, dst);                                        \
  } while (0)

/// Writes a drawing routine for the canvas into heap slot `slot`, built from
/// the template gBlitterTemplate for one clip, mirroring and blend mode.
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode) {
  const u32 *src;
  u32 *dst;
  u32 *noRows;
  u32 *noColumns;
  u32 *row;
  u32 *skip;
  u32 *run;
  s32 runSize;
  u32 *runCopy;
  u32 control;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  u32 frame[34];

  src = gBlitterTemplate;
  /* FAKEMATCH(signature): dst, a u32 *, holds the blitter's size before it
   * holds the allocation. */
  dst = (u32 *)BLITTER_BYTES;
  dst = HeapSlot_Alloc(slot, (s32)dst);

  // Block 1, the entry code.
  COPY(4);
  if (mode == BLITTER_ADDITIVE) {
    COPY_FROM(BattleEffect_Mask, 3);
  }

  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP_X) {
    COPY_FROM(src, 3);
  }
  src += 3;
  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP_Y) {
    COPY_FROM(src, 4);
  }
  src += 4;
  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP) {
    COPY_FROM(src, 3);
  }
  src += 3;
  if ((flags & COMPOSITE_FLIP) == 0) {
    PUT(src[0]);
  }
  src += 1;

  // Block 2, the vertical clip: the height enters the last two words.
  if ((flags & COMPOSITE_CLIP_Y) != 0) {
    COPY(2);
    if ((flags & COMPOSITE_FLIP_Y) != 0) {
      PUT(src[0]);
    } else {
      PUT(src[1]);
    }
    src += 2;
    COPY(2);
    PUT(src[0] + (1 << heightShift));
    PUT(src[1] + (1 << heightShift));
    src -= 6;
  }
  src += 8;

  // Block 3, the columns: the empty-rows skip, the width mask or the twelve
  // clip words, the empty-columns skip, then the rest of the set-up.
  PUT(*src++);
  noRows = dst;
  PUT(*src++);

  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(src[0] + (1 << widthShift) - 1);
  }
  src += 1;
  if ((flags & COMPOSITE_CLIP_X) != 0) {
    PUT(src[0]);
    PUT(src[1]);
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      PUT(src[2]);
      PUT(src[3]);
    } else {
      PUT(src[4]);
      PUT(src[5]);
    }
    PUT(src[6]);
    PUT(src[7]);
    PUT(src[8] + gBlitter_ImmediatePowers[widthShift]);
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      PUT(src[9]);
    } else {
      PUT(src[10]);
    }
    PUT(src[11] + gBlitter_ImmediatePowers[widthShift]);
  }
  src += 12;

  PUT(*src++);
  noColumns = dst;
  PUT(*src++);

  control = DMA_COPY_WORDS(0);
  COPY_LONG(6);

  PUT(*src++);
  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(src[0]);
  }
  src += 1;

  COPY_LONG(5);

  // Block 4, the row loop: its head, the leading partial run, the full-cell
  // loop, the trailing partial run and the step back to the head.
  row = dst;
  PUT(src[0]);
  PUT(src[1] + (1 << (heightShift - 3)) - 1);
  PUT(src[2] + ((widthShift - 3) << 7));
  PUT(src[3]);
  src += 4;

  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(src[0]);
  }
  src += 1;

  skip = dst + 1;
  COPY(2);
  if ((flags & COMPOSITE_FLIP_X) != 0) {
    PUT(src[0]);
  } else {
    PUT(src[1]);
  }
  src += 2;

  run = dst;
  runCopy = dst;
  switch (mode) {
  default: {
    const u32 *part;

    if ((flags & COMPOSITE_FLIP_X) != 0) {
      part = BattleEffect_CopyReverse;
    } else {
      part = BattleEffect_CopyForward;
    }
    COPY_FROM(part, 2);
    break;
  }
  case BLITTER_TRANSPARENT: {
    const u32 *part;

    if ((flags & COMPOSITE_FLIP_X) != 0) {
      part = BattleEffect_TransparentReverse;
    } else {
      part = BattleEffect_TransparentForward;
    }
    COPY_FROM(part, 4);
    break;
  }
  case BLITTER_MAX: {
    const u32 *part;

    if ((flags & COMPOSITE_FLIP_X) != 0) {
      part = BattleEffect_MaxReverse;
    } else {
      part = BattleEffect_MaxForward;
    }
    COPY_FROM(part, 4);
    break;
  }
  case BLITTER_ADDITIVE:
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      COPY_DMA(BattleEffect_AdditiveReverse, 6);
    } else {
      COPY_DMA(BattleEffect_AdditiveForward, 6);
    }
    break;
  }

  PUT(*src++);
  PUT_BRANCH(run);
  runSize = (u8 *)dst - (u8 *)runCopy;
  PUT(*src++);
  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(src[0]);
    PUT(src[1]);
  }
  src += 2;
  PATCH(skip);

  skip = dst + 1;
  COPY(2);

  run = dst;
  switch (mode) {
  default:
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      COPY_DMA(BattleEffect_Copy8Reverse, COPY8_WORDS);
    } else {
      COPY_DMA(BattleEffect_Copy8Forward, COPY8_WORDS);
    }
    break;
  case BLITTER_TRANSPARENT:
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      COPY_DMA(BattleEffect_Transparent8Reverse, TRANSPARENT8_WORDS);
    } else {
      COPY_DMA(BattleEffect_Transparent8Forward, TRANSPARENT8_WORDS);
    }
    break;
  case BLITTER_MAX:
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      COPY_TWICE(BattleEffect_Max4Reverse, MAX4_WORDS);
    } else {
      COPY_TWICE(BattleEffect_Max4Forward, MAX4_WORDS);
    }
    break;
  case BLITTER_ADDITIVE:
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      COPY_TWICE(BattleEffect_ColorReverse, COLOR_WORDS);
    } else {
      COPY_TWICE(BattleEffect_ColorForward, COLOR_WORDS);
    }
    break;
  }

  PUT(*src++);
  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(src[0]);
    PUT(src[1]);
  }
  src += 2;

  PUT(*src++);
  PUT_BRANCH(run);
  PATCH(skip);

  PUT(*src++);
  skip = dst;
  PUT(*src++);
  if ((flags & COMPOSITE_FLIP_X) != 0) {
    PUT(src[0]);
    src += 2;
  } else {
    PUT(src[1]);
    src += 2;
  }
  {
    s32 count = (u32)runSize / 4 * 4;

    /* FAKEMATCH(dead): the division by 4 rounded towards zero by hand, under a
     * test never true of a count of bytes. */
    if (count < 0)
      count += 3;
    Dma_Set(runCopy, dst, DMA_COPY_WORDS(count >> 2), REG_DMA3);
    dst += (u32)runSize / 4;
    /* FAKEMATCH(dead): count set to 0, a store nothing reads. */
    count = 0;
  }
  PATCH(skip);

  COPY(3);

  PUT_BRANCH(row);
  PATCH(noRows);
  PATCH(noColumns);

  // Block 5, the exit, where both skips of block 3 land.
  COPY(2);
  if (src == (const u32 *)gBlitter_ImmediatePowers)
    return TRUE;
  return FALSE;
}

/// Unused: nothing calls it. Copies Render_FillMeshKernel
/// (render_fill_mesh_kernels.s), an ARM build of Render_FillMesh: the same
/// transform of vertexCount points centred at (64, 64), back faces dropped and
/// each triangle sorted by y with XOR swaps, into a heap block (DMA 3, words),
/// runs it there and frees the block.
UNUSED void Render_RunFillMeshKernel(s32 vertexCount, s32 faceCount) {
  void (*routine)(s32, s32);

  RamCode_Load(routine, Render_FillMeshKernel, Render_FillMeshKernelCodeSize);
  routine(vertexCount, faceCount);
  Heap_Release(routine);
}

/// Copies the ARM vertex kernel into a heap block and runs it: count packed
/// s8 vertices through the current transform into screen coordinates.
void Render_TransformVertices(const void *corners, void *vertices, s32 count) {
  void (*routine)(const void *, void *, s32);

  RamCode_Load(routine, Render_TransformKernel, Render_TransformKernelCodeSize);
  routine(corners, vertices, count);
  Heap_Release(routine);
}

/// Projects count model vertices through Matrix_ProjectPoint into screen
/// coordinates.
void Render_ProjectVertices(struct ModelVertex *vertices,
                            struct ProjectedVertex *screen, s32 count) {
  s32 point[3];

  for (; count != 0; count--) {
    SET_VECTOR(point, INTtoFIXED(vertices->x), INTtoFIXED(vertices->y),
               INTtoFIXED(vertices->z));
    Matrix_ProjectPoint(point, point);
    screen->x = point[0];
    screen->y = point[1];
    screen->depth = point[2];
    vertices++;
    screen++;
  }
}

/// A struct RenderModel as Render_AllocateModelList clears it: head and next
/// over its kind and flags, tail over its texture scroll and the padding after
/// it, one word.
struct PoolEntry {
  s32 head;
  s32 next;
  u8 unused[16]; ///< nothing reads or writes them
  s32 tail;
};

/// Allocates count model entries plus a cleared one that ends the list.
struct RenderModel *Render_AllocateModelList(s32 count) {
  struct PoolEntry *entries =
      Heap_AllocEwram((count + 1) * sizeof(struct PoolEntry));
  s32 i;

  for (i = 0; i < count + 1; i++) {
    entries[i].head = 0;
    entries[i].next = 0;
    entries[i].tail = 0;
  }
  return (struct RenderModel *)entries;
}

/// Copies a texture's pixels into a heap block when IWRAM has room for them;
/// returns 1 with copy set up, 0 when there is no room.
s32 Render_CopyTextureToIwram(struct Texture *source, struct Texture *copy) {
  s32 size = (1 << source->logWidth) << source->logHeight;
  u32 pixels;
  s32 misalign;

  if (Heap_GetRemainingIwram() < size + 8)
    return 0;
  *(u16 *)copy = *(u16 *)source;
  copy->pixels = Heap_Alloc(size + 8);
  pixels = (u32)source->pixels;
  misalign = pixels & 3;
  Iwram_CopyWordsEntry(copy->pixels, (void *)(pixels - misalign),
                       (size + misalign + 3) & -4);
  copy->pixels = (u8 *)copy->pixels + misalign;
  return 1;
}

/// The texture stays where it is, even when IWRAM has room for a copy.
#define MODEL_TEXTURE_IN_PLACE 4

typedef void (*ModelKernel)(struct RenderModel *model);

/// Copies a kernel whose DMA control is a word of its own.
#define LOAD_KERNEL_WORDS(to, code, bytes, words)                              \
  {                                                                            \
    (to) = Heap_Alloc((u32)(bytes));                                           \
    Dma_Set((code), (to), DMA_COPY_WORDS(0) + (u32)(words), REG_DMA3);         \
  }

/// Copies the model's texture into IWRAM unless it is to stay in place.
#define COPY_TEXTURE()                                                         \
  {                                                                            \
    if (!(model->flags & MODEL_TEXTURE_IN_PLACE) &&                            \
        Render_CopyTextureToIwram(texture, &copy))                             \
      model->texture = &copy;                                                  \
  }

/// Draws a list of models. A run of models of one kind loads that kind's
/// rasterizer into IWRAM once, and it is freed when the kind changes. Before
/// a textured model is drawn, its texture is copied into IWRAM where there is
/// room, and the copy is freed after the model is drawn.
void Render_DrawModelList(struct RenderModel *model) {
  ModelKernel code = NULL;
  s32 lastKind = 0;
  struct Texture copy;
  struct Texture *texture;

  for (;;) {
    texture = model->texture;
    if (model->kind != lastKind && code != NULL)
      Heap_Release(code);
    if (model->kind == MODEL_END)
      break;
    switch (model->kind) {
    case MODEL_SOLID:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_SolidKernel, Render_SolidKernelCodeSize);
      code(model);
      break;
    case MODEL_SOLID_NARROW:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_SolidNarrowKernel,
                     Render_SolidNarrowKernelCodeSize);
      code(model);
      break;
    case MODEL_SOLID_MAX:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_SolidMaxKernel,
                     Render_SolidMaxKernelCodeSize);
      code(model);
      break;
    case MODEL_SOLID_ADDITIVE:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_SolidAdditiveKernel,
                     Render_SolidAdditiveKernelCodeSize);
      code(model);
      break;
    case MODEL_TEXTURED:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Render_TexturedKernel,
                          Render_TexturedKernelCodeSize,
                          Render_TexturedKernelCodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_SHADED:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Render_ShadedKernel,
                          Render_ShadedKernelCodeSize,
                          Render_ShadedKernelCodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_TEXTURED_NARROW:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Render_TexturedNarrowKernel,
                          Render_TexturedNarrowKernelCodeSize,
                          Render_TexturedNarrowKernelCodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_TEXTURED_MAX:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_TexturedMaxKernel,
                     Render_TexturedMaxKernelCodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_TEXTURED_ADDITIVE:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_TexturedAdditiveKernel,
                     Render_TexturedAdditiveKernelCodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_SHADED_MAX:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_ShadedMaxKernel,
                     Render_ShadedMaxKernelCodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_SHADED_ADDITIVE:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_ShadedAdditiveKernel,
                     Render_ShadedAdditiveKernelCodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    }
    lastKind = model->kind;
    if (model->texture == &copy) {
      Heap_Release((void *)((u32)model->texture->pixels & ~3));
      model->texture = texture;
    }
    model++;
  }
}

#undef XOR_SWAP
#undef FILL_SPAN
#undef DIFF_ABS
#undef DRAW_SPAN
#undef STEP_RIGHTWARD
#undef STEP_LEFTWARD
#undef BRANCH_FIELD
#undef PUT
#undef COPY_FROM
#undef COPY
#undef COPY_LONG
#undef COPY_DMA
#undef COPY_TWICE
#undef PUT_BRANCH
#undef PATCH
#undef MODEL_TEXTURE_IN_PLACE
#undef LOAD_KERNEL_WORDS
#undef COPY_TEXTURE
