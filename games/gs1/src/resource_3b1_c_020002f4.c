#include "types.h"
/*
 * resource_3b1 owner at 0x020002f4, 76 bytes.
 * Puts an actor into its ready state: stage byte at +89, two mode fields on the
 * linked record, and the flag byte at +35 rewritten. Returns 1.
 *
 * The two mode writes are bitfields (the -13 mask stays 32-bit and is shared
 * between them); the +35 write is ordinary byte arithmetic, and its 0xfe mask
 * is narrowed to a byte, which is why the two are spelled differently.
 */
struct Rec_3b1 {
    u8 pad00[9];
    u8 lo9 : 2;
    u8 mode9 : 2;               /* +9,  bits 2..3 */
    u8 hi9 : 4;
    u8 pad0a[11];
    u8 lo15 : 2;
    u8 mode15 : 2;              /* +21, bits 2..3 */
    u8 hi15 : 4;
};
struct Work_3b1 {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Rec_3b1 *f80;        /* +80 */
    u8 pad51[5];
    u8 f89;                     /* +89 */
};
void Func_02006786();
void Func_020068d6();
s32 Func_020002f4(struct Work_3b1 *work)
{
    struct Rec_3b1 *rec = work->f80;

    work->f89 = 8;
    Func_02006786(work, 0);
    rec->mode9 = 1;
    rec->mode15 = 1;
    work->f35 = (work->f35 & ~1) | 2;
    Func_020068d6(work, 15);
    return 1;
}
