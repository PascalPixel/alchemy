#include "palette_scene.h"
/*
 * resource_395 owner at 0x0200172c, 60 bytes.
 * Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names.
 */
struct PaletteSceneRecord {
    u8 pad00[216];
    u16 values[1];                 /* +216 */
};
s32 Func_020030b0();
s32 Func_020030b8();
struct PaletteSceneRecord *Func_020030bc();
void PaletteScene_SetRecordValue(s32 key, s32 value)
{
    s32 slot = Func_020030b0(key);

    if (slot != -1) {
        s32 index = Func_020030b8(slot, key);

        if (index != -1) {
            Func_020030bc(slot)->values[index] = value;
        }
    }
}
