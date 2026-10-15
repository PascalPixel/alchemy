/* Near miss, differ 2 of 96: The Broken Seal's FieldEffect_WatchLeaderDistance
 * with this game's offsets. The native reads the actor's x between the two
 * halves of the centre offset (movs r1, #192; lsls; ldr r0, [r6, #8];
 * adds r1, #168); this finishes the offset first. A split-offset temporary
 * with two barriers orders the rows but indexes the read by the offset
 * (ldrsh r3, [r5, r3]) where the native adds it to the work first. The offset's
 * 0xc00 forced through an empty asm and pinned to r1 orders the rows, but the work is
 * added before the 168 (differ 4; 2026-10-15). */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "iwram_call.h"

void FieldEffect_WatchLeaderDistance(void)
{
    u8 *work;
    struct ObjectRuntime *actor;
    s16 *timer;
    s32 dx;
    s32 dz;
    struct PartyState *party;

    work = Ram_HeapSlots->event_work;
    party = &gPartyState;
    /* FAKEMATCH: ⚓️ loads the party state's address before it builds the field offset (one pair swapped). */
    asm volatile("" : "+l"(party));
    actor = ObjectTable_Get(party->current_owner);
    if (*(s8 *)(work + 0xca5) != 0) {
        timer = (s16 *)(work + 0xca6);
        if (*timer != 0)
            (*timer)--;
    }

    dx = Iwram_MulQ16(*(s16 *)(work + 0xca8) - actor->x / 0x10000, 0xd105);
    dz = *(s16 *)(work + 0xcaa) - (actor->z - actor->y) / 0x10000;
    if (dx * dx + dz * dz >= 3600 || *(s16 *)(work + 0xca6) == 0) {
        u16 *slot = (u16 *)(work + 358);
        u32 request = 0x2090;

        *slot = request;
    }
}
