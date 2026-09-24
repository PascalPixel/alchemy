#include "SCRIPT_OBJECT_ENTRY.H"
#include "GLOBAL_CELLS.H"

s32 Runtime_CheckRadiusOverlap(s32 *a, s32 arg1, s32 *b, s32 arg3);

s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values)
{
    s32 tmp;
    s32 index;
    u8 *flags;
    struct ScriptObjectEntry *entry;

    entry = *(struct ScriptObjectEntry **)ADDR_03001E64;
    index = 0;
    flags = &entry->flags_59;
loop_1:
    if (entry->data != NULL && (1 & *flags) && entry != object) {
        tmp = index;
        if (Runtime_CheckRadiusOverlap(entry->values_08, entry->value_20 - 2,
                          values, object->value_20 - 2) >= 0) {
            return -1;
        }
    }
    index += 1;
    flags += 0x70;
    entry++;
    if (index > 0x3F) {
        return 0;
    }
    goto loop_1;
}
