#include "TYPES.H"

struct Stop {
    u8 id;
    u8 pos;
    u8 unknown_02[2];
};

struct StopSet {
    u8 unknown_00[4];
    struct Stop stops[3];
};

struct StopRecord {
    u8 unknown_00[16];
};

extern struct StopRecord Data_0200cf2c[];

/* Snap *pos to the nearest used stop of the set and return that stop's record,
 * or return null when every stop is unused. */
struct StopRecord *KuupuappuHeya_SnapToNearestStop(struct StopSet *set, s16 *pos)
{
    struct Stop *stop;
    s32 id;
    s32 best;
    s32 at;
    s32 d;
    u32 i;
    u16 p;

    id = -1;
    at = *pos;
    stop = set->stops;
    best = 0x8000;
    for (i = 0; i <= 2; i++, stop++) {
        p = stop->pos << 8;
        d = (s16)(p - *pos);
        if (d < 0)
            d = -d;
        /* FAKEMATCH: the used-stop test reads the id through a volatile
         * lvalue, so the id is loaded again for the result. */
        if (*(volatile u8 *)&stop->id != 0xff && d < best) {
            best = d;
            id = stop->id;
            at = (s16)p;
        }
    }
    if (id == -1)
        return 0;
    *pos = at;
    return &Data_0200cf2c[id];
}
