/* Near miss: listing 56 instructions, draft 56, common 53, differ 6; the rest differs in instruction order. */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "OBJECT_RUNTIME.H"

struct EventWork {
    u8 unknown_00[20];
    struct ObjectRuntime *objects[64];
};

struct ObjectRuntime *Func_080eaf28(s32 *position, s32 check_y)
{
    struct EventWork *event = Ram_HeapSlots->event_work;
    u32 i;

    for (i = 8; i < 64; i++) {
        struct ObjectRuntime *object = event->objects[i];

        if (check_y) {
            if (position[0] >> 20 == object->x >> 20 && position[1] / 0x10000 == object->y / 0x10000
                && position[2] >> 20 == object->z >> 20)
                return object;
        } else {
            if (position[0] >> 20 == object->x >> 20 && position[2] >> 20 == object->z >> 20)
                return object;
        }
    }
    return NULL;
}
