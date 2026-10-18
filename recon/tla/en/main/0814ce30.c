// Near miss, differ 150 of 1490 (summon 5, the thorn vines), with one
// tagged forced temporary (355 without).

// ☀️ twin: BattleFx_RunCybele
// (games/tbs/src/effect/effect.c), shape 0.870, structure 0.986, ported line
// for line: formation 645 spawned first, transparent blitters
// BattleFx_LoadBlitter(104, 0x23) and (188, 0x27) and then max blitters (104,
// 0x13) and (188, 0x17) for the explosion, Iwram_ClearWords when A or B
// skips, the camera from Ram_HeapSlots->cameraWork, shake (4, 4) in the
// middle phase where ☀️ has (2, 2), and the effect argument for every
// target loop.

// Same frame and instruction count. What is left: register choice and
// spill slots in the explosion block (frame > 301), and neighbouring rows
// in the other order elsewhere, with the pool offsets that follow them.

// Tried (differ):
// trial 2's port, no device (effects trial, 2026-10-15)              363
// trial 2 with the forced temporary at the second explosion blitter   158
// BattleFx_LoadBlitter declared returning s32 (plan 0c): with the temporary
// 150, without it 355

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "input.h"
#include "iwram_call.h"
#include "motobj.h"
#include "transform.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define CALLBACK_ORDER 0xc80
#define FORMATION_THORN_VINES 645 // eight objects

// The camera work, as far as this scene reads it.
struct EffectCamera {
  s32 eye[3];
  s32 pos[3];
};

extern char ResourceId_ThornVineSheet;
extern char ResourceId_YellowRingSheet;
extern char ResourceId_BlastSheet;
extern u8 Data_081981fc[];  // ☀️ gCybele_ObjectPoses
extern u16 Data_08198202[]; // ☀️ gCybele_ThornOffsets
extern u8 Data_0819820c[];  // ☀️ gCybele_ThornWidths
extern u8 Data_08198211[];  // ☀️ gCybele_ThornHeights
extern u16 Data_08198216[]; // ☀️ gCybele_LeafOffsets
extern u8 Data_0819821c[];  // ☀️ gCybele_LeafSizes
extern u16 Data_08198220[]; // ☀️ the second gCybele_ThornOffsets
extern u8 Data_0819822a[];  // ☀️ the second gCybele_ThornWidths
extern u8 Data_0819822f[];  // ☀️ the second gCybele_ThornHeights
extern u16 Data_08198234[]; // ☀️ gCybele_SparkOffsets
extern u8 Data_0819823e[];  // ☀️ gCybele_SparkSizes
extern u16 Data_08198244[]; // ☀️ gCybele_EmberOffsets
extern u16 Data_08198252[]; // ☀️ gCybele_EmberSizes

void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_ProcessPendingGraphicsTransfer(void);               // ☀️ ProcessPendingGraphicsTransfer
void BattleFx_EndCanvasLayer(void);               // ☀️ BattleFx_EndCanvasLayer
s32 BattleFx_LoadBlitter(s32 slot, s32 flags); // ☀️ BattleFx_LoadWork
void BattleFx_SelectLivingTargets(
    struct BattleEffectArgument *effect); // ☀️ SelectLivingTargets
void BattleFx_SpawnObjects(s32 count, s32 formation, s32 priority); // ☀️ SpawnObjects
void Func_0816442c(s32 formation, s32 x, s32 y); // ☀️ PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x, s32 y); // ☀️ RunImpactBurst
void Func_08118088(s32 actor, s32 motion);       // ☀️ ApplyVariantMotionFar
void Audio_PlayCue(s32 cue);
void Animation_ApplyChildArgumentFar(void *, s32);
void *GetMotionRecordFar(void *object, s32 index);
void Render_ResetTransformState(void);
void ResourceObject_ReleaseFar(void *object);
void Func_081180e8(s32 phase); // ☀️ BattleEventRuntime_BeginPhaseFar
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ ObjectGroup_UpdateMembers
void Func_08158ce0(s32 x, s32 y);    // ☀️ Camera_ApplyShake
void ObjectGroup_TickMemberTimers(void);            // ☀️ ObjectGroup_TickMemberTimers

// Summon 5, the thorn vines: thorns fly in, vines grow up under the targets
// and lift them while the ground throws up debris; the picture is erased
// dot by dot, the targets are dropped from on high, and each lands in an
// explosion with a ring of embers. A or B skips to the drop.
void BattleFx_RunCybele(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  u8 *canvas;
  s32 frame;
  s32 height;
  s32 speed;
  s32 phase;
  s32 rise;
  s32 i;
  s32 j;
  u8 *order;
  s32 point[3];
  struct EffectPosition screen;
  DrawRectangle draw[2];

  // Set up: the formation's eight objects, then the order in which the
  // picture is erased: eight shuffled runs of 0..127.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleFx_SpawnObjects(8, FORMATION_THORN_VINES, 1);

  order = Ram_MapCellBuffer;
  for (i = 0; i != 1024; i++)
    order[i] = i & 127;
  for (i = 0; i != 8; i++) {
    for (j = 0; j != 128; j++) {
      s32 a = Random16() & 127;
      s32 b = Random16() & 127;
      u8 *to = &order[i * 128 + b];
      u8 *from = &order[i * 128 + a];
      u8 t = *to;

      *to = *from;
      *from = t;
    }
  }

  // The canvas, unscaled and moved 32 pixels left, the vine sheet and two
  // transparent blitters, the second mirrored.
  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_ThornVineSheet, work->sheet, 1,
                             1);
  BattleFx_LoadBlitter(HEAP_BLITTER,
                BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                    COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  work->transferMode = 1;
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);
  REG_BG2X = -0x2000;

  // The vines (particles 0-15): x, the root's y, growth speed and delay.
  for (i = 0; i != 16; i++) {
    struct EffectStep *vine = &work->particles[i];

    if (i <= 4) {
      vine->x = i * 20;
      vine->y = (Random16() & 7) + 104;
    } else {
      vine->x = i * 20 - 90;
      vine->y = (Random16() & 7) + 108;
    }
    vine->velocityY = (Random16() & 7) + 4;
    vine->variant = (Random16() & 15) + 16;
  }
  // The glints (16-31): a negative variant is the wait before each shows.
  for (i = 0; i != 16; i++) {
    struct EffectStep *spark = &work->particles[16 + i];

    spark->x = (Random16() & 63) + 32;
    spark->y = (Random16() & 31) + 64;
    spark->variant = -(Random16() & 7) - 8;
  }
  // The thorns (32-47) fly in from the upper right.
  for (i = 0; i != 16; i++) {
    struct EffectStep *thorn = &work->particles[32 + i];

    thorn->x = 128 << 16;
    thorn->y = 64 << 16;
    thorn->velocityX = -((Random16() & 255) + 200) << 9;
    thorn->velocityY = 0;
    thorn->variant = 0;
  }
  BattleFx_SelectLivingTargets(work->effect);

  height = -(64 << 16);
  speed = 0;
  for (frame = 0; frame != 366; frame++) {
    struct EffectCamera *camera = Ram_HeapSlots->cameraWork;

    // A or B skips the vines to the drop.
    if ((gInput.repeat & 3) != 0 && frame > 190 && frame <= 285) {
      Iwram_ClearWords(canvas, 0x4000);
      frame = 286;
    }
    if (frame == 224)
      work->transferMode = 0;
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame == 31) {
      work->shakeFrames = 8;
      Audio_PlayCue(157);
      for (i = 0; i != effect->count; i++)
        Func_08118088(effect->actors[i], 6);
    }
    if (frame == 72)
      Audio_PlayCue(136);
    if (frame == 140)
      Audio_PlayCue(156);

    // Drop the formation's objects to 64 pixels up.
    speed += 0x4000;
    height += speed;
    if (height > 64 << 16)
      height = 64 << 16;
    Func_0816442c(2, 128 << 16, height);
    if (frame >= 48 && frame <= 96) {
      s32 pose = (frame - 48) / 24 % 3;

      Animation_ApplyChildArgumentFar(work->objects[3],
                                      Data_081981fc[pose * 2]);
      Animation_ApplyChildArgumentFar(work->objects[4],
                                      Data_081981fc[pose * 2 + 1]);
    }

    // The thorns fly in, one a frame.
    if (frame >= 72 && frame <= 127) {
      for (i = 0; i != 16; i++) {
        struct EffectStep *thorn = &work->particles[32 + i];

        if (frame >= i + 72 && thorn->y < (104 << 16)) {
          s32 cel = (frame + i) / 4 % 5;
          s32 x = HI(thorn->x);
          s32 y = thorn->y >> 16;
          u8 *cell = work->sheet + Data_08198202[cel];
          u32 width = Data_0819820c[cel];
          u32 tall;

          x -= width >> 1;
          tall = Data_08198211[cel];
          y -= tall >> 1;
          draw[0](canvas, cell, x, y, width, tall);
          BattleFxKernels_IntegrateVector2(thorn, 64, 0x1000);
        }
      }
    }
    // Debris (16-63) bursts up from the ground.
    if (frame == 128) {
      for (i = 0; i != 48; i++) {
        struct EffectStep *chip = &work->particles[16 + i];

        chip->x = (Random16() % 96) << 16;
        chip->y = ((Random16() & 7) + 88) << 16;
        chip->velocityX = ((Random16() & 255) - 128) << 11;
        chip->velocityY = -(Random16() & 255) << 11;
        chip->variant = -(Random16() & 15) - 16;
      }
      REG_BG2X = 0;
    }

    // The vines grow (shaking the screen while they do): a tip (24x8),
    // three 24x64 stems cut at the top of the screen and at the root, and
    // leaves on the odd vines.
    phase = frame - 128;
    if (phase >= 0 && phase <= 96) {
      rise = phase;
      if (rise > 80)
        rise = 80;
      else
        work->shakeFrames = 2;
      for (i = 0; i != 10; i++) {
        struct EffectStep *vine = &work->particles[i];

        if (rise > vine->variant) {
          u8 *sheet = work->sheet;
          s32 top;
          s32 wrap;
          s32 k;

          if (i > 5)
            sheet = work->sheet + 0x6c0;
          top = (rise - vine->variant) * vine->velocityY;
          for (wrap = top; wrap > 184; wrap -= 64)
            ;
          if (wrap <= 119)
            draw[i & 1](canvas, sheet, vine->x, vine->y - wrap - 8, 24, 8);
          for (k = 0; k != 3; k++) {
            s32 y = vine->y - wrap + k * 64;
            s32 skip = 0;
            s32 rows = 64;

            if (y >= -64) {
              if (y < 0) {
                skip = -y * 24;
                rows = y + 64;
                y = 0;
              }
              if (y + rows > vine->y)
                rows -= y + rows - vine->y;
              draw[i & 1](canvas, sheet + skip + 192, vine->x, y, 24, rows);
            }
          }
          if ((i & 1) != 0) {
            s32 y = ((vine->y - top) & 127) - 16;
            s32 kind = i % 3;
            s32 full = Data_0819821c[kind];
            s32 rows = full;

            if (y + rows > vine->y)
              rows -= y + rows - vine->y;
            if (rows > 0)
              draw[i & 1](canvas, work->sheet + Data_08198216[kind],
                          vine->x + 8, y, full, rows);
          }
        }
      }
    }
    // Debris that falls or leaves the screen bursts up again until frame
    // 159.
    if (phase >= 0 && phase <= 95) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *chip = &work->particles[32 + i];

        if (chip->variant >= 0) {
          s32 cel = i % 5;

          draw[0](canvas, work->sheet + Data_08198220[cel], HI(chip->x),
                  HI(chip->y), Data_0819822a[cel], Data_0819822f[cel]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->velocityY += 0x4000;
          if ((chip->y < 0 || chip->y > 120 << 16) && frame <= 159) {
            chip->x = (Random16() % 96) << 16;
            chip->y = ((Random16() & 7) + 88) << 16;
            chip->velocityX = ((Random16() & 255) - 128) << 11;
            chip->velocityY = -(Random16() & 255) << 11;
          }
        }
        chip->variant++;
      }
    }

    // Every four frames, step the 64 BG colours one shade toward grey.
    if (frame >= 224 && frame <= 247 && (frame & 3) == 0) {
      u16 *color = (u16 *)0x05000000;

      for (i = 0; i != 64; i++) {
        s32 red = *color & 31;
        s32 green = (*color >> 5) & 31;
        s32 blue = (*color >> 10) & 31;
        s32 grey = (red + green + blue) / 3;

        if (red > grey)
          red--;
        if (red < grey)
          red++;
        if (green > grey)
          green--;
        if (green < grey)
          green++;
        if (blue > grey)
          blue--;
        if (blue < grey)
          blue++;
        *color++ = (blue << 10) | (green << 5) | red;
      }
    }

    // Six glints flash in turn around each target.
    if (phase >= 0 && phase <= 172) {
      for (i = 0; i != effect->count; i++) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(effect->actors[i])->object;

        point[0] = object->x;
        point[1] = object->y;
        point[2] = object->z;
        EffectPosition_ApplyBaseAndYOffset(point, &screen);
        for (j = 0; j != 6; j++) {
          struct EffectStep *spark = &work->particles[16 + i * 6 + j];

          if (spark->variant == 0) {
            spark->x = screen.x + (Random16() & 15) - 8;
            spark->y = screen.y + (Random16() & 15) - 40;
          }
          if (spark->variant >= 0 && spark->variant <= 4) {
            u8 *cell = work->sheet + Data_08198234[spark->variant];
            u32 size = Data_0819823e[spark->variant];
            u32 half = size >> 1;

            draw[0](canvas, cell, spark->x - half, spark->y - half, size, size);
          }
          spark->variant++;
          if (frame <= 199 && spark->variant == 5)
            spark->variant = -(Random16() & 7);
        }
      }
    }

    // Erase the 128x128 picture two lines a frame, each line's dots in the
    // shuffled order.
    if (frame > 232) {
      s32 row = frame * 2 - 496;

      for (i = 0; i != 32; i++) {
        for (j = 0; j != 4; j++) {
          s32 y = row + i;

          if (y >= 0 && y <= 127) {
            s32 x = order[((y & 7) * 32 + i) * 4 + j];

            canvas[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
          }
        }
      }
      row++;
      for (i = 0; i != 32; i++) {
        for (j = 0; j != 4; j++) {
          s32 y = row + i;

          if (y >= 0 && y <= 127) {
            s32 x = order[((y & 7) * 32 + i) * 4 + j];

            canvas[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
          }
        }
      }
    }

    // Lift the targets to 128 pixels, eight frames apart.
    if (frame >= 161 && frame <= 223) {
      for (i = 0; i != effect->count; i++) {
        if (frame > i * 8 + 160) {
          struct BattleObj *slot = GetBattleObjectSlotFar(effect->actors[i]);
          struct MotionObject *object = slot->object;
          void *record;
          s32 n;

          object->y += 8 << 16;
          if (object->y > 128 << 16)
            object->y = 128 << 16;
          object->verticalMotionStrength = 0;
          for (n = 0; (record = GetMotionRecordFar(slot->object, n)) != 0; n++)
            Animation_ApplyChildArgumentFar(record, 5);
        }
      }
    }
    // Drop the targets from 96 pixels, five frames apart, and strike each
    // 16 frames later.
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 5 + 286) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(effect->actors[i])->object;

        object->y = 96 << 16;
        object->verticalMotionStrength = 0xab85;
      }
      if (frame == i * 5 + 302) {
        Func_0814cd48(effect->actors[i], 7, -1, i, 8);
        Audio_PlayCue(134);
        work->shakeFrames = 8;
      }
    }

    // Switch to the explosion: the blast sheet, max blitters, and ten
    // embers 72 degrees apart around each target.
    if (frame == 302) {
      Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
      Runtime_ReleaseHeapBlock(HEAP_BLITTER);
      Resource_LoadAndDecompress((s32)&ResourceId_YellowRingSheet, work->sheet,
                                 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                                 work->sheet + 0x1680, 1, 1);
      BattleFx_LoadBlitter(HEAP_BLITTER,
                    BLITTER_MAX << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      /* FAKEMATCH: forced temporary: the blitter slot's address held across
       * the call moves the allocation of the whole explosion block toward the
       * native's (differ 355 without, 150 with); found by alchemy permute.
       * Plain forms measured: the slot read after the call (355). */
      {
        void **slots = Ram_HeapSlots->unkb8;

        BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                      BLITTER_MAX << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                          COMPOSITE_FLIP_X);
        draw[1] = (DrawRectangle)slots[1];
      }
      REG_BLDCNT = 0x3f46;
      REG_BG2PA = 0x80;
      REG_BG2X = 0;
      work->transferMode = 2;
      work->transferValue = 75;
      Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);
      for (i = 0; i != effect->count; i++) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(effect->actors[i])->object;

        for (j = 0; j != 10; j++) {
          struct EffectStep *ember = &work->particles[i * 10 + j];

          ember->x = object->x;
          ember->y = 20 << 16;
          ember->z = object->z;
          ember->velocityX = Trig_Sin(j * 0x3334) << 2;
          ember->velocityY = (Random16() & 0x7fff) + (1 << 16);
          ember->velocityZ = Trig_Cos(j * 0x3334) << 2;
          ember->variant = 0;
        }
      }
    }
    // The explosions (40x24 in two mirrored halves, six cels) and the five
    // embers after them.
    if (frame > 301) {
      for (i = 0; i != effect->count; i++) {
        s32 start = i * 4 + 302;

        if (frame >= start && frame < i * 4 + 314) {
          s32 cel = (frame - start) / 2;
          struct MotionObject *object =
              GetBattleObjectSlotFar(effect->actors[i])->object;

          point[0] = object->x;
          point[1] = 0;
          point[2] = object->z;
          EffectPosition_ApplyBaseAndYOffset(point, &screen);
          screen.x >>= 1;
          draw[0](canvas, work->sheet + cel * 480, screen.x - 20, screen.y - 24,
                  20, 24);
          draw[1](canvas, work->sheet + cel * 480, screen.x, screen.y - 24, 20,
                  24);
        }
        if (frame >= start + 6) {
          for (j = 0; j != 5; j++) {
            struct EffectStep *ember = &work->particles[i * 10 + j];

            EffectPosition_ApplyBaseAndYOffset(&ember->x, &screen);
            screen.x >>= 1;
            if (ember->variant >= 0 && ember->variant <= 26) {
              s32 cel = 6;
              s32 offset = Data_08198244[cel];
              u32 size = Data_08198252[cel];

              draw[0](canvas, work->sheet + offset, screen.x - (size >> 1),
                      screen.y - (size >> 1), size, size);
            }
            BattleFxKernels_IntegrateVector2(ember, 60, 0x1000);
            ember->variant++;
          }
        }
      }
    }

    if (frame <= 127)
      Func_08158ce0(4, 16);
    else if (frame <= 301)
      Func_08158ce0(4, 4);
    else
      Func_08158ce0(4, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  Func_081180e8(134);
  // Tear down.
  Func_0816467c(2, 128 << 16, height);
  for (i = 0; i != 8; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  BattleFx_EndCanvasLayer();
}
