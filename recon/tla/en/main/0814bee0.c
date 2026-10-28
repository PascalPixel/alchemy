// Draft of BattleEffect_RunZagan, decompiled from the listing: differ 700 of 1150
// (draft 1134), the same 59 calls and control flow; allocation differs.

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
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
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

#include "bg_scroll.h"
#include "input.h"

void Func_08179e6c(void);
void Animation_ApplyChildArgumentFar(void *object, s32 animation);
void ResourceObject_ReleaseFar(void *object);

extern char ResourceId_Data134;
extern char ResourceId_PetalSheet;
extern char ResourceId_CrescentRowSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data0C1;
extern u32 gCameraSceneParameters[];
extern u8 gData_02014000[];
extern const u16 gData_08197410[];
extern const s32 Data_08196e0c[2];
extern const u16 Data_08197a38[][2]; // the charging Zagan's parts
extern const u16 Data_08197a64[][2]; // the galloping Zagan's
extern const u8 Data_08197a94[];     // each ring's start
extern const u8 Data_08197a98[];     // growth
extern const s8 Data_08197a9c[];     // height
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];

#define ZAGAN_PARTS 12

// Abilities: Zagan.
//
// The screen slides over to a white field and petals blow across it; Zagan
// (twelve objects) gallops in from the right as the backdrop darkens, rears,
// and at frame 157 swings its axe across the targets, striking four times
// while the screen shakes and rings of dust spread. Holding A or B draws
// only every other frame.
void BattleEffect_RunZagan(struct BattleEffectArgument *effect) {
  u8 *battle;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  s32 *transition;
  u16 savedScroll;
  DrawRectangle blitters[2];
  s32 request[2];
  s32 placement[4];
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct EffectStep *p;
  s32 skip;
  s32 x;
  s32 y;
  s32 velocityX;
  s32 velocityY;
  s32 scroll;
  s32 frame;
  s32 i;
  s32 j;

  battle = (u8 *)Ram_HeapSlots->battleWork;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  savedScroll = gBgScroll[1].x;
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(0);
  Func_08179e6c();
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The screen slides over to the field.
  for (frame = 0, scroll = -80; frame != 28; frame++) {
    if (frame == 27) {
      gCameraSceneParameters[4] = 240;
      BattleEffect_SelectLivingTargets(work->effect);
      work->scrollStepX = -8;
      transition[4] = 1;
      Iwram_FillWords((void *)(BG_PLTT + 0xc0), 256, 0x7fff7fff);
      Resource_LoadAndDecompress((s32)&ResourceId_Data134, work->sheet, 0, 0);
      // Ten ever-fainter copies of one mote.
      for (i = 0; i != 10; i++) {
        u8 *cell = sprites + i * 32;

        for (j = 0; j != 32; j++) {
          s32 value = work->sheet[gData_08197410[3] + j];

          if (value != 0) {
            value -= i * 7;
            if (value <= 0) {
              value = 1;
            }
          }
          *cell++ = value;
        }
      }
      Resource_LoadAndDecompress((s32)&ResourceId_PetalSheet, work->sheet, 1,
                                 1);
      for (j = 0; j != 32; j++) {
        work->sheet[0x5dc0 + j] = j + 16;
        work->sheet[0x5de0 + j] = j + 16;
        work->sheet[0x5e00 + j] = j + 16;
      }
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    WaitFrames(1);
  }
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  work->transferMode = 2;
  work->transferValue = 50;
  skip = 0;
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  BattleEffect_SelectLivingTargets(work->effect);
  WaitFrames(1);
  BattleEffect_SpawnObjects(ZAGAN_PARTS, 658, 1);
  REG_BG2X = skip;
  REG_BG2PA = 0x80;

  for (frame = 0; frame != 200; frame++) {
    if (gInput.repeat & 3) {
      skip = 1;
    }
    if (frame == 0) {
      Sound_PlayFar(212);
    }
    if (frame == 32) {
      Sound_PlayFar(191);
    }
    if ((u32)(frame - 78) <= 31) {
      REG_BLDALPHA = (16 - (frame - 78) / 2) | 0x1000;
    }
    if (frame == 140) {
      REG_BLDALPHA = 0x1010;
    }
    if (frame == 157) {
      Func_081180e8(145);
      goto strike;
    }
    if (frame == 161 || frame == 165 || frame == 169) {
    strike:
      for (i = 0; i != effect->count; i++) {
        Func_08118088(effect->actors[i], 4);
        Func_0814cd48(effect->actors[i], 7, -1, i, 4);
      }
    }
    if (frame == 0) {
      transition[4] = 1;
      x = 240 << 16;
      y = 0x400000;
      velocityX = -0x140000;
      velocityY = 0;
      for (i = 0; i != ZAGAN_PARTS; i++) {
        Animation_ApplyChildArgumentFar(work->objects[i], i + 12);
      }
      REG_BLDALPHA = 0x1010;
      Iwram_FillWords((void *)(BG_PLTT + 0xc0), 256, 0x7fff7fff);
      work->scrollStepX = -8;
      gCameraSceneParameters[3] = -968;
      gCameraSceneParameters[4] = 120;
      gBgScroll[1].x = savedScroll + 1128;
      Resource_LoadAndDecompress((s32)&ResourceId_PetalSheet, work->sheet, 1,
                                 1);
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
      work->transferMode = 2;
      work->transferValue = 50;
    }
    gCameraSceneParameters[3] -= work->scrollStepX;
    gBgScroll[1].x += work->scrollStepX;

    // The backdrop darkens toward the battle's palette.
    if (!(frame & 1) && (u32)(frame - 32) <= 111) {
      u16 *from = Resource_GetTableEntry(*(u16 *)(battle + 0x670));
      u16 *to = (u16 *)(BG_PLTT + 0xc0);

      for (i = 0; i != 128; i++, from++, to++) {
        s32 b = (*to >> 10) & 31;
        s32 g = (*to >> 5) & 31;
        s32 r = *to & 31;
        s32 blue = ((*from >> 10) & 31) - 2;
        s32 green = ((*from >> 5) & 31) - 10;
        s32 red = (*from & 31) - 6;

        if (blue < 0) {
          blue = 0;
        }
        if (green < 0) {
          green = 0;
        }
        if (red < 0) {
          red = 0;
        }
        if (b > blue) {
          b--;
        }
        if (g > green) {
          g--;
        }
        if (r > red) {
          r--;
        }
        *to = (b << 10) | (g << 5) | r;
      }
    }
    if ((u32)(frame - 64) <= 35) {
      velocityX += 0x8000;
      velocityY += -0x1000;
    }
    if (frame > 177) {
      velocityX += -0x20000;
      velocityY += -0x10000;
    } else if (frame > 139) {
      velocityX += -40000;
      velocityY += 0x2000;
    } else if (frame > 99) {
      velocityX += -0x2000;
      velocityY += 0x800;
    }
    if (frame == 156) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
    }
    if (frame == 157) {
      Resource_LoadAndDecompress((s32)&ResourceId_CrescentRowSheet, work->sheet,
                                 1, 0);
      work->shakeFrames = 16;
      for (i = 0; i != ZAGAN_PARTS - 1; i++) {
        Animation_ApplyChildArgumentFar(work->objects[i], i + 24);
      }
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if ((u32)(frame - 156) <= 7) {
      blitters[0](canvas, work->sheet, 12, 0, 58, 120);
    }
    for (i = 0, j = 114; i != 8; i++, j += 6) {
      if (frame == j) {
        work->scrollStepX++;
      }
    }
    if (frame == 163) {
      gBgScroll[1].x += -20;
    }
    if (frame == 167) {
      gBgScroll[1].x += -10;
    }
    if (frame == 171) {
      gBgScroll[1].x += -10;
    }

    // Petals on the wind.
    if (frame == 0) {
      s32 row = 3;

      for (i = 0, p = work->particles; i != 20; i++, p++, row += 6) {
        p->x = (Random16() & 127) - 160;
        p->y = row;
        p->variant = Random16() & 1;
        p->velocityX = (Random16() & 3) + p->variant * 8 + 8;
      }
    }
    if ((u32)frame <= 119) {
      s32 odd = frame & 1;

      for (i = 0, p = work->particles; i != 20; i++, p++) {
        if (!odd || !skip) {
          blitters[0](canvas, work->sheet + 0x5dc0, p->x, p->y, 32,
                      p->variant + 1);
        }
        p->x += p->velocityX;
        if (p->x > 128) {
          p->x -= 160;
        }
      }
    }
    if (frame > 195) {
      goto shake;
    }

    // Zagan.
    request[0] = Data_08196e0c[0];
    request[1] = Data_08196e0c[1];
    placement[3] = 0;
    placement[1] = 0;
    if (!(frame & 1) || !skip) {
      if (frame > 156) {
        for (i = 0; i != ZAGAN_PARTS - 1; i++) {
          placement[0] = (Data_08197a38[i][0] << 16) + x - 0x200000;
          placement[2] = (Data_08197a38[i][1] << 16) + y + 0x100000;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            request, 0);
        }
      } else {
        for (i = 0; i != ZAGAN_PARTS; i++) {
          placement[0] = (Data_08197a64[i][0] << 16) + x;
          placement[2] = (Data_08197a64[i][1] << 16) + y - 0x100000;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            request, 0);
        }
      }
    }
    if (frame > 177) {
      x += velocityX;
      y += velocityY;
      velocityX = velocityX * 63 / 64;
      velocityY = velocityY * 63 / 64;
    } else if (frame > 149) {
      if (y > 0x37ffff) {
        velocityX = 0;
        velocityY = 0;
      }
      x += velocityX;
      y += velocityY;
    } else if (frame <= 59) {
      x += velocityX;
      y += velocityY;
      velocityX = velocityX * 58 / 64;
      velocityY = velocityY * 62 / 64;
    } else {
      y += velocityY;
      x += velocityX;
      velocityX = velocityX * 58 / 64;
      velocityY = velocityY * 61 / 64;
    }
    if ((!(frame & 1) || !skip) && frame <= 119) {
      blitters[1](canvas, work->sheet + ((frame / 3) & 3) * 6000,
                  (x / 2 >> 16) - 12, (y >> 16) - 58, 60, 100);
    }
    if (frame == 157) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, gData_02014000, 0,
                                 0);
    }
    if ((frame & 1) && skip) {
      goto shake;
    }

    // Rings of dust from the blow.
    if (frame <= 156) {
      goto next;
    }
    {
      s32 k;

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = gData_02014000;
      model->texture = &texture;
      model->kind = 7;
      model->mesh = Data_08198ec4;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;
      for (k = 0; k != 4; k++) {
        s32 start = Data_08197a94[k] + 157;
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (((frame - start) * Data_08197a98[k] * 125) << 4) + 0x8000;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160(-0x100000, Data_08197a9c[k] << 16, 0);
        Func_080151e4(scale, scale * 4 / 3, scale);
        SceneTransform_ApplyPitch(3000);
        model->scrollU = ((frame + k * 5) & 7) << 4;
        Render_TransformVertices(Data_08198cac, vertices, 32);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
  shake:
    if (frame > 156) {
      if (work->shakeFrames > 0) {
        s32 dx = Random16() & 3;
        s32 dy = Random16() & 15;

        gBgScroll[1].y = dy + 24;
        gCameraSceneParameters[3] = 120 - (dx - 8);
        gCameraSceneParameters[4] = 120 - (dy - 8);
        work->shakeFrames--;
      } else {
        gBgScroll[1].y = work->savedBg1Y;
        gCameraSceneParameters[3] = 120;
        gCameraSceneParameters[4] = 120;
      }
    }
  next:
    BattleEffect_CountDownBattlerTints();
    if (!(frame & 1) || !skip) {
      work->transferPending = 1;
      WaitFrames(1);
    }
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  transition[4] = 0;
  for (i = 0; i != ZAGAN_PARTS; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}
