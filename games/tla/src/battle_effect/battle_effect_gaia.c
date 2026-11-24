/// @file
/// Gaia, and the effect sprites with tiles of their own.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define GAIA_FRAMES 192 ///< frames of Gaia
/// The shaft of light widens into the wall a row every three frames from
/// GAIA_WALL_WIDEN_FRAME, stands once it has risen, and sinks away a row every
/// three frames.
#define GAIA_WALL_WIDEN_FRAME 64
#define GAIA_WALL_STAND_FRAME 88
#define GAIA_WALL_SINK_FRAME 160

#define GAIA_ROCKS 11 ///< rock sprites tumbling up through the wall
/// The rocks that fly from the start; the rest join them from frame 72.
#define GAIA_EARLY_ROCKS 6
/// The rocks' motion is state->particles[GAIA_FIRST_ROCK] on.
#define GAIA_FIRST_ROCK 10
/// The columns gGaia_LateRockColumns gives the rocks after the early ones.
#define GAIA_LATE_ROCK_COLUMNS 6
/// A rock waits this many frames longer than the rock after it before it first
/// flies.
#define GAIA_ROCK_STAGGER 4
#define GAIA_DUST 256 ///< dust in PARTICLES
/// The dust drawn: the rest is set out and never shown.
#define GAIA_DUST_SHOWN 64

/// The pictures Gaia rebuilds from the earth wall sprite sheet: the rising
/// wall's rows, the standing wall's top and foot, then the flames.
#define GAIA_ROW_HEIGHT 32
/// The rising wall's rows, narrowest first, and the flames at its foot.
#define GAIA_ROW_PICTURES 9
#define GAIA_FLAME_PICTURES 3
/// The standing wall's top, stretched from a 48x32 row, and its foot.
#define GAIA_WALL_TOP_WIDTH 48
#define GAIA_WALL_TOP_HEIGHT 96
#define GAIA_WALL_FOOT_WIDTH 48
#define GAIA_WALL_FOOT_HEIGHT 21
/// Where each picture starts in EarthWallSheet, in bytes.
enum EarthWallSheetPicture {
  GAIA_WALL_TOP = GAIA_ROW_HEIGHT * (5 + 6 + 9 + 12 + 15 + 18 + 21 + 32 + 40),
  GAIA_WALL_FOOT = GAIA_WALL_TOP + PICTURE_BYTES(GAIA_WALL_TOP)
};

/// Flames flicker at the targets' feet under a narrow shaft of orange light,
/// which widens into a glowing wall and stands while rocks tumble up through it
/// and the targets are battered, then narrows away again.
///
/// Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche.
void BattleEffect_RunGaia(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  u8 *sprites;
  s32 wallX;
  s32 rockX;
  struct Scale scale;
  FIXED position[4];
  u8 *src;
  u16 *color;
  void *palette = (void *)BG_PLTT;
  s32 sheetIndex;
  s32 i;
  s32 j;
  s32 k;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);

  // The higher the variant, the wider the wall.
  if (args->variant == EFFECT_VARIANT_BASIC)
    REG_BG2PA = Q_8_8(0.8);
  else if (args->variant == EFFECT_VARIANT_STRONGER)
    REG_BG2PA = Q_8_8(1.0 / 1.5);
  if (args->side == EFFECT_SIDE_PARTY) {
    wallX = 8;
    if (args->variant == EFFECT_VARIANT_BASIC)
      rockX = 40;
    else if (args->variant == EFFECT_VARIANT_STRONGER)
      rockX = 36;
    else
      rockX = 40;
  } else {
    wallX = -16;
    rockX = -12;
  }
  REG_BG2X = gGaia_Bg2Shifts[args->side][args->variant] << 8;

  Resource_LoadAndDecompress(RESOURCE_EARTH_WALL_SHEET, Ram_MapCellBuffer, 1,
                             0);
  Iwram_CopyWordsEntry(palette,
                       Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_A),
                       SHEET_PALETTE_BYTES);
  color = (u16 *)&BG_PLTT_COLOR(0, 1);
  for (i = 0; i != SHEET_PALETTE_COLORS - 1; i++) {
    s32 c = *color;
    s32 blue = RGB_BLUE((u16)c);
    s32 green = RGB_GREEN((u16)c);
    s32 red = RGB_LEVEL_MAX;

    red &= c;
    blue -= 8;
    green -= 8;
    red -= 8;

    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = RGB(red, green, blue);
  }

  src = Ram_MapCellBuffer;
  sheetIndex = 0;
  for (j = 0; j != GAIA_ROW_PICTURES; j++) {
    for (i = 0; i != GAIA_ROW_HEIGHT; i++) {
      for (k = 0; k != gGaia_RowWidths[j]; k++, sheetIndex++)
        state->sheet[sheetIndex] = src[k];
    }
    src += gGaia_RowWidths[j];
  }
  for (j = 0; j != GAIA_ROW_HEIGHT; j++) {
    for (i = 0; i != GAIA_WALL_TOP_HEIGHT / GAIA_ROW_HEIGHT; i++) {
      for (k = 0; k != GAIA_WALL_TOP_WIDTH; k++, sheetIndex++)
        state->sheet[sheetIndex] = src[k];
    }
    src += GAIA_WALL_TOP_WIDTH;
  }
  for (i = 0; i != GAIA_WALL_FOOT_WIDTH * GAIA_WALL_FOOT_HEIGHT; i++)
    state->sheet[sheetIndex++] = *src++;
  for (i = 0; i != GAIA_FLAME_PICTURES; i++) {
    for (k = 0; k != gGaia_FlameWidths[i] * gGaia_FlameHeights[i]; k++)
      state->sheet[sheetIndex++] = *src++;
  }
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);

  for (i = 0; i != GAIA_ROCKS; i++) {
    struct Sprite *rock = Sprite_CreateFar(SPRITE_BATTLE_ROCKS);

    state->sprites[i] = rock;
    if (rock != 0) {
      rock->flags = 0;
      Sprite_StartAnimationFar(rock, i / 4);
      ((struct Sprite *)state->sprites[i])->image.priority = 1;
    }
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  // Rocks wait their turn (variant counts the frames) and tumble up through the
  // wall, y counting down from 128 by z a frame.
  for (i = 0; i != GAIA_ROCKS; i++) {
    struct Particle *rock = &state->particles[GAIA_FIRST_ROCK + i];

    SET_POSITION(rock, (Random16() & 15) + 88, 128, (Random16() & 15) + 2);
    SET_VELOCITY(rock, i, 1, toFIXED(1) / 2);
    rock->variant = GAIA_ROCKS * GAIA_ROCK_STAGGER - i * GAIA_ROCK_STAGGER;
  }
  // DEAD: the sixth column lands on the particle after the last rock, which
  // nothing reads; the ROM keeps the store.
  for (i = 0; i != GAIA_LATE_ROCK_COLUMNS; i++)
    state->particles[GAIA_FIRST_ROCK + GAIA_EARLY_ROCKS + i].x =
        gGaia_LateRockColumns[i];

  // Dust rises from the foot of the wall.
  for (i = 0; i != GAIA_DUST; i++) {
    struct Particle *dust = &PARTICLES[i];

    dust->x = INTtoFIXED((Random16() & 63) + wallX + 32);
    dust->y = INTtoFIXED((Random16() & 7) + 96);
    dust->velocityY = ((Random16() & 63) + 32) << 13;
    dust->variant = Random16() & 31;
  }
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  // The screen shakes throughout.
  state->shakeFrames = 250;
  for (frame = 0; frame != GAIA_FRAMES; frame++) {
    if (frame == 0)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 40)
      Sound_PlayFar(SE_RUMBLE);
    if (frame == 96)
      Sound_PlayFar(SE_CRASH);
    if (frame == 120)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // The flames flicker at the wall's foot.
    if (frame <= 81) {
      s32 picture = frame / 4;

      if (picture > 2)
        picture = (picture & 1) + 1;
      draw[0](canvas, state->sheet + gGaia_FlamePictures[picture],
              wallX - gGaia_FlameWidths[picture] + 64,
              116 - gGaia_FlameHeights[picture], gGaia_FlameWidths[picture],
              gGaia_FlameHeights[picture]);
      draw[1](canvas, state->sheet + gGaia_FlamePictures[picture], wallX + 64,
              116 - gGaia_FlameHeights[picture], gGaia_FlameWidths[picture],
              gGaia_FlameHeights[picture]);
    }

    // The rising wall widens a row at a time.
    if (frame >= 12 && frame < GAIA_WALL_STAND_FRAME) {
      s32 row = (frame - GAIA_WALL_WIDEN_FRAME) / 3;

      CLAMP(row, 0, 7);
      for (i = 0; i != 4; i++) {
        draw[0](canvas, state->sheet + gGaia_RowPictures[row],
                wallX - gGaia_RowWidths[row] + 64, i * GAIA_ROW_HEIGHT - 12,
                gGaia_RowWidths[row], GAIA_ROW_HEIGHT);
        draw[1](canvas, state->sheet + gGaia_RowPictures[row], wallX + 64,
                i * GAIA_ROW_HEIGHT - 12, gGaia_RowWidths[row],
                GAIA_ROW_HEIGHT);
      }
    }

    // The wall sinks away.
    if (frame >= GAIA_WALL_SINK_FRAME && frame < GAIA_WALL_SINK_FRAME + 24) {
      s32 row = 7 - (frame - GAIA_WALL_SINK_FRAME) / 3;

      CLAMP(row, 0, 7);
      for (i = 0; i != 4; i++) {
        draw[0](canvas, state->sheet + gGaia_RowPictures[row],
                wallX - gGaia_RowWidths[row] + 64, i * GAIA_ROW_HEIGHT - 12,
                gGaia_RowWidths[row], GAIA_ROW_HEIGHT);
        draw[1](canvas, state->sheet + gGaia_RowPictures[row], wallX + 64,
                i * GAIA_ROW_HEIGHT - 12, gGaia_RowWidths[row],
                GAIA_ROW_HEIGHT);
      }
    }

    // The standing wall.
    if (frame >= GAIA_WALL_STAND_FRAME && frame < GAIA_WALL_SINK_FRAME) {
      draw[0](canvas, &state->sheet[GAIA_WALL_TOP], wallX + 16, 0,
              GAIA_WALL_TOP_WIDTH, GAIA_WALL_TOP_HEIGHT);
      draw[1](canvas, &state->sheet[GAIA_WALL_TOP], wallX + 64, 0,
              GAIA_WALL_TOP_WIDTH, GAIA_WALL_TOP_HEIGHT);
      draw[0](canvas, &state->sheet[GAIA_WALL_FOOT], wallX + 16,
              GAIA_WALL_TOP_HEIGHT, GAIA_WALL_FOOT_WIDTH,
              GAIA_WALL_FOOT_HEIGHT);
      draw[1](canvas, &state->sheet[GAIA_WALL_FOOT], wallX + 64,
              GAIA_WALL_TOP_HEIGHT, GAIA_WALL_FOOT_WIDTH,
              GAIA_WALL_FOOT_HEIGHT);
    }

    // Dust keeps rising up the wall.
    if (frame >= GAIA_WALL_STAND_FRAME) {
      for (i = 0; i != GAIA_DUST_SHOWN; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant == 0) {
          s32 size = (i & 3) + 5;

          draw[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                  FIXED_INTEGER_PART(dust->x) - size / 2,
                  FIXED_INTEGER_PART(dust->y) - size, size, size * 2);
          dust->y -= dust->velocityY;
          if (dust->y < 0 && frame < GAIA_WALL_SINK_FRAME)
            dust->y = toFIXED(96);
        } else {
          dust->variant--;
        }
      }
    }

    // Rocks tumble up through the wall, larger with the variant.
    if (frame > 4) {
      s32 rocks = GAIA_EARLY_ROCKS;

      if (frame > 71)
        rocks = GAIA_ROCKS;
      for (k = 0; k != rocks; k++) {
        struct Particle *rock = &state->particles[GAIA_FIRST_ROCK + k];
        s32 slot;

        if (rock->variant == 0) {
          // DEAD: scale copied from gGaia_UnitScale before both its fields are
          // set; the ROM keeps the copy.
          scale = gGaia_UnitScale;
          if (frame > 71) {
            scale.x = k * (FIXED_ONE / 16) + FIXED_ONE / 2;
            scale.x += args->variant * (FIXED_ONE / 4);
          } else {
            scale.x = FIXED_ONE / 2;
          }
          scale.y = scale.x;
          position[3] = 0;
          position[0] = INTtoFIXED(rock->x + rockX * 2);
          position[1] = toFIXED(512) - INTtoFIXED(rock->y);
          position[2] = toFIXED(512);
          slot = (frame / 2 + k) % GAIA_ROCKS;
          // DEAD: slot != -1 always holds, slot being a remainder of a
          // positive count; the ROM keeps the test.
          if (slot != -1)
            Render_ApplyProjectedPlacementFar(state->sprites[slot], position,
                                              &scale, 0);
          rock->y -= rock->z;
          // DEAD: velocityX steps from 1 to 12 by velocityY and nothing reads
          // it; the ROM keeps the count.
          rock->velocityX += rock->velocityY;
          if (rock->velocityX > 12)
            rock->velocityX -= 12;
          if (rock->y < 0) {
            if (frame >= GAIA_WALL_SINK_FRAME) {
              rock->variant = -1;
            } else {
              if (frame >= GAIA_WALL_STAND_FRAME) {
                rock->z = (Random16() & 7) + 8;
                if (args->variant == EFFECT_VARIANT_BASIC)
                  rock->x = Random16() % 96 + 42;
                else if (args->variant == EFFECT_VARIANT_STRONGER)
                  rock->x = Random16() % 112 + 34;
                else
                  rock->x = Random16() % 160 + 10;
              }
              rock->y = 128;
              rock->variant = 8;
            }
          }
        } else {
          rock->variant--;
        }
      }
    }

    // The targets flash and recoil.
    if (frame <= 158) {
      EFFECT_FOR_EACH_TARGET(i, args) {
        if (frame > 85) {
          if (frame % 12 == 0)
            BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, i, 6);
          if ((frame & 3) == 0)
            BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_SKID);
        }
      }
    }
    // While the wall stands the screen shakes hard, otherwise softly.
    if (frame < 90 || frame > GAIA_WALL_SINK_FRAME)
      BattleCamera_Shake(2, 2);
    else
      BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  for (i = 0; i != GAIA_ROCKS; i++)
    Sprite_DestroyFar(state->sprites[i]);
  BattleEffect_EndCanvasLayer();
}

/// Takes the first free animation entry of the 64 and clears it for use. With
/// none free it clears the entry at address 0.
struct SpriteAnimation *AnimationObject_AllocateEmpty(void) {
  struct SpriteAnimation *entry = Ram_HeapSlots->spriteAnimations;
  struct SpriteAnimation *found = NULL;
  s32 i = 0;
  void *none = NULL;

  while (i < ANIM_OBJECT_COUNT) {
    if (entry->kind == 0) {
      found = entry;
      break;
    }
    i++;
    entry++;
  }
  found->animationId = 0;
  found->scripts = NULL;
  found->pictures = NULL;
  found->script = none;
  found->mode = 0;
  found->picture = 0;
  found->scriptOffset = 0;
  found->kind = 1;
  found->colorMode = 0;
  return found;
}

/// A struct Sprite as EffectSprite_CreateWithTiles searches and fills it: its
/// OAM attributes and its shadow's written as words.
struct SpriteWords {
  s32 link; ///< the link to the next part at its depth, as
            ///< AnimationSpritePart's
  s32 attributes;
  union {
    u32 word;
    struct {
      u32 tile : 10;
      u32 rest : 22;
    } bits;
  } oam;
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 sunkRows : 6;
  u16 rotation;
  u8 width;
  u8 height;
  u8 offsetX;
  u8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  /// The shadow's link, attributes 0 and 1, and attribute 2.
  s32 shadowLink;
  s32 shadowAttributes;
  s32 shadowTile;
  struct SpriteAnimation *entry; ///< its first layer
  struct SpriteAnimation *otherEntries[3];
};

/// Makes an effect sprite with tiles of its own in object VRAM: a free VRAM
/// cache entry and the first free effect sprite of the 64, as
/// EffectSprite_Acquire takes, set up for Sprite_StartAnimationFar. The
/// Lost Age's own.
void *EffectSprite_CreateWithTiles(s32 width, s32 height, s32 attributes,
                                   s32 flags) {
  struct SpriteWords *found = NULL;
  s32 vramEntry = VramCache_FindFreeEntry();
  struct SpriteWords *sprite = Ram_HeapSlots->sprites;
  s32 i = 0;
  struct ActorSystemState *actorSystem;
  s32 size;
  s32 tile;
  struct VramCacheEntry *entries;

  while (i < ANIM_OBJECT_COUNT) {
    if (sprite->width == 0) {
      found = sprite;
      break;
    }
    i++;
    sprite++;
  }
  tile = VramCache_Load(vramEntry, 0, 0);
  found->slot = vramEntry;
  found->rotation = 0;
  found->flags = 0;
  found->retainCacheEntry = 0;
  {
    u32 *words = (u32 *)found;

    *words++ = 0;
    *words++ = attributes;
    *words = flags | tile | OBJ_PRIORITY(2);
  }
  {
    s32 *p = &found->shadowLink;

    *p = 0;
  }
  // The shadow, as the field's sprites take it from the shared shadow tiles:
  // the table read as halfwords, the shadow entry's second its offset.
  found->shadowAttributes = OBJ_SIZE_16X8 | OBJ_256_COLORS;
  entries = gVramCacheEntries;
  found->shadowTile =
      (((u16 *)entries)[VRAM_CACHE_ENTRY_ACTOR_SHADOW * 2 + 1] >> 5) |
      OBJ_PRIORITY(2);
  sprite->entry = AnimationObject_AllocateEmpty();
  sprite->width = width;
  sprite->scale = FIXED_ONE;
  sprite->height = height;
  sprite->offsetY = 0;
  sprite->offsetX = 0;
  sprite->count = 1;
  sprite->flags = 0;
  Sprite_StartAnimationFar(sprite, 0);

  actorSystem = Ram_HeapSlots->actorSystem;
  size = sprite->width * sprite->height;
  if (!(attributes & OBJ_256_COLORS))
    size = (u32)size >> 1;
  VramCache_Load(sprite->slot, size, 0);
  actorSystem->used += size;
  sprite->oam.bits.tile = entries[sprite->slot].offset >> 5;
  return found;
}
#undef GAIA_FRAMES
#undef GAIA_WALL_WIDEN_FRAME
#undef GAIA_WALL_STAND_FRAME
#undef GAIA_WALL_SINK_FRAME
#undef GAIA_ROCKS
#undef GAIA_EARLY_ROCKS
#undef GAIA_FIRST_ROCK
#undef GAIA_LATE_ROCK_COLUMNS
#undef GAIA_ROCK_STAGGER
#undef GAIA_DUST
#undef GAIA_DUST_SHOWN
#undef GAIA_ROW_HEIGHT
#undef GAIA_ROW_PICTURES
#undef GAIA_FLAME_PICTURES
#undef GAIA_WALL_TOP_WIDTH
#undef GAIA_WALL_TOP_HEIGHT
#undef GAIA_WALL_FOOT_WIDTH
#undef GAIA_WALL_FOOT_HEIGHT
