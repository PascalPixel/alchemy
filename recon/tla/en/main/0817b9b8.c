// Draft (tla-en): listing 436 instructions, draft 436, common 424, differ 24
// (2026-10-26: the pinned BLDALPHA value and its one-pass block are gone,
// under -mthumb-slow-loads the plain store measures the same;
// BattleEffect_LoadBlitter returns s32, as nibble_pair.c defines it, 46 to
// 44; the star drawing loop walks a pointer of its own, which the native
// keeps in r5, 44 to 24). Left: the star base (work + 0x380) is built
// after the projection call here and before the divisions in the native,
// and order rows of one or two instructions.
// Written from the listing (☀️'s BattleEffect_RunJupiter is only a
// hint). The pink star burst: 32 smoke puffs rise from the caster for 24
// frames, the caster rises and leans to the target, and at frame 32 the
// target bursts into 16 pink stars for 24 frames.
// 2026-10-19 (run 33): the unreferenced slot is the blitter pair's second
// half (tagged below), as in 0817c27c; with it "stars" goes and each star
// loop starts at &particles[SMOKE_COUNT] as the native rebuilds it: 88 to
// 70; the smoke drawing loop on a pointer of its own (r5, the native's;
// the spawn loop keeps r7): 54. What is left is scheduling: the native's
// loads one row before their neighbouring constants, BattleEffect_LoadBlitter's
// 3 before its 104, and the star base built between the divisions.
// Earlier notes, from before the pair:
// - the native stack frame is 60 bytes with an unreferenced slot at sp+20
// between frame (sp+24) and draw (sp+16); 0817c27c has the same unused
// slot between the same two. Here "stars" (&particles[32], set once before
// the loop) takes that slot, but it is stored and loaded where the
// native rebuilds 0x380 + r9 at each loop. Without it the stack frame is 56
// and every stack offset moves (differ 132). Tried for the slot: the
// step or source pointer, angle or speed at function scope, a particles
// pointer, stars set in each block.
// - after BattleEffect_StartCanvasLayer, REG_BLDALPHA's 0x1010 is loaded from
// the pool before the register address: a u16 pinned to r3 with a
// one-pass block fixes it (the empty asm of f18284c.c instead moves the
// halfword pool by four estimated bytes). Another one-pass block before
// the heap loads puts effect's spill first, as the native does.
// - the native sets BattleEffect_LoadBlitter's 3 before its 104.
// - in the frame-32 block the native schedules the 0x380 + r9 of the
// burst loop between the divisions and loads target->x after the
// division's 100 (each load comes one row later after a store).
// 2026-10-15 (lane N2): rewritten with the work struct, named constants and
// comments, the devices tagged; the same 436 instructions and differ 88.
// The always-true "frame >= 0" test is a device: without it differ 105.
// Resources 302 (BattleEffect_PinkStarSheet) and 353 (BattleEffect_SmokeSheet)
// need their names in system/resource/directory.s when this is adopted.
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "types.h"

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
s32 Task_Add(s32 callback, s32 priority);
s32 Task_Remove(s32 callback);
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
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
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_CountDownMemberTimers(void);
void WaitFrames(s32 frames);
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
  /* FAKEMATCH(frame): the blitter as a pair of which only the first is used;
   * as one two-word DImode pseudo its spill takes sp+16 and the unused sp+20
   * the native stack frame has. As a single DrawRectangle the stack frame is 56 bytes
   * and every stack offset moves (132 of 436 instructions differ in tla-en
   * without "stars", 88 with it, 70 with the pair). */
  DrawRectangle draw[2];
  struct MotionObject *targetObject;
  struct MotionObject *casterObject;
  u8 *battle;
  struct EffectStep *step;
  s32 i;

  /* FAKEMATCH: the native spills the effect argument before it loads the
   * heap slots; this one-pass block ends the block there (differ 2 more
   * without it). */
  do {
  } while (0);
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                             work->sheet + STAR_CELS, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_ADDITIVE, CLIP_XY));
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
      for (i = 0, step = work->particles; i != SMOKE_COUNT; i++, step++) {
        s32 angle;
        s32 speed;

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
    /* FAKEMATCH: an always-true test; it keeps the smoke loop and the
     * caster's placing in one block as the native has them (without it
     * three instructions fewer and differ 105). */
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
      for (i = 0, step = &work->particles[SMOKE_COUNT]; i != STAR_COUNT; i++, step++) {
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

      for (i = 0, star = &work->particles[SMOKE_COUNT]; i != STAR_COUNT; i++, star++) {
        if (star->variant >= 0) {
          draw[0](canvas, work->sheet + STAR_CELS + (((frame + i) / 4) % 6) * 256,
               HI(star->x) - 8, HI(star->y) - 8, 16, 16);
          BattleEffectKernels_IntegrateVector2(star, 56, 0);
          star->variant--;
        }
      }
    }

    Func_08158ce0(4, 4);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  battle[0x866] = 0;
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
