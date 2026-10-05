/* Near miss, differ 4 (Script_FindFreeObject, the third, matches): the
   native starts each slot search's index as a copy of the zeroed result
   (adds r1, r0, #0) where this draft builds a second zero. Initialiser
   order, a comma initialiser, a chained assignment, an unsigned index and
   a peeled first test did not reproduce it. The script work in heap slot
   41 is described here as far as these three read it. */
#include "TYPES.H"
#include "RAM_BUFFER.H"

struct ScriptSlot {
    s16 id;
    u8 unknown_02[0x16];
    s32 owner;
};

struct ScriptObject {
    u8 unknown_00[0x12];
    s8 active;
    u8 unknown_13[0xd];
};

struct ScriptWork {
    u32 unknown_00;
    struct ScriptSlot slots[8];
    struct ScriptObject objects[128];
};

struct ScriptSlot *Script_FindFreeSlot(void)
{
    struct ScriptWork *work = Ram_HeapSlots->unknown_84[8];
    struct ScriptSlot *found = 0;
    s32 i;

    for (i = 0; i <= 7; i++) {
        if (work->slots[i].owner == 0) {
            found = &work->slots[i];
            break;
        }
    }
    return found;
}

struct ScriptSlot *Script_FindSlot(s32 id)
{
    struct ScriptWork *work = Ram_HeapSlots->unknown_84[8];
    struct ScriptSlot *found = 0;
    s32 i;

    for (i = 0; i <= 7; i++) {
        if (work->slots[i].id == id) {
            found = &work->slots[i];
            break;
        }
    }
    return found;
}

struct ScriptObject *Script_FindFreeObject(void)
{
    struct ScriptWork *work = Ram_HeapSlots->unknown_84[8];
    struct ScriptObject *object = work->objects;
    s32 i;

    for (i = 0; i <= 127; i++, object++) {
        if (object->active == 0)
            return object;
    }
    return 0;
}
