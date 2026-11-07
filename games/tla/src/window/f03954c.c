#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "window.h"
#include "ram_buffer.h"
#include "resource.h"
#include "iwram_call.h"

struct UiChannelSlot *Message_Start(struct Window *window, s32 entry, s32 x,
                                    s32 y, u16 *spacing, s32 flags) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct UiChannelSlot *render = work->channels;
  struct UiChannelSlot *found = NULL;
  u32 i;

  for (i = 0; i != 3; i++, render++) {
    if (render->work == NULL) {
      found = render;
      break;
    }
  }
  if (found != NULL) {
    found->work = window;
    found->startX = x << 8;
    found->x = x << 8;
    found->y = y << 8;
    found->entry = entry;
    found->colour = 15;
    found->lineSpacing = 10;
    found->countdown = 0;
    found->outline = 0;
    found->ended = 0;
    found->unk26 = 0;
    found->flags = flags;
    if (work->textMode != 0 && spacing != NULL)
      found->unk26 = spacing[3];
    if (spacing != NULL) {
      for (i = 0; i < 4; i++)
        found->spacing[i] = *spacing++;
    } else {
      for (i = 0; i < 4; i++)
        found->spacing[i] = 0;
    }
    found->line = 0;
  }
  return found;
}
