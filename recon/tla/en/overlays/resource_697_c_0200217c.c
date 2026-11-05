// Draft, differ 6 of 87 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Measured under -Os, 5 November 2026: differ 8 (tla-en); notes below may predate it.
// Draft, differ 105 of 87, skeleton gap 43 (tla-en, 2026-11-01)
// The backward twin of Func_0802b590 (field/map/edit.c): copies a width by
// height block of map cells and their shape-grid bytes from (srcX, srcY) to
// (dstX, dstY), last row and column first so that a block overlapping its
// destination copies whole. Written in the forward twin's shape, with the
// column loop as a goto loop.
// Left: the native's two for loops are strength-reduced (col << 16 and
// row << 16 stepped by 0xffff0000) yet nothing is hoisted: width - 1 is rebuilt
// on every row and 0xffff0000, -512 and -128 are loaded at each use. Plain for
// loops give that reduction but differ 125, gap 45: the first loop pass moves
// width - 1 into a stack slot (threshold 28 * savings 2 * lifetime 2 against 38
// insns), and the second moves the reduced steps into r9 and r11 (28 * 1 * 1
// against 15 insns in the column loop). Those counts are fixed by the loop's
// size, so no source form with these insns stays under the threshold.
// Tried, on the for form: gMapCellBuffer by symbol, col scoped to the row,
// col set apart, while (--col >= 0) (124), row the same way (124), both (127),
// a while with col-- in the body, (s16) casts on s32 counters (125), s32
// counters without them (differ 103, gap 43, but no shifts at all), split
// pointer sums, &array[] (all 125 unless noted), shapes before cells (143),
// goto outer loop (133), both goto (111).
#include "types.h"
#include "call.h"
#include "ram_buffer.h"

extern u8 gMapShapeGrid[];

void Func_0200217c(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  s16 row;
  s16 col;

  cells += src;
  for (row = height - 1; row >= 0; row--) {
    u32 *dstCell = dstCells + ((row << 7) + width) - 1;
    u32 *srcCell = cells + ((row << 7) + width) - 1;
    u8 *dstShape = dstShapes + ((row << 7) + width) - 1;
    u8 *srcShape = srcShapes + ((row << 7) + width) - 1;

    col = width - 1;
    if (col >= 0) {
    do {
      *dstCell-- = *srcCell--;
      *dstShape-- = *srcShape--;
      col--;
      /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
       * 8 of 87 instructions differ in tla-en. */
      asm("");
    } while (col >= 0);
    }
  }
}
