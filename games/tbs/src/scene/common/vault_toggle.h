/// @file
/// Vault, before and after the thieves are caught: touch triggers 1 and 2 swap
/// the collision of the 4x2 cells at cell (23, 26) between two versions kept
/// elsewhere in the map. FLAG_SCENE_TEMP_00 remembers the alternate version,
/// which each scene restores on a kept-position return, such as after battle.
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "flags.h"

/// Switches the passage to its alternate collision.
/// Cell code 1 reaches it; it sets FLAG_SCENE_TEMP_00, preserving that choice
/// across a kept-position return.
static void Vault_EnableAlternatePassageCells(void) {
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
  Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
}

/// Cell code 2 restores the passage's usual collision and clears
/// FLAG_SCENE_TEMP_00.
static void Vault_RestorePassageCells(void) {
  Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
  Map_CopyCellAttributes(23, 23, 4, 2, 23, 26);
}
