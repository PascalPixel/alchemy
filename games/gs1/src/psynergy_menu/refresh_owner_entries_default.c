#include "types.h"
#include "scene.h"
#include "abi/psynergy_menu/refresh_owner_entries_default.h"

void PsynergyMenu_RefreshOwnerEntriesDefault(void)
{
  int val;
  val = 0x28;
  Sys_Place(0x6C, val, 8);
}
