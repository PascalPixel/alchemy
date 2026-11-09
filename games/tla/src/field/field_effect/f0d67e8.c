#include "types.h"
#include "battle.h"
#include "graphics.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "task.h"
#include "palette_buffers.h"
#include "field_blend.h"
#include "dma.h"
#include "io_reg.h"
#include "system.h"

void BattleEffect_ApplyColorToTargetBuffer(void *buffer, s32 mode);

s32 GameFlag_Test(s32 flag);
void Sound_PlayFar(s32 cue);

// Lightning: selects the palette according to the remaining duration.
void Func_080d67e8(void) {
  struct FieldBlendWork *work = Ram_HeapSlots->unk70[2];
  struct BattleEffectBuffers *target = Ram_HeapSlots->block32;
  s16 *timer = &work->timer;

  if (*timer < 0)
    return;
  if (GameFlag_Test(358))
    work->timer = 128;
  switch ((*timer)--) {
  case 0:
    if (work->enabled != 0) {
      u32 a = Random16();
      u32 b = Random16();
      u32 draw = a * 24;
      u32 jitter = b * 100;
      work->timer = (((draw + a) << 4) >> 16) - (jitter >> 16) + 150;
      if (work->loud != 0)
        Sound_PlayFar(172);
      else
        Sound_PlayFar(171);
    }
  case 5:
  case 10:
    BattleEffect_ApplyColorToTargetBuffer(work, 1);
    Dma_Set(work->delta, target->delta, 0x840002a0, REG_DMA3);
    target->duration = 12;
    target->step = 0;
    break;
  case 1:
  case 6:
  case 11:
    BattleEffect_ApplyColorToTargetBuffer(work->to, 1);
    target->duration = 1;
    target->step = 0;
    break;
  }
}
