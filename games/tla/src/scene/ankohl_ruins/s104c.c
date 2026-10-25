#include "ankohl_ruins.h"
#include "lib/call.h"

void Func_02002e04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000b78(s32 kind, s32 a, s32 x, s32 y, s32 z, s32 b);

/// When the objects with ids 9 and 10 stand on cells (12, 45) and (14, 45),
/// each sets its flag (0x9d9, 0x9da); once both are set, shakes the screen,
/// copies the cells of rows 2 to 6 and 13 in two columns and has Func_02000b78
/// place five objects between them, as Func_02000e60 does for object 8.
void Func_02000c24(s32 unused, s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 toColumn;
  s32 lowRow;
  s32 highRow;
  s32 rise;
  s32 kind;

  if (id == 9 && column == 12 && row == 45) {
    Call1((void (*)())GameFlag_SetBit, 0x9d9);
    Call2((void (*)())Func_02000b30, 9, 181);
  }
  if (id == 10 && column == 14 && row == 45) {
    Call1((void (*)())GameFlag_SetBit, 0x9da);
    Call2((void (*)())Func_02000b30, 10, 181);
  }
  if (!GameFlag_Test(0x9da) || !GameFlag_Test(0x9d9))
    return;
  Func_02002e44();
  Call1((void (*)())Func_02002edc, 0);
  Call3((void (*)())Func_02002e24, 0x20000, 0x20000, 0x10000);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xd80000, -1, 0x3020000, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call1((void (*)())Func_02002eec, 202);
  toColumn = 12;
  highRow = 111;
  Call6((void (*)())Func_02002e04, 12, 91, 3, 1, toColumn, highRow);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call1((void (*)())Func_02002eec, 0x23f);
  lowRow = 47;
  Call6((void (*)())Func_02002df4, 16, 27, 3, 2, toColumn, lowRow);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 2, toColumn, highRow);
  rise = -40;
  // The last argument is also kept for the later calls, as the native keeps
  // the 3 it stores.
  Func_02000b78(3, 15, 200, rise, 0x2e8, kind = 3);
  Call6((void (*)())Func_02002df4, 16, 27, 3, 3, toColumn, lowRow);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 3, toColumn, highRow);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x2f8, kind);
  Call6((void (*)())Func_02002df4, 16, 27, 3, 4, toColumn, lowRow);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 4, toColumn, highRow);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x308, kind);
  Call6((void (*)())Func_02002df4, 16, 27, 3, 5, toColumn, lowRow);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 5, toColumn, highRow);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x318, kind);
  Call6((void (*)())Func_02002df4, 16, 27, 3, 6, toColumn, lowRow);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 6, toColumn, highRow);
  Call3((void (*)())Func_02002e24, -1, -1, 0xe666);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x328, kind);
  Call6((void (*)())Func_02002df4, 16, 27, 3, 13, toColumn, lowRow);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 13, toColumn, highRow);
  Func_02002e4c();
}

/// When the object with id 8 stands on cell (12, 5): sets flag 0x9dc, shakes
/// the screen, copies the cells of rows 2 to 6 and 16 in two columns and has
/// Func_02000b78 place four objects between them.
void Func_02000e60(s32 unused, s32 id) {
  struct ObjectRuntime *pillar = Object_GetById(id);
  s32 column = pillar->x >> 20;
  s32 row = pillar->z >> 20;

  if (id != 8 || column != 12 || row != 5)
    return;
  AnkohlRuins_FlagSet(0x9dc);
  Func_02000b30(8, 181);
  Func_02002e44();
  Func_02002edc(0);
  FieldEvent_WaitFrames(15);
  Func_02002eec(202);
  AnkohlRuins_Call2df4(29, 70, 3, 1, 12, 71);
  AnkohlRuins_CamBounds(0xd80000, -1, 0x780000, 1);
  AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
  FieldEvent_WaitFrames(60);
  Func_02002eec(0x23f);
  AnkohlRuins_Call2df4(35, 6, 7, 2, 10, 7);
  AnkohlRuins_Call2df4(35, 70, 7, 2, 10, 71);
  Func_02000b78(6, 10, 200, -40, 124, 3);
  AnkohlRuins_Call2df4(35, 6, 7, 3, 10, 7);
  AnkohlRuins_Call2df4(35, 70, 7, 3, 10, 71);
  Call6((void (*)())Func_02000b78, 3, 10, 184, -40, 140, 5);
  AnkohlRuins_Call2df4(35, 6, 7, 4, 10, 7);
  AnkohlRuins_Call2df4(35, 70, 7, 4, 10, 71);
  Call6((void (*)())Func_02000b78, 3, 10, 184, -40, 156, 5);
  AnkohlRuins_Call2df4(35, 6, 7, 5, 10, 7);
  AnkohlRuins_Call2df4(35, 70, 7, 5, 10, 71);
  Call6((void (*)())Func_02000b78, 3, 10, 184, -40, 172, 5);
  AnkohlRuins_Call2e24(-1, -1, 0xe666);
  AnkohlRuins_Call2df4(35, 6, 7, 6, 10, 7);
  AnkohlRuins_Call2df4(35, 70, 7, 6, 10, 71);
  FieldEvent_WaitFrames(15);
  AnkohlRuins_Call2df4(35, 6, 7, 16, 10, 7);
  AnkohlRuins_Call2df4(35, 70, 7, 16, 10, 71);
  Func_02002e4c();
}

void Func_0200104c(s32 unused, s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (id == 10 && x == 15 && z == 24) {
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    object->y -= 0x20000;
    FieldEvent_WaitFrames(1);
    object->y -= 0x20000;
    FieldEvent_WaitFrames(1);
    object->y -= 0x40000;
    FieldEvent_WaitFrames(1);
    object->y -= 0x80000;
    FieldEvent_WaitFrames(1);
    Object_SetModeById(9, 3);
    Func_02002e6c(10, 0, 0);
    Func_02002eec(188);
    FieldEvent_WaitFrames(45);
    Object_SetModeById(9, 4);
    Func_02002eec(564);
    GameFlag_SetBit(0x9dd);
  }
}
