typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 386 cutscene beat at 0x0200011c ("dialogue + camera + reaction").
 *
 * Complete owner: `push {lr}` at 0x0200011c, interworking return
 * `pop {r0} / bx r0` at 0x020001b2.  A control-flow walk reaches every
 * halfword up to 0x020001b4; 0x020001b6-0x020001c3 is never reached as an
 * instruction and is the owner's literal pool (an alignment halfword plus the
 * three words 0x00001cd4, 0x03001ebc, 0x00000868).  0x020001c4 is the next
 * owner, which already has a byte-exact source.
 *
 * Signature.  `pop {r0} / bx r0` puts the saved return address in r0, so
 * nothing is returned: this owner is `void` (HANDOVER section 0, interworking
 * epilogue rule).  It takes no arguments — r0 is written before every use.
 *
 * Link base 0x02008000, proven for this overlay several times over.  The
 * exported-entry veneer table at file offset 0 holds the words 0x020084e5,
 * 0x0200806d, 0x02008079, 0x02008081, 0x020084dd and 0x02008075; each is odd
 * and resolves under `offset = value - 0x8000` to a real prologue in this
 * image (0x4e4, 0x6c, 0x78, 0x80, 0x4dc, 0x74) with the Thumb bit set.  This
 * owner is itself named that way: 0x0200811d appears in the overlay's handler
 * pool and is `Func_0200011c + 1`.  Sharpest single witness: the sibling at
 * 0x020004e4 passes 0x02008031 to the task installer Func_080000d0, and
 * 0x02000030 is a banked byte-exact source in assets/code.
 *
 * Import naming.  Every `bl` below is resolved with the overlay rule
 * `true_target_offset = stored_displacement + 2`; the targets land in this
 * overlay's eight-byte import veneer band at 0x020005dc-0x020006dc, and each
 * import is named for the main-image address in the veneer's trailing word.
 * The disassembler's own annotations are wrong for every one of them.
 */

/* Old-style declarations: this overlay reaches several of these imports with
 * different argument counts at different sites, so no arity is committed. */
void Func_0808a010();  /* veneer 0x634 */
void Func_0808a018();  /* veneer 0x63c — scene begin */
void Func_0808a020();  /* veneer 0x644 — scene end */
s32  Func_0808a070();  /* veneer 0x65c — queried as a boolean */
void Func_0808a100();  /* veneer 0x674 */
void Func_0808a110();  /* veneer 0x67c */
void Func_0808a150();  /* veneer 0x68c */
void Func_0808a170();  /* veneer 0x694 — message */
void Func_0808a178();  /* veneer 0x69c */
void Func_0808a188();  /* veneer 0x6ac */
void Func_0808a1e8();  /* veneer 0x6bc */
void Func_080770c8();  /* veneer 0x61c — event-flag set */

/* Pointer CELL, not the workspace: the code loads the word at 0x03001ebc and
 * then indexes off the value.  See HANDOVER, `Data_03001ebc`. */
#define WORKSPACE (*(u8 **)0x03001ebc)

void Func_0200011c(void)
{
    Func_0808a018();
    Func_0808a170(0x1cd4);
    Func_0808a150(16, 0, 2);
    Func_0808a100(16, 1);
    Func_0808a188(16, 0, 20);
    Func_0808a110(16, 4);
    Func_0808a010(20);
    Func_0808a188(16, 0, 20);
    /* r1 is built as 129 << 1. */
    Func_0808a1e8(16, 0x102, 60);
    Func_0808a188(16, 0, 30);
    Func_0808a178(16, 0);

    /* Skip-beat counter: the u16 at workspace + 472 (built as 236 << 1) is
     * bumped on the arm that takes no scene time.  See HANDOVER. */
    if (Func_0808a070(0, 0) != 0) {
        ++*(u16 *)(WORKSPACE + 472);
    }

    Func_0808a188(16, 0, 20);
    /* r0 is built as 192 << 2. */
    Func_080770c8(0x300);
    Func_080770c8(0x868);
    Func_0808a020();
}
