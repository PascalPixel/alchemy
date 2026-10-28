// Draft of BattleEffect_RunThor, Thor (scene kind 12): differ 1410 of 1676
// (draft 1670; 2026-10-26: the twin's fullWidth barrier and the s32 returns
// of the blitter loaders and Task_Add, as defined, 1439 to 1410), the
// same 125 calls in the same order. Left: the stack frame is 328 bytes for 324,
// which shifts every stack offset, and the allocation differs from the
// prologue on (the native keeps the dimmed-copy counter in r9 and the
// effect at [sp, #120]). The native stack frame has an unused word at sp+96, as
// the twin's blitter pair leaves; with the pair here the stack frame is 332, so
// this draft spills two words the native keeps in registers (this draft's slots
// 12 to 44 hold one more variable, and r4's caller-save slot at sp+8 is used
// 22 times in the native, 8 here).

// Twin: The Broken Seal's BattleEffect_RunThor (effect.c), ported. The Lost Age
// loads the light fan once, copies palette 0x184 to BG palette RAM after the
// beam ramp and steps the dusk palette through Func_0815f0a0 (four
// resources by frame & 63, twice a frame) where ☀️ calls
// Graphics_UpdatePhasePalette. Its lightning part runs 128 frames, not 192,
// with 18 bolts, not 30, the phase at frame 120, not 140, and draws the four
// mirrored copies of the violet mote through Func_0818caa8. The strikes read
// the effect from the argument; the blitters live in heap slots 104 and 188
// (byte offsets), loaded through BattleEffect_LoadBlitter, and the smoke's through
// Func_08196404; modes count down from 3 (BLIT_MODE).

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "input.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "macros.h"

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Func_0813baec(void); // BattleEffect_AdvanceScrollOnInterval
void Func_0813ba50(void); // BattleEffect_ConfigureEffectDisplay
void BattleEffect_CountDownBattlerTints(void);
void Func_08163c2c(s32 page, s32 value);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
s32 Func_08196404(s32 slot, s32 widthLog, s32 heightLog, s32 flags,
                   s32 mode);     // BattleEffect_BuildBlitter, five arguments
void Func_0815f0a0(s32 resource); // the dusk palette step
void Func_0814cca8(void);         // BattleEffect_SetupBlendedDisplay
void BattleEffect_SelectLivingTargets(struct BattleEffectArgument *effect); // SelectLivingTargets
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode);   // SpawnObjects
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);       // BattleEffect_TintBattler
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_0816442c(s32 group, s32 x, s32 y); // BattleEffect_PlaceFormationObjects
void Func_0816467c(s32 group, s32 x, s32 y); // BattleEffect_RunImpactBurst
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void Func_0818caa8(const u8 *cells, s32 x, s32 y, s32 height); // four motes
void Func_080150e4(s32 angle);  // SceneTransform_ApplyRoll
void Func_080151ac(s32 *scale); // SceneTransform_ApplyScale
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Projection_Set(s32 a, s32 b, s32 c);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void BattleBackground_LoadFar(s32 layer, s32 resource, s32 mode);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
s32 Task_Add(void (*func)(void), s32 order);
s32 Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void ResourceObject_ReleaseFar(void *object);
void Sound_PlayFar(s32 cue);
void WaitFrames(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define BLIT_CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// Heap slots 104 and 188 (byte offsets from 0x03000000) hold the blitters.
#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])
#define BLITTER_ALTERNATE ((DrawRectangle)Ram_HeapSlots->unkb8[1])

extern char ResourceId_BlastSheet;
extern char ResourceId_CrystalSheet;
extern char ResourceId_Data134;
extern char ResourceId_Data135;
extern char ResourceId_DuskCloudsBackdrop;
extern char ResourceId_JupiterDjinnSheet;
extern char ResourceId_LightFanSheet;
extern char ResourceId_LightningPillarSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_ParticleSpritesC;
extern char ResourceId_TornadoSheet;
extern char ResourceId_VioletLightningSheetB;
extern char ResourceId_VioletMoteSheet;
extern u32 gCameraSceneParameters[];
extern const s16 Data_08196e2c[6][2]; // the three lines' end points
extern const u16 gData_08197410[];    // gBattleEffect_MoteCells
extern const u16 gData_08197486[];    // gBattleEffect_GlintCells
extern const u8 gData_08197492[];     // gBattleEffect_GlintCellWidths
extern const u8 gData_08197498[];     // gBattleEffect_GlintCellHeights
extern const u8 Data_0819855e[];      // gThor_TargetPalettes
extern const u8 Data_08198562[];      // gThor_SmokeFlips

#define STORM_PARTICLES ((struct EffectStep *)(Ram_MapCellBuffer + 0xc58))
#define STORM_MOTE_CELLS 0x6000
#define STORM_PARTICLE_COUNT 910
#define STORM_DIMMED_DOTS 10000
#define STORM_DIMMED_DOT_BYTES 0x302
#define STORM_DUSK_FRAMES 170
#define STORM_LIGHTNING_FRAMES 128
#define STORM_BOLTS 18

// Abilities: Thor (scene kind 12).
//
// As ☀️'s: in the dusk part (STORM_DUSK_FRAMES, which A or B cuts short) a
// circle of 128 dots and three lines across it form over scrolling dusk
// clouds, turn, sink into the sky and send down a beam of light; in the
// lightning part (STORM_LIGHTNING_FRAMES) a pillar of lightning strikes the
// formation and throws sparks, STORM_BOLTS violet bolts follow while a violet
// mote sways above, and the formation is pushed back in smoke until an
// impact burst ends the effect.
void BattleEffect_RunThor(struct BattleEffectArgument *effect) {
  u8 ramp[32];
  s32 point[3];
  struct EffectPosition screen[11];
  s32 position[3];
  s32 scale[3];
  struct BattleBackgroundView *view;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  s32 frame;
  DrawRectangle blit;
  s32 size;
  s32 count;
  s32 i;
  s32 j;
  s32 dotCount;
  struct EffectStep *spark;
  s32 rings = STORM_DIMMED_DOTS;

  view = (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesC,
                             work->sheet + 0x1800, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);

  // Eight copies of the dot pictures, each with its colour levels capped 7
  // lower than the one before.
  {
    s32 offset;
    for (i = 0, offset = 0; i != 8; i++) {
      u8 *src;
      for (j = 0, src = sprites; j != STORM_DIMMED_DOT_BYTES; j++) {
        s32 v = *src++;

        if (v > 64 - i * 7) {
          v = 64 - i * 7;
        }
        if (v < 0) {
          v = 0;
        }
        (work->sheet + STORM_DIMMED_DOTS)[offset + j] = v;
      }
      offset += j;
    }
  }

  Func_0813ba50();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  REG_BG2CNT = 0x2784;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = 1;
  work->scrollStepY = 0;
  view->busy = 1;
  Task_Add(Func_0813baec, 0xc80);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  work->transferMode = 0;
  Func_08163c2c(0, 0);
  BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
  gCameraSceneParameters[4] = 240;
  Func_08163c2c(0, 1);
  REG_BG2PA = 0x80;
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_ADDITIVE, BLIT_CLIP));
  blit = BLITTER;
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x100f;
  REG_BLDCNT = 0x3f44;
  work->transferMode = 2;
  work->transferValue = 75;

  // One row of the beam: brightest in the middle, darker to each side.
  for (i = 0; i != 16; i++) {
    ramp[i] = i * 8 + 3;
    ramp[31 - i] = i * 8 + 3;
    if (ramp[i] > 63) {
      ramp[i] = 63;
    }
    if (ramp[31 - i] > 63) {
      ramp[31 - i] = 63;
    }
  }
  REG_BG2CNT = 0x784;
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlastSheet), 128);

  for (frame = 0; frame != STORM_DUSK_FRAMES; frame++) {
    size = 2;
    count = frame * 2;
    if (frame == 16) {
      Sound_PlayFar(140);
    }
    if (frame == 132) {
      Sound_PlayFar(131);
    }
    if (frame == 151) {
      Sound_PlayFar(145);
    }
    if (gInput.repeat & 3) {
      break;
    }
    for (j = 0; j != 2; j++) {
      if ((frame & 63) <= 15) {
        Func_0815f0a0((s32)&ResourceId_BlastSheet);
      } else if ((frame & 63) <= 31) {
        Func_0815f0a0((s32)&ResourceId_MercuryDjinnSheet);
      } else if ((frame & 63) <= 47) {
        Func_0815f0a0((s32)&ResourceId_MarsDjinnSheet);
      } else {
        Func_0815f0a0((s32)&ResourceId_JupiterDjinnSheet);
      }
    }
    if (frame == 150) {
      work->transferMode = 1;
      work->transferValue = 0x1a1a1a1a;
    } else {
      work->transferMode = 2;
      work->transferValue = 75;
    }
    Projection_Set(0x1fe0000, Iwram_RatioMulQ14(0x1fe0000, 0xc000), 0x7fff0000);
    Render_ResetTransformState();
    if (frame > 128) {
      s32 sinkFrame = frame - 128;
      if (sinkFrame > 22) {
        sinkFrame = 20;
      }
      SET_VECTOR(position, 0, -sinkFrame << 17, 0x2000000);
      size = (sinkFrame >> 2) + 2;
      if (size > 8) {
        size = 8;
      }
      SceneTransform_ApplyPosition(position);
      SceneTransform_ApplyPitch(-0x1000);
      Func_080150e4(0x8000);
      SceneTransform_ApplyPitch(0x8000);
      SceneTransform_ApplyYaw(sinkFrame << 12);
      if (frame > 150) {
        SET_VECTOR(scale, frame * 0x1800 - 0xd9400, scale[0], scale[0]);
        size = 5;
      } else {
        SET_VECTOR(scale, 0x10000 - sinkFrame * 3 * 0x400, scale[0], scale[0]);
      }
      Func_080151ac(scale);
    } else {
      SET_VECTOR(position, 0, 0, 0x1000000);
      SceneTransform_ApplyPosition(position);
      SceneTransform_ApplyPitch(-0x1000);
      Func_080150e4(frame << 8);
      SceneTransform_ApplyPitch(frame << 8);
    }

    // The three lines, drawn dot by dot between pairs of points.
    if (frame <= 149) {
      for (i = 0; i != 6; i++) {
        SET_VECTOR(point, (Data_08196e2c[i][0] - 96) << 16, 0,
                   (Data_08196e2c[i][1] - 96) << 16);
        EffectPosition_ProjectPoint(point, &screen[i]);
        screen[i].x = (screen[i].x >> 17) + 64;
        screen[i].y = (screen[i].y >> 16) + 60;
      }
      for (i = 0; i != 3; i++) {
        dotCount = count * 2 - i * 48 - 256;
        if (dotCount > 48) {
          dotCount = 48;
        }
        if (dotCount >= 0) {
          for (j = 0; j != dotCount; j++) {
            s32 x = screen[i * 2].x +
                    Iwram_MulQ16(j * (screen[i * 2 + 1].x - screen[i * 2].x),
                                 0x555);
            s32 y = screen[i * 2].y +
                    Iwram_MulQ16(j * (screen[i * 2 + 1].y - screen[i * 2].y),
                                 0x555);
            blit(canvas, sprites + gData_08197410[size - 1], x - size / 2,
                 y - size, size, size * 2);
          }
        }
      }
    }

    // The circle of 128 dots, darkening once the beam has fallen.
    if (frame <= 179) {
      DrawRectangle drawAlt;
      s32 level = 0;

      if (frame > 155) {
        level = frame - 156;
      }
      if (level > 7) {
        level = 7;
      }
      if (frame <= 139) {
        BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_ADDITIVE, BLIT_CLIP));
        drawAlt = BLITTER_ALTERNATE;
      } else {
        BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, BLIT_CLIP));
        drawAlt = BLITTER_ALTERNATE;
      }
      dotCount = count;
      if (dotCount > 128) {
        dotCount = 128;
      }
      for (i = 0; i != dotCount; i++) {
        s32 dotSize = size + 1;
        point[0] = Trig_Sin(i << 9) * 96;
        point[2] = -(Trig_Cos(i << 9) * 96);
        EffectPosition_ProjectPoint(point, &screen[0]);
        screen[0].x = (screen[0].x >> 17) + 64;
        screen[0].y = (screen[0].y >> 16) + 60;
        drawAlt(
            canvas,
            work->sheet +
                (level * STORM_DIMMED_DOT_BYTES + gData_08197410[dotSize - 1]) +
                rings,
            screen[0].x - dotSize / 2, screen[0].y - dotSize, dotSize,
            dotSize * 2);
      }
      HeapSlot_Release(188);
    }

    // The beam: rows of the ramp narrowing at its top, and the light fan
    // falling down it.
    if ((u32)(frame - 151) <= 16) {
      s32 inset = 0;
      s32 beamHeight = (frame - 151) * 20 + 44;
      if (frame > 151) {
        inset = frame - 152;
      }
      if (inset > 15) {
        inset = 15;
      }
      for (i = 1; i != 10; i++) {
        if (inset + i <= 15) {
          blit(canvas, &ramp[inset + i], inset + 48 + i, 16 - i,
               32 - inset * 2 - i * 2, 1);
        }
      }
      for (i = 0; i != beamHeight; i++) {
        s32 fullWidth = 32;
        /* FAKEMATCH(barrier): fullWidth's 32 kept in this loop, as in The
         * Broken Seal's twin; as plain C, 1431 of 1676 instructions differ
         * in tla-en (here 1410). */
        asm("" : "+r"(fullWidth));
        blit(canvas, ramp + inset, inset + 48, i + 16, fullWidth - inset * 2, 1);
      }
      BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, BLIT_CLIP));
      BLITTER_ALTERNATE(canvas, work->sheet, 32, (frame - 151) * 20 - 12, 32,
                        96);
      HeapSlot_Release(188);
      BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, BLIT_CLIP | COMPOSITE_FLIP_X));
      BLITTER_ALTERNATE(canvas, work->sheet, 64, (frame - 151) * 20 - 12, 32,
                        96);
      HeapSlot_Release(188);
    }
    work->transferPending = 1;
    WaitFrames(1);
  }

  // The lightning part.
  view->busy = 0;
  Task_Remove(Func_0813baec);
  Func_0814cca8();
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_SpawnObjects(9, 0x282, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, Ram_MapCellBuffer,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_VioletMoteSheet,
                             work->sheet + STORM_MOTE_CELLS, 1, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data135, sprites, 0, 0);
  REG_BLDALPHA = 0x1010;
  {
    s32 formationX;
    s32 formationY;
    s32 formationSpeedX;
    s32 formationSpeedY;
    s32 pillarX;
    s32 pillarY;

    formationX = 0x500000;
    formationY = 0x400000;
    formationSpeedX = 0;
    formationSpeedY = 0;
    for (i = 0; i != STORM_PARTICLE_COUNT; i++) {
      STORM_PARTICLES[i].variant = 0;
    }
    for (i = 0; i != 16; i++) {
      work->particles[i].x = (Random16() & 31) - 16;
      work->particles[i].y = Random16() & 63;
      work->particles[i].variant = -(Random16() & 15);
    }
    work->transferMode = 2;
    work->transferValue = 50;
    Sound_PlayFar(145);
    pillarX = 60;
    pillarY = 112;

    for (frame = 0; frame != STORM_LIGHTNING_FRAMES; frame++) {
      s32 stormFrame;
      if (frame == 20) {
        Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetB,
                                   work->sheet, 1, 0);
      }
      stormFrame = frame - 80;
      if ((u32)stormFrame <= 59 && !(frame & 7)) {
        Sound_PlayFar(134);
      }
      if (frame == 120) {
        Func_081180e8(134);
      }
      for (j = 0; j != 1; j++) {
        s32 start = j * 128 + 8;
        if (frame >= start && frame < j * 128 + 17) {
          if (frame >= j * 128 + 9 && frame < j * 128 + 12) {
            blit(canvas, work->sheet, 36, 0, 48, pillarY);
          }
          if (frame >= j * 128 + 12 && frame < start + 8) {
            blit(canvas, work->sheet + 0x1500, 36, 0, 48, 112);
          }
          if (frame == start + 2) {
            s32 spawned = 0;
            for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
                 i++, spark++) {
              if (spark->variant == 0) {
                s32 speed = 0x3ff;
                s32 angle;
                speed &= Random16();
                angle = (Random16() & 0x7fff) - 0x4000;
                spark->x = pillarX << 16;
                spark->y = pillarY << 16;
                speed += 32;
                spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
                spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
                spark->variant = (Random16() & 7) + 32;
                if (++spawned == 512) {
                  break;
                }
              }
            }
            work->shakeFrames = 8;
          }
        }
      }
      if (frame == 10) {
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 7, 5, i, 8);
          Func_08118088(effect->actors[i], 4);
        }
      }
      // Frames 80 to 143: the violet bolts, 2 frames apart, and the violet
      // mote swaying in four mirrored copies.
      if ((u32)stormFrame <= 63) {
        s32 moteY;
        s32 mirrorX;
        for (i = 0; i != 6; i++) {
          if (frame == i * 8 + 80) {
            Iwram_FillWords(canvas, 0x4000, 0x10101010);
          }
        }
        for (i = 0; i != STORM_BOLTS; i++) {
          if (frame >= i * 2 + 80) {
            if (frame < i * 2 + 82) {
              s32 boltX = (Random16() & 3) - 3;
              s32 boltY = (Random16() & 3) + 32;
              BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, BLIT_CLIP));
              BLITTER_ALTERNATE(canvas, work->sheet + i % 3 * 0x1440, boltX,
                                boltY, 72, 72);
              HeapSlot_Release(188);
            }
            if (frame == i * 2 + 80) {
              s32 spawned = 0;
              for (j = 0, spark = STORM_PARTICLES; j != STORM_PARTICLE_COUNT;
                   j++, spark++) {
                if (spark->variant == 0) {
                  s32 speed = 0x3ff;
                  s32 angle;
                  speed &= Random16();
                  angle = Random16() & 0xffff;
                  spark->x = 0x100000;
                  spark->y = 0x500000;
                  speed += 32;
                  spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
                  spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
                  spark->variant = (Random16() & 7) + 32;
                  if (++spawned == 16) {
                    break;
                  }
                }
              }
              work->shakeFrames = 8;
              for (j = 0; j != effect->count; j++) {
                Func_0814cd48(effect->actors[j], Data_0819855e[Random16() & 3],
                              5, j, 4);
                Func_08118088(effect->actors[j], 4);
              }
            }
          }
        }
        {
          s32 radius = 7;
          s32 angle;
          radius &= Random16();
          angle = Random16() & 0xffff;
          mirrorX = ((Trig_Sin(angle) * (radius + 8)) >> 16) + 72;
          moteY = 32 - ((Trig_Cos(angle) * (radius + 8)) >> 16);
        }
        Func_0818caa8(work->sheet + STORM_MOTE_CELLS, mirrorX, moteY, 24);
        j = 0;
        for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
             i++, spark++) {
          if (spark->variant == 0) {
            s32 speed = 63;
            s32 angle;
            speed &= Random16();
            angle = Random16() & 0xffff;
            spark->x = mirrorX << 16;
            spark->y = moteY << 16;
            speed += 64;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
            spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
            spark->variant = (Random16() & 7) + 16;
            if (++j == 4) {
              break;
            }
          }
        }
      }

      BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, BLIT_CLIP | COMPOSITE_FLIP));
      {
        DrawRectangle draw = BLITTER_ALTERNATE;
        struct EffectStep *spark;
        for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
             i++, spark++) {
          if (spark->variant > 0) {
            spark->variant--;
            BattleEffectKernels_IntegrateVector2(spark, 60, 0);
            if (spark->y > 0x780000) {
              spark->velocityY = -spark->velocityY / 2;
            } else if ((u32)spark->x <= 0x7effff && spark->y >= 0) {
              s32 x = spark->x >> 16;
              s32 y = spark->y >> 16;
              s32 dotSize = spark->variant / 16 + 1;
              draw(canvas, sprites + gData_08197410[dotSize - 1],
                   x - dotSize / 2, y - dotSize, dotSize, dotSize * 2);
            }
          }
        }
      }
      HeapSlot_Release(188);

      // The formation is pushed back from frame 32.
      if (frame > 15) {
        if (frame == 32) {
          formationSpeedX = 0x40000;
          formationSpeedY = -0x4000;
        }
        if (frame > 31) {
          formationX += formationSpeedX;
          formationY += formationSpeedY;
          formationSpeedX = formationSpeedX * 60 / 64;
          formationSpeedY = formationSpeedY * 60 / 64;
        }
        Func_0816442c(0, formationX, formationY);
      }
      // Smoke around the formation.
      if ((u32)(frame - 16) <= 127) {
        s32 formationScreenX;
        for (i = 0, formationScreenX = formationX >> 17; i != 3; i++) {
          s32 cell = i & 3;
          s32 angle = Random16() & 0xffff;
          s32 x = ((Trig_Sin(angle) * 8) >> 16) + formationScreenX -
                  (gData_08197492[cell] >> 1);
          s32 y = ((Trig_Cos(angle) * 40) >> 16) - (gData_08197498[cell] >> 1);
          Func_08196404(188, 7, 7, 3 | Data_08198562[Random16() & 3], 1);
          BLITTER_ALTERNATE(canvas, Ram_MapCellBuffer + gData_08197486[cell], x,
                            y + 56, gData_08197492[cell], gData_08197498[cell]);
          HeapSlot_Release(188);
        }
      }
      if (frame <= 31) {
        Func_08158ce0(8, 16);
      } else {
        Func_08158ce0(4, 4);
      }
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      WaitFrames(1);
    }

    HeapSlot_Release(104);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    Func_0816467c(0, formationX, formationY);
  }
  for (i = 0; i != 9; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}
