// Draft, differ 694 of 629: builds a random board for the Gabomba floor puzzle
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
// Left: a rough reading: the alignment is poor; the native keeps gSceneState in
// r11, the cell's column and row in r8 and r7, the tries and the count left on
// the stack, and rebuilds each loop's constants inside it as a goto loop would.
#include "types.h"
#include "iwram_call.h"

extern u8 gSceneState[];
extern s32 gRandomState;
u32 Random16Far(void);
void Func_0200566c(s32 marked, s32 piece, s32 width, s32 height, s32 x, s32 z);
s32 Func_02003fc0(s32 x0, s32 z0, s32 x1, s32 z1, s32 a, s32 b, s32 c, s32 d);
void Func_02004270(s32 x, s32 z, s32 a, s32 b);
s32 Func_02003934(s32 col, s32 row);
s32 Func_02004380(s32 col, s32 row, s32 turn);

#define CELL(row, col) (gSceneState + ((row) * 11 + (col)) * 4)
#define SEED (*(s32 *)(gSceneState + 0x160))

s32 Func_02003a64(s32 seed) {
  s32 row, col, failed, straight, corner, cross, tries, left, n, mode;
  u32 piece, shape;
  u8 *cell;

  SEED = seed - 1;
retry:
  gRandomState = ++SEED;
  Iwram_ClearWords(gSceneState, 0x160);
  gSceneState[152] = 10;
  gSceneState[312] = 10;
  gSceneState[344] = 10;
  gSceneState[309] = 1;
  gSceneState[317] = 1;
  gSceneState[341] = 1;
  gSceneState[349] = 1;
  for (row = 0; row <= 7; row++)
    for (col = 0; col <= 10; col++)
      Func_0200566c(0, CELL(row, col)[0], 1, 1,
                    *(s16 *)(gSceneState + 364) + col,
                    *(s16 *)(gSceneState + 366) + row);
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
  if ((Func_02003fc0(0, 7, 5, 4, 19, 2, 3, 0) |
       Func_02003fc0(10, 7, 6, 3, 17, 0, 2, 1)) != 0)
    goto retry;
  straight = corner = cross = 0;
  for (row = 0; row <= 7; row++) {
    for (col = 0; col <= 10; col++) {
      piece = CELL(row, col)[0];
      shape = CELL(row, col)[2];
      if (shape == 5)
        straight++;
      if (shape == 10)
        corner++;
      if (shape == 12 && piece - 8 <= 1)
        cross++;
    }
  }
  if (straight == 0)
    goto retry;
  if (corner == 0)
    goto retry;
  if (cross == 0)
    goto retry;
  mode = (Random16Far() * 2) >> 16;
  left = 7;
  for (tries = 1; tries <= 88; tries++) {
    col = (Random16Far() * 11) >> 16;
    row = ((Random16Far() * 3) >> 16) + 5;
    if (Func_02003934(col, row) != 0)
      continue;
    if (CELL(row, col)[0] - 1 > 1)
      continue;
    shape = CELL(row, col)[2];
    if ((mode == 0 && (shape & 6) != 0) || (mode == 1 && (shape & 9) != 0)) {
      CELL(row, col)[0] = 10;
      CELL(row, col)[3] = 1;
      left = 6;
      break;
    }
  }
  for (n = 3; n >= 0; n--) {
    for (tries = 1; tries <= 88; tries++) {
      col = (Random16Far() * 11) >> 16;
      row = (Random16Far() * 8) >> 16;
      if (Func_02003934(col, row) != 0)
        continue;
      piece = CELL(row, col)[0];
      if (piece != 3 && piece != 8 && piece != 9)
        continue;
      cell = CELL(row, col);
      shape = cell[2];
      if (shape == 6 || shape == 9)
        continue;
      switch (piece) {
      case 3:
        cell[0] = ((Random16Far() * 2) >> 16) + 1;
        break;
      case 8:
        cell[0] = (((Random16Far() * 2) >> 16) << 1) + 4;
        break;
      case 9:
        cell[0] = (((Random16Far() * 2) >> 16) << 1) + 5;
        break;
      }
      CELL(row, col)[3] = 1;
      left--;
      break;
    }
  }
  for (tries = 1; tries <= 88; tries++) {
    col = (Random16Far() * 11) >> 16;
    row = (Random16Far() * 8) >> 16;
    if (Func_02003934(col, row) != 0)
      continue;
    piece = CELL(row, col)[0];
    if (piece == 4 || piece == 6 || piece == 5 || piece == 7) {
      CELL(row, col)[3] = 1;
      left--;
      break;
    }
  }
  if (left > 0) {
    for (n = left; n != 0; n--) {
      for (tries = 1; tries <= 88; tries++) {
        col = (Random16Far() * 11) >> 16;
        row = (Random16Far() * 8) >> 16;
        if (Func_02003934(col, row) != 0)
          continue;
        if (CELL(row, col)[0] - 1 <= 1) {
          CELL(row, col)[3] = 1;
          break;
        }
      }
    }
  }
  for (row = 0; row <= 7; row++) {
    for (col = 0; col <= 10; col++) {
      piece = CELL(row, col)[0];
      shape = CELL(row, col)[3];
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
      Func_0200566c(shape, piece, 1, 1, *(s16 *)(gSceneState + 364) + col,
                    *(s16 *)(gSceneState + 366) + row);
    }
  }
  return SEED;
}
