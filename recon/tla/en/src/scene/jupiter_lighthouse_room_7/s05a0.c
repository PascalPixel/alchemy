#include "types.h"
#include "actor.h"

extern s16 gData_02002c64[][2];

s32 Func_020005a0(struct Actor *actor) {
  s32 i;

  for (i = 0; gData_02002c64[i][0] != -1; i++) {
    if (gData_02002c64[i][0] == actor->x >> 20 &&
        gData_02002c64[i][1] == actor->z >> 20)
      return 1;
  }
  return 0;
}
