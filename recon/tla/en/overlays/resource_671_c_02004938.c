// Near miss, differ 226 of 500 (tla-en, 2026-10-31): the native keeps the
// ball in r11, the row in r8 and the heading in r9 (here r8, r9 and r11),
// gSceneState in r5 through the set-up, and 0x80000 in no register (built
// again at each use); here local allocation gives r5 to the 0x80000 that CSE
// shares between both products and the cell centre, and r8 to the board
// pointer, which the ball then takes, so gSceneState is reloaded from the
// pool and the branch offsets shift by those instructions. The native also
// recomputes ball + 99 for the last check, where GCSE reuses state. Zeroing
// in the native's order (col, row, heading) measures 246; reading piece and
// marked by index, as the native addresses them, 562.
// Moves a ball along the Gabomba floor's channels one
// sixteenth of a cell per step (step 0 to 15): its cell (the object's
// halfwords 0x64 and 0x66) and heading (byte 0x62, quarter turns) pick the
// channel piece from gSceneState's 11-column grid (byte 0 its kind, byte 3
// 2 for a goal), and the piece sends it straight on or round a quarter
// circle of radius 8 pixels (sine and cosine of the step), turning it to
// face along the curve. At the last step the new cell and heading are
// kept. A ball still rolling (byte 0x63 1) that meets a dead end stops
// with sound 132 (gData_02006ec0), one that falls off the board's end
// stops with sound 204 (gData_02006f38), except at cell (24, 14), where it
// is home (7); a goal piece takes it with sound 204.
#include "types.h"
#include "actor.h"
#include "iwram_call.h"

struct PuzzleCell {
  u8 piece;
  u8 unk1;
  u8 shape;
  u8 marked;
};

struct SceneState {
  struct PuzzleCell cells[88];
  u8 unk160[0x0c];
  s16 originX;
  s16 originZ;
};

extern struct SceneState gSceneState;
extern const u8 gData_02006f38[], gData_02006ec0[];
s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);
void Func_02005854(s32 sound);
void Func_02005634(struct Actor *object, const u8 *script);

void Func_02004938(struct Actor *ball, s32 step) {
  s16 *colp = (s16 *)((u8 *)ball + 100);
  struct SceneState *board = &gSceneState;
  s32 col = *colp;
  s16 *rowp = (s16 *)((u8 *)ball + 102);
  s32 row = *rowp;
  u8 *headingp = (u8 *)ball + 98;
  s32 heading = *headingp;
  s32 angle = heading << 14;
  struct PuzzleCell *cell = &board->cells[row * 11 + col];
  s32 key = (heading << 8) + cell->piece;
  s32 goal = cell->marked;
  s32 d = step - 8;
  s32 arc = step << 10;
  u8 *state;
  s32 s = Iwram_MulQ16(0x80000, Math_Sine(arc));
  s32 c = Iwram_MulQ16(0x80000, Math_Cosine(arc));
  s32 x = ((board->originX + col) << 20) + 0x80000;
  s32 z = ((board->originZ + row) << 20) + 0x80000;

  state = (u8 *)ball + 99;
  if (goal == 2) {
    col = row = heading = 0;
    if (*state == 1) {
      Func_02005854(204);
      Func_02005634(ball, gData_02006f38);
      *state = goal;
    }
    goto done;
  }
  switch (key) {
  case 514:
  case 515:
    col--;
    x -= d << 16;
    break;
  case 2:
  case 3:
    col++;
    x += d << 16;
    break;
  case 257:
  case 259:
    row++;
    z += d << 16;
    break;
  case 769:
  case 771:
    row--;
    z -= d << 16;
    break;
  case 516:
  case 520:
    row++;
    heading = 1;
    x = x - s + 0x80000;
    z = z - c + 0x80000;
    angle = 0x8000 - arc;
    break;
  case 773:
  case 777:
    col--;
    heading = 2;
    x = x + c - 0x80000;
    z = z - s + 0x80000;
    angle = 0xc000 - arc;
    break;
  case 6:
  case 8:
    row--;
    heading = 3;
    x = x + s - 0x80000;
    z = z + c - 0x80000;
    angle = -arc;
    break;
  case 263:
  case 265:
    col++;
    heading = 0;
    x = x - c + 0x80000;
    z = z + s - 0x80000;
    angle = 0x4000 - arc;
    break;
  case 519:
  case 521:
    row--;
    heading = 3;
    x = x - s + 0x80000;
    z = z + c - 0x80000;
    angle = arc + 0x8000;
    break;
  case 262:
  case 264:
    col--;
    heading = 2;
    x = x + c - 0x80000;
    z = z + s - 0x80000;
    angle = arc + 0x4000;
    break;
  case 5:
  case 9:
    row++;
    heading = 1;
    x = x + s - 0x80000;
    z = z - c + 0x80000;
    angle = arc;
    break;
  case 772:
  case 776:
    col++;
    heading = 0;
    x = x - c + 0x80000;
    z = z - s + 0x80000;
    angle = arc + 0xc000;
    break;
  case 11:
    col = row = heading = 0;
    break;
  case 267:
    col = row = heading = 0;
    break;
  case 523:
    col = row = heading = 0;
    break;
  case 10:
  case 266:
  case 522:
  case 778:
    if (*state == 1) {
      ball->x = x;
      ball->y = 0;
      ball->z = z;
      ball->angle = angle;
      if (x >> 20 == 24 && z >> 20 == 14) {
        *state = 7;
      } else {
        Func_02005854(204);
        Func_02005634(ball, gData_02006f38);
        *state = 2;
      }
    }
    break;
  case 779:
    col = row = heading = 0;
    break;
  default:
    col = row = heading = 0;
    if (*state == 1) {
      Func_02005854(132);
      Func_02005634(ball, gData_02006ec0);
      *state = 2;
    }
    break;
  }
done:
  if (step == 15) {
    *colp = col;
    *rowp = row;
    *headingp = heading;
  }
  if (((u8 *)ball)[99] == 1) {
    ball->y = 0;
    ball->x = x;
    ball->z = z;
    ball->angle = angle;
    ball->unk4c++;
  }
}
