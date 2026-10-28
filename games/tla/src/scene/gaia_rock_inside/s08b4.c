#include "types.h"
#include "object_runtime.h"
#include "adventure.h"

extern const u8 gData_020056c4[];
extern const u8 gData_02005728[];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004dac(struct ObjectRuntime *object, const u8 *script);

s32 Func_020008b4(void) {
  struct ObjectRuntime *object = Object_GetById(9);
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);

  if (leader->z >> 19 <= 45 && leader->x >> 19 <= 57)
    Func_02004dac(object, gData_02005728);
  else
    Func_02004dac(object, gData_020056c4);
  return 0;
}
