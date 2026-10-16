// Draft: listing 1246 instructions, draft 1252, differ 796. No close ☀️ twin
// (call order BattleFx_InitializeMode10 0.468): ⚓️'s own effect on ☀️'s frame, a
// sibling of the four cards of Func_0818a220. The card itself scrolls on a
// tiled background; its picture is a textured quad by ⚓️'s model kernels.
// The native frame (84) holds effect at sp+48, the mode 44, canvas 40, the
// blitter pair 32, the card's scale 28, the background's scroll 24, the
// card's overflow past the scroll's range 20, the length 16, the length less
// 8 at 12 and frame - 48 at 8; the texture is at sp+52, the targets' middle
// at 60 and the card at 72. work stays in r10 and the frame in r11.
// Written from the listing (2026-10-15, fx-n3), in the native's statement
// order: 836 at the first compile. Graphics read into a local at the top,
// as the native loads it into r6 before the first call: the prologue then
// matches but the rest scores 864, kept as the native's form. Left: this
// frame is 88, one slot more than the native's, so every spill sits 4
// higher, and work takes r9 for r10. The smoke's window grouped as (frame
// >= 40 && frame <= 47), so that it folds into the native's one unsigned
// compare: 864 to 796.
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer callback runs at this order, where ☀️'s runs at 0x480.
#define TRANSFER_CALLBACK_ORDER 0xc80

// ⚓️'s blitter modes count the other way from ☀️'s COMPOSITE_* modes.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

// Func_081963ec's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

#define HI(v) (((s16 *)&(v))[1])

// The card's picture, unpacked into the map cell buffer, and the 256 random
// steps that dissolve it after it.
#define CARD (Ram_MapCellBuffer + 0x603e)
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
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  void *pixels;
};

// One entry of a model list, as Func_081969f8 allocates them and
// Func_08196a7c draws them.
struct Model {
  s32 kind;
  s32 next;
  void *mesh;
  void *vertices;
  struct ModelTexture *texture;
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
extern u16 Data_081974dc[];
extern u8 Data_081974f4[];
extern u8 Data_08197500[];

// The reaper's seven panels: x by side (seven each), then y.
extern u8 gData_0819750c[];
extern u8 gData_0819751a[];

// The card's quad: its mesh and its corners.
extern u8 Data_081992b0[];
extern s32 Data_081991e0[];

void BattleFx_BeginTiledCanvas(s32 bgControl);
void BattleFx_BeginTiledCanvasFilled(s32 bgControl);
void Func_081963ec(s32 slot, s32 packed);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Audio_PlayCue(s32 cue);
void EffectPosition_ApplyAnimationAndYOffset(s32 id,
                                             struct EffectPosition *position);
void Func_0815e22c(struct EffectPosition *middle);
void Func_0815f0a0(s32 resource);
void Func_081180e8(s32 phase);
void BattleFx_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct Model *Func_081969f8(s32 count);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_080150e4(s32 angle);
void Func_0801521c(s32 scale);
void Func_08196958(void *corners, void *vertices, s32 count);
void Func_08196a7c(struct Model *model);
void MemFree(void *block);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08143bb8(void);
void Func_08143000(void);
void Func_08152474(void);

// The four cards (Sleep, Baffle, Death and Sword Card): a card flies from
// the user to above the targets' middle for 48 frames, spinning upright and
// growing, the background scrolling with it as far as it can. Then it shows
// its picture: sheep jump out for the Sleep Card, butterflies flutter and
// the screen waves for the Baffle Card, and for the Death and Sword cards the
// card dissolves into the background's colours while a skeleton reaps the
// target or a dagger strikes it and breaks into shards. The screen fades in
// the last eight frames.
void Func_0818ad40(struct BattleEffectArgument *effect, s32 mode) {
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
  struct ModelTexture texture;
  struct EffectStep *step;
  struct Model *model;
  void *vertices;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up a tiled canvas, filled on the user's side, and a clipped MAX
  // blitter (additive for the Sword Card), mirrored to face the targets.
  BattleFx_BeginTiledCanvas(0x2001);
  if (effect->side == 0)
    BattleFx_BeginTiledCanvasFilled(0x2001);
  else
    BattleFx_BeginTiledCanvas(0x2001);
  REG_BLDALPHA = 0x210;
  if (mode == CARD_SWORD) {
    if (effect->side == 0)
      Func_081963ec(
          HEAP_SLOT_BLITTER,
          BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_ADDITIVE));
    else
      Func_081963ec(
          HEAP_SLOT_BLITTER,
          BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                  BLIT_ADDITIVE));
  } else {
    if (effect->side == 0)
      Func_081963ec(HEAP_SLOT_BLITTER,
                    BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_MAX));
    else
      Func_081963ec(
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
  Scheduler_AddOrUpdateCallback(Func_08143000, TRANSFER_CALLBACK_ORDER);
  if (mode == CARD_BAFFLE)
    Scheduler_AddOrUpdateCallback(Func_08152474, TRANSFER_CALLBACK_ORDER);
  REG_BG2PA = 0x100;

  for (frame = 0; frame != length; frame++) {
    // Frame 0: find the user and the targets' middle (the Death Card's 32
    // pixels toward the user), and scroll the background to the user.
    if (frame == 0) {
      Audio_PlayCue(190);
      EffectPosition_ApplyAnimationAndYOffset(effect->actor, &card);
      Func_0815e22c(&middle);
      if (mode == CARD_DEATH) {
        if (effect->side == 0)
          middle.x += 32;
        else
          middle.x -= 32;
      }
      REG_BG2X = (56 - card.x) << 8;
      scale = 0;
      scroll = 56 - card.x;
      for (i = 0; i != 64; i++)
        work->particles[i].variant = -1;
    }

    // The last eight frames: the screen fades.
    if (frame >= length - 8) {
      s32 fade = frame - length + 8;

      REG_BLDALPHA = ((fade + 8) << 8) | (16 - fade * 2);
    }
    if (frame == 48 && mode == CARD_BAFFLE)
      Audio_PlayCue(138);

    // Frames 0-47: the card flies to the middle, rising and growing; the
    // background follows it as far as it can scroll.
    if (frame <= 47) {
      s32 x = card.x - 64 + frame * (middle.x - card.x) / 48;

      if (x < 0) {
        overflow = x;
        x = 0;
      } else if (x > 112) {
        overflow = x - 112;
        x = 112;
      }
      REG_BG2X = scroll = -(x << 8);
      card.y--;
      scale += 0x400;
      if (scale > 0x8000)
        scale = 0x8000;
    }

    if (mode != CARD_SLEEP) {
      if (mode == CARD_BAFFLE) {
        // The screen waves, line by line.
        s32 *line = work->bg2X;

        for (i = 0; i != 160; i++)
          *line++ =
              scroll - ((Trig_Sin((frame << 12) + i * 0x1000) << 2) >> 10);
      } else {
        // The background steps to the smoke's colours, then the picture's;
        // from frame 48 the card dissolves in 16 random steps.
        if ((mode == CARD_DEATH || mode == CARD_SWORD) &&
            (frame >= 40 && frame <= 47))
          Func_0815f0a0((s32)&ResourceId_SmokeSheet);
        if (frame > 51) {
          if (mode == CARD_DEATH)
            Func_0815f0a0((s32)&ResourceId_RedCrescentSheetA);
          else
            Func_0815f0a0((s32)&ResourceId_DaggerSheet);
        }
        if (frame == 48) {
          u8 *noise;

          for (noise = CARD_NOISE; noise != CARD_NOISE + 256; noise++)
            *noise = Random16() & 15;
        }
        if (frame >= 48 && frame <= 63) {
          for (i = 0; i != 64; i++) {
            for (j = 0; j != 32; j++) {
              if (CARD_NOISE[(i & 7) * 32 + j] == frame - 48)
                CARD[i * 32 + j] = 0;
            }
          }
        }
      }
    }

    // The card, until its picture takes over.
    if (frame < (mode == CARD_SLEEP ? 120 : mode == CARD_SWORD ? 72 : 80)) {
      vertices = Runtime_BumpAllocateAlternatePool(32);
      model = Func_081969f8(1);
      texture.logWidth = 5;
      texture.logHeight = 6;
      model->vertices = vertices;
      texture.pixels = CARD;
      model->kind = 8;
      model->texture = &texture;
      model->mesh = Data_081992b0;
      Render_ResetTransformState();
      Func_080151e4(0x10000, 0x10000, 0x10000);
      Func_08015160(overflow << 16, (card.y - 64) << 16, 0);
      if (frame <= 47) {
        if (effect->side == 0)
          Func_080150e4((frame << 10) - 0xd000);
        else
          Func_080150e4((52 - frame) << 10);
        SceneTransform_ApplyPitch((52 - frame) << 10);
      } else {
        if (effect->side == 0)
          Func_080150e4(-0x1000);
        else
          Func_080150e4(0x1000);
        SceneTransform_ApplyPitch(0x1000);
      }
      SceneTransform_ApplyYaw(0x1770);
      Func_0801521c(scale);
      Func_080151e4(0x10000, 0x20000, 0x10000);
      Func_08196958(Data_081991e0, vertices, 4);
      Func_08196a7c(model);
      MemFree(model);
      MemFree(vertices);
    }

    if (mode == CARD_BAFFLE) {
      // Frame 48: the butterflies take flight, two a frame for 32 frames;
      // each flutters to the ground and skims along it.
      if (frame == 48) {
        Func_081180e8(136);
        work->transferMode = 2;
        work->transferValue = 50;
        REG_BLDALPHA = 0x1010;
      }
      if (frame >= 48 && frame <= 79) {
        for (i = 0; i != 2; i++) {
          step = &work->particles[(frame - 48) * 2 + i];
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
          BattleFxKernels_IntegrateVector2(step, 62, -0x1000);
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
        Audio_PlayCue(212);
      if (frame == 64)
        Func_081180e8(-1);
      if (frame >= 48 && frame <= 79) {
        step = &work->particles[frame - 48];
        step->x = 64 << 16;
        step->y = 48 << 16;
        step->velocityX = ((Random16() & 127) - 64) << 11;
        step->velocityY = ((Random16() & 127) - 64) << 11;
        step->variant = mode;
      }
      for (i = 0; i != 64; i++) {
        step = &work->particles[i];
        if (step->variant >= 0 && step->variant <= 63) {
          draw[0](canvas, CARD_NOISE + ((step->variant / 4 & 3) << 10),
                  overflow + HI(step->x) - 16, HI(step->y) - 16, 32, 32);
          BattleFxKernels_IntegrateVector2(step, 62, 0x2000);
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
        Audio_PlayCue(212);
        Func_081180e8(-1);
        work->shakeFrames = 4;
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        Func_0814cd48(effect->actors[0], 7, -1, 0, 16);
        for (i = 0; i != 64; i++) {
          s32 angle = Random16() & 0xffff;
          s32 force = (Random16() & 127) + 64;

          step = &work->particles[i];
          step->x = 64 << 16;
          step->y = (i + 48) << 16;
          step->velocityX = (Trig_Cos(angle) * force) >> 6;
          step->velocityY = (Trig_Sin(angle) * force) >> 6;
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
                  work->sheet + 0x2580 + fade * 696 + Data_081974dc[cell],
                  overflow + HI(step->x) - Data_081974f4[cell] / 2,
                  HI(step->y) - Data_08197500[cell] / 2, Data_081974f4[cell],
                  Data_08197500[cell]);
          BattleFxKernels_IntegrateVector2(step, 60, 0);
        }
      }
    }

    if (mode == CARD_DEATH) {
      // The reaper rises in seven panels and swings at frame 64.
      if (frame == 60)
        Audio_PlayCue(212);
      if (frame == 64) {
        Audio_PlayCue(144);
        Func_081180e8(-1);
        BattleFx_StartActorMotion(effect->actors[0], 1, 0x20000, 0x80000,
                                  0x20000, 110);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
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
            Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
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
    Func_08158ce0(4, 4);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  if (mode == CARD_BAFFLE)
    Scheduler_RemoveCallback(Func_08152474);
  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Func_08143bb8();
}
