#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);
void Render_ResetTransformState(void);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
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
void Func_081180e8(s32 phase);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
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
extern const u16 gData_08197410[];

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
  struct EffectStep *particle;
  struct EffectStep *drop;
  s32 *camera;
  s32 delay;
  s32 member;
  s32 firstDrop;
  s32 i;

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
  for (i = 0; i != 2; i++) {
    u8 *picture = work->sheet + 0x1200 + i * 24 * 48;
    s32 y;

    for (y = 0; y != 24; y++) {
      s32 x;

      for (x = 0; x != 12; x++) {
        picture[y * 24 + x] = work->sheet[i * 24 * 24 + y * 24 + x * 2 + 1];
        picture[y * 24 + 23 - x] = work->sheet[i * 24 * 24 + y * 24 + x * 2];
        picture[(47 - y) * 24 + x] =
            work->sheet[i * 24 * 24 + y * 24 + x * 2 + 1];
        picture[(47 - y) * 24 + 23 - x] =
            work->sheet[i * 24 * 24 + y * 24 + x * 2];
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  work->transferMode = 2; // keeps 50 or 75 percent (see transferMode)
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
        // DEAD: both stores are overwritten by the 0 below; the ROM keeps them.
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
         star++, delay += 8, firstDrop += 32, i++) {
      s32 k;
      struct StarTarget *target;
      s32 height;
      s32 picture;
      s32 dx;
      s32 dy;
      s32 dz;
      s32 a;
      s32 b;

      member = i % effect->count;
      target = *GetBattleObjectSlotFar(effect->actors[member]);
      height = BattleUnit_GetEffectHeightFar(effect->actors[member]) / 2;
      if (star->variant < 0) {
        continue;
      }
      picture = ((frame + i) / 2) & 1;
      EffectPosition_ProjectPoint(star, &pos);
      pos.x >>= 1;
      blitters[0](canvas, work->sheet + 0x1200 + picture * 24 * 48, pos.x - 12,
                  pos.y - 24, 24, 48);
      BattleEffectKernels_IntegrateVector3(star, 63, 0);
      if (frame <= delay + 10) {
        continue;
      }
      dx = (target->x - star->x) >> 10;
      dy = (target->y + height - star->y) >> 10;
      dz = (target->z - star->z) >> 10;
      star->velocityX += dx;
      star->velocityY += dy;
      star->velocityZ += dz;
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
      for (k = 0; k != 4; k++) {
        particle = &work->particles[STARS + delay + k];
        particle->x = pos.x << 16;
        particle->y = pos.y << 16;
        particle->velocityX = ((Random16() & 127) - 64) << 12;
        particle->velocityY = (192 - (Random16() & 255)) << 10;
        particle->variant = 0;
      }
      for (k = 0; k != 32; k++) {
        s32 angle;
        s32 speed;

        drop = &DROPS[firstDrop + k];
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
      particle = &work->particles[STARS + i];
      if ((u32)particle->variant <= 23) {
        if (particle->variant > 1) {
          blitters[0](canvas,
                      work->sheet + 0x4000 + ((particle->variant / 4) << 11),
                      HI(particle->x) - 16, HI(particle->y) - 32, 32, 64);
        }
        BattleEffectKernels_IntegrateVector2(particle, 60, -0x1000);
        particle->variant++;
      }
    }
    for (i = 0; i != STAR_DROPS; i++) {
      particle = &DROPS[i];
      if (particle->variant >= 0) {
        s32 size = (particle->variant >> 3) + 1;

        blitters[0](canvas, sprites + gData_08197410[size - 1],
                    HI(particle->x) - size / 2, HI(particle->y) - size, size,
                    size * 2);
        BattleEffectKernels_IntegrateVector2(particle, 60, 0x1000);
        particle->variant--;
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
