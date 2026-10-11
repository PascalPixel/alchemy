/* Near miss: differ 2 of 20 (one reordered row): the offset of the held id is begun before the window
 * tiles pointer is loaded. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

struct Ov6aeParty {
    u8 unknown_000[0x26a];
    s16 held;
};

void Func_02004d24(s32 id)
{
    u8 *work = Ram_HeapSlots->window_tiles;
    struct Ov6aeParty *party = (struct Ov6aeParty *)&gPartyState;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
    do {
    } while (0);
    if (party->held == id)
        work[0x138a] = 1;
}
