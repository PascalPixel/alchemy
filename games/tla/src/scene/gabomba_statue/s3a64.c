/// @file
/// Scene code of overlay 0x671, in ROM order, from s3a64's offset up to the next object the overlay links.

#include "types.h"
#include "iwram_call.h"
#include "event_inline.h"

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
    Map_CopyCellsRect(0, drawn, 1, 1, drawX + col, drawZ);
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
    Map_CopyCellsRect(shape, piece, 1, 1, drawX + col, drawZ);
  }
turn_next:
  if (++col <= 10)
    goto turn_col;
  if (++row <= 7)
    goto turn_row;
  return state->count;
}

extern u8 Data_02006fc8[], Data_02006fe8[], Data_02007008[], Data_02007028[],
    Data_02007048[], gData_02007068[];
extern s8 gData_02005ac0[];
extern u8 gData_02005ac8[];
s32 abs(s32);
s32 Func_02004224(s32 from, s32 to, s32 kind);
void Func_02004270(s32 x, s32 z, s32 piece, s32 flags);
// Lays a random path of `length` cells across the Gabomba Statue grid (gSceneState,
// 11 by 8 cells of four bytes) from (x0, z0) to (endX, endZ), backtracking through
// per-step stacks (Data_02006fc8 to gData_02007068: direction, x, z, kind, tried
// directions and flags) when a cell has no free neighbour; each step joins the
// cells (Func_02004224 picks the piece, Func_02004270 writes it). Returns 0, or -1
// when the parity of the distance rules a path out or the search gives up.
s32 Func_02003fc0(s32 x0, s32 z0, s32 endX, s32 endZ, s32 length, s32 direction,
                  s32 last, s32 bit) {
  u8 *cells, *kinds;
  s32 piece;
  s32 spare, step, dir, used, dz, index, one;
  u32 x, z, nextX, nextZ, tries, attempts;
  u8 tried;

  length--;
  dz = endZ - z0;
  spare = length - abs(endX - x0) - abs(dz);
  Iwram_ClearWords(Data_02006fc8, 32);
  Iwram_ClearWords(Data_02006fe8, 32);
  Iwram_ClearWords(Data_02007008, 32);
  Iwram_ClearWords(Data_02007028, 32);
  Iwram_ClearWords(Data_02007048, 32);
  Iwram_ClearWords(gData_02007068, 32);
  if (spare & 1)
    return -1;
  x = x0;
  cells = (u8 *)((struct PuzzleCell *)&gSceneState);
  one = 1;
  step = 0;
  attempts = 0;
  z = z0;
  while (1) {
    if (++attempts > 1280)
      return -1;
    {
      s32 offset = (z * 11 + x) * 4;
      Data_02007028[step] = cells[offset];
      gData_02007068[step] = ((struct PuzzleCell *)(cells + offset))->shape;
    }
    Data_02006fe8[step] = x;
    Data_02007008[step] = z;
    Data_02006fc8[step] = direction;
    {
      u8 *joins = gData_02005ac8 + 1;
      kinds = Data_02007028;
      Data_02007048[step] = Data_02007048[step] | joins[kinds[step]] |
                            1 << ((direction + 2) & 3);
    }
    tries = 0;
    while (1) {
      u32 value;
      if (++tries > 63)
        goto back;
      value = Random16Far();
      dir = direction;
      if (value > 0x7fff)
        dir = (Random16Far() * 4) >> 16;
      tried = Data_02007048[step];
      if ((tried >> dir) & one)
        continue;
      {
        s8 *steps = gData_02005ac0 + 1;
        index = dir * 2;
        nextX = x + steps[index];
        index += 1;
        nextZ = z + steps[index];
      }
      if (nextX > 10 || nextZ > 7)
        goto blocked;
      if (step == length - 1 && (nextX != endX || nextZ != endZ))
        goto blocked;
      {
        u8 *cell = cells + (nextZ * 11 + nextX) * 4;
        if (cell[1] != 0)
          goto blocked;
        used = (cell[2] >> bit) & one;
        if (used != 0)
          goto blocked;
        if (cell[0] == 10)
          goto blocked;
      }
      tried |= one << dir;
      Data_02007048[step] = tried;
      kinds = Data_02007028;
      piece = Func_02004224(direction, dir, kinds[step]);
      {
        s32 offset = (z * 11 + x) * 4;
        Func_02004270(x, z, piece, (cells + offset)[2] | one << bit);
      }
      step++;
      x = nextX;
      z = nextZ;
      direction = dir;
      if (step == length)
        goto done;
      Data_02007048[step] = 0;
      goto top;
    blocked:
      Data_02007048[step] |= 1 << dir;
      if (Data_02007048[step] != 0x0f)
        continue;
    back:
    Data_02007048[step] = 0;
    if (step == 0)
      return -1;
    step--;
    x = Data_02006fe8[step];
    z = Data_02007008[step];
    direction = Data_02006fc8[step];
    Func_02004270(x, z, Data_02007028[step], gData_02007068[step]);
    goto top;
    }
    top:;
  }
done:
  {
    s32 offset = (nextZ * 11 + nextX) * 4;
    piece = Func_02004224(dir, last, cells[offset]);
    Func_02004270(nextX, nextZ, piece, (cells + offset)[2] | 1 << bit);
  }
  return 0;
}

