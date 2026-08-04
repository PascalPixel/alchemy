#include "types.h"

extern void Func_02004a58(s32);
extern void Func_020048b0(s32);
extern void Func_02004774(s32);
extern void Func_0200477a(s32);
extern void Func_02004790(s32);
extern void Func_0200487c(s32, s32, s32, s32, s32, s32);
extern void Func_0200488e(s32, s32, s32, s32, s32, s32);
/* Address-taken rather than called. Overlays are LINKED at 0x02008000 and run
 * at 0x02000000, so a pooled function address carries the link-base form: the
 * reference word is 0x02008715, which is this symbol plus the Thumb bit that
 * `.thumb_set` supplies. A called symbol uses the raw run-time annotation; an
 * address-taken one does not. */
extern void Func_02008714();

extern volatile s32 Data_0200c834;
extern volatile s32 Data_0200c838;

void Func_020009b0(void)
{
    Func_02004a58(31);
    Func_020048b0(820);                 /* 205 << 2 */

    if (Data_0200c834 != 0) {
        Data_0200c838 = 0;
    }

    Func_02004774(30);
    Func_0200477a(1);

    /* The task is published as its entry address with the Thumb bit set. The
     * `.thumb_set` alias the exact reconstruction emits for a Thumb symbol already carries
     * bit 0, so adding it again here overshoots by one. */
    Func_02004790((s32) Func_02008714);

    Func_0200487c(58, 28, 7, 1, 58, 13);
    Func_0200488e(57, 11, 1, 1, 58, 11);
}
