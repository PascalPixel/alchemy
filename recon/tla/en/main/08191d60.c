// Draft, differ 538 of 893 (draft 881 instructions, no device): summon 16,
// written from the listing on The Broken Seal's summon skeleton
// (BattleEffect_RunNereid: canvas layer, blitter pair through
// BattleEffect_FetchRectangleBlitters, transfer callback at 0xc80,
// SelectLivingTargets, SpawnObjects, a frame loop with an A/B skip,
// PlaceFormationObjects, RunImpactBurst and the twelve object releases). Its
// rings use the ARM model renderer as summon 25's pillars do (Func_0819273c).
// The native stack frame is 60 bytes: the blitter pair at sp+52, the ring
// texture at sp+44, effect sp+40, canvas sp+36, graphics sp+32, &draw
// sp+28, focus sp+20/24, frame - 217 at sp+16 and frame - 91 at sp+12.
// Left: mostly allocation. The native keeps the mote pointer in r7, the
// loop counter in r10 and size << 14 in r8 (formed after the draw, again in
// each arm of the gather test); here r6, r8 and r7, and frame - 91 stays in
// r10 instead of sp+12. The motes' first loop does not hoist its 255.
// Measured: the flame column x as (i * 4 + 44) << 16 (strength-reduced to
// one giv; the native adds a 3i giv to i: (i * 3 + i + 44), 640 -> 558),
// -((r & 3) + 2) << 16 negated before the shift, the swirl's pull signed
// (Random16 returns u32; unsigned it divides with lsrs: 558 -> 538).
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "input.h"
#include "mem.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The integer (pixel) half of a 16.16 value, read as the halfword it is.
#define HI(v) (((s16 *)&(v))[1])

// Summon 16's frames: the shower of motes, the rising flames, the rings and
// the closing glow, by frame.
#define SUMMON16_FRAMES 296
#define SUMMON16_SKIP_TO 232

struct Point2D {
  s32 x;
  s32 y;
};

// A texture for the ARM model renderer: its size as powers of two and the
// address of its pixels.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

// A model for the ARM renderer (Render_DrawModelList).
struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0C1;
extern char ResourceId_Data134;
extern struct EffectStep gMapCellBuffer[];
extern const u8 Data_08196f28[];
extern u16 gData_08197410[];
extern const u16 Data_08198ec4[];
extern const u8 Data_08198cac[];
extern const u8 Data_08199f34[];
extern const u8 Data_08199f38[];
extern const s8 Data_08199f3c[];

void BattleEffect_StartCanvasLayer(s32 layer);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_FetchRectangleBlitters(s32 flip, DrawRectangle *out);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Graphics_LoadGreyRamp(void);
void BattleEffect_SelectLivingTargets(struct BattleEffectArgument *effect);
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 priority);
void Func_0816442c(s32 formation, s32 x, s32 y);
void Func_0816467c(s32 formation, s32 x, s32 y);
void Func_0814cd48(s32 actor, s32 palette, s32 mode, s32 timer, s32 frames);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Func_08164a4c(s32 blue, s32 green, s32 red);
void Func_08158ce0(s32 randomMask, u32 range);
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase);
void Sound_PlayFar(s32 cue);
void Resource_LoadAndDecompress(s32 id, void *target, s32 unpack, s32 wait);
s32 Task_Add(void (*callback)(void), s32 order);
s32 Task_Remove(void (*callback)(void));
void HeapSlot_Release(s32 slot);
void WaitFrames(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void Render_TransformVertices(const u8 *vertices, void *projected, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void ResourceObject_ReleaseFar(void *object);

// Summon 16. A shower of 256 motes drifts down the screen and, from frame
// 91, gathers toward the middle; from frame 97 flames rise in twelve columns
// and from frame 136 four textured rings spread out from the summoned
// objects. At frames 212 to 268 the targets are struck in turn, and the
// scene closes with a glow of the whole palette. A or B skips to frame 232.
//
// The work's particles 0-11 are the flames (velocityZ picks the blitter, 2
// retires the column), particles 24-39 the embers of the closing glow; the
// map cell buffer holds the motes (variant counts down the frames before a
// mote appears, 0 is a live mote).
void BattleEffect_RunMoloch(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct ModelTexture texture;
  struct Point2D focus;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;
  struct EffectStep *mote;
  struct EffectStep *flame;
  struct RenderModel *model;
  void *projected;
  s32 frame;
  s32 i;
  s32 k;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet + 0x3000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Graphics_LoadGreyRamp();
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  focus.x = 128 << 16;
  focus.y = 64 << 16;
  BattleEffect_SelectLivingTargets(work->effect);
  WaitFrames(1);
  BattleEffect_SpawnObjects(12, 654, 2);

  for (frame = 0; frame != SUMMON16_FRAMES; frame++) {
    if ((gInput.repeat & 3) && frame <= 231 && frame > 15) {
      Sound_PlayFar(144);
      frame = SUMMON16_SKIP_TO;
    }

    if (frame == 0) {
      for (i = 0; i != 64; i++)
        work->particles[i].variant = -1;

      // The motes start along the top, each after its own delay.
      mote = gMapCellBuffer;
      for (i = 0; i != 256; i++) {
        Random16();
        mote->variant = i;
        mote->x = ((Random16() & 255) - 64) << 16;
        mote->y = 0;
        mote->velocityX = 0;
        mote->velocityY = 0;
        mote++;
      }

      // Twelve flame columns rise from y = 72, four pixels apart.
      flame = work->particles;
      for (i = 0; i != 16; i++) {
        flame->x = (i * 3 + i + 44) << 16;
        flame->y = 72 << 16;
        flame->velocityX = 0;
        flame->velocityY = -((Random16() & 3) + 2) << 16;
        flame->velocityZ = 0;
        flame->variant = -i - (Random16() & 15);
        flame++;
      }

      // The embers of the closing glow start at (54, 56).
      flame = &work->particles[24];
      for (i = 0; i != 16; i++) {
        flame->x = 54 << 16;
        flame->y = 56 << 16;
        flame->variant = i * -2;
        flame->velocityX = -(((Random16() & 3) << 16) + (2 << 16));
        flame->velocityY = -(Random16() & 3) << 15;
        flame++;
      }
    }

    if (frame <= 271) {
      mote = gMapCellBuffer;
      for (i = 0; i != 256; i++) {
        if (mote->variant == 0) {
          s32 size = Data_08196f28[i & 7];
          s32 fall;

          draw[0](canvas, graphics + gData_08197410[size - 1],
                  HI(mote->x) - (size >> 1), HI(mote->y) - size, size,
                  size * 2);
          fall = size << 14;

          // Frames 91-119: motes still at rest head for a random point of
          // the middle, timed to arrive as they reach the ground.
          if ((u32)(frame - 91) <= 28 && mote->velocityX == 0 &&
              HI(mote->y) <= 113) {
            s32 dx;

            mote->velocityX = ((Random16() & 63) << 16) + (56 << 16);
            dx = mote->velocityX - mote->x;
            mote->velocityX = dx / (((114 << 16) - mote->y) / (size << 14));
          }

          // Frames 142-161: they swirl about the centre line.
          if ((u32)(frame - 142) <= 19) {
            s32 distance = mote->x - (96 << 16);
            s32 pull;

            if (distance < 0)
              distance = -distance;
            pull = (mote->y / 2 - distance + (s32)((Random16() & 31) << 16) +
                    (96 << 16)) /
                   32;
            if (mote->x < 96 << 16 && pull > 0)
              pull = -pull;
            if (mote->x > 96 << 16 && pull < 0)
              pull = -pull;
            mote->velocityX += pull / 8;
          }

          mote->x += mote->velocityX + Trig_Sin(mote->y >> 5) / 5;
          mote->y += mote->velocityY + fall;
          if (mote->y > 114 << 16) {
            if (frame > 215) {
              // Late motes fly back in from the right.
              mote->variant = 0;
              mote->x = 120 << 16;
              mote->y = ((Random16() & 127) - 32) << 16;
              mote->velocityX = ((Random16() & 3) - 8) << 16;
              mote->velocityY = 0;
            } else {
              mote->variant = Random16() & 255;
              mote->x = ((Random16() & 255) - 64) << 16;
              mote->y = -8 << 16;
              mote->velocityX = 0;
              mote->velocityY = 0;
            }
          }
        } else {
          mote->variant--;
        }

        // Frames 217-245: the wind turns.
        if ((u32)(frame - 217) <= 28) {
          mote->velocityX -= 0x2000;
          mote->velocityY += 0x2000;
        }
        mote++;
      }
    }

    // Frames 91-255: the flame columns, a 32x64 cell for every four frames.
    if ((u32)(frame - 91) <= 164) {
      flame = work->particles;
      for (i = 0; i != 12; i++) {
        if (flame->velocityZ != 2) {
          if ((u32)flame->variant <= 23) {
            draw[flame->velocityZ](canvas,
                                   work->sheet + (flame->variant >> 2) * 2048,
                                   HI(flame->x), HI(flame->y), 32, 64);
            flame->x += flame->velocityX;
            flame->y += flame->velocityY;
          }
          if (++flame->variant == 24) {
            flame->x = (i * 3 + i + 44) << 16;
            flame->y = 72 << 16;
            flame->variant = 0;
            flame->velocityY = -((Random16() & 3) + 2) << 16;
            flame->velocityZ = 0;
            if ((u32)(frame - 97) <= 42)
              flame->velocityZ = 1;
            else if (frame > 140)
              flame->velocityZ = 2;
          }
          if (frame == 136) {
            if (flame->x < 80 << 16)
              flame->velocityX -= 4 << 16;
            else
              flame->velocityX += 4 << 16;
          }
        }
        flame++;
      }
    }

    // Frames 201-271: the embers of the closing glow.
    if ((u32)(frame - 200) <= 71 && frame > 200) {
      flame = &work->particles[24];
      for (i = 0; i != 16; i++) {
        if ((u32)flame->variant <= 23) {
          draw[0](canvas, work->sheet + (flame->variant >> 2) * 2048,
                  HI(flame->x), HI(flame->y), 32, 64);
          flame->x += flame->velocityX;
          flame->y += flame->velocityY;
        }
        if (++flame->variant == 24) {
          flame->x = 54 << 16;
          flame->y = 56 << 16;
          flame->velocityX = -(((Random16() & 3) << 16) + (2 << 16));
          flame->variant = 0;
        }
        flame++;
      }
    }

    // From frame 136: four rings, each starting at its own frame, grow and
    // fade out over eight frames.
    if (frame > 135) {
      projected = Heap_AllocEwram(1024);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 5;
      texture.pixels = work->sheet + 0x3000;
      model->mesh = Data_08198ec4;
      model->kind = 7;
      model->texture = &texture;
      model->vertices = projected;
      model->scrollU = 0;
      model->scrollV = 0;
      for (k = 0; k != 4; k++) {
        s32 start = Data_08199f34[k] + 136;
        s32 scale;
        s32 shade;

        if (frame <= start)
          continue;
        scale = (frame - start) * Data_08199f38[k] * 2000 + 0x8000;
        shade = (start - frame) * 8 + 56;
        if (shade > 0)
          shade = 0;
        if (shade <= -64)
          continue;
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160(Data_08199f3c[k] << 16, 16 << 16, 0);
        Func_080151e4(scale, scale, scale);
        SceneTransform_ApplyPitch(0xbb8);
        model->scrollU = ((frame + k * 5) & 7) << 4;
        Render_TransformVertices(Data_08198cac, projected, 32);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(projected);
    }

    if (frame > 139)
      Func_0816442c(5, focus.x, focus.y);

    // Frames 212-268, every eight: the targets flash and are thrown back.
    for (k = 0; k != 8; k++) {
      if (frame == k * 8 + 212) {
        for (i = 0; i != work->effect->count; i++) {
          Func_0814cd48(work->effect->actors[i], 7, 5, i, 8);
          BattleEffect_StartActorMotion(effect->actors[i], 1, 0x10000, 0x40000,
                                    0x10000, k * 16 + 100);
        }
      }
    }
    if ((u32)(frame - 201) <= 54)
      work->shakeFrames = 2;

    if (frame == 32)
      Sound_PlayFar(246);
    if (frame == 90)
      Sound_PlayFar(225);
    if (frame == 140)
      Sound_PlayFar(154);
    if (frame == 200)
      Sound_PlayFar(208);
    if (frame == 232)
      Sound_PlayFar(144);

    // Frames 256-271 brighten the whole screen to white; at 272 the battle
    // phase resumes and the canvas fades back.
    if (frame == 272) {
      Func_081180e8(134);
      work->transferMode = 3;
      work->transferValue = 0x02020202;
      Graphics_LoadGreyRamp();
    } else if ((u32)(frame - 256) <= 15) {
      work->transferMode = 1;
      work->transferValue = 0x10101010;
      if (frame > 270)
        work->transferValue = 0x3f3f3f3f;
      Func_08164a4c(2, 2, 2);
    }

    Func_08158ce0(4, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Func_0816467c(5, focus.x, focus.y);
  for (i = 0; i != 12; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  BattleEffect_EndCanvasLayer();
}
