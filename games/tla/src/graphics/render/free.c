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
#include "iwram_call.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

void Func_08039754(struct UiChannelSlot *channel);

/// Starts a channel typing the decoded text at entry into the battle message
/// window. A channel that already serves a finished window continues below
/// its last line (scrolling up once both lines are used) unless keepPosition
/// is set. Returns the channel, or NULL when all three are busy.
// As ☀️'s BattleMessage_Continue.
struct UiChannelSlot *BattleMessage_Continue(struct Window *window,
                                                s32 entry, s32 keepPosition) {
  struct UiChannelSlot *slot;
  struct UiChannelSlot *selected;
  u16 *spacing;
  u16 zero;
  u16 lineSpacing;
  u32 index;

  slot = UI_WORK->channels;
  selected = 0;
  for (index = 0; index != 3; slot++, index++) {
    if (slot->work == 0 || slot->work->state != 0) {
      selected = slot;
      break;
    }
  }

  if (selected != 0) {
    // Positions are in 1/256 pixel: the first line is 10 pixels down, the
    // second 13 pixels below it, and after the second the text scrolls up.
    if (selected->work == 0) {
      selected->y = 10 << 8;
      selected->work = window;
      selected->x = 3 << 8;
    } else if (keepPosition == 0) {
      if (selected->y == 0) {
        selected->y = 10 << 8;
      } else if (selected->y < 13 << 8) {
        selected->y += 13 << 8;
      } else {
        Func_08039754(selected);
      }
      selected->x = 3 << 8;
    }

    zero = 0;
    selected->startX = 3 << 8;
    selected->work->state = zero;
    // Start the next text run without retaining the previous run's style.
    selected->colour = 15;
    selected->entry = entry;
    lineSpacing = 10;
    selected->countdown = zero;
    selected->outline = zero;
    selected->line = zero;
    selected->ended = zero;
    /* FAKEMATCH(barrier): the line spacing held behind an empty asm, so it is
     * built after the entry store and stored after the cleared fields; as a
     * plain store, 6 of 81 instructions differ in tla-en. */
    asm volatile("" : "+l"(lineSpacing));
    selected->lineSpacing = lineSpacing;
    index = 0;
    spacing = selected->spacing;
    do {
      index++;
      *spacing++ = zero;
    } while (index < 4);
  }
  return selected;
}

s32 Vram_ClearWindowBlock(void) {
  s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWords;

  return fill((void *)0x06002500, 0xf00, 0);
}

void Vram_FillWindowBlockPattern(void) {
  s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWords;

  fill((void *)0x06002500, 0xf00, 0x44444444);
}

void Message_ResetIdle(void) {
  struct UiChannelSlot *slot =
      ((struct WindowWork *)Ram_HeapSlots->windowTiles)->channels;
  struct UiChannelSlot *selection = 0;
  s32 i;

  for (i = 0; i != 3; slot++, i++) {
    if (slot->work == 0 || slot->work->state != 0) {
      selection = slot;
      break;
    }
  }
  if (selection != 0) {
    if (selection->work != 0) {
      Vram_ClearWindowBlock();
      selection->y = 0;
    }
    selection->x = 0;
    selection->countdown = 0;
    selection->colour = 0xF;
    selection->outline = 0;
    selection->lineSpacing = 0xA;
  }
}
