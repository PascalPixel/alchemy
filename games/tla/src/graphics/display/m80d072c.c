#include "types.h"
#include "callback_scheduler.h"
#include "display_transition.h"
#include "frame.h"
#include "io_reg.h"
#include "irq.h"
#include "iwram_call.h"
#include "map.h"
#include "ram_buffer.h"

s32 Func_080d072c(void) {
  s32 other = (s32)gIrqHandlers[1] ^ (s32)DisplayTransition_UpdateScanline;

  return 1 - ((u32)(-other | other) >> 31);
}

struct MapLayerFlags {
  u8 unk00[0x14];
  u16 flags;
};

/* FAKEMATCH: read as a volatile halfword the flags stay the AND's second
 * operand, so the mask is built in the result register as in ⚓️; a plain
 * temporary swaps the two registers (differ 24). */
#define MAP_LAYER_FLAGS(map) (*(volatile u16 *)&(map)->flags)

/// Clear the flags for each map layer.
void Map_ClearLayerFlags(s32 layer0, s32 layer1, s32 layer2) {
  struct MapLayerFlags *map;
  u16 flags;

  map = (struct MapLayerFlags *)Ram_HeapSlots->mapWork;
  if (map == 0)
    return;
  if (layer2) {
    flags = MAP_LAYER_FLAGS(map);
    /* FAKEMATCH: ⚓️ reads the flags before it starts the mask; written as
     * map->flags &= ~0x200 the mask's first half is scheduled ahead of the read
     * (differ 2) */
    asm volatile("");
    map->flags = flags & ~0x200;
  }
  if (layer1) {
    flags = MAP_LAYER_FLAGS(map);
    asm volatile(""); /* FAKEMATCH: as above, for ~0x400 (differ 2) */
    map->flags = flags & ~0x400;
  }
  if (layer0) {
    flags = MAP_LAYER_FLAGS(map);
    asm volatile(""); /* FAKEMATCH: as above, for ~0x800 (differ 2) */
    map->flags = flags & ~0x800;
  }
}

/// Steps the transition's value toward its end and opens the window from the
/// top and bottom edges by that many lines.
void DisplayTransition_Update(void) {
  struct DisplayTransitionState *state;
  struct MapViewWork *window;
  s8 *duration;
  u32 value;

  /* FAKEMATCH: the one-pass block loads the transition state before the
   * duration's offset is built (one reordered row, differ 2 without it). */
  do {
    state = Ram_HeapSlots->displayTransition;
  } while (0);
  duration = &state->duration;
  window = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Scheduler_RemoveCallback(DisplayTransition_Update);
      Runtime_SetIrqHandler(1, 0, 0);
      window->extraTiles = 3;
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 offset;

      (*step)++;
      /* FAKEMATCH: the one-pass block ends at the divide, so the start is read
       * before the value's offset is built (one reordered row, differ 2 without
       * it). */
      do {
        offset = Iwram_SignedDivide(delta * *step, *duration);
      } while (0);
      state->value = state->start + offset;
    }
  }

  value = state->value;
  if (value > 79) {
    window->windowTop = 200;
    window->windowBottom = 250;
  } else {
    window->windowTop = value;
    window->windowBottom = 159 - value;
  }
}

/// As DisplayTransition_Update, but the window opens from the centre, on
/// alternate frames only.
void DisplayTransition_UpdateFromCentre(void) {
  struct DisplayTransitionState *state;
  struct MapViewWork *window;
  s8 *duration;
  u16 value;

  /* FAKEMATCH: the one-pass block loads the transition state before the
   * duration's offset is built (one reordered row, differ 2 without it). */
  do {
    state = Ram_HeapSlots->displayTransition;
  } while (0);
  duration = &state->duration;
  window = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Scheduler_RemoveCallback(DisplayTransition_UpdateFromCentre);
      Runtime_SetIrqHandler(1, 0, 0);
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 offset;

      (*step)++;
      /* FAKEMATCH: the one-pass block ends at the divide, so the start is read
       * before the value's offset is built (one reordered row, differ 2 without
       * it). */
      do {
        offset = Iwram_SignedDivide(delta * *step, *duration);
      } while (0);
      state->value = state->start + offset;
    }
  }

  value = state->value;
  if (value > 79) {
    window->windowTop = 200;
    window->windowBottom = 250;
  } else if (value != 0 && (gFrameCount & 1)) {
    window->windowTop = value + 80;
    window->windowBottom = 80 - value;
  } else {
    window->windowTop = 0;
    window->windowBottom = 159;
  }
}

/* FAKEMATCH: the one-pass block reads the display control before the mask is
   built; written as REG_DISPCNT &= 0xFFF8 the mask's first half is scheduled
   ahead of the read (one reordered row at each of the four uses, differ 8). */
#define READ_DISPCNT(dispcnt)                                                  \
  do {                                                                         \
    dispcnt = REG_DISPCNT;                                                     \
  } while (0)

/// The VCount interrupt: switches the display to mode 2 at the window's first
/// line and back to mode 0 at its second.
void DisplayTransition_UpdateScanline(void) {
  u32 line;
  struct MapViewWork *state;
  u16 dispcnt;

  line = REG_VCOUNT;
  state = (struct MapViewWork *)Ram_HeapSlots->mapWork;

again:
  switch (state->irqPhase) {
  case 3:
    if (line >= state->irqFirstLine) {
      READ_DISPCNT(dispcnt);
      REG_DISPCNT = (dispcnt & 0xFFF8) | 2;
      state->irqPhase = 9;
    }
    break;
  case 2:
    if (line >= state->irqSecondLine) {
      READ_DISPCNT(dispcnt);
      REG_DISPCNT = dispcnt & 0xFFF8;
      state->irqPhase = 9;
    }
    break;
  case 1:
    if (line >= state->irqFirstLine) {
      READ_DISPCNT(dispcnt);
      REG_DISPCNT = (dispcnt & 0xFFF8) | 2;
      state->irqPhase++;
      goto again;
    }
    if (line >= state->irqSecondLine) {
      READ_DISPCNT(dispcnt);
      REG_DISPCNT = dispcnt & 0xFFF8;
      state->irqPhase = 3;
      goto again;
    }
    break;
  case 0:
    if (line <= 158) {
      state->irqPhase = 1;
      goto again;
    }
    break;
  }
}
