#include "types.h"

/*
 * resource_38a owner at 0x02000030, 22 bytes. Leaf, no frame.
 *
 * Sets the two-bit mode field of the record the caller's +80 pointer names.
 * The bitfield assignment preserves the reference's full-width mask shape.
 */

struct Rec_38a {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;                /* +9, bits 2..3 */
    u8 hi : 4;
};

struct Work_38a {
    u8 pad00[80];
    struct Rec_38a *rec;        /* +80 */
};

void Func_02000030(struct Work_38a *work, s32 value)
{
    work->rec->mode = value;
}
