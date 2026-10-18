// Draft of BattleFx_RunVengeance, decompiled from the listing: differ 744 of
// 861 (draft 859), the same 36 calls and control flow; allocation differs.

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

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143bb8(void); // BattleFx_EndCanvasLayer
void Func_08143000(void); // BattleFx_ProcessPendingGraphicsTransfer
void Func_081434f8(void); // ObjectGroup_TickMemberTimers
void Func_081963ec(s32 slot, s32 modeAndFlags); // BattleFx_LoadWork
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // ObjectGroup_UpdateMembers
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Func_081969f8(s32 count);
void Func_08196958(const void *corners, void *vertices, s32 count);
void Func_08196a7c(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id,
                                                 struct EffectPosition *out);
void Func_08152404(s32 count, s32 resource, s32 mode);   // SpawnObjects
void Func_0814cc4c(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
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

void Func_08178680(void *mesh, s32 columns, s32 width, s32 height);
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

extern char ResourceId_Data0B0;
extern const u8 Data_081995f3[][7]; // per level: which blades strike

#define VENGEANCE_FRAMES 124
#define BLADES 7

// Abilities: Vengeance.
//
// The avenger (a tiled background in a window) slides in from the far
// side and raises its arm every twelve frames from frame 22; each time a
// blade (one of seven) flies across, and those the level picks strike the
// target, the last one hardest. Everything is mirrored on the right side.
void BattleFx_RunVengeance(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u16 *camera;
  DrawRectangle blit;
  DrawRectangle blitFlipped;
  struct EffectPosition pos;
  struct EffectStep *blade;
  s32 backgroundX;
  s32 speed;
  s32 raised;
  s32 frame;
  s32 row;
  s32 column;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleFx_BeginTiledCanvas(0);
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x410;

  // The avenger's tile map: the body, and its arm in rows 3 to 6.
  i = 0;
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 16; column++) {
      s32 tileX;
      s32 tileY;
      s32 tile;

      if (effect->side == 0) {
        if (column > 7) {
          goto body;
        }
        if ((u32)(row - 3) > 3) {
          goto blank;
        }
        tileX = column;
        tileY = row + 9;
      } else {
        if (column <= 7) {
        body:
          if (row > 11) {
            goto blank;
          }
          tileX = column & 7;
          tileY = row;
        } else {
          if ((u32)(row - 3) > 3) {
            goto blank;
          }
          tileX = column & 7;
          tileY = row + 9;
        }
      }
      goto store;
    blank:
      tileX = 0;
      tileY = 0;
    store:
      tile = (tileY * 8 + tileX) * 2;
      ((s16 *)0x06003800)[i++] = (s16)((tile + 1) << 8 | tile);
    }
  }
  REG_BG2Y = 0xffffd800;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x2888;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x2888;
  REG_WININ = 0x3737;
  REG_WINOUT = 0x2033;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B0, work->sheet, 1, 1);
  Func_081963ec(SLOT_BLITTER,
                BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Func_081963ec(188,
                BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                              COMPOSITE_FLIP_X));
  work->transferMode = 1;
  work->transferValue = 0;
  blitFlipped = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
  backgroundX = 0x560000;
  speed = 0x100000;

  for (frame = 0; frame != VENGEANCE_FRAMES; frame++) {
    camera = (u16 *)Ram_HeapSlots->cameraWork;
    EffectPosition_ApplyStepAndYOffset(effect->actors[0], &pos);
    if (frame <= 31) {
      if (effect->side == 0) {
        camera[27] -= 128;
      } else {
        camera[27] += 128;
      }
    }
    if (frame >= 108) {
      REG_BLDALPHA = (124 - frame - 1) | 0x1000;
    }
    if (frame == 0) {
      if (effect->side == 0) {
        backgroundX = 0x560000;
        speed = 0x100000;
      } else {
        backgroundX = 0xffaa0000;
        speed = 0xfff00000;
      }
      for (i = 0; i != BLADES; i++) {
        blade = &work->particles[i];
        if (effect->side == 0) {
          blade->x = 0xa00000;
          blade->velocityX = 0xc0000;
        } else {
          blade->x = 0x200000;
          blade->velocityX = -0xc0000;
        }
        blade->y = 0xc0000;
        blade->z = Data_081995f3[effect->variant][i] != 0;
        blade->velocityY = (Random16() & 0xffff) - 0x8000;
        blade->velocityZ = 0;
        blade->variant = 0;
      }
    }

    // The avenger slides in, its window following.
    if (frame <= 24) {
      s32 x = backgroundX >> 16;

      REG_BG2X = -x << 8;
      if (effect->side == 0) {
        REG_WIN0H = (x + 120) << 8 | 0xf0;
        REG_WIN1H = (x + 120) << 8 | 0xf0;
      } else {
        REG_WIN0H = x + 240;
        REG_WIN1H = x + 240;
      }
    } else {
      REG_BG2X = -(backgroundX >> 16) << 8;
      REG_WIN0H = 0xf0;
      REG_WIN1H = 0xf0;
    }
    backgroundX -= speed;
    speed = speed * 52 / 64;

    raised = 0;
    if (frame > 105) {
      raised = 1;
    } else if (frame > 21 && (frame - 22) % 12 > 3) {
      raised = 1;
    }
    if (effect->side == 0) {
      if (raised == 0) {
        blit(canvas, work->sheet, 32, 0, 88, 96);
      } else {
        blit(canvas, work->sheet + 0x2100, 32, 0, 88, 96);
      }
    } else if (raised == 0) {
      blitFlipped(canvas, work->sheet, 8, 0, 88, 96);
    } else {
      blitFlipped(canvas, work->sheet + 0x2100, 8, 0, 88, 96);
    }

    // The blades.
    for (i = 0, blade = work->particles; i != BLADES; i++, blade++) {
      s32 state;

      if (frame < i * 12 + 22 && (frame > 21 || i != 0)) {
        continue;
      }
      if (frame >= i * 12 + 26 && blade->variant == 0) {
        blade->x -= blade->velocityX;
        blade->y += blade->velocityY;
      }
      state = blade->variant;
      if (state > 4) {
        continue;
      }
      if (state > 0) {
        blade->variant = ++state;
      }
      if (blade->z == 1 && state == 0) {
        s32 hit = 0;

        if (effect->side == 0) {
          if ((blade->x >> 16) < pos.x / 2 + 16) {
            hit = 1;
          }
        } else if ((blade->x >> 16) > pos.x / 2 + 48) {
          hit = 1;
        }
        if (hit == 1) {
          work->shakeFrames = 4;
          if (i == 6) {
            Func_08118088(effect->actors[0], 4);
            Func_081180e8(134);
          } else {
            BattleFx_StartActorMotion(effect->actors[0], 1, 0x20000, 0x80000,
                                      0x20000, i * 10 + 110);
            Audio_PlayCue(133);
          }
          Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
          blade->variant = 1;
          if (effect->side == 0) {
            blade->x = pos.x / 2 - HI(blade->x);
          } else {
            blade->x = pos.x / 2 - HI(blade->x) + 128;
          }
          state = 1;
        }
      }
      if (effect->side == 0) {
        if (state == 0) {
          blit(canvas, work->sheet + 0x42cc, (blade->x >> 16) - 111,
               HI(blade->y) + 24, 51, 12);
          blit(canvas, work->sheet + 0x42cc, HI(blade->x) + 17,
               HI(blade->y) + 96, 51, 12);
          blit(canvas, work->sheet + 0x4200, HI(blade->x) - 128,
               HI(blade->y) + 24, 17, 12);
          blit(canvas, work->sheet + 0x4200, HI(blade->x), HI(blade->y) + 96,
               17, 12);
        } else {
          blit(canvas, work->sheet + 0x42cc, pos.x / 2 + blade->x - 111,
               HI(blade->y) + 24, 51, 12);
          blit(canvas, work->sheet + 0x42cc, pos.x / 2 + blade->x + 17,
               HI(blade->y) + 96, 51, 12);
        }
      } else if (state == 0) {
        blitFlipped(canvas, work->sheet + 0x42cc, blade->x >> 16,
                    HI(blade->y) + 24, 51, 12);
        blitFlipped(canvas, work->sheet + 0x42cc, HI(blade->x) - 128,
                    HI(blade->y) + 96, 51, 12);
        blitFlipped(canvas, work->sheet + 0x4200, HI(blade->x) + 51,
                    HI(blade->y) + 24, 17, 12);
        blitFlipped(canvas, work->sheet + 0x4200, HI(blade->x) - 77,
                    HI(blade->y) + 96, 17, 12);
      } else {
        blitFlipped(canvas, work->sheet + 0x42cc, pos.x / 2 + blade->x,
                    HI(blade->y) + 24, 51, 12);
        blitFlipped(canvas, work->sheet + 0x42cc, pos.x / 2 + blade->x - 128,
                    HI(blade->y) + 96, 51, 12);
      }
    }

    // The avenger's eye, while the arm is down.
    if (raised == 0) {
      if (effect->side == 0) {
        blit(canvas, work->sheet + 0x4530, 88, 29, 14, 13);
      } else {
        blitFlipped(canvas, work->sheet + 0x4530, 26, 29, 14, 13);
      }
    }
    Func_081434f8();
    Func_08158ce0(1, 4);
    work->transferPending = 1;
    WaitFrames(1);
  }
  REG_WIN0V = 0x1088;
  REG_WIN1V = 0x1088;
  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  Func_08143bb8();
}
