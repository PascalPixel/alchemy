/// @file
/// Parch, with the canvas pixels and clipped 16 by 16 BG map copies it draws.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "field/field_state.h"
#include "battle/battle_unit.h"
#include "window/window.h"
#include "graphics/palette_buffers.h"
#include "system/bg_scroll.h"

/// Parch cast on a target: runs FieldPsynergy_Parch.
void FieldPsynergy_ParchOnTarget(void) { FieldPsynergy_Parch(); }

/// A puff of Parch's steam: its place, its sway and rise, and its age.
struct ParchPuff {
  s32 x;
  s32 y;
  s32 z;
  s32 sway;
  s32 rise;
  s32 swayRate;
  s32 timer;
};

/// The puffs of steam Parch keeps, a power of two: the next wraps by a mask.
#define PARCH_PUFF_COUNT 64
/// The disc's tiles, this many tiles after the window sprites' glyph tiles.
#define PARCH_DISC_TILE_OFFSET 128
/// The disc's colours, the last 32 of the sprite palette, which glow as the
/// Ring's do (GLOW_RED and the rest); the white flash of drying water raises
/// them all.
#define PARCH_GLOW_COLORS 32

/// What Parch finds before the caster.
enum ParchResult {
  PARCH_NOTHING = 0, ///< no water: the disc swells and shrinks
  PARCH_WATER = 1,   ///< water with no Parch event: steam rises from it
  /// A puddle, or water with a Parch event: it dries up in a white flash.
  PARCH_DRIES = 2
};

/// The stages of Parch's disc, which differ by what it finds (ParchResult).
/// Every result starts with PARCH_SWELLS.
enum ParchPhase {
  PARCH_SWELLS =
      0, ///< the disc grows, to three quarters (a quarter on nothing)
  /// On nothing: the disc holds for 30 frames, then shrinks away.
  PARCH_HOLDS = 1,
  PARCH_EMPTY_SHRINKS = 2,
  /// On water: steam rises two puffs a frame, then one, and the disc shrinks
  /// away. Water that dries flashes white 40 frames into the steam, and the
  /// cast ends with the thinner steam.
  PARCH_STEAMS = 1,
  PARCH_STEAM_THINS = 2,
  PARCH_STEAM_SHRINKS = 3
};

/// Parch's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the glow disc over the
/// target and the 64 puffs of steam that rise as it dries.
struct ParchState {
  u8 tiles[4096];
  struct EffectSprite disc;
  u8 unusedAfterDisc[40]; ///< no code names it
  s32 discX;
  s32 discY;
  s32 discScaleX;
  s32 discScaleY;
  struct EffectSprite sprites[PARCH_PUFF_COUNT];
  struct ParchPuff particles[PARCH_PUFF_COUNT];
};

void BattleEffect_FadeToSavedPalettes(s32 frames);

/// Parch ("Evaporate standing water."): a glowing disc spreads 32 pixels before
/// the caster and white puffs rise from it. On a puddle (SPRITE_PUDDLE), or on
/// water with a Parch event, the water there dries up and the event runs.
void FieldPsynergy_Parch(void) {
  struct ParchState *state =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct ParchState));
  struct HeapSlots *heap;
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct WindowState *windowState =
      (struct WindowState *)Ram_HeapSlots->windowState;
  s32 offset; // into the field state's actor list
  struct Actor *caster;
  struct Actor *actor;
  struct Sprite *actorSprite;
  struct EffectSprite *sprite;
  struct EffectSprite *fresh;
  struct ParchPuff *particle;
  FIXED screen[3];
  FIXED pos[3];
  s32 phase;
  s32 timer;
  s32 frame;
  s32 entry;
  s32 tile;
  s32 next;
  s32 spawn;
  s32 fade;
  s32 result;
  s32 found;
  s32 kind;
  void *pending;
  s32 scaleY;
  s32 scaleX;
  s32 y;
  s32 x;
  s32 red;
  s32 green;
  s32 blue;
  s32 add;
  s32 packed;
  s32 i;
  u8 *dst;

  caster = scene->caster;
  found = -1;
  fade = -1;
  pending = NULL;
  FieldPsynergy_FadeToCastColor();
  pos[0] = caster->x.fixed;
  pos[1] = caster->y.fixed;
  pos[2] = caster->z.fixed;
  Vector_AddPolarOffset(toFIXED(32), scene->angle, pos);

  i = 0;
  kind = SPRITE_PUDDLE;
  heap = Ram_HeapSlots;
  for (offset = offsetof(struct FieldState, actors); i < FIELD_ACTOR_COUNT;
       i++, offset += sizeof(struct Actor *)) {
    actor = *(struct Actor **)((u8 *)heap->fieldState + offset);
    actorSprite = actor->sprite;
    if (actor->script != NULL && actorSprite->entries[0]->animationId == kind &&
        actorSprite->lastNo == PUDDLE_ANIMATION_PUDDLE &&
        FieldPsynergy_CheckOverlap(pos, 4, &actor->x.fixed,
                                   actor->radius - 2) >= 0) {
      result = PARCH_DRIES;
      found = i;
      Actor_GetById(i)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      goto loaded;
    }
  }
  if (Field_MeasurePsynergyHeight(scene->fieldPsynergy, caster, pos) == -1) {
    result = PARCH_NOTHING;
    // DEAD: pending is already NULL; the ROM keeps the second store.
    pending = NULL;
  } else if (MapGrid_IsWaterCell(pos[0], pos[2], caster->layer) != 0) {
    pending = Scene_FindPsynergyTrigger(
        PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), scene->fieldPsynergy,
        MapGrid_GetCellBehavior(pos[0], pos[2], caster->layer));
    result = PARCH_DRIES;
    if (pending == NULL)
      result = PARCH_WATER;
  } else {
    result = PARCH_NOTHING;
  }
loaded: {
  void *source = Resource_GetTableEntry(RESOURCE_GLOW_DISC);
  dst = (u8 *)OBJ_VRAM0 + PARCH_DISC_TILE_OFFSET * TILE_SIZE_4BPP +
        windowState->glyphTiles * TILE_SIZE_4BPP;
  Decompress_TaggedLz(source, state);
}
  Dma_Set(state, dst, DMA_COPY_WORDS(1024), REG_DMA3);
  screen[0] = pos[0];
  screen[1] = pos[1];
  screen[2] = pos[2];
  Camera_WorldToScreen(screen);
  state->discX = screen[0];
  state->discY = screen[2];
  state->discScaleX = 0;
  state->discScaleY = 0;
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_WHITE_PUFF), state);
  entry = VramCache_FindFreeEntry();
  tile = VramCache_Load(entry, 2048, state);
  particle = state->particles;
  for (i = 0; i < PARCH_PUFF_COUNT; i++) {
    fresh = &state->sprites[i];
    EffectSprite_Init(fresh, 8, 15, OBJ_SIZE_16X16, tile);
    fresh->depth = EFFECT_SPRITE_FRONT_DEPTH;
    fresh->oam.priority = 0;
    fresh->oam.fullColor = 1;
    fresh->oam.palette = 0;
    particle->timer = -1;
    particle++;
  }
  Sound_PlayFar(SE_PSYNERGY_START);
  wait(1);
  next = 0;
  spawn = 0;
  phase = PARCH_SWELLS;
  timer = 0;
  frame = 0;
  do {
    for (i = spawn; i != 0; i--) {
      s32 slot = next & (PARCH_PUFF_COUNT - 1);
      sprite = &state->sprites[slot];
      sprite->scaleX = FIXED_ONE / 5;
      sprite->scaleY = FIXED_ONE / 5;
      particle = &state->particles[slot];
      SET_POSITION(particle, pos[0], pos[1], pos[2]);
      Vector_AddPolarOffset(Random16() * 12 + toFIXED(9), Random16(),
                            &particle->x);
      particle->sway = (Random16() << 5) >> 16;
      particle->swayRate = ((Random16() << 9) >> 16) + 256;
      particle->rise = Random16() + toFIXED(1) / 5;
      particle->timer = 0;
      next = (next + 1) & (PARCH_PUFF_COUNT - 1);
    }
    for (i = 0, particle = state->particles; i < PARCH_PUFF_COUNT;
         i++, particle++) {
      if (particle->timer >= 0) {
        s32 grown;

        sprite = &state->sprites[i];
        sprite->oam.index = tile + ((particle->timer >> 2) & 7) * 8;
        grown = sprite->scaleX + FIXED_ONE / 10;
        // DEAD: stored again after the clamp; the ROM keeps both stores.
        sprite->scaleX = grown;
        if (grown > FIXED_ONE)
          grown = FIXED_ONE;
        sprite->scaleX = grown;
        sprite->scaleY = grown;
        screen[0] = particle->x;
        screen[1] = particle->y;
        screen[2] = particle->z;
        Camera_WorldToScreen(screen);
        sprite->x =
            screen[0] + (Trig_Sin(particle->sway * particle->swayRate) << 3);
        sprite->y = screen[2];
        EffectSprite_Draw(sprite);
        particle->sway++;
        particle->y += particle->rise;
        if (++particle->timer == 32)
          particle->timer = -1;
      }
    }

    for (i = 0; i < PARCH_GLOW_COLORS; i++) {
      if (fade == -1)
        add = 0;
      else
        add = fade / 4;
      blue = Trig_Sin((frame / 2 + i) * (TRIG_TURN / 32)); // green's sine
      green = FIXEDtoINT(blue * GLOW_GREEN_SWING) + GLOW_GREEN;
      blue = Trig_Sin((frame / 4 + i) * (TRIG_TURN / 32) + TRIG_TURN * 9 / 32);
      blue = FIXEDtoINT(blue * GLOW_BLUE_SWING) + GLOW_BLUE;
      red = add + GLOW_RED;
      if (red > RGB_LEVEL_MAX)
        red = RGB_LEVEL_MAX;
      green += add;
      if (green > RGB_LEVEL_MAX)
        green = RGB_LEVEL_MAX;
      blue += add;
      if (blue > RGB_LEVEL_MAX)
        blue = RGB_LEVEL_MAX;
      Io_Write16(RGB(red, green, blue), &OBJ_PLTT[OBJ_PLTT_SIZE / 2 - 1 - i]);
    }
    if (fade != -1 && fade <= 126)
      fade++;

    x = state->discX;
    y = state->discY;
    scaleX = state->discScaleX;
    scaleY = state->discScaleY;
    packed = windowState->glyphTiles + PARCH_DISC_TILE_OFFSET;
    sprite = &state->disc;
    EffectSprite_Init(sprite, 31, 31, OBJ_SIZE_64X64, packed);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority =
        FieldPsynergy_GetCanvasPriority(scene->caster, scene->angle);
    sprite->depth = 1;
    sprite->x = x;
    sprite->y = y;
    sprite->scaleX = scaleX;
    sprite->scaleY = scaleY;
    EffectSprite_Draw(sprite);

    if (result == PARCH_NOTHING) {
      switch (phase) {
      case PARCH_SWELLS:
        if (timer == 10)
          Sound_PlayFar(SE_PARCH);
        state->discScaleX += FIXED_ONE * 3 / 256;
        state->discScaleY = state->discScaleX;
        if (state->discScaleX > toFIXED(0.25) - 1) {
          phase = PARCH_HOLDS;
          timer = -1;
        }
        break;
      case PARCH_HOLDS:
        if (timer == 30) {
          phase = PARCH_EMPTY_SHRINKS;
          timer = -1;
        }
        break;
      case PARCH_EMPTY_SHRINKS:
        spawn = 0;
        state->discScaleX += -(FIXED_ONE * 3 / 256);
        state->discScaleY = state->discScaleX;
        if (state->discScaleX <= 0)
          phase = FIELD_PSYNERGY_FINISHED;
        break;
      }
    }
    if (result == PARCH_WATER) {
      switch (phase) {
      case PARCH_SWELLS:
        if (timer == 30)
          Sound_PlayFar(SE_SPIN);
        state->discScaleX += FIXED_ONE * 3 / 256;
        state->discScaleY = state->discScaleX;
        if (state->discScaleX > toFIXED(0.75) - 1) {
          phase = PARCH_STEAMS;
          timer = -1;
        }
        break;
      case PARCH_STEAMS:
        spawn = 2;
        if (timer == 40) {
          phase = PARCH_STEAM_THINS;
          timer = -1;
        }
        break;
      case PARCH_STEAM_THINS:
        spawn = 1;
        if (timer == 40) {
          phase = PARCH_STEAM_SHRINKS;
          timer = -1;
        }
        break;
      case PARCH_STEAM_SHRINKS:
        spawn = 0;
        state->discScaleX += -(FIXED_ONE / 64);
        state->discScaleY = state->discScaleX;
        if (state->discScaleX <= 0)
          phase = FIELD_PSYNERGY_FINISHED;
        break;
      }
    }
    if (result == PARCH_DRIES) {
      switch (phase) {
      case PARCH_SWELLS:
        if (timer == 40)
          Sound_PlayFar(SE_SPIN);
        state->discScaleX += FIXED_ONE * 3 / 256;
        state->discScaleY = state->discScaleX;
        if (state->discScaleX > toFIXED(0.75) - 1) {
          phase = PARCH_STEAMS;
          timer = -1;
        }
        break;
      case PARCH_STEAMS:
        if (timer == 40) {
          fade = 0;
          PaletteFade_SetTarget(RGB_WHITE, 0);
          PaletteFade_StartInterpolation(100);
        }
        spawn = 2;
        if (timer == 100) {
          phase = PARCH_STEAM_THINS;
          timer = -1;
        }
        break;
      case PARCH_STEAM_THINS:
        spawn = 1;
        if (timer == 80)
          phase = FIELD_PSYNERGY_FINISHED;
        break;
      }
    }
    wait(1);
    timer++;
    frame++;
  } while (phase != FIELD_PSYNERGY_FINISHED);

  if (found != -1) {
    struct Actor *marked = Actor_GetById(found);
    MapGrid_SetCellBehavior(marked->x.fixed, marked->z.fixed, marked->layer, 0);
    Actor_SetHorizontalPositionWithTerrain(found, 0, 0);
  }
  if (pending != NULL)
    Field_RunPsynergyEvent(pending, scene->casterId, scene->targetId);
  VramCache_FreeEntry(entry);
  if (result == PARCH_DRIES)
    BattleEffect_FadeToSavedPalettes(100);
  else
    FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

void BattleEffect_PlotCanvasPixel(s32 x, s32 y, s32 color);

/// Plots one pixel of the 128x64 canvas on a circle around its bottom middle
/// (63, 63): a radius (16.16) and an angle, the cosine leftward and the sine
/// upward.
void Canvas_PlotPolarPixel(s32 unused, FIXED radius, s32 angle, s32 color) {
  BattleEffect_PlotCanvasPixel(
      63 - FIXEDtoINT(Iwram_MulFX(Trig_Cos(angle), radius)),
      63 - FIXEDtoINT(Iwram_MulFX(Trig_Sin(angle), radius)), color);
}

extern const u16 gParch_PictureTiles[];

/// The picture BgMap_DrawClipped16x16 draws, 16 tiles square.
#define CLIPPED_PICTURE_TILES 16

/// Copies Parch's 16x16-tile picture (gParch_PictureTiles) to BG0's map,
/// leaving out the tiles that would fall outside the 30x20-tile screen at (x,
/// y).
void BgMap_DrawClipped16x16(s32 x, s32 y) {
  const u16 *tiles = gParch_PictureTiles;
  s32 row;
  s32 column;
  s32 sx;
  s32 sy;

  x /= 8;
  y /= 8;
  IWRAM_FILL_WORDS((void *)BG_SCREEN_BLOCK(4), 1024, 0);
  for (row = 0, sy = y; row < CLIPPED_PICTURE_TILES; row++, sy++) {
    if (sy < 0)
      continue;
    if (sy > DISPLAY_TILE_HEIGHT - 1)
      break;
    for (sx = x, column = 0; column < CLIPPED_PICTURE_TILES; column++, sx++) {
      if (sx < 0)
        continue;
      if (sx > DISPLAY_TILE_WIDTH - 1)
        break;
      ((u16 *)BG_SCREEN_BLOCK(4))[row * BG_SCREEN_WIDTH + column] =
          tiles[row * CLIPPED_PICTURE_TILES + column];
    }
  }
}
#undef PARCH_DISC_TILE_OFFSET
#undef PARCH_GLOW_COLORS
#undef CLIPPED_PICTURE_TILES
