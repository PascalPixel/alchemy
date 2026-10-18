// Draft of BattleFx_RunSpire, effect kind 2: shattering rocks.

// On screen: boulders drop out of the sky onto the targets, one after
// another, from the variant's row of drop frames. Each falls faster every
// frame and comes to rest at y 50; eighteen frames after its drop it
// shatters into 21 shards that fly up, tumble and fall back under gravity,
// with cue 134, a short shake, and every target flashing for eight frames.
// The first boulder's crash lets the battle resume with hit cue 134; 48
// frames after the last drop it resumes again with hit cue 133.

// Setup: the battle effect canvas on BG2 at a scale of one, centred
// between the first and the last target, the boulder and shard sheet in
// the effect work, a transparent blitter, and the transfer callback (copy
// mode); each boulder starts 64 pixels above the canvas and each shard at
// its place in its boulder.
// Frame loop, the last drop frame + 65 frames: for each boulder, draw and
// drop it, or draw and move its shards once it has shattered.
// Teardown: the callback and the blitter are released and the canvas
// layer is closed.

// Port of The Broken Seal's BattleFx_RunSpire (kind 2 there
// too), with The Lost Age's heap slots, blitter mode (3 minus ☀️'s) and
// callback order, no work->effect store, and The Lost Age's timing: the
// effect runs 65 frames past the last drop, not 80, and the first crash
// resumes the battle where ☀️ only plays the cue.

// Listing 409 instructions, draft 409, differ 32 (trial 1, 2026-10-15).
// Left: 11 single-row order differences (the native issues pool loads
// before neighbouring immediates, the effect spill before the heap base,
// movs r1 before movs r0 for the loader), and the first literal pool lands
// one call later, which shifts four pc offsets.

// Tried:
// ☀️'s body with The Lost Age's names                       34
// BattleFx_LoadBlitter returning s32 (LoadWork returns its
// store-flag result)                                      34 -> 32
// an empty asm at the top fixes the first row; the call
// argument against the canvas load cannot be reached by
// a barrier                                               30, not kept
// the loader's parameter types                              no change
// alchemy permute, 6 minutes: only a canvas load moved
// past the call, which the native does not do             not kept

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"

// Byte offset of the blitter slot in gHeapSlots (unk64[1]).
#define HEAP_BLITTER 104

// The Lost Age numbers the blitter modes the other way round from The
// Broken Seal's COMPOSITE_ modes; BattleFx_LoadBlitter takes mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// The shards live in the map cell buffer, 21 for each boulder.
#define SHARDS ((struct EffectStep *)Ram_MapCellBuffer)
#define SHARDS_PER_ROCK 21
// A boulder shatters this many frames after it starts to fall.
#define FALL_FRAMES 18

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_BoulderSheet;
// ☀️'s gSpire_RockCounts, by variant.
extern u8 Data_081985b0[];
// ☀️'s gSpire_ShardOffsets: x, y in the boulder for each shard.
extern u8 Data_08198566[];
// ☀️'s gSpire_RockX and gSpire_DropFrames, by boulder.
extern s8 Data_081985a6[];
extern u8 Data_081985ab[];
// The shard cels: ☀️'s gSpire_ShardCells, _ShardWidths and
// _ShardHeights.
extern u16 gData_081974bc[];
extern u8 gData_0819749e[];
extern u8 gData_081974ad[];

void BattleFx_BeginCanvasLayer(s32 bgControl);
// Closes the canvas layer.
void BattleFx_EndCanvasLayer(void);
// Builds a blitter in a heap slot: BattleFx_BuildBlitter(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleFx_LoadBlitter(s32 slot, s32 modeFlags);
// Copies the canvas to VRAM when a transfer is pending.
s32 BattleFx_ProcessPendingGraphicsTransfer(void);
s32 Scheduler_AddOrUpdateCallback(s32 (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(s32 (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
// BattleEventRuntime_BeginPhase through its far call.
void Func_081180e8(s32 hitCue);
u32 Random16(void);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
// Flashes an actor in a palette for some frames.
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
// Shakes the camera while work->shakeFrames lasts.
void Func_08158ce0(s32 randomMask, s32 shakeRange);
// Counts down the actors' flash timers.
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);

void BattleFx_RunSpire(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  s32 rocks;
  struct EffectPosition first;
  struct EffectPosition final;
  struct EffectStep *rock;
  struct EffectStep *shard;
  s32 frame;
  s32 i;
  s32 j;
  s32 cel;
  u32 width;
  u32 height;

  // Setup: the canvas on BG2, the sheet, a transparent blitter.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  BattleFx_LoadBlitter(HEAP_BLITTER,
                BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // The canvas is centred between the first and the last target.
  EffectPosition_ApplyStepAndYOffset(effect->actors[0], &first);
  EffectPosition_ApplyStepAndYOffset(effect->actors[effect->count - 1], &final);
  first.x += (final.x - first.x) / 2;
  REG_BG2X = (64 - first.x) << 8;
  work->transferMode = 1;
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);

  // Each boulder waits 64 pixels above the canvas; each shard sits at its
  // place in its boulder, ready to fly up at random.
  rocks = Data_081985b0[effect->variant];
  for (i = 0; i != rocks; i++) {
    rock = &work->particles[i];
    rock->y = -0x400000;
    rock->velocityY = 0;
  }
  for (i = 0; i != rocks; i++) {
    for (j = 0; j != SHARDS_PER_ROCK; j++) {
      struct EffectStep *seed = &SHARDS[i * SHARDS_PER_ROCK + j];

      seed->x = (Data_08198566[j * 2] + Data_081985a6[i]) << 16;
      seed->y = Data_08198566[j * 2 + 1] << 16;
      seed->velocityX = ((Random16() % 96) - 48) << 10;
      seed->velocityY = -((Random16() & 127) + 32) << 11;
      seed->z = 32;
      seed->variant = 0;
    }
  }

  for (frame = 0; frame != Data_081985ab[rocks - 1] + 65; frame++) {
    // 48 frames after the last drop the battle resumes, hit cue 133.
    if (frame == Data_081985ab[rocks - 1] + 48)
      Func_081180e8(133);
    for (i = 0; i != rocks; i++) {
      rock = &work->particles[i];
      // The crash: the first one resumes the battle, the others sound.
      if (frame == Data_081985ab[i] + FALL_FRAMES) {
        if (i == 0)
          Func_081180e8(134);
        else
          Audio_PlayCue(134);
        work->shakeFrames = 4;
      }
      // The shards fly, each tumbling through its kind's three cels more
      // slowly every other frame.
      if (frame >= Data_081985ab[i] + FALL_FRAMES) {
        for (j = 0; j != SHARDS_PER_ROCK; j++) {
          shard = &SHARDS[i * SHARDS_PER_ROCK + j];
          cel = (j % 5) * 3 + shard->variant / 96 % 3;
          blit(canvas, work->sheet + gData_081974bc[cel] + 0x83c,
               HI(shard->x) - ((width = gData_0819749e[cel]) >> 1),
               HI(shard->y) - ((height = gData_081974ad[cel]) >> 1), width,
               height);
          BattleFxKernels_IntegrateVector2(shard, 64, 0x4000);
          shard->variant += shard->z;
          if (shard->z > 1 && (frame & 1))
            shard->z--;
        }
      }
      // Until then the boulder falls, a pixel a frame faster each frame,
      // and rests at y 50.
      if (frame < Data_081985ab[i] + FALL_FRAMES) {
        if (frame >= Data_081985ab[i])
          blit(canvas, work->sheet, Data_081985a6[i] + 47, HI(rock->y), 34, 62);
        rock->y += rock->velocityY;
        if (frame > Data_081985ab[i])
          rock->velocityY += 0x10000;
        if (rock->y > 0x320000)
          rock->y = 0x320000;
      }
      // Every target flashes for eight frames at each crash.
      if (frame == Data_081985ab[i] + FALL_FRAMES) {
        for (j = 0; j != effect->count; j++)
          Func_0814cd48(effect->actors[j], 7, 5, j, 8);
      }
    }
    Func_08158ce0(2, 4);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Teardown.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleFx_EndCanvasLayer();
}
