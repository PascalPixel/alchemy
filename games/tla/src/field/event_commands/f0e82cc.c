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
#include "resource.h"
#include "resource.h"
s32 Task_Add(void (*func)(void), s32 order);
void Func_080e807c(void);
void Func_080e7f94(u8 *canvas, s32 radius, s32 angle, s32 color);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void EffectSprite_DrawAtWorldPosition(struct ScreenSprite *sprite,
                                      s32 *position);
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
    {
      s32 (*fill)(void *, s32, u32);

      // An empty once-only loop, and the fill routine's entry in a local, load
      // the entry ahead of the size's constant; as a direct Iwram_FillWords
      // call, 10 of 563 instructions differ in tla-en.
      do {
      } while (0);
      fill = Iwram_FillWords;
      fill((void *)0x06002000, 0x800, 0);
    }

    // The picture's release and allocation in a once-only block, which ends
    // scheduling at both sides; without it, 8 of 563 instructions differ in
    // tla-en.
    do {
      HeapSlot_Release(96);
      picture = HeapSlot_AllocEwram(96, 0x2000);
    } while (0);
    Iwram_FillWords(picture, 0x2000, 0);

    wait(1);
    Dma_Set(picture, (void *)0x06000000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x800,
            REG_DMA3);
    {
      s32 control;

      control = 0x480;
      // An empty once-only loop keeps the register's address after the
      // control value; without it, 2 of 563 instructions differ in tla-en.
      do {
      } while (0);
      REG_BG0CNT = control;
    }
    Resource_DecodeType01(
        Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
    entry = Resource_FindFreeEntry();
    tile = VramCache_Load(entry, 0x100, work);
    for (i = 0, mote = work->sparks; i < 64; i++, mote++) {
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
        for (i = 0; i < 8; i++) {
          Func_080e7f94(picture, 0x340000, radius, 243);
          Func_080e7f94(picture, 0x350000, radius, 247);
          Func_080e7f94(picture, 0x360000, radius, 251);
          Func_080e7f94(picture, 0x370000, radius, 255);
          radius += 128;
        }
        canvas->command = CANVAS_UPLOAD_TO_BG;
      }
      for (i = 0, mote = work->sparks; i < 64; i++, mote++) {
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
          phase = 2;
          timer = -1;
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
      s32 *place;

      work->entry = moteEntry;
      work->tile = moteTile;
      work->sparkling = 1;
      work->orbiting = 1;
      for (i = 0, mote = work->motes; i < 32; i++) {
        heading = Random16();
        sprite = &work->sprites[i];
        Func_080eaf98(sprite, 8, 8, 0x40000000, moteTile);
        sprite->depth = 240;
        sprite->oam.priority = 0;
        sprite->oam.palette256 = 0;
        sprite->oam.palette = 15;
        sprite->angle = heading + 0x4000;
        place = position;
        place[0] = scene->mainObject->x;
        place[1] = scene->mainObject->y + 0x100000;
        place[2] = scene->mainObject->z;
        Camera_WorldToScreen(place);
        Vector_AddPolarOffset(0x400000, heading, place);
        mote->x = place[0];
        mote->y = place[2];
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

#include "main_routine_work.h"
#include "task.h"
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
void Window_LoadGraphics(void);
void BattleEffect_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);
extern char ResourceId_SparkleFade;

/// The burst of 64 sparkles that Func_080e807c's effect ends with: once the
/// event work asks for it, it plays sound 167, fills the tiles at 0x06002000,
/// reloads the window graphics, loads
/// the sparkle picture (FieldEffect_SparkleFade) and scatters the sparkles
/// around the current owner, starting a quarter of them a frame later than
/// the last, then moves and animates them for 40 frames before releasing the
/// effect.
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
      /* FAKEMATCH(barrier): an empty asm that opens the loop's body, which
       * keeps the index's 0xfffffc00 mask loaded inside the loop rather than
       * hoisted to r9; without it, 36 of 226 instructions differ in tla-en. */
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

#include "event_work.h"
#include "map.h"
#include "view_move.h"
#include "version.h"

// How far the view stays from the edge it is beside, in pixels.
#define VIEW_EDGE_MARGIN 12

// A map's view bounds, in 8-pixel units.
struct MapBounds {
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
};

void Func_08020328(s32 map, struct MapBounds *bounds);
void Map_SetScreenShakeFar(s32 shakeX, s32 shakeY, s32 decay);

// Opens the whole map to the view: keeps the view centre's position and the
// map's view bounds for Func_080e8c9c to restore, notes which edges the view
// is beside, and sets the bounds to the map's own. Where the view may not
// move (the view is held, or the field is in control mode 3), it shakes the
// screen along the asked direction instead.
void Func_080e89e4(s32 direction) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *center = event->viewCenter;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 viewX = map->viewX >> 16;
  s32 viewZ = map->viewZ >> 16;
  struct ViewMoveWork *work = Ram_HeapSlots->battleFxWork[0];
  struct MapBounds bounds;
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;

  // Japanese does not check the map's rectangle list here.
#if TLA_PATCH_1
  if (*(s32 *)map->unk0fc != 0) {
    if (direction & VIEW_MOVE_X) {
      Map_SetScreenShakeFar(0x30000, 0, 0x10000);
      return;
    }
    if (direction & VIEW_MOVE_Z) {
      Map_SetScreenShakeFar(0, 0x30000, 0x10000);
      return;
    }
  }
#endif
  if (event->controlMode == 3) {
    if (direction & VIEW_MOVE_X) {
      Map_SetScreenShakeFar(0x30000, 0, 0x10000);
      return;
    }
    if (direction & VIEW_MOVE_Z) {
      Map_SetScreenShakeFar(0, 0x30000, 0x10000);
      return;
    }
  }

  /* FAKEMATCH(barrier): an empty asm before the bounds are fetched, the first
   * of three that lengthen viewX's and viewZ's lives so the allocator ranks
   * them below the bounds; without it, 33 of 157 instructions differ in
   * tla-ja. */
  __asm__("");
  /* FAKEMATCH(barrier): the second; without it, 33 of 157 instructions differ
   * in tla-ja. */
  __asm__("");
  /* FAKEMATCH(barrier): the third; without it, 33 of 157 instructions differ
   * in tla-ja. */
  __asm__("");
  Func_08020328(gMainRoutineWork.map, &bounds);
  minX = bounds.minX;
  maxX = bounds.maxX;
  minZ = bounds.minZ;
  maxZ = bounds.maxZ;
  left = minX * 8;
  right = maxX * 8;
  top = minZ * 8;
  bottom = maxZ * 8;
  work->x = center->x;
  work->y = center->y;
  work->z = center->z;

  work->edgeX = 0;
  if (viewX < left + VIEW_EDGE_MARGIN)
    work->edgeX = -1;
  if (viewX + 240 >= right - VIEW_EDGE_MARGIN)
    work->edgeX = 1;
  work->edgeZ = 0;
  if (viewZ < top + VIEW_EDGE_MARGIN)
    work->edgeZ = -1;
  if (viewZ + 160 >= bottom - VIEW_EDGE_MARGIN)
    work->edgeZ = 1;

  work->savedMinX = map->minX;
  work->savedMaxX = map->maxX;
  work->savedMinZ = map->minZ;
  work->savedMaxZ = map->maxZ;
  map->minX = minX << 19;
  map->maxX = maxX << 19;
  map->minZ = minZ << 19;
  map->maxZ = maxZ << 19;
  Actor_ResetMotion(center);
}

#include "fixed_math.h"

#define ABS(x) ((x) >= 0 ? (x) : -(x))

/// Swings the view centre away from where Func_080e89e4 found it, along the
/// asked directions, by amplitude times the sine of the phase. The swing goes
/// into the room: away from the edge the view is beside, or forward when it is
/// beside neither. A held view does not move.
void Func_080e8b44(s32 direction, s32 phase, s32 amplitude) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct ViewMoveWork *work = Ram_HeapSlots->battleFxWork[0];
  struct Actor *center = event->viewCenter;
  s32 x;
  s32 z;
  s32 edge;

  // Japanese does not check the map's rectangle list here.
#if TLA_PATCH_1
  if (*(s32 *)Ram_HeapSlots->mapWork->unk0fc != 0)
    return;
#endif
  if (event->controlMode == 3)
    return;

  if (direction & VIEW_MOVE_X) {
    edge = work->edgeX;
    if (edge == -1)
      x = work->x + Iwram_MulQ16(ABS(Trig_Sin(phase << 12)), amplitude);
    else if (edge == 1)
      x = work->x + Iwram_MulQ16(-ABS(Trig_Sin(phase << 12)), amplitude);
    else
      x = work->x + Iwram_MulQ16(ABS(Trig_Sin(phase << 12)), amplitude);
  } else {
    x = work->x;
  }

  if (direction & VIEW_MOVE_Z) {
    edge = work->edgeZ;
    if (edge == -1)
      z = work->z + Iwram_MulQ16(ABS(Trig_Sin(phase << 12)), amplitude);
    else if (edge == 1)
      z = work->z + Iwram_MulQ16(-ABS(Trig_Sin(phase << 12)), amplitude);
    else
      z = work->z + Iwram_MulQ16(ABS(Trig_Sin(phase << 12)), amplitude);
  } else {
    z = work->z;
  }

  center->x = x;
  center->y = work->y;
  center->z = z;
}

struct EffectScene {
  u8 unk00[0x18];
  s16 firstObjectId;
};

void Camera_FollowActor(s32 id, s32 mode);

// Ends a view move: gives the map back the view bounds Func_080e89e4 kept
// and lets the view follow the scene's first object again. Where the view
// was held, it only stops the screen shake.
void Func_080e8c9c(void) {
  struct ViewMoveWork *work = Ram_HeapSlots->battleFxWork[0];
  struct EffectScene *scene = Ram_HeapSlots->effectWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct MapWork *map = Ram_HeapSlots->mapWork;

#if TLA_PATCH_1
  if (*(s32 *)map->unk0fc != 0 || event->controlMode == 3) {
#else
  if (event->controlMode == 3) {
#endif
    Map_SetScreenShakeFar(0, 0, 0);
  } else {
    map->minX = work->savedMinX;
    map->maxX = work->savedMaxX;
    map->minZ = work->savedMinZ;
    map->maxZ = work->savedMaxZ;
    Camera_FollowActor(scene->firstObjectId, 1);
  }
}

#include "field_psynergy_work.h"

struct FxGroup {
  u8 unk00[32];
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
};

union FxSprite {
  u16 halves[10];
  u32 words[5];
};

s32 Func_080cdac0(s32 effect, s32 from, s32 to);

/// Gathers up to eight live objects, other than the scene's main object, for
/// which Func_080cdac0 measures a distance from the first object, with each
/// one's sprite value.
void ViewMove_SaveNearbySprites(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct FxGroup *group = (struct FxGroup *)Ram_HeapSlots->battleFxWork[0];
  struct Actor **objects;
  struct Actor *obj;
  s32 i;

  group->count = 0;
  objects = work->objects;
  for (i = 0; i < 80; i++) {
    obj = *objects++;
    if (obj == NULL || obj->script == NULL || obj == scene->mainObject)
      continue;
    if (Func_080cdac0((s16)(scene->fieldKind | 0x100), scene->firstObjectId,
                      i) == -1)
      continue;
    group->objects[group->count] = obj;
    group->values[group->count] = ((union FxSprite *)obj->animation)->halves[9];
    if (++group->count == 8)
      break;
  }
}

/// Writes each gathered object's saved sprite value back.
void ViewMove_RestoreNearbySprites(void) {
  struct FxGroup *group = (struct FxGroup *)Ram_HeapSlots->battleFxWork[0];
  s32 i;

  for (i = 0; i < group->count; i++)
    ((union FxSprite *)group->objects[i]->animation)->halves[9] =
        group->values[i];
}
