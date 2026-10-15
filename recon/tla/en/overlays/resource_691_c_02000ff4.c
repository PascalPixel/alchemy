#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct MagmaRockEventWork {
    u8 unk000[0x15a];
    s16 value15a;
    u8 unk15c[2];
    s16 value15e;
    u8 unk160[6];
    s16 value166;
    s16 value168;
    u8 unk16a[0x48];
    u16 value1b2;
};

void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_02001738(void);

void Func_02000ff4(struct ObjectRuntime *object)
{
    struct MagmaRockEventWork *work = Ram_HeapSlots->eventWork;
    s32 mask;
    u16 timer = object->action;

    if (object->action > 528) {
        mask = 2;
        goto blink;
    }
    if (object->action > 400) {
        mask = 16;
    blink:
        if (timer & mask)
            Object_SetPartAttribute(object, 10);
        else
            Object_SetPartAttribute(object, 0);
    } else {
        object->update = Func_02001738;
        object->action = 1;
    }
    if (work->value166 == 0 && work->value15a == 0 && work->value15e == 0 && work->value168 == 0
        && work->value1b2 != 0)
        object->action--;
}
