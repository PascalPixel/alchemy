// Draft of Func_08173aac, decompiled from the listing: differ 3362 of 4138
// (draft 4196), the same 356 calls and control flow; allocation differs.

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

extern u8 gMapCellBuffer[];
extern struct EffectStep gData_02014000[];
extern s32 gCameraSceneParameters[];
extern const u16 gData_08197410[];
extern const s8 Data_08198c28[];
extern const u8 Data_08198c34[];
extern const u8 Data_08198c64[];
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];
extern const u8 gData_081991b0[];
extern const u8 Data_081991d0[];
extern const u8 Data_081991e0[];
extern const u8 gData_08199210[];
extern const u8 Data_08199244[];
extern const u8 gData_08199268[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];

extern char ResourceId_Data150;
extern char ResourceId_Data152;
extern char ResourceId_Data154;

void Func_0815f0a0(s32 resource); // BattleEffect_StepPaletteToResource
void Func_08164b2c(s32 red, s32 green, s32 blue);
void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase, s32 bluePhase);
void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames, s32 percent);
void BattleEffect_ArmBg2AffineHBlankDma(void); // BattleEffect_ArmBg2AffineHBlankDma
void Actor_SetMode(void *object, s32 mode);
void Actor_SetAnimationSpeedFar(void *object, s32 value);
void Graphics_LoadGreyRamp(void);
void Func_0815b510(u8 *map, s32 size, s32 scale);
void Palette_LoadBasicObjColors(void);
void Heap_Release(void *buffer);

// The user's figure: a 20 x 40 cell drawn over the canvas.
#define FIGURE(work) ((work)->sheet + 0x65c0)
#define WAIT_TRANSFER(work)                                                    \
  while (*(volatile u32 *)&(work)->vblanksSinceTransfer,                        \
         *(volatile u32 *)&(work)->vblanksSinceTransfer <= 1)

// The shared body of sixteen abilities' effects (variant_i.c): Gasp (mode
// 0), Reflux (2), Djinn Counter (3), Mold and Meld (4), Shine (5), Lull (6),
// Counter-rush (7), Demon Eye (8), Emu and its kind's four other abilities
// (9), Call Dullahan and Formina Sage (10), Beat Dance (11), Element Swap
// (12), Cage (13), Clarion Cry (14) and Psy Boost (15). Mode 1 has no
// caller. Each mode loads its own sheets, then draws a canvas layer and a
// model layer every frame for the mode's length.
void Func_08173aac(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *sprites = Ram_HeapSlots->unk64[0];
  s32 *camera = Ram_HeapSlots->cameraWork;
  DrawRectangle draw;
  struct RenderModel *model;
  void *verts;
  struct ModelTexture texture;
  s32 screen[3];
  s32 other[3];
  s32 frames;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  if (mode == 0) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 35, screen, other);
    Resource_LoadAndDecompress(0x153, FIGURE(work), 1, 1);
    Resource_LoadAndDecompress(0xd8, work->sheet, 0, 0);
    REG_BG2CNT = 0x785;
    frames = 84;
  } else if (mode == 1) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 33, screen, other);
    Resource_LoadAndDecompress(0xd4, work->sheet, 1, 0);
    Resource_LoadAndDecompress(0x155, FIGURE(work), 1, 1);
    Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x148), 128);
    frames = 48;
  } else if (mode == 4) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 0x120, screen, other);
    Resource_LoadAndDecompress(0x157, FIGURE(work), 1, 0);
    Resource_LoadAndDecompress(0x156, work->sheet + 0x1000, 1, 0);
    Iwram_ClearWords(work->sheet, 0x1000);
    for (i = 0; i != 40; i++)
      for (j = 0; j != 40; j++)
        work->sheet[0x30c + i * 64 + j] = work->sheet[0x1000 + i * 40 + j];
    frames = 42;
  } else if (mode == 6) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 131, screen, other);
    Resource_LoadAndDecompress(0x153, FIGURE(work), 1, 1);
    Resource_LoadAndDecompress(0xd7, work->sheet, 0, 0);
    frames = 80;
    Sound_PlayFar(84);
  } else if (mode == 11) {
    Resource_LoadAndDecompress(0xd7, work->sheet, 0, 0);
    Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x151), 128);
    frames = 64;
  } else if (mode == 14) {
    Resource_LoadAndDecompress(0xd7, work->sheet, 0, 0);
    Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x155), 128);
    work->fadeFrames = 24;
    work->fadeStep = 0;
    frames = 64;
  } else if (mode == 13) {
    Resource_LoadAndDecompress(0xf4, work->sheet, 1, 1);
    frames = 40;
  } else if (mode == 12 || mode == 15) {
    Resource_LoadAndDecompress(0xc2, gMapCellBuffer, 0, 0);
    Resource_LoadAndDecompress(0xd6, work->sheet + 0x4000, 1, 1);
    Func_0816ae40(work->sheet + 0x4000, work->sheet, 64, 64);
    work->fadeFrames = 24;
    work->fadeStep = 0;
    frames = 48;
  } else if (mode == 5) {
    Func_0815585c(effect, effect->side << 4 | 226, screen, other);
    REG_BG2CNT = 0x785;
    Resource_LoadAndDecompress(0x151, FIGURE(work), 1, 0);
    Resource_LoadAndDecompress(0xc1, work->sheet, 0, 0);
    Resource_LoadAndDecompress(0xc9, work->sheet + 0x4000, 0, 0);
    Resource_LoadAndDecompress(0xc2, gMapCellBuffer, 0, 0);
    Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x148), 128);
    frames = 82;
  } else if (mode == 10) {
    Resource_LoadAndDecompress(0x188, gMapCellBuffer, 1, 1);
    Iwram_ClearWords(work->sheet, 0x4000);
    for (k = 0; k != 4; k++)
      for (i = 0; i != 120; i++)
        for (j = 0; j != 24; j++)
          work->sheet[4 + k * 0x1000 + i * 32 + j] =
              gMapCellBuffer[(k * 45 << 6) + i * 24 + j];
    Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x149), 128);
    frames = 60;
  } else if (mode == 7) {
    Resource_LoadAndDecompress(0xb4, work->sheet, 0, 0);
    Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x155), 128);
    frames = 76;
  } else if (mode == 8) {
    Resource_LoadAndDecompress(0xd6, work->sheet + 0x4000, 1, 1);
    Func_0816ae40(work->sheet + 0x4000, work->sheet, 64, 64);
    REG_BG2CNT = 0x785;
    frames = 16;
  } else if (mode == 9) {
    u8 *p;

    REG_BG2CNT = 0x785;
    Resource_LoadAndDecompress(0xc2, work->sheet + 0x3080, 0, 0);
    Resource_LoadAndDecompress(0x161, work->sheet, 1, 0);
    for (i = 0, p = work->sheet; i != 0x1b00; i++, p++) {
      *p <<= 1;
      if (*p > 63)
        *p = 63;
    }
    if (effect->variant == 2)
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x148), 128);
    else
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x184), 128);
    BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 10, 80);
    frames = 41;
  } else {
    if (mode == 2) {
      Func_0815585c(effect, (effect->side ^ 1) << 4 | 2, screen, other);
      frames = 76;
    } else {
      work->fadeFrames = 24;
      work->fadeStep = 0;
      frames = 70;
    }
    Resource_LoadAndDecompress(0xb4, work->sheet, 0, 0);
    Resource_LoadAndDecompress(0x151, FIGURE(work), 1, 1);
    Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x148), 128);
  }
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  if (mode == 0 || mode == 6)
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  if (mode == 2 || mode == 7 || mode == 4) {
    if (effect->side == 0)
      BattleEffect_LoadBlitter(SLOT_BLITTER, 7);
    else
      BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  } else if (mode == 3 || mode == 5) {
    if (effect->side == 1)
      BattleEffect_LoadBlitter(SLOT_BLITTER, 7);
    else
      BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  } else {
    if (effect->side == 0)
      BattleEffect_LoadBlitter(SLOT_BLITTER, 23);
    else
      BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  }
  draw = Ram_HeapSlots->unk64[1];

  for (frame = 0; frame != frames; frame++) {
    // The canvas layer.
    if (mode == 0) {
      s32 *lines = (s32 *)(work->sheet + 0x6980);

      for (i = 0; i != 160; i++)
        *lines++ = (Random16() & 31) << 4;
    } else if (mode == 6) {
      s32 *lines = (s32 *)(work->sheet + 0x6980);

      for (i = 0; i != 160; i++)
        *lines++ = (0x60000 - Trig_Sin((frame << 11) + i * 0x800) * 6) >> 10;
    } else if (mode == 1 && frame > 31) {
      u16 *pal = (u16 *)(BG_PLTT + 2);

      for (i = 0; i != 63; i++) {
        s32 r = pal[i] & 31;
        s32 g = pal[i] >> 5 & 31;
        s32 b = pal[i] >> 10 & 31;

        if (frame > 40) {
          r += 4;
          g += 4;
          b += 4;
        } else {
          r += 2;
          g += 2;
          b += 2;
        }
        if (r > 31)
          r = 31;
        if (g > 31)
          g = 31;
        if (b > 31)
          b = 31;
        pal[i] = b << 10 | g << 5 | r;
      }
      Iwram_FillWords(canvas, 0x4000, 0x01010101);
    }

    if (mode == 3) {
      struct EffectPosition from;
      struct EffectPosition to;

      EffectPosition_ProjectScaledBattler(effect->actor, &from);
      EffectPosition_ProjectScaledBattler(effect->actors[0], &to);
      from.x /= 2;
      to.x /= 2;
      if ((u32)(frame - 32) <= 8) {
        s32 n = frame - 31;
        s32 dx;
        s32 head;
        s32 tail;
        s32 step;
        s32 acc;

        if (n > 8)
          n = 8;
        dx = to.x - from.x;
        head = from.x + Data_08198c28[n] * dx / 16;
        tail = from.x + Data_08198c28[n - 1] * dx / 16;
        step = head - tail;
        for (i = 0, acc = 0; i != 4; i++, acc += step)
          draw(canvas, FIGURE(work), tail + acc / 3 - 10, from.y - 20, 20, 40);
      }
      if (frame > 37) {
        if (frame == 38) {
          for (i = 0; i != 63; i++) {
            struct EffectStep *p = &work->particles[i];
            s32 a;
            s32 s;

            p->x = to.x << 16;
            p->y = to.y << 16;
            a = Random16();
            s = (Random16() & 511) + 256;
            p->velocityX = s * Trig_Sin(a) >> 8;
            p->velocityY = s * Trig_Cos(a) >> 7;
            p->variant = (Random16() & 15) + 16;
          }
          Func_081180e8(134);
          Func_08118088(effect->actors[0], 4);
          work->shakeFrames = 8;
          Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        }
        for (i = 0; i != 63; i++) {
          struct EffectStep *p = &work->particles[i];

          if (p->variant >= 0) {
            s32 n = (p->variant >> 3) + 2;

            draw(canvas, sprites + gData_08197410[n - 1], HI(p->x) - n / 2,
                 HI(p->y) - n, n, n * 2);
            BattleEffectKernels_IntegrateVector2(p, 60, 0x400);
            p->variant--;
          }
        }
      }
    } else if (mode == 6) {
      s32 a = frame << 11;
      s32 x = (Trig_Sin(a) * 20 >> 16) + screen[0] / 2;
      s32 y = (Trig_Cos(a) * 4 >> 16) + screen[1];

      x -= 10;
      y -= 24;
      if (frame > 44)
        y = y - frame * 2 + 88;
      draw(canvas, FIGURE(work), x, y, 20, 40);
      Func_081496c8(frame, 0xaaab, 0x5555, 0);
    } else if (mode == 2 || mode == 7) {
      if (mode == 2) {
        s32 x = screen[0] / 2 - 10;
        s32 y = screen[1] - 20;
        s32 r = frame;
        s32 a;

        if (r > 12)
          r = 12;
        a = frame << 9;
        x -= r * Trig_Sin(a) * 2 >> 16;
        y -= (r * Trig_Cos(a) >> 16) - r;
        if (frame <= 61)
          draw(canvas, FIGURE(work), x, y, 20, 40);
      }
      if (frame == 0)
        Sound_PlayFar(142);
      if (frame == 62)
        Sound_PlayFar(139);
      if (frame == 0) {
        for (i = 0; i != 63; i++) {
          struct EffectStep *p = &work->particles[i];

          p->z = Random16();
          p->velocityZ = (Random16() & 63) + 64;
          Random16();
          p->variant = i + 32;
        }
      }
      {
        struct EffectPosition target;

        EffectPosition_ProjectScaledBattler(effect->actors[0], &target);
        for (i = 0; i != 48; i++) {
          struct EffectStep *p = &work->particles[i];

          if ((u32)p->variant <= 31) {
            s32 n = p->variant / 8 + 2;
            s32 x = p->velocityZ * Trig_Sin(p->z) / 0x20000 + target.x / 2;
            s32 y = (p->velocityZ * Trig_Cos(p->z) >> 16) + target.y;

            p->z += 512;
            p->velocityZ -= 4;
            if (p->velocityZ > 0)
              draw(canvas, sprites + gData_08197410[n - 1], x - n / 2, y - n, n,
                   n * 2);
          }
          p->variant--;
        }
      }
    } else if (mode == 5) {
      struct EffectStep *ball = &work->particles[63];
      struct EffectPosition pos;

      if (frame == 0) {
        ball->velocityX = other[0];
        ball->velocityY = other[1];
        ball->velocityZ = other[2];
      }
      if (frame <= 47) {
        s32 *user = *GetBattleObjectSlotFar(effect->actor);

        ball->velocityX += (user[2] - other[0]) / 40;
        ball->velocityY += (0x3c0000 - other[1]) / 40;
        ball->velocityZ += (user[4] - other[2]) / 40;
      } else {
        s32 *target = *GetBattleObjectSlotFar(effect->actors[0]);
        struct EffectStep *fall = &work->particles[62];

        if (frame == 48) {
          fall->x = 0;
          fall->y = 0;
          fall->z = 0;
          fall->velocityX = (target[2] - ball->velocityX) / 32;
          fall->velocityY = -ball->velocityY / 32;
          fall->velocityZ = (target[4] - ball->velocityZ) / 32;
        }
        ball->velocityX += fall->x;
        ball->velocityY += fall->y;
        ball->velocityZ += fall->z;
        fall->x += fall->velocityX;
        fall->y += fall->velocityY;
        fall->z += fall->velocityZ;
      }
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      EffectPosition_ProjectPoint(&ball->velocityX, &pos);
      pos.x >>= 1;
      ball->x = pos.x;
      ball->y = pos.y;
      draw(canvas, FIGURE(work), pos.x - 10, pos.y - 20, 20, 40);
      if (frame == 0)
        Sound_PlayFar(140);
      if (frame == 55) {
        work->particles[61].x = pos.x;
        work->particles[61].y = pos.y;
        Func_081180e8(145);
        Func_08118088(effect->actors[0], 4);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 24);
        work->shakeFrames = 16;
      }
      Func_08158ce0(8, 8);
    } else if (mode == 0) {
      s32 a = frame << 11;
      s32 x = (Trig_Sin(a) * 20 >> 16) + screen[0] / 2;
      s32 y = (Trig_Cos(a) * 4 >> 16) + screen[1];

      x -= 10;
      y -= 24;
      if (frame > 48)
        y = y - frame * 2 + 96;
      if (y > -40)
        draw(canvas, FIGURE(work), x, y, 20, 40);
    }

    // The model layer.
    Render_ResetTransformState();
    gCameraSceneParameters[4] = 104;
    verts = Heap_AllocEwram(0x400);
    model = Render_AllocateModelList(1);
    model->texture = &texture;
    model->vertices = verts;
    if (mode == 0) {
      if (frame == 0) {
        for (i = 0; i != 16; i++) {
          struct EffectStep *p = &work->particles[i];

          p->x = i << 15;
          p->y = 0x380000;
          p->velocityX = (Random16() & 511) + 512;
          p->velocityY = -((Random16() & 127) + 612) << 7;
          p->variant = -i * 8;
        }
      }
      if (frame == 4)
        Sound_PlayFar(147);
      if (frame == 48)
        Func_081180e8(-1);
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 7;
      model->mesh = Data_08199244;
      for (i = 0; i != 6; i++) {
        struct EffectStep *p = &work->particles[i];

        if (p->variant >= 0) {
          s32 size = 30000;
          s32 shade = -16;

          if (p->variant <= 6)
            shade = p->variant * 8 - 64;
          if (p->variant > 31) {
            shade = (32 - p->variant) * 4;
            if (shade < -64)
              continue;
          }
          model->shade = shade;
          texture.pixels = work->sheet + (i % 2 << 10);
          Render_ResetTransformState();
          p->y += p->velocityY;
          p->x += p->velocityX;
          Func_08015160((Trig_Sin(p->x) << 4) + ((screen[0] / 2 - 64) << 16),
                        p->y, 0);
          Func_080151e4(size >> 1, size, size >> 1);
          SceneTransform_ApplyPitch(-0x4000);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
        p->variant++;
      }
    } else if (mode == 6) {
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = Data_08199244;
      model->vertices = verts;
      for (k = 0; k != 1; k++) {
        if (frame >= k * 32) {
          s32 t = frame - k * 32;
          s32 scale = (t << 10) + 0x1000;
          s32 shade = 0;
          s32 half;
          s32 spin;

          if (t > 63) {
            shade = (64 - t) * 4;
            if (shade < -64)
              continue;
          }
          model->shade = shade;
          half = scale >> 1;
          Render_ResetTransformState();
          Func_080151e4(half, scale, scale);
          Func_080150e4((frame << 8) + 0xc000);
          SceneTransform_ApplyPitch((frame << 6) - 0x4770);
          spin = frame * 356;
          for (j = 0; j != 12; j++) {
            texture.pixels = work->sheet + (j % 3 << 10);
            Transform_Push();
            Func_080150e4(spin + j * 5461);
            Func_08015160(0, 0x740000, 0);
            Func_080151e4(half, half, half);
            Render_TransformVertices(gData_08199210, verts, 4);
            Transform_Pop();
            Render_DrawModelList(model);
          }
        }
      }
    } else if (mode == 1) {
      model->shade = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gData_08199340;
      model->kind = 6;
      texture.pixels = work->sheet;
      {
        s32 yaw = frame << 9;
        s32 scale = frame * 2596 >> 1;
        struct EffectPosition pos;

        for (i = 0; i != effect->count; i++) {
          EffectPosition_ProjectScaledBattler(effect->actors[i], &pos);
          Render_ResetTransformState();
          Func_08015160((pos.x / 2 - 64) << 16, (pos.y - 48) << 16, 0);
          SceneTransform_ApplyPitch(0x4000);
          SceneTransform_ApplyYaw(yaw - 0x1000);
          Func_080151e4(scale, scale, scale);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
      }
    } else if (mode == 5) {
      struct EffectPosition pos;

      if (frame <= 47) {
        struct EffectStep *ball = &work->particles[63];
        s32 twist;

        model->shade = -16;
        texture.logWidth = 5;
        texture.logHeight = 2;
        texture.pixels = work->sheet + 0x4000;
        model->texture = &texture;
        model->kind = 7;
        model->mesh = Data_08198c34;
        model->vertices = verts;
        if (frame == 0) {
          for (i = 0; i != 16; i++) {
            struct EffectStep *p = &work->particles[i];

            p->x = Random16() & 0xffff;
            p->y = (Random16() & 255) + 128;
            if (Random16() & 1)
              p->y = -p->y;
            p->variant = -i * 2;
          }
        }
        twist = frame * 2;
        for (i = 0; i != 8; i++) {
          struct EffectStep *p = &work->particles[i];

          p->variant++;
          p->x += p->y * 2;
          model->scrollV = (twist + i * 7) & 31;
          if ((u32)p->variant <= 31) {
            Render_ResetTransformState();
            Func_08015160((ball->x << 16) - 0x400000,
                          (ball->y << 16) - 0x400000, 0);
            Func_080151e4(0x8000, 0x10000, 0x10000);
            Func_080150e4(p->x);
            Func_080151e4(Trig_Sin(p->variant * 682) * 3 + 0xc000, 0x20000,
                          0x10000);
            Render_TransformVertices(gData_081991b0, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
      EffectPosition_ProjectBattler(effect->actors[0], &pos);
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = gData_08199364;
      texture.pixels = gMapCellBuffer;
      model->vertices = verts;
      for (i = 0; i != 3; i++) {
        s32 t = frame - i * 4 - 55;

        if (t >= 0) {
          s32 scale = (t * 3 << 12) + 0x4000;
          s32 shade = 0;

          if (t > 7)
            shade = (8 - t) * 8;
          model->shade = shade;
          if (shade > -64) {
            Render_ResetTransformState();
            Func_08015160((pos.x / 2 - 64) << 16, (pos.y - 64) << 16, 0);
            Func_080151e4(0x8000, 0x10000, 0x8000);
            Func_080150e4((i << 14) + 0x2000);
            SceneTransform_ApplyPitch(-0x1000);
            Func_0801521c(scale);
            Render_TransformVertices(gData_08199210, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
    } else if (mode == 4) {
      if ((u32)(frame - 16) <= 64) {
        s32 t = frame - 16;
        s32 scale;
        struct EffectPosition pos;

        if (t <= 23)
          work->transferValue = 75;
        else
          work->transferValue = 50;
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->mesh = gData_08199340;
        model->kind = 7;
        texture.pixels = work->sheet;
        EffectPosition_ProjectScaledBattler(effect->actors[0], &pos);
        scale = 0x10000 - t * 4592;
        for (j = 0; j != 2; j++, scale += 0x4000) {
          if (scale > 0) {
            s32 shade = -(j << 4);

            if (shade > 0)
              shade = 0;
            model->shade = shade;
            Render_ResetTransformState();
            Func_08015160((pos.x / 2 - 64) << 16, (pos.y - 48) << 16, 0);
            SceneTransform_ApplyPitch(0xc000);
            Func_080151e4(scale, scale, scale);
            Render_TransformVertices(gData_08199210, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame == 14)
        Sound_PlayFar(212);
      if (frame == 24)
        Func_0814cd48(effect->actors[0], 7, -1, 0, 12);
    } else if (mode == 9) {
      struct EffectPosition from;

      EffectPosition_ProjectBattler(effect->actor, &from);
      if (frame == 0) {
        struct EffectPosition to;
        struct EffectStep *ball = &work->particles[63];
        struct EffectStep *q;

        EffectPosition_ProjectScaledBattler(effect->actors[0], &to);
        ball->x = to.x;
        ball->y = to.y;
        for (i = 0; i != 32; i++)
          work->particles[i].variant = -1;
        for (i = 0, q = gData_02014000; i != 256; i++, q++) {
          s32 s = Random16() & 511;
          s32 a = Random16();

          s += 64;
          q->velocityX = s * Trig_Sin(a) >> 6;
          q->velocityY = s * Trig_Cos(a) >> 4;
          if (effect->variant == 0) {
            q->velocityX /= 4;
            q->velocityY /= 4;
          } else if (effect->variant == 1) {
            q->velocityX /= 2;
            q->velocityY /= 2;
          }
          q->x = to.x / 2 << 16;
          q->y = (to.y + 16) << 16;
          q->variant = (Random16() & 15) + 8;
        }
      }
      if (frame <= 9) {
        struct EffectStep *p = &work->particles[frame * 2];
        s32 v;

        p->x = from.x / 2 << 16;
        p->y = (from.y + 8) << 16;
        v = ((Random16() & 255) + 256) << 8;
        p->velocityX = v;
        if (effect->side == 1)
          p->velocityX = -v;
        p->velocityY = -((Random16() & 255) + 256) << 4;
        p->variant = 0;
      }
      for (i = 0; i != 32; i++) {
        struct EffectStep *p = &work->particles[i];

        if ((u32)p->variant <= 23) {
          draw(canvas, work->sheet + p->variant / 4 * 1152, HI(p->x) - 12,
               HI(p->y) - 24, 24, 48);
          BattleEffectKernels_IntegrateVector2(p, 60, -0x1000);
          p->variant++;
        }
      }
      if (frame > 9) {
        struct EffectStep *q = gData_02014000;

        for (i = 0; i != Data_08198c64[effect->variant]; i++, q++) {
          if (q->variant >= 0) {
            s32 n = q->variant / 4 + 3;

            draw(canvas, sprites + gData_08197410[n - 1], HI(q->x) - n / 2,
                 HI(q->y) - n, n, n * 2);
            BattleEffectKernels_IntegrateVector2(q, 62, 0);
            q->variant--;
          }
        }
      }
      if (frame > 9) {
        struct EffectStep *ball = &work->particles[63];
        s32 scale = ((frame - 10) << 15) + 0x4000;
        s32 shade = 0;

        if (effect->variant == 0)
          scale /= 4;
        else if (effect->variant == 1)
          scale /= 2;
        if (frame > 15)
          shade = (16 - frame) * 8;
        if (shade > -64) {
          model->shade = shade;
          texture.logWidth = 7;
          texture.logHeight = 7;
          model->mesh = gData_08199364;
          model->kind = 7;
          texture.pixels = work->sheet + 0x3080;
          Render_ResetTransformState();
          Func_08015160((ball->x / 2 - 64) << 16, (ball->y - 48) << 16, 0);
          Func_080151e4(scale / 4, scale, scale / 2);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
      if (frame == 10) {
        Func_081180e8(134);
        work->shakeFrames = effect->variant * 2 + 8;
        Func_08118088(effect->actors[0], 0);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 12);
      }
      Func_08158ce0(effect->variant * 2 + 8, effect->variant * 2 + 8);
    } else if (mode == 11) {
      if (frame == 0) {
        struct EffectPosition pos;

        EffectPosition_ProjectBattler(effect->actors[0], &pos);
        for (i = 0; i != 32; i++) {
          struct EffectStep *p = &work->particles[i];
          s32 s = Random16() & 255;
          s32 a = (Random16() & 0x7fff) + 0x4000;

          s += 128;
          p->velocityX = s * Trig_Sin(a) >> 7;
          p->velocityY = s * Trig_Cos(a) >> 6;
          p->velocityZ = 0x400;
          p->x = pos.x / 2 << 16;
          p->y = (pos.y - (Random16() & 31) + 16) << 16;
          p->z = Random16();
        }
        Sound_PlayFar(212);
        work->shakeFrames = 16;
        Func_0814cd48(effect->actors[0], 10, -1, 0, 32);
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = Data_08199244;
      model->vertices = verts;
      for (i = 0; i != 12; i++) {
        struct EffectStep *p = &work->particles[i];

        if (frame >= i * 2) {
          s32 t = frame - i * 2;
          s32 shade = 0;

          if (t > 23) {
            shade = (24 - t) * 4;
            if (shade < -64)
              continue;
          }
          model->shade = shade;
          texture.pixels = work->sheet + (i % 3 << 10);
          Render_ResetTransformState();
          Func_08015160(p->x - 0x400000, p->y - 0x400000, 0);
          Func_080151e4(0x8000, 0x10000, 0x10000);
          Func_080150e4(p->z);
          Func_0801521c(0x8000);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
          BattleEffectKernels_IntegrateVector3(p, 60, 0);
        }
      }
      Func_08158ce0(8, 8);
    } else if (mode == 14) {
      if (frame == 0) {
        struct EffectPosition pos;

        EffectPosition_ProjectBattler(effect->actor, &pos);
        for (i = 0; i != 32; i++) {
          struct EffectStep *p = &work->particles[i];
          s32 s = Random16() & 255;
          s32 a = (Random16() & 0x7fff) + 0x4000;

          s += 128;
          p->velocityX = s * Trig_Sin(a) >> 6;
          p->velocityY = s * Trig_Cos(a) >> 6;
          p->velocityZ = 0x400;
          p->x = pos.x / 2 << 16;
          p->y = (pos.y - (Random16() & 31) - 16) << 16;
          p->z = Random16();
        }
        Sound_PlayFar(619);
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = Data_08199244;
      model->vertices = verts;
      for (i = 0; i != 12; i++) {
        struct EffectStep *p = &work->particles[i];

        if (frame >= i * 2) {
          s32 t = frame - i * 2;
          s32 shade = 0;

          if (t > 23) {
            shade = (24 - t) * 4;
            if (shade < -64)
              continue;
          }
          model->shade = shade;
          texture.pixels = work->sheet + (i % 3 << 10);
          Render_ResetTransformState();
          Func_08015160(p->x - 0x400000, p->y - 0x400000, 0);
          Func_080151e4(0x8000, 0x10000, 0x10000);
          Func_080150e4(p->z);
          Func_0801521c(0x8000);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
          BattleEffectKernels_IntegrateVector3(p, 60, 0);
        }
      }
    } else if (mode == 10) {
      struct EffectPosition pos;

      if (frame == 0) {
        void *user = *GetBattleObjectSlotFar(effect->actor);

        Actor_SetMode(user, 0);
        Actor_SetMode(user, 2);
        Actor_SetAnimationSpeedFar(user, 0);
        Sound_PlayFar(212);
      }
      if (frame == 14)
        Actor_SetAnimationSpeedFar(
            *GetBattleObjectSlotFar(effect->actor), 16);
      if (frame == 22)
        Sound_PlayFar(212);
      if (frame == 26) {
        Func_081180e8(144);
        work->shakeFrames = 8;
        Func_0814cd48(effect->actors[0], 7, 5, 0, 12);
        Func_08118088(effect->actors[0], 4);
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      }
      if (frame > 27) {
        struct EffectStep *q;

        if (frame == 28) {
          EffectPosition_ProjectScaledBattler(effect->actors[0], &pos);
          for (i = 0, q = gData_02014000; i != 128; i++, q++) {
            s32 s = Random16() & 511;
            s32 a = Random16();

            s += 32;
            q->velocityX = s * Trig_Sin(a) >> 7;
            q->velocityY = s * Trig_Cos(a) >> 6;
            q->x = (pos.x / 2 << 16) + q->velocityX * 2;
            q->y = ((pos.y + 16) << 16) + q->velocityY * 2;
            q->variant = (Random16() & 15) + 16;
          }
        }
        for (i = 0, q = gData_02014000; i != 64; i++, q++) {
          if (q->variant >= 0) {
            for (j = 0; j != 4; j++) {
              s32 n = q->variant / 8 + 1;

              draw(canvas, sprites + gData_08197410[n - 1], HI(q->x) - n / 2,
                   HI(q->y) - n, n, n * 2);
              BattleEffectKernels_IntegrateVector2(q, 63, 0x800);
              if (q->y > 0x680000) {
                q->y = 0x680000;
                q->velocityY = -q->velocityY / 2;
              }
            }
            q->variant--;
          }
        }
      }
      EffectPosition_ProjectScaledBattler(effect->actor, &pos);
      texture.logWidth = 5;
      model->texture = &texture;
      model->mesh = gData_08199268;
      texture.logHeight = 7;
      model->kind = 7;
      model->vertices = verts;
      Render_ResetTransformState();
      Func_080151e4(0x8000, 0x10000, 0x10000);
      if (effect->side == 0)
        Func_08015160((pos.x - 136) << 16, (pos.y - 48) << 16, 0);
      else
        Func_08015160((pos.x - 108) << 16, (pos.y - 56) << 16, 0);
      for (j = 0; j != 4; j++) {
        s32 scale;

        Transform_Push();
        texture.pixels = work->sheet + ((Random16() & 3) << 12);
        texture.logWidth = 5;
        texture.logHeight = 7;
        if (frame > 23) {
          s32 d = frame * 16 - j * 2 - 352;
          s32 angle;

          if (d < 0)
            continue;
          if (d > 80)
            d = 80;
          if (effect->side == 0)
            Func_08015160(d * 3 << 13, d * 7 << 11, 0);
          else
            Func_08015160(-(d * 3 << 13), d * 7 << 11, 0);
          angle = d * 48152 / 80 + 0x6000;
          if (effect->side != 0)
            angle = -angle;
          Func_080150e4(angle);
        } else {
          if (j != 0)
            continue;
          Func_080150e4(effect->side == 0 ? -0x6000 : 0x6000);
        }
        model->shade = -j << 3;
        if (frame <= 23)
          scale = frame * 2730;
        else if (frame > 31)
          scale = 0x40000 - (frame * 3 << 11);
        else
          scale = 0x10000;
        if (scale > 0) {
          Func_080151e4(scale * 6, scale * 2, 0x10000);
          Render_TransformVertices(Data_081991d0, verts, 4);
          Render_DrawModelList(model);
          Transform_Pop();
        }
      }
      Func_08158ce0(8, 8);
    } else if (mode == 8) {
      struct EffectPosition pos;

      work->transferValue = 50;
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->mesh = gData_08199364;
      model->kind = 6;
      work->transferMode = 1;
      work->transferValue = 0;
      texture.pixels = work->sheet;
      EffectPosition_ProjectScaledBattler(effect->actor, &pos);
      if (frame <= 15) {
        for (j = 0; j != 2; j++) {
          s32 scale = Trig_Sin(frame << 11) * 5;

          if (scale > 0) {
            model->shade = 0;
            Render_ResetTransformState();
            Func_08015160((pos.x / 2 + j * 16 - 72) << 16, (pos.y - 64) << 16,
                          0);
            Func_080151e4(scale / 2, scale, scale / 2);
            Func_080150e4(0x2000);
            Render_TransformVertices(Data_081991e0, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame == 8) {
        Sound_PlayFar(212);
        work->shakeFrames = 8;
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        for (i = 0; i != effect->count; i++)
          Func_0814cd48(effect->actors[i], 7, 5, i, 7);
      }
      Func_08158ce0(8, 8);
    } else if (mode == 13) {
      struct EffectPosition pos;
      s32 shade;
      s32 spin;
      s32 wobble;

      if (frame == 0)
        Sound_PlayFar(104);
      EffectPosition_ProjectBattler(effect->actors[0], &pos);
      shade = frame * 4 - 64;
      if (shade >= -16)
        shade = -16;
      model->shade = shade;
      model->texture = &texture;
      model->kind = 6;
      model->vertices = verts;
      model->mesh = Data_08198ec4;
      texture.pixels = work->sheet;
      texture.logWidth = 7;
      texture.logHeight = 4;
      if (frame == 26) {
        work->shakeFrames = 8;
        Func_081180e8(134);
      }
      spin = frame << 9;
      wobble = frame;
      for (j = 0; j != 8; j++, spin += 0x2000, wobble += 5) {
        s32 scale = 0x20000;

        if (frame >= j * 2 + 16)
          scale = 0x20000 - ((frame - (j * 2 + 16)) << 13);
        if (scale > 0x8000) {
          model->scrollU = (wobble & 7) << 4;
          Render_ResetTransformState();
          Func_08015160((pos.x / 2 - 60) << 16, 0x80000, 0);
          Func_080151e4(scale >> 1, scale, 0x10000);
          SceneTransform_ApplyPitch((j << 13) + 4000);
          SceneTransform_ApplyYaw(frame << 9);
          Func_080150e4(spin);
          Func_080151e4(0x10000, 0x8000, 0x10000);
          Render_TransformVertices(Data_08198cac, verts, 32);
          Render_DrawModelList(model);
        }
      }
    } else if (mode == 12) {
      struct EffectPosition pos;

      texture.logWidth = 7;
      texture.logHeight = 7;
      model->mesh = gData_08199364;
      model->kind = 6;
      texture.pixels = gMapCellBuffer;
      EffectPosition_ProjectScaledBattler(effect->actor, &pos);
      Render_ResetTransformState();
      Func_08015160((pos.x / 2 - 64) << 16, (pos.y - 48) << 16, 0);
      if (frame <= 15) {
        s32 scale = 0x30000 - (frame * 3 << 12);

        model->shade = 0;
        Func_080151e4(scale >> 1, scale, scale >> 1);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
      Render_ResetTransformState();
      Func_08015160((pos.x / 2 - 64) << 16, (pos.y - 48) << 16, 0);
      if ((u32)(frame - 12) <= 31) {
        s32 scale = Trig_Sin((frame << 10) - 0x3000) * 3;

        texture.pixels = work->sheet;
        Func_080151e4(scale / 2, scale, scale / 2);
        Func_080150e4((frame << 9) + 0x2000);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
      if (frame == 0) {
        for (i = 0; i != 64; i++) {
          struct EffectStep *p = &work->particles[i];

          Random16();
          p->x = 768 + i * 24;
          p->velocityX = -p->x / 48;
          p->y = Random16();
        }
        Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x184), 128);
      }
      if (frame <= 15) {
        for (i = 0; i != 64; i++) {
          struct EffectStep *p = &work->particles[i];

          for (j = 0; j != 3; j++) {
            s32 x = (p->x * Trig_Sin(p->y) >> 21) + pos.x / 2;
            s32 y = p->x * Trig_Cos(p->y) >> 20;

            draw(canvas, sprites + gData_08197410[3], x - 2, y + 56, 4, 8);
            p->x += p->velocityX;
          }
        }
      }
      for (j = 0; j != 3; j++) {
        if (frame > 7 && frame <= 15)
          Func_0815f0a0((s32)&ResourceId_Data154);
        else if (frame > 15 && frame <= 23)
          Func_0815f0a0((s32)&ResourceId_Data150);
        else if (frame > 23)
          Func_0815f0a0((s32)&ResourceId_Data152);
      }
      if (frame == 16) {
        Sound_PlayFar(212);
        Func_0814cd48(effect->actors[0], 7, -1, 0, 30);
      }
      if (frame == 28)
        Func_081180e8(-1);
    } else if (mode == 15) {
      struct EffectPosition pos;

      texture.logWidth = 7;
      model->mesh = gData_08199364;
      texture.logHeight = 7;
      texture.pixels = gMapCellBuffer;
      model->kind = 6;
      EffectPosition_ProjectScaledBattler(effect->actor, &pos);
      for (j = 0; j != 2; j++) {
        s32 t = frame - j * 8;

        Render_ResetTransformState();
        Func_08015160((pos.x / 2 - 64) << 16, (pos.y - 48) << 16, 0);
        if ((u32)t <= 15) {
          s32 scale = 0x40000 - (t << 14);
          s32 shade = 0;

          if (t <= 7)
            shade = t * 8 - 64;
          model->shade = shade;
          Func_080151e4(scale / 2, scale, scale / 2);
          Func_080150e4((j << 14) + 0x2000);
          SceneTransform_ApplyPitch(-0x1000);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
      }
      Render_ResetTransformState();
      Func_08015160((pos.x / 2 - 64) << 16, (pos.y - 48) << 16, 0);
      if ((u32)(frame - 12) <= 31) {
        s32 scale = Trig_Sin((frame << 10) - 0x3000) * 2;

        texture.pixels = work->sheet;
        Func_080151e4(scale / 2, scale, scale / 2);
        Func_080150e4((frame << 9) + 0x2000);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
      if (frame == 0)
        Graphics_LoadGreyRamp();
      if (frame == 16) {
        Sound_PlayFar(212);
        Func_0814cd48(effect->actors[0], 7, -1, 0, 30);
      }
      if (frame == 28)
        Func_081180e8(-1);
    } else {
      s32 scale;

      model->shade = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gData_08199340;
      model->kind = 7;
      texture.pixels = work->sheet;
      if (mode == 2 || mode == 7) {
        if (frame <= 31)
          scale = frame * 1500;
        else if (frame <= 59)
          scale = 48000;
        else
          scale = 228000 - frame * 2999;
      } else {
        if (frame <= 31)
          scale = frame * 1500;
        else if (frame <= 39)
          scale = 48000;
        else
          scale = 168000 - frame * 2999;
      }
      if (scale > 0) {
        struct EffectPosition pos;

        if (mode == 2 || mode == 7)
          EffectPosition_ProjectScaledBattler(effect->actors[0], &pos);
        else
          EffectPosition_ProjectScaledBattler(effect->actor, &pos);
        Render_ResetTransformState();
        Func_08015160((pos.x / 2 - 64) << 16, (pos.y - 48) << 16, 0);
        SceneTransform_ApplyPitch(0x4000);
        Func_080151e4(scale / 3, scale, scale);
        if (mode == 2)
          SceneTransform_ApplyYaw(-frame << 10);
        else
          SceneTransform_ApplyYaw(frame << 10);
        Render_TransformVertices(gData_08199210, verts, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    BattleEffect_CountDownBattlerTints();
    if (mode == 1)
      WAIT_TRANSFER(work) {}
    work->transferPending = 1;
    WaitFrames(1);
  }
  HeapSlot_Release(SLOT_BLITTER);
  if (mode == 0 || mode == 6)
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
