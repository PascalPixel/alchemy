// Draft of BattleEffect_RunStarMine, decompiled from the listing: differ 567 of
// 615 (draft 608), the same 44 calls and control flow; allocation differs.

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
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);

struct StarTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};
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

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_BlueCrescentSheet;
extern char ResourceId_Data134;
extern char ResourceId_OrangePaletteB;
extern u8 gMapCellBuffer[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const s32 Data_081994a8[4]; // the early drops' drags
extern u8 gData_02014000[];        // the spray sheet

#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define BLIT_CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)
#define HI(v) (((s16 *)&(v))[1])
#define STAR_MINE_FRAMES 144
#define STARS 6
#define STAR_SPARKS 48
#define STAR_DROPS 192

// Abilities: Star Mine.
//
// Six stars (a 24 by 48 picture mirrored both ways from the sheet's two
// halves) are thrown from the caster, each one at a target in turn; from
// frame 8 * i + 11 each is drawn toward its target and bursts once it has
// passed it, into four sparks and 32 drops, pushing the target back.
void BattleEffect_RunStarMine(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  DrawRectangle blitters[2];
  struct EffectPosition pos;
  struct EffectStep *drops = (struct EffectStep *)gMapCellBuffer;
  struct EffectStep *star;
  struct EffectStep *spark;
  struct EffectStep *drop;
  s32 *camera;
  s32 frame;
  s32 half;
  s32 i;
  s32 k;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet, work->sheet, 1,
                             1);

  // The star: each half of the sheet's 24-wide picture mirrored left to
  // right and top to bottom.
  for (half = 0; half != 2; half++) {
    u8 *picture = work->sheet + 0x1200 + half * 0x480;

    for (i = 0; i != 24; i++) {
      u8 *source = work->sheet + half * 576 + i * 24;
      u8 *top = picture + i * 24;
      u8 *bottom = picture + (47 - i) * 24;

      for (k = 0; k != 12; k++) {
        top[k] = source[1];
        top[23 - k] = source[0];
        bottom[k] = source[1];
        bottom[23 - k] = source[0];
        source += 2;
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  work->transferMode = half;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  for (frame = 0; frame != STAR_MINE_FRAMES; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    if (frame == 0) {
      for (i = 0, star = work->particles; i != STARS; i++, star++) {
        struct StarTarget *caster = *GetBattleObjectSlotFar(effect->actor);
        s32 height = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
        s32 velocity;

        star->x = caster->x;
        star->y = caster->y + height;
        star->z = caster->z;
        velocity = ((Random16() & 255) - 128) << 8;
        star->velocityX = velocity;
        if (star->x > 0) {
          star->velocityX = -velocity;
        }
        star->velocityX = 0;
        star->velocityY = ((Random16() % 400) - 128) << 9;
        star->velocityZ = ((Random16() & 255) - 128) << 10;
        star->variant = 0;
      }
      for (i = 0; i != STAR_SPARKS; i++) {
        work->particles[STARS + i].variant = -1;
      }
      for (i = 0; i != STAR_DROPS; i++) {
        drops[i].variant = -1;
      }
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    for (i = 0, star = work->particles; i != STARS; i++, star++) {
      s32 member = i % effect->count;
      struct StarTarget *target =
          *GetBattleObjectSlotFar(effect->actors[member]);
      s32 height = BattleUnit_GetEffectHeightFar(effect->actors[member]) / 2;
      s32 picture;
      s32 dx;
      s32 dy;
      s32 dz;
      s32 a;
      s32 b;

      if (star->variant < 0) {
        continue;
      }
      picture = ((frame + i) / 2) & 1;
      EffectPosition_ProjectPoint(star, &pos);
      pos.x >>= 1;
      blitters[0](canvas, work->sheet + 0x1200 + picture * 0x480, pos.x - 12,
                  pos.y - 24, 24, 48);
      BattleEffectKernels_IntegrateVector3(star, 63, 0);
      if (frame <= i * 8 + 10) {
        continue;
      }
      dy = target->y + height - star->y;
      dz = target->z - star->z;
      dx = target->x - star->x;
      star->velocityX += dx >> 10;
      star->velocityY += dy >> 10;
      star->velocityZ += dz >> 10;
      a = star->x;
      if (a < 0) {
        a = -a;
      }
      b = target->x;
      if (b < 0) {
        b = -b;
      }
      if (a <= b) {
        continue;
      }

      // Passed its target: the burst.
      for (k = 0, spark = &work->particles[STARS + i * 8]; k != 4;
           k++, spark++) {
        spark->x = pos.x << 16;
        spark->y = pos.y << 16;
        spark->velocityX = ((Random16() & 127) - 64) << 12;
        spark->velocityY = (192 - (Random16() & 255)) << 10;
        spark->variant = 0;
      }
      for (k = 0, drop = &drops[i * 32]; k != 32; k++, drop++) {
        s32 angle;
        s32 speed;

        drop->x = pos.x << 16;
        drop->y = pos.y << 16;
        angle = (Random16() & 0x7fff) + 0x4000;
        speed = (Random16() & 255) + 127;
        drop->velocityX = (speed * Trig_Sin(angle)) >> 5;
        drop->velocityY = (speed * Trig_Cos(angle)) >> 5;
        drop->variant = (Random16() & 15) + 16;
      }
      Func_081180e8(134);
      Audio_PlayCue(145);
      BattleEffect_StartActorMotion(effect->actors[member], 1, 0x20000, 0x80000,
                                0x20000, i * 5 + 110);
      Func_0814cd48(effect->actors[member], 7, 5, member, 16);
      work->shakeFrames = 4;
      star->variant = -1;
    }

    for (i = 0, spark = &work->particles[STARS]; i != STAR_SPARKS;
         i++, spark++) {
      if ((u32)spark->variant <= 23) {
        if (spark->variant > 1) {
          blitters[0](canvas,
                      work->sheet + 0x4000 + ((spark->variant / 4) << 11),
                      HI(spark->x) - 16, HI(spark->y) - 32, 32, 64);
        }
        BattleEffectKernels_IntegrateVector2(spark, 60, -0x1000);
        spark->variant++;
      }
    }
    for (i = 0, drop = drops; i != STAR_DROPS; i++, drop++) {
      if (drop->variant >= 0) {
        s32 size = (drop->variant >> 3) + 1;

        blitters[0](canvas, sprites + gData_08197410[size - 1],
                    HI(drop->x) - size / 2, HI(drop->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(drop, 60, 0x1000);
        drop->variant--;
      }
    }
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}
