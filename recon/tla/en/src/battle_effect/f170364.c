#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
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
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
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
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource,
                               s32 mode); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle);            // SceneTransform_ApplyRoll
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
s32 Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
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
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
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
#define gMapCellBuffer ((struct EffectStep *)Ram_MapCellBuffer) // 512 sparks
#define gData_02014000 FIXED_ADDRESS((u8 *), 0x02014000, gData_02014000)
struct Point {
  s32 x;
  s32 y;
};
extern const struct Point Data_08196e94;
extern const u16 gData_08197410[];
extern const u16 Data_0819744c[]; // the sparkles' cells
extern const u8 Data_0819745e[];  // and sizes
extern u16 gData_08197486[];
extern u8 gData_08197492[];
extern u8 gData_08197498[];
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
  s32 frame;
  u8 *sprites;
  DrawRectangle *rects;
  struct Point position;
  struct Point velocity;
  DrawRectangle blitters[2];
  struct Point request;
  s32 placement[4];
  struct EffectStep *p;
  u8 *pictures;
  s32 i;
  s32 j;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  pictures = gData_02014000;
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  rects = blitters;
  BattleEffect_FetchRectangleBlitters(0, (void **)rects);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x3800, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_WhirlBeamSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, pictures, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             pictures + 0xc56, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, pictures + 0x2e16, 0,
                             0);
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
  for (i = 0; i != 512; i++) {
    gMapCellBuffer[i].variant = -1;
  }
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
  BattleEffect_SelectLivingTargets(work->effect);
  wait(1);

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
      position.x = 0x200000;
      position.y = 0x480000;
      velocity.x = 0x90000;
      velocity.y = -0x20000;
      BattleEffect_SpawnObjects(2, 657, 2);
    }
    if (frame == 108) {
      ResourceObject_ReleaseFar(work->objects[0]);
      ResourceObject_ReleaseFar(work->objects[1]);
      BattleEffect_SpawnObjects(11, 656, 2);
    }

    // Megaera swoops, trailing sparkles.
    if (frame > 107) {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 59 / 64;
      velocity.y = velocity.y * 59 / 64;
      Func_0816442c(4, position.x, position.y);
      if (frame == 108) {
        for (i = 0; i != 512; i++) {
          gMapCellBuffer[i].variant = -1;
        }
      }
      {
        s32 t = frame - 108;
        if ((u32)t <= 23) {
          s32 count = 10 - t / 2;

          if (count > 0) {
            s32 half;

            for (i = 0, half = position.x / 2; i != count / 5; i++) {
              struct EffectStep *cell;
              s32 angle;
              s32 radius;

              cell = &gMapCellBuffer[((t << 5) + i) & 511];
              angle = Random16();
              radius = (Random16() & 31) + 16;
              cell->x = Trig_Sin(angle) * radius + half + 0x100000;
              cell->y = Trig_Cos(angle) * radius + position.y - 0x80000;
              cell->velocityX = 0;
              cell->velocityY = 0;
              cell->variant = Random16() & 15;
            }
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
        s32 picture = (frame / 4) % 3;
        s32 sway = ((Trig_Sin(frame << 11) << 3) >> 16) - 16;
        s32 which = frame == 108;
        DrawRectangle *draw = &blitters[which];
        u8 *cells = work->sheet + picture * 3 * 0x800;

        (*draw)(canvas, cells, sway, 0, 96, 64);
        (*draw)(canvas, cells, sway, 64, 96, 52);
        {
          s32 picture2 = (frame / 2) % 3;
          u8 *cells2;

          sway = (Trig_Sin((frame << 12) + 0x8000) << 3) >> 16;
          cells2 = work->sheet + picture2 * 3 * 0x800;
          (*draw)(canvas, cells2, sway, 0, 96, 64);
          (*draw)(canvas, cells2, sway, 64, 96, 52);
        }
      }
      for (i = 0, p = work->particles; i != 24; i++, p++) {
        if ((u32)p->variant <= 15) {
          s32 row = p->variant / 4;
          s32 which = i & 1;
          u8 *smoke = work->sheet + 0x4800 + (row << 11);

          rects[which](canvas, smoke, HI(p->x) - 16, HI(p->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(p, 62, -0x4000);
        }
        p->variant++;
        if (frame == 108) {
          p->velocityX += 0x20000;
          p->velocityY /= 4;
        }
        if (frame <= 107) {
          s32 v = p->variant;

          if (v == 16) {
            p->variant = v = 0;
          }
          if (v == 0) {
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
    {
      s32 crackle;

      for (i = 0, crackle = 75; i != 8; i++, crackle += 4) {
        if (frame == crackle) {
          Sound_PlayFar(134);
        }
      }
    }
    if (frame == 10) {
      work->shakeFrames = i;
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
      }
    }
    if (frame > 107) {
      for (i = 0, p = gMapCellBuffer; i != 512; i++, p++) {
        s32 k;

        if ((u32)p->variant > 35) {
          continue;
        }
        k = (p->variant / 2) % 9;
        blitters[0](canvas, (gData_02014000 + 0x2e16) + Data_0819744c[k],
                    HI(p->x) - Data_0819745e[k] / 2,
                    HI(p->y) - Data_0819745e[k] / 2, Data_0819745e[k],
                    Data_0819745e[k]);
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
      HeapSlot_Release(188);
      for (i = 0; i != 2; i++) {
        s32 start = i * 16;
        s32 column = i * 32 + 24;

        if (frame == start + 5) {
          Sound_PlayFar(212);
        }
        if (frame == start + 9) {
          work->shakeFrames = 8;
          Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
          Sound_PlayFar(144);
        }
        if (frame >= start) {
          s32 t = frame - start;
          s32 fall = t * 16 - 16;

          request = Data_08196e94;
          if (fall > 131) {
            fall = 132;
          }
          placement[3] = 0;
          placement[1] = 0;
          placement[0] = column << 17;
          placement[2] = fall << 16;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            &request.x, 0);
          if (t == 9) {
            Iwram_ClearWords(
                (u8 *)0x06010e00 +
                    ((u32)(((u16 *)work->objects[i])[4] << 22) >> 17),
                0x200);
          }
          if (frame == start + 10) {
            for (j = 0; j != 256; j++) {
              struct EffectStep *spark = &gMapCellBuffer[i * 256 + j];
              s32 angle = Random16() & 0x7fff;
              s32 speed = Random16() & 255;

              spark->x = column << 16;
              spark->y = 0x700000;
              spark->velocityY = (-Trig_Sin(angle) * speed) >> 5;
              spark->velocityX = (Trig_Cos(angle) * speed) >> 5;
              spark->variant = 0;
            }
          }
          if (frame >= start + 10 && frame < start + 18) {
            blitters[0](canvas,
                        (gData_02014000 + 0xc56) +
                            (((frame + i) / 4) & 3) * 2880,
                        column - 12, 0, 24, 112);
          }
          if (frame >= start + 10) {
            for (j = 0; j != 2;) {
              s32 k = j & 3;
              s32 gx = column + (Random16() & 7) - (gData_08197492[k] >> 1);
              s32 gy = (Random16() & 63) - (gData_08197498[k] >> 1);

              /* FAKEMATCH(barrier): an empty once-only block, which keeps the
               * table reads apart until cse2; without it, 188 of 1152
               * instructions differ in tla-en. */
              do {
              } while (0);
              Func_08196404(188, 7, 7, 3 | Data_08198bf4[Random16() & 3],
                            Data_08198bf8[effect->variant]);
              ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
                  canvas, gData_02014000 + gData_08197486[k], gx - 4, gy + 40,
                  gData_08197492[k], gData_08197498[k]);
              j++;
              HeapSlot_Release(188);
            }
          }
        }
      }
      BattleEffect_LoadBlitter(188, 3);
      rects[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      if (frame <= 69) {
        for (i = 0, p = gMapCellBuffer; i != 512; i++, p++) {
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
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Func_0816467c(4, position.x, position.y);
  for (i = 0; i != 11; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}
