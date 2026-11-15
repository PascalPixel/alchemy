/// @file
/// The Ring, the field Psynergy of the unused ability "Ma????" (named here for
/// the ring of light it draws): its task, its burst of sparkles, and moving the
/// view (opening the whole map to it, swinging it and giving the bounds back).
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "field/field_work.h"
#include "battle/battle_unit.h"
#include "window/window.h"
#include "system/bg_scroll.h"
#include "field/map_grid.h"
#include "field/view_move.h"
#include "version.h"

/// The Ring's work (heap slot battleEffect): the sparkle
/// picture, the 64 sparkles that rise round the caster as the ring is drawn,
/// the motes that circle it after, and what the per-frame task shows.
struct RingWork {
  u8 tiles[0x1000]; ///< the decoded sparkle picture, then the mote picture
  struct EffectSprite sparkSprites[64];
  struct FieldPsynergyParticle sparks[64];
  /// The motes: on screen as they gather, then 16 round the caster.
  struct EffectSprite sprites[32];
  struct FieldPsynergyParticle motes[32];
  u16 entry;    ///< the motes' VRAM cache entry
  u16 tile;     ///< the motes' first tile
  s8 sparkling; ///< the task draws 16 motes round the caster
  s8 orbiting;  ///< the ring sways round the caster as its time runs down
  s8 cycling;   ///< the task cycles BG and OBJ palette 15
  u8 unused;
  u16 phase; ///< the colour cycle's step
};

/// Task of the Ring: its motes and colours each frame.
void FieldPsynergy_UpdateRing(void) {
  struct RingWork *work = Ram_HeapSlots->battleEffect;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 position[3];
  s32 *pos = position;
  struct FieldPsynergyParticle *entry;
  struct EffectSprite *sprite;
  s32 x;
  s32 y;
  s32 i;
  s32 timer;
  u16 *phase;
  u16 *tile;
  s32 red;

  pos[0] = actor->x.fixed;
  pos[1] = actor->y.fixed + px(16);
  pos[2] = actor->z.fixed;
  if (work->orbiting != 0)
    Vector_AddPolarOffset(px(2), -(event->ringFrames << 11), pos);
  Camera_WorldToScreen(pos);
  x = FIELD_PIXEL(pos[0]);
  gBgScroll[0].x = 64 - x;
  y = FIELD_PIXEL(pos[2]);
  gBgScroll[0].y = 64 - y;
  BgMap_DrawClipped16x16(x - 64, y - 64);

  if (work->cycling != 0) {
    i = 0;
    phase = &work->phase;
    red = 28;
  cycle:
    {
      /* FAKEMATCH(pin): green pinned to r5. */
      register s32 green asm("r5") = Trig_Sin(((*phase >> 1) + i) << 11) * 8;
      s32 blue;
      s32 color;

      blue = Trig_Sin((((*phase >> 2) + i) << 11) + 0x4800) * 10;
      green = (green >> 16) + 23;
      blue = ((blue >> 16) + 12) << 10;
      green <<= 5;
      /* FAKEMATCH(dead): an empty once-only loop. */
      do {
      } while (0);
      color = blue | green | red;

      ((u16 *)&BG_PLTT_COLOR(15, 0))[i] = color;
      ((u16 *)&OBJ_PLTT_COLOR(15, 0))[i] = color;
    }
    if (++i < 16)
      goto cycle;
    work->phase++;
  }

  if (work->sparkling != 0) {
    entry = work->motes;
    sprite = work->sprites;
    tile = &work->tile;
    for (i = 15; i >= 0; i--, sprite++, entry++) {
      timer = entry->timer;
      if (timer == 0) {
        sprite->angle = Random16();
        pos[2] = timer;
        pos[1] = timer;
        pos[0] = timer;
        Vector_AddPolarOffset(px(54), sprite->angle + TRIG_TURN / 8, pos);
        SET_POSITION(entry, pos[0], pos[1] + px(16), pos[2]);
        timer = entry->timer;
      }
      if (timer >= 0) {
        sprite->oam.index = *tile + ((timer >> 2) & 3) * 4;
        pos[0] = actor->x.fixed + entry->x;
        pos[1] = actor->y.fixed + entry->y;
        pos[2] = actor->z.fixed + entry->z;
        EffectSprite_DrawAtWorldPosition(sprite, pos);
        timer = entry->timer;
      }
      if ((entry->timer = timer + 1) == 16)
        entry->timer = 0;
    }
  }

  if (event->ringTimerRunning != 0 && event->ringFrames != 0)
    event->ringFrames--;
  if (event->ringFrames == 0)
    event->psynergy = 0x209b;
}
#undef NEXT_PHASE
#undef WHIRL_COUNT
#undef SPARK_COUNT

void Camera_WorldToScreen(s32 *position);

/// The Ring: a ring of light is drawn round the caster among rising
/// sparkles, and motes gather to circle it; FieldPsynergy_UpdateRing keeps
/// it for 600 frames, then cue 0x209b ends it (FieldPsynergy_EndRing).
/// Cast again while it lasts, it only starts the 600 frames again.
void FieldPsynergy_Ring(void) {
  struct RingWork *work =
      HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct RingWork));
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *caster = scene->mainActor;
  struct CanvasSpriteWork *canvas;
  s32 entry;
  s32 tile;
  s32 spin;
  s32 angle;
  s32 repeat;
  s32 drawing;
  s32 *pos;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *mote;
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
  if (event->ringActive == 0) {
    FieldPsynergy_FadeToCastColor();
    gCpuLoadDisplayEnabled = angle;
    BattleEffect_StartCanvasSprite();
    canvas = Ram_HeapSlots->canvasSprite;
    canvas->mode = angle;
    pos = position;
    pos[0] = scene->mainActor->x.fixed;
    pos[1] = scene->mainActor->y.fixed + px(16);
    pos[2] = scene->mainActor->z.fixed;
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

      /* FAKEMATCH(dead): an empty once-only loop. */
      do {
      } while (0);
      fill = Iwram_FillWordsEntry;
      fill((void *)BG_SCREEN_BLOCK(4), 0x800, 0);
    }

    /* FAKEMATCH(dead): the picture's release and allocation in a once-only
     * block. */
    do {
      HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
      picture = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_CANVAS, 0x2000);
    } while (0);
    Iwram_FillWordsEntry(picture, 0x2000, 0);

    wait(1);
    Dma_Set(picture, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(2048), REG_DMA3);
    {
      s32 control;

      control = 0x480;
      /* FAKEMATCH(dead): an empty once-only loop. */
      do {
      } while (0);
      REG_BG0CNT = control;
    }
    Decompress_TaggedLz(
        Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
    entry = VramCache_FindFreeEntry();
    tile = VramCache_Load(entry, 0x100, work);
    for (i = 0, mote = work->sparks; i < 64; i++, mote++) {
      sprite = &work->sparkSprites[i];
      EffectSprite_Init(sprite, 4, 4, 0, tile);
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
    event->ringActive = 1;
    event->ringTimerRunning = 1;
    event->ringFrames = 999;
    off = 0;
    Task_Add(FieldPsynergy_UpdateRing, TASK_PASS_VBLANK);
    wait(1);
    work->cycling = off;
    phase = 0;
    timer = 0;
    do {
      if (drawing) {
        radius = spin;
        for (i = 0; i < 8; i++) {
          Canvas_PlotPolarPixel(picture, px(52), radius, 243);
          Canvas_PlotPolarPixel(picture, px(53), radius, 247);
          Canvas_PlotPolarPixel(picture, px(54), radius, 251);
          Canvas_PlotPolarPixel(picture, px(55), radius, 255);
          radius += 128;
        }
        canvas->command = CANVAS_UPLOAD_TO_BG;
      }
      for (i = 0, mote = work->sparks; i < 64; i++, mote++) {
        sprite = &work->sparkSprites[i];
        if (mote->timer == 0) {
          s32 heading = angle + ((i & 1) << 15);

          SET_POSITION(mote, caster->x.fixed, caster->y.fixed + px(16),
                       caster->z.fixed);
          Vector_AddPolarOffset(px(55), heading, &mote->x);
          Vector_AddPolarOffset(Random16() * 6, Random16(), &mote->x);
        }
        if ((u32)mote->timer <= 15) {
          sprite->oam.index = tile + (mote->timer & 3) * 2;
          EffectSprite_DrawAtWorldPosition(sprite, mote);
          mote->y += -px(1);
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
          phase = FIELD_PSYNERGY_FINISHED;
        break;
      }
      wait(1);
      timer++;
    } while (phase != FIELD_PSYNERGY_FINISHED);

    Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_RingArcs),
                        picture);
    Dma_Set(picture, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(2048), REG_DMA3);
    Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_Data1ED), work);
    {
      s32 heading;
      s32 moteEntry = VramCache_FindFreeEntry();
      s32 moteTile = VramCache_Load(moteEntry, 0x200, work);
      s32 *place;

      work->entry = moteEntry;
      work->tile = moteTile;
      work->sparkling = 1;
      work->orbiting = 1;
      for (i = 0, mote = work->motes; i < 32; i++) {
        heading = Random16();
        sprite = &work->sprites[i];
        EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, moteTile);
        sprite->depth = 240;
        sprite->oam.priority = 0;
        sprite->oam.palette256 = 0;
        sprite->oam.palette = 15;
        sprite->angle = heading + TRIG_TURN / 4;
        place = position;
        place[0] = scene->mainActor->x.fixed;
        place[1] = scene->mainActor->y.fixed + px(16);
        place[2] = scene->mainActor->z.fixed;
        Camera_WorldToScreen(place);
        Vector_AddPolarOffset(px(64), heading, place);
        mote->x = place[0];
        mote->y = place[2];
        mote->timer = -i;
        mote++;
      }
    }
    VramCache_FreeEntry(entry);
    FieldPsynergy_FadeFromCastColor();
  }
  work->cycling = 1;
  event->ringActive = 1;
  event->ringTimerRunning = 1;
  event->ringFrames = 600;
  Task_Add(FieldPsynergy_UpdateRing, TASK_PASS_VBLANK);
}

/// One mote: a position and a velocity (BattleEffect_IntegrateVector3's vector)
/// and a frame count, negative before it starts.
struct BurstMote {
  s32 x;
  s32 y;
  s32 z;
  s32 vx;
  s32 vy;
  s32 vz;
  s32 timer;
};

/// The work of the sparkle burst the Ring ends with (heap slot
/// battleEffect): the sparkle picture, 64 sparkles and their motes.
struct SparkleBurstWork {
  u8 unused[0x1000]; ///< no code names it
  struct EffectSprite sprites[64];
  struct BurstMote motes[64];
  u8 unusedAfterMotes[0x880]; ///< no code names it
  s16 resource;
};

/// The burst of 64 sparkles that FieldPsynergy_UpdateRing's effect ends
/// with: once the event work asks for it, it plays sound 167, fills the tiles
/// at 0x06002000, reloads the window graphics, loads the sparkle picture
/// (FieldEffect_SparkleFade) and scatters the sparkles around the current
/// owner, starting a quarter of them a frame later than the last, then moves
/// and animates them for 40 frames before releasing the effect.
void FieldPsynergy_EndRing(void) {
  struct SparkleBurstWork *work = Ram_HeapSlots->battleEffect;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct BurstMote *mote;
  struct EffectSprite *sprite;
  s32 slot;
  s32 tile;
  s32 angle;
  s32 i;
  s32 frame;

  if (event->ringActive == 0)
    return;
  event->ringActive = 0;
  event->ringTimerRunning = 0;
  event->ringFrames = 0;
  Task_Remove(FieldPsynergy_UpdateRing);
  wait(1);
  /* FAKEMATCH(dead): a once-only block. */
  do {
    Sound_PlayFar(SE_MAP_TRANSITION);
    {
      s32 (*fill)(void *, s32, u32);
      // A once-only block: the fill's entry loads ahead of the size (as a
      // direct Iwram_FillWords call, 2 of 226 differ).
      do {
      } while (0);
      fill = Iwram_FillWordsEntry;
      fill((void *)BG_SCREEN_BLOCK(4), 0x500, 0xf000f000);
    }
    Window_LoadGraphics();
  } while (0);
  {
    /* FAKEMATCH(temporary): the control word in a local of its own. */
    s32 control = 0x400;

    REG_BG0CNT = control;
  }
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  gCpuLoadDisplayEnabled = 1;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                      work);
  slot = VramCache_FindFreeEntry();
  tile = VramCache_Load(slot, 0x100, work);

  i = 0;
  mote = work->motes;
  sprite = work->sprites;
  for (; i < 64; i++, sprite++, mote++) {
    angle = Random16();
    SET_POSITION(mote, actor->x.fixed, actor->y.fixed + px(16), actor->z.fixed);
    Vector_AddPolarOffset(Random16() * 4 + px(52), angle, &mote->x);
    mote->vx = 0;
    mote->vy = -0x40000;
    mote->vz = 0;
    Vector_AddPolarOffset(Random16() * 3 + px(2), angle, &mote->vx);
    mote->timer = -i / 4;
    EffectSprite_Init(sprite, 4, 4, 0, tile);
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
      /* FAKEMATCH(barrier): an empty asm that opens the loop's body. */
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
  VramCache_FreeEntry(slot);
  VramCache_FreeEntry(work->resource);
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

// How far the view stays from the edge it is beside, in pixels.
#define VIEW_EDGE_MARGIN 12

/// A map's view bounds, in 8-pixel units.
struct MapBounds {
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
};

void Map_GetHeaderSizeFar(s32 map, struct MapBounds *bounds);

/// Opens the whole map to the view: keeps the view centre's position and the
/// map's view bounds for ViewMove_End to restore, notes which edges the view
/// is beside, and sets the bounds to the map's own. Where the view may not
/// move (the view is held, or the field is in control mode 3), it shakes the
/// screen along the asked direction instead.
void ViewMove_Start(s32 direction) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *center = event->camera;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 viewX = FIELD_PIXEL(map->viewX);
  s32 viewZ = FIELD_PIXEL(map->viewZ);
  struct ViewMoveWork *work = Ram_HeapSlots->battleEffect;
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
  if (map->cameraBoxes != NULL) {
    if (direction & VIEW_MOVE_X) {
      MapGrid_SetScreenShakeFar(px(3), 0, Q16_ONE);
      return;
    }
    if (direction & VIEW_MOVE_Z) {
      MapGrid_SetScreenShakeFar(0, px(3), Q16_ONE);
      return;
    }
  }
#endif
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    if (direction & VIEW_MOVE_X) {
      MapGrid_SetScreenShakeFar(px(3), 0, Q16_ONE);
      return;
    }
    if (direction & VIEW_MOVE_Z) {
      MapGrid_SetScreenShakeFar(0, px(3), Q16_ONE);
      return;
    }
  }

  /* FAKEMATCH(barrier): an empty asm before the bounds are fetched. */
  __asm__("");
  /* FAKEMATCH(barrier): a second empty asm. */
  __asm__("");
  /* FAKEMATCH(barrier): a third empty asm. */
  __asm__("");
  Map_GetHeaderSizeFar(gMainRoutineWork.mapId, &bounds);
  minX = bounds.minX;
  maxX = bounds.maxX;
  minZ = bounds.minZ;
  maxZ = bounds.maxZ;
  left = minX * 8;
  right = maxX * 8;
  top = minZ * 8;
  bottom = maxZ * 8;
  SET_POSITION(work, center->x.fixed, center->y.fixed, center->z.fixed);

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
  Actor_StartIdleFar(center);
}

/// The swing's size at an angle: its sine without the sign.
#define SWING_SINE(angle)                                                      \
  (Trig_Sin(angle) >= 0 ? Trig_Sin(angle) : -Trig_Sin(angle))

/// Swings the view centre away from where ViewMove_Start found it, along the
/// asked directions, by amplitude times the sine of the phase. The swing goes
/// into the room: away from the edge the view is beside, or forward when it is
/// beside neither. A held view does not move.
void ViewMove_Swing(s32 direction, s32 phase, s32 amplitude) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct ViewMoveWork *work = Ram_HeapSlots->battleEffect;
  struct Actor *center = event->camera;
  s32 x;
  s32 z;
  s32 edge;

  // Japanese does not check the map's rectangle list here.
#if TLA_PATCH_1
  if (Ram_HeapSlots->mapGrid->cameraBoxes != NULL)
    return;
#endif
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    return;

  if (direction & VIEW_MOVE_X) {
    edge = work->edgeX;
    if (edge == -1)
      x = work->x + Iwram_MulQ16(SWING_SINE(phase << 12), amplitude);
    else if (edge == 1)
      x = work->x + Iwram_MulQ16(-SWING_SINE(phase << 12), amplitude);
    else
      x = work->x + Iwram_MulQ16(SWING_SINE(phase << 12), amplitude);
  } else {
    x = work->x;
  }

  if (direction & VIEW_MOVE_Z) {
    edge = work->edgeZ;
    if (edge == -1)
      z = work->z + Iwram_MulQ16(SWING_SINE(phase << 12), amplitude);
    else if (edge == 1)
      z = work->z + Iwram_MulQ16(-SWING_SINE(phase << 12), amplitude);
    else
      z = work->z + Iwram_MulQ16(SWING_SINE(phase << 12), amplitude);
  } else {
    z = work->z;
  }

  SET_ACTOR_POSITION(center, x, work->y, z);
}

/// Ends a view move: gives the map back the view bounds ViewMove_Start kept
/// and lets the view follow the scene's first object again. Where the view
/// was held, it only stops the screen shake.
void ViewMove_End(void) {
  struct ViewMoveWork *work = Ram_HeapSlots->battleEffect;
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct MapWork *map = Ram_HeapSlots->mapGrid;

#if TLA_PATCH_1
  if (map->cameraBoxes != NULL || event->mode == ACTOR_SYSTEM_WORLD_MAP) {
#else
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
#endif
    MapGrid_SetScreenShakeFar(0, 0, 0);
  } else {
    map->minX = work->savedMinX;
    map->maxX = work->savedMaxX;
    map->minZ = work->savedMinZ;
    map->maxZ = work->savedMaxZ;
    FieldCamera_FollowActor(scene->firstActorId, 1);
  }
}

/// The actors near the caster whose sprite value a view move saves and gives
/// back (heap slot battleEffect): up to eight, with each one's value.
struct FxGroup {
  u8 unused[32]; ///< no code names it
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
};

union FxSprite {
  u16 halves[10];
  u32 words[5];
};

/// Gathers up to eight live objects, other than the scene's main object, for
/// which Field_MeasurePsynergyReach measures a distance from the first object,
/// with each one's sprite value.
void ViewMove_SaveNearbySprites(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct FxGroup *group = (struct FxGroup *)Ram_HeapSlots->battleEffect;
  struct Actor **objects;
  struct Actor *obj;
  s32 i;

  group->count = 0;
  objects = work->actors;
  for (i = 0; i < 80; i++) {
    obj = *objects++;
    if (obj == NULL || obj->script == NULL || obj == scene->mainActor)
      continue;
    if (Field_MeasurePsynergyReach((s16)(scene->fieldPsynergy | 0x100),
                                   scene->firstActorId, i) == -1)
      continue;
    group->objects[group->count] = obj;
    group->values[group->count] = ((union FxSprite *)obj->sprite)->halves[9];
    if (++group->count == 8)
      break;
  }
}

/// Writes each gathered object's saved sprite value back.
void ViewMove_RestoreNearbySprites(void) {
  struct FxGroup *group = (struct FxGroup *)Ram_HeapSlots->battleEffect;
  s32 i;

  for (i = 0; i < group->count; i++)
    ((union FxSprite *)group->objects[i]->sprite)->halves[9] = group->values[i];
}
#undef VIEW_EDGE_MARGIN
#undef SWING_SINE
