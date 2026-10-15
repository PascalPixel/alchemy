#include "local.h"

void Func_0200063c(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  Func_02000668(leader, 0);
}
