#include "TYPES.H"
#include "OBJECT_DISPATCH.H"
#include "ANIMSPR.H"

/* ☀️'s FIELD/COMMON/OBJECT/GROUP_SET_CHILD_VALUE_UNLESS_FIFTEEN.C over ⚓️'s
   object record, whose count and dirty flag sit earlier; ⚓️ adds a setter
   for the first layer alone. */

/* Gives the record's first layer the draw parameter value. */
void Animation_ApplyChildValueToFirst(struct DispatchChild *record, u32 value)
{
    if (record != NULL) {
        record->entries[0]->param = value;
        record->dirty = 1;
    }
}

/* Gives every layer of the record the draw parameter value, except layers
   whose parameter is 15. */
void Animation_ApplyChildValuesToRecord(struct DispatchChild *record, u32 value)
{
    s32 i;

    if (record != NULL) {
        for (i = 0; i < record->count; i++) {
            /* FAKEMATCH: the byte store may alias the count, forcing its reload. */
            u8 *layer = (u8 *)record->entries[i];

            if (layer[5] != 15)
                layer[5] = value;
        }
        record->dirty = 1;
    }
}
