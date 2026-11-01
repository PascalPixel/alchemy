// Draft, differ 234 of 734, skeleton gap 58 (tla-en, 2026-11-01)

// No twin: an effect new to ⚓️, reached from the scenes through the
// far-call table (table_47.s) with a kind of 78, 79 or 80. Its frame is the
// canvas skeleton of the scene effects (its own heap blocks, the work marked
// active, the canvas transfer), and its pictures are textured models drawn
// by the IWRAM renderer (Render_AllocateModelList, Render_TransformVertices,
// Render_DrawModelList), as 0813d098's star is.

// Left: one allocation swap. The native keeps the centre's address in r11 and
// spills the corners and the burst's y; here global.c ranks the corners
// (14 refs over 173 insns, priority .243) just ahead of the centre's address
// (27 over 462, .234), so the corners take r11, y a register and the address a
// stack slot, and the frame is 76 instead of 80. The refs can be read off the
// listing, so the native's differ must be in live lengths or in one more
// reference: 28 refs at 462 is enough, or 27 with the loop at 447 or less, or
// the corners at 184 or more. Proof the swap is the cause: reading the ring
// loop's corners through model->corners (one wrong instruction) gives frame 80,
// differ 95, skeleton 31.
//
// Left after that swap, measured under that trial:
// - The preheader: the native sets index (-96) before the centre's address.
// - The flash preheader's scratch registers start one rotation away (index in
//   r4, pool r0, zero r1, 15 r2 in the native).
// - model->depthBias = 0: the native's zero is in r0 and its base reloaded in
//   r1, between the two texture masks. No statement position of the eight tried
//   gives it; zero seems to be set while r2 and r3 are busy.
// - The second model block's order, which the trial itself disturbs.
// - The crystal table byte: `start += 48` gives start r1 and bias r0 as the
//   native has them (15 refs beat bias's 15 over a shorter life) but loses
//   `ldrb r3; adds r1, r3, #0`; `Data[i] + 48` keeps the copy and swaps r0, r1.
//
// Found this round: draw[2] for the unused word at sp+40. The flash pointer is
// a giv of a base outside the loop, `flashes = &gData_02014000[index]` and
// `flash = &flashes[k]`, set after the hoisted constants as the native's is.
// `speed = 31; speed &= Random16()` gives `movs r5, #31; ands r5, r0`; the
// constant set moves below the call.
//
// Tried for the swap: center as s32[3] (no change); a pointer to the centre in
// or out of the loop; named temporaries for the transforms' arguments (copy
// propagation removes them); an empty asm reading &center after the call
// (PRE loses the hoist, the address becomes a doubled spilled pseudo), in the crystal
// before the read (43 refs, the address outranks frame and takes r10) or after
// it (26 refs): none kept. The two hoisted copies at the flash and at cx match
// as one movable, so only one preheader ref reaches the address; unmatching them would
// be the 28th ref, but both sources are the same register. The crystal's read is
// single-use and so substituted, which is why only it goes through r11.

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "heap_release.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "bg_scroll.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "graphics.h"
#include "resource.h"

// Heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BATTLE_EFFECT 92 // battleFxWork[0]
#define SLOT_BATTLE_CANVAS 96 // battleFxWork[1]
#define SLOT_BLITTER 104      // unk64[1]

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define TASK_PASS_FRAME 0xc80

// The pixel half of a 16.16 value.
#define HI(value) (((s16 *)&(value))[1])

// The three kinds the scenes ask for.
enum { KIND_RIGHT = 78, KIND_LEFT = 79, KIND_FINALE = 80 };

// A texture for the IWRAM renderer.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
};

// One model for the IWRAM renderer.
struct ModelDraw {
  s32 kind;
  s32 next;
  const void *model;
  void *corners;
  struct ModelTexture *texture;
  s32 depthBias;
  u8 unk18;
  u8 shade;
  u8 unk1a[2];
};

// The four crystals: the frame each starts after 48, how fast it grows and
// how tall.
extern u8 Data_081977ec[];
extern u8 Data_081977f0[];
extern u8 Data_081977f4[];

// The models: the rings' quad, its corners, the crystal and its corners.
extern u8 Data_08199220[];
extern u8 gData_081991b0[];
extern u8 gData_081990d0[];
extern u8 gData_08199090[];

// The crystal texture and the flashes live in EWRAM after the map cells.
extern u8 Data_02012000[];
extern struct EffectStep gData_02014000[];

extern u32 gCameraSceneParameters[];

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0C9;
extern char ResourceId_Data0DA;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_EffectPalette03;

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
s32 BattleEffect_ProcessPendingGraphicsTransfer(void);  // the canvas transfer callback
void Palette_StepFadeTransfer(void);
s32 Task_Add(void *func, s32 order);
s32 Task_Remove(void *func);
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
u32 Random16(void);
void EffectPosition_ProjectBattler(s32 actor,
                                                 struct EffectPosition *out);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void Func_08158ce0(s32 x, s32 y);    // Camera_ApplyShake
void BattleEffect_CountDownBattlerTints(void);
void Func_08164a4c(s32 blueDelta, s32 greenDelta, s32 redDelta);
void Func_08118150(s32 kind);
void Func_0815b434(u8 *map, s32 size, s32 scale);
void Func_0818caa8(void *source, s32 x, s32 y, s32 size);
struct ModelDraw *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *out, s32 count);
void Render_DrawModelList(struct ModelDraw *draw);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_080150e4(s32 roll);
void SceneTransform_ApplyPitch(s32 pitch);
void SceneTransform_ApplyYaw(s32 yaw);
void wait(s32 frames);

// Rings and crystals gather on the user, for the scenes.

// A glow on the canvas map widens over the user for 56 frames while 16
// spinning rings rise around it, four abreast. From frame 48 the screen
// shakes, flashes burst upward and four crystals grow out of the ground one
// after another, spinning. The user flashes at frames 32 to 68. Kinds 78
// and 79 stand to either side and hand over to the battle (Func_08118150)
// at frame 62; kind 80 runs 100 frames, fades to white and restores the
// scene's view itself.

void Func_08142944(struct BattleEffectArgument *effect, s32 kind) {
  struct EffectPosition center;
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frames;
  s32 frame;
  s32 index;
  s32 i;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  work->vblanksSinceTransfer = 1;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_ADDITIVE, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x3000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, Data_02012000, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  frames = 100;
  if (kind != KIND_FINALE)
    frames = 76;
  for (frame = 0; frame != frames; frame++) {
    index = (frame - 48) * 2;
    // The effect's centre, beside or above the user.
    EffectPosition_ProjectBattler(effect->actor, &center);
    if (kind == KIND_RIGHT) {
      center.x += 40;
      center.y -= 24;
    } else if (kind == KIND_LEFT) {
      center.x -= 16;
      center.y -= 24;
    } else {
      center.y -= 48;
    }

    if (frame == 0) {
      work->transferMode = 2;
      work->transferValue = 50;
      if (kind == KIND_LEFT)
        Iwram_CopyWords((void *)BG_PLTT,
                        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                        128);
      else
        Iwram_CopyWords(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_EffectPalette03), 128);
      if (frame == 0)
        Sound_PlayFar(140);
    }
    if (frame == 48)
      Sound_PlayFar(144);
    if (frame == 56)
      Sound_PlayFar(148);
    if (frame == 32)
      Func_0814cd48(effect->actor, 7, -1, -1, -1);
    if (frame == 68)
      Func_0814cd48(effect->actor, 0, -1, -1, -1);
    if (kind == KIND_FINALE) {
      // The finale fades to white from frame 62.
      if (frame > 61) {
        work->transferMode = 1;
        work->transferValue = 0x10101010;
        Func_08164a4c(2, 2, 2);
      }
    } else if (frame == 62) {
      Func_08118150(kind);
    }

    // From frame 48: shake, clear the canvas, and for 8 frames burst two
    // flashes a frame from a point near the centre.
    if (frame >= 48) {
      if (frame == 48) {
        {
          s32 *variant = &gData_02014000[0].variant;

          for (i = 0; i != 64; i++) {
            *variant = -1;
            variant += 7;
          }
        }
        work->shakeFrames = 8;
        Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      }
      if (frame >= 48 && frame <= 55) {
        s32 x = ((Random16() & 63) + center.x / 2 - 32) << 16;
        s32 y = ((Random16() & 31) + center.y) << 16;
        s32 k;
        struct EffectStep *flashes = &gData_02014000[index];

        for (k = 0; k != 2; k++) {
          struct EffectStep *flash = &flashes[k];
          s32 speed = 31; // Set first: the mask is the tied operand.
          s32 angle;

          speed &= Random16();
          angle = Random16();

          flash->x = x + (((Random16() & 15) - 8) << 16);
          flash->y = y + (((Random16() & 15) - 8) << 16);
          flash->velocityX = Trig_Sin(angle) * (speed + 32) >> 3;
          flash->velocityY = Trig_Cos(angle) * (speed + 32) >> 3;
          flash->variant = 0;
        }
      }
      {
        struct EffectStep *flash = gData_02014000;

        for (i = 0; i != 16; i++, flash++) {
          if (flash->variant >= 0 && flash->variant <= 17) {
            draw[0](canvas, work->sheet + (flash->variant / 3 << 11),
                 HI(flash->x) - 16, HI(flash->y) - 48, 32, 64);
            BattleEffectKernels_IntegrateVector2(flash, 62, -0x800);
            flash->variant++;
          }
        }
      }
    }

    if (frame >= 0) {
      s32 cx = center.x / 2 << 16;
      s32 cy = center.y << 16;
      struct ModelDraw *model;
      void *corners;

      // The glow on the canvas map widens for 56 frames.
      if (frame <= 63) {
        s32 size = 128;
        s32 width = frame + 8;

        if (width <= 64) {
          size = width * 2;
          Func_0815b434(Ram_MapCellBuffer, size, 0x10000);
        }
        Func_0818caa8(Ram_MapCellBuffer, cx >> 16, cy >> 16, size);
      }

      // Each ring has an angle (x), a spin (y) and an age (variant), the
      // later ones 4 frames apart.
      if (frame == 0) {
        for (i = 0; i != 16; i++) {
          struct EffectStep *ring = &work->particles[i];

          ring->x = Random16() & 0xffff;
          ring->y = (Random16() & 255) + 128;
          if (Random16() & 1)
            ring->y = -ring->y;
          ring->variant = -4 * i;
        }
      }

      corners = Heap_AllocEwram(0x200);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      model->depthBias = 0;
      texture.logHeight = 2;
      texture.pixels = work->sheet + 0x3000;
      model->kind = 7;
      model->texture = &texture;
      model->model = Data_08199220;
      model->corners = corners;

      // The rings rise for 24 frames, spinning and swelling.
      for (i = 0; i != 16; i++) {
        struct EffectStep *ring = &work->particles[i];

        ring->variant += 2;
        ring->x += ring->y * 2;
        if (ring->variant >= 0 && ring->variant <= 47) {
          Render_ResetTransformState();
          {
            s32 column = i & 3;

            Func_08015160(cx + ((column * 2 + 2) << 16) - 0x400000,
                          cy - ((column + 2) << 16) - 0x400000, 0);
          }
          Func_080151e4(0x8000, 0x10000, 0x10000);
          Func_080150e4(ring->x);
          Func_080151e4(Trig_Sin(ring->variant * 682) + 0x10000, 0x20000,
                        0x10000);
          Render_TransformVertices(gData_081991b0, corners, 4);
          Render_DrawModelList(model);
        }
      }

      // The four crystals grow out of the ground from frame 48 on.
      model->texture = &texture;
      model->corners = corners;
      texture.logWidth = 6;
      texture.pixels = Data_02012000;
      model->kind = 7;
      texture.logHeight = 6;
      model->model = gData_081990d0;
      for (i = 0; i != 4; i++) {
        s32 start = Data_081977ec[i];

        model->shade = (frame * 8 + i * 32) & 127;
        start += 48;
        if (frame > start) {
          s32 bias = (start - frame) * 8 + 56;

          if (bias > -16)
            bias = -16;
          if (bias > -64) {
            s32 growth = (frame - start) * Data_081977f0[i] * 2000 + 0x4000;

            model->depthBias = bias;
            Render_ResetTransformState();
            Func_08015160((center.x / 2 - 60) << 16, 0x200000, 0);
            Func_080151e4(growth * 2, Data_081977f4[i] * growth + 0x40000,
                          growth / 4);
            SceneTransform_ApplyPitch(1000);
            if (i & 1)
              SceneTransform_ApplyYaw((i * 8 + frame) << 10);
            else
              SceneTransform_ApplyYaw((i * 8 - frame) << 10);
            Render_TransformVertices(gData_08199090, corners, 32);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(corners);
    }

    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  if (kind == KIND_FINALE) {
    // Put the scene's view back by hand.
    Sound_PlayFar(390);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    Task_Remove(Palette_StepFadeTransfer);
  } else {
    BattleEffect_EndCanvasLayer();
  }
  HeapSlot_Release(SLOT_BATTLE_CANVAS);
  HeapSlot_Release(SLOT_BATTLE_EFFECT);
}
