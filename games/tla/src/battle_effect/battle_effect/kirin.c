/// @file
/// The Kirin summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"

#undef FORMATION

// The map cell buffer holds the hit bursts, 32 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

#define FORMATION 644

struct FxPair {
  s32 a;
  s32 b;
};

struct FxPlacement {
  s32 x;
  s32 y;
  s32 z;
  s32 unk0c;
};

struct FxControl {
  u8 unk00[16];
  s32 busy;
};

extern char ResourceId_FireBlobSheet;
extern struct FxPair gData_08196e7c; // ☀️ gObjectRow_SweepPair
extern struct FxPair gData_08196e84; // ☀️ gObjectRow_RisePair
extern u8 gKirin_Columns[];          // the row's x offsets
extern u8 gKirin_Rows[];             // the row's heights

// Summon 3: the battlefield turns to forest; a row of nine riders gallops in
// from the right over a ring of embers, trailing fire, while flames climb at
// the left; then the row charges across, striking each target as it passes
// in a burst of sparks, and a wall of flame sweeps after it.
//
// Act one (120 frames, A or B ends it after frame 16): the backdrop scrolls
// faster from frame 24; the row bobs in, its riders on a sine wave, the
// screen shaking every 32 frames; the ember ring grows over the first 27
// frames; the fire trail from frame 18; the flames from 32. Act two (96
// frames): the row sweeps left 16 pixels a frame, rising; each target it
// passes takes a burst of 32 sparks; the flames slide across from frame 41.
void BattleEffect_RunKirin(struct BattleEffectArgument *effect) {
  struct FxPlacement place;
  struct EffectPosition unit;
  u8 jitterRow[16];
  u8 hitRow[14];
  struct FxPlacement place2;
  void *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw;
  u8 *sheet;
  s32 baseX;
  s32 grow;
  s32 savedX;
  struct FxControl *control;
  s32 shift;
  s32 wave;
  s32 frame;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];

  // Open the canvas over the forest and spawn the row.
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  *(u16 *)BG_PLTT = 0;
  *(u16 *)(BG_PLTT + 1) = 0x18;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_WipeCanvas(1, 0);
  BattleEffect_SpawnObjects(9, 644, 1);
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);
  REG_WININ = WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 |
              WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG0 |
              WININ_WIN1_BG1 | WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = 0xca;
  wait(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
  BattleEffect_WipeCanvas(1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBlobSheet, work->sheet, 1, 1);
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
                DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;

  // The fire trail's 64 flames wait at the left, each with its own delay.
  baseX = 0;
  grow = 0;
  savedX = gBgScroll[1].x;
  control = (struct FxControl *)Ram_HeapSlots->transitionWork;
  shift = 0;
  work->transferMode = 1;
  work->transferValue = baseX;
  control->busy = 1;

  i = 0;
  do {
    struct EffectStep *fire = &work->particles[i];

    fire->x = (Random16() & 31) + 16;
    fire->y = ((Random16() & 31) + 48) << 16;
    fire->velocityY = ((Random16() & 31) - 16) << 16;
    fire->variant = Random16() % 48 + 2;
    i++;
  } while (i != 64);

  BattleEffect_LoadBlitter(
      HEAP_BLITTER,
      BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  for (frame = 0; frame != 120; frame++) {
    wave = 0;
    if (frame == 0)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 26)
      Sound_PlayFar(SE_RUMBLE);
    if (frame == 40)
      Sound_PlayFar(SE_CAST);
    if (frame == 72)
      Sound_PlayFar(SE_CAST);
    if (frame == 104)
      Sound_PlayFar(SE_CAST);
    if ((gInput.repeat & (KEY_A | KEY_B)) != 0 && frame > 16)
      break;

    // The forest scrolls ever faster.
    if (frame >= 24 && frame <= 55)
      shift++;
    if (shift > 24)
      shift = 24;
    if (frame <= 135) {
      gBgScroll[1].x -= shift;
      grow += shift;
    }

    // The row bobs in on a sine wave, shaking the screen every 32 frames.
    if (frame <= 149) {
      struct FxPair pair = gData_08196e7c;
      s32 lift;

      lift = 0;
      if (frame > 103)
        lift = frame * 16 - 1664;
      if (frame >= 8 && frame <= 31)
        baseX = baseX + shift - 8;
      if (frame > 7) {
        s32 amp;
        s32 ang;

        amp = 96;
        if (frame <= 104)
          amp = 32;
        ang = (frame * 1024 - 0x2000) & 0xffff;
        if (ang > 0x8000)
          ang -= 0x8000;
        wave = (Trig_Sin(ang) * amp) >> 16;
        if ((frame & 31) == 8)
          work->shakeFrames = 4;
      }
      place.unk0c = 0;
      place.y = px(255);
      i = 0;
      do {
        place.x = ((baseX + gKirin_Columns[i] - lift) << 16) + px(224);
        place.z = ((gKirin_Rows[i] - wave) << 16) + (144 << 15);
        Render_ApplyProjectedPlacementFar(work->objects[i], &place.x, &pair.a,
                                          0);
        i++;
      } while (i != 9);
    }

    // The ember ring, growing over the first 27 frames.
    if (frame <= 26) {
      s32 size = grow + 4;
      s32 count = frame * 8;

      if (size > 10)
        size = 10;
      if (count > 64)
        count = 64;
      for (i = 0; i != count; i++) {
        s32 ang = i << 10;
        s32 x = ((Trig_Sin(ang) * (grow * 2 + 8)) >> 16) + grow + 96;
        s32 y = ((Trig_Cos(ang) * (grow * 12 + 48)) >> 16) + 64;

        draw(canvas, sheet + gBattleEffect_MoteCells[size - 1], x - size / 2,
             y - size, size, size * 2);
      }
    }

    if (frame == 24) {
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 28)
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

    // The fire trail streams back from the riders.
    if (frame > 17) {
      i = 0;
      do {
        struct EffectStep *fire = &work->particles[i];

        if (fire->variant == 0) {
          s32 size = i % 3 + 1;

          draw(canvas, sheet + gBattleEffect_MoteCells[size - 1], fire->x,
               HI(fire->y) - size, size, size * 2);
          fire->x += 2;
          fire->y += fire->velocityY;
          fire->velocityY = fire->velocityY * 48 / 64;
        } else {
          fire->variant--;
        }
        if (fire->x > 128 || fire->variant == 1) {
          fire->x = (Random16() & 31) + baseX + 172;
          fire->y = ((Random16() & 31) - wave + 56) << 16;
          fire->velocityY = ((Random16() & 31) - 16) << 15;
        }
        i++;
      } while (i != 48);
    }

    // The flames climb at the left.
    if (frame > 31) {
      s32 rise = (frame - 32) / 2;
      s32 y;

      if (rise > 40)
        rise = 40;
      i = 0;
      y = 0;
      do {
        draw(canvas, work->sheet + (Random16() & 3) * 0x600, 120 - rise, y, 48,
             32);
        i++;
        y += 18;
      } while (i != 6);
    }

    // The shake jolts BG1 alone.
    if (work->shakeFrames > 0) {
      work->shakeFrames--;
      gBgScroll[1].y = (Random16() & 7) + 28;
    } else {
      gBgScroll[1].y = 32;
    }
    work->transferPending = 1;
    wait(1);
  }

  // Act two: the charge.
  gBgScroll[1].x = savedX;
  control->busy = 0;
  BattleEffect_SetupBlendedDisplay();
  REG_WIN0H = 0xf0;

  i = 0;
  do {
    ((u8 *)work->objects[i])[9] |= 12;
    i++;
  } while (i != 9);

  {
    s32 rowX;
    u8 *hit;
    u8 *jitter;

    rowX = 224;
    hit = hitRow;
    i = 0;
    do {
      hit[i] = 0;
      i++;
    } while (i != 14);
    jitter = jitterRow;
    i = 0;
    do {
      jitter[i] = Random16() & 31;
      i++;
    } while (i != 16);
    i = 0;
    do {
      PARTICLES[i].variant = 0;
      i++;
    } while (i != 160 << 1);

    work->transferMode = 2;
    work->transferValue = 75;
    REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);

    frame = 0;
    do {
      if (frame <= 23) {
        struct FxPair pair2 = gData_08196e84;
        s32 ang;
        s32 rise;

        rowX -= 16;
        if (frame <= 8) {
          ang = frame * 2048 + 0x4000;
          if (ang > 0x8000)
            ang -= 0x8000;
          rise = (Trig_Sin(ang) << 6) >> 16;
        } else {
          ang = frame * 2048 + 0x4000;
          if (ang > 0x8000)
            ang -= 0x8000;
          rise = (Trig_Sin(ang) << 5) >> 16;
        }
        place2.unk0c = 0;
        place2.y = px(255);
        i = 0;
        do {
          place2.x = (rowX + gKirin_Columns[i]) << 16;
          place2.z = ((gKirin_Rows[i] - rise) << 16) + (144 << 15);
          Render_ApplyProjectedPlacementFar(work->objects[i], &place2.x,
                                            &pair2.a, 0);
          i++;
        } while (i != 9);
      }

      if (frame == 8) {
        work->shakeFrames = frame;
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 11)
        Sound_PlayFar(SE_CRASH);
      if (frame == 46)
        Sound_PlayFar(SE_137);

      // Each target the row passes is struck once, in a burst of sparks.
      for (i = 0; i != effect->count; i++) {
        if (hit[i] == 0) {
          EffectPosition_ProjectBattler(effect->actors[i], &unit);
          if (unit.x > rowX) {
            s32 k;

            hit[i] = 1;
            k = 0;
            do {
              struct EffectStep *burst = &PARTICLES[i * 32 + k];
              s32 up;

              burst->x = unit.x << 15;
              burst->y = (unit.y - 16) << 16;
              burst->velocityX = ((Random16() & 255) - 128) << 10;
              up = (Random16() & 255) - 192;
              burst->velocityY = up << 11;
              burst->x += burst->velocityX * 4;
              burst->y += up << 13;
              burst->variant = (Random16() & 15) + 8;
              k++;
            } while (k != 32);
            BattleMotion_ApplyKnockbackFar(effect->actors[i], 1);
            Sound_PlayFar(SE_HEAVY_IMPACT);
          }
        }
      }

      i = 0;
      do {
        struct EffectStep *burst = &PARTICLES[i];

        if (burst->variant > 0) {
          s32 size = 3;

          draw(canvas, sheet + gBattleEffect_MoteCells[size - 1],
               HI(burst->x) - size / 2, HI(burst->y) - size, size, size * 2);
          burst->x += burst->velocityX;
          burst->y += burst->velocityY;
          burst->variant--;
        }
        i++;
      } while (i != 192);

      if (frame == 48)
        Sound_PlayFar(SE_WHOOSH);

      // The wall of flame slides across after the row.
      if (frame > 40) {
        s32 slide = (frame - 40) * 12;
        s32 y;

        work->transferMode = 0;
        work->transferValue = 75;
        i = 0;
        y = -8;
        do {
          draw(canvas, work->sheet + (Random16() & 3) * 0x600,
               jitter[i] - slide + 120, y, 48, 32);
          i++;
          y += 8;
        } while (i != 16);
      }
      if (frame > 64)
        work->transferMode = 2;
      if (frame == 58) {
        for (i = 0; i != effect->count; i++)
          BattleEffect_TintBattler(effect->actors[i], 14, 5, -1, 0);
      }

      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
    } while (frame != 96);
  }

  BattlePlayback_StartFar(PHASE_SUMMON_RESULTS);
  i = 0;
  do {
    Sprite_DestroyFar(work->objects[i]);
    i++;
  } while (i != 9);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

/// BG2 back to one texel per pixel across.
void BattleEffect_ResetBg2Pa(void) { REG_BG2PA = Q_8_8(1); }

/// BG2 at half a texel per pixel across: twice as wide.
void BattleEffect_SetBg2AffineScaleHalf(void) { REG_BG2PA = Q_8_8(0.5); }

#undef SCENE_FRAMES
#undef RESULTS_FRAME
#undef SPARK_COUNT
#undef HIT_FRAME

// Here PARTICLES, the map cell buffer, holds the spray (and, at the start,
// the serpent's tiles and the window's column noise).

#define SERPENT_PARTS 8 // four across, two down, 48 pixels apart
#define SPRAY_COUNT 1024
#define SPARKLE_SHEET 0x4e20 // the sparkle cells after the spray picture

#define SCENE_FRAMES 244  // act one; A or B ends it after frame 16
#define OPEN_FRAMES 16    // the window opens over the sea
#define SERPENT_FRAME 22  // the serpent rises
#define SECOND_SPRAY 76   // a second fountain of spray
#define GATHER_FRAME 152  // light gathers at its mouth
#define RAYS_FRAME 176    // the rays reach out
#define BREATH_FRAME 222  // and it breathes
#define PILLAR_FRAMES 144 // act two
#define RESULTS_FRAME 96

/* VIEW(ability_effects): tag struct SpriteObject as struct
 * SpriteObject__ability_effects; battle_effect/cybele has another definition;
 * not tried: struct SpriteObject__ability_effects was not found. */
struct SpriteObject {
  u16 unk00;
  u16 unk02;
  u16 unk04;
  u16 unk06;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[6];
  u8 slot;
};
