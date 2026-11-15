/// @file
/// Meteor Blow.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct MeshTriangle;
struct StripQuad;

/// One star of the opening sky: a point on a sphere and its cell kind.
struct MeteorStar {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
};

extern const struct Scale gMeteorBlow_DoubleScale;

/* VIEW(meteor_blow): tag struct RenderModel as struct RenderModel__meteor_blow;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunMeteorBlow moves: 1 of 1545 instructions in
 * tla-en. */
struct RenderModel__meteor_blow {
  s32 kind;
  s32 flags;
  const u16 *mesh;
  s16 *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

/// Where DustBlobSheet is decoded in the effect's pictures.
#define METEOR_BLOW_DUST_BLOB_PICTURES 0x2000

#define METEOR_BLOW_FRAMES 50

/// Where BillowTexture is decoded in the effect's pictures.
#define METEOR_BLOW_BILLOW 0x4000

/// Where FlameBandTexture is decoded in the effect's pictures.
#define METEOR_BLOW_FLAME_BAND 0x5000

/// Meteor Blow.
void BattleEffect_RunMeteorBlow(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *map;
  // Unused: blit[1] gives the stack frame its 4 bytes.
  DrawRectangle blit[2];
  u8 *sprites;
  u8 *projected;
  u8 *meshA;
  u8 *triangles;
  s32 *transition;
  struct MeteorStar *stars;
  s32 pitch;
  s32 yaw;
  s32 x;
  s32 z;
  s32 size;
  s32 scale;
  s32 hx;
  s32 hz;
  void *verts;
  s32 x0;
  s32 half;
  s32 blank;
  s32 mode;
  s32 offset;
  s32 frame;
  s32 arcAngle;
  s32 j;
  s32 star;
  s32 i;
  s32 *starKind;
  struct MeteorStar *current;
  s32 position[3];
  s32 screen[3];
  s32 point[3];
  s32 place[4];
  s32 summonPlace[4];
  struct ModelTexture waveTexture;
  struct Scale summonScale;
  s32 req[2];
  struct ModelTexture texture;

  canvas = Ram_HeapSlots->battleCanvas;
  map = gMapGridCellBuffer;
  work = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  blank = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSetSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet,
                             work->sheet + METEOR_BLOW_DUST_BLOB_PICTURES, 1,
                             1);
  Iwram_CopyWordsEntry(sprites, work->sheet + 0x1c2, 30);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       SHEET_PALETTE_BYTES);
  Graphics_LoadGreyRamp();
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  IWRAM_FILL_WORDS(map, 0x8000, 0x01010101);
  Iwram_CopyWordsEntry((void *)BG_CHAR_BLOCK(2), map, 0x7800);
  BattleEffect_SelectLivingTargets(work->args);
  BattleEffect_SpawnObjects(1, 0x28a, 1);
  REG_BLDCNT = blank;
  REG_BG2PA = Q_8_8(1);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
  Iwram_CopyWordsEntry((void *)BG_CHAR_BLOCK(1), canvas, 0x4000);
  Task_Add(BattleEffect_FlushPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  projected = work->sheet + 0x5740;
  meshA = work->sheet + 0x5f40;
  triangles = work->sheet + 0x6340;
  transition = Ram_HeapSlots->battleBackground;
  stars = (struct MeteorStar *)(work->sheet + 0x7a4);
  // Default: the flight sets these at frame 0.
  pitch = 0;
  yaw = 0;
  x = 0;
  z = 0;
  size = 0;
  scale = Q16_ONE;
  Mesh_BuildGrid(triangles, 2, 16, 32, 2);
  for (offset = 0, frame = 0, arcAngle = 0; frame != 16;
       arcAngle += 1092, offset += 8, frame++) {
    s32 y = Q_16_16_TO_INT(-Trig_Cos(arcAngle) * 48);
    s32 r = Q_16_16_TO_INT(Trig_Sin(arcAngle) * 48);
    struct ModelVertex *v;

    j = 0;
    v = (struct ModelVertex *)(meshA + offset);
    for (; j != 2; j++, v++) {
      v->x = Q_16_16_TO_INT(Trig_Sin(j << 13) * r);
      v->y = y;
      v->z = Q_16_16_TO_INT(Trig_Cos(j << 13) * r);
    }
  }
  mode = 3;
  BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 8, 7, mode, 1);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  // DEAD: both fields are set again before anything reads them.
  EFFECT_SET_TRANSFER(work, mode, CANVAS_FILL_WORD(4));
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Task_Add(BattleEffect_ArmMeteorBlowPaletteDma, TASK_PASS_FRAME + 126);
  gBgScroll[1].x = 0;
  transition[4] = 1;
  starKind = &stars->kind;
  current = stars;
  for (star = 0; star != 256; star++, starKind += 4, current++) {
    s32 a = (star & 255) << 7;
    s32 r = 160;
    s32 b;

    current->y = Trig_Sin(a) * r;
    r = Q_16_16_TO_INT(Trig_Cos(a) * r);
    b = Random16() & 0x7fff;
    current->x = Trig_Sin(b) * r;
    current->z = Trig_Cos(b) * r;
    if ((star & 31) == 0) {
      *starKind = (star / 32) % 7;
    } else {
      *starKind = 7;
    }
  }

  // The flight through the stars.
  for (frame = 0;
       frame != 160 && !(gInput.repeat & KEY_A) && !(gInput.repeat & KEY_B);
       frame++) {
    s32 fade;
    s32 speed;

    if (frame == 0) {
      x = px(272);
      z = px(88);
      size = 0x400;
      pitch = 0xd00;
      yaw = 0x2500;
      work->frame = 8;
      Sound_PlayFar(SE_WAVE);
    }
    if (frame == 120) {
      Sound_PlayFar(SE_163);
    }
    fade = frame - 96;
    if ((u32)fade <= 16) {
      REG_BLDALPHA = BLEND_ALPHA(16 - fade, BLEND_WEIGHT_MAX);
    }
    if (frame == 115) {
      REG_BLDALPHA = EFFECT_BLEND_ADD;
    }
    if (frame == 116) {
      Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
      Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2CNT, 0x784, IO_WRITE_HALFWORD);
    }
    if (frame > 128) {
      BattleEffect_StepPaletteToResource((s32)&ResourceId_OrangePaletteB);
    }
    if (frame == 117) {
      work->frame = 45;
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        work->particles[i].variant = -1;
      }
      scale = Q16_ONE;
    }
    // DEAD: frame never reaches 1600 here; the source keeps the test.
    if (frame <= 1599) {
      u16 *dst = (u16 *)((u8 *)work + 0x7640);
      s32 value = work->frame;
      s32 base = value / 2 + 16;

      if (frame > 79) {
        work->frame++;
        if (work->frame > 236) {
          work->frame = 236;
        }
      }
      for (i = 0; i != 15; i++) {
        *dst++ = 0;
      }
      for (; i != 135; i++) {
        s32 g = i - 16;
        s32 b = g / 4;

        value = b + base;
        b = value - 32;
        g = value - 80;

        if (b < 0)
          b = 0;
        if (b > RGB_LEVEL_MAX)
          b = RGB_LEVEL_MAX;
        if (g < 0)
          g = 0;
        if (g > RGB_LEVEL_MAX)
          g = RGB_LEVEL_MAX;
        *dst++ = RGB(g >> 1, g, b);
      }
      for (; i != 160; i++) {
        *dst++ = 0;
      }
    }

    if (frame > 115) {
      struct RenderModel *model;
      s32 shade;
      s32 scroll;
      s32 angle;

      model = Render_AllocateModelList(1);
      shade = frame * 4 - 528;
      if (shade > 0) {
        shade = 0;
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      texture.pixels = work->sheet + METEOR_BLOW_DUST_BLOB_PICTURES;
      model->kind = MODEL_SHADED_MAX;
      model->texture = &texture;
      model->mesh = triangles;
      model->vertices = projected;
      model->shade = shade;
      model->scrollU = 0;
      if (frame > 127) {
        s32 d = (frame << 6) - 0x2000;

        if (d > 1023) {
          d = 1024;
        }
        scale += d;
        Transform_ApplyAxisScale(scale, scale, scale);
      }
      i = 0;
      angle = -TRIG_TURN / 4;
      scroll = frame * 2;
      for (; i != 5; i++) {
        model->scrollV = ~scroll & 127;
        Transform_Reset();
        Transform_ApplyTranslation(-px(2), -px(64), 0);
        Transform_ApplyAxisScale(42000, 0x24650, Q16_ONE);
        Transform_ApplyAxisScale(scale, scale, scale);
        Transform_ApplyRoll(TRIG_TURN / 2);
        Transform_ApplyYaw(angle);
        Render_TransformVertices(meshA, projected, 32);
        Render_DrawModelList(model);
        angle += 0x1800;
        scroll += 20;
      }
      Heap_Release(model);
    } else {
      // DEAD: frame is at most 115 here, so the second arm never runs; the
      // source keeps both tests.
      if (frame <= 159) {
        pitch = Q_16_16_TO_INT((pitch << 16) + (256 << 16));
      } else if (frame <= 191) {
        pitch = Q_16_16_TO_INT(((u16)pitch - frame * 8 + 1216) << 16);
      }
      if (frame <= 47) {
        yaw = Q_16_16_TO_INT((yaw << 16) + (256 << 16));
      } else if (frame <= 111) {
        yaw = Q_16_16_TO_INT(((u16)yaw - frame * 4 + 448) << 16);
      }
      position[0] = 120;
      position[1] = 64;
      position[2] = 0;
      Transform_Reset();
      Transform_ApplyPosition(position);
      Transform_ApplyPitch((u16)pitch);
      Transform_ApplyYaw((u16)yaw);
      if (frame <= 112) {
        struct MeteorStar *s = stars;

        for (i = 0; i != 256; i++, s++) {
          s32 kind = s->kind;

          point[0] = Q_16_16_UPPER_HALF(s->x);
          point[1] = Q_16_16_UPPER_HALF(s->y);
          point[2] = Q_16_16_UPPER_HALF(s->z);
          Render_ProjectPoint(point, screen);
          if (screen[2] > 0) {
            if (kind == 7) {
              blit[0](gMapGridCellBuffer, sprites + i % 30, screen[0],
                      screen[1], 1, 1);
            } else {
              blit[0](gMapGridCellBuffer,
                      work->sheet + gMeteorBlow_Pictures[kind], screen[0],
                      screen[1], gMeteorBlow_PictureWidths[kind],
                      gMeteorBlow_PictureHeights[kind]);
            }
          }
        }
      } else {
        Iwram_ClearWordsEntry(gMapGridCellBuffer, 0x7800);
      }
    }

    req[1] = size;
    req[0] = size;
    place[3] = 0;
    place[1] = px(255);
    place[0] = x;
    place[2] = z;
    Render_ApplyProjectedPlacementFar(work->sprites[0], place, req, 0);
    speed = px(1) / 32;
    {
      s32 a = 0x6000;

      if (frame > 23) {
        a = a - frame * 286 + 0x11e0;
        if (a < 0) {
          a = 0;
        }
      }
      if (frame > 87) {
        speed = speed - (frame << 6) + 0x1600;
        if (speed < 0) {
          speed = 0;
        }
      }
      x -= (Trig_Sin(a) * speed) >> 10;
      z += (Trig_Cos(a) * speed) >> 10;
      size += 512;
    }
    if (frame > 127) {
      speed = (frame << 13) - 0x100000;
      if (speed > 0x1ffff) {
        speed = px(2);
      }
      z += speed;
      size += 512;
    } else if (size > 0x10000) {
      size = 0x10000;
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  wait(1);
  transition[4] = 0;
  Task_Remove(BattleEffect_ArmMeteorBlowPaletteDma);
  if (frame <= 115) {
    Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
  } else {
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_SetupBlendedDisplay();
  // DEAD: frame 0 sets hx and hz again before any read.
  hx = 200;
  hz = 0;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BG2Y = -0x1000;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG0 |
               WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2;
  REG_WIN0V = WIN_RANGE(16, 124);
  REG_WIN1V = WIN_RANGE(0, 136);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA,
                             work->sheet + METEOR_BLOW_BILLOW, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1,
                             work->sheet + METEOR_BLOW_FLAME_BAND, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       SHEET_PALETTE_BYTES);
  {
    s32 *q = (s32 *)((s32 *)(Ram_MapCellBuffer + 0x18));

    for (i = 0; i != 256; i++, q += 7) {
      *q = 0;
    }
  }
  Palette_BrightenBattleBg(5, -4, -12);

  // The impact: the meteor lands, rings of fire and shards fly.
  for (frame = 0; frame != METEOR_BLOW_FRAMES; frame++) {
    struct RenderModel__meteor_blow *model;
    s32 angle;
    s32 back;
    s32 shade;
    s32 step;

    if (frame == 0) {
      hx = 144;
      hz = 0;
    }
    if (frame <= 33) {
      summonScale = gMeteorBlow_DoubleScale;
      summonPlace[3] = 0;
      summonPlace[1] = 0xff0000;
      summonPlace[0] = px(hx);
      summonPlace[2] = px(hz);
      if (frame <= 15) {
        hx -= 1;
        hz += 9;
      } else {
        hz += 2;
      }
      Render_ApplyProjectedPlacementFar(work->sprites[0], summonPlace,
                                        (s32 *)&summonScale, 0);
    }
    if (frame == 34) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if (frame == 16) {
      struct Particle *p;
      s32 count = 256;

      p = (struct Particle *)gMapGridCellBuffer;
      i = 0;
      half = px(hx / 2);
      /* FAKEMATCH(dead): updates of hx that cancel out end the shard's halving
       * here. */
      hx++;
      hx--;
      for (; i != count; i++, p++) {
        s32 r;
        s32 a;

        /* FAKEMATCH(dead): updates of count. */
        count++;
        count--;
        r = (Random16() & 511) + count;
        a = (Random16() & 0x7fff) + 0x4000;

        p->x = half;
        p->y = px(114);
        p->velocityX = (Trig_Sin(a) * r) >> 6;
        p->velocityY = (Trig_Cos(a) * r * 2) >> 6;
        p->variant = (Random16() & 7) + 24;
      }
      work->shakeFrames = 4;
      Sound_PlayFar(SE_CRASH);
      for (i = 0; i != args->targetCount; i++) {
        BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 8);
      }
    }

    verts = Heap_AllocEwram(1024);
    model = Render_AllocateModelList(1);
    shade = 0;
    model->texture = &waveTexture;
    model->vertices = verts;
    model->shade = shade;
    waveTexture.logWidth = 6;
    waveTexture.logHeight = 6;
    waveTexture.pixels = work->sheet + METEOR_BLOW_BILLOW;
    model->kind = MODEL_SHADED_ADDITIVE;
    model->mesh = gBattleEffect_RingMesh;
    x0 = hx / 2 - 60;
    step = frame << 11;
    i = 0;
    back = -step;
    angle = step;
    for (; i != 4; i++, back += TRIG_TURN / 8, angle += TRIG_TURN / 8) {
      s32 start = gMeteorBlow_WaveStarts[i] + 16;

      if (frame > start) {
        shade = (start - frame) * 8 + 56;

        if (shade > -16) {
          shade = -16;
        }
        if (shade > MODEL_SHADE_INVISIBLE) {
          s32 s = (frame - start) * gMeteorBlow_WaveGrowth[i] * 2000 + 42768;

          model->shade = shade;
          Transform_Reset();
          Transform_ApplyTranslation(px(x0), px(40), 0);
          Transform_ApplyAxisScale(s, s * gMeteorBlow_WaveStretch[i], s / 4);
          Transform_ApplyPitch(1000);
          if (i & 1) {
            Transform_ApplyYaw(angle);
          } else {
            Transform_ApplyYaw(back);
          }
          Render_TransformVertices(gBattleEffect_RingCorners, verts, 32);
          Render_DrawModelList(model);
        }
      }
    }
    model->mesh = gBattleEffect_CylinderMesh;
    waveTexture.pixels = work->sheet + METEOR_BLOW_FLAME_BAND;
    waveTexture.logWidth = 7;
    waveTexture.logHeight = 7;
    half = ((hx + (s32)((u32)hx >> 31)) >> 1) - 64;
    for (i = 0; i != 3; i++) {
      s32 start = gMeteorBlow_RingStarts[i] + 8;

      if (frame > start) {
        shade = (start - frame) * 8 + 40;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > MODEL_SHADE_INVISIBLE) {
          s32 s = (frame - start) * gMeteorBlow_RingGrowth[i] * 1500 + 0x4180;

          model->shade = shade;
          model->scrollU = ((frame + i * 5) & 7) << 4;
          Transform_Reset();
          Transform_ApplyTranslation(px(half), px(gMeteorBlow_RingHeights[i]),
                                     0);
          Transform_ApplyAxisScale(s, s * 2, s);
          Transform_ApplyPitch(2000);
          Render_TransformVertices(gBattleEffect_CylinderCorners, verts, 32);
          Render_DrawModelList(model);
        }
      }
    }
    Heap_Release(model);
    Heap_Release(verts);

    {
      struct Particle *p;

      p = (struct Particle *)gMapGridCellBuffer;
      for (i = 0; i != 256; i++, p++) {
        if (p->variant >= 0) {
          s32 n = (p->variant >> 2) + 1;

          blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                  Q_16_16_UPPER_HALF(p->x) - n / 2,
                  Q_16_16_UPPER_HALF(p->y) - n, n, n * 2);
          p->variant--;
          BattleEffectKernels_IntegrateVector2(p, 56, px(1) / 8);
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Sprite_DestroyFar(work->sprites[0]);
  BattleEffect_EndCanvasLayer();
}
#undef BLITTER
