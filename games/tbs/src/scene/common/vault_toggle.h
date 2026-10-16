#include "types.h"
#include "field_obj.h"
#include "field_event.h"

static void Vault_EnableAlternatePassageCells(void) {
  Engine_GameFlagSet(0x200);
  {
    Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  }
}

static void Vault_RestorePassageCells(void) {
  Engine_GameFlagClear(0x200);
  {
    Map_CopyCellAttributes(23, 23, 4, 2, 23, 26);
  }
}
