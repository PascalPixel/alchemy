// Draft, differ 2 of 20 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Near miss, differ 6 of 20: The Broken Seal's UiText_ApplyChannelStyle. The
// native loads the work pointer before it builds the 0xf3e offset, and each
// field's load between the two halves of its split offset constant (movs r4,
// #240; ldrh r1, [r0, #22]; lsls r4, r4, #4); sched2 builds the constant
// first. The barrier fixes the first (differ 8 without it); reading the
// fields into locals first differs 30; four minutes of permute found nothing
// better. Callers still call it as Func_0803972c (08039864).
#include "types.h"
#include "ram_buffer.h"
#include "window.h"

// Copies the channel's text colour, outline and line spacing into the shared
// glyph renderer before that channel draws its next character.
void UiText_ApplyChannelStyle(struct UiChannelSlot *channel) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;

  /* FAKEMATCH: the work pointer loads before the offset is built. */
  asm("");
  work->colour = channel->colour;
  work->outline = channel->outline;
  work->lineSpacing = channel->lineSpacing;
}
