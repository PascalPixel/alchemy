// Draft of BattleEffect_RunTrident, decompiled from the listing: differ 1082 of
// 1122 (draft 1112), the same 92 calls and control flow; allocation differs.
// 1122 (draft 1112), the same 93 calls and control flow; allocation differs.

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

void Func_0815e22c(s32 *center); // the targets' centre on screen
void Func_0815f0a0(s32 palette);
void Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                   s32 mode);
void ResourceObject_ReleaseFar(void *object);

extern char ResourceId_TornadoSheet;
extern char ResourceId_Data0F8;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0C1;
extern char ResourceId_Data0FA;
extern char ResourceId_Data0DA;
extern char ResourceId_Data134;
extern char ResourceId_EffectPalette02;
extern char ResourceId_OrangePaletteB;
extern u8 gMapCellBuffer[];
extern u8 gData_02015000[];
extern u8 Data_02011000[];
extern u8 Data_02011400[];
extern u8 Data_02012964[]; // 128 sparks
extern const s32 Data_08196f18[2];
extern const u16 gData_08197410[];
extern const u16 gData_08197486[]; // the glints' cells
extern const u8 gData_08197492[];  // widths
extern const u8 gData_08197498[];  // heights
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];
extern const u8 gData_08199268[];
extern const u8 Data_08199dcb[];  // each ring's start
extern const u8 Data_08199dd3[];  // growth
extern const s8 Data_08199ddb[];  // height
extern const s32 Data_08199de4[]; // each trident's yaw and turn
extern const u8 Data_08199e24[];
extern const u8 Data_08199e54[];

// Abilities: Trident (Poseidon).
//
// Poseidon (four objects) rises behind the targets as rings of foam
// spread across the water; seven spinning tridents fly in from frame 18,
// spray bursts up around them, and at 64 the waters strike every target.
void BattleEffect_RunTrident(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  DrawRectangle blit;
  u16 *circles;
  s32 center[3];
  s32 placement[4];
  s32 source[2];
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct EffectStep *p;
  struct EffectStep *sparks;
  s32 centerX;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_SpawnObjects(4, 664, 1);
  REG_WININ = 0x2737;
  REG_WINOUT = 0x3f21;
  REG_WIN0V = 0x107c;
  REG_WIN1V = 0x1088;
  BattleEffect_LoadBlitter(SLOT_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  work->transferMode = 2;
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  circles = (u16 *)(work->sheet + 0x4c56);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F8, work->sheet + 0xc56, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             gMapCellBuffer + 0x5000, 1, 0);
  {
    s32 offset = 0x2400;
    s32 size = 2;

    // Twelve shaded droplets, 2 to 24 wide.
    for (i = 0; i != 12; i++) {
      Func_0815b434(gMapCellBuffer + offset, size, 0x10000);
      circles[i] = offset;
      offset += (i * 2 + 3) * size / 2;
      size += 2;
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, gMapCellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0FA, Data_02011000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, Data_02011400, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                  128);
  for (i = 0, p = work->particles; i != 32; i++, p++) {
    s32 radius = i * 2;
    s32 angle = Random16() & 0xffff;

    p->x = radius * Trig_Sin(angle);
    p->y = -(radius * Trig_Cos(angle));
    p->variant = i / 2 + 25;
  }
  Func_0815e22c(center);
  centerX = center[0] / 2;
  sparks = (struct EffectStep *)Data_02012964;

  for (frame = 0; frame != 104; frame++) {
    if (frame == 0) {
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                      128);
    }
    if (frame <= 76) {
      if (frame > 64) {
        Func_0815f0a0((s32)&ResourceId_OrangePaletteB);
      } else if (frame > 23) {
        Func_0815f0a0((s32)&ResourceId_EffectPalette02);
      } else if (frame > 8) {
        Func_0815f0a0((s32)&ResourceId_OrangePaletteB);
      }
    } else {
      Func_0815f0a0((s32)&ResourceId_EffectPalette02);
    }
    if (frame == 8) {
      work->shakeFrames = frame;
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
      }
    }
    if (frame == 48) {
      work->shakeFrames = 8;
    }
    if (frame == 64) {
      work->shakeFrames = 16;
      Func_081180e8(144);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x20000, 0x80000,
                                  0x80000, 120);
      }
    }
    if (frame == 0) {
      Audio_PlayCue(212);
    }
    if (frame == 4) {
      Audio_PlayCue(144);
    }
    if (frame == 16) {
      Audio_PlayCue(164);
    }
    if (frame == 64) {
      Audio_PlayCue(145);
    }

    // Poseidon rises.
    if (frame <= 63) {
      s32 x = centerX - 8;
      s32 visible = 128;
      s32 rise;

      if (frame <= 9) {
        rise = frame * 16 - 144;
      } else {
        rise = 0;
      }
      if (rise + 128 > 108) {
        visible = 128 - rise - 20;
      }
      if (visible > 0) {
        s32 y;

        source[0] = Data_08196f18[0];
        source[1] = Data_08196f18[1];
        placement[3] = 0;
        placement[1] = 255 << 16;
        for (i = 0, y = (rise << 16) + 0x280000; i != 4; i++, y += 0x200000) {
          placement[0] = (x << 17) + 0x100000;
          placement[2] = y;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement, source,
                                            0);
        }
      }
    }

    // Spray: a pool of foam, then droplets thrown up.
    if ((u32)(frame - 12) <= 52) {
      s32 grow;

      if (frame == 12) {
        for (i = 0; i != 32; i++) {
          work->particles[i].variant = -2 * i;
        }
      }
      grow = frame * 4 - 156;
      for (i = 0, p = work->particles; i != 14; i++, p++) {
        p->variant++;
        if (i == 0) {
          if (frame > 39) {
            u8 *pool = work->sheet + 0x4c86;

            Func_0815b434(pool, grow, 0x10000);
            Func_0818caa8(pool, centerX, 100, grow);
          }
          continue;
        }
        if (p->variant == 0) {
          s32 angle = Random16() & 0x7fff;
          s32 speed = (Random16() & 511) + 512;

          p->x = ((speed * Trig_Sin(angle + 0x4000)) >> 4) + (centerX << 16);
          p->y = ((speed * Trig_Cos(angle + 0x4000)) >> 3) + 0x680000;
          angle += 0xc000;
          p->velocityX = (speed * Trig_Sin(angle)) >> 9;
          p->velocityY = (speed * Trig_Cos(angle)) >> 8;
          p->velocityZ = (Random16() & 7) + 4;
        }
        if ((u32)p->variant <= 31) {
          s32 size = ((Trig_Sin(p->variant << 10) * p->velocityZ) >> 16) * 2;

          if ((u32)(size - 2) <= 61) {
            s32 y = HI(p->y);
            s32 x = HI(p->x);

            BattleEffectKernels_IntegrateVector2(p, 64, 0);
            Func_0818caa8(gMapCellBuffer + circles[(size - 2) / 2], x, y, size);
          }
        }
      }
    }

    if (frame > 7) {
      s32 k;

      vertices = Runtime_BumpAllocateAlternatePool(0x400);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->unk04 = 0;
      texture.pixels = gMapCellBuffer;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = Data_08198ec4;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;

      // Eight rings of foam.
      for (k = 0; k != 8; k++) {
        s32 start = Data_08199dcb[k];
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (frame - start) * Data_08199dd3[k] * 1500 + 0x4180;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160((centerX - 64) << 16, Data_08199ddb[k] << 16, 0);
        Func_080151e4(scale, scale * 2, scale);
        model->scrollU = ((frame + k * 5) & 7) << 4;
        SceneTransform_ApplyPitch(3000);
        Render_TransformVertices(Data_08198cac, vertices, 32);
        Render_DrawModelList(model);
      }

      // The tridents.
      if (frame <= 71) {
        struct EffectStep *trident = &work->particles[32];
        const s32 *spin;

        texture.logWidth = 5;
        texture.logHeight = 7;
        model->texture = &texture;
        model->kind = 7;
        model->mesh = gData_08199268;
        model->vertices = vertices;
        model->scrollU = 0;
        model->scrollV = 0;
        if (frame == 8) {
          for (i = 0, p = trident, spin = Data_08199de4; i != 8;
               i++, p++, spin += 2) {
            p->z = spin[0];
            p->velocityZ = spin[1];
            p->variant = (i % 6) * 4;
          }
        }
        for (i = 0, p = trident; i != 7; i++, p++) {
          s32 picture;

          if (frame < i * 4 + 18 || frame >= i * 4 + 42) {
            continue;
          }
          picture = (p->variant / 8) % 3;
          model->shade = -16;
          texture.pixels = work->sheet + 0xc56 + (picture << 12);
          Render_ResetTransformState();
          Func_08015160((centerX - 64) << 16, 0x200000, 0);
          Func_080150e4(p->z);
          if (i & 1) {
            SceneTransform_ApplyYaw(0x8000);
          }
          p->z += p->velocityZ;
          Func_080151e4(0x20000, 0x12710, 0x10000);
          Render_TransformVertices(Data_08199e24 + picture * 16, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      MemFree(model);
      MemFree(vertices);
    }

    // The strike: flashes and a fountain of sparks.
    if (frame > 63) {
      if (frame == 64) {
        s32 x = centerX << 16;

        for (i = 0, p = work->particles; i != 32; i++, p++) {
          s32 angle = Random16();
          s32 speed = (Random16() & 511) + 512;
          s32 velocityY;
          s32 delay;

          p->velocityX = (speed * Trig_Sin(angle)) >> 8;
          velocityY = (speed * Trig_Cos(angle)) >> 7;
          p->velocityY = velocityY;
          if (i > 15) {
            p->x = (p->velocityX << 2) + x;
            p->y = (velocityY << 2) + 0x640000;
            delay = 16 - i;
          } else {
            p->x = x;
            p->y = (Random16() % 104) << 16;
            delay = -i;
          }
          p->variant = delay / 4;
        }
        for (i = 0, p = sparks; i != 128; i++, p++) {
          s32 angle = (Random16() & 0x7fff) + 0x4000;
          s32 speed = (Random16() & 511) + 512;

          p->x = x;
          p->y = 0x640000;
          p->velocityX = (speed * Trig_Sin(angle)) >> 6;
          p->velocityY = (speed * Trig_Cos(angle)) >> 5;
          p->variant = (Random16() & 15) + 16;
        }
      }
      for (i = 0, p = work->particles; i != 32; i++, p++) {
        if ((u32)p->variant <= 23) {
          blit(canvas, gData_02015000 + ((p->variant / 4) << 11), HI(p->x) - 16,
               HI(p->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(p, 62, -0x4000);
        }
        p->variant++;
      }
      for (i = 0, p = sparks; i != 128; i++, p++) {
        s32 size;

        if (p->variant < 0) {
          continue;
        }
        size = (p->variant >> 3) + 1;
        blit(canvas, sprites + gData_08197410[size - 1], HI(p->x) - size / 2,
             HI(p->y) - size, size, size * 2);
        p->variant--;
        BattleEffectKernels_IntegrateVector2(p, 56, 0x2000);
      }
    }

    // Glints over the water.
    if ((u32)(frame - 24) <= 39) {
      for (i = 0; i != 4; i++) {
        s32 k = i & 3;
        s32 angle = Random16();
        s32 x =
            centerX + ((Trig_Sin(angle) << 3) >> 16) - (gData_08197492[k] >> 1);
        s32 y = (i << 5) + ((Trig_Cos(angle) << 4) >> 16) -
                (gData_08197498[k] >> 1);

        Func_08196404(188, 7, 7, 3 | Data_08199e54[Random16() & 3], 1);
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, work->sheet + gData_08197486[k], x, y, gData_08197492[k],
            gData_08197498[k]);
        Runtime_ReleaseHeapBlock(188);
      }
    }
    Func_08158ce0(16, 16);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  for (i = 0; i != 4; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}
