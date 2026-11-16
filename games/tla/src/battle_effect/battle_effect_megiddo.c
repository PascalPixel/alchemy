/// @file
/// The Megiddo summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct StripQuad;

/// Where DustBlobSheet is decoded in the effect's pictures.
#define MEGIDDO_DUST_BLOB_PICTURES 0x2400

#define MEGIDDO_FRAMES 188

/// Where FireRingTexture is decoded in the map cell buffer.
#define MEGIDDO_FIRE_RING 0x4000

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define MEGIDDO_GLOW_BAND 0x800

/// Where FireStripesTexture is decoded in the effect's pictures.
#define MEGIDDO_FIRE_STRIPES 0x1000

/// Megiddo: the caster rises in a column of light, a winged figure gathers
/// above and dives in a storm of feathers and rings onto the target.
///
/// Abilities: Megiddo.
void BattleEffect_RunMegiddo(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  u8 *canvas;
  s32 frame;
  s32 f;
  s32 k;
  s32 zero;
  u8 *ring;
  DrawRectangle blit[2];
  u8 *sprites;
  s32 *transition;
  struct BattleCamera *camera;
  s16 savedYaw;
  u8 *meshA;
  u8 *meshB;
  s32 casterValue;
  struct MotionObject *caster;
  FIXED x;
  FIXED z;
  FIXED scale;
  u8 *transformed;
  u8 *strip;
  u8 *fireRing;
  s32 i;
  s32 j;
  s32 m;
  struct RenderModel *model;
  void *verts;
  struct Particle *p;
  struct ScreenPoint pos;
  s16 list[UNIT_MAX];
  s16 targetList[14];
  FIXED place[4];
  s32 screen[3];
  u8 req[8];
  struct ModelTexture scratchTexture;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  transition = Ram_HeapSlots->battleBackground;
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = 0;
  zero = 0;
  ring = gMapGridCellBuffer;
  transformed = Ram_MapCellBuffer + 0x1000;
  strip = Ram_MapCellBuffer + 0x2000;
  fireRing = Ram_MapCellBuffer + MEGIDDO_FIRE_RING;
  BattleMotion_ApproachTargetFar(args->user, args->user, 150, toFIXED(5));
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  meshA = work->sheet + 0x5f40;
  meshB = work->sheet + 0x6340;
  caster = BattleObject_GetFar(args->user)->object;
  x = 0;
  z = 0;
  scale = FIXED_ONE;
  Render_BuildQuadStrip(strip, 8, 16, 32);
  Render_BuildQuadStrip(meshB, 2, 32, 15);
  for (j = 0; j != 16; j++) {
    FIXED c = -Trig_Cos(j * 1092) * 3;
    s32 s = Trig_Sin(j * 1092);
    s32 y = FIXEDtoINT(c * 16);
    s32 r = FIXEDtoINT(s * 48);

    for (m = 0; m != 2; m++) {
      struct ModelVertex *v = (struct ModelVertex *)(meshA + j * 8 + m * 4);

      SET_POSITION(v, FIXEDtoINT(Trig_Sin(m << 13) * r), y + 24,
                   FIXEDtoINT(Trig_Cos(m << 13) * r));
    }
  }
  casterValue = caster->verticalMotionStrength;
  Actor_SetAnimationSpeedFar(caster, 0);
  caster->velocityX = 0;
  caster->velocityY = 0;
  caster->velocityZ = 0;
  caster->acceleration = 0;
  caster->verticalMotionStrength = 0;

  for (frame = 0; frame != MEGIDDO_FRAMES; frame++) {
    if (frame == 4) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 28) {
      Sound_PlayFar(SE_SPIN);
    }
    if (frame == 84) {
      Sound_PlayFar(SE_LAUNCH);
    }
    if (frame == 143) {
      Sound_PlayFar(SE_191);
    }
    if (frame == 171) {
      Sound_PlayFar(SE_CRASH);
    }
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 3 && frame <= 142) {
      frame = 143;
    }
    if (frame == 0) {
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
                                 Ram_MapCellBuffer + MEGIDDO_FIRE_RING, 0, 0);
      Resource_LoadAndDecompress(RESOURCE_FLAME_GRADIENT_TEXTURE, work->sheet,
                                 0, 0);
      Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                                 work->sheet + MEGIDDO_GLOW_BAND, 0, 0);
      Resource_LoadAndDecompress(RESOURCE_FIRE_STRIPES_TEXTURE,
                                 work->sheet + MEGIDDO_FIRE_STRIPES, 1, 1);
      Resource_LoadAndDecompress(RESOURCE_DUST_BLOB_SHEET,
                                 work->sheet + MEGIDDO_DUST_BLOB_PICTURES, 1,
                                 0);
      Graphics_LoadGreyRamp();
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      if (!args->side) {
        x = toFIXED(160);
      } else {
        x = toFIXED(80);
      }
      z = -toFIXED(96);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2CNT,
                          BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                              BGCNT_SCREEN_BASE(7),
                          IO_WRITE_HALFWORD);
    }
    if (frame == 1) {
      transition[4] = 1;
      list[0] = args->user;
      list[1] = 255;
      BattleActor_SpawnObjectsForListFar(list, 0);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_RED_HAZE_BACKDROP, 0);
      BattleEffect_BuildRepeatingTilemap();
      Palette_BrightenBattleBg(8, 2, 2);
      BattleEffect_SpawnObjects(1, SPRITE_SUN, 2);
    }
    EffectPosition_ProjectScaledBattler(args->user, &pos);
    if (frame == 3) {
      savedYaw = camera->yaw;
    }
    if ((u32)(frame - 16) <= 61) {
      if (!args->side) {
        camera->yaw -= 256;
      } else {
        camera->yaw += 256;
      }
    }

    for (k = 0; k != 33; k++) {
      for (i = 0; i != 2; i++) {
        struct ModelVertex *v = (struct ModelVertex *)(ring + k * 8 + i * 4);
        s32 r = 80;

        if (i != 1) {
          r = (FIXEDtoINT(Trig_Sin((frame + k * 4) << 12) * 2)) + 120;
        }
        SET_POSITION(v, FIXEDtoINT(Trig_Sin(k << 11) * r),
                     FIXEDtoINT(Trig_Cos(k << 11) * r), zero);
      }
    }

    {
      s32 y = gBgScroll[1].y;
      s32 d;

      if (y > 104) {
        y -= toFIXED(1);
      }
      if (frame <= 63) {
        d = -16;
      } else if ((u32)(frame - 64) <= 7) {
        s32 twice = frame * 2;

        /* FAKEMATCH(barrier): an empty asm on twice that claims to clobber r3.
         */
        asm("" : "+l"(twice) : : "r3");
        d = twice - 144;
      } else if ((u32)(frame - 72) <= 11) {
        d = 0;
      } else if (frame > 154) {
        d = 16 - (frame - 155) / 2;
        if (d < 0) {
          d = 0;
        }
      } else {
        d = frame - 84;
        if (d > 16) {
          d = 16;
        }
        gProjection.centerY -= d;
      }
      y += d;
      if (y < -16) {
        y += 120;
      }
      if (y > 104) {
        y -= 120;
      }
      gBgScroll[1].y = y;
    }
    if ((u32)(frame - 42) <= 37) {
      z += toFIXED(2);
      BattleEffect_StepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
    }
    if ((u32)(frame - 84) <= 58) {
      if (!args->side) {
        x -= toFIXED(2);
      } else {
        x += toFIXED(2);
      }
      z += toFIXED(4);
    }
    if (frame == 143) {
      struct MotionObject *target;

      Palette_BrightenBattleBg(-12, -2, -2);
      camera->yaw = savedYaw;
      x = toFIXED(120);
      z = toFIXED(48);
      targetList[0] = args->targets[0];
      targetList[1] = 255;
      BattleActor_SpawnObjectsForListFar(targetList, 0);
      gProjection.centerY = 556;
      target = BattleObject_GetFar(args->targets[0])->object;
      target->x = zero;
      target->z = zero;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
    }
    if (frame > 154) {
      s32 d = 16 - (frame - 155) / 2;

      if (d < 0) {
        d = 0;
      }
      gProjection.centerY -= d;
    }
    if (frame == 84) {
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2CNT,
                          BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) |
                              BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7),
                          IO_WRITE_HALFWORD);
      Actor_SetAnimationSpeedFar(caster, 128);
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(6));
    }
    if (frame == 171) {
      work->shakeFrames = 8;
      Sound_PlayFar(SE_CRASH);
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));
    }
    /* FAKEMATCH(temporary): f, a copy of frame tested below. */
    f = frame;
    if (frame > 174) {
      Palette_BrightenBgEntries(2, 2, 2);
    }

    if (frame > 1) {
      struct Sprite *obj;
      s32 *pair;
      s32 v = 0; // Default: set before it is read.

      if (frame > 142) {
        s32 n = frame - 143;

        scale = n * 1236 + FIXED_ONE / 4;
        if (scale > FIXED_ONE * 3 / 4) {
          scale = FIXED_ONE * 3 / 4;
        }
      }
      pair = (s32 *)req;
      /* FAKEMATCH(barrier): an empty asm on pair. */
      asm("" : "+r"(pair));
      v = scale * 2;
      pair[1] = v;
      *(s32 *)req = v;
      place[3] = zero;
      place[1] = toFIXED(255);
      place[0] = x;
      place[2] = z;
      obj = work->sprites[0];
      obj->offsetY = 32;
      obj->offsetX = zero;
      if ((u32)(place[2] + toFIXED(52)) < toFIXED(252)) {
        Render_ApplyProjectedPlacementFar(obj, place, pair, 0);
      }
    }
    if (f > 1) {
      u8 textureBytes[8];
      struct ModelTexture *t;
      s32 tint;

      model = Render_AllocateModelList(1);
      t = (struct ModelTexture *)textureBytes;
      model->kind = MODEL_SHADED_MAX;
      // The texture's sizes go in as bytes (char stores), its pixels as a word.
      textureBytes[0] = 6;
      model->vertices = transformed;
      textureBytes[1] = 4;
      model->texture = t;
      model->shade = 0;
      model->scrollV = 0;
      /* FAKEMATCH(barrier): an empty asm that reads and writes *t. */
      asm("" : "+m"(*t));
      /* FAKEMATCH(barrier): the second asm on *t. */
      asm("" : "+m"(*t));
      /* FAKEMATCH(barrier): the third asm on *t. */
      asm("" : "+m"(*t));
      /* FAKEMATCH(dead): a once-only block. */
      do {
        t->pixels =
            work->sheet + MEGIDDO_FIRE_STRIPES + ((frame / 2) % 5) * 0x400;
      } while (0);
      model->mesh = strip;
      {
        FIXED half = FIXED_ONE / 2;

        /* FAKEMATCH(barrier): an empty asm on half. */
        asm("" : "+r"(half));
        Matrix_Initialize();
        Matrix_Scale(half, FIXED_ONE, half);
      }
      Matrix_Translate(x - toFIXED(126), z - toFIXED(76), 0);
      Matrix_ScaleUniform(46536);
      Matrix_ScaleUniform(scale);
      Render_TransformVertices(ring, transformed, 66);
      Render_DrawModelList(model);

      t->logWidth = 5;
      t->logHeight = 5;
      t->pixels = work->sheet + MEGIDDO_DUST_BLOB_PICTURES;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->mesh = meshB;
      tint = -24;
      model->flags = 3;
      model->shade = tint;
      model->scrollV = zero;
      if (frame > 142) {
        s32 n = frame - 143;
        // Default: scroll, angle and tilt are each set before they are read.
        s32 scroll = 0;
        s32 angle = 0;
        s32 tilt = 0;
        s32 twice;

        if (n > 32) {
          n = 32;
        }
        i = 0;
        /* FAKEMATCH(temporary): twice, frame * 2 kept apart. */
        twice = frame * 2;
        angle = -DEGtoANG(90);
        tilt = n * 384;
        scroll = twice + frame;
        for (; i != 10; i++) {
          model->scrollU = ~scroll & 127;
          Matrix_Initialize();
          Matrix_Translate(-toFIXED(2), -toFIXED(32), 0);
          Matrix_Scale(82000, 0x35208, FIXED_ONE);
          Matrix_ScaleUniform(scale);
          Matrix_RotateZ(DEGtoANG(180));
          scroll += 20;
          Matrix_RotateX(tilt - TRIG_TURN * 3 / 16);
          Matrix_RotateY(angle);
          Render_TransformVertices(meshA, transformed, 32);
          Render_DrawModelList(model);
          angle += TRIG_TURN * 3 / 32;
        }
      }
      Heap_Release(model);
    }

    if (frame == 8) {
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      p = work->particles;
      for (i = 0; i != ARRAY_COUNT(work->particles); i++, p++) {
        p->x = Random16() & 127;
        p->y = Random16() & 127;
      }
    }
    if ((u32)(frame - 8) <= 79) {
      s32 off = (frame * 2 - 16) & 127;

      p = work->particles;
      for (i = 0; i != ARRAY_COUNT(work->particles); i++, p++) {
        if (p->variant >= 0) {
          s32 size = 1;
          s32 mx = p->x;
          s32 y = p->y - off;

          if (y > 127) {
            y -= 128;
          }
          if (y < 0) {
            y += 128;
          }
          blit[0](canvas, sprites + gBattleEffect_MotePictures[size - 1], mx, y,
                  size, 2);
        }
      }
    }
    if (frame > 142) {
      s32 n = frame - 143;

      if (n > 32) {
        n = 32;
      }
      if (frame == 143) {
        p = work->particles;
        for (i = 0; i != ARRAY_COUNT(work->particles); i++, p++) {
          SET_POSITION(p, Random16() & 127, Random16() % 192 - 64,
                       Random16() & 127);
        }
      }
      Matrix_Initialize();
      Matrix_RotateX(n * 384 - TRIG_TURN * 3 / 16);
      p = work->particles;
      for (i = 0; i != ARRAY_COUNT(work->particles); i++, p++) {
        s32 size = 1;

        Render_ProjectPoint(p, screen);
        blit[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                screen[0], screen[1], size, 4);
        p->y -= 2;
      }
    }

    verts = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    scratchTexture.logWidth = 7;
    scratchTexture.logHeight = 7;
    scratchTexture.pixels = fireRing;
    model->kind = MODEL_SHADED_ADDITIVE;
    model->mesh = gBattleEffect_LargeSquareMesh;
    model->texture = &scratchTexture;
    model->vertices = verts;
    for (i = 0; i != 5; i++) {
      s32 start = gMegiddo_FlashStarts[i];

      if (frame >= start && frame < start + 20) {
        s32 t = frame - start;
        s32 shade = 64 - t * 8;
        s32 s;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > MODEL_SHADE_INVISIBLE) {
          s = (t << 14) + FIXED_ONE / 4;
          model->shade = shade;
          Matrix_Initialize();
          if (i <= 1) {
            Matrix_Translate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48),
                             0);
          } else {
            Matrix_Translate(0, toFIXED(16), 0);
          }
          Matrix_Scale(s, s * 2, s);
          Matrix_RotateX(gMegiddo_FlashPitches[i]);
          Matrix_RotateY(gMegiddo_FlashYaws[i]);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    model->kind = MODEL_SHADED_ADDITIVE;
    scratchTexture.logWidth = 5;
    scratchTexture.logHeight = 6;
    scratchTexture.pixels = work->sheet;
    model->mesh = gBattleEffect_TallMesh;
    for (i = 0; i != 8; i++) {
      s32 start = 16 + i * 2;

      if (frame >= start && frame < i * 2 + 80) {
        s32 t = frame - start;
        s32 shade = t * 8 - 64;
        s32 s = t << 11;

        if (shade > 0) {
          shade = 0;
        }
        if (s > FIXED_ONE) {
          s = FIXED_ONE;
        }
        if (t > 47) {
          s = s - (t << 12) + FIXED_ONE * 3;
        }
        model->shade = shade;
        Matrix_Initialize();
        if (!args->side) {
          Matrix_Translate(INTtoFIXED(pos.x / 2 + gMegiddo_RayX[i] - 64),
                           INTtoFIXED(gMegiddo_RayY[i] + pos.y - 54), 0);
        } else {
          Matrix_Translate(INTtoFIXED(pos.x / 2 - gMegiddo_RayX[i] - 62),
                           INTtoFIXED(gMegiddo_RayY[i] + pos.y - 54), 0);
        }
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        if (!args->side) {
          Matrix_RotateZ(gMegiddo_RayRolls[i]);
        } else {
          Matrix_RotateZ(-gMegiddo_RayRolls[i]);
        }
        Matrix_RotateX(gMegiddo_RayPitches[i]);
        Matrix_RotateY(gMegiddo_RayYaws[i]);
        Matrix_Scale(s / 4, s * 4, s / 2);
        Render_TransformVertices(gBattleEffect_StandingSquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);

    {
      volatile u32 *vblanks = (volatile u32 *)&work->vblanksSinceTransfer;

      do {
        (void)*vblanks;
      } while (*vblanks <= 1);
    }
    EFFECT_END_FRAME(work);
  }

  Sprite_DestroyFar(work->sprites[0]);
  caster->verticalMotionStrength = casterValue;
  BattleObject_PutFar();
  transition[4] = 0;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           Ram_HeapSlots->battleWork->background, 24);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
#undef BLITTER
