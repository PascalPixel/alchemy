#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "OBJECT_RUNTIME.H"


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
