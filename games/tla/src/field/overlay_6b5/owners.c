#include "types.h"
#include "party_state.h"

// Writes up to three active party owners to out, ending the list with 0xff.
void Func_02000538(u16 *out)
{
    struct PartyState *party;
    s32 count = Party_CountActiveOwners();
    s32 i;

    if (count > 3)
        count = 3;
    i = 0;
    /* FAKEMATCH: a goto loop; as a for, while or do loop, loop.c strength-reduces the owner index (differ 24). */
    if (i >= count)
        goto done;
    party = &gPartyState;
next:
    {
        u8 owner = party->active_owners[i];

        if (out != NULL)
            *out++ = owner;
    }
    i++;
    if (i < count)
        goto next;
done:
    if (out != NULL)
        *out = 0xff;
}
