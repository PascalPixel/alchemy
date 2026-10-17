// Near miss, differ 41 of 558.

// Twin: The Broken Seal's BattleFx_RunFallingParticles (effect kind 7 in
// both games), ported as is apart from ⚓️'s calling conventions: the effect is
// read from the argument (⚓️ never stores work->effect), the heap blocks are
// read as Ram_HeapSlots fields, the blitters go through Func_081963ec with
// ⚓️'s mode numbers (3 - The Broken Seal's), Func_0813bba0 arms the WIN0
// HBlank DMA, the callbacks take order 0xc80 and the results phase is 133.

// Left: the native stores draw[0] before it builds the pair's address, and
// builds that address as movs r7, #28; add r7, sp with the alternate
// blitter's load in between; here the two halves sit together and the
// peephole makes them add r7, sp, #28, one instruction short, which moves the
// first literal pool and four pc offsets. The rest are single rows where the
// native issues a constant or a pooled load one slot earlier.

// Tried:
// the trial port as written                                  47
// the shard's y stored after x and variant (permute)         41
// alchemy permute, 6 + 8 minutes (trial 1)                   41
// alchemy permute, 10 minutes, 2 jobs (2,218 candidates)     41

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"

typedef DrawRectangle BlitterPair[2];

// The blitter pair's heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define CALLBACK_ORDER 0xc80
#define PHASE_RESULTS 133

// Shards and frames for each variant (The Broken Seal's
// gFallingShards_Counts).
extern u8 Data_08197521[];
#define SHARD_COUNT(variant) Data_08197521[(variant) * 2]
#define FRAME_COUNT(variant) Data_08197521[(variant) * 2 + 1]

// The sparkle puff's size and cell for each of its nine pictures
// (gBattleFx_PuffSizes and gBattleFx_PuffCells).
extern u8 Data_0819745e[];
extern u16 Data_0819744c[];

extern char ResourceId_IceShardSheet;
extern char ResourceId_SparkleDots;
extern u16 gMapCellBuffer[];

void BattleFx_BeginCanvasLayer(s32 control);
void Func_08143bb8(void); // BattleFx_EndCanvasLayer
void Func_0813ba50(void); // BattleFx_ConfigureEffectDisplay
s32 Func_081963ec(s32 slot, s32 flags);
s32 Func_08143000(void);  // the canvas transfer callback
void Func_0813bba0(void); // arms WIN0's HBlank DMA
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 order);
s32 Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase); // BattleEventRuntime_BeginPhaseFar
u32 Random16(void);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void Func_08158ce0(s32 x, s32 y);    // Camera_ApplyShake
void Func_081434f8(void);            // ObjectGroup_TickMemberTimers
void WaitFrames(s32 frames);

// Ice, Ice Horn and Ice Missile (effect kind 7, variants 0 to 2).

// Shards of ice fall in a slanting column onto the targets' side. Each one
// flies down at 8 pixels a frame until it reaches the ground line, then
// flashes every target, shakes the screen and shatters: fourteen frames of
// breaking pictures, with a burst of sparkle puffs in the middle of them.
// Window 0 is narrowed line by line through an HBlank DMA table into a
// slanting band, so the shards appear only over the targets.

// Particles 0-31 are the shards and 32-63 the puffs. A particle's variant is
// its age: -1 is a shard still falling or a free puff.

void Func_0813bbd8(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *graphics;
  BlitterPair draw;
  struct EffectStep *shard;
  struct EffectStep *puff;
  u16 *line;
  s32 i;
  s32 k;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas, the shard pictures in the sheet and the puffs.
  BattleFx_BeginCanvasLayer(0x2001);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_IceShardSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, graphics, 0, 0);
  Func_0813ba50();
  REG_BLDCNT = 0x3f44;
  REG_WININ = 0x3337;

  // Line the shards up above the screen, each 8 pixels higher and further
  // out than the one before, with a little scatter.
  {
    s32 y;
    s32 slant;

    for (i = 0, y = -128, slant = -16; i != 32; y -= 64, i++, slant -= 8) {
      s32 x;

      shard = &work->particles[i];
      x = (Random16() & 63) + (Random16() & 7) + 24;
      if (effect->side == 1)
        x = x + slant + 24;
      else
        x = x - slant + 80;
      shard->x = x << 3;
      shard->variant = -1;
      shard->y = y;
    }
  }
  for (i = 0; i != 32; i++)
    work->particles[32 + i].variant = -1;

  // Falling shards draw at their maximum; the puffs add their light. Both
  // mirror for the other side.
  if (effect->side == 0) {
    Func_081963ec(SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_Y));
    Func_081963ec(SLOT_BLITTER_ALTERNATE,
                  BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_Y));
  } else {
    Func_081963ec(SLOT_BLITTER,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X));
    Func_081963ec(SLOT_BLITTER_ALTERNATE,
                  BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X));
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // Window 0's left and right edge for each line: a band slanting with the
  // shards' fall, then the targets' full width, then nothing.
  if (effect->side == 0) {
    line = gMapCellBuffer;
    for (i = 0; i != 160; i++) {
      if (i >= 8 && i <= 103)
        line[i] = (0xf0 - i) | ((0x70 - i) << 8);
      else if (i <= 135)
        line[i] = 0x888;
      else
        line[i] = 0x100;
    }
  } else {
    line = gMapCellBuffer;
    for (i = 0; i != 160; i++) {
      if (i >= 8 && i <= 95)
        line[i] = (i - 8 + 160) | ((i + 0x18) << 8);
      else if (i <= 135)
        line[i] = 0x78f8;
      else
        line[i] = 0x100;
    }
  }

  Scheduler_AddOrUpdateCallback(Func_0813bba0, CALLBACK_ORDER);
  work->transferMode = 2;
  if (effect->variant == 1)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(Func_08143000, CALLBACK_ORDER);

  for (frame = 0; frame != FRAME_COUNT(effect->variant); frame++) {
    if (frame == FRAME_COUNT(effect->variant) - 16)
      Func_081180e8(PHASE_RESULTS);

    for (i = 0; i != SHARD_COUNT(effect->variant); i++) {
      shard = &work->particles[i];

      // A falling shard moves 8 pixels down and 8 across; at the ground
      // line it lands.
      if (shard->variant == -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        draw[effect->variant == 2 ? 1 : 0](canvas, work->sheet, x, y, 32, 32);
        if (shard->y <= 0x27f) {
          if (effect->side == 0)
            shard->x -= 64;
          else
            shard->x += 64;
          shard->y += 64;
        } else {
          if ((i & 3) == 0)
            Audio_PlayCue(115);
          work->shakeFrames = 2;
          shard->variant = 0;
          for (k = 0; k != effect->count; k++)
            Func_0814cd48(effect->actors[k], 9, 5, k, 8);
        }
      }

      // A landed shard breaks over fourteen frames, three to a picture,
      // and throws a sparkle puff on each of frames 9 to 11.
      if (shard->variant != -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        if (shard->variant >= 1 && shard->variant <= 14) {
          draw[effect->variant == 2 ? 1 : 0](
              canvas, work->sheet + 0x400 + ((shard->variant / 3) << 10), x, y,
              32, 32);
        }
        if (shard->variant >= 9 && shard->variant <= 11) {
          for (k = 0; k != 32; k++) {
            puff = &work->particles[32 + k];
            if (puff->variant == -1) {
              puff->variant = 18;
              puff->x = (((Random16() & 31) + shard->x / 8) << 3) + 8;
              puff->y = ((Random16() & 15) + shard->y / 8 - 15) << 3;
              break;
            }
          }
        }
        if (shard->variant <= 14)
          shard->variant++;
      }
    }

    // Each puff counts down from 18 through its nine pictures, two frames
    // apiece, centred on its point.
    for (i = 0; i != 32; i++) {
      puff = &work->particles[32 + i];
      if (puff->variant != -1) {
        if (puff->variant <= 17) {
          s32 image = puff->variant / 2;
          s32 x = puff->x / 8 - (Data_0819745e[image] >> 1);
          s32 y = puff->y / 8 - (Data_0819745e[image] >> 1);

          draw[effect->variant == 2 ? 1 : 0](
              canvas, graphics + Data_0819744c[image], x, y,
              Data_0819745e[image], Data_0819745e[image]);
        }
        if (puff->variant > -1)
          puff->variant--;
      }
    }

    Func_081434f8();
    Func_08158ce0(4, 4);
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(Func_08143000);
  Scheduler_RemoveCallback(Func_0813bba0);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  Func_08143bb8();
  Func_0813ba50();
}
