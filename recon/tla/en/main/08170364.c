// Draft of BattleEffect_RunMegaera, decompiled from the listing: differ 1274 of
// 1152 (draft 1136), the same 90 calls and control flow; allocation differs.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "ram_buffer.h"

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

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

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode);   // SpawnObjects
void BattleEffect_SelectLivingTargets(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Task_Add(void (*callback)(void), s32 order);
void Task_Remove(void (*callback)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void WaitFrames(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(void *mesh, s32 columns, s32 width, s32 height);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#include "input.h"

void Func_0815f0a0(s32 palette);
void Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                   s32 mode);
void Func_0816442c(s32 formation, s32 x, s32 y); // ..PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x, s32 y); // ..RunImpactBurst
void ResourceObject_ReleaseFar(void *object);

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_WhirlBeamSheet;
extern char ResourceId_Data134;
extern char ResourceId_TornadoSheet;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_SparkleDots;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_TanPalette;
extern u8 gMapCellBuffer[]; // 512 sparks
extern u8 gData_02014000[];
extern u8 Data_02014c56[];
extern u8 Data_02016e16[];
extern const s32 Data_08196e94[2];
extern const u16 gData_08197410[];
extern const u16 Data_0819744c[]; // the sparkles' cells
extern const u8 Data_0819745e[];  // and sizes
extern const u16 gData_08197486[];
extern const u8 gData_08197492[];
extern const u8 gData_08197498[];
extern const u8 Data_08198bf4[];
extern const u8 Data_08198bf8[];

// Abilities: Megaera.
//
// Two bolts of lightning fall at frames 0 and 16, each with a burst of
// sparks; a whirl of flame turns over the field from 64 while smoke rises,
// and at 108 Megaera (eleven objects) swoops in across the targets in a
// trail of sparkles. Holding A or B after frame 10 skips to the swoop.
void BattleEffect_RunMegaera(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  DrawRectangle blitters[2];
  s32 request[2];
  s32 placement[4];
  struct EffectStep *p;
  struct EffectStep *sparks;
  s32 x;
  s32 y;
  s32 velocityX;
  s32 velocityY;
  s32 frame;
  s32 i;
  s32 j;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x3800, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_WhirlBeamSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, gData_02014000, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             gData_02014000 + 0xc56, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots,
                             gData_02014000 + 0x2e16, 0, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  for (i = 0; i != 64; i++) {
    work->particles[i].variant = -1;
  }
  for (i = 0, p = &work->particles[24]; i != 16; i++, p++) {
    p->x = Random16() & 127;
    p->y = (Random16() & 7) + 56;
    p->variant = -(Random16() & 15);
  }
  for (i = 0, p = work->particles; i != 32; i++, p++) {
    p->x = ((Random16() & 31) + 28) << 16;
    p->y = ((Random16() % 48) + 56) << 16;
    p->velocityX = ((Random16() & 255) - 128) << 12;
    p->velocityY = (16 - (Random16() & 127)) << 10;
    p->variant = ~i;
  }
  sparks = (struct EffectStep *)gMapCellBuffer;
  for (i = 0; i != 512; i++) {
    sparks[i].variant = -1;
  }
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
  BattleEffect_SelectLivingTargets(work->effect);
  WaitFrames(1);

  for (frame = 0; frame != 170; frame++) {
    if ((gInput.repeat & 3) && frame > 10) {
      if (frame > 107) {
        goto skipped;
      }
      frame = 107;
    }
    if (frame == 107) {
      Sound_PlayFar(212);
    }
  skipped:
    if (frame == 148) {
      Func_081180e8(134);
    }
    if (frame == 0) {
      x = 0x200000;
      y = 0x480000;
      velocityX = 0x90000;
      velocityY = -0x20000;
      BattleEffect_SpawnObjects(2, 657, 2);
    }
    if (frame == 108) {
      ResourceObject_ReleaseFar(work->objects[0]);
      ResourceObject_ReleaseFar(work->objects[1]);
      BattleEffect_SpawnObjects(11, 656, 2);
    }

    // Megaera swoops, trailing sparkles.
    if (frame > 107) {
      x += velocityX;
      y += velocityY;
      velocityX = velocityX * 59 / 64;
      velocityY = velocityY * 59 / 64;
      Func_0816442c(4, x, y);
      if (frame == 108) {
        for (i = 0; i != 512; i++) {
          sparks[i].variant = -1;
        }
      }
      if ((u32)(frame - 108) <= 23) {
        s32 count = 10 - (frame - 108) / 2;

        if (count > 0) {
          s32 half = x / 2;

          for (i = 0; i != count / 5; i++) {
            s32 angle;
            s32 radius;

            p = &sparks[(((frame - 108) << 5) + i) & 511];
            angle = Random16();
            radius = (Random16() & 31) + 16;
            p->x = radius * Trig_Sin(angle) + half + 0x100000;
            p->y = radius * Trig_Cos(angle) + y - 0x80000;
            p->velocityX = 0;
            p->velocityY = 0;
            p->variant = Random16() & 15;
          }
        }
      }
    }
    if ((u32)(frame - 64) <= 3) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame > 103) {
      Func_0815f0a0((s32)&ResourceId_TanPalette);
    }
    if (frame == 64) {
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
    }

    // The whirl of flame, and smoke.
    if (frame > 63) {
      if (frame <= 111) {
        DrawRectangle draw = blitters[frame == 108];
        s32 picture = (frame / 4) % 3;
        s32 sway = (Trig_Sin(frame << 11) << 3) >> 16;
        u8 *cells = work->sheet + picture * 3 * 0x800;

        draw(canvas, cells, sway - 16, 0, 96, 64);
        draw(canvas, cells, sway - 16, 64, 96, 52);
        picture = (frame / 2) % 3;
        sway = (Trig_Sin((frame << 12) + 0x8000) << 3) >> 16;
        cells = work->sheet + picture * 3 * 0x800;
        draw(canvas, cells, sway, 0, 96, 64);
        draw(canvas, cells, sway, 64, 96, 52);
      }
      for (i = 0, p = work->particles; i != 24; i++, p++) {
        if ((u32)p->variant <= 15) {
          blitters[i & 1](canvas,
                          work->sheet + 0x4800 + ((p->variant / 4) << 11),
                          HI(p->x) - 16, HI(p->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(p, 62, -0x4000);
        }
        p->variant++;
        if (frame == 108) {
          p->velocityX += 0x20000;
          p->velocityY /= 4;
        }
        if (frame <= 107) {
          if (p->variant == 16) {
            p->variant = 0;
          }
          if (p->variant == 0) {
            p->x = ((Random16() & 31) + 28) << 16;
            p->y = ((Random16() % 48) + 56) << 16;
            p->velocityX = ((Random16() & 255) - 128) << 12;
            p->velocityY = (16 - (Random16() & 127)) << 10;
          }
        }
      }
    }
    if (frame == 64) {
      Sound_PlayFar(163);
    }
    for (i = 0, j = 75; i != 8; i++, j += 4) {
      if (frame == j) {
        Sound_PlayFar(134);
      }
    }
    if (frame == 10) {
      work->shakeFrames = i;
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
      }
    }
    if (frame > 107) {
      for (i = 0, p = sparks; i != 512; i++, p++) {
        s32 k;
        s32 size;

        if ((u32)p->variant > 35) {
          continue;
        }
        k = (p->variant / 2) % 9;
        size = Data_0819745e[k];
        blitters[0](canvas, Data_02016e16 + Data_0819744c[k],
                    HI(p->x) - size / 2, HI(p->y) - size / 2, size, size);
        BattleEffectKernels_IntegrateVector2(p, 58, 0);
        if (i & 1) {
          p->velocityX += -0x800;
          p->velocityY += 0x1000;
        } else {
          p->velocityX += -0x1000;
          p->velocityY += 0x800;
        }
        p->variant++;
      }
    }

    // Two bolts of lightning.
    if (frame <= 63) {
      s32 bolt;

      HeapSlot_Release(188);
      for (bolt = 0; bolt != 2; bolt++) {
        s32 start = bolt * 16;
        s32 strike = bolt * 16 + 10;
        s32 column = bolt * 32 + 24;

        if (frame == start + 5) {
          Sound_PlayFar(212);
        }
        if (frame == start + 9) {
          work->shakeFrames = 8;
          Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
          Sound_PlayFar(144);
        }
        if (frame < start) {
          continue;
        }
        {
          s32 t = frame - start;
          s32 fall = t * 16 - 16;

          request[0] = Data_08196e94[0];
          request[1] = Data_08196e94[1];
          if (fall > 131) {
            fall = 132;
          }
          placement[3] = 0;
          placement[1] = 0;
          placement[0] = column << 17;
          placement[2] = fall << 16;
          Render_ApplyProjectedPlacementFar(work->objects[bolt], placement,
                                            request, 0);
          if (t == 9) {
            Iwram_ClearWords(
                (u8 *)0x06010e00 +
                    ((((u16 *)work->objects[bolt])[4] << 22) >> 17),
                0x200);
          }
        }
        if (frame == strike) {
          for (j = 0, p = &sparks[bolt * 256]; j != 256; j++, p++) {
            s32 angle = Random16() & 0x7fff;
            s32 speed = Random16() & 255;

            p->x = 0x180000 + bolt * 0x200000;
            p->y = 0x700000;
            p->velocityY = (speed * -Trig_Sin(angle)) >> 5;
            p->velocityX = (speed * Trig_Cos(angle)) >> 5;
            p->variant = 0;
          }
        }
        if (frame < strike) {
          continue;
        }
        if (frame < start + 18) {
          blitters[0](canvas, Data_02014c56 + (((frame + bolt) / 4) & 3) * 2880,
                      bolt * 32 + 12, 0, 24, 112);
        }
        for (j = 0; j != 2; j++) {
          s32 k = j & 3;
          s32 gx = column + (Random16() & 7) - (gData_08197492[k] >> 1);
          s32 gy = (Random16() & 63) - (gData_08197498[k] >> 1);

          Func_08196404(188, 7, 7, 3 | Data_08198bf4[Random16() & 3],
                        Data_08198bf8[effect->variant]);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas, gData_02014000 + gData_08197486[k], gx - 4, gy + 40,
              gData_08197492[k], gData_08197498[k]);
          HeapSlot_Release(188);
        }
      }
      BattleEffect_LoadBlitter(188, 3);
      blitters[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      if (frame <= 69) {
        for (i = 0, p = sparks; i != 512; i++, p++) {
          s32 size;

          if (p->variant < 0) {
            continue;
          }
          size = i % 3 + 1;
          blitters[p->velocityY > 0](canvas, sprites + gData_08197410[size - 1],
                                     HI(p->x) - size / 2, HI(p->y) - size, size,
                                     size * 2);
          BattleEffectKernels_IntegrateVector2(p, 62, 0x4000);
          p->variant++;
          if (p->velocityY > 0 && HI(p->y) > 104) {
            p->variant = -1;
          }
        }
      }
    }
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Func_0816467c(4, x, y);
  for (i = 0; i != 11; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}
