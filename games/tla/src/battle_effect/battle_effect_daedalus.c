/// @file
/// A symmetrical burst over the target, and the Daedalus summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define BURST_FRAMES 48
#define BURST_DRAW_FRAMES 28

/// The three 64x64 cels of the burst, built in the sheet's first 0x3000
/// bytes; the picture loads behind them.
#define CEL_SIZE 0x1000
#define PICTURE_OFFSET 0x4000

/// Copies a half-picture of width by height pixels into the bottom middle of
/// a 64x64 cel, once as it is and once mirrored, so the cel is symmetrical.
#define MIRROR_HALF(cel, half, width, height)                                  \
  for (y = 0; y != (height); y++) {                                            \
    for (x = 0; x != (width); x++) {                                           \
      work->sheet[(cel) * CEL_SIZE + (y + 64 - (height)) * 64 + 32 - (width) + \
                  x] = work->sheet[(half) + y * (width) + x];                  \
      work->sheet[(cel) * CEL_SIZE + (y + 64 - (height)) * 64 + 31 + (width) - \
                  x] = work->sheet[(half) + y * (width) + x];                  \
    }                                                                          \
  }

/// Raises a mirrored wall at the first target's feet while the target glows.
void BattleEffect_RunMirroredWall(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct ModelTexture texture;
  struct ScreenPoint target;
  struct RenderModel *quad;
  void *vertices;
  s32 frame;
  s32 cel;
  s32 y;
  s32 x;

  // Build the three cels from their halves.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_EARTH_WALL_SHEET,
                             work->sheet + PICTURE_OFFSET, 1, 1);
  Iwram_ClearWords(work->sheet, 0x4000);
  MIRROR_HALF(0, PICTURE_OFFSET + 0xa8e, 17, 49)
  MIRROR_HALF(1, PICTURE_OFFSET + 0xdcf, 19, 55)
  MIRROR_HALF(2, PICTURE_OFFSET + 0x11e4, 20, 64)
  if (mode == MIRRORED_WALL_SACK)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                    SHEET_PALETTE_BYTES);
  else if (mode == MIRRORED_WALL_RECOVERY)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_FIREBALL_SHEET),
                    SHEET_PALETTE_BYTES);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET),
                    SHEET_PALETTE_BYTES);

  // Centre the canvas's x 64 on the target.
  EffectPosition_ProjectBattler(args->targets[0], &target);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = (64 - target.x) << 8;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (mode == MIRRORED_WALL_RECOVERY) {
    work->shakeFrames = 0;
    Sound_PlayFar(SE_POWER_UP);
  } else if (mode == MIRRORED_WALL_BERSERK || mode == MIRRORED_WALL_SACK) {
    work->shakeFrames = 8;
    Sound_PlayFar(SE_POWER_UP);
  } else {
    work->shakeFrames = 32;
  }

  for (frame = 0; frame != BURST_FRAMES; frame++) {
    if (frame == 0) {
      if (mode == MIRRORED_WALL_RECOVERY)
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, 0, 32);
      else
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_RED_TINT,
                                 TINT_KEEP, 0, 32);
    }
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(0);
    if (frame == 8 && mode == MIRRORED_WALL_SPASM)
      Sound_PlayFar(SE_RECOVERY);

    // Cels 0, 1 and 2 four frames each, then 1 and 2 by turns.
    if (frame < 32) {
      cel = frame / 4;
      if (cel > 2)
        cel = (cel & 1) + 1;
      if (frame < BURST_DRAW_FRAMES) {
        vertices = Heap_AllocEwram(32);
        quad = Render_AllocateModelList(1);
        texture.logWidth = 6;
        texture.logHeight = 6;
        texture.pixels = work->sheet + cel * CEL_SIZE;
        quad->kind = MODEL_SHADED_MAX;
        quad->mesh = gBattleEffect_SquareMesh;
        quad->texture = &texture;
        quad->shade = gMirroredWall_BurstShades[mode];
        quad->vertices = vertices;
        Matrix_Initialize();
        Matrix_Translate(0, INTtoFIXED(target.y - 58), 0);
        Matrix_ScaleUniform(gMirroredWall_BurstScales[mode] << 1);
        Render_TransformVertices(gBattleEffect_StandingSquareCorners, vertices,
                                 4);
        Render_DrawModelList(quad);
        Heap_Release(quad);
        Heap_Release(vertices);
      }
    }

    if (mode == MIRRORED_WALL_SPASM)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}

/// Copies 288 rows of 40 bytes, reducing each horizontal pair to one byte.
/// The second byte, from the odd column, survives.
/// stride: source bytes per row; destination uses half as many.
void BattleEffect_PackTileRows(u8 *src, u8 *dst, s32 stride) {
  s32 y = 0;
  s32 h = 288;
  s32 offset = 0;

  do {
    s32 x;

    for (x = 0; x != YELLOW_ORB_WIDTH; x++) {
      dst[offset / 2 + x / 2] = src[offset + x];
    }
    offset += stride;
    y++;
  } while (y != h);
}

typedef struct DaedalusSprite {
  /// The part's link and its attributes 0 and 1, which the effect leaves alone.
  u8 linkAndPosition[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unusedAfterPalette[2];
  s32 scale;
  u8 slot;
  u8 cacheFlags;
  u16 rotation;
  u8 size[2];
  u8 offsetX;
  u8 offsetY;
} Sprite;

/// The effect work's sprite slot `i`, read as Daedalus's sprite.
#define DAEDALUS_SPRITE(i) ((Sprite *)work->sprites[i])

/// A sprite's first ten bytes, its attribute 2 fields after eight others.
struct SpriteAttributes {
  u8 linkAndPosition[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

typedef void (*CopyFunc)(void *, const void *, s32);
typedef void (*ClearFunc)(void *, s32);
typedef void (*BlitFunc)(void *, const void *, s32, s32, s32, s32);

/// Iwram_CopyWords reached from IwramIrqMain rather than at its fixed address,
/// as the trade's snapshot copies reach it: its address is then a pool load,
/// made after the arguments and after the call before it.
#define IWRAM_COPY_WORDS_LINKED                                                \
  ((CopyFunc)(IwramIrqMain + Iwram_CopyWordsOffset))

#define DECODE_BUFFER Ram_MapCellBuffer
#define DAEDALUS_PARTICLES ((struct Particle *)(Ram_MapCellBuffer + 0x4000))

/// Where FlashBurstSheet is decoded in the effect's pictures.
#define DAEDALUS_FLASH_BURST_PICTURES 0x6c0

/// Where BirdFlockSheet is decoded in the effect's frames.
#define DAEDALUS_BIRD_FLOCK 0x36c0

/// Where RocketFlameSheet is decoded in the effect's frames.
#define DAEDALUS_ROCKET_FLAMES 0x3000
#define ROCKET_FLAME_WIDTH 12
#define ROCKET_FLAME_HEIGHT 24

/// Runs of DAEDALUS_PARTICLES in the first scene: each missile's trail of 32,
/// the rocket's exhaust flames, the bursts where its shots land, and the
/// birds that drift across.
/// The first scene's sprites: two rows of the forest canopy, eight pictures
/// and a copy of the first in each; the twenty-one 32x32 pieces of the flying
/// machine (BronzeAutomatonTiles), among them its spinning blade, then the
/// 32x64 rocket it launches; and the missile with fifteen copies.
#define DAEDALUS_CANOPY_ROW 9
#define DAEDALUS_MACHINE 18
#define DAEDALUS_MACHINE_PIECES 21
#define DAEDALUS_BLADE 38
#define DAEDALUS_ROCKET 39
#define DAEDALUS_MISSILE 41
#define DAEDALUS_MISSILE_COPIES 15
#define DAEDALUS_TRAIL_LENGTH 32
#define DAEDALUS_EXHAUST 400
#define DAEDALUS_EXHAUST_FLAMES 32
#define DAEDALUS_BURSTS 432
#define DAEDALUS_BURST_COUNT 32
#define DAEDALUS_BIRDS 502
#define DAEDALUS_BIRD_COUNT 10

/// Daedalus: over a forest the great machine rises into the sky among birds,
/// turns and fires its rockets, and they fall on the foes in bursts of fire;
/// then the second scene (BattleEffect_RunDaedalusAlt) plays.
///
/// Abilities: Daedalus.
void BattleEffect_RunDaedalus(struct BattleEffectParams *args) {
  s16 targets[UNIT_MAX];
  struct SpritePlacement pos;
  struct SpritePlacement shipPlace;
  struct SpritePlacement particlePlace;
  struct Scale scale;
  void *blit[2];
  struct Scale shipScale;
  struct ModelTexture bitmap;
  struct Scale particleScale;
  u8 *dest;
  s32 frame;
  u8 *map;
  s32 *ctl;
  s32 *bank;
  FIXED camera;
  FIXED prevCamera;
  FIXED waveX;
  s32 waveScroll;
  FIXED shipY;
  s32 wavePhase;
  s32 savedY;
  s32 bgX;
  struct BattleEffectWork *work;
  s32 k;
  s32 i;
  s32 j;
  // The map rows are 16 words apart; held in a local for its whole life.
  s32 step = 16;
  // Reused: the second phase's ring radius, and the 0 the third phase stores.
  s32 radius;

  dest = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  map = Ram_HeapSlots->battleSheet;
  ctl = (s32 *)Ram_HeapSlots->battleBackground;
  bank = (s32 *)Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  RagingHeat_FillGradientCanvas();
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit[0] = Ram_HeapSlots->ramCode;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  camera = 0;
  waveScroll = 0;
  shipY = -toFIXED(29);
  wavePhase = 0;
  savedY = gBgScroll[1].x;
  bgX = -80;

  for (frame = 0; frame != SUMMON_OPENING_FRAMES; frame++) {
    if (frame == SUMMON_OPENING_LOAD_FRAME) {
      u16 *col;
      s32 n;
      s32 w;
      s32 off;
      u8 *mp;
      s32 k;
      s32 r, g, b;
      s32 v;
      const u8 *src;
      Sprite *obj;

      VramCache_FreeEntry(bank[21]);
      gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
      targets[0] = BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForListFar(targets, 0);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_VIOLET_SKY_BACKDROP, 0);
      for (col = (u16 *)&BG_PLTT_COLOR(6, 0), i = 0;
           i != BATTLE_BACKGROUND_COLORS; i++, col++) {
        s32 mid;
        s32 s;

        s = RGB_RED(*col);
        mid = RGB_GREEN(*col);
        b = RGB_BLUE(*col) - 4;
        g = mid - 4;
        r = s - 4;
        if (g < 0)
          g = 0;
        if (r < 0)
          r = 0;
        if (b < 0)
          b = 0;
        if (g > RGB_LEVEL_MAX)
          g = RGB_LEVEL_MAX;
        if (r > RGB_LEVEL_MAX)
          r = RGB_LEVEL_MAX;
        if (b > RGB_LEVEL_MAX)
          b = RGB_LEVEL_MAX;
        *col = RGB(g, r, b);
      }
      work->scrollTimer = 0;
      work->scrollInterval = 5;
      work->scrollStepX = -1;
      work->scrollStepY = 0;
      Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_VBLANK);
      ctl[4] = 1;
      Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, work->sheet, 0,
                                 0);
      off = 0x700;
      n = 3;
      for (j = 0; j != 10; j++) {
        k = j * step;
        i = 0;
        // v, the loop's value local, first holds the row's byte offset.
        v = k * 2;
        mp = (u8 *)(v + (s32)map);
        do {
          v = ((u8 *)work)[gBattleEffect_MotePictures[n] + i + off];
          /* FAKEMATCH(dead): w set to 32 inside the row loop, after the load.
           */
          w = 32;
          if (v != 0) {
            v = v - j * 4 - 40;
            if (v <= 0)
              v = 1;
          }
          *mp++ = v;
        } while (++i != w);
      }
      src = (const u8 *)Resource_GetTableEntry(RESOURCE_FOREST_CANOPY_TILES);
      Iwram_CopyWords((void *)&OBJ_PLTT_COLOR(15, 0), src, PALETTE_BANK_BYTES);
      src += PALETTE_BANK_BYTES;
      Decompress_TaggedLz(src, work->sheet);
      for (j = 0; j != 2; j++) {
        for (i = 0; i != 8; i++) {
          obj = EffectSprite_CreateWithTiles(32, 32, OBJ_SIZE_32X32,
                                             OBJ_PALETTE(15));
          work->sprites[j * DAEDALUS_CANOPY_ROW + i] = obj;
          obj->priority = 1;
          Iwram_CopyWords(
              (void *)(gVramCacheEntries[obj->slot].offset + OBJ_VRAM0),
              &work->sheet[(j << 12) + i * 0x200], 0x200);
        }
        obj = EffectSprite_Acquire(32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(15));
        work->sprites[j * DAEDALUS_CANOPY_ROW + 8] = obj;
        Iwram_CopyWords(obj, work->sprites[j * DAEDALUS_CANOPY_ROW], 24);
        for (i = 0; i != 8; i++)
          work->particles[j * DAEDALUS_CANOPY_ROW + i].variant =
              DAEDALUS_SPRITE(j * DAEDALUS_CANOPY_ROW + i)->tile;
      }
      src = (const u8 *)Resource_GetTableEntry(RESOURCE_BRONZE_AUTOMATON_TILES);
      Iwram_CopyWords((void *)OBJ_PLTT, src, 0x1c0);
      src += 0x1c0;
      Decompress_TaggedLz(src, DECODE_BUFFER);
      for (i = 0; i != DAEDALUS_MACHINE_PIECES; i++) {
        obj = EffectSprite_CreateWithTiles(32, 32,
                                           OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
        obj->priority = 3;
        work->sprites[DAEDALUS_MACHINE + i] = obj;
        Iwram_CopyWords(
            (void *)(gVramCacheEntries[obj->slot].offset + OBJ_VRAM0),
            DECODE_BUFFER + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
      }
      obj = EffectSprite_CreateWithTiles(32, 64,
                                         OBJ_SIZE_32X64 | OBJ_256_COLORS, 0);
      /* FAKEMATCH(dead): the slot's store alone in a once-only block; as the
       * plain store, 83 of 3921 instructions differ in tla-en. */
      do {
        work->sprites[DAEDALUS_MACHINE + i] = obj;
      } while (0);
      obj->priority = 3;
      /* FAKEMATCH(barrier): an asm operand cuts i, known 21, from the copy's
       * source. */
      __asm__("" : "=r"(i) : "0"(i));
      Iwram_CopyWords((void *)(gVramCacheEntries[obj->slot].offset + OBJ_VRAM0),
                      DECODE_BUFFER + i * 0x400, 0x800);
      src = (const u8 *)Resource_GetTableEntry(RESOURCE_MISSILE_TILES);
      Iwram_CopyWords((void *)&OBJ_PLTT_COLOR(14, 0), src, PALETTE_BANK_BYTES);
      src += PALETTE_BANK_BYTES;
      Decompress_TaggedLz(src, DECODE_BUFFER);
      obj = EffectSprite_CreateWithTiles(16, 8, OBJ_SIZE_16X8, OBJ_PALETTE(14));
      obj->priority = 1;
      work->sprites[DAEDALUS_MISSILE] = obj;
      Iwram_CopyWords((void *)(gVramCacheEntries[obj->slot].offset + OBJ_VRAM0),
                      DECODE_BUFFER, 0x100);
      /* FAKEMATCH(barrier): an empty asm. */
      __asm__("");
      for (i = 0; i != DAEDALUS_MISSILE_COPIES; i++) {
        work->sprites[DAEDALUS_MISSILE + 1 + i] =
            EffectSprite_Acquire(16, 8, OBJ_SIZE_16X8, OBJ_PALETTE(15));
        Iwram_CopyWords(work->sprites[DAEDALUS_MISSILE + 1 + i],
                        work->sprites[DAEDALUS_MISSILE], 24);
        DAEDALUS_SPRITE(DAEDALUS_MISSILE + 1 + i)->tile =
            DAEDALUS_SPRITE(DAEDALUS_MISSILE)->tile;
        if (i > 6)
          DAEDALUS_SPRITE(DAEDALUS_MISSILE + 1 + i)->priority = 2;
      }
    }
    if (frame > 27) {
      struct SpritePlacement *posp;
      s32 row;
      Sprite **slot;

      scale = gDaedalus_WaveScale;
      pos.groundHeight = 0;
      pos.height = toFIXED(255);
      waveScroll += 0x8000;
      waveX = waveScroll + camera;
      if (waveX > 0x1fffff) {
        wavePhase = (wavePhase + 7) % 8;
        waveX -= toFIXED(32);
        waveScroll -= 0x200000;
      }
      for (j = 0; j != 2; j++) {
        for (i = 0; i != 9; i++) {
          struct SpriteAttributes *obj;
          s32 v;

          posp = &pos;
          {
            s32 h;

            k = INTtoFIXED(i * 32) + waveX;
            h = (j << 5) + 103;
            posp->x = k;
            posp->y = INTtoFIXED(h);
          }
          if (i == 8) {
            obj = work->sprites[j * DAEDALUS_CANOPY_ROW + 8];
            v = work->particles[j * 9 + (wavePhase + 71) % 8].variant;
          } else {
            row = j * 9;
            k = row;
            k += i;
            obj = work->sprites[k];
            v = work->particles[row + (wavePhase + 64 + i) % 8].variant;
          }
          obj->tile = v & 0x3ff;
          if (pos.x > 0xffffff)
            pos.x -= toFIXED(288);
          Render_ApplyProjectedPlacementFar(
              work->sprites[j * DAEDALUS_CANOPY_ROW + i], &pos, &scale, 0);
        }
      }
    }
    bgX += 4;
    REG_BG2X = bgX << 8;
    EFFECT_PRESENT_CANVAS(work);
  }

  {
    void **blits;
    u16 *pal;
    s32 swayX;
    s32 swayY;
    FIXED bobX;
    FIXED bobY;
    s32 *ringX;
    void *pixels;

    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Iwram_ClearWordsEntry(dest, 0x3c00);
    Iwram_ClearWordsEntry((void *)BG_CHAR_BLOCK(1), 0x4000);
    REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    REG_BG2PA = Q_8_8(0.5);
    REG_BG2X = 0;
    blits = blit;
    BattleEffect_FetchRectangleBlitters(0, blits);
    work->scrollTimer = 0;
    work->scrollInterval = 5;
    work->scrollStepX = -1;
    work->scrollStepY = 0;
    ctl[4] = 1;
    Resource_LoadAndDecompress(RESOURCE_ROCKET_FLAME_SHEET, work->sheet, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                               work->sheet + DAEDALUS_FLASH_BURST_PICTURES, 1,
                               0);
    Resource_LoadAndDecompress(RESOURCE_BIRD_FLOCK_SHEET,
                               work->sheet + DAEDALUS_BIRD_FLOCK, 0, 0);
    pixels = DECODE_BUFFER;
    Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, pixels, 0, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    (const void *)Resource_GetTableEntry(RESOURCE_SMOKE_SHEET),
                    SHEET_PALETTE_BYTES);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    for (i = 0; i != 224; i++) {
      pal = (u16 *)OBJ_PLTT + i;
      if (RGB_GREEN(*pal) == 31)
        break;
    }
    for (i = 0; i != 512; i++)
      DAEDALUS_PARTICLES[i].variant = -1;

    frame = 0;
    if (!(gInput.held & KEY_B) && !(gInput.repeat & KEY_A)) {
    secondPhase:
      if (frame == 0) {
        REG_BG2PA = Q_8_8(1);
        *pal = 0x190b;
        shipY = -toFIXED(29);
        camera = 0;
        DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation = TRIG_TURN * 3 / 4;
        DAEDALUS_SPRITE(DAEDALUS_BLADE)->rotation = 0;
        DAEDALUS_SPRITE(31)->rotation = 0;
        work->particles[20].x = 0;
        work->particles[20].y = 0;
        work->particles[20].velocityX = -toFIXED(6);
        work->particles[20].velocityY = 0;
        work->particles[20].variant = 0;
        work->particles[23].x = 0;
        work->particles[23].y = 0;
        work->particles[23].velocityX = -toFIXED(12);
        work->particles[23].velocityY = 0;
        work->particles[23].variant = 0;
        work->particles[21].x = 0;
        work->particles[21].y = 0;
        work->particles[21].velocityX = -toFIXED(1);
        work->particles[21].velocityY = 0;
        work->particles[21].variant = 0;
        work->particles[22].x = toFIXED(160);
        work->particles[22].y = 0;
        work->particles[22].velocityX = 0;
        work->particles[22].velocityY = 0;
        work->particles[22].variant = 0;
        for (i = 0; i != 8; i++) {
          struct Particle *p;

          p = &work->particles[32 + i];
          p->x = INTtoFIXED((i / 3) * 4) + toFIXED(176);
          p->y = INTtoFIXED((i % 3) * 3 * 2) + toFIXED(74);
          p->velocityX = 0;
          p->velocityY = 0;
          p->variant = 0;
        }
        for (i = 0; i != 8; i++) {
          struct Particle *p;

          p = &work->particles[40 + i];
          p->x = INTtoFIXED((i / 3) * 4) + toFIXED(112);
          p->y = INTtoFIXED((i % 3) * 3 * 2) + toFIXED(82);
          p->velocityX = 0;
          p->velocityY = 0;
          p->variant = 0;
        }
      }
      prevCamera = camera;
      if (frame >= 542 && frame < 558) {
        s32 r, g, b;

        r = RGB_RED(*pal);
        g = RGB_GREEN(*pal);
        b = RGB_BLUE(*pal);
        if (r < RGB_LEVEL_MAX)
          r++;
        if (g < 20)
          g++;
        if (b < 6)
          b++;
        *pal = RGB(r, g, b);
      }
      if (frame > 557) {
        s32 r, g, b;

        r = RGB_RED(*pal);
        g = RGB_GREEN(*pal);
        b = RGB_BLUE(*pal);
        if (r > 20)
          r--;
        if (g > 0)
          g--;
        if (b > 0)
          b--;
        *pal = RGB(r, g, b);
      }
      if (frame == 280) {
        Iwram_CopyWords(
            (void *)BG_PLTT,
            (const void *)Resource_GetTableEntry(RESOURCE_SMOKE_SHEET),
            SHEET_PALETTE_BYTES);
        work->particles[20].variant = 1;
      }
      if (frame == 288)
        work->particles[23].variant = 1;
      if (frame == 279) {
        REG_BG2PA = Q_8_8(0.5);
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        REG_BG2PA = Q_8_8(0.5);
      }
      if (frame == 506)
        work->particles[21].variant = 1;
      if (frame == 489) {
        REG_BG2PA = Q_8_8(0.5);
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        work->scrollStepX = 0;
      }
      if (frame == 528) {
        REG_BG2PA = Q_8_8(0.5);
        REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                     BGCNT_SCREEN_BASE(7);
      }
      if (frame == 0) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        for (i = 0; i != DAEDALUS_BIRD_COUNT; i++) {
          struct Particle *p;

          p = &DAEDALUS_PARTICLES[DAEDALUS_BIRDS + i];
          p->x = INTtoFIXED(gDaedalus_DriftStarts[i][0]);
          p->y = INTtoFIXED(gDaedalus_DriftStarts[i][1]);
          p->velocityX = gDaedalus_DriftSpeeds[i][0];
          p->velocityY = gDaedalus_DriftSpeeds[i][1];
          p->variant = Random16() & 127;
        }
      }
      for (i = 0; i != DAEDALUS_BIRD_COUNT; i++) {
        struct Particle *p;

        p = &DAEDALUS_PARTICLES[DAEDALUS_BIRDS + i];
        if (frame >= gDaedalus_DriftFrames[i]) {
          ((BlitFunc)blit[0])(
              dest,
              &work->sheet[(gDaedalus_DriftPictures[(p->variant / 4) % 12]
                            << 8) +
                           DAEDALUS_BIRD_FLOCK],
              FIXEDtoINT(p->x), FIXEDtoINT(p->y), 16, 16);
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->variant++;
        }
      }
      if (frame <= 473)
        waveScroll += 0x8000;
      waveX = waveScroll + camera;
      if (waveX >= toFIXED(32)) {
        wavePhase = (wavePhase + 7) % 8;
        waveX -= toFIXED(32);
        waveScroll -= 0x200000;
      }
      if (shipY < 0)
        shipY += toFIXED(1) / 8;
      shipScale = gDaedalus_ShipScale;
      bobX = 0;
      bobY = 0;
      if (frame > 537) {
        swayX = Trig_Cos(0x21a00) << 2;
        swayY = Trig_Sin(0x21a00) << 2;
        if (frame >= 542 && frame < 558) {
          s32 v;

          v = (frame << 11) - 0x10f000;
          bobX = Trig_Sin(v) * 3;
          bobY = Trig_Sin(v) * 2;
        }
        if (frame >= 560 && frame < 576) {
          s32 v;

          v = (frame << 11) - 0x118000;
          bobX = Trig_Sin(v) * 2;
          bobY = Trig_Sin(v) * 3;
        }
      } else {
        swayX = Trig_Cos(frame << 8) << 2;
        swayY = Trig_Sin(frame << 8) << 2;
      }
      if (swayY > 0)
        swayY = -swayY;
      swayY -= shipY * 2;
      shipPlace.groundHeight = 0;
      shipPlace.height = toFIXED(255);
      for (i = 0; i != 5; i++) {
        if (frame == gDaedalus_ClatterFrames[i]) {
          work->shakeFrames = 23;
          Sound_PlayFar(SE_CLATTER);
        }
      }
      if (frame == 510)
        Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
      if (frame == 568) {
        work->shakeFrames = 10;
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 578)
        Sound_PlayFar(SE_WAVE_HIGH);
      for (i = 0; i != 16; i++) {
        struct Particle *p;

        p = &work->particles[32 + i];
        if (p->variant != 0) {
          struct Particle *q;

          p->variant++;
          if (p->x > -toFIXED(16)) {
            p->x += p->velocityX;
            p->y += p->velocityY;
            if (p->velocityY > -toFIXED(1) && p->velocityY < toFIXED(1))
              p->velocityY += p->velocityZ;
          }
          if (p->velocityX > -toFIXED(8))
            p->velocityX -= toFIXED(1) / 2;
          q = &DAEDALUS_PARTICLES[i * DAEDALUS_TRAIL_LENGTH +
                                  (p->variant & 31)];
          q->variant = 0;
          q->x = p->x / 2;
          q->y = p->y - toFIXED(20);
          q->velocityX = 0;
          q->velocityY = 0;
        }
        if (work->particles[20].variant == (i & 7) * 6 + 10) {
          Sound_PlayFar(SE_ITEM_BREAK);
          p->variant = 1;
          p->x += swayX;
          p->y += swayY - INTtoFIXED(gDaedalus_ShakeOffsets[work->shakeFrames]);
          p->velocityY = (Random16() & TRIG_ANGLE_MASK) - 0x8000;
          p->velocityZ = (Random16() & 0x3fff) - 0x2000;
        }
      }
      if (frame <= 489) {
        for (i = 8; i != 16; i++) {
          struct Particle *p;

          p = &work->particles[32 + i];
          shipPlace.x = p->x + camera;
          shipPlace.y = p->y;
          if (p->variant != 0)
            Render_ApplyProjectedPlacementFar(work->sprites[40 + i], &shipPlace,
                                              &shipScale, 0);
        }
      }
      if (frame == 578) {
        work->scrollInterval = 1;
        work->scrollTimer = 0;
      }
      if (frame >= 578 && frame < 594)
        work->scrollStepX = ~((frame - 578) / 8);
      {
        for (i = 0; i != 2; i++) {
          shipPlace.x = camera - swayX + bobX + toFIXED(120);
          shipPlace.y =
              INTtoFIXED((i << 5) - gDaedalus_ShakeOffsets[work->shakeFrames]) +
              swayY + bobY + toFIXED(100);
          Render_ApplyProjectedPlacementFar(work->sprites[32 + i], &shipPlace,
                                            &shipScale, 0);
        }
      }
      if (frame <= 316) {
        struct Particle *p;

        p = &work->particles[23];
        if (p->variant != 0) {
          p->variant++;
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->velocityY += toFIXED(1) / 8;
          p->velocityX = p->velocityX * 48 / DAMPING_SCALE;
          p->velocityY = p->velocityY * 63 / DAMPING_SCALE;
          DAEDALUS_SPRITE(DAEDALUS_BLADE)->rotation += 0x200;
        }
        shipPlace.x = p->x - swayX + camera + toFIXED(120);
        shipPlace.y =
            INTtoFIXED(99 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY +
            p->y;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_BLADE],
                                          &shipPlace, &shipScale, 0);
      }
      {
        struct Particle *p;
        s32 v;

        v = camera + bobX;
        shipPlace.x = v + toFIXED(142);
        shipPlace.y =
            INTtoFIXED(72 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY +
            bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[18], &shipPlace,
                                          &shipScale, 0);
        shipPlace.x = v + toFIXED(174);
        shipPlace.y =
            INTtoFIXED(72 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY +
            bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[19], &shipPlace,
                                          &shipScale, 0);
        v += 0x800000;
        shipPlace.x = v;
        shipPlace.y =
            INTtoFIXED(104 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
            swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[20], &shipPlace,
                                          &shipScale, 0);
        p = &work->particles[22];
        if (frame > 602)
          BattleEffect_StepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
        if (frame == 570) {
          p->x = toFIXED(152);
          p->y = INTtoFIXED(92 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                 swayY;
          p->variant = 1;
          p->velocityX = -toFIXED(4);
          p->velocityY = -toFIXED(5) / 16;
          for (i = 0; i != 16; i++) {
            struct Particle *q;

            q = &DAEDALUS_PARTICLES[312 + i];
            q->x = INTtoFIXED((Random16() & 15) + 56);
            q->y = INTtoFIXED((Random16() & 31) + 64);
            q->velocityX = -((Random16() & 31) + 8) << 12;
            q->velocityY = -((Random16() & 31) + 8) << 12;
            q->variant = 0;
          }
          for (i = 0; i != DAEDALUS_EXHAUST_FLAMES; i++)
            DAEDALUS_PARTICLES[DAEDALUS_EXHAUST + i].variant = -1;
          for (i = 0; i != DAEDALUS_BURST_COUNT; i++)
            DAEDALUS_PARTICLES[DAEDALUS_BURSTS + i].variant = -1;
        }
        if (p->variant != 0) {
          if (p->variant > 9) {
            struct Particle *q;

            q = &DAEDALUS_PARTICLES[DAEDALUS_EXHAUST + (p->variant & 31)];
            q->variant = 0;
            q->velocityX =
                -(Trig_Sin(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) * 2);
            q->velocityY =
                Trig_Cos(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) * 2;
            q->x =
                (p->x + camera) / 2 -
                ((Trig_Sin(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) * 27) >>
                 1);
            q->y = p->y +
                   Trig_Cos(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) * 27 -
                   toFIXED(12);
          }
          if (((p->variant >= 50 && p->variant < 60) ||
               (p->variant >= 113 && p->variant < 200)) &&
              (p->variant & 3) == 0) {
            struct Particle *q;

            q = &DAEDALUS_PARTICLES[DAEDALUS_BURSTS + (p->variant & 31)];
            q->variant = 0;
            q->velocityX = ((Random16() & 31) + 16) << 12;
            q->velocityY = (-Random16() | ~15) << 14;
            q->x = toFIXED(88);
            q->y = toFIXED(128);
          }
          prevCamera = camera;
          {
            struct Particle *f;

            f = &work->particles[22];
            if (f->variant > 229)
              camera += (toFIXED(152) - (f->x + ((f->variant - 230) << 17)) -
                         camera) /
                        4;
            else if (f->variant > 0)
              camera += (toFIXED(152) - f->x - camera) / 4;
          }
          if (p->variant > 1) {
            p->x += p->velocityX;
            p->y += p->velocityY;
          }
          if (p->variant > 7) {
            p->velocityX = p->velocityX * 63 / DAMPING_SCALE;
            p->velocityY = p->velocityY * 63 / DAMPING_SCALE + 0x8c8;
          }
          if (p->variant < 80)
            p->velocityY += 0x200;
          if (p->variant > 29)
            p->velocityX -= toFIXED(1) / 4;
          if (p->variant >= 40 && p->variant < 68)
            p->velocityY -= toFIXED(1) / 16;
          if (p->variant >= 50 && p->variant < 60) {
            DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation =
                (p->variant << 7) - 0x5900;
            p->velocityY -= toFIXED(1) / 16;
          }
          if (p->variant >= 108 && p->variant < 140) {
            if (p->variant >= 120 && p->variant < 123)
              DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation -= 0x180;
            else
              DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation += 0x100;
            p->velocityY -= toFIXED(1) / 16;
          }
          if (p->variant >= 140 && p->variant < 160)
            p->velocityY -= toFIXED(1) / 32;
          if (p->variant > 159)
            p->velocityY -= toFIXED(1) / 8;
          for (i = 0; i != DAEDALUS_EXHAUST_FLAMES; i++) {
            struct Particle *q;

            q = &DAEDALUS_PARTICLES[DAEDALUS_EXHAUST + i];
            if ((u32)q->variant <= 5) {
              ((BlitFunc)blits[1])(
                  dest, &work->sheet[q->variant * PICTURE_BYTES(ROCKET_FLAME)],
                  (FIXEDtoINT(q->x)) - 2, (FIXEDtoINT(q->y)) - 12,
                  ROCKET_FLAME_WIDTH, ROCKET_FLAME_HEIGHT);
              BattleEffectKernels_IntegrateVector2(q, PARTICLE_DAMPING_NONE, 0);
              q->variant++;
            }
          }
          for (i = 0; i != DAEDALUS_BURST_COUNT; i++) {
            struct Particle *q;

            q = &DAEDALUS_PARTICLES[DAEDALUS_BURSTS + i];
            if ((u32)q->variant <= 23) {
              ((BlitFunc)blits[1])(
                  dest,
                  &work->sheet[q->variant / 4 * PICTURE_BYTES(FLASH_BURST) +
                               DAEDALUS_FLASH_BURST_PICTURES],
                  (FIXEDtoINT(q->x)) - 16, (FIXEDtoINT(q->y)) - 32,
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
              q->velocityX += toFIXED(1) / 16;
              BattleEffectKernels_IntegrateVector2(q, PARTICLE_DAMPING_NONE, 0);
              q->variant++;
            }
          }
          p->variant++;
        }
        shipScale.x = 0x103ff;
        shipScale.y = 0x103ff;
        if (p->variant == 0) {
          shipPlace.x = camera + bobX + toFIXED(152);
          shipPlace.y =
              INTtoFIXED(92 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
              swayY + bobY;
        } else {
          shipPlace.x = p->x + camera;
          shipPlace.y = p->y;
        }
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_ROCKET],
                                          &shipPlace, &shipScale, 0);
      }
      DAEDALUS_SPRITE(DAEDALUS_ROCKET)->offsetX = 0;
      DAEDALUS_SPRITE(DAEDALUS_ROCKET)->offsetY = 32;
      if (frame >= 568) {
        s32 *beam;
        void *buffer;
        s32 v;
        s32 z;

        buffer = Heap_AllocEwram(512);
        beam = Render_AllocateModelList(1);
        bitmap.logWidth = 7;
        bitmap.logHeight = 7;
        bitmap.pixels = pixels;
        beam[4] = (s32)&bitmap;
        beam[1] = 0;
        beam[2] = (s32)gBattleEffect_LargeSquareMesh;
        beam[0] = 7;
        beam[3] = (s32)buffer;
        v = frame - 568;
        z = 96 - (v << 4);
        if (z > 0)
          z = 0;
        if (z >= -63) {
          beam[5] = z;
          Matrix_Initialize();
          Matrix_Translate(camera + toFIXED(2), toFIXED(8), 0);
          Matrix_RotateZ(TRIG_TURN * 67 / 256);
          Matrix_RotateX(-TRIG_TURN / 32);
          Matrix_ScaleUniform((v << 14) + 0x4180);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, buffer,
                                   4);
          Render_DrawModelList(beam);
        }
        Heap_Release(beam);
        Heap_Release(buffer);
      }
      if (frame == 568) {
        work->particles[30].x = 0;
        work->particles[30].velocityX = toFIXED(8);
      }
      for (j = 0; j != 1; j++) {
        ringX = &work->particles[30].x;
        if (frame >= gDaedalus_RingStarts[j] &&
            frame < gDaedalus_RingStarts[j] + 26) {
          s32 n;

          radius = FIXEDtoINT(*ringX);
          n = (frame - gDaedalus_RingStarts[j]) / 2;
          *ringX += work->particles[30].velocityX;
          work->particles[30].velocityX =
              work->particles[30].velocityX * 48 / DAMPING_SCALE;
          if (n <= 5) {
            for (i = 0; i != 32; i++) {
              s32 v;

              v = (FIXEDtoINT(Trig_Sin(i << 11) * radius / 2)) +
                  gDaedalus_RingX[j];
              ((BlitFunc)blit[0])(
                  dest, &work->sheet[n * 288], v - 6,
                  (FIXEDtoINT(Trig_Cos(i << 11) * radius * 2)) + 60, 12, 24);
            }
          }
        }
      }
      shipScale.x = FIXED_ONE;
      shipScale.y = FIXED_ONE;
      DAEDALUS_SPRITE(21)->priority = 2;
      DAEDALUS_SPRITE(22)->priority = 2;
      DAEDALUS_SPRITE(23)->priority = 2;
      DAEDALUS_SPRITE(24)->priority = 2;
      {
        s32 v;

        v = prevCamera + bobX;
        shipPlace.x = v + toFIXED(160);
        shipPlace.y =
            INTtoFIXED(104 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
            swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[21], &shipPlace,
                                          &shipScale, 0);
        shipPlace.x = v + toFIXED(128);
        shipPlace.y =
            INTtoFIXED(136 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
            swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[22], &shipPlace,
                                          &shipScale, 0);
        shipPlace.x = v + toFIXED(160);
        shipPlace.y =
            INTtoFIXED(136 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
            swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[23], &shipPlace,
                                          &shipScale, 0);
        shipPlace.x = v + toFIXED(192);
        shipPlace.y =
            INTtoFIXED(136 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
            swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[24], &shipPlace,
                                          &shipScale, 0);
        DAEDALUS_SPRITE(25)->priority = 2;
        shipPlace.x = v + toFIXED(128);
        shipPlace.y =
            INTtoFIXED(104 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
            swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[25], &shipPlace,
                                          &shipScale, 0);
      }
      for (i = 0; i != 5; i++) {
        shipPlace.x = INTtoFIXED(gDaedalus_SpritePoints[i][0]) + swayX +
                      prevCamera + bobX + toFIXED(176);
        shipPlace.y = ((gDaedalus_SpritePoints[i][1] -
                        gDaedalus_ShakeOffsets[work->shakeFrames])
                       << 16) +
                      swayY + bobY + toFIXED(84);
        DAEDALUS_SPRITE(26 + i)->priority = 2;
        Render_ApplyProjectedPlacementFar(work->sprites[26 + i], &shipPlace,
                                          &shipScale, 0);
      }
      if (frame <= 489) {
        for (i = 0; i != 8; i++) {
          struct Particle *p;

          p = &work->particles[32 + i];
          shipPlace.x = p->x + camera;
          shipPlace.y = p->y;
          if (p->variant != 0)
            Render_ApplyProjectedPlacementFar(
                work->sprites[DAEDALUS_MISSILE + i], &shipPlace, &shipScale, 0);
        }
      }
      if (frame < 540) {
        struct Particle *p;

        p = &work->particles[21];
        if (p->variant == 1) {
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->velocityY += toFIXED(1) / 8;
          p->velocityX = p->velocityX * 60 / DAMPING_SCALE;
          p->velocityY = p->velocityY * 63 / DAMPING_SCALE;
        }
        for (i = 0; i != 4; i++) {
          shipPlace.x = INTtoFIXED((i & 1) * 32) + p->x + camera + toFIXED(128);
          shipPlace.y =
              ((((i / 2) << 5) - gDaedalus_ShakeOffsets[work->shakeFrames])
               << 16) +
              swayY + p->y + toFIXED(104);
          DAEDALUS_SPRITE(34 + i)->priority = 1;
          Render_ApplyProjectedPlacementFar(work->sprites[34 + i], &shipPlace,
                                            &shipScale, 0);
        }
      }
      if (frame <= 316) {
        struct Particle *p;

        p = &work->particles[20];
        if (p->variant != 0) {
          p->variant++;
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->velocityY += toFIXED(1) / 8;
          p->velocityX = p->velocityX * 48 / DAMPING_SCALE;
          p->velocityY = p->velocityY * 63 / DAMPING_SCALE;
          DAEDALUS_SPRITE(31)->rotation += 0x100;
        }
        shipPlace.x = swayX + p->x + camera + toFIXED(184);
        shipPlace.y =
            INTtoFIXED(92 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY +
            p->y;
        DAEDALUS_SPRITE(31)->priority = 1;
        Render_ApplyProjectedPlacementFar(work->sprites[31], &shipPlace,
                                          &shipScale, 0);
      }
      {
        struct SpritePlacement *wave;
        s32 row;
        Sprite **slot;

        for (j = 0; j != 2; j++) {
          for (i = 0; i != 9; i++) {
            Sprite *obj;
            s32 v;

            wave = &shipPlace;
            wave->x = INTtoFIXED(i * 32) + waveX;
            wave->y = INTtoFIXED((j << 5) -
                                 gDaedalus_ShakeOffsets[work->shakeFrames]) +
                      shipY + toFIXED(132);
            if (i == 8) {
              DAEDALUS_SPRITE(j * DAEDALUS_CANOPY_ROW + i)->tile =
                  work->particles[j * 9 + (wavePhase + 71) % 8].variant & 0x3ff;
            } else {
              row = j * 9;
              k = row;
              k += i;
              DAEDALUS_SPRITE(k)->tile =
                  work->particles[row + (wavePhase + 64 + i) % 8].variant &
                  0x3ff;
            }
            if (shipPlace.x > 0xffffff)
              shipPlace.x -= toFIXED(288);
            Render_ApplyProjectedPlacementFar(
                work->sprites[j * DAEDALUS_CANOPY_ROW + i], &shipPlace,
                &shipScale, 0);
          }
        }
      }
      for (j = 0; j != 4; j++) {
        for (i = 0; i != 6; i++) {
          if (frame >= i * 2 + 490 && frame < i * 2 + 502) {
            s32 x, y, n;

            x = gDaedalus_SplashPoints[j][i][0];
            n = (frame - (i * 2 + 490)) / 2;
            y = gDaedalus_SplashPoints[j][i][1] + (FIXEDtoINT(swayY)) -
                gDaedalus_ShakeOffsets[work->shakeFrames];
            ((BlitFunc)blit[0])(dest, &work->sheet[n * 288], x - 6, y - 12, 12,
                                24);
          }
        }
      }
      for (i = 0; i != 16; i++) {
        for (j = 0; j != 32; j++) {
          struct Particle *q;

          q = &DAEDALUS_PARTICLES[i * 32 + j];
          if ((u32)q->variant <= 5) {
            ((BlitFunc)blit[0])(dest, &work->sheet[q->variant * 288],
                                (FIXEDtoINT(q->x)) - 2, (FIXEDtoINT(q->y)) - 12,
                                ROCKET_FLAME_WIDTH, ROCKET_FLAME_HEIGHT);
            q->x += q->velocityX;
            q->y += q->velocityY;
            q->velocityY -= toFIXED(1) / 4;
            q->variant++;
          }
        }
      }
      {
        s32 *shake;
        s32 v;
        struct BgScroll *bg;
        s32 *params;

        shake = &work->shakeFrames;
        bg = gBgScroll;
        v = gDaedalus_ShakeOffsets[*shake];
        bg[1].y = v + 32;
        gProjection.centerY = BATTLE_PROJECTION_CENTER_Y - v;
        if (*shake > 0)
          *shake -= 1;
      }
      EFFECT_END_FRAME(work);
      frame++;
      if (frame != 800 && !(gInput.held & KEY_B) && !(gInput.repeat & KEY_A))
        goto secondPhase;
    }

    Task_Remove(BattleEffect_AdvanceScrollOnInterval);
    gBgScroll[1].x = savedY;
    ctl[4] = 0;
    BattleEffect_SetupBlendedDisplay();
    for (i = 0; i != 57; i++)
      Sprite_DestroyFar(work->sprites[i]);
    bank[21] = VramCache_AllocEntry(128);
    Palette_LoadBasicSpriteColors();
    Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
            DMA_COPY_HALFWORDS(16), REG_DMA3);
    BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    REG_BG2X = 0;
    REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    REG_BG2PA = Q_8_8(0.5);
    REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    args->targets[args->targetCount] = 0xff;
    BattleActor_SpawnObjectsForListFar(args->targets, 0);
    BattleEffect_SpawnObjects(1, 663, 1);
    for (i = 0; i != 15; i++) {
      work->sprites[1 + i] =
          EffectSprite_Acquire(32, 16, OBJ_SIZE_32X16 | OBJ_256_COLORS, 0);
      IWRAM_COPY_WORDS_LINKED(work->sprites[1 + i], work->sprites[0], 24);
      DAEDALUS_SPRITE(1 + i)->offsetX = 32;
      DAEDALUS_SPRITE(1 + i)->offsetY = 8;
    }
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_ROCKET_FLAME_SHEET,
                               work->sheet + DAEDALUS_ROCKET_FLAMES, 0, 0);
    {
      const void *colors =
          (const void *)Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B);
      CopyFunc copy = IWRAM_COPY_WORDS_LINKED;

      copy((void *)BG_PLTT, colors, SHEET_PALETTE_BYTES);
    }

    for (frame = 0; frame != 109; frame++) {
      if (frame == 0) {
        s32 z;
        struct Particle *p;

        i = 0;
        radius = 0;
        for (; i != 16; i++) {
          /* FAKEMATCH(dead): k = 0 under a test that is never true, a store
           * nothing reads; without it, 90 of 3921 instructions differ in
           * tla-en. */
          if (work == NULL)
            k = 0;
          p = &work->particles[i];
          z = i * -0x100;
          p->variant = radius;
          Random16();
          p->velocityZ = radius;
          /* FAKEMATCH(volatile): velocityZ read back through a volatile view;
           * as p->velocityZ, 167 of 3921 instructions differ in tla-en. */
          p->z = z - (*(volatile s32 *)&p->velocityZ * 40) - 0x800;
          p->velocityX = -Trig_Cos(p->z) * 8;
          p->velocityY = -Trig_Sin(p->z) * 8;
          p->x = INTtoFIXED(Random16() % 80 + 80) - p->velocityX * 24;
          p->y = INTtoFIXED((Random16() & 7) + 96) - p->velocityY * 24;
          DAEDALUS_SPRITE(i)->rotation = p->z;
          DAEDALUS_SPRITE(i)->offsetX = radius;
          DAEDALUS_SPRITE(i)->offsetY = 8;
        }
        for (i = 0; i != 512; i++)
          DAEDALUS_PARTICLES[i].variant = -1;
      }
      particleScale = gDaedalus_ParticleScale;
      particlePlace.groundHeight = 0;
      particlePlace.height = toFIXED(255);
      for (i = 0; i != 16; i++) {
        struct Particle *p;

        p = &work->particles[i];
        if (frame >= gDaedalus_ParticleStarts[i] && p->variant == 0) {
          particlePlace.x = p->x;
          particlePlace.y = p->y;
          if (particlePlace.y > 0 && particlePlace.y < toFIXED(128))
            Render_ApplyProjectedPlacementFar(work->sprites[i], &particlePlace,
                                              &particleScale, 0);
          if (p->y > 0x6fffff) {
            p->variant = 1;
            for (j = 0; j != 2; j++) {
              s32 speed;
              s32 angle;
              struct Particle *q;

              q = &DAEDALUS_PARTICLES[i * 8 + j];
              speed = (Random16() & 31) + 32;
              angle = (Random16() & 0x7fff) + DEGtoANG(90);
              q->x = p->x / 2;
              q->y = p->y;
              q->velocityX = Trig_Sin(angle) * speed >> 4;
              q->velocityY = Trig_Cos(angle) * speed >> 4;
              q->variant = 0;
            }
            Sound_PlayFar(SE_CLATTER);
            for (j = 0; j != args->targetCount; j++) {
              BattleEffect_StartActorMotion(args->targets[j], 1, 0, 0,
                                            toFIXED(2), 100);
              BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, j, 7);
              work->shakeFrames = 4;
            }
          } else {
            struct Particle *q;

            {
              s32 m = (frame - gDaedalus_ParticleStarts[i]) % 16;
              q = &DAEDALUS_PARTICLES[(i << 4) + m + 256];
            }
            q->x = p->x / 2;
            q->y = p->y - toFIXED(16);
            q->velocityX = 0;
            q->velocityY = 0;
            q->variant = 0;
          }
          DAEDALUS_SPRITE(i)->rotation = p->z;
          BattleEffectKernels_IntegrateVector3(p, PARTICLE_DAMPING_NONE, 0);
          p->velocityX = -Trig_Cos(DAEDALUS_SPRITE(i)->rotation) * 8;
          p->velocityY = -Trig_Sin(DAEDALUS_SPRITE(i)->rotation) * 8;
          DAEDALUS_SPRITE(i)->tile = DAEDALUS_SPRITE(0)->tile;
        }
      }
      for (i = 0; i != 256; i++) {
        struct Particle *q;

        q = &DAEDALUS_PARTICLES[256 + i];
        if ((u32)q->variant <= 7) {
          s32 v;

          v = q->variant - 2;
          if (v >= 0)
            ((BlitFunc)blits[1])(dest,
                                 &work->sheet[v * PICTURE_BYTES(ROCKET_FLAME) +
                                              DAEDALUS_ROCKET_FLAMES],
                                 (FIXEDtoINT(q->x)) - 6,
                                 (FIXEDtoINT(q->y)) - 12, 12, 24);
          q->variant++;
        }
      }
      for (i = 0; i != 128; i++) {
        struct Particle *q;

        q = &DAEDALUS_PARTICLES[i];
        if ((u32)q->variant <= 17) {
          s32 v;

          v = q->variant / 3;
          ((BlitFunc)blits[i & 1])(dest, &work->sheet[v << 11],
                                   (FIXEDtoINT(q->x)) - 16,
                                   (FIXEDtoINT(q->y)) - 48, 32, 64);
          BattleEffectKernels_IntegrateVector2(q, 56, -toFIXED(1) / 8);
          q->variant++;
        }
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
    for (i = 0; i != 16; i++)
      Sprite_DestroyFar(work->sprites[i]);
    EFFECT_FINISH_CANVAS();
  }
}

#undef BLITTER

/// The summon's placement source.
extern const struct Scale gDaedalusAlt_SummonScale;
/// The sparks, after the map cells.
#define STRIKE_SPARKS ((struct Particle *)(Ram_MapCellBuffer + 0x4000))

#define BLITTER ((DrawRectangle)Ram_HeapSlots->ramCode)
#define DAEDALUS_FRAMES 108
#define DAEDALUS_SPARKS 64

/// The summoned object's display fields that the scene sets: a struct Sprite
/// from its rotation on.
struct SummonObject {
  u8 imageScaleAndSlot[18]; ///< struct Sprite's fields before the rotation
  u16 rotation;
  u8 size[2];
  u8 offsetX;
  u8 offsetY;
};

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define DAEDALUS_ALT_GLOW_BAND 0x4000

/// The summoned object glides in from the right and down over 92 frames
/// while, from frame 56, a shaded disc swells at the centre and six rings of
/// light spin out from it. At the seven frames of gDaedalusAlt_StrikeFrames
/// every target is knocked back and flashes; from frame 80 eight bursts of
/// eight sparks, one a frame, scatter over the canvas.
void BattleEffect_RunDaedalusAlt(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  s32 request[4];
  struct Scale source;
  s32 frame;
  s32 i;
  s32 j;
  FIXED centerX;
  FIXED centerY;
  s32 burst;
  struct Particle *spark;
  s32 mask;
  FIXED discX;
  s32 ringOffset;
  FIXED ringX;
  FIXED discY;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  BattleEffect_ConfigureEffectDisplay();
  REG_WIN0V = WIN_RANGE(16, 128);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_SpawnObjects(1, 0x296, 3);
  BattleEffect_SelectLivingTargets(work->args);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = BLITTER;
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;

  frame = 0;
  burst = -80 * 8;
  do {
    if (frame == 0) {
      discX = toFIXED(64);
      ringOffset = -0x400000;
      ringX = toFIXED(66);
      discY = toFIXED(80);
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                           SHEET_PALETTE_BYTES);
    }
    if (frame == 56) {
      Sound_PlayFar(SE_SPIN);
    }
    for (i = 0; i != 7; i++) {
      if (frame == gDaedalusAlt_StrikeFrames[i]) {
        for (j = 0; j != args->targetCount; j++) {
          BattleEffect_StartActorMotion(args->targets[j], 1, toFIXED(2),
                                        toFIXED(8), toFIXED(2), 120);
          BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 7);
        }
        work->shakeFrames = 4;
        Sound_PlayFar(SE_CRASH);
      }
    }

    // From frame 80: eight bursts of sparks, one a frame.
    if (frame > 79) {
      // One pointer to the sparks for the burst and the draw loop below.
      struct Particle *sparks;

      if (frame == 80) {
        // UB: the lives are cleared through one s32 pointer stepped a whole
        // step at a time, past the step's scalar members.
        s32 *life = &STRIKE_SPARKS[0].variant;

        for (i = 0; i != DAEDALUS_SPARKS; i++) {
          *life = -1;
          life += sizeof(struct Particle) / sizeof(s32);
        }
        work->shakeFrames = 8;
        Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      sparks = STRIKE_SPARKS;
      if ((u32)(frame - 80) <= 7) {
        centerX = INTtoFIXED((Random16() & 63) + 28);
        centerY = INTtoFIXED((Random16() & 31) + 80);

        j = 0;
        mask = 15;
        spark = sparks + burst;
        for (; j != 8; j++, spark++) {
          s32 speed;
          s32 angle;
          speed = 31;
          speed &= Random16();
          angle = Random16();

          spark->x = centerX + INTtoFIXED((Random16() & mask) - 8);
          spark->y = centerY + INTtoFIXED((Random16() & mask) - 8);
          speed += 32;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 3;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 3;
          spark->variant = 0;
        }
      }
      {
        struct Particle *mote;

        for (i = 0, mote = sparks; i != DAEDALUS_SPARKS; i++, mote++) {
          if ((u32)mote->variant <= 17) {
            draw[0](canvas, work->sheet + ((mote->variant / 3) << 11),
                    FIXED_INTEGER_PART(mote->x) - 16,
                    FIXED_INTEGER_PART(mote->y) - 48, FLASH_BURST_WIDTH,
                    FLASH_BURST_HEIGHT);
            BattleEffectKernels_IntegrateVector2(mote, 62, -toFIXED(1) / 32);
            mote->variant++;
          }
        }
      }
    }

    // The summoned object glides in over 92 frames.
    if (frame <= 91) {
      struct SummonObject *object;

      source = gDaedalusAlt_SummonScale;
      object = work->sprites[0];
      object->offsetY = 32;
      object->offsetX = 0;
      request[3] = 0;
      object->rotation = 0x8800;
      request[1] = 0xff0000;
      if (frame <= 63) {
        request[0] = INTtoFIXED(152 - frame / 4);
        request[2] = (frame << 17) - 0x400000;
      } else if ((u32)(frame - 64) <= 63) {
        s32 t = frame - 64;

        request[0] = INTtoFIXED(136 - t / 16);
        request[2] = INTtoFIXED(t / 2) + toFIXED(64);
      } else {
        request[0] = toFIXED(136);
        request[2] = 0x400000;
      }
      Render_ApplyProjectedPlacementFar(work->sprites[0], request, &source.x,
                                        0);
    }

    // Frames 56 to 91: the disc and the rings.
    if ((u32)(frame - 56) <= 35) {
      s32 size;
      s32 grow = frame * 2 - 104;
      struct RenderModel *model;
      void *vertices;
      struct Particle *ring;
      u8 *cells;

      /* FAKEMATCH(temporary): the disc's x copied into the loop counter i. */
      i = discX;
      size = 128;
      if (grow <= 64) {
        size = grow * 2;
        cells = gMapGridCellBuffer;
        BattleEffect_DrawShadedCircles(cells, size, FIXED_ONE);
      }
      cells = gMapGridCellBuffer;
      Canvas_BlitAtFoot(cells, FIXEDtoINT(i), FIXEDtoINT(discY), size);
      if (frame == 56) {
        s32 delay;

        i = 0;
        delay = 0;
        for (ring = work->particles; i != 16; i++, ring++) {
          ring->x = Random16() & TRIG_ANGLE_MASK;
          ring->y = (Random16() & 255) + 128;
          if (Random16() & 1) {
            ring->y = -ring->y;
          }
          ring->variant = delay;
          delay -= 4;
        }
        Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                                   work->sheet + DAEDALUS_ALT_GLOW_BAND, 0, 0);
      }
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      model->shade = 0;
      texture.logWidth = 5;
      texture.logHeight = 2;
      model->vertices = vertices;
      texture.pixels = work->sheet + DAEDALUS_ALT_GLOW_BAND;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &texture;
      model->mesh = gBattleEffect_SmallSquareMesh;

      for (i = 0; i != 6; i++) {
        ring = &work->particles[i];
        ring->variant += 2;
        ring->x += ring->y * 2;
        if ((u32)ring->variant <= 47) {
          s32 k;

          Matrix_Initialize();
          k = i & 3;
          Matrix_Translate(INTtoFIXED(k * 2) + ringX + ringOffset,
                           discY - INTtoFIXED(k + 2) + ringOffset, 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
          Matrix_RotateZ(ring->x);
          Matrix_Scale(Trig_Sin(ring->variant * 682) + TRIG_TURN, 0x20000,
                       0x10000);
          Render_TransformVertices(gBattleEffect_TaperedBeamCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
    burst += 8;
    frame++;
  } while (frame != DAEDALUS_FRAMES);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Sprite_DestroyFar(work->sprites[0]);
  BattleEffect_EndCanvasLayer();
}
#undef BLITTER
#undef BURST_FRAMES
#undef BURST_DRAW_FRAMES
#undef CEL_SIZE
#undef PICTURE_OFFSET
#undef MIRROR_HALF
#undef SYSTEM
#undef IWRAM_COPY_WORDS_LINKED
#undef DECODE_BUFFER
#undef DAEDALUS_FRAMES
#undef DAEDALUS_SPARKS
