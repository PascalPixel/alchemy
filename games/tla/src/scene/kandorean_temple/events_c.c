#include "local.h"

void Func_0200063c(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);

  ObjectDispatch_SetSingleChildFlags(Object_GetById(gAdventure.currentOwner),
                                     0);
  Func_02000668(leader, 0);
}
