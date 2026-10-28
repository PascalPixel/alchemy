// Draft of BattleEffect_RunJupiter, effect kind 283: the Jupiter Djinn's fall.

// On screen: a small Jupiter Djinn circles over the targets, 11.25 degrees
// a frame, on a radius of 64 - 2 x frame pixels (half that across) that
// shrinks through the middle at frame 32 and opens out on the other side.
// Meanwhile eight balls of flame drop one after another, every four frames
// from frame 8, from the upper right to the lower left (6 pixels left and
// 12 down a frame). Each bursts into 32 dots when it passes y 95, with cue
// 133 and a shake, and every target flashes and recoils. The dots shrink
// with age and fall back under gravity.

// Setup: the battle effect canvas and the max and additive blitter pair,
// the dot sheet in the sheet block, the Djinn and flame ball sheets in the
// effect work, then the canvas transfer callback; each ball starts at a
// random point above the canvas (x 64 to 127, y -80 to -17).
// Frame loop, 96 frames, as above. At frame 56 the battle resumes and
// resolves the targets.
// Teardown: the callback and both blitters are released and the canvas
// layer is closed.

// Port of The Broken Seal's BattleEffect_RunJupiter (kind 253 there),
// with The Lost Age's heap slots and callback order and no work->effect
// store.

// Listing 354 instructions, draft 354, differ 32. Left: about a dozen
// reordered pairs, nearly all a load placed one row earlier by the native
// (heap words, pool constants, stack reloads), and one branch over a block
// two bytes longer.

// Tried:
// ☀️'s body with The Lost Age's names, work read first     32
// (trial 0, 2026-10-15: no further change)

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

// The dots live in the map cell buffer: 32 for each of the eight balls.
#define DOTS ((struct EffectStep *)Ram_MapCellBuffer)
#define DOT_COUNT 512
#define DOTS_PER_BALL 32
#define BALL_COUNT 8

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_ParticleSpritesA;
extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_FlameballSheet;
// Byte offsets of the dot cels in the particle sheet, by size - 1.
extern const u16 gData_08197410[];

u32 Random16(void);
void WaitFrames(s32 frames);
void Sound_PlayFar(s32 cue);
void BattleEffect_StartCanvasLayer(s32 bgControl);
// Closes the canvas layer.
void BattleEffect_EndCanvasLayer(void);
// Copies the canvas to VRAM when a transfer is pending.
void BattleEffect_ProcessPendingGraphicsTransfer(void);
// Counts down the actors' flash timers.
void ObjectGroup_CountDownMemberTimers(void);
// Fetches the max and additive blitters, flipped or not.
void BattleEffect_FetchRectangleBlitters(s32 flip, DrawRectangle *out);
// BattleEventRuntime_StartPhase through its far call.
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
s32 Task_Add(void (*callback)(void), s32 order);
s32 Task_Remove(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);

void BattleEffect_RunJupiter(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  DrawRectangle blit[2];
  DrawRectangle *draw;
  struct EffectStep *ball;
  struct EffectStep *dot;
  s32 i;
  s32 j;
  s32 angle;

  // Setup: the canvas, the blitter pair, the sheets.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet,
                             work->sheet + 0x320, 1, 0);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The balls start above the canvas to the right; no dot is alive yet.
  for (i = 0; i != 32; i++) {
    ball = &work->particles[i];
    ball->x = (Random16() & 63) + 64;
    ball->y = (Random16() & 63) - 80;
  }
  for (i = 0; i != DOT_COUNT; i++)
    DOTS[i].variant = -1;

  Sound_PlayFar(171);
  frame = 0;
  angle = 0x8000;
  do {
    // The battle resumes and resolves the targets, hit cue 133.
    if (frame == 56)
      Func_081180e8(133);

    // The Djinn circles on a radius that shrinks through the middle.
    if (frame <= 95) {
      s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      blit[0](canvas, work->sheet, x + 86, y + 28, 20, 40);
    }

    // Ball i drops from frame 8 + 4i until it passes y 95 and bursts.
    for (i = 0; i != BALL_COUNT; i++) {
      ball = &work->particles[i];
      if (frame >= i * 4 + 8 && ball->y <= 95) {
        blit[0](canvas, work->sheet + 0x320, ball->x - 20, ball->y - 32, 40,
                64);
        ball->x -= 6;
        ball->y += 12;
        if (ball->y > 95) {
          for (j = 0; j != DOTS_PER_BALL; j++) {
            s32 direction;
            s32 speed;

            dot = &DOTS[i * DOTS_PER_BALL + j];
            direction = Random16() & 0xffff;
            speed = (Random16() & 0x1ff) + 256;
            dot->x = ball->x << 16;
            dot->y = ball->y << 16;
            dot->velocityX = (Trig_Sin(direction) * speed) >> 7;
            dot->velocityY = (Trig_Cos(direction) * speed) >> 6;
            // Each dot lives 32 to 47 frames.
            dot->variant = (Random16() & 15) + 32;
          }
          Sound_PlayFar(133);
          work->shakeFrames = 4;
          // Every target flashes for six frames and recoils.
          for (j = 0; j != effect->count; j++) {
            Func_0814cd48(effect->actors[j], 7, 5, j, 6);
            Func_08118088(effect->actors[j], 6);
          }
        }
      }
    }

    // Each live dot shrinks from 3 to 1 pixel and falls.
    for (i = 0; i != DOT_COUNT; i++) {
      dot = &DOTS[i];
      if (dot->variant != -1) {
        s32 size = dot->variant / 16 + 1;

        draw[1](canvas, sheet + gData_08197410[size - 1], HI(dot->x) - size / 2,
                HI(dot->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(dot, 62, 0x2000);
        dot->variant--;
      }
    }

    Func_08158ce0(4, 4);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
    angle -= 0x800;
    frame++;
  } while (frame != 96);

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
