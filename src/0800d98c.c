#include "script_object_entry.h"

s32 Func_0800eba0(s32 *a, s32 arg1, s32 *b, s32 arg3);

struct ScriptObjectEntry *Func_0800d98c(
    struct ScriptObjectEntry *object, s32 *values)
{
    s32 index_copy;
    s32 index;
    u8 *flags;
    struct ScriptObjectEntry *entry;

    entry = *(struct ScriptObjectEntry **)0x03001E64;
    index = 0;
    flags = &entry->flags_59;
loop_1:
    if (entry->data != NULL && (1 & *flags) && entry != object) {
        index_copy = index;
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
