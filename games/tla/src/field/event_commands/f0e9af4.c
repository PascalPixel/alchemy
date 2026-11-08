#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "task.h"
#include "resource.h"
#include "heap_release.h"
#include "effect_sprite.h"
#include "battle.h"

// Kind 24's work (battle_fx_work[0]), as Func_080e9940's draft has it.
struct GlowWork {
  u8 unk0000[0x1000];
  struct EffectSprite sprite;
  s32 scale;
  s16 resource;
  u16 tile;
  u16 unk1030;
  u16 phase;
  s16 fading;
};

struct GlowScene {
  u8 unk00[0x10];
  struct Actor *mainObject;
  u8 unk14[4];
  s16 unk18;
  s16 unk1a;
  u8 unk1c[2];
  s16 kind;
};

struct GlowEvent {
  u8 unk000[0x14];
  struct Actor *objects[80];
  u8 unk154[0xbf0];
  s16 glow;
  s16 glowHeld;
};

extern char ResourceId_RingOrb;

void wait(s32 frames);
s32 Func_080dbcc0(s32 x, s32 z, s32 layer);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);
void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);
s32 Resource_ResetEntry(u32 entry);
void HeapSlot_Release(s32 slot);
void Func_080e9940(void);

void Func_080e9af4(void) {
  struct GlowWork *work = HeapSlot_Alloc(92, 0x1038);
  struct GlowScene *scene = Ram_HeapSlots->effectWork;
  struct GlowEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *player = scene->mainObject;
  struct PendingEvent *pending;
  struct EffectSprite *sprite;
  struct Actor *other;
  s32 position[3];
  s32 found;
  s32 slot;
  struct Actor **list;
  s32 tile;
  s32 i;
  s32 n;

  pending = Func_080ce458(0x50000005, scene->kind, Func_080ce31c(scene->kind));
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_RingOrb), work);
  slot = Resource_FindFreeEntry();
  tile = VramCache_Load(slot, 0x800, work);
  work->resource = slot;
  work->tile = tile;
  work->scale = 0;
  sprite = &work->sprite;
  Func_080eaf98(sprite, 15, 15, 0x80000000, (s16)work->tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  Actor_SetPositionAndResetMotionFar(player, player->x, player->y, player->z);
  BattleEffect_SetObjectUpdateMode(player, 0);
  Actor_SetMode(player, 5);
  Sound_PlayFar(130);
  wait(1);
  work->unk1030 = 0;
  work->phase = 0;
  work->fading = 0;
  Task_Add(Func_080e9940, TASK_PASS_VBLANK);

  found = 0;
  if (Func_080dbcc0(player->x, player->z, player->terrainId) == 0) {
    found = 1;
  } else {
    for (n = 0; n < 80; n++) {
      other = event->objects[n];
      if (other != NULL && other->script != NULL &&
          (other->collisionFlags & 1) && other != scene->mainObject) {
        position[0] = other->x;
        position[1] = other->terrainHeight;
        position[2] = other->z;
        if (Func_080dbe80(&player->x, player->radius, &other->x,
                          other->radius - 2) >= 0) {
          found = 1;
          break;
        }
      }
    }
  }

  if (found) {
    wait(5);
    Sound_PlayFar(246);
    for (i = 0; i < 32; i++) {
      work->scale = i << 11;
      wait(1);
    }
    wait(10);
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, scene->unk18, scene->unk1a);
    for (i = 31; i != 0; i--) {
      work->scale = i << 11;
      wait(1);
    }
    wait(5);
    Task_Remove(Func_080e9940);
    Resource_ResetEntry(work->resource);
    HeapSlot_Release(92);
  } else {
    player->motionFlags = 0;
    player->y = player->terrainHeight;
    work->scale = 0;
    event->glowHeld = 0;
    event->glow = 100;
    for (i = 0; i < 32; i++) {
      if (i == 10)
        Sound_PlayFar(246);
      player->y += 0x10000;
      work->scale = i << 11;
      wait(1);
    }
    player->motionFlags = 4;
    player->y = player->terrainHeight + 0x248000;
    player->gravity = 0x30000;
    gMainRoutineWork.renderMode = 10;
    work->fading = 1;
    event->glowHeld = 1;
    event->glow = 112;
  }
}
