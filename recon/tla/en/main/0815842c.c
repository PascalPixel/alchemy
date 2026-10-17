// Draft of Func_0815842c, effect kind 207: a spray of ice chips over a
// rising wave.

// On screen: a band of water five tiles wide rises 32 pixels from the
// bottom of the canvas over the first eight frames and drifts sideways a
// pixel every four frames, away from the caster's side. From frame 4, 64
// ice chips spray up from the first target, four more every frame, tumble
// through four cels and fall back. If the effect has its Djinn prelude, a
// small Mercury Djinn hovers over the anchor point, drawn twice as bright
// for the first four frames. At frame 8 the screen shakes, the battle
// resumes with hit cue 0x86, and the first target flashes for sixteen
// frames and recoils. The last sixteen frames fade the layer out.

// Setup: the battle effect canvas, the canvas preparer (kind 1, anchor 2,
// for the caster's side), the max and additive blitter pair flipped for
// that side, the water sheet in the effect work and the transfer callback;
// the chips are placed at the first target before the first frame. The ice
// chip and Djinn sheets load on frame 1.
// Frame loop, 64 frames, as above.
// Teardown: the callback and both blitters are released and the canvas
// layer is closed.

// Port of The Broken Seal's BattleFx_RunHail (kind 207 there
// too), with The Lost Age's heap slots and callback order, no work->effect
// store, and the canvas preparer's packed arguments: kind | side << 4 |
// anchor << 5, with the screen point returned in a three-word array.

// Listing 422 instructions, draft 422, differ 20 (trial 0, 2026-10-15).
// Left: 10 single rows one place apart, constant and stack loads around
// the blitter calls.

// Tried:
// ☀️'s body with The Lost Age's names; the cel sizes as u32
// (they divide unsigned)                                  20

#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "io_reg.h"
#include "ram_buffer.h"

// Byte offsets of the two blitter slots in gHeapSlots (unk64[1], unkb8[1]).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The canvas preparer's packed arguments.
#define PREPARE(kind, side, anchor) ((kind) | (side) << 4 | (anchor) << 5)

// The ice chips live in the map cell buffer.
#define CHIPS ((struct EffectStep *)Ram_MapCellBuffer)
#define CHIP_COUNT 64

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_WaterSpraySheet;
extern char ResourceId_IceChipSheet;
extern char ResourceId_MercuryDjinnSmallSheet;
// The ice chip's four cels: ☀️'s gVolley_PelletOffsets,
// gHail_CellWidths and gHail_CellHeights.
extern const u16 Data_0819851c[];
extern const u8 Data_08198513[];
extern const u8 Data_08198517[];

u32 Random16(void);
void WaitFrames(s32 frames);
void BattleFx_BeginCanvasLayer(s32 bgControl);
// Prepares the canvas around an anchor and returns its screen point.
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
// Fetches the max and additive blitters, flipped or not.
void Func_08144aac(s32 flip, DrawRectangle *out);
// Closes the canvas layer.
void Func_08143bb8(void);
// Copies the canvas to VRAM when a transfer is pending.
void Func_08143000(void);
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
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, s32 *position);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);

void Func_0815842c(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 screen[3];
  s32 otherPoint[3];
  s32 spawn[3];
  DrawRectangle blit[2];
  struct EffectStep *chip;
  s32 i;
  s32 frame;
  s32 rise;

  // Setup: the canvas, the blitter pair for the caster's side, the water.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleFx_BeginCanvasLayer(0);
  Func_0815585c(effect, PREPARE(1, effect->side, 2), screen, otherPoint);
  Func_08144aac(effect->side, blit);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

  // The chips start in a 16-pixel spread at the first target and fly up
  // at random, each to live 32 to 47 frames.
  EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], spawn);
  for (i = 0; i != CHIP_COUNT; i++) {
    s32 angle;
    s32 speed;

    angle = (Random16() & 0x7fff) + 0x4000;
    speed = (Random16() & 0x1ff) + 0x80;
    CHIPS[i].x = ((spawn[0] / 2 + (Random16() & 0xf)) - 8) << 16;
    CHIPS[i].y = (spawn[1] + 8) << 16;
    CHIPS[i].velocityX = (Trig_Sin(angle) * speed) >> 9;
    CHIPS[i].velocityY = (Trig_Cos(angle) * speed) >> 6;
    CHIPS[i].z = Random16() & 0x7f;
    CHIPS[i].velocityZ = Random16() & 0x7f;
    CHIPS[i].variant = (Random16() & 0xf) + 32;
  }

  for (frame = 0; frame != 64; frame++) {
    // The last sixteen frames fade the layer out.
    if (frame > 47)
      REG_BLDALPHA = (64 - frame) | 0x1000;
    if (frame == 1) {
      Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet,
                                 work->sheet + 0x400, 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                                 work->sheet + 0x65c0, 1, 0);
    }

    // With its Djinn prelude, the Djinn hovers over the anchor.
    if (effect->prelude == 1) {
      s32 orbit;
      s32 x;
      s32 y;

      orbit = frame << 11;
      x = ((-Trig_Sin(orbit) << 2) >> 16) + screen[0] / 2 - 10;
      y = ((Trig_Cos(orbit) << 1) >> 16) + screen[1] - 22;
      // Never true in 64 frames; ☀️ has the same test.
      if (frame > 69)
        y = (y - frame * 2) + 138;
      blit[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
      if (frame <= 3)
        blit[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
    }

    // Chip i flies from frame 4 + i / 4, through the two blitters in turn.
    for (i = 0, chip = CHIPS; i != CHIP_COUNT; i++, chip++) {
      if (frame >= i / 4 + 4) {
        s32 cel;
        u32 width;
        u32 height;

        cel = (chip->z / 128) & 3;
        blit[i & 1](canvas, work->sheet + 0x400 + Data_0819851c[cel],
                    HI(chip->x) - (width = Data_08198513[cel]) / 2,
                    HI(chip->y) - (height = Data_08198517[cel]) / 2, width,
                    height);
        BattleFxKernels_IntegrateVector3(chip, 63, 0x1000);
      }
    }

    // The hit: an eight-frame shake, the battle resumes, the first target
    // flashes for sixteen frames and recoils.
    if (frame == 8) {
      work->shakeFrames = frame;
      Func_081180e8(0x86);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
      Func_08118088(effect->actors[0], 3);
    }

    // The water rises 4 pixels a frame to 32 and drifts away from the
    // caster's side.
    rise = frame * 4;
    if (rise > 32)
      rise = 32;
    if (effect->side == 0) {
      for (i = 0; i != 5; i++)
        blit[0](canvas, work->sheet, (i << 5) - (frame / 4 & 31), 120 - rise,
                32, 32);
    } else {
      for (i = 0; i != 5; i++)
        blit[0](canvas, work->sheet, ((i << 5) + (frame / 4 & 31)) - 32,
                120 - rise, 32, 32);
    }

    Func_08158ce0(4, 8);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Teardown.
  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  Func_08143bb8();
}
