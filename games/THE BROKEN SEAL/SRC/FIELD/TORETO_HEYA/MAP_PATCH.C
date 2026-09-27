/* Exact 96-byte owner, resource_396:020017ec..0200184c, including pool. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Each patch stores flag, enabled, source x/y and destination x/y. */
extern s16 Data_02009ca8[];

/* Copy the one-cell patch of each enabled entry whose flag is set. */
void ToretoHeya_ApplyFlaggedMapPatches(void)
{
    s32 i;
    /* FAKEMATCH: halfword storage delays the size producer until after the
     * sentinel and offset, matching the loop preheader. */
    struct Half { u16 v; } size;

    for (i = 0; Data_02009ca8[i] != -1; i += 6) {
        size.v = 1;
        if (Engine_GameFlagIsSet(Data_02009ca8[i]) && Data_02009ca8[i + 1] != 0)
            Map_CopyCellsTo(Data_02009ca8[i + 2], Data_02009ca8[i + 3],
                Data_02009ca8[i + 4], Data_02009ca8[i + 5], size.v, size.v);
    }
}
