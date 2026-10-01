/*
 * Draft: ScriptObject_FindOverlappingEntry, ported from its ☀️ twin with
 * ⚓️'s 128-byte objects from their heap slot; the callee is listed as
 * Func_08026f80, ☀️'s Runtime_CheckRadiusOverlap. Score 180: besides that
 * name, the listing opens the frame (sub sp, #4) right after loading the
 * objects, where this C opens it first.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

/* ⚓️'s script objects are 128 bytes; ☀️'s 112. */
struct ScriptObjectEntry {
    void *data;
    u8 unknown_04[4];
    s32 values_08[6];
    u16 value_20;
    u8 unknown_22[0x37];
    u8 flags_59;
    u8 unknown_5a[0x26];
};

s32 Runtime_CheckRadiusOverlap(s32 *a, s32 arg1, s32 *b, s32 arg3);

struct ScriptObjectEntry *ScriptObject_FindOverlappingEntry(
    struct ScriptObjectEntry *object, s32 *values)
{
    s32 tmp;
    s32 index;
    u8 *flags;
    struct ScriptObjectEntry *entry;

    entry = Ram_HeapSlots->script_objects;
    index = 0;
    flags = &entry->flags_59;
loop_1:
    if (entry->data != NULL && (1 & *flags) && entry != object) {
        tmp = index;
        if (Runtime_CheckRadiusOverlap(entry->values_08, entry->value_20 - 2,
                          values, object->value_20 - 2) >= 0) {
            return entry;
        }
    }
    index += 1;
    flags += 0x80;
    entry++;
    if (index > 0x3F) {
        return NULL;
    }
    goto loop_1;
}
