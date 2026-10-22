// Draft, differ 774 of 1493 (the draft has 1507).

// Summon 8 (BattleEffect_RunSceneEffect's case 8). Twin: The Broken Seal's
// BattleEffect_RunProcne, ported by trial 2 and rewritten here. The
// Lost Age's changes: BattleEffect_FetchRectangleBlitters for the pair; the
// whirlwind unpacked with its palette by Resource_LoadAndDecompress into the
// work sheet, the comets into work->sheet + 0x3600; frame / 4 % 3 written
// through Math_UnsignedMulHighAdd(x, 0x55555556, -3) and the depth divisions by
// 36 and 90 through Math_UnsignedMulHigh with their reciprocals (calls the
// source makes: agscc emits __modsi3 and __divsi3 for the operators); HI()
// reads of screen.y and screen.depth; Iwram_TransformVector at the comet
// sparks; formation resource 646; the effect argument for the actor loops;
// gCameraSceneParameters declared unsized (with the shared u32[3], word 3
// folds into the address).

// Left: allocation. The frame is 372 bytes for the native's 364: alive and
// slot are spilled where the native keeps them in r10 and r11, and index *
// 2 takes a register where the native spills it; 14 more instructions
// follow from that, most in the comet strikes (the native keeps the mark's
// x, wave.x >> 17 + 32, and the ring's angle in stack slots).

// Tried: trial 2's plain port 854, its declaration order from a greedy scan
// 774 (kept, with the names changed); alchemy permute 6 minutes reached 552
// only with a temporary for HI(screen.y) (not kept). 2026-10-15: the effect
// state in a block opened after the setup, as in summon 7 (whose slots it
// fixed), 854, and 808 after a declaration scan of that form.

#include "battle.h"
#include "graphics.h"
#include "lib.h"
#include "resource.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "input.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "transform.h"

// The comet sparks live in the map buffer, 128 for each of four comets.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

// The high halfword of a 16.16 value: its whole pixels.
#define HI(v) (((s16 *)&(v))[1])

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_BeginCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);      // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void);      // ..ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void);      // ObjectGroup_TickMemberTimers
void Func_081180e8(s32 phase); // BattleEventRuntime_BeginPhaseFar
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames);                    // ObjectGroup_UpdateMembers
void BattleEffect_FetchRectangleBlitters(
    s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // ..SelectLivingTargets
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 priority); // ..SpawnObjects
void Func_0814cca8(void);      // BattleEffect_SetupBlendedDisplay
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPosition(s32 *position);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 frames);
void BattlePresentation_ConfigurePaletteFadeFar(s32 layer, s32 background,
                                                s32 frames);
s32 ArcTan2(s32 x, s32 y);
u32 Math_UnsignedMulHigh(u32 value, u32 magic);
s32 Math_UnsignedMulHighAdd(s32 value, u32 magic, s32 multiplier);
void Audio_PlayCue(s32 cue);

struct Point2D {
  s32 x;
  s32 y;
};

struct Scale {
  s32 x;
  s32 y;
};

extern char ResourceId_WhirlwindSheet;
extern char ResourceId_Data134;
extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_VioletCometSheet;
extern u32 gCameraSceneParameters[];
extern struct Scale Data_08196df8; // the comets' unit scale
extern u8 Data_0819793e[];         // the nine comets' grid: x
extern u8 Data_08197947[];         // ... and depth
extern u16 Data_08197950[];        // the four comet strikes' frames
extern u8 Data_08197958[];         // a strike's three pictures: widths
extern u8 Data_0819795b[];         // ... heights
extern u8 Data_0819795e[];         // ... raise above the mark
extern u16 Data_08197962[];        // ... cell offsets
extern u16 gData_08197410[];       // dot sizes 1 to 10: cell offsets

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
  void *camera;
  struct BattleEffectWork *work;
  void *canvas;
  struct Point2D wave;
  DrawRectangle draw[2];
  s32 frame;
  u8 *graphics;
  struct Point2D drift;
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
  BattleEffect_BeginCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_WhirlwindSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
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

    Render_ResetTransformState();
    Transform_SetLookAt(camera, (s32 *)((u8 *)camera + 12));

    if (frame == 16)
      Audio_PlayCue(141);
    if (frame == 256)
      Audio_PlayCue(140);
    if (frame == 334)
      Audio_PlayCue(212);
    if (frame == 347)
      Audio_PlayCue(212);
    if (frame == 359)
      Audio_PlayCue(212);
    if (frame == 372)
      Audio_PlayCue(212);

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
          if (frame == Data_08197950[slot]) {
            object->velocityY = 0;
            Func_0814cd48(effect->actors[index], 7, 5, index, 8);
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
      draw[0](canvas,
              &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL + (48 - size) * 48],
              32, 112 - size, 48, size);
    }
    if ((u32)(frame - 48) <= 111) {
      size = frame * 2 - 96;
      if (size > 64)
        size = 64;
      draw[0](canvas,
              &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL + (64 - size) * 48],
              32, 64 - size, 48, size);
    }
    if ((u32)(frame - 160) <= 239) {
      draw[0](canvas, &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL], 8, 0, 48,
              64);
      draw[0](canvas, &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL], 8, 64, 48,
              64);
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
      BattlePresentation_ConfigurePaletteFadeFar(
          1, (s32)&ResourceId_VioletSkyBackdrop, 8);
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
        place[0] = (Data_0819793e[index] << 16) + wave.x + 0x280000;
        place[1] = 0xff0000 + index * 0x400000;
        place[2] = ((Data_08197947[index] + index * 64) << 16) + wave.y;
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
        Render_ResetTransformState();
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
              screen.y = HI(screen.y) + (wave.y >> 16) - 4;
              screen.depth = HI(screen.depth);
              if (screen.depth <= 169)
                screen.depth = 170;
              if (screen.depth > 350)
                screen.depth = 350;
              span = 6 - Math_UnsignedMulHigh(screen.depth - 170, 0x071c71c8);
              draw[1](canvas, graphics + gData_08197410[span - 1],
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
            draw[1](canvas, graphics + gData_08197410[span - 1],
                    (wave.x >> 17) - span / 2 + 32, (wave.y >> 16) - span - 4,
                    span, span * 2);
          }
        }

        // Each strike throws out 96 sparks that fly and fade.
        slot = 0;
        do {
          if (frame == Data_08197950[slot]) {
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

          if (frame >= Data_08197950[0]) {
            spark = PARTICLES;
            index = 0;
            do {
              if (spark[index].variant > 0) {
                Iwram_TransformVector(&spark[index].x, &screen.x);
                screen.x = (screen.x >> 17) + 32;
                screen.y = HI(screen.y) + 56;
                screen.depth = HI(screen.depth);
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                draw[0](canvas, graphics + gData_08197410[span - 1],
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
            draw[1](canvas, work->sheet + Data_08197962[cel], mark->x,
                    mark->y - Data_0819795e[cel], Data_08197958[cel],
                    Data_0819795b[cel]);
            mark->x -= 8;
            mark->y += 2;

            if (frame < threshold + 8) {
              Func_080150e4(-2048);
              SceneTransform_ApplyYaw(-4096);

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
                screen.y = HI(screen.y) + work->particles[index + 60].velocityY;
                screen.depth = HI(screen.depth);
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                draw[0](canvas, graphics + gData_08197410[span - 1],
                        screen.x - span / 2, screen.y - span, span, span * 2);
                slot++;
              } while (slot != 28);
            }
          }
          index++;
        } while (index != 4);
      }
    }

    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
    frame++;
  } while (frame != 400);

  // The results; the enemies go back where they stood, facing as before.
  Func_081180e8(134);
  for (index = 0; index != effect->count; index++) {
    struct MotionObject *enemy;

    context = GetBattleObjectSlotFar(effect->actors[index]);
    enemy = context->object;
    enemy->x = home[index].x;
    enemy->z = home[index].y;
    enemy->angle = baseAngle[index];
  }
  gCameraSceneParameters[3] = 120;
  Func_0814cca8();

  index = 0;
  do {
    ResourceObject_ReleaseFar(work->objects[index]);
    index++;
  } while (index != COMETS);

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}
