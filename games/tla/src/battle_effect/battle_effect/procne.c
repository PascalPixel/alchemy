/// @file
/// The Procne summon, and the scene of summon 7.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "module/battle_effect/battle_effect.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "module/system/system.h"
#include "gamepak/far_call/field.h"
#include "mul_q16.h"
#include "resource_ids.h"

// Summon 8 (BattleEffect_RunSceneEffect's case 8). Twin: The Broken Seal's
// BattleEffect_RunProcne, ported by trial 2 and rewritten here. The
// Lost Age's changes: BattleEffect_FetchRectangleBlitters for the pair; the
// whirlwind unpacked with its palette by Resource_LoadAndDecompress into the
// work sheet, the comets into work->sheet + 0x3600; frame / 4 % 3 written
// through Math_UnsignedMulHighAdd(x, 0x55555556, -3) and the depth divisions by
// 36 and 90 through Math_UnsignedMulHigh with their reciprocals (calls the
// source makes: agscc emits __modsi3 and __divsi3 for the operators);
// Iwram_TransformVector at the comet sparks; formation resource 646; the effect
// argument for the actor loops; gCameraSceneParameters declared unsized (with
// the shared u32[3], word 3 folds into the address).

// What fixed the frame and slots: screen.y >> 16 and screen.depth >>= 16 as the
// twin writes them (combine makes the ldrsh off the pointer to screen; a cast
// to s16 * gave screen + 6 its own register and two spills); the twin's
// declaration order; wave and drift in a block opened after the blitter fetch,
// whose spill slots fall below the pointer to draw.

// frame / 4 % 3, as the source writes it: the multiply-high routine with the
// reciprocal of 3, where a plain % calls __modsi3.
#define MOD3(v) Math_UnsignedMulHighAdd((v), 0x55555556, -3)

#define WHIRLWIND_CEL (48 * 64) // one of the whirlwind's three frames
#define DUST_SHEET 0x2400       // the dust motes, after the whirlwind
#define COMET_SHEET 0x3600      // the violet comets
#define COMETS 9
#define FORMATION_RESOURCE 646

// Summon 8: a whirlwind rises under the enemies and lifts them, spinning,
// while dust motes swirl up through it (frames 0 to 159; A or B skips to
// 160). Then the sky turns violet and a formation of nine comets wheels
// in; a cloud of sparks gathers on its centre, and from frame 330 four
// comets strike one after another, each with a burst of sparks, throwing
// the enemies down. A or B skips frames 161 to 394.
void BattleEffect_RunProcne(struct BattleEffectArgument *effect) {
  void *canvas;
  void *camera;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *graphics;
  struct MotionObject *object;
  struct BattleObj *context;
  struct EffectStep *point;
  struct EffectStep *spark;
  struct EffectStep *mark;
  s32 index;
  s32 slot;
  s32 size;
  s32 span;
  s32 dist;
  s32 alive;
  s32 heading[8];
  s32 radius[8];
  s32 spin[8];
  struct Point2D home[8];
  s32 baseAngle[8];
  s32 vector[3];
  struct EffectPosition screen;
  s32 shake[3];
  s32 place[4];
  struct Scale scale;

  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up: the max and additive pair, the whirlwind with its palette, the
  // dots, and the transfer at half strength.
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, draw);
  {
    struct Point2D wave;
    struct Point2D drift;
    Resource_LoadAndDecompress((s32)&ResourceId_WhirlwindSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
    work->transferMode = 2;
    work->transferValue = 50;
    Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
    wave.x = 0x800000;
    wave.y = 0x280000;
    drift.x = 0;
    drift.y = 0;

    // Each enemy's place, heading, distance and facing, to restore at the
    // end; its bobbing stops.
    for (index = 0; index != effect->count; index++) {
      context = GetBattleObjectSlotFar(effect->actors[index]);
      object = context->object;
      home[index].x = object->x;
      home[index].y = object->z;
      baseAngle[index] = object->angle;
      heading[index] = (u16)ArcTan2(object->x, object->z);
      radius[index] = Iwram_Sqrt((object->x >> 8) * (object->x >> 8) +
                                 (object->z >> 8) * (object->z >> 8)) >>
                      7;
      spin[index] = 0;
      object->verticalMotionStrength = 0;
    }

    // The dust motes start at the whirlwind's foot, each after its own wait.
    index = 0;
    do {
      point = &work->particles[index];
      point->z = 120;
      point->y = 0;
      Random16();
      point->velocityY = 0;
      point->velocityX = 0;
      point->variant = Random16() & 63;
      index++;
    } while (index != 64);

    frame = 0;
    do {
      if ((gInput.repeat & 3) != 0) {
        if (frame <= 159)
          frame = 160;
        else if (frame <= 394)
          frame = 395;
      }

      Transform_Reset();
      Transform_SetLookAt(camera, (s32 *)((u8 *)camera + 12));

      if (frame == 16)
        Sound_PlayFar(141);
      if (frame == 256)
        Sound_PlayFar(140);
      if (frame == 334)
        Sound_PlayFar(212);
      if (frame == 347)
        Sound_PlayFar(212);
      if (frame == 359)
        Sound_PlayFar(212);
      if (frame == 372)
        Sound_PlayFar(212);

      // The enemies, one every 16 frames, spiral in toward the centre and
      // rise, spinning faster; each comet strike knocks them down.
      for (index = 0; index != effect->count; index++) {
        if (frame > index * 16) {
          context = GetBattleObjectSlotFar(effect->actors[index]);
          object = context->object;
          object->x = (radius[index] * Trig_Sin(heading[index])) >> 1;
          object->z = (radius[index] * Trig_Cos(heading[index])) >> 1;

          if (frame <= 159) {
            if (frame > index * 16 + 16)
              spin[index] += 48;
            if (radius[index] <= 31)
              object->y += 0x60000;
            else
              object->y += 0x8000;
            if (object->y > 0x7c0000)
              object->y = 0x7c0000;
          }

          if (frame <= 490) {
            heading[index] += spin[index];
            if (heading[index] > 0x10000)
              heading[index] -= 0x10000;
            object->angle += spin[index] / 4;
          }

          if (frame == 395)
            object->verticalMotionStrength = 0xab85;

          slot = 0;
          do {
            if (frame == gProcne_Frames[slot]) {
              object->velocityY = 0;
              BattleEffect_TintBattler(effect->actors[index], 7, 5, index, 8);
            }
            slot++;
          } while (slot != 4);

          if (radius[index] > 16)
            radius[index] -= 2;
        }
      }

      // The whirlwind: its foot grows from frame 16, its top from 48, and
      // from 160 it stands whole, two cels high.
      if ((u32)(frame - 16) <= 143) {
        size = frame * 2 - 32;
        if (size > 48)
          size = 48;
        draw[0](
            canvas,
            &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL + (48 - size) * 48],
            32, 112 - size, 48, size);
      }
      if ((u32)(frame - 48) <= 111) {
        size = frame * 2 - 96;
        if (size > 64)
          size = 64;
        draw[0](
            canvas,
            &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL + (64 - size) * 48],
            32, 64 - size, 48, size);
      }
      if ((u32)(frame - 160) <= 239) {
        draw[0](canvas, &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL], 8, 0, 48,
                64);
        draw[0](canvas, &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL], 8, 64,
                48, 64);
      }

      // The dust motes swirl up the whirlwind, speeding up, and start again
      // from the foot when they pass the top.
      if (frame <= 159) {
        index = 0;
        point = work->particles;
        do {
          if (point[index].variant == 0) {
            vector[0] = point[index].z * Trig_Sin(point[index].velocityY);
            vector[1] = point[index].y;
            vector[2] = point[index].z * Trig_Cos(point[index].velocityY);
            EffectPosition_ProjectPoint(vector, &screen);
            screen.x >>= 1;
            if (point[index].y <= 0x3fffff)
              draw[0](canvas, work->sheet + DUST_SHEET, screen.x - 12,
                      screen.y - 12, 16, 16);
            if (point[index].z > 24)
              point[index].z -= 4;
            point[index].velocityY += point[index].velocityX * 2;
            if (point[index].velocityY > 0x10000)
              point[index].velocityY -= 0x10000;
            point[index].velocityX += 50;
            if (point[index].velocityX > 4096)
              point[index].velocityX = 4096;
            point[index].velocityY += 2048;
            point[index].y += point[index].velocityY * 2;
            if (point[index].y > 0x300000) {
              point[index].y = 0;
              point[index].z = 100;
              point[index].velocityY = 0;
              point[index].velocityX = 0;
            }
          } else {
            point[index].variant--;
          }
          index++;
        } while (index != 24);
      }

      // Frame 160: the violet sky, the formation of comets, sparks scattered
      // round its centre, and the enemies dropped back at random heights.
      if (frame == 160) {
        s32 speed;
        s32 angle;

        BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
        BattleBackground_SetModeFar(1, (s32)&ResourceId_VioletSkyBackdrop, 8);
        BattleEffect_SelectLivingTargets(work->effect);
        BattleEffect_SpawnObjects(COMETS, FORMATION_RESOURCE, 2);
        Resource_LoadAndDecompress((s32)&ResourceId_VioletCometSheet,
                                   work->sheet + COMET_SHEET, 1, 1);

        index = 0;
        do {
          PARTICLES[index].variant = 0;
          index++;
        } while (index != 512);

        for (index = 0; index != effect->count; index++) {
          context = GetBattleObjectSlotFar(effect->actors[index]);
          object = context->object;
          object->y = ((Random16() & 15) + 8) << 16;
        }

        gCameraSceneParameters[3] = 72;

        index = 0;
        do {
          speed = Random16() & 127;
          angle = (Random16() & 0x7fff) + 0x9fff;
          work->particles[index].x = Trig_Sin(angle) * speed;
          work->particles[index].y = Trig_Cos(angle) * speed;
          work->particles[index].z = (Random16() % 200 - 100) << 16;
          work->particles[index].variant = 0;
          index++;
        } while (index != 32);

        for (index = 0; index != effect->count; index++) {
          radius[index] = 16;
          spin[index] = 6000;
        }
      }

      if (frame > 159) {
        // The formation sways on a slow wave, pushed back by each strike.
        scale = Data_08196df8;
        size = 16;
        if (frame - 160 <= 64)
          size = 32;
        wave.x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
        wave.y += (Trig_Sin((frame - 160) << 9) * size) >> 6;
        wave.x += drift.x;
        wave.y += drift.y;
        drift.x /= 2;
        drift.y /= 2;

        scale.x = 0x10000;
        scale.y = 0x10000;
        place[3] = 0;
        index = 0;
        do {
          place[0] = (gProcne_GridX[index] << 16) + wave.x + 0x280000;
          place[1] = 0xff0000 + index * 0x400000;
          place[2] = ((gProcne_GridY[index] + index * 64) << 16) + wave.y;
          Render_ApplyProjectedPlacementFar(
              (struct ScaledSprite *)work->objects[index], place, &scale.x, 0);
          index++;
        } while (index != COMETS);

        if (frame > 255) {
          s32 threshold;

          shake[2] = 0x1000000;
          alive = 0;
          shake[0] = 0;
          shake[1] = 0;
          Transform_Reset();
          SceneTransform_ApplyPosition(shake);

          // Until frame 330 the scattered sparks gather on the centre, each
          // a part of its distance a frame; those that have arrived make one
          // spark that grows with their number.
          if (frame <= 329) {
            index = 0;
            do {
              s32 xx;
              s32 yy;
              s32 zz;

              point = &work->particles[index];
              xx = (point->x >> 8) * (point->x >> 8);
              yy = (point->y >> 8) * (point->y >> 8);
              zz = (point->z >> 8) * (point->z >> 8);
              dist = Iwram_Sqrt(xx + yy + zz) >> 9;
              if (dist != 0) {
                EffectPosition_ProjectPoint(&point->x, &screen);
                screen.x = (screen.x >> 17) + (wave.x >> 17) + 32;
                screen.y = (screen.y >> 16) + (wave.y >> 16) - 4;
                screen.depth >>= 16;
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 6 - Math_UnsignedMulHigh(screen.depth - 170, 0x071c71c8);
                draw[1](canvas, graphics + gBattleEffect_MoteCells[span - 1],
                        screen.x - span / 2, screen.y - span, span, span * 2);
                point->x -= point->x / dist;
                point->y -= point->y / dist;
                point->z -= point->z / dist;
              } else {
                alive++;
              }
              index++;
            } while (index != 32);

            if (alive > 0) {
              span = alive / 10 + 1;
              draw[1](canvas, graphics + gBattleEffect_MoteCells[span - 1],
                      (wave.x >> 17) - span / 2 + 32, (wave.y >> 16) - span - 4,
                      span, span * 2);
            }
          }

          // Each strike throws out 96 sparks that fly and fade.
          slot = 0;
          do {
            if (frame == gProcne_Frames[slot]) {
              s32 speed;
              s32 angle;

              index = 0;
              do {
                speed = (Random16() & 127) + 16;
                angle = Random16() & 0xffff;
                spark = &PARTICLES[slot * 128 + index];
                spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
                spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
                spark->velocityY = ((Random16() & 255) - 128) << 10;
                spark->x = 0;
                spark->y = 0;
                spark->z = 0;
                spark->variant = (Random16() & 15) + 64;
                index++;
              } while (index != 96);
            }

            if (frame >= gProcne_Frames[0]) {
              spark = PARTICLES;
              index = 0;
              do {
                if (spark[index].variant > 0) {
                  // The transform in a once-only block, which ends CSE's block
                  // before it; as a plain block, 6 of 1493 instructions differ
                  // in tla-en.
                  do {
                    void (*transform)(const void *, s32 *) =
                        Iwram_TransformVector;
                    /* FAKEMATCH(pin): the result pointer in r1, set before the
                     * point in r0; as a plain argument, 2 of 1493 instructions
                     * differ in tla-en. */
                    register s32 *result __asm__("r1") = &screen.x;
                    transform(&spark[index].x, result);
                  } while (0);
                  screen.x = (screen.x >> 17) + 32;
                  screen.y = (screen.y >> 16) + 56;
                  screen.depth >>= 16;
                  if (screen.depth <= 169)
                    screen.depth = 170;
                  if (screen.depth > 350)
                    screen.depth = 350;
                  span =
                      3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                  draw[0](canvas, graphics + gBattleEffect_MoteCells[span - 1],
                          screen.x - span / 2, screen.y - span, span, span * 2);
                  spark[index].x += spark[index].velocityX;
                  spark[index].y += spark[index].velocityY;
                  spark[index].z += spark[index].velocityY;
                  spark[index].variant--;
                }
                index++;
              } while (index != 512);
            }
            slot++;
          } while (slot != 4);

          // From frame 330, a comet every twelve frames: it strikes at the
          // formation's centre, the screen shakes and the formation is
          // pushed back; a ring of 28 sparks opens round the mark.
          index = 0;
          do {
            mark = &work->particles[index + 60];
            threshold = index * 12 + 330;
            if (frame == threshold) {
              mark->x = (wave.x >> 17) + 32;
              mark->y = (wave.y >> 16) - 4;
              mark->velocityX = (wave.x >> 17) + 32;
              mark->velocityY = (wave.y >> 16) - 4;

              slot = 0;
              do {
                work->particles[slot + 32].variant = 4;
                slot++;
              } while (slot != 28);

              drift.x = 0x80000;
              drift.y = -0x20000;
              work->shakeFrames = 8;
            }

            if (frame >= threshold) {
              s32 cel;

              cel = (frame - threshold) / 2;
              if (cel > 2)
                cel = 2;
              draw[1](canvas, work->sheet + gProcne_Cells[cel], mark->x,
                      mark->y - gProcne_CellBiasY[cel], gProcne_CellWidths[cel],
                      gProcne_CellHeights[cel]);
              mark->x -= 8;
              mark->y += 2;

              if (frame < threshold + 8) {
                Transform_ApplyRoll(-2048);
                Transform_ApplyYaw(-4096);

                slot = 0;
                spark = &work->particles[32];
                do {
                  struct EffectStep *ring;

                  ring = &spark[slot];
                  vector[0] = 0;
                  vector[1] = ring->variant * Trig_Cos(slot * 2340);
                  vector[2] = ring->variant * Trig_Sin(slot * 2340);
                  ring->variant += 2;
                  EffectPosition_ProjectPoint(vector, &screen);
                  screen.x =
                      (screen.x >> 17) + work->particles[index + 60].velocityX;
                  screen.y =
                      (screen.y >> 16) + work->particles[index + 60].velocityY;
                  screen.depth >>= 16;
                  if (screen.depth <= 169)
                    screen.depth = 170;
                  if (screen.depth > 350)
                    screen.depth = 350;
                  span =
                      3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                  draw[0](canvas, graphics + gBattleEffect_MoteCells[span - 1],
                          screen.x - span / 2, screen.y - span, span, span * 2);
                  slot++;
                } while (slot != 28);
              }
            }
            index++;
          } while (index != 4);
        }
      }

      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
    } while (frame != 400);

    // The results; the enemies go back where they stood, facing as before.
    BattlePlayback_StartFar(134);
    for (index = 0; index != effect->count; index++) {
      struct MotionObject *enemy;

      context = GetBattleObjectSlotFar(effect->actors[index]);
      enemy = context->object;
      enemy->x = home[index].x;
      enemy->z = home[index].y;
      enemy->angle = baseAngle[index];
    }
    gCameraSceneParameters[3] = 120;
    BattleEffect_SetupBlendedDisplay();

    index = 0;
    do {
      Sprite_DestroyFar(work->objects[index]);
      index++;
    } while (index != COMETS);

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    BattleEffect_EndCanvasLayer();
  }
}

/// Abilities: Dragon Driver (kind 385).
DEFINE_EFFECT_KIND(BattleEffect_RunDragonDriver, BattleEffect_RunDragonFire, 1)

/// Abilities: Tiamat (scene kind 7).
DEFINE_EFFECT_KIND(BattleEffect_RunTiamat, BattleEffect_RunDragonFire, 0)

#undef FORMATION_RESOURCE

// The rising grains live in the map buffer.

// The high halfword of a 16.16 value: its whole pixels.
#define HI(v) (((s16 *)&(v))[1])

#define STREAK_SHEET 0x3000 // the ember streaks, after the flashes
#define FORMATION_RESOURCE 647

// Summon 7 (mode 0; mode 1 is the same scene cast from the caster's side,
// without the formation): streaks rain down and splash, sparks and grains
// rise from where the formation stands, a burst of flashes at frame 128,
// then the screen whitens and the canvas fades away. In mode 0 a formation
// of eight drops in at frame 40 and bursts at the end. A or B skips frames
// 49 to 159.
void BattleEffect_RunDragonFire(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition origin;
  DrawRectangle draw[2];
  void *canvas;
  s32 *camera;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];

  // Set up: half-and-half blending, the max and additive pair, the flashes
  // with their palette, the streaks, the dots, and in mode 1 the lightning
  // palette.
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  REG_BG2CNT = 0x784;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             work->sheet + STREAK_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == 1) {
    const void *lightning =
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWordsEntry;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 1072 instructions differ in tla-en.
    copy((void *)BG_PLTT, lightning, 128);
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    s32 dir;
    s32 alternate;

    // When the first target's animation kind is 2 the formation stands 8
    // pixels further right and lower, with its sparks and flashes.
    alternate = 0;

    if (((struct Actor *)GetBattleObjectSlotFar(effect->actors[0])->object)
            ->animationKind == 2)
      alternate = 1;

    // Mode 1 on the far side mirrors everything sideways, and starts from
    // the caster.
    if (mode == 0)
      dir = 1;
    else if (effect->side == 1)
      dir = -1;
    else
      dir = 1;
    if (mode == 1) {
      EffectPosition_ProjectScaledBattler(effect->actor, &origin);
      origin.x /= 2;
      origin.y = 66;
      if (effect->side == 1)
        origin.x = 76;
      else
        origin.x = 44;
    }

    // The formation enters from above the right edge.
    if (alternate == 1) {
      focus.x = 220 << 16;
      focus.y = -(52 << 16);
    } else {
      focus.x = 212 << 16;
      focus.y = -(60 << 16);
    }
    for (i = 0; i != 64; i++)
      work->particles[i].variant = -1;
    for (i = 0; i != 16; i++) {
      struct EffectStep *streak = &work->particles[24 + i];

      if (dir == 1)
        streak->x = (Random16() & 127) + 128;
      else
        streak->x = (Random16() & 127) - 128;
      streak->y = (Random16() & 7) - 72;
      streak->variant = -(Random16() & 31); // frames before it falls
    }
    for (i = 0; i != 512; i++)
      PARTICLES[i].variant = -1;
    if (mode == 0) {
      BattleEffect_SelectLivingTargets(work->effect);
      wait(1);
      BattleEffect_SpawnObjects(8, FORMATION_RESOURCE, 2);
    }

    for (frame = 0; frame != 208; frame++) {
      if ((gInput.repeat & 3) && frame > 48 && frame <= 159) {
        if (mode == 0) {
          Animation_ApplyChildArgumentFar(work->objects[0], 8);
          Animation_ApplyChildArgumentFar(work->objects[1], 9);
          Animation_ApplyChildArgumentFar(work->objects[3], 10);
          Animation_ApplyChildArgumentFar(work->objects[4], 11);
        }
        for (i = 0; i != effect->count; i++) {
          BattleEffect_TintBattler(effect->actors[i], 10, 5, -1, 0);
          BattleMotion_ApplyKnockbackFar(effect->actors[i], 4);
        }
        frame = 160;
      }
      Transform_Reset();
      Transform_SetLookAt(camera, camera + 3);
      if (frame == 178)
        BattlePlayback_StartFar(134);

      // From frame 128 the trails halve; from 160 to 175 the canvas is filled
      // and the palette brightens; from 176 the canvas fades by 2 a frame.
      if (frame == 128)
        work->transferValue = 50;
      if (frame == 176) {
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        Iwram_CopyWordsEntry(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet), 128);
      } else if (frame >= 160 && frame <= 175) {
        work->transferMode = 1;
        work->transferValue = 0x10101010;
        if (frame > 173)
          work->transferValue = 0x3f3f3f3f;
        Palette_BrightenBgEntries(2, 2, 2);
      }

      // Grains rise from y = 112: one a frame from 33, eight from 104.
      if (frame >= 33 && frame <= 175) {
        s32 spawned = 0;
        s32 limit = 1;

        if (frame > 103)
          limit = 8;
        for (i = 0; i != 512; i++) {
          struct EffectStep *grain = &PARTICLES[i];

          if (grain->variant == -1) {
            grain->x = ((Random16() & 255) - 32) << 16;
            grain->y = 112 << 16;
            grain->velocityX = ((Random16() & 127) + gTiamat_DropSpeeds[i & 3])
                               << 9;
            grain->velocityY = -((Random16() & 127) + gTiamat_DropSpeeds[i & 3])
                               << 11;
            grain->variant = 0;
            spawned++;
            if (spawned == limit)
              break;
          }
        }
      }

      // Sparks fly up and out from the formation, one every other frame.
      if (frame >= 41 && frame <= 127) {
        s32 spawned = 0;

        if (frame & 1) {
          for (i = 0; i != 24; i++) {
            struct EffectStep *spark = &work->particles[40 + i];

            if (spark->variant == -1) {
              s32 speed = (Random16() & 255) + 128;
              s32 angle = (Random16() & 0x1fff) + 20000;

              if (mode == 0) {
                if (alternate == 1) {
                  spark->x = ((Random16() & 7) + 78) << 16;
                  spark->y = 78 << 16;
                } else {
                  spark->x = ((Random16() & 7) + 78) << 16;
                  spark->y = 70 << 16;
                }
              } else {
                spark->x = ((Random16() & 7) + origin.x - 8) << 16;
                spark->y = origin.y << 16;
              }
              spark->velocityX = speed * Trig_Sin(angle) >> 9;
              spark->velocityY = speed * Trig_Cos(angle) >> 9;
              spark->variant = 0;
              spawned++;
              if (spawned == 1)
                break;
            }
          }
        }
      }
      if (frame == 48)
        Sound_PlayFar(141);
      if (frame == 128)
        Sound_PlayFar(145);

      // Flashes fly out sideways, one a frame from 129.
      if (frame >= 129 && frame <= 175) {
        s32 spawned = 0;

        for (i = 0; i != 24; i++) {
          struct EffectStep *flash = &work->particles[i];

          if (flash->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) - 20000;

            if (mode == 0) {
              if (alternate == 1) {
                flash->x = 72 << 16;
                flash->y = 72 << 16;
              } else {
                flash->x = 68 << 16;
                flash->y = 64 << 16;
              }
            } else {
              flash->x = origin.x << 16;
              flash->y = origin.y << 16;
            }
            flash->velocityX = speed * Trig_Sin(angle) >> 6;
            flash->velocityY = speed * Trig_Cos(angle) >> 6;
            flash->variant = 0;
            spawned++;
            if (spawned == 1)
              break;
          }
        }
      }

      // A flash steps its picture every four frames and lives 24.
      if (frame <= 175) {
        for (i = 0; i != 24; i++) {
          struct EffectStep *flash = &work->particles[i];

          if (flash->variant >= 0) {
            draw[0](canvas, work->sheet + ((flash->variant >> 2) << 11),
                    HI(flash->x) - 16, HI(flash->y) - 32, 32, 64);
            flash->x += flash->velocityX * dir;
            flash->y += flash->velocityY;
            flash->variant++;
            if (flash->variant == 24)
              flash->variant = -1;
          }
        }
      }

      // A spark is two pixels, speeding upward, and lives 48 frames.
      for (i = 0; i != 24; i++) {
        struct EffectStep *spark = &work->particles[40 + i];
        s32 size = 1;

        if (spark->variant >= 0) {
          draw[1](canvas, Data_0819796c, HI(spark->x), HI(spark->y) - size,
                  size, size * 2);
          spark->x += spark->velocityX * dir;
          spark->y += spark->velocityY;
          spark->velocityY += -0x400;
          spark->variant++;
          if (spark->variant == 48)
            spark->variant = -1;
        }
      }

      // Grains speed upward until frame 128, then bend away sideways (the odd
      // ones harder); each lives 256 frames.
      if (frame <= 175) {
        for (i = 0; i != 512; i++) {
          struct EffectStep *grain = &PARTICLES[i];

          if (grain->variant >= 0) {
            u8 size = gTiamat_DropSizes[i & 3];
            s32 height = size * 2;

            draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                    HI(grain->x) - size / 2, HI(grain->y) - size, size, height);
            grain->x += grain->velocityX * dir;
            grain->y += grain->velocityY;
            if (frame > 128) {
              if (i & 1)
                grain->velocityX += -0x8000;
              else
                grain->velocityX += -0x2000;
            } else {
              grain->velocityY += -0x400;
            }
            grain->variant++;
            if (grain->variant == 256)
              grain->variant = -1;
          }
        }
      }

      // The shakes, and the formation: it drops in over frames 40 to 47 and
      // turns to face the targets from 128 to 175.
      if (frame == 128)
        work->shakeFrames = 48;
      if (mode == 0 && frame == 48)
        work->shakeFrames = 8;
      if (frame >= 40 && frame <= 47) {
        focus.x += -(8 << 16);
        focus.y += 16 << 16;
      }
      if (mode == 0) {
        if (frame == 128) {
          Animation_ApplyChildArgumentFar(work->objects[0], 8);
          Animation_ApplyChildArgumentFar(work->objects[1], 9);
          Animation_ApplyChildArgumentFar(work->objects[3], 10);
          Animation_ApplyChildArgumentFar(work->objects[4], 11);
        }
        if (frame == 176) {
          Animation_ApplyChildArgumentFar(work->objects[0], 0);
          Animation_ApplyChildArgumentFar(work->objects[1], 1);
          Animation_ApplyChildArgumentFar(work->objects[3], 3);
          Animation_ApplyChildArgumentFar(work->objects[4], 4);
        }
        if (alternate == 1)
          BattleEffect_PlaceFormationObjects(6, focus.x, focus.y);
        else
          BattleEffect_PlaceFormationObjects(3, focus.x, focus.y);
      }
      if (frame == 138) {
        for (i = 0; i != effect->count; i++) {
          BattleEffect_TintBattler(effect->actors[i], 10, 5, -1, 0);
          BattleMotion_ApplyKnockbackFar(effect->actors[i], 4);
        }
      }

      // A streak falls six pixels a frame after its wait, then below y = 55
      // splashes through six pictures, two frames each, over and over.
      if (frame <= 175) {
        for (i = 0; i != 16; i++) {
          struct EffectStep *streak = &work->particles[24 + i];

          if (streak->y > 55) {
            if (streak->variant >= 0 && streak->variant <= 11) {
              s32 cel = streak->variant / 2;

              draw[0](canvas, work->sheet + Data_08197984[cel] + STREAK_SHEET,
                      streak->x - Data_08197972[cel] / 2,
                      streak->y + gTiamat_StreakDrops[cel], Data_08197972[cel],
                      Data_08197978[cel]);
            }
            streak->variant++;
            if (streak->variant == 12)
              streak->variant = 0;
          } else if (streak->variant == 0) {
            s32 size = 10;

            streak->x -= dir * 6;
            streak->y += 6;
            if (frame <= 47 && streak->y > 55)
              Sound_PlayFar(136);
            draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                    streak->x - size / 2, streak->y + 30, size, size * 2);
          } else {
            streak->variant++;
          }
        }
      }
      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }

    // Tear down: the formation bursts where it stands and is released.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    if (mode == 0) {
      if (alternate == 1)
        BattleEffect_RunImpactBurst(6, focus.x, focus.y);
      else
        BattleEffect_RunImpactBurst(3, focus.x, focus.y);
    }
    if (mode == 0) {
      for (i = 0; i != 8; i++)
        Sprite_DestroyFar(work->objects[i]);
    }
  }
  BattleEffect_EndCanvasLayer();
}
