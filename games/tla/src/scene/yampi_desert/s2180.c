#include "scene.h"

void Func_02002180(void)
{
    if (*(s16 *)((u8 *)Ram_HeapSlots->event_work + 358) == 0 && gFrameCount % 5 == 0) {
        struct PartyState *party = &gPartyState;

        asm volatile(""); /* FAKEMATCH: ⚓️ loads the party state address before it builds the render mode offset */
        if (party->render_mode != 5 || gInput == 0)
            Func_020041f8(596);
    }
}
