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
#define FLASHES ((struct Particle *)gData_02014000)

/// Where TornadoSheet is decoded in the effect's pictures.
#define ECLIPSE_TORNADO_PICTURES 0x2000

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define ECLIPSE_GLOW_BAND_A 0x4000

/// Where GlowBandTextureB is decoded in the effect's pictures.
#define ECLIPSE_GLOW_BAND_B 0x3840

/// Where FlameBandTexture is decoded in the effect's pictures.
#define ECLIPSE_FLAME_BAND 0x400

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
  s32 baseX;
  s32 baseY;
  s32 velX;
  s32 velY;
  s32 cx;
  s32 cy;
  u32 u;
  s32 count;
  s32 ring;
  s32 shade;
  s32 offset;
  u8 *cells;
  s32 fade;
  s16 list[14];
  s32 place[4];
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
  for (frame = 0; frame != 52; frame++) {
    if (frame == 27) {
      u8 *tiles;

      list[0] = 0xff;
      BattleActor_SpawnObjectsForListFar(list, 0);
      VramCache_FreeEntry(battle->arrowTiles);
      VramBlock_Reset93Far();
      gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               (s32)&ResourceId_EarthHorizonBackdrop, 0);
      work->scrollTimer = 0;
      work->scrollInterval = 1;
      work->scrollStepX = -1;
      work->scrollStepY = 0;
      Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_VBLANK);
      transition[4] = 1;
      cells = Resource_GetTableEntry((s32)&ResourceId_GoldenDragonTiles);
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
      cells = Resource_GetTableEntry((s32)&ResourceId_GoldenDragonHeadTiles);
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
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
  REG_BG2X = 0;
  baseX = 0;
  baseY = 0;
  velX = 0;
  velY = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = Ram_HeapSlots->ramCode;
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                  SHEET_PALETTE_BYTES);
  {
    const u8 *texturePixels = (const u8 *)FLASHES;

    // The dragon looms over the horizon while shafts of light fall about it,
    // then breathes its blast; A or B cuts it short.
    for (frame = 0; frame != 220 && !(gInput.repeat & (KEY_A | KEY_B));
         frame++) {
      if (frame == 0) {
        baseX = -px(48);
        baseY = px(212);
        velX = px(4);
        velY = -px(10);
        work->fadeFrames = 1;
        work->fadeStep = frame;
        work->particles[16].variant = frame;
        Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, FLASHES, 0, 0);
      }
      if (frame == 136) {
        u8 *dst = Ram_MapCellBuffer + 0x400;
        for (i = 0; i != 7; i++) {

          void *o = (void *)OBJ_TILES(
              (struct Sprite *)work->sprites[gEclipse_TileObjects[i]]);
          // Default: set for real below; as s32 size = 0x400, 762 differ.
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
      place[1] = px(255);
      baseX += velX;
      baseY += velY;
      if (frame > 171) {
        if (frame > 183) {
          velX += -0x200;
          velY += 0x400;
        }
        if (frame <= 205) {
          velX = velX * 54 / DAMPING_SCALE;
          velY = velY * 54 / DAMPING_SCALE;
        }
      } else {
        if (velY < 0) {
          velY += px(1) / 4;
        }
        if (velX > 0) {
          velX += -px(1) / 8;
        }
        if (frame > 31) {
          baseY += Trig_Sin((frame - 32) << 9) << 2 >> 4;
          if (frame <= 159) {
            baseX += px(1) / 4;
          }
        }
      }
      for (i = 0; i != 32; i++) {
        if (i == 25 && frame > 135) {
          place[0] = baseX + px(32);
          place[2] = baseY + px(64);
        } else {
          place[0] = baseX + px(gEclipse_ObjectX[i]);
          place[2] = baseY + px(gEclipse_ObjectZ[i]);
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
      cx = baseX / 2 - (work->particles[16].variant << 13) + px(28);
      cy = baseY + (work->particles[16].variant << 14) + px(56);
      u = frame - 136;
      if (u <= 84) {
        if (frame == 136) {
          work->particles[16].variant = 4;
          work->particles[16].x = 0;
          work->particles[16].z = 0;
          BattleEffect_DrawShadedCircles(
              work->sheet, work->particles[16].variant * 2, 0x10000);
        }
        if (frame > 171) {
          if (frame == 172) {
            work->particles[16].variant = 64;
            BattleEffect_DrawShadedCircles(work->sheet, 128, Q16_ONE);
            velX = px(3);
            velY = -px(3);
          }
          if (frame > 203 && work->particles[16].variant > 1) {
            work->particles[16].variant -= 4;
            if (work->particles[16].variant <= 1) {
              work->particles[16].variant = 2;
            }
            BattleEffect_DrawShadedCircles(
                work->sheet, work->particles[16].variant * 2, 0x10000);
          }
        } else if (frame > 135) {
          velX += -px(1) / 32;
          velY += px(1) / 32;
          if (++work->particles[16].variant <= 32) {
            BattleEffect_DrawShadedCircles(
                work->sheet, work->particles[16].variant * 2, 0x10000);
          } else {
            work->particles[16].variant = 32;
          }
        }
        {
          s32 size = work->particles[16].variant;

          Canvas_BlitAtFoot(work->sheet, Q_16_16_TO_INT(cx), Q_16_16_TO_INT(cy),
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
          Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                                     work->sheet + ECLIPSE_TORNADO_PICTURES, 1,
                                     0);
          Resource_LoadAndDecompress((s32)&ResourceId_Data0C9,
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
            Transform_Reset();
            {
              s32 x = ((i & 3) << 17) + px(2);
              s32 o = -0x400000;
              Transform_ApplyTranslation(cx + x + o, cy - px((i & 3) + 2) + o,
                                         0);
            }
            Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE);
            Transform_ApplyRoll(work->particles[i].x);
            Transform_ApplyAxisScale(
                Trig_Sin(work->particles[i].variant * 682) + TRIG_TURN / 4,
                0x20000, 0x10000);
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
          Transform_Reset();
          Transform_ApplyTranslation(cx - px(62), cy - px(66), 0);
          Transform_ApplyAxisScale(s >> 1, s, s);
          Transform_ApplyPitch(TRIG_TURN / 8);
          Transform_ApplyRoll(TRIG_TURN * 5 / 32);
          Transform_ApplyYaw(-frame << 10);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
        if (frame >= 172 && frame <= 220) {
          Transform_Reset();
          Transform_ApplyTranslation(cx - px(62), cy - px(66), 0);
          Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE);
          Transform_ApplyRoll(TRIG_TURN * 5 / 32);
          if (frame > 203) {
            Transform_ApplyAxisScale(Q16_ONE * 8 - ((frame - 204) << 15),
                                     Q16_ONE * 2, 0x10000);
          } else {
            Transform_ApplyAxisScale(Q16_ONE * 8, Q16_ONE * 2, Q16_ONE);
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
            s32 size = work->particles[16].variant * 2;

            x = Random16() % size + (Q_16_16_TO_INT(cx)) - (size >> 1) -
                (gBattleEffect_GlintPictureWidths[t] >> 1);
            y = Random16() % size + (Q_16_16_TO_INT(cy)) - (size >> 1) -
                (gBattleEffect_GlintPictureHeights[t] >> 1);
          } else {
            u32 a = Random16() & 63;
            s32 r = work->particles[16].variant;

            x = (Q_16_16_TO_INT(cx)) - (a >> 1) -
                (gBattleEffect_GlintPictureWidths[t] >> 1) + Random16() % r -
                r / 2 - 8;
            y = a + (Q_16_16_TO_INT(cy)) -
                (gBattleEffect_GlintPictureHeights[t] >> 1);
          }
          BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7,
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
  VramBlock_Load93Far();
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
  args->targets[args->targetCount] = 0xff;
  BattleActor_SpawnObjectsForListFar(args->targets, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA,
                             work->sheet + ECLIPSE_GLOW_BAND_B, 0, 0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  for (i = 0; i != 120; i++) {
    s32 v = i / 2;

    if (v > CANVAS_LEVEL_BRIGHTEST) {
      v = CANVAS_LEVEL_BRIGHTEST;
    }
    for (j = 0; j != 120; j++) {
      work->sheet[j * 120 + i] = v;
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
          q->x = px(128);
          q->y = -px(40);
        }
        if (frame <= 19) {
          q->x += -px(32);
          q->y += px(64);
        }
        if (frame <= 27) {
          cells = work->sheet + 0x38c0;
          BattleEffect_DrawShadedCircles(cells, 96, Q16_ONE);
          Canvas_BlitAtFoot(cells, Q_16_16_TO_INT(q->x), Q_16_16_TO_INT(q->y),
                            96);
        }
        Transform_Reset();
        Transform_ApplyTranslation(q->x - px(63), q->y - px(64), 0);
        Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        Transform_ApplyRoll(TRIG_TURN * 19 / 32);
        Transform_ApplyAxisScale(0xdc500, 0x29c40, Q16_ONE / 2);
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
                           (s32)&ResourceId_AerialViewBackdrop, 0);
  Palette_BrightenSavedBattleBg(RGB_LEVEL_MAX, RGB_LEVEL_MAX, RGB_LEVEL_MAX);
  gBgScroll[1].x = 0;
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1,
                             work->sheet + ECLIPSE_FLAME_BAND, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA,
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
    const void *mesh = Ram_MapCellBuffer + 0x1000;
    void *vertices = gMapGridCellBuffer;
    const void *bandMesh = Ram_MapCellBuffer + 0x2800;

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
        // A constant here and in p below, so both share one pool word (as
        // gData_02014000, 70 to 786 differ).
        struct Particle *q = (struct Particle *)(Ram_MapCellBuffer + 0x4000) +
                             ((frame * 16 + i) & 511);
        // Default: set for real below.
        s32 a = 0;
        s32 r = 0;

        a = Random16();
        r = (Random16() & 15) + frame * 2;
        q->x = Trig_Sin(a) * r + px(60);
        q->y = Trig_Cos(a) * r + px(60);
        q->velocityX = 0;
        q->velocityY = 0;
        q->variant = (Random16() & 15) + 16;
      }
      {
        const u16 *sizeOffsets = gBattleEffect_MotePictures;
        for (i = 0; i != 512; i++) {
          p = (struct Particle *)(Ram_MapCellBuffer + 0x4000) + i;
          if (p->variant >= 0) {
            s32 n = (p->variant >> 3) + 1;
            // sizeOffsets[n - 1] spelled as a byte offset: plain, 49 differ.
            s32 off = n * 2 - 2;

            blit[0](canvas,
                    sprites + *(const u16 *)((const u8 *)sizeOffsets + off),
                    (Q_16_16_TO_INT(p->x)) - n / 2, (Q_16_16_TO_INT(p->y)) - n,
                    n, n * 2);
            BattleEffectKernels_IntegrateVector2(p, 60, -px(1) / 4);
            p->variant--;
          }
        }
      }
      model = Render_AllocateModelList(1);
      shade = 0;
      if (frame <= 31) {
        s32 s = 0x10000;

        if (frame > 15) {
          s += 0x10000 - (frame << 12);
        }
        landingTexture.logWidth = 5;
        landingTexture.logHeight = 2;
        landingTexture.pixels = work->sheet + ECLIPSE_GLOW_BAND_B;
        model->mesh = gBattleEffect_SmallSquareMesh;
        model->shade = shade;
        model->vertices = gMapGridCellBuffer;
        model->kind = MODEL_SHADED_MAX;
        model->texture = &landingTexture;
        Transform_Reset();
        Transform_ApplyTranslation(-px(2), px(16), 0);
        Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        Transform_ApplyRoll(TRIG_TURN * 19 / 32);
        Transform_ApplyAxisScale(s, Q16_ONE * 2, Q16_ONE / 2);
        Render_TransformVertices(gBattleEffect_BeamCorners, gMapGridCellBuffer,
                                 4);
        Render_DrawModelList(model);
      }
      if (frame == 0) {
        Sound_PlayFar(SE_BANE_ESCAPE);
        Mesh_BuildGrid(Ram_MapCellBuffer + 0x1000, 16, 8, 8, 8);
        {
          s32 ringAngle = 0;
          for (ring = 0, offset = 0; ring != 8;
               ringAngle += 0x888, offset += 64, ring++) {
            s32 y = Q_16_16_TO_INT(-Trig_Cos(ringAngle) * 96);
            s32 rr = Q_16_16_TO_INT(Trig_Sin(ringAngle) * 48);
            struct ModelVertex *v =
                (struct ModelVertex *)(Ram_MapCellBuffer + 0x800);

            k = 0;
            v = (struct ModelVertex *)((u8 *)v + offset);
            for (; k != 16; k++) {
              SET_POSITION(v, Q_16_16_TO_INT(Trig_Sin(k << 12) * rr), y,
                           Q_16_16_TO_INT(Trig_Cos(k << 12) * rr));
              v++;
            }
          }
        }
        Mesh_BuildCylinderBand(Ram_MapCellBuffer + 0x2000,
                               Ram_MapCellBuffer + 0x2800, 32);
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
        s32 s = (frame << 9) + 0x8000;

        if (s > 0x10000) {
          s = 0x10000;
        }
        Transform_Reset();
        Transform_ApplyTranslation(-px(2), px(12), 0);
        Transform_ApplyPitch(TRIG_TURN / 8);
        Transform_ApplyYaw(-frame << 10);
        Transform_ApplyUniformScale(s);
        Render_TransformVertices(Ram_MapCellBuffer + 0x800, vertices, 128);
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
            Transform_Reset();
            Transform_ApplyTranslation(-px(2), px(32), 0);
            Transform_ApplyAxisScale(s / 4, s / 4, s / 4);
            Transform_ApplyPitch(10000);
            Transform_ApplyYaw((frame + i * 8) << 10);
            Transform_ApplyAxisScale(Q16_ONE, Q16_ONE * 6, Q16_ONE);
            Transform_ApplyTranslation(0, px(gEclipse_RingHeights[i]), 0);
            Render_TransformVertices(Ram_MapCellBuffer + 0x2000, vertices, 64);
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
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9,
                             work->sheet + FORM_CHANGE_GLOW_BAND, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, Data_02012000, 0, 0);
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
        Iwram_CopyWordsEntry(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
            SHEET_PALETTE_BYTES);
      else
        Iwram_CopyWordsEntry(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_OrangePaletteC),
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
            variant += 7;
          }
        }
        work->shakeFrames = 8;
        Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      flashes = FLASHES;
      if (frame >= 48 && frame <= 55) {
        s32 x = px((Random16() & 63) + center.x / 2 - 32);
        s32 y = px((Random16() & 31) + center.y);
        s32 k = 0;
        s32 spread = 15;
        struct Particle *burst = &flashes[index];

        for (; k != 2; k++) {
          struct Particle *flash = &burst[k];
          s32 speed = 31; // Set first: the mask is the tied operand.
          s32 angle;

          speed &= Random16();
          angle = Random16();

          flash->x = x + px((Random16() & spread) - 8);
          flash->y = y + px((Random16() & spread) - 8);
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
                    Q_16_16_UPPER_HALF(flash->x) - 16,
                    Q_16_16_UPPER_HALF(flash->y) - 48, FLASH_BURST_WIDTH,
                    FLASH_BURST_HEIGHT);
            BattleEffectKernels_IntegrateVector2(flash, 62, -px(1) / 32);
            flash->variant++;
          }
        }
      }
    }

    if (frame >= 0) {
      s32 cx = center.x / 2 << 16;
      s32 cy = px(center.y);
      struct RenderModel *model;
      void *corners;
      s32 bias;

      // The glow on the canvas map widens for 56 frames.
      if (frame <= 63) {
        s32 size = 128;
        s32 width = frame + 8;

        if (width <= 64) {
          size = width * 2;
          BattleEffect_DrawShadedCircles(Ram_MapCellBuffer, size, Q16_ONE);
        }
        Canvas_BlitAtFoot(Ram_MapCellBuffer, Q_16_16_TO_INT(cx),
                          Q_16_16_TO_INT(cy), size);
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
          Transform_Reset();
          {
            s32 column = i & 3;

            Transform_ApplyTranslation(cx + px(column * 2 + 2) - px(64),
                                       cy - px(column + 2) - px(64), 0);
          }
          Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE);
          Transform_ApplyRoll(ring->x);
          Transform_ApplyAxisScale(Trig_Sin(ring->variant * 682) + TRIG_TURN,
                                   0x20000, 0x10000);
          Render_TransformVertices(gBattleEffect_TaperedBeamCorners, corners,
                                   4);
          Render_DrawModelList(model);
        }
      }

      // The four crystals grow out of the ground from frame 48 on.
      model->texture = &texture;
      model->vertices = corners;
      texture.logWidth = 6;
      texture.pixels = Data_02012000;
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
            s32 growth =
                (frame - start) * gFormChange_WaveGrowth[i] * 2000 + 0x4000;

            model->shade = bias;
            Transform_Reset();
            Transform_ApplyTranslation(px(center.x / 2 - 60), px(32), 0);
            Transform_ApplyAxisScale(
                growth * 2, gFormChange_WaveStretch[i] * growth + 0x40000,
                growth / 4);
            Transform_ApplyPitch(1000);
            if (i & 1)
              Transform_ApplyYaw((i * 8 + frame) << 10);
            else
              Transform_ApplyYaw((i * 8 - frame) << 10);
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
    case 0:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      break;
    case 1:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      Iwram_FillWords(source, CANVAS_BYTES, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(1),
                                       Q16_ONE / 4);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)BG_CHAR_BLOCK(1),
                         CANVAS_BYTES);
      break;
    case 4:
      ColorBuffer_Brighten(source, work->transferValue,
                           (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
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
    case 0:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      break;
    case 1:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      Iwram_FillWords(source, BATTLE_BACKGROUND_BYTES, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)BG_CHAR_BLOCK(2),
                          BATTLE_BACKGROUND_BYTES);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(2),
                                       BATTLE_BACKGROUND_BYTES);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)BG_CHAR_BLOCK(2),
                         BATTLE_BACKGROUND_BYTES);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
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
    case 0:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      break;
    case 1:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      Iwram_FillWords(source, 0x3c00, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)BG_CHAR_BLOCK(2), 0x3c00);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(2),
                                       0x3c00);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)BG_CHAR_BLOCK(2),
                         0x3c00);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
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

/// One step of the background palette fade.
#define CANVAS_FADE_STEP 1092

void Palette_StepFadeTransfer(void) {
  u8 *work = Ram_HeapSlots->battleEffect;
  s32 *framesLeft;
  struct BattleWork *battle;

  framesLeft = (s32 *)(work + 0x77b4);
  battle = Ram_HeapSlots->battleWork;

  if (*framesLeft > 0) {
    s32 *step = (s32 *)(work + 0x77b8);

    (*step)++;
    ColorBuffer_ScaleFar(battle->palette, (void *)&BG_PLTT_COLOR(6, 0),
                         Q16_ONE - *step * CANVAS_FADE_STEP, 128);
    (*framesLeft)--;
  }
}

/// Clears every target's flash timer.
void BattleEffect_ClearActorTimers(void) {
  Iwram_ClearWords(Ram_HeapSlots->battleEffect + 0x78dc, 8);
}

/// Counts down each target's flash timer; when one runs out, the target gets
/// its own palette back.
void BattleEffect_CountDownBattlerTints(void) {
  u8 *base;
  s32 i;
  s32 index;

  base = Ram_HeapSlots->battleEffect;
  i = 0;
  do {
    if (base[0x78dc + i] != 0) {
      if ((base[0x78dc + i] = base[0x78dc + i] - 1) == 0) {
        index = i * 2 + 36;
        BattleEffect_TintBattler(*(s16 *)(*(u8 **)(base + 0x78f0) + index),
                                 SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
      }
    }
    i += 1;
  } while (i != 8);
}

/// Shows the canvas on BG2 at half width across the whole screen, blended over
/// the battle, inside window 0.
void BattleEffect_SetupCanvasWindows(void) {
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(14);
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
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
  struct BattleBackgroundView *display =
      (struct BattleBackgroundView *)Ram_HeapSlots->battleBackground;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  wait(1);

  BattleBackground_SetModeFar(1, battle->background, 0);
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
  display->secondMode = 0;
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
  struct BattleBackgroundView *display =
      (struct BattleBackgroundView *)Ram_HeapSlots->battleBackground;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_HALFWORD);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  wait(1);
  BattleBackground_SetModeFar(1, battle->background, 128);
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
  display->secondMode = 0;
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
void BattleEffect_StartTiledCanvas(s32 bg_control) {
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_StartCanvasLayer(bg_control);
  REG_BG2CNT = bg_control | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 tile;
      s16 pair;

      tile = row * 16 + col * 2;
      pair = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + offset) = pair;
    }
    for (col = 0; col != 8; col++, offset += 2) {
      *(volatile u16 *)(0x06003800 + offset) = 0;
    }
  }
}

/// Opens the canvas layer with the canvas repeated across a wrapping map, every
/// tile numbered in order.
void BattleEffect_StartTiledCanvasFilled(s32 bg_control) {
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_StartCanvasLayer(bg_control);
  REG_BG2CNT = bg_control | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 tile;
      s16 pair;

      tile = row * 16 + col * 2;
      pair = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + offset) = pair;
    }
    for (col = 0; col != 8; col++, offset += 2) {
      *(volatile u16 *)(0x06003800 + offset) = 0xf0f;
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

  BattleBackground_SetModeFar(2, battle->background, 7);
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

  flag = &((struct BattleBackgroundView *)Ram_HeapSlots->battleBackground)
              ->secondMode;
  battle = Ram_HeapSlots->battleWork;
  *flag = 1;
  IoWriteQueue_Store16((u32)&REG_DISPCNT,
                       DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON |
                           DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1);
  wait(1);
  BattleBackground_SetModeFar(2, battle->background, 0);
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
  struct BattleBackgroundView *layer =
      (struct BattleBackgroundView *)Ram_HeapSlots->battleBackground;
  s32 row;
  s32 col;
  s32 off;

  BattleEffect_ClearActorTimers();
  BattleBackground_SetModeFar(2, screen->background, 0);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  layer->secondMode = 0;
  Iwram_ClearWords((void *)0x06003fc0, 64);
  Iwram_FillWords((void *)0x0600f900, 0x200, -1);
  off = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    for (col = 0; col != 32; col++, off += 2) {
      if (col > 15)
        *(volatile u16 *)(0x0600fb00 + off) = 0xff;
      else
        *(volatile u16 *)(0x0600fb00 + off) = row * CANVAS_TILES + 0x100 + col;
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
