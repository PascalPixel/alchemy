// Draft, differ 39 of 90: Func_02001e08's twin for the cell west of the leader:
// pushes the object found there, copies its cells and sets flag 0x8db when it
// stops at (35, 38). Left: the native copies the leader into r1 and starts the
// position argument in r0 before the stores, and keeps 99 in r6 and 38 in r8;
// column as a local set before the search is the closest form (plain
// constants 45).
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"

extern s16 gData_02003de8[];

struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Object_FindInCell(s32 *position);
void GameFlag_SetBit(s32 flag);
void Func_0200074c(struct ObjectRuntime *object);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);
void Func_02003aa4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020003b4(void);
void Func_020006cc(s16 *ids);

void Func_02001edc(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  s32 position[3];
  struct ObjectRuntime *object;
  s32 column;

  position[0] = leader->x - 0x100000;
  position[1] = leader->y;
  position[2] = leader->z;
  column = 99;
  object = Object_FindInCell(position);
  Func_0200074c(object);
  Func_020005cc(object->terrainId, object->x >> 20, object->z >> 20,
                object->terrainHeight >> 19);
  Func_02003aa4(94, 44, 5, 1, column, 38);
  Func_020003b4();
  object->terrainId = 0;
  Func_02003aa4(column, 42, 5, 1, column, 38);
  Func_020005cc(object->terrainId, object->x >> 20, object->z >> 20,
                (object->terrainHeight >> 19) + 4);
  Func_020006cc(gData_02003de8);
  position[0] = object->x >> 20;
  position[2] = object->z >> 20;
  if (position[0] == 35 && position[2] == 38)
    GameFlag_SetBit(0x8db);
}
