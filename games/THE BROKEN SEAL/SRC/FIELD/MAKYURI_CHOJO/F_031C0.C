#include "TYPES.H"
#include "FIELD_EVENT.H"

struct Rec_39d {
    u8 pad00[8];
    s32 x;
    u8 pad0c[4];
    s32 y;
};

/* The scene's record list: 66 slots from event work +20. */
struct RecList_39d {
    u8 pad[20];
    struct Rec_39d *recs[66];
};

/* Append to out the slot of every record inside the band below the caller's
 * y: x cells 4 to 8, and rows 8 to 10 below 64 - y. */
void MakyuriChojo_Func020031c0(s32 *out, s32 y)
{
    struct RecList_39d *list;
    u32 slot;

    list = (struct RecList_39d *)gEventWork;
    y = 64 - (y >> 20);
    for (slot = 0; slot <= 65; slot++) {
        struct Rec_39d *rec = list->recs[slot];

        if (rec != 0) {
            s32 cell = (rec->x >> 20) - 4;
            s32 row = rec->y >> 20;

            if ((u32)cell <= 4 && y + 8 <= row && row < y + 11)
                *out++ = slot;
        }
    }
}
