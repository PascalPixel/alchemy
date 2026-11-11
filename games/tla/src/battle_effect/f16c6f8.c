#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
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
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_LoadBlitter(s32 slot,
                              s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void Func_081180e8(s32 phase);                      // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
s32 Task_Add(void *func, s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#include "bg_scroll.h"
#include "io_write_queue.h"
#include "irq_lock.h"

void Actor_SetAnimationSpeedFar(void *object, s32 value);
void BattleBackground_LoadFar(s32 layer, s32 resourceId, s32 mode);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void Func_0818de3c(void *triangles, s32 columns, s32 rows, s32 cellWidth,
                   s32 cellHeight); // builds a grid's mesh
void Func_08143354(void);
void BattleEffect_ConfigureEffectDisplay(void);
void Display_ApplyBg2Affine(void);
void BattleEffect_ProcessPendingBlock2Transfer(void);

extern char ResourceId_VenusDjinnSmallSheet;
extern char ResourceId_Data0DA;
extern char ResourceId_OrangePaletteB;
extern u8 gMapCellBuffer[];
extern const u8 gData_081990d0[];
extern const u8 Data_08198b52[]; // each ring's start
extern const u8 Data_08198b56[]; // growth
extern const s8 Data_08198b5a[]; // height
extern const u8 Data_08199050[];

struct ModelVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 unk03;
};

#define DOME_ROWS 16
#define DOME_COLUMNS 16

// Abilities: Geode.
//
// The Venus Djinni tears up the battlefield itself: the screen is copied
// into a texture and peeled up into a spinning dome over the target as the
// palette fades, the Djinni flies across to it, and at frame 48 the dome
// slams down in rings of dust before the backdrop fades back in.
void BattleEffect_RunGeode(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: blit[1], the second of The Broken Seal's pair of blitters, gives
  // the stack frame its 4 bytes at sp 72 (differ 688 as one DrawRectangle).
  DrawRectangle blit[2];
  s32 *transition;
  u8 *projected;
  struct ModelVertex *vertices;
  u8 *mesh;
  s32 row;
  s32 curl;
  s32 pitch;
  u8 *battle;
  u8 *picture;
  void *caster;
  s32 screenB[3];
  s32 screenA[3];
  struct EffectPosition source;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct ModelTexture dust;
  struct RenderModel *model;
  struct RenderModel *rings;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  BattleEffect_StartCanvasLayer(2);
  Func_0815585c(effect, (effect->side << 4) | 0xc0, screenA, screenB);
  BattleEffect_ConfigureEffectDisplay();
  REG_WININ = 0x3537;
  REG_WINOUT = 0x3f31;
  transition[4] = 1;
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER,
        BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                             BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                        COMPOSITE_CLIP_Y |
                                                        COMPOSITE_FLIP_X));
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc8f);
  Task_Add(Display_ApplyBg2Affine, 0xcff);
  projected = work->sheet;
  vertices = (struct ModelVertex *)(work->sheet + 0x800);
  mesh = work->sheet + 0xc00;
  pitch = 0;
  battle = (u8 *)Ram_HeapSlots->battleWork;
  caster = *GetBattleObjectSlotFar(effect->actor);
  gBgScroll[1].x = -56;
  // The battlefield's picture, later the dome's texture.
  picture = gMapCellBuffer;
  {
    u8 *cell = picture;
    s32 x;

    // Take the battlefield's picture off BG2's tiles, row by row.
    for (i = 0; i <= 119; i++) {
      for (x = 0; x <= 2047; x += 64) {
        Iwram_CopyWords(
            cell, (u8 *)0x06008000 + (((i / 8) << 8) + (i & 7)) * 8 + x, 8);
        cell += 8;
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                             work->sheet + 0x65c0, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, work->sheet + 0x4710, 0,
                             0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
  Actor_SetAnimationSpeedFar(caster, 48);
  Func_0818de3c(mesh, DOME_COLUMNS, DOME_ROWS, 16, 8);

  for (frame = 0; frame != 74; frame++) {
    EffectPosition_ProjectBattler(effect->actor, &source);
    EffectPosition_ProjectBattler(effect->actors[0], &target);

    // The backdrop's palette fades toward the battle's.
    if (frame <= 11) {
      u16 *from = (u16 *)(battle + 0x56c);
      u16 *to = (u16 *)(BG_PLTT + 96);

      for (i = 0; i != 128; i++, from++, to++) {
        s32 red = *from & 31;
        s32 green = (*from >> 5) & 31;
        s32 blue = (*from >> 10) & 31;
        s32 r = *to & 31;
        s32 g = (*to >> 5) & 31;
        s32 b = (*to >> 10) & 31;

        if (r > red + 3) {
          r -= 4;
        } else {
          r = red;
        }
        if (g > green + 3) {
          g -= 4;
        } else {
          g = green;
        }
        if (b > blue + 3) {
          b -= 4;
        } else {
          b = blue;
        }
        *to = (b << 10) | (g << 5) | r;
      }
    }
    if (frame == 1) {
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
      Func_08143354();
    }
    if (frame == 34) {
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      Task_Add(BattleEffect_ProcessPendingBlock2Transfer, 0xc8f);
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
      Iwram_ClearWords(canvas, 0x3c00);
      gBgScroll[1].y = 78;
      gBgScroll[1].x = 64 - source.x;
      REG_BG1CNT &= 0xfffc;
      REG_BG1CNT |= 1;
      REG_BG2CNT = 0x784;
    }
    if (frame == 35) {
      Task_Remove(Display_ApplyBg2Affine);
      Task_Remove(BattleEffect_ProcessPendingBlock2Transfer);
      Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc8f);
      IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
      IO_WRITE_QUEUE_PUSH(0x0400002c, -0x1000, 0x30000);
      IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
      IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f44, 0x20000);
    }
    if (frame == 18) {
      Func_0814cd48(effect->actor, -1, 2, 0, -1);
    }
    if (frame == 36) {
      work->transferMode = 2;
      work->transferValue = 50;
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
    }
    if ((u32)(frame - 34) <= 5) {
      if (effect->side == 0) {
        REG_WIN0H = 0x20f0;
        REG_WIN1H = 0x20f0;
      } else {
        REG_WIN0H = 0xd0;
        REG_WIN1H = 0xd0;
      }
    } else if (frame == 40) {
      REG_WIN0H = 0xf0;
      REG_WIN1H = 0xf0;
    }

    // The Djinni flies to the target.
    if ((u32)(frame - 36) <= 25) {
      s32 t = frame - 36;
      s32 x;
      s32 y;

      if (t > 12) {
        t = 12;
      }
      x = source.x + (target.x - source.x) * t / 12;
      y = (t << 6) / 12 + 16;
      gBgScroll[1].x = 64 - x;
      gBgScroll[1].y = t * -46 / 12 + 78;
      if (effect->side == 0) {
        x = x / 2 + 20;
      } else {
        x = x / 2 - 20;
      }
      blit[0](canvas, work->sheet + 0x65c0, x - 10, y - 48, 20, 40);
      blit[0](canvas, work->sheet + 0x65c0, x - 10, y - 48, 20, 40);
    }

    // The battlefield peels up into a dome.
    if ((u32)frame <= 34) {
      s32 size;
      s32 centerX;
      s32 centerY;
      s32 height;
      s32 angle;

      curl = frame;
      if (curl > 16) {
        curl = 16;
      }
      size = 64 - curl * 2;
      // DEAD: the ROM sets these defaults and tests frame >= 0, which inside
      // this range is always true, so the defaults are always overwritten.
      centerX = 120;
      centerY = -16;
      if (frame >= 0) {
        s32 f = frame;

        if (f > 16) {
          f = 16;
        }
        centerX = (source.x - 120) * f / 16 + 120;
        centerY = 46 * f / 16 - 16;
      }
      work->bg2x = (64 - (centerX << 5) / size) << 8;
      work->bg2y = centerY << 8;
      {
        s32 ratio = 0x2000 / size;

        // DEAD: the ROM reads each scale before storing it and drops the value
        // (as plain stores, 4 of 1139 differ).
        *(volatile u16 *)&work->bg2pa;
        work->bg2pa = ratio;
        *(volatile u16 *)&work->bg2pd;
        work->bg2pd = 0x100;
      }
      for (row = 0, height = -64, angle = 0; row != DOME_ROWS;
           height += 8, angle += 2184, row++) {
        s32 lift = ((-Trig_Cos(angle) * 3) << 4) >> 16;
        s32 reach = (Trig_Sin(angle) * 48) >> 16;
        s32 column;
        s32 y;
        s32 radius;
        y = (lift - height) * curl / 16 + height;
        radius = (reach - 56) * curl / 16 + 56;
        for (column = 0; column != DOME_COLUMNS; column++) {
          vertices[row * DOME_COLUMNS + column].x =
              (Trig_Sin(column << 12) * radius) >> 16;
          vertices[row * DOME_COLUMNS + column].y = y;
          vertices[row * DOME_COLUMNS + column].z =
              (Trig_Cos(column << 12) * radius) >> 16;
        }
      }
      model = Render_AllocateModelList(1);
      texture.logWidth = 8;
      texture.logHeight = 7;
      texture.pixels = picture;
      model->unk04 = 3;
      model->kind = 8;
      model->texture = &texture;
      model->mesh = mesh;
      model->vertices = projected;
      model->scrollU = 0;
      model->scrollV = 0;
      Render_ResetTransformState();
      if (frame > 7) {
        pitch += Trig_Sin(frame * 924 - 7392) >> 5;
        SceneTransform_ApplyPitch(pitch * 3 / 2);
      }
      SceneTransform_ApplyYaw(frame << 9);
      Render_TransformVertices(vertices, projected, 256);
      Render_DrawModelList(model);
      Heap_Release(model);
    }
    if (frame == 48) {
      Sound_PlayFar(145);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 64) {
      Func_081180e8(134);
    }

    // Rings of dust.
    if (frame > 47) {
      void *ring = Heap_AllocEwram(0x200);

      rings = Render_AllocateModelList(1);
      dust.logWidth = 6;
      dust.logHeight = 6;
      dust.pixels = work->sheet + 0x4710;
      rings->kind = 7;
      rings->texture = &dust;
      rings->mesh = gData_081990d0;
      rings->vertices = ring;
      rings->scrollU = 0;
      rings->scrollV = 0;
      for (i = 0; i != 4; i++) {
        s32 start = Data_08198b52[i] + 48;
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (((frame - start) * Data_08198b56[i] * 125) << 4) + 0x8000;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        rings->shade = shade;
        Render_ResetTransformState();
        Func_08015160((target.x / 2 - 64) << 16, Data_08198b5a[i] << 16, 0);
        Func_080151e4(scale >> 1, scale << 2, scale >> 1);
        SceneTransform_ApplyPitch(1000);
        if (i & 1) {
          SceneTransform_ApplyYaw((i * 4 + frame) << 11);
        } else {
          SceneTransform_ApplyYaw((i * 4 - frame) << 11);
        }
        Render_TransformVertices(Data_08199050, ring, 32);
        Render_DrawModelList(rings);
      }
      Heap_Release(rings);
      Heap_Release(ring);
    }
    if (frame == 61) {
      transition[4] = 0;
      Iwram_FillWords((void *)(BG_PLTT + 96), 256, 0);
    }
    if (frame == 62) {
      BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), -1);
    }
    if ((u32)(frame - 63) <= 4) {
      *(s32 *)(battle + 0x66c) = (frame << 13) - 0x7c000;
      ColorBuffer_ScaleFar(battle + 0x56c, (void *)(BG_PLTT + 96),
                           *(s32 *)(battle + 0x66c), 128);
    }
    if (frame == 48) {
      work->shakeFrames = 16;
    }
    if (frame > 47) {
      BattleCamera_Shake(8, 8);
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Actor_SetAnimationSpeedFar(caster, 16);
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
