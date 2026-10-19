// Draft of Func_0814fab8, the body behind the twelve wrappers of
// stat_boost.c (17 effect kinds).

// Twin: The Broken Seal's BattleFx_RunStatBoost (effect.c), grown from six
// modes to thirteen. The Lost Age:
// - packs the prelude Djinni's element, side and anchor for
// Func_0815585c (Venus for modes 3 and 6, Mercury for 7, 8 and 11,
// Jupiter for 2 and 4, Mars otherwise; mode 10 shows none);
// - plays mode 10 as mode 0 started at frame 53 with a fade of 24, which
// skips the gathering lights;
// - adds modes 6 and 11 (the palette stepping to a resource every four
// frames), 8 and 12 (the violet streak; 12 draws a grey ramp for its
// Djinni), 9 (the wisp in the Mars palette) and 7;
// - calls BattleFx_BuildBlitter (Func_08196404) itself, MAX as mode 1 and
// ADDITIVE as 0, where The Broken Seal's modes are 2 and 3;
// - plays the opening cue at frame 0 inside the loop.
// Resources not yet named in directory.s, by row and asset: 0x137
// BattleFx_ParticleSpritesD, 0x17b BattleFx_GlowOrbSheet, 0x174
// BattleFx_VioletStreakSheet, 0x16a BattleFx_WispSheet, 0x15f
// BattleFx_DaggerSheet, 0x15e BattleFx_ShieldSheet, 0x17f
// BattleFx_PinkBurstSheet, 0xb9 BattleFx_EffectPalette01 and the Djinn
// sheets 0x150, 0x152, 0x154 and 0x156.

// Draft: differ 341 of 1146 (draft 1145).

// Each blitter is built and fetched by one statement macro (LOAD_BLITTER),
// because the native computes the flags afresh for every call: a one-pass
// block ends CSE's block, where plain calls let agscc keep flags ^
// COMPOSITE_FLIP_X in r5 across the first call (670, and every register
// after it shifted).

// Left: the member loop's setup (35 rows) and the Djinni's position (17)
// differ in register choice; the rest is order residue in the draws.

// Tried (2026-10-15):
// - greedy declaration-order sweep, two passes: no gain.
// - a separate counter for the frame loop's lights: 688.
// - flags declared volatile: 916; the loader as a static inline returning
// the slot's entry: 774; the macro reading the entry at slot bytes
// into Ram_HeapSlots: 390; the macro with the field: 341.
// - greedy loop-counter sweep (each loop its own counter): no gain.

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "motobj.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void); // BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void); // BattleFx_ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
void Func_08196404(s32 slot, s32 width, s32 height, s32 flags,
                   s32 mode); // BattleFx_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void Func_081180e8(s32 phase);       // BattleEventRuntime_BeginPhaseFar
void Func_0815585c(struct BattleEffectArgument *effect, s32 djinni, s32 *origin,
                   s32 *target);  // BattleFx_RunDjinniEntrance
void Func_0815f0a0(s32 resource); // BattleFx_StepPaletteToResource
void Func_080150e4(s32 angle);    // SceneTransform_ApplyRoll
void Graphics_LoadGreyRamp(void);         // a grey ramp from color 1
s32 BattleUnit_GetEffectHeightFar(s32 actor);

// The two blitters' heap slots, byte offsets into Ram_HeapSlots (The Broken
// Seal's HEAP_SLOT_BLITTER and HEAP_SLOT_BLITTER_ALTERNATE).
#define BLITTER_SLOT 104           // Ram_HeapSlots->unk64[1]
#define BLITTER_SLOT_ALTERNATE 188 // Ram_HeapSlots->unkb8[1]

// Build a blitter in its heap slot and fetch its entry, as one statement:
// the flags are computed afresh for each blitter.
#define LOAD_BLITTER(out, flags, mode)                                         \
  do {                                                                         \
    Func_08196404(BLITTER_SLOT, 7, 7, flags, mode);                            \
    out = (DrawRectangle)Ram_HeapSlots->unk64[1];                              \
  } while (0)
#define LOAD_BLITTER_ALTERNATE(out, flags, mode)                               \
  do {                                                                         \
    Func_08196404(BLITTER_SLOT_ALTERNATE, 7, 7, flags, mode);                  \
    out = (DrawRectangle)Ram_HeapSlots->unkb8[1];                              \
  } while (0)

// The Lost Age's blitter modes: 3 minus The Broken Seal's
// COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };

// Func_0815585c's packed argument: the Djinni's element, the side, and
// whether the Djinni stands at the anchor.
enum { DJINNI_VENUS, DJINNI_MERCURY, DJINNI_MARS, DJINNI_JUPITER };
#define DJINNI_SIDE(side) ((side) << 4)
#define DJINNI_ANCHORED 32

// Places in the sheet.
enum SeriesBSheet {
  SERIES_B_FLARES = 0x0000,  // the gathering lights
  SERIES_B_BURST = 0x060e,   // six 40x40 bursts
  SERIES_B_PICTURE = 0x2b8e, // the mode's picture
  SERIES_B_DJINNI = 0x65c0   // the 40x40 Djinni
};

#define FLARES ((struct EffectStep *)Ram_MapCellBuffer)
#define FLARES_PER_MEMBER 64

extern char ResourceId_ParticleSpritesD;
extern char ResourceId_GlowOrbSheet;
extern char ResourceId_VioletStreakSheet;
extern char ResourceId_WispSheet;
extern char ResourceId_DaggerSheet;
extern char ResourceId_ShieldSheet;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_EffectPalette01;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_VenusDjinnSheet;
extern char ResourceId_JupiterDjinnSheet;
extern u16 Data_08197424[]; // gBattleFx_FlareCells

// Lights gather around each target in turn, eight frames apart, turning as
// they close in; at the center they burst, and the mode's picture plays
// there (a dagger, a shield, a streak or a wisp).

// Setup: an optional Djinni appears first (the prelude); the canvas is
// 1.25 times as wide, centered on a lone target or set by the side. Frame
// loop: the Djinni sways and rises; each target's 32 lights come out one a
// frame and close in two pixels a frame; from frame 52 of a target its
// burst plays, six pictures four frames apart, and from frame 80 the
// mode's picture.
void Func_0814fab8(struct BattleEffectArgument *effect, u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 member;
  s32 shift;
  s32 flags;
  s32 start;
  s32 frame;
  s32 i;
  s32 origin[3];
  s32 target[3];
  struct EffectPosition base;
  s32 center[3];
  struct EffectPosition screen;
  s32 point[3];

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  start = 0;
  BattleFx_BeginCanvasLayer(0);

  // The prelude: the Djinni appears on the far side; origin receives where.
  if (effect->prelude == 1) {
    if (mode == 3 || mode == 6)
      Func_0815585c(effect,
                    DJINNI_VENUS | DJINNI_SIDE(effect->side ^ 1) |
                        DJINNI_ANCHORED,
                    origin, target);
    else if (mode == 7 || mode == 8 || mode == 11)
      Func_0815585c(effect,
                    DJINNI_MERCURY | DJINNI_SIDE(effect->side ^ 1) |
                        DJINNI_ANCHORED,
                    origin, target);
    else if (mode == 2 || mode == 4)
      Func_0815585c(effect,
                    DJINNI_JUPITER | DJINNI_SIDE(effect->side ^ 1) |
                        DJINNI_ANCHORED,
                    origin, target);
    else if (mode != 10)
      Func_0815585c(
          effect, DJINNI_MARS | DJINNI_SIDE(effect->side ^ 1) | DJINNI_ANCHORED,
          origin, target);
    origin[0] = origin[0] * 4 / 5;
  }

  // Mode 10 is mode 0 joined at frame 53, with a fade.
  if (mode == 10) {
    work->fadeFrames = 24;
    start = 53;
    mode = 0;
  }
  REG_BG2PA = 0xcc; // 1.25 times as wide

  // The pictures and the colors.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + SERIES_B_FLARES, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                             work->sheet + SERIES_B_BURST, 1, 1);
  if (mode == 3 || mode == 5 || mode == 8 || mode == 12) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletStreakSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 1);
    if (mode == 3)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet),
                      128);
    else if (mode == 5)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                      128);
    else
      Iwram_CopyWords(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet), 128);
  } else if (mode == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 1);
  } else if (mode == 9) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                    128);
  } else {
    void *palette;

    if (mode == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet,
                                 work->sheet + SERIES_B_PICTURE, 1, 0);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet,
                                 work->sheet + SERIES_B_PICTURE, 1, 0);
    if (mode == 0)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else if (mode == 2 || mode == 4)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    else if (mode == 6)
      palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
    else if (mode == 7 || mode == 11)
      palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
    else if (mode == 1)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }
  if (mode == 3 || mode == 6)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else if (mode == 12)
    Graphics_LoadGreyRamp();
  else if (mode == 2 || mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else if (mode == 7 || mode == 8 || mode == 11)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);

  // The lights: 25 pixels across and deep, 50 up and down.
  for (i = 0; i != 512; i++) {
    struct EffectStep *flare = &FLARES[i];

    flare->x = (Random16() % 200 - 100) << 14;
    flare->y = (Random16() % 200 - 100) << 15;
    flare->z = (Random16() % 200 - 100) << 14;
    flare->variant = 0;
  }

  // A lone target is moved to the canvas's middle (64).
  if (effect->count == 1) {
    EffectPosition_ProjectScaledBattler(effect->actors[0], &base);
    shift = -base.x * 4 / 5 + 64;
  } else {
    if (effect->side == 1)
      shift = -64;
    else
      shift = 0;
  }
  REG_BG2X = shift << 8;
  work->transferMode = 2;   // each transfer fades the canvas: a trail
  work->transferValue = 50; // to 1/2
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);
  if (effect->side == 1)
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  else
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;

  for (frame = start; frame != effect->count * 8 + 108; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 0)
      Audio_PlayCue(142);
    if (mode == 6 && (frame & 3) == 0)
      Func_0815f0a0((s32)&ResourceId_EffectPalette01);
    if (mode == 11 && (frame & 3) == 0)
      Func_0815f0a0((s32)&ResourceId_JupiterDjinnSheet);
    if (frame == 80)
      Func_081180e8(0);

    // The Djinni sways, and after frame 32 rises two pixels a frame; for
    // the first four frames it is drawn ADDITIVE as well.
    if (effect->prelude == 1) {
      s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
      s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin[1] - 24;

      LOAD_BLITTER(draw[0], flags ^ COMPOSITE_FLIP_X, BLIT_MAX);
      LOAD_BLITTER_ALTERNATE(draw[1], flags ^ COMPOSITE_FLIP_X, BLIT_ADDITIVE);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
      Runtime_ReleaseHeapBlock(BLITTER_SLOT_ALTERNATE);
      Runtime_ReleaseHeapBlock(BLITTER_SLOT);
    }

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;
      s32 height =
          BattleUnit_GetEffectHeightFar(effect->actors[member]) * 2 / 3;

      if (frame == member * 8 + 80)
        Audio_PlayCue(212);
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      // The lights turn about two thirds of the target's height.
      point[0] = object->x;
      point[1] = height;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);
      if (start != 0) {
        if (frame == member * 8 + 53)
          Func_0814cd48(effect->actors[member], 7, -1, member, 16);
      } else {
        if (frame == member * 8 + 48)
          Func_0814cd48(effect->actors[member], 7, -1, member, 16);

        // The lights turn and close in two pixels a frame.
        if (frame > member * 8) {
          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          LOAD_BLITTER_ALTERNATE(draw[1], flags, BLIT_ADDITIVE);
          if (mode == 0) {
            SceneTransform_ApplyPitch(-frame << 10);
          } else if (mode == 1) {
          } else if (mode == 2) {
            SceneTransform_ApplyYaw(frame << 10);
          } else {
            SceneTransform_ApplyYaw(frame << 10);
            Func_080150e4(frame << 10);
          }
          for (i = 0; i != 32; i++) {
            struct EffectStep *flare = &FLARES[member * FLARES_PER_MEMBER + i];

            if (frame > member * 8 + i) {
              s32 xx = (flare->x >> 8) * (flare->x >> 8);
              s32 yy = (flare->y >> 8) * (flare->y >> 8);
              s32 zz = (flare->z >> 8) * (flare->z >> 8);
              s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

              if (distance != 0) {
                s32 size;

                // Half the width: depth 314-634 gives 6 down to 1.
                EffectPosition_ProjectPoint((s32 *)flare, &screen);
                screen.x = screen.x * 4 / 5 + shift;
                if (screen.depth < 314)
                  screen.depth = 314;
                if (screen.depth > 634)
                  screen.depth = 634;
                size = 6 - (screen.depth - 314) / 64;
                draw[1](canvas, work->sheet + Data_08197424[size - 1],
                        screen.x - size, screen.y - size, size * 2, size * 2);
                flare->x -= flare->x / distance;
                flare->y -= flare->y / distance;
                flare->z -= flare->z / distance;
              }
            }
          }
          Runtime_ReleaseHeapBlock(BLITTER_SLOT_ALTERNATE);
          Runtime_ReleaseHeapBlock(BLITTER_SLOT);
        }
      }

      // Where the center is on the screen.
      center[0] = 0;
      center[1] = 0;
      center[2] = 0;
      EffectPosition_ProjectPoint(center, &screen);
      screen.x = screen.x * 4 / 5 + shift;

      // The burst: from frame 52, six pictures four frames apart.
      if (frame >= member * 8 + 52 && frame < member * 8 + 76) {
        s32 cel = (frame - member * 8 - 52) / 4 % 6;

        LOAD_BLITTER(draw[0], flags, BLIT_MAX);
        draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_BURST,
                screen.x - 20, screen.y - 20, 40, 40);
        Runtime_ReleaseHeapBlock(BLITTER_SLOT);
      }

      // From frame 80, the mode's picture.
      if (mode == 0) {
        // Seven 24x40 pictures, four frames apart.
        if (frame >= member * 8 + 80 && frame < member * 8 + 108) {
          s32 cel = (frame - member * 8 - 80) / 4 % 7;

          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          draw[0](canvas, work->sheet + cel * 960 + SERIES_B_PICTURE,
                  screen.x - 12, screen.y - 20, 24, 40);
          Runtime_ReleaseHeapBlock(BLITTER_SLOT);
        }
      } else if (mode == 3 || mode == 5 || mode == 8 || mode == 12) {
        // Six 32x64 pictures, four frames apart.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          draw[0](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE,
                  screen.x - 16, screen.y - 32, 32, 64);
          Runtime_ReleaseHeapBlock(BLITTER_SLOT);
        }
      } else if (mode == 4 || mode == 9) {
        // Six 64x32 pictures two frames apart, mirrored below into 64x64.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 2 % 6;

          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          LOAD_BLITTER_ALTERNATE(draw[1], flags | COMPOSITE_FLIP_Y, BLIT_MAX);
          draw[0](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE,
                  screen.x - 32, screen.y - 24, 64, 32);
          draw[1](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE,
                  screen.x - 32, screen.y + 8, 64, 32);
          Runtime_ReleaseHeapBlock(BLITTER_SLOT_ALTERNATE);
          Runtime_ReleaseHeapBlock(BLITTER_SLOT);
        }
      } else {
        // Six 40x40 pictures four frames apart, ADDITIVE.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          LOAD_BLITTER(draw[0], flags, BLIT_ADDITIVE);
          draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_PICTURE,
                  screen.x - 20, screen.y - 20, 40, 40);
          Runtime_ReleaseHeapBlock(BLITTER_SLOT);
        }
      }
    }
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}
