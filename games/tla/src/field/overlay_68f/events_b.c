#include "ov68f.h"

void Func_02000bfc(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
    do {
    } while (0);
    if (party->scene == (s32)&SceneId_Ov68fRoom6) {
        struct ObjectRuntime *object;

        Func_02003170(Data_02003264);
        object = *(struct ObjectRuntime **)((u8 *)work + 0xcbc);
        object->action = ((object->x >> 20) << 8) | ((object->z >> 20) & 0xff);
    } else if (party->scene == (s32)&SceneId_Ov68fRoom7) {
        Func_02003170(Data_0200326e);
    }
}
