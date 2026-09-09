/*
 * Spawns an overlay object and puts it into its configured state --
 * resource_39f.
 */
#include "types.h"

/*
 * The four callees are declared at the in-overlay entry points that are
 * called directly, not at their veneer addresses.
 */
u8 *Func_0200373a(s32 kind, s32 x, s32 y, s32 z);
void Func_02003788(u8 *obj, s32 mode);
void Func_02003860(u8 *obj, s32 mode);
void Func_020037a0(u8 *obj, s32 mode);

u8 *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_0200373a(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        /*
         * The mask is a local initialised first and ANDed with the loaded
         * byte afterwards; folding the clear into one expression swaps the
         * two registers.
         */
        u32 mask = ~12;
        mask = mask & rec[9];
        rec[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Func_02003788(obj, 0);
        Func_02003860(obj, 14);
        Func_020037a0(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}
