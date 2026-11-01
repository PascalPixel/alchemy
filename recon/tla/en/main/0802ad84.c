// Draft, differ 35 of 246, skeleton gap 5 (tla-en, 2026-11-01)
// The Broken Seal's MapGrid_UpdateLayerScroll for this game's layers: no phase
// masks or enabled flags, the view's vertical motion clamped to 16 pixels a
// frame, and the layer-window renderers 0802abbc and 0802ac90. Allocation
// agrees; the differ is mostly pool and branch offsets shifted by the extra
// bl. Two gaps are left, both from scheduling:
// - Before the loop the native orders the two view stores, then 0, r11, r8.
//   That needs a barrier after the stores, with the loop's view-X address a
//   hoisted copy of the store's. A one-pass block round the stores gives it
//   (gap 3, differ 33), but that is a device; no plain form found yet.
// - The native shares 0802ac90's bl between its two arms and stops at the
//   str r4. Here cross-jumping merges only the ldr r4, stopped by the USE nop
//   flow.c puts after the else arm's call; the native must have that USE
//   between the str and the bl. Its cause is unknown: loop form, a u32
//   tile x, and one-pass blocks round the arms or the whole if did not help.
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
  struct MapWork *work;
  struct MapLayerScroll *layer;
  s32 *origin;
  s32 x, y;
  s32 minX, minY, maxX, maxY;
  s32 baseX, baseY;
  u32 layerIndex;

  work = Ram_HeapSlots->mapWork;
  origin = work->origin;
  layer = (struct MapLayerScroll *)work->layers;
  if (origin == NULL)
    return;
  baseX = *origin++ - 0x780000;
  {
    s32 height = *origin++;

    baseY = *origin - height - 0x600000;
  }
  minX = work->minX + work->shakeX;
  maxX = work->maxX - work->shakeX - 0xf00000;
  minY = work->minZ + work->shakeY;
  maxY = work->maxZ - work->shakeY - 0xa00000;
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
  if (work->shakeX != 0) {
    s32 shakeRoll = Random16();

    shakeRoll -= Random16();
    baseX += Iwram_MulQ16(work->shakeX, shakeRoll);
    work->shakeX = Iwram_MulQ16(work->shakeX, work->shakeDecay);
  }
  if (work->shakeY != 0) {
    s32 shakeRoll = Random16();

    shakeRoll -= Random16();
    baseY += Iwram_MulQ16(work->shakeY, shakeRoll);
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
  for (layerIndex = 0; layerIndex <= 2; layerIndex++, layer++) {
    baseX = Iwram_MulQ16(work->viewX, layer->scaleX);
    baseY = Iwram_MulQ16(work->viewZ, layer->scaleY);
    if (layer->speedX != 0) {
      layer->phaseX += layer->speedX;
      baseX += layer->phaseX;
    }
    if (layer->speedY != 0) {
      layer->phaseY += layer->speedY;
      baseY += layer->phaseY;
    }
    baseX += layer->offsetX;
    baseY += layer->offsetY;
    x = (u32)baseX >> 19;
    y = (u32)baseY >> 19;
    if ((layer->x ^ baseX) & 0x80000) {
      if (layer->x < baseX)
        Func_0802ac90(layerIndex, x + 30, y);
      else
        Func_0802ac90(layerIndex, x, y);
    }
    if ((layer->y ^ baseY) & 0x100000) {
      if (layer->y < baseY)
        Func_0802abbc(layerIndex, x, y + 20);
      else
        Func_0802abbc(layerIndex, x, y);
    }
    layer->x = baseX;
    layer->y = baseY;
    gBgScroll[3 - layerIndex].x = baseX >> 16;
    gBgScroll[3 - layerIndex].y = baseY >> 16;
  }
}
