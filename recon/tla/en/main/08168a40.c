// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 18 of
// 1065 with three empty asm the greedy device search placed (26 without
// them); the rest is the instruction order described below.
// Draft, differ 126 of 1065: BattleEffect_RunKirin, summon 3 (BattleEffect_RunSceneEffect's case 3,
// actflags 0x8003), The Broken Seal's BattleEffect_RunKirin.

// Twin: ☀️ BattleEffect_RunKirin (games/tbs/src/effect/effect.c), ported:
// Func_0813ba50 sets the display, the second BG colour is 0x18, the formation
// is resource 644, the projection's centre is gCameraSceneParameters[4], the
// control is the heap slot at 0xb0, one blitter (BattleEffect_LoadBlitter(104, 3)), the
// effect stays an argument for the hit loops, callback BattleEffect_ProcessPendingGraphicsTransfer at
// 0xc80.

// The objects' flag byte is or-ed as a byte (as a struct field the allocation
// of that block moved: 171).

// Left: the 126 differing rows are the native's own rows reordered: loads and
// stores ahead of independent constants, the kind Func_08164b2c's header
// traces to a load costing 3 and a store 2.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "input.h"
#include "bg_scroll.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The map cell buffer holds the hit bursts, 32 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).
#define HEAP_BLITTER 104

// BattleEffect_LoadBlitter's argument: The Lost Age's blitter mode (☀️'s counted down
// from 3) in the high nibble, the COMPOSITE_ flags in the low one.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define BLIT_ADDITIVE 0

#define FORMATION 644
#define FORMATION_SIZE 9

#define PHASE_SUMMON_RESULTS 134
#define SE_GALLOP 136
#define SE_EMBERS 141
#define SE_FLAMES 154
#define SE_CHARGE 145
#define SE_FADE 137
#define SE_HIT 134

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

struct ScaledSprite;

extern char ResourceId_ForestBackdrop;
extern char ResourceId_Data134; // ☀️ ParticleSpritesA
extern char ResourceId_FireBlobSheet;
extern struct FxPair gData_08196e7c; // ☀️ gObjectRow_SweepPair
extern struct FxPair gData_08196e84; // ☀️ gObjectRow_RisePair
extern u8 gData_08198ada[];          // the row's x offsets
extern u8 gData_08198ae3[];          // the row's heights
extern u16 gData_08197410[];         // ☀️ gBattleEffect_MoteCells
extern u32 gCameraSceneParameters[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void Func_0813ba50(void); // BattleEffect_ConfigureEffectDisplay
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void Func_08163c2c(s32 mode, s32 value);
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // BattleEffect_SelectLivingTargets
void BattleEffect_SpawnObjects(s32 count, s32 resource,
                   s32 priority); // BattleEffect_SpawnObjects
void ResourceObject_ReleaseFar(void *object);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter(slot,
                                                // 7, 7, flags, mode)
void BattleBackground_LoadFar(s32 layer, s32 resourceId, s32 mode);
void Sound_PlayFar(s32 cue);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
void Func_08118088(s32 actor,
                   s32 variant); // BattleMotion_ApplyVariantMotionFar
void Func_0814cd48(s32 actor, s32 palette, s32 a, s32 timer,
                   s32 frames);   // ObjectGroup_UpdateMembers
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void ObjectGroup_CountDownMemberTimers(void);         // ObjectGroup_CountDownMemberTimers
void Func_081180e8(s32 phase);    // BattleEventRuntime_StartPhaseFar
void Func_0814cca8(void);         // BattleEffect_SetupBlendedDisplay
void BattleEffect_EndCanvasLayer(void);         // BattleEffect_EndCanvasLayer

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
  __asm__("");

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];

  // Open the canvas over the forest and spawn the row.
  BattleEffect_StartCanvasLayer(0);
  Func_0813ba50();
  REG_BG2CNT = 0x784;
  *(u16 *)0x05000000 = 0;
  *(u16 *)0x05000002 = 0x18;
  work->transferMode = 0;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Func_08163c2c(1, 0);
  BattleEffect_SpawnObjects(9, 644, 1);
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);
  REG_WININ = 0x2737;
  REG_WIN0H = 0xca;
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
  Func_08163c2c(1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBlobSheet, work->sheet, 1, 1);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f44;

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

  BattleEffect_LoadBlitter(HEAP_BLITTER,
                BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BG2CNT = 0x786;

  for (frame = 0; frame != 120; frame++) {
    wave = 0;
    if (frame == 0)
      Sound_PlayFar(SE_GALLOP);
    if (frame == 26)
      Sound_PlayFar(SE_EMBERS);
    if (frame == 40)
      Sound_PlayFar(SE_FLAMES);
    if (frame == 72)
      Sound_PlayFar(SE_FLAMES);
    if (frame == 104)
      Sound_PlayFar(SE_FLAMES);
    if ((gInput.repeat & 3) != 0 && frame > 16)
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
      place.y = 255 << 16;
      i = 0;
      do {
        place.x = ((baseX + gData_08198ada[i] - lift) << 16) + (224 << 16);
        place.z = ((gData_08198ae3[i] - wave) << 16) + (144 << 15);
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

        draw(canvas, sheet + gData_08197410[size - 1], x - size / 2, y - size,
             size, size * 2);
      }
    }

    if (frame == 24) {
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 28)
      REG_BG2CNT = 0x784;

    // The fire trail streams back from the riders.
    if (frame > 17) {
      i = 0;
      do {
        struct EffectStep *fire = &work->particles[i];

        if (fire->variant == 0) {
          s32 size = i % 3 + 1;

          draw(canvas, sheet + gData_08197410[size - 1], fire->x,
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
    WaitFrames(1);
  }

  // Act two: the charge.
  gBgScroll[1].x = savedX;
  control->busy = 0;
  Func_0814cca8();
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
  __asm__("");
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
    REG_BG2CNT = 0x784;
    REG_BLDALPHA = 0x1010;

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
        place2.y = 255 << 16;
        i = 0;
        do {
          place2.x = (rowX + gData_08198ada[i]) << 16;
          place2.z = ((gData_08198ae3[i] - rise) << 16) + (144 << 15);
          Render_ApplyProjectedPlacementFar(work->objects[i], &place2.x,
                                            &pair2.a, 0);
          i++;
        } while (i != 9);
      }

      if (frame == 8) {
        work->shakeFrames = frame;
        Sound_PlayFar(SE_CHARGE);
      }
      if (frame == 11)
        Sound_PlayFar(SE_CHARGE);
      if (frame == 46)
        Sound_PlayFar(SE_FADE);

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
            Func_08118088(effect->actors[i], 1);
            Sound_PlayFar(SE_HIT);
          }
        }
      }

      i = 0;
      do {
        struct EffectStep *burst = &PARTICLES[i];

        if (burst->variant > 0) {
          s32 size = 3;

          draw(canvas, sheet + gData_08197410[size - 1],
               HI(burst->x) - size / 2, HI(burst->y) - size, size, size * 2);
          burst->x += burst->velocityX;
          burst->y += burst->velocityY;
          burst->variant--;
        }
        i++;
      } while (i != 192);

      if (frame == 48)
        Sound_PlayFar(SE_GALLOP);

      // The wall of flame slides across after the row.
      if (frame > 40) {
        s32 slide = (frame - 40) * 12;
        s32 y;

        work->transferMode = 0;
        work->transferValue = 75;
  __asm__("");
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
          Func_0814cd48(effect->actors[i], 14, 5, -1, 0);
      }

      Func_08158ce0(8, 8);
      ObjectGroup_CountDownMemberTimers();
      work->transferPending = 1;
      WaitFrames(1);
      frame++;
    } while (frame != 96);
  }

  Func_081180e8(PHASE_SUMMON_RESULTS);
  i = 0;
  do {
    ResourceObject_ReleaseFar(work->objects[i]);
    i++;
  } while (i != 9);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
