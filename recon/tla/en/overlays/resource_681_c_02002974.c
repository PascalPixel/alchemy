// Draft, differ 96 of 75: records which of eleven listed cells persons 10 and
// 11 stand on, as the two halves of the byte flag 0x210.
#include "types.h"
#include "object_runtime.h"

struct CellPoint {
  s16 x;
  s16 z;
};

extern s16 gData_0200530c[][2];

struct ObjectRuntime *Object_GetById(s32 id);
u32 GameFlag_SetByte(u32 flag, u8 value);

void Func_02002974(void) {
  struct ObjectRuntime *object = Object_GetById(10);
  s32 firstX = object->x >> 19;
  s32 firstZ;
  s32 secondX;
  s32 secondZ;
  u8 first = 0;
  u8 second = 0;
  s8 i;

  firstZ = object->z >> 19;
  object = Object_GetById(11);
  secondX = object->x >> 19;
  secondZ = object->z >> 19;
  i = 1;
loop:
  if (firstX == gData_0200530c[i][0] && firstZ == gData_0200530c[i][1])
    first = i;
  if (secondX == gData_0200530c[i][0] && secondZ == gData_0200530c[i][1])
    second = i;
  i++;
  if (i <= 11)
    goto loop;
  GameFlag_SetByte(0x210, (s8)(first | second << 4));
}
