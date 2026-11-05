// Draft, rewritten as C from the listing's control flow (tla-en, 5 November 2026).
#include "types.h"
#include "actor.h"

struct CellEntry {
  s16 x;
  s16 z;
};

extern struct CellEntry gData_02002c64[];

s32 Func_020005a0(struct Actor *actor) {
  struct CellEntry *table = gData_02002c64;
  s32 i;

  for (i = 0; gData_02002c64[i].x != -1; i++) {
    if (table[i].x == actor->x >> 20 && table[i].z == actor->z >> 20)
      return 1;
  }
  return 0;
}
