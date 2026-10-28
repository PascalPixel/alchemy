// The command icon menu's frame task (The Broken Seal's IconMenu_DrawIcons):
// pulses the selected icon with one affine matrix, draws the others plain,
// and while the display transition runs widens window 1 over the icon row.
#include "types.h"
#include "ram_buffer.h"
#include "resource_menu.h"
#include "frame.h"
#include "display_transition.h"
#include "vram_tab.h"
#include "task.h"

// games/tla/include/affine.h's transform, written out: the stale affine.h
// beside these drafts would shadow it.
struct AffineTransform {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 unk06;
};

s32 AffineMatrix_BuildForEffect(struct AffineTransform *transform);

extern const u16 gUi_BounceScales[];

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

void Func_0804cda8(void) {
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
  sprite = work->entries;
  scale = gUi_BounceScales[(gFrameCount * 2) & 31];
  buffers = Ram_HeapSlots->displayTransition;
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
      *attr = gResourceTableEntries[sprite->slot].offset >> 5;
      slot = 246;
    } else {
      u32 base;
      u32 value;

      attr = &sprite->link;
      y = sprite->y;
      *attr++ = 0;
      value = y | (px << 16) | 0x80002000;
      base = gResourceTableEntries[sprite->slot].offset;
      *attr++ = value;
      *attr = base >> 5;
      slot = 245;
    }
    Oam_AddSpriteAtDepth(sprite, slot);
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
      /* FAKEMATCH(temporary): the mask through start; as (*line & 0xff), 194 of
       * 386 instructions differ in tla-en. */
      start = 0xff;
      *line = (*line & start) | edge;
      line += 2;
    }
  }
}
