/// @file
/// The Sleep, Baffle, Death and Sword Cards.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer task runs at this priority, where ☀️'s runs at 0x480.
#define TASK_PASS_FRAME 0xc80

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

#define CARD (gMapCellBuffer + 0x603e)
#define CARD_NOISE (CARD + 0x800)

// The four cards, as the wrappers pass them.
enum {
  CARD_SLEEP,  // Sleep Card, kind 92: sheep jump from the card.
  CARD_BAFFLE, // Baffle Card, kind 90: butterflies, and the screen waves.
  CARD_DEATH,  // Death Card, kind 93: a skeleton reaps the target.
  CARD_SWORD,  // Sword Card, kind 91: a dagger strikes, and shards fly.
};

// A texture for the model kernels: its size as powers of two, then its
// pixels.
/* VIEW(f18ad40): tag struct ModelTexture as struct ModelTexture__f18ad40;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunCards moves: 1 of 1232 instructions in
 * tla-ja. */
struct ModelTexture__f18ad40 {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
};

/* VIEW(f18ad40): tag struct Model as struct Model__f18ad40; ability_effects.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect/cards.c:323: structure has no
 * member named `vertices'. */
struct Model__f18ad40 {
  s32 kind;
  s32 next;
  void *mesh;
  void *vertices;
  struct ModelTexture__f18ad40 *texture;
  s32 alpha;
  s32 tail;
};

// The four cards (Sleep, Baffle, Death and Sword Card): a card flies from
// the user to above the targets' middle for 48 frames, spinning upright and
// growing, the background scrolling with it as far as it can. Then it shows
// its picture: sheep jump out for the Sleep Card, butterflies flutter and
// the screen waves for the Baffle Card, and for the Death and Sword cards the
// card dissolves into the background's colours while a skeleton reaps the
// target or a dagger strikes it and breaks into shards. The screen fades in
// the last eight frames.
void BattleEffect_RunCards(struct BattleEffectArgument *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *graphics;
  DrawRectangle draw[2];
  s32 scale;
  s32 scroll;
  s32 overflow;
  s32 length;
  s32 frame;
  struct EffectPosition card;
  struct EffectPosition middle;
  struct ModelTexture__f18ad40 texture;
  struct EffectStep *step;
  struct Model__f18ad40 *model;
  void *vertices;
  s32 i;
  s32 j;
  s32 limit;
  s32 first;
  s32 slot;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up a tiled canvas, filled on the user's side, and a clipped MAX
  // blitter (additive for the Sword Card), mirrored to face the targets.
  BattleEffect_StartTiledCanvas(0x2001);
  if (args->side == 0)
    BattleEffect_StartTiledCanvasFilled(0x2001);
  else
    BattleEffect_StartTiledCanvas(0x2001);
  REG_BLDALPHA = BLEND_ALPHA(16, 2);
  if (mode == CARD_SWORD) {
    if (args->side == 0)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          BLITTER_ADDITIVE);
  } else {
    if (args->side == 0)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  scale = 0;
  scroll = 0;
  overflow = 0;

  // Load the card and its picture; the Sword Card makes seven darker copies
  // of its shards for their fade.
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  if (mode == CARD_SLEEP) {
    Resource_LoadAndDecompress((s32)&ResourceId_SheepCardSheet, CARD, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0CF, work->sheet, 0, 0);
    length = 96;
  } else if (mode == CARD_BAFFLE) {
    Resource_LoadAndDecompress((s32)&ResourceId_ButterflyCardSheet, CARD, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0CF, work->sheet, 0, 0);
    length = 80;
  } else if (mode == CARD_SWORD) {
    Resource_LoadAndDecompress((s32)&ResourceId_SwordCardSheet, CARD, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage,
                               work->sheet + 0x2580, 0, 0);
    for (i = 1; i != 8; i++) {
      for (j = 0; j != 696; j++) {
        s32 shade = work->sheet[0x2580 + j];

        if (shade > 0) {
          shade -= i * 6;
          if (shade <= 0)
            shade = 1;
        }
        work->sheet[0x2580 + i * 696 + j] = shade;
      }
    }
    length = 116;
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_SkeletonCardSheet, CARD, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_CounterRevealSheet, work->sheet,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetA,
                               Ram_MapCellBuffer, 1, 0);
    length = 88;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  if (mode == CARD_BAFFLE)
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  REG_BG2PA = Q_8_8(1);

  for (frame = 0; frame != length; frame++) {
    // Frame 0: find the user and the targets' middle (the Death Card's 32
    // pixels toward the user), and scroll the background to the user.
    if (frame == 0) {
      Sound_PlayFar(SE_WARP);
      EffectPosition_ProjectSprite(args->actor, &card);
      BattleEffect_GetTargetsMidpoint(&middle);
      if (mode == CARD_DEATH) {
        if (args->side == 0)
          middle.x += 32;
        else
          middle.x -= 32;
      }
      REG_BG2X = (56 - card.x) << 8;
      scroll = 56 - card.x;
      scale = 0;
      EFFECT_FREE_PARTICLES(i, work->particles, 64);
    }

    // The last eight frames: the screen fades.
    if (frame >= length - 8) {
      s32 fade = frame - length + 8;

      REG_BLDALPHA = ((fade + 8) << 8) | (16 - fade * 2);
    }
    if (frame == 48 && mode == CARD_BAFFLE)
      Sound_PlayFar(SE_ITEM_BREAK);

    // Frames 0-47: the card flies to the middle, rising and growing; the
    // background follows it as far as it can scroll.
    if (frame <= 47) {
      s32 t = frame * (middle.x - card.x);
      s32 q;
      s32 x;

      first = card.x;
      q = t / 48;
      first -= 64;
      x = first + q;

      if (x < 0) {
        overflow = x;
        x = 0;
      } else if (x > 112) {
        overflow = x - 112;
        x = 112;
      }
      x <<= 8;
      REG_BG2X = -x;
      scroll = -x;
      card.y--;
      scale += 0x400;
      if (scale > Q16_ONE / 2)
        scale = Q16_ONE / 2;
    }

    if (mode != CARD_SLEEP) {
      if (mode == CARD_BAFFLE) {
        // The screen waves, line by line.
        s32 *line = work->bg2xLines;

        for (i = 0; i != 160; i++)
          *line++ =
              scroll -
              ((Trig_Sin((frame << 12) + i * (TRIG_TURN / 16)) << 2) >> 10);
      } else {
        // The background steps to the smoke's colours, then the picture's;
        // from frame 48 the card dissolves in 16 random steps.
        u8 *noise = CARD_NOISE;

        if ((mode == CARD_DEATH || mode == CARD_SWORD) &&
            (frame >= 40 && frame <= 47))
          BattleEffect_StepPaletteToResource((s32)&ResourceId_SmokeSheet);
        if (frame > 51) {
          if (mode == CARD_DEATH)
            BattleEffect_StepPaletteToResource(
                (s32)&ResourceId_RedCrescentSheetA);
          else
            BattleEffect_StepPaletteToResource((s32)&ResourceId_DaggerSheet);
        }
        if (frame == 48) {
          for (i = 0; i != 256; i++)
            noise[i] = Random16() & 15;
        }
        if (frame >= 48 && frame <= 63) {
          u8 *card = CARD;

          for (i = 0; i != 64; i++) {
            // first, free here, holds the cleared pixel: a second lifetime
            // keeps the flight's copy of card.x (see the header).
            for (j = 0; j != 32; j++) {
              /* FAKEMATCH(dead): first cleared inside the test, or'd into frame
               * - 48. */
              if (noise[(i & 7) * 32 + j] == (frame - 48 | (first = 0)))
                card[i * 32 + j] = first;
            }
          }
        }
      }
    }

    // The card, until its picture takes over.
    if (mode == CARD_SLEEP)
      limit = 120;
    else
      limit = mode == CARD_SWORD ? 72 : 80;
    if (frame < limit) {
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      texture.logHeight = 6;
      model->vertices = vertices;
      texture.pixels = CARD;
      model->kind = 8;
      model->texture = &texture;
      model->mesh = gBattleEffect_TallMesh;
      Transform_Reset();
      SceneTransform_ApplyScale3(Q16_ONE, Q16_ONE, Q16_ONE);
      SceneTransform_ApplyPosition3(overflow << 16, (card.y << 16) - px(64), 0);
      if (frame <= 47) {
        if (args->side == 0)
          Transform_ApplyRoll((frame << 10) - 0xd000);
        else
          Transform_ApplyRoll((52 - frame) << 10);
        Transform_ApplyPitch((52 - frame) << 10);
      } else {
        if (args->side == 0)
          Transform_ApplyRoll(-TRIG_TURN / 16);
        else
          Transform_ApplyRoll(TRIG_TURN / 16);
        Transform_ApplyPitch(TRIG_TURN / 16);
      }
      Transform_ApplyYaw(0x1770);
      SceneTransform_ApplyUniformScale(scale);
      SceneTransform_ApplyScale3(Q16_ONE, Q16_ONE * 2, Q16_ONE);
      Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(vertices);
    }

    if (mode == CARD_BAFFLE) {
      // Frame 48: the butterflies take flight, two a frame for 32 frames;
      // each flutters to the ground and skims along it.
      if (frame == 48) {
        EFFECT_SHOW_DAMAGE(SE_WHOOSH);
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        REG_BLDALPHA = BLEND_ALPHA(16, 16);
      }
      if (frame >= 48 && frame <= 79) {
        s32 time = frame - 48;

        slot = time * 2;
        for (i = 0; i != 2; i++) {
          step = &work->particles[slot + i];
          step->x = px(64);
          step->y = px(48);
          step->velocityX = ((Random16() & 127) - 64) << 11;
          step->velocityY = ((Random16() & 127) + 64) << 11;
          step->variant = 0;
        }
      }
      for (i = 0; i != 64; i++) {
        step = &work->particles[i];
        if (step->variant >= 0 && step->variant <= 15) {
          draw[0](canvas, work->sheet + (step->variant / 4 << 10),
                  overflow + Q_16_16_UPPER_HALF(step->x) - 16,
                  Q_16_16_UPPER_HALF(step->y) - 16, 32, 32);
          BattleEffectKernels_IntegrateVector2(step, 62, -0x1000);
          if (step->y > px(96)) {
            step->velocityY = 0;
            step->y = px(96);
            step->velocityX *= 2;
          }
          step->variant++;
        }
      }
    }

    if (mode == CARD_SLEEP) {
      // Frame 48: a sheep a frame for 32 frames jumps from the card.
      if (frame == 48)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == 64)
        EFFECT_SHOW_DAMAGE(-1);
      if (frame >= 48 && frame <= 79) {
        struct EffectStep *sheep;
        u32 mask = 127; // Holds one constant for its whole life.

        slot = frame - 48;
        sheep = (struct EffectStep *)(slot * 28 + (s32)work->particles);
        sheep->x = px(64);
        sheep->y = px(48);
        sheep->velocityX = ((Random16() & mask) - 64) << 11;
        sheep->velocityY = ((Random16() & mask) - 64) << 11;
        sheep->variant = mode;
      }
      for (i = 0; i != 64; i++) {
        step = &work->particles[i];
        if (step->variant >= 0 && step->variant <= 63) {
          draw[0](canvas,
                  Ram_MapCellBuffer + 0x683e + ((step->variant / 4 & 3) << 10),
                  overflow + Q_16_16_UPPER_HALF(step->x) - 16,
                  Q_16_16_UPPER_HALF(step->y) - 16, 32, 32);
          BattleEffectKernels_IntegrateVector2(step, 62, 0x2000);
          step->variant++;
        }
      }
    }

    if (mode == CARD_SWORD && frame > 47) {
      s32 time = frame - 48;

      // The dagger swings for 24 frames.
      if (time <= 23)
        draw[0](canvas, work->sheet + time / 4 % 6 * 960, overflow + 52, 44, 24,
                40);

      // Frame 72: it strikes; the screen flashes and 64 shards burst.
      if (time == 24) {
        Sound_PlayFar(SE_POWER_UP);
        EFFECT_SHOW_DAMAGE(-1);
        work->shakeFrames = 4;
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
        BattleEffect_TintBattler(args->actors[0], SPRITE_COLOR_WHITE, TINT_KEEP,
                                 0, 16);
        for (i = 0; i != 64; i++) {
          s32 angle = Random16() & 0xffff;
          s32 force = 127; // The AND lands in the register holding 127.

          force &= Random16();

          step = &work->particles[i];
          step->x = px(64);
          step->y = (i + 48) << 16;
          step->velocityX = (Trig_Cos(angle) * (force + 64)) >> 6;
          step->velocityY = (Trig_Sin(angle) * (force + 64)) >> 6;
          step->variant = Random16() & 255;
        }
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        REG_BLDALPHA = BLEND_ALPHA(16, 16);
      }

      // The shards tumble through three cells and darken.
      if (time >= 24 && time <= 60) {
        s32 fade = 0;

        if (time > 28) {
          fade = (time - 24) / 4;
          if (fade > 7)
            fade = 7;
        }
        for (i = 0; i != 32; i++) {
          s32 cell;

          step = &work->particles[i];
          cell = i % 4 * 3 + (step->variant + time) / 8 % 3;
          draw[0](
              canvas,
              work->sheet + 0x2580 +
                  (fade * 696 + gStatChange_ShardPictures[cell]),
              overflow + Q_16_16_UPPER_HALF(step->x) -
                  gStatChange_ShardWidths[cell] / 2,
              Q_16_16_UPPER_HALF(step->y) - gStatChange_ShardHeights[cell] / 2,
              gStatChange_ShardWidths[cell], gStatChange_ShardHeights[cell]);
          BattleEffectKernels_IntegrateVector2(step, 60, 0);
        }
      }
    }

    if (mode == CARD_DEATH) {
      // The reaper rises in seven panels and swings at frame 64.
      if (frame == 60)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == 64) {
        Sound_PlayFar(SE_CLATTER);
        EFFECT_SHOW_DAMAGE(-1);
        BattleEffect_StartActorMotion(args->actors[0], 1, 0x20000, 0x80000,
                                      0x20000, 110);
        BattleEffect_TintBattler(args->actors[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        work->shakeFrames = 4;
      }
      if (frame > 51) {
        if (frame <= 63) {
          draw[0](canvas, work->sheet,
                  overflow + gCastingImpact_PictureX[args->side * 7],
                  gCastingImpact_PictureY[0], 57, 98);
        } else {
          if (frame <= 67)
            draw[0](canvas, work->sheet,
                    overflow + gCastingImpact_PictureX[args->side * 7],
                    gCastingImpact_PictureY[0], 57, 98);
          draw[0](canvas, work->sheet + 0x15d2,
                  overflow + gCastingImpact_PictureX[args->side * 7 + 1],
                  gCastingImpact_PictureY[1], 99, 69);
          if (frame >= 64 && frame <= 65)
            Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
          if (frame >= 66 && frame <= 67)
            draw[0](canvas, work->sheet + 0x3081,
                    overflow + gCastingImpact_PictureX[args->side * 7 + 2],
                    gCastingImpact_PictureY[2], 128, 91);
          if (frame >= 68 && frame <= 69)
            draw[0](canvas, Ram_MapCellBuffer,
                    overflow + gCastingImpact_PictureX[args->side * 7 + 3],
                    gCastingImpact_PictureY[3], 128, 91);
          if (frame >= 70 && frame <= 71)
            draw[0](canvas, Ram_MapCellBuffer + 0x2d80,
                    overflow + gCastingImpact_PictureX[args->side * 7 + 4],
                    gCastingImpact_PictureY[4], 128, 59);
          if (frame >= 72 && frame <= 73)
            draw[0](canvas, Ram_MapCellBuffer + 0x4b00,
                    overflow + gCastingImpact_PictureX[args->side * 7 + 5],
                    gCastingImpact_PictureY[5], 122, 29);
          if (frame >= 74 && frame <= 75)
            draw[0](canvas, Ram_MapCellBuffer + 0x58d2,
                    overflow + gCastingImpact_PictureX[args->side * 7 + 6],
                    gCastingImpact_PictureY[6], 76, 25);
        }
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  if (mode == CARD_BAFFLE)
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
#undef HEAP_SLOT_BLITTER
#undef BLITTER
#undef CARD
#undef CARD_NOISE
