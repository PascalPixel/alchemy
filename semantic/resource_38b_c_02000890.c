#include "types.h"

/*
 * Resource 38b scene-variant dispatcher at 0x02000890 (96 bytes, 4 distinct
 * callees).
 *
 * Complete owner: `push {lr}` at 0x02000890, epilogue `movs r0, #0 /
 * pop {r1} / bx r1` at 0x020008d0.  The popped register is r1, not r0, so by
 * the HANDOVER section 0 epilogue rule r0 survives and IS the result: this
 * owner returns s32, and it always returns 0.  0x020008d6 is alignment and
 * 0x020008d8-0x020008ef is the literal pool.
 *
 * The scene selector is the cross-overlay idiom Data_02000240[224] — the
 * signed halfword at byte offset 448 — compared against 0x1e, 0x23 and 0x20.
 * The byte-exact sibling assets/code/resource_38b_c_02000088.c tests the
 * *same* word against the *same* three constants, which confirms both the
 * layout and the constants against tracked material.  (That sibling spells the
 * constants `(s32)&Value_0000001e` and so on; that is a literal-pooling
 * device for the byte-exact reconstruction only, so the integers are written directly
 * here.)
 *
 * Link-base witness: the pool word 0x02008ed9 passed to the task installer
 * Func_080000d0 is Func_02000ed8 plus the Thumb bit under the 0x02008000
 * base — an odd in-image pool word, which by the parity rule is a callback
 * entry rather than a data address.  It both proves the base and names
 * 0x02000ed8 as the task this branch installs.
 *
 * Call targets resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --.  Three of the
 * four are the overlay's own prologues (0x020008f0, 0x02000ae0, 0x02000d10),
 * reached as ordinary calls — they are real function starts, not the
 * hidden-context shape.  Only Func_080000d0 goes through a veneer.
 */

/* The overlay's scene workspace pointer CELL. */
extern u8 *Data_03001ebc;

/* Cross-overlay scene-state halfword array; [224] is the scene id. */
extern s16 Data_02000240[];

/* Task installer, main image, reached through the veneer at 0x02001020.
 * The first argument is the Thumb entry of the callback to install. */
extern void Func_0200119c(void);
extern void Func_02001398(void);
extern void Func_020018e2();
extern void Func_020015de(void);

/* This overlay's own scene bodies. */

                                

/* The callback installed for the 0x23 variant, at file offset 0x0ed8. */
extern void Func_02000ed8(void);

s32 Func_02000890(void)
{
    u8 *workspace = Data_03001ebc;
    s16 scene;

    /* Offset 448 and value 256 are separate quantities carried in the same
     * register pair — the displacement/value shape HANDOVER section 0 warns
     * about.  This is a store of 256 to workspace + 448, not to 448 + 256. */
    *(s32 *)(workspace + 448) = 256;

    scene = Data_02000240[224];

    if (scene == 0x1e) {
        Func_0200119c();
    } else if (scene == 0x23) {
        Func_02001398();
        /* 200 << 4 = 3200. */
        Func_020018e2((void *)Func_02000ed8, 3200);
    } else if (scene == 0x20) {
        Func_020015de();
    }

    /* Unrecognised scene ids fall through with no effect. */
    return 0;
}
