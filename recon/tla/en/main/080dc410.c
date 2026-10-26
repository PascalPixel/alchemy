// Near miss, differ 2 of 239 (34 before; 2026-10-26): the event work is read
// through a struct, so its byte stores no longer make the animation reload
// (through u8 indexing they alias it), the three shake values are copied in
// field order, and a tagged once-only block keeps the work's copy right after
// the allocation. Left: after the shake x store the native builds the map id
// offset's second half before it loads the shake y (the store-then-load
// pattern no C form reproduces; no single barrier or once-only block helps).
// Earlier, differ 34 of 239 (2026-10-18): BattleEffect_LoadActionEffectResources,
// from ☀️'s function of the same name (games/tbs/src/object/psynergy_effects.c)
// with ⚓️'s 0x7f4-byte effect work and seven lit scenes (SceneId_ symbols:
// the listing loads each id from the pool). Left: the native loads the event
// and map work before it builds the resolving-flag offset, and each scene id
// before it shifts the map; the pool sits one row earlier (b over it after
// adds r3, #33); and the native reads the animation signed and unsigned at
// once and tests the unsigned copy against 28 after the event store, where
// this reloads it (a held u16 or s32 copy behind an empty asm moves every
// register, differ 132). The map is kept in r1 across the first lit store
// only when that store is a field and the later ones raw bytes (all fields:
// differ 53, all bytes: differ 50).
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "party_state.h"
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
  if (gPartyState.map == (s32)&SceneId_Ov673Room5)
    work->lit = 1;
  if (gPartyState.map == (s32)&SceneId_Ov68fRoom1)
    *((u8 *)work + 0x41) = 1;
  if (gPartyState.map == (s32)&SceneId_Ov68fRoom2)
    *((u8 *)work + 0x41) = 1;
  if (gPartyState.map == (s32)&SceneId_Ov68fRoom7)
    *((u8 *)work + 0x41) = 1;
  if (gPartyState.map == (s32)&SceneId_Ov6a4)
    *((u8 *)work + 0x41) = 1;
  if (gPartyState.map == (s32)&SceneId_Ov6a5)
    *((u8 *)work + 0x41) = 1;
  if (gPartyState.map == (s32)&SceneId_Ov6A6Room1)
    *((u8 *)work + 0x41) = 1;
  BattleEffect_SetupObjectPair(gPartyState.currentOwner, -1);
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
