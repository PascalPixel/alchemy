// Near miss, differ 36 of 226 (2026-10-18, written from the listing; ☀️ has
// no twin): the burst of 64 sparkles Func_080e807c's effect ends with. The
// resource is directory entry 0x1ea, FieldEffect_SparkleFade: adoption names it
// SparkleFade in system/resource/directory.s. What remains: the frame count
// takes r5 and the second loop's sprite r7, where the ROM has r7 and r5
// (separate block-scoped pointers 44, the count fixed in r7 93, other
// declaration orders 36); and reordered pairs where the native loads
// gAdventure's address, the second pool word and the mote's y before the
// constants beside them, copies the angle after them, and builds the 13 of
// the priority mask before the store before it.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "adventure.h"
#include "task.h"
#include "resource.h"
#include "effect_sprite.h"
#include "bg_scroll.h"
#include "frame.h"
#include "canvas_sprite.h"
#include "io_reg.h"

// One mote: a position and a velocity (BattleEffect_IntegrateVector3's vector)
// and a frame count, negative before it starts.
struct BurstMote {
  s32 x;
  s32 y;
  s32 z;
  s32 vx;
  s32 vy;
  s32 vz;
  s32 timer;
};

struct BurstWork {
  u8 unk0000[0x1000];
  struct EffectSprite sprites[64];
  struct BurstMote motes[64];
  u8 unk2100[0x880];
  s16 resource;
};

struct BurstEvent {
  u8 unk000[0xcac];
  s8 pending;
  u8 unkcad;
  u16 unkcae;
};

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Window_LoadGraphics(void);
s32 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(u32 entry);
u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
void Func_080eb298(struct EffectSprite *sprite, struct BurstMote *mote);
void BattleEffect_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);
void HeapSlot_Release(s32 slot);
void Func_080e807c(void);
extern char ResourceId_SparkleFade;

void Func_080e87c4(void) {
  struct BurstWork *work = Ram_HeapSlots->battleFxWork[0];
  struct BurstEvent *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *object = Object_GetById(gAdventure.currentOwner);
  struct BurstMote *mote;
  struct EffectSprite *sprite;
  s32 slot;
  s32 tile;
  s32 angle;
  s32 i;
  s32 frame;

  if (event->pending == 0)
    return;
  event->pending = 0;
  event->unkcad = 0;
  event->unkcae = 0;
  Task_Remove(Func_080e807c);
  WaitFrames(1);
  Sound_PlayFar(167);
  Iwram_FillWords((void *)0x06002000, 0x500, 0xf000f000);
  Window_LoadGraphics();
  {
    /* FAKEMATCH: ⚓️ builds the control word with a shift; stored as a
     * halfword constant it loads from the pool, which also places the pool
     * before the first loop (differ 68). */
    s32 control = 0x400;

    REG_BG0CNT = control;
  }
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  gCpuLoadDisplayEnabled = 1;
  WaitFrames(1);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                        work);
  slot = Resource_FindFreeEntry();
  tile = VramCache_Load(slot, 0x100, work);

  mote = work->motes;
  sprite = work->sprites;
  for (i = 0; i < 64; i++, sprite++, mote++) {
    angle = Random16();
    mote->x = object->x;
    mote->y = object->y + 0x100000;
    mote->z = object->z;
    Vector_AddPolarOffset(Random16() * 4 + 0x340000, angle, &mote->x);
    mote->vx = 0;
    mote->vy = -0x40000;
    mote->vz = 0;
    Vector_AddPolarOffset(Random16() * 3 + 0x20000, angle, &mote->vx);
    mote->timer = -i / 4;
    Func_080eaf98(sprite, 4, 4, 0, tile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
  }
  WaitFrames(1);

  for (frame = 0; frame < 40; frame++) {
    mote = work->motes;
    sprite = work->sprites;
    for (i = 63; i >= 0; i--, sprite++, mote++) {
      if (mote->timer >= 0) {
        sprite->oam.index = tile + (mote->timer & 3) * 2;
        /* FAKEMATCH: ⚓️ loads the index masks inside the loop; without the
         * barrier loop.c hoists 0xfffffc00 into r9 (differ 70; a goto loop
         * also keeps it, 36). */
        asm("");
        Func_080eb298(sprite, mote);
        BattleEffect_IntegrateVector3(&mote->x, 62, 0x8000);
      }
      mote->timer++;
    }
    WaitFrames(1);
  }
  Resource_ResetEntry(slot);
  Resource_ResetEntry(work->resource);
  Func_080eb930();
  HeapSlot_Release(92);
}
