// Draft of BattleFx_RunVenus, effect kind 280: the Venus Djinn's columns.

// On screen: a small Venus Djinn swings down over the targets on a
// narrowing arc while ten columns burst out of the ground one after
// another, one every four frames from frame 16, and climb off the top of
// the canvas. Each burst throws a spray of sixteen dots that arc and fall
// back under gravity, shakes the screen, and makes every target flash and
// recoil; every other burst sounds cue 133.

// Setup: the battle effect canvas, a max blitter (clipped, flipped
// vertically) for the columns and an additive one for the Djinn and the
// dots, the dot sheet in the sheet block and the Djinn and column sheets
// in the effect work, then the canvas transfer callback.
// Frame loop, 96 frames: draw the Djinn on its arc, raise and draw each
// column that has started, start a column's burst on its first frame,
// and draw and move every live dot. At frame 56 the battle resumes and
// resolves the targets.
// Teardown: the callback and both blitters are released and the canvas
// layer is closed.

// Port of The Broken Seal's BattleFx_RunVenus (kind 250 there),
// with The Lost Age's heap slots, blitter modes (3 minus ☀️'s) and callback
// order, and no work->effect store: the effect argument stays live instead.

// Listing 367 instructions, draft 367, differ 24 (2026-10-15). Left: nine
// places where the native issues a load one slot earlier than agscc: the
// sheet slot before work moves to r9, the REG_BLDALPHA value before the
// register's address is built, the dot loop's pool loads before its zero
// and minus one, ldr [r2, #4] before adds r5, #255 in the burst, a stack
// reload before movs r3, #15 after Random16, and the column loop's two
// register copies after its stores.

// Tried:
// ☀️'s body with The Lost Age's names                       73
// effect work read before the canvas                        54 -> 46
// a start variable for each column's delay, stepped
// before i (the delay and angle take the native's slots)  46 -> 28
// one empty asm in the prologue                             32
// Func_081963ec returning s32, as LoadWork returns its
// store-flag result (the r1-before-r0 rows close)         28 -> 24
// alchemy permute, 5 and 8 minutes: 24, only by a false
// u16 return type on the loader                           not kept

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

// The Lost Age numbers the blitter modes the other way round from The
// Broken Seal's COMPOSITE_ modes; Func_081963ec takes mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// The dots live in the map cell buffer, 512 of them.
#define DOTS ((struct EffectStep *)Ram_MapCellBuffer)
#define DOT_COUNT 512
#define DOTS_PER_COLUMN 32
#define COLUMN_COUNT 10

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_ParticleSpritesA;
extern char ResourceId_VenusDjinnSmallSheet;
extern char ResourceId_BlueFlameSheet;
// Byte offsets of the dot cels in the particle sheet, by size - 1.
extern const u16 gData_08197410[];

u32 Random16(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void BattleFx_BeginCanvasLayer(s32 bgControl);
// Closes the canvas layer.
void Func_08143bb8(void);
// Copies the canvas to VRAM when a transfer is pending.
void Func_08143000(void);
// Counts down the actors' flash timers.
void Func_081434f8(void);
// Builds a blitter in a heap slot: BattleFx_LoadWork(slot, 7, 7,
// flags, mode), whose result it returns.
s32 Func_081963ec(s32 slot, s32 modeFlags);
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
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);

void BattleFx_RunVenus(struct BattleEffectArgument *effect) {
  struct HeapSlots *slots;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *sheet;
  struct EffectStep *column;
  struct EffectStep *dot;
  s32 i;
  s32 j;
  s32 start;
  s32 angle;

  // Setup: the canvas, the two blitters, the three sheets.
  slots = Ram_HeapSlots;
  work = (struct BattleEffectWork *)slots->battleFxWork[0];
  canvas = slots->battleFxWork[1];
  sheet = slots->unk64[0];
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Func_081963ec(HEAP_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                      COMPOSITE_FLIP_Y));
  Func_081963ec(HEAP_BLITTER_ALTERNATE,
                BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)slots->unk64[1];
  draw[1] = (DrawRectangle)slots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet,
                             work->sheet + 0x2f8, 1, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

  // Each column starts somewhere in the canvas's 64 pixels, on the ground.
  for (i = 0; i != 32; i++) {
    column = &work->particles[i];
    column->x = Random16() & 63;
    column->y = 104;
  }
  // No dot is alive yet.
  for (i = 0; i != DOT_COUNT; i++)
    DOTS[i].variant = -1;

  Audio_PlayCue(141);
  frame = 0;
  angle = 0x8000;
  do {
    // The Djinn swings on an arc that narrows by two pixels a frame.
    if (frame <= 79) {
      s32 x = (Trig_Sin(angle) * 24) >> 16;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      draw[1](canvas, work->sheet, x + 22, y + 29, 20, 38);
    }
    // The battle resumes and resolves the targets, hit cue 133.
    if (frame == 56)
      Func_081180e8(133);

    // Column i starts at frame 16 + 4i and climbs 12 pixels a frame.
    for (i = 0, start = 16; i != COLUMN_COUNT; start += 4, i++) {
      column = &work->particles[i];
      if (frame >= start) {
        draw[0](canvas, work->sheet + 0x9e0, column->x - 17, column->y - 32, 34,
                65);
        if (frame == start) {
          // The burst: sixteen dots sprayed upward from the column's foot.
          for (j = 0; j != 16; j++) {
            s32 direction;
            s32 speed;

            dot = &DOTS[i * DOTS_PER_COLUMN + j];
            direction = (Random16() & 0x7fff) + 0x4000;
            speed = (Random16() & 0x1ff) + 256;
            dot->x = column->x << 16;
            dot->y = (column->y + 16) << 16;
            dot->velocityX = (Trig_Sin(direction) * speed) >> 7;
            dot->velocityY = (Trig_Cos(direction) * speed) >> 6;
            dot->variant = (Random16() & 15) + 32;
          }
          if (i & 1)
            Audio_PlayCue(133);
          work->shakeFrames = 4;
          // Every target flashes for six frames and recoils.
          for (j = 0; j != effect->count; j++) {
            Func_0814cd48(effect->actors[j], 7, 5, j, 6);
            Func_08118088(effect->actors[j], 6);
          }
        }
        column->y -= 12;
      }
    }

    // Each live dot shrinks from 4 to 2 pixels as its life runs out.
    for (i = 0; i != DOT_COUNT; i++) {
      dot = &DOTS[i];
      if (dot->variant != -1) {
        s32 size = dot->variant / 16 + 2;

        draw[1](canvas, sheet + gData_08197410[size - 1], HI(dot->x) - size / 2,
                HI(dot->y) - size, size, size * 2);
        BattleFxKernels_IntegrateVector2(dot, 62, 0x2000);
        dot->variant--;
      }
    }

    Func_08158ce0(4, 4);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
    angle -= 0x800;
    frame++;
  } while (frame != 96);

  // Teardown.
  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  Func_08143bb8();
}
