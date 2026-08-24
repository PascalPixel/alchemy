#include "types.h"
/*
 * resource_39c owner at 0x02004f90, 60 bytes.
 * Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names.
 */
struct Rec_395 {
    u8 pad00[216];
    u16 tbl[1];                 /* +216 */
};
s32 Func_0200aa18();
s32 Func_0200aa20();
struct Rec_395 *Func_0200aa1c();
void Func_02004f90(s32 key, s32 value)
{
    s32 slot = Func_0200aa18(key);

    if (slot != -1) {
        s32 index = Func_0200aa20(slot, key);

        if (index != -1) {
            Func_0200aa1c(slot)->tbl[index] = value;
        }
    }
}
