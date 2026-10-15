/* Near miss: differ 2 of 27 (one reordered row): sched2 builds the saved-byte
 * offset (movs r1, #181, priority 74) before it loads the party state address
 * (priority 73); the native loads the address first. Copying the object from
 * a block-local found after the first store puts mov r8, r0 where the native
 * has it, with no device. Tried: an empty asm barrier or a one-pass block
 * before the stores (the copy to r8 then joins the call's return and moves
 * up: differ 2 to 4), an asm input operand on the party pointer, the party
 * pointer pinned to r3, the stores straight through gPartyState. */
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
    struct ObjectRuntime *found = Object_GetById(from);
    struct ObjectRuntime *object;
    struct ObjectRuntime *target;
    struct PartyState *party = &gPartyState;

    party->saved_byte = 0;
    object = found;
    ((struct Ov6aeParty *)party)->held = to;
    target = Object_GetById(to);
    target->update = object->update;
    object->update = NULL;
}
