// Draft, differ 176 of 977 (198 before; 2026-10-26): the instruction count
// is now the listing's. Case 8 clamps the row distance to -120..120 before
// squaring it, as the listing does, and cases 7, 9 and 77 read the view's x
// and z through one pointer (the native loads them as [r2, #0] and [r2, #4]);
// the view is the map work's viewX and viewZ. Left: case 8 reads both view
// words first and the value after the two header stores in the native (in
// that order here, 230: allocation elsewhere moves), and instruction order.
// Earlier: differ 198 of 977 (441 before; 2026-10-26: in the circle case the
// value is read before the camera's y, which gives the map work r6 as the
// native has it): the window transition's frame callback, ported from
// ☀️'s ScreenTransition_UpdateScanlineTable (games/tbs/src/overworld/
// overworld/transition.c) with this game's state (heap slot 0x7c, s8
// steps), its map work's view at 0xe4, the controlled owner in place of the
// leader, the task table, and one addition: a transition that ends with no
// next mode also opens WINOUT's layers when the object window is on.
// Left: the draft is six instructions shorter, the DISPCNT masks load
// REG_DISPCNT before the end test, and instruction order.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "display_transition.h"
#include "map.h"

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
extern u32 gFrameCount;

void Task_Remove(void (*callback)(void));
void BattleEffect_StartWindowHBlankDma(void);
void UiWindow_FillFromSceneFar(u16 *pairs);
s32 EventRuntime_GetControlledOwner(void);
u32 Random16(void);

void Func_080cf78c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ScreenTransitionState *state = Ram_HeapSlots->screenTransition;
  struct TransitionSpan *span;
  struct TransitionShape *shape;
  struct ObjectRuntime *object;
  u16 *scanlineData;
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
  s32 cameraX;
  s32 cameraY;
  u32 leftEdge;
  s32 oppositeEdges;
  s32 *view;

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
  UiWindow_FillFromSceneFar(scanlineData + 2);

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

  case 7:
    view = &map->viewX;
    cameraX = view[0] & 0xffff0000;
    cameraY = view[1] & 0xffff0000;
    object = ObjectTable_Get(EventRuntime_GetControlledOwner());
    centerX = (object->x - cameraX) / 0x10000;
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

  case 8:
    cameraX = map->viewX & 0xffff0000;
    centerX = (state->centreX - cameraX) / 0x10000;
    value = state->value;
    cameraY = map->viewZ & 0xffff0000;
    centerY = (state->centreY - cameraY) / 0x10000 - 16;
    k = -centerY * 2 + gFrameCount;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    if (value & 32)
      value = 32 - (value & 31);
    else
      value &= 31;
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      if (value < -120)
        value = -120;
      if (value > 120)
        value = 120;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < right) {
        delta = Data_080f01e6[k & 31];
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

  case 9:
    view = &map->viewX;
    cameraX = view[0] & 0xffff0000;
    cameraY = view[1] & 0xffff0000;
    object = ObjectTable_Get(EventRuntime_GetControlledOwner());
    centerX = (object->x - cameraX) / 0x10000;
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

  case 77:
    view = &map->viewX;
    cameraX = view[0] & 0xffff0000;
    cameraY = view[1] & 0xffff0000;
    object = ObjectTable_Get(EventRuntime_GetControlledOwner());
    centerX = (object->x - cameraX) / 0x10000;
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

  state->page ^= 1;
}
