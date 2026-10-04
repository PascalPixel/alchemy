/* Matches with one barrier (differ 0); waits for its neighbours 080228e0 and 0802296c to join RESOURCE3. */
#include "TYPES.H"
#include "ANIMSPR.H"
#include "METADATA_LOOKUP.H"
#include "RAM_BUFFER.H"

enum { RESOURCE_SLOT_COUNT = 8 };
s32 Animation_LookupValueByKey(s32 key)
{
    struct ObjectSystemWork *work = Ram_HeapSlots->object_system;
    u32 no;
    struct ResourceSlot *p;

    no = 0;
    /* FAKEMATCH: clear the counter before taking the slots' address. */
    asm("");
    for (p = work->slots; no < RESOURCE_SLOT_COUNT; no++, p++) {
        if (p->tag == key)
            return (s32)p->buffer;
    }
    return 0;
}
