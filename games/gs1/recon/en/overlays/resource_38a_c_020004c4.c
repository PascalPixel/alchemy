#include "types.h"

/*
 * resource_38a owner at 0x020004c4, 16 bytes. Leaf, no frame.
 *
 * Steps the halfword at +30 of the record the caller's +80 pointer names down
 * by a fixed 2048. The subtrahend is wider than a Thumb add immediate, so it
 * is the literal pool word 0xfffff800 at 0x020004d0 and the add is
 * register-to-register.
 *
 * Written as a subtraction on the halfword rather than `+= -2048`: the latter
 * lets the compiler narrow the constant to 0xf800 and build it with
 * `movs / lsls`, which is two bytes shorter than the reference and is not what
 * the ROM does.
 */

struct Rec_38a {
    u8 pad00[30];
    u16 f30;                    /* +30 */
};

struct Work_38a {
    u8 pad00[80];
    struct Rec_38a *rec;        /* +80 */
};

void Func_020004c4(struct Work_38a *work)
{
    work->rec->f30 -= 2048;
}
