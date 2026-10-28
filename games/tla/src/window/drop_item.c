#include "types.h"

void PartyInventory_RemoveFar(s32 item);

s32 Func_0803f968(s32 unused, s32 item) {
  PartyInventory_RemoveFar(item);
  return 0;
}
