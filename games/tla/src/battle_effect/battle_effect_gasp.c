/// @file
/// The body that sixteen abilities' effects share, from Gasp to Demon Eye.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct StripQuad;

/// A second, larger particle table, always reached at its fixed address.
#define EXTRA_PARTICLES ((struct Particle *)(Ram_MapCellBuffer + 16384))

/// Where the small Djinni sheet is decoded in the effect's pictures, as
/// The Broken Seal places it.
#define GASP_DJINNI_PICTURE 26048
/// The small Djinni: a 20 x 40 picture drawn over the canvas.
#define FIGURE(work) ((work)->sheet + GASP_DJINNI_PICTURE)
/// Sets the transform for one of modes 11 and 14's spinning particles: at its
/// position, half as wide (the canvas holds every other column), turned by z.
#define PLACE_PARTICLE(p)                                                      \
  do {                                                                         \
    Matrix_Initialize();                                                       \
    Matrix_Translate((p)->x - toFIXED(64), (p)->y - toFIXED(64), 0);           \
    Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);                         \
    Matrix_RotateZ((p)->z);                                                    \
  } while (0)
// DEAD: each test reads the count twice and drops the first read; the ROM
// keeps both loads.
#define WAIT_TRANSFER(work)                                                    \
  while ((work)->vblanksSinceTransfer, (work)->vblanksSinceTransfer <= 1)

/// The last three particles: the point a mode centres on (Shine's Djinni in
/// flight, Emu's target), Shine's fall towards the target, and where its
/// flash strikes.
#define GASP_CENTRE 63
#define GASP_FALL 62
#define GASP_FLASH 61
/// The motes of Djinn Counter's blow and of Reflux's and Counter Rush's swirl:
/// every particle but the last.
#define GASP_MOTES 63
/// Mold's 40x40 Djinni is copied to the middle of a 64x64 cel.
#define GASP_DJINNI_CEL 64
#define GASP_DJINNI_CEL_MARGIN ((GASP_DJINNI_CEL - DJINNI_WIDTH) / 2)
/// Call Dullahan's four bolts each take a 32x128 cel, 4 pixels in from its
/// left.
#define GASP_BOLT_CEL_WIDTH 32
#define GASP_BOLT_CEL_HEIGHT 128
#define GASP_BOLT_CEL_BYTES (GASP_BOLT_CEL_WIDTH * GASP_BOLT_CEL_HEIGHT)

/// Where VenusDjinnSheet is decoded in the effect's pictures.
#define GASP_FAMILY_VENUS_DJINN_PICTURES 4096
/// Where CornerGlowSheet is decoded in the effect's pictures.
#define GASP_FAMILY_CORNER_GLOW_PICTURES 16384

/// Where FireRingTexture is decoded in the effect's pictures.
#define GASP_FIRE_RING 12416

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
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
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
    Resource_LoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET, FIGURE(work),
                               1, 1);
    Resource_LoadAndDecompress(RESOURCE_BURNING_SKULL_SHEET, work->sheet, 0, 0);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    frames = 84;
  } else if (mode == GASP_MODE_UNUSED) {
    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_MERCURY,
                                                   args->side ^ 1,
                                                   DJINNI_ANCHOR_TARGET_X),
                                   screen, other);
    Resource_LoadAndDecompress(RESOURCE_LIGHT_BURST_SHEET, work->sheet, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET, FIGURE(work),
                               1, 1);
    LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
    frames = 48;
  } else if (mode == GASP_MODE_MOLD) {
    s32 j;
    s32 k;

    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_VENUS,
                                                   args->side ^ 1,
                                                   DJINNI_ANCHOR_HIGH_TARGET_X),
                                   screen, other);
    Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET, FIGURE(work),
                               1, 0);
    Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SHEET,
                               work->sheet + GASP_FAMILY_VENUS_DJINN_PICTURES,
                               1, 0);
    {
      s32 (*clear)(void *, s32) = Iwram_ClearWordsEntry;

      clear(work->sheet, GASP_DJINNI_CEL * GASP_DJINNI_CEL);
    }
    for (k = 0; k != DJINNI_HEIGHT; k++)
      for (j = 0; j != DJINNI_WIDTH; j++)
        work->sheet[(k + GASP_DJINNI_CEL_MARGIN) * GASP_DJINNI_CEL +
                    GASP_DJINNI_CEL_MARGIN + j] =
            work->sheet[GASP_FAMILY_VENUS_DJINN_PICTURES + k * DJINNI_WIDTH +
                        j];
    frames = 42;
  } else if (mode == GASP_MODE_LULL) {
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER, args->side ^ 1, DJINNI_ANCHOR_CENTRE),
        screen, other);
    Resource_LoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET, FIGURE(work),
                               1, 1);
    Resource_LoadAndDecompress(RESOURCE_MUSIC_NOTE_SHEET, work->sheet, 0, 0);
    frames = 80;
    Sound_PlayFar(MUS_CURE);
  } else if (mode == GASP_MODE_BEAT_DANCE) {
    Resource_LoadAndDecompress(RESOURCE_MUSIC_NOTE_SHEET, work->sheet, 0, 0);
    LOAD_BG_PALETTE(RESOURCE_MARS_DJINN_SMALL_SHEET);
    frames = 64;
  } else if (mode == GASP_MODE_CLARION_CRY) {
    Resource_LoadAndDecompress(RESOURCE_MUSIC_NOTE_SHEET, work->sheet, 0, 0);
    LOAD_BG_PALETTE(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    work->fadeStep = 0;
    frames = 64;
  } else if (mode == GASP_MODE_CAGE) {
    Resource_LoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, work->sheet, 1, 1);
    frames = 40;
  } else if (mode == GASP_MODE_ELEMENT_SWAP) {
    Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_CORNER_GLOW_SHEET,
                               work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES,
                               1, 1);
    Picture_MirrorIntoQuadrants(work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES,
                                work->sheet, 64, 64);
    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    work->fadeStep = 0;
    frames = 48;
  } else if (mode == GASP_MODE_PSY_BOOST) {
    Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_CORNER_GLOW_SHEET,
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
    Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET, FIGURE(work), 1,
                               0);
    Resource_LoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE, work->sheet, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                               work->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES,
                               0, 0);
    Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
    LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
    frames = 82;
  } else if (mode == GASP_MODE_CALL_DULLAHAN) {
    s32 j;
    s32 k;

    Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, cells, 1, 1);
    Iwram_ClearWordsEntry(work->sheet, 4 * GASP_BOLT_CEL_BYTES);
    for (i = 0; i != 4; i++)
      for (k = 0; k != LIGHTNING_BOLT_HEIGHT; k++)
        for (j = 0; j != LIGHTNING_BOLT_WIDTH; j++)
          work->sheet[4 + i * GASP_BOLT_CEL_BYTES + k * GASP_BOLT_CEL_WIDTH +
                      j] = cells[i * LIGHTNING_BOLT_PICTURE_BYTES +
                                 k * LIGHTNING_BOLT_WIDTH + j];
    LOAD_BG_PALETTE(RESOURCE_VIOLET_PALETTE_F);
    frames = 60;
  } else if (mode == GASP_MODE_COUNTER_RUSH) {
    Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE, work->sheet, 0, 0);
    LOAD_BG_PALETTE(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
    frames = 76;
  } else if (mode == GASP_MODE_DEMON_EYE) {
    Resource_LoadAndDecompress(RESOURCE_CORNER_GLOW_SHEET,
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
    Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
                               work->sheet + GASP_FIRE_RING, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET, work->sheet, 1, 0);
    for (i = 0, p = work->sheet; i != 6 * PICTURE_BYTES(SMOKE_PUFF); i++, p++) {
      *p <<= 1;
      if (*p > CANVAS_LEVEL_BRIGHTEST)
        *p = CANVAS_LEVEL_BRIGHTEST;
    }
    if (args->variant == EFFECT_VARIANT_STRONGEST) {
      LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
    } else {
      LOAD_BG_PALETTE(RESOURCE_BLAST_SHEET);
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
    Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE, work->sheet, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET, FIGURE(work), 1,
                               1);
    LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
  }
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  if (mode == GASP_MODE_GASP || mode == GASP_MODE_LULL)
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
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
      s32 *lines = work->bg2xLines;

      for (i = 0; i != ARRAY_COUNT(work->bg2xLines); i++)
        *lines++ = (Random16() & 31) << 4;
    } else if (mode == GASP_MODE_LULL) {
      s32 *lines = work->bg2xLines;

      for (i = 0; i != ARRAY_COUNT(work->bg2xLines); i++)
        *lines++ =
            (toFIXED(6) - Trig_Sin((frame << 11) + i * (TRIG_TURN / 32)) * 6) >>
            10;
    } else if (mode == GASP_MODE_UNUSED && frame > 31) {
      u16 *pal = (u16 *)&BG_PLTT_COLOR(0, 1);

      for (i = 0; i != SHEET_PALETTE_COLORS - 1; i++, pal++) {
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
          for (i = 0; i != GASP_MOTES; i++) {
            struct Particle *p = &work->particles[i];
            s32 s;

            p->x = INTtoFIXED(to.x);
            p->y = INTtoFIXED(to.y);
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
        for (i = 0; i != GASP_MOTES; i++) {
          struct Particle *p = &work->particles[i];

          if (p->variant >= 0) {
            s32 n = (p->variant >> 3) + 2;

            draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                    FIXED_INTEGER_PART(p->x) - n / 2,
                    FIXED_INTEGER_PART(p->y) - n, n, n * 2);
            Particle_AdvanceWithGravity2D(p, 60, toFIXED(1) / 64);
            p->variant--;
          }
        }
      }
    } else if (mode == GASP_MODE_LULL) {
      s32 b = frame << 11;
      s32 x;
      s32 y;

      x = (FIXEDtoINT(Trig_Sin(b) * 20)) + screen[0] / 2 - 10;
      y = (FIXEDtoINT(Trig_Cos(b) * 4)) + screen[1] - 24;
      if (frame > 44) {
        twice = frame * 2;
        y = y - twice + 88;
      }
      draw[0](canvas, FIGURE(work), x, y, SMALL_DJINNI_WIDTH,
              SMALL_DJINNI_HEIGHT);
      BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120),
                                      DEGtoANG(120), 0);
    } else if (mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH) {
      struct ScreenPoint target;

      if (mode == GASP_MODE_REFLUX) {
        s32 r = frame;

        x = screen[0] / 2 - 10;
        y = screen[1] - 20;
        if (r > 12)
          r = 12;
        a = frame << 9;
        x -= FIXEDtoINT(Trig_Sin(a) * r * 2);
        y -= (FIXEDtoINT(Trig_Cos(a) * r)) - r;
        if (frame <= 61)
          draw[0](canvas, FIGURE(work), x, y, SMALL_DJINNI_WIDTH,
                  SMALL_DJINNI_HEIGHT);
      }
      if (frame == 0)
        Sound_PlayFar(SE_SWIRL);
      if (frame == 62)
        Sound_PlayFar(SE_DJINNI_SET);
      if (frame == 0) {
        for (i = 0; i != GASP_MOTES; i++) {
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
          s32 x =
              (FIXEDtoINT(Trig_Sin(p->z) * p->velocityZ / 2)) + target.x / 2;
          s32 y = (FIXEDtoINT(Trig_Cos(p->z) * p->velocityZ)) + target.y;

          p->z += 512;
          p->velocityZ -= 4;
          if (p->velocityZ > 0)
            draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                    x - n / 2, y - n, n, n * 2);
        }
        p->variant--;
      }
    } else if (mode == GASP_MODE_SHINE) {
      struct Particle *ball = &work->particles[GASP_CENTRE];
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
        ball->velocityY += (toFIXED(60) - other[1]) / 40;
        ball->velocityZ += (user->z - other[2]) / 40;
      } else {
        struct MotionObject *target =
            BattleObject_GetFar(args->targets[0])->object;
        struct Particle *fall = &work->particles[GASP_FALL];

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
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
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
        struct Particle *flash = &work->particles[GASP_FLASH];

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

      x = (FIXEDtoINT(Trig_Sin(b) * 20)) + screen[0] / 2 - 10;
      y = (FIXEDtoINT(Trig_Cos(b) * 4)) + screen[1] - 24;
      if (frame > 48) {
        twice = frame * 2;
        y = y - twice + 96;
      }
      if (y > -40)
        draw[0](canvas, FIGURE(work), x, y, SMALL_DJINNI_WIDTH,
                SMALL_DJINNI_HEIGHT);
    }

    // The model layer.
    Matrix_Initialize();
    gProjection.centerY = 104;
    verts = Heap_AllocEwram(1024);
    model = Render_AllocateModelList(1);
    model->texture = &texture;
    model->vertices = verts;
    if (mode == GASP_MODE_GASP) {
      if (frame == 0) {
        for (i = 0; i != 16; i++) {
          struct Particle *p = &work->particles[i];

          p->x = i << 15;
          p->y = toFIXED(56);
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
      model->kind = MODEL_SHADED_ADDITIVE;
      model->mesh = gBattleEffect_SmallSquareMeshReversed;
      for (i = 0; i != 6; i++) {
        struct Particle *p = &work->particles[i];

        if (p->variant >= 0) {
          FIXED size = 30000;
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
          Matrix_Initialize();
          p->y += p->velocityY;
          p->x += p->velocityX;
          Matrix_Translate(
              (Trig_Sin(p->x) << 4) + INTtoFIXED(screen[0] / 2 - 64), p->y, 0);
          Matrix_Scale(size >> 1, size, size >> 1);
          Matrix_RotateX(-DEGtoANG(90));
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
        p->variant++;
      }
    } else if (mode == GASP_MODE_LULL) {
      s32 k;

      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = MODEL_SHADED_MAX;
      model->texture = &texture;
      model->mesh = gBattleEffect_SmallSquareMeshReversed;
      model->vertices = verts;
      for (k = 0; k != 1; k++) {
        if (frame >= k * 32) {
          s32 t = frame - k * 32;
          s32 scale = (t << 10) + FIXED_ONE / 16;
          s32 shade = 0;

          if (t > 63) {
            shade = (64 - t) * 4;
            if (shade < MODEL_SHADE_INVISIBLE)
              continue;
          }
          model->shade = shade;
          Matrix_Initialize();
          Matrix_Scale(scale >> 1, scale, scale);
          Matrix_RotateZ((frame << 8) + TRIG_TURN * 3 / 4);
          Matrix_RotateX((frame << 6) - 18288);
          for (i = 0; i != 12; i++) {
            texture.pixels = work->sheet + (i % 3 << 10);
            Matrix_Push();
            Matrix_RotateZ(frame * 356 + i * 5461);
            Matrix_Translate(0, toFIXED(116), 0);
            Matrix_Scale(scale >> 1, scale >> 1, scale >> 1);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     4);
            Matrix_Pop();
            Render_DrawModelList(model);
          }
        }
      }
    } else if (mode == GASP_MODE_UNUSED) {
      model->shade = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gBattleEffect_SquareMesh;
      model->kind = MODEL_SHADED_MAX;
      texture.pixels = work->sheet;
      for (i = 0; i != args->targetCount; i++) {
        struct ScreenPoint pos;

        EffectPosition_ProjectScaledBattler(args->targets[i], &pos);
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
        Matrix_RotateX(DEGtoANG(90));
        Matrix_RotateY((frame << 9) - TRIG_TURN / 16);
        Matrix_Scale(frame * 2596 >> 1, frame * 2596 >> 1, frame * 2596 >> 1);
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
        model->kind = MODEL_SHADED_ADDITIVE;
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
        for (i = 0, ball = &work->particles[GASP_CENTRE], twist = frame * 2;
             i != 8; i++) {
          struct Particle *p = &work->particles[i];

          p->variant++;
          p->x += p->y * 2;
          model->scrollV = (twist + i * 7) & 31;
          if ((u32)p->variant <= 31) {
            Matrix_Initialize();
            Matrix_Translate(INTtoFIXED(ball->x) - toFIXED(64),
                             INTtoFIXED(ball->y) - toFIXED(64), 0);
            Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
            Matrix_RotateZ(p->x);
            Matrix_Scale(Trig_Sin(p->variant * 682) * 3 + FIXED_ONE * 3 / 4,
                         FIXED_ONE * 2, FIXED_ONE);
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
        model->kind = MODEL_SHADED_ADDITIVE;
        model->texture = &texture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = verts;
        for (i = 0; i != 3; i++) {
          s32 t = frame - i * 4 - 55;

          if (t >= 0) {
            s32 scale = (t * 3 << 12) + FIXED_ONE / 4;
            s32 shade = 0;

            if (t > 7)
              shade = (8 - t) * 8;
            model->shade = shade;
            if (shade > MODEL_SHADE_INVISIBLE) {
              Matrix_Initialize();
              Matrix_Translate(INTtoFIXED(pos.x / 2 - 64),
                               INTtoFIXED(pos.y - 64), 0);
              Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
              Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
              Matrix_RotateX(-TRIG_TURN / 16);
              Matrix_ScaleUniform(scale);
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
        model->kind = MODEL_SHADED_ADDITIVE;
        texture.pixels = work->sheet;
        EffectPosition_ProjectScaledBattler(args->targets[0], &pos);
        for (i = 0, scale = t * -4592 + FIXED_ONE; i != 2;
             scale += FIXED_ONE / 4, i++) {
          if (scale > 0) {
            s32 shade = -(i << 4);

            if (shade > 0)
              shade = 0;
            model->shade = shade;
            Matrix_Initialize();
            Matrix_Translate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48),
                             0);
            Matrix_RotateX(TRIG_TURN * 3 / 4);
            Matrix_Scale(scale, scale, scale);
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
        struct Particle *ball = &work->particles[GASP_CENTRE];
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
          q->x = INTtoFIXED(to.x / 2);
          q->y = INTtoFIXED(to.y + 16);
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
        p->x = INTtoFIXED(from.x / 2);
        p->y = INTtoFIXED(from.y + 8);
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
                  FIXED_INTEGER_PART(p->x) - 12, FIXED_INTEGER_PART(p->y) - 24,
                  SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
          Particle_AdvanceWithGravity2D(p, 60, -toFIXED(1) / 16);
          p->variant++;
        }
      }
      if (frame > 9) {
        for (i = 0; i != gGaspFamily_Counts[args->variant]; i++) {
          struct Particle *q = &EXTRA_PARTICLES[i];

          if (q->variant >= 0) {
            s32 n = q->variant / 4 + 3;

            draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                    FIXED_INTEGER_PART(q->x) - n / 2,
                    FIXED_INTEGER_PART(q->y) - n, n, n * 2);
            Particle_AdvanceWithGravity2D(q, 62, 0);
            q->variant--;
          }
        }
      }
      if (frame > 9) {
        struct Particle *ball = &work->particles[GASP_CENTRE];
        s32 scale = ((frame - 10) << 15) + FIXED_ONE / 4;
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
          model->kind = MODEL_SHADED_ADDITIVE;
          texture.pixels = work->sheet + GASP_FIRE_RING;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(ball->x / 2 - 64),
                           INTtoFIXED(ball->y - 48), 0);
          Matrix_Scale(scale / 4, scale, scale / 2);
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
          a = (Random16() & 0x7fff) + DEGtoANG(90);
          s += 128;
          p->velocityX = Trig_Sin(a) * s >> 7;
          p->velocityY = Trig_Cos(a) * s >> 6;
          p->velocityZ = toFIXED(1) / 64;
          SET_POSITION(p, INTtoFIXED(pos.x / 2),
                       INTtoFIXED(pos.y - (Random16() & 31) + 16), Random16());
        }
        Sound_PlayFar(SE_POWER_UP);
        work->shakeFrames = 16;
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_RED_TINT,
                                 TINT_KEEP, 0, 32);
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = MODEL_SHADED_MAX;
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
          Matrix_ScaleUniform(FIXED_ONE / 2);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
          Render_DrawModelList(model);
          Particle_AdvanceWithGravity3D(p, 60, 0);
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
          a = (Random16() & 0x7fff) + DEGtoANG(90);
          s += 128;
          p->velocityX = Trig_Sin(a) * s >> 6;
          p->velocityY = Trig_Cos(a) * s >> 6;
          p->velocityZ = toFIXED(1) / 64;
          SET_POSITION(p, INTtoFIXED(pos.x / 2),
                       INTtoFIXED(pos.y - (Random16() & 31) - 16), Random16());
        }
        Sound_PlayFar(SE_619);
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = MODEL_SHADED_MAX;
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
          Matrix_ScaleUniform(FIXED_ONE / 2);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
          Render_DrawModelList(model);
          Particle_AdvanceWithGravity3D(p, 60, 0);
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
            q->x = INTtoFIXED(pos.x / 2) + q->velocityX * 2;
            q->y = INTtoFIXED(pos.y + 16) + q->velocityY * 2;
            q->variant = (Random16() & 15) + 16;
          }
        }
        for (p = EXTRA_PARTICLES, i = 0; i != 64; i++, p++) {
          if (p->variant >= 0) {
            s32 j;

            for (j = 0; j != 4; j++) {
              s32 n = p->variant / 8 + 1;

              draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                      FIXED_INTEGER_PART(p->x) - n / 2,
                      FIXED_INTEGER_PART(p->y) - n, n, n * 2);
              Particle_AdvanceWithGravity2D(p, 63, toFIXED(1) / 32);
              if (p->y > toFIXED(104)) {
                p->y = toFIXED(104);
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
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &texture;
      model->mesh = gBattleEffect_ColumnMesh;
      model->vertices = verts;
      Matrix_Initialize();
      Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
      if (args->side == EFFECT_SIDE_ENEMIES)
        Matrix_Translate(INTtoFIXED(pos.x - 136), INTtoFIXED(pos.y - 48), 0);
      else
        Matrix_Translate(INTtoFIXED(pos.x - 108), INTtoFIXED(pos.y - 56), 0);
      for (i = 0; i != 4; i++) {
        s32 scale;

        Matrix_Push();
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
            Matrix_Translate(d * 3 << 13, d * 7 << 11, 0);
          else
            Matrix_Translate(-(d * 3 << 13), d * 7 << 11, 0);
          angle = d * 48152 / 80 + TRIG_TURN * 3 / 8;
          if (args->side == EFFECT_SIDE_ENEMIES)
            angle = -angle;
          Matrix_RotateZ(angle);
        } else {
          if (i != 0)
            continue;
          Matrix_RotateZ(args->side == EFFECT_SIDE_ENEMIES ? -TRIG_TURN * 3 / 8
                                                           : TRIG_TURN * 3 / 8);
        }
        model->shade = -i << 3;
        if (frame <= 23)
          scale = frame * 2730;
        else if (frame > 31)
          scale = FIXED_ONE * 4 - (frame * 3 << 11);
        else
          scale = FIXED_ONE;
        if (scale > 0) {
          Matrix_Scale(scale * 6, scale * 2, FIXED_ONE);
          Render_TransformVertices(gBattleEffect_LongBeamCorners, verts, 4);
          Render_DrawModelList(model);
          Matrix_Pop();
        }
      }
      BattleCamera_Shake(8, 8);
    } else if (mode == GASP_MODE_DEMON_EYE) {
      struct ScreenPoint pos;

      work->transferValue = CANVAS_FADE_HALF;
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->kind = MODEL_SHADED_MAX;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
      texture.pixels = work->sheet;
      EffectPosition_ProjectScaledBattler(args->user, &pos);
      if (frame <= 15) {
        for (i = 0; i != 2; i++) {
          s32 scale = Trig_Sin(frame << 11) * 5;

          if (scale > 0) {
            model->shade = 0;
            Matrix_Initialize();
            Matrix_Translate(INTtoFIXED(pos.x / 2 + i * 16 - 72),
                             INTtoFIXED(pos.y - 64), 0);
            Matrix_Scale(scale / 2, scale, scale / 2);
            Matrix_RotateZ(DEGtoANG(45));
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
        Sound_PlayFar(SE_LAUNCH);
      EffectPosition_ProjectBattler(args->targets[0], &pos);
      shade = frame * 4 - 64;
      if (shade >= -16)
        shade = -16;
      model->shade = shade;
      model->texture = &texture;
      model->vertices = verts;
      model->kind = MODEL_SHADED_MAX;
      model->mesh = gBattleEffect_CylinderMesh;
      texture.pixels = work->sheet;
      texture.logWidth = 7;
      texture.logHeight = 4;
      if (frame == 26) {
        work->shakeFrames = 8;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
      for (i = 0; i != 8; i++) {
        s32 scale = FIXED_ONE * 2;
        s32 start = i * 2 + 16;

        if (frame >= start)
          scale = FIXED_ONE * 2 - ((frame - start) << 13);
        if (scale > FIXED_ONE / 2) {
          model->scrollU = ((frame + i * 5) & 7) << 4;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(pos.x / 2 - 60), toFIXED(8), 0);
          Matrix_Scale(scale >> 1, scale, FIXED_ONE);
          Matrix_RotateX((i << 13) + 4000);
          Matrix_RotateY(frame << 9);
          Matrix_RotateZ((frame + i * 16) << 9);
          Matrix_Scale(FIXED_ONE, FIXED_ONE / 2, FIXED_ONE);
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
      model->kind = MODEL_SHADED_MAX;
      EffectPosition_ProjectScaledBattler(args->user, &pos);
      Matrix_Initialize();
      Matrix_Translate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
      if (frame <= 15) {
        s32 scale = FIXED_ONE * 3 - (frame * 3 << 12);

        model->shade = 0;
        Matrix_Scale(scale >> 1, scale, scale >> 1);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
      Matrix_Initialize();
      Matrix_Translate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
      if ((u32)(frame - 12) <= 31) {
        s32 scale = Trig_Sin((frame << 10) - TRIG_TURN * 3 / 16) * 3;

        texture.pixels = work->sheet;
        Matrix_Scale(scale / 2, scale, scale / 2);
        Matrix_RotateZ((frame << 9) + DEGtoANG(45));
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
        LOAD_BG_PALETTE(RESOURCE_BLAST_SHEET);
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
            BattleEffect_StepPaletteToResource(RESOURCE_MERCURY_DJINN_SHEET);
          else if (frame <= 23)
            BattleEffect_StepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
          else
            BattleEffect_StepPaletteToResource(RESOURCE_JUPITER_DJINN_SHEET);
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
      model->kind = MODEL_SHADED_MAX;
      EffectPosition_ProjectScaledBattler(args->user, &pos);
      for (i = 0; i != 2; i++) {
        s32 t = frame - i * 8;

        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
        if ((u32)t <= 15) {
          s32 scale = FIXED_ONE * 4 - (t << 14);
          s32 shade = 0;

          if (t <= 7)
            shade = t * 8 - 64;
          model->shade = shade;
          Matrix_Scale(scale / 2, scale, scale / 2);
          Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
          Matrix_RotateX(-TRIG_TURN / 16);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
      Matrix_Initialize();
      Matrix_Translate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
      if ((u32)(frame - 12) <= 31) {
        s32 scale = Trig_Sin((frame << 10) - TRIG_TURN * 3 / 16) * 2;

        texture.pixels = work->sheet;
        Matrix_Scale(scale / 2, scale, scale / 2);
        Matrix_RotateZ((frame << 9) + DEGtoANG(45));
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
      model->kind = MODEL_SHADED_ADDITIVE;
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
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
        Matrix_RotateX(DEGtoANG(90));
        Matrix_Scale(scale / 3, scale, scale);
        if (mode == GASP_MODE_REFLUX)
          Matrix_RotateY(-frame << 10);
        else
          Matrix_RotateY(frame << 10);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
    BattleEffect_CountDownBattlerTints();
    if (mode == GASP_MODE_UNUSED)
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
#undef PLACE_PARTICLE
#undef WAIT_TRANSFER
