/// @file
/// Gaia, and the effect sprites with tiles of their own.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#undef ROCK_COUNT

/// The dust lives in the map cell buffer once the picture is laid out.
#define DUST ((struct Particle *)Ram_MapCellBuffer)
#define ROCK_COUNT 11
/// The rocks' motion is particles[10..20], the late rocks' columns [16..21].
#define FIRST_ROCK 10

#define GAIA_FRAMES 192

#define GAIA_WALL_SINK_FRAME 160
#define GAIA_DUST 256 ///< dust in PARTICLES
/// The pictures Gaia rebuilds from the earth wall sheet: the rising wall's
/// rows, the standing wall's top and foot, then the flames.
#define GAIA_ROW_HEIGHT 32

/// The wall stands once it has risen, and sinks away a row every three
/// frames.
#define GAIA_WALL_STAND_FRAME 88
#define GAIA_WALL_FOOT_HEIGHT 21

/// Flames flicker at the targets' feet under a narrow shaft of orange light,
/// which widens into a glowing wall and stands while rocks tumble up through it
/// and the targets are battered, then narrows away again.
///
/// Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche.
void BattleEffect_RunGaia(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  u8 *sheet;
  s32 wallX;
  s32 rockX;
  struct Scale scale;
  s32 position[4];
  u8 *source;
  u16 *color;
  void *palette = (void *)BG_PLTT;
  s32 offset;
  s32 i;
  s32 j;
  s32 k;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);

  // The larger variants stretch the wall wider: BG2 at 1.25 and 1.5.
  if (args->variant == EFFECT_VARIANT_BASIC)
    REG_BG2PA = 204;
  else if (args->variant == EFFECT_VARIANT_STRONGER)
    REG_BG2PA = 170;
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

  // The palette: BG colours 1-63 darkened eight steps in each channel.
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, Ram_MapCellBuffer,
                             1, 0);
  Iwram_CopyWordsEntry(
      palette, Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
  color = (u16 *)&BG_PLTT_COLOR(0, 1);
  for (i = 0; i != 63; i++) {
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

  // The picture, laid out again in the effect work:
  //   the growing wall: nine one-line rows, each stretched to 32 lines;
  //   the top of the standing wall at 0x13c0: 48x32, stretched to 48x96;
  //   its foot at 0x25c0: 48x21;
  //   the mound: three cels.
  source = Ram_MapCellBuffer;
  offset = 0;
  for (j = 0; j != 9; j++) {
    for (i = 0; i != 32; i++) {
      for (k = 0; k != gGaia_RowWidths[j]; k++, offset++)
        work->sheet[offset] = source[k];
    }
    source += gGaia_RowWidths[j];
  }
  for (j = 0; j != 32; j++) {
    for (i = 0; i != 3; i++) {
      for (k = 0; k != 48; k++, offset++)
        work->sheet[offset] = source[k];
    }
    source += 48;
  }
  for (i = 0; i != 1008; i++)
    work->sheet[offset++] = *source++;
  for (i = 0; i != 3; i++) {
    for (k = 0; k != gGaia_FlameWidths[i] * gGaia_FlameHeights[i]; k++)
      work->sheet[offset++] = *source++;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

  // The rocks: eleven animated objects, four of each size.
  for (i = 0; i != ROCK_COUNT; i++) {
    struct Sprite *object = Sprite_CreateFar(653);

    work->sprites[i] = object;
    if (object != 0) {
      object->flags = 0;
      Sprite_StartAnimationFar(object, i / 4);
      ((struct Sprite *)work->sprites[i])->image.priority = 1;
    }
  }
  // Two max blitters, the second mirrored for the right half.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // A rock's x is its column, y counts down from 128 by z, velocityX cycles
  // 1 to 12 as its animation step, and variant is its wait before it flies.
  for (i = 0; i != ROCK_COUNT; i++) {
    struct Particle *rock = &work->particles[FIRST_ROCK + i];

    SET_POSITION(rock, (Random16() & 15) + 88, 128, (Random16() & 15) + 2);
    rock->velocityX = i;
    rock->velocityY = 1;
    rock->velocityZ = px(1) / 2;
    rock->variant = 44 - i * 4;
  }
  for (i = 0; i != 6; i++)
    work->particles[16 + i].x = gGaia_LateRockColumns[i];

  // The dust rises from the foot of the wall after a random wait.
  for (i = 0; i != GAIA_DUST; i++) {
    struct Particle *dust = &DUST[i];

    dust->x = px((Random16() & 63) + wallX + 32);
    dust->y = px((Random16() & 7) + 96);
    dust->velocityY = ((Random16() & 63) + 32) << 13;
    dust->variant = Random16() & 31;
  }
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  work->shakeFrames = 250;

  for (frame = 0; frame != GAIA_FRAMES; frame++) {
    if (frame == 0)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 40)
      Sound_PlayFar(SE_RUMBLE);
    if (frame == 96)
      Sound_PlayFar(SE_CRASH);
    // The battle resumes and resolves the targets, hit cue 134.
    if (frame == 120)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // The ground heaves up (frames 0-81), then pulses between two cels.
    if (frame <= 81) {
      s32 cel = frame / 4;

      if (cel > 2)
        cel = (cel & 1) + 1;
      draw[0](canvas, work->sheet + gGaia_FlamePictures[cel],
              wallX - gGaia_FlameWidths[cel] + 64,
              116 - gGaia_FlameHeights[cel], gGaia_FlameWidths[cel],
              gGaia_FlameHeights[cel]);
      draw[1](canvas, work->sheet + gGaia_FlamePictures[cel], wallX + 64,
              116 - gGaia_FlameHeights[cel], gGaia_FlameWidths[cel],
              gGaia_FlameHeights[cel]);
    }

    // The wall grows out of it, a row every three frames (frames 12-87).
    if (frame >= 12 && frame <= 87) {
      s32 row = (frame - 64) / 3;

      if (row < 0)
        row = 0;
      if (row > 7)
        row = 7;
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowPictures[row],
                wallX - gGaia_RowWidths[row] + 64, i * GAIA_ROW_HEIGHT - 12,
                gGaia_RowWidths[row], GAIA_ROW_HEIGHT);
        draw[1](canvas, work->sheet + gGaia_RowPictures[row], wallX + 64,
                i * GAIA_ROW_HEIGHT - 12, gGaia_RowWidths[row], 32);
      }
    }

    // It sinks back the same way (frames 160-183).
    if (frame >= GAIA_WALL_SINK_FRAME && frame <= 183) {
      s32 row = 7 - (frame - GAIA_WALL_SINK_FRAME) / 3;

      if (row < 0)
        row = 0;
      if (row > 7)
        row = 7;
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowPictures[row],
                wallX - gGaia_RowWidths[row] + 64, i * GAIA_ROW_HEIGHT - 12,
                gGaia_RowWidths[row], GAIA_ROW_HEIGHT);
        draw[1](canvas, work->sheet + gGaia_RowPictures[row], wallX + 64,
                i * GAIA_ROW_HEIGHT - 12, gGaia_RowWidths[row], 32);
      }
    }

    // The standing wall: its top and its foot (frames 88-159).
    if (frame >= GAIA_WALL_STAND_FRAME && frame <= 159) {
      draw[0](canvas, &work->sheet[0x13c0], wallX + 16, 0, 48, 96);
      draw[1](canvas, &work->sheet[0x13c0], wallX + 64, 0, 48, 96);
      draw[0](canvas, &work->sheet[0x25c0], wallX + 16, 96, 48,
              GAIA_WALL_FOOT_HEIGHT);
      draw[1](canvas, &work->sheet[0x25c0], wallX + 64, 96, 48,
              GAIA_WALL_FOOT_HEIGHT);
    }

    // The dust (from frame 88): what leaves the top goes back to the foot
    // until frame 159.
    if (frame > 87) {
      for (i = 0; i != 64; i++) {
        struct Particle *dust = &DUST[i];

        if (dust->variant == 0) {
          s32 size = (i & 3) + 5;

          draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                  Q_16_16_UPPER_HALF(dust->x) - size / 2,
                  Q_16_16_UPPER_HALF(dust->y) - size, size, size * 2);
          dust->y -= dust->velocityY;
          if (dust->y < 0 && frame <= 159)
            dust->y = px(96);
        } else {
          dust->variant--;
        }
      }
    }

    // The rocks (from frame 5): eleven and bigger from frame 72.
    if (frame > 4) {
      s32 rocks = 6;

      if (frame > 71)
        rocks = ROCK_COUNT;
      for (k = 0; k != rocks; k++) {
        struct Particle *rock = &work->particles[FIRST_ROCK + k];
        s32 slot;

        if (rock->variant == 0) {
          scale = gGaia_UnitScale;
          if (frame > 71) {
            scale.x = (k << 12) + Q16_ONE / 2;
            scale.x += args->variant << 14;
          } else {
            scale.x = Q16_ONE / 2;
          }
          scale.y = scale.x;
          position[3] = 0;
          position[0] = px(rock->x + rockX * 2);
          position[1] = px(512) - px(rock->y);
          position[2] = px(512);
          slot = (frame / 2 + k) % ROCK_COUNT;
          if (slot != -1)
            Render_ApplyProjectedPlacementFar(work->sprites[slot], position,
                                              &scale, 0);
          rock->y -= rock->z;
          rock->velocityX += rock->velocityY;
          if (rock->velocityX > 12)
            rock->velocityX -= 12;
          // Once it leaves the top it starts again from 128; from frame 88
          // it takes a new column and speed, and after 159 it stops.
          if (rock->y < 0) {
            if (frame > 159) {
              rock->variant = -1;
            } else {
              if (frame > 87) {
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

    // The targets (frames 86-158): a flash every 12 frames, a recoil
    // every 4.
    if (frame <= 158) {
      for (i = 0; i != args->targetCount; i++) {
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
    if (frame < 90 || frame > 160)
      BattleCamera_Shake(2, 2);
    else
      BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  for (i = 0; i != ROCK_COUNT; i++)
    Sprite_DestroyFar(work->sprites[i]);
  BattleEffect_EndCanvasLayer();
}

/// Takes the first free animation entry of the 64 and clears it for use. With
/// none free it clears the entry at address 0.
struct SpriteAnimation *AnimationObject_AllocateEmpty(void) {
  struct SpriteAnimation *entry = Ram_HeapSlots->spriteAnimations;
  struct SpriteAnimation *found = NULL;
  s32 i = 0;
  void *none = NULL;

  while (i <= 63) {
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

struct SpriteAnimation;

/// An effect sprite as Gaia searches them: its attributes and OAM word.
struct EffectSprite {
  s32 unk00;
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
  u8 otherFlags : 7;
  u16 rotation;
  u8 width;
  u8 height;
  u8 offsetX;
  u8 offsetY;
  u8 lastNoAndDirty[2];
  u8 flags;
  u8 count;
  s32 unk1c;
  s32 unk20;
  s32 unk24;
  struct SpriteAnimation *entry;
  u8 moreEntries[0x0c];
};

/// Makes an effect sprite with tiles of its own in object VRAM: a free VRAM
/// cache entry and the first free effect sprite of the 64, as
/// EffectSprite_Acquire takes, set up for Sprite_StartAnimationFar. The
/// Lost Age's own.
void *EffectSprite_CreateWithTiles(s32 width, s32 height, s32 attributes,
                                   s32 flags) {
  struct EffectSprite *found = NULL;
  s32 vramEntry = VramCache_FindFreeEntry();
  struct EffectSprite *sprite = Ram_HeapSlots->sprites;
  s32 i = 0;
  struct ObjectSystemWork *objects;
  s32 size;
  s32 tile;
  struct VramCacheEntry *entries;

  while (i <= 63) {
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
    s32 *p = &found->unk1c;

    *p = 0;
  }
  found->unk20 = 0x6000;
  entries = gVramCacheEntries;
  found->unk24 = (((u16 *)entries)[187] >> 5) | 0x800;
  sprite->entry = AnimationObject_AllocateEmpty();
  sprite->width = width;
  sprite->scale = Q16_ONE;
  sprite->height = height;
  sprite->offsetY = 0;
  sprite->offsetX = 0;
  sprite->count = 1;
  sprite->flags = 0;
  Sprite_StartAnimationFar(sprite, 0);

  objects = Ram_HeapSlots->actorSystem;
  size = sprite->width * sprite->height;
  if (!(attributes & OBJ_256_COLORS))
    size = (u32)size >> 1;
  VramCache_Load(sprite->slot, size, 0);
  objects->used += size;
  sprite->oam.bits.tile = entries[sprite->slot].offset >> 5;
  return found;
}
#undef Actor_SetAnimationSpeedFar
#undef PARTICLES
#undef CLIP
#undef BLITTER
#undef SPARKS
#undef PUFF_COUNT
#undef ROCK_COUNT
#undef MOTES
#undef DUST
#undef FIRST_ROCK
