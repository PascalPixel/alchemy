/// @file
/// Charon, and the quad strips it draws.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Charon's particles in the effect work: the drops, the crew's places and
/// their homes, then the columns; the last particle's variant holds the
/// shockwave's size.
#define DROPS (work->particles)
#define MEMBERS (work->particles + 16)
#define HOMES (work->particles + 32)
#define COLUMNS (work->particles + 48)
#define WAVE_SIZE (work->particles[63].variant)
/// The crew's sprites, Charon's first.
#define OBJECTS ((struct Sprite **)work->sprites)
/// Charon's objects: the crimson beast (CrimsonBeastTiles) as a 4 x 3 grid of
/// 32x32 sprites, the white wisp and the 64x64 picture the reveal is drawn
/// into.
#define CHARON_BEAST_SPRITES 12
#define CHARON_BEAST_COLUMNS 4
#define CHARON_WISP 12
#define CHARON_REVEAL 13
#define CHARON_OBJECTS 14
/// The white arrows (WhiteArrowsTiles): sixteen pictures of an 8x8 arrow, and
/// the 48 sprites that swarm with them.
#define CHARON_ARROW_FRAMES 16
#define CHARON_ARROWS 48

/// A list of units to spawn, as Charon passes an empty one.
#define SPRITES ((struct Sprite **)Ram_MapCellBuffer)
#define SPRITE_BUFFER gMapGridCellBuffer
#define OAM_BUFFER ((u32 *)(gMapGridCellBuffer + 0x4000))
#define REVEAL_BUFFER (Ram_MapCellBuffer + 0x4000)
#define CHARON_PARTICLES ((struct Particle *)(Ram_MapCellBuffer + 0x4200))
#define OBJ_VRAM ((u8 *)OBJ_VRAM0)
/// The battle background's tiles from column 4 of row 5, where the rune seal
/// is stamped.
#define CHARON_SEAL_TILES (BG_CHAR_BLOCK(2) + (5 * 32 + 4) * 64)

extern const struct Scale gCharon_SwarmScale;
extern const struct Scale gCharon_PulseScale;
extern const struct Scale gCharon_RevealScale;
extern const struct Scale gCharon_FinaleScale;

/// Where ParticleSpritesD is decoded in the effect's pictures.
#define CHARON_PARTICLE_SPRITES_D_PICTURES 0x2400
/// RuneSealSheet, decoded over ParticleSpritesD at frame 56, and its size.
#define CHARON_RUNE_SEAL 0x2400
#define RUNE_SEAL_WIDTH 184
#define RUNE_SEAL_HEIGHT 80
/// Where each act decodes its own sheet in the effect's pictures: SwirlTexture
/// with CharmRingSheet, CurseSealSheet, FireRingTexture or shaded circles.
#define CHARON_ACT_PICTURES 0x1000
/// The finale's brightest block, which the four columns of light are drawn
/// from, and GlowRingTexture made all bright.
#define CHARON_BRIGHT_BLOCK 0x2000
#define CHARON_GLOW_RING 0x3000

#define CHARON_FRAMES                                                          \
  394 ///< frames of the whole summon, unless A or B skips it

/// Charon: over a red wasteland a violet whirlpool turns about a red dome that
/// rises out of it; in a white flash among dark shards Charon appears and
/// slashes across the vortex, and a black shadow sweeps from the left over the
/// foes.
///
/// Abilities: Charon.
void BattleEffect_RunCharon(struct BattleEffectParams *arg) {
  u16 tiles[CHARON_ARROW_FRAMES];
  s16 emptyList[UNIT_MAX];
  FIXED swarmPlace[4];
  FIXED pulsePlace[4];
  FIXED objectPlace[4];
  FIXED revealPlace[4];
  FIXED finalePlace[4];
  DrawRectangleFn routine[2];
  struct ModelTexture lateTexture;
  struct ModelTexture revealTexture;
  struct ModelTexture risingTexture;
  struct ModelTexture openingTexture;
  u8 *canvas;
  struct BattleEffectWork *work;
  struct BattleBackgroundWork *backgroundView;
  s32 j;
  s32 frame;
  u32 savedWidth;
  u8 *glyphs;
  struct BattleWork *battle;
  DrawRectangleFn *blit;
  FIXED posX;
  FIXED posZ;
  s32 t56;
  s32 red;
  s32 green;
  s32 blue;
  struct BattleCamera *camera;
  s32 shake;
  s16 savedYaw;
  s32 i;
  s32 fade;
  u16 *palette;
  u8 *source;
  s32 t;
  struct Sprite **sprites;
  struct VramCacheEntry *slots;
  struct VramCacheEntry *objectSlots;
  s32 cell;
  s32 phase;
  s32 halfCell;
  s32 twenty;
  s32 width;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  backgroundView = Ram_HeapSlots->battleBackground;
  savedWidth = gBgScroll[1].x;
  glyphs = Ram_HeapSlots->battleSheet;
  sprites = (struct Sprite **)gMapGridCellBuffer;
  battle = Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  {
    s32 order = TASK_PASS_FRAME;

    Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, order);
  }
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  backgroundView->busy = 1;
  gBgScroll[1].x = 0;
  emptyList[0] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(emptyList, 0);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  wait(1);
  Palette_LoadBasicSpriteColors();
  battle->keepBackdropPalette = 1;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_AERIAL_VIEW_BACKDROP, 0);
  Palette_BrightenSavedBattleBg(0, -20, -20);
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  blit = routine;
  BattleEffect_FetchRectangleBlitters(0, blit);

  source = (u8 *)Resource_GetTableEntry(RESOURCE_RED_SLASH_TILES);
  Iwram_CopyWords((u16 *)&OBJ_PLTT_COLOR(2, 0), source, 0x140);
  source += 0x140;
  Decompress_TaggedLz(source, work->sheet);
  for (i = 0; i != 0x1000; i++) {
    if (work->sheet[i] != 0) {
      work->sheet[i] += 32;
    }
  }
  {
    struct Sprite *object = EffectSprite_CreateWithTiles(
        64, 64, OBJ_SIZE_64X64 | OBJ_256_COLORS, 0);

    OBJECTS[0] = object;
    object->image.priority = 3;
    slots = gVramCacheEntries;
    Iwram_CopyWords(OBJ_VRAM + slots[object->slot].offset, work->sheet, 0x1000);
  }
  OBJECTS[0]->image.objectMode = 0;
  OBJECTS[0]->offsetY = 32;

  source = (u8 *)Resource_GetTableEntry(RESOURCE_WHITE_ARROWS_TILES);
  Iwram_CopyWords((u16 *)&OBJ_PLTT_COLOR(15, 0), source, PALETTE_BANK_BYTES);
  source += PALETTE_BANK_BYTES;
  Decompress_TaggedLz(source, work->sheet);
  for (i = 0; i != CHARON_ARROW_FRAMES; i++) {
    struct Sprite *object =
        EffectSprite_CreateWithTiles(16, 16, 0, OBJ_PALETTE(15));
    SPRITES[i] = object;
    object->image.priority = 1;
    Iwram_CopyWords(OBJ_VRAM + slots[object->slot].offset,
                    work->sheet + i * TILE_SIZE_4BPP, TILE_SIZE_4BPP);
    tiles[i] = SPRITES[i]->image.tile;
  }
  for (i = 0; i != CHARON_ARROWS - CHARON_ARROW_FRAMES; i++) {
    struct Sprite *object = EffectSprite_Acquire(8, 8, 0, OBJ_PALETTE(15));
    SPRITES[CHARON_ARROW_FRAMES + i] = object;
    Iwram_CopyWords(object, sprites[0], 24);
  }
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, glyphs, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, work->sheet, 0, 0);
  for (j = 0; j != 10; j++) {
    cell = 4;
    for (i = 0; i != cell * cell * 2; i++) {
      s32 value = work->sheet[gBattleEffect_MotePictures[cell - 1] + i];

      if (value != 0) {
        value -= j * 7;
        if (value <= 0) {
          value = 1;
        }
      }
      glyphs[cell * cell * j * 2 + i] = value;
    }
  }
  Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE, work->sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_CHARM_RING_SHEET,
                             work->sheet + CHARON_ACT_PICTURES, 1, 1);
  source = (u8 *)Resource_GetTableEntry(RESOURCE_BOLT_SHEET);
  Iwram_CopyWords(BG_PLTT, source, SHEET_PALETTE_BYTES);
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  REG_BG2X = 0x800;
  posX = 0;
  posZ = 0;

  for (frame = 0; frame != CHARON_FRAMES && !(gInput.repeat & (KEY_A | KEY_B));
       frame++) {
    u8 *order = REVEAL_BUFFER + (frame - 244) * 2;

    t = frame - 32;
    if (t >= 0 && t < 22) {
      Palette_BrightenSavedBattleBg(-t, -20, frame - 52);
    }
    if (frame == 55) {
      Palette_BrightenSavedBattleBg(0, -10, -20);
    }
    if (frame == 4) {
      Sound_PlayFar(SE_TREMOR);
    }
    if (frame == 55) {
      Sound_PlayFar(SE_BANE_ESCAPE);
    }
    if (frame == 78) {
      Sound_PlayFar(SE_GUSH);
    }
    if (frame == 127) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 131) {
      Sound_PlayFar(SE_MONSTER_ROAR);
    }
    if (frame == 206) {
      Sound_PlayFar(SE_SWIRL);
    }
    if (frame == 244) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 302) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 308) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 312) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 350) {
      Sound_PlayFar(SE_LAUNCH);
    }
    if (frame == 0) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      {
        u32 hidden;
        /* FAKEMATCH(temporary): the mask held in a halfword struct set in the
         * for clause. */
        struct {
          u16 value;
        } mask;
        s32 *timer;
        u32 *b;
        u32 *a;

        for (i = 0, hidden = (240 << 16) + 160, mask.value = OBJ_PALETTE(15),
            timer = &CHARON_PARTICLES[0].variant,
            b = (u32 *)(REVEAL_BUFFER + 4), a = OAM_BUFFER;
             i != 64; i++) {
          s32 t;

          *a = hidden;
          t = *timer;
          timer += 7;
          *b = tiles[t / CHARON_ARROW_FRAMES] | mask.value;
          b += 2;
          a += 2;
        }
      }
    }
    if (frame >= 132 && frame < 240) {
      struct Scale swarmScale = gCharon_SwarmScale;

      swarmScale.y = FIXED_ONE * 5 / 4;
      swarmScale.x = FIXED_ONE * 5 / 4;
      if (frame == 132) {
        for (i = 0; i != 128; i++) {
          CHARON_PARTICLES[i].x = INTtoFIXED(Random16() % 160 + 40);
          CHARON_PARTICLES[i].y = INTtoFIXED((Random16() & 63) + 32);
          CHARON_PARTICLES[i].variant = Random16() & 255;
          CHARON_PARTICLES[i].velocityX = ((Random16() & 255) - 127) << 9;
          CHARON_PARTICLES[i].velocityY = (-(Random16() & 255) - 128) << 8;
        }
      }
      for (i = 0; i != CHARON_ARROWS; i++) {
        if (i == 10) {
          swarmScale.y = FIXED_ONE;
          swarmScale.x = FIXED_ONE;
        }
        swarmPlace[3] = 0;
        swarmPlace[1] = toFIXED(255);
        swarmPlace[0] = CHARON_PARTICLES[64 + i].x;
        swarmPlace[2] = CHARON_PARTICLES[64 + i].y + toFIXED(32);
        Render_ApplyProjectedPlacementFar(SPRITES[i], swarmPlace, &swarmScale.x,
                                          0);
        SPRITES[i]->image.tile =
            tiles[CHARON_PARTICLES[64 + i].variant / CHARON_ARROW_FRAMES];
      }
      for (i = 0; i != 128; i++) {
        if (CHARON_PARTICLES[i].y > -toFIXED(16) &&
            CHARON_PARTICLES[i].x > -toFIXED(16) &&
            CHARON_PARTICLES[i].x < toFIXED(256)) {
          CHARON_PARTICLES[i].x += CHARON_PARTICLES[i].velocityX;
          CHARON_PARTICLES[i].y += CHARON_PARTICLES[i].velocityY;
        }
        CHARON_PARTICLES[i].variant += i / 16 + 2;
        if (CHARON_PARTICLES[i].variant > 255) {
          CHARON_PARTICLES[i].variant -= 256;
        }
      }
    }
    t56 = frame - 56;
    if (t56 >= 0 && t56 < 76) {
      struct Scale pulseScale = gCharon_PulseScale;

      pulseScale.x = pulseScale.y = Trig_Sin(t56 << 7) * 2 + DEGtoANG(90);
      pulsePlace[3] = 0;
      pulsePlace[1] = toFIXED(255);
      pulsePlace[0] = toFIXED(114);
      pulsePlace[2] = INTtoFIXED(t56 >= 0 ? 78 : 100);
      Render_ApplyProjectedPlacementFar(OBJECTS[0], pulsePlace, &pulseScale.x,
                                        0);
    }
    if (frame == 131) {
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_AERIAL_VIEW_BACKDROP, 0);
      Iwram_FillWords((u16 *)&BG_PLTT_COLOR(6, 0), 0x100,
                      COLOR_PAIR(RGB_WHITE));
      Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame >= 132 && frame < 147) {
      Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                      (0x3f - (frame - 132) * 4) * 0x01010101);
    }
    if ((frame & 3) == 0 && (frame >= 132 && frame < 240)) {
      u16 *target = Resource_GetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP);
      u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);

      for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, target++, color++) {
        s32 b = RGB_BLUE(*color);
        s32 g = RGB_GREEN(*color);
        s32 r = RGB_RED(*color);
        s32 tb;
        s32 tg;
        s32 tr;

        if (frame < 168) {
          tb = RGB_BLUE(*target);
          tg = RGB_GREEN(*target);
          tr = RGB_RED(*target);
        } else {
          tb = RGB_BLUE(*target) - 16;
          tg = RGB_GREEN(*target) - 16;
          tr = RGB_RED(*target) + 8;
        }
        if (tb < 0) {
          tb = 0;
        }
        if (tg < 0) {
          tg = 0;
        }
        if (tr > RGB_LEVEL_MAX) {
          tr = RGB_LEVEL_MAX;
        }
        if (b > tb) {
          b--;
        }
        if (g > tg) {
          g--;
        }
        if (r > tr) {
          r--;
        }
        *color = RGB(r, g, b);
      }
      for (i = 0; i != 15; i++) {
        s32 r = 31 - (frame - 132) / 4;
        s32 g = r;
        s32 b = r;

        if (r > RGB_LEVEL_MAX) {
          r = RGB_LEVEL_MAX;
        }
        if (r < 12) {
          r = 12;
        }
        if (g > RGB_LEVEL_MAX) {
          g = RGB_LEVEL_MAX;
        }
        if (g < 4) {
          g = 4;
        }
        if (b > RGB_LEVEL_MAX) {
          b = RGB_LEVEL_MAX;
        }
        if (b < 4) {
          b = 4;
        }
        OBJ_PLTT_COLOR(14, 1 + i) = RGB(r, g, b);
      }
    }
    if (frame >= 132 && frame < 240) {
      u16 *target = Resource_GetTableEntry(RESOURCE_CRIMSON_BEAST_TILES);
      u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 0);

      for (i = 0; i != 192; i++, target++, color++) {
        s32 b = RGB_BLUE(*color);
        s32 g = RGB_GREEN(*color);
        s32 r = RGB_RED(*color);
        s32 tb = RGB_BLUE(*target);
        s32 tg = RGB_GREEN(*target);
        s32 tr = RGB_RED(*target);

        if (b > tb) {
          b--;
        }
        if (g > tg) {
          g--;
        }
        if (r > tr) {
          r--;
        }
        *color = RGB(r, g, b);
      }
    }
    fade = 0;
    if (frame >= 214 && frame < 246) {
      s32 t = (frame - 214) / 4;

      red = t / 2;
      green = 0;
      blue = t;
      fade = 1;
    }
    if (frame >= 251 && frame < 283) {
      s32 t = (frame - 251) / 2;

      red = 4 - t / 4;
      green = -(t / 2);
      blue = 8 - t / 2;
      fade = 1;
    }
    if (frame >= 348 && frame < 412) {
      red = (frame - 348) / 4;
      green = red / 2 - 8;
      blue = red / 8 - 4;
      fade = 1;
    }
    if (fade == 1) {
      u16 *color = Resource_GetTableEntry(RESOURCE_CRIMSON_BEAST_TILES);

      for (i = 0; i != 192; i++, color++) {
        s32 r = RGB_RED(*color) + red;
        s32 g = RGB_GREEN(*color) + green;
        s32 b = RGB_BLUE(*color) + blue;

        if (r > RGB_LEVEL_MAX) {
          r = RGB_LEVEL_MAX;
        }
        if (g > RGB_LEVEL_MAX) {
          g = RGB_LEVEL_MAX;
        }
        if (b > RGB_LEVEL_MAX) {
          b = RGB_LEVEL_MAX;
        }
        if (r < 0) {
          r = 0;
        }
        if (g < 0) {
          g = 0;
        }
        if (b < 0) {
          b = 0;
        }
        OBJ_PLTT_COLOR(0, i) = RGB(r, g, b);
      }
    }
    if (frame >= 132 && frame < 860) {
      if (frame == 132) {
        Sprite_DestroyFar((struct Sprite *)OBJECTS[0]);
        source = (u8 *)Resource_GetTableEntry(RESOURCE_CRIMSON_BEAST_TILES);
        Iwram_FillWords((u16 *)&OBJ_PLTT_COLOR(0, 0), 0x180,
                        COLOR_PAIR(RGB_WHITE));
        source += 0x180;
        Decompress_TaggedLz(source,
                            work->sheet + CHARON_PARTICLE_SPRITES_D_PICTURES);
        for (i = 0, objectSlots = gVramCacheEntries; i != CHARON_BEAST_SPRITES;
             i++) {
          struct Sprite *object = EffectSprite_CreateWithTiles(
              32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);

          object->image.priority = 3;
          OBJECTS[i] = object;
          Iwram_CopyWords(OBJ_VRAM + objectSlots[object->slot].offset,
                          work->sheet + CHARON_PARTICLE_SPRITES_D_PICTURES +
                              i * OBJ_32X32_8BPP_BYTES,
                          OBJ_32X32_8BPP_BYTES);
        }
        source = (u8 *)Resource_GetTableEntry(RESOURCE_WHITE_WISPS_TILES);
        Iwram_CopyWords((u16 *)&OBJ_PLTT_COLOR(14, 0), source,
                        PALETTE_BANK_BYTES);
        Iwram_FillWords((u16 *)&OBJ_PLTT_COLOR(14, 0), 32,
                        COLOR_PAIR(RGB_WHITE));
        source += 32;
        Decompress_TaggedLz(source,
                            work->sheet + CHARON_PARTICLE_SPRITES_D_PICTURES);
        {
          struct Sprite *object = EffectSprite_CreateWithTiles(
              64, 64, OBJ_SIZE_64X64, OBJ_PALETTE(14));
          struct VramCacheEntry *tileSlots;

          OBJECTS[CHARON_WISP] = object;
          object->image.priority = 3;
          tileSlots = gVramCacheEntries;
          {
            u8 *vram = OBJ_VRAM;

            // vram holds the base, then the destination.
            vram += tileSlots[object->slot].offset;
            Iwram_CopyWords(
                vram, work->sheet + CHARON_PARTICLE_SPRITES_D_PICTURES, 0x800);
          }
          object = EffectSprite_CreateWithTiles(
              64, 64, OBJ_SIZE_64X64 | OBJ_256_COLORS, 0);
          OBJECTS[CHARON_REVEAL] = object;
          object->image.priority = 2;
          object->offsetY = 32;
          Iwram_FillWords(OBJ_VRAM + tileSlots[object->slot].offset, 0x1000, 0);
        }
        source = (u8 *)Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_E);
        Iwram_CopyWords(BG_PLTT, source, SHEET_PALETTE_BYTES);
        Resource_LoadAndDecompress(
            RESOURCE_PARTICLE_SPRITES_D,
            work->sheet + CHARON_PARTICLE_SPRITES_D_PICTURES, 0, 0);
        Iwram_CopyWords(
            glyphs, work->sheet + CHARON_PARTICLE_SPRITES_D_PICTURES, 0x302);
        Resource_LoadAndDecompress(RESOURCE_CURSE_SEAL_SHEET,
                                   work->sheet + CHARON_ACT_PICTURES, 1, 1);
        posX = toFIXED(72);
        posZ = toFIXED(56);
      }
      if (frame == 243) {
        source = (u8 *)Resource_GetTableEntry(RESOURCE_RED_CRESCENT_TILES);
        Iwram_FillWords((u16 *)&OBJ_PLTT_COLOR(12, 0), 64, 0);
        source += 64;
        Decompress_TaggedLz(source, gMapGridCellBuffer + 0x4000);
        for (i = 0; i != 0x1000; i++) {
          if ((gMapGridCellBuffer + 0x4000)[i] != 0) {
            (gMapGridCellBuffer + 0x4000)[i] += 192;
          }
        }
        for (i = 0; i != 32; i++) {
          REVEAL_BUFFER[0x1000 + i] = i;
        }
        {
          u8 *slot;
          s32 half = 0x1000;

          for (i = 0, slot = REVEAL_BUFFER + 0x1000; i != 32;) {
            u8 *other;
            s32 swap;
            s32 value;
            s32 mask;

            mask = 31;
            half = 0x1000;
            other = (Random16() & mask) + (gMapGridCellBuffer + 0x4000);
            other += half;
            swap = *slot;
            value = *other;
            // These casts change nothing; they keep the loop long enough that
            // the buffer's address is rebuilt in it.
            swap = (u8)swap;
            value = (u8)value;
            i++;
            *slot = value;
            *other = swap;
            slot++;
          }
        }
      }
      {
        FIXED objectScale[2];

        objectPlace[3] = 0;
        objectPlace[1] = toFIXED(255);
        if (frame < 160) {
          posX += toFIXED(1) / 8;
          posZ -= toFIXED(1) / 4;
        } else if (frame < 332) {
          posX += toFIXED(1) / 8;
          posZ += toFIXED(1) / 16;
        } else {
          posX += toFIXED(1) / 8;
        }
        objectScale[1] = FIXED_ONE * 5 / 4;
        objectScale[0] = FIXED_ONE * 5 / 4;
        objectScale[0] = objectScale[1] = posZ / 48 + FIXED_ONE / 4;
        objectPlace[0] = posX + toFIXED(64);
        objectPlace[2] = toFIXED(144);
        Render_ApplyProjectedPlacementFar(OBJECTS[CHARON_WISP], objectPlace,
                                          objectScale, 0);
        objectScale[1] = FIXED_ONE;
        objectScale[0] = FIXED_ONE;
        for (i = 0; i != CHARON_BEAST_SPRITES; i++) {
          objectPlace[0] = INTtoFIXED((i % CHARON_BEAST_COLUMNS) * 32) + posX;
          objectPlace[2] = INTtoFIXED((i / CHARON_BEAST_COLUMNS) * 32) + posZ;
          Render_ApplyProjectedPlacementFar(OBJECTS[i], objectPlace,
                                            objectScale, 0);
        }
      }
      if (frame == 200) {
        source = (u8 *)Resource_GetTableEntry(RESOURCE_BOLT_SHEET);
        Iwram_CopyWords(BG_PLTT, source, SHEET_PALETTE_BYTES);
      }
      if (frame >= 200 && frame < 302) {
        void *texture = Heap_AllocEwram(32);
        struct RenderModel *model = Render_AllocateModelList(1);
        s32 t = frame - 200;
        s32 depth;
        s32 scale;

        depth = t * 4 - 64;
        scale = 100000 - t * 1524;
        if (frame > 253) {
          depth = (254 - frame) * 8;
        }
        if (scale < 13800) {
          scale = 13800;
        }
        if (depth > 0) {
          depth = 0;
        }
        model->shade = depth;
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                            CANVAS_FADE_THREE_QUARTERS);
        lateTexture.logWidth = 7;
        lateTexture.logHeight = 7;
        lateTexture.pixels = work->sheet + CHARON_ACT_PICTURES;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->texture = &lateTexture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = texture;
        if (frame <= 282) {
          Matrix_Initialize();
          Matrix_Scale(scale * 2, scale * 2, scale * 2);
          Matrix_Translate(-toFIXED(16), -toFIXED(8), 0);
          Matrix_RotateX(TRIG_TURN * 7 / 8);
          Matrix_RotateZ(DEGtoANG(90) - t * 256);
          Matrix_RotateY(-t << 10);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture,
                                   4);
          Render_DrawModelList(model);
        }
        if (frame > 231) {
          t = frame - 232;
          depth = t * 2 - 64;
          scale = 100000 - t * 1200;
          if (depth < -64) {
            depth = -64;
          }
          if (depth > 0) {
            depth = 0;
          }
          lateTexture.logWidth = 6;
          lateTexture.logHeight = 6;
          model->shade = depth;
          lateTexture.pixels = work->sheet;
          model->texture = &lateTexture;
          model->mesh = gBattleEffect_SquareMesh;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->vertices = texture;
          Matrix_Initialize();
          Matrix_Translate(-toFIXED(8), -toFIXED(8), 0);
          Matrix_Scale(scale, scale, scale);
          Matrix_RotateX(TRIG_TURN * 3 / 4);
          Matrix_RotateY(-t << 10);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture,
                                   4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(texture);
      }
      if (frame >= 244 && frame < 310) {
        struct Scale revealScale = gCharon_RevealScale;

        if (frame < 260) {
          u8 *low = REVEAL_BUFFER;
          u8 *high = REVEAL_BUFFER + 1;
          /* FAKEMATCH(temporary): orderCopy, a copy of order set again in the
           * loop. */
          u8 *orderCopy = order;

          for (i = 0; i != 64; i++) {
            s32 column = (orderCopy = order)[0x1000] * 2;
            s32 row = i * 64;
            s32 pixel = row + column;
            u8 *lowByte = low + pixel;
            u8 *highByte = high + pixel;
            s32 texel = *lowByte | (*highByte << 8);

            *(u16 *)(OBJ_VRAM +
                     (row +
                      gVramCacheEntries[OBJECTS[CHARON_REVEAL]->slot].offset) +
                     column) = texel;
            column = orderCopy[0x1001] * 2;
            pixel = row + column;
            lowByte = low + pixel;
            highByte = high + pixel;
            texel = *lowByte | (*highByte << 8);
            *(u16 *)(OBJ_VRAM +
                     (row +
                      gVramCacheEntries[OBJECTS[CHARON_REVEAL]->slot].offset) +
                     column) = texel;
          }
        }
        revealScale.y = revealScale.x = (155 << 12) - (frame << 11);
        revealPlace[3] = 0;
        revealPlace[1] = toFIXED(255);
        revealPlace[0] = toFIXED(92);
        revealPlace[2] = toFIXED(72);
        Render_ApplyProjectedPlacementFar(OBJECTS[CHARON_REVEAL], revealPlace,
                                          &revealScale.x, 0);
        {
          u16 *target = Resource_GetTableEntry(RESOURCE_RED_CRESCENT_TILES);
          u16 *color = (u16 *)&OBJ_PLTT_COLOR(12, 0);

          for (i = 0; i != 32; i++, target++, color++) {
            s32 b = RGB_BLUE(*color);
            s32 g = RGB_GREEN(*color);
            s32 r = RGB_RED(*color);
            s32 tb = RGB_BLUE(*target);
            s32 tg = RGB_GREEN(*target);
            s32 tr = RGB_RED(*target);

            if (b < tb) {
              b++;
            }
            if (g < tg) {
              g++;
            }
            if (r < tr) {
              r++;
            }
            *color = RGB(r, g, b);
          }
        }
      }
      if (frame > 301) {
        if (frame == 302) {
          Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          Iwram_FillWords((u16 *)&OBJ_PLTT_COLOR(12, 0), 64,
                          COLOR_PAIR(RGB_WHITE));
          Iwram_CopyWords((u16 *)&BG_PLTT_COLOR(6, 0),
                          Resource_GetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP),
                          0x100);
          source = (u8 *)Resource_GetTableEntry(RESOURCE_BOLT_SHEET);
          Iwram_CopyWords(BG_PLTT, source, SHEET_PALETTE_BYTES);
          Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
                                     work->sheet + CHARON_ACT_PICTURES, 0, 0);
          Graphics_LoadGreyRamp();
        }
        if (frame > 303) {
          u16 *target = Resource_GetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP);
          u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);

          for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, target++, color++) {
            s32 b = RGB_BLUE(*color);
            s32 g = RGB_GREEN(*color);
            s32 r = RGB_RED(*color);
            s32 tb = RGB_BLUE(*target) - 20;
            s32 tg = RGB_GREEN(*target) - 20;
            s32 tr = RGB_RED(*target) + 8;

            if (tb < 0) {
              tb = 0;
            }
            if (tg < 0) {
              tg = 0;
            }
            if (tr > RGB_LEVEL_MAX) {
              tr = RGB_LEVEL_MAX;
            }
            if (b > tb) {
              b--;
            }
            if (g > tg) {
              g--;
            }
            if (r > tr) {
              r--;
            }
            *color = RGB(r, g, b);
          }
        }
        if (frame > 301) {
          s32 k;
          s32 x;
          s32 stripShade;
          s32 size;
          void *texture = Heap_AllocEwram(32);
          struct RenderModel *model = Render_AllocateModelList(1);

          revealTexture.logWidth = 7;
          revealTexture.logHeight = 7;
          revealTexture.pixels = work->sheet + CHARON_ACT_PICTURES;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->texture = &revealTexture;
          model->mesh = gBattleEffect_LargeSquareMesh;
          model->vertices = texture;
          for (k = 0; k != 3; k++) {
            x = frame - 302 - k * 8;
            if (x >= 0) {
              stripShade = x * 4 - 16;
              size = 140000 - x * 6192;
              if (size < 0) {
                size = 0;
              }
              if (stripShade > 0) {
                stripShade = 0;
              }
              model->shade = stripShade;
              Matrix_Initialize();
              Matrix_Translate(-toFIXED(8), -toFIXED(8), 0);
              Matrix_Scale(size / 2, size, size / 2);
              if (k == 0) {
                Matrix_RotateZ(TRIG_TURN * 15 / 16);
              } else if (k == 1) {
                Matrix_RotateZ(DEGtoANG(45));
              } else {
                Matrix_RotateZ(TRIG_TURN * 7 / 8);
              }
              Matrix_RotateX(TRIG_TURN / 32);
              Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                       texture, 4);
              Render_DrawModelList(model);
            }
          }
          Heap_Release(model);
          Heap_Release(texture);
        }
      }
      if (frame == 330) {
        Iwram_FillWords((u16 *)&OBJ_PLTT_COLOR(12, 0), 64,
                        COLOR_PAIR(RGB(RGB_LEVEL_MAX, 0, RGB_LEVEL_MAX)));
      }
      if (frame >= 330 && frame < 351) {
        struct Scale finaleScale = gCharon_FinaleScale;
        s32 dx = 0;
        s32 dz = 0;

        if (frame > 346) {
          dx = 347 - frame;
          dz = (frame - 347) / 4;
        }
        finaleScale.x = finaleScale.y = (frame << 11) - 0xa5000;
        finalePlace[3] = 0;
        finalePlace[1] = toFIXED(255);
        finalePlace[0] = INTtoFIXED(dx) + toFIXED(92);
        finalePlace[2] = INTtoFIXED(dz) + toFIXED(72);
        Render_ApplyProjectedPlacementFar(OBJECTS[CHARON_REVEAL], finalePlace,
                                          &finaleScale.x, 0);
      }
      if (frame == 347) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        REG_BLDCNT = 0;
        {
          u16 *entry = (u16 *)&BG_PLTT_COLOR(0, 1);
          for (i = 0; i != 63; i++, entry++) {
            s32 r = 31 - i;
            s32 b = r;

            if (r < 0) {
              r = 0;
            }
            if (b < 0) {
              b = 0;
            }
            *entry = (b << 10) | r;
          }
        }
      }
      if (frame > 346) {
        s32 t = frame * 4 - 1380;
        s32 height = 54;

        if (frame > 349) {
          height = (FIXEDtoINT(-(Trig_Sin((frame << 10) - 0x57800) * 54))) + 54;
        }
        if (frame == 360) {
          work->transferMode = CANVAS_COPY;
        }
        if (frame <= 371) {
          BattleEffect_DrawShadedCircles(work->sheet + CHARON_ACT_PICTURES, t,
                                         FIXED_ONE);
        } else {
          t = 104;
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        Canvas_BlitAtFoot(work->sheet + CHARON_ACT_PICTURES, height, 58, t);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                            BLITTER_ADDITIVE);
        blit[1] = Ram_HeapSlots->ramCodePaired;
      }
      for (j = 0; j != 2; j++) {
        s32 t = frame - 357 - j * 19;

        if (t == 0) {
          Iwram_FillWords((u16 *)&BG_PLTT_COLOR(6, 0), 0x100,
                          COLOR_PAIR(RGB_WHITE));
        }
        if (t >= 0 && t < 160) {
          u16 *target = Resource_GetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP);
          u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);

          for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, target++, color++) {
            s32 b = RGB_BLUE(*color);
            s32 g = RGB_GREEN(*color);
            s32 r = RGB_RED(*color);
            s32 tb;
            s32 tg;
            s32 tr;

            if (t < 8) {
              tb = RGB_BLUE(*target);
              tg = RGB_GREEN(*target);
              tr = RGB_RED(*target);
            } else {
              tb = RGB_BLUE(*target) - 16;
              tg = RGB_GREEN(*target) - 16;
              tr = RGB_RED(*target) + 8;
            }
            if (tb < 0) {
              tb = 0;
            }
            if (tg < 0) {
              tg = 0;
            }
            if (tr > RGB_LEVEL_MAX) {
              tr = RGB_LEVEL_MAX;
            }
            if (b > tb + 1) {
              b -= 2;
            }
            if (g > tg + 1) {
              g -= 2;
            }
            if (r > tr + 1) {
              r -= 2;
            }
            *color = RGB(r, g, b);
          }
        }
      }
    }
    if (t56 >= 0 && t56 < 24) {
      s32 radius = frame * 2 - 104;
      s32 row = (radius - 10) / 6;
      s32 x;
      s32 y;
      s32 w;
      s32 h;
      FIXED cosine;

      if (row < 0) {
        row = 0;
      }
      w = 4;
      for (i = 0, h = 8; i != 256; i++) {
        x = FIXEDtoINT(Trig_Sin(i << 8) * radius * 2);
        cosine = Trig_Cos(i << 8);
        y = (FIXEDtoINT(cosine * radius)) + 68;
        x += 66;
        routine[0](canvas, glyphs + row * 32, x - w / 2, y, w, h);
      }
    }
    if (frame >= 108 && frame < 132) {
      u16 *color;

      for (i = 0, color = (u16 *)&BG_PLTT_COLOR(6, 0);
           i != BATTLE_BACKGROUND_COLORS; i++, color++) {
        s32 r = RGB_RED(*color);
        s32 g = RGB_GREEN(*color);
        s32 b = RGB_BLUE(*color);

        g--;
        if (g < 0) {
          r--;
        }
        if (r > RGB_LEVEL_MAX) {
          r = RGB_LEVEL_MAX;
        } else if (r < 0) {
          r = 0;
        }
        if (g > RGB_LEVEL_MAX) {
          g = RGB_LEVEL_MAX;
        } else if (g < 0) {
          g = 0;
        }
        if (b > RGB_LEVEL_MAX) {
          b = RGB_LEVEL_MAX;
        } else if (b < 0) {
          b = 0;
        }
        *color = RGB(r, g, b);
      }
    }
    if (t56 >= 0 && t56 < 36) {
      if (frame == 56) {
        s32 x;
        s32 y;

        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
        source = (u8 *)Resource_GetTableEntry(RESOURCE_RUNE_SEAL_SHEET);
        source += 32;
        Decompress_TaggedLz(source, work->sheet + CHARON_RUNE_SEAL);
        for (y = 0; y != RUNE_SEAL_HEIGHT; y++) {
          for (x = 0; x != RUNE_SEAL_WIDTH; x += 2) {
            u16 *dst =
                (u16 *)(CHARON_SEAL_TILES +
                        ((((y / 8) * 32 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)));
            u8 hi = *dst >> 8;
            u8 lo = *dst;

            if (work->sheet[CHARON_RUNE_SEAL + y * RUNE_SEAL_WIDTH + x + 1] !=
                0) {
              hi = work->sheet[CHARON_RUNE_SEAL + y * RUNE_SEAL_WIDTH + x + 1] -
                   32;
            }
            if (work->sheet[CHARON_RUNE_SEAL + y * RUNE_SEAL_WIDTH + x] != 0) {
              lo = work->sheet[CHARON_RUNE_SEAL + y * RUNE_SEAL_WIDTH + x] - 32;
            }
            *dst = (hi << 8) | lo;
          }
        }
      }
      if (frame < 88) {
        u16 *color = Resource_GetTableEntry(RESOURCE_RUNE_SEAL_SHEET);
        u16 *entry = (u16 *)&BG_PLTT_COLOR(14, 0);

        for (i = 0; i != 16; i++, color++, entry++) {
          s32 add = (FIXEDtoINT(Trig_Cos(t56 << 9) * 12)) + 16;
          s32 r = RGB_RED(*color) + add;
          s32 g = RGB_GREEN(*color) + add;
          s32 b = RGB_BLUE(*color) + add;

          if (r > RGB_LEVEL_MAX) {
            r = RGB_LEVEL_MAX;
          }
          if (g > RGB_LEVEL_MAX) {
            g = RGB_LEVEL_MAX;
          }
          if (b > RGB_LEVEL_MAX) {
            b = RGB_LEVEL_MAX;
          }
          *entry = RGB(r, g, b);
        }
      }
    }
    if (frame == 54) {
      Iwram_FillWords(canvas, CANVAS_VIEW_BYTES, CANVAS_FILL_WORD(32));
    }
    if (frame == 55) {
      Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame < 132) {
      s32 risingScale;
      s32 depth;
      s32 rise = -52576;
      s32 base;
      s32 quarter;
      s32 twice;
      void *texture = Heap_AllocEwram(32);
      struct RenderModel *model = Render_AllocateModelList(1);

      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      twice = frame * 2;
      quarter = frame * 381;
      depth = twice - 64;
      base = quarter * 4;
      risingScale = 115536 - base;
      if (frame > 55) {
        risingScale = base + rise;
      }
      if (depth >= -8) {
        depth = -8;
      }
      if (frame > 95) {
        depth = 184 - frame * 2;
        model->shade = depth;
      }
      risingTexture.logWidth = 6;
      risingTexture.logHeight = 6;
      risingTexture.pixels = work->sheet;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &risingTexture;
      model->mesh = gBattleEffect_SquareMesh;
      model->vertices = texture;
      if (depth > -48) {
        model->shade = depth;
        Matrix_Initialize();
        Matrix_Translate(toFIXED(4), 0, 0);
        Matrix_Scale(risingScale, risingScale, risingScale);
        Matrix_RotateX(DEGtoANG(45));
        if (frame > 55) {
          Matrix_RotateY(frame << 10);
        } else {
          Matrix_RotateY(-frame << 10);
        }
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture, 4);
        Render_DrawModelList(model);
      }
      if (frame > 95) {
        depth = 568 - frame * 6;
        model->shade = depth;
      }
      if (depth > -48) {
        Matrix_Initialize();
        Matrix_Translate(toFIXED(4), -toFIXED(16), 0);
        Matrix_ScaleUniform(-0xa710);
        Matrix_RotateX(DEGtoANG(45));
        Matrix_RotateY(-frame << 9);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture, 4);
        Render_DrawModelList(model);
      }
      if (frame > 55) {
        s32 last = frame * 2 - 208;

        risingTexture.pixels = work->sheet + CHARON_ACT_PICTURES;
        if (last >= -32) {
          last = -32;
        }
        model->shade = last;
        Matrix_Initialize();
        Matrix_Translate(toFIXED(2), -toFIXED(32), 0);
        Matrix_ScaleUniform(0x58f0);
        Matrix_RotateX(DEGtoANG(45));
        Matrix_RotateY(frame << 10);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture, 4);
        Render_DrawModelList(model);
        /* FAKEMATCH(dead): twice raised to frame though nothing reads it again.
         */
        if (frame > twice) {
          twice = frame;
        }
        if (frame > -48) {
          Matrix_Initialize();
          Matrix_Translate(toFIXED(2), -toFIXED(48), 0);
          Matrix_ScaleUniform(-0x31e0);
          Matrix_RotateX(DEGtoANG(45));
          Matrix_RotateY(-frame << 10);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture,
                                   4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(texture);
      // DEAD: each test reads the count twice and drops the first read; the ROM
      // keeps both loads.
      while (*(volatile u32 *)&work->vblanksSinceTransfer,
             *(volatile u32 *)&work->vblanksSinceTransfer <= 1) {
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  Sound_PlayFar(SE_WAVE);
  if (frame < 132) {
    Sprite_DestroyFar((struct Sprite *)OBJECTS[0]);
  } else {
    for (i = 0; i != CHARON_OBJECTS; i++) {
      Sprite_DestroyFar((struct Sprite *)OBJECTS[i]);
    }
  }
  for (i = 0; i != CHARON_ARROWS; i++) {
    Sprite_DestroyFar((struct Sprite *)SPRITES[i]);
  }
  EFFECT_RELEASE_BLITTERS();
  Palette_LoadBasicSpriteColors();
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON;
  wait(1);
  battle->keepBackdropPalette = 0;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, 24);
  IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
  IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES, 0);
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  camera = Ram_HeapSlots->cameraWork;
  shake = 0;
  savedYaw = camera->yaw;
  camera->yaw = shake;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = BLEND_ALPHA(16, 4);
  REG_BG2X = 0;
  REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  for (i = 1; i != 64; i++) {
    s32 q = i * 2 / 3;
    s32 r = 31 - q;
    s32 g = 8 - i / 2;
    s32 b = 27 - q;

    if (r < 0) {
      r = 0;
    }
    if (g < 0) {
      g = 0;
    }
    if (b < 0) {
      b = 0;
    }
    ((u16 *)BG_PLTT)[i] = RGB(r, g, b);
  }
  arg->targets[arg->targetCount] = 0xff;
  BattleActor_SpawnObjectsForListFar(arg->targets, 0);
  BattleEffect_DrawShadedCircles(work->sheet, 56, FIXED_ONE);
  BattleEffect_DrawShadedCircles(work->sheet + CHARON_ACT_PICTURES, 16,
                                 FIXED_ONE);
  Iwram_FillWords(work->sheet + CHARON_BRIGHT_BLOCK, 0x1000,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  Resource_LoadAndDecompress(RESOURCE_GLOW_RING_TEXTURE,
                             work->sheet + CHARON_GLOW_RING, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, glyphs, 0, 0);
  {
    s32 limit;

    i = 0;
    limit = 0x302;
  empty:
    i += 35;
    if (i != limit) {
      goto empty;
    }
  }
  for (i = 0; i != 0x1000; i++) {
    if (work->sheet[CHARON_GLOW_RING + i] != 0) {
      work->sheet[CHARON_GLOW_RING + i] = CANVAS_LEVEL_BRIGHTEST;
    }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
  for (i = 0; i != 16; i++) {
    DROPS[i].variant = -i;
  }
  for (i = 0; i != 128; i++) {
    CHARON_PARTICLES[i].variant = -1;
  }
  for (i = 0; i != arg->targetCount; i++) {
    struct Actor *actor = BattleObject_GetFar(arg->targets[i])->object;

    HOMES[i].x = actor->x.fixed;
    HOMES[i].y = actor->y.fixed;
    HOMES[i].z = actor->z.fixed;
  }

  width = 56;
  twenty = 20;
  frame = 0;
  do {
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame < 120) {
      frame = 120;
    }
    if (frame == 0) {
      Sound_PlayFar(SE_WAVE);
      WAVE_SIZE = 160;
      shake = 0;
      for (i = 0; i != arg->targetCount; i++) {
        MEMBERS[i].variant = -1;
      }
      for (i = 0; i != arg->targetCount; i++) {
        struct Actor *actor = BattleObject_GetFar(arg->targets[i])->object;

        BattleEffect_TintBattler(arg->targets[i], SPRITE_COLOR_NORMAL,
                                 POSE_DAMAGE, TINT_KEEP, -1);
        actor->x.fixed = -(i + 4) * toFIXED(128);
        actor->z.fixed = 0;
        actor->y.fixed = toFIXED(20);
        actor->gravity = 0;
      }
    }
    if (frame == 120) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
      ColorBuffer_ScaleFar(battle->palette, (u16 *)&BG_PLTT_COLOR(6, 0), 0x99a0,
                           BATTLE_BACKGROUND_COLORS);
      for (i = 0; i != arg->targetCount; i++) {
        struct Actor *actor = BattleObject_GetFar(arg->targets[i])->object;

        SET_ACTOR_POSITION(actor, 0, toFIXED(124), 0);
        actor->gravity = 0;
      }
    }
    if (frame == 160) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if (frame >= 140) {
      if (frame == 140) {
        for (i = 0; i != 32; i++) {
          s32 speed = (Random16() & 511) + 32;
          s32 angle = Random16();

          CHARON_PARTICLES[96 + i].x = toFIXED(60);
          CHARON_PARTICLES[96 + i].y = toFIXED(60);
          CHARON_PARTICLES[96 + i].velocityX = (Trig_Sin(angle) * speed) >> 5;
          CHARON_PARTICLES[96 + i].velocityY = (Trig_Cos(angle) * speed) >> 5;
          CHARON_PARTICLES[96 + i].variant = 0;
        }
        for (i = 0; i != 128; i++) {
          s32 speed = (Random16() & 255) + 32;
          s32 angle = Random16();

          CHARON_PARTICLES[128 + i].x = toFIXED(60);
          CHARON_PARTICLES[128 + i].y = toFIXED(60);
          CHARON_PARTICLES[128 + i].velocityX = (Trig_Sin(angle) * speed) >> 6;
          CHARON_PARTICLES[128 + i].velocityY = (Trig_Cos(angle) * speed) >> 6;
          CHARON_PARTICLES[128 + i].variant = (Random16() & 15) + 16;
        }
        Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, SPRITE_BUFFER, 1,
                                   0);
        Sound_PlayFar(SE_CLATTER);
        Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                        CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        for (i = 0; i != arg->targetCount; i++) {
          struct Actor *actor = BattleObject_GetFar(arg->targets[i])->object;

          SET_ACTOR_POSITION(actor, 0, toFIXED(32), HOMES[i].z);
        }
        for (i = 0; i != arg->targetCount; i++) {
          s32 spin = Random16();
          s32 delay = (Random16() & 31) + 120;
          s32 speed = spin * 2 + 0x20000;

          BattleEffect_StartActorMotion(arg->targets[i], 1, toFIXED(4),
                                        toFIXED(8), speed, delay);
          BattleEffect_TintBattler(arg->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 32);
        }
        work->shakeFrames = 3;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
      routine[0] = ((DrawRectangle)Ram_HeapSlots->ramCode);
      for (i = 0; i != 16; i++) {
        struct Particle *p = &CHARON_PARTICLES[96 + i];

        if (p->variant >= 0 && p->variant < 24) {
          s32 row = (p->variant / 4) * 2048;
          u8 *buffer = SPRITE_BUFFER;
          s32 x = (FIXEDtoINT(p->x)) - 16;

          row += (s32)buffer;
          buffer = (u8 *)row;
          routine[0](canvas, buffer, x, (FIXEDtoINT(p->y)) - 32,
                     FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          BattleEffectKernels_IntegrateVector2(p, 60, -toFIXED(1) / 8);
        }
        p->variant++;
      }
      for (i = 0; i != 128; i++) {
        struct Particle *p = &CHARON_PARTICLES[128 + i];

        if (p->variant >= 0) {
          s32 size = (p->variant >> 3) + 2;
          s32 k;

          for (k = 0; k != 4; k++) {
            routine[0](canvas, glyphs + gBattleEffect_MotePictures[size - 1],
                       (FIXEDtoINT(p->x)) - size / 2, (FIXEDtoINT(p->y)) - size,
                       size, size * 2);
            BattleEffectKernels_IntegrateVector2(p, 63, 0);
            if (p->y > toFIXED(112)) {
              p->velocityY = -p->velocityY;
            }
          }
          p->variant--;
        }
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    }
    if (frame < 120) {
      for (i = 0; i != 3; i++) {
        if (frame == gCharon_FlashFrames[i]) {
          Iwram_FillWords((u16 *)&BG_PLTT_COLOR(6, 0), 0x100,
                          COLOR_PAIR(RGB_WHITE));
        }
        if (frame >= gCharon_FlashFrames[i] + 1 &&
            frame < gCharon_FlashFrames[i] + 16) {
          u16 *target = Ram_HeapSlots->battleWork->palette;
          u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);

          for (j = 0; j != BATTLE_BACKGROUND_COLORS; j++) {
            s32 tr = RGB_RED(target[j]) - twenty / 2;
            s32 tg = RGB_GREEN(target[j]) - 20;
            s32 tb = RGB_BLUE(target[j]) - twenty / 2;
            s32 r = RGB_RED(color[j]);
            s32 g = RGB_GREEN(color[j]);
            s32 b = RGB_BLUE(color[j]);

            if (tr < 0) {
              tr = 0;
            }
            if (tg < 0) {
              tg = 0;
            }
            if (tb < 0) {
              tb = 0;
            }
            if (tr < r - 1) {
              r -= 2;
            }
            if (tg < g - 1) {
              g -= 2;
            }
            if (tb < b - 1) {
              b -= 2;
            }
            color[j] = RGB(r, g, b);
          }
        }
      }
    }
    if (frame > 120) {
      if (frame == 121) {
        gBgScroll[1].x = savedWidth;
        backgroundView->busy = 0;
        camera->yaw = savedYaw;
      }
      if (frame < 148) {
        s32 step = 8 - (frame - 120) / 6;
        s32 wave = WAVE_SIZE;

        WAVE_SIZE = wave - step;
        if (wave > 0) {
          BattleEffect_DrawShadedCircles(work->sheet, wave, FIXED_ONE);
          Canvas_BlitAtFoot(work->sheet, 60, 60, wave);
        }
      }
    } else {
      if (frame >= 4 && frame < 26) {
        shake += 4;
      }
      if (frame > 107) {
        shake -= 8;
        if (shake < 0) {
          shake = 0;
        }
      }
      for (i = 0; i != arg->targetCount; i++) {
        struct Actor *actor = BattleObject_GetFar(arg->targets[i])->object;
        struct Particle *member = &MEMBERS[i];
        s32 landed = 0;

        if (actor->y.fixed < toFIXED(100)) {
          if (member->variant == -1) {
            actor->x.fixed += toFIXED(10);
          } else {
            actor->x.fixed += toFIXED(2);
          }
          if (member->variant == -1) {
            if (actor->x.fixed > toFIXED(48)) {
              landed = 1;
            }
            if (landed == 1) {
              member->variant = 0;
              EffectPosition_ProjectScaledBattler(arg->targets[i], member);
              Sound_PlayFar(SE_CURSE);
              for (j = 0; j != 16; j++) {
                s32 angle = Random16() & 0x7fff;
                s32 speed;
                struct Particle *p = &CHARON_PARTICLES[i * 16 + j];

                speed = (Random16() & 255) + 256;

                p->x = member->x << 15;
                p->y = INTtoFIXED(member->y);
                p->velocityX = -(Trig_Sin(angle) * speed) >> 7;
                p->velocityY = (Trig_Cos(angle) * speed) >> 6;
                p->variant = (Random16() & 15) + 16;
              }
            }
          }
        }
        if (member->variant == 2 || member->variant == 7) {
          BattleEffect_TintBattler(arg->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 32);
        }
        /* FAKEMATCH(dead): landed set to 2 though nothing reads it again. */
        if (actor == 0) {
          landed = 2;
        }
        if (member->variant == 6) {
          BattleEffect_TintBattler(arg->targets[i], SPRITE_COLOR_HIDDEN,
                                   TINT_KEEP, i, 32);
        }
        if (member->variant == 8) {
          SET_ACTOR_POSITION(actor, 0, toFIXED(148), 0);
        }
      }
      if (frame >= 0) {
        gBgScroll[1].x += 0xfff4;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
        {
          struct Particle *column;
          s32 x;
          s32 y;
          s32 y2;
          for (i = 0, y2 = -26, y = 0, x = shake, phase = frame << 10,
              column = COLUMNS, halfCell = width / 2;
               i != 4;
               y2 += 30, y += 30, x += 8, phase += 0x2000, column++, i++) {
            column->x = x + (FIXEDtoINT(Trig_Sin(phase) * 4));
            column->y = y;
            if (i == 3) {
              column->x -= 8;
              column->y = y2;
              HeapSlot_Release(HEAP_SLOT_RAM_CODE);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                                  BLITTER_MAX);
            }
            ((DrawRectangle)Ram_HeapSlots->ramCode)(canvas, work->sheet,
                                                    column->x - halfCell,
                                                    column->y, halfCell, width);
          }
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        for (i = 0; i != 12; i++) {
          struct Particle *drop = &DROPS[i];

          if (++drop->variant == 4) {
            drop->variant = 0;
          }
          if (drop->variant == 0) {
            drop->x = INTtoFIXED(Random16() % 24 + shake - i * 2);
            drop->y = toFIXED(80) + i * (192 << 10);
            drop->velocityX = -(Random16() & TRIG_ANGLE_MASK) - 0x4000;
            drop->velocityY = -toFIXED(1);
          }
          if (drop->variant >= 0) {
            Canvas_BlitAtFoot(work->sheet + CHARON_ACT_PICTURES,
                              FIXEDtoINT(drop->x), FIXEDtoINT(drop->y), 16);
            drop->x += drop->velocityX;
            drop->y += drop->velocityY;
          }
        }
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
        routine[0] = ((DrawRectangle)Ram_HeapSlots->ramCode);
        for (i = 0; i != 4; i++) {
          s32 width = 120 - COLUMNS[i].x;

          if (width > 0) {
            routine[0](canvas, work->sheet + CHARON_BRIGHT_BLOCK, COLUMNS[i].x,
                       i * 30, width, 30);
          }
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                            BLITTER_TRANSPARENT);
        routine[0] = ((DrawRectangle)Ram_HeapSlots->ramCode);
        for (i = 0; i != 96; i++) {
          struct Particle *p = &CHARON_PARTICLES[i];

          if (p->variant >= 0) {
            s32 size = (p->variant >> 3) + 2;

            routine[0](canvas, glyphs + gBattleEffect_MotePictures[size - 1],
                       (FIXEDtoINT(p->x)) - size / 2, (FIXEDtoINT(p->y)) - size,
                       size, size * 2);
            BattleEffectKernels_IntegrateVector2(p, 60, 0);
            p->variant--;
          }
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      }
      {
        void *texture = Heap_AllocEwram(32);
        struct RenderModel *model = Render_AllocateModelList(1);

        openingTexture.logWidth = 6;
        openingTexture.logHeight = 6;
        openingTexture.pixels = work->sheet + CHARON_GLOW_RING;
        model->kind = MODEL_SHADED;
        model->mesh = gBattleEffect_SquareMesh;
        model->texture = &openingTexture;
        model->vertices = texture;
        for (i = 0; i != arg->targetCount; i++) {
          struct Particle *member = &MEMBERS[i];
          s32 timer = member->variant;

          if (timer >= 0 && timer < 18) {
            s32 step = timer * 3;
            s32 depth = step - 64;
            s32 size = step * 1016 + 0x800;

            member->variant = timer + 1;
            model->shade = depth;
            Matrix_Initialize();
            Matrix_Translate(INTtoFIXED(member->x / 2 - 56),
                             INTtoFIXED(member->y - 56), 0);
            Matrix_Scale(size / 3, size, size);
            Matrix_RotateX(TRIG_TURN * 3 / 4);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture,
                                     4);
            Render_DrawModelList(model);
          }
          Heap_Release(model);
          Heap_Release(texture);
        }
      }
    }
    Camera_ApplyVerticalShake(8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != 180);
  for (i = 0; i != arg->targetCount; i++) {
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Two triangles of a strip: three vertex indices and a texture coordinate
/// pair for each corner.
struct StripTriangle {
  u8 vertex[3];
  u8 pad;
  u8 uv[3][2];
  u8 endPad[2];
};

struct StripQuad {
  struct StripTriangle upper;
  struct StripTriangle lower;
};

/// Builds count quads of a vertex strip two vertices apart, each width texels
/// wide and height texels tall, and ends the list with an empty triangle.
void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count) {
  s32 i;

  for (i = 0; i != count; i++) {
    quad[i].upper.uv[0][1] = 1;
    quad[i].upper.uv[2][1] = 1;
    quad[i].lower.uv[2][1] = 1;
    quad[i].upper.vertex[0] = i * 2;
    quad[i].upper.uv[0][0] = i * width;
    quad[i].upper.uv[1][0] = i * width;
    quad[i].upper.uv[2][0] = i * width + width - 1;
    quad[i].lower.uv[0][0] = i * width;
    quad[i].lower.uv[1][0] = i * width + width - 1;
    quad[i].lower.uv[2][0] = i * width + width - 1;
    quad[i].upper.vertex[1] = i * 2 + 1;
    quad[i].upper.vertex[2] = i * 2 + 2;
    quad[i].lower.vertex[0] = i * 2 + 1;
    quad[i].lower.vertex[2] = i * 2 + 2;
    quad[i].lower.vertex[1] = i * 2 + 3;
    quad[i].upper.uv[1][1] = height - 1;
    quad[i].lower.uv[0][1] = height - 1;
    quad[i].lower.uv[1][1] = height - 1;
  }
  SET_VECTOR(quad[count].upper.vertex, 0, 0, 0);
}
#undef SPRITES
#undef SPRITE_BUFFER
#undef OAM_BUFFER
#undef REVEAL_BUFFER
#undef OBJ_VRAM
#undef CHARON_SEAL_TILES
#undef DROPS
#undef MEMBERS
#undef HOMES
#undef COLUMNS
#undef WAVE_SIZE
#undef OBJECTS
