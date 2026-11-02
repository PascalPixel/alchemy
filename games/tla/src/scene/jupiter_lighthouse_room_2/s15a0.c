#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);

extern s16 gData_02002bda;
extern struct Actor *gData_02002bdc[];

void Func_020015a0(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object) {
    object->action = value;
    {
      s16 *count;

      count = &gData_02002bda;
      gData_02002bdc[*count] = object;
      (*count)++;
    }
  }
}
