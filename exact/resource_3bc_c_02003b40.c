#include "types.h"

/*
 * resource_3bc owner at 0x02003b40, 64 bytes.
 *
 * Fetches a record and, when it exists, resets its scale pair, clears the busy
 * byte at +91 and hands the record on with the caller's two 16.16 arguments.
 *
 * The second scale is written as `scale / 2` of the first, not as its own
 * literal: the reference derives it with `asrs r3, r3, #1` from the value
 * already in the register, and both `0x10000` and `scale >> 1` build it afresh
 * with `movs / lsls` instead.
 */

struct Rec_3bc {
    u8 pad00[12];
    s32 f12;                    /* +12 */
    u8 pad10[32];
    s32 f48;                    /* +48 */
    s32 f52;                    /* +52 */
    u8 pad38[35];
    u8 f91;                     /* +91 */
};

struct Rec_3bc *Func_020086b0();
void Func_02008464();
void Func_0200844c();
void Func_02008488();

void Func_02003b40(s32 id, s32 x, s32 y)
{
    struct Rec_3bc *rec = Func_020086b0(id);
    s32 scale;

    if (rec != 0) {
        scale = 0x20000;
        rec->f48 = scale;
        rec->f52 = scale / 2;
        rec->f91 = 0;
        Func_02008464(rec);
        Func_0200844c(rec, 5);
        Func_02008488(rec, x << 16, rec->f12, y << 16);
    }
}
