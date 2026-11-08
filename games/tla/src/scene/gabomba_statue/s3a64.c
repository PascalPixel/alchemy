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
void Func_0200566c(s32 marked, s32 piece, s32 width, s32 height, s32 col,
                   s32 row);
s32 Func_02003fc0(s32 x0, s32 z0, s32 x1, s32 z1, s32 a, s32 b, s32 c, s32 d);
void Func_02004270(s32 col, s32 row, s32 a, s32 b);
s32 Func_02003934(s32 col, s32 row);
s32 Func_02004380(s32 col, s32 row, s32 turn);

s32 Func_02003a64(s32 value) {
  struct SceneState *state = &gSceneState;
  s32 row, col, n, straight, corner, cross, tries, left, mode;
  s32 piece, shape;
  struct PuzzleCell *cell;

  state->count = value - 1;
  /* FAKEMATCH(dead): the form kept; as no such test, 580 of 629 instructions
   * differ in tla-en. */
  {
    s32 mark = state->cells[0].marked;
    if (mark & 0x100) {
      if (value == 1)
        goto draw_next;
      if (value == 2)
        goto count_next;
      if (value == 3)
        goto turn_next;
      if (value == 4)
        goto start_miss;
      if (value == 5)
        goto corner_miss;
      if (value == 6)
        goto straight_miss;
      if (value == 7)
        goto plain_miss;
    }
  }
retry:
  state->count += 1;
  gRandomState = state->count;
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
  /* FAKEMATCH(temporary): the form kept; as plain arguments, 231 of 629
   * instructions differ in tla-en. */
  {
    s32 drawn = state->cells[row * 11 + col].piece;
    s32 drawZ, drawX = state->originX;
    drawZ = state->originZ + row;
    Func_0200566c(0, drawn, 1, 1, drawX + col, drawZ);
  }
draw_next:
  if (++col <= 10)
    goto draw_col;
  if (++row <= 7)
    goto draw_row;
  n = 0;
  if (Random16Far() < 0x8000) {
    n |= Func_02003fc0(2, 7, 5, 7, 4, 0, 3, 2);
    n |= Func_02003fc0(8, 7, 6, 6, 4, 2, 2, 3);
    Func_02004270(5, 6, 9, 12);
    n |= Func_02003fc0(4, 6, 4, 3, 12, 2, 0, 2);
    n |= Func_02003fc0(5, 5, 5, 2, 14, 3, 1, 3);
  } else {
    n |= Func_02003fc0(2, 7, 4, 6, 4, 0, 0, 2);
    n |= Func_02003fc0(8, 7, 5, 7, 4, 2, 3, 3);
    Func_02004270(5, 6, 8, 12);
    n |= Func_02003fc0(5, 5, 4, 3, 12, 3, 0, 2);
    n |= Func_02003fc0(6, 6, 5, 2, 14, 0, 1, 3);
  }
  if (n != 0)
    goto retry;
  n = Func_02003fc0(0, 7, 5, 4, 19, 2, 3, 0);
  n |= Func_02003fc0(10, 7, 6, 3, 17, 0, 2, 1);
  if (n != 0)
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
  if (shape == 12 && (u32)(piece - 8) <= 1)
    cross++;
count_next:
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
  col = (Random16Far() * 11) >> 16;
  row = ((Random16Far() * 3) >> 16) + 5;
  if (Func_02003934(col, row) == 0) {
    piece = state->cells[row * 11 + col].piece;
    if ((u32)(piece - 1) <= 1) {
      shape = state->cells[row * 11 + col].shape;
      if ((mode == 0 && (shape & 6) != 0) || (mode == 1 && (shape & 9) != 0)) {
        state->cells[row * 11 + col].piece = 10;
        state->cells[row * 11 + col].marked = 1;
        left = 6;
        goto start_done;
      }
    }
  }
start_miss:
  if (++tries <= 88)
    goto start_try;
start_done:
  n = 3;
corner_next:
  tries = 1;
corner_try:
  col = (Random16Far() * 11) >> 16;
  row = (Random16Far() * 8) >> 16;
  if (Func_02003934(col, row) == 0) {
    piece = state->cells[row * 11 + col].piece;
    if (piece == 3 || piece == 8 || piece == 9) {
      cell = &state->cells[row * 11 + col];
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
        state->cells[row * 11 + col].marked = 1;
        left--;
        goto corner_done;
      }
    }
  }
corner_miss:
  if (++tries <= 88)
    goto corner_try;
corner_done:
  if (--n >= 0)
    goto corner_next;
  tries = 1;
straight_try:
  col = (Random16Far() * 11) >> 16;
  row = (Random16Far() * 8) >> 16;
  if (Func_02003934(col, row) == 0) {
    piece = state->cells[row * 11 + col].piece;
    if (piece == 4 || piece == 6 || piece == 5 || piece == 7) {
      state->cells[row * 11 + col].marked = 1;
      left--;
      goto straight_done;
    }
  }
straight_miss:
  if (++tries <= 88)
    goto straight_try;
straight_done:
  if (left > 0) {
    n = left;
  plain_next:
    tries = 1;
  plain_try:
    col = (Random16Far() * 11) >> 16;
    row = (Random16Far() * 8) >> 16;
    if (Func_02003934(col, row) == 0) {
      piece = state->cells[row * 11 + col].piece;
      if ((u32)(piece - 1) <= 1) {
        state->cells[row * 11 + col].marked = 1;
        goto plain_done;
      }
    }
  plain_miss:
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
  /* FAKEMATCH(temporary): the form kept; as plain arguments, 12 of 629
   * instructions differ in tla-en. */
  {
    s32 drawZ, drawX = state->originX;
    drawZ = state->originZ + row;
    Func_0200566c(shape, piece, 1, 1, drawX + col, drawZ);
  }
turn_next:
  if (++col <= 10)
    goto turn_col;
  if (++row <= 7)
    goto turn_row;
  return state->count;
}
