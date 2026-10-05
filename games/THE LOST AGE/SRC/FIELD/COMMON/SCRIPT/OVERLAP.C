#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "OBJECT_RUNTIME.H"

s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);

/* -1 when another active script object lies within touching distance of
   the position values, else 0. */
s32 ScriptObject_CheckOverlap(struct ObjectRuntime *object, s32 *values)
{
    s32 index;
    u8 *flags;
    struct ObjectRuntime *entry = Ram_HeapSlots->script_objects;

    flags = &entry->collision_flags;
    for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
        if (entry->script != NULL && (1 & *flags) && entry != object) {
            if (Runtime_CheckRadiusOverlap(&entry->x, entry->radius - 2,
                    values, object->radius - 2) >= 0)
                return -1;
        }
    }
    return 0;
}

/* The first other active script object within touching distance of the
   position values, or NULL. */
struct ObjectRuntime *ScriptObject_FindOverlappingEntry(
    struct ObjectRuntime *object, s32 *values)
{
    s32 index;
    u8 *flags;
    struct ObjectRuntime *entry = Ram_HeapSlots->script_objects;

    flags = &entry->collision_flags;
    for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
        if (entry->script != NULL && (1 & *flags) && entry != object) {
            if (Runtime_CheckRadiusOverlap(&entry->x, entry->radius - 2,
                    values, object->radius - 2) >= 0)
                return entry;
        }
    }
    return NULL;
}
