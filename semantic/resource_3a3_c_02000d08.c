#include "types.h"

/*
 * Resource 3a3 object spawner at 0x02000d08 (80 bytes,
 * 0x02000d08 .. 0x02000d57, of which 0x02000d50 .. 0x02000d57 is the pool).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000d08 and the interworking
 * return `pop {r5, r6} / pop {r0} / bx r0` at 0x02000d4a.  r0 is the popped
 * return address, so the owner returns nothing.
 *
 * All 3 call sites are placed (row reports calls=3): Func_080090c8,
 * Func_02000c0c and Func_08009080, one each.
 *
 * LINK-BASE WITNESS.  The pool word at 0x02000d54 is 0x02008c45.  Under the
 * project's proven 0x02008000 overlay link base that is file offset 0x0c45,
 * i.e. `Func_02000c44 + 1` (the Thumb bit) -- an in-image function pointer,
 * not a RAM address.  It is stored into the spawned object at +0x6c and the
 * object is then handed to Func_08009080, so 0x02000c44 is this overlay's
 * per-frame callback for the object spawned here.  This confirms the 0x8000
 * base for resource_3a3 and names the 0x02000c44 row.
 *
 * `0x03001e40` is read directly as a word (single `ldr r6,[r3]` after the
 * pool load), masked with 3.  Nonzero means the spawn is skipped entirely.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
u8 *Func_080090c8();        /* allocates/spawns an object, 0 on failure */
void Func_08009080();       /* activates the object with a mode */

/* Overlay-local: the state initializer at 0x02000c0c. */
struct Actor02000c0c;
void Func_02000c0c();

extern u32 Data_03001e40;

void Func_02000d08(void)
{
    u32 phase;
    u8 *object;

    phase = Data_03001e40 & 3;
    if (phase != 0) {
        return;
    }

    /* r0=222, r1=0x80<<15, r2=0, r3=0xc8<<17. */
    object = Func_080090c8(222, 0x00400000, 0, 0x01900000);
    if (object == 0) {
        return;
    }

    *(u16 *)(object + 100) = 20;
    /* r6 is still the masked phase word, which is 0 on this path. */
    *(u16 *)(object + 102) = (u16)phase;
    *(s32 *)(object + 104) = 20;

    /* r0 still holds the spawned object here. */
    Func_02000c0c(object);

    /* Func_02000c44 + Thumb bit: the object's per-frame callback. */
    *(u32 *)(object + 108) = 0x02008c45;

    Func_08009080(object, 1);
}
