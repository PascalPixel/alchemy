#include "local.h"

void Func_020003b0(void) {
  Object_GetById(gPartyState.currentOwner)->terrainId = 2;
}

void Func_020003cc(void) {
  Object_GetById(gPartyState.currentOwner)->terrainId = 0;
}
