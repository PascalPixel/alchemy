/// @file
/// Screen transitions between scenes: window transitions and dissolves.
// Part of f0cf050.c.

#include "../f0cf050_internal.h"

struct TransitionSpan {
  u16 count;
  u16 value;
  u8 x0;
  u8 x1;
  u8 y0;
  u8 y1;
};

struct TransitionShape {
  u16 first;
  u16 second;
  struct TransitionSpan spans[3];
};

extern const u8 Data_080f01c4[];
extern const s8 Data_080f01e6[];
extern struct TransitionShape Data_080f2e9c[];

void ScanlineWindow_FillSpansFar(u16 *pairs);

void Func_080cf78c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ScreenTransitionState *state = Ram_HeapSlots->screenTransition;
  struct TransitionSpan *span;
  struct TransitionShape *shape;
  u16 *scanlineData;
  s32 *viewInverse;
  s32 *viewRipple;
  s32 *viewLeader;
  s32 *viewLeaderLow;
  struct Actor *object;
  u32 i;
  s32 k;
  s32 lineCount;
  s32 value;
  s32 radiusSquaredQ16;
  s32 delta;
  u32 horizontalExtent;
  s32 left;
  s32 right;
  s32 centerX;
  s32 centerY;
  s32 cameraY;
  s32 cameraX;
  s32 cameraY8;
  u32 leftEdge;
  s32 oppositeEdges;

  if (state->duration != 0) {
    if (state->step >= state->duration) {
      state->duration = 0;
      if (state->nextMode == 0) {
        if (state->end == 64)
          REG_DISPCNT &= 0x81ff;
        else
          REG_DISPCNT &= 0x9fff;
        if (REG_DISPCNT & 0x8000)
          REG_WINOUT |= 0x1f;
        Task_Remove(BattleEffect_StartWindowHBlankDma);
        Task_Remove(Func_080cf78c);
        {
          volatile u16 *dma0 = (volatile u16 *)0x040000b0;

          dma0[5] &= ~0x3a00;
          dma0[5] &= ~0x8000;
          dma0[5];
        }
        return;
      }
      state->mode = state->nextMode;
    } else {
      state->value =
          state->start +
          Iwram_SignedDivide((state->end - state->start) * ++state->step,
                             state->duration);
    }
  }

  scanlineData = state->pages[1 ^ state->page];
  ScanlineWindow_FillSpansFar(scanlineData + 2);

  switch (state->mode) {
  case 0:
    *scanlineData++ = 0x7f7f;
    *scanlineData++ = 1;
    value = state->value;
    if (value & 32)
      value = 32 - (value & 31);
    else
      value &= 31;
    value = Data_080f01c4[value];
    for (i = 0; i < 160; i++) {
      leftEdge = (Random16() * (241 - value)) >> 16;
      scanlineData[0] = (leftEdge << 8) | (leftEdge + value);
      scanlineData += 2;
    }
    break;

  case 1:
    value = state->value;
    shape = &Data_080f2e9c[value & 31];
    if (value & 32) {
      *scanlineData++ = shape->first;
      *scanlineData++ = shape->second;
    } else {
      *scanlineData++ = shape->second;
      *scanlineData++ = shape->first;
    }
    for (i = 0; i < 3; i++) {
      span = &shape->spans[i];
      lineCount = span->count;
      right = span->value;
      if (lineCount != 0) {
        if (right != 0) {
          for (k = 0; k < lineCount; k++) {
            scanlineData[0] = right;
            scanlineData += 2;
          }
        } else {
          radiusSquaredQ16 = span->x0;
          left = span->y0;
          delta = span->x1;
          right = span->y1;
          for (k = 0; k < lineCount; k++) {
            scanlineData[0] =
                ((radiusSquaredQ16 + (delta - radiusSquaredQ16) * k / lineCount)
                 << 8) +
                (left + (right - left) * k / lineCount);
            scanlineData += 2;
          }
        }
      }
    }
    break;

  case 2:
    value = state->value - 1;
    if (value & 32) {
      *scanlineData++ = 1;
      *scanlineData++ = 0x7f7f;
    } else {
      *scanlineData++ = 0x7f7f;
      *scanlineData++ = 1;
    }
    value &= 31;
    for (i = 0; i < 160; i++) {
      horizontalExtent = (value << 4) + ((16 * Random16()) >> 16);
      if (horizontalExtent > 255)
        horizontalExtent = 255;
      scanlineData[0] = horizontalExtent;
      scanlineData += 2;
    }
    break;

  case 3:
    value = state->value;
    *scanlineData++ = 0x7f7f;
    *scanlineData++ = 1;
    if ((u32)value > 32) {
      *scanlineData++ = 1;
      *scanlineData++ = 0x7f7f;
      value = 64 - value;
    }
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - 80;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value) << 16)) >> 8;
      left = 120 - horizontalExtent;
      right = horizontalExtent + 120;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > 240)
        left = 240;
      if (right > 240)
        right = 240;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;

  case 4:
    value = state->value;
    if (value & 32) {
      *scanlineData++ = 1;
      *scanlineData++ = 0x7f7f;
    } else {
      *scanlineData++ = 0x7f7f;
      *scanlineData++ = 1;
    }
    value = (u32)(240 * (value & 31)) >> 5;
    for (i = 0; i < 160; i += 2) {
      oppositeEdges = ((240 - value) << 8) + 240;
      scanlineData[0] = value;
      scanlineData[2] = oppositeEdges;
      scanlineData += 4;
    }
    break;

  case 7: {
    s32 ax;

    viewInverse = &map->viewX;
    ax = viewInverse[0] & 0xffff0000;
    cameraY = viewInverse[1] & 0xffff0000;
    object = ObjectTable_Get(Party_GetLeaderActor());
    centerX = (object->x - ax) / 0x10000;
    centerY = ((object->z - object->y) - cameraY) / 0x10000 - 16;
    *scanlineData++ = state->active;
    *scanlineData++ = state->mask;
    value = state->value;
    if ((value & 32) == 0)
      value = 32 - (value & 31);
    else
      value &= 31;
    if (gFrameCount & 1)
      value = 0;
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > 240)
        left = 240;
      if (right > 240)
        right = 240;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;
  }

  case 8:
    viewRipple = &map->viewX;
    cameraX = viewRipple[0] & 0xffff0000;
    cameraY8 = viewRipple[1] & 0xffff0000;
    centerX = (state->centreX - cameraX) / 0x10000;
    centerY = (state->centreY - cameraY8) / 0x10000 - 16;
    k = -centerY * 2 + gFrameCount;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & 32)
      value = 32 - (value & 31);
    else
      value &= 31;
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      delta = i - centerY;
      if (delta < -120)
        delta = -120;
      if (delta > 120)
        delta = 120;
      delta *= delta;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((delta * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < right) {
        value = k & 31;
        delta = Data_080f01e6[value];
        left -= delta;
        right += delta;
        if (left >= right) {
          left = 240;
          right = 240;
        }
      }
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > 240)
        left = 240;
      if (right > 240)
        right = 240;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
      k += 2;
    }
    break;

  case 9: {
    s32 ax;

    viewLeader = &map->viewX;
    ax = viewLeader[0] & 0xffff0000;
    cameraY = viewLeader[1] & 0xffff0000;
    object = ObjectTable_Get(Party_GetLeaderActor());
    centerX = (object->x - ax) / 0x10000;
    centerY = ((object->z - object->y) - cameraY) / 0x10000 - 16;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & 32)
      value = 32 - (value & 31);
    else
      value &= 31;
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > 240)
        left = 240;
      if (right > 240)
        right = 240;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;
  }

  case 77: {
    s32 ax;

    viewLeaderLow = &map->viewX;
    ax = viewLeaderLow[0] & 0xffff0000;
    cameraY = viewLeaderLow[1] & 0xffff0000;
    object = ObjectTable_Get(Party_GetLeaderActor());
    centerX = (object->x - ax) / 0x10000;
    centerY = ((object->z - object->y) - cameraY) / 0x10000 - 8;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & 32)
      value = 32 - (value & 31);
    else
      value &= 31;
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 14)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > 240)
        left = 240;
      if (right > 240)
        right = 240;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;
  }
  }

  state->page ^= 1;
}

void ScreenTransition_InitializeBattleEffectState(s32 mode) {
  struct ScreenTransitionState *state =
      HeapSlot_AllocEwram(124, sizeof(*state));
  volatile u32 zero;

  zero = 0;
  Dma_Set((const void *)&zero, state,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*state) / 4),
          REG_DMA3);
  state->mode = mode;
  state->value = 0;
  state->mask = 0x3f3f;
  state->active = (REG_WINOUT & 0xff00) | 1;
  Task_Add(Func_080cf78c, TASK_PASS_VBLANK);
  Task_Add(BattleEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
}

/// Unused: suspends the window transition's two tasks. Nothing calls it,
/// by name, far-call entry or table.
UNUSED void ScreenTransition_SuspendCallbacks(void) {
  Task_SuspendAll(Func_080cf78c);
  Task_SuspendAll(BattleEffect_StartWindowHBlankDma);
}

/// Allocates the screen transition state (heap slot 124) and clears it.
struct ScreenTransitionState *ScreenTransition_AllocateAndClearState(void) {
  struct ScreenTransitionState *state =
      HeapSlot_AllocEwram(124, sizeof(*state));
  volatile u32 zero = 0;

  Dma_Set(&zero, state,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*state) / 4),
          REG_DMA3);
  return state;
}


void Fade_FromBlack(s32 vblanks);
void Fade_ToBlack(s32 vblanks);
void BattleEffect_SetPrimaryBufferValue(u32 value);

// Fades the screen in over frames frames, as ☀️'s ScreenTransition_Start.
void ScreenTransition_Start(s32 mode, s32 frames) {
  struct MapViewWork *display;
  s32 variant;
  s32 transitionKind;

  display = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  transitionKind = (mode >> 8) & 0xff;
  variant = mode & 0xff;
  switch (transitionKind) {
  case 0:
    Fade_ToBlack(0);
    Fade_FromBlack(frames);
    wait(1);
    break;
  case 1:
    BattleEffect_ApplyColorToSourceBuffer(0x8000, PLTT_BACKDROP);
    BattleEffect_StartBufferInterpolation(frames);
    wait(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    BattleEffect_SetPrimaryBufferValue(0);
    return;
  case 2: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 0;
    state->mask = 63;
    state->active = (REG_WINOUT & 0xff00) | 1;
    Task_Add(Func_080cf78c, TASK_PASS_VBLANK);
    Task_Add(BattleEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
    wait(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 3: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    ScreenTransition_FillTilemapAndSolidTile(15);
    wait(1);
    Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_VBLANK);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 4: {
    struct ScreenTransitionState *state;

    display = (struct MapViewWork *)Ram_HeapSlots->mapWork;
    state = ScreenTransition_AllocateAndClearState();
    display->windowTop = 80;
    display->windowBottom = 80;
    wait(1);
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_VBLANK);
      Irq_SetHandler(1, 0, ScreenTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    } else {
      Task_Add(ScreenTransition_UpdateFromCentre, TASK_PASS_VBLANK);
      Irq_SetHandler(1, 0, ScreenTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                      0x20000);
}

void BattleEffect_ApplyColorToTargetBuffer(s32 color, s32 index);

/// Fades the screen out over frames frames, as The Broken Seal's
/// ScreenTransition_Finish does, with this game's callbacks and priorities.
void ScreenTransition_Finish(s32 mode, s32 frames) {
  s32 variant;
  s32 transitionKind;

  transitionKind = (mode >> 8) & 0xff;
  variant = mode & 0xff;
  switch (transitionKind) {
  case 0:
    Fade_FromBlack(0);
    Fade_ToBlack(frames);
    break;
  case 1:
    BattleEffect_ApplyColorToTargetBuffer(0x8000, 0);
    BattleEffect_StartBufferInterpolation(frames);
    break;
  case 2: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    state->mask = 63;
    state->active = (REG_WINOUT & 0xff00) | 1;
    Task_Add(Func_080cf78c, TASK_PASS_VBLANK);
    Task_Add(BattleEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
    wait(1);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 3: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    ScreenTransition_FillTilemapAndSolidTile(0);
    wait(1);
    Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_VBLANK);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 4: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    // One zero for both branches' stores; a literal 0 at each store differs.
    s32 zero = 0;

    ((u8 *)state)[0x53f] = 1;
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_VBLANK);
      Irq_SetHandler(1, 0, ScreenTransition_UpdateScanline);
      state->start = zero;
      state->end = 80;
      state->duration = frames;
      state->step = zero;
    } else {
      Task_Add(ScreenTransition_UpdateFromCentre, TASK_PASS_VBLANK);
      Irq_SetHandler(1, 0, ScreenTransition_UpdateScanline);
      state->start = zero;
      state->end = 80;
      state->duration = frames;
      state->step = zero;
    }
    break;
  }
  }
}

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

extern const u8 gData_080f0206[];

/// The Broken Seal's ScreenTransition_UpdateDissolve: steps the transition's
/// value toward its end each frame, turning DMA0 off when it arrives, then
/// sets the two pixel nibbles the value selects and queues the tile to
/// character block 0.
void ScreenTransition_UpdateDissolve(void) {
  struct ScreenTransitionDissolveState *state;
  s32 value;
  s32 blend;
  u32 i;

  state =
      (struct ScreenTransitionDissolveState *)Ram_HeapSlots->screenTransition;
  if (state->duration != 0) {
    if (state->step >= state->duration) {
      volatile u16 *dma0;
      u16 control;

      state->duration = 0;
      Task_Remove(ScreenTransition_UpdateDissolve);
      dma0 = REG_DMA0;
      control = dma0[5];
      dma0[5] = control & 0xc5ff;
      dma0[5] &= 0x7fff;
      (void)dma0[5];
      return;
    } else {
      s32 delta = state->end - state->start;

      state->step++;
      state->value = state->start +
                     Iwram_SignedDivide(delta * state->step, state->duration);
    }
  }

  value = state->value - 1;
  state->ditherToggle ^= 1;
  blend = 0;
  if (value & 32)
    blend = 15;
  value = (value & 31) * 2;
  for (i = 0; i <= 1; i++) {
    u8 entry = gData_080f0206[value & 63];
    u8 *p = &state->paletteNibbles[entry >> 1];

    if (entry & 1)
      *p = (*p & 0x0f) | (blend << 4);
    else
      *p = (*p & 0xf0) | blend;
    value++;
  }

  IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->paletteNibbles,
                      DMA_COPY_WORDS(8));
}

/// The Broken Seal's ScreenTransition_FillTilemapAndSolidTile: blanks the
/// screen block at 0x06002000 and, unless color is -1, fills the first tile
/// with that colour.
void ScreenTransition_FillTilemapAndSolidTile(s32 color) {
  struct ScreenTransitionState *work = Ram_HeapSlots->screenTransition;
  Dma_Fill32(0xf000f000, (void *)0x06002000, 0x500);
  if (color != -1) {
    u32 pattern = 0;
    s32 cnt;
    u32 *tile;
    for (cnt = 7; cnt >= 0; --cnt)
      pattern = (pattern << 4) | color;
    tile = work->solidTile;
    for (cnt = 7; cnt >= 0; --cnt)
      *tile++ = pattern;
    Dma_Set(work->solidTile, (void *)0x06000000, DMA_COPY_WORDS(8), REG_DMA3);
  }
}

void ScreenTransition_InitializeState(s32 value) {
  struct ScreenTransitionState *state;

  state = HeapSlot_AllocEwram(0x7c, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  ScreenTransition_FillTilemapAndSolidTile(0);
  state->mode = value;
  state->value = 0;
  Task_Add((s32)(ScreenTransition_UpdateDissolve), TASK_PASS_VBLANK);
  wait(0x78);
}
