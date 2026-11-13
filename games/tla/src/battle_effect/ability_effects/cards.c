/// @file
/// The Sleep, Baffle, Death and Sword Cards.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer task runs at this priority, where ☀️'s runs at 0x480.
#define TASK_PASS_FRAME 0xc80

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

#define HI(v) (((s16 *)&(v))[1])

// The card's picture, unpacked into the map cell buffer, and the 256 random
// steps that dissolve it after it. Named as a symbol, not Ram_MapCellBuffer's
// number: see the header.
extern u8 gMapCellBuffer[];
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

// One entry of a model list, as Render_AllocateModelList allocates them and
// Render_DrawModelList draws them.
/* VIEW(f18ad40): tag struct Model as struct Model__f18ad40; ability_effects.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:56486: structure has no member
 * named `vertices'. */
struct Model__f18ad40 {
  s32 kind;
  s32 next;
  void *mesh;
  void *vertices;
  struct ModelTexture__f18ad40 *texture;
  s32 alpha;
  s32 tail;
};

extern char ResourceId_Data134;
extern char ResourceId_SheepCardSheet;
extern char ResourceId_Data0CF;
extern char ResourceId_ButterflyCardSheet;
extern char ResourceId_SwordCardSheet;
extern char ResourceId_DaggerSheet;
extern char ResourceId_PaletteRampImage;
extern char ResourceId_SkeletonCardSheet;
extern char ResourceId_CounterRevealSheet;
extern char ResourceId_RedCrescentSheetA;
extern char ResourceId_SmokeSheet;

// The dagger's shards, ☀️'s palette ramp shards.
extern u16 gStatChange_ShardCells[];
extern u8 gStatChange_ShardWidths[];
extern u8 gStatChange_ShardHeights[];

// The reaper's seven panels: x by side (seven each), then y.
extern u8 gData_0819750c[];
extern u8 gData_0819751a[];

// The card's quad: its mesh and its corners.
extern u8 Data_081992b0[];
extern const u8 Data_081991e0[];

void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);

void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position);
void BattleEffect_GetTargetsMidpoint(struct EffectPosition *middle);
void BattleEffect_StepPaletteToResource(s32 resource);
void BattlePlayback_StartFar(s32 phase);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 group, s32 timerSlot,
                              s32 frames);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
struct PoolEntry *Render_AllocateModelList(s32 count);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z);
void Transform_ApplyPitch(s32 angle);
void Transform_ApplyYaw(s32 angle);
void Transform_ApplyRoll(s32 angle);
void SceneTransform_ApplyUniformScale(s32 scale);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_EndCanvasLayer(void);

// The four cards (Sleep, Baffle, Death and Sword Card): a card flies from
// the user to above the targets' middle for 48 frames, spinning upright and
// growing, the background scrolling with it as far as it can. Then it shows
// its picture: sheep jump out for the Sleep Card, butterflies flutter and
// the screen waves for the Baffle Card, and for the Death and Sword cards the
// card dissolves into the background's colours while a skeleton reaps the
// target or a dagger strikes it and breaks into shards. The screen fades in
// the last eight frames.
void BattleEffect_RunCards(struct BattleEffectArgument *effect, s32 mode) {
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
  if (effect->side == 0)
    BattleEffect_StartTiledCanvasFilled(0x2001);
  else
    BattleEffect_StartTiledCanvas(0x2001);
  REG_BLDALPHA = 0x210;
  if (mode == CARD_SWORD) {
    if (effect->side == 0)
      BattleEffect_LoadBlitter(
          HEAP_SLOT_BLITTER,
          BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_ADDITIVE));
    else
      BattleEffect_LoadBlitter(
          HEAP_SLOT_BLITTER,
          BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                  BLIT_ADDITIVE));
  } else {
    if (effect->side == 0)
      BattleEffect_LoadBlitter(
          HEAP_SLOT_BLITTER,
          BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_MAX));
    else
      BattleEffect_LoadBlitter(
          HEAP_SLOT_BLITTER,
          BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                  BLIT_MAX));
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
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (mode == CARD_BAFFLE)
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  REG_BG2PA = 0x100;

  for (frame = 0; frame != length; frame++) {
    // Frame 0: find the user and the targets' middle (the Death Card's 32
    // pixels toward the user), and scroll the background to the user.
    if (frame == 0) {
      Sound_PlayFar(190);
      EffectPosition_ProjectSprite(effect->actor, &card);
      BattleEffect_GetTargetsMidpoint(&middle);
      if (mode == CARD_DEATH) {
        if (effect->side == 0)
          middle.x += 32;
        else
          middle.x -= 32;
      }
      REG_BG2X = (56 - card.x) << 8;
      scroll = 56 - card.x;
      scale = 0;
      for (i = 0; i != 64; i++)
        work->particles[i].variant = -1;
    }

    // The last eight frames: the screen fades.
    if (frame >= length - 8) {
      s32 fade = frame - length + 8;

      REG_BLDALPHA = ((fade + 8) << 8) | (16 - fade * 2);
    }
    if (frame == 48 && mode == CARD_BAFFLE)
      Sound_PlayFar(138);

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
      if (scale > 0x8000)
        scale = 0x8000;
    }

    if (mode != CARD_SLEEP) {
      if (mode == CARD_BAFFLE) {
        // The screen waves, line by line.
        s32 *line = work->bg2xLines;

        for (i = 0; i != 160; i++)
          *line++ =
              scroll - ((Trig_Sin((frame << 12) + i * 0x1000) << 2) >> 10);
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
              /* FAKEMATCH(dead): first cleared inside the test, or'd into
               * frame - 48, so loop.c hoists the zero after the test's 7 and
               * level; as `first = 0;` at the top of the outer body, 2 of
               * 1246 instructions differ in tla-en. */
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
      model->mesh = Data_081992b0;
      Transform_Reset();
      SceneTransform_ApplyScale3(0x10000, 0x10000, 0x10000);
      SceneTransform_ApplyPosition3(overflow << 16, (card.y << 16) - (64 << 16),
                                    0);
      if (frame <= 47) {
        if (effect->side == 0)
          Transform_ApplyRoll((frame << 10) - 0xd000);
        else
          Transform_ApplyRoll((52 - frame) << 10);
        Transform_ApplyPitch((52 - frame) << 10);
      } else {
        if (effect->side == 0)
          Transform_ApplyRoll(-0x1000);
        else
          Transform_ApplyRoll(0x1000);
        Transform_ApplyPitch(0x1000);
      }
      Transform_ApplyYaw(0x1770);
      SceneTransform_ApplyUniformScale(scale);
      SceneTransform_ApplyScale3(0x10000, 0x20000, 0x10000);
      Render_TransformVertices(Data_081991e0, vertices, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(vertices);
    }

    if (mode == CARD_BAFFLE) {
      // Frame 48: the butterflies take flight, two a frame for 32 frames;
      // each flutters to the ground and skims along it.
      if (frame == 48) {
        BattlePlayback_StartFar(136);
        work->transferMode = 2;
        work->transferValue = 50;
        REG_BLDALPHA = 0x1010;
      }
      if (frame >= 48 && frame <= 79) {
        s32 time = frame - 48;

        slot = time * 2;
        for (i = 0; i != 2; i++) {
          step = &work->particles[slot + i];
          step->x = 64 << 16;
          step->y = 48 << 16;
          step->velocityX = ((Random16() & 127) - 64) << 11;
          step->velocityY = ((Random16() & 127) + 64) << 11;
          step->variant = 0;
        }
      }
      for (i = 0; i != 64; i++) {
        step = &work->particles[i];
        if (step->variant >= 0 && step->variant <= 15) {
          draw[0](canvas, work->sheet + (step->variant / 4 << 10),
                  overflow + HI(step->x) - 16, HI(step->y) - 16, 32, 32);
          BattleEffectKernels_IntegrateVector2(step, 62, -0x1000);
          if (step->y > 96 << 16) {
            step->velocityY = 0;
            step->y = 96 << 16;
            step->velocityX *= 2;
          }
          step->variant++;
        }
      }
    }

    if (mode == CARD_SLEEP) {
      // Frame 48: a sheep a frame for 32 frames jumps from the card.
      if (frame == 48)
        Sound_PlayFar(212);
      if (frame == 64)
        BattlePlayback_StartFar(-1);
      if (frame >= 48 && frame <= 79) {
        struct EffectStep *sheep;
        u32 mask = 127; // Holds one constant for its whole life.

        slot = frame - 48;
        sheep = (struct EffectStep *)(slot * 28 + (s32)work->particles);
        sheep->x = 64 << 16;
        sheep->y = 48 << 16;
        sheep->velocityX = ((Random16() & mask) - 64) << 11;
        sheep->velocityY = ((Random16() & mask) - 64) << 11;
        sheep->variant = mode;
      }
      for (i = 0; i != 64; i++) {
        step = &work->particles[i];
        if (step->variant >= 0 && step->variant <= 63) {
          // CARD_NOISE at its fixed address: reload builds the number here.
          draw[0](canvas,
                  Ram_MapCellBuffer + 0x683e + ((step->variant / 4 & 3) << 10),
                  overflow + HI(step->x) - 16, HI(step->y) - 16, 32, 32);
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
        Sound_PlayFar(212);
        BattlePlayback_StartFar(-1);
        work->shakeFrames = 4;
        Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
        BattleEffect_TintBattler(effect->actors[0], 7, -1, 0, 16);
        for (i = 0; i != 64; i++) {
          s32 angle = Random16() & 0xffff;
          s32 force = 127; // The AND lands in the register holding 127.

          force &= Random16();

          step = &work->particles[i];
          step->x = 64 << 16;
          step->y = (i + 48) << 16;
          step->velocityX = (Trig_Cos(angle) * (force + 64)) >> 6;
          step->velocityY = (Trig_Sin(angle) * (force + 64)) >> 6;
          step->variant = Random16() & 255;
        }
        work->transferMode = 2;
        work->transferValue = 50;
        REG_BLDALPHA = 0x1010;
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
          draw[0](canvas,
                  work->sheet + 0x2580 +
                      (fade * 696 + gStatChange_ShardCells[cell]),
                  overflow + HI(step->x) - gStatChange_ShardWidths[cell] / 2,
                  HI(step->y) - gStatChange_ShardHeights[cell] / 2,
                  gStatChange_ShardWidths[cell],
                  gStatChange_ShardHeights[cell]);
          BattleEffectKernels_IntegrateVector2(step, 60, 0);
        }
      }
    }

    if (mode == CARD_DEATH) {
      // The reaper rises in seven panels and swings at frame 64.
      if (frame == 60)
        Sound_PlayFar(212);
      if (frame == 64) {
        Sound_PlayFar(144);
        BattlePlayback_StartFar(-1);
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000, 0x80000,
                                      0x20000, 110);
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
        work->shakeFrames = 4;
      }
      if (frame > 51) {
        if (frame <= 63) {
          draw[0](canvas, work->sheet,
                  overflow + gData_0819750c[effect->side * 7],
                  gData_0819751a[0], 57, 98);
        } else {
          if (frame <= 67)
            draw[0](canvas, work->sheet,
                    overflow + gData_0819750c[effect->side * 7],
                    gData_0819751a[0], 57, 98);
          draw[0](canvas, work->sheet + 0x15d2,
                  overflow + gData_0819750c[effect->side * 7 + 1],
                  gData_0819751a[1], 99, 69);
          if (frame >= 64 && frame <= 65)
            Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
          if (frame >= 66 && frame <= 67)
            draw[0](canvas, work->sheet + 0x3081,
                    overflow + gData_0819750c[effect->side * 7 + 2],
                    gData_0819751a[2], 128, 91);
          if (frame >= 68 && frame <= 69)
            draw[0](canvas, Ram_MapCellBuffer,
                    overflow + gData_0819750c[effect->side * 7 + 3],
                    gData_0819751a[3], 128, 91);
          if (frame >= 70 && frame <= 71)
            draw[0](canvas, Ram_MapCellBuffer + 0x2d80,
                    overflow + gData_0819750c[effect->side * 7 + 4],
                    gData_0819751a[4], 128, 59);
          if (frame >= 72 && frame <= 73)
            draw[0](canvas, Ram_MapCellBuffer + 0x4b00,
                    overflow + gData_0819750c[effect->side * 7 + 5],
                    gData_0819751a[5], 122, 29);
          if (frame >= 74 && frame <= 75)
            draw[0](canvas, Ram_MapCellBuffer + 0x58d2,
                    overflow + gData_0819750c[effect->side * 7 + 6],
                    gData_0819751a[6], 76, 25);
        }
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  if (mode == CARD_BAFFLE)
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
#undef HEAP_SLOT_BLITTER
#undef BLITTER
#undef HI
#undef CARD
#undef CARD_NOISE
