// Draft of BattleEffect_RunMegiddo, decompiled from the listing: differ 1240 of
// 1361 (draft 1361), the same 110 calls and control flow; allocation differs.

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
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void ObjectGroup_CountDownMemberTimers(void); // ObjectGroup_CountDownMemberTimers
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // ObjectGroup_UpdateMembers
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
void Func_081180e8(s32 phase);             // BattleEventRuntime_StartPhaseFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
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

#include "bg_scroll.h"
#include "input.h"
#include "io_write_queue.h"

extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const u8 gData_08199364[];
extern const u8 Data_081991e0[];
extern const u8 gData_081991f0[];
extern const u8 Data_081992b0[];
extern const u16 Data_08199ee0[]; // the five rings' start frames
extern const s16 Data_08199ecc[];
extern const s16 Data_08199ed6[];
extern const s16 Data_08199eea[];
extern const s16 Data_08199efa[];
extern const s16 Data_08199f0a[];
extern const s8 Data_08199f1a[];
extern const s8 Data_08199f22[];
extern s32 gCameraSceneParameters[];

void BattleMotion_ApproachTargetFar(s32 actor, s16 target, s32 frames,
                                    s32 speed);
void ObjectDispatch_SetAnimationSpeedFar(void *object, s32 value);
void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleActor_CommitPlacementFar(void);
void BattleEffect_BuildRepeatingTilemap(void);
void Func_08164abc(s32 red, s32 green, s32 blue);
void Func_08164a4c(s32 blue, s32 green, s32 red);
void Func_0815f0a0(s32 palette);
void Graphics_LoadGreyRamp(void);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void ResourceObject_ReleaseFar(void *object);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);

// Megiddo: the caster rises in a column of light, a winged figure gathers
// above and dives in a storm of feathers and rings onto the target.
void BattleEffect_RunMegiddo(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  u8 *canvas;
  u8 *sprites;
  s32 *transition;
  s16 *camera;
  s16 savedYaw;
  u8 *meshA;
  u8 *meshB;
  DrawRectangle blit;
  s32 *caster;
  s32 casterValue;
  s32 scale;
  s32 x;
  s32 z;
  s32 frame;
  s32 i;
  s32 j;
  struct EffectPosition pos;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct RenderModel *model;
  void *verts;
  struct EffectStep *p;
  s32 req[2];
  s32 place[4];
  s32 screen[2];
  s16 list[2];
  s16 list2[2];

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  transition = Ram_HeapSlots->transitionWork;
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = 0;
  BattleMotion_ApproachTargetFar(effect->actor, effect->actor, 150, 0x50000);
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  meshA = work->sheet + 0x5f40;
  meshB = work->sheet + 0x6340;
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  caster = *GetBattleObjectSlotFar(effect->actor);
  scale = 0x10000;
  x = 0;
  z = 0;
  Render_BuildQuadStrip(Data_02012000, 8, 16, 32);
  Render_BuildQuadStrip(meshB, 2, 32, 15);
  for (i = 0; i != 16; i++) {
    s32 angle = i * 1092;
    s32 y = ((-Trig_Cos(angle) * 48) >> 16) + 24;
    s32 r = (Trig_Sin(angle) * 48) >> 16;
    u8 *v = meshA + i * 8;

    for (j = 0; j != 2; j++, v += 4) {
      v[0] = (r * Trig_Sin(j << 13)) >> 16;
      v[1] = y;
      v[2] = (r * Trig_Cos(j << 13)) >> 16;
    }
  }
  casterValue = caster[18];
  ObjectDispatch_SetAnimationSpeedFar(caster, 0);
  caster[9] = 0;
  caster[10] = 0;
  caster[11] = 0;
  caster[13] = 0;
  caster[18] = 0;

  for (frame = 0; frame != 188; frame++) {
    if (frame == 4) {
      Audio_PlayCue(212);
    }
    if (frame == 28) {
      Audio_PlayCue(140);
    }
    if (frame == 84) {
      Audio_PlayCue(104);
    }
    if (frame == 143) {
      Audio_PlayCue(191);
    }
    if (frame == 171) {
      Audio_PlayCue(145);
    }
    if ((gInput.repeat & 3) && frame > 3 && frame <= 142) {
      frame = 143;
    }
    if (frame == 0) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      Resource_LoadAndDecompress(0xc2, gData_02014000, 0, 0);
      Resource_LoadAndDecompress(0xe9, work->sheet, 0, 0);
      Resource_LoadAndDecompress(0xc9, work->sheet + 0x800, 0, 0);
      Resource_LoadAndDecompress(0x103, work->sheet + 0x1000, 1, 1);
      Resource_LoadAndDecompress(0xcd, work->sheet + 0x2400, 1, 0);
      Graphics_LoadGreyRamp();
      REG_BLDALPHA = 0x1010;
      x = 0xa00000;
      if (effect->side) {
        x = 0x500000;
      }
      z = -0x600000;
      IO_WRITE_QUEUE_PUSH(0x0400000c, 0x784, 0x20000);
    }
    if (frame == 1) {
      transition[4] = 1;
      list[0] = effect->actor;
      list[1] = 255;
      BattleActor_SpawnObjectsForListFar(list, 0);
      BattleBackground_LoadFar(1, 0x72, 0);
      BattleEffect_BuildRepeatingTilemap();
      Func_08164abc(8, 2, 2);
      BattleEffect_SpawnObjects(1, 0x2a9, 2);
    }
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    if (frame == 3) {
      savedYaw = camera[27];
    }
    if ((u32)(frame - 16) <= 61) {
      if (!effect->side) {
        camera[27] -= 256;
      } else {
        camera[27] += 256;
      }
    }

    for (i = 0; i != 33; i++) {
      u8 *v = gMapCellBuffer + i * 8;
      s32 angle = i << 11;

      for (j = 0; j != 2; j++, v += 4) {
        s32 r = 80;

        if (j != 1) {
          r = ((Trig_Sin((frame + i * 4) << 12) * 2) >> 16) + 120;
        }
        v[0] = (r * Trig_Sin(angle)) >> 16;
        v[1] = (r * Trig_Cos(angle)) >> 16;
        v[2] = 0;
      }
    }

    {
      s32 y = gBgScroll[1].y;
      s32 d;

      if (y > 104) {
        y -= 0x10000;
      }
      if (frame <= 63) {
        d = -16;
      } else if ((u32)(frame - 64) <= 7) {
        d = frame * 2 - 144;
      } else if ((u32)(frame - 72) <= 11) {
        d = 0;
      } else if (frame > 154) {
        d = 16 - (frame - 155) / 2;
        if (d < 0) {
          d = 0;
        }
      } else {
        d = frame - 84;
        if (d > 16) {
          d = 16;
        }
        gCameraSceneParameters[4] -= d;
      }
      y += d;
      if (y < -16) {
        y += 120;
      }
      if (y > 104) {
        y -= 120;
      }
      gBgScroll[1].y = y;
    }
    if ((u32)(frame - 42) <= 37) {
      z += 0x20000;
      Func_0815f0a0(0x148);
    }
    if ((u32)(frame - 84) <= 58) {
      if (!effect->side) {
        x -= 0x20000;
      } else {
        x += 0x20000;
      }
      z += 0x40000;
    }
    if (frame == 143) {
      s32 *target;

      Func_08164abc(-12, -2, -2);
      camera[27] = savedYaw;
      x = 0x780000;
      z = 0x300000;
      list2[0] = effect->actors[0];
      list2[1] = 255;
      BattleActor_SpawnObjectsForListFar(list2, 0);
      gCameraSceneParameters[4] = 556;
      target = *GetBattleObjectSlotFar(effect->actors[0]);
      target[2] = 0;
      target[4] = 0;
      work->transferMode = 2;
      work->transferValue = 50;
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x148), 128);
    }
    if (frame > 154) {
      s32 d = 16 - (frame - 155) / 2;

      if (d < 0) {
        d = 0;
      }
      gCameraSceneParameters[4] -= d;
    }
    if (frame == 84) {
      IO_WRITE_QUEUE_PUSH(0x0400000c, 0x785, 0x20000);
      ObjectDispatch_SetAnimationSpeedFar(caster, 128);
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      work->transferMode = 3;
      work->transferValue = 0x06060606;
    }
    if (frame == 171) {
      work->shakeFrames = 8;
      Audio_PlayCue(145);
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      Func_08118088(effect->actors[0], 0);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
      work->transferMode = 1;
      work->transferValue = 0x10101010;
    }
    if (frame > 174) {
      Func_08164a4c(2, 2, 2);
    }

    if (frame > 1) {
      u8 *obj;

      if (frame > 142) {
        scale = (frame - 143) * 1236 + 0x4000;
        if (scale > 0xc000) {
          scale = 0xc000;
        }
      }
      req[1] = req[0] = scale * 2;
      place[3] = 0;
      place[1] = 0xff0000;
      place[0] = x;
      place[2] = z;
      obj = work->objects[0];
      obj[23] = 32;
      obj[22] = 0;
      if ((u32)(place[2] + 0x340000) <= 0xfbffff) {
        Render_ApplyProjectedPlacementFar(obj, place, req, 0);
      }
    }
    if (frame > 1) {
      model = Render_AllocateModelList(1);
      model->kind = 6;
      texture.logWidth = 6;
      texture.logHeight = 4;
      model->vertices = Data_02011000;
      model->texture = &texture;
      model->shade = 0;
      model->scrollV = 0;
      texture.pixels = work->sheet + 0x1000 + ((frame / 2) % 5) * 0x400;
      model->mesh = Data_02012000;
      Render_ResetTransformState();
      Func_080151e4(0x8000, 0x10000, 0x8000);
      Func_08015160(x - 0x7e0000, z - 0x4c0000, 0);
      Func_0801521c(46536);
      Func_0801521c(scale);
      Render_TransformVertices(gMapCellBuffer, Data_02011000, 66);
      Render_DrawModelList(model);

      texture.logWidth = 5;
      texture.logHeight = 5;
      texture.pixels = work->sheet + 0x2400;
      model->kind = 7;
      model->mesh = meshB;
      model->unk04 = 3;
      model->shade = -24;
      model->scrollV = 0;
      if (frame > 142) {
        s32 n = frame - 143;
        s32 scroll = frame * 3;
        s32 angle = -0x4000;

        if (n > 32) {
          n = 32;
        }
        for (i = 0; i != 10; i++) {
          model->scrollU = ~scroll & 127;
          Render_ResetTransformState();
          Func_08015160(-0x20000, -0x200000, 0);
          Func_080151e4(82000, 0x35208, 0x10000);
          Func_0801521c(scale);
          Func_080150e4(0x8000);
          scroll += 20;
          SceneTransform_ApplyPitch(n * 384 - 0x3000);
          SceneTransform_ApplyYaw(angle);
          Render_TransformVertices(meshA, Data_02011000, 32);
          Render_DrawModelList(model);
          angle += 0x1800;
        }
      }
      MemFree(model);
    }

    if (frame == 8) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      p = work->particles;
      for (i = 0; i != 64; i++, p++) {
        p->x = Random16() & 127;
        p->y = Random16() & 127;
      }
    }
    if ((u32)(frame - 8) <= 79) {
      s32 off = (frame * 2 - 16) & 127;

      p = work->particles;
      for (i = 0; i != 64; i++, p++) {
        if (p->variant >= 0) {
          s32 y = p->y - off;

          if (y > 127) {
            y -= 128;
          }
          if (y < 0) {
            y += 128;
          }
          blit(canvas, sprites + gData_08197410[0], p->x, y, 1, 2);
        }
      }
    }
    if (frame > 142) {
      s32 n = frame - 143;

      if (n > 32) {
        n = 32;
      }
      if (frame == 143) {
        p = work->particles;
        for (i = 0; i != 64; i++, p++) {
          p->x = Random16() & 127;
          p->y = Random16() % 192 - 64;
          p->z = Random16() & 127;
        }
      }
      Render_ResetTransformState();
      SceneTransform_ApplyPitch(n * 384 - 0x3000);
      p = work->particles;
      for (i = 0; i != 64; i++, p++) {
        Render_ProjectPoint(p, screen);
        blit(canvas, sprites + gData_08197410[0], screen[0], screen[1], 1, 4);
        p->y -= 2;
      }
    }

    verts = Runtime_BumpAllocateAlternatePool(32);
    model = Render_AllocateModelList(1);
    texture2.logWidth = 7;
    texture2.logHeight = 7;
    texture2.pixels = gData_02014000;
    model->kind = 7;
    model->mesh = gData_08199364;
    model->texture = &texture2;
    model->vertices = verts;
    for (i = 0; i != 5; i++) {
      s32 start = Data_08199ee0[i];

      if (frame >= start && frame < start + 20) {
        s32 t = frame - start;
        s32 shade = 64 - t * 8;
        s32 s;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > -64) {
          model->shade = shade;
          s = (t << 14) + 0x4000;
          Render_ResetTransformState();
          if (i <= 1) {
            Func_08015160((pos.x / 2 - 64) << 16, (pos.y - 48) << 16, 0);
          } else {
            Func_08015160(0, 0x100000, 0);
          }
          Func_080151e4(s, s * 2, s);
          SceneTransform_ApplyPitch(Data_08199ecc[i]);
          SceneTransform_ApplyYaw(Data_08199ed6[i]);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    model->kind = 7;
    texture2.logWidth = 5;
    texture2.logHeight = 6;
    texture2.pixels = work->sheet;
    model->mesh = Data_081992b0;
    for (i = 0; i != 8; i++) {
      s32 start = 16 + i * 2;

      if (frame >= start && frame < i * 2 + 80) {
        s32 t = frame - start;
        s32 shade = t * 8 - 64;
        s32 s = t << 11;

        if (shade > 0) {
          shade = 0;
        }
        if (s > 0x10000) {
          s = 0x10000;
        }
        if (t > 47) {
          s = s - (t << 12) + 0x30000;
        }
        model->shade = shade;
        Render_ResetTransformState();
        if (!effect->side) {
          Func_08015160((pos.x / 2 + Data_08199f1a[i] - 64) << 16,
                        (Data_08199f22[i] + pos.y - 54) << 16, 0);
        } else {
          Func_08015160((pos.x / 2 - Data_08199f1a[i] - 62) << 16,
                        (Data_08199f22[i] + pos.y - 54) << 16, 0);
        }
        Func_080151e4(0x8000, 0x10000, 0x8000);
        if (!effect->side) {
          Func_080150e4(Data_08199f0a[i]);
        } else {
          Func_080150e4(-Data_08199f0a[i]);
        }
        SceneTransform_ApplyPitch(Data_08199eea[i]);
        SceneTransform_ApplyYaw(Data_08199efa[i]);
        Func_080151e4(s / 4, s * 4, s / 2);
        Render_TransformVertices(gData_081991f0, verts, 4);
        Render_DrawModelList(model);
      }
    }
    MemFree(model);
    MemFree(verts);

    while (*(volatile s32 *)&work->framesSinceTransfer <= 1) {
    }
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  ResourceObject_ReleaseFar(work->objects[0]);
  caster[18] = casterValue;
  BattleActor_CommitPlacementFar();
  transition[4] = 0;
  BattleBackground_LoadFar(1, *(u16 *)((u8 *)Ram_HeapSlots->battleWork + 0x670),
                           24);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
