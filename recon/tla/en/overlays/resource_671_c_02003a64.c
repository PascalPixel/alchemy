// Near miss, differ 478 of 629 (tla-en, 2026-10-31): the control flow, calls,
// arguments and loads are the native's; what is left is allocation. The
// native keeps gSceneState in r11, row and z in r7, col and x in r8, failed
// and n in r9, mode and cell in r10, left and tries on the stack; here row
// takes r4 saved around every call, gSceneState r10, tries r11 and n the
// stack, because gcse's PRE hoists gSceneState + 152 (the first fixed cell)
// out of the retry loop into r9, where the native recomputes it each time.
// Tried: the grid walks as for loops (loop.c strength-reduces the cell
// address, 607); the retry as for (;;) with state reloaded at its top (the
// hoist goes, but loop.c hoists the argument constants 1, 3 and 2, 592);
// gSceneState used directly (reloaded at every use, 649); x and z shared
// with col and row (516 to 570); failed and n as one variable (536).
// The cells are read through struct PuzzleCell: the native loads the piece
// as [state + index] and then adds state for the shape or mark, which a
// u8 pointer expression does not give.
//
// Builds a random board for the Gabomba floor puzzle
// in gSceneState's 8 by 11 grid of 4-byte cells (byte 0 the piece, byte 2
// its channel shape, byte 3 marked to be drawn turned) from the seed after
// the one given, kept at gSceneState + 0x160: it clears the grid, places
// the fixed pieces, lays the channels with Func_02003fc0 and Func_02004270
// in one of two layouts (by a coin toss) and starts over with the next
// seed until every channel fits and the board holds a piece of each kind
// the puzzle needs; then it marks a start piece among the bottom three
// rows, four corner pieces and one straight piece (Func_02003934 rejects
// cells in use), marks plain pieces for whatever count is left, turns
// each marked piece at random (Func_02004380) and draws every cell
// (Func_0200566c). Returns the seed it used.
#include "types.h"
#include "iwram_call.h"

struct PuzzleCell {
  u8 piece;
  u8 unk1;
  u8 shape;
  u8 marked;
};

struct SceneState {
  struct PuzzleCell cells[88];
  s32 count;
  s16 firstId;
  s16 endId;
  u16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
  u8 unk170[0x80];
};

extern struct SceneState gSceneState;
extern s32 gRandomState;
u32 Random16Far(void);
void Func_0200566c(s32 marked, s32 piece, s32 width, s32 height, s32 x, s32 z);
s32 Func_02003fc0(s32 x0, s32 z0, s32 x1, s32 z1, s32 a, s32 b, s32 c, s32 d);
void Func_02004270(s32 x, s32 z, s32 a, s32 b);
s32 Func_02003934(s32 col, s32 row);
s32 Func_02004380(s32 col, s32 row, s32 turn);

s32 Func_02003a64(s32 value) {
  struct SceneState *state = &gSceneState;
  s32 row, col, x, z, failed, straight, corner, cross, tries, left, n, mode;
  u32 piece, shape;
  struct PuzzleCell *cell;

  state->count = value - 1;
retry:
  gRandomState = ++state->count;
  Iwram_ClearWords(state, sizeof(state->cells));
  state->cells[3 * 11 + 5].piece = 10;
  state->cells[7 * 11 + 1].piece = 10;
  state->cells[7 * 11 + 9].piece = 10;
  state->cells[7 * 11 + 0].unk1 = 1;
  state->cells[7 * 11 + 2].unk1 = 1;
  state->cells[7 * 11 + 8].unk1 = 1;
  state->cells[7 * 11 + 10].unk1 = 1;
  row = 0;
draw_row:
  col = 0;
draw_col:
  Func_0200566c(0, state->cells[row * 11 + col].piece, 1, 1,
                state->originX + col, state->originZ + row);
  if (++col <= 10)
    goto draw_col;
  if (++row <= 7)
    goto draw_row;
  failed = 0;
  if (Random16Far() < 0x8000) {
    failed = Func_02003fc0(2, 7, 5, 7, 4, 0, 3, 2);
    failed |= Func_02003fc0(8, 7, 6, 6, 4, 2, 2, 3);
    Func_02004270(5, 6, 9, 12);
    failed |= Func_02003fc0(4, 6, 4, 3, 12, 2, 0, 2);
    failed |= Func_02003fc0(5, 5, 5, 2, 14, 3, 1, 3);
  } else {
    failed = Func_02003fc0(2, 7, 4, 6, 4, 0, 0, 2);
    failed |= Func_02003fc0(8, 7, 5, 7, 4, 2, 3, 3);
    Func_02004270(5, 6, 8, 12);
    failed |= Func_02003fc0(5, 5, 4, 3, 12, 3, 0, 2);
    failed |= Func_02003fc0(6, 6, 5, 2, 14, 0, 1, 3);
  }
  if (failed != 0)
    goto retry;
  failed = Func_02003fc0(0, 7, 5, 4, 19, 2, 3, 0);
  failed |= Func_02003fc0(10, 7, 6, 3, 17, 0, 2, 1);
  if (failed != 0)
    goto retry;
  straight = 0;
  corner = 0;
  cross = 0;
  row = 0;
count_row:
  col = 0;
count_col:
  piece = state->cells[row * 11 + col].piece;
  shape = state->cells[row * 11 + col].shape;
  if (shape == 5)
    straight++;
  if (shape == 10)
    corner++;
  if (shape == 12 && piece - 8 <= 1)
    cross++;
  if (++col <= 10)
    goto count_col;
  if (++row <= 7)
    goto count_row;
  if (straight == 0)
    goto retry;
  if (corner == 0)
    goto retry;
  if (cross == 0)
    goto retry;
  mode = (Random16Far() * 2) >> 16;
  left = 7;
  tries = 1;
start_try:
  x = (Random16Far() * 11) >> 16;
  z = ((Random16Far() * 3) >> 16) + 5;
  if (Func_02003934(x, z) == 0) {
    piece = state->cells[z * 11 + x].piece;
    if (piece - 1 <= 1) {
      shape = state->cells[z * 11 + x].shape;
      if ((mode == 0 && (shape & 6) != 0) || (mode == 1 && (shape & 9) != 0)) {
        state->cells[z * 11 + x].piece = 10;
        state->cells[z * 11 + x].marked = 1;
        left = 6;
        goto start_done;
      }
    }
  }
  if (++tries <= 88)
    goto start_try;
start_done:
  n = 3;
corner_next:
  tries = 1;
corner_try:
  x = (Random16Far() * 11) >> 16;
  z = (Random16Far() * 8) >> 16;
  if (Func_02003934(x, z) == 0) {
    piece = state->cells[z * 11 + x].piece;
    if (piece == 3 || piece == 8 || piece == 9) {
      cell = &state->cells[z * 11 + x];
      shape = cell->shape;
      if (shape != 6 && shape != 9) {
        switch (piece) {
        case 3:
          cell->piece = ((Random16Far() * 2) >> 16) + 1;
          break;
        case 8:
          cell->piece = (((Random16Far() * 2) >> 16) << 1) + 4;
          break;
        case 9:
          cell->piece = (((Random16Far() * 2) >> 16) << 1) + 5;
          break;
        }
        state->cells[z * 11 + x].marked = 1;
        left--;
        goto corner_done;
      }
    }
  }
  if (++tries <= 88)
    goto corner_try;
corner_done:
  if (--n >= 0)
    goto corner_next;
  tries = 1;
straight_try:
  x = (Random16Far() * 11) >> 16;
  z = (Random16Far() * 8) >> 16;
  if (Func_02003934(x, z) == 0) {
    piece = state->cells[z * 11 + x].piece;
    if (piece == 4 || piece == 6 || piece == 5 || piece == 7) {
      state->cells[z * 11 + x].marked = 1;
      left--;
      goto straight_done;
    }
  }
  if (++tries <= 88)
    goto straight_try;
straight_done:
  if (left > 0) {
    n = left;
  plain_next:
    tries = 1;
  plain_try:
    x = (Random16Far() * 11) >> 16;
    z = (Random16Far() * 8) >> 16;
    if (Func_02003934(x, z) == 0) {
      piece = state->cells[z * 11 + x].piece;
      if (piece - 1 <= 1) {
        state->cells[z * 11 + x].marked = 1;
        goto plain_done;
      }
    }
    if (++tries <= 88)
      goto plain_try;
  plain_done:
    if (--n != 0)
      goto plain_next;
  }
  row = 0;
turn_row:
  col = 0;
turn_col:
  piece = state->cells[row * 11 + col].piece;
  shape = state->cells[row * 11 + col].marked;
  if (shape != 0) {
    switch ((Random16Far() * 8) >> 16) {
    case 0:
    case 1:
    case 2:
      piece = Func_02004380(col, row, 0);
      break;
    case 3:
    case 4:
    case 5:
      piece = Func_02004380(col, row, 1);
      break;
    case 6:
      Func_02004380(col, row, 0);
      piece = Func_02004380(col, row, 0);
      break;
    }
  }
  Func_0200566c(shape, piece, 1, 1, state->originX + col,
                state->originZ + row);
  if (++col <= 10)
    goto turn_col;
  if (++row <= 7)
    goto turn_row;
  return state->count;
}
