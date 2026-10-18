// Draft, differ 338 of 755: BattleFx_RunTargetMotes, the body of nine effect
// kinds through the wrappers in games/tla/src/battle/effect/render_mode.c
// (modes 0 to 7).

// ☀️ twin: BattleFx_RunTargetMotes (games/tbs/src/effect/effect.c), shape 0.898,
// structure 0.907. ⚓️'s changes: the effect argument read directly; mode
// 1 takes its palette from the Mars Djinn sheet (directory row 0x150) as
// mode 7 does, where ☀️'s mode 1 takes the pink burst's; the blitters from
// BattleFx_LoadBlitter(104, 0x03) (additive) and (188, 0x13) (max), and the
// tornado's glints through Func_08196404(188, 7, 7, flags, 1) directly;
// the affine wave's HBlank arm BattleFx_ArmBg2AffineHBlankDma and the transfer callback at
// 0xc80. In mode 3's glint block the native ands the frame with the mode
// register, CSE's reuse of the known mode 3 for the constant: the source
// is frame & 3.

// What is left is allocation (and two instructions: 753 of 755): the native
// frame is 120 bytes where this draft's is 112, with two more spill slots (the
// target's offset into the actor list and member * 8 held apart from the loop's
// own start, angle and the glint height table), mode in r10 where agscc gives
// it r8, and the rows that follow from that.

// Tried (differ):
// ☀️'s body with ⚓️'s names and changes                             364
// greedy declaration-order scan: total, member and count first      338

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "motobj.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "graphics.h"
#include "transform.h"
#include "fixed_math.h"
#include "resource.h"

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define CALLBACK_ORDER 0xc80
#define MOTE_MAX 512

// The motes live in the map cell buffer, 64 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

// Modes whose motes close in on the target and start again; the others
// drift up and are thrown back at random. Mode 7 closes in once.
#define CONVERGES(mode)                                                        \
  ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
#define DECAYS(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

// Where mode 3 unpacks the tornado's glints in the sheet.
enum { RENDER_MODE_TORNADO = 0x2580 };

extern char ResourceId_SmokeSheet;     // directory row 0x161
extern char ResourceId_LimePalette;    // directory row 0x163
extern char ResourceId_MarsDjinnSheet; // directory row 0x150
extern char ResourceId_CyanPalette;    // directory row 0x166
extern char ResourceId_BlastSheet;     // directory row 0x184
extern char ResourceId_PinkBurstSheet; // directory row 0x17f
extern char ResourceId_GlowOrbSheet;   // directory row 0x17b
extern char ResourceId_VortexSheet;
extern char ResourceId_HeartSheet; // directory row 0x16e
extern char ResourceId_TornadoSheet;

extern u16 gData_08197486[]; // ☀️ gBattleFx_GlintCellOffsets
extern u8 gData_08197492[];  // ☀️ gBattleFx_GlintCellWidths
extern u8 gData_08197498[];  // ☀️ gBattleFx_GlintCellHeights
extern u8 Data_08197a34[];   // ☀️ gTargetMotes_GlintFlips

void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void);                  // ☀️ BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void);                  // ☀️ ProcessPendingGraphicsTransfer
void BattleFx_ArmBg2AffineHBlankDma(void);                  // ☀️ BattleFx_ArmBg2AffineHBlankDma
void ObjectGroup_TickMemberTimers(void);                  // ☀️ ObjectGroup_TickMemberTimers
void Func_08158ce0(s32 x, s32 y);          // ☀️ Camera_ApplyShake
void Func_081180e8(s32 phase);             // ☀️ BattleEventRuntime_BeginPhaseFar
void Func_08118088(s32 actor, s32 motion); // ☀️ ApplyVariantMotionFar
s32 BattleFx_LoadBlitter(s32 slot, s32 flags);    // ☀️ BattleFx_BuildBlitter
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // ☀️ BattleFx_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ ObjectGroup_UpdateMembers
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void Audio_PlayCue(s32 cue);

// Motes swirl around each target in turn, eight frames apart, over a
// rippling canvas: smoke, hearts, glowing orbs or vortex motes by mode,
// in the mode's palette; mode 3 adds the tornado's glints, and mode 7 sends
// the caster at the first target for the final blow.
void BattleFx_RunTargetMotes(struct BattleEffectArgument *effect, u32 mode) {
  s32 total;
  s32 member;
  s32 count;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
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
  BattleFx_BeginCanvasLayer(0);
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
      mote->x = ((Random16() & 0xff) - 127) << 15;
      mote->y = ((Random16() & 0xff) - 127) << 14;
      mote->z = ((Random16() & 0xff) - 127) << 15;
    } else {
      mote->x = ((Random16() & 0xff) - 127) << 13;
      mote->y = ((Random16() & 0xff) - 255) << 13;
      mote->z = ((Random16() & 0xff) - 127) << 13;
    }
    mote->variant = 0;
  }

  if (CONVERGES(mode))
    total = effect->count * 8 + 64;
  else
    total = effect->count * 8 + 32;
  if (!(mode <= 1 || mode == 3))
    Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, CALLBACK_ORDER);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);
  Audio_PlayCue(142);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // Mode 7 sends the caster at the first target 46 frames before the
    // end and strikes it 14 frames later; the others begin phase 133.
    if (mode == 7) {
      if (frame == total - 46)
        BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
      if (frame == total - 32) {
        Func_081180e8(134);
        Func_08118088(effect->actors[0], 4);
        work->shakeFrames = 8;
      }
    } else if (frame == total - 32) {
      Func_081180e8(133);
    }

    // Ripple the canvas: one horizontal offset for each line.
    row = work->bg2X;
    for (i = 0; i != 160; i++)
      *row++ = (0x40000 - (Trig_Sin((frame << 12) + i * 0x800) << 2)) >> 10;

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;

      // Mode 3: two tornado glints a frame around the target, each drawn
      // with a blitter of its own, mirrored at random.
      if (mode == 3 && frame > member * 8 && frame < member * 8 + 32) {
        EffectPosition_ApplyStepAndYOffset(effect->actors[member], &base);
        for (i = 0; i != 2; i++) {
          s32 angle;
          s32 radius;
          s32 x;
          s32 y;

          angle = Random16() & 0xffff;
          radius = (Random16() & 31) + 4;
          x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
              (gData_08197492[frame & 3] >> 1);
          y = base.y - ((Trig_Cos(angle) * radius) >> 16) -
              (gData_08197498[frame & 3] >> 1);
          Func_08196404(HEAP_BLITTER_ALTERNATE, 7, 7,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                            Data_08197a34[Random16() & 3],
                        BLITTER_MAX);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas,
              work->sheet + gData_08197486[frame & 3] + RENDER_MODE_TORNADO, x,
              y + 16, gData_08197492[frame & 3], gData_08197498[frame & 3]);
          Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
        }
      }

      // An additive and a max blitter for this target's motes.
      BattleFx_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE << 4 | COMPOSITE_CLIP_X |
                                      COMPOSITE_CLIP_Y);
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                    BLITTER_MAX << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = object->x;
      point[1] = 160 << 13;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);

      // From its turn on, the target's motes turn around it.
      if (frame > member * 8) {
        SceneTransform_ApplyYaw(frame << 9);
        if (mode <= 1 || mode == 4)
          SceneTransform_ApplyPitch(frame << 9);
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

              EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
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
                mote->y += 0x10000;
              }
              mote->variant++;
              if (mote->variant == 24) {
                if (DECAYS(mode)) {
                  mote->variant = 0;
                } else {
                  mote->x = ((Random16() & 0xff) - 127) << 13;
                  mote->y = ((Random16() & 0xff) - 255) << 12;
                  mote->z = ((Random16() & 0xff) - 127) << 13;
                }
              }
            }
          }
        }
      }
      Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
      Runtime_ReleaseHeapBlock(HEAP_BLITTER);

      // The target flashes 16 frames into its turn, until the end.
      if (frame == member * 8 + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        Func_0814cd48(effect->actors[member], 7, 5, member, delay);
      }
    }

    Func_08158ce0(16, 16);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Tear down.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  if (!(mode <= 1 || mode == 3))
    Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  BattleFx_EndCanvasLayer();
}
