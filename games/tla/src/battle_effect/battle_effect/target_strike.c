/// @file
/// Monster bites and slashes on one target, and Mad Zephyr.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "module/battle_effect/battle_effect.h"
#include "module/system/system.h"
#include "resource_ids.h"

/// Abilities: Numbing Sting, Paralytail (kind 313).
void BattleEffect_RunNumbingSting(void *object) {
  BattleEffect_RunTargetStrike(object, 6);
}

/// Abilities: Cannibal Fang (kind 316).
void BattleEffect_RunCannibalFang(void *object) {
  BattleEffect_RunTargetStrike(object, 3);
}

/// Abilities: Poisonous Bite (kind 318); Poison Fang (kind 323).
void BattleEffect_RunPoisonousBite(void *object) {
  BattleEffect_RunTargetStrike(object, 1);
}

/// Abilities: Rabid Fang (kind 336).
void BattleEffect_RunRabidFang(void *object) {
  BattleEffect_RunTargetStrike(object, 10);
}

/// Abilities: Electric Bite (kind 324).
void BattleEffect_RunElectricBite(void *object) {
  BattleEffect_RunTargetStrike(object, 5);
}

/// Abilities: Poison Tail (kind 325); Poison Sting (kind 440).
void BattleEffect_RunPoisonTail(void *object) {
  BattleEffect_RunTargetStrike(object, 9);
}

/// Abilities: Vampiric Fang (kind 327).
void BattleEffect_RunVampiricFang(void *object) {
  BattleEffect_RunTargetStrike(object, 4);
}

/// Abilities: Double Fang (kind 332).
void BattleEffect_RunDoubleFang(void *object) {
  BattleEffect_RunTargetStrike(object, 0);
}

/// Abilities: Mortal Blow (kind 333).
void BattleEffect_RunMortalBlow(void *object) {
  BattleEffect_RunTargetStrike(object, 8);
}

/// Abilities: Twin Beaks, Gryphon (kind 335).
void BattleEffect_RunTwinBeaks(void *object) {
  BattleEffect_RunTargetStrike(object, 7);
}

/// Abilities: Acid Bite (kind 337).
void BattleEffect_RunAcidBite(void *object) {
  BattleEffect_RunTargetStrike(object, 2);
}

/// Abilities: Drain Fang (kind 386).
void BattleEffect_RunDrainFang(void *object) {
  BattleEffect_RunTargetStrike(object, 11);
}

/// Abilities: Angle Spike (kind 399).
void BattleEffect_RunAngleSpike(void *object) {
  BattleEffect_RunTargetStrike(object, 16);
}

/// Abilities: Heat Stun (kind 458).
void BattleEffect_RunHeatStun(void *object) {
  BattleEffect_RunTargetStrike(object, 14);
}

/// Abilities: Maneater (kind 421).
void BattleEffect_RunManeater(void *object) {
  BattleEffect_RunTargetStrike(object, 15);
}

/// Effect kind 438: the target strike in style 17.
void BattleEffect_RunTargetStrikeMode17(void *object) {
  BattleEffect_RunTargetStrike(object, 17);
}

/// Effect kind 439: the target strike in style 18.
void BattleEffect_RunTargetStrikeMode18(void *object) {
  BattleEffect_RunTargetStrike(object, 18);
}

/// Abilities: Kill Sting (kind 441).
void BattleEffect_RunKillSting(void *object) {
  BattleEffect_RunTargetStrike(object, 19);
}

/// Effect kind 456: the target strike in style 20.
void BattleEffect_RunTargetStrikeMode20(void *object) {
  BattleEffect_RunTargetStrike(object, 20);
}

/// Abilities: Mad Zephyr (kind 117).
void BattleEffect_RunMadZephyr(u8 *object) {
  BattleEffect_SetApproachMotion(*(s32 *)(object + 8), *(s16 *)(object + 36), 4,
                                 20);
  wait(2);
  if (*(s32 *)(object + 24) == 0)
    BattleEffect_RunTargetStrike(object, 12);
  else
    BattleEffect_RunTargetStrike(object, 13);
}

#undef RECORD

// The body of 21 strike and column effects.

struct Model;

#define gBlitter ((DrawRectangle)Ram_HeapSlots->unk64[1])
#define gBlitterAlternate ((DrawRectangle)Ram_HeapSlots->unkb8[1])

// The pixel half of a 16.16 value.
#define HI(value) (((s16 *)&(value))[1])

#define RECORD(mode, field) Data_0819754f[(mode)][(field)]
#define RECORD_KIND 0    // 0: strikes across the target, 1: columns on it
#define RECORD_COUNT 1   // strikes or columns
#define RECORD_FLAGS 2   // the parts drawn (FLAG_*)
#define RECORD_PALETTE 3 // the colours
#define RECORD_RATE 4    // frames for each picture
#define RECORD_BLEND 5   // the columns' blitter mode
#define RECORD_FRAMES 6  // the effect's length

#define FLAG_WISPS 1  // smoke wisps rise from the target
#define FLAG_MOTES 2  // motes leave the target and fly back to the user
#define FLAG_GLINTS 4 // glints flicker around the target
#define FLAG_SPARKS 8 // the hits throw sparks
#define FLAG_HEAVY 16 // the hits shake harder and knock the target back
#define FLAG_FLASH 32 // as heavy, with flash pictures for wisps
#define FLAG_STAR 64  // a star model swells on the target

// The motes live in the map cell buffer, the sparks 0x3800 bytes on.
#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)
#define gSparks ((struct EffectStep *)(Ram_MapCellBuffer + 0x3800))

void Render_DrawModelList(struct Model *model);

#define BUILD_BLITTER(slot, width, height, flags, mode)                        \
  do {                                                                         \
    BattleEffect_BuildBlitter(slot, width, height, flags, mode);               \
  } while (0)

/// Strikes (kind 0) are six-picture slashes across the first target, one
/// every six pictures' time; columns (kind 1) are a pair of mirrored 18x48
/// pillars that rise and fall on it. Each hit flashes and shakes the target,
/// starts eight wisps of smoke and a burst of sparks, and the battle moves on
/// at the first. Around them the mode's flags add rising wisps, motes that
/// stream back to the user, glints, sparks, a flash or a star.
void BattleEffect_RunTargetStrike(struct BattleEffectArgument *effect,
                                  s32 mode) {
  struct EffectPosition ground;
  struct EffectPosition targetPosition;
  struct EffectPosition alternate;
  struct EffectPosition center;
  struct EffectPosition position;
  BlitterPair draw;
  struct StarTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  u8 *sheet;
  struct BattleCamera *camera;
  struct MotionObject *target;
  struct MotionObject *source;
  s32 resource;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  if (mode == 8)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  if (mode == 8) {
    work->fadeFrames = 24;
    work->fadeStep = 0;
  }

  // The sheets: motes, the tornado, the strikes or columns, the mode's
  // colours and the wisps (smoke, flash or star).
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (RECORD(mode, RECORD_KIND) == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_BeamImage, work->sheet + 0xc80,
                               0, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage,
                               work->sheet + 0xc80, 0, 0);
  switch (RECORD(mode, RECORD_PALETTE)) {
  case 0:
    resource = (s32)&ResourceId_BlueBeamSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    resource = (s32)&ResourceId_PinkPalette;
    break;
  case 3:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  case 4:
    resource = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case 5:
  default:
    resource = (s32)&ResourceId_LightningBoltSheet;
    break;
  case 6:
    resource = (s32)&ResourceId_OrangePaletteB;
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  if (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH)
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->sheet + 0x3200, 1, 0);
  else if (RECORD(mode, RECORD_FLAGS) & FLAG_STAR)
    Resource_LoadAndDecompress((s32)&ResourceId_StarCrossSheet,
                               work->sheet + 0x3200, 1, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                               work->sheet + 0x3200, 1, 0);

  for (k = 0; k != 1; k++) {
    // The wisps start at the target's feet, waiting (-1) until a hit.
    target = BattleObject_GetFar(effect->actors[k])->object;
    EffectPosition_ProjectBattler(effect->actors[k], &ground);
    for (i = 0; i != 64; i++) {
      struct EffectStep *step = &work->particles[i];

      step->x = (ground.x / 2) << 16;
      step->y = 0x500000;
      step->z = 0;
      step->velocityX = ((Random16() & 255) - 128) << 9;
      step->velocityY = ((Random16() & 255) - 128) << 9;
      step->velocityZ = 0;
      step->variant = -1;
    }

    // The motes burst from 20 pixels above the target.
    for (i = 0; i != 128; i++) {
      struct EffectStep *step = &gMotes[k * 128 + i];

      step->x = target->x;
      step->y = 0x140000;
      step->z = target->z;
      step->velocityX = ((Random16() & 255) - 128) << 11;
      step->velocityY = ((Random16() & 255) - 128) << 11;
      step->velocityZ = ((Random16() & 255) - 128) << 11;
      step->variant = -1;
    }

    // The sparks fly away from the hit: heavy hits throw them in a fan
    // away from the user, the others towards the middle of the field.
    for (i = 0; i != 256; i++) {
      gSparks[i].x = target->x;
      gSparks[i].y = 0x140000;
      gSparks[i].z = target->z;
      if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY) {
        s32 angle = Random16();
        s32 speed = Random16() & 255;

        gSparks[i].velocityX = (Random16() & 0x1ff) << 11;
        if (effect->side == 1)
          gSparks[i].velocityX = -gSparks[i].velocityX;
        gSparks[i].velocityY = Trig_Sin(angle) * speed >> 4;
        gSparks[i].velocityZ = Trig_Cos(angle) * speed >> 4;
      } else {
        if (RECORD(mode, RECORD_KIND) == 1) {
          gSparks[i].velocityX = ((Random16() & 255) - 128) << 11;
          gSparks[i].velocityY = ((Random16() & 255) - 128) << 11;
          gSparks[i].velocityZ = ((Random16() & 255) - 128) << 11;
        } else {
          gSparks[i].velocityX = (Random16() & 255) << 11;
          gSparks[i].velocityY = ((Random16() & 255) - 128) << 11;
          gSparks[i].velocityZ = ((Random16() & 255) - 128) << 11;
        }
        if (gSparks[i].x > 0)
          gSparks[i].velocityX = -gSparks[i].velocityX;
      }
      gSparks[i].variant = -1;
    }
  }

  work->transferMode = 2;
  if (RECORD(mode, RECORD_FLAGS) & FLAG_STAR)
    work->transferValue = 50;
  else
    work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Aim at the middle of the first target.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &targetPosition);
  EffectPosition_ProjectBattler(effect->actors[0], &alternate);
  targetPosition.y += (alternate.y - targetPosition.y) / 2;

  for (frame = 0; frame != RECORD(mode, RECORD_FRAMES); frame++) {
    s32 span = RECORD(mode, RECORD_COUNT) * RECORD(mode, RECORD_RATE);
    s32 length = span * 4;
    s32 glint = 0;

    if (RECORD(mode, RECORD_KIND) == 0) {
      // Strikes: six pictures each, crossing the target from the user's
      // side.
      if (frame < span * 6) {
        s32 picture = frame / RECORD(mode, RECORD_RATE) % 6;

        if (frame % (RECORD(mode, RECORD_RATE) * 6) == 0 &&
            (u32)(mode - 12) <= 1) {
          EffectPosition_ProjectScaledBattler(effect->actors[0],
                                              &targetPosition);
          EffectPosition_ProjectBattler(effect->actors[0], &alternate);
          targetPosition.y += (alternate.y - targetPosition.y) / 2;
          targetPosition.y += (Random16() & 15) - 8;
        }
        if (effect->side == 1) {
          BattleEffect_LoadBlitter(SLOT_BLITTER,
                                   BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
          gBlitter(canvas, work->sheet + gTargetStrike_Offsets[picture],
                   targetPosition.x / 2 - (gTargetStrike_Reach[picture] / 2) -
                       gTargetStrike_Widths[picture] + 8,
                   targetPosition.y - (gTargetStrike_Heights[picture] / 2),
                   gTargetStrike_Widths[picture],
                   gTargetStrike_Heights[picture]);
        } else {
          BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP));
          gBlitter(
              canvas, work->sheet + gTargetStrike_Offsets[picture],
              targetPosition.x / 2 + (gTargetStrike_Reach[picture] / 2) - 8,
              targetPosition.y - (gTargetStrike_Heights[picture] / 2),
              gTargetStrike_Widths[picture], gTargetStrike_Heights[picture]);
        }
        HeapSlot_Release(SLOT_BLITTER);

        // A star sounds four frames before its hit.
        if (frame % (RECORD(mode, RECORD_RATE) * 6) ==
                RECORD(mode, RECORD_RATE) * 4 - 4 &&
            (RECORD(mode, RECORD_FLAGS) & FLAG_STAR))
          Sound_PlayFar(212);

        // The hit, on the strike's fifth picture.
        if (frame % (RECORD(mode, RECORD_RATE) * 6) ==
            RECORD(mode, RECORD_RATE) * 4) {
          if (mode == 12) {
            if (frame / RECORD(mode, RECORD_RATE) / 6 <= 2)
              Sound_PlayFar(133);
            else
              BattlePlayback_StartFar(133);
          } else if (mode == 7) {
            if (frame <= 8)
              Sound_PlayFar(133);
            else
              BattlePlayback_StartFar(133);
          } else if (mode == 13) {
            if (frame / RECORD(mode, RECORD_RATE) / 6 > 6)
              BattlePlayback_StartFar(134);
            else
              Sound_PlayFar(134);
          } else if ((RECORD(mode, RECORD_FLAGS) & FLAG_STAR) ||
                     (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH)) {
            BattlePlayback_StartFar(144);
          } else if (mode == 8) {
            BattlePlayback_StartFar(134);
          } else {
            BattlePlayback_StartFar(133);
          }

          if (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) {
            work->shakeFrames = 8;
            BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 12);
            BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
            for (i = 0; i != 32; i++)
              gSparks[i].variant = (Random16() & 15) + 15;
          } else if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY) {
            work->shakeFrames = 8;
            BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 12);
            BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
            for (i = 0; i != 256; i++)
              gSparks[i].variant = (Random16() & 15) + 15;
          } else {
            work->shakeFrames = 4;
            BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
            for (i = 0; i != 32; i++) {
              struct EffectStep *step =
                  &gSparks[frame / (RECORD(mode, RECORD_RATE) * 6) * 32 + i];

              step->variant = (Random16() & 15) + 7;
            }
          }
          if (mode == 16) {
            for (i = 0; i != 4; i++) {
              struct EffectStep *step =
                  &work->particles[frame / (RECORD(mode, RECORD_RATE) * 6) *
                                       16 +
                                   i];

              step->variant = 0;
            }
          } else {
            for (i = 0; i != 8; i++) {
              struct EffectStep *step =
                  &work->particles[frame / (RECORD(mode, RECORD_RATE) * 6) *
                                       16 +
                                   i];

              step->variant = 0;
            }
          }
        }
      }
      if (frame >= 12 && frame <= 31)
        glint = 1;
    } else if (frame < length + 4) {
      // Columns: a mirrored pair rising through its stages.
      s32 stage;

      if (frame < length) {
        s32 picture = frame / RECORD(mode, RECORD_RATE);

        while (picture > 4)
          picture -= 4;
        stage = gTargetStrike_ColumnStages[picture];
      } else {
        stage = 3;
      }
      BUILD_BLITTER(SLOT_BLITTER, 7, 7, CLIP, RECORD(mode, RECORD_BLEND));
      gBlitter(canvas, work->sheet + stage * 864 + 0xc80,
               targetPosition.x / 2 - 18, 56, 18, 48);
      HeapSlot_Release(SLOT_BLITTER);
      BattleEffect_BuildBlitter(SLOT_BLITTER, 7, 7, CLIP | COMPOSITE_FLIP_X,
                                RECORD(mode, RECORD_BLEND));
      do {
        // Empty: its loop notes end CSE's block after the blitter call.
      } while (0);
      gBlitter(canvas, work->sheet + stage * 864 + 0xc80, targetPosition.x / 2,
               56, 18, 48);
      HeapSlot_Release(SLOT_BLITTER);

      // The hit, on each column's fourth picture.
      if (frame % (RECORD(mode, RECORD_RATE) * 4) ==
          RECORD(mode, RECORD_RATE) * 3) {
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
        work->shakeFrames = 4;
        if ((RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) ||
            (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY))
          BattlePlayback_StartFar(144);
        else if (frame > (RECORD(mode, RECORD_COUNT) * 4 - 4) *
                             RECORD(mode, RECORD_RATE))
          BattlePlayback_StartFar(133);
        else
          Sound_PlayFar(133);
        if (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) {
          work->shakeFrames = 8;
          BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 12);
          BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
          for (i = 0; i != 32; i++)
            gSparks[i].variant = (Random16() & 15) + 15;
        } else if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY) {
          work->shakeFrames = 8;
          BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 12);
          BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
          for (i = 0; i != 256; i++)
            gSparks[i].variant = (Random16() & 15) + 15;
        }
        for (i = 0; i != 64; i++) {
          struct EffectStep *step =
              &gSparks[frame / (RECORD(mode, RECORD_RATE) * 6) * 64 + i];

          step->variant = (Random16() & 15) + 7;
        }
        for (i = 0; i != 8; i++) {
          struct EffectStep *step =
              &work->particles[frame / (RECORD(mode, RECORD_RATE) * 6) * 16 +
                               i];

          step->variant = 0;
        }
        for (i = 0; i != 16; i++)
          gMotes[frame / (RECORD(mode, RECORD_RATE) * 6) * 16 + i].variant = 0;
      }
      if (stage == 3)
        glint = 1;
    }

    // Three glints a frame around the target's middle.
    if ((RECORD(mode, RECORD_FLAGS) & FLAG_GLINTS) && glint) {
      EffectPosition_ProjectScaledBattler(effect->actors[0], &center);
      EffectPosition_ProjectBattler(effect->actors[0], &alternate);
      center.y += (alternate.y - center.y) / 2;
      for (i = 0; i != 3; i++) {
        s32 image = frame & 3;
        s32 angle = Random16() & 0xffff;
        s32 range = 31;
        s32 radius = Random16() & range;
        s32 x;
        s32 y;

        x = center.x / 2 + (Trig_Sin(angle) * (radius + 4) >> 17) -
            (gData_08197492[image] / 2);
        y = center.y - (Trig_Cos(angle) * (radius + 4) >> 17) -
            (gData_08197498[image] / 2);
        BattleEffect_BuildBlitter(
            SLOT_BLITTER_ALTERNATE, 7, 7,
            gDeathPlunge_SwordWidths[Random16() & 3] | CLIP, BLIT_ADDITIVE);
        do {
          // Empty: its loop notes end CSE's block after the blitter call.
        } while (0);
        gBlitterAlternate(canvas, work->sheet + gBattleEffect_GlintCells[image],
                          x, y, gData_08197492[image], gData_08197498[image]);
        HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
      }
    }

    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = gBlitter;
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                             BLITTER(BLIT_ADDITIVE, CLIP));
    draw[1] = gBlitterAlternate;
    for (k = 0; k != 1; k++) {
      source = BattleObject_GetFar(effect->actor)->object;

      // Wisps of smoke rise and spread for 24 frames.
      if (RECORD(mode, RECORD_FLAGS) & FLAG_WISPS) {
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &work->particles[i];

          if (step->variant >= 0 && step->variant <= 23) {
            s32 picture = step->variant / 4;

            draw[i & 1](canvas, work->sheet + picture * 1152 + 0x3200,
                        HI(step->x) - 12, HI(step->y) - 24, 24, 48);
            BattleEffectKernels_IntegrateVector2(step, 62, -0x400);
            step->variant++;
          }
        }
      }

      // Flashes rise twice as fast, 32x64.
      if (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) {
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &work->particles[i];

          if (step->variant >= 0 && step->variant <= 23) {
            draw[0](canvas, work->sheet + (step->variant / 4 << 11) + 0x3200,
                    HI(step->x) - 16, HI(step->y) - 32, 32, 64);
            BattleEffectKernels_IntegrateVector2(step, 62, -0x800);
            BattleEffectKernels_IntegrateVector2(step, 62, -0x800);
            step->variant++;
          }
        }
      }

      // Motes burst out, then stream back to the user, who flashes.
      if (RECORD(mode, RECORD_FLAGS) & FLAG_MOTES) {
        s32 size = 3;

        if (mode == 11)
          size = 8;
        if (frame == 55)
          BattleEffect_TintBattler(effect->actor, 7, -1, -1, 0);
        if (frame == 90)
          BattleEffect_TintBattler(effect->actor, 0, -1, -1, 0);
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &gMotes[i];

          if (step->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)step, &position);
            position.x >>= 1;
            draw[1](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                    position.x - ((u32)size >> 1), position.y - size, size,
                    size * 2);
            BattleEffectKernels_IntegrateVector3(step, 60, 0);
            step->variant++;
            if (step->variant > 10) {
              s32 dx = (source->x - step->x) >> 8;
              s32 dy = (source->y - step->y + 0x140000) >> 8;
              s32 dz = (source->z - step->z) >> 8;

              step->velocityX += dx;
              step->velocityY += dy;
              step->velocityZ += dz;
              if ((dx >= -0xfff && dx <= 0xfff) &&
                  (dz >= -0xfff && dz <= 0xfff))
                step->variant = -1;
            }
          }
        }
      }

      // Sparks fall away, shrinking.
      if (RECORD(mode, RECORD_FLAGS) & FLAG_SPARKS) {
        for (i = 0; i != 256; i++) {
          struct EffectStep *step = &gSparks[i];
          s32 size = step->variant;

          if (size >= 0) {
            EffectPosition_ProjectPoint((s32 *)step, &position);
            position.x >>= 1;
            BattleEffectKernels_IntegrateVector3(step, 60, 0);
            size >>= 3;
            size++;
            draw[1](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
            step->variant--;
          }
        }
      }
    }
    HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
    HeapSlot_Release(SLOT_BLITTER);

    // The star: a textured square, swelling on a sine from frame 8 to 17
    // over the target's middle (the canvas is cleared to colour 0x3f first).
    if (RECORD(mode, RECORD_FLAGS) & FLAG_STAR) {
      void *corners = Heap_AllocEwram(32);
      struct ModelDraw *model = Render_AllocateModelList(1);

      model->unk14 = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet + 0x3200;
      model->kind = 6;
      model->model = gData_08199340;
      model->texture = &texture;
      model->corners = corners;
      if (frame == 8)
        Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
      if (frame >= 8 && frame <= 17) {
        s32 scale = Trig_Sin(((frame - 8) << 11) + 0x3000) * 612 >> 8;

        Transform_Reset();
        SceneTransform_ApplyPosition3((targetPosition.x / 2 - 64) << 16,
                                      (targetPosition.y - 64) << 16, 0);
        Transform_ApplyPitch(0x4000);
        Transform_ApplyYaw(0x2000);
        SceneTransform_ApplyScale3(scale, scale / 2, scale / 2);
        Render_TransformVertices(gData_08199210, corners, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(corners);
    }

    if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY)
      BattleCamera_Shake(8, 8);
    else
      BattleCamera_Shake(2, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// A rain of daggers on the first target (unused).

// Sixteen daggers fall 8 pixels a frame, one every 4 frames, from columns
// 16 to 111, drifting towards the middle (x 64) the further out they start,
// and stick in the ground where they stop. The canvas fades in over 16
// frames and out from frame 104; from frame 24 to 84 the target flashes and
// the screen shakes every 4 frames, with a sound every 8.

void BattleEffect_RunBladeRain(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle *blit;
  struct EffectStep *dagger;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1000;

  // [0] as drawn, [1] mirrored left to right; both skip clear pixels.
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                           BLITTER(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  blit = draw;
  Resource_LoadAndDecompress((s32)&ResourceId_CrossedDaggersSheet, work->sheet,
                             1, 1);
  work->transferMode = 1; // each transfer clears the canvas
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Centre BG2 on the first target.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &pos);
  REG_BG2X = (64 - pos.x) << 8;

  // Daggers start in columns 16 to 111; the further from the middle, the
  // faster they drift towards it (in steps of 2 pixels a frame).
  for (i = 0; i != 64; i++) {
    s32 column;

    dagger = &work->particles[i];
    column = Random16() % 96 + 16;
    dagger->x = column;
    dagger->y = (24 - i / 4) << 16;
    if (column <= 43)
      dagger->velocityX = 3;
    else if (column <= 51)
      dagger->velocityX = 2;
    else if (column <= 59)
      dagger->velocityX = 1;
    else if (column <= 67)
      dagger->velocityX = 0;
    else if (column <= 75)
      dagger->velocityX = -1;
    else if (column <= 83)
      dagger->velocityX = -2;
    else
      dagger->velocityX = -3;
    dagger->velocityX <<= 17;
    dagger->velocityY = 8 << 16;
    dagger->x <<= 16;
  }
  Sound_PlayFar(212);

  for (frame = 0; frame != 120; frame++) {
    if (frame <= 16) {
      REG_BLDALPHA = frame | 0x1000;
      if (frame == 16)
        REG_BLDCNT = 0;
    }
    if (frame > 103) {
      REG_BLDALPHA = (120 - frame) | 0x1000;
      if (frame == 104)
        REG_BLDCNT = 0x3f44;
    }

    // The drift picks the picture; a dagger going left is drawn mirrored.
    // Each one falls from 16 frames after its turn until it lands at 25.
    for (i = 15; i != -1; i--) {
      s32 drift;
      s32 speed;
      s32 cell;

      dagger = &work->particles[i];
      drift = dagger->velocityX;
      speed = drift;
      if (drift < 0)
        speed = -drift;
      cell = speed >> 17;
      if (frame < i * 4 + 25) {
        blit[(u32)drift >> 31](canvas, work->sheet + (cell << 10),
                               (dagger->x >> 16) - 16, (dagger->y >> 16) - 16,
                               32, 32);
        if (frame >= i * 4 + 16) {
          dagger->x += dagger->velocityX;
          dagger->y += dagger->velocityY;
        }
      } else {
        blit[(u32)drift >> 31](canvas, work->sheet + (cell << 10),
                               (dagger->x >> 16) - 16, (dagger->y >> 16) - 16,
                               32, 32);
      }
    }

    // From frame 24 to 84, every 4 frames: flash the target for 2 frames
    // and shake the screen; every 8, a sound.
    if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 2);
      work->shakeFrames = 1;
      if ((frame & 7) == 0)
        Sound_PlayFar(133);
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BATTLE_EFFECT 92 // battleFxWork[0]
#define SLOT_BATTLE_CANVAS 96 // battleFxWork[1]
#define SLOT_BATTLE_SHEET 100 // unk64[0]

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

// The flecks live in the map cell buffer.
#define gFlecks ((struct EffectStep *)Ram_MapCellBuffer)

// A pillar of ice rising at the user's feet with flecks spiralling up
// around it, for the scenes.

// The pillar is two mirrored halves of a 17x64 picture with a 24x43 glow,
// drawn at their maximum: four pillar pictures two frames apart and three
// glow pictures four frames apart. 32 flecks spiral up around it, one more
// each frame, turning 0x400 and rising 5 pixels a frame while the spiral
// widens. effect->kind picks the colours (yellow, ice, red or violet) and
// effect->variant + 1, from 1 to 4, the flecks' size. At frame 16 the first
// target flashes; the last 6 frames fade out.

void BattleEffect_RunElementPillar(struct BattleEffectArgument *effect) {
  struct EffectPosition ground;
  struct EffectPosition pos;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 frame;
  void *palette;
  s32 i;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  sheet = HeapSlot_Alloc(SLOT_BATTLE_SHEET, 0x60e);
  camera = Ram_HeapSlots->cameraWork;
  work->vblanksSinceTransfer = 1;
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = 0x100c;
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  switch (effect->kind) {
  case 0:
    palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
    break;
  case 1:
    palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
    break;
  case 2:
    palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
    break;
  default:
    palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, 128);

  // A fleck's x is its angle and z its radius, in 1/16 pixels; fleck i
  // waits i frames before it rises.
  for (i = 0; i != 128; i++) {
    gFlecks[i].y = 0;
    gFlecks[i].x = Random16() & 0xffff;
    gFlecks[i].z = (Random16() & 0x1ff) + i * 2;
    gFlecks[i].variant = -i;
  }
  work->transferMode = 2;   // each transfer leaves 3/4 of the old picture,
  work->transferValue = 75; // a trail
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_ADDITIVE, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  effect->variant++;
  if (effect->variant <= 0)
    effect->variant = 1;
  if (effect->variant > 4)
    effect->variant = 4;
  Sound_PlayFar(212);

  for (frame = 0; frame != 56; frame++) {
    EffectPosition_ProjectBattler(effect->actor, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = (112 - frame * 2) | 0x1000;
    if (frame == 16)
      BattleEffect_TintBattler(effect->actors[0], 7, -1, 0, 20);

    // The pillar: the left half at x 47, then the right half mirrored at
    // x 64, each with its glow.
    if (frame <= 55) {
      s32 n;

      n = frame / 2 % 4; // a pillar picture every 2 frames
      BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));
      // The left half's pillar draw and glow index in a once-only block,
      // which ends CSE's block so the mirrored half works out both pictures
      // again; as plain statements, 235 of 456 instructions differ in tla-en.
      do {
        draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
        draw[1](canvas, work->sheet + n * 1088, 47, ground.y - 64, 17, 64);
        n = frame / 4 % 3; // a glow picture every 4 frames
      } while (0);
      draw[1](canvas, work->sheet + n * 1032 + 0x1100, 40, ground.y - 36, 24,
              43);
      HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
      BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      n = frame / 2 % 4;
      draw[1](canvas, work->sheet + n * 1088, 64, ground.y - 64, 17, 64);
      n = frame / 4 % 3;
      draw[1](canvas, work->sheet + n * 1032 + 0x1100, 64, ground.y - 36, 24,
              43);
      HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
    }
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);

    // The first 32 flecks turn 0x400, rise 5 pixels and widen 4 pixels a
    // frame.
    for (i = 0; i != 32; i++) {
      struct EffectStep *fleck = &gFlecks[i];

      if (fleck->variant >= 0) {
        s32 size;

        point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
        point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
        point[1] = fleck->y;
        fleck->x += 0x400;
        fleck->y += 5 << 16;
        fleck->z += 64;
        EffectPosition_ProjectPoint(point, &pos);
        pos.x /= 2;
        size = (i & 1) + effect->variant;
        draw[0](canvas, sheet + gBattleEffect_FlarePictures[size - 1],
                pos.x - size, pos.y - size, size * 2, size * 2);
      }
      fleck->variant++;
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
  HeapSlot_Release(SLOT_BATTLE_SHEET);
  HeapSlot_Release(SLOT_BATTLE_CANVAS);
  HeapSlot_Release(SLOT_BATTLE_EFFECT);
}

#define BACKGROUND(battle) (((union BattleBackgroundId *)(battle)->unk670)->id)

// Shows the battle background fading in, until A (unused).

void Func_0813e7e0(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  battle = Ram_HeapSlots->battleWork;
  view = (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  work->effect = effect;
  work->vblanksSinceTransfer = 1;
  Func_08191530(effect);

  // Make BG2 a 128x128 canvas, as BattleEffect_StartCanvasLayer does.
  BattleEffect_ClearActorTimers();
  view->secondMode = 1;
  gBgScroll[1].y = 32;
  BattleBackground_SetModeFar(1, BACKGROUND(battle), 0);
  REG_BG2CNT = 0x784;
  BattleCamera_SetViewFar(0, 0, 0, 100);
  view->secondMode = 0;
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BG2PA = 0x80;
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = 0x100;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x1088;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x1088;
  REG_WININ = 0x3537;
  REG_WINOUT = 0x3f21;

  i = 0;
  for (k = 0; k != 16; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s32 entry;

      entry = (s16)(((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2));
      // The map written through a volatile pointer, which keeps the ROM's
      // order of stores; as a plain pointer, 185 of 376 instructions differ in
      // tla-en.
      *(volatile u16 *)(0x06003800 + i) = *(u16 *)&entry;
    }
  }

  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));

  // Keep the picture at 0x06008000 aside, show the battle background with
  // its colours black, and flash the first target.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
  Iwram_FillWords(Ram_MapCellBuffer, 0x7800, 0x01010101);
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  BattleBackground_LoadFar(1, BACKGROUND(battle), -1);
  Palette_BrightenSavedBattleBg(-31, -31, -31);
  BattleEffect_TintBattler(effect->actors[0], 7, 3, 0, 30);

  // Fade the colours up a shade every two frames; B starts again, A ends.
  for (frame = 0; frame != 32096; frame++) {
    // The A test reads the held keys through a volatile pointer, a load apart
    // from the B test's; as gInput.held, 10 of 376 instructions differ in
    // tla-en.
    if (gInput.held & 2)
      frame = 0;
    if (*(volatile u32 *)&gInput.held & 1)
      break;
    if (frame <= 63)
      Palette_BrightenSavedBattleBg(frame / 2 - 31, frame / 2 - 31,
                                    frame / 2 - 31);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Put the background and the display back.
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_TintBattler(effect->actors[0], -1, 1, -1, 0);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = 32;
  BattleBackground_SetModeFar(2, BACKGROUND(battle), 0);
  wait(1);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  HeapSlot_Release(SLOT_BATTLE_SHEET);
  HeapSlot_Release(SLOT_BATTLE_CANVAS);
  HeapSlot_Release(SLOT_BATTLE_EFFECT);
}
#undef SLOT_BLITTER
#undef SLOT_BLITTER_ALTERNATE
#undef BLITTER
#undef CLIP
#undef RECORD
#undef gBlitter
#undef gBlitterAlternate
#undef HI
#undef RECORD_KIND
#undef RECORD_COUNT
#undef RECORD_FLAGS
#undef RECORD_PALETTE
#undef RECORD_RATE
#undef RECORD_BLEND
#undef RECORD_FRAMES
#undef FLAG_WISPS
#undef FLAG_MOTES
#undef FLAG_GLINTS
#undef FLAG_SPARKS
#undef FLAG_HEAVY
#undef FLAG_FLASH
#undef FLAG_STAR
#undef gMotes
#undef gSparks
#undef BUILD_BLITTER
#undef SLOT_BATTLE_EFFECT
#undef SLOT_BATTLE_CANVAS
#undef SLOT_BATTLE_SHEET
#undef BATTLE_EFFECT_WORK_SIZE
#undef gFlecks
#undef BACKGROUND
