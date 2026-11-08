/// @file
/// The burst of 64 sparkles that Func_080e807c's effect ends with: once the
/// event work asks for it, it plays sound 167, fills the tiles at 0x06002000,
/// reloads the window graphics, loads
/// the sparkle picture (FieldEffect_SparkleFade) and scatters the sparkles
/// around the current owner, starting a quarter of them a frame later than
/// the last, then moves and animates them for 40 frames before releasing the
/// effect.

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
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

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void Window_LoadGraphics(void);
s32 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(u32 entry);
u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct BurstMote *mote);
void BattleEffect_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);
void HeapSlot_Release(s32 slot);
void Func_080e807c(void);
extern char ResourceId_SparkleFade;

void Func_080e87c4(void) {
  struct BurstWork *work = Ram_HeapSlots->battleFxWork[0];
  struct BurstEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
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
  wait(1);
  // A once-only block: its end holds the order around it (as plain
  // statements, 6 of 226 differ in tla-en).
  do {
    Sound_PlayFar(167);
    {
      s32 (*fill)(void *, s32, u32);
      // A once-only block: the fill's entry loads ahead of the size (as a
      // direct Iwram_FillWords call, 2 of 226 differ).
      do {
      } while (0);
      fill = Iwram_FillWords;
      fill((void *)0x06002000, 0x500, 0xf000f000);
    }
    Window_LoadGraphics();
  } while (0);
  {
    // A local that holds one constant: the control word is built with a
    // shift; stored as a constant it loads from the pool (60 of 226 differ).
    s32 control = 0x400;

    REG_BG0CNT = control;
  }
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  gCpuLoadDisplayEnabled = 1;
  wait(1);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                        work);
  slot = Resource_FindFreeEntry();
  tile = VramCache_Load(slot, 0x100, work);

  i = 0;
  mote = work->motes;
  sprite = work->sprites;
  for (; i < 64; i++, sprite++, mote++) {
    angle = Random16();
    mote->x = actor->x;
    mote->y = actor->y + 0x100000;
    mote->z = actor->z;
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
  wait(1);

  for (frame = 0; frame < 40; frame++) {
    struct EffectSprite *spark;
    mote = work->motes;
    spark = work->sprites;
    for (i = 63; i >= 0; i--, spark++, mote++) {
      /* FAKEMATCH(barrier): the index masks loaded inside the loop, where
       * loop.c would hoist 0xfffffc00 into r9; as no asm, 36 of 226
       * instructions differ in tla-en. */
      asm("");
      if (mote->timer >= 0) {
        spark->oam.index = tile + (mote->timer & 3) * 2;
        EffectSprite_DrawAtWorldPosition(spark, mote);
        BattleEffect_IntegrateVector3(&mote->x, 62, 0x8000);
      }
      mote->timer++;
    }
    wait(1);
  }
  Resource_ResetEntry(slot);
  Resource_ResetEntry(work->resource);
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(92);
}
