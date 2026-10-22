// Near miss, differ 40 of 1072.

// Summon 7, mode 0 (the wrapper in series_e.c; mode 1 is the kind that
// casts the same scene from the caster's side). Twin: The Broken Seal's
// BattleEffect_RunPaletteParticles, ported call for call. The Lost Age's
// changes: every picture unpacked by Resource_LoadAndDecompress; the pair
// from BattleEffect_FetchRectangleBlitters; BG2CNT set with the blending; an
// alternate layout (formation 6, eight pixels right and lower, sparks and
// flashes with it) when the first target's animation kind is 2; formation
// resource 647; two separate mode-0 tests at the teardown; the callback at
// 0xc80.

// Left: order only: about twenty single swaps of a load (the argument's
// spill, a pooled constant, the work pointer) with the constant beside it.

// Tried (2026-10-15): The Broken Seal's layout, every local at the top,
// 102 (the spill slots of dir, the alternate flag and focus come before
// the blitter pair's address); the effect state (focus, dir, alternate) in a
// block opened after the transfer callback, as The Broken Seal's
// EmberColumns does, 40: the native's slot order.

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
#include "object_runtime.h"
#include "ram_buffer.h"

// The rising grains live in the map buffer.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

// The high halfword of a 16.16 value: its whole pixels.
#define HI(v) (((s16 *)&(v))[1])

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);                   // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void);                   // ..ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void);                   // ObjectGroup_TickMemberTimers
void Func_08158ce0(s32 x, s32 y);           // Camera_ApplyShake
void Func_081180e8(s32 phase);              // BattleEventRuntime_StartPhaseFar
void Func_08118088(s32 actor, s32 variant); // ..ApplyVariantMotionFar
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames);                    // ObjectGroup_UpdateMembers
void BattleEffect_FetchRectangleBlitters(
    s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // ..SelectLivingTargets
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 priority); // ..SpawnObjects
void Func_0816442c(s32 formation, s32 x, s32 y);  // ..PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x, s32 y);  // ..RunImpactBurst
void Func_08164a4c(s32 blue, s32 green, s32 red); // Palette_BrightenBgEntries
void Animation_ApplyChildArgumentFar(void *object, s32 animation);
void Audio_PlayCue(s32 cue);

struct Point2D {
  s32 x;
  s32 y;
};

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_Data134;
extern char ResourceId_LightningBoltSheet;
extern u8 Data_08197968[];   // a grain's added speed, by grain
extern u8 Data_0819796c[];   // a spark's two pixels
extern u8 Data_0819796e[];   // a grain's size, by grain
extern u8 Data_08197972[];   // the streak's six pictures: widths
extern u8 Data_08197978[];   // ... heights
extern u8 Data_0819797e[];   // ... drops below the streak
extern u16 Data_08197984[];  // ... cell offsets
extern u16 gData_08197410[]; // dot sizes 1 to 10: cell offsets in the sheet

#define STREAK_SHEET 0x3000 // the ember streaks, after the flashes
#define FORMATION_RESOURCE 647

// Summon 7 (mode 0; mode 1 is the same scene cast from the caster's side,
// without the formation): streaks rain down and splash, sparks and grains
// rise from where the formation stands, a burst of flashes at frame 128,
// then the screen whitens and the canvas fades away. In mode 0 a formation
// of eight drops in at frame 40 and bursts at the end. A or B skips frames
// 49 to 159.
void Func_08147aec(struct BattleEffectArgument *effect, s32 mode) {
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
  if (mode == 1)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    s32 dir;
    s32 alternate;

    // When the first target's animation kind is 2 the formation stands 8
    // pixels further right and lower, with its sparks and flashes.
    alternate = 0;
    if (((struct ObjectRuntime *)GetBattleObjectSlotFar(effect->actors[0])
             ->object)
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
      WaitFrames(1);
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
          Func_0814cd48(effect->actors[i], 10, 5, -1, 0);
          Func_08118088(effect->actors[i], 4);
        }
        frame = 160;
      }
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      if (frame == 178)
        Func_081180e8(134);

      // From frame 128 the trails halve; from 160 to 175 the canvas is filled
      // and the palette brightens; from 176 the canvas fades by 2 a frame.
      if (frame == 128)
        work->transferValue = 50;
      if (frame == 176) {
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        Iwram_CopyWords(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet), 128);
      } else if (frame >= 160 && frame <= 175) {
        work->transferMode = 1;
        work->transferValue = 0x10101010;
        if (frame > 173)
          work->transferValue = 0x3f3f3f3f;
        Func_08164a4c(2, 2, 2);
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
            grain->velocityX = ((Random16() & 127) + Data_08197968[i & 3]) << 9;
            grain->velocityY = -((Random16() & 127) + Data_08197968[i & 3])
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
        Audio_PlayCue(141);
      if (frame == 128)
        Audio_PlayCue(145);

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
            u8 size = Data_0819796e[i & 3];
            s32 height = size * 2;

            draw[0](canvas, sheet + gData_08197410[size - 1],
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
          Func_0816442c(6, focus.x, focus.y);
        else
          Func_0816442c(3, focus.x, focus.y);
      }
      if (frame == 138) {
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 10, 5, -1, 0);
          Func_08118088(effect->actors[i], 4);
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
                      streak->y + Data_0819797e[cel], Data_08197972[cel],
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
              Audio_PlayCue(136);
            draw[0](canvas, sheet + gData_08197410[size - 1],
                    streak->x - size / 2, streak->y + 30, size, size * 2);
          } else {
            streak->variant++;
          }
        }
      }
      Func_08158ce0(8, 8);
      ObjectGroup_TickMemberTimers();
      work->transferPending = 1;
      WaitFrames(1);
    }

    // Tear down: the formation bursts where it stands and is released.
    Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    if (mode == 0) {
      if (alternate == 1)
        Func_0816467c(6, focus.x, focus.y);
      else
        Func_0816467c(3, focus.x, focus.y);
    }
    if (mode == 0) {
      for (i = 0; i != 8; i++)
        ResourceObject_ReleaseFar(work->objects[i]);
    }
  }
  BattleEffect_EndCanvasLayer();
}
