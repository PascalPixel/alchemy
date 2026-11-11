#include "types.h"
#include "actor.h"
#include "event_inline.h"

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
  u8 unk160[4];
  s16 statueId;
  u8 unk166[2];
  u16 tileEntry;
};

extern struct SceneState gSceneState;
extern const u8 gData_02005ad4[];
extern const u8 gData_02006ea8[];
void Func_02004288(s32 id, s32 value);
void Func_02005624(struct Actor *object, s32 mode);
s32 Func_02004224(s32 row, s32 column, s32 direction) {
  s32 value = gData_02005ad4[row * 4 + column];

  if (direction == 2 && value == 1)
    value = 3;
  if (direction == 1 && value == 2)
    value = 3;
  if (direction == 4 && value == 6)
    value = 8;
  if (direction == 6 && value == 4)
    value = 8;
  if (direction == 7 && value == 5)
    value = 9;
  if (direction == 5 && value == 7)
    value = 9;
  return value;
}

void Func_02004270(s32 x, s32 z, s32 kind, s32 state) {
  s32 i;

  {
    s32 column = x;

    /* FAKEMATCH(barrier): an empty asm on column that names the cell table, so
     * the table is loaded before x is added; without it, 9 of 10 instructions
     * differ in tla-en. */
    asm("" : "+l"(column) : "l"(gSceneState.cells));
    i = z * 11 + column;
  }
  gSceneState.cells[i].kind = kind;
  gSceneState.cells[i].state = state;
}
