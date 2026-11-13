/// @file
/// Ransack, the rush bursts and Flying Attack, and the burst scene they play.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

void BattleEffect_RunRansack(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 0);
}

void BattleEffect_RunBacteriaRush(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 1);
}

void BattleEffect_RunFreebiteRush(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 2);
}

void BattleEffect_RunFlyingAttack(struct EffectStep *step) {
  if (step->variant == 0) {
    BattleEffect_RunBurstScene(step, 3);
    return;
  }
  if (step->variant == 1) {
    BattleEffect_RunBurstScene(step, 4);
    return;
  }
  BattleEffect_RunBurstScene(step, 5);
}

void EffectStep_RunAnimationMode9(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 9);
}

void EffectStep_RunAnimationMode12(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 12);
}

void EffectStep_RunAnimationMode10(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 10);
}

void EffectStep_RunAnimationMode11(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 11);
}

void EffectStep_RunAnimationMode6Or7Or8B(struct EffectStep *step) {
  if (step->variant == 0) {
    BattleEffect_RunBurstScene(step, 6);
    return;
  }
  if (step->variant == 1) {
    BattleEffect_RunBurstScene(step, 7);
    return;
  }
  BattleEffect_RunBurstScene(step, 8);
}

// The motes live in the map cell buffer, then the wave's, the smoke's and
// the packed orb's cels.
#define MOTES ((struct EffectStep *)Ram_MapCellBuffer)
#define WAVE_CELS (Ram_MapCellBuffer + 0x2400)
#define SMOKE_CELS (Ram_MapCellBuffer + 0x3788)
#define ORB_CELS (Ram_MapCellBuffer + 0x5288)
#define MOTE_COUNT 256

// A variant's record.
#define RECORD(field) Data_081985b3[variant * 8 + (field)]
enum {
  RECORD_ARC_SHEET,
  RECORD_PALETTE,
  RECORD_BLOWS,
  RECORD_MOTES,
  RECORD_INTERVAL,
  RECORD_FRAMES,
  RECORD_FILL,
  RECORD_EXTRA
};
enum { EXTRA_NONE, EXTRA_GLINTS, EXTRA_RIPPLE };

extern char ResourceId_YellowOrbSheet;

extern char ResourceId_BlueArcSheetA;
extern char ResourceId_BlueArcSheetB;
extern char ResourceId_WaveSheet;

// The variants' records, eight bytes each.
extern u8 Data_081985b3[];

// A glint's four flips.
extern const u8 Data_0819861b[];

void BattleEffect_RunBurstScene(struct BattleEffectArgument *effect,
                                s32 variant) {
  struct EffectPosition actorPos;
  struct EffectPosition targetPos;
  s32 screen[3];
  s32 otherPoint[3];
  struct EffectPosition pos;
  struct EffectPosition spot;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 *camera;
  struct MotionObject *target;
  s32 resource;
  u8 *cels;
  s32 row;
  s32 col;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  if (effect->prelude == 1)
    Func_0815585c(effect, PREPARE(7, effect->side, 2), screen, otherPoint);

  // The pictures and the palette.
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  // The orb (40x288) packed to half width in the map cell buffer: the
  // canvas is shown twice as wide.
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1,
                             0);
  cels = WAVE_CELS;
  for (row = 0; row != 288; row++) {
    for (col = 0; col != 40; col++) {
      s32 dst = row * 20 + col / 2;
      s32 src = row * 40 + col;

      (cels + dst)[0x2e88] = work->sheet[src];
    }
  }
  // The glints' sheet takes the orb's place.
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (RECORD(RECORD_ARC_SHEET) == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA,
                               work->sheet + 0xc80, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB,
                               work->sheet + 0xc80, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, WAVE_CELS, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, SMOKE_CELS, 1, 0);
  switch (RECORD(RECORD_PALETTE)) {
  case 0:
    resource = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    resource = (s32)&ResourceId_BlueArcSheetB;
    break;
  default:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);

  // The caster closes in; variant 12 lunges further and waits.
  if (variant == 12) {
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 8,
                                   0x60000);
    wait(7);
  } else {
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 4, 0);
  }
  wait(1);

  EffectPosition_ProjectScaledBattler(effect->actors[0], &targetPos);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  if (RECORD(RECORD_EXTRA) == EXTRA_RIPPLE)
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);

  // The motes wait 25 pixels above the target, aimed away from the
  // caster; none is out yet.
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0; i != MOTE_COUNT; i++) {
    struct EffectStep *mote = &MOTES[i];

    mote->x = target->x;
    mote->y = target->y + 0x190000;
    mote->z = target->z;
    mote->velocityX = (Random16() & 255) << 12;
    mote->velocityY = ((Random16() & 255) - 127) << 12;
    mote->velocityZ = ((Random16() & 255) - 127) << 12;
    if (mote->x > 0)
      mote->velocityX = -mote->velocityX;
    mote->variant = -1;
  }
  // The smoke puffs start at the target's feet, one a frame.
  for (i = 0; i != 64; i++) {
    struct EffectStep *puff = &work->particles[i];

    puff->x = (targetPos.x / 2) << 16;
    puff->y = 0x580000;
    puff->z = 0;
    puff->velocityX = ((Random16() & 255) - 128) << 9;
    puff->velocityY = ((Random16() & 255) - 128) << 9;
    puff->variant = -i;
    puff->velocityZ = 0;
  }

  for (frame = 0; frame != RECORD(RECORD_FRAMES); frame++) {
    s32 *lines;
    s32 blows;

    // The ripple's line table: BG2 swings six pixels on a sine wave.
    lines = work->bg2xLines;
    for (i = 0; i != 160; i++)
      *lines++ = (0x60000 - Trig_Sin((frame + i) * 0x800) * 6) >> 10;
    if (variant == 10 && frame == 8)
      Sound_PlayFar(221);
    blows = RECORD(RECORD_BLOWS);
    EffectPosition_ProjectScaledBattler(effect->actor, &actorPos);
    actorPos.x /= 2;

    // The arcs and waves, mirrored on odd blows.
    if (effect->side == 0) {
      BattleEffect_LoadBlitter(
          HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                     COMPOSITE_CLIP_Y |
                                                     COMPOSITE_FLIP_Y));
    } else {
      BattleEffect_LoadBlitter(
          HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                              COMPOSITE_FLIP_X));
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                     COMPOSITE_CLIP_Y |
                                                     COMPOSITE_FLIP));
    }
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    // The slot read through an offset struct: as unkb8[1], 8 of 989
    // instructions differ.
    draw[1] = ((struct {
                u8 before[188];
                DrawRectangle draw;
              } *)Ram_HeapSlots)
                  ->draw;
    for (i = 0; i != blows; i++) {
      s32 start = i * RECORD(RECORD_INTERVAL);

      if (frame >= start && frame < start + 6) {
        s32 step = frame - start;

        if ((i & 3) <= 1 || RECORD(RECORD_ARC_SHEET) == 1) {
          // The arc: six 48x72 cels.
          if (effect->side == 0)
            draw[i & 1](canvas, work->sheet + 0xc80 + step * 3456,
                        targetPos.x / 2 - 16, targetPos.y - 40, 48, 72);
          else
            draw[i & 1](canvas, work->sheet + 0xc80 + step * 3456,
                        targetPos.x / 2 - 32, targetPos.y - 40, 48, 72);
        } else {
          // The wave: six 48x16 cels.
          if (effect->side == 0)
            draw[i & 1](canvas, WAVE_CELS + step * 768, targetPos.x / 2 - 16,
                        actorPos.y - 8, 48, 16);
          else
            draw[i & 1](canvas, WAVE_CELS + step * 768, targetPos.x / 2 - 32,
                        actorPos.y - 8, 48, 16);
        }
      }
      // The hit; the last blow ends the run.
      if (frame == start + 2) {
        if (RECORD(RECORD_FILL) == 1)
          Iwram_FillWordsEntry(canvas, 0x4000, 0x2f2f2f2f);
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 4);
        if (i == blows - 1) {
          BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
          work->shakeFrames = 8;
          BattlePlayback_StartFar(134);
        } else {
          if (i & 1)
            BattleMotion_ApplyKnockbackFar(effect->actors[0], 7);
          work->shakeFrames = 4;
          Sound_PlayFar(134);
        }
        for (j = 0; j != RECORD(RECORD_MOTES); j++)
          MOTES[i * 32 + j].variant = (Random16() & 7) + 15;
      }
      // The orb: six 20x48 cels, two frames each.
      if (frame >= start + 2 && frame < start + 14)
        draw[0](canvas, ORB_CELS + ((frame - start - 2) / 2) * 960,
                targetPos.x / 2 - 10, targetPos.y - 24, 20, 48);
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);

    // The motes, until their life runs out, bouncing off the ground.
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    BattleEffect_LoadBlitter(
        HEAP_BLITTER,
        BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    BattleEffect_LoadBlitter(
        HEAP_BLITTER_ALTERNATE,
        BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    // The slot read through an offset struct: as unkb8[1], 8 of 989
    // instructions differ.
    draw[1] = ((struct {
                u8 before[188];
                DrawRectangle draw;
              } *)Ram_HeapSlots)
                  ->draw;
    for (i = 0; i != MOTE_COUNT; i++) {
      struct EffectStep *mote = &MOTES[i];
      s32 size = mote->variant;

      if (size > 0) {
        EffectPosition_ProjectPoint(&mote->x, &pos);
        pos.x /= 2;
        size >>= 3;
        size++;
        draw[(i / 2) & 1](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                          pos.x - size / 2, pos.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(mote, 60, -0x400);
        if (mote->y < 0x80000)
          mote->velocityY = -mote->velocityY / 2;
        mote->variant--;
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);

    if (frame > 3) {
      // The glints: three a frame around the target, each with its own
      // random flip, until frame 31.
      if (RECORD(RECORD_EXTRA) == EXTRA_GLINTS && frame <= 31) {
        EffectPosition_ProjectScaledBattler(effect->actors[0], &spot);
        for (i = 0; i != 3; i++) {
          s32 angle = Random16() & 0xffff;
          s32 cel = frame & 3;
          s32 radius = (Random16() & 31) + 4;
          s32 x = spot.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
                  gData_08197492[cel] / 2;
          s32 y = spot.y - ((Trig_Cos(angle) * radius) >> 17) -
                  gData_08197498[cel] / 2;
          u32 flip = Random16();

          // An empty once-only loop ends CSE's block, so the two reads of each
          // size table's address stay apart; without it, 397 of 989
          // instructions differ in tla-en.
          do {
          } while (0);
          Func_08196404(HEAP_BLITTER_ALTERNATE, 7, 7,
                        Data_0819861b[flip & 3] | COMPOSITE_CLIP_X |
                            COMPOSITE_CLIP_Y,
                        BLIT_ADDITIVE);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas, work->sheet + gBattleEffect_GlintCells[cel], x, y,
              gData_08197492[cel], gData_08197498[cel]);
          HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
        }
      }
      // The smoke: sixteen puffs rise from the target's feet, six cels of
      // four frames each.
      if (RECORD(RECORD_EXTRA) == EXTRA_RIPPLE) {
        BattleEffect_LoadBlitter(
            HEAP_BLITTER,
            BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
        draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
        for (i = 0; i != 16; i++) {
          struct EffectStep *puff = &work->particles[i];

          if ((u32)puff->variant <= 23) {
            draw[0](canvas, SMOKE_CELS + (puff->variant / 4) * 0x480,
                    HI(puff->x) - 12, HI(puff->y) - 24, 24, 48);
            BattleEffectKernels_IntegrateVector2(puff, 62, -0x1000);
          }
          puff->variant++;
        }
        HeapSlot_Release(HEAP_BLITTER);
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  if (RECORD(RECORD_EXTRA) == EXTRA_RIPPLE)
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
