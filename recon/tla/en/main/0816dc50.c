// Draft of BattleFx_RunSerac, decompiled from the listing: differ 1130 of 1101
// (draft 1093), the same 91 calls and control flow; allocation differs.
// 2026-10-19: the native frame is 160 bytes (here 176 at first). Its
// arrays above the arguments' slots take 92 bytes, one 12-byte point
// fewer than here: the falling Djinni is projected into shard, not a
// point of its own (164); screenB is declared before screenA and frame
// after work, as the native's stack offsets show. One spill slot more
// than the native's remains (the native has one slot between frame and
// the blitter pair and nine below it, here none and eleven), so every
// offset is still 4 off and the count stays 1130.

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

void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void); // BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(
    void); // BattleFx_ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
void BattleFx_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleFx_BuildBlitter
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
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleFx_SpawnObjects(s32 count, s32 resource, s32 mode); // SpawnObjects
void BattleFx_SelectLivingTargets(
    struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle);            // SceneTransform_ApplyRoll
void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleFx_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattleEventRuntime_BeginPhaseFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
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
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 Battle_GetObjectTableValueFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleFx_BeginTiledCanvas(s32 bgControl);
void BattleFx_BeginTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#include "io_write_queue.h"
#include "irqlock.h"
#include "object_runtime.h"

struct ProjectedPoint {
  s32 x, y, depth;
};

s32 Render_ProjectPoint(const void *point, struct ProjectedPoint *screen);
void Render_ProjectVertices(const u8 *vertices, void *projected, s32 count);

extern char ResourceId_Data0C1;
extern char ResourceId_DustBlobSheet;
extern char ResourceId_Data0CF;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_BlueFlameSheet;
extern u32 gCameraSceneParameters[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02012000[];
extern const s8 Data_08198b7a[]; // the cone's ring heights
extern const s8 Data_08198b82[]; // and radii
extern const u8 Data_08198b8a[]; // each ripple's delay
extern const u8 Data_08198b8e[]; // its growth
extern const s8 Data_08198b92[]; // its height
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];
extern const u8 Data_0819919c[];
extern const u8 Data_081991a4[];

// Abilities: Serac.
//
// The Mercury Djinni drops onto the target, dust bursting where it lands;
// a spire of ice (a cone of 8 rings of 8 vertices, its mesh built on the
// first frame) grows up around the target in rings of mist while the
// camera circles, the caster closes in at frame 56, and at 64 the spire
// shatters into 64 tumbling shards.
void BattleFx_RunSerac(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle blitters[2];
  s32 screenB[3];
  s32 screenA[3];
  s16 *camera;
  s16 savedYaw;
  s32 *target;
  s32 world[3];
  struct ProjectedPoint screen;
  struct ProjectedPoint shard;
  struct ProjectedPoint drop;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct EffectStep *p;
  u8 color;
  s32 savedScrollX;
  s32 savedScrollY;
  s32 row;
  s32 column;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  Func_0813ba50();
  if (effect->side == 0) {
    BattleFx_BeginTiledCanvasFilled(0x2000);
  } else {
    BattleFx_BeginTiledCanvas(0x2000);
  }
  Func_0815585c(effect, (effect->side << 4) | 0x101, screenA, screenB);
  Iwram_ClearWords((void *)0x06004000, 0x3c00);
  Iwram_ClearWords(canvas, 0x3c00);
  REG_BLDALPHA = 0x1010;
  REG_BG2PA = 0x100;
  BattleFx_FetchRectangleBlitters(0, (void **)blitters);
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, gData_02014000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CF, work->sheet + 0x400, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                             work->sheet + 0x4000, 1, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
  camera = (s16 *)Ram_HeapSlots->cameraWork;
  savedYaw = camera[27];
  target = *GetBattleObjectSlotFar(effect->actors[0]);
  REG_BG2PA = 0x100;
  REG_BG2X = 0;

  for (frame = 0; frame != 88; frame++) {
    if (frame == 0) {
      camera[27] = savedYaw;
      for (row = 0; row != 8; row++) {
        for (column = 0; column != 8; column++) {
          s32 height;
          s32 radius;
          s32 angle;
          s8 *vertex;

          if (row == 0) {
            height = Data_08198b7a[row];
            radius = Data_08198b82[row];
          } else {
            height = Data_08198b7a[row] + (Random16() & 3);
            radius = Data_08198b82[row] + (Random16() & 3);
          }
          angle = (column << 13) + 0x1000;
          vertex = (s8 *)gMapCellBuffer + (row * 8 + column) * 4;
          vertex[0] = (radius * Trig_Sin(angle)) >> 16;
          vertex[1] = -height;
          vertex[2] = (radius * Trig_Cos(angle)) >> 16;
          if (row <= 6) {
            u8 *quad = Data_02012000 + (row * 8 + column) * 24;
            s32 next = (column + 1) % 8 + row * 8;
            s32 here = column % 8 + row * 8;

            quad[9] = row * 8 + 7;
            quad[7] = row * 8 + 7;
            quad[8] = column * 16;
            quad[4] = column * 16 + 15;
            quad[5] = row * 8;
            quad[6] = column * 16 + 15;
            quad[16] = column * 16;
            quad[20] = column * 16;
            quad[17] = row * 8;
            quad[21] = row * 8 + 7;
            quad[18] = column * 16 + 15;
            quad[19] = row * 8;
            quad[0] = next;
            quad[1] = here + 8;
            quad[2] = next + 8;
            quad[12] = here;
            quad[13] = here + 8;
            quad[14] = next;
          }
        }
      }
      Data_02012000[56 * 24] = 0;
      Data_02012000[56 * 24 + 1] = 0;
      Data_02012000[56 * 24 + 2] = 0;
    }

    // The camera circles the spire.
    if ((u32)(frame - 4) <= 79) {
      s32 turn = 320;

      if (frame > 63) {
        turn = 1600 - frame * 20;
      }
      if (effect->side == 0) {
        camera[27] -= turn;
      } else {
        camera[27] += turn;
      }
      if (effect->side == 0) {
        if (camera[27] < -14000) {
          camera[27] = -14000;
        }
      } else if (camera[27] > 14000) {
        camera[27] = 14000;
      }
    }

    vertices = Runtime_BumpAllocateAlternatePool(0x400);
    model = Render_AllocateModelList(1);
    savedScrollX = gCameraSceneParameters[3];
    savedScrollY = gCameraSceneParameters[4];
    gCameraSceneParameters[4] = savedScrollY - 16;
    Render_ResetTransformState();
    Transform_SetLookAt((s32 *)camera, (s32 *)camera + 3);
    world[1] = 0;
    world[0] = target[2];
    world[2] = target[4];
    Render_ProjectPoint(world, &screen);
    if (screen.x <= 63) {
      screen.x = 64;
    }
    if (screen.x > 176) {
      screen.x = 176;
    }
    gCameraSceneParameters[3] = 184 - screen.x;
    IO_WRITE_QUEUE_PUSH(0x04000028, (64 - screen.x) << 8, 0x30000);

    // Dust where the Djinni lands.
    if ((u32)frame <= 63) {
      if (frame == 0) {
        for (i = 0, p = work->particles; i != 32; i++, p++) {
          s32 speed = Random16() & 1023;
          s32 angle = Random16() & 0xffff;

          speed += 1024;
          p->y = 0;
          p->x = target[2];
          p->z = target[4];
          p->velocityX = (speed * Trig_Sin(angle)) >> 9;
          p->velocityY = 0;
          p->velocityZ = (speed * Trig_Cos(angle)) >> 9;
          p->variant = -i;
        }
      }
      for (i = 0, p = work->particles; i != 32; i++, p++) {
        s32 picture;

        if ((u32)++p->variant > 23) {
          continue;
        }
        picture = p->variant / 6;
        Render_ProjectPoint(p, &shard);
        BattleFxKernels_IntegrateVector3(p, 60, 0x2000);
        blitters[0](canvas, work->sheet + 0x400 + (picture << 10), shard.x - 16,
                    shard.y - 16, 32, 32);
      }
    }
    // The Djinni falls.
    if (frame <= 29) {
      drop.x = target[2];
      drop.y = 0x3c0000 - (frame << 17);
      drop.depth = target[4] + (Trig_Sin(frame << 9) << 5);
      Render_ProjectPoint(&drop, &shard);
      blitters[1](canvas, work->sheet + 0x4000, shard.x - 20, shard.y - 30, 40,
                  40);
    }
    if (frame == 4) {
      Audio_PlayCue(156);
    }
    if (frame == 30) {
      Audio_PlayCue(139);
    }
    if (frame == 60) {
      Audio_PlayCue(212);
    }
    if (frame == 64) {
      Audio_PlayCue(134);
      Func_081180e8(-1);
    }
    if (frame == 30) {
      Func_0814cd48(effect->actors[0], 9, 5, -1, 0);
    }
    if (frame == 26) {
      work->shakeFrames = 8;
    }
    if (frame == 64) {
      work->shakeFrames = 4;
      Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
    }
    if (frame == 70) {
      Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
    }
    if (frame == 56) {
      // The caster closes four fifths of the way.
      struct ObjectRuntime *caster =
          *(struct ObjectRuntime **)GetBattleObjectSlotFar(effect->actor);
      s32 *to = *GetBattleObjectSlotFar(effect->actors[0]);
      s32 *from = (s32 *)caster;
      s32 x = from[2];
      s32 z;
      s32 dx = (to[2] - x) * 80 / 100;
      s32 dz;
      s32 speed;

      z = from[4];
      dz = (to[4] - z) * 80 / 100;
      x += dx;
      z += dz;
      dx >>= 8;
      dz >>= 8;
      speed = (Iwram_Sqrt(dx * dx + dz * dz) << 8) / 10;
      from[13] = speed;
      from[12] = speed;
      ((u8 *)caster)[88] = 1;
      from[10] = 0x60000;
      from[18] = 0x13333;
      from[17] = 0;
      ((u8 *)caster)[90] = 1;
      Object_ResetMotion(caster);
      Object_SetPosition(caster, x, 0, z);
      Object_SetMode(caster, 2);
    }

    if (frame > 63) {
      if (frame == 64) {
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        for (i = 0, p = work->particles; i != 64; i++, p++) {
          s32 angle = (i & 7) << 13;
          s32 ring = i / 8;

          p->x = target[2] + ((ring * Trig_Sin(angle)) << 2);
          p->y = ((Random16() & 0xffff) << 2) - ((ring * 5) << 17) + 0x400000;
          p->z = target[4] + ((ring * Trig_Cos(angle)) << 2);
          p->velocityX = ((Random16() & 0xffff) << 1) + 0x10000;
          p->velocityY = ((Random16() & 0xffff) - 12768) * 3;
          p->velocityZ = ((Random16() & 0xffff) - 0x8000) << 1;
          if (p->x < 0) {
            p->velocityX = -p->velocityX;
          }
          p->variant = (Random16() & 31) + 32;
        }
      } else {
        s32 angle;

        model->kind = 2;
        model->unk04 = 0;
        model->mesh = Data_0819919c;
        model->vertices = vertices;
        for (i = 0, p = work->particles, angle = frame << 10; i != 64;
             i++, p++, angle += 0x400) {
          if (p->variant < 0) {
            continue;
          }
          color = p->variant;
          p->variant -= 2;
          model->shade = (s32)&color;
          BattleFxKernels_IntegrateVector3(p, 62, 0x400);
          Transform_Push();
          Func_08015160(p->x, p->y, p->z);
          SceneTransform_ApplyPitch(angle);
          Func_080150e4(angle);
          Func_0801521c(0x10000);
          Render_ProjectVertices(Data_081991a4, vertices, 3);
          Render_DrawModelList(model);
          Transform_Pop();
        }
      }
    } else if (frame > 25) {
      s32 scale;
      s32 k;
      s32 start;

      // The spire, then the mist inside it.
      texture.logWidth = 5;
      texture.logHeight = 5;
      texture.pixels = work->sheet;
      model->kind = 6;
      model->unk04 = 3;
      model->vertices = vertices;
      model->scrollU = 0;
      model->texture = &texture;
      model->mesh = Data_02012000;
      model->scrollV = -((frame / 2) & 31);
      Transform_Push();
      if (frame <= 31) {
        scale = frame * 10000 - 260000;
      } else {
        scale = 60000;
      }
      model->shade = 0;
      Func_08015160(target[2], -0x40000, target[4]);
      Func_080151e4(scale, scale, scale);
      Render_ProjectVertices(gMapCellBuffer, vertices, 64);
      Render_DrawModelList(model);
      model->kind = 7;
      model->scrollU = 0;
      model->shade = -32;
      model->scrollV = (frame / 2) & 31;
      model->unk04 = 2;
      Render_DrawModelList(model);
      Transform_Pop();

      // Four rings of mist spread from its foot.
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = gData_02014000;
      model->texture = &texture;
      model->mesh = Data_08198ec4;
      model->kind = 7;
      model->unk04 = 0;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;
      for (k = 0; k != 4; k++) {
        s32 shade;

        start = Data_08198b8a[k] + 22;
        if (frame <= start) {
          continue;
        }
        scale = (frame - start) * Data_08198b8e[k] * 1500 + 0x4180;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        Transform_Push();
        model->shade = shade;
        Func_08015160(target[2], Data_08198b92[k] << 16, target[4]);
        Func_080151e4(scale, scale, scale);
        model->scrollU = ((frame + k * 5) & 7) << 4;
        Render_ProjectVertices(Data_08198cac, vertices, 32);
        Render_DrawModelList(model);
        Transform_Pop();
      }
    }
    MemFree(model);
    MemFree(vertices);
    gCameraSceneParameters[3] = savedScrollX;
    gCameraSceneParameters[4] = savedScrollY;
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  BattleFx_EndCanvasLayer();
}
