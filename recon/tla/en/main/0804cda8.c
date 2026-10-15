/* Draft, differ 198 of 386, 2026-10-15: The Broken Seal's
 * AffineEffect_UpdateFrame on this game's work: the display transition is
 * read from its heap slot at the start, the window lines are left alone
 * while the transition's byte at 0x53f is set, and the pages are inline in
 * the transition. Every instruction's form is the native's; the register
 * allocation differs from the first statement (the slot base in r2 where
 * the native has r0, the count's address and the zero index registers)
 * and cascades. Orders of the work and transition reads tried: 192 to 204. */
#include "types.h"
#include "ram_buffer.h"
#include "resource_menu.h"
#include "affine.h"
#include "frame.h"
#include "display_transition.h"
#include "vram_tab.h"
#include "callback_scheduler.h"

extern const u16 Ui_BounceScales[];

struct WindowLine {
    u16 left;
    u16 span;
};

struct WindowBuffer {
    u8 unk000[4];
    struct WindowLine lines[160];
};

// The transition's byte after its fields; while it is set the window lines
// are left alone.
#define TRANSITION_BUSY(state) (((s8 *)(state))[0x53f])

void AffineEffect_UpdateFrame(void)
{
    struct ResourceMenuWork *work;
    struct ResourceMenuEntry *sprite;
    struct AffineTransform effect;
    s32 scale;
    s32 matrix;
    s32 edge;
    s32 y;
    u32 end;
    u32 i;
    u16 *line;
    struct DisplayTransitionState *buffers;
    struct WindowBuffer *window;
    s32 span;

    work = Ram_HeapSlots->menuSelectWork;
    buffers = Ram_HeapSlots->displayTransition;
    sprite = work->entries;
    scale = Ui_BounceScales[(gFrameCount * 2) & 31];
    scale = (scale - 256) / 4 + 304;
    effect.scaleX = scale;
    effect.scaleY = scale;
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
            *attr = ResourceTableEntries[sprite->slot].offset >> 5;
            slot = 246;
        } else {
            u32 base;
            u32 value;

            attr = &sprite->link;
            y = sprite->y;
            *attr++ = 0;
            value = y | (px << 16) | 0x80002000;
            base = ResourceTableEntries[sprite->slot].offset;
            *attr++ = value;
            *attr = base >> 5;
            slot = 245;
        }
        Runtime_PushSlotEntry(sprite, slot);
    }
    if (buffers == 0 || TRANSITION_BUSY(buffers) != 0 || work->count == 0)
        return;
    window = (struct WindowBuffer *)buffers->pages[buffers->page];
    if (work->row == 0) {
        edge = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((edge - span) << 8) + (edge + span) + 23;
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
        s32 start;

        edge = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((edge - span) << 8) + (edge + span) + 23;
        start = work->entries[work->selection].y;
        start -= (scale * 32 - 7937) / 512 + 1;
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
