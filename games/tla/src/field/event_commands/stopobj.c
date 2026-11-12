#include "types.h"
#include "task.h"
#include "frame.h"
#include "actor_view.h"
#include "ram_buffer.h"

struct EffectSceneWork {
  u8 reserved00[16];
  struct Actor *mainObject;
};

void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);

void BattleEffect_StopMainActor(void) {
  struct EffectSceneWork *work = Ram_HeapSlots->effectWork;

  BattleEffect_SetObjectUpdateMode(work->mainObject, 0);
  wait(1);
}

#include "actor.h"
#include "field_particle.h"
#include "field_sprite.h"
#include "map.h"

struct CleanupWork {
  u8 unk000[16];
  struct Actor *owner;
  u8 unk014[10];
  s16 animation;
  u8 unk020[36];
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  struct FieldParticle slots[24];
  u8 unk710[8];
  u8 ownerByte;
  u8 layer;
};

struct CleanupEventWork {
  u8 unk000[394];
  s8 mode;
  u8 unk18b[0xca5 - 395];
  s8 unkca5;
  u8 unkca6[7];
  s8 unkcad;
  u8 unkcae[8];
  s8 unkcb6;
  s8 unkcb7;
};

void BattleEffect_ClearOwnedSlot(struct FieldParticle *slot);
void Func_080dc954(void);
void BattleEffect_WaitForArcGroups(void);
void BattleEffect_EndArcEffect(void);
void Func_080dbb40(void);
void BattleEffect_UpdateAllEffectSlots(void);
void Event_NoOp(void);

void BattleEffect_CleanupSceneObjects(void) {
  struct CleanupWork *work = Ram_HeapSlots->effectWork;
  struct CleanupEventWork *event = Ram_HeapSlots->eventWork;
  s8 *block = Ram_HeapSlots->block32;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *owner = work->owner;
  s32 i;
  s16 first;

  for (i = 0; i < 24; i++) {
    struct FieldParticle *slot = &work->slots[i];
    if (slot->active)
      BattleEffect_ClearOwnedSlot(slot);
  }
  // A local that holds one constant: it makes the first test cmp #17; blt as
  // the native (with the literal, 4 of 171 differ in tla-en).
  first = 17;
  if (work->animation >= first &&
      (work->animation <= 27 || work->animation == 30))
    Func_080dc954();
  if (event->unkcb6 != 0)
    return;
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  if (event->mode == 3 && block != NULL) {
    i = 0;
    if (block[0x2a01] != 0) {
      do {
        wait(1);
        if (++i > 89)
          break;
      } while (block[0x2a01] != 0);
    }
  }
  event->unkcb7 = 0;
  Func_080dbb40();
  Task_Remove(BattleEffect_UpdateAllEffectSlots);
  map->shakeX = work->savedShakeX;
  map->shakeY = work->savedShakeY;
  map->shakeDecay = work->savedShakeDecay;
  if (owner != NULL) {
    owner->priorityFlags = work->ownerByte;
    ((struct FieldSprite *)owner->sprite)->priority = work->layer;
  }
  if (work->animation != 8)
    event->unkca5 = 1;
  if (work->animation != 28)
    event->unkcad = 1;
  Event_NoOp();
  HeapSlot_Release(224);
}

/// When the event work's mode byte at 0x18a is 3, copies it to the map
/// work's byte at 0x970.
void Func_080dc954(void) {
  struct HeapSlots *slots = Ram_HeapSlots;
  s32 mode = *(s8 *)((u8 *)slots->eventWork + 0x18a);

  if (mode == 3)
    *((u8 *)slots->mapWork + 0x970) = mode;
}

#include "heap_release.h"
#include "dma.h"

#define WAVE_FX_SLOT 88

struct WaveFxWork {
  u8 unk000[0x28b];
  s8 red;
  s8 green;
  s8 blue;
  u16 phase;
  u16 id;
  u16 value;
  u8 steps;
  u8 unk295;
  u8 unk296[2];
};

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_AdvanceHueCycle(void);
void BattleEffect_ApplyColorToTargetBuffer(s32 colors, s32 mode);
void BattleEffect_StartBufferInterpolation(s32 frames);
void Palette_FillNumber15Grey(void);
void Func_080dcb44(void);

/// Starts the wave effect's work in heap slot WAVE_FX_SLOT: a phase taken
/// from the frame count, id and value, then the shared scene, the first hue
/// step and its colour fade, with Func_080dcb44 run each frame.
void Func_080dc978(s32 id, s32 value) {
  struct WaveFxWork *work;

  work = HeapSlot_AllocEwram(WAVE_FX_SLOT, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  work->phase = (gFrameCount * 2) % 360;
  work->id = id;
  work->value = value;
  work->steps = 8;
  work->unk295 = 0;
  BattleEffect_InitializeSharedScene();
  BattleEffect_AdvanceHueCycle();
  BattleEffect_ApplyColorToTargetBuffer(
      (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
  BattleEffect_StartBufferInterpolation(8);
  Palette_FillNumber15Grey();
  Task_Add((TaskFunc)Func_080dcb44, TASK_PASS_VBLANK);
}

struct Actor;
struct Actor *Actor_GetById(s32 id);
void BattleEffect_ApplyFlickerValue(struct Actor *object, s32 value);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080dcadc(void);

void BattleEffect_FinishSceneAndReleaseHeapBlock(void) {
  u8 *work = Ram_HeapSlots->unk4c[3];

  Func_080dcadc();
  Task_Remove((TaskFunc)Func_080dcb44);
  BattleEffect_ApplyFlickerValue(Actor_GetById(*(u16 *)(work + 0x290)), 1);
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(WAVE_FX_SLOT);
}

#define BG_PLTT_COLOR(bank, color)                                             \
  (*(volatile u16 *)(0x05000000 + ((bank) * 16 + (color)) * 2))
#define GREY28 0x739c

void BattleEffect_ArmBg0HBlankDma(void);

/// Gray BG palette 15 colors 1 and 3-12, then start line scrolling.
void Palette_FillNumber15Grey(void) {
  volatile u16 *p;

  Ram_HeapSlots->windowTiles[4] = 1;
  p = &BG_PLTT_COLOR(15, 1);
  *p = GREY28;
  p += 2;
  *p = GREY28;
  p += 8;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p -= 8;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  Task_Add((TaskFunc)BattleEffect_ArmBg0HBlankDma, TASK_PASS_FRAME - 20);
}

#include "main_routine_work.h"

void Func_08038348(s32 first, s32 second);

/// Stops line scrolling and restores background palette 15, as The
/// Broken Seal's FieldPsynergy_RestoreMindReadPalette does.
void Func_080dcadc(void) {
  u8 *work = Ram_HeapSlots->windowTiles;

  Task_Remove((TaskFunc)BattleEffect_ArmBg0HBlankDma);
  BG_PLTT_COLOR(15, 1) = 0x7fff;  // White.
  BG_PLTT_COLOR(15, 3) = 0;       // Black.
  BG_PLTT_COLOR(15, 11) = 0x294a; // Gray (10).
  BG_PLTT_COLOR(15, 12) = 0x5294; // Gray (20).
  Func_08038348(gMainRoutineWork.unk220[5], gMainRoutineWork.unk220[6]);
  work[4] = 0;
}
