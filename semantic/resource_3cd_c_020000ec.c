#include "types.h"

/*
 * resource_3cd owner at 0x020000ec, 296 bytes: an interactive panel — open a
 * 30x9 window, draw three fixed caption lines plus the current item's icon and
 * count, then loop on the button latch until B closes it.
 *
 * Role known in advance from the in-image scene-script table, with no
 * disassembly: the three-word records at file offset 0x0450 hold
 * `(0xffff000b, 0x020080ed, 0)`, and under this overlay's link base
 * 0x020080ed is file offset 0x00ec plus the Thumb bit — this function.  So it
 * is the selector-0x0b entry, invoked with no arguments.  (Its sibling
 * 0x020084b1 is the selector-0x0c entry at 0x04b0, converted alongside it.)
 *
 * Link base 0x02008000 confirmed for this overlay by three witnesses: the
 * offset-0 export table's odd words 0x02008031 / 0x02008039 / 0x0200803d /
 * 0x02008045 / 0x0200804d / 0x02008099 / 0x0200871d all land on function
 * starts at `word - 0x8000`, 0x0200804d is `Func_0200004c + 1` whose byte-exact
 * source is tracked as `assets/code/resource_3cd_c_0200004c.c`, and the table
 * above names two more tracked starts.  The table also carries even words
 * (0x020088d0 and up) which are in-image DATA, and one of them is read here.
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr} / mov r7,r8 / push {r7}` at
 * 0x020000ec with a 4-byte frame for the fifth arguments; single epilogue at
 * 0x020001ee ending `pop {r0} / bx r0`, so the popped word is the return
 * address and the owner is **void**.  Code runs 0x020000ec..0x02000201; the
 * literal pool is 0x02000204..0x02000213 and 0x02000214 is past the row.  296
 * bytes, matching the inventory row.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3cd
 * 00ec` (an overlay `bl` stores target offset - 2).  22 sites, 12 distinct:
 *   0x0850 Func_08077008 x1   0x07f0 Func_08015010 x1   0x0800 Func_08015078 x3
 *   0x0830 Func_08015270 x2   0x0810 Func_08015090 x1   0x0818 Func_08015098 x1
 *   0x0828 Func_080150b0 x1   0x00c0 (prologue) x2      0x08c8 Func_080f9010 x3
 *   0x07e8 Func_080000c0 x2   0x07f8 Func_08015018 x1   0x0858 Func_08077010 x4
 * The 0x00c0 prologue is the in-overlay helper whose byte-exact source is
 * tracked as `assets/code/resource_3cd_c_020000c0.c` — it walks a 16-entry list
 * and applies `arg` to each member.
 *
 * The Func_08015xxx family is the window/text layer established by the tracked
 * main-image sources: `src/080a153c.c` fixes Func_08015090 and Func_08015098 as
 * `(image, layer, x, y)` and `src/080a14f0.c` fixes Func_080150a8 as
 * `(value, digits, layer, x, y)`, which is the same shape Func_080150b0 is
 * called with here.  The item record's byte at +15 being a count is confirmed
 * independently by the tracked `assets/code/resource_3cd_c_020000a0.c`, which
 * reads `entry[15]` off the same accessor.
 *
 * Closing with `Func_08077010` over slots 0, 1, 3, 2 is the documented
 * non-sequential refresh order — the same odd ordering two independently-read
 * owners of resource_3ce use.  It is deliberate and is NOT sorted here.
 *
 * `redraw` is set by the prologue and by each of the two adjusting branches, so
 * the panel repaints only on the frames where something changed; the `for (;;)`
 * is entered at its test, which is why the listing branches forward over the
 * repaint block on the first pass.
 *
 * Uncertainties: 0x03001c94 is the button-press latch this family spins on (the
 * tracked `assets/code/resource_36f_c_02000054.c` waits on the same word); bits
 * 0/1/2/3 are read here as A / B / Select / Start on the standard GBA key
 * assignment, which fits "A adjusts by 1, Select-or-Start adjusts by 5, B
 * closes" but is not otherwise witnessed.  The pointer handed to Func_08015090
 * is the record Func_08077008 returned, so that accessor's result serves as
 * both a record and an icon source; that is what the code does.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_02000950();
s32 Func_02000902();
void Func_02000922();
void Func_02000930();
void Func_0200093e();
void Func_02000978();
void Func_02000964();
void Func_0200099c();
void Func_02000250();
void Func_02000a5e();
void Func_02000268();
void Func_02000a76();
void Func_02000a88();
void Func_020009f6();
void Func_020009b4();
void Func_020009cc();
void Func_02000a32();
void Func_02000a38();
void Func_02000a3e();
void Func_02000a44();
void Func_020009e6();
                                /* item/party record by id */
                                /* refresh one display slot */
                                /* open a window, return its handle */
                                /* close a window */
                                /* draw a canned caption line */
                                /* draw an image (image, layer, x, y) */
                                /* draw an image (image, layer, x, y) */
                                /* draw a number (value, digits, layer, x, y) */
                                /* clear a window's contents */
                                /* advance the task scheduler */
                                /* play a sound cue */

                                /* in-overlay: apply a delta to every member */

/* RAM: the shared work area, and the newly-pressed button latch. */
extern u8 Data_02000240[];

/* In-image: the icon drawn beside the count. */
extern u8 Data_020088d0[];

void Func_020000ec(void)
{
    u8 *record;
    s32 window;
    s32 redraw;

    record = Func_02000950(*(s32 *)(Data_02000240 + 500));
    window = Func_02000902(0, 0, 30, 9, 2);

    Func_02000922(0x0c20, window, 0, 0);
    Func_02000930(0x0c21, window, 0, 16);
    Func_0200093e(0x0c22, window, 0, 32);

    redraw = 1;

loop:
    {
        u32 pressed;

        if (redraw != 0) {
            Func_02000978(window);
            Func_02000964(record, window, 0, 48);
            Func_02000978(Data_020088d0, window, 48, 48);
            Func_0200099c(record[15], 0, window, 72, 48);
            redraw = 0;
        }

        pressed = *(u32 *)0x03001c94;

        if ((pressed & 8) != 0 || (pressed & 4) != 0) {
            Func_02000250(5);
            Func_02000a5e(93);
            redraw = 1;
        }

        if ((pressed & 1) != 0) {
            Func_02000268(1);
            Func_02000a76(91);
            redraw = 1;
        }

        if ((pressed & 2) == 0) {
            goto scheduler;
        }
    }

    Func_02000a88(113);
    Func_020009f6(window);
    Func_020009b4(1);
    Func_020009cc(window, 1);

    /* Deliberate non-sequential refresh order. */
    Func_02000a32(0);
    Func_02000a38(1);
    Func_02000a3e(3);
    Func_02000a44(2);
    return;

scheduler:
    Func_020009e6(1);
    goto loop;
}
