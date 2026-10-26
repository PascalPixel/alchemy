#include "types.h"
#include "ram_buffer.h"
#include "window.h"

// Copies the channel's text colour, outline and line spacing into the shared
// glyph renderer before that channel draws its next character.
void Func_0803972c(struct UiChannelSlot *channel) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;

  work->colour = channel->colour;
  work->outline = channel->outline;
  work->lineSpacing = channel->lineSpacing;
}
