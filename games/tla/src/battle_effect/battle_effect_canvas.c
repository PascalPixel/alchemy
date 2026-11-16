/// @file
/// Eclipse, then the canvas layer: transfers, opening and closing, tile maps,
/// lines.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "gamepak/far_call/field.h"
#include "system/system.h"
#include "resource_ids.h"

/// The flashes, after the map cells.
#define FLASHES ((struct Particle *)gEffectScratch)

/// Where TornadoSheet is decoded in the effect's pictures.
#define ECLIPSE_TORNADO_PICTURES 0x2000

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define ECLIPSE_GLOW_BAND_A 0x4000

/// Where GlowBandTextureB is decoded in the effect's pictures.
#define ECLIPSE_GLOW_BAND_B 0x3840

/// Where FlameBandTexture is decoded in the effect's pictures.
#define ECLIPSE_FLAME_BAND 0x400

/// The glowing orb the dragon gathers: its radius in variant, drawn as shaded
/// circles into the effect's pictures.
#define ECLIPSE_ORB (work->particles[16])
/// The 120x120 ramp of the second act, brightening from left to right.
#define ECLIPSE_RAMP_SIZE 120
/// The landing's buffers in the map cell buffer: the dome's vertices (8 rings
/// of 16) and its grid mesh, the band's vertices and mesh, and 512 motes.
#define ECLIPSE_DOME_VERTICES (Ram_MapCellBuffer + 0x800)
#define ECLIPSE_DOME_MESH (Ram_MapCellBuffer + 0x1000)
#define ECLIPSE_BAND_VERTICES (Ram_MapCellBuffer + 0x2000)
#define ECLIPSE_BAND_MESH (Ram_MapCellBuffer + 0x2800)
#define ECLIPSE_MOTES ((struct Particle *)(Ram_MapCellBuffer + 0x4000))
#define ECLIPSE_MOTE_COUNT 512
#define ECLIPSE_DOME_RINGS 8
#define ECLIPSE_DOME_RING_VERTICES 16

/// Eclipse: after a flash the golden dragon (thirty-two sprites, its head one
/// of them) looms over a blue sky while shafts of light fall about it; it
/// breathes a blast of light that whites out the field, and the blast lands
/// on the land far below and spreads in rings before a last white flash.
///
/// Abilities: Eclipse.
void BattleEffect_RunEclipse(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectWork *work;
  // Unused: blit[1] gives the stack frame its 4 bytes.
  DrawRectangle blit[2];
  s32 frame;
  s32 savedScroll;
  s32 *transition;
  u8 *sprites;
  struct BattleWork *battle;
  s32 i;
  s32 j;
  s32 k;
  s32 scroll;
  FIXED baseX;
  FIXED baseY;
  s32 velocityX;
  s32 velocityY;
  FIXED cx;
  FIXED cy;
  u32 u;
  s32 count;
  s32 ring;
  s32 shade;
  s32 offset;
  u8 *cells;
  s32 fade;
  s16 list[UNIT_MAX];
  FIXED place[4];
  struct Scale req;
  struct ModelTexture texture;
  struct ModelTexture breathTexture;
  struct ModelTexture landingTexture;
  struct RenderModel *model;
  struct Particle *p;
  struct Sprite *obj;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  savedScroll = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->battleBackground;
  sprites = Ram_HeapSlots->battleSheet;
  battle = Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0);
  RagingHeat_FillGradientCanvas();
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  // The battle flashes away; at frame 27 the sky and the dragon's sprites load.
  scroll = -80;
  for (frame = 0; frame != SUMMON_OPENING_FRAMES; frame++) {
    if (frame == SUMMON_OPENING_LOAD_FRAME) {
      u8 *tiles;

      list[0] = BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForListFar(list, 0);
      VramCache_FreeEntry(battle->arrowTiles);
      ActorShadow_FreeTilesFar();
      gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_EARTH_HORIZON_BACKDROP, 0);
      work->scrollTimer = 0;
      work->scrollInterval = 1;
      work->scrollStepX = -1;
      work->scrollStepY = 0;
      Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_VBLANK);
      transition[4] = 1;
      cells = Resource_GetTableEntry(RESOURCE_GOLDEN_DRAGON_TILES);
      Iwram_CopyWords((void *)OBJ_PLTT, cells, 480);
      cells += 480;
      Decompress_TaggedLz(cells, gMapGridCellBuffer);
      tiles = gMapGridCellBuffer;
      for (i = 0; i != 31; i++) {
        obj = EffectSprite_CreateWithTiles(32, 32,
                                           OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
        obj->image.priority = 2;
        work->sprites[i] = obj;
        Iwram_CopyWords((void *)OBJ_TILES(obj), tiles, 0x400);
        tiles += 0x400;
      }
      cells = Resource_GetTableEntry(RESOURCE_GOLDEN_DRAGON_HEAD_TILES);
      Iwram_CopyWords((void *)OBJ_PLTT, cells, 480);
      cells += 480;
      Decompress_TaggedLz(cells, gMapGridCellBuffer);
      obj = EffectSprite_CreateWithTiles(32, 32,
                                         OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
      work->sprites[31] = obj;
      obj->image.priority = 2;
      Iwram_CopyWords((void *)OBJ_TILES(obj), gMapGridCellBuffer, 0x400);
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    EFFECT_PRESENT_CANVAS(work);
  }
  Iwram_FillWords(canvas, CANVAS_VIEW_BYTES, 0);
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
  REG_BG2X = 0;
  baseX = 0;
  baseY = 0;
  velocityX = 0;
  velocityY = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = Ram_HeapSlots->ramCode;
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                  SHEET_PALETTE_BYTES);
  {
    const u8 *texturePixels = (const u8 *)FLASHES;

    // The dragon looms over the horizon while shafts of light fall about it,
    // then breathes its blast; A or B cuts it short.
    for (frame = 0; frame != 220 && !(gInput.repeat & (KEY_A | KEY_B));
         frame++) {
      if (frame == 0) {
        baseX = -toFIXED(48);
        baseY = toFIXED(212);
        velocityX = toFIXED(4);
        velocityY = -toFIXED(10);
        work->fadeFrames = 1;
        work->fadeStep = frame;
        ECLIPSE_ORB.variant = frame;
        Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE, FLASHES, 0, 0);
      }
      if (frame == 136) {
        u8 *dst = Ram_MapCellBuffer + 0x400;
        for (i = 0; i != 7; i++) {

          void *o = (void *)OBJ_TILES(
              (struct Sprite *)work->sprites[gEclipse_TileObjects[i]]);
          // Default: set for real below.
          s32 size = 0;
          size = 0x400;
          Iwram_CopyWordsEntry(o, &dst[i * 0x400], size);
        }
      }
      if (frame == 48) {
        Sound_PlayFar(SE_DJINNI_SET);
      }
      if (frame == 136) {
        Sound_PlayFar(SE_SPIN);
      }
      if (frame == 172) {
        Sound_PlayFar(SE_CLATTER);
      }
      for (i = 0; i != 17; i++) {
        if (frame == gEclipse_FillFrames[i]) {
          Iwram_FillWords(canvas, CANVAS_VIEW_BYTES, gEclipse_FillLevels[i]);
        }
      }
      req = gEclipse_Scales;
      place[3] = 0;
      place[1] = toFIXED(255);
      baseX += velocityX;
      baseY += velocityY;
      if (frame > 171) {
        if (frame > 183) {
          velocityX += -0x200;
          velocityY += 0x400;
        }
        if (frame <= 205) {
          velocityX = velocityX * 54 / DAMPING_SCALE;
          velocityY = velocityY * 54 / DAMPING_SCALE;
        }
      } else {
        if (velocityY < 0) {
          velocityY += toFIXED(1) / 4;
        }
        if (velocityX > 0) {
          velocityX += -toFIXED(1) / 8;
        }
        if (frame > 31) {
          baseY += Trig_Sin((frame - 32) << 9) << 2 >> 4;
          if (frame <= 159) {
            baseX += toFIXED(1) / 4;
          }
        }
      }
      for (i = 0; i != 32; i++) {
        if (i == 25 && frame > 135) {
          place[0] = baseX + toFIXED(32);
          place[2] = baseY + toFIXED(64);
        } else {
          place[0] = baseX + INTtoFIXED(gEclipse_ObjectX[i]);
          place[2] = baseY + INTtoFIXED(gEclipse_ObjectZ[i]);
        }
        if (place[2] <= 0xc7ffff) {
          Render_ApplyProjectedPlacementFar(work->sprites[i], place,
                                            (s32 *)&req, 0);
        }
      }
      if (frame == 136) {
        work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
        work->fadeStep = 0;
      }
      cx = baseX / 2 - (ECLIPSE_ORB.variant << 13) + toFIXED(28);
      cy = baseY + (ECLIPSE_ORB.variant << 14) + toFIXED(56);
      u = frame - 136;
      if (u <= 84) {
        if (frame == 136) {
          ECLIPSE_ORB.variant = 4;
          ECLIPSE_ORB.x = 0;
          ECLIPSE_ORB.z = 0;
          BattleEffect_DrawShadedCircles(work->sheet, ECLIPSE_ORB.variant * 2,
                                         FIXED_ONE);
        }
        if (frame > 171) {
          if (frame == 172) {
            ECLIPSE_ORB.variant = 64;
            BattleEffect_DrawShadedCircles(work->sheet, 128, FIXED_ONE);
            velocityX = toFIXED(3);
            velocityY = -toFIXED(3);
          }
          if (frame > 203 && ECLIPSE_ORB.variant > 1) {
            ECLIPSE_ORB.variant -= 4;
            if (ECLIPSE_ORB.variant <= 1) {
              ECLIPSE_ORB.variant = 2;
            }
            BattleEffect_DrawShadedCircles(work->sheet, ECLIPSE_ORB.variant * 2,
                                           FIXED_ONE);
          }
        } else if (frame > 135) {
          velocityX += -toFIXED(1) / 32;
          velocityY += toFIXED(1) / 32;
          if (++ECLIPSE_ORB.variant <= 32) {
            BattleEffect_DrawShadedCircles(work->sheet, ECLIPSE_ORB.variant * 2,
                                           FIXED_ONE);
          } else {
            ECLIPSE_ORB.variant = 32;
          }
        }
        {
          s32 size = ECLIPSE_ORB.variant;

          Canvas_BlitAtFoot(work->sheet, FIXEDtoINT(cx), FIXEDtoINT(cy),
                            size * 2);
        }
      }
      if (frame > 47) {
        struct RenderModel *model;
        void *verts;
        s32 modelShade;

        if (frame == 48) {
          for (i = 0; i != 16; i++) {
            work->particles[i].x = Random16() & TRIG_ANGLE_MASK;
            work->particles[i].y = (Random16() & 255) + 128;
            if (Random16() & 1) {
              work->particles[i].y = -work->particles[i].y;
            }
            work->particles[i].variant = i * -8;
          }
          Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET,
                                     work->sheet + ECLIPSE_TORNADO_PICTURES, 1,
                                     0);
          Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                                     work->sheet + ECLIPSE_GLOW_BAND_A, 0, 0);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        modelShade = 0;
        model->shade = modelShade;
        texture.logWidth = 5;
        texture.logHeight = 2;
        texture.pixels = work->sheet + ECLIPSE_GLOW_BAND_A;
        model->kind = MODEL_SHADED_MAX;
        model->mesh = gBattleEffect_SmallSquareMesh;
        model->texture = &texture;
        model->vertices = verts;
        for (i = 0; i != 8; i++) {
          work->particles[i].variant++;
          work->particles[i].x += work->particles[i].y;
          if (work->particles[i].variant >= 0 &&
              work->particles[i].variant <= 47) {
            Matrix_Initialize();
            {
              s32 x = INTtoFIXED((i & 3) * 2) + toFIXED(2);
              s32 o = -0x400000;
              Matrix_Translate(cx + x + o, cy - INTtoFIXED((i & 3) + 2) + o, 0);
            }
            Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
            Matrix_RotateZ(work->particles[i].x);
            Matrix_Scale(Trig_Sin(work->particles[i].variant * 682) +
                             FIXED_ONE / 4,
                         FIXED_ONE * 2, FIXED_ONE);
            Render_TransformVertices(gBattleEffect_TaperedBeamCorners, verts,
                                     4);
            Render_DrawModelList(model);
          }
        }
        if (frame >= 136 && frame <= 171) {
          s32 s = 0x40000 - u * 6000;

          modelShade = u * 4 - 64;
          if (modelShade > -16) {
            modelShade = -16;
          }
          texture.logWidth = 6;
          model->shade = modelShade;
          texture.logHeight = 6;
          model->kind = MODEL_SHADED_ADDITIVE;
          texture.pixels = texturePixels;
          model->texture = &texture;
          model->mesh = gBattleEffect_SquareMesh;
          model->vertices = verts;
          Matrix_Initialize();
          Matrix_Translate(cx - toFIXED(62), cy - toFIXED(66), 0);
          Matrix_Scale(s >> 1, s, s);
          Matrix_RotateX(DEGtoANG(45));
          Matrix_RotateZ(TRIG_TURN * 5 / 32);
          Matrix_RotateY(-frame << 10);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
        if (frame >= 172 && frame <= 220) {
          Matrix_Initialize();
          Matrix_Translate(cx - toFIXED(62), cy - toFIXED(66), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
          Matrix_RotateZ(TRIG_TURN * 5 / 32);
          if (frame > 203) {
            Matrix_Scale(FIXED_ONE * 8 - ((frame - 204) << 15), FIXED_ONE * 2,
                         FIXED_ONE);
          } else {
            Matrix_Scale(FIXED_ONE * 8, FIXED_ONE * 2, FIXED_ONE);
          }
          Render_TransformVertices(gBattleEffect_BeamCorners, verts, 4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      if (frame >= 148 && frame <= 219) {
        for (j = 0; j != 4; j++) {
          s32 t = (frame + j) & 3;
          // Default: set for real below.
          s32 x = 0;
          s32 y = 0;

          if (frame <= 171) {
            s32 size = ECLIPSE_ORB.variant * 2;

            x = Random16() % size + (FIXEDtoINT(cx)) - (size >> 1) -
                (gBattleEffect_GlintPictureWidths[t] >> 1);
            y = Random16() % size + (FIXEDtoINT(cy)) - (size >> 1) -
                (gBattleEffect_GlintPictureHeights[t] >> 1);
          } else {
            u32 a = Random16() & 63;
            s32 r = ECLIPSE_ORB.variant;

            x = (FIXEDtoINT(cx)) - (a >> 1) -
                (gBattleEffect_GlintPictureWidths[t] >> 1) + Random16() % r -
                r / 2 - 8;
            y = a + (FIXEDtoINT(cy)) -
                (gBattleEffect_GlintPictureHeights[t] >> 1);
          }
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              gEclipse_GlintFlips[Random16() & 3] | 3, 1);
          ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
              canvas,
              work->sheet + ECLIPSE_TORNADO_PICTURES +
                  gBattleEffect_GlintPictures[t],
              x, y, gBattleEffect_GlintPictureWidths[t],
              gBattleEffect_GlintPictureHeights[t]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        }
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_PRESENT_CANVAS(work);
    }
  }
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  for (i = 0; i != 32; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  ActorShadow_LoadTilesFar();
  battle->arrowTiles = VramCache_AllocEntry(128);
  BattleEffect_SelectLivingTargets(work->args);
  for (i = 0; i != args->targetCount; i++) {
    BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_GRB, TINT_KEEP,
                             TINT_KEEP, 0);
    BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_NORMAL, TINT_KEEP,
                             TINT_KEEP, 0);
  }
  Palette_LoadBasicSpriteColors();
  REG_DISPCNT = DISPCNT_MODE_1;
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, 24);
  IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
  IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES, 0);
  wait(1);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
  Iwram_FillWords(canvas, CANVAS_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  args->targets[args->targetCount] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(args->targets, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_B,
                             work->sheet + ECLIPSE_GLOW_BAND_B, 0, 0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  for (i = 0; i != ECLIPSE_RAMP_SIZE; i++) {
    s32 v = i / 2;

    if (v > CANVAS_LEVEL_BRIGHTEST) {
      v = CANVAS_LEVEL_BRIGHTEST;
    }
    for (j = 0; j != ECLIPSE_RAMP_SIZE; j++) {
      work->sheet[j * ECLIPSE_RAMP_SIZE + i] = v;
    }
  }
  // The blast whites out the battlefield.
  fade = 0;
  for (frame = 0; frame != 48; frame++) {
    if (frame > 39) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_BRIGHTEN, CANVAS_FILL_WORD(4));
    }
    if (frame == 16) {
      Sound_PlayFar(SE_SPIN);
    }
    if (frame == 18) {
      work->shakeFrames = 90;
      Sound_PlayFar(SE_CRASH);
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 4);
      }
    }
    if (frame <= 109) {
      // Default: set for real below.
      struct RenderModel *model = 0;
      void *verts = 0;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      breathTexture.logWidth = 5;
      breathTexture.logHeight = 2;
      breathTexture.pixels = work->sheet + ECLIPSE_GLOW_BAND_B;
      model->mesh = gBattleEffect_SmallSquareMesh;
      model->shade = fade;
      model->texture = &breathTexture;
      model->vertices = verts;
      model->kind = MODEL_SHADED_MAX;
      if (frame > 15) {
        struct Particle *q = &work->particles[63];

        if (frame == 16) {
          q->x = toFIXED(128);
          q->y = -toFIXED(40);
        }
        if (frame <= 19) {
          q->x += -toFIXED(32);
          q->y += toFIXED(64);
        }
        if (frame <= 27) {
          cells = work->sheet + 0x38c0;
          BattleEffect_DrawShadedCircles(cells, 96, FIXED_ONE);
          Canvas_BlitAtFoot(cells, FIXEDtoINT(q->x), FIXEDtoINT(q->y), 96);
        }
        Matrix_Initialize();
        Matrix_Translate(q->x - toFIXED(63), q->y - toFIXED(64), 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_RotateZ(TRIG_TURN * 19 / 32);
        Matrix_Scale(0xdc500, 0x29c40, FIXED_ONE / 2);
        Render_TransformVertices(gBattleEffect_BeamCorners, verts, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(verts);
    }
    {
      s32 r = 8;

      if (frame > 130) {
        r = 2;
      } else if (frame > 120) {
        r = 4;
      } else if (frame > 110) {
        r = 6;
      }
      BattleCamera_Shake(r, r);
    }
    EFFECT_END_FRAME(work);
  }
  // The blast lands on the land far below and spreads in rings of light.
  count = -1;
  transition[4] = 1;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_AERIAL_VIEW_BACKDROP, 0);
  Palette_BrightenSavedBattleBg(RGB_LEVEL_MAX, RGB_LEVEL_MAX, RGB_LEVEL_MAX);
  gBgScroll[1].x = 0;
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  Resource_LoadAndDecompress(RESOURCE_DUST_BLOB_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE,
                             work->sheet + ECLIPSE_FLAME_BAND, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_B,
                             work->sheet + ECLIPSE_GLOW_BAND_B, 0, 0);
  {
    s32 *v = &FLASHES[0].variant;
    s32 c = -1;

    for (i = 0; i != 512; i++) {
      *v = c;
      v += 7;
    }
  }
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  {
    const void *mesh = ECLIPSE_DOME_MESH;
    void *vertices = gMapGridCellBuffer;
    const void *bandMesh = ECLIPSE_BAND_MESH;

    for (frame = 0; frame != 26; frame++) {
      if ((gInput.repeat & (KEY_A | KEY_B)) && count == -1) {
        count = 8;
      }
      if (frame <= 6 && count == -1) {
        s32 b = 31 - frame * 8;
        s32 a = b;
        s32 c = b;

        if (c < -10) {
          a = -10;
        }
        if (c < -15) {
          b = -15;
        }
        if (c <= 3) {
          c = 4;
        }
        Palette_BrightenSavedBattleBg(a, b, c);
      }
      if (frame > 17 || count > 0) {
        Palette_BrightenBattleBg(4, 4, 4);
        if (--count == 0) {
          break;
        }
      }
      for (i = 0; i != 16; i++) {
        struct Particle *q =
            ECLIPSE_MOTES + ((frame * 16 + i) & (ECLIPSE_MOTE_COUNT - 1));
        // Default: set for real below.
        s32 a = 0;
        s32 r = 0;

        a = Random16();
        r = (Random16() & 15) + frame * 2;
        q->x = Trig_Sin(a) * r + toFIXED(60);
        q->y = Trig_Cos(a) * r + toFIXED(60);
        q->velocityX = 0;
        q->velocityY = 0;
        q->variant = (Random16() & 15) + 16;
      }
      for (i = 0; i != ECLIPSE_MOTE_COUNT; i++) {
        p = ECLIPSE_MOTES + i;
        if (p->variant >= 0) {
          s32 n = (p->variant >> 3) + 1;

          blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                  (FIXEDtoINT(p->x)) - n / 2, (FIXEDtoINT(p->y)) - n, n, n * 2);
          Particle_AdvanceWithGravity2D(p, 60, -toFIXED(1) / 4);
          p->variant--;
        }
      }
      model = Render_AllocateModelList(1);
      shade = 0;
      if (frame <= 31) {
        s32 s = FIXED_ONE;

        if (frame > 15) {
          s += FIXED_ONE - (frame << 12);
        }
        landingTexture.logWidth = 5;
        landingTexture.logHeight = 2;
        landingTexture.pixels = work->sheet + ECLIPSE_GLOW_BAND_B;
        model->mesh = gBattleEffect_SmallSquareMesh;
        model->shade = shade;
        model->vertices = gMapGridCellBuffer;
        model->kind = MODEL_SHADED_MAX;
        model->texture = &landingTexture;
        Matrix_Initialize();
        Matrix_Translate(-toFIXED(2), toFIXED(16), 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_RotateZ(TRIG_TURN * 19 / 32);
        Matrix_Scale(s, FIXED_ONE * 2, FIXED_ONE / 2);
        Render_TransformVertices(gBattleEffect_BeamCorners, gMapGridCellBuffer,
                                 4);
        Render_DrawModelList(model);
      }
      if (frame == 0) {
        Sound_PlayFar(SE_BANE_ESCAPE);
        Mesh_BuildGrid(ECLIPSE_DOME_MESH, ECLIPSE_DOME_RING_VERTICES,
                       ECLIPSE_DOME_RINGS, 8, 8);
        {
          s32 ringAngle = 0;
          for (ring = 0, offset = 0; ring != ECLIPSE_DOME_RINGS;
               ringAngle += DEGtoANG(12),
              offset += ECLIPSE_DOME_RING_VERTICES * sizeof(struct ModelVertex),
              ring++) {
            s32 y = FIXEDtoINT(-Trig_Cos(ringAngle) * 96);
            s32 rr = FIXEDtoINT(Trig_Sin(ringAngle) * 48);
            struct ModelVertex *v = (struct ModelVertex *)ECLIPSE_DOME_VERTICES;

            k = 0;
            v = (struct ModelVertex *)((u8 *)v + offset);
            for (; k != ECLIPSE_DOME_RING_VERTICES; k++) {
              SET_POSITION(v, FIXEDtoINT(Trig_Sin(k << 12) * rr), y,
                           FIXEDtoINT(Trig_Cos(k << 12) * rr));
              v++;
            }
          }
        }
        Mesh_BuildCylinderBand(ECLIPSE_BAND_VERTICES, ECLIPSE_BAND_MESH, 32);
      }
      landingTexture.logWidth = 5;
      landingTexture.logHeight = 5;
      landingTexture.pixels = work->sheet;
      {
        model->kind = MODEL_SHADED_MAX;
        model->mesh = mesh;
        model->vertices = vertices;
        model->texture = &landingTexture;
        model->shade = shade;
        model->flags = 3;
      }
      {
        s32 s = (frame << 9) + FIXED_ONE / 2;

        if (s > FIXED_ONE) {
          s = FIXED_ONE;
        }
        Matrix_Initialize();
        Matrix_Translate(-toFIXED(2), toFIXED(12), 0);
        Matrix_RotateX(DEGtoANG(45));
        Matrix_RotateY(-frame << 10);
        Matrix_ScaleUniform(s);
        Render_TransformVertices(ECLIPSE_DOME_VERTICES, vertices,
                                 ECLIPSE_DOME_RINGS *
                                     ECLIPSE_DOME_RING_VERTICES);
        Render_DrawModelList(model);
      }
      landingTexture.logHeight = 5;
      landingTexture.logWidth = 7;
      landingTexture.pixels = work->sheet + ECLIPSE_FLAME_BAND;
      model->texture = &landingTexture;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->mesh = bandMesh;
      model->flags = shade;
      model->vertices = vertices;
      model->scrollU = shade;
      model->scrollV = shade;
      {
        for (i = 0; i != 4; i++) {
          s32 t = frame + 24 - i * 6;

          if (t >= 0 && t <= 63) {
            s32 s = (gEclipse_RingGrowth[i] * t << 10) + 0x4000;

            shade = 0;
            if (t <= 15) {
              shade = t * 4 - 64;
            }
            if (t > 40) {
              shade = (40 - t) * 4;
            }
            model->shade = shade;
            Matrix_Initialize();
            Matrix_Translate(-toFIXED(2), toFIXED(32), 0);
            Matrix_Scale(s / 4, s / 4, s / 4);
            Matrix_RotateX(10000);
            Matrix_RotateY((frame + i * 8) << 10);
            Matrix_Scale(FIXED_ONE, FIXED_ONE * 6, FIXED_ONE);
            Matrix_Translate(0, INTtoFIXED(gEclipse_RingHeights[i]), 0);
            Render_TransformVertices(ECLIPSE_BAND_VERTICES, vertices, 64);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      EFFECT_PRESENT_CANVAS(work);
    }
  }
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, 24);
  wait(1);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
  Graphics_LoadGreyRamp();
  Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  for (frame = 0; frame != 32; frame++) {
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Unused: two empty functions after Eclipse; nothing calls them.
void BattleEffect_ReservedNoOpA(void) {}

/// Does nothing; nothing calls it.
void BattleEffect_ReservedNoOpB(void) {}

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define FORM_CHANGE_GLOW_BAND 0x3000
/// Where BillowTexture, the crystals' texture, is decoded in the map cell
/// buffer.
#define FORM_CHANGE_BILLOW (Ram_MapCellBuffer + 0x2000)

/// A glow on the canvas map widens over the user for 56 frames while 16
/// spinning rings rise around it, four abreast. From frame 48 the screen
/// shakes, flashes burst upward and four crystals grow out of the ground one
/// after another, spinning. The user flashes at frames 32 to 68. Kinds 78
/// and 79 stand to either side and hand over to the battle
/// (Battle_ChangeEnemyFormFar) at frame 62; kind 80 runs 100 frames, fades to
/// white and restores the scene's view itself.
void BattleEffect_RunFormChange(struct BattleEffectParams *args, s32 kind) {
  struct ScreenPoint center;
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frames;
  s32 frame;
  s32 index;
  s32 i;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  work->vblanksSinceTransfer = 1;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                             work->sheet + FORM_CHANGE_GLOW_BAND, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_BILLOW_TEXTURE, FORM_CHANGE_BILLOW, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;

  frames = 100;
  if (kind != KIND_FINALE)
    frames = 76;
  for (frame = 0; frame != frames; frame++) {
    index = (frame - 48) * 2;
    // The effect's centre, beside or above the user.
    EffectPosition_ProjectBattler(args->user, &center);
    if (kind == KIND_RIGHT) {
      center.x += 40;
      center.y -= 24;
    } else if (kind == KIND_LEFT) {
      center.x -= 16;
      center.y -= 24;
    } else {
      center.y -= 48;
      /* FAKEMATCH(barrier): an empty asm taking the centre's address gives it
       * two more references. */
      __asm__("" : : "r"(&center));
    }

    if (frame == 0) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      if (kind == KIND_LEFT)
        Iwram_CopyWordsEntry((void *)BG_PLTT,
                             Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                             SHEET_PALETTE_BYTES);
      else
        Iwram_CopyWordsEntry((void *)BG_PLTT,
                             Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_C),
                             SHEET_PALETTE_BYTES);
      if (frame == 0)
        Sound_PlayFar(SE_SPIN);
    }
    if (frame == 48)
      Sound_PlayFar(SE_CLATTER);
    if (frame == 56)
      Sound_PlayFar(SE_BOSS_ROAR);
    if (frame == 32)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, -1);
    if (frame == 68)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, -1);
    if (kind == KIND_FINALE) {
      // The finale fades to white from frame 62.
      if (frame > 61) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));
        Palette_BrightenBgEntries(2, 2, 2);
      }
    } else if (frame == 62) {
      Battle_ChangeEnemyFormFar(kind);
    }

    // From frame 48: shake, clear the canvas, and for 8 frames burst two
    // flashes a frame from a point near the centre.
    if (frame >= 48) {
      struct Particle *flashes;

      if (frame == 48) {
        {
          s32 *variant = &FLASHES[0].variant;

          for (i = 0; i != 64; i++) {
            *variant = -1;
            variant += sizeof(struct Particle) / sizeof(*variant);
          }
        }
        work->shakeFrames = 8;
        Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      flashes = FLASHES;
      if (frame >= 48 && frame <= 55) {
        s32 x = INTtoFIXED((Random16() & 63) + center.x / 2 - 32);
        s32 y = INTtoFIXED((Random16() & 31) + center.y);
        s32 k = 0;
        s32 spread = 15;
        struct Particle *burst = &flashes[index];

        for (; k != 2; k++) {
          struct Particle *flash = &burst[k];
          s32 speed = 31; // Set first: the mask is the tied operand.
          s32 angle;

          speed &= Random16();
          angle = Random16();

          flash->x = x + INTtoFIXED((Random16() & spread) - 8);
          flash->y = y + INTtoFIXED((Random16() & spread) - 8);
          flash->velocityX = Trig_Sin(angle) * (speed + 32) >> 3;
          flash->velocityY = Trig_Cos(angle) * (speed + 32) >> 3;
          flash->variant = 0;
        }
      }
      {
        struct Particle *flash = flashes;

        for (i = 0; i != 16; i++, flash++) {
          if (flash->variant >= 0 && flash->variant <= 17) {
            draw[0](canvas, work->sheet + (flash->variant / 3 << 11),
                    FIXED_INTEGER_PART(flash->x) - 16,
                    FIXED_INTEGER_PART(flash->y) - 48, FLASH_BURST_WIDTH,
                    FLASH_BURST_HEIGHT);
            Particle_AdvanceWithGravity2D(flash, 62, -toFIXED(1) / 32);
            flash->variant++;
          }
        }
      }
    }

    if (frame >= 0) {
      FIXED cx = INTtoFIXED(center.x / 2);
      FIXED cy = INTtoFIXED(center.y);
      struct RenderModel *model;
      void *corners;
      s32 bias;

      // The glow on the canvas map widens for 56 frames.
      if (frame <= 63) {
        s32 size = 128;
        s32 width = frame + 8;

        if (width <= 64) {
          size = width * 2;
          BattleEffect_DrawShadedCircles(Ram_MapCellBuffer, size, FIXED_ONE);
        }
        Canvas_BlitAtFoot(Ram_MapCellBuffer, FIXEDtoINT(cx), FIXEDtoINT(cy),
                          size);
      }

      // Each ring has an angle (x), a spin (y) and an age (variant), the
      // later ones 4 frames apart.
      if (frame == 0) {
        for (i = 0; i != 16; i++) {
          struct Particle *ring = &work->particles[i];

          ring->x = Random16() & TRIG_ANGLE_MASK;
          ring->y = (Random16() & 255) + 128;
          if (Random16() & 1)
            ring->y = -ring->y;
          ring->variant = -4 * i;
        }
      }

      corners = Heap_AllocEwram(0x200);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      bias = 0;
      model->shade = bias;
      texture.logHeight = 2;
      texture.pixels = work->sheet + FORM_CHANGE_GLOW_BAND;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &texture;
      model->mesh = gBattleEffect_SmallSquareMesh;
      model->vertices = corners;

      // The rings rise for 24 frames, spinning and swelling.
      for (i = 0; i != 16; i++) {
        struct Particle *ring = &work->particles[i];

        ring->variant += 2;
        ring->x += ring->y * 2;
        if (ring->variant >= 0 && ring->variant <= 47) {
          Matrix_Initialize();
          {
            s32 column = i & 3;

            Matrix_Translate(cx + INTtoFIXED(column * 2 + 2) - toFIXED(64),
                             cy - INTtoFIXED(column + 2) - toFIXED(64), 0);
          }
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
          Matrix_RotateZ(ring->x);
          Matrix_Scale(Trig_Sin(ring->variant * 682) + FIXED_ONE, FIXED_ONE * 2,
                       FIXED_ONE);
          Render_TransformVertices(gBattleEffect_TaperedBeamCorners, corners,
                                   4);
          Render_DrawModelList(model);
        }
      }

      // The four crystals grow out of the ground from frame 48 on.
      model->texture = &texture;
      model->vertices = corners;
      texture.logWidth = 6;
      texture.pixels = FORM_CHANGE_BILLOW;
      model->kind = MODEL_SHADED_ADDITIVE;
      texture.logHeight = 6;
      model->mesh = gBattleEffect_RingMesh;
      for (i = 0; i != 4; i++) {
        s32 start = gFormChange_WaveStarts[i] + 48;

        model->scrollV = (frame * 8 + i * 32) & 127;
        if (frame > start) {
          bias = (start - frame) * 8 + 56;
          if (bias > -16)
            bias = -16;
          if (bias > -64) {
            s32 growth = (frame - start) * gFormChange_WaveGrowth[i] * 2000 +
                         FIXED_ONE / 4;

            model->shade = bias;
            Matrix_Initialize();
            Matrix_Translate(INTtoFIXED(center.x / 2 - 60), toFIXED(32), 0);
            Matrix_Scale(growth * 2,
                         gFormChange_WaveStretch[i] * growth + FIXED_ONE * 4,
                         growth / 4);
            Matrix_RotateX(1000);
            if (i & 1)
              Matrix_RotateY((i * 8 + frame) << 10);
            else
              Matrix_RotateY((i * 8 - frame) << 10);
            Render_TransformVertices(gBattleEffect_RingCorners, corners, 32);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(corners);
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  if (kind == KIND_FINALE) {
    // Put the scene's view back by hand.
    Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
    Task_Remove(Palette_StepFadeTransfer);
  } else {
    BattleEffect_EndCanvasLayer();
  }
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// The frame callback that copies the effect canvas to BG character block 1
/// when a transfer is pending, in the way the transfer mode asks for.
void BattleEffect_ProcessPendingGraphicsTransfer(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *source = Ram_HeapSlots->battleCanvas;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case CANVAS_COPY:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      Iwram_FillWords(source, CANVAS_BYTES, work->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      if (work->transferValue == CANVAS_FADE_HALF)
        ColorBuffer_Halve(source, (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(1),
                                       CANVAS_BYTES);
      break;
    case CANVAS_COPY_AND_DARKEN:
      ColorBuffer_Darken(source, work->transferValue, (void *)BG_CHAR_BLOCK(1),
                         CANVAS_BYTES);
      break;
    case CANVAS_COPY_AND_BRIGHTEN:
      ColorBuffer_Brighten(source, work->transferValue,
                           (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      break;
    }
    // DEAD: nothing uses the counter read here; the ROM keeps its load.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// When a transfer is pending, copies the 0x1000-byte canvas at work 0x6780
/// to the stamp's tiles (the pointer at work 0x7b74), then clears it.
void BattleEffect_ProcessPendingStampTransfer(void) {
  u8 *work = Ram_HeapSlots->battleEffect;
  u32 ime;
  s32 *pending;

  IRQ_LOCK(ime);
  pending = (s32 *)(work + 0x78ec);
  if (*pending == 1) {
    *pending = 0;
    IRQ_UNLOCK(ime);
    Iwram_CopyWords(*(void **)(work + 0x7b74), work + 0x6780, 0x1000);
    Iwram_FillWords(work + 0x6780, 0x1000, 0);
  } else {
    IRQ_UNLOCK(ime);
  }
}

/// The same callback for the 0x7800-byte canvas in the map cell buffer, copied
/// to BG character block 2.
void BattleEffect_FlushPendingGraphicsTransfer(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *source = Ram_MapCellBuffer;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case CANVAS_COPY:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      Iwram_FillWords(source, BATTLE_BACKGROUND_BYTES, work->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      if (work->transferValue == CANVAS_FADE_HALF)
        ColorBuffer_Halve(source, (void *)BG_CHAR_BLOCK(2),
                          BATTLE_BACKGROUND_BYTES);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(2),
                                       BATTLE_BACKGROUND_BYTES);
      break;
    case CANVAS_COPY_AND_DARKEN:
      ColorBuffer_Darken(source, work->transferValue, (void *)BG_CHAR_BLOCK(2),
                         BATTLE_BACKGROUND_BYTES);
      break;
    }
    // DEAD: nothing uses the counter read here; the ROM keeps its load.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// The same for the canvas in the second effect work block.
void BattleEffect_ProcessPendingBlock2Transfer(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *source;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    source = Ram_HeapSlots->battleCanvas;
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case CANVAS_COPY:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      Iwram_FillWords(source, CANVAS_VIEW_BYTES, work->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      if (work->transferValue == CANVAS_FADE_HALF)
        ColorBuffer_Halve(source, (void *)BG_CHAR_BLOCK(2), CANVAS_VIEW_BYTES);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(2),
                                       CANVAS_VIEW_BYTES);
      break;
    case CANVAS_COPY_AND_DARKEN:
      ColorBuffer_Darken(source, work->transferValue, (void *)BG_CHAR_BLOCK(2),
                         CANVAS_VIEW_BYTES);
      break;
    }
    // DEAD: nothing uses the counter read here; the ROM keeps its load.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// Writes BG2's map in screen block 31 for the canvas: the rows above and below
/// it blank, and the canvas's tiles numbered across between them.
void BattleEffect_BuildCanvasTilemap(void) {
  u32 *map = (u32 *)BG_SCREEN_BLOCK(31);
  u32 tiles;
  s32 row;
  s32 column;

  Iwram_FillWords((void *)map, 0x100, -1);
  map += 0x40;
  Iwram_FillWords((void *)map, 0x80, 0x03ff03ff);
  map += 0x20;
  tiles = (u32)(Ram_MapCellBuffer + 0x200);
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 8; column++) {
      *map++ = tiles;
      tiles += 0x00020002;
    }
    for (column = 0; column != 8; column++)
      *map++ = 0x03ff03ff;
  }
}

// The scenes' frame callbacks that copy the display registers the effect
// keeps in its work to the hardware.

/// Writes the window, display and blend registers, all at once.
void BattleEffect_ApplyWindowBlendRegisters(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  REG_WIN0H = work->win0h;
  REG_WIN0V = work->win0v;
  REG_WIN1H = work->win1h;
  REG_WIN1V = work->win1v;
  REG_WININ = work->winin;
  REG_WINOUT = work->winout;
  REG_DISPCNT = work->dispcnt;
  REG_BLDCNT = work->bldcnt;
  REG_BLDALPHA = work->bldalpha;
}

/// Writes background 2's scale (BG2PA and BG2PD) and its reference point.
void Display_ApplyBg2Affine(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  REG_BG2PA = work->bg2pa;
  REG_BG2PD = work->bg2pd;
  /* FAKEMATCH(barrier): an empty asm. */
  __asm__ volatile("");
  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

/// Writes background 2's reference point.
void Display_ApplyBg2Reference(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

/// While fadeFrames lasts, scales the battle background's colours (BG palette 6
/// on) one fade step darker than the battle's own palette each frame.
void Palette_StepFadeTransfer(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  s32 *framesLeft;
  struct BattleWork *battle;

  framesLeft = &work->fadeFrames;
  battle = Ram_HeapSlots->battleWork;

  if (*framesLeft > 0) {
    s32 *step = &work->fadeStep;

    (*step)++;
    ColorBuffer_ScaleFar(battle->palette, (void *)&BG_PLTT_COLOR(6, 0),
                         FIXED_ONE - *step * CANVAS_FADE_STEP,
                         BATTLE_BACKGROUND_COLORS);
    (*framesLeft)--;
  }
}

/// Clears every target's flash timer.
void BattleEffect_ClearActorTimers(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  Iwram_ClearWords(work->actorTimers, sizeof work->actorTimers);
}

/// Counts down each target's flash timer; when one runs out, the target gets
/// its own palette back.
void BattleEffect_CountDownBattlerTints(void) {
  struct BattleEffectWork *work;
  s32 actorIndex;

  work = Ram_HeapSlots->battleEffect;
  actorIndex = 0;
  do {
    if (work->actorTimers[actorIndex] != 0) {
      if ((work->actorTimers[actorIndex] = work->actorTimers[actorIndex] - 1) ==
          0) {
        BattleEffect_TintBattler(work->args->targets[actorIndex],
                                 SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
      }
    }
    actorIndex += 1;
  } while (actorIndex != 8);
}

/// Shows the canvas on BG2 at half width across the whole screen, blended over
/// the battle, inside window 0.
void BattleEffect_SetupCanvasWindows(void) {
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(14);
  REG_BG2X = 0;
  REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = Q_8_8(1);
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
}

/// Opens the canvas layer on BG2 with control bgControl: the display, the
/// canvas's tiles and map, the windows and the backdrop's fade.
void BattleEffect_StartCanvasLayer(s32 bgControl) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleCanvas;
  struct BattleBackgroundWork *display = Ram_HeapSlots->battleBackground;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_ClearActorTimers();
  display->secondMode = BATTLE_BACKGROUND_FIXED;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  wait(1);

  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
  display->secondMode = BATTLE_BACKGROUND_NONE;
  wait(1);

  BattleEffect_SetupCanvasWindows();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);

  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 chr;
      s16 pair;

      chr = row * 16 + col * 2;
      pair = ((chr + 1) << 8) | chr;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
    }
  }

  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  work->shakeFrames = 0;
  work->transferPending = 0;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  wait(1);
}

/// Opens the effect canvas layer: BG2 becomes a 2x-wide 128x128 canvas of
/// 16x16 characters, blended over the battle background, whose palette
/// starts fading.
void BattleEffect_OpenCanvasLayer(s32 bgControl) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleCanvas;
  struct BattleBackgroundWork *display = Ram_HeapSlots->battleBackground;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ClearActorTimers();
  display->secondMode = BATTLE_BACKGROUND_FIXED;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  wait(1);
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background, 128);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
  display->secondMode = BATTLE_BACKGROUND_NONE;
  wait(1);

  BattleEffect_SetupCanvasWindows();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  wait(1);

  // The canvas map: 16 rows of 8 character pairs.
  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 chr;
      s16 pair;

      chr = row * 16 + col * 2;
      pair = ((chr + 1) << 8) | chr;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
    }
  }

  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  work->shakeFrames = 0;
  work->transferPending = 0;
  wait(1);
}

/// Opens the canvas layer with the canvas repeated across a wrapping map.
void BattleEffect_StartTiledCanvas(s32 bgControl) {
  s32 row;
  s32 column;
  s32 offset;

  BattleEffect_StartCanvasLayer(bgControl);
  REG_BG2CNT = bgControl | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    for (column = 0; column != 8; column++, offset += 2) {
      *(volatile u16 *)((u8 *)BG_CANVAS_MAP + offset) = 0;
    }
  }
}

/// Opens the canvas layer with the canvas repeated across a wrapping map, every
/// tile numbered in order.
void BattleEffect_StartTiledCanvasFilled(s32 bgControl) {
  s32 row;
  s32 column;
  s32 offset;

  BattleEffect_StartCanvasLayer(bgControl);
  REG_BG2CNT = bgControl | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    for (column = 0; column != 8; column++, offset += 2) {
      *(volatile u16 *)((u8 *)BG_CANVAS_MAP + offset) = 0xf0f;
    }
  }
}

/// Closes the effect canvas layer: BG1 back where it was, BG2 and its
/// characters cleared, then the background palette faded back in.
void BattleEffect_EndCanvasLayer(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 i;

  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  REG_BG2CNT = BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1) |
               BGCNT_PRIORITY(3);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Task_Remove(Palette_StepFadeTransfer);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  REG_BLDCNT = 0;
  wait(1);

  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 7);
  wait(1);
  for (i = 0; i != 8; i++) {
    BattleBackground_SetPaletteLevelFar(battle->background, 21 - i * 3);
    wait(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  wait(1);
}

/// Hides the canvas layer: marks the background view, queues DISPCNT 0x1541,
/// fades the background palette back and queues BG1's control word.
void BattleEffect_HideBg1AndRestoreBackdrop(void) {
  struct BattleWork *battle;
  s32 *flag;

  flag = &(Ram_HeapSlots->battleBackground)->secondMode;
  battle = Ram_HeapSlots->battleWork;
  *flag = 1;
  IoWriteQueue_Store16((u32)&REG_DISPCNT,
                       DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON |
                           DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1);
  wait(1);
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS |
                          BGCNT_PRIORITY(3),
                      IO_WRITE_HALFWORD);
  wait(1);
}

/// Shows the canvas on BG1 instead of BG2, at its own size.
void BattleEffect_SetupCanvasTileMap(void) {
  void *canvas = Ram_HeapSlots->battleCanvas;
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  struct BattleBackgroundWork *layer = Ram_HeapSlots->battleBackground;
  s32 row;
  s32 column;
  s32 byteOffset;

  BattleEffect_ClearActorTimers();
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, screen->background, 0);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  layer->secondMode = BATTLE_BACKGROUND_NONE;
  // Blank tile 255, then rows that show nothing.
  Iwram_ClearWords(BG_CHAR_BLOCK(0) + 255 * 64, 64);
  Iwram_FillWords(BG_SCREEN_BLOCK(31) + 4 * 64, 8 * 64, -1);
  // The canvas's tiles, then blank tile 255 across the rest of each row.
  byteOffset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    for (column = 0; column != 32; column++, byteOffset += 2) {
      if (column >= CANVAS_TILES)
        *(volatile u16 *)(BG_SCREEN_BLOCK(31) + 12 * 64 + byteOffset) = 255;
      else
        *(volatile u16 *)(BG_SCREEN_BLOCK(31) + 12 * 64 + byteOffset) =
            row * CANVAS_TILES + 256 + column;
    }
  }
  REG_DISPCNT = DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
  REG_BG1CNT = BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS | BGCNT_PRIORITY(1);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(14);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  wait(1);
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

/// Draws a line into the canvas, clipped to it, with the fraction kept in
/// bit 31 (started at 0x80 << 23, carried when negative and cleared with a
/// shift pair) and dx and dy reused as the step direction.
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80 << 23;
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  if (y0 < 0)
    y0 = 0;
  if (y0 > 127)
    y0 = 127;
  if (y1 < 0)
    y1 = 0;
  if (y1 > 127)
    y1 = 127;

  if (ABS(dx) < ABS(dy)) {
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
    step <<= 23;
    x = x0;
    if (dx < 0)
      dx = -1;
    else
      dx = 1;
    for (i = y0; i != y1; i++) {
      offset = (((((u32)i >> 3) << 4) + ((u32)x >> 3)) << 3) + (i & 7);
      offset = (offset << 3) + (x & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        x += dx;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  } else {
    if (dx < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
    step <<= 23;
    y = y0;
    if (dy < 0)
      dy = -1;
    else
      dy = 1;
    for (i = x0; i != x1; i++) {
      offset = (((((u32)y >> 3) << 4) + ((u32)i >> 3)) << 3) + (y & 7);
      offset = (offset << 3) + (i & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        y += dy;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  }
}

#undef FLASHES
#undef CLIP
