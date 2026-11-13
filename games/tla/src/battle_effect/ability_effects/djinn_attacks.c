/// @file
/// The summons that spend one Djinni: Mercury, Jupiter, Venus and Mars.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

// Tiles: copy a byte image rotated 90 degrees clockwise.

/// Rotates w-by-h src clockwise into h-by-w dst.
/// The top row of src becomes the rightmost column of dst.
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h) {
  s32 y;
  s32 x;
  s32 col;
  s32 offset;
  u8 *d;
  const u8 *s;

  y = 0;
  if (h != 0) {
    offset = 0;
    col = h;
    do {
      x = 0;
      if (w != 0) {
        d = (u8 *)(col + (u32)dst - 1);
        s = (const u8 *)(offset + (u32)src);
        do {
          x++;
          *d = *s;
          s++;
          d += h;
        } while (x != w);
      }
      y++;
      offset += w;
      col--;
    } while (y != h);
  }
}

// The Mercury Djinn's flames (effect kind 281).

// Byte offsets of the two blitter slots in gHeapSlots (unk64[1], unkb8[1]).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The dots live in the map cell buffer: 32 for each of the five flames.
#define DOTS ((struct EffectStep *)Ram_MapCellBuffer)
#define DOTS_PER_FLAME 32
#define FLAME_COUNT 5

extern char ResourceId_BlueFlameSheet;
/// The frames the Djinn throws its five flames on (☀️'s
/// gDjinnAttacks_LaunchFrames).
extern const u8 Data_0819850e[];

/// Fetches the max and additive blitters, flipped or not.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
/// A small Mercury Djinn circles over the targets, one turn every 32 frames, on
/// a radius of 64 - 2 x frame pixels (half that across) that shrinks through
/// the middle at frame 32 and opens out on the other side. At frames 4, 7, 10,
/// 21 and 34 it throws a blue flame that shoots left 12 pixels a frame and
/// shatters on its fifth frame into 32 dots, with a shake; six frames after
/// each throw every target flashes and recoils. The dots shrink with age and
/// fall back under gravity. The effect runs 96 frames; at frame 56 the battle
/// resumes and resolves the targets (hit cue 133).
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
  BattleEffect_StartCanvasLayer(0);
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);

  // The flame picture is the blue flame sheet turned on its side.
  cells = Ram_MapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, cells, 1, 1);
  Graphics_TransposeCopy(cells, work->sheet + 0x2a8, 17, 104);
  Graphics_TransposeCopy(cells + 0x6e8, work->sheet + 0x990, 34, 65);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // No flame and no dot is alive yet.
  for (i = 0; i != 8; i++)
    work->particles[i].variant = -1;
  for (i = 0; i != 512; i++)
    DOTS[i].variant = -1;

  Sound_PlayFar(162);
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
          Sound_PlayFar(133);
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
            BattleMotion_ApplyKnockbackFar(effect->actors[j], 6);
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

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != 96);

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// The dots live in the map cell buffer: 32 for each of the eight balls.

#define DOT_COUNT 512
#define DOTS_PER_BALL 32
#define BALL_COUNT 8

extern char ResourceId_FlameballSheet;

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
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
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
            BattleMotion_ApplyKnockbackFar(effect->actors[j], 6);
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

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    angle -= 0x800;
    frame++;
  } while (frame != 96);

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Resources: decompress a resource with a palette.

/// Decompress resource id into dst. If the first 128 bytes are a palette,
/// copy_pal copies it to the start of BG palettes; skip_pal skips it on
/// decoding.
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal) {
  u8 *res = Resource_GetTableEntry(id);

  if (copy_pal != 0) {
    // The entry held in a local loads after the destination is built; as a
    // plain Iwram_CopyWords call, 2 of 26 instructions differ in tla-en.
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, res, 128);
  }
  if (skip_pal != 0)
    res += 128;
  Resource_DecodeType01(res, dst);
}

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define DOTS_PER_COLUMN 32
#define COLUMN_COUNT 10

void BattleEffect_RunVenus(struct BattleEffectArgument *effect) {
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
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                          COMPOSITE_FLIP_Y));
  BattleEffect_LoadBlitter(
      HEAP_BLITTER_ALTERNATE,
      BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)slots->unk64[1];
  draw[1] = (DrawRectangle)slots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet,
                             work->sheet + 0x2f8, 1, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each column starts somewhere in the canvas's 64 pixels, on the ground.
  for (i = 0; i != 32; i++) {
    column = &work->particles[i];
    column->x = Random16() & 63;
    column->y = 104;
  }
  // No dot is alive yet.
  for (i = 0; i != DOT_COUNT; i++)
    DOTS[i].variant = -1;

  Sound_PlayFar(141);
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
            Sound_PlayFar(133);
          work->shakeFrames = 4;
          // Every target flashes for six frames and recoils.
          for (j = 0; j != effect->count; j++) {
            Func_0814cd48(effect->actors[j], 7, 5, j, 6);
            BattleMotion_ApplyKnockbackFar(effect->actors[j], 6);
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
        BattleEffectKernels_IntegrateVector2(dot, 62, 0x2000);
        dot->variant--;
      }
    }

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    angle -= 0x800;
    frame++;
  } while (frame != 96);

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// The sparks live in the map cell buffer: sixteen under each of nine puffs.
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)
#define PUFF_COUNT 9
#define SPARKS_PER_PUFF 16

extern char ResourceId_GlowOrbSheet;
extern char ResourceId_EmberStreakSheet;

// The ember puff's six cels: ☀️'s gBattleEffect_PuffArcCellWidths, _CellHeights,
// _CellBiasY and _CellSourceOffsets.
extern u8 gData_08197467[];
extern u8 gData_0819746d[];
extern u8 gData_08197473[];
extern u16 gData_0819747a[];

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
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
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
      Iwram_CopyWordsEntry(
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
        BattleMotion_ApplyKnockbackFar(effect->actors[member], 6);
      }
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    diveY += 8;
    frame++;
  } while (frame != 112);

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
