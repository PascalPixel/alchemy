/* Near miss: differ 2 of 27 (one reordered row): the draft builds the saved-byte offset before it loads
 * the party state address. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct Ov6aeParty {
    u8 unknown_000[0x26a];
    s16 held;
};

struct ObjectRuntime *Object_GetById(s32 id);

void Func_02004e80(s32 from, s32 to)
{
    struct ObjectRuntime *object = Object_GetById(from);
    struct ObjectRuntime *target;
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
    do {
    } while (0);
    party->saved_byte = 0;
    ((struct Ov6aeParty *)party)->held = to;
    target = Object_GetById(to);
    target->update = object->update;
    object->update = NULL;
}
