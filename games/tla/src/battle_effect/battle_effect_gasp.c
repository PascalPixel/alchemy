/// @file
/// The body that sixteen abilities' effects share, from Gasp to Demon Eye.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct StripQuad;

/// A second, larger particle table, always reached at its fixed address.
#define EXTRA_PARTICLES ((struct Particle *)(Ram_MapCellBuffer + 0x4000))

/// The user's figure: a 20 x 40 cell drawn over the canvas.
#define FIGURE(work) ((work)->sheet + 0x65c0)
/// Loads a sheet's colours into the background palette.
#define LOAD_BG_PALETTE(resource)                                              \
  {                                                                            \
    void *palette = Resource_GetTableEntry((s32)&(resource));                  \
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;             \
                                                                               \
    copy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);                       \
  }
/// Sets the transform for one of modes 11 and 14's spinning particles: at its
/// position, half as wide (the canvas holds every other column), turned by z.
#define PLACE_PARTICLE(p)                                                      \
  do {                                                                         \
    Transform_Reset();                                                         \
    Transform_ApplyTranslation((p)->x - px(64), (p)->y - px(64), 0);           \
    Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE);                   \
    Transform_ApplyRoll((p)->z);                                               \
  } while (0)
/// DEAD: each test reads the count twice and drops the first read; both loads
/// are in the ROM.
#define WAIT_TRANSFER(work)                                                    \
  while (*(volatile u32 *)&(work)->vblanksSinceTransfer,                       \
         *(volatile u32 *)&(work)->vblanksSinceTransfer <= 1)

/// Where VenusDjinnSheet is decoded in the effect's pictures.
#define GASP_FAMILY_VENUS_DJINN_PICTURES 0x1000
/// Where CornerGlowSheet is decoded in the effect's pictures.
#define GASP_FAMILY_CORNER_GLOW_PICTURES 0x4000

/// Where FireRingTexture is decoded in the effect's pictures.
#define GASP_FIRE_RING 0x3080

/// The shared body of sixteen abilities' effects, named for the first (its
/// wrappers are in rime.c): Gasp (mode 0), Reflux (2), Djinn Counter (3), Mold
/// and Meld (4), Shine (5), Lull (6), Counter-rush (7), Demon Eye (8), Emu and
/// its kind's four other abilities (9), Call Dullahan and Formina Sage (10),
/// Beat Dance (11), Element Swap (12), Cage (13), Clarion Cry (14) and Psy
/// Boost (15). Mode 1 has no caller. Each mode loads its own sheets, then draws
/// a canvas layer and a model layer every frame for the mode's length.
void BattleEffect_RunGaspFamily(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  DrawRectangle draw[2];
  s32 frames;
  u8 *sprites = Ram_HeapSlots->battleSheet;
  s32 *camera = Ram_HeapSlots->cameraWork;
  u8 *cells = Ram_MapCellBuffer;
  struct RenderModel *model;
  void *verts;
  struct ModelTexture texture;
  s32 screen[3];
  s32 other[3];
  s32 frame;
  s32 i;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (mode == GASP_MODE_GASP) {
    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_JUPITER,
                                                   args->side ^ 1,
                                                   DJINNI_ANCHOR_TARGET_X),
                                   screen, other);
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                               FIGURE(work), 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D8, work->sheet, 0, 0);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    frames = 84;
  } else if (mode == 1) {
    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_MERCURY,
                                                   args->side ^ 1,
                                                   DJINNI_ANCHOR_TARGET_X),
                                   screen, other);
    Resource_LoadAndDecompress((s32)&ResourceId_LightBurstSheet, work->sheet, 1,
                               0);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               FIGURE(work), 1, 1);
    LOAD_BG_PALETTE(ResourceId_OrangePaletteB);
    frames = 48;
  } else if (mode == GASP_MODE_MOLD) {
    s32 j;
    s32 k;

    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_VENUS,
                                                   args->side ^ 1,
                                                   DJINNI_ANCHOR_HIGH_TARGET_X),
                                   screen, other);
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                               FIGURE(work), 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + GASP_FAMILY_VENUS_DJINN_PICTURES,
                               1, 0);
    {
      s32 (*clear)(void *, s32) = Iwram_ClearWordsEntry;

      clear(work->sheet, 0x1000);
    }
    for (k = 0; k != 40; k++)
      for (j = 0; j != 40; j++)
        work->sheet[0x30c + k * 64 + j] = work->sheet[0x1000 + k * 40 + j];
    frames = 42;
  } else if (mode == GASP_MODE_LULL) {
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER, args->side ^ 1, DJINNI_ANCHOR_CENTRE),
        screen, other);
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                               FIGURE(work), 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D7, work->sheet, 0, 0);
    frames = 80;
    Sound_PlayFar(MUS_CURE);
  } else if (mode == GASP_MODE_BEAT_DANCE) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D7, work->sheet, 0, 0);
    LOAD_BG_PALETTE(ResourceId_MarsDjinnSmallSheet);
    frames = 64;
  } else if (mode == GASP_MODE_CLARION_CRY) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D7, work->sheet, 0, 0);
    LOAD_BG_PALETTE(ResourceId_MercuryDjinnSmallSheet);
    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    work->fadeStep = 0;
    frames = 64;
  } else if (mode == GASP_MODE_CAGE) {
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                               1);
    frames = 40;
  } else if (mode == GASP_MODE_ELEMENT_SWAP) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, cells, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_CornerGlowSheet,
                               work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES,
                               1, 1);
    Picture_MirrorIntoQuadrants(work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES,
                                work->sheet, 64, 64);
    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    work->fadeStep = 0;
    frames = 48;
  } else if (mode == GASP_MODE_PSY_BOOST) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, cells, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_CornerGlowSheet,
                               work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES,
                               1, 1);
    Picture_MirrorIntoQuadrants(work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES,
                                work->sheet, 64, 64);
    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    work->fadeStep = 0;
    frames = 48;
  } else if (mode == GASP_MODE_SHINE) {
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_MARS, args->side, DJINNI_ANCHOR_LOW_CENTRE),
        screen, other);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                               FIGURE(work), 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C9,
                               work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES,
                               0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, cells, 0, 0);
    LOAD_BG_PALETTE(ResourceId_OrangePaletteB);
    frames = 82;
  } else if (mode == GASP_MODE_CALL_DULLAHAN) {
    s32 j;
    s32 k;

    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, cells, 1,
                               1);
    Iwram_ClearWordsEntry(work->sheet, 0x4000);
    for (i = 0; i != 4; i++)
      for (k = 0; k != 120; k++)
        for (j = 0; j != 24; j++)
          work->sheet[4 + i * 0x1000 + k * 32 + j] =
              cells[(i * 45 << 6) + k * 24 + j];
    LOAD_BG_PALETTE(ResourceId_VioletPaletteF);
    frames = 60;
  } else if (mode == GASP_MODE_COUNTER_RUSH) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet, 0, 0);
    LOAD_BG_PALETTE(ResourceId_MercuryDjinnSmallSheet);
    frames = 76;
  } else if (mode == GASP_MODE_DEMON_EYE) {
    Resource_LoadAndDecompress((s32)&ResourceId_CornerGlowSheet,
                               work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES,
                               1, 1);
    Picture_MirrorIntoQuadrants(work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES,
                                work->sheet, 64, 64);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    frames = 16;
  } else if (mode == GASP_MODE_EMU) {
    u8 *p;

    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2,
                               work->sheet + GASP_FIRE_RING, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 0);
    for (i = 0, p = work->sheet; i != 0x1b00; i++, p++) {
      *p <<= 1;
      if (*p > CANVAS_LEVEL_BRIGHTEST)
        *p = CANVAS_LEVEL_BRIGHTEST;
    }
    if (args->variant == EFFECT_VARIANT_STRONGEST) {
      LOAD_BG_PALETTE(ResourceId_OrangePaletteB);
    } else {
      LOAD_BG_PALETTE(ResourceId_BlastSheet);
    }
    BattleEffect_SetApproachMotion(args->user, args->targets[0], 10, 80);
    frames = 41;
  } else {
    if (mode == GASP_MODE_REFLUX) {
      BattleEffect_RunDjinniEntrance(args,
                                     DJINNI_ENTRANCE(ELEMENT_MARS,
                                                     args->side ^ 1,
                                                     DJINNI_ANCHOR_OVER_TARGET),
                                     screen, other);
      frames = 76;
    } else {
      work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
      work->fadeStep = 0;
      frames = 70;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                               FIGURE(work), 1, 1);
    LOAD_BG_PALETTE(ResourceId_OrangePaletteB);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  if (mode == GASP_MODE_GASP || mode == GASP_MODE_LULL)
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH ||
      mode == GASP_MODE_MOLD) {
    if (args->side == EFFECT_SIDE_ENEMIES)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          BLITTER_ADDITIVE);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
  } else if (mode == GASP_MODE_DJINN_COUNTER || mode == GASP_MODE_SHINE) {
    if (args->side == EFFECT_SIDE_PARTY)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          BLITTER_ADDITIVE);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
  } else {
    if (args->side == EFFECT_SIDE_ENEMIES)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;

  for (frame = 0; frame != frames; frame++) {
    s32 a;
    s32 twice;
    s32 x;
    s32 y;

    // The canvas layer.
    if (mode == GASP_MODE_GASP) {
      s32 *lines = (s32 *)(work->sheet + 0x6980);

      for (i = 0; i != 160; i++)
        *lines++ = (Random16() & 31) << 4;
    } else if (mode == GASP_MODE_LULL) {
      s32 *lines = (s32 *)(work->sheet + 0x6980);

      for (i = 0; i != 160; i++)
        *lines++ = (0x60000 - Trig_Sin((frame << 11) + i * 0x800) * 6) >> 10;
    } else if (mode == 1 && frame > 31) {
      u16 *pal = (u16 *)&BG_PLTT_COLOR(0, 1);

      for (i = 0; i != 63; i++, pal++) {
        s32 r = RGB_RED(*pal);
        s32 g = RGB_GREEN(*pal);
        s32 b = RGB_BLUE(*pal);

        if (frame > 40) {
          r += 4;
          g += 4;
          b += 4;
        } else {
          r += 2;
          g += 2;
          b += 2;
        }
        if (r > RGB_LEVEL_MAX)
          r = RGB_LEVEL_MAX;
        if (g > RGB_LEVEL_MAX)
          g = RGB_LEVEL_MAX;
        if (b > RGB_LEVEL_MAX)
          b = RGB_LEVEL_MAX;
        *pal = RGB(r, g, b);
      }
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(1));
    }

    if (mode == GASP_MODE_DJINN_COUNTER) {
      struct ScreenPoint to;
      struct ScreenPoint from;

      EffectPosition_ProjectScaledBattler(args->user, &from);
      EffectPosition_ProjectScaledBattler(args->targets[0], &to);
      from.x /= 2;
      to.x /= 2;
      if ((u32)(frame - 32) <= 8) {
        s32 n = frame - 31;
        s32 dx;
        s32 head;
        s32 tail;
        s32 step;
        s32 acc;

        if (n > 8)
          n = 8;
        dx = to.x - from.x;
        head = from.x + gGaspFamily_StreakSteps[n] * dx / 16;
        tail = from.x + gGaspFamily_StreakSteps[n - 1] * dx / 16;
        for (i = 0, step = head - tail, acc = 0; i != 4; i++, acc += step)
          draw[0](canvas, FIGURE(work), tail + acc / 3 - 10, from.y - 20,
                  SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      }
      if (frame > 37) {
        if (frame == 38) {
          for (i = 0; i != 63; i++) {
            struct Particle *p = &work->particles[i];
            s32 s;

            p->x = px(to.x);
            p->y = px(to.y);
            a = Random16();
            s = (Random16() & 511) + 256;
            p->velocityX = Trig_Sin(a) * s >> 8;
            p->velocityY = Trig_Cos(a) * s >> 7;
            p->variant = (Random16() & 15) + 16;
          }
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
          work->shakeFrames = 8;
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 8);
        }
        for (i = 0; i != 63; i++) {
          struct Particle *p = &work->particles[i];

          if (p->variant >= 0) {
            s32 n = (p->variant >> 3) + 2;

            draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                    Q_16_16_UPPER_HALF(p->x) - n / 2,
                    Q_16_16_UPPER_HALF(p->y) - n, n, n * 2);
            BattleEffectKernels_IntegrateVector2(p, 60, px(1) / 64);
            p->variant--;
          }
        }
      }
    } else if (mode == GASP_MODE_LULL) {
      s32 b = frame << 11;
      s32 x;
      s32 y;

      x = (Q_16_16_TO_INT(Trig_Sin(b) * 20)) + screen[0] / 2 - 10;
      y = (Q_16_16_TO_INT(Trig_Cos(b) * 4)) + screen[1] - 24;
      if (frame > 44) {
        twice = frame * 2;
        y = y - twice + 88;
      }
      draw[0](canvas, FIGURE(work), x, y, SMALL_DJINNI_WIDTH,
              SMALL_DJINNI_HEIGHT);
      BattleEffect_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
    } else if (mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH) {
      struct ScreenPoint target;

      if (mode == GASP_MODE_REFLUX) {
        s32 r = frame;

        x = screen[0] / 2 - 10;
        y = screen[1] - 20;
        if (r > 12)
          r = 12;
        a = frame << 9;
        x -= Q_16_16_TO_INT(Trig_Sin(a) * r * 2);
        y -= (Q_16_16_TO_INT(Trig_Cos(a) * r)) - r;
        if (frame <= 61)
          draw[0](canvas, FIGURE(work), x, y, SMALL_DJINNI_WIDTH,
                  SMALL_DJINNI_HEIGHT);
      }
      if (frame == 0)
        Sound_PlayFar(SE_SWIRL);
      if (frame == 62)
        Sound_PlayFar(SE_DJINNI_SET);
      if (frame == 0) {
        for (i = 0; i != 63; i++) {
          struct Particle *p = &work->particles[i];

          p->z = Random16();
          p->velocityZ = (Random16() & 63) + 64;
          Random16();
          p->variant = i + 32;
        }
      }
      EffectPosition_ProjectScaledBattler(args->targets[0], &target);
      for (i = 0; i != 48; i++) {
        struct Particle *p = &work->particles[i];

        if ((u32)p->variant <= 31) {
          s32 n = p->variant / 8 + 2;
          s32 x = (Q_16_16_TO_INT(Trig_Sin(p->z) * p->velocityZ / 2)) +
                  target.x / 2;
          s32 y = (Q_16_16_TO_INT(Trig_Cos(p->z) * p->velocityZ)) + target.y;

          p->z += 512;
          p->velocityZ -= 4;
          if (p->velocityZ > 0)
            draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                    x - n / 2, y - n, n, n * 2);
        }
        p->variant--;
      }
    } else if (mode == GASP_MODE_SHINE) {
      struct Particle *ball = &work->particles[63];
      struct ScreenPoint pos;
      s32 x;
      s32 y;

      if (frame == 0) {
        ball->velocityX = other[0];
        ball->velocityY = other[1];
        ball->velocityZ = other[2];
      }
      if (frame <= 47) {
        struct MotionObject *user = BattleObject_GetFar(args->user)->object;

        ball->velocityX += (user->x - other[0]) / 40;
        ball->velocityY += (0x3c0000 - other[1]) / 40;
        ball->velocityZ += (user->z - other[2]) / 40;
      } else {
        struct MotionObject *target =
            BattleObject_GetFar(args->targets[0])->object;
        struct Particle *fall = &work->particles[62];

        if (frame == 48) {
          SET_POSITION(fall, 0, 0, 0);
          fall->velocityX = (target->x - ball->velocityX) / 32;
          fall->velocityY = -ball->velocityY / 32;
          fall->velocityZ = (target->z - ball->velocityZ) / 32;
        }
        ball->velocityX += fall->x;
        ball->velocityY += fall->y;
        ball->velocityZ += fall->z;
        fall->x += fall->velocityX;
        fall->y += fall->velocityY;
        fall->z += fall->velocityZ;
      }
      Transform_Reset();
      Transform_SetLookAt(camera, camera + 3);
      EffectPosition_ProjectPoint(&ball->velocityX, &pos);
      x = pos.x >> 1;
      pos.x = x;
      ball->x = x;
      y = pos.y;
      ball->y = y;
      draw[0](canvas, FIGURE(work), x - 10, y - 20, SMALL_DJINNI_WIDTH,
              SMALL_DJINNI_HEIGHT);
      if (frame == 0)
        Sound_PlayFar(SE_SPIN);
      if (frame == 55) {
        struct Particle *flash = &work->particles[61];

        flash->x = pos.x;
        flash->y = pos.y;
        EFFECT_SHOW_DAMAGE(SE_CRASH);
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 24);
        work->shakeFrames = 16;
      }
      BattleCamera_Shake(8, 8);
    } else if (mode == GASP_MODE_GASP) {
      s32 b = frame << 11;
      s32 x;
      s32 y;

      x = (Q_16_16_TO_INT(Trig_Sin(b) * 20)) + screen[0] / 2 - 10;
      y = (Q_16_16_TO_INT(Trig_Cos(b) * 4)) + screen[1] - 24;
      if (frame > 48) {
        twice = frame * 2;
        y = y - twice + 96;
      }
      if (y > -40)
        draw[0](canvas, FIGURE(work), x, y, SMALL_DJINNI_WIDTH,
                SMALL_DJINNI_HEIGHT);
    }

    // The model layer.
    Transform_Reset();
    gProjection.centerY = 104;
    verts = Heap_AllocEwram(0x400);
    model = Render_AllocateModelList(1);
    model->texture = &texture;
    model->vertices = verts;
    if (mode == GASP_MODE_GASP) {
      if (frame == 0) {
        for (i = 0; i != 16; i++) {
          struct Particle *p = &work->particles[i];

          p->x = i << 15;
          p->y = px(56);
          p->velocityX = (Random16() & 511) + 512;
          p->velocityY = -((Random16() & 127) + 612) << 7;
          p->variant = -i * 8;
        }
      }
      if (frame == 4)
        Sound_PlayFar(SE_BEAST_CRY);
      if (frame == 48)
        EFFECT_SHOW_DAMAGE(-1);
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 7;
      model->mesh = gBattleEffect_SmallSquareMeshReversed;
      for (i = 0; i != 6; i++) {
        struct Particle *p = &work->particles[i];

        if (p->variant >= 0) {
          s32 size = 30000;
          s32 shade = -16;

          if (p->variant <= 6)
            shade = p->variant * 8 - 64;
          if (p->variant > 31) {
            shade = (32 - p->variant) * 4;
            if (shade < MODEL_SHADE_INVISIBLE)
              continue;
          }
          model->shade = shade;
          texture.pixels = work->sheet + (i % 2 << 10);
          Transform_Reset();
          p->y += p->velocityY;
          p->x += p->velocityX;
          Transform_ApplyTranslation(
              (Trig_Sin(p->x) << 4) + ((screen[0] / 2 - 64) << 16), p->y, 0);
          Transform_ApplyAxisScale(size >> 1, size, size >> 1);
          Transform_ApplyPitch(-TRIG_TURN / 4);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
        p->variant++;
      }
    } else if (mode == GASP_MODE_LULL) {
      s32 k;

      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = gBattleEffect_SmallSquareMeshReversed;
      model->vertices = verts;
      for (k = 0; k != 1; k++) {
        if (frame >= k * 32) {
          s32 t = frame - k * 32;
          s32 scale = (t << 10) + Q16_ONE / 16;
          s32 shade = 0;
          /* FAKEMATCH(pin): half kept in r5. */
          register s32 half __asm__("r5");

          if (t > 63) {
            shade = (64 - t) * 4;
            if (shade < MODEL_SHADE_INVISIBLE)
              continue;
          }
          model->shade = shade;
          half = scale >> 1;
          Transform_Reset();
          Transform_ApplyAxisScale(half, scale, scale);
          Transform_ApplyRoll((frame << 8) + TRIG_TURN * 3 / 4);
          Transform_ApplyPitch((frame << 6) - 0x4770);
          for (i = 0; i != 12; i++) {
            texture.pixels = work->sheet + (i % 3 << 10);
            Transform_Push();
            Transform_ApplyRoll(frame * 356 + i * 5461);
            Transform_ApplyTranslation(0, px(116), 0);
            Transform_ApplyAxisScale(half, half, half);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     4);
            Transform_Pop();
            Render_DrawModelList(model);
          }
        }
      }
    } else if (mode == 1) {
      model->shade = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gBattleEffect_SquareMesh;
      model->kind = 6;
      texture.pixels = work->sheet;
      for (i = 0; i != args->targetCount; i++) {
        struct ScreenPoint pos;

        EffectPosition_ProjectScaledBattler(args->targets[i], &pos);
        Transform_Reset();
        Transform_ApplyTranslation(px(pos.x / 2 - 64), px(pos.y - 48), 0);
        Transform_ApplyPitch(TRIG_TURN / 4);
        Transform_ApplyYaw((frame << 9) - TRIG_TURN / 16);
        Transform_ApplyAxisScale(frame * 2596 >> 1, frame * 2596 >> 1,
                                 frame * 2596 >> 1);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
    } else if (mode == GASP_MODE_SHINE) {
      if (frame <= 47) {
        struct Particle *ball;
        s32 twist;

        model->shade = -16;
        texture.logWidth = 5;
        texture.logHeight = 2;
        texture.pixels = work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES;
        model->texture = &texture;
        model->kind = 7;
        model->mesh = gGaspFamily_StreakMesh;
        model->vertices = verts;
        if (frame == 0) {
          for (i = 0; i != 16; i++) {
            struct Particle *p = &work->particles[i];

            p->x = Random16() & TRIG_ANGLE_MASK;
            p->y = (Random16() & 255) + 128;
            if (Random16() & 1)
              p->y = -p->y;
            p->variant = -i * 2;
          }
        }
        for (i = 0, ball = &work->particles[63], twist = frame * 2; i != 8;
             i++) {
          struct Particle *p = &work->particles[i];

          p->variant++;
          p->x += p->y * 2;
          model->scrollV = (twist + i * 7) & 31;
          if ((u32)p->variant <= 31) {
            Transform_Reset();
            Transform_ApplyTranslation(px(ball->x) - px(64),
                                       px(ball->y) - px(64), 0);
            Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE);
            Transform_ApplyRoll(p->x);
            Transform_ApplyAxisScale(Trig_Sin(p->variant * 682) * 3 +
                                         TRIG_TURN * 3 / 4,
                                     0x20000, 0x10000);
            Render_TransformVertices(gBattleEffect_TaperedBeamCorners, verts,
                                     4);
            Render_DrawModelList(model);
          }
        }
      }
      {
        struct ScreenPoint pos;

        EffectPosition_ProjectBattler(args->targets[0], &pos);
        texture.logWidth = 7;
        texture.logHeight = 7;
        texture.pixels = cells;
        model->kind = 7;
        model->texture = &texture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = verts;
        for (i = 0; i != 3; i++) {
          s32 t = frame - i * 4 - 55;

          if (t >= 0) {
            s32 scale = (t * 3 << 12) + Q16_ONE / 4;
            s32 shade = 0;

            if (t > 7)
              shade = (8 - t) * 8;
            model->shade = shade;
            if (shade > MODEL_SHADE_INVISIBLE) {
              Transform_Reset();
              Transform_ApplyTranslation(px(pos.x / 2 - 64), px(pos.y - 64), 0);
              Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
              Transform_ApplyRoll(i * (TRIG_TURN / 4) + TRIG_TURN / 8);
              Transform_ApplyPitch(-TRIG_TURN / 16);
              Transform_ApplyUniformScale(scale);
              Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                       4);
              Render_DrawModelList(model);
            }
          }
        }
      }
    } else if (mode == GASP_MODE_MOLD) {
      if ((u32)(frame - 16) <= 64) {
        s32 t = frame - 16;
        s32 scale;
        struct ScreenPoint pos;

        if (t <= 23)
          work->transferValue = CANVAS_FADE_THREE_QUARTERS;
        else
          work->transferValue = CANVAS_FADE_HALF;
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->mesh = gBattleEffect_SquareMesh;
        model->kind = 7;
        texture.pixels = work->sheet;
        EffectPosition_ProjectScaledBattler(args->targets[0], &pos);
        for (i = 0, scale = t * -4592 + Q16_ONE; i != 2;
             scale += Q16_ONE / 4, i++) {
          if (scale > 0) {
            s32 shade = -(i << 4);

            if (shade > 0)
              shade = 0;
            model->shade = shade;
            Transform_Reset();
            Transform_ApplyTranslation(px(pos.x / 2 - 64), px(pos.y - 48), 0);
            Transform_ApplyPitch(TRIG_TURN * 3 / 4);
            Transform_ApplyAxisScale(scale, scale, scale);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     4);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame == 14) {
        Sound_PlayFar(SE_POWER_UP);
      }
      if (frame == 24) {
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, 0, 12);
      }
    } else if (mode == GASP_MODE_EMU) {
      struct ScreenPoint from;
      struct Particle *p;

      EffectPosition_ProjectBattler(args->user, &from);
      if (frame == 0) {
        struct ScreenPoint to;
        struct Particle *ball = &work->particles[63];
        struct Particle *q;

        EffectPosition_ProjectScaledBattler(args->targets[0], &to);
        ball->x = to.x;
        ball->y = to.y;
        EFFECT_FREE_PARTICLES(i, work->particles, 32);
        for (i = 0, q = EXTRA_PARTICLES; i != 256; i++, q++) {
          s32 s = 511;
          s32 a;

          s &= Random16();
          a = Random16();
          s += 64;
          q->velocityX = Trig_Sin(a) * s >> 6;
          q->velocityY = Trig_Cos(a) * s >> 4;
          if (args->variant == EFFECT_VARIANT_BASIC) {
            q->velocityX /= 4;
            q->velocityY /= 4;
          } else if (args->variant == EFFECT_VARIANT_STRONGER) {
            q->velocityX /= 2;
            q->velocityY /= 2;
          }
          q->x = to.x / 2 << 16;
          q->y = px(to.y + 16);
          q->variant = (Random16() & 15) + 8;
        }
      }
      if (frame <= 9) {
        s32 v;
        s32 mask = 255;

        i = 0;
        twice = frame * 2;
        // Index first, as a plain integer sum: the add takes the offset first.
        p = (void *)((twice * 28) + (s32)work->particles);
        p->x = from.x / 2 << 16;
        p->y = px(from.y + 8);
        v = ((Random16() & mask) + 256) << 8;
        p->velocityX = v;
        if (args->side == EFFECT_SIDE_PARTY)
          p->velocityX = -v;
        p->velocityY = -((Random16() & mask) + 256) << 4;
        p->variant = i;
      }
      for (i = 0; i != 32; i++) {
        p = &work->particles[i];

        if ((u32)p->variant <= 23) {
          draw[0](canvas,
                  work->sheet + p->variant / 4 * PICTURE_BYTES(SMOKE_PUFF),
                  Q_16_16_UPPER_HALF(p->x) - 12, Q_16_16_UPPER_HALF(p->y) - 24,
                  SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
          BattleEffectKernels_IntegrateVector2(p, 60, -px(1) / 16);
          p->variant++;
        }
      }
      if (frame > 9) {
        for (i = 0; i != gGaspFamily_Counts[args->variant]; i++) {
          struct Particle *q = &EXTRA_PARTICLES[i];

          if (q->variant >= 0) {
            s32 n = q->variant / 4 + 3;

            draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                    Q_16_16_UPPER_HALF(q->x) - n / 2,
                    Q_16_16_UPPER_HALF(q->y) - n, n, n * 2);
            BattleEffectKernels_IntegrateVector2(q, 62, 0);
            q->variant--;
          }
        }
      }
      if (frame > 9) {
        struct Particle *ball = &work->particles[63];
        s32 scale = ((frame - 10) << 15) + Q16_ONE / 4;
        s32 shade = 0;

        if (args->variant == EFFECT_VARIANT_BASIC)
          scale /= 4;
        else if (args->variant == EFFECT_VARIANT_STRONGER)
          scale /= 2;
        if (frame > 15)
          shade = (16 - frame) * 8;
        if (shade > MODEL_SHADE_INVISIBLE) {
          model->shade = shade;
          texture.logWidth = 7;
          texture.logHeight = 7;
          model->mesh = gBattleEffect_LargeSquareMesh;
          model->kind = 7;
          texture.pixels = work->sheet + GASP_FIRE_RING;
          Transform_Reset();
          Transform_ApplyTranslation(px(ball->x / 2 - 64), px(ball->y - 48), 0);
          Transform_ApplyAxisScale(scale / 4, scale, scale / 2);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
      if (frame == 10) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        work->shakeFrames = args->variant * 2 + 8;
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 12);
      }
      BattleCamera_Shake(args->variant * 2 + 8, args->variant * 2 + 8);
    } else if (mode == GASP_MODE_BEAT_DANCE) {
      if (frame == 0) {
        struct ScreenPoint pos;

        EffectPosition_ProjectBattler(args->targets[0], &pos);
        for (i = 0; i != 32; i++) {
          struct Particle *p = &work->particles[i];
          s32 s = 255;
          s32 a;

          s &= Random16();
          a = (Random16() & 0x7fff) + 0x4000;
          s += 128;
          p->velocityX = Trig_Sin(a) * s >> 7;
          p->velocityY = Trig_Cos(a) * s >> 6;
          p->velocityZ = px(1) / 64;
          SET_POSITION(p, pos.x / 2 << 16, px(pos.y - (Random16() & 31) + 16),
                       Random16());
        }
        Sound_PlayFar(SE_POWER_UP);
        work->shakeFrames = 16;
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_RED_TINT,
                                 TINT_KEEP, 0, 32);
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = gBattleEffect_SmallSquareMeshReversed;
      model->vertices = verts;
      for (i = 0; i != 12; i++) {
        struct Particle *p = &work->particles[i];

        if (frame >= i * 2) {
          s32 t = frame - i * 2;
          s32 shade = 0;

          if (t > 23) {
            shade = (24 - t) * 4;
            if (shade < MODEL_SHADE_INVISIBLE)
              continue;
          }
          model->shade = shade;
          texture.pixels = work->sheet + (i % 3 << 10);
          PLACE_PARTICLE(p);
          Transform_ApplyUniformScale(Q16_ONE / 2);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
          Render_DrawModelList(model);
          BattleEffectKernels_IntegrateVector3(p, 60, 0);
        }
      }
      BattleCamera_Shake(8, 8);
    } else if (mode == GASP_MODE_CLARION_CRY) {
      if (frame == 0) {
        struct ScreenPoint pos;

        EffectPosition_ProjectBattler(args->user, &pos);
        for (i = 0; i != 32; i++) {
          struct Particle *p = &work->particles[i];
          s32 s = 255;
          s32 a;

          s &= Random16();
          a = (Random16() & 0x7fff) + 0x4000;
          s += 128;
          p->velocityX = Trig_Sin(a) * s >> 6;
          p->velocityY = Trig_Cos(a) * s >> 6;
          p->velocityZ = px(1) / 64;
          SET_POSITION(p, pos.x / 2 << 16, px(pos.y - (Random16() & 31) - 16),
                       Random16());
        }
        Sound_PlayFar(SE_619);
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = gBattleEffect_SmallSquareMeshReversed;
      model->vertices = verts;
      for (i = 0; i != 12; i++) {
        struct Particle *p = &work->particles[i];

        if (frame >= i * 2) {
          s32 t = frame - i * 2;
          s32 shade = 0;

          if (t > 23) {
            shade = (24 - t) * 4;
            if (shade < MODEL_SHADE_INVISIBLE)
              continue;
          }
          model->shade = shade;
          texture.pixels = work->sheet + (i % 3 << 10);
          PLACE_PARTICLE(p);
          Transform_ApplyUniformScale(Q16_ONE / 2);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
          Render_DrawModelList(model);
          BattleEffectKernels_IntegrateVector3(p, 60, 0);
        }
      }
    } else if (mode == GASP_MODE_CALL_DULLAHAN) {
      struct ScreenPoint pos;

      if (frame == 0) {
        void *user = BattleObject_GetFar(args->user)->object;

        Actor_SelectAnimationFar(user, 0);
        Actor_SelectAnimationFar(user, 2);
        Actor_SetAnimationSpeedFar(user, 0);
        Sound_PlayFar(SE_POWER_UP);
      }
      if (frame == 14)
        Actor_SetAnimationSpeedFar(BattleObject_GetFar(args->user)->object,
                                   ANIMATION_STEP_NORMAL);
      if (frame == 22)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == 26) {
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
        work->shakeFrames = 8;
        EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      if (frame > 27) {
        struct Particle *q;
        struct Particle *p;

        if (frame == 28) {
          EffectPosition_ProjectScaledBattler(args->targets[0], &pos);
          for (q = EXTRA_PARTICLES, i = 0; i != 128; i++, q++) {
            s32 s = 511;
            s32 a;

            s &= Random16();
            a = Random16();
            s += 32;
            q->velocityX = Trig_Sin(a) * s >> 7;
            q->velocityY = Trig_Cos(a) * s >> 6;
            q->x = (pos.x / 2 << 16) + q->velocityX * 2;
            q->y = px(pos.y + 16) + q->velocityY * 2;
            q->variant = (Random16() & 15) + 16;
          }
        }
        for (p = EXTRA_PARTICLES, i = 0; i != 64; i++, p++) {
          if (p->variant >= 0) {
            s32 j;

            for (j = 0; j != 4; j++) {
              s32 n = p->variant / 8 + 1;

              draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                      Q_16_16_UPPER_HALF(p->x) - n / 2,
                      Q_16_16_UPPER_HALF(p->y) - n, n, n * 2);
              BattleEffectKernels_IntegrateVector2(p, 63, px(1) / 32);
              if (p->y > px(104)) {
                p->y = px(104);
                p->velocityY = -p->velocityY / 2;
              }
            }
            p->variant--;
          }
        }
      }
      EffectPosition_ProjectScaledBattler(args->user, &pos);
      texture.logWidth = 5;
      texture.logHeight = 7;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = gBattleEffect_ColumnMesh;
      model->vertices = verts;
      Transform_Reset();
      Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE);
      if (args->side == EFFECT_SIDE_ENEMIES)
        Transform_ApplyTranslation(px(pos.x - 136), px(pos.y - 48), 0);
      else
        Transform_ApplyTranslation(px(pos.x - 108), px(pos.y - 56), 0);
      for (i = 0; i != 4; i++) {
        s32 scale;

        Transform_Push();
        texture.pixels = work->sheet + ((Random16() & 3) << 12);
        texture.logWidth = 5;
        texture.logHeight = 7;
        if (frame > 23) {
          s32 d = frame * 16 - i * 2 - 352;
          s32 angle;

          if (d < 0)
            continue;
          if (d > 80)
            d = 80;
          if (args->side == EFFECT_SIDE_ENEMIES)
            Transform_ApplyTranslation(d * 3 << 13, d * 7 << 11, 0);
          else
            Transform_ApplyTranslation(-(d * 3 << 13), d * 7 << 11, 0);
          angle = d * 48152 / 80 + TRIG_TURN * 3 / 8;
          if (args->side == EFFECT_SIDE_ENEMIES)
            angle = -angle;
          Transform_ApplyRoll(angle);
        } else {
          if (i != 0)
            continue;
          Transform_ApplyRoll(args->side == EFFECT_SIDE_ENEMIES
                                  ? -TRIG_TURN * 3 / 8
                                  : TRIG_TURN * 3 / 8);
        }
        model->shade = -i << 3;
        if (frame <= 23)
          scale = frame * 2730;
        else if (frame > 31)
          scale = Q16_ONE * 4 - (frame * 3 << 11);
        else
          scale = Q16_ONE;
        if (scale > 0) {
          Transform_ApplyAxisScale(scale * 6, scale * 2, Q16_ONE);
          Render_TransformVertices(gBattleEffect_LongBeamCorners, verts, 4);
          Render_DrawModelList(model);
          Transform_Pop();
        }
      }
      BattleCamera_Shake(8, 8);
    } else if (mode == GASP_MODE_DEMON_EYE) {
      struct ScreenPoint pos;

      work->transferValue = CANVAS_FADE_HALF;
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->kind = 6;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
      texture.pixels = work->sheet;
      EffectPosition_ProjectScaledBattler(args->user, &pos);
      if (frame <= 15) {
        for (i = 0; i != 2; i++) {
          s32 scale = Trig_Sin(frame << 11) * 5;

          if (scale > 0) {
            model->shade = 0;
            Transform_Reset();
            Transform_ApplyTranslation(px(pos.x / 2 + i * 16 - 72),
                                       px(pos.y - 64), 0);
            Transform_ApplyAxisScale(scale / 2, scale, scale / 2);
            Transform_ApplyRoll(TRIG_TURN / 8);
            Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame == 8) {
        Sound_PlayFar(SE_POWER_UP);
        work->shakeFrames = 8;
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        for (i = 0; i != args->targetCount; i++)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 7);
      }
      BattleCamera_Shake(8, 8);
    } else if (mode == GASP_MODE_CAGE) {
      struct ScreenPoint pos;
      s32 shade;

      if (frame == 0)
        Sound_PlayFar(SE_104);
      EffectPosition_ProjectBattler(args->targets[0], &pos);
      shade = frame * 4 - 64;
      if (shade >= -16)
        shade = -16;
      model->shade = shade;
      model->texture = &texture;
      model->vertices = verts;
      model->kind = 6;
      model->mesh = gBattleEffect_CylinderMesh;
      texture.pixels = work->sheet;
      texture.logWidth = 7;
      texture.logHeight = 4;
      if (frame == 26) {
        work->shakeFrames = 8;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
      for (i = 0; i != 8; i++) {
        s32 scale = Q16_ONE * 2;
        s32 start = i * 2 + 16;

        if (frame >= start)
          scale = Q16_ONE * 2 - ((frame - start) << 13);
        if (scale > Q16_ONE / 2) {
          model->scrollU = ((frame + i * 5) & 7) << 4;
          Transform_Reset();
          Transform_ApplyTranslation(px(pos.x / 2 - 60), px(8), 0);
          Transform_ApplyAxisScale(scale >> 1, scale, Q16_ONE);
          Transform_ApplyPitch((i << 13) + 4000);
          Transform_ApplyYaw(frame << 9);
          Transform_ApplyRoll((frame + i * 16) << 9);
          Transform_ApplyAxisScale(Q16_ONE, Q16_ONE / 2, Q16_ONE);
          Render_TransformVertices(gBattleEffect_CylinderCorners, verts, 32);
          Render_DrawModelList(model);
        }
      }
    } else if (mode == GASP_MODE_ELEMENT_SWAP) {
      struct ScreenPoint pos;
      s32 j;

      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = cells;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->kind = 6;
      EffectPosition_ProjectScaledBattler(args->user, &pos);
      Transform_Reset();
      Transform_ApplyTranslation(px(pos.x / 2 - 64), px(pos.y - 48), 0);
      if (frame <= 15) {
        s32 scale = Q16_ONE * 3 - (frame * 3 << 12);

        model->shade = 0;
        Transform_ApplyAxisScale(scale >> 1, scale, scale >> 1);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
      Transform_Reset();
      Transform_ApplyTranslation(px(pos.x / 2 - 64), px(pos.y - 48), 0);
      if ((u32)(frame - 12) <= 31) {
        s32 scale = Trig_Sin((frame << 10) - TRIG_TURN * 3 / 16) * 3;

        texture.pixels = work->sheet;
        Transform_ApplyAxisScale(scale / 2, scale, scale / 2);
        Transform_ApplyRoll((frame << 9) + TRIG_TURN / 8);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
      if (frame == 0) {
        for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
          struct Particle *p = &work->particles[i];

          Random16();
          p->x = 768 + i * 24;
          p->velocityX = -p->x / 48;
          p->y = Random16();
        }
        LOAD_BG_PALETTE(ResourceId_BlastSheet);
      }
      if (frame <= 15) {
        for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
          struct Particle *p = &work->particles[i];

          for (j = 0; j != 3; j++) {
            s32 x = (Trig_Sin(p->y) * p->x >> 21) + pos.x / 2;
            s32 y = (p->x * Trig_Cos(p->y) >> 20) + 60;
            s32 n = 4;

            draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                    x - n / 2, y - n, n, n * 2);
            p->x += p->velocityX;
          }
        }
      }
      for (i = 0; i != 3; i++) {
        if (frame > 7) {
          if (frame <= 15)
            BattleEffect_StepPaletteToResource(
                (s32)&ResourceId_MercuryDjinnSheet);
          else if (frame <= 23)
            BattleEffect_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
          else
            BattleEffect_StepPaletteToResource(
                (s32)&ResourceId_JupiterDjinnSheet);
        }
      }
      if (frame == 16) {
        Sound_PlayFar(SE_POWER_UP);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, 0, 30);
      }
      if (frame == 28)
        EFFECT_SHOW_DAMAGE(-1);
    } else if (mode == GASP_MODE_PSY_BOOST) {
      struct ScreenPoint pos;

      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = cells;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->kind = 6;
      EffectPosition_ProjectScaledBattler(args->user, &pos);
      for (i = 0; i != 2; i++) {
        s32 t = frame - i * 8;

        Transform_Reset();
        Transform_ApplyTranslation(px(pos.x / 2 - 64), px(pos.y - 48), 0);
        if ((u32)t <= 15) {
          s32 scale = Q16_ONE * 4 - (t << 14);
          s32 shade = 0;

          if (t <= 7)
            shade = t * 8 - 64;
          model->shade = shade;
          Transform_ApplyAxisScale(scale / 2, scale, scale / 2);
          Transform_ApplyRoll(i * (TRIG_TURN / 4) + TRIG_TURN / 8);
          Transform_ApplyPitch(-TRIG_TURN / 16);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
      Transform_Reset();
      Transform_ApplyTranslation(px(pos.x / 2 - 64), px(pos.y - 48), 0);
      if ((u32)(frame - 12) <= 31) {
        s32 scale = Trig_Sin((frame << 10) - TRIG_TURN * 3 / 16) * 2;

        texture.pixels = work->sheet;
        Transform_ApplyAxisScale(scale / 2, scale, scale / 2);
        Transform_ApplyRoll((frame << 9) + TRIG_TURN / 8);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
      if (frame == 0)
        Graphics_LoadGreyRamp();
      if (frame == 16) {
        Sound_PlayFar(SE_POWER_UP);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, 0, 30);
      }
      if (frame == 28)
        EFFECT_SHOW_DAMAGE(-1);
    } else {
      s32 scale;

      model->shade = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gBattleEffect_SquareMesh;
      model->kind = 7;
      texture.pixels = work->sheet;
      if (mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH) {
        if (frame <= 31)
          scale = frame * 1500;
        else if (frame > 59)
          scale = 228000 - frame * 3000;
        else
          scale = 48000;
      } else {
        if (frame <= 31)
          scale = frame * 1500;
        else if (frame > 39)
          scale = 168000 - frame * 3000;
        else
          scale = 48000;
      }
      if (scale > 0) {
        struct ScreenPoint pos;

        if (mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH)
          EffectPosition_ProjectScaledBattler(args->targets[0], &pos);
        else
          EffectPosition_ProjectScaledBattler(args->user, &pos);
        Transform_Reset();
        Transform_ApplyTranslation(px(pos.x / 2 - 64), px(pos.y - 48), 0);
        Transform_ApplyPitch(TRIG_TURN / 4);
        Transform_ApplyAxisScale(scale / 3, scale, scale);
        if (mode == GASP_MODE_REFLUX)
          Transform_ApplyYaw(-frame << 10);
        else
          Transform_ApplyYaw(frame << 10);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
    BattleEffect_CountDownBattlerTints();
    if (mode == 1)
      WAIT_TRANSFER(work) {}
    EFFECT_PRESENT_CANVAS(work);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  if (mode == GASP_MODE_GASP || mode == GASP_MODE_LULL)
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
#undef BLITTER
#undef EXTRA_PARTICLES
#undef FIGURE
#undef LOAD_BG_PALETTE
#undef PLACE_PARTICLE
#undef WAIT_TRANSFER
