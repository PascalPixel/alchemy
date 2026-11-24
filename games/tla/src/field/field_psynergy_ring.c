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
#include "field/field_state.h"
#include "battle/battle_unit.h"
#include "window/window.h"
#include "system/bg_scroll.h"
#include "system/io_write.h"
#include "field/map_grid.h"
#include "field/view_move.h"
#include "version.h"

/// The sparkles that rise round the caster as the Ring is drawn.
#define RING_SPARK_COUNT 64
/// The motes the Ring places, and the first 16 that twinkle on it.
#define RING_MOTE_COUNT 32
#define RING_CIRCLING_MOTES 16
/// The frames the Ring lasts once it is drawn, or cast again.
#define RING_FRAMES 600
/// The OBJ and BG palette the Ring's colours cycle in.
#define RING_PALETTE 15
/// The ring's inner radius in pixels: its four bands run out to 3 more.
#define RING_RADIUS 52
/// The colour of the ring's band n, 0 innermost: every fourth colour of the
/// cycling palette, so the bands shimmer one after another.
#define RING_BAND_COLOR(n) (RING_PALETTE * PALETTE_COLORS + (n) * 4 + 3)
/// The arc the sparkles sweep, and the ring is drawn over, each frame: half
/// a turn in 32 frames, the picture's lower half being its upper half turned
/// round (gParch_PictureTiles).
#define RING_ARC_STEP (TRIG_TURN / 64)

/// The Ring's state (heap slot battleEffect): the sparkle picture, the 64
/// sparkles that rise round the caster as the ring is drawn and burst when it
/// ends, the motes that circle it after, and what the per-frame task shows.
struct RingState {
  u8 tiles[4096]; ///< the sparkle picture, then the motes', then the burst's
  struct EffectSprite sparkSprites[RING_SPARK_COUNT];
  struct FieldPsynergyParticle sparks[RING_SPARK_COUNT];
  /// The motes, placed round the caster; the task draws the first 16.
  struct EffectSprite sprites[RING_MOTE_COUNT];
  struct FieldPsynergyParticle motes[RING_MOTE_COUNT];
  s16 entry;    ///< the motes' VRAM cache entry
  u16 tile;     ///< the motes' first tile
  s8 sparkling; ///< the task draws RING_CIRCLING_MOTES motes round the caster
  s8 orbiting;  ///< the ring sways round the caster as its time runs down
  s8 cycling;   ///< the task cycles BG and OBJ palette RING_PALETTE
  u8 unused;
  u16 phase; ///< the colour cycle's step
};

/// Task of the Ring: its motes and colours each frame.
void FieldPsynergy_UpdateRing(void) {
  struct RingState *state = Ram_HeapSlots->battleEffect;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 position[3];
  struct FieldPsynergyParticle *entry;
  struct EffectSprite *sprite;
  s32 x;
  s32 y;
  s32 i;
  s32 timer;
  u16 *phase;
  u16 *tile;
  s32 red;
  s32 green;
  s32 blue;
  FIXED sine;

  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed + toFIXED(16);
  position[2] = actor->z.fixed;
  if (state->orbiting != 0)
    Vector_AddPolarOffset(toFIXED(2), -(field->ringFrames * (TRIG_TURN / 32)),
                          position);
  Camera_WorldToScreen(position);
  x = FIXEDtoINT(position[0]);
  gBgScroll[0].x = 64 - x;
  y = FIXEDtoINT(position[2]);
  gBgScroll[0].y = 64 - y;
  BgMap_DrawClipped16x16(x - 64, y - 64);

  if (state->cycling != 0) {
    i = 0;
    phase = &state->phase;
    red = GLOW_RED;
  cycle:
    sine = Trig_Sin(((*phase >> 1) + i) * (TRIG_TURN / 32));
    green = sine * GLOW_GREEN_SWING;
    sine =
        Trig_Sin(((*phase >> 2) + i) * (TRIG_TURN / 32) + TRIG_TURN * 9 / 32);
    blue = sine * GLOW_BLUE_SWING;
    green = FIXEDtoINT(green) + GLOW_GREEN;
    blue = FIXEDtoINT(blue) + GLOW_BLUE;
    Io_Write16(RGB(red, green, blue),
               &((u16 *)&BG_PLTT_COLOR(RING_PALETTE, 0))[i]);
    ((u16 *)&OBJ_PLTT_COLOR(RING_PALETTE, 0))[i] =
        ((u16 *)&BG_PLTT_COLOR(RING_PALETTE, 0))[i];
    if (++i < PALETTE_COLORS)
      goto cycle;
    state->phase++;
  }

  if (state->sparkling != 0) {
    entry = state->motes;
    sprite = state->sprites;
    tile = &state->tile;
    for (i = 0; i < RING_CIRCLING_MOTES; i++, sprite++, entry++) {
      timer = entry->timer;
      if (timer == 0) {
        sprite->angle = Random16();
        position[2] = timer;
        position[1] = timer;
        position[0] = timer;
        Vector_AddPolarOffset(toFIXED(RING_RADIUS + 2),
                              sprite->angle + DEGtoANG(45), position);
        SET_POSITION(entry, position[0], position[1] + toFIXED(16),
                     position[2]);
        timer = entry->timer;
      }
      if (timer >= 0) {
        sprite->oam.index = *tile + ((timer >> 2) & 3) * 4;
        position[0] = actor->x.fixed + entry->x;
        position[1] = actor->y.fixed + entry->y;
        position[2] = actor->z.fixed + entry->z;
        EffectSprite_DrawAtWorldPosition(sprite, position);
        timer = entry->timer;
      }
      if ((entry->timer = timer + 1) == 16)
        entry->timer = 0;
    }
  }

  if (field->ringTimerRunning != 0 && field->ringFrames != 0)
    field->ringFrames--;
  if (field->ringFrames == 0)
    field->psynergy = ABILITY_MAGNET | ACT_PACKED;
}

/// The Ring: a ring of light is drawn round the caster among rising
/// sparkles, then motes twinkle on it as it sways; FieldPsynergy_UpdateRing
/// keeps it for RING_FRAMES frames, then cue ABILITY_MAGNET | ACT_PACKED ends
/// it (FieldPsynergy_EndRing). Cast again while it lasts, it only starts its
/// frames again.
void FieldPsynergy_Ring(void) {
  struct RingState *state =
      HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct RingState));
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *caster = scene->mainActor;
  struct CanvasSpriteState *canvas;
  s32 entry;
  s32 tile;
  s32 arcStart;
  s32 angle;
  s32 repeat;
  s32 drawing;
  s32 *pos;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *mote;
  s32 position[3];
  s32 phase;
  s32 timer;
  s32 arc;
  s32 i;
  u8 *picture;

  arcStart = 0;
  angle = 0;
  repeat = 1;
  drawing = 0;
  if (field->ringActive == 0) {
    FieldPsynergy_FadeToCastColor();
    gCpuLoadDisplayEnabled = 0;
    BattleEffect_StartCanvasSprite();
    canvas = Ram_HeapSlots->canvasSprite;
    canvas->mode = 0;
    pos = position;
    pos[0] = scene->mainActor->x.fixed;
    pos[1] = scene->mainActor->y.fixed + toFIXED(16);
    pos[2] = scene->mainActor->z.fixed;
    Camera_WorldToScreen(pos);
    // The whole pixels of the caster's place on screen.
    {
      s32 x = *((s16 *)pos + 1);
      s32 y;

      gBgScroll[0].x = 64 - x;
      y = *((s16 *)pos + 5);
      gBgScroll[0].y = 64 - y;
    }
    IWRAM_FILL_WORDS((void *)BG_SCREEN_BLOCK(4), 2048, 0);

    HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
    picture = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_CANVAS, 8192);
    IWRAM_FILL_WORDS(picture, 8192, 0);

    wait(1);
    Dma_Set(picture, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(2048), REG_DMA3);
    Io_Write16(BGCNT_SCREEN_BASE(4) | BGCNT_256_COLORS, &REG_BG0CNT);
    Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_STAR_SPARKLE_FADE),
                        state);
    entry = VramCache_FindFreeEntry();
    tile = VramCache_Load(entry, 256, state);
    for (i = 0, mote = state->sparks; i < RING_SPARK_COUNT; i++, mote++) {
      sprite = &state->sparkSprites[i];
      EffectSprite_Init(sprite, 4, 4, 0, tile);
      sprite->oam.fullColor = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = 0;
      sprite->depth = OAM_DEPTH_WINDOW;
      mote->timer = -i / 4;
    }
    state->sparkling = 0;
    state->orbiting = 0;
    state->cycling = 1;
    state->phase = 0;
    field->ringActive = 1;
    field->ringTimerRunning = 1;
    field->ringFrames = 999;
    Task_Add(FieldPsynergy_UpdateRing, TASK_PASS_FRAME);
    wait(1);
    state->cycling = 0;
    phase = 0;
    timer = 0;
    do {
      if (drawing) {
        arc = arcStart;
        for (i = 0; i < 8; i++) {
          Canvas_PlotPolarPixel(picture, toFIXED(RING_RADIUS), arc,
                                RING_BAND_COLOR(0));
          Canvas_PlotPolarPixel(picture, toFIXED(RING_RADIUS + 1), arc,
                                RING_BAND_COLOR(1));
          Canvas_PlotPolarPixel(picture, toFIXED(RING_RADIUS + 2), arc,
                                RING_BAND_COLOR(2));
          Canvas_PlotPolarPixel(picture, toFIXED(RING_RADIUS + 3), arc,
                                RING_BAND_COLOR(3));
          arc += RING_ARC_STEP / 8;
        }
        canvas->command = CANVAS_UPLOAD_TO_BG;
      }
      for (i = 0, mote = state->sparks; i < RING_SPARK_COUNT; i++, mote++) {
        sprite = &state->sparkSprites[i];
        if (mote->timer == 0) {
          s32 heading = angle + (i & 1) * DEGtoANG(180);

          SET_POSITION(mote, caster->x.fixed, caster->y.fixed + toFIXED(16),
                       caster->z.fixed);
          Vector_AddPolarOffset(toFIXED(RING_RADIUS + 3), heading, &mote->x);
          Vector_AddPolarOffset(Random16() * 6, Random16(), &mote->x);
        }
        if ((u32)mote->timer <= 15) {
          sprite->oam.index = tile + (mote->timer & 3) * 2;
          EffectSprite_DrawAtWorldPosition(sprite, mote);
          mote->y += -toFIXED(1);
        }
        mote->timer++;
        if (repeat && mote->timer == 16)
          mote->timer = 0;
      }
      switch (phase) {
      case 0:
        angle = timer * RING_ARC_STEP;
        if (timer == 32) {
          phase = 1;
          timer = -1;
        }
        break;
      case 1:
        drawing = 1;
        angle = arcStart = timer * RING_ARC_STEP;
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

    Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_RING_ARCS), picture);
    Dma_Set(picture, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(2048), REG_DMA3);
    Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_RING_WAVES), state);
    {
      s32 heading;
      s32 moteEntry = VramCache_FindFreeEntry();
      s32 moteTile = VramCache_Load(moteEntry, 512, state);

      state->entry = moteEntry;
      state->tile = moteTile;
      state->sparkling = 1;
      state->orbiting = 1;
      for (i = 0, mote = state->motes; i < RING_MOTE_COUNT; i++) {
        heading = Random16();
        sprite = &state->sprites[i];
        EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, moteTile);
        sprite->depth = OAM_DEPTH_WINDOW;
        sprite->oam.priority = 0;
        sprite->oam.fullColor = 0;
        sprite->oam.palette = RING_PALETTE;
        sprite->angle = heading + DEGtoANG(90);
        position[0] = scene->mainActor->x.fixed;
        position[1] = scene->mainActor->y.fixed + toFIXED(16);
        position[2] = scene->mainActor->z.fixed;
        Camera_WorldToScreen(position);
        Vector_AddPolarOffset(toFIXED(64), heading, position);
        mote->x = position[0];
        mote->y = position[2];
        mote->timer = -i;
        mote++;
      }
    }
    VramCache_FreeEntry(entry);
    FieldPsynergy_FadeFromCastColor();
  }
  state->cycling = 1;
  field->ringActive = 1;
  field->ringTimerRunning = 1;
  field->ringFrames = RING_FRAMES;
  Task_Add(FieldPsynergy_UpdateRing, TASK_PASS_FRAME);
}

/// Ends the Ring, if one is up: the ring gives way to a burst of 64 sparkles
/// from round the leader, four more setting off each frame, that fly out and
/// fall for 40 frames, and the window graphics come back to BG0.
void FieldPsynergy_EndRing(void) {
  struct RingState *state = Ram_HeapSlots->battleEffect;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldPsynergyParticle *mote;
  struct EffectSprite *sprite;
  s32 slot;
  s32 tile;
  s32 angle;
  s32 i;
  s32 frame;

  if (field->ringActive == 0)
    return;
  field->ringActive = 0;
  field->ringTimerRunning = 0;
  field->ringFrames = 0;
  Task_Remove(FieldPsynergy_UpdateRing);
  wait(1);
  Sound_PlayFar(SE_MAP_TRANSITION);
  IWRAM_FILL_WORDS((void *)BG_SCREEN_BLOCK(4), 1280,
                   WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
  Window_LoadGraphics();
  Io_Write16(BGCNT_SCREEN_BASE(4), &REG_BG0CNT);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  gCpuLoadDisplayEnabled = 1;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_SPARKLE_FADE), state);
  slot = VramCache_FindFreeEntry();
  tile = VramCache_Load(slot, 256, state);

  i = 0;
  mote = state->sparks;
  sprite = state->sparkSprites;
  for (; i < RING_SPARK_COUNT; i++, sprite++, mote++) {
    angle = Random16();
    SET_POSITION(mote, actor->x.fixed, actor->y.fixed + toFIXED(16),
                 actor->z.fixed);
    Vector_AddPolarOffset(Random16() * 4 + toFIXED(RING_RADIUS), angle,
                          &mote->x);
    mote->velocityX = 0;
    mote->velocityY = -toFIXED(4);
    mote->velocityZ = 0;
    Vector_AddPolarOffset(Random16() * 3 + toFIXED(2), angle, &mote->velocityX);
    mote->timer = -i / 4;
    EffectSprite_Init(sprite, 4, 4, 0, tile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
  }
  wait(1);

  for (frame = 0; frame < 40; frame++) {
    struct EffectSprite *spark;
    mote = state->sparks;
    spark = state->sparkSprites;
    for (i = 0; i < RING_SPARK_COUNT; i++, spark++, mote++) {
      if (mote->timer >= 0) {
        // angle carries the spark's age here, as it carried each spark's
        // heading above.
        angle = mote->timer;
        spark->oam.index = tile + (angle & 3) * 2;
        EffectSprite_DrawAtWorldPosition(spark, mote);
        Field_AdvanceWithGravity3D(&mote->x, 62, toFIXED(1) / 2);
      }
      mote->timer++;
    }
    wait(1);
  }
  VramCache_FreeEntry(slot);
  VramCache_FreeEntry(state->entry);
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// How far the view stays from the edge it is beside, in pixels.
#define VIEW_EDGE_MARGIN 12

/// A map's view bounds, in 8-pixel units.
struct MapBounds {
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
};

/// Writes a map's view bounds from its header, in 8-pixel units.
void Map_GetCameraBoundsFar(s32 map, struct MapBounds *bounds);

/// Opens the whole map to the view: keeps the view centre's position and the
/// map's view bounds for ViewMove_End to restore, notes which edges the view
/// is beside, and sets the bounds to the map's own. Where the view may not
/// move (on the World Map, and outside the Japanese release on a map whose
/// view keeps to its camera boxes), it shakes the screen along the asked
/// direction instead.
void ViewMove_Start(s32 direction) {
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *center = field->camera;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s32 viewX = FIXEDtoINT(map->viewX);
  s32 viewZ = FIXEDtoINT(map->viewZ);
  struct ViewMoveState *state = Ram_HeapSlots->battleEffect;
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
      MapGrid_SetScreenShakeFar(toFIXED(3), 0, FIXED_ONE);
      return;
    }
    if (direction & VIEW_MOVE_Z) {
      MapGrid_SetScreenShakeFar(0, toFIXED(3), FIXED_ONE);
      return;
    }
  }
#endif
  if (field->mode == ACTOR_SYSTEM_WORLD_MAP) {
    if (direction & VIEW_MOVE_X) {
      MapGrid_SetScreenShakeFar(toFIXED(3), 0, FIXED_ONE);
      return;
    }
    if (direction & VIEW_MOVE_Z) {
      MapGrid_SetScreenShakeFar(0, toFIXED(3), FIXED_ONE);
      return;
    }
  }

  Map_GetCameraBoundsFar(gMainRoutineState.mapId, &bounds);
  minX = bounds.minX;
  left = minX * 8;
  maxX = bounds.maxX;
  right = maxX * 8;
  minZ = bounds.minZ;
  top = minZ * 8;
  maxZ = bounds.maxZ;
  bottom = maxZ * 8;
  SET_POSITION(state, center->x.fixed, center->y.fixed, center->z.fixed);

  state->edgeX = 0;
  if (viewX < left + VIEW_EDGE_MARGIN)
    state->edgeX = -1;
  if (viewX + DISPLAY_WIDTH >= right - VIEW_EDGE_MARGIN)
    state->edgeX = 1;
  state->edgeZ = 0;
  if (viewZ < top + VIEW_EDGE_MARGIN)
    state->edgeZ = -1;
  if (viewZ + DISPLAY_HEIGHT >= bottom - VIEW_EDGE_MARGIN)
    state->edgeZ = 1;

  state->savedMinX = map->minX;
  state->savedMaxX = map->maxX;
  state->savedMinZ = map->minZ;
  state->savedMaxZ = map->maxZ;
  map->minX = toFIXED(minX * 8);
  map->maxX = toFIXED(maxX * 8);
  map->minZ = toFIXED(minZ * 8);
  map->maxZ = toFIXED(maxZ * 8);
  Actor_StartIdleFar(center);
}

/// The swing's size at an angle: its sine without the sign.
#define SWING_SINE(angle)                                                      \
  (Trig_Sin(angle) >= 0 ? Trig_Sin(angle) : -Trig_Sin(angle))

/// Swings the view centre away from where ViewMove_Start found it, along the
/// asked directions, by amplitude times the sine of the phase. The swing goes
/// into the room: away from the edge the view is beside, or forward when it is
/// beside neither. Where ViewMove_Start shook the screen instead, nothing
/// moves.
void ViewMove_Swing(s32 direction, s32 phase, FIXED amplitude) {
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct ViewMoveState *state = Ram_HeapSlots->battleEffect;
  struct Actor *center = field->camera;
  FIXED x;
  FIXED z;
  s32 edge;

  // Japanese does not check the map's rectangle list here.
#if TLA_PATCH_1
  if (((struct MapState *)Ram_HeapSlots->mapGrid)->cameraBoxes != NULL)
    return;
#endif
  if (field->mode == ACTOR_SYSTEM_WORLD_MAP)
    return;

  if (direction & VIEW_MOVE_X) {
    edge = state->edgeX;
    if (edge == -1)
      x = state->x +
          Iwram_MulFX(SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
    else if (edge == 1)
      x = state->x +
          Iwram_MulFX(-SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
    else
      x = state->x +
          Iwram_MulFX(SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
  } else {
    x = state->x;
  }

  if (direction & VIEW_MOVE_Z) {
    edge = state->edgeZ;
    if (edge == -1)
      z = state->z +
          Iwram_MulFX(SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
    else if (edge == 1)
      z = state->z +
          Iwram_MulFX(-SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
    else
      z = state->z +
          Iwram_MulFX(SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
  } else {
    z = state->z;
  }

  SET_ACTOR_POSITION(center, x, state->y, z);
}

/// Ends a view move: gives the map back the view bounds ViewMove_Start kept
/// and lets the view follow the caster again. Where ViewMove_Start shook the
/// screen instead, it only stops the shake.
void ViewMove_End(void) {
  struct ViewMoveState *state = Ram_HeapSlots->battleEffect;
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct MapState *map = Ram_HeapSlots->mapGrid;

#if TLA_PATCH_1
  if (map->cameraBoxes != NULL || field->mode == ACTOR_SYSTEM_WORLD_MAP) {
#else
  if (field->mode == ACTOR_SYSTEM_WORLD_MAP) {
#endif
    MapGrid_SetScreenShakeFar(0, 0, 0);
  } else {
    map->minX = state->savedMinX;
    map->maxX = state->savedMaxX;
    map->minZ = state->savedMinZ;
    map->maxZ = state->savedMaxZ;
    FieldCamera_FollowActor(scene->firstActorId, 1);
  }
}

/// Gathers up to eight actors near enough to the caster to shake
/// (Field_MeasurePsynergyReach), with each one's sprite rotation, for Tremor's
/// sway to rock.
void ViewMove_SaveNearbySprites(void) {
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct ViewMoveState *group = Ram_HeapSlots->battleEffect;
  struct Actor **actors;
  struct Actor *actor;
  s32 i;

  group->count = 0;
  actors = state->actors;
  for (i = 0; i < FIELD_ACTOR_COUNT; i++) {
    actor = *actors++;
    if (actor == NULL || actor->script == NULL || actor == scene->mainActor)
      continue;
    if (Field_MeasurePsynergyReach(
            (s16)(scene->fieldPsynergy | REACH_SHAKEN_OBJECTS),
            scene->firstActorId, i) == -1)
      continue;
    group->objects[group->count] = actor;
    group->spriteRotations[group->count] =
        ((struct Sprite *)actor->sprite)->rotation;
    if (++group->count == ARRAY_COUNT(group->objects))
      break;
  }
}

/// Gives each gathered actor's sprite its saved rotation back.
void ViewMove_RestoreNearbySprites(void) {
  struct ViewMoveState *group = Ram_HeapSlots->battleEffect;
  s32 i;

  for (i = 0; i < group->count; i++)
    ((struct Sprite *)group->objects[i]->sprite)->rotation =
        group->spriteRotations[i];
}
#undef VIEW_EDGE_MARGIN
#undef SWING_SINE
#undef RING_RADIUS
#undef RING_BAND_COLOR
#undef RING_ARC_STEP
