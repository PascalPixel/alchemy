#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_action.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "types.h"
#include "task.h"

#define HEAP_SLOT_BLITTER 104 // Ram_HeapSlots->unk64[1]

// The Lost Age's blitter modes run the other way from The Broken Seal's:
// additive 0, max 1, transparent 2, copy 3.
#define BLITTER_ADDITIVE 0
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP_XY 3

#define STAR_FRAMES 60
#define STAR_LEAP_FRAME 16  // the caster leaps at the target
#define STAR_BURST_FRAME 32 // and strikes it
#define SMOKE_COUNT 32
#define STAR_COUNT 16
#define STAR_CELS 0x3e80 // the pink star's six 16x16 cels, after the smoke's

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_PinkStarSheet;
extern char ResourceId_SmokeSheet;

void BattleEffect_StartCanvasLayer(s32 bgControl);
void *Resource_GetTableEntry(s32 resourceId);
s32 Task_Add(void (*func)(void), s32 order);

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
s32 BattleEffect_LoadBlitter(s32 slot, s32 blitter);
void BattleObject_SetRecordPriority(s32 id, s32 priority);
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void HeapSlot_Release(s32 slot);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_EndCanvasLayer(void);

// A pink star burst. Smoke puffs from the caster for its first 24 frames
// while the caster stands just past the target; at frame 16 it leaps at
// the target, and at frame 32 the palette turns pink, the caster lands
// beside the target, which is knocked back and flashes, and sixteen pink
// stars burst from it for 24 frames. Battle work byte 0x866 is set for the
// whole effect.
void BattleEffect_RunBackstab(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  u8 *canvas;
  struct EffectPosition caster;
  struct EffectPosition target;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  struct MotionObject *targetObject;
  struct MotionObject *casterObject;
  u8 *battle;
  struct EffectStep *step;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                             work->sheet + STAR_CELS, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER,
                           BLITTER(BLITTER_ADDITIVE, CLIP_XY));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  targetObject = GetBattleObjectSlotFar(effect->actors[0])->object;
  casterObject = GetBattleObjectSlotFar(effect->actor)->object;
  battle = (u8 *)Ram_HeapSlots->battleWork;
  battle[0x866] = 1;

  for (frame = 0; frame != STAR_FRAMES; frame++) {
    BattleObject_SetRecordPriority(effect->actor, 3);

    // The first frame sends the smoke out from the caster's feet.
    if (frame == 0) {
      EffectPosition_ProjectBattler(effect->actor, &caster);
      for (i = 0; i != SMOKE_COUNT; i++) {
        s32 angle;
        s32 speed;

        step = &work->particles[i];
        step->variant = 0;
        step->x = (caster.x / 2) << 16;
        step->y = (caster.y - 8) << 16;
        angle = Random16();
        speed = (Random16() & 255) + 127;
        step->velocityX = (Trig_Sin(angle) * speed) >> 5;
        step->velocityY = (Trig_Cos(angle) * speed) >> 5;
      }
      Sound_PlayFar(136);
    }

    // The smoke rises through its six 24x48 cels, slowing; the caster
    // starts 160 percent of the way out to the target.
    // DEAD: always true, as frame counts up from 0; the ROM keeps the test.
    if (frame >= 0) {
      struct EffectStep *puff;

      for (i = 0, puff = work->particles; i != 24; i++, puff++) {
        if (frame >= i / 8 && puff->variant <= 23) {
          draw[0](canvas, work->sheet + (puff->variant / 4) * 0x480,
                  HI(puff->x) - 12, HI(puff->y) - 24, 24, 48);
          BattleEffectKernels_IntegrateVector2(puff, 48, -0x2000);
          puff->variant++;
        }
      }
      if (frame == 0) {
        casterObject->z = targetObject->z;
        casterObject->x = targetObject->x * 160 / 100;
        casterObject->angle = targetObject->angle;
      }
    }

    if (frame == STAR_LEAP_FRAME) {
      BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 16, 120);
      casterObject->velocityY = 7 << 16;
    }

    // The strike: pink palette, the caster lands at 120 percent and the
    // target is pushed to 80, and the stars burst upward from it.
    if (frame == STAR_BURST_FRAME) {
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_PinkStarSheet),
                      128);
      casterObject->z = targetObject->z;
      casterObject->x = targetObject->x * 120 / 100;
      targetObject->x = targetObject->x * 80 / 100;
      casterObject->angle = targetObject->angle;
      EffectPosition_ProjectScaledBattler(effect->actors[0], &target);
      for (i = 0, step = &work->particles[SMOKE_COUNT]; i != STAR_COUNT;
           i++, step++) {
        s32 angle;
        s32 speed;

        step->variant = (Random16() & 15) + 16;
        step->x = (target.x / 2) << 16;
        step->y = (target.y - 8) << 16;
        angle = (Random16() & 0x7fff) + 0x4000;
        speed = (Random16() & 255) + 127;
        step->velocityX = (Trig_Sin(angle) * speed) >> 7;
        step->velocityY = (Trig_Cos(angle) * speed) >> 5;
      }
      work->shakeFrames = 4;
      Func_081180e8(134);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
      BattleEffect_StartActorMotion(effect->actors[0], 1, 8 << 16, 20 << 16,
                                    6 << 16, 10);
    }

    // The stars twinkle through their six cels as they fly.
    if (frame >= STAR_BURST_FRAME && frame < STAR_BURST_FRAME + 24) {
      struct EffectStep *star;

      for (i = 0, star = &work->particles[SMOKE_COUNT]; i != STAR_COUNT;
           i++, star++) {
        if (star->variant >= 0) {
          draw[0](canvas,
                  work->sheet + STAR_CELS + (((frame + i) / 4) % 6) * 256,
                  HI(star->x) - 8, HI(star->y) - 8, 16, 16);
          BattleEffectKernels_IntegrateVector2(star, 56, 0);
          star->variant--;
        }
      }
    }

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  {
    // A local for the 0, so the address is built first, in r2.
    s32 cleared = 0;

    battle[0x866] = cleared;
  }
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
