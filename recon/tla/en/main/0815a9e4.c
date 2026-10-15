/* Draft of Func_0815a9e4, effect kind 3: a wall of rock rises and sinks.
 *
 * On screen: the background darkens, the ground in front of the caster
 * heaves up in a mound (frames 0-81), then a wall of rock grows out of it,
 * stretching up row by row (frames 12-87). It stands from frame 88 to 159,
 * with dust streaming up from its foot and the targets flashing every 12
 * frames and recoiling every 4 (frames 86-158); it sinks back down from
 * frame 160 to 183. Rocks fly up out of the ground the whole time from
 * frame 5, six at first and eleven from frame 72, bigger for the larger
 * variants. The wall is symmetric: each piece draws its left half, then
 * its right half mirrored, about x = 64 + wallX on the canvas. Larger
 * variants stretch the layer wider (BG2 at 1.25 and 1.5).
 *
 * Setup: the battle effect canvas; BG2's scale and offset for the variant
 * and the caster's side; the earth wall picture decoded into the map cell
 * buffer and laid out again in the effect work (see below); BG colours
 * 1-63 darkened eight steps; the dot sheet in the sheet block; eleven rock
 * objects; two max blitters, the second mirrored; alpha blending; the
 * transfer callback; the rocks' and the dust's starting points.
 * Frame loop, 192 frames, as above; cues 212, 141 and 145 at frames 0, 40
 * and 96, and the battle resumes with hit cue 134 at frame 120.
 * Teardown: the callback, both blitters and the rock objects are released
 * and the canvas layer is closed.
 *
 * Port of The Broken Seal's BattlePres_RunRisingWall (kind 3 there too),
 * with The Lost Age's heap slots, blitter modes (3 minus ☀️'s) and
 * callback order, no work->effect store, and the rock object's flags at
 * 0x1a.
 *
 * Listing 973 instructions, draft 974, differ 87 (trial 1, 2026-10-15).
 * Left: single-row order differences and the palette loop's literal pool
 * placed one branch later (one instruction more).
 *
 * Tried:
 *   ☀️'s body with The Lost Age's names                       532
 *   frame declared after canvas (its spill slot)              532 -> 420
 *   the standing wall's width written as 48 where ☀️ has a
 *     local "width = 48", which swaps i and j between r6 and
 *     r7 for the whole function (found by alchemy permute)    420 -> 87
 */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"

// Byte offsets of the two blitter slots in gHeapSlots (unk64[1], unkb8[1]).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The Lost Age numbers the blitter modes the other way round from The
// Broken Seal's COMPOSITE_ modes; Func_081963ec takes mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// The dust lives in the map cell buffer once the picture is laid out.
#define DUST ((struct EffectStep *)Ram_MapCellBuffer)
#define ROCK_COUNT 11
// The rocks' motion is particles[10..20], the late rocks' columns [16..21].
#define FIRST_ROCK 10

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

typedef DrawRectangle BlitterPair[2];

struct Scale {
  s32 x;
  s32 y;
};

struct RockSpritePart {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
};

struct RockObject {
  struct RockSpritePart part[2];
  u8 unk18[2];
  u8 flags;
};

extern char ResourceId_EarthWallSheet;
extern char ResourceId_OrangePaletteA;
extern char ResourceId_ParticleSpritesA;
// ☀️'s gRisingWall_Bg2Shifts, by side and variant.
extern s8 Data_08198648[][3];
// The growing wall's nine rows: ☀️'s gRisingWall_RowWidths and
// gRisingWall_RowSheetOffsets.
extern u8 Data_08198632[];
extern u16 Data_08198620[];
// The mound's three cels: ☀️'s gRisingWall_MoundWidths, _MoundHeights and
// _MoundSheetOffsets.
extern u8 Data_08198642[];
extern u8 Data_08198645[];
extern u16 Data_0819863c[];
// ☀️'s gRisingWall_LateRockColumns.
extern u8 Data_0819864e[];
// Byte offsets of the dot cels in the particle sheet, by size - 1.
extern u16 gData_08197410[];
// ☀️'s gRisingWall_UnitScale.
extern struct Scale Data_08196e44;

void BattleFx_BeginCanvasLayer(s32 bgControl);
// Closes the canvas layer.
void Func_08143bb8(void);
// Builds a blitter in a heap slot: BattleEffect_LoadWork(slot, 7, 7,
// flags, mode), whose result it returns.
s32 Func_081963ec(s32 slot, s32 modeFlags);
// Copies the canvas to VRAM when a transfer is pending.
s32 Func_08143000(void);
s32 Scheduler_AddOrUpdateCallback(s32 (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(s32 (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
// BattleEventRuntime_BeginPhase through its far call.
void Func_081180e8(s32 hitCue);
u32 Random16(void);
void *Resource_GetTableEntry(s32 resourceId);
void *ResourceObject_CreateFar(s32 resource);
void ResourceObject_ReleaseFar(void *object);
void Animation_ApplyChildArgumentFar(void *object, s32 value);
void Render_ApplyProjectedPlacementFar(void *object, s32 *position,
                                       struct Scale *scale, s32 mode);
// An actor's recoil motion, through a far call.
void Func_08118088(s32 actor, s32 motion);
// Flashes an actor in a palette for some frames.
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
// Shakes the camera while work->shakeFrames lasts.
void Func_08158ce0(s32 randomMask, s32 shakeRange);
// Counts down the actors' flash timers.
void Func_081434f8(void);
void WaitFrames(s32 frames);

void Func_0815a9e4(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  u8 *sheet;
  s32 wallX;
  s32 rockX;
  struct Scale scale;
  s32 position[4];
  u8 *source;
  u16 *color;
  void *palette = (void *)BG_PLTT;
  s32 offset;
  s32 i;
  s32 j;
  s32 k;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleFx_BeginCanvasLayer(0);

  // The larger variants stretch the wall wider: BG2 at 1.25 and 1.5.
  if (effect->variant == 0)
    REG_BG2PA = 204;
  else if (effect->variant == 1)
    REG_BG2PA = 170;
  if (effect->side == 1) {
    wallX = 8;
    if (effect->variant == 0)
      rockX = 40;
    else if (effect->variant == 1)
      rockX = 36;
    else
      rockX = 40;
  } else {
    wallX = -16;
    rockX = -12;
  }
  REG_BG2X = Data_08198648[effect->side][effect->variant] << 8;

  // The palette: BG colours 1-63 darkened eight steps in each channel.
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, Ram_MapCellBuffer,
                             1, 0);
  Iwram_CopyWords(palette,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
  color = (u16 *)&BG_PLTT[1];
  for (i = 0; i != 63; i++) {
    s32 c = *color;
    s32 blue = ((u16)c >> 10) & 31;
    s32 green = ((u16)c >> 5) & 31;
    s32 red = 31;

    red &= c;
    blue -= 8;
    green -= 8;
    red -= 8;

    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = (blue << 10) | (green << 5) | red;
  }

  // The picture, laid out again in the effect work:
  //   the growing wall: nine one-line rows, each stretched to 32 lines;
  //   the top of the standing wall at 0x13c0: 48x32, stretched to 48x96;
  //   its foot at 0x25c0: 48x21;
  //   the mound: three cels.
  source = Ram_MapCellBuffer;
  offset = 0;
  for (j = 0; j != 9; j++) {
    for (i = 0; i != 32; i++) {
      for (k = 0; k != Data_08198632[j]; k++, offset++)
        work->sheet[offset] = source[k];
    }
    source += Data_08198632[j];
  }
  for (j = 0; j != 32; j++) {
    for (i = 0; i != 3; i++) {
      for (k = 0; k != 48; k++, offset++)
        work->sheet[offset] = source[k];
    }
    source += 48;
  }
  for (i = 0; i != 1008; i++)
    work->sheet[offset++] = *source++;
  for (i = 0; i != 3; i++) {
    for (k = 0; k != Data_08198642[i] * Data_08198645[i]; k++)
      work->sheet[offset++] = *source++;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

  // The rocks: eleven animated objects, four of each size.
  for (i = 0; i != ROCK_COUNT; i++) {
    struct RockObject *object = ResourceObject_CreateFar(653);

    work->objects[i] = object;
    if (object != 0) {
      object->flags = 0;
      Animation_ApplyChildArgumentFar(object, i / 4);
      ((struct RockObject *)work->objects[i])->part[0].priority = 1;
    }
  }
  // Two max blitters, the second mirrored for the right half.
  Func_081963ec(HEAP_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Func_081963ec(HEAP_BLITTER_ALTERNATE,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                      COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  REG_BLDCNT = 0x3f46;
  REG_BLDALPHA = 0x1010;
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

  // A rock's x is its column, y counts down from 128 by z, velocityX cycles
  // 1 to 12 as its animation step, and variant is its wait before it flies.
  for (i = 0; i != ROCK_COUNT; i++) {
    struct EffectStep *rock = &work->particles[FIRST_ROCK + i];

    rock->x = (Random16() & 15) + 88;
    rock->y = 128;
    rock->z = (Random16() & 15) + 2;
    rock->velocityX = i;
    rock->velocityY = 1;
    rock->velocityZ = 0x8000;
    rock->variant = 44 - i * 4;
  }
  for (i = 0; i != 6; i++)
    work->particles[16 + i].x = Data_0819864e[i];

  // The dust rises from the foot of the wall after a random wait.
  for (i = 0; i != 256; i++) {
    struct EffectStep *dust = &DUST[i];

    dust->x = ((Random16() & 63) + wallX + 32) << 16;
    dust->y = ((Random16() & 7) + 96) << 16;
    dust->velocityY = ((Random16() & 63) + 32) << 13;
    dust->variant = Random16() & 31;
  }
  REG_BG2CNT = 0x785;
  work->shakeFrames = 250;

  for (frame = 0; frame != 192; frame++) {
    if (frame == 0)
      Audio_PlayCue(212);
    if (frame == 40)
      Audio_PlayCue(141);
    if (frame == 96)
      Audio_PlayCue(145);
    // The battle resumes and resolves the targets, hit cue 134.
    if (frame == 120)
      Func_081180e8(134);

    // The ground heaves up (frames 0-81), then pulses between two cels.
    if (frame <= 81) {
      s32 cel = frame / 4;

      if (cel > 2)
        cel = (cel & 1) + 1;
      draw[0](canvas, work->sheet + Data_0819863c[cel],
              wallX - Data_08198642[cel] + 64, 116 - Data_08198645[cel],
              Data_08198642[cel], Data_08198645[cel]);
      draw[1](canvas, work->sheet + Data_0819863c[cel], wallX + 64,
              116 - Data_08198645[cel], Data_08198642[cel], Data_08198645[cel]);
    }

    // The wall grows out of it, a row every three frames (frames 12-87).
    if (frame >= 12 && frame <= 87) {
      s32 row = (frame - 64) / 3;

      if (row < 0)
        row = 0;
      if (row > 7)
        row = 7;
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + Data_08198620[row],
                wallX - Data_08198632[row] + 64, i * 32 - 12,
                Data_08198632[row], 32);
        draw[1](canvas, work->sheet + Data_08198620[row], wallX + 64,
                i * 32 - 12, Data_08198632[row], 32);
      }
    }

    // It sinks back the same way (frames 160-183).
    if (frame >= 160 && frame <= 183) {
      s32 row = 7 - (frame - 160) / 3;

      if (row < 0)
        row = 0;
      if (row > 7)
        row = 7;
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + Data_08198620[row],
                wallX - Data_08198632[row] + 64, i * 32 - 12,
                Data_08198632[row], 32);
        draw[1](canvas, work->sheet + Data_08198620[row], wallX + 64,
                i * 32 - 12, Data_08198632[row], 32);
      }
    }

    // The standing wall: its top and its foot (frames 88-159).
    if (frame >= 88 && frame <= 159) {
      draw[0](canvas, &work->sheet[0x13c0], wallX + 16, 0, 48, 96);
      draw[1](canvas, &work->sheet[0x13c0], wallX + 64, 0, 48, 96);
      draw[0](canvas, &work->sheet[0x25c0], wallX + 16, 96, 48, 21);
      draw[1](canvas, &work->sheet[0x25c0], wallX + 64, 96, 48, 21);
    }

    // The dust (from frame 88): what leaves the top goes back to the foot
    // until frame 159.
    if (frame > 87) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *dust = &DUST[i];

        if (dust->variant == 0) {
          s32 size = (i & 3) + 5;

          draw[0](canvas, sheet + gData_08197410[size - 1],
                  HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);
          dust->y -= dust->velocityY;
          if (dust->y < 0 && frame <= 159)
            dust->y = 0x600000;
        } else {
          dust->variant--;
        }
      }
    }

    // The rocks (from frame 5): eleven and bigger from frame 72.
    if (frame > 4) {
      s32 rocks = 6;

      if (frame > 71)
        rocks = ROCK_COUNT;
      for (k = 0; k != rocks; k++) {
        struct EffectStep *rock = &work->particles[FIRST_ROCK + k];
        s32 slot;

        if (rock->variant == 0) {
          scale = Data_08196e44;
          if (frame > 71) {
            scale.x = (k << 12) + 0x8000;
            scale.x += effect->variant << 14;
          } else {
            scale.x = 0x8000;
          }
          scale.y = scale.x;
          position[3] = 0;
          position[0] = (rock->x + rockX * 2) << 16;
          position[1] = 0x2000000 - (rock->y << 16);
          position[2] = 0x2000000;
          slot = (frame / 2 + k) % ROCK_COUNT;
          if (slot != -1)
            Render_ApplyProjectedPlacementFar(work->objects[slot], position,
                                              &scale, 0);
          rock->y -= rock->z;
          rock->velocityX += rock->velocityY;
          if (rock->velocityX > 12)
            rock->velocityX -= 12;
          // Once it leaves the top it starts again from 128; from frame 88
          // it takes a new column and speed, and after 159 it stops.
          if (rock->y < 0) {
            if (frame > 159) {
              rock->variant = -1;
            } else {
              if (frame > 87) {
                rock->z = (Random16() & 7) + 8;
                if (effect->variant == 0)
                  rock->x = Random16() % 96 + 42;
                else if (effect->variant == 1)
                  rock->x = Random16() % 112 + 34;
                else
                  rock->x = Random16() % 160 + 10;
              }
              rock->y = 128;
              rock->variant = 8;
            }
          }
        } else {
          rock->variant--;
        }
      }
    }

    // The targets (frames 86-158): a flash every 12 frames, a recoil
    // every 4.
    if (frame <= 158) {
      for (i = 0; i != effect->count; i++) {
        if (frame > 85) {
          if (frame % 12 == 0)
            Func_0814cd48(effect->actors[i], 7, 5, i, 6);
          if ((frame & 3) == 0)
            Func_08118088(effect->actors[i], 5);
        }
      }
    }
    // While the wall stands the screen shakes hard, otherwise softly.
    if (frame < 90 || frame > 160)
      Func_08158ce0(2, 2);
    else
      Func_08158ce0(8, 8);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Teardown.
  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  for (i = 0; i != ROCK_COUNT; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  Func_08143bb8();
}
