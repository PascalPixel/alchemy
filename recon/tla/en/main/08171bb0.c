// Draft of Func_08171bb0, decompiled from the listing: differ 1370 of 1748
// (draft 1750), the same 109 calls and control flow; allocation differs.
// Left (2026-10-19), among others: the mirror loop of mode 2 keeps
// row * 2 and row * 64 as shifts ("not worth while" in loop.c) where the
// native reduces both to induction variables, and it reuses the sheet
// address it passed to the load where the native recomputes
// gData_02014000 + 0x1000.
// Tried (differ):
// first draft                                                      1664
// the blitter as The Broken Seal's pair (FAKEMATCH below), and frame
// declared after it before camera, sprites, frames and unit (the
// native's stack order)                                            1472
// the mode 2 sheet size 0x1000 in a local used for the load's offset
// and the clear (the native keeps the 0x1000 in r5 for both)        1370
// the mirror loop's addresses as &gData_02014000[...] (1370)

#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "ram_buffer.h"
#include "types.h"

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
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(
    void); // BattleEffect_ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void); // BattleEffect_CountDownBattlerTints
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // Actor_UpdateMembers
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
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle);            // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattleEventRuntime_StartPhaseFar
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

extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02014208[];
extern u8 gData_02015000[];
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const u8 gData_08199340[];
extern const u8 gData_08199210[];
extern s32 gCameraSceneParameters[];

u8 *Unit_Get(s32 unit);
void Render_ProjectVertices(const u8 *vertices, void *projected, s32 count);
void SceneTransform_ApplyPosition(void *position);

// The water-column scene shared by Whorl (0), Gale (1), Lethe Albion (2),
// Poison Flow (3) and Raging Flood (4): a twisting funnel of water (or wind)
// rises around the target, with spray, bubbles and a burst.
void Func_08171bb0(struct BattleEffectArgument *effect, u32 mode) {
  struct BattleEffectWork *work;
  u8 *canvas;
  /* FAKEMATCH(frame): the blitter is declared as The Broken Seal's pair
   * though only the first is used; its second word is the unused word at
   * sp 76 of the native's 176-byte stack frame. As one DrawRectangle the frame
   * is 172 bytes and every stack offset below it moves (differ 1488). */
  DrawRectangle blit[2];
  s32 frame;
  s16 *camera;
  u8 *sprites;
  s32 frames;
  u8 *unit;
  s32 *target;
  struct EffectStep *p;
  struct RenderModel *model;
  struct ModelTexture texture;
  s32 phase;
  s32 shade;
  s32 i;
  s32 j;
  s32 point[3];
  s32 screen[3];
  s32 pos[3];
  struct EffectPosition proj;
  struct EffectPosition apos;
  struct EffectPosition spos;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  unit = Unit_Get(effect->actors[0]);
  BattleEffect_StartCanvasLayer(0);
  if (mode <= 1) {
    frames = 38;
    Func_0815585c(effect, (effect->side << 4) | 163, screen, point);
    REG_BG2CNT = 0x785;
  } else if (mode == 2 || mode == 4) {
    s32 *caster = *GetBattleObjectSlotFar(effect->actor);

    if (mode == 2) {
      s32 row;
      s32 size = 0x1000;

      Resource_LoadAndDecompress(0x13a, gData_02014000 + size, 1, 0);
      Iwram_ClearWords(gData_02014000, size);
      for (row = 0; row != 48; row++) {
        u8 *src = gData_02014000 + (row + row * 2) * 8 + 0x1000;
        u8 *left = Data_02014208 + row * 64;
        u8 *right = gData_02014000 + row * 64 + 0x237;
        s32 k;

        for (k = 0; k != 24; k++) {
          *left++ = *src;
          *right-- = *src++;
        }
      }
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x130), 128);
    } else {
      Resource_LoadAndDecompress(0xb4, gData_02014000, 0, 0);
    }
    pos[0] = caster[2];
    pos[1] = caster[3];
    pos[2] = caster[4];
    Resource_LoadAndDecompress(0x13e, gData_02015000, 1, 0);
    frames = 52;
  } else {
    REG_BG2CNT = 0x785;
    frames = 52;
  }
  if (!effect->side) {
    BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER, 23);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  target = *GetBattleObjectSlotFar(effect->actors[0]);
  work->particles[63].x = target[2];
  work->particles[63].y = target[3];
  work->particles[63].z = target[4];
  work->particles[63].velocityX = target[6];
  work->particles[63].velocityY = target[7];
  if (mode == 2 || mode == 4) {
    Resource_LoadAndDecompress(0xd2, work->sheet, 0, 0);
  } else {
    Resource_LoadAndDecompress(0xd3, work->sheet, 0, 0);
  }
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  Resource_LoadAndDecompress(0x153, work->sheet + 0x2000, 1, 1);
  if (mode == 3) {
    Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x150), 128);
  }
  if (mode == 4 || mode == 2) {
    Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x12f), 128);
  }
  Render_BuildQuadStrip(Data_02012000, (mode == 2 || mode == 4) ? 8 : 4, 32,
                        32);

  frame = 0;
  if (frames != 0) {
    phase = 0;
    do {
      if (frame == 0) {
        Sound_PlayFar(199);
      }
      if (frame == 32) {
        Sound_PlayFar(206);
      }
      if (frame == 0) {
        target[2] = work->particles[63].x;
        target[3] = work->particles[63].y;
        target[4] = work->particles[63].z;
        target[6] = work->particles[63].velocityX;
        target[7] = work->particles[63].velocityY;
        if (mode == 0) {
          p = work->particles;
          for (i = 0; i != 63; i++, p++) {
            p->x = effect->side == 1 ? 0xc80000 : -0xc80000;
            p->y = ((Random16() & 127) - 64) << 16;
            p->velocityX = p->x;
            p->z = ((Random16() & 127) - 64) << 16;
            p->velocityZ = p->z;
            p->velocityY = p->y;
          }
        } else if (mode == 1) {
          p = work->particles;
          for (i = 0; i != 63; i++, p++) {
            p->x = point[0];
            p->y = ((Random16() & 31) << 16) + 0x400000;
            p->z = 0;
            p->velocityX = ((Random16() & 63) + 64) << 13;
            if (p->x > 0) {
              p->velocityX = -p->velocityX;
            }
            p->velocityY = (Random16() | -128) << 11;
            p->velocityZ = 0;
          }
        }
      }
      Render_ResetTransformState();
      Transform_SetLookAt((s32 *)camera, (s32 *)camera + 3);
      if (mode <= 1) {
        s32 k = 4;

        pos[0] = point[0] + (Trig_Sin(frame << 11) << 4);
        if (frame > 39) {
          k = frame - 36;
        }
        pos[1] = point[1] + (k << 16) - k * Trig_Cos(frame * 3304);
        if (frame > 27) {
          pos[1] = pos[1] + (frame << 18) - 0x700000;
        }
        pos[2] = point[2];
        EffectPosition_ProjectPoint(pos, (struct EffectPosition *)screen);
        blit[0](canvas, work->sheet + 0x2000, screen[0] / 2 - 10,
                screen[1] - 20, 20, 40);
      }
      if (frame > 7) {
        if (mode <= 1 && frame <= 47) {
          work->shakeFrames = 3;
        }
        if (mode == 0) {
          if (frame == 32) {
            if (effect->variant == 1) {
              target[3] = 0x7c0000;
            } else {
              work->shakeFrames = 8;
              Func_081180e8(134);
              target[6] = work->particles[63].velocityX;
              target[7] = work->particles[63].velocityY;
              BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000, 0x80000,
                                        0x40000, 150);
            }
          } else if (frame <= 19) {
            target[18] = mode;
            target[2] += (pos[0] - work->particles[63].x) / 128;
            target[3] += (pos[1] - work->particles[63].y) / 128;
            target[4] += (pos[2] - work->particles[63].z) / 128;
          } else if (frame <= 31 && (effect->variant == 1 || frame <= 23)) {
            target[2] += (pos[0] - work->particles[63].x) / 16;
            target[3] += (pos[1] - work->particles[63].y) / 16;
            target[4] += (pos[2] - work->particles[63].z) / 16;
            target[6] += (0x4000 - work->particles[63].velocityX) / 16;
            target[7] += (0x4000 - work->particles[63].velocityY) / 16;
          }
        } else if (mode == 1) {
          target[18] = 0;
          if (frame > 55) {
            target[3] = 0x900000;
          } else if (frame <= 31) {
            if (target[2] > 0) {
              target[2] += 0x8000;
            } else {
              target[2] -= 0x8000;
            }
          } else {
            if (target[2] > 0) {
              target[2] += 0x100000;
            } else {
              target[2] -= 0x100000;
            }
          }
        }
      }
      if (frame > 0 && mode <= 1) {
        p = work->particles;
        for (i = 0; i != 16; i++, p++) {
          if (frame >= i && frame < i + 30) {
            s32 n = (i & 3) + 1;

            if (mode == 0) {
              p->x += (pos[0] - p->velocityX) / 32;
              p->y += (pos[1] - p->velocityY) / 32;
              p->z += (pos[2] - p->velocityZ) / 32;
            } else {
              BattleEffectKernels_IntegrateVector3(p, 62, 1024);
            }
            EffectPosition_ProjectPoint(p, &proj);
            blit[0](canvas, sprites + gData_08197410[n - 1], proj.x / 2 - n / 2,
                    proj.y - n, n, n * 2);
          }
        }
      }

      if (mode == 2 || mode == 4) {
        for (i = 0; i != 33; i++) {
          s32 r = i > 28 ? 28 : i;
          u8 *v = gMapCellBuffer + i * 8;
          s32 h = 0;

          for (j = 0; j != 2; j++, v += 4) {
            v[0] = 64 - i * 4;
            v[1] = ((r * Trig_Cos((i << 10) - (frame << 11))) >> 16) + h - 24;
            v[2] = 0;
            h += 48;
          }
        }
      } else if (mode == 3) {
        for (i = 0; i != 33; i++) {
          s32 r = i * 4 > 64 ? 64 : i * 4;
          s32 w = i * 2 + 4;
          u8 *v = gMapCellBuffer + i * 8;
          s32 h = 0;

          for (j = 0; j != 2; j++, v += 4) {
            v[0] = -3 * i;
            v[1] = ((r * Trig_Cos((i << 10) - (frame << 11))) >> 16) + h - w;
            h += w * 2;
            v[2] = 0;
          }
        }
      } else {
        for (i = 0; i != 33; i++) {
          s32 r = i * 3 > 96 ? 96 : i * 3;
          s32 w = i * 2 > 40 ? 40 : i * 2;
          u8 *v = gMapCellBuffer + i * 8;
          s32 h = 0;

          for (j = 0; j != 2; j++, v += 4) {
            v[0] = -4 * i;
            v[1] = ((r * Trig_Cos((frame + i * 2) << 10)) >> 16) + h - w;
            h += w * 2;
            v[2] = 0;
          }
        }
      }

      model = Render_AllocateModelList(1);
      if (mode == 4) {
        if (frame <= 45) {
          shade = frame * 4 - 64;
        } else {
          shade = frame * 8 - 376;
        }
        if (shade > -8) {
          shade = -8;
        }
      } else {
        if (frame <= 45) {
          shade = frame * 4 - 64;
        } else {
          shade = 348 - frame * 8;
        }
        if (shade > -20) {
          shade = -20;
        }
      }
      if (shade < -64) {
        shade = -64;
      }
      if (mode == 2 || mode == 4) {
        texture.logWidth = 5;
      } else {
        texture.logWidth = 7;
      }
      texture.logHeight = 5;
      texture.pixels = work->sheet;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = Data_02012000;
      model->vertices = Data_02011000;
      model->shade = shade;
      if (mode == 2 || mode == 4) {
        EffectPosition_ProjectBattler(effect->actors[0], &apos);
        if (frame == 16) {
          Func_081180e8(144);
          BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0x70000, 100);
          work->shakeFrames = 8;
          Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        }
        if (frame == 0) {
          p = work->particles;
          for (i = 0; i != 63; i++, p++) {
            s32 r = (Random16() & 255) + 256;
            s32 a = (Random16() & 0x7fff) + 0x4000;

            p->velocityX = (r * Trig_Sin(a)) >> 7;
            p->velocityY = (r * Trig_Cos(a)) >> 7;
            p->x = (apos.x / 2) << 16;
            p->variant = 0;
            p->y = (apos.y - 8) << 16;
          }
        }
        if (frame > 15) {
          p = work->particles;
          for (i = 0; i != 8; i++, p++) {
            if (p->variant <= 23) {
              blit[0](canvas, gData_02015000 + ((p->variant / 4) << 11),
                      HI(p->x) - 16, HI(p->y) - 32, 32, 64);
              BattleEffectKernels_IntegrateVector2(p, 60, -0x2000);
              p->variant++;
            }
          }
        }
        model->scrollU = ~(frame << 3) & 127;
        model->scrollV = 0;
        for (i = 0; i != 3; i++) {
          s32 y = 64 - ((Trig_Sin(phase * 2) << 6) >> 16);

          Transform_Push();
          Func_08015160(0, ((y - i * 8) << 16) + 0x300000, 0);
          if (effect->side == 1) {
            SceneTransform_ApplyYaw(0x8000);
          }
          Func_080150e4((i << 10) + 256);
          SceneTransform_ApplyPitch((i << 13) - (frame << 11));
          Func_080151e4(0x21770, 0x10000, 0x10000);
          Render_TransformVertices(gMapCellBuffer, Data_02011000, 66);
          Render_DrawModelList(model);
          Transform_Pop();
        }
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->mesh = gData_08199340;
        model->vertices = Data_02011000;
        model->shade = shade;
        model->scrollU = 0;
        model->scrollV = 0;
        texture.pixels = gData_02014000;
        for (i = 0; i != 2; i++) {
          s32 t = frame - i * 4 - 16;

          if (t >= 0) {
            s32 s = (t * 3) << 12;
            s32 sh = 0;

            if (t > 7) {
              sh = (8 - t) * 8;
            }
            model->shade = sh;
            if (sh > -64) {
              Render_ResetTransformState();
              Func_08015160((apos.x / 2 - 64) << 16, (apos.y - 64) << 16, 0);
              Func_080151e4(0x8000, 0x10000, 0x8000);
              Func_080150e4((i << 14) + 0x2000);
              SceneTransform_ApplyPitch(-0x1000);
              SceneTransform_ApplyYaw(frame << 12);
              Func_0801521c(s);
              Render_TransformVertices(gData_08199210, Data_02011000, 4);
              Render_DrawModelList(model);
            }
          }
        }
      } else if (mode == 3) {
        EffectPosition_ProjectScaledBattler(effect->actor, &spos);
        if (frame == 8) {
          Sound_PlayFar(144);
        }
        if (frame == 16) {
          Func_081180e8(134);
        }
        for (i = 0; i != effect->count; i++) {
          if (frame == i * 4 + 16) {
            Sound_PlayFar(134);
            BattleEffect_StartActorMotion(effect->actors[i], 1, 0, 0, 0x70000, 100);
            Func_0814cd48(effect->actors[i], 7, 5, i, 8);
            work->shakeFrames = 8;
          }
        }
        model->scrollV = 0;
        for (i = 0; i != 1; i++) {
          model->scrollU = ~(frame * 8 + i * 48) & 127;
          Render_ResetTransformState();
          Func_08015160((spos.x / 2 - 64) << 16, (spos.y << 16) - 0x340000, 0);
          if (effect->side == 1) {
            SceneTransform_ApplyYaw(0x8000);
          }
          SceneTransform_ApplyPitch(i << 15);
          Func_080150e4((Trig_Sin((frame << 11) + i * 0x4000) << 11) >> 16);
          Func_080151e4(0x14e20, 0x8000, 0x10000);
          Render_TransformVertices(gMapCellBuffer, Data_02011000, 66);
          Render_DrawModelList(model);
        }
      } else {
        if (frame <= 29) {
          shade = frame * 4 - 64;
        } else {
          shade = 220 - frame * 8;
        }
        if (shade > -20) {
          shade = -20;
        }
        if (shade > -64) {
          model->shade = shade;
          Func_080151e4(0x8000, 0x10000, 0x10000);
          gCameraSceneParameters[3] = 60;
          if (mode == 0) {
            model->scrollU = (frame << 3) & 127;
          } else {
            model->scrollU = ~(frame << 3) & 127;
          }
          model->scrollV = 0;
          SceneTransform_ApplyPosition(pos);
          Func_08015160(0, 0x100000, 0);
          for (i = 0; i != 3; i++) {
            Transform_Push();
            if (effect->side == 1) {
              SceneTransform_ApplyYaw(0x8000);
            }
            Func_080150e4((i << 10) + 3096);
            if (mode != 1) {
              SceneTransform_ApplyPitch((frame << 11) + i * 0x2000);
            } else {
              SceneTransform_ApplyPitch((i << 13) - (frame << 11));
            }
            Func_080151e4(0x40000, 0x10000, 0x10000);
            Render_ProjectVertices(gMapCellBuffer, Data_02011000, 66);
            Render_DrawModelList(model);
            Transform_Pop();
          }
        }
        gCameraSceneParameters[3] = 120;
      }
      Heap_Release(model);
      if (mode == 2 || mode == 4) {
        Func_08158ce0(8, 8);
      } else {
        Func_08158ce0(4, 4);
      }
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      WaitFrames(1);
      phase += 315;
      frame++;
    } while (frame != frames);
  }

  if (unit[0x129] && mode == 0 && effect->variant == 1) {
    target[2] = work->particles[63].x;
    target[3] = work->particles[63].y;
    target[4] = work->particles[63].z;
    target[6] = work->particles[63].velocityX;
    target[7] = work->particles[63].velocityY;
  }
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
