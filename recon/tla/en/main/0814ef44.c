// Draft of BattleEffect_RunHealingMotes, the body behind the eleven wrappers of
// series_a.c (18 effect kinds).

// Twin: The Broken Seal's BattleEffect_RunHealingMotes (effect.c), grown from seven
// modes to fourteen. The Lost Age adds:
// - a Djinni of any element for the prelude (Func_0815585c takes the
// element, the side and the anchor packed into one word: Venus for
// modes 2 and 7, Mars for 6, Jupiter for 9 and 10, Mercury otherwise);
// - a blue beam picture, decoded and packed to bytes for mode 12;
// - mode 7 (motes thrown up and out from the targets' feet, the palette
// stepping to cyan), mode 8 (like mode 0 with 128 motes and a fixed 76
// frames), mode 9 (Jupiter, fast yaw, a fixed 54 frames), mode 10,
// mode 11 (a wide cloud that never closes in, 46 frames past the
// targets), mode 12 (16 beams) and mode 13 (a roll, a fade of 24);
// - every effect 58 frames past the targets where The Broken Seal ran 88
// or 72, a later hit (frame 36, 24 or 41 by mode), and motes three
// quarters as far apart.
// Resources not yet named in directory.s, by row and asset: 0x17e
// BattleEffect_SparkleDots, 0x190 BattleEffect_BlueBeamSheet, 0x177
// BattleEffect_IceShardSheet, 0x184 BattleEffect_BlastSheet, 0x148
// BattleEffect_OrangePaletteB, 0x166 BattleEffect_CyanPalette and the eight Djinn
// sheets 0x150-0x157.

// Draft: differ 34 of 1243 (draft 1243), tla-en, 2026-10-26.

// What moved it: the yaw angles written (frame - start) where The Broken
// Seal's twin writes (frame - member * 8). As member * 8 they are givs of
// their own, which loop.c combines with start and strength-reduces into a
// register stepped by 8 (three instructions short, and the frame counter
// and start swapped between r9 and r11 for the whole member loop); through
// start the giv is not worth reducing, and the native recomputes member * 8
// from its stack slot each iteration (269 to 52). shift = 0 after the
// sheet read: 50. The IWRAM calls through local inline forwarders, as
// f143d80.c makes them and as The Broken Seal's IWRAM call layer makes
// every one: the palette copy's constants no longer share registers with
// the frame loop's invariants, whose spill slots (camera + 12, mode - 3
// and &point) then fall in the native's order (50 to 34).

// Left: the prologue's argument spill and shift's zero before the first
// call, and order rows in the draws' arguments: the known store-then-load
// scheduling limit, one row each.

// Tried (2026-10-15):
// - The Broken Seal's declaration order: 315; the five arrays declared
// origin, target, base, screen, point, so that they take the native's
// stack frame offsets (blit at sp+80 up to origin at sp+136): 299.
// - greedy declaration-order sweep, two passes: no gain.
// - a separate counter for the frame loop's motes: no change.
// Tried (2026-10-26): declaration order, statement moves, each local
// scoped to a block, point through a pointer, member volatile: no gain;
// shift = 0 back before the canvas layer: 36.

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
#include "task.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "motobj.h"
#include "actor.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"

static __inline__ s32 CopyWords_(void *destination, const void *source,
                                 s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

static __inline__ s32 Sqrt_(s32 value) { return Iwram_Sqrt(value); }

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void); // BattleEffect_CountDownBattlerTints
void BattleEffect_FetchRectangleBlitters(
    s32 side,
    DrawRectangle *pair); // BattleEffect_FetchRectangleBlitters
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // Actor_UpdateMembers
void Func_081180e8(s32 phase);       // BattleEventRuntime_StartPhaseFar
void Func_0815585c(struct BattleEffectArgument *effect, s32 djinni, s32 *origin,
                   s32 *target);  // BattleEffect_RunDjinniEntrance
void Func_0815f0a0(s32 resource); // BattleEffect_StepPaletteToResource
void Func_080150e4(s32 angle);    // SceneTransform_ApplyRoll

// Func_0815585c's packed argument: the Djinni's element, the side, and
// whether the Djinni stands at the anchor.
enum { DJINNI_VENUS, DJINNI_MERCURY, DJINNI_MARS, DJINNI_JUPITER };
#define DJINNI_SIDE(side) ((side) << 4)
#define DJINNI_ANCHORED 32

#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARKS_PER_MEMBER 64
#define BEAM_SHEET 0x800    // the blue beam as unpacked, two bytes a pixel
#define BEAM_BYTES 0x2000   // the beam packed to a byte a pixel
#define DJINNI_SHEET 0x1000 // the Djinni picture

extern char ResourceId_Data134; // the particle sheet
extern char ResourceId_SparkleDots;
extern char ResourceId_BlueBeamSheet;
extern char ResourceId_IceShardSheet;
extern char ResourceId_IceBlockSheet;
extern char ResourceId_BlastSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_CyanPalette;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_MercuryDjinnSmallSheet;
extern char ResourceId_VenusDjinnSheet;
extern char ResourceId_VenusDjinnSmallSheet;
extern char ResourceId_JupiterDjinnSheet;
extern char ResourceId_JupiterDjinnSmallSheet;
extern u16 gData_08197410[]; // gBattleEffect_MoteCells
extern u16 Data_0819744c[];  // gBattleEffect_PuffCells
extern u8 Data_0819745e[];   // gBattleEffect_PuffSizes

// Motes of light gather around each target in turn, eight frames apart,
// turning as they close in on its middle; a healing chime sounds as they
// gather and the target glows.

// Setup: an optional Djinni appears first (the prelude); the mode chooses
// the canvas scale, the palette, the Djinni and how the 512 motes are
// scattered. Frame loop: the Djinni sways and, after frame 32, rises two
// pixels a frame; each target's motes come out four a frame around a point
// 40 pixels above it, turn about it by the mode's axes, and close in two
// pixels a frame (modes 3-5 let them fade instead, mode 7 throws them).
// The variant chooses 32, 64 or 128 motes a target.
void BattleEffect_RunHealingMotes(struct BattleEffectArgument *effect, u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 total;
  s32 member;
  u8 *sprites;
  s32 limit;
  s32 shift;
  s32 narrow;
  DrawRectangle *draw;
  s32 frame;
  s32 i;
  u8 *palette;
  s32 origin[3];
  s32 target[3];
  struct EffectPosition base;
  struct EffectPosition screen;
  s32 point[3];
  DrawRectangle blit[2];

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  shift = 0;
  if (mode == 13)
    work->fadeFrames = 24;

  // The prelude: the Djinni appears; origin receives where.
  if (effect->prelude == 1) {
    s32 djinni;

    if (mode == 6)
      djinni = DJINNI_MARS;
    else if (mode == 2 || mode == 7)
      djinni = DJINNI_VENUS;
    else if (mode == 9 || mode == 10)
      djinni = DJINNI_JUPITER;
    else
      djinni = DJINNI_MERCURY;
    if (mode == 6 || mode == 0 || mode == 8 || mode == 9 || mode == 10)
      Func_0815585c(effect, djinni | DJINNI_SIDE(effect->side), origin, target);
    else
      Func_0815585c(effect,
                    djinni | DJINNI_SIDE(effect->side) | DJINNI_ANCHORED,
                    origin, target);
    effect->variant = 0;
  }

  // Modes 0, 8 and 9 show the canvas at full width, the first target at
  // the middle (64); the rest keep it twice as wide (narrow).
  if (mode == 0 || mode == 8 || mode == 9) {
    EffectPosition_ProjectScaledBattler(effect->actors[0], &base);
    shift = 64 - base.x;
    REG_BG2X = shift << 8;
    REG_BG2PA = 0x100;
    narrow = 0;
  } else {
    narrow = 1;
  }

  // The pictures, the beam packed to a byte a pixel, and the colors.
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet,
                             work->sheet + BEAM_SHEET, 1, 0);
  for (i = 0; i != 288; i++)
    work->sheet[BEAM_BYTES + i] = work->sheet[BEAM_SHEET + i * 2];
  if (mode <= 1 || mode == 3 || mode == 4 || mode == 5 || mode == 8 ||
      mode == 12) {
    if (effect->variant == 0)
      palette = Resource_GetTableEntry((s32)&ResourceId_IceShardSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
  } else if (mode == 9 || mode == 10) {
    palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
  } else if (mode == 6 || mode == 11 || mode == 13) {
    palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
  } else {
    palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  }
  CopyWords_((void *)BG_PLTT, palette, 128);

  // The Djinni: 40x40 at full width, a 20x40 picture when narrow.
  if (narrow == 0) {
    if (mode == 6)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else if (mode == 2 || mode == 7)
      palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
    else if (mode == 9 || mode == 10)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
  } else {
    if (mode == 6 || mode == 11 || mode == 13)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet);
    else if (mode == 2 || mode == 7)
      palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSmallSheet);
    else if (mode == 9 || mode == 10)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSmallSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet);
  }
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + DJINNI_SHEET);
  BattleEffect_FetchRectangleBlitters(effect->side,
                                  draw = blit); // [0] MAX, [1] ADDITIVE

  // Scatter the motes; the mode decides how, and how long the effect is.
  if (mode == 0 || mode == 6 || mode == 8 || mode == 9 || mode == 10) {
    // Across and deep 64 pixels each way, 32-95 pixels up.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = ((Random16() & 0xff) - 127) << 15;
      spark->y = ((Random16() & 0x7f) + 64) << 15;
      spark->z = ((Random16() & 0xff) - 127) << 15;
      spark->variant = 0;
    }
    if (mode == 9)
      total = 54;
    else if (mode == 8)
      total = 76;
    else
      total = effect->count * 8 + 58;
  } else if (mode == 7) {
    // From the feet, 48 pixels up, thrown out and up.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->y = 48 << 16;
      spark->x = 0;
      spark->z = 0;
      spark->velocityX = ((Random16() & 0xff) - 127) << 8;
      spark->velocityY = -((Random16() & 0x7f) + 64) << 9;
      spark->velocityZ = ((Random16() & 0xff) - 127) << 8;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  } else if (mode == 12) {
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = ((Random16() & 0xff) - 127) << 15;
      spark->y = ((Random16() & 0x7f) + 64) << 15;
      spark->z = ((Random16() & 0xff) - 127) << 15;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  } else if (mode == 1 || mode == 13) {
    // 64 pixels each way.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = ((Random16() & 0xff) - 127) << 15;
      spark->y = ((Random16() & 0xff) - 127) << 15;
      spark->z = ((Random16() & 0xff) - 127) << 15;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  } else if (mode == 11) {
    // A wide cloud, 100 pixels each way.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = (Random16() % 200 - 100) << 15;
      spark->y = (Random16() % 200 - 100) << 15;
      spark->z = (Random16() % 200 - 100) << 15;
      spark->variant = 0;
    }
    total = effect->count * 8 + 46;
  } else if (mode == 2) {
    // A ring 32-95 pixels across, 50 pixels down, rising.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];
      s32 heading = Random16() & 0xffff;
      s32 radius = (Random16() & 63) + 32;

      spark->x = Trig_Sin(heading) * radius;
      spark->y = -50 << 16;
      spark->z = Trig_Cos(heading) * radius;
      spark->velocityY = ((Random16() & 31) + 32) << 13;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  } else if (mode == 3) {
    // 64 pixels across and deep, 32 up and down.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = ((Random16() & 0xff) - 127) << 15;
      spark->y = ((Random16() & 0xff) - 127) << 14;
      spark->z = ((Random16() & 0xff) - 127) << 15;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  } else {
    // 64 pixels each way.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = ((Random16() & 0xff) - 127) << 15;
      spark->y = ((Random16() & 0xff) - 127) << 15;
      spark->z = ((Random16() & 0xff) - 127) << 15;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  }

  // Motes a target: 32 for variant 0, 128 for variant 2, else 64; mode 8
  // takes 128 and mode 12 16.
  limit = 64;
  if (effect->variant == 0)
    limit = 32;
  else if (effect->variant == 2)
    limit = 128;
  if (mode == 8)
    limit = 128;
  if (mode == 12)
    limit = 16;
  work->transferMode = 2;   // each transfer fades the canvas: a trail
  work->transferValue = 75; // to 3/4
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 40)
      Func_081180e8(0);
    if (mode == 7 && (frame & 3) == 0)
      Func_0815f0a0((s32)&ResourceId_CyanPalette);

    // The Djinni sways, and after frame 32 rises two pixels a frame; for
    // the first four frames it is drawn ADDITIVE as well.
    if (effect->prelude == 1) {
      if (narrow == 0) {
        s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin[1] - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + DJINNI_SHEET, x, y, 40, 40);
        if (frame <= 3)
          draw[1](canvas, work->sheet + DJINNI_SHEET, x, y, 40, 40);
      } else {
        s32 x = ((Trig_Sin(frame << 11) * 10) >> 16) + origin[0] / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin[1] - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + DJINNI_SHEET, x, y, 20, 40);
        if (frame <= 3)
          draw[1](canvas, work->sheet + DJINNI_SHEET, x, y, 20, 40);
      }
    }

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;
      s32 start = member * 8;

      // The motes turn about a point 40 pixels above the target.
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = object->x;
      point[1] = 40 << 16;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);
      if (frame == start + 20)
        Sound_PlayFar(126);
      if (mode == 8) {
        if (frame == start + 41)
          Func_0814cd48(effect->actors[member], 7, -1, member, 32);
      } else if (mode == 11) {
        if (frame == start + 24)
          Func_0814cd48(effect->actors[member], 7, -1, member, 28);
      } else if (frame == start + 36) {
        Func_0814cd48(effect->actors[member], 7, -1, member, 28);
      }
      if (frame > start) {
        if (mode == 0 || mode == 6 || mode == 7 || mode == 8 || mode == 10 ||
            mode == 12) {
          SceneTransform_ApplyYaw((frame - start) << 9);
        } else if (mode == 9) {
          SceneTransform_ApplyYaw((frame - start) << 11);
        } else if (mode == 11) {
        } else if (mode == 13) {
          Func_080150e4(frame << 9);
        } else if (mode == 1) {
          SceneTransform_ApplyPitch(frame << 9);
          Func_080150e4(frame << 9);
        } else if (mode == 2 || mode == 12) {
          SceneTransform_ApplyYaw((frame - member * 40) << 9);
        } else if (mode == 3) {
          SceneTransform_ApplyYaw((frame - start) << 9);
        } else {
          SceneTransform_ApplyYaw((frame - start) << 9);
          SceneTransform_ApplyPitch((frame - start) << 9);
        }

        // Four motes come out a frame; in modes 3-5 each fades at
        // start + i / 2 + 32, in mode 7 at start + i / 2 + 64.
        for (i = 0; i != limit; i++) {
          struct EffectStep *spark = &SPARKS[member * SPARKS_PER_MEMBER + i];
          s32 end;

          if (mode == 3 || mode == 4 || mode == 5)
            end = start + i / 2 + 32;
          else if (mode == 7)
            end = start + i / 2 + 64;
          else
            end = 0x10000;
          if (frame > start + i / 4 && frame < end) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Sqrt_(xx + yy + zz) >> 9;

            distance = distance * 48 / 64;
            if (distance != 0) {
              s32 size;

              EffectPosition_ProjectPoint((s32 *)spark, &screen);
              if (mode == 0 || mode == 8 || mode == 9)
                screen.x += shift;
              else
                screen.x >>= 1;
              screen.y += 16;

              // Depth 314-634 gives sizes 3 down to 1; some modes cycle
              // through the nine puffs instead.
              if (screen.depth < 314)
                screen.depth = 314;
              if (screen.depth > 634)
                screen.depth = 634;
              size = 3 - (screen.depth - 314) / 128;
              if (mode == 0 || mode == 8 || mode == 9)
                size = (i * 4 + frame) % 9;
              else if (mode == 11 || mode == 13)
                size = 4 - (screen.depth - 314) / 128;
              else if (mode == 7)
                size = (i * 4 + frame) % 9;
              if (mode == 0 || mode == 3 || mode == 4 || mode == 5 ||
                  mode == 7 || mode == 8 || mode == 9 || mode == 11 ||
                  mode == 13) {
                u8 *cell = work->sheet + Data_0819744c[size];
                u32 edge = Data_0819745e[size];
                u32 half = edge >> 1;

                draw[1](canvas, cell, screen.x - half, screen.y - half, edge,
                        edge);
              } else if (mode == 12) {
                blit[0](canvas, work->sheet + BEAM_BYTES, screen.x - 6,
                        screen.y - 12, 12, 24);
              } else {
                draw[1](canvas, sprites + gData_08197410[size - 1],
                        screen.x - size / 2, screen.y - size, size, size * 2);
              }

              // Close in by 1/distance of the way a frame.
              if (mode <= 2 || mode == 6 || mode == 8 || mode == 9 ||
                  mode == 10 || mode == 11 || mode == 13 || mode == 12) {
                spark->x -= spark->x / distance;
                spark->y -= spark->y / distance;
                spark->z -= spark->z / distance;
              }
              if (mode == 7) {
                spark->x += spark->velocityX;
                spark->y += spark->velocityY;
                spark->z += spark->velocityZ;
              }
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
