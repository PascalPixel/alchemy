// Draft, differ 330 of 615, skeleton gap 76 (tla-en, 2026-11-01)
// BattleEffect_RunStarMine: same length, calls, control flow and frame. What is
// left is reload's register choice, which sched2 then orders differently: the
// constants at the end of the mirror loops, the 0 kept in r5 across the first
// calls (here sched2 hoists the sheet's 0x700 into r5 instead), the homing
// arithmetic, where velocityX takes r2 instead of r3, and the drop burst's k
// spill. The stack slots of i * 8 and firstDrop are also swapped. Declaring the
// mirror counters inside their loops, sharing i with the half loop, the throw
// loop's own pointer and spark set inside its loop each closed a real gap.
// Tried with no gain: declaration orders of dx, dy, dz, a, b and k, shifts in
// the differences, a separate counter for half * 8, loop initialiser orders.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
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
void wait(s32 frames);
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
#define DROPS ((struct EffectStep *)Ram_MapCellBuffer)

// Abilities: Star Mine.
//
// Six stars (a 24 by 48 picture mirrored both ways from the sheet's two
// halves) are thrown from the caster, each one at a target in turn; from
// frame 8 * i + 11 each is drawn toward its target and bursts once it has
// passed it, into four sparks and 32 drops, pushing the target back.
void BattleEffect_RunStarMine(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sprites;
  DrawRectangle blitters[2];
  struct EffectPosition pos;
  struct EffectStep *star;
  struct EffectStep *spark;
  struct EffectStep *drop;
  s32 *camera;
  s32 delay;
  s32 firstDrop;
  s32 i = 0;

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
  for (; i != 2; i++) {
    u8 *picture = work->sheet + 0x1200 + i * 0x480;
    s32 y;

    for (y = 0; y != 24; y++) {
      s32 x;

      for (x = 0; x != 12; x++) {
        picture[y * 24 + x] = work->sheet[i * 576 + y * 24 + x * 2 + 1];
        picture[y * 24 + 23 - x] = work->sheet[i * 576 + y * 24 + x * 2];
        picture[(47 - y) * 24 + x] = work->sheet[i * 576 + y * 24 + x * 2 + 1];
        picture[(47 - y) * 24 + 23 - x] = work->sheet[i * 576 + y * 24 + x * 2];
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  work->transferMode = i;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  for (frame = 0; frame != STAR_MINE_FRAMES; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    if (frame == 0) {
      struct EffectStep *thrown;

      for (i = 0, thrown = work->particles; i != STARS; i++, thrown++) {
        struct StarTarget *caster = *GetBattleObjectSlotFar(effect->actor);
        s32 height = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
        s32 velocity;

        thrown->x = caster->x;
        thrown->y = caster->y + height;
        thrown->z = caster->z;
        velocity = ((Random16() & 255) - 128) << 8;
        thrown->velocityX = velocity;
        if (thrown->x > 0) {
          thrown->velocityX = -velocity;
        }
        thrown->velocityX = 0;
        thrown->velocityY = ((Random16() % 400) - 128) << 9;
        thrown->velocityZ = ((Random16() & 255) - 128) << 10;
        thrown->variant = 0;
      }
      for (i = 0; i != STAR_SPARKS; i++) {
        work->particles[STARS + i].variant = -1;
      }
      for (i = 0; i != STAR_DROPS; i++) {
        DROPS[i].variant = -1;
      }
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    for (i = 0, star = work->particles, delay = 0, firstDrop = 0; i != STARS;
         i++, star++, delay += 8, firstDrop += 32) {
      s32 member = i % effect->count;
      s32 k;
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
      if (frame <= delay + 10) {
        continue;
      }
      a = star->x;
      dy = target->y + height - star->y;
      dz = target->z - star->z;
      dx = target->x - a;
      star->velocityX += dx >> 10;
      star->velocityY += dy >> 10;
      star->velocityZ += dz >> 10;
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
      for (k = 0, spark = &work->particles[STARS + delay]; k != 4;
           k++, spark++) {
        spark->x = pos.x << 16;
        spark->y = pos.y << 16;
        spark->velocityX = ((Random16() & 127) - 64) << 12;
        spark->velocityY = (192 - (Random16() & 255)) << 10;
        spark->variant = 0;
      }
      for (k = 0, drop = &DROPS[firstDrop]; k != 32; k++, drop++) {
        s32 angle;
        s32 speed;

        drop->x = pos.x << 16;
        drop->y = pos.y << 16;
        angle = (Random16() & 0x7fff) + 0x4000;
        speed = (Random16() & 255) + 127;
        drop->velocityX = (Trig_Sin(angle) * speed) >> 5;
        drop->velocityY = (Trig_Cos(angle) * speed) >> 5;
        drop->variant = (Random16() & 15) + 16;
      }
      Func_081180e8(134);
      Sound_PlayFar(145);
      BattleEffect_StartActorMotion(effect->actors[member], 1, 0x20000, 0x80000,
                                0x20000, i * 5 + 110);
      Func_0814cd48(effect->actors[member], 7, 5, member, 16);
      work->shakeFrames = 4;
      star->variant = -1;
    }

    for (i = 0; i != STAR_SPARKS; i++) {
      spark = &work->particles[STARS + i];
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
    for (i = 0, drop = DROPS; i != STAR_DROPS; i++, drop++) {
      if (drop->variant >= 0) {
        s32 size = (drop->variant >> 3) + 1;

        blitters[0](canvas, sprites + gData_08197410[size - 1],
                    HI(drop->x) - size / 2, HI(drop->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(drop, 60, 0x1000);
        drop->variant--;
      }
    }
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
