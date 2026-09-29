#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The deck's placements, chosen by the story flags the voyage has set. */
extern u8 gFuneKanpanPlacementsFlag93e[];
extern u8 gFuneKanpanPlacementsFlag927[];
extern u8 gFuneKanpanPlacementsFlag928[];
extern u8 gFuneKanpanPlacementsFlag911[];
extern u8 gFuneKanpanPlacements[];

/* The actors placed on the deck. Once flag 0x911 is set the table adjusts
 * three of its entries by flags 0x925 and 0x922 before it is used. */
u8 *FuneKanpan_GetPlacements(void)
{
    s32 v;

    if (GameFlag_IsSet(0x93e))
        return gFuneKanpanPlacementsFlag93e;
    if (GameFlag_IsSet(0x927))
        return gFuneKanpanPlacementsFlag927;
    v = GameFlag_IsSet(0x928);
    if (v != 0)
        return gFuneKanpanPlacementsFlag928;
    if (GameFlag_IsSet(0x911)) {
        if (GameFlag_IsSet(0x925)) {
            gFuneKanpanPlacementsFlag911[0x14E] = v;
            gFuneKanpanPlacementsFlag911[0x1AE] = 2;
            gFuneKanpanPlacementsFlag911[0x1C6] = 2;
        } else if (GameFlag_IsSet(0x922)) {
            gFuneKanpanPlacementsFlag911[0x1AE] = 1;
            gFuneKanpanPlacementsFlag911[0x1C6] = 1;
        }
        return gFuneKanpanPlacementsFlag911;
    }
    return gFuneKanpanPlacements;
}
