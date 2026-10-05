#include "TYPES.H"
#include "ANIMSPR.H"
#include "METADATA_LOOKUP.H"
#include "RAM_BUFFER.H"

enum { RESOURCE_SLOT_COUNT = 8 };

/* The buffer of the loaded resource slot tagged key, or 0. */
s32 Animation_LookupValueByKey(s32 key)
{
    struct ObjectSystemWork *work = Ram_HeapSlots->object_system;
    u32 no;
    struct ResourceSlot *p;

    no = 0;
    for (p = work->slots; no < RESOURCE_SLOT_COUNT; no++, p++) {
        if (p->tag == key)
            return (s32)p->buffer;
    }
    return 0;
}
