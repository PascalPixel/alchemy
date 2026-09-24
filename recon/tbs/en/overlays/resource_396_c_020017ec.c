/* NONMATCHING: 96 bytes, candidate 96, 3 differing halfwords (2026-09-24).
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * the preheader order: the reference sets the -1 (r8), the offset 0 (r5) and
 * then size 1 (r7); here the hoisted size comes first. The loop dump shows
 * size as the first movable; for, while, goto and condition placements of
 * size did not move it after the strength-reduced offset. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Map patches, six halfwords each: story flag, enabled, source x and y,
 * destination x and y; the list ends at flag -1. */
extern s16 Data_02009ca8[];

/* Copy the one-cell patch of each enabled entry whose flag is set. */
void Local_020017ec(void)
{
    s32 i;
    s32 size;

    for (i = 0; Data_02009ca8[i] != -1; i += 6) {
        size = 1;
        if (Engine_GameFlagIsSet(Data_02009ca8[i]) && Data_02009ca8[i + 1] != 0)
            Engine_MapCopyCellsTo(Data_02009ca8[i + 2], Data_02009ca8[i + 3],
                Data_02009ca8[i + 4], Data_02009ca8[i + 5], size, size);
    }
}
