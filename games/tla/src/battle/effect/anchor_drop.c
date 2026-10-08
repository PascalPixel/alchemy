#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct AnchorWork {
    u8 unknown_0000[0x1930];
    struct ObjectRuntime *anchor;
};

struct ModeEvent {
    u8 unknown_000[352];
    u16 cue;
};

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);

void Func_080e70f8(s32 id)
{
    struct ModeEvent *event = Ram_HeapSlots->event_work;
    struct ObjectRuntime **slot;
    struct ObjectRuntime *anchor;
    struct ObjectRuntime *object;

    /* FAKEMATCH: ⚓️ loads the event work before it builds the anchor offset */
    asm volatile("");
    slot = &((struct AnchorWork *)Ram_HeapSlots->battle_fx_work[0])->anchor;
    {
        /* FAKEMATCH: ⚓️ keeps the id in r0 until it has the anchor's address */
        register s32 incoming asm("r0") = id;

        /* FAKEMATCH: and copies it only then */
        asm("" : "+l"(incoming) : "l"(slot));
        id = incoming;
    }
    anchor = *slot;
    object = Object_GetById(id);

    if (anchor != NULL) {
        object->x = anchor->x;
        object->y = anchor->y;
        object->z = anchor->z;
    }
    if (*((s8 *)event + 394) == 3) {
        object->motion_flags = 2;
        object->terrain_height = 0;
    } else {
        object->motion_flags = 3;
    }
    object->gravity = 0x170a;
    WaitFrames(50);
    event->cue = id | 0x1000;
}
