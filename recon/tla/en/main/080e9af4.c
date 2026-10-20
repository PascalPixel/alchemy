// Near miss, differ 10 of 318 (2026-10-18, written from the listing; ☀️ has
// no twin): field Psynergy kind 24, the glow Func_080e9940 draws each frame.
// Two tagged empty asm barriers in the fall bring 16 to 10; what remains is
// the sprite's address built before the scale's store (an empty asm there
// fixes it but reorders the opening, 18; a one-pass block 14), the search
// counter's save among the position stores, and the glow's store and the
// motion flags pointer's copy after the scale's address. The resource is
// directory entry 0x1da, FieldFx_RingOrb: adoption names it RingOrb in
// system/resource/directory.s. The object search has a counter of its own, as
// the ROM keeps it in r4 and saves it around the call (one i for every loop
// takes r5, 38 -> 20); the slot is stored before the tile (46 -> 38), the
// countdown is a for loop (20 -> 18) and the list is indexed (18 -> 16, order
// in four places, before the barriers). alchemy-devtools permute (5 minutes, before the
// barriers): 14 with a one-pass block around the three stores after the fall.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "event_work.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
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
  struct ObjectRuntime *mainObject;
  u8 unk14[4];
  s16 unk18;
  s16 unk1a;
  u8 unk1c[2];
  s16 kind;
};

struct GlowEvent {
  u8 unk000[0x14];
  struct ObjectRuntime *objects[80];
  u8 unk154[0xbf0];
  s16 glow;
  s16 glowHeld;
};

extern char ResourceId_RingOrb;

void WaitFrames(s32 frames);
s32 Func_080dbcc0(s32 x, s32 z, s32 layer);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void BattleFx_SetObjectTickMode(struct ObjectRuntime *object, s32 mode);
s32 Resource_ResetEntry(u32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_080e9940(void);

void Func_080e9af4(void) {
  struct GlowWork *work = Runtime_AllocateHeapBlock(92, 0x1038);
  struct GlowScene *scene = Ram_HeapSlots->effectWork;
  struct GlowEvent *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *player = scene->mainObject;
  struct PendingEvent *pending;
  struct EffectSprite *sprite;
  struct ObjectRuntime *other;
  s32 position[3];
  s32 found;
  s32 slot;
  struct ObjectRuntime **list;
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
  Object_SetPositionAndResetMotionFar(player, player->x, player->y, player->z);
  BattleFx_SetObjectTickMode(player, 0);
  Object_SetMode(player, 5);
  Audio_PlayCue(130);
  WaitFrames(1);
  work->unk1030 = 0;
  work->phase = 0;
  work->fading = 0;
  Scheduler_AddOrUpdateCallback(Func_080e9940, 0x480);

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
    WaitFrames(5);
    Audio_PlayCue(246);
    for (i = 0; i < 32; i++) {
      work->scale = i << 11;
      WaitFrames(1);
    }
    WaitFrames(10);
    if (pending != NULL)
      EventRuntime_RunPendingEvent(pending, scene->unk18, scene->unk1a);
    for (i = 31; i != 0; i--) {
      work->scale = i << 11;
      WaitFrames(1);
    }
    WaitFrames(5);
    Scheduler_RemoveCallback(Func_080e9940);
    Resource_ResetEntry(work->resource);
    Runtime_ReleaseHeapBlock(92);
  } else {
    player->motionFlags = 0;
    player->y = player->terrainHeight;
    work->scale = 0;
    event->glowHeld = 0;
    event->glow = 100;
    /* FAKEMATCH: ⚓️ builds the scale's address after the glow's constant;
     * plainly agscc starts it among the stores before (differ 2). */
    asm("");
    for (i = 0; i < 32; i++) {
      if (i == 10)
        Audio_PlayCue(246);
      player->y += 0x10000;
      work->scale = i << 11;
      WaitFrames(1);
    }
    player->motionFlags = 4;
    player->y = player->terrainHeight + 0x248000;
    player->gravity = 0x30000;
    /* FAKEMATCH: ⚓️ starts the render mode's offset after the gravity store;
     * plainly agscc starts it before (differ 4). */
    asm("");
    gPartyState.renderMode = 10;
    work->fading = 1;
    event->glowHeld = 1;
    event->glow = 112;
  }
}
