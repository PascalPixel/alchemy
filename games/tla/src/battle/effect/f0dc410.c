#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "adventure.h"
#include "scene_ids.h"
#include "iwram_call.h"
#include "battle_unit.h"

struct EffectWork {
  u8 unk000[0x1c];
  u16 action;
  s16 animation;
  s8 enabled;
  u8 mode;
  s8 visible;
  s8 active;
  u8 unk024[0x1d];
  u8 lit;
  u16 freeBlocks;
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  u8 unk050[0x6c2];
  s8 messageMode;
  u8 unk713;
  u8 unk714[0xb0];
  s16 unk7c4;
  s16 unk7c6;
  u8 unk7c8[0x2a];
  s16 unk7f2;
};

struct LoadEventWork {
  u8 unk000[0xca5];
  u8 unkca5;
  u8 unkca6[7];
  u8 unkcad;
  u8 unkcae[8];
  s8 unkcb6;
  u8 unkcb7[0x79];
  u8 unkd30;
  s8 target;
  s8 targetY;
  s8 targetZ;
  u8 unkd34[4];
  u8 unkd38;
};

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
struct BattleAct *BattleAction_Get(s32 actionId);
s32 ResourceTable_CountFreeBlocks(void);
void BattleEffect_SetupObjectPair(s32 userId, s32 targetId);
void BattleEffect_StartArcEffect(void);
void BattleEffect_UpdateAllEffectSlots(void);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);

#define HEAP_SLOT_FX 224

void BattleEffect_LoadActionEffectResources(s32 abilityId, s32 mode) {
  struct LoadEventWork *event = Ram_HeapSlots->eventWork;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct EffectWork *work;
  s32 running;

  if (event->unkcb6 == 0) {
    work = Runtime_AllocateHeapBlock(HEAP_SLOT_FX, sizeof(*work));
    /* FAKEMATCH(one-pass): an empty once-only block, so the work's copy is
     * made at once after the allocation; as plain statements, 4 of 239
     * instructions differ in tla-en. */
    do {
    } while (0);
    Iwram_FillWords(work, sizeof(*work), 0);
  } else {
    work = Ram_HeapSlots->effectWork;
  }
  work->action = abilityId;
  work->animation = BattleAction_Get(abilityId)->unk04[2];
  running = event->unkcb6;
  if (running != 0)
    return;
  work->freeBlocks = 512 - ResourceTable_CountFreeBlocks();
  work->mode = mode;
  work->visible = 1;
  work->enabled = 1;
  work->active = 1;
  work->messageMode = 1;
  work->unk713 = 0;
  work->unk7c4 = running;
  work->unk7c6 = -1;
  work->unk7f2 = running;
  work->savedShakeX = map->shakeX;
  work->savedShakeY = map->shakeY;
  work->savedShakeDecay = map->shakeDecay;
  if (gAdventure.map == (s32)&SceneId_Ov673Room5)
    work->lit = 1;
  if (gAdventure.map == (s32)&SceneId_Ov68fRoom1)
    *((u8 *)work + 0x41) = 1;
  if (gAdventure.map == (s32)&SceneId_Ov68fRoom2)
    *((u8 *)work + 0x41) = 1;
  if (gAdventure.map == (s32)&SceneId_Ov68fRoom7)
    *((u8 *)work + 0x41) = 1;
  if (gAdventure.map == (s32)&SceneId_Ov6a4)
    *((u8 *)work + 0x41) = 1;
  if (gAdventure.map == (s32)&SceneId_Ov6a5)
    *((u8 *)work + 0x41) = 1;
  if (gAdventure.map == (s32)&SceneId_Ov6A6Room1)
    *((u8 *)work + 0x41) = 1;
  BattleEffect_SetupObjectPair(gAdventure.currentOwner, -1);
  if (work->animation != 8)
    event->unkca5 = 0;
  if (work->animation != 28)
    event->unkcad = 0;
  event->target = 0;
  event->targetY = 0;
  event->targetZ = 0;
  event->unkd38 = 0;
  event->unkd30 = 255;
  BattleEffect_StartArcEffect();
  Scheduler_AddOrUpdateCallback(BattleEffect_UpdateAllEffectSlots, 0x480);
}
