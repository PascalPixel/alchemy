// Draft of BattleEffect_RunRisingDragonEnemy, decompiled from the listing: differ
// 1038 of 1034 (draft 1024), the same 76 calls and control flow; allocation
// differs.

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

void BattleBackground_LoadFar(s32 layer, s32 resourceId, s32 mode);
void Func_08164a4c(s32 red, s32 green, s32 blue); // Palette_BrightenBgEntries
void BattleEffect_BuildRepeatingTilemap(void);

extern char ResourceId_SmokeBandSheet;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_ClawSlashSheet;
extern char ResourceId_Data134;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_RedHazeBackdrop;
extern u32 gCameraSceneParameters[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[]; // 256 sparks
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern const u16 gData_08197410[];
extern const u8 gData_08199340[];
extern const u8 Data_081991e0[];

#define SEGMENTS 33

// Abilities: Rising Dragon, the enemy's.
//
// The dragon bursts up under the target and carries it into the sky: the
// backdrop changes to a red haze scrolling past as the target is lifted,
// swaying, then is dropped back to the field at frame 166 and lands at 179.
void BattleEffect_RunRisingDragonEnemy(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  DrawRectangle blit;
  s32 *victim;
  s16 *camera;
  s16 savedYaw;
  s32 savedMode;
  s32 savedX;
  s32 savedZ;
  s32 height;
  struct EffectPosition source;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *sparks;
  struct EffectStep *spark;
  s32 frame;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(1);
  BattleEffect_LoadBlitter(SLOT_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  victim = *GetBattleObjectSlotFar(effect->actors[0]);
  camera = (s16 *)Ram_HeapSlots->cameraWork;
  savedYaw = camera[27];
  height = 0;
  savedMode = victim[18];
  savedX = victim[2];
  savedZ = victim[4];
  EffectPosition_ProjectBattler(effect->actors[0], &source);
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  REG_BG2PA = 0x80;
  Render_BuildQuadStrip(Data_02012000, 4, 32, 32);
  sparks = (struct EffectStep *)gData_02014000;

  for (frame = 0; frame != 186; frame++) {
    if (frame == 0) {
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
      for (i = 0; i != 128; i++) {
        sparks[128 + i].variant = -1;
      }
      for (i = 0, spark = sparks; i != 128; i++, spark++) {
        s32 angle;
        s32 speed;

        spark->x = (source.x / 2) << 16;
        spark->y = 120 << 15;
        angle = (Random16() & 0x7fff) + 0x4000;
        speed = (Random16() & 1023) + 32;
        spark->velocityX = (speed * Trig_Sin(angle)) >> 6;
        spark->velocityY = (speed * Trig_Cos(angle)) >> 6;
        spark->variant = (Random16() & 7) + 32;
      }
    }
    if (frame == 4) {
      Func_0814cd48(effect->actors[0], 7, 5, 0, 2);
      work->shakeFrames = 8;
      Audio_PlayCue(145);
    }
    if (frame == 163) {
      REG_BG1CNT = 0x784;
    }
    if (frame == 164) {
      BattleBackground_LoadFar(1, ((u16 *)Ram_HeapSlots->battleWork)[824], 24);
    }
    if (frame == 165) {
      gCameraSceneParameters[4] = 120;
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
    }
    if (frame == 166) {
      camera[27] = savedYaw;
      victim[18] = savedMode;
      victim[3] = 200 << 15;
      victim[2] = savedX;
      victim[4] = savedZ;
      gBgScroll[1].y = 32;
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
    }
    if (frame == 179) {
      Func_081180e8(134);
      work->shakeFrames = 8;
    }
    if (frame > 165) {
      work->transferMode = 3;
      work->transferValue = 0x04040404;
      goto next;
    }

    if (frame == 59) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 60) {
      u16 *color;

      // Into the sky: the red haze, its palette made grey-blue.
      BattleBackground_LoadFar(1, (s32)&ResourceId_RedHazeBackdrop, 0);
      for (i = 0, color = (u16 *)(BG_PLTT + 0xc0); i != 128; i++, color++) {
        s32 red = *color & 31;
        s32 blue = (*color >> 10) & 31;

        if (red > 31) {
          red = 31;
        }
        if (blue > 31) {
          blue = 31;
        }
        *color = (red << 10) | (red << 5) | blue;
      }
      BattleEffect_BuildRepeatingTilemap();
      gBgScroll[1].y = 104;
      gCameraSceneParameters[4] = 240;
      if (effect->side == 0) {
        camera[27] = 0x4000;
      } else {
        camera[27] = 0xc000;
      }
      for (i = 0; i != 256; i++) {
        sparks[i].variant = -1;
      }
      height = 220;
    }
    if (frame == 134) {
      Audio_PlayCue(145);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame > 59) {
      s32 scroll = gBgScroll[1].y;
      s32 climb;

      if (scroll > 104) {
        scroll -= 0x10000;
      }
      scroll -= 8;
      if (scroll < -16) {
        scroll += 120;
      }
      gBgScroll[1].y = scroll;
      climb = 4 - (frame - 60) / 8;
      if (climb < 0) {
        climb = 0;
      }
      if (frame > 119) {
        climb = frame - 120;
      }
      height -= climb;
    } else {
      height = 200 - frame * 8;
    }
    if (height <= 55) {
      height = 56;
    }
    victim[18] = 0;
    if ((u32)(frame - 4) <= 55) {
      victim[3] = (frame * 9 - 18) << 16;
    } else if (frame > 59) {
      victim[2] = 0;
      victim[3] = (294 - height) << 16;
      victim[4] = -(Trig_Sin(frame << 10) << 3);
      if (frame == 60) {
        EffectPosition_ProjectBattler(effect->actors[0], &source);
      }
      EffectPosition_ProjectBattler(effect->actors[0], &target);
    }
    if ((u32)(frame - 152) <= 15) {
      work->transferMode = 1;
      work->transferValue = 0x20202020;
      if (frame > 162) {
        work->transferValue = 0x3f3f3f3f;
      }
      Func_08164a4c(2, 2, 2);
    }
    if (frame < 0) {
      goto next;
    }

    // The dragon's body, swaying.
    for (j = 0; j != SEGMENTS; j++) {
      s8 *corner = (s8 *)gMapCellBuffer + j * 8;

      for (i = 0; i != 2; i++, corner += 4) {
        corner[0] =
            i * 20 + ((Trig_Sin(j * 2018 + (frame << 10)) * 2) >> 16) - 10;
        corner[2] = 0;
        corner[1] = j * 2 - 64;
      }
    }
    model = Render_AllocateModelList(1);
    model->kind = 6;
    model->shade = 0;
    model->scrollV = 0;
    model->vertices = Data_02011000;
    texture.logWidth = 6;
    texture.logHeight = 5;
    model->texture = &texture;
    texture.pixels = work->sheet;
    model->scrollU = 127 & ~(frame << 2);
    model->mesh = Data_02012000;
    Render_ResetTransformState();
    Func_080151e4(0x8000, 0x10000, 0x8000);
    Transform_Push();
    Func_08015160((source.x - 128) << 16, height << 16, 0);
    if (effect->side == 1) {
      SceneTransform_ApplyYaw(0x8000);
    }
    Func_0801521c(0x21770);
    Render_TransformVertices(gMapCellBuffer, Data_02011000, 66);
    Render_DrawModelList(model);
    Transform_Pop();
    if (frame <= 133 && (u32)(frame - 17) > 42) {
      // The claw.
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gData_08199340;
      model->scrollU = 0;
      texture.pixels = work->sheet + 0x4000;
      Func_08015160((target.x - 124) << 16, (height - 144) << 16, 0);
      Func_080150e4(0x4000);
      Func_080151e4(0x19c40, 0x14e20, 0x10000);
      Render_TransformVertices(Data_081991e0, Data_02011000, 4);
      Render_DrawModelList(model);
    }
    MemFree(model);

    if (frame == 134) {
      for (i = 0, spark = sparks; i != 256; i++, spark++) {
        s32 angle;
        s32 speed;

        spark->x = (source.x / 2) << 16;
        spark->y = 0;
        angle = (Random16() & 0x7fff) - 0x4000;
        speed = (Random16() & 255) + 32;
        spark->velocityX = (speed * Trig_Sin(angle)) >> 5;
        spark->velocityY = (speed * Trig_Cos(angle)) >> 5;
        spark->variant = (Random16() & 7) + 32;
      }
    }
    if ((u32)(frame - 134) <= 7) {
      struct EffectStep *burst = &work->particles[(frame - 134) * 4];

      for (i = 0; i != 4; i++, burst++) {
        s32 angle;
        s32 speed;

        burst->y = 0;
        burst->x = (source.x / 2) << 16;
        angle = (Random16() & 0x7fff) - 0x4000;
        speed = (Random16() & 255) + 32;
        burst->velocityX = (speed * Trig_Sin(angle)) >> 5;
        burst->velocityY = (speed * Trig_Cos(angle)) >> 5;
        burst->variant = 0;
      }
    }
    for (i = 0, spark = work->particles; i != 32; i++, spark++) {
      s32 picture = spark->variant;

      if ((u32)picture <= 23) {
        if (picture > 1) {
          blit(canvas, work->sheet + 0x800 + ((picture / 4) << 11),
               HI(spark->x) - 16, HI(spark->y) - 32, 32, 64);
        }
        BattleEffectKernels_IntegrateVector2(spark, 60, -0x1000);
        spark->variant++;
      }
    }
    if (frame <= 133) {
      s32 y = 120;

      if (frame > 59) {
        y = height - 80;
      }
      spark = &sparks[128 + ((frame - 16) & 63) * 2];
      for (i = 0; i != 2; i++, spark++) {
        s32 angle;
        s32 speed;

        spark->y = y << 16;
        spark->x = (source.x / 2) << 16;
        angle = Random16() & 0x7fff;
        if (frame > 59) {
          angle += 0xc000;
        } else {
          angle += 0x4000;
        }
        speed = (Random16() & 255) + 32;
        spark->velocityX = (speed * Trig_Sin(angle)) >> 6;
        spark->velocityY = (speed * Trig_Cos(angle)) >> 6;
        if (frame > 59) {
          spark->velocityX /= 2;
        }
        spark->variant = (Random16() & 7) + 32;
      }
    }
    {
      s32 gravity = -0x1000;

      if (frame > 59) {
        gravity = 0x4000;
      }
      for (i = 0, spark = sparks; i != 256; i++, spark++) {
        s32 size;

        if (spark->variant < 0) {
          continue;
        }
        size = (spark->variant - 8) / 8;
        if (frame > 59) {
          size += 2;
        }
        if (size <= 0) {
          size = 1;
        }
        blit(canvas, sprites + gData_08197410[size - 1],
             HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(spark, 60, gravity);
        spark->variant--;
      }
    }
  next:
    ObjectGroup_CountDownMemberTimers();
    if ((u32)(frame - 32) > 133) {
      Func_08158ce0(8, 8);
    }
    work->transferPending = 1;
    WaitFrames(1);
  }
  Runtime_ReleaseHeapBlock(104);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
