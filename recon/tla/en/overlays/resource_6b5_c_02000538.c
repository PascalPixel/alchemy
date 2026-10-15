/* Draft, differ 0, but only through goto loops, which new C may not add
 * (S2: the goto count may only fall). As a for, while or do loop, loop.c
 * strength-reduces the owner index and it differs by 24. Adopted with the
 * gotos at 12/12 on its lane, it waits here until a loop form without goto
 * is found. */
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
