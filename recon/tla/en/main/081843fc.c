// Draft of BattleEffect_RunCatastrophe, decompiled from the listing: differ 4844 of
// 4580 (draft 4586), the same 321 calls and control flow; allocation differs.

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

void BattleEffect_BeginCanvasLayer(s32 bgControl);
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
void Func_081180e8(s32 phase);             // BattleEventRuntime_BeginPhaseFar
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
void BattleEffect_BeginTiledCanvas(s32 bgControl);
void BattleEffect_BeginTiledCanvasFilled(s32 bgControl);
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

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct WordPair {
  s32 a;
  s32 b;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern u8 Data_02016000[];
extern struct EffectStep Data_02016d00[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196ef8;
extern const u16 gData_08197410[];
extern const u16 gData_08197486[];
extern const u8 gData_08197492[];
extern const u8 gData_08197498[];
extern const u8 gData_081991b0[];
extern const u8 Data_081991d0[];
extern const u8 Data_081991e0[];
extern const u8 gData_081991f0[];
extern const u8 gData_08199268[];
extern const u8 Data_0819931c[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];
extern const u16 Data_08199768[];
extern const u16 Data_0819978c[];
extern const u8 Data_081997b0[];
extern const u8 Data_081997cc[];
extern const u8 Data_081997e8[];
extern const u16 Data_081997f4[];
extern const u8 Data_0819980a[];
extern const u8 Data_08199810[];
extern const u16 Data_08199816[];
extern const u8 Data_08199824[];
extern const u8 Data_0819982b[];
extern const u8 Data_08199832[];
extern const u16 Data_0819983a[];
extern const u8 Data_0819985c[];
extern const u8 Data_08199880[];
extern const u16 Data_081998a4[];
extern const u8 Data_081998f2[];
extern const s16 Data_08199916[];
extern const s16 Data_0819995e[];
extern const s16 Data_081999a6[];
extern const u8 Data_081999ee[];
extern const s16 Data_081999f2[];

s32 Resource_ResetEntry(u32 entry);
void VramBlock_Reset93Far(void);
void Func_08020388(void);
s32 VramCache_AllocEntry(s32 size);
void Transform_Push(void);
void Transform_Pop(void);
u8 *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
u8 *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void);
void Func_08163c2c(s32 a, s32 b);
void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void Resource_DecodeType01(const void *source, void *destination);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_0815f0a0(s32 resource); // BattleEffect_StepPaletteToResource
void Camera_ApplyVerticalShake(s32 range);
void Func_08164a4c(s32 red, s32 green, s32 blue); // Palette_BrightenBgEntries
void Func_08164b2c(s32 r, s32 g, s32 b);
void Func_08196404(s32 slot, s32 width, s32 height, s32 mode, s32 flags);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void MemFree(void *buffer);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[((u8 *)(obj))[16]].tile)
#define HALF_HI(v) (((s16 *)&(v))[1])
// Sprites outside this projected band are not placed.
#define ON_SCREEN(z) ((z) > 0xfffff && (z) <= 0xa80000)

// The first phase's loop test: fades the screen in over its first frames and
// stops when a button skips the scene.
static inline s32 Catastrophe_FadeInAndPoll(s32 frame) {
  if (frame <= 15) {
    s32 v = frame * 2 - 30;

    Func_08164b2c(v, v, v);
  }
  return !(gInput.repeat & 3);
}

// Catastrophe: a black moon rises over a burning city, its towers crack and
// fall in fire, the sky tears open, and a meteor of shadow crushes the foes.
void BattleEffect_RunCatastrophe(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u16 savedScroll;
  s32 *transition;
  u8 *sprites;
  u8 *battle;
  DrawRectangle blit;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;
  s32 camZ;
  s32 swapped;
  s32 drift;
  s32 sway;
  s32 cueTimer;
  s32 cueIndex;
  s32 shade;
  s32 spinA;
  s32 spinB;
  s32 rise;
  s32 phase;
  s32 slide;
  s32 slideStep;
  s32 place[4];
  s32 apply[2];
  struct WordPair req;
  s16 list[2];
  struct EffectPosition pos;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct ModelTexture texture3;
  struct RenderModel *model;
  void *verts;
  struct EffectStep *p;
  struct EffectStep *q;
  u8 *obj;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  savedScroll = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  battle = Ram_HeapSlots->battleWork;
  Resource_ResetEntry(*(u32 *)(battle + 84));
  VramBlock_Reset93Far();
  BattleEffect_BeginCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  Func_0813ba50();
  *(volatile u16 *)BG_PLTT = 0;
  *(volatile u16 *)(BG_PLTT + 2) = 0;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  blit = Ram_HeapSlots->unk64[1];
  work->transferMode = 0;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  Func_08163c2c(0, 0);
  list[0] = 255;
  BattleActor_SpawnObjectsForListFar(list, 0);
  gCameraSceneParameters[4] = 240;
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  BattleBackground_LoadFar(1, 0x72, 0);
  Func_08164b2c(-31, -31, -31);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  {
    u16 *map = (u16 *)0x0600f800;
    s32 base = 0;

    for (i = 0; i != 30; i++, base += 32) {
      if (i > 14) {
        for (j = 0; j != 32; j++) {
          *map++ = base + (j & 15) + 48;
        }
      } else {
        for (j = 0; j != 32; j++) {
          *map++ = base + (j & 15) + 0x200;
        }
      }
    }
  }
  {
    u8 *e = Resource_GetTableEntry(0xab);
    u8 *cells;

    Iwram_CopyWords((void *)OBJ_PLTT, e, 384);
    Resource_DecodeType01(e + 384, gMapCellBuffer);
    cells = gMapCellBuffer;
    for (i = 0; i != 24; i++, cells += 1024) {
      obj = Func_0815b290(32, 32, 0x80002000, 0);
      obj[9] = (obj[9] & ~12) | 8;
      work->objects[i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 1024);
    }
    cells = Data_02016000;
    for (i = 0; i != 4; i++, cells += 1024) {
      obj = Func_0815b3b0(32, 32, 0x80002000, 0);
      work->objects[24 + i] = obj;
      Iwram_CopyWords(obj, work->objects[i], 24);
      Iwram_CopyWords((void *)OBJ_TILES(work->objects[24 + i]), cells, 1024);
    }
  }
  Iwram_CopyWords(gMapCellBuffer + 0x1000, Data_02016000, 0x1000);
  for (i = 0; i != 6; i++) {
    obj = Func_0815b3b0(32, 32, 0x80002000, 0);
    work->objects[28 + i] = obj;
    Iwram_CopyWords(obj, work->objects[i], 24);
  }
  {
    u8 *e = Resource_GetTableEntry(0xae);
    u8 *cells;

    Iwram_CopyWords((void *)(OBJ_PLTT + 0x180), e, 32);
    cells = work->sheet;
    Resource_DecodeType01(e + 32, cells);
    for (i = 0; i != 5; i++, cells += 512) {
      obj = Func_0815b290(32, 32, 0x80000000, 0xc000);
      obj[9] = (obj[9] & ~12) | 4;
      work->objects[34 + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 512);
    }
  }
  {
    u8 *e = Resource_GetTableEntry(0xac);
    u8 *cells;

    Iwram_CopyWords((void *)(OBJ_PLTT + 0x1a0), e, 32);
    cells = work->sheet;
    Resource_DecodeType01(e + 32, cells);
    for (i = 0; i != 5; i++, cells += 512) {
      obj = Func_0815b290(32, 32, 0x80000000, 0xd000);
      obj[9] = (obj[9] & ~12) | 4;
      work->objects[39 + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 512);
    }
  }
  {
    u8 *e = Resource_GetTableEntry(0xad);
    u8 *cells;

    Iwram_CopyWords((void *)(OBJ_PLTT + 0x1c0), e, 32);
    cells = work->sheet;
    Resource_DecodeType01(e + 32, cells);
    for (i = 0; i != 6; i++, cells += 512) {
      obj = Func_0815b290(32, 32, 0x80000000, 0xe000);
      obj[9] = (obj[9] & ~12) | 4;
      work->objects[44 + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 512);
    }
  }
  Iwram_CopyWords(work->sheet + 0x5000, (void *)OBJ_PLTT, 480);
  Resource_LoadAndDecompress(0xf9, work->sheet, 0, 0);
  Resource_LoadAndDecompress(0xf8, work->sheet + 0x2000, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x188), 512);
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  Resource_LoadAndDecompress(0xb4, Data_02012000, 0, 0);
  camZ = 0;
  swapped = 0;
  drift = 0;
  sway = 0;
  cueTimer = 0;
  cueIndex = 0;
  work->transferMode = 2;
  work->transferValue = 50;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit = Ram_HeapSlots->unk64[1];
  spinA = 0xff9021c0;
  frame = 0;
  spinB = 0;
  while (frame != 637 && Catastrophe_FadeInAndPoll(frame)) {
    s32 fr;
    s32 fg;
    s32 fb;
    s32 level;
    s32 bx;
    s32 off;

    if (frame == 0) {
      REG_BG2CNT = 0x786;
      for (i = 3; i != 64; i++) {
        work->particles[i].variant = -1;
      }
    }
    if (frame == 440) {
      REG_BG2CNT = 0x784;
    }
    if (frame <= 369) {
      camZ = (frame * 334 / 379 << 16) + -0x11a0000;
    } else if ((u32)(frame - 370) <= 19) {
      camZ = Trig_Sin((frame - 370) * 1638) * 9 + 0x2b0000;
    } else if ((u32)(frame - 390) <= 95) {
      camZ += -0x8000;
    } else if ((u32)(frame - 486) <= 19) {
      camZ += -0x10000;
    } else if ((u32)(frame - 506) <= 47) {
      s32 v = (frame * 57 << 7) + -0x378eb0;

      if (v > 120000) {
        v = 120000;
      }
      camZ -= v << 1;
    }
    if (frame <= 379) {
      gBgScroll[1].y = 104 - frame * 104 / 379;
    } else {
      s32 v = (frame - 380) / 4;

      gBgScroll[1].y = v;
      if ((u32)(v << 16) > 0x680000) {
        gBgScroll[1].y = 104;
      }
    }
    if ((u32)(frame - 370) <= 59) {
      sway = Trig_Sin((frame - 370) * 273 / 2) << 4;
    }
    if ((u32)(frame - 420) <= 51) {
      drift = (420 - frame) << 12;
      sway += -0x4000;
    } else if ((u32)(frame - 472) <= 13) {
      drift = drift - ((frame - 472) << 12) + 120000;
      sway += 75536;
    } else if ((u32)(frame - 486) <= 8) {
      drift += 0x2000;
      sway -= Trig_Sin((frame << 7) + -0xf300);
    } else if ((u32)(frame - 554) <= 29) {
      sway += -0x10000;
    } else if ((u32)(frame - 584) <= 9) {
      sway += -0x80000;
    }
    if (frame == 0) {
      cueTimer = -1;
      cueIndex = 0;
    }
    if (cueIndex <= 17) {
      level = -1;
      if (frame == Data_08199768[cueIndex]) {
        cueTimer = Data_0819978c[cueIndex];
        switch (cueIndex & 3) {
        case 0:
          Audio_PlayCue(227);
          break;
        case 1:
          Audio_PlayCue(171);
          break;
        case 2:
          Audio_PlayCue(208);
          break;
        case 3:
          Audio_PlayCue(172);
          break;
        }
      }
      if (cueTimer >= 0) {
        level = cueTimer * 31 / Data_0819978c[cueIndex];
        if (--cueTimer == -1) {
          cueIndex++;
        }
      }
      if (level >= 0) {
        u16 *src = (u16 *)(battle + 0x56c);
        u16 *dst = (u16 *)(BG_PLTT + 0xc0);

        for (i = 0; i != 128; i++) {
          u32 c = *src++;
          s32 r = c & 31;
          s32 g = (c << 16) >> 21 & 31;
          s32 b = ((c << 16) >> 26 & 31) + level;

          if (r > 31) {
            r = 31;
          }
          if (g > 31) {
            g = 31;
          }
          if (b > 31) {
            b = 31;
          }
          *dst++ = b << 10 | g << 5 | r;
        }
      }
    }
    apply[0] = 0;
    apply[1] = 0;
    fr = 0;
    fg = 0;
    fb = 0;
    if (frame == 0) {
      fg = -31;
      fr = fg;
      fb = fg;
      apply[0] = 1;
    } else if (frame == 106) {
      fg = -31;
      fr = fg;
      fb = -15;
      apply[0] = 1;
    } else if ((u32)(frame - 154) <= 59) {
      s32 h = (frame - 154) / 2;

      fg = 5 - h * 2;
      fr = fg;
      fb = 5 - h;
      apply[0] = 1;
    } else if ((u32)(frame - 226) <= 15) {
      fg = (frame - 226) / 2 - 31;
      fr = fg;
      fb = frame - 257;
      apply[0] = 1;
    } else if (frame == 242) {
      fr = -24;
      fg = -31;
      apply[0] = 1;
    } else if ((u32)(frame - 256) <= 21) {
      s32 d = frame - 256;

      fr = -24 - d * 2;
      fg = -31 - d * 2;
      fb = -d;
      apply[0] = 1;
    } else if ((u32)(frame - 282) <= 40) {
      s32 d = frame - 282;

      fg = -d;
      fr = fg;
      fb = 16 - d;
      apply[0] = 1;
    } else if (frame == 324) {
      fr = 31;
      fg = 31;
      fb = 31;
      apply[0] = 1;
      apply[1] = 1;
    } else if ((u32)(frame - 332) <= 59) {
      fg = 30 - (frame - 332) / 2;
      apply[0] = 1;
      fr = fg;
      fb = fg;
      apply[1] = 1;
    } else if ((u32)(frame - 489) <= 42) {
      s32 d = frame - 489;

      fr = d / 4;
      fg = d / 6;
      apply[0] = 1;
      fb = d / 16;
    }
    for (k = 0; k != 2; k++) {
      if (apply[k] != 0) {
        u16 *src;
        u16 *dst;
        s32 n;

        if (k == 0) {
          src = (u16 *)(work->sheet + 0x5002);
          n = 240;
          dst = (u16 *)(BG_PLTT + 2);
        } else {
          src = (u16 *)(battle + 0x56c);
          dst = (u16 *)(BG_PLTT + 0xc0);
          n = 128;
        }
        for (j = 1; j != n; j++) {
          u32 c = *src++;
          s32 r = (c & 31) + fr;
          s32 g = ((c << 16) >> 21 & 31) + fg;
          s32 b = ((c << 16) >> 26 & 31) + fb;

          if (r > 31) {
            r = 31;
          }
          if (g > 31) {
            g = 31;
          }
          if (b > 31) {
            b = 31;
          }
          if (r < 0) {
            r = 0;
          }
          if (g < 0) {
            g = 0;
          }
          if (b < 0) {
            b = 0;
          }
          *dst++ = b << 10 | g << 5 | r;
        }
      }
    }
    if ((u32)(frame - 554) <= 31 && !((frame - 554) & 1)) {
      ColorBuffer_ScaleFar(battle + 0x56c, (void *)(BG_PLTT + 0xc0),
                           0x10000 - ((frame - 554) << 10), 128);
    }
    if (swapped == 0) {
      if (frame == 339) {
        swapped = 1;
        for (i = 0; i != 4; i++) {
          Iwram_CopyWords((void *)OBJ_TILES(work->objects[24 + i]),
                          gMapCellBuffer + (i << 10), 1024);
        }
      }
    } else if (frame == 450) {
      u8 *cells = Data_02011000;

      swapped = 0;
      for (i = 0; i != 4; i++, cells += 1024) {
        Iwram_CopyWords((void *)OBJ_TILES(work->objects[24 + i]), cells, 1024);
      }
    }
    if (frame <= 593) {
      req = Data_08196ef8;
      place[3] = 0;
      place[1] = 0xff0000;
      for (i = 0; i != 28; i++) {
        place[0] = (Data_081997b0[i] << 16) + 0x80000;
        place[2] = (Data_081997cc[i] << 16) + camZ;
        if (ON_SCREEN(place[2])) {
          Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                            (s32 *)&req, 0);
        }
      }
      for (i = 0; i != 5; i++) {
        s32 h = i / 2;

        place[0] = ((i - h * 2) << 21) + drift + 0x480000;
        place[2] = (h << 21) + camZ + sway + 0x3c0000;
        if (ON_SCREEN(place[2])) {
          Render_ApplyProjectedPlacementFar(work->objects[34 + i], place,
                                            (s32 *)&req, 0);
        }
      }
      for (i = 0; i != 11; i++) {
        place[0] = (Data_081997e8[i] << 16) - drift + 0x840000;
        place[2] = (Data_081997f4[i] << 16) + camZ + sway + 0x1d0000;
        if (ON_SCREEN(place[2])) {
          Render_ApplyProjectedPlacementFar(work->objects[39 + i], place,
                                            (s32 *)&req, 0);
        }
      }
      req.a = -0x10001;
      for (i = 0; i != 6; i++) {
        place[0] = (Data_0819980a[i] << 16) + 0x80000;
        place[2] = (Data_08199810[i] << 16) + camZ;
        if (ON_SCREEN(place[2])) {
          Render_ApplyProjectedPlacementFar(work->objects[28 + i], place,
                                            (s32 *)&req, 0);
        }
      }
    }
    verts = Runtime_BumpAllocateAlternatePool(512);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    shade = 0;
    texture.logHeight = 6;
    model->mesh = Data_0819931c;
    model->texture = &texture;
    model->kind = 6;
    model->vertices = verts;
    for (i = 0; i != 7; i++) {
      s32 st = Data_08199816[i];

      if (frame >= st && frame < st + 8) {
        shade = (st - frame) << 3;
        model->shade = shade;
        texture.pixels = work->sheet + Data_08199832[i] * 0x600;
        Render_ResetTransformState();
        Func_08015160((Data_08199824[i] - 48) << 16,
                      (Data_0819982b[i] - 44) << 16, 0);
        Func_080150e4(0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
    }
    if (frame == 489) {
      for (i = 0; i != 3; i++) {
        work->particles[i].variant = 0;
      }
    }
    p = &work->particles[3];
    for (i = 3; i != 64; i++, p++) {
      if (p->variant >= 0) {
        s32 n = (p->variant >> 3) + 1;

        blit(canvas, sprites + gData_08197410[n - 1], HALF_HI(p->x) - n / 2,
             HALF_HI(p->y) - n, n, n * 2);
        BattleEffectKernels_IntegrateVector2(p, 63, 0);
        p->variant--;
      }
    }
    for (i = 0; i != 17; i++) {
      s32 t = Data_0819983a[i];

      if (frame == t + 242) {
        work->transferMode = 1;
        work->transferValue = 0x1f1f1f1f;
      }
      if (frame == t + 243) {
        work->transferMode = 2;
        work->transferValue = 50;
      }
    }
    if ((u32)(frame - 537) <= 55) {
      if (frame & 1) {
        work->transferMode = 1;
        work->transferValue = 0x1f1f1f1f;
      } else {
        work->transferMode = 2;
        work->transferValue = 50;
      }
    }
    if (frame == 324) {
      Audio_PlayCue(206);
    }
    if (frame == 560) {
      Audio_PlayCue(220);
    }
    if (frame == 602) {
      Audio_PlayCue(145);
    }
    texture.logWidth = 5;
    texture.logHeight = 7;
    model->kind = 6;
    model->shade = shade;
    model->mesh = gData_08199268;
    model->texture = &texture;
    model->vertices = verts;
    if ((u32)(frame - 489) <= 130) {
      shade = 0;
      p = work->particles;
      for (i = 0, off = 0; i != 3; i++, off += 12, p++) {
        s32 idx = p->variant;

        if (frame == (Data_081998a4[i * 13 + idx] - 492) / 2 + 489) {
          s32 n = off + idx;
          s32 count = 0;
          s32 a = Data_08199916[n];

          if (a != 0) {
            p->x = Data_0819985c[n] << 3;
            p->y = Data_08199880[n] << 3;
            p->z = a << 8;
          }
          p->velocityY = Data_081998f2[n];
          p->velocityZ = Data_0819995e[n];
          p->velocityX = Data_081999a6[n];
          p->variant = idx + 1;
          if (p->velocityX == -1) {
            continue;
          }
          bx = 0x80000 - drift;
          blit(canvas, sprites + gData_08197410[9],
               p->x / 8 - 5 + ((bx / 2) >> 16),
               p->y / 8 - 10 + ((camZ + sway) >> 16), 10, 20);
          switch (i) {
            s32 cue;

          case 0:
            cue = 186;
            goto play;
          case 1:
            cue = 187;
            goto play;
          case 2:
            cue = 222;
          play:
            Audio_PlayCue(cue);
            break;
          }
          {
            s32 half = bx / 2;

            q = &work->particles[3];
            for (j = 3; j != 64; j++, q++) {
              if (q->variant < 0) {
                s32 angle;
                s32 speed;

                q->x = (p->x << 13) + half;
                q->y = (p->y << 13) + camZ + sway;
                angle = Random16();
                speed = (Random16() & 255) + 127;
                q->velocityX = speed * Trig_Sin(angle) >> 6;
                q->velocityY = speed * Trig_Cos(angle) >> 6;
                q->variant = (Random16() & 7) + 16;
                if (++count == 10) {
                  break;
                }
              }
            }
          }
        }
        if (p->velocityX != -1) {
          model->shade = shade;
          texture.pixels = work->sheet + 0x2000 + (p->velocityX << 12);
          Render_ResetTransformState();
          Func_08015160(((p->x / 8 - 64) << 16) + (0x80000 - drift) / 2,
                        ((p->y / 8 - 64) << 16) + camZ + sway, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_080150e4(p->z);
          Func_080151e4(0x40000, 85536, 0x8000);
          Render_TransformVertices(Data_081991d0, verts, 4);
          Render_DrawModelList(model);
          p->y += p->velocityY * 2;
          p->z += p->velocityZ * 2;
        }
      }
    }
    if (frame == 602) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 630) {
      Iwram_FillWords(canvas, 0x4000, 0x1f1f1f1f);
    }
    if (frame == 634) {
      Iwram_FillWords(canvas, 0x4000, 0x1f1f1f1f);
    }
    if ((u32)(frame - 552) <= 43) {
      p = work->particles;
      if (!(frame & 1)) {
        p->y = (Random16() & 0x3fff) + 0x6000;
        p->z = (Random16() % 3 << 12) + 0x2000;
      }
      texture.pixels = work->sheet + p->z;
      Render_ResetTransformState();
      Func_08015160((0x80000 - drift) / 2 + 0x40000, camZ + 0xcc0000, 0);
      Func_080151e4(0x8000, 0x10000, 0x8000);
      Func_080150e4(p->y);
      Func_080151e4(0x40000, 85536, 0x8000);
      Render_TransformVertices(Data_081991d0, verts, 4);
      Render_DrawModelList(model);
    }
    if ((u32)(frame - 602) <= 15 || (u32)(frame - 630) <= 3) {
      s32 n = 1;
      s32 base = Random16() % 3;
      s32 sx = 0x50000;
      s32 spread = 8;

      if (frame > 611) {
        sx = 0x50000 - frame * 61845 + 0x2418834;
      }
      if (sx <= 65535) {
        sx = 0x10000;
      }
      if (frame <= 613) {
        if (frame > 605) {
          n = 3;
        } else if (frame > 603) {
          n = 2;
        }
      }
      model->kind = 6;
      model->shade = shade;
      p = work->particles;
      for (j = 0; j != n; j++, p++) {
        p->x = (Random16() % spread - 4) << 16;
        p->y = camZ + 0xc80000;
        p->z = ((base + j) % 3 << 12) + 0x2000;
        texture.pixels = work->sheet + p->z;
        Render_ResetTransformState();
        Func_08015160(0x10000, p->y, 0);
        Func_080151e4(sx, 0x10000, 0x8000);
        Func_08015160(p->x, 0, 0);
        Func_080151e4(0x8000, 0x20000, 0x10000);
        Func_080150e4(0x8000);
        Render_TransformVertices(Data_081991d0, verts, 4);
        Render_DrawModelList(model);
      }
    }
    if (frame == 552) {
      Resource_LoadAndDecompress(0xb4, Data_02012000, 0, 0);
    }
    if (frame == 551) {
      Resource_LoadAndDecompress(0xee, work->sheet, 0, 0);
      Resource_LoadAndDecompress(0xef, work->sheet + 0x1000, 0, 0);
    }
    if (frame == 602) {
      Resource_LoadAndDecompress(0xc2, Data_02012000, 0, 0);
    }
    if ((u32)(frame - 602) <= 6) {
      camZ += 0x100000;
    }
    {
      s32 d = frame - 552;

      if ((u32)d <= 58) {
        s32 s = d * 4424;
        s32 a;
        s32 b;

        shade = -16;
        if (s > 0x22538) {
          s = 0x22538;
        }
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->kind = 7;
        model->shade = shade;
        model->texture = &texture;
        model->mesh = gData_08199340;
        model->vertices = verts;
        texture.pixels = work->sheet + 0x1000;
        Render_ResetTransformState();
        bx = 0x80000 - drift;
        Func_08015160(bx / 2 + 0x50000, camZ + 0xe60000, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        SceneTransform_ApplyPitch(18768);
        a = -frame << 10;
        Func_080150e4(a);
        Func_0801521c(s);
        Func_08015160(0, -0xf0000, 0);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Func_08015160(0, 0xf0000, 0);
        Render_DrawModelList(model);
        texture.pixels = work->sheet;
        a += 10922;
        b = 10922;
        for (j = 0; j != 3; j++, a += 21845, b += 21845) {
          Transform_Push();
          Func_080150e4(b);
          model->shade = -24 - (Trig_Cos(a) << 3 >> 16);
          Func_08015160(0, -0x100000, 0);
          if (frame > 579) {
            s32 v = spinB + -0x9b320;

            if (v > 19712) {
              v = 19712;
            }
            SceneTransform_ApplyPitch(-v);
          }
          Render_TransformVertices(gData_081991f0, verts, 4);
          Render_DrawModelList(model);
          Transform_Pop();
        }
        if (frame > 589) {
          s = s + frame * 11060 + -0x6391d8;
        }
        if (s > 0 && frame <= 601) {
          shade = -16;
          if (frame > 589) {
            shade = -16 - frame * 2 + 1180;
          }
          if (shade > -64) {
            model->shade = shade;
            texture.pixels = Data_02012000;
            Render_ResetTransformState();
            if (frame > 553) {
              s32 v = (554 - frame) * 120000;

              if (v < -0xcd140) {
                v = -0xcd140;
              }
              Func_08015160(0, v, 0);
            }
            Func_08015160(bx / 2 + 0x50000, camZ + 0xdc0000, 0);
            Func_080151e4(0x10000, 32000, 0x10000);
            Func_080150e4(frame << 11);
            Func_0801521c(s);
            Render_TransformVertices(Data_081991e0, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
    }
    if (frame > 601) {
      shade = 0;
      if (frame > 611) {
        shade = -frame * 8 + 4896;
      }
      if (shade > -64) {
        model->shade = shade;
        model->mesh = gData_08199364;
        model->kind = 7;
        texture.pixels = Data_02012000;
        texture.logWidth = 7;
        texture.logHeight = 7;
        Render_ResetTransformState();
        Func_08015160(0x10000, -0x100000, 0);
        Func_080151e4(0x10000, 32000, 0x10000);
        Func_080150e4(frame << 10);
        Func_0801521c(spinA);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
    }
    MemFree(model);
    MemFree(verts);
    if ((u32)(frame - 554) <= 51) {
      s32 r = frame - 553;
      s32 y = 90 - (frame - 554) / 2;

      if (frame > 600) {
        y = y + (frame << 4) - 9616;
      }
      if (y <= 63) {
        y = 64;
      }
      if (r <= 38) {
        Func_0815b434(gMapCellBuffer, r, 96000);
      } else {
        r = 38;
      }
      Func_0818caa8(gMapCellBuffer, 62, y, r);
    }
    work->transferPending = 1;
    WaitFrames(1);
    spinA += 12192;
    spinB += 1096;
    frame++;
  }
  for (i = 0; i != 50; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Func_08020388();
  *(s32 *)(battle + 84) = VramCache_AllocEntry(128);
  BattleEffect_SelectLivingTargets(work->effect);
  for (i = 0; i != effect->count; i++) {
    Func_0814cd48(effect->actors[i], 1, -1, -1, 0);
    Func_0814cd48(effect->actors[i], 0, -1, -1, 0);
  }
  Palette_LoadBasicObjColors();
  Func_0814cca8();
  gCameraSceneParameters[4] = 240;
  work->fadeFrames = 0;
  work->fadeStep = 0;
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(1, 0x75, 0);
  Func_08164b2c(0, -16, -16);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress(0x188, gMapCellBuffer, 1, 1);
  Resource_LoadAndDecompress(0x192, work->sheet + 0x800, 1, 0);
  Iwram_ClearWords(work->sheet + 0x1456, 0x4000);
  for (k = 0; k != 4; k++) {
    for (j = 0; j != 120; j++) {
      u8 *dst = work->sheet + 0x145a + (k << 12) + j * 32;
      u8 *src = gMapCellBuffer + k * 45 * 64 + j * 24;
      s32 m;

      for (m = 0; m != 24; m++) {
        *dst++ = *src++;
      }
    }
  }
  Resource_LoadAndDecompress(0xee, gMapCellBuffer, 0, 0);
  Resource_LoadAndDecompress(0xef, Data_02011000, 0, 0);
  Resource_LoadAndDecompress(0xc2, Data_02012000, 0, 0);
  rise = 0;
  work->transferMode = 2;
  work->transferValue = 50;
  frame = 0;
  if (!(gInput.repeat & 3)) {
    phase = 0;
    do {
      s32 r;
      s32 top;
      s32 sh;

      if (frame == 0) {
        Iwram_FillWords(canvas, 0x4000, 0);
        rise = -0x400000;
        Func_08164b2c(0, -16, -16);
        work->transferMode = 2;
        work->transferValue = 50;
        Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x149), 512);
      }
      if (rise <= 0xfffff) {
        rise += 0x10000;
      }
      if ((u32)(frame - 96) <= 23) {
        Func_08164b2c(-(frame - 96), -16, frame - 112);
        work->shakeFrames = 8;
      }
      if (frame > 139) {
        work->transferMode = 1;
        if (frame > 153) {
          work->transferValue = 0x3f3f3f3f;
        } else {
          work->transferValue = 0x10101010;
        }
        Func_08164a4c(1, 1, 1);
      }
      if (frame == 80 || frame == 83) {
        u16 *c = (u16 *)(BG_PLTT + 0xc0);

        for (i = 0; i != 128; i++, c++) {
          *c ^= 0x7fff;
        }
      }
      if (frame == 82) {
        work->shakeFrames = 8;
        Audio_PlayCue(208);
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      }
      if (frame == 96) {
        Audio_PlayCue(104);
      }
      if (frame == 128) {
        u16 *c = (u16 *)(BG_PLTT + 0xc0);

        work->shakeFrames = 16;
        for (i = 0; i != 128; i++, c++) {
          *c ^= 0x7fff;
        }
      }
      if (frame > 128) {
        s32 d = (frame - 128) * 2;

        Func_08164b2c(d - 11, frame - 136, 3 - d);
      }
      r = 16;
      if (frame > 92) {
        r = frame - 75;
      }
      if (frame <= 79) {
        r += Trig_Sin(frame << 11) * 2 >> 16;
      }
      if (r <= 64) {
        Func_0815b434(work->sheet, r, 96000);
      } else {
        r = 64;
      }
      top = (rise >> 16) - r / 2 + 60;
      Func_0818caa8(work->sheet, 60, top, r);
      if (frame > 95) {
        for (i = 0; i != 4; i++) {
          s32 n = i & 3;
          s32 angle = Random16();
          s32 x = (r * Trig_Sin(angle) >> 17) + 60 - (gData_08197492[n] >> 1);
          s32 y = top + (r * Trig_Cos(angle) >> 17) - (gData_08197498[n] >> 1);

          Func_08196404(SLOT_ALT, 7, 7, Data_081999ee[Random16() & 3] | 3, 0);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas, work->sheet + 0x800 + gData_08197486[n], x, y,
              gData_08197492[n], gData_08197498[n]);
          Runtime_ReleaseHeapBlock(SLOT_ALT);
        }
      }
      verts = Runtime_BumpAllocateAlternatePool(512);
      model = Render_AllocateModelList(1);
      if (frame <= 127) {
        s32 s = 85536;
        s32 a;
        s32 b;

        if (frame > 71) {
          s = (frame << 11) + -0xf1e0;
        }
        sh = -16;
        if (frame > 119) {
          sh = 944 - frame * 8;
        }
        if (s > 0x22538) {
          s = 0x22538;
        }
        texture2.logWidth = 6;
        texture2.logHeight = 6;
        model->shade = sh;
        model->kind = 6;
        model->texture = &texture2;
        model->mesh = gData_08199340;
        model->vertices = verts;
        texture2.pixels = Data_02011000;
        Render_ResetTransformState();
        Func_08015160(-0x20000, rise, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        SceneTransform_ApplyPitch(18768);
        a = frame << 9;
        Func_080150e4(a);
        Func_0801521c(s);
        Func_08015160(0, -0xf0000, 0);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Func_08015160(0, 0xf0000, 0);
        Render_DrawModelList(model);
        texture2.pixels = gMapCellBuffer;
        b = 10922;
        a += 10922;
        for (j = 0; j != 3; j++, a += 21845, b += 21845) {
          s32 v = sh + (Trig_Cos(a) << 3 >> 16) - 16;

          Transform_Push();
          Func_080150e4(b);
          model->shade = v;
          Func_08015160(0, -0x100000, 0);
          v = 19712;
          if (frame > 63) {
            v = 19712 - (frame << 10) + 0x10000;
          }
          if (v < 0) {
            v = 0;
          }
          SceneTransform_ApplyPitch(-v);
          Render_TransformVertices(gData_081991f0, verts, 4);
          Render_DrawModelList(model);
          Transform_Pop();
        }
      }
      {
        s32 s1 = phase * 127 * 32 + -0xee200;
        s32 s2 = phase * 127 * 16 + -0x77100;
        s32 start = 80;
        s32 fade = 640 - frame * 8;

        for (j = 0; j != 2;
             j++, start += 8, s2 += -0xbe80, fade += 64, s1 += -0x17d00) {
          s32 s;

          if (frame < start) {
            continue;
          }
          sh = 0;
          if (j == 0) {
            s = s1 + 0x2000;
            if (frame >= j * 8 + 96) {
              sh = fade + 128;
            }
          } else {
            s = s2 + 0x2000;
            if (frame >= j * 8 + 112) {
              sh = fade + 256;
            }
          }
          if (sh > -64) {
            model->shade = sh;
            model->mesh = gData_08199364;
            model->kind = 6;
            texture2.pixels = Data_02012000;
            texture2.logWidth = 7;
            texture2.logHeight = 7;
            Render_ResetTransformState();
            Func_08015160(0, 0x100000, 0);
            Func_080151e4(0x10000, 32000, 0x10000);
            Func_080150e4(frame << 10);
            Func_0801521c(s);
            Render_TransformVertices(Data_081991e0, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
      texture2.logWidth = 5;
      texture2.logHeight = 7;
      model->kind = 6;
      model->mesh = gData_08199268;
      model->shade = 0;
      if (frame == 0) {
        p = work->particles;
        for (i = 0; i != 16; i++, p++) {
          p->z = Data_081999f2[i];
          p->velocityZ = (Random16() & 255) + 128;
          if (Random16() & 1) {
            p->velocityZ = -p->velocityZ;
          }
          p->variant = Random16() & 3;
        }
      }
      for (i = 0; i != 8; i++) {
        if (frame == 128 + i * 4) {
          Audio_PlayCue(144);
        }
      }
      p = work->particles;
      for (i = 0; i != 16; i++, p++) {
        if (frame >= i + 128 && frame < i + 144) {
          s32 s = (frame - (i + 128)) << 13;

          if (s > 0x10000) {
            s = 0x10000;
          }
          texture2.pixels = work->sheet + 0x1456 + (p->variant << 12);
          Render_ResetTransformState();
          Func_080150e4(p->z);
          p->z += p->velocityZ;
          Func_080151e4(s << 2, s, 0x10000);
          Render_TransformVertices(gData_081991b0, verts, 4);
          Render_DrawModelList(model);
        }
      }
      MemFree(model);
      MemFree(verts);
      if (frame > 133) {
        if (frame == 134) {
          p = &work->particles[16];
          for (i = 0; i != 48; i++, p++) {
            s32 a = Random16();
            s32 angle = i * 1365;
            s32 rr = (a & 31) + 32;

            p->x = (rr * Trig_Sin(angle) >> 16) + 60;
            p->y = (rr * Trig_Cos(angle) >> 16) + 60;
            p->z = (Random16() & 3) + 5;
            p->variant = -(Random16() & 15);
          }
        }
        p = &work->particles[16];
        for (i = 0; i != 48; i++, p++) {
          if ((u32)p->variant <= 7) {
            s32 n = (Trig_Sin(p->variant << 12) * p->z >> 16) + 1;

            blit(canvas, sprites + gData_08197410[n - 1], p->x - n / 2,
                 p->y - n, n, n * 2);
          }
          p->variant++;
        }
      }
      if (work->shakeFrames > 0) {
        s32 a = Random16();
        s32 b = Random16();

        gBgScroll[1].x = (a & 7) - 4;
        gBgScroll[1].y = (b & 7) + 28;
        work->shakeFrames--;
      } else {
        gBgScroll[1].x = 0;
        gBgScroll[1].y = work->savedBg1Y;
      }
      work->transferPending = 1;
      WaitFrames(1);
      phase += 3;
      frame++;
    } while (frame != 160 && !(gInput.repeat & 3));
  }
  slide = 0;
  slideStep = 0;
  Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
  work->transferPending = 1;
  WaitFrames(1);
  gBgScroll[1].x = savedScroll;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  Func_08164b2c(-16, -16, -4);
  Resource_LoadAndDecompress(0xf4, work->sheet, 1, 1);
  Resource_LoadAndDecompress(0xe8, work->sheet + 0x800, 1, 0);
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  Resource_LoadAndDecompress(0x13e, work->sheet + 0x1800, 1, 0);
  Resource_LoadAndDecompress(0x188, gData_02014000, 1, 0);
  Render_BuildQuadStrip(Data_02012000, 4, 32, 11);
  REG_BG2CNT = 0x784;
  frame = 0;
  do {
    if ((gInput.repeat & 3) && frame > 2 && frame <= 87) {
      frame = 88;
    }
    if (frame == 88) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      work->transferMode = 3;
      work->transferValue = 0x02020202;
      gBgScroll[1].x = savedScroll;
      gCameraSceneParameters[3] = 120;
      work->scrollStepX = 0;
      Func_081180e8(134);
    }
    if (frame == 0) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      work->scrollStepX = -16;
      gCameraSceneParameters[3] = -968;
      gCameraSceneParameters[4] = 120;
      gBgScroll[1].x = savedScroll + 1088;
      Graphics_LoadGreyRamp();
      work->transferMode = 3;
      work->transferValue = 0x02020202;
      for (i = 0; i != 164; i++) {
        Data_02016d00[i].variant = -1;
      }
      slide = 0x800000;
      slideStep = 0;
    }
    if (frame == 16) {
      work->transferMode = 2;
      work->transferValue = 75;
    }
    if ((u32)(frame - 16) <= 47) {
      Func_0815f0a0(0x149);
    }
    if (frame == 66) {
      s32 base = 0;

      for (i = 0; i != effect->count; i++, base += 16) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x40000, 0x80000,
                                  0x40000, 150);
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        EffectPosition_ProjectBattler(effect->actors[i], &pos);
        q = &Data_02016d00[64 + base];
        for (j = 0; j != 16; j++, q++) {
          s32 angle;
          s32 speed;

          q->x = (pos.x / 2) << 16;
          q->y = 0x500000;
          angle = (Random16() & 0x7fff) + 0x4000;
          speed = (Random16() & 127) + 128;
          q->velocityX = speed * Trig_Sin(angle) >> 5;
          q->velocityY = speed * Trig_Cos(angle) >> 4;
          q->variant = 16;
        }
      }
      work->shakeFrames = 16;
      Audio_PlayCue(145);
    }
    if ((u32)(frame - 72) <= 15) {
      work->transferMode = 4;
      work->transferValue = 0x01010101;
      Func_08164a4c(1, 1, 1);
    }
    if (frame >= 0) {
      s32 x = 0;

      if (frame > 71) {
        x = -frame * 8 + 576;
      }
      x += 116;
      q = &Data_02016d00[(frame & 31) * 2];
      for (j = 0; j != 2; j++, q++) {
        s32 angle;
        s32 speed;

        q->x = x << 16;
        q->y = (Random16() % 112 + 4) << 16;
        angle = (Random16() & 0x7fff) + 0x8000;
        speed = (Random16() & 127) + 128;
        q->velocityX = speed * Trig_Sin(angle) >> 6;
        q->velocityY = speed * Trig_Cos(angle) >> 6;
        q->variant = 0;
      }
      q = Data_02016d00;
      for (j = 0; j != 64; j++, q++) {
        if ((u32)q->variant <= 11) {
          blit(canvas, work->sheet + 0x1800 + (q->variant / 2 << 11),
               HALF_HI(q->x) - 16, HALF_HI(q->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(q, 63, 0);
          q->variant++;
        }
      }
    }
    if (frame <= 87) {
      s32 jitter = 0;
      s32 y = 0;
      s32 wave = frame;
      s32 off = 0;
      s32 a;
      s32 b;
      s32 c;

      for (k = 0; k != 12; k++, wave += 2, off += 8) {
        s8 *v = (s8 *)gMapCellBuffer + off;
        s32 x = 0;

        for (j = 0; j != 2; j++, x += 10, v += 4) {
          v[0] = x + (Trig_Sin(wave << 10) * 2 >> 16) + jitter - 5;
          v[2] = 0;
          v[1] = y;
        }
        jitter = jitter + (Random16() & 7) - 4;
        y -= 6;
      }
      model = Render_AllocateModelList(1);
      model->kind = 6;
      model->texture = &texture3;
      model->shade = 0;
      model->scrollV = 0;
      model->vertices = Data_02011000;
      if (frame == 16) {
        slideStep = -0x80000;
      }
      if ((u32)(frame - 48) <= 5) {
        slideStep = 0x80000;
      }
      if (frame > 59) {
        slideStep = -0x100000;
      }
      if (frame > 31) {
        slideStep = slideStep * 48 / 64;
      }
      slide += slideStep;
      texture3.logWidth = 6;
      texture3.logHeight = 5;
      texture3.pixels = work->sheet;
      model->scrollU = (frame << 3) & 127;
      model->mesh = Data_02012000;
      Render_ResetTransformState();
      Func_080151e4(0x8000, 0x10000, 0x8000);
      a = frame * 3 * 127 * 16;
      b = frame << 13;
      c = Trig_Sin(a);
      Func_08015160(slide, (c << 5) + Trig_Cos(b) * 24, 0);
      Func_0801521c(55536);
      Transform_Push();
      Func_080150e4(0x4000);
      Func_0801521c(0x42ee0);
      Render_TransformVertices(gMapCellBuffer, Data_02011000, 24);
      Render_DrawModelList(model);
      Transform_Pop();
      model->mesh = gData_08199340;
      texture3.logWidth = 6;
      texture3.logHeight = 6;
      model->scrollU = 0;
      texture3.pixels = work->sheet + 0x800;
      Func_08015160(-0x20000, (Trig_Sin(frame << 10) << 2) + -0x60000, 0);
      Func_080150e4(-0x800);
      {
        s32 s0 = Trig_Sin(a);
        s32 y0 = (s0 << 5) + Trig_Cos(b) * 24;
        s32 s1;
        s32 y1;

        a += 6096;
        s1 = Trig_Sin(a);
        b += 0x2000;
        y1 = (s1 << 5) + Trig_Cos(b) * 24;
        Func_080150e4((y0 - y1) / 512 + -0x400);
      }
      Func_080151e4(0x21170, 105536, 0x10000);
      Render_TransformVertices(Data_081991e0, Data_02011000, 4);
      Render_DrawModelList(model);
      MemFree(model);
      if (frame <= 87) {
        s32 lim = 16;

        p = work->particles;
        for (i = 0; i != 6; i++, lim += 8, p++) {
          if (frame == lim) {
            p->x = (Random16() & 31) + 64;
            p->variant = Random16();
            work->shakeFrames = 4;
            switch (i & 3) {
            case 0:
              Audio_PlayCue(227);
              break;
            case 1:
              Audio_PlayCue(171);
              break;
            case 2:
              Audio_PlayCue(208);
              break;
            case 3:
              Audio_PlayCue(172);
              break;
            }
            q = &Data_02016d00[64 + (i & 7) * 12];
            for (j = 0; j != 12; j++, q++) {
              s32 angle;
              s32 speed;

              q->x = p->x << 16;
              q->y = 0x680000;
              angle = (Random16() & 0x7fff) + 0x4000;
              speed = (Random16() & 127) + 128;
              q->velocityX = speed * Trig_Sin(angle) >> 5;
              q->velocityY = speed * Trig_Cos(angle) >> 4;
              q->variant = 16;
            }
          }
          if (frame >= lim && frame < lim + 8) {
            blit(canvas, gData_02014000 + ((p->variant / 2) & 3) * 2880,
                 p->x - 12, 0, 24, 120);
            p->variant++;
          }
        }
        q = &Data_02016d00[64];
        for (j = 0; j != 100; j++, q++) {
          if (q->variant >= 0) {
            s32 n = (q->variant >> 2) + 2;

            blit(canvas, sprites + gData_08197410[n - 1], HALF_HI(q->x) - n / 2,
                 HALF_HI(q->y) - n, n, n * 2);
            BattleEffectKernels_IntegrateVector2(q, 56, 0x2000);
            q->velocityX -= work->scrollStepX << 12;
            q->variant--;
          }
        }
      }
    }
    gCameraSceneParameters[3] -= work->scrollStepX;
    gBgScroll[1].x += work->scrollStepX;
    for (i = 0; i != 16; i++) {
      if (frame == i + 62) {
        work->scrollStepX++;
      }
    }
    Camera_ApplyVerticalShake(8);
    ObjectGroup_TickMemberTimers();
    frame++;
    work->transferPending = 1;
    WaitFrames(1);
  } while (frame != 104);
  transition[4] = 0;
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
