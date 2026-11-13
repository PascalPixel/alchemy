/// @file
/// The status motes of Taint and its kin, Sleep's sheep and Curse's flames, a
/// Djinni riding a swarm of sparks, and Balm, Tinder and Phoenix.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "module/battle_effect/battle_effect.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "resource_ids.h"
#include "macros.h"

// Battle effects: particle effect entry points.

/// Each entry passes a particle type (color and count) to
/// BattleEffect_RunTargetMotes. effect: effect arguments.
void BattleEffect_RunTaint(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleEffect_RunTargetMotes(effect, 0);
    return;
  }
  BattleEffect_RunTargetMotes(effect, 1);
}

DEFINE_EFFECT_KIND(BattleEffect_RunTargetMotesMode1,
                   BattleEffect_RunTargetMotes, 1)

DEFINE_EFFECT_KIND(BattleEffect_RunDelude, BattleEffect_RunTargetMotes, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunParalyze, BattleEffect_RunTargetMotes, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunHeartrender, BattleEffect_RunTargetMotes, 4)

DEFINE_EFFECT_KIND(BattleEffect_RunMysticFlame, BattleEffect_RunTargetMotes, 5)

DEFINE_EFFECT_KIND(BattleEffect_RunDemonFire, BattleEffect_RunTargetMotes, 7)

DEFINE_EFFECT_KIND(BattleEffect_RunDrown, BattleEffect_RunTargetMotes, 6)

#define MOTE_MAX 512

// The motes live in the map cell buffer, 64 for each target.

// Modes whose motes close in on the target and start again; the others
// drift up and are thrown back at random. Mode 7 closes in once.
#define CONVERGES(mode)                                                        \
  ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
#define DECAYS(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

// Motes swirl around each target in turn, eight frames apart, over a
// rippling canvas: smoke, hearts, glowing orbs or vortex motes by mode,
// in the mode's palette; mode 3 adds the tornado's glints, and mode 7 sends
// the caster at the first target for the final blow.
void BattleEffect_RunTargetMotes(struct BattleEffectArgument *effect,
                                 u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 total;
  s32 member;
  s32 count;
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition base;
  DrawRectangle draw[2];
  void *palette;
  s32 *row;
  s32 i;

  // Set up: the canvas layer and the mode's sheet and palette.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  count = 16;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  if (mode == 0) {
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  } else if (mode == 1) {
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else if (mode == 2) {
    palette = Resource_GetTableEntry((s32)&ResourceId_CyanPalette);
  } else if (mode == 3) {
    palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  } else if (mode == 4) {
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  } else if (mode == 5) {
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_GlowOrbSheet);
  } else if (mode == 7) {
    count = 24;
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else {
    count = 32;
    Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 0);
    palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
  }
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  if (mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
  if (mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                               work->sheet + RENDER_MODE_TORNADO, 1, 0);

  // Scatter the motes around the origin.
  for (i = 0; i != MOTE_MAX; i++) {
    struct EffectStep *mote = &PARTICLES[i];

    if (CONVERGES(mode)) {
      SET_POSITION(mote, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 14,
                   ((Random16() & 0xff) - 127) << 15);
    } else {
      SET_POSITION(mote, ((Random16() & 0xff) - 127) << 13,
                   ((Random16() & 0xff) - 255) << 13,
                   ((Random16() & 0xff) - 127) << 13);
    }
    mote->variant = 0;
  }

  if (CONVERGES(mode))
    total = effect->count * 8 + 64;
  else
    total = effect->count * 8 + 32;
  if (!(mode <= 1 || mode == 3))
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // Mode 7 sends the caster at the first target 46 frames before the
    // end and strikes it 14 frames later; the others begin phase 133.
    if (mode == 7) {
      if (frame == total - 46)
        BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
      if (frame == total - 32) {
        BattlePlayback_StartFar(134);
        BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
        work->shakeFrames = 8;
      }
    } else if (frame == total - 32) {
      BattlePlayback_StartFar(133);
    }

    // Ripple the canvas: one horizontal offset for each line.
    row = work->bg2xLines;
    for (i = 0; i != 160; i++)
      *row++ = (0x40000 - (Trig_Sin((frame << 12) + i * 0x800) << 2)) >> 10;

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          BattleObject_GetFar(effect->actors[member])->object;

      // Mode 3: two tornado glints a frame around the target, each drawn
      // with a blitter of its own, mirrored at random.
      if (mode == 3 && frame > member * 8 && frame < member * 8 + 32) {
        EffectPosition_ProjectScaledBattler(effect->actors[member], &base);

        for (i = 0; i != 2; i++) {
          s32 angle;
          s32 radius;
          s32 x;
          s32 y;
          const u8 *h;
          const u8 *w;

          /* FAKEMATCH(temporary): w and h set inside statement expressions at
           * their first reads. */
          angle = Random16() & 0xffff;
          radius = (Random16() & 31) + 4;
          x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
              (({
                 w = gData_08197492;
                 w;
               })[frame & 3] >>
               1);
          /* FAKEMATCH(barrier): an empty asm through w hides that it holds the
           * width table's address. */
          __asm__("" : "=r"(w) : "0"(w));
          y = base.y - ((Trig_Cos(angle) * radius) >> 16) -
              (({
                 h = gData_08197498;
                 h;
               })[frame & 3] >>
               1);
          /* FAKEMATCH(barrier): an empty asm through h, the same for the height
           * table. */
          __asm__("" : "=r"(h) : "0"(h));
          BattleEffect_BuildBlitter(HEAP_BLITTER_ALTERNATE, 7, 7,
                                    COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                        Data_08197a34[Random16() & 3],
                                    BLITTER_MAX);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas,
              work->sheet + gBattleEffect_GlintCells[frame & 3] +
                  RENDER_MODE_TORNADO,
              x, y + 16, w[frame & 3], h[frame & 3]);
          HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
        }
      }

      // An additive and a max blitter for this target's motes.
      BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE << 4 |
                                                 COMPOSITE_CLIP_X |
                                                 COMPOSITE_CLIP_Y);
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE, BLITTER_MAX << 4 |
                                                           COMPOSITE_CLIP_X |
                                                           COMPOSITE_CLIP_Y);
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = object->x;
      point[1] = 160 << 13;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);

      // From its turn on, the target's motes turn around it.
      if (frame > member * 8) {
        Transform_ApplyYaw(frame << 9);
        if (mode <= 1 || mode == 4)
          Transform_ApplyPitch(frame << 9);
        for (i = 0; i != count; i++) {
          struct EffectStep *mote = &PARTICLES[member * 64 + i];
          s32 distance;

          if (frame > member * 8 + i) {
            s32 xx = (mote->x >> 8) * (mote->x >> 8);
            s32 yy = (mote->y >> 8) * (mote->y >> 8);
            s32 zz = (mote->z >> 8) * (mote->z >> 8);

            distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0 && mote->variant <= 23) {
              s32 cel = mote->variant / 4;

              EffectPosition_ProjectPoint((s32 *)mote, &screen);
              screen.x >>= 1;
              if (mode == 5 || mode == 7)
                draw[1](canvas, work->sheet + cel * 1600, screen.x - 20,
                        screen.y - 20, 40, 40);
              else if (mode == 6)
                draw[1](canvas, work->sheet + 0xc00, screen.x - 6,
                        screen.y - 12, 12, 24);
              else if (mode == 4)
                draw[1](canvas, work->sheet, screen.x - 11, screen.y - 21, 22,
                        42);
              else
                draw[(i & 3) != 0](canvas, work->sheet + cel * 1152,
                                   screen.x - 12, screen.y - 24, 24, 48);

              // Close in by a step of the distance, or drift up.
              if (DECAYS(mode)) {
                mote->x -= mote->x / distance;
                mote->y -= mote->y / distance;
                mote->z -= mote->z / distance;
              } else {
                mote->y += px(1);
              }
              mote->variant++;
              if (mote->variant == 24) {
                if (DECAYS(mode)) {
                  mote->variant = 0;
                } else {
                  SET_POSITION(mote, ((Random16() & 0xff) - 127) << 13,
                               ((Random16() & 0xff) - 255) << 12,
                               ((Random16() & 0xff) - 127) << 13);
                }
              }
            }
          }
        }
      }
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);

      // The target flashes 16 frames into its turn, until the end.
      if (frame == member * 8 + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        BattleEffect_TintBattler(effect->actors[member], 7, 5, member, delay);
      }
    }

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  if (!(mode <= 1 || mode == 3))
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleEffect_EndCanvasLayer();
}

#undef GROUND_Y

#define SHEEP_COUNT 12
#define SHEEP_FRAMES 98
#define GROUND_Y (92 << 16)

// A flock of sheep cards drops in over the targets, one every four frames,
// and bounces on the ground, tumbling through eight cels; at frame 80 the
// next phase begins, and each target turns back to its own palette in turn.
void BattleEffect_RunSleep(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  struct EffectPosition middle;
  s32 record[3];
  struct EffectStep *one;
  struct EffectStep *sheep;
  s32 i;
  s32 frame;
  DrawRectangle drawB;
  DrawRectangle drawA;
  s32 scroll;
  s32 member;

  // Set up: the canvas layer, the sheep sheet and a transparent blitter
  // pair, the second mirrored.
  sheep = (struct EffectStep *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  scroll = 0;
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = scroll;
  Resource_LoadAndDecompress((s32)&ResourceId_SheepCardSheet, work->sheet, 1,
                             1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_TRANSPARENT << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  drawA = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP);
  drawB = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The sheep wait above the screen with a random spin and starting cel.
  for (i = 0; i != 32; i++) {
    one = &sheep[i];
    one->x = ((Random16() & 63) + 32) << 16;
    one->y = -px(32);
    one->velocityY = Random16() & 0;
    one->z = Random16() & 3;
    one->variant = Random16() & 255;
  }

  // Centre the canvas on the targets; what BG2 cannot reach moves the sheep.
  BattleEffect_GetTargetsMidpoint(&middle);
  middle.x -= 64;
  if (middle.x < 0) {
    scroll = middle.x;
    middle.x = 0;
  } else if (middle.x > 112) {
    scroll = middle.x - 112;
    middle.x = 112;
  }
  REG_BG2X = -middle.x << 8;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != SHEEP_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 80)
      BattlePlayback_StartFar(0);

    // Place each target, and from frame 64 restore its palette, one
    // target every 16 frames.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object;

      object = BattleObject_GetFar(effect->actors[member])->object;
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      record[0] = object->x;
      record[1] = 160 << 14;
      record[2] = object->z;
      SceneTransform_ApplyPosition(record);
      if (frame == member * 16 + 64)
        BattleEffect_TintBattler(effect->actors[member], 0, 5, -1, 0);
    }

    // The sheep fall and bounce, each spinning through its eight cels:
    // the first four drawn as they are, the last four mirrored.
    one = sheep;
    for (i = 0; i != SHEEP_COUNT; i++, one++) {
      if (frame > i * 4 && one->y <= 0x7fffff) {
        s32 cel;

        cel = (one->variant / 16) & 7;
        if (cel < 4)
          drawA(canvas, work->sheet + (cel << 10) + 0x800,
                scroll + (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
        else
          drawB(canvas, work->sheet + (cel << 10) - 0x800,
                scroll + (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
        one->y += one->velocityY;
        one->velocityY += px(1) / 8;
        one->variant += one->z;
        if (one->y > GROUND_Y && one->velocityY == 0) {
          s32 speed = one->velocityY + 1;
          one->y = GROUND_Y;
          one->z += 4;
          one->velocityY = -speed / 2;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define MARKER_FRAMES 72 // how long each marker stays
#define MARKER_STAGGER 16
#define MARKER_CELS 9

// As in The Broken Seal's twin, which loads the same sheet: a small flame
// burns above each target in turn, MARKER_STAGGER frames apart, for up to
// MARKER_FRAMES frames each, over a marker at its feet.
void BattleEffect_RunCurse(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  struct EffectStep *marker;
  s32 i;
  s32 frame;
  s32 start;
  s32 cel;
  s32 y;
  struct EffectPosition pos;

  // Set up: the canvas layer, unscaled; for side 1 the canvas shows the
  // right half of the field.
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1,
                             1);
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_TRANSPARENT << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  work->transferMode = 1;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Each flame starts at a random point in its turn.
  for (i = 0; i != effect->count; i++) {
    work->particles[i].variant = Random16() & 63;
  }

  frame = 0;
  while (frame != (effect->count << 5) + 32) {
    if (frame == 32) {
      BattlePlayback_StartFar(0);
    }
    i = 0;
    if (effect->count != 0) {
      do {
        start = i * MARKER_STAGGER;
        if (frame == start) {
          Sound_PlayFar(SE_CRACK);
        }
        if (frame >= start && frame < start + MARKER_FRAMES) {
          // The marker at the target's feet, then the flame, six frames a cel.
          marker = &work->particles[i];
          EffectPosition_ProjectScaledBattler(effect->actors[i], &pos);
          if (effect->side == 1) {
            pos.x -= 112;
          }
          y = pos.y;
          pos.y = y - 16;
          draw[0](canvas, work->sheet + 1728, pos.x - 8, y - 20, 16, 20);
          // DEAD: always true here; the ROM keeps the test.
          if (frame >= start) {
            cel = ((frame - start) + marker->variant) / 6 % MARKER_CELS;
            draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16, 16,
                    12);
          }
        }
        i++;
      } while (i != effect->count);
    }
    work->transferPending = 1;
    wait(1);
    frame += 1;
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// A Djinn rides a swarm of sparks from the caster to the first target: 32
// sparks spin around it in four planes while it travels, then it hovers
// over the targets while puffs of sparkle gather on each in turn.
void BattleEffect_RunDjinnHeal(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 member;
  u8 *sheet;
  DrawRectangle *draw;
  struct MotionObject *actor;
  struct MotionObject *goal;
  s32 frame;
  s32 i;
  s32 position[3];
  s32 target[3];
  s32 delta[3];
  s32 rider[3];
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition view;
  DrawRectangle callbacks[2];

  // Set up: the canvas layer, the Djinn and the particle sprites, and the
  // blitter pair for the side the Djinn faces.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(effect->side ^ 1, draw = callbacks);
  actor = BattleObject_GetFar(effect->actor)->object;
  goal = BattleObject_GetFar(effect->actors[0])->object;

  // The swarm: 64 sparks thrown out level from the Djinn.
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 heading = Random16() & 0xffff;
    s32 speed = (Random16() & 0xff) + 128;

    SET_POSITION(spark, 0, ((Random16() & 31) + 20) << 16, 0);
    spark->velocityX = (Trig_Sin(heading) * speed) >> 5;
    spark->velocityY = 0;
    spark->velocityZ = (Trig_Cos(heading) * speed) >> 5;
    spark->variant = 0;
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The Djinn travels from the caster to above the first target in 40
  // frames.
  position[0] = actor->x;
  position[1] = 0;
  position[2] = actor->z;
  target[0] = goal->x;
  target[1] = 180 << 15;
  target[2] = 0;
  delta[0] = (target[0] - position[0]) / 40;
  delta[1] = (target[1] - position[1]) / 40;
  delta[2] = (target[2] - position[2]) / 40;

  for (frame = 0; frame != 98; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 8)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 80)
      Sound_PlayFar(SE_SWIRL);
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= 30 && frame < 70) {
      position[0] += delta[0];
      position[1] += delta[1];
      position[2] += delta[2];
    }
    SceneTransform_ApplyPosition(position);

    // The caster flashes for the first 24 frames.
    if (frame == 0)
      BattleEffect_TintBattler(effect->actor, 7, -1, -1, 0);
    if (frame == 24)
      BattleEffect_TintBattler(effect->actor, 0, -1, -1, 0);

    // Each spark spins in one of four planes, smaller further away, and
    // after 30 frames is pulled back toward the Djinn.
    for (i = 0; i != 32; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (frame > i && spark->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case 0:
          Transform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case 1:
          Transform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case 2:
          Transform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        case 3:
          Transform_ApplyPitch(-frame * (i * 32 + 256));
          Transform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        }
        EffectPosition_ProjectPoint((s32 *)spark, &screen);
        screen.x >>= 1;
        Transform_Pop();
        if (screen.depth < 250)
          screen.depth = 250;
        if (screen.depth > 634)
          screen.depth = 634;
        size = 9 - (screen.depth - 250) / 64;
        callbacks[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                     screen.x - size / 2, screen.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(spark, 60, 0);
        if (frame > i + 30) {
          s32 dx = -spark->x >> 8;
          s32 dy = -spark->y >> 8;
          s32 dz = -spark->z >> 8;

          spark->velocityX += dx;
          spark->velocityY += dy;
          spark->velocityZ += dz;
        }
      }
    }

    // The Djinn bobs over the target for the last 15 frames.
    if (frame > 82) {
      rider[0] = 0;
      rider[1] = Trig_Sin(frame << 10) << 2;
      rider[2] = 0;
      EffectPosition_ProjectPoint(rider, &screen);
      screen.x >>= 1;
      draw[1](canvas, work->sheet, screen.x - 10, screen.y - 17, 20, 34);
    }
    work->transferPending = 1;
    wait(1);
  }

  // The second part: 64 puffs of sparkle for each target.
  for (i = 0; i != 512; i++) {
    struct EffectStep *spark = &PARTICLES[i];

    SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                 ((Random16() & 0x7f) + 64) << 15,
                 ((Random16() & 0xff) - 127) << 15);
    spark->variant = 0;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);

  for (frame = 0; frame != effect->count * 8 + 72; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // The Djinn sways over the targets, then rises away for the last 32
    // frames.
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= effect->count * 8 + 40)
      target[1] += 0x40000;
    point[0] = target[0];
    point[1] = target[1];
    point[2] = target[2] + Trig_Sin(frame << 11) * 40;
    EffectPosition_ProjectPoint(point, &view);
    view.x >>= 1;
    callbacks[0](canvas, work->sheet, view.x - 10, view.y - 17, 20, 34);

    // Each target in turn, eight frames apart: its puffs close in around
    // it, and it flashes from 40 to 64 frames into its turn.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          BattleObject_GetFar(effect->actors[member])->object;
      s32 start = member * 8;

      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      position[0] = object->x;
      position[1] = 160 << 14;
      position[2] = object->z;
      SceneTransform_ApplyPosition(position);
      if (frame == start + 30)
        Sound_PlayFar(SE_RECOVERY);
      if (frame == start + 40)
        BattleEffect_TintBattler(effect->actors[member], 7, -1, -1, 0);
      if (frame == start + 64)
        BattleEffect_TintBattler(effect->actors[member], 0, -1, -1, 0);
      if (frame > start) {
        Transform_ApplyYaw((frame - start) << 9);
        for (i = 0; i != 64; i++) {
          struct EffectStep *spark = &PARTICLES[member * 64 + i];

          if (frame > start + i / 2) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 cel;

              EffectPosition_ProjectPoint((s32 *)spark, &view);
              view.x >>= 1;
              if (view.depth < 314)
                view.depth = 314;
              if (view.depth > 634)
                view.depth = 634;
              // DEAD: BattleEffect_RunHealingMotes' size by depth, copied with
              // its code; the next line sets cel for real.
              cel = 3 - (view.depth - 314) / 128;
              cel = (i * 4 + frame) % 9;
              {
                u8 *cell = sheet + gBattleEffect_PuffCells[cel];
                u32 edge = gBattleEffect_PuffSizes[cel];
                u32 half = edge >> 1;

                draw[1](canvas, cell, view.x - half, view.y - half, edge, edge);
              }
              spark->x -= spark->x / distance;
              spark->y -= spark->y / distance;
              spark->z -= spark->z / distance;
            }
          }
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Balm (kind 230).
void BattleEffect_RunBalm(struct BattleEffectArgument *effect) {
  BattleEffect_RunTumblingQuads(effect, 0);
}

/// Abilities: Tinder (kind 231).
void BattleEffect_RunTinder(struct BattleEffectArgument *effect) {
  BattleEffect_RunTumblingQuads(effect, 1);
}

/// Abilities: Phoenix, Regen Dance (kind 436).
void BattleEffect_RunPhoenix(struct BattleEffectArgument *effect) {
  BattleEffect_RunTumblingQuads(effect, 2);
}
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef HEAP_BLITTER
#undef GROUND_Y
#undef MOTE_MAX
#undef CONVERGES
#undef DECAYS
#undef SHEEP_COUNT
#undef SHEEP_FRAMES
#undef MARKER_FRAMES
#undef MARKER_STAGGER
#undef MARKER_CELS
