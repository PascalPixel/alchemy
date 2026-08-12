#include "types.h"

/*
 * resource_3bb owner at 0x02003638, 964 bytes (0x02003638-0x020039fb):
 * code 0x02003638-0x020039db plus the eight-word literal pool at 0x020039dc,
 * which the epilogue at 0x020039ca sits in front of.
 *
 * Prologue at 0x02003638 saves r5, r6, r7, lr and then fp, sl, r9 and r8
 * before `sub sp, #20`; the epilogue restores all of them and ends
 * `pop {r0} / bx r0`, so r0 holds the popped return address and the owner is
 * void.  The twenty bytes of frame are five slots: a scratch word, the entry
 * stride, the tile base, the output cursor and a second copy of the
 * workspace pointer.
 *
 * This is the task Func_020039fc installs: that owner passes 0x0200b639,
 * which is this entry plus the Thumb bit, under key 0xc76.  It also fills in
 * the same descriptor fields this owner reads - +216, +218, +220, +222, +224,
 * +230, +232 and +236 - so the two are only interpretable together.  Under
 * the overlay's 0x02008000 link base 0x0200c174 and 0x0200c194 are in-image
 * data at file offsets 0x4174 and 0x4194.
 *
 * All 20 `bl` sites are placed and reach eight distinct callees.  The
 * inventory row records calls=19; that count predates the corrected branch
 * decoding.  Targets come from cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- (target offset =
 * stored displacement + 2), never from the disassembler's annotations, which
 * print each of the nine Func_080001e8 sites as a different function.
 * 0x3ee8 -> Func_080770c0, 0x3de8 -> Func_08000170, 0x3df8 ->
 * Func_080001a8, 0x3e10 -> Func_080001c8, 0x3df0 -> Func_08000178, 0x3e08 ->
 * Func_080001c0, 0x3e28 -> Func_080001e8 (nine sites), 0x4078 ->
 * Func_0808a400 (twice), 0x3d98 -> Func_03000380 (four sites).
 *
 * Func_03000380 is a relocated IWRAM helper reached through its own veneer,
 * in the same family as the relocated square root at 0x030001d8.  Here it is
 * always called as (difference, 0xe0000) and its result is used as a screen
 * coordinate, so it is a fixed-point scale.
 *
 * Shape.  The workspace pointer at 0x03001f3c carries the descriptor.  A
 * three-state counter at +218 is driven first: it is forced to 2 while +220
 * is non-zero, otherwise it counts down while Func_080770c0(262) is set and
 * counts up to a ceiling of 2 while it is clear.  Exactly on the up-step to 1
 * a palette block is pushed by programming DMA3 at 0x040000d4 from the
 * in-image data at file offset 0x4174 into 0x050003c0, sixteen words, and the
 * descriptor's own graphics are loaded through the Func_08000170 /
 * Func_080001a8 / Func_080001c8 / Func_08000178 sequence that Func_020039fc
 * also uses.  With the counter back at zero the display is torn down through
 * Func_080001c0 and nothing is drawn.
 *
 * Otherwise a list of twelve-byte entries is built from the start of the
 * workspace and each one is submitted with Func_080001e8(entry, 255, 12).
 * Separate write and submission cursors reproduce the two synchronized roles
 * visible in the assembly without conflating the stores with the call input.
 * The list is a frame: one top piece, `count` left-column pieces, two middle
 * pieces, `count` right-column pieces, one bottom piece, and up to two marker
 * pieces for the participants named at +224 and +222 - the latter only when
 * the low nibble of 0x03001e40 is above 4.  Each entry is three words: zero,
 * a packed word holding the column in its low byte, a row in its high half
 * and the piece flags, and a tile word ORed with 0xe400.
 *
 * Uncertainties: the packed words are reproduced bit for bit rather than
 * decomposed into named fields, since the consumer of Func_080001e8 is not in
 * this overlay.  The final Func_080001e8 call leaves r2 holding the output
 * cursor rather than the constant 12 that every other site passes; only two
 * arguments are asserted for it.  The counter's `(value << 16) == 0x10000`
 * test is a 16-bit equality against 1 and is written that way.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
s32 Func_02007574();
s32 Func_020074bc();
void Func_020074d6();
void Func_020074fc();
void Func_020074e2();
void Func_0200750e();
void Func_02007584();
void Func_020075c6();
void Func_02007606();
void Func_02007636();
void Func_0200767c();
void Func_020076d2();
u8 *Func_02007942();
s32 Func_0200767c_b();
s32 Func_02007692();
void Func_0200776e();
u8 *Func_020079ca();
s32 Func_02007704();
s32 Func_0200771a();
void Func_020077f0();

                     

                     

/* Relocated IWRAM helper reached through this overlay's veneer at 0x3d98. */

/* In-image palette block at file offset 0x4174 and descriptor at 0x4194. */
extern u8 Data_0200c174[];
extern u8 Data_0200c194[];

/* IWRAM. */
extern u8 *Data_03001f3c;
extern u16 Data_03001b10[];
extern u32 Data_03001e40;

void Func_02003638(void)
{
    u8 *workspace;
    u32 *entry;
    struct {
        u32 tile;
        u32 *write;
        u8 *workspace;
    } cursors;
    u32 column;
    s32 state;
    s32 count;
    s32 index;
    s32 handle;
    u8 *marker;
    s32 screen_x;
    s32 screen_y;

    workspace = Data_03001f3c;
    cursors.write = (u32 *)workspace;
    entry = cursors.write;
    cursors.workspace = workspace;

    cursors.tile = Data_03001b10[*(s16 *)(workspace + 216) * 2 + 1] >> 5;

    count = *(s16 *)(workspace + 230);

    if (*(s16 *)(workspace + 220) != 0) {
        *(u16 *)(workspace + 218) = 2;
    } else if (Func_02007574(0x106) != 0) {
        if (*(s16 *)(workspace + 218) > 0) {
            *(u16 *)(workspace + 218) = *(u16 *)(workspace + 218) - 1;
        }
    } else if (*(s16 *)(workspace + 218) <= 1) {
        u16 stepped = *(u16 *)(workspace + 218) + 1;
        *(u16 *)(workspace + 218) = stepped;

        if (((s32)stepped << 16) == 0x10000) {
            *(volatile u32 *)0x040000d4 = (u32)Data_0200c174;
            *(volatile u32 *)0x040000d8 = 0x050003c0;
            *(volatile u32 *)0x040000dc = 0x80000010;

            handle = Func_020074bc(0x200);
            Func_020074d6(Data_0200c194, handle);
            Func_020074fc(*(s16 *)(workspace + 216), 0x200, handle);
            Func_020074e2(handle);
        }
    }

    state = *(s16 *)(workspace + 218);
    if (state == 0) {
        Func_0200750e(*(s16 *)((u8 *)cursors.write + 216));
        return;
    }

    column = ((u32)(state * 6) - 8) & 0xff;

    cursors.write[0] = 0;
    cursors.write[1] = (u32)((104 - (count << 4)) << 16) | column | 0x8000;
    cursors.write[2] = cursors.tile | 0xe400;
    Func_02007584(entry, 255, 12);
    entry += 3;
    cursors.write += 3;

    for (index = 0; (u32)index < (u32)count; index++) {
        cursors.write[0] = 0;
        cursors.write[1] = (u32)((96 - (index << 4)) << 16) | column | 0x40000000;
        cursors.write[2] = (cursors.tile + 2) | 0xe400;
        Func_020075c6(entry, 255, 12);
        entry += 3;
        cursors.write += 3;
    }

    cursors.write[0] = 0;
    cursors.write[1] = 0x700000 | column | 0x8000;
    cursors.write[2] = (cursors.tile + 6) | 0xe400;
    Func_02007606(entry, 255, 12);
    entry += 3;
    cursors.write += 3;

    cursors.write[0] = 0;
    cursors.write[1] = 0x780000 | column | 0x8000 | 0x10000000;
    cursors.write[2] = (cursors.tile + 6) | 0xe400;
    Func_02007636(entry, 255, 12);
    entry += 3;
    cursors.write += 3;

    {
        u32 vertical = 0x800000;

        for (index = 0; (u32)index < (u32)count; index++) {
            cursors.write[0] = 0;
            cursors.write[1] = column | vertical | 0x40000000 | 0x10000000;
            cursors.write[2] = (cursors.tile + 2) | 0xe400;
            Func_0200767c(entry, 255, 12);
            entry += 3;
            cursors.write += 3;
            vertical += 0x100000;
        }
    }

    column |= (u32)(((count << 4) + 128) << 16);
    column |= 0x8000;
    column |= 0x10000000;

    cursors.write[0] = 0;
    cursors.write[1] = column;
    cursors.write[2] = cursors.tile | 0xe400;
    Func_020076d2(entry, 255, 12);
    entry += 3;
    cursors.write += 3;

    if ((Data_03001e40 & 15) <= 4) {
        return;
    }

    marker = Func_02007942(*(s16 *)(cursors.workspace + 224));
    if (marker != 0) {
        screen_x = Func_0200767c_b(*(s32 *)(marker + 8) - *(s32 *)(cursors.workspace + 232),
                                   0xe0000);
        screen_y = screen_x + 112;
        screen_x = Func_02007692(*(s32 *)(marker + 16) - *(s32 *)(cursors.workspace + 236),
                                 0xe0000);
        screen_x = screen_x + (*(s16 *)(cursors.workspace + 218) * 6);

        cursors.write[0] = 0;
        cursors.write[1] = (((u32)(screen_x - 4)) & 0xff) |
                           (u32)(screen_y << 16) | 0x40000000;
        cursors.write[2] = (cursors.tile + 12) | 0xe400;
        Func_0200776e(entry, 255, 12);
        entry += 3;
        cursors.write += 3;
    }

    marker = Func_020079ca(*(s16 *)(cursors.workspace + 222));
    if (marker == 0) {
        return;
    }

    screen_x = Func_02007704(*(s32 *)(marker + 8) - *(s32 *)(cursors.workspace + 232),
                             0xe0000);
    screen_y = screen_x + 112;
    screen_x = Func_0200771a(*(s32 *)(marker + 16) - *(s32 *)(cursors.workspace + 236),
                             0xe0000);
    screen_x = screen_x + (*(s16 *)(cursors.workspace + 218) * 6);

    cursors.write[0] = 0;
    cursors.write[1] = (((u32)(screen_x - 4)) & 0xff) |
                       (u32)(screen_y << 16) | 0x40000000;
    cursors.write[2] = (cursors.tile + 8) | 0xe400;

    Func_020077f0(entry, 255);
}
