// Draft, differ 481 of 927 (draft 930 instructions, no device): the body
// of the leaping strike family (BattleEffect_RunPlanetDiver mode 0 and
// BattleEffect_RunPlanetary mode 1 in summon_modes_7.c), written from the listing
// on The Broken Seal's canvas skeleton (twin BattleEffect_RunPlanetDiver 0.53 by
// shape and call order: the caster object leaps, lands and slides as there)
// with the ARM model renderer for mode 1's claw, rings and pillars. The native
// frame is 92 bytes: effect sp+68, mode sp+64, work sp+60, canvas sp+56, draw
// sp+48, graphics sp+44, target sp+40, stepX sp+36, stepY sp+32, homeX sp+28,
// homeZ sp+24, landed sp+20, &pos sp+16, projected sp+12, &texture sp+8, the
// texture at sp+72 and pos at sp+80. Here it is 88 (one spill fewer), so every
// stack offset is 4 lower, and the caster stays in r9 as in the native but the
// loops' registers differ. The slide divides by a local 16, which the native
// passes to __divsi3. The slide is written as two plain ifs; the native's shape
// (the landing test reusing the new y) is the same code (564 either way).
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "motobj.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

#define LEAP_FRAMES 88

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

extern char ResourceId_Data134;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_FlameballSheet;
extern char ResourceId_ClawSlashSheet;
extern char ResourceId_Data0DA;
extern char ResourceId_Data0C1;
extern char ResourceId_OrangePaletteB;
extern struct EffectStep Data_02016000[];
extern u16 gData_08197410[];
extern const u16 gData_08199340[];
extern const u16 gData_081990d0[];
extern const u16 Data_08198ec4[];
extern const u8 Data_081991e0[];
extern const u8 gData_08199090[];
extern const u8 Data_08198cac[];
extern const u8 Data_08199e58[];
extern const u8 Data_08199e5c[];
extern const s8 Data_08199e60[];
extern const u8 Data_08199e64[];
extern const u8 Data_08199e68[];
extern const s8 Data_08199e6c[];

void Func_0813ba50(void);
void BattleEffect_StartCanvasLayer(s32 layer);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_LoadBlitter(s32 slot, s32 value);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Resource_LoadAndDecompress(s32 id, void *target, s32 unpack, s32 wait);
void *Resource_GetTableEntry(s32 id);
s32 Task_Add(void (*callback)(void), s32 order);
s32 Task_Remove(void (*callback)(void));
struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *out);
void Object_SetMode(struct MotionObject *object, s32 mode);
void Object_ResetMotion(struct MotionObject *object);
void Runtime_ReleaseHeapBlock(s32 slot);
void WaitFrames(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_080150e4(s32 angle);
void Func_0801521c(s32 value);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_TransformVertices(const u8 *vertices, void *projected, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_0814cd48(s32 actor, s32 palette, s32 mode, s32 timer, s32 frames);
void Func_08118088(s32 actor, s32 mode);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Func_08158ce0(s32 randomMask, u32 range);
void ObjectGroup_CountDownMemberTimers(void);
void Func_081180e8(s32 phase);
void Sound_PlayFar(s32 cue);

// The leaping strike family (BattleEffect_RunPlanetDiver is mode 0, BattleEffect_RunPlanetary mode
// 1; effect kinds 60 and 128). The caster flashes and sixteen flames ring
// it (frames 2-15), leaps up at frame 4 and comes down on the target at
// frame 16, sliding to it over the frames after; on landing 256 sparks
// burst from it. In mode 0 the caster then shows a fireball and one target
// is struck; in mode 1 a claw slashes in 3D, three rings and two pillars
// spread, every target is struck and at frame 50 all are thrown back.
void Func_0818d5a8(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition pos;
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *graphics;
  DrawRectangle draw;
  struct MotionObject *caster;
  struct MotionObject *target;
  struct RenderModel *model;
  void *projected;
  struct EffectStep *spark;
  s32 stepX;
  s32 stepY;
  s32 homeX;
  s32 homeZ;
  s32 landed;
  s32 frame;
  s32 i;
  s32 k;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  if (mode == 0) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet,
                               work->sheet + 0x3000, 1, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                               work->sheet + 0x3000, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, work->sheet + 0x4000,
                               0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet + 0x5000,
                               0, 0);
  }
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  // The blitter faces the caster's side.
  if (effect->side == 0)
    BattleEffect_LoadBlitter(104, 1 << 4 | 3);
  else
    BattleEffect_LoadBlitter(104, 1 << 4 | 7);
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  if (mode == 0)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  for (i = 0; i != 256; i++)
    Data_02016000[i].variant = 0;

  caster = GetBattleObjectSlotFar(effect->actor)->object;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  stepX = 0;
  stepY = 0;
  homeX = caster->x;
  homeZ = caster->z;
  landed = 0;

  for (frame = 0; frame != LEAP_FRAMES; frame++) {
    if (frame == 0) {
      caster->x = homeX;
      caster->y = 0;
      caster->z = homeZ;
      landed = 0;
    }
    if (frame > 17 || frame == 0) {
      EffectPosition_ProjectBattler(effect->actor, &pos);
      pos.x /= 2;
    }

    // Frames 2-15: a flash, then sixteen flames ringing the caster.
    if ((u32)(frame - 2) <= 1)
      draw(canvas, work->sheet, pos.x - 16, pos.y - 64, 32, 64);
    if ((u32)(frame - 4) <= 11) {
      u8 *cell = work->sheet + (frame - 4) / 2 * 2048;

      for (k = 0; k != 16; k++) {
        s32 angle = k << 12;
        s32 x = pos.x + ((frame * Trig_Sin(angle)) >> 16);
        s32 y = pos.y + ((frame * Trig_Cos(angle)) >> 16) - frame;

        draw(canvas, cell, x - 16, y - 64, 32, 64);
      }
    }

    // Frame 4: the leap, away from the caster's own side.
    if (frame == 4) {
      caster->autoFaceMotion = 0;
      caster->verticalMotionStrength = 0x9999;
      caster->velocityY = 15 << 16;
      if (caster->x < 0)
        caster->velocityX = -8 << 16;
      else
        caster->velocityX = 8 << 16;
      if (mode == 1)
        caster->velocityX = -caster->velocityX;
      Object_SetMode(caster, 2);
      work->shakeFrames = 4;
      Sound_PlayFar(136);
    }

    // Frame 16: down at the target's depth, then sliding to it over 16
    // frames.
    if (frame == 16) {
      s32 slide = 16;

      caster->verticalMotionStrength = 0;
      if (mode == 0)
        caster->z = target->z;
      else
        caster->z = 0;
      caster->z = target->z;
      caster->velocityX = 0;
      caster->velocityY = 0;
      caster->velocityZ = 0;
      stepX = (target->x * 60 / 100 - caster->x) / slide;
      stepY = (target->y - caster->y) / slide;
      Object_ResetMotion(caster);
    }

    if (frame > 17) {
      // Sliding to the target while in the air.
      if (caster->y > 0) {
        caster->x += stepX;
        caster->y += stepY;
      }
      if (caster->y <= 0) {
        if (!landed) {
          // The landing: sparks burst from the caster.
          landed = 1;
          caster->y = 0;
          spark = Data_02016000;
          for (i = 0; i != 256; i++) {
            s32 speed = Random16() & 0x3ff;
            s32 angle = Random16() & 0xffff;

            speed += 32;
            spark->x = pos.x << 16;
            spark->y = (pos.y - 24) << 16;
            spark->velocityX = (speed * Trig_Sin(angle)) >> 6;
            spark->velocityY = -((speed * Trig_Cos(angle)) << 1) >> 6;
            spark->variant = (Random16() & 7) + 32;
            spark++;
          }
          work->shakeFrames = 4;
          if (mode == 0) {
            Func_081180e8(145);
            Func_08118088(effect->actors[0], 4);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
          } else {
            Sound_PlayFar(145);
            for (i = 0; i != effect->count; i++) {
              Func_08118088(effect->actors[i], 3);
              Func_0814cd48(effect->actors[i], 7, 5, i, 16);
            }
          }
        }
      }

      if (mode == 0) {
        // The fireball over the caster while it is in the air.
        if (caster->y > 0)
          draw(canvas, work->sheet + 0x3000, pos.x - 20, pos.y - 52, 40, 64);
      } else {
        projected = Runtime_BumpAllocateAlternatePool(1024);
        model = Render_AllocateModelList(1);
        if (frame == 40)
          work->shakeFrames = 14;

        // The claw, slashing until frame 45.
        texture.logWidth = 6;
        texture.logHeight = 6;
        texture.pixels = work->sheet + 0x3000;
        model->kind = 6;
        model->mesh = gData_08199340;
        model->texture = &texture;
        model->vertices = projected;
        model->shade = 0;
        if (frame <= 45) {
          Render_ResetTransformState();
          Func_08015160((pos.x - 60) << 16, (pos.y - 80) << 16, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          if (effect->side == 0) {
            Func_080150e4(-0x3388);
          } else {
            SceneTransform_ApplyYaw(0x8000);
            Func_080150e4(-0x3388);
          }
          Func_0801521c(0x286a0);
          Render_TransformVertices(Data_081991e0, projected, 4);
          Render_DrawModelList(model);
        }

        // Three rings, each from its own frame, spinning as they spread.
        texture.logWidth = 6;
        texture.logHeight = 6;
        texture.pixels = work->sheet + 0x4000;
        model->kind = 7;
        model->mesh = gData_081990d0;
        for (k = 0; k != 3; k++) {
          s32 start = Data_08199e58[k] + 40;

          if (frame > start) {
            s32 shade = (start - frame) * 8 + 56;

            if (shade > 0)
              shade = 0;
            if (shade > -64) {
              s32 scale = (frame - start) * Data_08199e5c[k] * 2000 + 0x8000;

              model->shade = shade;
              Render_ResetTransformState();
              model->scrollV = (frame * 8 + k * 16) & 63;
              Func_08015160((pos.x - 60) << 16, Data_08199e60[k] << 16, 0);
              Func_080151e4(scale >> 1, scale << 2, scale / 4);
              SceneTransform_ApplyPitch(1000);
              if (k & 1)
                SceneTransform_ApplyYaw((k * 4 + frame) << 11);
              else
                SceneTransform_ApplyYaw((k * 4 - frame) << 11);
              Render_TransformVertices(gData_08199090, projected, 32);
              Render_DrawModelList(model);
            }
          }
        }

        // Two pillars where the caster landed, from their own frames.
        model->scrollV = 0;
        model->mesh = Data_08198ec4;
        texture.pixels = work->sheet + 0x5000;
        texture.logWidth = 7;
        texture.logHeight = 7;
        for (k = 0; k != 2; k++) {
          s32 start = Data_08199e64[k] + 34;
          struct EffectStep *base = &work->particles[k];

          if (frame == start) {
            base->x = pos.x;
            base->y = pos.y;
          }
          if (frame > start) {
            s32 shade = (start - frame) * 8 + 40;

            if (shade > 0)
              shade = 0;
            if (shade > -64) {
              s32 scale = (frame - start) * Data_08199e68[k] * 1500 + 0x4180;

              model->shade = shade;
              model->scrollU = ((frame + k * 5) & 7) << 4;
              Render_ResetTransformState();
              Func_08015160((base->x - 60) << 16, Data_08199e6c[k] << 16, 0);
              Func_080151e4(scale, scale * 2, scale);
              SceneTransform_ApplyPitch(2000);
              Render_TransformVertices(Data_08198cac, projected, 32);
              Render_DrawModelList(model);
            }
          }
        }
        MemFree(model);
        MemFree(projected);
      }
    }

    if (mode == 1 && frame == 50) {
      Func_081180e8(134);
      for (i = 0; i != effect->count; i++)
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x40000, 0x100000,
                                  0x10000, 250);
    }

    // The sparks fall, bounce off the ground at 112 and shrink.
    spark = Data_02016000;
    for (i = 0; i != 256; i++) {
      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 56, 0x2000);
        if (spark->y > 112 << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((u32)spark->x < 127 << 16 && spark->y >= 0) {
          s32 size = spark->variant / 8 + 1;

          draw(canvas, graphics + gData_08197410[size - 1],
               (spark->x >> 16) - size / 2, (spark->y >> 16) - size, size,
               size * 2);
        }
      }
      spark++;
    }

    Func_08158ce0(12, 12);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}
