// Draft of BattleFx_RunBurstScene, the burst family's body (ten kinds
// through eight wrappers): blow after blow lands on the first target.

// On screen: the caster closes in on the first target (kind variant 12
// lunges further and waits seven frames), then a run of blows lands there,
// one every few frames: each is an arc (or, every third and fourth blow,
// a wave low across the ground) drawn mirrored on alternate blows, an orb
// that bursts over the target, the target flashing, a recoil on every
// other blow, cue 134 and a short shake, and a spray of motes that fall
// and bounce off the ground. The last blow recoils the target harder,
// shakes for eight frames and lets the battle resume with hit cue 134.
// Some records fill the canvas at each hit. Record mode 1 adds glints
// sparkling around the target for the first 32 frames; mode 2 ripples BG2
// through an H-blank line table and lets 16 puffs of smoke rise from the
// target. With its Djinn prelude the canvas preparer runs first.

// Each variant has an eight-byte record (☀️'s gBurstScene_Records had
// seven):
// [0] the arc's sheet (0 BlueArcSheetA, else B; 1 also draws every blow
// as an arc)
// [1] the palette's resource (0 to 3)
// [2] the number of blows
// [3] the motes each blow throws
// [4] the frames between blows
// [5] the effect's length in frames
// [6] 1 to fill the canvas at each hit
// [7] 1 for the glints, 2 for the ripple and the smoke

// Setup: the battle effect canvas, the dot sheet, the orb sheet packed to
// half width in the map cell buffer (the canvas is shown twice as wide),
// the arc, wave and smoke sheets, the record's palette, the approach, the
// target's position, the transfer callback (and the ripple's), 256 motes
// waiting 25 pixels above the target, and 64 smoke puffs at its feet.
// Frame loop, record[5] frames: the ripple's line table, the blows (arc or
// wave, hit, orb), the motes, then the glints or the smoke.
// Teardown: the callbacks are removed and the canvas layer is closed; the
// blitters are built and released every frame.

// Port of The Broken Seal's BattleFx_RunBurstScene, reworked in The Lost
// Age: an eighth record byte and its two extras, the orb packed into the
// map cell buffer, 256 motes where ☀️ has 768, the approach by variant,
// The Lost Age's heap slots, blitter modes (3 minus ☀️'s) and callback
// order, and no work->effect store.

// Listing 989 instructions, draft 990, differ 189 (2026-10-15). Left: the
// hoisted pointers' spill slots one word off (&actorPos, &draw and
// camera + 12 at sp+20, 24 and 16 where the native has 24, 28 and 20), the
// glint loop's invariants (the native keeps the constant 3 in r9 for both
// masks and re-reads the width and height tables; here the height table
// and cel * 2 are hoisted), and rows one place apart.

// Tried:
// ☀️'s body with The Lost Age's changes as read from the
// listing                                                 584 (975 rows)
// the Tornado sheet load after the packing, the arcs at
// work->sheet + 0xc80                                      584 -> 535
// declaration order (greedy sweep)                           535 -> 509
// the record table without const, as ☀️'s readers extern
// theirs: the blow count is read again after each call,
// and the frame drops to the native 152 bytes              509 -> 362
// declaration order again                                    362 -> 328
// the packing loop: Ram_MapCellBuffer + 0x2400 as a symbol
// (846), folded with 0x2e88 (343), split as
// (base + dst)[0x2e88] (295), the base a local pointer set
// before the loop (247), dst declared before src (the
// native's row * 20 in r6)                                328 -> 239
// the height table without const, Trig_Sin(angle) * radius
// as the native multiplies                                 239 -> 211
// (the width table without const, or both: 295 and 311;
// frame & 3 in place of cel: 351)
// declaration order again                                    211 -> 189

#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "battle_session.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"

// Byte offsets of the two blitter slots in gHeapSlots (unk64[1], unkb8[1]).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The Lost Age numbers the blitter modes the other way round from The
// Broken Seal's COMPOSITE_ modes; Func_081963ec takes mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// The canvas preparer's packed arguments.
#define PREPARE(kind, side, anchor) ((kind) | (side) << 4 | (anchor) << 5)

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

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

typedef DrawRectangle BlitterPair[2];

extern char ResourceId_ParticleSpritesA;
extern char ResourceId_YellowOrbSheet;
extern char ResourceId_TornadoSheet;
extern char ResourceId_BlueArcSheetA;
extern char ResourceId_BlueArcSheetB;
extern char ResourceId_WaveSheet;
extern char ResourceId_SmokeSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_LimePalette;
extern char ResourceId_EmberStreakSheet;
// The variants' records, eight bytes each.
extern u8 Data_081985b3[];
// Byte offsets of the dot cels in the particle sheet, by size - 1.
extern const u16 gData_08197410[];
// The glint's six cels: ☀️'s gBattleFx_GlintCellOffsets, _GlintCellWidths
// and _GlintCellHeights.
extern const u16 gData_08197486[];
extern const u8 gData_08197492[];
extern u8 gData_08197498[];
// A glint's four flips.
extern const u8 Data_0819861b[];

u32 Random16(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void *Resource_GetTableEntry(s32 id);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleMotion_ApproachTargetFar(s32 actor, s16 target, s32 frames,
                                    s32 height);
void EffectPosition_ApplyStepAndYOffset(s32 actor,
                                        struct EffectPosition *position);
void Render_ResetTransformState(void);
void Transform_SetLookAt(void *camera, void *eye);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
// Prepares the canvas around an anchor and returns its screen point.
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
// Builds a blitter in a heap slot: BattleFx_LoadWork(slot, 7, 7,
// flags, mode), whose result it returns.
s32 Func_081963ec(s32 slot, s32 modeFlags);
// BattleFx_LoadWork itself.
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  s32 mode);
// Closes the canvas layer.
void Func_08143bb8(void);
// Copies the canvas to VRAM when a transfer is pending.
void Func_08143000(void);
// Arms the H-blank DMA of the work's line table into BG2X.
void Func_08152474(void);
// Counts down the actors' flash timers.
void Func_081434f8(void);
// BattleEventRuntime_BeginPhase through its far call.
void Func_081180e8(s32 hitCue);
// An actor's recoil motion, through a far call.
void Func_08118088(s32 actor, s32 motion);
// Flashes an actor in a palette for some frames.
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
// Shakes the camera while work->shakeFrames lasts.
void Func_08158ce0(s32 randomMask, u32 shakeRange);

void BattleFx_RunBurstScene(struct BattleEffectArgument *effect,
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
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  if (effect->prelude == 1)
    Func_0815585c(effect, PREPARE(7, effect->side, 2), screen, otherPoint);

  // The pictures and the palette.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
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
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);

  // The caster closes in; variant 12 lunges further and waits.
  if (variant == 12) {
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 8,
                                   0x60000);
    WaitFrames(7);
  } else {
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 4, 0);
  }
  WaitFrames(1);

  EffectPosition_ApplyStepAndYOffset(effect->actors[0], &targetPos);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
  if (RECORD(RECORD_EXTRA) == EXTRA_RIPPLE)
    Scheduler_AddOrUpdateCallback(Func_08152474, 0xc80);

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

    puff->z = 0;
    puff->x = (targetPos.x / 2) << 16;
    puff->y = 0x580000;
    puff->velocityX = ((Random16() & 255) - 128) << 9;
    puff->velocityY = ((Random16() & 255) - 128) << 9;
    puff->variant = -i;
    puff->velocityZ = 0;
  }

  for (frame = 0; frame != RECORD(RECORD_FRAMES); frame++) {
    s32 blows;

    // The ripple's line table: BG2 swings six pixels on a sine wave.
    for (i = 0; i != 160; i++)
      work->bg2X[i] = (0x60000 - Trig_Sin((frame << 11) + i * 0x800) * 6) >> 10;
    if (variant == 10 && frame == 8)
      Audio_PlayCue(221);
    blows = RECORD(RECORD_BLOWS);
    EffectPosition_ApplyStepAndYOffset(effect->actor, &actorPos);
    actorPos.x /= 2;

    // The arcs and waves, mirrored on odd blows.
    if (effect->side == 0) {
      Func_081963ec(HEAP_BLITTER,
                    BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      Func_081963ec(HEAP_BLITTER_ALTERNATE,
                    BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                          COMPOSITE_FLIP_Y));
    } else {
      Func_081963ec(HEAP_BLITTER,
                    BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                          COMPOSITE_FLIP_X));
      Func_081963ec(HEAP_BLITTER_ALTERNATE,
                    BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                          COMPOSITE_FLIP));
    }
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
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
          Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
        if (i == blows - 1) {
          Func_08118088(effect->actors[0], 4);
          work->shakeFrames = 8;
          Func_081180e8(134);
        } else {
          if (i & 1)
            Func_08118088(effect->actors[0], 7);
          work->shakeFrames = 4;
          Audio_PlayCue(134);
        }
        for (j = 0; j != RECORD(RECORD_MOTES); j++)
          MOTES[i * 32 + j].variant = (Random16() & 7) + 15;
      }
      // The orb: six 20x48 cels, two frames each.
      if (frame >= start + 2 && frame < start + 14)
        draw[0](canvas, ORB_CELS + ((frame - start - 2) / 2) * 960,
                targetPos.x / 2 - 10, targetPos.y - 24, 20, 48);
    }
    Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);

    // The motes, until their life runs out, bouncing off the ground.
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    Func_081963ec(HEAP_BLITTER,
                  BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    Func_081963ec(HEAP_BLITTER_ALTERNATE,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    for (i = 0; i != MOTE_COUNT; i++) {
      struct EffectStep *mote = &MOTES[i];
      s32 size = mote->variant;

      if (size > 0) {
        EffectPosition_ApplyBaseAndYOffset(&mote->x, &pos);
        pos.x /= 2;
        size >>= 3;
        size++;
        draw[(i / 2) & 1](canvas, sheet + gData_08197410[size - 1],
                          pos.x - size / 2, pos.y - size, size, size * 2);
        BattleFxKernels_IntegrateVector3(mote, 60, -0x400);
        if (mote->y < 0x80000)
          mote->velocityY = -mote->velocityY / 2;
        mote->variant--;
      }
    }
    Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);

    if (frame > 3) {
      // The glints: three a frame around the target, each with its own
      // random flip, until frame 31.
      if (RECORD(RECORD_EXTRA) == EXTRA_GLINTS && frame <= 31) {
        s32 cel;

        EffectPosition_ApplyStepAndYOffset(effect->actors[0], &spot);
        cel = frame & 3;
        for (i = 0; i != 3; i++) {
          s32 angle = Random16() & 0xffff;
          s32 radius = (Random16() & 31) + 4;
          s32 x = spot.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
                  gData_08197492[cel] / 2;
          s32 y = spot.y - ((Trig_Cos(angle) * radius) >> 17) -
                  gData_08197498[cel] / 2;

          Func_08196404(HEAP_BLITTER_ALTERNATE, 7, 7,
                        Data_0819861b[Random16() & 3] | COMPOSITE_CLIP_X |
                            COMPOSITE_CLIP_Y,
                        BLIT_ADDITIVE);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas, work->sheet + gData_08197486[cel], x, y,
              gData_08197492[cel], gData_08197498[cel]);
          Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
        }
      }
      // The smoke: sixteen puffs rise from the target's feet, six cels of
      // four frames each.
      if (RECORD(RECORD_EXTRA) == EXTRA_RIPPLE) {
        Func_081963ec(
            HEAP_BLITTER,
            BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
        draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
        for (i = 0; i != 16; i++) {
          struct EffectStep *puff = &work->particles[i];

          if ((u32)puff->variant <= 23) {
            draw[0](canvas, SMOKE_CELS + (puff->variant / 4) * 0x480,
                    HI(puff->x) - 12, HI(puff->y) - 24, 24, 48);
            BattleFxKernels_IntegrateVector2(puff, 62, -0x1000);
          }
          puff->variant++;
        }
        Runtime_ReleaseHeapBlock(HEAP_BLITTER);
      }
    }
    Func_08158ce0(8, 8);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Teardown.
  if (RECORD(RECORD_EXTRA) == EXTRA_RIPPLE)
    Scheduler_RemoveCallback(Func_08152474);
  Scheduler_RemoveCallback(Func_08143000);
  Func_08143bb8();
}
