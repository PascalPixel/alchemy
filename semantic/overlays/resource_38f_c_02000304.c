typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_38f owner at 0x02000304, 196 bytes: code 0x02000304-0x020003bb and
 * the three-word literal pool at 0x020003bc-0x020003c7.  The next inventory
 * row starts at 0x020003c8 (`push {r5, r6, r7, lr}` / `mov r7, r8`), so the
 * owner is exactly its advertised span.
 *
 * Epilogue `pop {r5, r6, r7} / pop {r0} / bx r0`: r0 holds the popped return
 * address, so the owner returns nothing.  No argument register is read before
 * the first branch either (r3/r7 are loaded from the pool), so it takes no
 * arguments - the caller's r0-r3 simply reach Func_0808a018 unchanged.
 *
 * Call accounting: 10 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38f 0304` - 9 import veneers and
 * one intra-overlay prologue (Func_020002b4).  Each appears below exactly
 * once, except Func_0808a020 which closes both arms.  The disassembler's own
 * `bl` annotations are wrong in the usual overlay way (a `bl` stores
 * `target_offset - 2`) and must not be used.
 *
 * LINK BASE CONFIRMED at 0x02008000 for this overlay: the pool word 0x02008330
 * at 0x020003c0 is the base of the five-entry jump table that physically sits
 * at file offset 0x330, and its entries 0x02008344/0x0200834a/0x02008350/
 * 0x02008356/0x0200835c are exactly the five case bodies at 0x344/0x34a/0x350/
 * 0x356/0x35c.  The remaining pool word 0x0200ae48 is therefore in-image data
 * at file offset 0x2e48, not a RAM global.
 *
 * The imports are the same scripted-scene family resource_373 and resource_39f
 * use; Func_0808a080(selector) returning the actor record whose +0x55 byte is
 * cleared is confirmed against the byte-exact sibling
 * assets/code/resource_38f_c_020002b4.c, which performs the identical
 * record[0x55] = 0 followed by the same 0x8000/0x4000 scale call and the same
 * scene[0x1c8] = 16 store.
 *
 * UNCERTAINTIES:
 *  - The s16 at scene+0x16c is a scene/step selector; only values 5..9 are
 *    handled, and 5..8 merely choose the (r6, r5) pair handed to
 *    Func_08009178 together with the in-image table Data_0200ae48.  Anything
 *    else falls through with both zero, which is passed on unchanged - that
 *    is the original's behaviour, not an omission.
 *  - Func_08009178's second and third arguments (71/9, 73/17, 80/21, 84/12)
 *    look like an index and a count into Data_0200ae48, but nothing here
 *    proves the shape, so they are left as plain integers.
 */

/* The overlay's scene block, reached through the IWRAM pointer at 0x03001ebc,
 * the same pointer assets/code/resource_38f_c_020002b4.c writes +0x1c8 through. */
#define RESOURCE_38F_SCENE (*(u8 *volatile *)0x03001ebc)

/* In-image data at file offset 0x2e48 (0x0200ae48 - 0x8000). */
extern u8 Data_0200ae48[];

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */
void Func_0808a018();
void Func_0808a020();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0e0();
void Func_0808a248();
void Func_080f9010();
void Func_08009178();
void Func_020002b4(s32);

void Func_02000304(void)
{
    u8 *scene;
    s16 step;
    s32 a;
    s32 b;
    u8 *record;

    scene = RESOURCE_38F_SCENE;

    Func_0808a018();
    Func_080f9010(158);

    step = *(s16 *)(scene + 0x16c);

    a = 0;
    b = 0;
    switch ((s32)step) {
    case 5:
        a = 71;
        b = 9;
        break;
    case 6:
        a = 73;
        b = 17;
        break;
    case 7:
        a = 80;
        b = 21;
        break;
    case 8:
        a = 84;
        b = 12;
        break;
    case 9:
        record = Func_0808a080(0);
        record[0x55] = 0;
        /* 128 << 8 and 128 << 7 in the original: 0.5 and 0.25 in 16.16. */
        Func_0808a090(0, 0x8000, 0x4000);
        Func_0808a0e0(0, 0, 8);
        *(s32 *)(RESOURCE_38F_SCENE + 0x1c8) = 16;
        Func_0808a248(9);
        Func_0808a020();
        return;
    default:
        break;
    }

    Func_08009178(Data_0200ae48, a, b);

    /* Re-read through the r7 copy taken before the first call, not a fresh
     * load of 0x03001ebc. */
    Func_020002b4(*(s16 *)(scene + 0x16c));
    Func_0808a020();
}
