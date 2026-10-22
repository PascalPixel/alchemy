// Draft of BattleEffect_RunVoidBeam, decompiled from the listing: differ 851 of 743
// (draft 752), the same 45 calls and control flow; allocation differs.
// 2026-10-19: the native frame is 248 bytes (here 184 at first). Its
// arrays are point at sp 236, then 132 bytes of corners from sp 104 (11
// points, of which the hexagon uses 6), then the ring and the segment;
// its two blitters are one pair stored together (sp 56 and 60); frame is
// declared after canvas, camera before sprites. With all that the frame
// is 244: the native spills one pseudo more below sprites (11 slots
// against 10 here), so every offset is still 4 off.

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

void BattleEffect_BeginCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(
    void); // BattleEffect_ProcessPendingGraphicsTransfer
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
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle);            // SceneTransform_ApplyRoll
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

void BattlePres_SetupTransitionSceneFar(s32 a, s32 b, s32 c, s32 d);
void Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                   s32 mode);
void Camera_ApplyPhasedDelta(void); // the camera's sway

struct VoidCaster {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

extern char ResourceId_EmberStreakSheet;
extern char ResourceId_CometSheetB;
extern char ResourceId_Data134;
extern char ResourceId_RuneSheet;
extern char ResourceId_LightningBoltSheet;
extern u8 gMapCellBuffer[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const s8 Data_08196e9c[];   // the hexagon's corners (z, y)

#define VOID_BEAM_FRAMES 88
#define BEAM_SEGMENTS 10

// Abilities: Void Beam.
//
// A hexagon of motes is traced, side by side, around the caster while a
// ring of 64 gathers behind; at frame 72 a beam (a 32-line gradient,
// repeated ten times) shoots from the caster toward the targets' side,
// fading out from frame 80 with the blend.
void BattleEffect_RunVoidBeam(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 *camera;
  u8 *sprites;
  struct VoidCaster *caster;
  s32 point[3];
  struct EffectPosition corners[11];
  struct EffectPosition ring;
  struct EffectPosition segment;
  s32 flip;
  s32 scroll;
  s32 twice;
  s32 count;
  u8 *row;
  s32 i;
  s32 k;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB, work->sheet + 0x1000,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);

  // The motes, one shade dimmer.
  for (i = 0; i != 770; i++) {
    s32 pixel = sprites[i];

    if (pixel != 0) {
      pixel -= 8;
      if (pixel <= 0) {
        pixel = 1;
      }
      sprites[i] = pixel;
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet,
                             gMapCellBuffer + 0x1000, 1, 0);
  Iwram_ClearWords(gMapCellBuffer, 0x1000);

  // The beam's head: the rune, mirrored to 48 columns.
  for (i = 0; i != 48; i++) {
    u8 *source = gMapCellBuffer + 0x1000 + i * 24;
    u8 *left = gMapCellBuffer + 0x208 + i * 64;
    u8 *right = gMapCellBuffer + 0x237 + i * 64;

    for (k = 0; k != 24; k++) {
      *left++ = *source;
      *right-- = *source++;
    }
  }
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                  128);

  // The beam's body: a 32-line gradient.
  row = work->sheet + 0x1a20;
  for (i = 0; i != 32; i++) {
    s32 shade = (Trig_Sin(i << 9) * 63) >> 16;

    for (k = 0; k != 32; k++) {
      *row++ = shade;
    }
  }
  REG_BLDALPHA = 0x1010;
  flip = 4;
  if (effect->side != 0) {
    flip = 0;
  }
  Func_08196404(SLOT_BLITTER, 7, 7, flip | 3, 1);
  Func_08196404(188, 7, 7, flip | 11, 1);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Scheduler_AddOrUpdateCallback(Camera_ApplyPhasedDelta, 0x480);
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Audio_PlayCue(140);

  for (frame = 0, scroll = 0; frame != VOID_BEAM_FRAMES; frame++, scroll += 8) {
    s32 traced;

    if ((u32)(frame - 9) <= 54) {
      work->cameraDelta = -256;
    } else if ((u32)(frame - 64) <= 15) {
      work->cameraDelta = frame * 16 - 1280;
    } else {
      work->cameraDelta = 0;
    }
    if (effect->side == 1) {
      work->cameraDelta = -work->cameraDelta;
    }
    if (frame > 79) {
      twice = frame * 2;
      REG_BLDALPHA = (16 - (twice - 160)) | 0x1000;
    } else {
      twice = frame * 2;
    }
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    caster = *GetBattleObjectSlotFar(effect->actor);
    if (caster->x > 0) {
      point[0] = caster->x - 0x80000;
    } else {
      point[0] = caster->x + 0x80000;
    }

    // The hexagon, one side every 36 frames.
    traced = scroll - 336;
    if (traced >= 0) {
      for (i = 0; i != 6; i++) {
        s32 corner = (i + 4) % 6;

        point[1] = (Data_08196e9c[corner * 2 + 1] << 16) + 0x1e0000;
        point[2] = (Data_08196e9c[corner * 2] << 16) + caster->z;
        EffectPosition_ProjectPoint(point, &corners[i]);
        corners[i].x >>= 1;
      }
      for (i = 0; i != 6; i++) {
        s32 motes = traced - i * 36;

        if (motes > 36) {
          motes = 36;
        }
        if (motes < 0) {
          continue;
        }
        for (k = 0; k != motes; k++) {
          s32 next = (i + 1) % 6;
          s32 x = corners[i].x +
                  Iwram_MulQ16(k * (corners[next].x - corners[i].x), 1820);
          s32 y = corners[i].y +
                  Iwram_MulQ16(k * (corners[next].y - corners[i].y), 1820);

          draw[0](canvas, sprites + gData_08197410[3], x - 2, y - 4, 4, 8);
        }
      }
    }

    // The ring behind the caster.
    count = (twice + frame) / 2;
    if (count > 64) {
      count = 64;
    }
    for (i = 0; i != count; i++) {
      s32 angle = i << 10;

      point[1] = caster->y - Trig_Cos(angle) * 40 + 0x200000;
      point[2] = caster->z + Trig_Sin(angle) * 40;
      EffectPosition_ProjectPoint(point, &ring);
      ring.x >>= 1;
      if (ring.depth <= 159) {
        ring.depth = 160;
      }
      if (ring.depth > 799) {
        ring.depth = 799;
      }
      draw[1](canvas, sprites + gData_08197410[3], ring.x - 2, ring.y - 4, 4,
              8);
    }

    // The beam.
    if (frame > 71) {
      s32 fade;

      if (frame == 72) {
        work->particles[0].x = point[0];
        if (point[0] > 0) {
          work->particles[0].velocityX = -0x200000;
        } else {
          work->particles[0].velocityX = 0x200000;
        }
        REG_BG2CNT = 0x784;
      }
      if (frame == 72) {
        work->shakeFrames = 8;
        Func_081180e8(145);
        Func_08118088(effect->actors[0], 4);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      }
      for (i = 0; i != BEAM_SEGMENTS; i++) {
        point[0] = work->particles[0].x + i * work->particles[0].velocityX;
        point[1] = 0x1e0000;
        point[2] = caster->z;
        EffectPosition_ProjectPoint(point, &segment);
        segment.x >>= 1;
        if (effect->side == 0) {
          segment.x -= 32;
        }
        if (i > 0) {
          draw[0](canvas, work->sheet + 0x1a20, segment.x, segment.y - 32, 32,
                  32);
          draw[1](canvas, work->sheet + 0x1a20, segment.x, segment.y, 32, 32);
        } else if (frame <= 81) {
          draw[0](canvas, work->sheet + 0x1000, segment.x, segment.y - 36, 32,
                  36);
          draw[1](canvas, work->sheet + 0x1000, segment.x, segment.y, 32, 36);
        }
      }
      row = work->sheet + 0x1a20;
      fade = 0;
      if (frame > 79) {
        fade = scroll - 640;
      }
      for (i = 0; i != 32; i++) {
        s32 shade = ((Trig_Sin(i << 9) * 63) >> 16) - fade;

        if (shade < 0) {
          shade = 0;
        }
        for (k = 0; k != 32; k++) {
          *row++ = shade;
        }
      }
    }
    if (work->cameraPhase == 0) {
      work->cameraPhase = 1;
    }
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(Camera_ApplyPhasedDelta);
  BattleEffect_EndCanvasLayer();
}
