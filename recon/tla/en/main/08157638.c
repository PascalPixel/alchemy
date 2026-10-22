// Draft of BattleEffect_RunMercury, effect kind 281: the Mercury Djinn's flames.

// On screen: a small Mercury Djinn circles over the targets, one turn every
// 32 frames, on a radius of 64 - 2 x frame pixels (half that across) that
// shrinks through the middle at frame 32 and opens out on the other side.
// At frames 4, 7, 10, 21 and 34 it throws a blue flame that shoots left 12
// pixels a frame and shatters on its fifth frame into 32 dots, with cue
// 133 and a shake; six frames after each throw every target flashes and
// recoils. The dots shrink with age and fall back under gravity.

// Setup: the battle effect canvas and the max and additive blitter pair,
// the dot sheet in the sheet block, the Djinn sheet in the effect work,
// and the blue flame sheet turned on its side (decoded into the map cell
// buffer, then transposed into the effect work) so the flames fly across.
// Frame loop, 96 frames: move, draw and shatter the flames, draw the Djinn
// and throw on the launch frames, then draw and move the dots. At frame 56
// the battle resumes and resolves the targets.
// Teardown: the callback and both blitters are released and the canvas
// layer is closed.

// Port of The Broken Seal's BattleEffect_RunMercury (kind 251
// there), with The Lost Age's heap slots and callback order and no
// work->effect store.

// Listing 415 instructions, draft 415, differ 30 (trial 0, 2026-10-15).
// Left: 15 single rows one place apart, the native issuing pool and stack
// loads and call constants one slot earlier.

// Tried:
// ☀️'s body with The Lost Age's names                       30
// alchemy permute, 5 minutes                                43 at best

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

// The dots live in the map cell buffer: 32 for each of the five flames.
#define DOTS ((struct EffectStep *)Ram_MapCellBuffer)
#define DOTS_PER_FLAME 32
#define FLAME_COUNT 5

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_ParticleSpritesA;
extern char ResourceId_MercuryDjinnSmallSheet;
extern char ResourceId_BlueFlameSheet;
// The frames the Djinn throws its five flames on (☀️'s
// gDjinnAttacks_LaunchFrames).
extern const u8 Data_0819850e[];
// Byte offsets of the dot cels in the particle sheet, by size - 1.
extern const u16 gData_08197410[];

u32 Random16(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void BattleEffect_BeginCanvasLayer(s32 bgControl);
// Closes the canvas layer.
void BattleEffect_EndCanvasLayer(void);
// Copies the canvas to VRAM when a transfer is pending.
void BattleEffect_ProcessPendingGraphicsTransfer(void);
// Counts down the actors' flash timers.
void ObjectGroup_TickMemberTimers(void);
// Fetches the max and additive blitters, flipped or not.
void BattleEffect_FetchRectangleBlitters(s32 flip, DrawRectangle *out);
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 width, s32 height);
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
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);

void BattleEffect_RunMercury(struct BattleEffectArgument *effect) {
  struct HeapSlots *slots;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  DrawRectangle blit[2];
  DrawRectangle *draw;
  struct EffectStep *flame;
  struct EffectStep *dot;
  u8 *cells;
  s32 i;
  s32 j;

  // Setup: the canvas, the blitter pair, the sheets.
  slots = Ram_HeapSlots;
  work = (struct BattleEffectWork *)slots->battleFxWork[0];
  canvas = slots->battleFxWork[1];
  sheet = slots->unk64[0];
  BattleEffect_BeginCanvasLayer(0);
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);

  // The flame picture is the blue flame sheet turned on its side.
  cells = Ram_MapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, cells, 1, 1);
  Graphics_TransposeCopy(cells, work->sheet + 0x2a8, 17, 104);
  Graphics_TransposeCopy(cells + 0x6e8, work->sheet + 0x990, 34, 65);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // No flame and no dot is alive yet.
  for (i = 0; i != 8; i++)
    work->particles[i].variant = -1;
  for (i = 0; i != 512; i++)
    DOTS[i].variant = -1;

  Audio_PlayCue(162);
  frame = 0;
  do {
    // The battle resumes and resolves the targets, hit cue 133.
    if (frame == 56)
      Func_081180e8(133);

    // Flames fly 12 pixels left a frame and shatter on their fifth frame.
    for (i = 0; i != FLAME_COUNT; i++) {
      flame = &work->particles[i];
      if (flame->variant != -1) {
        blit[0](canvas, work->sheet + 0x990, flame->x - 16, flame->y - 17, 65,
                34);
        flame->x -= 12;
        flame->variant++;
        if (flame->variant == 5) {
          Audio_PlayCue(133);
          work->shakeFrames = 4;
          for (j = 0; j != DOTS_PER_FLAME; j++) {
            s32 direction;
            s32 speed;

            dot = &DOTS[i * DOTS_PER_FLAME + j];
            direction = Random16() & 0xffff;
            speed = (Random16() & 0x1ff) + 256;
            dot->x = flame->x << 16;
            dot->y = flame->y << 16;
            dot->velocityX = (Trig_Sin(direction) * speed) >> 8;
            dot->velocityY = (Trig_Cos(direction) * speed) >> 7;
            // Each dot lives 32 to 47 frames.
            dot->variant = (Random16() & 15) + 32;
          }
        }
      }
    }

    // The Djinn circles on a radius that shrinks through the middle.
    if (frame <= 95) {
      s32 x = ((Trig_Sin(frame << 11) * (64 - frame * 2)) >> 17) + 96;
      s32 y = ((Trig_Cos(frame << 11) * (64 - frame * 2)) >> 16) + 60;

      draw[1](canvas, work->sheet, x - 10, y - 17, 20, 34);
      for (i = 0; i != FLAME_COUNT; i++) {
        // On its launch frame a free flame leaves the Djinn's hand.
        if (frame == Data_0819850e[i]) {
          struct EffectStep *shot = &work->particles[i];

          if (shot->variant == -1) {
            shot->x = x - 8;
            shot->velocityX = x;
            shot->y = y;
            shot->variant = 0;
            break;
          }
        }
        // Six frames later every target flashes and recoils.
        if (frame == Data_0819850e[i] + 6) {
          for (j = 0; j != effect->count; j++) {
            Func_0814cd48(effect->actors[j], 7, 5, j, 6);
            Func_08118088(effect->actors[j], 6);
          }
        }
      }
    }

    // Each live dot shrinks from 4 to 2 pixels and falls.
    for (i = 0; i != 256; i++) {
      dot = &DOTS[i];
      if (dot->variant != -1) {
        s32 size = dot->variant / 16 + 2;

        draw[1](canvas, sheet + gData_08197410[size - 1], HI(dot->x) - size / 2,
                HI(dot->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(dot, 62, 0x2000);
        dot->variant--;
      }
    }

    Func_08158ce0(4, 4);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
    frame++;
  } while (frame != 96);

  // Teardown.
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
