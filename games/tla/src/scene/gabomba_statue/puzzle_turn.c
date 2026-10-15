#include "types.h"
#include "object_runtime.h"

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
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004288(s32 id, s32 value);
void Func_02005624(struct ObjectRuntime *object, s32 mode);
u8 Func_02004380(s32 x, s32 z, s32 direction) {
  s32 i;

  {
    s32 column = x;

    /* FAKEMATCH: load the cell table before adding x. */
    asm("" : "+l"(column) : "l"(gSceneState.cells));
    i = z * 11 + column;
  }
  gSceneState.cells[i].kind =
      gData_02006ea8[gSceneState.cells[i].kind * 2 + direction];
  return gSceneState.cells[i].kind;
}

void Func_020043a4(s32 value, s32 row) {
  struct ObjectRuntime *object = Object_GetById(15);

  if (row <= 7)
    object->scaleY += -0x1f00;
  if (value == 1) {
    Func_02004288(gSceneState.statueId, 1);
    Func_02005624(object, 6);
    object->scaleY = 0x10000;
  }
}
