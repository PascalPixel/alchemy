/* Draft, differ 32 of 32 listed (16 common): the native reads pos[0] on every pass, after the object
 * (ldmia r5!, {r0}; ldr r2, [r1]; ldr r3, [r0, #8]); this form moves that read and its shift out of the loop
 * (r6 is saved for it). A goto loop loses the walking pointer as well (differ 30). The other two coordinates
 * and the loop's end already agree. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct EventObjects {
    u8 unknown_00[20];
    struct ObjectRuntime *objects[64];
};

// pos と同じマスにいるオブジェクト（8 番から 63 番まで）を探す
struct ObjectRuntime *Func_02000374(s32 *pos)
{
    struct EventObjects *work = Ram_HeapSlots->event_work;
    u32 i;

    for (i = 8; i < 64; i++) {
        struct ObjectRuntime *object = work->objects[i];

        if (pos[0] >> 20 == object->x >> 20 && pos[1] >> 20 == object->y >> 20
            && pos[2] >> 20 == object->z >> 20)
            return object;
    }
    return NULL;
}
