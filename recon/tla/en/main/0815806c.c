// Draft of BattleEffect_RunMars, effect kind 282: the Mars Djinn's puffs.

// On screen: a glowing orb swells over the targets for 24 frames; at frame
// 20 a small Mars Djinn appears in it, and from frame 24 it dives down and
// to the left, 4 pixels across and 8 down a frame. As it lands (frame 32,
// cue 0x91, an eight-frame shake) nine fiery puffs bloom in an oval around
// the targets, two frames apart, each a six-cel ember animation. From frame
// 40 a column of sixteen sparks rises from under each puff, wobbling as it
// climbs. At frame 38 every target flashes for 16 frames and recoils.

// Setup: the battle effect canvas and the max and additive blitter pair,
// the dot sheet in the sheet block, the Djinn sheet and the orb sheet in
// the effect work, then the canvas transfer callback; the puff positions
// and the 144 sparks are placed before the first frame.
// Frame loop, 112 frames, as above. At frame 56 the battle resumes and
// resolves the targets.
// Teardown: the callback and both blitters are released and the canvas
// layer is closed.

// Port of The Broken Seal's BattleEffect_RunMars (kind 252 there),
// with The Lost Age's heap slots and callback order and no work->effect
// store.

// Listing 421 instructions, draft 421, differ 22 (trial 0, 2026-10-15).
// Left: single rows one place apart, the native issuing pool and stack
// loads and call constants one slot earlier.

// Tried:
// ☀️'s body with The Lost Age's names, canvas read first   68
// the effect work read before the canvas, as Venus found   68 -> 22
// alchemy permute, 5 minutes                                24 at best

// 2026-10-26, under -mthumb-slow-loads: differ 14 of 421, the instructions
// and registers otherwise the native's. What remains is single rows where
// the native issues a spill or argument store before a load (the known
// store-then-load scheduling limit); barriers and one-pass blocks at each
// statement, statement moves and each local scoped to a block gain only
// the prologue's row or rows of that kind.

#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "resource.h"

// Byte offsets of the two blitter slots in gHeapSlots (unk64[1], unkb8[1]).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The sparks live in the map cell buffer: sixteen under each of nine puffs.
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)
#define PUFF_COUNT 9
#define SPARKS_PER_PUFF 16

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_ParticleSpritesA;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_GlowOrbSheet;
extern char ResourceId_EmberStreakSheet;
// Byte offsets of the dot cels in the particle sheet, by size - 1.
extern u16 gData_08197410[];
// The ember puff's six cels: ☀️'s gBattleEffect_PuffArcCellWidths, _CellHeights,
// _CellBiasY and _CellSourceOffsets.
extern u8 gData_08197467[];
extern u8 gData_0819746d[];
extern u8 gData_08197473[];
extern u16 gData_0819747a[];

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
void HeapSlot_Release(s32 slot);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);

void BattleEffect_RunMars(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  DrawRectangle blit[2];
  DrawRectangle *draw;
  struct EffectStep *puff;
  struct EffectStep *spark;
  s32 member;
  s32 frame;
  s32 i;
  s32 diveY;

  // Setup: the canvas, the blitter pair, the sheets.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet, work->sheet,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet + 0x320,
                             1, 1);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The puffs sit on an oval, alternately left and right of the middle,
  // each two frames behind the last; sixteen sparks wait under each one.
  member = 0;
  puff = work->particles;
  do {
    s32 angle = member << 11;

    puff->x = (Trig_Sin(angle) * 24) >> 16;
    puff->y = ((Trig_Cos(angle) * 4) >> 16) + 52;
    if (member & 1)
      puff->x = 32 - puff->x;
    else
      puff->x += 32;
    puff->variant = -(member * 2);
    for (i = 0; i != SPARKS_PER_PUFF; i++) {
      spark = &SPARKS[member * SPARKS_PER_PUFF + i];
      spark->x = (((Random16() & 15) + puff->x) - 8) << 16;
      spark->y = ((Random16() & 7) + 96) << 16;
      spark->velocityX = ((Random16() & 127) - 64) << 11;
      spark->velocityY = ((Random16() & 127) - 64) << 10;
      spark->z = Random16() & 0xffff;
      spark->velocityZ = Random16() & 0xffff;
    }
    puff++;
    member++;
  } while (member != PUFF_COUNT);

  Sound_PlayFar(0x88);
  frame = 0;
  diveY = -172;
  do {
    // The battle resumes and resolves the targets, hit cue 0x85.
    if (frame == 56)
      Func_081180e8(0x85);

    // The orb swells, a cel every four frames.
    if (frame <= 23)
      blit[0](canvas, work->sheet + 0x320 + (frame / 4) * 0x640, 40, 20, 40,
              40);
    // The Djinn appears in the orb, then dives down to the left.
    if (frame == 20)
      Iwram_CopyWords(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
    if (frame >= 20 && frame <= 31) {
      if (frame > 23)
        draw[1](canvas, work->sheet, 146 - frame * 4, diveY, 20, 40);
      else
        blit[0](canvas, work->sheet, 50, 20, 20, 40);
    }
    // It lands: the ember sheet replaces the Djinn's.
    if (frame == 32) {
      Sound_PlayFar(0x91);
      work->shakeFrames = 8;
      Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet,
                                 1, 1);
    }
    // Each puff plays its six cels, eight frames each, once its delay ends.
    if (frame > 31) {
      member = 0;
      puff = work->particles;
      do {
        if (puff->variant >= 0 && puff->variant <= 47) {
          s32 cel = puff->variant / 8;
          u32 width;

          blit[0](canvas, work->sheet + gData_0819747a[cel],
                  puff->x - ((width = gData_08197467[cel]) >> 1),
                  puff->y + gData_08197473[cel], width, gData_0819746d[cel]);
        }
        member++;
        puff->variant++;
        puff++;
      } while (member != PUFF_COUNT);
    }

    // The sparks rise from frame 40, a puff's column two frames after the
    // last, wobbling four pixels either way.
    spark = SPARKS;
    member = 0;
    do {
      if (frame >= (member / SPARKS_PER_PUFF) * 2 + 40) {
        s32 size;
        s32 x;
        s32 wobble;

        size = (member & 1) + 3;
        x = HI(spark->x) + ((Trig_Sin(spark->z) * 4) >> 16);
        draw[1](canvas, sheet + gData_08197410[size - 1], x - ((u32)size >> 1),
                HI(spark->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(spark, 64, -0x2000);
        wobble = spark->z;
        spark->z = wobble + 0x800;
        if (spark->z > 0xffff)
          spark->z = wobble + 0x800 - 0xffff;
      }
      member++;
      spark++;
    } while (member != PUFF_COUNT * SPARKS_PER_PUFF);

    // Every target flashes for sixteen frames and recoils.
    if (frame == 38) {
      for (member = 0; member != effect->count; member++) {
        Func_0814cd48(effect->actors[member], 7, 5, member, 16);
        Func_08118088(effect->actors[member], 6);
      }
    }

    Func_08158ce0(8, 8);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
    diveY += 8;
    frame++;
  } while (frame != 112);

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
