// Draft, differ 30 of 977, skeleton gap 15 (tla-en, 2026-11-01)
// The window transition's frame callback, ported from ☀️'s
// ScreenTransition_UpdateScanlineTable (games/tbs/src/field/field_sequence/
// screen_transition.c; confirmed twin) with this game's state (heap slot 0x7c,
// s8 steps), its map work's view at 0xe4, the task table, and one addition: a
// transition that ends with no next mode also opens WINOUT's layers when the
// object window is on.
// Left: only sched2 order, no register or shape difference. The native issues
// a store ahead of a split-constant chain at five sites, the spill of state in
// the prologue (ldr, sub sp, movs r0, str, lsls) and the first header store
// plus the second one's `adds r4, #2` in cases 7, 8, 9 and 77 (ldrh, movs r0,
// strh, lsls); here the chain's lsls wins, so the store lands two places late.
// In each the store's only long path runs through a later aliased load, one
// short of the chain, so a store-to-load cost one or two higher in the native
// would explain all five. That is the scheduling model (K2): out of scope.
// Tried on case 7's header, no gain: split `*p = x; p++` (same), `p[0], p[1];
// p += 2` (103), increment first (76), a local for the value (177), a u16 *
// to the field (same); `asm("")` after the store only moves the miss, because
// movs falls before the store and lsls after it, so no barrier can separate
// them. A five-minute permute (28,000 candidates) found nothing below 30.
// What closed the rest, from 159: the twin's four view pointers, its camera
// locals and block `ax` (107); the clamped row is its own variable, `delta`,
// squared in place (the copy pair around muls); `k & 31` as a statement of
// its own, which shortens the table pointer's life from 3 to 2, so loop.c
// keeps it in the loop (threshold 15 * life against 45 real insns) and r5
// stays free for reloads (120 to 30).
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "actor.h"
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

void Task_Remove(void (*func)(void));
void BattleEffect_StartWindowHBlankDma(void);
void Window_FillFromSceneFar(u16 *pairs);
s32 Party_GetLeaderActor(void);
u32 Random16(void);

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
  Window_FillFromSceneFar(scanlineData + 2);

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
