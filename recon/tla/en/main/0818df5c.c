// Draft of BattleEffect_RunMeteorBlow, decompiled from the listing: differ 1767 of
// 1556 (draft 1577), the same 113 calls and control flow; allocation differs.

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
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
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

// One star of the opening sky: a point on a sphere and its cell kind.
struct MeteorStar {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
};

extern u8 gMapCellBuffer[];
extern u8 Data_02010018[];         // the shards' variants, 28 bytes apart
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const s32 Data_08196f20[2];
extern const u8 Data_08199e70[];
extern const u8 Data_08199e77[];
extern const u16 Data_08199e7e[];
extern const u8 Data_08199e8c[];
extern const u8 Data_08199e90[];
extern const u8 Data_08199e94[];
extern const u8 Data_08199e98[];
extern const u8 Data_08199e9c[];
extern const s8 Data_08199ea0[];
extern const u8 gData_081990d0[];
extern const u8 gData_08199090[];
extern const u8 Data_08198ec4[];
extern const u8 Data_08198cac[];
extern s32 gCameraSceneParameters[];

void BattleEffect_FlushPendingGraphicsTransfer(void);
void BattleEffect_ArmMeteorBlowPaletteDma(void);
void Func_0813ba50(void);
void Func_08163c2c(s32 mode, s32 value);
void Graphics_LoadGreyRamp(void);
void Func_0814cca8(void);
void Func_0815f0a0(s32 palette);
void Func_08164abc(s32 red, s32 green, s32 blue);
void Func_0818de3c(void *triangles, s32 columns, s32 rows, s32 cellW,
                   s32 cellH);
void Func_08196404(s32 slot, s32 a, s32 b, s32 flags, s32 mode);
void SceneTransform_ApplyPosition(void *position);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void ResourceObject_ReleaseFar(void *object);

// Meteor Blow: the caster soars through a field of stars and comes down as a
// burning meteor that shatters on the targets.
void BattleEffect_RunMeteorBlow(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  s32 *transition;
  u8 *triangles;
  u8 *projected;
  u8 *meshA;
  struct MeteorStar *stars;
  DrawRectangle blit;
  struct RenderModel *model;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  void *verts;
  struct EffectStep *p;
  s32 size;
  s32 pitch;
  s32 yaw;
  s32 x;
  s32 z;
  s32 scale;
  s32 hx;
  s32 hz;
  s32 frame;
  s32 i;
  s32 j;
  s32 req[2];
  s32 place[4];
  s32 req2[2];
  s32 place2[4];
  s32 point[3];
  s32 screen[3];
  s32 position[3];

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  Func_0813ba50();
  ((volatile u16 *)BG_PLTT)[0] = 0;
  ((volatile u16 *)BG_PLTT)[1] = 0;
  work->transferMode = 0;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  Func_08163c2c(0, 0);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Resource_LoadAndDecompress(0xaf, work->sheet, 1, 1);
  Resource_LoadAndDecompress(0xcd, work->sheet + 0x2000, 1, 1);
  Iwram_CopyWords(sprites, work->sheet + 0x1c2, 30);
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x148), 128);
  Graphics_LoadGreyRamp();
  gCameraSceneParameters[4] = 240;
  WaitFrames(1);
  Iwram_FillWords(gMapCellBuffer, 0x8000, 0x01010101);
  Iwram_CopyWords((void *)0x06008000, gMapCellBuffer, 0x7800);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_SpawnObjects(1, 0x28a, 1);
  REG_BLDCNT = 0;
  REG_BG2PA = 0x100;
  REG_BG1CNT = 0x1f81;
  REG_BG2CNT = 0x2787;
  Iwram_FillWords(canvas, 0x4000, 0);
  Iwram_CopyWords((void *)0x06004000, canvas, 0x4000);
  Scheduler_AddOrUpdateCallback(BattleEffect_FlushPendingGraphicsTransfer, 3200);
  REG_BLDCNT = 0x3f46;
  REG_BLDALPHA = 0x1010;
  triangles = work->sheet + 0x6340;
  projected = work->sheet + 0x5740;
  meshA = work->sheet + 0x5f40;
  transition = Ram_HeapSlots->transitionWork;
  stars = (struct MeteorStar *)(work->sheet + 0x7a4);
  scale = 0x10000;
  pitch = 0;
  yaw = 0;
  x = 0;
  z = 0;
  size = 0;
  Func_0818de3c(triangles, 2, 16, 32, 2);
  for (i = 0; i != 16; i++) {
    s32 angle = i * 1092;
    s32 y = (-Trig_Cos(angle) * 48) >> 16;
    s32 r = (Trig_Sin(angle) * 48) >> 16;
    u8 *v = meshA + i * 8;

    for (j = 0; j != 2; j++, v += 4) {
      v[0] = (r * Trig_Sin(j << 13)) >> 16;
      v[1] = y;
      v[2] = (r * Trig_Cos(j << 13)) >> 16;
    }
  }
  Func_08196404(SLOT_BLITTER, 8, 7, 3, 1);
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ArmMeteorBlowPaletteDma, 0xcfe);
  gBgScroll[1].x = 0;
  transition[4] = 1;
  for (i = 0; i != 256; i++) {
    struct MeteorStar *s = &stars[i];
    s32 a = (i & 255) << 7;
    s32 r;
    s32 b;

    s->y = Trig_Sin(a) * 160;
    r = (Trig_Cos(a) * 160) >> 16;
    b = Random16() & 0x7fff;
    s->x = r * Trig_Sin(b);
    s->z = r * Trig_Cos(b);
    if ((i & 31) == 0) {
      s->kind = (i / 32) % 7;
    } else {
      s->kind = 7;
    }
  }

  // The flight through the stars.
  for (frame = 0; frame != 160 && !(gInput.repeat & 1) && !(gInput.repeat & 2);
       frame++) {
    if (frame == 0) {
      x = 0x1100000;
      work->frame = 8;
      z = 0x580000;
      size = 0x400;
      pitch = 0xd00;
      yaw = 0x2500;
      Audio_PlayCue(162);
    }
    if (frame == 120) {
      Audio_PlayCue(163);
    }
    if ((u32)(frame - 96) <= 16) {
      REG_BLDALPHA = (16 - (frame - 96)) | 0x1000;
    }
    if (frame == 115) {
      REG_BLDALPHA = 0x1010;
    }
    if (frame == 116) {
      Scheduler_RemoveCallback(BattleEffect_FlushPendingGraphicsTransfer);
      Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
      IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
      IO_WRITE_QUEUE_PUSH(0x0400000c, 0x784, 0x20000);
    }
    if (frame > 128) {
      Func_0815f0a0(0x148);
    }
    if (frame == 117) {
      work->frame = 45;
      p = work->particles;
      for (i = 0; i != 64; i++, p++) {
        p->variant = -1;
      }
      scale = 0x10000;
    }
    if (frame <= 1599) {
      s32 base = work->frame / 2 + 16;
      u16 *dst = (u16 *)((u8 *)work + 0x7640);

      if (frame > 79) {
        work->frame++;
        if (work->frame > 236) {
          work->frame = 236;
        }
      }
      for (i = 0; i != 15; i++) {
        *dst++ = 0;
      }
      for (; i != 135; i++) {
        s32 v = (i - 16) / 4 + base;
        s32 b = v - 32;
        s32 g = v - 80;

        if (b < 0)
          b = 0;
        if (b > 31)
          b = 31;
        if (g < 0)
          g = 0;
        if (g > 31)
          g = 31;
        *dst++ = b << 10 | g << 5 | g >> 1;
      }
      for (; i != 160; i++) {
        *dst++ = 0;
      }
    }

    if (frame > 115) {
      s32 shade;
      s32 scroll;
      s32 angle;

      model = Render_AllocateModelList(1);
      shade = frame * 4 - 528;
      if (shade > 0) {
        shade = 0;
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      texture.pixels = work->sheet + 0x2000;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = triangles;
      model->shade = shade;
      model->vertices = projected;
      model->scrollU = 0;
      if (frame > 127) {
        s32 d = (frame << 6) - 0x2000;

        if (d > 1023) {
          d = 1024;
        }
        scale += d;
        Func_080151e4(scale, scale, scale);
      }
      angle = -0x4000;
      scroll = frame * 2;
      for (i = 0; i != 5; i++) {
        model->scrollV = ~scroll & 127;
        Render_ResetTransformState();
        Func_08015160(-0x20000, -0x400000, 0);
        Func_080151e4(42000, 0x24650, 0x10000);
        Func_080151e4(scale, scale, scale);
        Func_080150e4(0x8000);
        SceneTransform_ApplyYaw(angle);
        Render_TransformVertices(meshA, projected, 32);
        Render_DrawModelList(model);
        angle += 0x1800;
        scroll += 20;
      }
      MemFree(model);
    } else {
      if (frame <= 159) {
        pitch = (s16)(pitch + 256);
      } else if (frame <= 191) {
        pitch = (s16)((u16)pitch - frame * 8 + 1216);
      }
      if (frame <= 47) {
        yaw = (s16)(yaw + 256);
      } else if (frame <= 111) {
        yaw = (s16)((u16)yaw - frame * 4 + 448);
      }
      position[0] = 120;
      position[1] = 64;
      position[2] = 0;
      Render_ResetTransformState();
      SceneTransform_ApplyPosition(position);
      SceneTransform_ApplyPitch((u16)pitch);
      SceneTransform_ApplyYaw((u16)yaw);
      if (frame <= 112) {
        struct MeteorStar *s = stars;

        for (i = 0; i != 256; i++, s++) {
          s32 kind = s->kind;

          point[0] = HI(s->x);
          point[1] = HI(s->y);
          point[2] = HI(s->z);
          Render_ProjectPoint(point, screen);
          if (screen[2] > 0) {
            if (kind == 7) {
              blit(gMapCellBuffer, sprites + i % 30, screen[0], screen[1], 1,
                   1);
            } else {
              blit(gMapCellBuffer, work->sheet + Data_08199e7e[kind], screen[0],
                   screen[1], Data_08199e70[kind], Data_08199e77[kind]);
            }
          }
        }
      } else {
        Iwram_ClearWords(gMapCellBuffer, 0x7800);
      }
    }

    req[1] = size;
    req[0] = size;
    place[3] = 0;
    place[1] = 0xff0000;
    place[0] = x;
    place[2] = z;
    Render_ApplyProjectedPlacementFar(work->objects[0], place, req, 0);
    {
      s32 a = 0x6000;
      s32 speed = 0x800;

      if (frame > 23) {
        a = 0x6000 - frame * 286 + 0x11e0;
        if (a < 0) {
          a = 0;
        }
      }
      if (frame > 87) {
        speed = 0x800 - (frame << 6) + 0x1600;
        if (speed < 0) {
          speed = 0;
        }
      }
      x -= (speed * Trig_Sin(a)) >> 10;
      z += (speed * Trig_Cos(a)) >> 10;
      size += 512;
    }
    if (frame > 127) {
      s32 d = (frame << 13) - 0x100000;

      if (d > 0x1ffff) {
        d = 0x20000;
      }
      z += d;
      size += 512;
    } else if (size > 0x10000) {
      size = 0x10000;
    }
    work->transferPending = 1;
    WaitFrames(1);
  }

  WaitFrames(1);
  transition[4] = 0;
  Scheduler_RemoveCallback(BattleEffect_ArmMeteorBlowPaletteDma);
  if (frame <= 115) {
    Scheduler_RemoveCallback(BattleEffect_FlushPendingGraphicsTransfer);
  } else {
    Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  }
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  Func_0814cca8();
  hx = 200;
  hz = 0;
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  REG_BG2Y = -0x1000;
  REG_BLDALPHA = 0x1010;
  REG_BG2CNT = 0x784;
  REG_WININ = 0x2737;
  REG_WINOUT = 0x721;
  REG_WIN0V = 0x107c;
  REG_WIN1V = 0x88;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  Resource_LoadAndDecompress(0x13e, work->sheet, 1, 0);
  Resource_LoadAndDecompress(0xda, work->sheet + 0x4000, 0, 0);
  Resource_LoadAndDecompress(0xc1, work->sheet + 0x5000, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x148), 128);
  {
    s32 *q = (s32 *)Data_02010018;

    for (i = 0; i != 256; i++, q += 7) {
      *q = 0;
    }
  }
  Func_08164abc(5, -4, -12);

  // The impact: the meteor lands, rings of fire and shards fly.
  for (frame = 0; frame != 50; frame++) {
    s32 x0;
    s32 angle;

    if (frame == 0) {
      hx = 144;
      hz = 0;
    }
    if (frame <= 33) {
      req2[0] = Data_08196f20[0];
      req2[1] = Data_08196f20[1];
      place2[3] = 0;
      place2[1] = 0xff0000;
      place2[0] = hx << 16;
      place2[2] = hz << 16;
      if (frame <= 15) {
        hx -= 1;
        hz += 9;
      } else {
        hz += 2;
      }
      Render_ApplyProjectedPlacementFar(work->objects[0], place2, req2, 0);
    }
    if (frame == 34) {
      Func_081180e8(134);
    }
    if (frame == 16) {
      s32 half = (hx / 2) << 16;

      p = (struct EffectStep *)gMapCellBuffer;
      for (i = 0; i != 256; i++, p++) {
        s32 r = (Random16() & 511) + 256;
        s32 a = (Random16() & 0x7fff) + 0x4000;

        p->x = half;
        p->y = 0x720000;
        p->velocityX = (r * Trig_Sin(a)) >> 6;
        p->velocityY = (r * Trig_Cos(a) * 2) >> 6;
        p->variant = (Random16() & 7) + 24;
      }
      work->shakeFrames = 4;
      Audio_PlayCue(145);
      for (i = 0; i != effect->count; i++) {
        Func_08118088(effect->actors[i], 4);
        Func_0814cd48(effect->actors[i], 7, 5, i, 8);
      }
    }

    verts = Runtime_BumpAllocateAlternatePool(1024);
    model = Render_AllocateModelList(1);
    model->texture = &texture2;
    model->vertices = verts;
    model->shade = 0;
    texture2.logWidth = 6;
    texture2.logHeight = 6;
    texture2.pixels = work->sheet + 0x4000;
    model->mesh = gData_081990d0;
    model->kind = 7;
    x0 = hx / 2 - 60;
    angle = frame << 11;
    for (i = 0; i != 4; i++, angle += 0x2000) {
      s32 start = Data_08199e8c[i] + 16;

      if (frame > start) {
        s32 shade = (start - frame) * 8 + 56;

        if (shade > -16) {
          shade = -16;
        }
        if (shade > -64) {
          s32 s = (frame - start) * Data_08199e90[i] * 2000 + 42768;

          model->shade = shade;
          Render_ResetTransformState();
          Func_08015160(x0 << 16, 0x280000, 0);
          Func_080151e4(s, Data_08199e94[i] * s, s / 4);
          SceneTransform_ApplyPitch(1000);
          if (i & 1) {
            SceneTransform_ApplyYaw(angle);
          } else {
            SceneTransform_ApplyYaw(-angle);
          }
          Render_TransformVertices(gData_08199090, verts, 32);
          Render_DrawModelList(model);
        }
      }
    }
    model->mesh = Data_08198ec4;
    texture2.pixels = work->sheet + 0x5000;
    texture2.logWidth = 7;
    texture2.logHeight = 7;
    x0 = hx / 2 - 64;
    for (i = 0; i != 3; i++) {
      s32 start = Data_08199e98[i] + 8;

      if (frame > start) {
        s32 shade = (start - frame) * 8 + 40;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > -64) {
          s32 s = (frame - start) * Data_08199e9c[i] * 1500 + 0x4180;

          model->shade = shade;
          model->scrollU = ((frame + i * 5) & 7) << 4;
          Render_ResetTransformState();
          Func_08015160(x0 << 16, Data_08199ea0[i] << 16, 0);
          Func_080151e4(s, s * 2, s);
          SceneTransform_ApplyPitch(2000);
          Render_TransformVertices(Data_08198cac, verts, 32);
          Render_DrawModelList(model);
        }
      }
    }
    MemFree(model);
    MemFree(verts);

    p = (struct EffectStep *)gMapCellBuffer;
    for (i = 0; i != 256; i++, p++) {
      if (p->variant >= 0) {
        s32 n = p->variant / 4 + 1;

        blit(canvas, sprites + gData_08197410[n - 1], HI(p->x) - n / 2,
             HI(p->y) - n, n, n * 2);
        p->variant--;
        BattleEffectKernels_IntegrateVector2(p, 56, 0x2000);
      }
    }
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  ResourceObject_ReleaseFar(work->objects[0]);
  BattleEffect_EndCanvasLayer();
}
