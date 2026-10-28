#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

extern const u8 gData_020056c4[];
extern const u8 gData_02005728[];

struct Actor *Actor_GetById(s32 id);
void Func_02004dac(struct Actor *object, const u8 *script);

s32 Func_020008b4(void) {
  struct Actor *object = Actor_GetById(9);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  if (leader->z >> 19 <= 45 && leader->x >> 19 <= 57)
    Func_02004dac(object, gData_02005728);
  else
    Func_02004dac(object, gData_020056c4);
  return 0;
}
