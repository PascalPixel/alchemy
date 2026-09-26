/* NONMATCHING: 272 bytes, 123 differing halfwords, 76 aligned edits.
 * Three structural hypotheses exhausted. Indexed active rows and the
 * linked 0xffff8000 mask retain three spills and available-offset induction. */
#include "TYPES.H"
#include "OWNER_STATE.H"

extern u8 Value_ffff8000;

u32 Unnamed_080ac8fc(u16 *out, u32 id, s32 row_select)
{
    struct OwnerDjinnState *work =
        (struct OwnerDjinnState *)Runtime_GetObject(id);
    u32 cnt = 0;
    s32 bit;
    s32 row;

    if (row_select == -1) {
        u32 id_shifted = id << 8;
        u32 *tbl = work->active;
        for (row = 0; row <= 3; row++) {
            u32 active = tbl[row];
            for (bit = 0; bit <= 19; bit++) {
                u32 mask = 1u << bit;
                u32 packed;
                if (active & mask) {
                    packed = (row << 5) | bit | (u32)&Value_ffff8000 | id_shifted;
                    out[cnt] = (u16)packed;
                    cnt++;
                } else if (work->available[row] & mask) {
                    packed = (row << 5) | bit | id_shifted;
                    out[cnt] = (u16)packed;
                    cnt++;
                }
            }
        }
    } else {
        u32 active = work->active[row_select];
        s32 row_shifted = row_select << 5;
        for (bit = 0; bit <= 19; bit++) {
            u32 mask = 1u << bit;
            u32 packed;
            if (active & mask) {
                packed = row_shifted | bit | (u32)&Value_ffff8000;
                out[cnt] = (u16)packed;
                cnt++;
            } else if (work->available[row_select] & mask) {
                packed = row_shifted | bit;
                out[cnt] = (u16)packed;
                cnt++;
            }
        }
    }

    return cnt;
}
