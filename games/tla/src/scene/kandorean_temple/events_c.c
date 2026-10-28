#include "local.h"

void Func_0200063c(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  Func_02000668(leader, 0);
}
