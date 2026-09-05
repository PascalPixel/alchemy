#include "types.h"

/*
 * resource_3b5 owner at 0x020006e8, 62 bytes.
 *
 * Copies the player's two-bit mode into both mode fields of the given actor's
 * record and clears the actor's flag byte at +35.
 *
 * `f80` is a volatile pointer because the reference loads it again for the
 * second store, on a path where nothing can have changed it. Without the
 * qualifier the compiler keeps the first load in a register and the second
 * `ldr r0, [r5, #80]` disappears.
 *
 * The two mode writes are bitfields: the -13 mask stays 32-bit and is shared
 * between them, which explicit `(x & ~12) | bits` arithmetic does not produce.
 */

struct Rec_3b5 {
    u8 pad00[9];
    u8 lo9 : 2;
    u8 mode9 : 2;               /* +9,  bits 2..3 */
    u8 hi9 : 4;
    u8 pad0a[11];
    u8 lo15 : 2;
    u8 mode15 : 2;              /* +21, bits 2..3 */
    u8 hi15 : 4;
};

struct Work_3b5 {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Rec_3b5 *volatile f80;   /* +80, re-read for the second store */
};

struct Work_3b5 *Func_02001470();

void SceneActor_CopyPlayerModeToActor(struct Work_3b5 *work)
{
    s32 bits;

    if (work != 0) {
        bits = Func_02001470(0)->f80->mode9;
        work->f35 = 0;
        work->f80->mode9 = bits;
        work->f80->mode15 = bits;
    }
}
