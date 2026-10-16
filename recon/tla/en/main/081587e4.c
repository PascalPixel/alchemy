// Draft of Func_081587e4, effect kind 203: the Venus Djinn's rock toss.

// On screen: the background slowly takes on the portal's colours from
// frame 17. Six rocks leave the caster one after another, every two
// frames, flung out sideways with a random lift and drift; sixteen frames
// after each leaves, it is pulled towards the first target and slowed,
// until it drops below y 20 there: then it vanishes with cue 134, an
// eight-frame shake, and the target flashing and recoiling. Each rock is
// drawn with a rotating highlight and four chips that change shape and
// flip at random every other frame. If the effect has its Djinn prelude,
// a small Venus Djinn hovers over the anchor point, drawn twice as bright
// for the first four frames, and rises off from frame 17.

// Setup: the battle effect canvas and the canvas preparer (kind 0, anchor
// 2, for the caster's side), a max blitter for the side, the portal sheet
// and the Djinn sheet in the effect work, the transfer callback, and the
// eight rocks' starting points and velocities at the caster.
// Frame loop, 96 frames, as above, with a heavy shake throughout.
// Teardown: the callback and the blitter are released and the canvas
// layer is closed.

// Port of The Broken Seal's BattleFx_RunVenusDjinnRockToss (kind 203 there
// too), with The Lost Age's heap slots, blitter modes (3 minus ☀️'s) and
// callback order, no work->effect store, the canvas preparer's packed
// arguments, and the chips' blitter built by BattleEffect_LoadWork
// directly. Other editions place a differently built version; only the
// English listing was compared.

// Listing 571 instructions, draft 571, differ 36 (2026-10-15). Left: the
// spill slots of target and of the screen point's address are swapped
// (sp+24 and sp+28; 8 rows), and rows one place apart (constants and
// stores around the heap base, the blitter's stacked arguments, pool
// loads).

// Tried:
// ☀️'s body with The Lost Age's changes                      42
// the preparer's two arrays declared before the position,
// as the native frame has them (screen at sp+84)          42 -> 36
// declaration order, a greedy sweep of 12 locals           36

#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "battle_session.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "io_reg.h"
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

#define ROCK_COUNT 6
#define CHIPS_PER_ROCK 4
// The chips' shapes and flips live in particles[32..63].
#define FIRST_CHIP 32

typedef DrawRectangle BlitterPair[2];

extern char ResourceId_PortalSheet;
extern char ResourceId_VenusDjinnSmallSheet;
// The chips: ☀️'s gRockToss_ChipFlips (a blitter's flags), and, by shape,
// gRockToss_ChipCells, _ChipX, _ChipY, _ChipWidths and _ChipHeights.
extern const u8 Data_08198524[];
extern const u16 Data_0819853a[];
extern const u8 Data_0819854c[];
extern const u8 Data_08198555[];
extern const u8 Data_08198528[];
extern const u8 Data_08198531[];

u32 Random16(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void BattleFx_BeginCanvasLayer(s32 bgControl);
// Prepares the canvas around an anchor and returns its screen point.
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
// Steps the background colours one shade toward a resource's palette.
void Func_0815f0a0(s32 resource);
// Builds a blitter in a heap slot: BattleEffect_LoadWork(slot, 7, 7,
// flags, mode), whose result it returns.
s32 Func_081963ec(s32 slot, s32 modeFlags);
// BattleEffect_LoadWork itself.
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  s32 mode);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
s32 EffectPosition_ApplyBaseAndYOffset(s32 *point,
                                       struct EffectPosition *position);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(void *camera, void *eye);
// Closes the canvas layer.
void Func_08143bb8(void);
// Copies the canvas to VRAM when a transfer is pending.
void Func_08143000(void);
// Counts down the actors' flash timers.
void Func_081434f8(void);
// An actor's recoil motion, through a far call.
void Func_08118088(s32 actor, s32 motion);
// Flashes an actor in a palette for some frames.
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
// Shakes the camera while work->shakeFrames lasts.
void Func_08158ce0(s32 randomMask, u32 shakeRange);

void Func_081587e4(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  BlitterPair draw;
  s32 *camera;
  struct MotionObject *target;
  struct MotionObject *actor;
  s32 screen[3];
  s32 otherPoint[3];
  struct EffectPosition position;
  s32 j;

  // Setup: the canvas, a max blitter for the side, the two sheets.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  BattleFx_BeginCanvasLayer(0);
  Func_0815585c(effect, PREPARE(0, effect->side, 2), screen, otherPoint);
  REG_BLDALPHA = 0x1010;
  if (effect->side == 1)
    Func_081963ec(HEAP_BLITTER,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                        COMPOSITE_FLIP_X));
  else
    Func_081963ec(HEAP_BLITTER,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_PortalSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                             work->sheet + 0x65c0, 1, 1);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

  // The rocks start at the caster, 66 high, flung out to the side away
  // from the caster with a random lift and drift.
  actor = GetBattleObjectSlotFar(effect->actor)->object;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0; i != 8; i++) {
    struct EffectStep *rock = &work->particles[i];

    rock->x = actor->x;
    rock->y = 0x420000;
    rock->z = actor->z;
    rock->velocityX = (i * 0x500000) >> 5;
    rock->velocityY = (s32)(((Random16() & 127) - 64) << 16) >> 6;
    rock->velocityZ = (s32)(((Random16() & 255) - 127) << 16) >> 5;
    if (rock->x > 0)
      rock->velocityX = -rock->velocityX;
    rock->variant = 1;
  }

  for (frame = 0; frame != 96; frame++) {
    // The background takes on the portal's colours from frame 17.
    if (frame > 16)
      Func_0815f0a0((s32)&ResourceId_PortalSheet);

    // With its Djinn prelude, the Djinn hovers over the anchor and rises
    // off from frame 17; the first four frames add a bright copy.
    if (effect->prelude == 1) {
      s32 angle = frame << 11;
      s32 x = ((-Trig_Sin(angle) << 2) >> 16) + screen[0] / 2 - 10;
      s32 y = ((Trig_Cos(angle) << 1) >> 16) + screen[1] - 22;

      if (frame > 16)
        y = y - frame * 2 + 32;
      if (effect->side == 1)
        Func_081963ec(HEAP_BLITTER_ALTERNATE,
                      BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                 COMPOSITE_CLIP_Y |
                                                 COMPOSITE_FLIP_X));
      else
        Func_081963ec(
            HEAP_BLITTER_ALTERNATE,
            BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      if (frame <= 3)
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, work->sheet + 0x65c0,
                                                 x, y, 20, 40);
      Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
      draw[0](canvas, work->sheet + 0x65c0, x, y, 20, 40);
    }

    // Every other frame each chip takes a new shape (3 to 8) and flip.
    if ((frame & 1) == 0) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *chip = &work->particles[FIRST_CHIP + i];

        chip->velocityX = Random16() % 6 + 3;
        chip->velocityY = Data_08198524[Random16() & 3];
      }
    }

    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(camera, camera + 3);
    for (i = 0; i != ROCK_COUNT; i++) {
      struct EffectStep *rock = &work->particles[i];

      if (rock->variant == 1) {
        // Rock i flies from frame 2i: the rock, its highlight (two shapes
        // in turn, two frames each) and its four chips.
        if (frame > i * 2) {
          s32 x;
          s32 y;

          EffectPosition_ApplyBaseAndYOffset(&rock->x, &position);
          position.x >>= 1;
          x = position.x - 12;
          y = position.y - 24;
          draw[0](canvas, work->sheet, x, y, 24, 48);
          if ((frame & 3) <= 1)
            draw[0](canvas, work->sheet + Data_0819853a[1],
                    x + Data_0819854c[1], y + Data_08198555[1],
                    Data_08198528[1], Data_08198531[1]);
          else
            draw[0](canvas, work->sheet + Data_0819853a[2],
                    x + Data_0819854c[2], y + Data_08198555[2],
                    Data_08198528[2], Data_08198531[2]);
          for (j = 0; j != CHIPS_PER_ROCK; j++) {
            struct EffectStep *chip =
                &work->particles[FIRST_CHIP + i * CHIPS_PER_ROCK + j];
            s32 chipX;
            s32 chipY;

            // A max blitter with the chip's own flip, for this chip only.
            Func_08196404(HEAP_BLITTER_ALTERNATE, 7, 7, chip->velocityY,
                          BLIT_MAX);
            draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
            if (chip->velocityY & COMPOSITE_FLIP_X)
              chipX = x - Data_08198528[chip->velocityX] -
                      Data_0819854c[chip->velocityX] + 24;
            else
              chipX = x + Data_0819854c[chip->velocityX];
            if (chip->velocityY & COMPOSITE_FLIP_Y)
              chipY = y - Data_08198531[chip->velocityX] -
                      Data_08198555[chip->velocityX] + 48;
            else
              chipY = y + Data_08198555[chip->velocityX];
            draw[1](canvas, work->sheet + Data_0819853a[chip->velocityX], chipX,
                    chipY, Data_08198528[chip->velocityX],
                    Data_08198531[chip->velocityX]);
            Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
          }
          rock->x += rock->velocityX;
          rock->y += rock->velocityY;
          rock->z += rock->velocityZ;
        }
        // Sixteen frames later it is pulled towards the target, 20 high,
        // and slowed until frame 2i + 85; below y 20 it hits.
        if (frame > i * 2 + 16) {
          rock->velocityX += (target->x - rock->x) >> 8;
          rock->velocityY += (0x140000 - rock->y) >> 8;
          rock->velocityZ += (target->z - rock->z) >> 8;
          if (frame < i * 2 + 85) {
            rock->velocityX = rock->velocityX * 60 / 64;
            rock->velocityY = rock->velocityY * 60 / 64;
            rock->velocityZ = rock->velocityZ * 60 / 64;
          }
          if (rock->y < 0x140000) {
            work->shakeFrames = 8;
            rock->variant = 0;
            Audio_PlayCue(134);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
            Func_08118088(effect->actors[0], 4);
          }
        }
      }
    }
    Func_08158ce0(16, 16);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Teardown.
  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  Func_08143bb8();
}
