// Draft, differ 1668 of 1864 (20 instructions short).

// Twin: The Broken Seal's BattleEffect_RunTwelveMode, rebuilt by ⚓️ for 21 modes
// (19 handler-table wrappers, kinds 117 and 386-458). The skeleton, the
// record table and the strike, column, wisp, mote, spark and glint parts
// are the twin's; ⚓️ adds:
// - a seventh palette (OrangePaletteB) and two more effect sheets: a flash
// (record flag 32) drawn as 32x64 pictures, and a star (flag 64) drawn as
// a textured 3D model that swells and shrinks over frames 8 to 17;
// - per-mode cues: modes 7, 12 and 13 sound their first hits and only then
// move the battle on, and modes 12 and 13 aim each strike afresh at a
// jittered point;
// - heavier hits (flags 16 and 32) set every spark going, and 256 sparks
// where the twin has 512;
// - the target is measured once for the wisps, not once for each wisp.

// Left: allocation through the whole body. The native keeps the addresses
// of targetPosition, ground, alternate and the blitter pair in spill slots
// (sp+36, +32, +48, +40) and reads the structs through them, and keeps mode
// * 8 in two slots (sp+28, +52) for the record index; here the structs are
// read sp-relative, the frame is 176 bytes for 164, and every spill slot
// moves. The control flow follows the native block for block; the 20
// missing instructions are reloads the native's extra pseudos cost.

// Tried:
// the twin rebuilt from the listing                          1668

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "motobj.h"
#include "battle_display.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "graphics.h"
#include "transform.h"
#include "resource.h"

typedef DrawRectangle BlitterPair[2];

// The blitters' heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]
#define gBlitter ((DrawRectangle)Ram_HeapSlots->unk64[1])
#define gBlitterAlternate ((DrawRectangle)Ram_HeapSlots->unkb8[1])

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define CALLBACK_ORDER 0xc80

// The pixel half of a 16.16 value.
#define HI(value) (((s16 *)&(value))[1])

// Seven bytes for each mode (The Broken Seal's gTargetStrike_Records, grown
// from 12 modes to 21).
extern u8 Data_0819754f[];
#define RECORD(mode, field) Data_0819754f[(mode) * 7 + (field)]
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

// The strike pictures (gTargetStrike_Widths, Heights, Offsets, Reach)
// and the column stages (gTargetStrike_ColumnStages).
extern u8 Data_081975e2[];
extern u8 Data_081975e8[];
extern u16 Data_081975ee[];
extern u8 Data_081975fa[];
extern u8 Data_08197600[];

// The glint pictures (gBattleEffect_GlintCellWidths, Heights, Offsets) and
// their flips (gTargetStrike_GlintDrawFlags).
extern u8 gData_08197492[];
extern u8 gData_08197498[];
extern u16 gData_08197486[];
extern u8 Data_08197605[];

// The mote's cell for each size (gBattleEffect_MoteCells).
extern u16 gData_08197410[];

// The star model and its four projected corners.
extern u8 gData_08199340[];
extern u8 gData_08199210[];

// The motes live in the map cell buffer, the sparks 0x3800 bytes on.
#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)
#define gSparks ((struct EffectStep *)(Ram_MapCellBuffer + 0x3800))

extern char ResourceId_Data134; // The Broken Seal's ParticleSpritesA
extern char ResourceId_TornadoSheet;
extern char ResourceId_BeamSequenceImage;
extern char ResourceId_TwelveModeImage;
extern char ResourceId_BlueBeamSheet;
extern char ResourceId_LimePalette;
extern char ResourceId_PinkPalette;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_StarCrossSheet;
extern char ResourceId_SmokeSheet;

// The star's texture, 64x64 pixels.
struct StarTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
};

// One model for the IWRAM renderer.
struct ModelDraw {
  s32 kind;
  s32 next;
  const void *model;
  void *corners;
  struct StarTexture *texture;
  s32 unk14;
  s32 tail;
};

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
void Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                   s32 mode); // BattleEffect_BuildBlitter
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 order);
s32 Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase);             // BattleEventRuntime_StartPhaseFar
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
u32 Random16(void);
void EffectPosition_ProjectBattler(s32 actor,
                                                 struct EffectPosition *out);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void Func_08158ce0(s32 x, s32 y);    // Camera_ApplyShake
void ObjectGroup_CountDownMemberTimers(void);            // ObjectGroup_CountDownMemberTimers
struct ModelDraw *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *out, s32 count);
void Render_DrawModelList(struct ModelDraw *draw);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 pitch);
void SceneTransform_ApplyYaw(s32 yaw);
void WaitFrames(s32 frames);

// The body of 21 strike and column effects.

// Strikes (kind 0) are six-picture slashes across the first target, one
// every six pictures' time; columns (kind 1) are a pair of mirrored 18x48
// pillars that rise and fall on it. Each hit flashes and shakes the target,
// starts eight wisps of smoke and a burst of sparks, and the battle moves on
// at the first. Around them the mode's flags add rising wisps, motes that
// stream back to the user, glints, sparks, a flash or a star.

void Func_0813d098(struct BattleEffectArgument *effect, s32 mode) {
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
    Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage,
                               work->sheet + 0xc80, 0, 0);
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
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
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
    target = GetBattleObjectSlotFar(effect->actors[k])->object;
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
    // away from the user, column hits every way, strikes away from the
    // middle.
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
          if (gSparks[i].x > 0)
            gSparks[i].velocityX = -gSparks[i].velocityX;
        }
      }
      gSparks[i].variant = -1;
    }
  }

  work->transferMode = 2;
  if (RECORD(mode, RECORD_FLAGS) & FLAG_STAR)
    work->transferValue = 50;
  else
    work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);

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
          EffectPosition_ProjectBattler(effect->actors[0],
                                                      &alternate);
          targetPosition.y += (alternate.y - targetPosition.y) / 2;
          targetPosition.y += (Random16() & 15) - 8;
        }
        if (effect->side == 1) {
          BattleEffect_LoadBlitter(SLOT_BLITTER,
                        BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
          gBlitter(canvas, work->sheet + Data_081975ee[picture],
                   targetPosition.x / 2 - (Data_081975fa[picture] / 2) -
                       Data_081975e2[picture] + 8,
                   targetPosition.y - (Data_081975e8[picture] / 2),
                   Data_081975e2[picture], Data_081975e8[picture]);
        } else {
          BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP));
          gBlitter(canvas, work->sheet + Data_081975ee[picture],
                   targetPosition.x / 2 + (Data_081975fa[picture] / 2) - 8,
                   targetPosition.y - (Data_081975e8[picture] / 2),
                   Data_081975e2[picture], Data_081975e8[picture]);
        }
        Runtime_ReleaseHeapBlock(SLOT_BLITTER);

        // A star sounds four frames before its hit.
        if (frame % (RECORD(mode, RECORD_RATE) * 6) ==
                RECORD(mode, RECORD_RATE) * 4 - 4 &&
            (RECORD(mode, RECORD_FLAGS) & FLAG_STAR))
          Audio_PlayCue(212);

        // The hit, on the strike's fifth picture.
        if (frame % (RECORD(mode, RECORD_RATE) * 6) ==
            RECORD(mode, RECORD_RATE) * 4) {
          if (mode == 12) {
            if (frame / RECORD(mode, RECORD_RATE) / 6 > 2)
              Func_081180e8(133);
            else
              Audio_PlayCue(133);
          } else if (mode == 7) {
            if (frame > 8)
              Func_081180e8(133);
            else
              Audio_PlayCue(133);
          } else if (mode == 13) {
            if (frame / RECORD(mode, RECORD_RATE) / 6 > 6)
              Func_081180e8(134);
            else
              Audio_PlayCue(134);
          } else if ((RECORD(mode, RECORD_FLAGS) & FLAG_STAR) ||
                     (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH)) {
            Func_081180e8(144);
          } else if (mode == 8) {
            Func_081180e8(134);
          } else {
            Func_081180e8(133);
          }

          if (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) {
            work->shakeFrames = 8;
            Func_0814cd48(effect->actors[0], 7, 5, 0, 12);
            Func_08118088(effect->actors[0], 4);
            for (i = 0; i != 32; i++)
              gSparks[i].variant = (Random16() & 15) + 15;
          } else if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY) {
            work->shakeFrames = 8;
            Func_0814cd48(effect->actors[0], 7, 5, 0, 12);
            Func_08118088(effect->actors[0], 4);
            for (i = 0; i != 256; i++)
              gSparks[i].variant = (Random16() & 15) + 15;
          } else {
            work->shakeFrames = 4;
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
            for (i = 0; i != 32; i++) {
              struct EffectStep *step =
                  &gSparks[frame / (RECORD(mode, RECORD_RATE) * 6) * 32 + i];

              step->variant = (Random16() & 15) + 7;
            }
          }
          if (mode == 16) {
            for (i = 0; i != 4; i++)
              work->particles[frame / (RECORD(mode, RECORD_RATE) * 6) * 16 + i]
                  .variant = 0;
          } else {
            for (i = 0; i != 8; i++)
              work->particles[frame / (RECORD(mode, RECORD_RATE) * 6) * 16 + i]
                  .variant = 0;
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
        stage = Data_08197600[picture];
      } else {
        stage = 3;
      }
      Func_08196404(SLOT_BLITTER, 7, 7, CLIP, RECORD(mode, RECORD_BLEND));
      gBlitter(canvas, work->sheet + stage * 864 + 0xc80,
               targetPosition.x / 2 - 18, 56, 18, 48);
      Runtime_ReleaseHeapBlock(SLOT_BLITTER);
      Func_08196404(SLOT_BLITTER, 7, 7, CLIP | COMPOSITE_FLIP_X,
                    RECORD(mode, RECORD_BLEND));
      gBlitter(canvas, work->sheet + stage * 864 + 0xc80, targetPosition.x / 2,
               56, 18, 48);
      Runtime_ReleaseHeapBlock(SLOT_BLITTER);

      // The hit, on each column's fourth picture.
      if (frame % (RECORD(mode, RECORD_RATE) * 4) ==
          RECORD(mode, RECORD_RATE) * 3) {
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        work->shakeFrames = 4;
        if ((RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) ||
            (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY))
          Func_081180e8(144);
        else if (frame > (RECORD(mode, RECORD_COUNT) * 4 - 4) *
                             RECORD(mode, RECORD_RATE))
          Func_081180e8(133);
        else
          Audio_PlayCue(133);
        if (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) {
          work->shakeFrames = 8;
          Func_0814cd48(effect->actors[0], 7, 5, 0, 12);
          Func_08118088(effect->actors[0], 4);
          for (i = 0; i != 32; i++)
            gSparks[i].variant = (Random16() & 15) + 15;
        } else if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY) {
          work->shakeFrames = 8;
          Func_0814cd48(effect->actors[0], 7, 5, 0, 12);
          Func_08118088(effect->actors[0], 4);
          for (i = 0; i != 256; i++)
            gSparks[i].variant = (Random16() & 15) + 15;
        }
        for (i = 0; i != 64; i++) {
          struct EffectStep *step =
              &gSparks[frame / (RECORD(mode, RECORD_RATE) * 6) * 64 + i];

          step->variant = (Random16() & 15) + 7;
        }
        for (i = 0; i != 8; i++)
          work->particles[frame / (RECORD(mode, RECORD_RATE) * 6) * 16 + i]
              .variant = 0;
        for (i = 0; i != 16; i++)
          gMotes[frame / (RECORD(mode, RECORD_RATE) * 6) * 16 + i].variant = 0;
      }
      if (stage == 3)
        glint = 1;
    }

    // Three glints a frame around the target's middle.
    if ((RECORD(mode, RECORD_FLAGS) & FLAG_GLINTS) && glint) {
      s32 image;

      EffectPosition_ProjectScaledBattler(effect->actors[0], &center);
      EffectPosition_ProjectBattler(effect->actors[0],
                                                  &alternate);
      center.y += (alternate.y - center.y) / 2;
      image = frame & 3;
      for (i = 0; i != 3; i++) {
        s32 angle = Random16() & 0xffff;
        s32 radius = Random16() & 31;
        s32 x;
        s32 y;

        x = center.x / 2 + (Trig_Sin(angle) * (radius + 4) >> 17) -
            (gData_08197492[image] / 2);
        y = center.y - (Trig_Cos(angle) * (radius + 4) >> 17) -
            (gData_08197498[image] / 2);
        Func_08196404(SLOT_BLITTER_ALTERNATE, 7, 7,
                      Data_08197605[Random16() & 3] | CLIP, BLIT_ADDITIVE);
        gBlitterAlternate(canvas, work->sheet + gData_08197486[image], x, y,
                          gData_08197492[image], gData_08197498[image]);
        Runtime_ReleaseHeapBlock(SLOT_BLITTER_ALTERNATE);
      }
    }

    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = gBlitter;
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_ADDITIVE, CLIP));
    draw[1] = gBlitterAlternate;
    for (k = 0; k != 1; k++) {
      source = GetBattleObjectSlotFar(effect->actor)->object;

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
          Func_0814cd48(effect->actor, 7, -1, -1, 0);
        if (frame == 90)
          Func_0814cd48(effect->actor, 0, -1, -1, 0);
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &gMotes[i];

          if (step->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)step, &position);
            position.x >>= 1;
            draw[1](canvas, sheet + gData_08197410[size - 1],
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
            draw[1](canvas, sheet + gData_08197410[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
            step->variant--;
          }
        }
      }
    }
    Runtime_ReleaseHeapBlock(SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(SLOT_BLITTER);

    // The star: a textured square, swelling on a sine from frame 8 to 17
    // over the target's middle (the canvas is cleared to colour 0x3f first).
    if (RECORD(mode, RECORD_FLAGS) & FLAG_STAR) {
      void *corners = Runtime_BumpAllocateAlternatePool(32);
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
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      if (frame >= 8 && frame <= 17) {
        s32 scale = Trig_Sin(((frame - 8) << 11) + 0x3000) * 612 >> 8;

        Render_ResetTransformState();
        Func_08015160((targetPosition.x / 2 - 64) << 16,
                      (targetPosition.y - 64) << 16, 0);
        SceneTransform_ApplyPitch(0x4000);
        SceneTransform_ApplyYaw(0x2000);
        Func_080151e4(scale, scale / 2, scale / 2);
        Render_TransformVertices(gData_08199210, corners, 4);
        Render_DrawModelList(model);
      }
      MemFree(model);
      MemFree(corners);
    }

    if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY)
      Func_08158ce0(8, 8);
    else
      Func_08158ce0(2, 4);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
