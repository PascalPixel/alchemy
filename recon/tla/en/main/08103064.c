/* Earlier trial: score 200, one ble and three reordered instructions
   (str r5, [sp, #0]; ldr r2, [r6]; adds r3, r1, #0).
   EN 2026-10-03: canonical Djinn banks score 840, 37 differing instructions;
   both complete extents are 260 bytes. Loop-load placement and register
   allocation still differ. */
#include "TYPES.H"
#include "OWNER_STATE.H"

/* A set Djinni's entry carries bit 15, spelled as the signed halfword flag. */
#define DJINN_ENTRY_SET (-0x8000)

/*
 * Lists an owner's Djinn as packed halfword entries: element in bits 5-6,
 * index in bits 0-4 and, for set Djinn, bit 15. With element -1 every
 * element is listed and each entry also carries the owner in bits 8-14.
 * Returns the number of entries written.
 */

s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element)
{
    struct OwnerDjinnState *state = Owner_GetState(owner);
    s32 count = 0;
    s32 row;
    s32 bit;
    s32 entry;

    if (element == -1) {
        for (row = 0; row < 4; row++) {
            for (bit = 0; bit < 20; bit++) {
                if (state->flags.banks.active[row] & (1 << bit)) {
                    entry = (row << 5) | bit | DJINN_ENTRY_SET;
                    entry |= owner << 8;
                    out[count++] = entry;
                } else if (state->flags.banks.owned[row] & (1 << bit)) {
                    out[count++] = (row << 5) | bit | (owner << 8);
                }
            }
        }
    } else {
        for (bit = 0; bit < 20; bit++) {
            if (state->flags.banks.active[element] & (1 << bit))
                out[count++] = (element << 5) | bit | DJINN_ENTRY_SET;
            else if (state->flags.banks.owned[element] & (1 << bit))
                out[count++] = (element << 5) | bit;
        }
    }
    return count;
}
