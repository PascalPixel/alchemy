#include "local.h"

void Func_020025a8(s32 id) {
  struct AquaRockWork *work = &gData_020034f0;

  if (id == 0) {
    work->object = 0;
  } else {
    work->object = Object_GetById(id);
    if (id == 8)
      work->direction = -1;
    else
      work->direction = 1;
  }
}
