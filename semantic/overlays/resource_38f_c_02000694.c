typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_38f owner at 0x02000694, 364 bytes: code 0x02000694-0x020007dd,
 * two bytes of alignment at 0x020007de, and the eight-word literal pool at
 * 0x020007e0-0x020007ff.  The next inventory row starts at 0x02000800 with its
 * own `push {lr}` and has a byte-exact source, so the owner is exactly its
 * advertised span.
 *
 * Epilogue `add sp, #8 / pop {r5} / pop {r1} / bx r1`: the popped return
 * address goes to r1, so r0 survives and IS the result.  Every path reaches
 * `movs r0, #0` first, so the owner always returns 0.  No incoming argument
 * register is read, so it takes none.
 *
 * The 8-byte stack frame exists purely to carry the fifth and sixth arguments
 * of Func_080091c0, written as `str rN, [sp, #0]` / `[sp, #4]` before each of
 * its three call sites.  The byte-exact sibling
 * assets/code/resource_38f_c_02002910.c already documents that six-argument
 * shape for the same family of imports.
 *
 * Call accounting: 32 `bl` sites (the inventory's `calls=30` counts only the
 * 30 veneer sites), all resolved with
 * `bun tools/overlay_call_targets.ts resource_38f 0694`.  Nine distinct
 * targets: two intra-overlay prologues, Func_02002910 (byte-exact sibling,
 * void/void) and Func_020008ec, plus seven import veneers - Func_080091c0 x3,
 * Func_080091e0 x5, Func_080770c0 x3, Func_0808a030 x1, Func_0808a080 x5,
 * Func_0808a088 x9, Func_0808a098 x4.  Every site appears below exactly once.
 * The disassembler's own `bl` annotations are wrong in the usual overlay way
 * and were not used.
 *
 * Addresses.  0x0200add8 and 0x0200b2d8 are in-image data under this overlay's
 * confirmed 0x02008000 link base (jump-table witness in
 * semantic/overlays/resource_38f_c_02000304.c) - file offsets 0x2dd8 and
 * 0x32d8.  0x02000240 is NOT: it is below the link base and cannot be an image
 * offset, so it is an EWRAM block - and it is the same block 0x03001ebc points
 * at, which this owner demonstrates directly by reading +0x1c0 through the
 * literal and writing +0x1c0 through the pointer two instructions apart.  The
 * other overlay owners reach the same block's +0x16c and +0x1c8 through
 * 0x03001ebc only.
 *
 * Behaviour: the scene-step dispatcher for steps other than 0x26 and 0x27.
 * Step 0x27 hands off to Func_02002910; step 0x26 just parks the step field at
 * 0x204.  Otherwise it resets actors 23..26, applies the shared table
 * Data_0200add8 to each, and then gates two further blocks on the results of
 * Func_080770c0 for flags 0x845 and 0x843.
 *
 * UNCERTAINTIES:
 *  - The step field at +0x1c0 is read as a signed halfword and written as a
 *    word (0x1c0 + 68 = 0x204 is computed by biasing the offset register, not
 *    loaded).  Both accesses are reproduced as written; whether the field is
 *    really 32-bit is not settled by this owner.
 *  - Func_080770c0(flag) is used three times with two flag values and its
 *    result tested against zero twice one way and once the other.  The 0x843
 *    query is issued twice in a row rather than reused, which is in the
 *    original and is preserved.
 *  - Func_080091c0's six arguments (13, 9, 1, 1, mode, index) are passed as
 *    plain integers; only the last two vary and their meaning is not
 *    reconstructed.
 */

/* The scene block.  Reachable both ways, as this owner proves. */
#define SCENE_BLOCK ((u8 *)0x02000240)
#define SCENE_POINTER (*(u8 *volatile *)0x03001ebc)

/* In-image data under the 0x02008000 link base. */
extern u8 Data_0200add8[];
extern u8 Data_0200b2d8[];

/* Old-style declarations are mandatory in overlay sources. */
s32 Func_0808a080();
void Func_080091e0();
void Func_080091c0();
void Func_0808a098();
void Func_0808a088();
void Func_0808a030();
s32 Func_080770c0();
void Func_02002910(void);
void Func_020008ec();

s32 Func_02000694(void)
{
    s16 step;
    s32 actor;

    step = *(s16 *)(SCENE_BLOCK + 0x1c0);

    if (step == 0x27) {
        Func_02002910();
        return 0;
    }

    if (step == 0x26) {
        *(s32 *)(SCENE_POINTER + 0x1c0) = 0x204;
        return 0;
    }

    /* r0 is carried straight from each query into the following call. */
    Func_080091e0(Func_0808a080(23), 0);
    Func_080091e0(Func_0808a080(24), 0);
    Func_080091e0(Func_0808a080(25), 0);
    Func_080091e0(Func_0808a080(26), 0);

    Func_0808a098(23, Data_0200add8);
    Func_0808a098(24, Data_0200add8);
    Func_0808a098(25, Data_0200add8);
    Func_0808a098(26, Data_0200add8);

    if (Func_080770c0(0x845) == 0) {
        for (actor = 8; actor <= 16; actor++) {
            Func_080091e0(Func_0808a080(actor), 0);
        }
        Func_080091c0(13, 9, 1, 1, 13, 8);
        Func_080091c0(13, 9, 1, 1, 15, 8);
        Func_080091c0(13, 9, 1, 1, 14, 9);
    }

    if (Func_080770c0(0x843) == 0) {
        if (*(s16 *)(SCENE_BLOCK + 0x1c2) == 1) {
            Func_020008ec();
        }
    }

    if (Func_080770c0(0x843) != 0) {
        Func_0808a088(1);
        Func_0808a088(2);
        Func_0808a088(3);
        Func_0808a088(17);
        Func_0808a088(18);
        Func_0808a088(19);
        Func_0808a088(20);
        Func_0808a088(21);
        Func_0808a088(22);
        Func_0808a030(Data_0200b2d8);
    }

    return 0;
}
