// Vault, before and after the thieves are caught: touch triggers 1 and 2 swap
// the collision of the 4x2 cells at cell (23, 26) between two versions kept
// elsewhere in the map. FLAG_SCENE_TEMP_00 remembers the alternate version,
// which each scene's entry state copies in again.
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "flags.h"

// Touch trigger 1: use the alternate collision, copied from cell (55, 26).
static void Vault_EnableAlternatePassageCells(void) {
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
  Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
}

// Touch trigger 2: restore the usual collision, copied from cell (23, 23).
static void Vault_RestorePassageCells(void) {
  Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
  Map_CopyCellAttributes(23, 23, 4, 2, 23, 26);
}
