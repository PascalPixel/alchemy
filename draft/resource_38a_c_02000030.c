#include "types.h"

/*
 * resource_38a owner at 0x02000030, 22 bytes. Leaf, no frame.
 *
 * Inserts the low two bits of the argument into bits 2..3 of the byte at +9 of
 * the record the caller's +80 pointer names. The mask ~12 is built as
 * `movs r3,#13 / negs r3,r3`, which is this compiler's spelling of -13.
 */

struct Rec_38a {
    u8 pad00[9];
    u8 f9;                      /* +9 */
};

struct Work_38a {
    u8 pad00[80];
    struct Rec_38a *rec;        /* +80 */
};

void Func_02000030(struct Work_38a *work, s32 value)
{
    struct Rec_38a *rec = work->rec;

    rec->f9 = (rec->f9 & ~12) | ((value & 3) << 2);
}
