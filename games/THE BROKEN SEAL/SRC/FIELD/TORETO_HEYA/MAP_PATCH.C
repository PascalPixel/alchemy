/* Exact 96-byte owner, resource_396:020017ec..0200184c, including pool. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Each patch stores flag, enabled, source x/y and destination x/y. */
extern s16 ToretoHeya_MapPatches[];

/* Copy the one-cell patch of each enabled entry whose flag is set. */
void ToretoHeya_ApplyFlaggedMapPatches(void)
{
    s32 i;
    /* FAKEMATCH: halfword storage delays the size producer until after the
     * sentinel and offset, matching the loop preheader. */
    struct Half { u16 v; } size;

    for (i = 0; ToretoHeya_MapPatches[i] != -1; i += 6) {
        size.v = 1;
        if (Engine_GameFlagIsSet(ToretoHeya_MapPatches[i]) && ToretoHeya_MapPatches[i + 1] != 0)
            Map_CopyCellsTo(ToretoHeya_MapPatches[i + 2], ToretoHeya_MapPatches[i + 3],
                ToretoHeya_MapPatches[i + 4], ToretoHeya_MapPatches[i + 5], size.v, size.v);
    }
}
