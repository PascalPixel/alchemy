#include "TYPES.H"
#include "OWNER_STATE.H"

#define ACTION_MASK 0x3fff
#define FLAG_FIRST 0x8000
#define FLAG_SECOND 0x4000

/* Lists all slots in b, marks those absent from a, then appends slots only in a. */
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                          u16 *out, s32 *first_count, s32 *second_count)
{
    s32 i;
    s32 j;
    s32 total;
    s32 first;
    s32 second;

    total = 0;
    first = 0;
    second = 0;
    for (i = 0; i < 32 && b[i].encoded_action != 0; i++) {
        out[total] = b[i].encoded_action & ACTION_MASK;
        total++;
        for (j = 0; j < 32; j++) {
            if (((b[i].encoded_action ^ a[j].encoded_action) & ACTION_MASK) == 0)
                break;
        }
        if (j == 32) {
            first++;
            out[total - 1] |= FLAG_FIRST;
        }
    }
    for (i = 0; i < 32 && a[i].encoded_action != 0; i++) {
        for (j = 0; j < 32; j++) {
            if (((a[i].encoded_action ^ b[j].encoded_action) & ACTION_MASK) == 0)
                break;
        }
        if (j == 32) {
            second++;
            out[total] = (a[i].encoded_action & ACTION_MASK) | FLAG_SECOND;
            total++;
        }
    }
    *first_count = first;
    *second_count = second;
    return total;
}
