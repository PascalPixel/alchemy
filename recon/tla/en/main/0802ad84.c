// Near miss, differ 294 (300 before statements reordered 2026-10-26) of 246 (2026-10-18), first pass: The Broken Seal's
// Map_UpdateLayerScroll for this game's layers: no phase masks or enabled
// flags, the view's vertical motion clamped to 16 pixels a frame, and the
// layer-window renderers 0802abbc and 0802ac90. Left: allocation; the native
// keeps the map work in r8, the layer in r6 and IwramMulQ16's address in r9,
// where this keeps the layer in r8 and the work in r7.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "iwram_call.h"
#include "bg_scroll.h"

u32 Random16(void);
void Func_0802abbc(u32 layer, s32 tileX, s32 tileY);
void Func_0802ac90(u32 layer, s32 tileX, s32 tileY);

struct MapLayerScroll {
  s32 x, y;
  s32 offsetX, offsetY;
  s32 scaleX, scaleY;
  s32 speedX, speedY;
  s32 phaseX, phaseY;
  u8 unk28[0x10];
};

void Func_0802ad84(void) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  struct MapLayerScroll *layer = (struct MapLayerScroll *)work->layers;
  s32 *origin = work->origin;
  s32 baseX;
  s32 baseY;
  s32 minX, minY, maxX, maxY;
  s32 shakeX;
  s32 shakeY;
  u32 layerIndex;

  if (origin == NULL)
    return;
  baseX = *origin++ - 0x780000;
  {
    s32 height = *origin++;

    baseY = *origin - height - 0x600000;
  }
  shakeX = work->shakeX;
  minX = work->minX + shakeX;
  maxX = work->maxX - shakeX - 0xf00000;
  shakeY = work->shakeY;
  minY = work->minZ + shakeY;
  maxY = work->maxZ - shakeY - 0xa00000;
  if (minX > maxX)
    maxX = minX;
  if (minY > maxY)
    maxY = minY;
  if (baseX < minX)
    baseX = minX;
  if (baseX > maxX)
    baseX = maxX;
  if (baseY < minY)
    baseY = minY;
  if (baseY > maxY)
    baseY = maxY;
  if (shakeX != 0) {
    s32 shakeDifference = Random16();

    shakeDifference -= Random16();
    baseX += Iwram_MulQ16(work->shakeX, shakeDifference);
    work->shakeX = Iwram_MulQ16(work->shakeX, work->shakeDecay);
  }
  if (work->shakeY != 0) {
    s32 shakeDifference = Random16();

    shakeDifference -= Random16();
    baseY += Iwram_MulQ16(work->shakeY, shakeDifference);
    work->shakeY = Iwram_MulQ16(work->shakeY, work->shakeDecay);
  }
  {
    s32 step = work->viewZ - baseY;

    if (step > 0x100000) {
      baseY = work->viewZ - 0x100000;
      step = work->viewZ - baseY;
    }
    if (step < -0x100000)
      baseY = work->viewZ + 0x100000;
  }
  work->viewX = baseX;
  work->viewZ = baseY;
  for (layerIndex = 0; layerIndex <= 2; layerIndex++) {
    s32 x = Iwram_MulQ16(work->viewX, layer->scaleX);
    s32 y = Iwram_MulQ16(work->viewZ, layer->scaleY);
    u32 tileX;
    u32 tileY;

    if (layer->speedX != 0) {
      layer->phaseX += layer->speedX;
      x += layer->phaseX;
    }
    if (layer->speedY != 0) {
      layer->phaseY += layer->speedY;
      y += layer->phaseY;
    }
    x += layer->offsetX;
    y += layer->offsetY;
    tileX = (u32)x >> 19;
    tileY = (u32)y >> 19;
    if ((layer->x ^ x) & 0x80000) {
      if (layer->x < x)
        Func_0802ac90(layerIndex, tileX + 30, tileY);
      else
        Func_0802ac90(layerIndex, tileX, tileY);
    }
    if ((layer->y ^ y) & 0x100000) {
      if (layer->y < y)
        Func_0802abbc(layerIndex, tileX, tileY + 20);
      else
        Func_0802abbc(layerIndex, tileX, tileY);
    }
    layer->x = x;
    layer->y = y;
    layer++;
    gBgScroll[3 - layerIndex].x = x >> 16;
    gBgScroll[3 - layerIndex].y = y >> 16;
  }
}
