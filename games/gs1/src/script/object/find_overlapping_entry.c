#include "script_object_entry.h"
#include "global_cells.h"

#define ScriptObject_FindOverlappingEntry Func_0800d98c

s32 Func_0800eba0(s32 *a, s32 arg1, s32 *b, s32 arg3);

struct ScriptObjectEntry *ScriptObject_FindOverlappingEntry(
    struct ScriptObjectEntry *object, s32 *values)
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
        if (Func_0800eba0(entry->values_08, entry->value_20 - 2,
                          values, object->value_20 - 2) >= 0) {
            return entry;
        }
    }
    index += 1;
    flags += 0x70;
    entry++;
    if (index > 0x3F) {
        return NULL;
    }
    goto loop_1;
}
