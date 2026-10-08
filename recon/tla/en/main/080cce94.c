/* Near miss: listing 21 instructions, draft 21, common 20, differ 2; the rest differs in instruction order. */
#include "types.h"
#include "party_state.h"

void *Func_080ccd78(s32 group, s32 id);

void *Func_080cce94(s32 id)
{
    void *result = Func_080ccd78(0, id);
    struct PartyState *party = &gPartyState;
    s32 value = *(s16 *)party->unknown_26a;

    if (value == id) {
        void *next = Func_080ccd78(7, value);

        if (next != NULL)
            return next;
    }
    return result;
}
