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

struct AffineEffectScale {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};

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

s32 AffineMatrix_BuildForEffect(struct AffineEffectScale *source);
void Runtime_PushSlotEntry(void *entry, s32 slot);

void AffineEffect_UpdateFrame(void)
{
    struct ResourceMenuWork *work;
    struct ResourceMenuEntry *sprite;
    struct AffineEffectScale effect;
    struct WindowBuffer *window;
    s16 *count;
    s16 *selected;
    s32 scale;
    s32 matrix;
    s32 span;
    s32 edge;
    s32 x;
    s32 y;
    u32 i;
    u32 end;
    u16 *line;
    u32 mask;
    u8 *buffers;

    work = gMenuSelectWork;
    scale = Data_080366f8[(gFrameCount * 2) & 31];
    sprite = work->entries;
    scale = (scale - 256) / 4 + 304;
    effect.x = scale;
    effect.y = scale;
    effect.angle = 0;
    matrix = AffineMatrix_BuildForEffect(&effect);
    count = &work->count;
    for (i = 0; i < *count; i++, sprite++) {
        u32 *attr;
        s32 slot;

        x = sprite->x;
        if (x == 0)
            continue;
        if (i == work->selection) {
            s32 left = x + scale * 7 / 512 - 20;

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
            attr = &sprite->link;
            *attr++ = 0;
            *attr++ = sprite->y | (x << 16) | 0x80002000;
            *attr = gVramBlockCache[sprite->slot].offset >> 5;
            slot = 245;
        }
        Runtime_PushSlotEntry(sprite, slot);
    }
    if (work->row == 0) {
        buffers = Data_03001ecc;
        if (buffers == 0 || *count == 0)
            return;
        window = (struct WindowBuffer *)(buffers + buffers[0x539] * 644);
        selected = &work->selection;
        x = work->entries[*selected].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((x - span) << 8) + x + span + 23;
        y = work->entries[*selected].y + 24;
        end = y + (scale * 32 - 7937) / 512 + 1;
        line = &window->lines[24].span;
        i = 24;
        if (i < end) {
            mask = 255;
            do {
                *line = (*line & mask) | edge;
                i++;
                line += 2;
            } while (i < end);
        }
        edge = work->entries[0].x;
        if (*selected == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[0].span;
        mask = 255;
        for (i = 0; i <= 23; i++) {
            *line = (*line & mask) | edge;
            line += 2;
        }
    } else {
        buffers = Data_03001ecc;
        if (buffers == 0 || *count == 0)
            return;
        window = (struct WindowBuffer *)(buffers + buffers[0x539] * 644);
        selected = &work->selection;
        x = work->entries[*selected].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((x - span) << 8) + x + span + 23;
        i = work->entries[*selected].y - (scale * 32 - 7937) / 512 - 1;
        line = &window->lines[i].span;
        if (i <= 135) {
            mask = 255;
            do {
                *line = (*line & mask) | edge;
                i++;
                line += 2;
            } while (i <= 135);
        }
        edge = work->entries[0].x;
        if (*selected == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[136].span;
        mask = 255;
        for (i = 136; i <= 159; i++) {
            *line = (*line & mask) | edge;
            line += 2;
        }
    }
}
