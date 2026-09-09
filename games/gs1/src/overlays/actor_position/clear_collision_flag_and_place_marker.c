#include "types.h"

/*
 * resource_3a4 owner at 0x02003460, 34 bytes with no literal pool: clear
 * bit 0 of the scene record's flags byte at +89, then place a marker at the
 * record's x and z at level 255. Both callees are reached through veneers.
 */

u8 *Func_02006ffc();           /* Scene record accessor. */
void Func_02006fbc();          /* Place a marker: kind, x, z, level. */

void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no)
{
    u8 *record;

    record = Func_02006ffc(no);
    record[89] &= 0xfe;

    Func_02006fbc(0, *(s32 *)(record + 8), *(s32 *)(record + 16), 255);
}
