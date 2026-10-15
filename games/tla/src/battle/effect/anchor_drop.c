#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct AnchorWork {
    u8 unk0000[0x1930];
    struct ObjectRuntime *anchor;
};

struct ModeEvent {
    u8 unk000[352];
    u16 cue;
};

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);

void Func_080e70f8(s32 id)
{
    struct ModeEvent *event = Ram_HeapSlots->eventWork;
    s32 incoming = id;
    struct ObjectRuntime **slot;
    struct ObjectRuntime *anchor;
    struct ObjectRuntime *object;

    /* FAKEMATCH: ⚓️ loads the event work before it builds the anchor offset */
    asm volatile("");
    slot = &((struct AnchorWork *)Ram_HeapSlots->battleFxWork[0])->anchor;
    // Passing the cue snapshot to Object_GetById adds a copy and changes two registers.
    /* FAKEMATCH: keep the cue's entry-id copy after the anchor address; removing the constraint swaps two rows. */
    asm("" : "+l"(incoming) : "l"(slot));
    anchor = *slot;
    object = Object_GetById(id);

    if (anchor != NULL) {
        object->x = anchor->x;
        object->y = anchor->y;
        object->z = anchor->z;
    }
    if (*((s8 *)event + 394) == 3) {
        object->motionFlags = 2;
        object->terrainHeight = 0;
    } else {
        object->motionFlags = 3;
    }
    object->gravity = 0x170a;
    WaitFrames(50);
    event->cue = incoming | 0x1000;
}
