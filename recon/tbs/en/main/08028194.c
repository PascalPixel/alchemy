/* 2026-10-04 (second-wave finisher 6): EN score 62 (was 238), 384/384
 * instructions, no FAKEMATCH device. Changes: the loop reads the sprite x
 * into its own block local (px), which lets the gVramBlockCache base take
 * caller-saved r4 as listed; each window half declares its own window and
 * span (the listing keeps window in sl in the first half and lr in the
 * second, so they are separate pseudos); edge adds (x + span) as a group;
 * the first half accumulates end from the entry's y + 24; the second half
 * computes start, takes the line from it and then copies it into i.
 * Left: (1) the second half reads x before computing window, where the
 * listing computes window first; with window first (and the same body) the
 * selection offset takes r4, x r1 and the -2817 constant r0 instead of
 * r1/r0/r4 (score 84). Pinning a half-local x to r0 fixes that head (72)
 * but moves x + span into x's register. Declaration order in the half has
 * no effect. (2) The unselected sprite's arm: the listing forms attr before
 * loading y and stores the attribute word after the slot load. Reading y
 * inline, attr[] indexing, a value temporary and attr-first all score
 * worse (125-218). Earlier notes follow. */
/* 2026-10-04 (finisher 7): EN score 238 (was 328), 384/384 instructions.
 * work->count and work->selection are read directly (the listing reloads
 * both with ldrsh through a hoisted &count in sl, as GCC does for a field
 * test); there is no mask variable (the 255 is loop.c's hoisted constant);
 * the else arm reads y before its stores; sprite is set right after work,
 * so the allocator ranks i above sprite (i r5, sprite r6 as listed).
 * Left: x takes r4 where the listing has r2, so the gVramBlockCache base
 * cannot take caller-saved r4 (the listing saves it at sp+0 around
 * Runtime_PushSlotEntry, frame 12) and lands in r9, pushing the matrix to
 * fp; left is ip in the listing; the else arm's slot load still follows its
 * second store; the window halves keep the window in sl and &selection in
 * lr in the listing. Global-allocation priority (floor_log2(refs) * refs /
 * live length, refs weighted by loop depth; see a -dg dump) orders them. */
/* 2026-10-03 ordinary owner/API attempt: consume the shared affine input
 * and canonical resource/allocator contracts. Earlier measurements are
 * prior checkpoints; fresh complete-object comparison is pending. */
#include "AFFINE.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESMENU.H"
/* 2026-09-29: eight minutes of permutation reached 4740 from 6970 through
 * 64 rewrites; not kept, since the owner is far from exact and its scale
 * table, Data_080366f8, has no label in the build yet. */
/* Draft, not exact (2026-09-25): 856 of 840 bytes, 381 differing halfwords.
   Written from the listing after splitting this owner out of the main:08027f82
   fragment. The prologue, scale pulse, affine matrix set-up and sprite
   attribute words line up. Remaining: GCSE keeps the sprite count in a
   register (an ldrh before the loop and after each slot push) where the ROM
   reads it through sl with ldrsh at every test, which shifts the loop's
   registers; the two window-edge halves after the loop are not yet checked
   against the listing. Tried: a volatile count pointer, work->count
   directly, a do-while and a goto loop. */
#include "TYPES.H"

struct WindowLine {
    u16 left;
    u16 span;
};

struct WindowBuffer {
    u8 unknown_000[4];
    struct WindowLine lines[160];
};

struct IconSlot {
    u16 size;
    u16 offset;
};

extern u32 gFrameCount;
extern u16 Data_080366f8[];
extern struct IconSlot gVramBlockCache[];
extern u8 *Data_03001ecc;
void AffineEffect_UpdateFrame(void)
{
    struct ResourceMenuWork *work;
    struct ResourceMenuEntry *sprite;
    struct AffineTransform effect;
    s32 scale;
    s32 matrix;
    s32 edge;
    s32 x;
    s32 y;
    u32 end;
    u32 i;
    u16 *line;
    u8 *buffers;

    work = gMenuSelectWork;
    sprite = work->entries;
    scale = Data_080366f8[(gFrameCount * 2) & 31];
    scale = (scale - 256) / 4 + 304;
    effect.scale_x = scale;
    effect.scale_y = scale;
    effect.angle = 0;
    matrix = AffineMatrix_BuildForEffect(&effect);
    for (i = 0; i < work->count; i++, sprite++) {
        u32 *attr;
        s32 slot;
        s32 px;

        px = sprite->x;
        if (px == 0)
            continue;
        if (i == work->selection) {
            s32 left = px + scale * 7 / 512 - 20;

            if (sprite->y != 0)
                y = sprite->y + scale * 3 / 256 - 20;
            else
                y = (scale * 15 / 256 - 30) & 255;
            attr = &sprite->link;
            *attr++ = 0;
            *attr++ = (matrix << 25) | y | (left << 16) | 0x80002300;
            *attr = gVramBlockCache[sprite->slot].offset >> 5;
            slot = 246;
        } else {
            y = sprite->y;
            attr = &sprite->link;
            *attr++ = 0;
            *attr++ = y | (px << 16) | 0x80002000;
            *attr = gVramBlockCache[sprite->slot].offset >> 5;
            slot = 245;
        }
        Runtime_PushSlotEntry(sprite, slot);
    }
    if (work->row == 0) {
        struct WindowBuffer *window;
        s32 span;

        buffers = Data_03001ecc;
        if (buffers == 0 || work->count == 0)
            return;
        window = (struct WindowBuffer *)(buffers + buffers[0x539] * 644);
        x = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((x - span) << 8) + (x + span) + 23;
        end = work->entries[work->selection].y + 24;
        end += (scale * 32 - 7937) / 512 + 1;
        line = &window->lines[24].span;
        i = 24;
        if (i < end) {
            do {
                *line = (*line & 0xff) | edge;
                i++;
                line += 2;
            } while (i < end);
        }
        edge = work->entries[0].x;
        if (work->selection == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[0].span;
        for (i = 0; i <= 23; i++) {
            *line = (*line & 0xff) | edge;
            line += 2;
        }
    } else {
        struct WindowBuffer *window;
        s32 start;
        s32 span;

        buffers = Data_03001ecc;
        if (buffers == 0 || work->count == 0)
            return;
        x = work->entries[work->selection].x;
        window = (struct WindowBuffer *)(buffers + buffers[0x539] * 644);
        span = (scale * 12 - 2817) / 256;
        edge = ((x - span) << 8) + (x + span) + 23;
        start = work->entries[work->selection].y - (scale * 32 - 7937) / 512 - 1;
        line = &window->lines[start].span;
        i = start;
        if (i <= 135) {
            do {
                *line = (*line & 0xff) | edge;
                i++;
                line += 2;
            } while (i <= 135);
        }
        edge = work->entries[0].x;
        if (work->selection == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[136].span;
        for (i = 136; i <= 159; i++) {
            *line = (*line & 0xff) | edge;
            line += 2;
        }
    }
}
