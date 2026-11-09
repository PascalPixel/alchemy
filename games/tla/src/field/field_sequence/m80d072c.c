#include "types.h"
#include "task.h"
#include "display_transition.h"
#include "frame.h"
#include "io_reg.h"
#include "irq.h"
#include "iwram_call.h"
#include "map.h"
#include "ram_buffer.h"

s32 Func_080d072c(void) {
  s32 other = (s32)gIrqHandlers[1] ^ (s32)ScreenTransition_UpdateScanline;

  return 1 - ((u32)(-other | other) >> 31);
}

struct MapLayerFlags {
  u8 unk00[0x14];
  u16 flags;
};

/// The layer flags read through a volatile pointer, which keeps them the AND's
/// second operand and builds the mask in the result register; as map->flags,
/// 24 of 31 instructions of Map_ClearLayerFlags differ in tla-en.
#define MAP_LAYER_FLAGS(map) (*(volatile u16 *)&(map)->flags)

/// Clears the flag of each map layer whose argument is nonzero.
void Map_ClearLayerFlags(s32 layer0, s32 layer1, s32 layer2) {
  struct MapLayerFlags *map;
  u16 flags;

  map = (struct MapLayerFlags *)Ram_HeapSlots->mapWork;
  if (map == 0)
    return;
  if (layer2) {
    flags = MAP_LAYER_FLAGS(map);
    map->flags = flags & ~0x200;
  }
  if (layer1) {
    flags = MAP_LAYER_FLAGS(map);
    map->flags = flags & ~0x400;
  }
  if (layer0) {
    flags = MAP_LAYER_FLAGS(map);
    map->flags = flags & ~0x800;
  }
}

/// Steps the transition's value toward its end and opens the window from the
/// top and bottom edges by that many lines.
void ScreenTransition_Update(void) {
  struct ScreenTransitionState *state;
  struct MapViewWork *window;
  s8 *duration;
  u32 value;

  state = Ram_HeapSlots->screenTransition;
  duration = &state->duration;
  window = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Task_Remove(ScreenTransition_Update);
      Irq_SetHandler(1, 0, 0);
      window->extraTiles = 3;
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 offset;

      (*step)++;
      offset = Iwram_SignedDivide(delta * *step, *duration);
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

/// As ScreenTransition_Update, but the window opens from the centre, on
/// alternate frames only.
void ScreenTransition_UpdateFromCentre(void) {
  struct ScreenTransitionState *state;
  struct MapViewWork *window;
  s8 *duration;
  u16 value;

  state = Ram_HeapSlots->screenTransition;
  duration = &state->duration;
  window = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Task_Remove(ScreenTransition_UpdateFromCentre);
      Irq_SetHandler(1, 0, 0);
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 offset;

      (*step)++;
      offset = Iwram_SignedDivide(delta * *step, *duration);
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

/// The VCount interrupt: switches the display to mode 2 at the window's first
/// line and back to mode 0 at its second.
void ScreenTransition_UpdateScanline(void) {
  u32 line;
  struct MapViewWork *state;

  line = REG_VCOUNT;
  state = (struct MapViewWork *)Ram_HeapSlots->mapWork;

again:
  switch (state->irqPhase) {
  case 3:
    if (line >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & 0xFFF8) | 2;
      state->irqPhase = 9;
    }
    break;
  case 2:
    if (line >= state->irqSecondLine) {
      REG_DISPCNT &= 0xFFF8;
      state->irqPhase = 9;
    }
    break;
  case 1:
    if (line >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & 0xFFF8) | 2;
      state->irqPhase++;
      goto again;
    }
    if (line >= state->irqSecondLine) {
      REG_DISPCNT &= 0xFFF8;
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
