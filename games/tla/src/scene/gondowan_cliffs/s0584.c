#include "types.h"
#include "object_runtime.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_020011d4();
void Func_020011dc();
void Func_0200124c();
s32 Map_GetTerrainHeight(s32, s32, s32);

void Func_02000584(void) {
  struct ObjectRuntime *object = Object_GetById(12);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Call6(Func_020011d4, 79, 28, 1, 1, 81, 28);
  GameFlag_SetBit(0x201);
}

void Func_020005bc(void) {
  struct ObjectRuntime *object = Object_GetById(13);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  object->terrainHeight = 0x200000;
  object->y = 0x200000;
  Call6(Func_020011d4, 115, 35, 1, 1, 117, 35);
  GameFlag_SetBit(0x202);
}

void Func_02000600(void) { GameFlag_SetBit(0x885); }

void Func_02000610(s32 unused, s32 cell) {
  struct ObjectRuntime *object;

  if (cell == 30) {
    object = Object_GetById(17);
    Call6(Func_020011dc, 124, 1, 1, 1, 119, 43);
    Call3(Func_0200124c, 17, 0x7780000, 0x2d80000);
    object->terrainHeight = object->y =
        Map_GetTerrainHeight(0, object->x, object->z);
    GameFlag_SetBit(0x886);
  }
}
