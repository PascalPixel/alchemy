/// @file
/// Gaia, and the effect sprites with tiles of their own.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

#undef ROCK_COUNT

// The dust lives in the map cell buffer once the picture is laid out.
#define DUST ((struct EffectStep *)Ram_MapCellBuffer)
#define ROCK_COUNT 11
// The rocks' motion is particles[10..20], the late rocks' columns [16..21].
#define FIRST_ROCK 10

struct RockSpritePart {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
};

struct RockObject {
  struct RockSpritePart part[2];
  u8 unk18[2];
  u8 flags;
};

extern char ResourceId_EarthWallSheet;
extern char ResourceId_OrangePaletteA;

// ☀️'s gGaia_Bg2Shifts, by side and variant.
extern s8 gGaia_Bg2Shifts[][3];
// The growing wall's nine rows: ☀️'s gGaia_RowWidths and
// gGaia_RowCells.
extern u8 gGaia_RowWidths[];
extern u16 gGaia_RowCells[];
// The mound's three cels: ☀️'s gGaia_MoundWidths, _MoundHeights and
// _MoundSheetOffsets.
extern u8 gGaia_FlameWidths[];
extern u8 gGaia_FlameHeights[];
extern u16 gGaia_MoundCells[];
// ☀️'s gGaia_LateRockColumns.
extern u8 gGaia_LateRockColumns[];

// ☀️'s gRisingWall_UnitScale.
extern struct Scale Data_08196e44;

void Animation_ApplyChildArgumentFar(void *object, s32 value);

void BattleEffect_RunGaia(struct BattleEffectArgument *effect) {
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

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);

  // The larger variants stretch the wall wider: BG2 at 1.25 and 1.5.
  if (effect->variant == 0)
    REG_BG2PA = 204;
  else if (effect->variant == 1)
    REG_BG2PA = 170;
  if (effect->side == 1) {
    wallX = 8;
    if (effect->variant == 0)
      rockX = 40;
    else if (effect->variant == 1)
      rockX = 36;
    else
      rockX = 40;
  } else {
    wallX = -16;
    rockX = -12;
  }
  REG_BG2X = gGaia_Bg2Shifts[effect->side][effect->variant] << 8;

  // The palette: BG colours 1-63 darkened eight steps in each channel.
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, Ram_MapCellBuffer,
                             1, 0);
  Iwram_CopyWordsEntry(
      palette, Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
  color = (u16 *)&BG_PLTT[1];
  for (i = 0; i != 63; i++) {
    s32 c = *color;
    s32 blue = ((u16)c >> 10) & 31;
    s32 green = ((u16)c >> 5) & 31;
    s32 red = 31;

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
    *color++ = (blue << 10) | (green << 5) | red;
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
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);

  // The rocks: eleven animated objects, four of each size.
  for (i = 0; i != ROCK_COUNT; i++) {
    struct RockObject *object = Sprite_CreateFar(653);

    work->objects[i] = object;
    if (object != 0) {
      object->flags = 0;
      Animation_ApplyChildArgumentFar(object, i / 4);
      ((struct RockObject *)work->objects[i])->part[0].priority = 1;
    }
  }
  // Two max blitters, the second mirrored for the right half.
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                 COMPOSITE_CLIP_Y |
                                                 COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  REG_BLDCNT = 0x3f46;
  REG_BLDALPHA = 0x1010;
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // A rock's x is its column, y counts down from 128 by z, velocityX cycles
  // 1 to 12 as its animation step, and variant is its wait before it flies.
  for (i = 0; i != ROCK_COUNT; i++) {
    struct EffectStep *rock = &work->particles[FIRST_ROCK + i];

    rock->x = (Random16() & 15) + 88;
    rock->y = 128;
    rock->z = (Random16() & 15) + 2;
    rock->velocityX = i;
    rock->velocityY = 1;
    rock->velocityZ = px(1) / 2;
    rock->variant = 44 - i * 4;
  }
  for (i = 0; i != 6; i++)
    work->particles[16 + i].x = gGaia_LateRockColumns[i];

  // The dust rises from the foot of the wall after a random wait.
  for (i = 0; i != 256; i++) {
    struct EffectStep *dust = &DUST[i];

    dust->x = ((Random16() & 63) + wallX + 32) << 16;
    dust->y = ((Random16() & 7) + 96) << 16;
    dust->velocityY = ((Random16() & 63) + 32) << 13;
    dust->variant = Random16() & 31;
  }
  REG_BG2CNT = 0x785;
  work->shakeFrames = 250;

  for (frame = 0; frame != 192; frame++) {
    if (frame == 0)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 40)
      Sound_PlayFar(SE_RUMBLE);
    if (frame == 96)
      Sound_PlayFar(SE_CRASH);
    // The battle resumes and resolves the targets, hit cue 134.
    if (frame == 120)
      BattlePlayback_StartFar(134);

    // The ground heaves up (frames 0-81), then pulses between two cels.
    if (frame <= 81) {
      s32 cel = frame / 4;

      if (cel > 2)
        cel = (cel & 1) + 1;
      draw[0](canvas, work->sheet + gGaia_MoundCells[cel],
              wallX - gGaia_FlameWidths[cel] + 64,
              116 - gGaia_FlameHeights[cel], gGaia_FlameWidths[cel],
              gGaia_FlameHeights[cel]);
      draw[1](canvas, work->sheet + gGaia_MoundCells[cel], wallX + 64,
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
        draw[0](canvas, work->sheet + gGaia_RowCells[row],
                wallX - gGaia_RowWidths[row] + 64, i * 32 - 12,
                gGaia_RowWidths[row], 32);
        draw[1](canvas, work->sheet + gGaia_RowCells[row], wallX + 64,
                i * 32 - 12, gGaia_RowWidths[row], 32);
      }
    }

    // It sinks back the same way (frames 160-183).
    if (frame >= 160 && frame <= 183) {
      s32 row = 7 - (frame - 160) / 3;

      if (row < 0)
        row = 0;
      if (row > 7)
        row = 7;
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowCells[row],
                wallX - gGaia_RowWidths[row] + 64, i * 32 - 12,
                gGaia_RowWidths[row], 32);
        draw[1](canvas, work->sheet + gGaia_RowCells[row], wallX + 64,
                i * 32 - 12, gGaia_RowWidths[row], 32);
      }
    }

    // The standing wall: its top and its foot (frames 88-159).
    if (frame >= 88 && frame <= 159) {
      draw[0](canvas, &work->sheet[0x13c0], wallX + 16, 0, 48, 96);
      draw[1](canvas, &work->sheet[0x13c0], wallX + 64, 0, 48, 96);
      draw[0](canvas, &work->sheet[0x25c0], wallX + 16, 96, 48, 21);
      draw[1](canvas, &work->sheet[0x25c0], wallX + 64, 96, 48, 21);
    }

    // The dust (from frame 88): what leaves the top goes back to the foot
    // until frame 159.
    if (frame > 87) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *dust = &DUST[i];

        if (dust->variant == 0) {
          s32 size = (i & 3) + 5;

          draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                  HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);
          dust->y -= dust->velocityY;
          if (dust->y < 0 && frame <= 159)
            dust->y = 0x600000;
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
        struct EffectStep *rock = &work->particles[FIRST_ROCK + k];
        s32 slot;

        if (rock->variant == 0) {
          scale = Data_08196e44;
          if (frame > 71) {
            scale.x = (k << 12) + Q16_ONE / 2;
            scale.x += effect->variant << 14;
          } else {
            scale.x = Q16_ONE / 2;
          }
          scale.y = scale.x;
          position[3] = 0;
          position[0] = (rock->x + rockX * 2) << 16;
          position[1] = 0x2000000 - (rock->y << 16);
          position[2] = 0x2000000;
          slot = (frame / 2 + k) % ROCK_COUNT;
          if (slot != -1)
            Render_ApplyProjectedPlacementFar(work->objects[slot], position,
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
                if (effect->variant == 0)
                  rock->x = Random16() % 96 + 42;
                else if (effect->variant == 1)
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
      for (i = 0; i != effect->count; i++) {
        if (frame > 85) {
          if (frame % 12 == 0)
            BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 6);
          if ((frame & 3) == 0)
            BattleMotion_ApplyKnockbackFar(effect->actors[i], 5);
        }
      }
    }
    // While the wall stands the screen shakes hard, otherwise softly.
    if (frame < 90 || frame > 160)
      BattleCamera_Shake(2, 2);
    else
      BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  for (i = 0; i != ROCK_COUNT; i++)
    Sprite_DestroyFar(work->objects[i]);
  BattleEffect_EndCanvasLayer();
}

/// Takes the first free animation entry of the 64 and clears it for use. With
/// none free it clears the entry at address 0.
struct AnimationEntry *AnimationObject_AllocateEmpty(void) {
  struct AnimationEntry *entry = Ram_HeapSlots->unk0c[0];
  struct AnimationEntry *found = NULL;
  s32 i = 0;
  // The script's NULL held from the start lives longest, so the word zero
  // takes r3 and the byte zero r2; as a plain NULL, 24 of 30 instructions
  // differ in tla-en.
  void *none = NULL;

  while (i <= 63) {
    if (entry->kind == 0) {
      found = entry;
      break;
    }
    i++;
    entry++;
  }
  found->animId = 0;
  found->scripts = NULL;
  found->pictures = NULL;
  found->script = none;
  found->mode = 0;
  found->picture = 0;
  found->pos = 0;
  found->kind = 1;
  found->param = 0;
  return found;
}

struct AnimationEntry;

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
  s32 unk0c;
  u8 vramEntry;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
  u16 unk12;
  u8 width;
  u8 height;
  u8 unk16;
  u8 unk17;
  u8 unk18[2];
  u8 unk1a;
  u8 unk1b;
  s32 unk1c;
  s32 unk20;
  s32 unk24;
  struct AnimationEntry *entry;
  u8 unk2c[0x0c];
};

/// Makes an effect sprite with tiles of its own in object VRAM: a free VRAM
/// cache entry and the first free effect sprite of the 64, as
/// EffectSprite_Acquire takes, set up for Animation_ApplyChildArgumentFar. The
/// Lost Age's own.
struct EffectSprite *EffectSprite_CreateWithTiles(s32 width, s32 height,
                                                  s32 attributes, s32 flags) {
  struct EffectSprite *found = NULL;
  s32 vramEntry = VramCache_FindFreeEntry();
  struct EffectSprite *sprite = Ram_HeapSlots->unk0c[1];
  s32 i = 0;
  struct ObjectSystemWork *objects;
  s32 size;
  s32 tile;
  struct VramBlockCacheEntry *entries;

  while (i <= 63) {
    if (sprite->width == 0) {
      found = sprite;
      break;
    }
    i++;
    sprite++;
  }
  tile = VramCache_Load(vramEntry, 0, 0);
  found->vramEntry = vramEntry;
  found->unk12 = 0;
  found->unk1a = 0;
  found->unk11_0 = 0;
  {
    u32 *words = (u32 *)found;

    *words++ = 0;
    *words++ = attributes;
    *words = flags | tile | 0x800;
  }
  {
    s32 *p = &found->unk1c;

    *p = 0;
  }
  found->unk20 = 0x6000;
  entries = gResourceTableEntries;
  found->unk24 = (((u16 *)entries)[187] >> 5) | 0x800;
  sprite->entry = AnimationObject_AllocateEmpty();
  sprite->width = width;
  sprite->unk0c = 0x10000;
  sprite->height = height;
  sprite->unk17 = 0;
  sprite->unk16 = 0;
  sprite->unk1b = 1;
  sprite->unk1a = 0;
  Animation_ApplyChildArgumentFar(sprite, 0);

  objects = Ram_HeapSlots->objectSystem;
  size = sprite->width * sprite->height;
  if (!(attributes & 0x2000))
    size = (u32)size >> 1;
  VramCache_Load(sprite->vramEntry, size, 0);
  objects->used += size;
  sprite->oam.bits.tile = entries[sprite->vramEntry].offset >> 5;
  return found;
}
#undef Actor_SetAnimationSpeedFar
#undef PARTICLES
#undef HI
#undef BLITTER_ADDITIVE
#undef BLITTER_MAX
#undef CLIP
#undef PHASE_RESULTS
#undef HEAP_BLITTER
#undef HEAP_BLITTER_ALTERNATE
#undef BLITTER
#undef SPARKS
#undef PUFF_COUNT
#undef ROCK_COUNT
#undef BLIT_MODE
#undef BLIT_CLIP
#undef MOTES
#undef DUST
#undef FIRST_ROCK
