#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct CueEvent {
    u8 unk000[358];
    u16 cue;
};

extern u8 gPartyState[];

s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbd48(s32 x, s32 z, s32 layer);
s32 Func_080e4244(struct ObjectRuntime *object);

void Func_080e42d4(void)
{
    struct CueEvent *event = Ram_HeapSlots->eventWork;
    u8 *party = gPartyState;
    struct ObjectRuntime *object = ObjectTable_Get(*(s32 *)(party + 532));
    s32 reason = 0;

    if (*(s8 *)(party + 707) != 0)
        reason = 1;
    if (Func_080dbcd8(object->x, object->z, object->terrainId) == 0)
        reason = 2;
    if (party[530] == 5 && *(s16 *)((u8 *)object + 102) != 0 && object->action == 0) {
        s32 x;
        s32 z;
        s32 back;

        if (Func_080dbd48(object->x, object->z, 2) == 0)
            reason = 3;
        x = object->x & 0xfff00000;
        z = object->z & 0xfff00000;
        back = -0x80000;
        /* FAKEMATCH: ⚓️ loads the back offset before it builds the forward one */
        asm("" : "+l"(back));
        /* FAKEMATCH: and builds the forward offset only after that load */
        asm volatile("");
        if (Func_080dbd48(x + 0x80000, z + back, 2) == 0)
            reason = 4;
    }
    if (reason != 0 && (Func_080e4244(object) & 2) == 0)
        event->cue = 0x208b;
}
