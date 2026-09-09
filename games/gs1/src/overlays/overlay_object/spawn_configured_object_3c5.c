/*
 * Spawns an overlay object and puts it into its configured state --
 * resource_3c5.
 */
#include "types.h"

/*
 * The four callees are declared at the in-overlay entry points that are
 * called directly, not at their veneer addresses.
 */
u8 *Func_020037ae(s32 kind, s32 x, s32 y, s32 z);
void Func_0200381c(u8 *object, s32 mode);
void Func_02003904(u8 *object, s32 mode);
void Func_02003834(u8 *object, s32 mode);

u8 *OverlayObject_SpawnConfiguredObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Func_020037ae(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        /*
         * The mask is a local initialised first and ANDed with the loaded
         * byte afterwards; folding the clear into one expression swaps the
         * two registers.
         */
        u32 masked = ~12;
        masked = masked & owner[9];
        owner[9] = (u8)masked;
        object[85] = 0;
        object[89] = 8;
        Func_0200381c(object, 0);
        Func_02003904(object, 14);
        Func_02003834(object, 1);
        result = object;
    } else {
        result = 0;
    }
    return result;
}
