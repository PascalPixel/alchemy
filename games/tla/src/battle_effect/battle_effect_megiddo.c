/// @file
/// Megiddo, the Sol Blade's unleash.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define MEGIDDO_FRAMES 188

/// Where the textures are decoded: FlameGradientTexture at the start of the
/// effect's pictures, then GlowBandTextureA, FireStripesTexture's five 64x16
/// pictures of the corona's flames and DustBlobSheet's 32x32 cloud;
/// FireRingTexture in the map cell buffer.
#define MEGIDDO_GLOW_BAND 2048
#define MEGIDDO_FIRE_STRIPES 4096
#define MEGIDDO_FIRE_STRIPES_WIDTH 64
#define MEGIDDO_FIRE_STRIPES_HEIGHT 16
#define MEGIDDO_FIRE_STRIPES_PICTURES 5
#define MEGIDDO_DUST_BLOB 9216
#define MEGIDDO_DUST_BLOB_SIZE 32
#define MEGIDDO_FIRE_RING 16384

/// The turned corners of the corona and of the flames, and the corona's strip,
/// in the map cell buffer; the flames' corners and their strip in the effect's
/// pictures.
#define MEGIDDO_TRANSFORMED (Ram_MapCellBuffer + 4096)
#define MEGIDDO_CORONA_MESH (Ram_MapCellBuffer + 8192)
#define MEGIDDO_FLAME_CORNERS 24384
#define MEGIDDO_FLAME_MESH 25408

/// The sun's corona: a ring of MEGIDDO_CORONA_QUADS quads of the fire stripes
/// round the sun, from MEGIDDO_CORONA_INNER_RADIUS out to a flickering
/// MEGIDDO_CORONA_OUTER_RADIUS, scaled by MEGIDDO_CORONA_SCALE with the sun.
#define MEGIDDO_CORONA_QUADS 32
#define MEGIDDO_CORONA_VERTICES ((MEGIDDO_CORONA_QUADS + 1) * 2)
#define MEGIDDO_CORONA_INNER_RADIUS 80
#define MEGIDDO_CORONA_OUTER_RADIUS 120
#define MEGIDDO_CORONA_SCALE toFIXED(0.71009)

/// The flames that wrap the falling sun: MEGIDDO_FLAMES bands of the cloud
/// turned 3/32 of a turn apart about it, each a quarter circle of
/// MEGIDDO_FLAME_ROWS rows from its point to its rim, MEGIDDO_FLAME_RADIUS
/// round and an eighth of a turn wide. Each band's cloud is scrolled
/// MEGIDDO_FLAME_SCROLL_STEP texels on from the one before.
#define MEGIDDO_FLAMES 10
#define MEGIDDO_FLAME_ROWS 16
#define MEGIDDO_FLAME_VERTICES (MEGIDDO_FLAME_ROWS * 2)
#define MEGIDDO_FLAME_RADIUS 48
#define MEGIDDO_FLAME_SCROLL_STEP 20
#define MEGIDDO_FLAME_SCROLL_MASK 0x7f

/// As the sun falls, its flames and the sparks about it start turned by
/// MEGIDDO_FALL_TILT about the screen's horizontal and turn back over
/// MEGIDDO_FALL_TILT_FRAMES.
#define MEGIDDO_FALL_TILT (TRIG_TURN * 3 / 16)
#define MEGIDDO_FALL_TILT_FRAMES 32

/// How much the falling sun's scale grows a frame, from a quarter to three
/// quarters.
#define MEGIDDO_SUN_GROWTH toFIXED(0.01886)

/// How many lines a frame the red haze scrolls at its fastest; it slows to a
/// stop at MEGIDDO_HAZE_REST_FRAME.
#define MEGIDDO_HAZE_SPEED 16
#define MEGIDDO_HAZE_REST_FRAME 72
/// The red haze's colours: raised as it loads, its red by MEGIDDO_HAZE_FADE_RED
/// and its green and blue by MEGIDDO_HAZE_FADE; lowered again as the sun
/// falls, the red by MEGIDDO_HAZE_FALL_FADE_RED, below where it loaded.
#define MEGIDDO_HAZE_FADE_RED 8
#define MEGIDDO_HAZE_FADE 2
#define MEGIDDO_HAZE_FALL_FADE_RED 12

/// Once the sun strikes, the canvas is filled at this level after each copy.
#define MEGIDDO_STRIKE_FILL_LEVEL 16

/// The rings of light, FireRingTexture flashing out from the caster as the
/// effect starts and from the target as the sun strikes (gMegiddo_Flash*).
/// Each shines for MEGIDDO_FLASH_SHINE_FRAMES, then fades 8 levels a frame.
#define MEGIDDO_FLASHES 5
#define MEGIDDO_FLASH_SHINE_FRAMES 8

/// The rays that fan out about the caster (gMegiddo_Ray*), the flame of
/// FlameGradientTexture stretched long: one every two frames from
/// MEGIDDO_RAYS_START, each for MEGIDDO_RAY_FRAMES, shrinking away over the
/// last MEGIDDO_RAY_SHRINK_FRAMES.
#define MEGIDDO_RAYS 8
#define MEGIDDO_RAYS_START 16
#define MEGIDDO_RAY_FRAMES 64
#define MEGIDDO_RAY_SHRINK_FRAMES 16

/// Megiddo: the caster leaps and hangs in the air before a red haze, rings of
/// light flashing round them, rays fanning out about them and sparks
/// rising; a burning sun sweeps down across the sky, then falls onto the
/// target wrapped in flames and bursts in rings of light.
///
/// Abilities: Megiddo.
void BattleEffect_RunMegiddo(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  u8 *canvas;
  s32 frame;
  s32 doubled;
  s32 k;
  /* FAKEMATCH(temporary): 0 held in a local for the stores below; as 0. */
  s32 zero;
  struct ModelVertex *coronaCorners;
  DrawRectangle blit[2];
  u8 *sprites;
  struct BattleBackgroundWork *background;
  struct BattleCamera *camera;
  s16 savedYaw;
  struct ModelVertex *flameCorners;
  u8 *flameMesh;
  s32 casterVerticalMotion;
  struct MotionObject *caster;
  FIXED x;
  FIXED z;
  FIXED scale;
  u8 *transformed;
  u8 *coronaMesh;
  u8 *fireRing;
  s32 i;
  s32 j;
  s32 m;
  struct RenderModel *model;
  void *verts;
  struct Particle *p;
  struct ScreenPoint pos;
  s16 list[UNIT_MAX];
  s16 targetList[UNIT_MAX];
  FIXED place[4];
  s32 screen[3];
  struct ModelTexture scratchTexture;
  s32 twice;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  background = Ram_HeapSlots->battleBackground;
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = 0;
  zero = 0;
  coronaCorners = (struct ModelVertex *)gMapGridCellBuffer;
  transformed = MEGIDDO_TRANSFORMED;
  coronaMesh = MEGIDDO_CORONA_MESH;
  fireRing = Ram_MapCellBuffer + MEGIDDO_FIRE_RING;
  // The caster leaps on the spot.
  BattleMotion_ApproachTargetFar(args->user, args->user, 150, toFIXED(5));
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  flameCorners = (struct ModelVertex *)(work->sheet + MEGIDDO_FLAME_CORNERS);
  flameMesh = work->sheet + MEGIDDO_FLAME_MESH;
  caster = BattleObject_GetFar(args->user)->object;
  x = 0;
  z = 0;
  scale = FIXED_ONE;
  Render_BuildQuadStrip(coronaMesh, 8, MEGIDDO_FIRE_STRIPES_HEIGHT,
                        MEGIDDO_CORONA_QUADS);
  Render_BuildQuadStrip(flameMesh, 2, MEGIDDO_DUST_BLOB_SIZE,
                        MEGIDDO_FLAME_ROWS - 1);
  for (j = 0; j != MEGIDDO_FLAME_ROWS; j++) {
    s32 angle = j * (DEGtoANG(90) / (MEGIDDO_FLAME_ROWS - 1));
    s32 y = FIXEDtoINT(-Trig_Cos(angle) * MEGIDDO_FLAME_RADIUS);
    s32 r = FIXEDtoINT(Trig_Sin(angle) * MEGIDDO_FLAME_RADIUS);

    for (m = 0; m != 2; m++) {
      struct ModelVertex *v = &flameCorners[j * 2 + m];

      SET_POSITION(v, FIXEDtoINT(Trig_Sin(m * (TRIG_TURN / 8)) * r),
                   y + MEGIDDO_FLAME_RADIUS / 2,
                   FIXEDtoINT(Trig_Cos(m * (TRIG_TURN / 8)) * r));
    }
  }
  // The caster hangs in the air, still.
  casterVerticalMotion = caster->verticalMotionStrength;
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
      Sound_PlayFar(SE_TABLET_RISE_HIGH);
    }
    if (frame == 171) {
      Sound_PlayFar(SE_CRASH);
    }
    // A or B skips to the sun's fall.
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
      // DEAD: GlowBandTextureA, which nothing draws; the ROM keeps its decode.
      Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                                 work->sheet + MEGIDDO_GLOW_BAND, 0, 0);
      Resource_LoadAndDecompress(RESOURCE_FIRE_STRIPES_TEXTURE,
                                 work->sheet + MEGIDDO_FIRE_STRIPES, 1, 1);
      Resource_LoadAndDecompress(RESOURCE_DUST_BLOB_SHEET,
                                 work->sheet + MEGIDDO_DUST_BLOB, 1, 0);
      Graphics_LoadGreyRamp();
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      if (args->side == EFFECT_SIDE_ENEMIES) {
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
    // The caster alone before the red haze, and the sun's sprite.
    if (frame == 1) {
      background->busy = 1;
      list[0] = args->user;
      list[1] = BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForListFar(list, 0);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_RED_HAZE_BACKDROP, 0);
      BattleEffect_BuildRepeatingTilemap();
      Palette_BrightenBattleBg(MEGIDDO_HAZE_FADE_RED, MEGIDDO_HAZE_FADE,
                               MEGIDDO_HAZE_FADE);
      BattleEffect_SpawnObjects(1, SPRITE_SUN, 2);
    }
    EffectPosition_ProjectScaledBattler(args->user, &pos);
    if (frame == 3) {
      savedYaw = camera->yaw;
    }
    if ((u32)(frame - 16) <= 61) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        camera->yaw -= TRIG_TURN / 256;
      } else {
        camera->yaw += TRIG_TURN / 256;
      }
    }

    // The corona's rim flickers a row at a time.
    for (k = 0; k != MEGIDDO_CORONA_QUADS + 1; k++) {
      for (i = 0; i != 2; i++) {
        struct ModelVertex *v = &coronaCorners[k * 2 + i];
        s32 r = MEGIDDO_CORONA_INNER_RADIUS;

        if (i != 1) {
          r = (FIXEDtoINT(Trig_Sin((frame + k * 4) * (TRIG_TURN / 16)) * 2)) +
              MEGIDDO_CORONA_OUTER_RADIUS;
        }
        SET_POSITION(
            v, FIXEDtoINT(Trig_Sin(k * (TRIG_TURN / MEGIDDO_CORONA_QUADS)) * r),
            FIXEDtoINT(Trig_Cos(k * (TRIG_TURN / MEGIDDO_CORONA_QUADS)) * r),
            0);
      }
    }

    // The red haze scrolls fast, comes to rest at MEGIDDO_HAZE_REST_FRAME, and
    // scrolls the other way, the view with it, as the sun comes down.
    {
      s32 y = gBgScroll[1].y;
      s32 d;

      // A scroll above the haze's range is a negative one, read back from its
      // halfword.
      if (y > BATTLE_BACKGROUND_HEIGHT - BATTLE_VIEW_TOP) {
        y -= BG_SCROLL_WRAP;
      }
      if (frame <= 63) {
        d = -MEGIDDO_HAZE_SPEED;
      } else if ((u32)(frame - 64) <= 7) {
        twice = frame * 2;
        d = twice - 2 * MEGIDDO_HAZE_REST_FRAME;
      } else if ((u32)(frame - MEGIDDO_HAZE_REST_FRAME) <= 11) {
        d = 0;
      } else if (frame > 154) {
        d = MEGIDDO_HAZE_SPEED - (frame - 155) / 2;
        if (d < 0) {
          d = 0;
        }
      } else {
        d = frame - 84;
        if (d > MEGIDDO_HAZE_SPEED) {
          d = MEGIDDO_HAZE_SPEED;
        }
        gProjection.centerY -= d;
      }
      y += d;
      if (y < -BATTLE_VIEW_TOP) {
        y += BATTLE_BACKGROUND_HEIGHT;
      }
      if (y > BATTLE_BACKGROUND_HEIGHT - BATTLE_VIEW_TOP) {
        y -= BATTLE_BACKGROUND_HEIGHT;
      }
      gBgScroll[1].y = y;
    }
    if ((u32)(frame - 42) <= 37) {
      z += toFIXED(2);
      BattleEffect_StepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
    }
    // The sun sweeps down across the sky.
    if ((u32)(frame - 84) <= 58) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        x -= toFIXED(2);
      } else {
        x += toFIXED(2);
      }
      z += toFIXED(4);
    }
    // The fall: the camera comes back, and the sun falls on the target, alone
    // at the middle of the field.
    if (frame == 143) {
      struct MotionObject *target;

      Palette_BrightenBattleBg(-MEGIDDO_HAZE_FALL_FADE_RED, -MEGIDDO_HAZE_FADE,
                               -MEGIDDO_HAZE_FADE);
      camera->yaw = savedYaw;
      x = toFIXED(120);
      z = toFIXED(48);
      targetList[0] = args->targets[0];
      targetList[1] = BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForListFar(targetList, 0);
      gProjection.centerY = 556;
      target = BattleObject_GetFar(args->targets[0])->object;
      target->x = zero;
      target->z = zero;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
    }
    if (frame > 154) {
      s32 d = MEGIDDO_HAZE_SPEED - (frame - 155) / 2;

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
      Actor_SetAnimationSpeedFar(caster, ANIM_STEP_NORMAL * 8);
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(6));
    }
    // The sun strikes.
    if (frame == 171) {
      work->shakeFrames = 8;
      Sound_PlayFar(SE_CRASH);
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR,
                          CANVAS_FILL_WORD(MEGIDDO_STRIKE_FILL_LEVEL));
    }
    if (frame > 174) {
      Palette_BrightenBgEntries(2, 2, 2);
    }

    // The sun's sprite, at twice its scale, while it is on screen.
    if (frame > 1) {
      struct Sprite *obj;
      FIXED req[2];

      if (frame > 142) {
        s32 n = frame - 143;

        scale = n * MEGIDDO_SUN_GROWTH + FIXED_ONE / 4;
        if (scale > FIXED_ONE * 3 / 4) {
          scale = FIXED_ONE * 3 / 4;
        }
      }
      req[1] = scale * 2;
      req[0] = scale * 2;
      place[3] = zero;
      place[1] = toFIXED(255);
      place[0] = x;
      place[2] = z;
      obj = work->sprites[0];
      obj->offsetY = 32;
      obj->offsetX = 0;
      if ((u32)(place[2] + toFIXED(52)) < toFIXED(252)) {
        Render_ApplyProjectedPlacementFar(obj, place, req, 0);
      }
    }
    /* FAKEMATCH(dead): a product nothing reads, between the two blocks that
     * test frame > 1. */
    doubled = frame * 2;
    if (frame > 1) {
      struct ModelTexture texture;
      /* FAKEMATCH(temporary): t, a pointer standing in for the texture's own
       * address; as texture. */
      struct ModelTexture *t;

      model = Render_AllocateModelList(1);
      t = &texture;
      // The corona: a 64x16 picture of the fire stripes, its sizes as powers
      // of two.
      model->kind = MODEL_SHADED_MAX;
      /* FAKEMATCH(barrier): an empty asm that rewrites model and reads a
       * zero. */
      asm("" : "+r"(model) : "r"(0));
      t->logWidth = 6;
      model->vertices = transformed;
      t->logHeight = 4;
      model->texture = t;
      model->shade = 0;
      model->scrollV = 0;
      /* FAKEMATCH(dead): the pixels' store alone in a once-only block; as the
       * plain store. */
      do {
        t->pixels = work->sheet + MEGIDDO_FIRE_STRIPES +
                    ((frame / 2) % MEGIDDO_FIRE_STRIPES_PICTURES) *
                        PICTURE_BYTES(MEGIDDO_FIRE_STRIPES);
      } while (0);
      model->mesh = coronaMesh;
      /* FAKEMATCH(dead): a block that only places the width scale's
       * declaration before the matrix is reset. */
      {
        FIXED widthScale = FIXED_ONE / 2;

        Matrix_Initialize();
        Matrix_Scale(widthScale, FIXED_ONE, widthScale);
      }
      Matrix_Translate(x - toFIXED(126), z - toFIXED(76), 0);
      Matrix_ScaleUniform(MEGIDDO_CORONA_SCALE);
      Matrix_ScaleUniform(scale);
      Render_TransformVertices(coronaCorners, transformed,
                               MEGIDDO_CORONA_VERTICES);
      Render_DrawModelList(model);

      // The flames that wrap the sun as it falls.
      /* FAKEMATCH(dead): updates of t that cancel out. */
      t += frame;
      t -= frame;
      t->logWidth = 5;
      t->logHeight = 5;
      t->pixels = work->sheet + MEGIDDO_DUST_BLOB;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->mesh = flameMesh;
      /* FAKEMATCH(dead): a block that only places the shade's declaration
       * before the flags' store. */
      {
        s32 shade = -24;

        model->flags = MODEL_ONE_SIDED | MODEL_CLOCKWISE;
        model->shade = shade;
      }
      model->scrollV = zero;
      if (frame > 142) {
        s32 n = frame - 143;
        s32 scroll;
        s32 angle;
        s32 tilt;

        if (n > MEGIDDO_FALL_TILT_FRAMES) {
          n = MEGIDDO_FALL_TILT_FRAMES;
        }
        i = 0;
        /* FAKEMATCH(temporary): twice, frame * 2 kept apart. */
        twice = frame * 2;
        angle = -DEGtoANG(90);
        tilt = n * (MEGIDDO_FALL_TILT / MEGIDDO_FALL_TILT_FRAMES);
        scroll = twice + frame;
        for (; i != MEGIDDO_FLAMES; i++) {
          model->scrollU = ~scroll & MEGIDDO_FLAME_SCROLL_MASK;
          Matrix_Initialize();
          Matrix_Translate(-toFIXED(2), -toFIXED(32), 0);
          Matrix_Scale(toFIXED(1.25123), toFIXED(3.32044), FIXED_ONE);
          Matrix_ScaleUniform(scale);
          Matrix_RotateZ(DEGtoANG(180));
          scroll += MEGIDDO_FLAME_SCROLL_STEP;
          Matrix_RotateX(tilt - MEGIDDO_FALL_TILT);
          Matrix_RotateY(angle);
          Render_TransformVertices(flameCorners, transformed,
                                   MEGIDDO_FLAME_VERTICES);
          Render_DrawModelList(model);
          angle += TRIG_TURN * 3 / 32;
        }
      }
      Heap_Release(model);
    }

    // Sparks rise over the canvas while the rays shoot.
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
    // Sparks rise about the falling sun.
    if (frame > 142) {
      s32 n = frame - 143;

      if (n > MEGIDDO_FALL_TILT_FRAMES) {
        n = MEGIDDO_FALL_TILT_FRAMES;
      }
      if (frame == 143) {
        p = work->particles;
        for (i = 0; i != ARRAY_COUNT(work->particles); i++, p++) {
          SET_POSITION(p, Random16() & 127, Random16() % 192 - 64,
                       Random16() & 127);
        }
      }
      Matrix_Initialize();
      Matrix_RotateX(n * (MEGIDDO_FALL_TILT / MEGIDDO_FALL_TILT_FRAMES) -
                     MEGIDDO_FALL_TILT);
      p = work->particles;
      for (i = 0; i != ARRAY_COUNT(work->particles); i++, p++) {
        s32 size = 1;

        Matrix_ProjectPoint(p, screen);
        blit[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                screen[0], screen[1], size, 4);
        p->y -= 2;
      }
    }

    // The rings of light.
    verts = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    scratchTexture.logWidth = 7;
    scratchTexture.logHeight = 7;
    scratchTexture.pixels = fireRing;
    model->kind = MODEL_SHADED_ADDITIVE;
    model->mesh = gBattleEffect_LargeSquareMesh;
    model->texture = &scratchTexture;
    model->vertices = verts;
    for (i = 0; i != MEGIDDO_FLASHES; i++) {
      s32 start = gMegiddo_FlashStarts[i];

      if (frame >= start && frame < start + 20) {
        s32 t = frame - start;
        s32 shade = MEGIDDO_FLASH_SHINE_FRAMES * 8 - t * 8;
        s32 s;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > MODEL_SHADE_INVISIBLE) {
          s = t * (FIXED_ONE / 4) + FIXED_ONE / 4;
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
    // The rays.
    model->kind = MODEL_SHADED_ADDITIVE;
    scratchTexture.logWidth = 5;
    scratchTexture.logHeight = 6;
    scratchTexture.pixels = work->sheet;
    model->mesh = gBattleEffect_TallMesh;
    for (i = 0; i != MEGIDDO_RAYS; i++) {
      s32 start = MEGIDDO_RAYS_START + i * 2;

      if (frame >= start &&
          frame < i * 2 + MEGIDDO_RAYS_START + MEGIDDO_RAY_FRAMES) {
        s32 t = frame - start;
        s32 shade = MODEL_SHADE_INVISIBLE + t * 8;
        s32 s = t * (FIXED_ONE / 32);

        if (shade > 0) {
          shade = 0;
        }
        if (s > FIXED_ONE) {
          s = FIXED_ONE;
        }
        if (t >= MEGIDDO_RAY_FRAMES - MEGIDDO_RAY_SHRINK_FRAMES) {
          s = s - t * (FIXED_ONE / MEGIDDO_RAY_SHRINK_FRAMES) + FIXED_ONE * 3;
        }
        model->shade = shade;
        Matrix_Initialize();
        if (args->side == EFFECT_SIDE_ENEMIES) {
          Matrix_Translate(INTtoFIXED(pos.x / 2 + gMegiddo_RayX[i] - 64),
                           INTtoFIXED(gMegiddo_RayY[i] + pos.y - 54), 0);
        } else {
          Matrix_Translate(INTtoFIXED(pos.x / 2 - gMegiddo_RayX[i] - 62),
                           INTtoFIXED(gMegiddo_RayY[i] + pos.y - 54), 0);
        }
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        if (args->side == EFFECT_SIDE_ENEMIES) {
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

    // Waits until a VBlank has passed since the canvas was last copied.
    do {
      (void)work->vblanksSinceTransfer;
    } while (work->vblanksSinceTransfer <= 1);
    EFFECT_END_FRAME(work);
  }

  Sprite_DestroyFar(work->sprites[0]);
  caster->verticalMotionStrength = casterVerticalMotion;
  BattleObject_PutFar();
  background->busy = 0;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           Ram_HeapSlots->battleWork->background,
                           BACKGROUND_DARKEN_FRAMES);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
