#include "EDITION.H"
#include "TYPES.H"
#include "OWNER_STATE.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

/* A set Djinni's entry carries bit 15, spelled as the signed halfword flag. */
#define DJINN_ENTRY_SET (-0x8000)

extern u8 Data_03001f2c[];
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
s32 DjinnMenu_DrawStatPreview(s32, s32, s32, u8, s32, s32, s32, s32, s32);

/*
 * Lists an owner's Djinn as packed halfword entries: element in bits 5-6,
 * index in bits 0-4 and, for set Djinn, bit 15. With element -1 every
 * element is listed and each entry also carries the owner in bits 8-14.
 * Returns the number of entries written.
 */
s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element)
{
    struct OwnerDjinnState *state = Owner_GetStateFar(owner);
    s32 count = 0;
    s32 row;
    s32 bit;
    s32 entry;

    if (element == -1) {
        for (row = 0; row < 4; row++) {
            for (bit = 0; bit < 20; bit++) {
                if (state->active[row] & (1 << bit)) {
                    entry = (row << 5) | bit | DJINN_ENTRY_SET;
                    entry |= owner << 8;
                    out[count++] = entry;
                } else if (state->available[row] & (1 << bit)) {
                    out[count++] = (row << 5) | bit | (owner << 8);
                }
            }
        }
    } else {
        for (bit = 0; bit < 20; bit++) {
            if (state->active[element] & (1 << bit))
                out[count++] = (element << 5) | bit | DJINN_ENTRY_SET;
            else if (state->available[element] & (1 << bit))
                out[count++] = (element << 5) | bit;
        }
    }
    return count;
}

#if EDITION_INTERNATIONAL
#define ALT_PARAM 2
#else
#define ALT_PARAM 4
#endif

s32 Menu_RunPairedEntryAction(s32 mode, s32 param)
{
    s32 sp14;
    void *state;

    state = *(void **)((u32)&Data_03001f2c);
    if (mode == 0) {
        sp14 = mode;
        DjinnMenu_DrawStatPreview(FIELD_AT_OFFSET(state, s32 *, 0x34), 0, 0, FIELD_AT_OFFSET(state, u8 *, 0x259), 1, mode, 2, param, 1);
        DjinnMenu_DrawStatPreview(FIELD_AT_OFFSET(state, s32 *, 0x24), 0, 0, FIELD_AT_OFFSET(state, u8 *, 0x258), mode, 1, 2, param, mode);
    } else {
        DjinnMenu_DrawStatPreview(FIELD_AT_OFFSET(state, s32 *, 0x34), 0, 0, FIELD_AT_OFFSET(state, u8 *, 0x21B), 1, 0, ALT_PARAM, param, 1);
        DjinnMenu_DrawStatPreview(FIELD_AT_OFFSET(state, s32 *, 0x24), 0, 0, FIELD_AT_OFFSET(state, u8 *, 0x21A), 0, 0, 1, param, 0);
    }
    return 1;
}
