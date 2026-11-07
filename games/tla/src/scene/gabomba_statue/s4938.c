#include "types.h"
#include "call.h"
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
  s32 col = *colp;
  s16 *rowp = (s16 *)((u8 *)ball + 102);
  s32 row = *rowp;
  u8 *headingp = (u8 *)ball + 98;
  s32 heading = *headingp;
  s32 angle = heading << 14;
  s32 cell = row * 11 + col;
  struct SceneState *board = &gSceneState;
  s32 piece = board->cells[cell].piece;
  s32 goal = board->cells[cell].marked;
  s32 key = (heading << 8) + piece;
  s32 d = step - 8;
  s32 arc = step << 10;
  s32 s = Value2((s32 (*)())Iwram_MulQ16, 0x80000, Math_Sine(arc));
  s32 c = Value2((s32 (*)())Iwram_MulQ16, 0x80000, Math_Cosine(arc));
  s32 z;
  s32 x;

  x = ((board->originX + col) << 20) + 0x80000;
  z = ((board->originZ + row) << 20) + 0x80000;
  if (goal == 2) {
    heading = row = col = 0;
    if (((u8 *)ball)[99] == 1) {
      Func_02005854(204);
      Func_02005634(ball, gData_02006f38);
      ((u8 *)ball)[99] = goal;
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
    heading = row = col = 0;
    break;
  case 267:
    heading = row = col = 0;
    break;
  case 523:
    heading = row = col = 0;
    break;
  case 10:
  case 266:
  case 522:
  case 778:
    if (((u8 *)ball)[99] == 1) {
      ball->x = x;
      ball->y = 0;
      ball->z = z;
      ball->angle = angle;
      if (x >> 20 == 24 && z >> 20 == 14) {
        ((u8 *)ball)[99] = 7;
      } else {
        Func_02005854(204);
        Func_02005634(ball, gData_02006f38);
        ((u8 *)ball)[99] = 2;
      }
    }
    break;
  case 779:
    heading = row = col = 0;
    break;
  default:
    heading = row = col = 0;
    if (((u8 *)ball)[99] == 1) {
      Func_02005854(132);
      Func_02005634(ball, gData_02006ec0);
      ((u8 *)ball)[99] = 2;
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
