// Draft, differ 197 of 563, skeleton gap 83 (tla-en, 2026-11-01)
// Starts the ring of Func_080e807c round the caster. Unless the ring is
// already up (the event work's byte at 0xcac), it clears BG0's map and a
// 0x2000-byte picture on BG0 (heap slot 96), and for 64 frames sparkles
// (FieldEffect_StarSparkleFade) rise from two points that circle the caster
// 55 pixels out, half a turn apart, while from frame 32 four rings of radius
// 52 to 55 are drawn into the picture (Func_080e7f94) as the circle turns;
// then FieldEffect_RingArcs replaces the picture and 32 motes (resource
// 0x1ed, OBJ palette 15) start orbiting 64 pixels out from the caster, one
// frame apart. Either way the ring is marked up for 600 frames (0xcae) and
// Func_080e807c runs as a frame callback.
// Left: the instruction count is 3 short and almost all of the gap is
// reordered pairs in sched2. IwramFillWords and 0x2000 are issued early, ahead
// of calls, because the draft's address is a constant where the listing's
// pool holds a symbol (the same as 0817e698). The one real difference: the
// native copies the position pointer from its spill (sp 4) to r8 for the
// motes' loop; a copy (s32 *place = pos, or = position) is removed by gcse's
// copy propagation, so the loop reloads sp 4 (about 6 rows of the gap). A
// mismatch that remains in allocation: the native keeps the timer in r8 and
// the shared counter i in r10, here the other way round; a counter of its own
// for the rings' loop is worse (57 more differ). Evidence that shaped this:
// the frame's spill order is the declaration order (pos last); a halfword
// zero is what the pool's 0 is, and it crosses Task_Add and wait, so `off`;
// BG0CNT's value is set before its address so r3 keeps the DMA channel and
// reload turns the address into subs r3, #204 (`control`, two statements);
// the 999 store is a struct field, whose constant is split, where a cast
// pointer loads it from the pool; the setup loop's sprite is an index, its
// mote a pointer stepped after i, or -13 is hoisted out of the loop.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"
#include "canvas_sprite.h"
#include "bg_scroll.h"
#include "frame.h"
#include "dma.h"
#include "iwram_call.h"

struct RingMote {
  s32 x;
  s32 y;
  s32 z;
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 timer;
};

struct RingWork {
  u8 tiles[0x1000];
  struct EffectSprite sparkSprites[64];
  struct RingMote sparks[64];
  struct EffectSprite sprites[32];
  struct RingMote motes[32];
  u16 entry;
  u16 tile;
  s8 sparkling;
  s8 orbiting;
  s8 cycling;
  u8 unk2987;
  u16 phase;
};

struct RingEvent {
  u8 unk000[0xcac];
  s8 up;
  s8 unkcad;
  s16 timer;
};

struct FxScene {
  u16 facing;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
};

extern char ResourceId_StarSparkleFade, ResourceId_RingArcs, ResourceId_Data1ED;

void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void BattleEffect_InitializeSharedScene(void);
void Camera_WorldToScreen(s32 *position);
void wait(s32 frames);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
void Task_Add(void (*func)(void), s32 order);
void Func_080e807c(void);
void Func_080e7f94(u8 *canvas, s32 radius, s32 angle, s32 color);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct RingMote *mote);
u32 Random16(void);
void BattleEffect_PrepareBufferInterpolation(void);

void Func_080e82cc(void) {
  struct RingWork *work = HeapSlot_AllocEwram(92, 0x298c);
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  struct RingEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *caster = scene->mainObject;
  struct CanvasSpriteWork *canvas;
  s32 entry;
  s32 tile;
  s32 spin;
  s32 angle;
  s32 repeat;
  s32 drawing;
  s32 *pos;
  struct EffectSprite *sprite;
  struct RingMote *mote;
  s32 position[3];
  s16 off; // Cycling is switched off through a halfword zero.
  s32 phase;
  s32 timer;
  s32 radius;
  s32 i;
  u8 *picture;

  spin = 0;
  angle = 0;
  repeat = 1;
  drawing = 0;
  if (event->up == 0) {
    BattleEffect_InitializeSharedScene();
    gCpuLoadDisplayEnabled = angle;
    Func_080eb824();
    canvas = Ram_HeapSlots->unkec[1];
    canvas->mode = angle;
    pos = position;
    pos[0] = scene->mainObject->x;
    pos[1] = scene->mainObject->y + 0x100000;
    pos[2] = scene->mainObject->z;
    Camera_WorldToScreen(pos);
    {
      s32 x = *((s16 *)pos + 1);
      s32 y;

      gBgScroll[0].x = 64 - x;
      y = *((s16 *)pos + 5);
      gBgScroll[0].y = 64 - y;
    }
    Iwram_FillWords((void *)0x06002000, 0x800, 0);
    HeapSlot_Release(96);
    picture = HeapSlot_AllocEwram(96, 0x2000);
    Iwram_FillWords(picture, 0x2000, 0);
    wait(1);
    Dma_Set(picture, (void *)0x06000000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x800,
            REG_DMA3);
    {
      s32 control;

      control = 0x480;
      REG_BG0CNT = control;
    }
    Resource_DecodeType01(
        Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
    entry = Resource_FindFreeEntry();
    tile = VramCache_Load(entry, 0x100, work);
    mote = work->sparks;
    for (i = 0; i < 64; i++, mote++) {
      sprite = &work->sparkSprites[i];
      Func_080eaf98(sprite, 4, 4, 0, tile);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = 0;
      sprite->depth = 240;
      mote->timer = -i / 4;
    }
    work->sparkling = 0;
    work->orbiting = 0;
    work->cycling = 1;
    work->phase = 0;
    event->up = 1;
    event->unkcad = 1;
    event->timer = 999;
    off = 0;
    Task_Add(Func_080e807c, 0x480);
    wait(1);
    work->cycling = off;
    phase = 0;
    timer = 0;
    do {
      if (drawing) {
        radius = spin;
        for (i = 7; i >= 0; i--) {
          Func_080e7f94(picture, 0x340000, radius, 243);
          Func_080e7f94(picture, 0x350000, radius, 247);
          Func_080e7f94(picture, 0x360000, radius, 251);
          Func_080e7f94(picture, 0x370000, radius, 255);
          radius += 128;
        }
        canvas->command = CANVAS_UPLOAD_TO_BG;
      }
      mote = work->sparks;
      for (i = 0; i < 64; i++, mote++) {
        sprite = &work->sparkSprites[i];
        if (mote->timer == 0) {
          s32 heading = angle + ((i & 1) << 15);

          mote->x = caster->x;
          mote->y = caster->y + 0x100000;
          mote->z = caster->z;
          Vector_AddPolarOffset(0x370000, heading, &mote->x);
          Vector_AddPolarOffset(Random16() * 6, Random16(), &mote->x);
        }
        if ((u32)mote->timer <= 15) {
          sprite->oam.index = tile + (mote->timer & 3) * 2;
          EffectSprite_DrawAtWorldPosition(sprite, mote);
          mote->y += -0x10000;
        }
        mote->timer++;
        if (repeat && mote->timer == 16)
          mote->timer = 0;
      }
      switch (phase) {
      case 0:
        angle = timer << 10;
        if (timer == 32) {
          phase = 1;
          timer = -1;
        }
        break;
      case 1:
        drawing = 1;
        {
          s32 turn = timer << 10;

          spin = turn;
          angle = turn;
        }
        if (timer == 32) {
          timer = -1;
          phase = 2;
        }
        break;
      case 2:
        repeat = 0;
        if (timer == 16)
          phase = 999;
        break;
      }
      wait(1);
      timer++;
    } while (phase != 999);

    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_RingArcs),
                          picture);
    Dma_Set(picture, (void *)0x06000000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x800,
            REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Data1ED),
                          work);
    {
      s32 heading;
      s32 moteEntry = Resource_FindFreeEntry();
      s32 moteTile = VramCache_Load(moteEntry, 0x200, work);

      work->entry = moteEntry;
      work->tile = moteTile;
      work->sparkling = 1;
      work->orbiting = 1;
      mote = work->motes;
      sprite = work->sprites;
      for (i = 0; i < 32; i++) {
        heading = Random16();
        Func_080eaf98(sprite, 8, 8, 0x40000000, moteTile);
        sprite->depth = 240;
        sprite->oam.priority = 0;
        sprite->oam.palette = 15;
        sprite->oam.palette256 = 0;
        sprite->angle = heading + 0x4000;
        pos[0] = scene->mainObject->x;
        pos[1] = scene->mainObject->y + 0x100000;
        pos[2] = scene->mainObject->z;
        sprite++;
        Camera_WorldToScreen(pos);
        Vector_AddPolarOffset(0x400000, heading, pos);
        mote->x = pos[0];
        mote->y = pos[2];
        mote->timer = -i;
        mote++;
      }
    }
    Resource_ResetEntry(entry);
    BattleEffect_PrepareBufferInterpolation();
  }
  work->cycling = 1;
  event->up = 1;
  event->unkcad = 1;
  event->timer = 600;
  Task_Add(Func_080e807c, 0x480);
}
