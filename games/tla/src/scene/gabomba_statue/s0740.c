#include "types.h"
#include "actor.h"

extern s32 gData_02005af0;

struct Actor *Actor_GetById(s32 id);
s32 Func_0200584c(void);
struct Actor *Func_020005b4(struct Actor *origin, s32 id);

void Func_02000740(void) {
  s32 step = gData_02005af0;

  if (step != 0) {
    s32 turn = step * 1092;
    s32 delta = -turn;
    struct Actor *leader = Actor_GetById(Func_0200584c());
    struct Actor *near = Func_020005b4(leader, 358);

    if (near != NULL) {
      u16 angle = near->angle;

      /* FAKEMATCH: the native loads the neighbour's angle before it builds
       * 0x8000; the plain test builds the constant first (one reordered pair,
       * differ 2). */
      asm volatile("");
      if (angle == 0x8000)
        delta = turn;
    }
    leader->angle += delta;
  }
}
