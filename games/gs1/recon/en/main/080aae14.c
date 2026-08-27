#include "types.h"
#include "owner_state.h"

#define ACTION_MASK 0x3fff
#define FLAG_LOST 0x8000
#define FLAG_GAINED 0x4000

#define OwnerAction_DiffSlots Func_080aae14

s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b, u16 *out,
                           s32 *lost_count, s32 *gained_count)
{
    s32 total = 0;
    s32 lost = 0;
    s32 gained = 0;
    s32 i;

    if (b[0].encoded_action != 0) {
        i = 0;
        do {
            struct OwnerActionSlot *p;
            s32 cnt;

            out[total] = b[i].encoded_action & ACTION_MASK;
            total++;

            cnt = 0;
            if (((b[i].encoded_action ^ a[0].encoded_action) & ACTION_MASK) != 0) {
                p = a;
                do {
                    cnt++;
                    if (cnt > 31) {
                        break;
                    }
                    p++;
                } while (((b[i].encoded_action ^ p->encoded_action) & ACTION_MASK) != 0);
            }

            if (cnt == 32) {
                lost++;
                out[total - 1] |= FLAG_LOST;
            }

            i++;
        } while (i <= 31 && b[i].encoded_action != 0);
    }

    if (a[0].encoded_action != 0) {
        i = 0;
        do {
            struct OwnerActionSlot *p;
            s32 cnt;

            cnt = 0;
            if (((a[i].encoded_action ^ b[0].encoded_action) & ACTION_MASK) != 0) {
                p = b;
                do {
                    cnt++;
                    if (cnt > 31) {
                        break;
                    }
                    p++;
                } while (((a[i].encoded_action ^ p->encoded_action) & ACTION_MASK) != 0);
            }

            if (cnt == 32) {
                gained++;
                out[total] = (a[i].encoded_action & ACTION_MASK) | FLAG_GAINED;
                total++;
            }

            i++;
        } while (i <= 31 && a[i].encoded_action != 0);
    }

    *lost_count = lost;
    *gained_count = gained;
    return total;
}
