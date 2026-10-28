// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 8 of
// 374 with an empty asm the greedy device search placed before the stack
// offset's reload (10 without it): the second word's copy to fp after the
// store to sp+40, the loop's 0 and its copy to r9 after the two stack
// pointers' stores, and the reload of sp+8 after the store to sp+16.
// Draft of BattleEffect_RunStoneJustice, effect kind 100 (Stone Justice), written from the
// listing on The Broken Seal's canvas skeleton. No close twin (shape
// BattleEffect_RunSpire 0.502, call order BattleEffect_RunPrism
// 0.680); the shard drawing follows BattleEffect_RunSpire, and the
// shard tables are its gSpire_ShardWidths, Heights and Cells.

// Listing 374 instructions, draft 374, differ 14 (2026-10-18).
// Left: order rows only. The native stores the third argument before it
// copies the second to fp, loads the BLDCNT zero before it starts the
// sheet offset (224), loads the blitter slot before its offset, sets frame
// (r9) after the two position pointers, and starts REG_BG2X's address
// before it reads the user's x.
// Tried (differ):
// first draft, plain HI macro, pointer-stepped rock loops          372
// the two positions declared user first (their stack slots)        366
// scrollSpeed += 0x8000 then * 60 / 64 (two statements; one fold
// of the constant into the multiply otherwise)                    356
// width and height as locals before the call                        352
// the throw loop indexed (&work->particles[i])                       332
// both rock loops indexed, an explicit height accumulator,
// direct work->particles[i] fields                          332-384
// HI as ((struct Half *)&v)->hi, *(s16 *)((u8 *)&v + 2),
// (s16)(v >> 16), *((s16 *)&v + 1)                                332
// HI through an inline helper (FAKEMATCH below)                       30
// alchemy permute 8 minutes, 2 jobs (found the helper)                30
// the BG2PA halfword pinned in a one-pass block (FAKEMATCH)         24
// the height read where the call uses it (no local)                 18
// BattleEffect_LoadBlitter returning s32, as defined                    16
// an empty asm after the REG_BG2X store (FAKEMATCH)                 14
// the BLDCNT zero pinned as BG2PA's (16), an empty asm after the
// blitter slot read (22), position pointers before the loop (36),
// the frame loop as do-while (14), alchemy permute 5 minutes (14)
// The helper is a device here: BattleEffect_RunPlumeEdge goes from 36 to 135 with
// it, and BattleEffect_RunHurricane from 218 to 212.

#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "system.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "motobj.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
/* FAKEMATCH: the halves are read through an inline helper. Through the
 * cast macro ((s16 *)&v)[1], or any plain form tried (see the header), the
 * rock loop keeps i in r8 and spills the hit flag, and the stack frame grows by a
 * word (differ 332); through the helper the allocation is the native's
 * (differ 30). */
static inline s16 *Fixed_Halves(s32 *value) { return (s16 *)value; }
#define HI(v) (Fixed_Halves(&(v))[1])

// The Lost Age numbers the blitter modes from the other end of The Broken
// Seal's COMPOSITE_* list; the packed loader argument is mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define ROCKS 21

extern char ResourceId_BoulderSheet;

// The Broken Seal's gSpire_ShardWidths, ShardHeights and ShardCells.
extern u8 gData_0819749e[];
extern u8 gData_081974ad[];
extern u16 gData_081974bc[];

// The canvas layer and its helpers, with The Broken Seal's names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
s32 BattleEffect_LoadBlitter(s32 slot, s32 blitter); // BattleEffect_BuildBlitter, packed
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);          // ObjectGroup_UpdateMembers
void ObjectGroup_CountDownMemberTimers(void);                // ObjectGroup_CountDownMemberTimers
void Func_08158ce0(s32 mask, u32 range); // Camera_ApplyShake
void BattleEffect_StartActorMotion(s32 actor, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void ObjectDispatch_SetAnimationSpeedFar(struct MotionObject *object,
                                            s32 value);
void Object_SetMode(struct MotionObject *object, s32 mode);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Audio_PlayCue(s32 cue);

// Stone Justice: 21 boulders rise in a column beside the user and tumble
// across the screen into the target, while the battle background slides
// with them.
//
// Setup: canvas layer 1 at full width, the boulder sheet, one transparent
// blitter, and the user's sprite group set apart (48) for the length of
// the effect.
// Frame loop (107 frames): the boulders are thrown at frame 0 and spin as
// they fly; from frame 64 the background scrolls after them and each
// boulder turns toward the target; every boulder that reaches the target
// shakes the screen and knocks the target back.
void BattleEffect_RunStoneJustice(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  s32 scroll;
  s32 scrollSpeed;
  s32 cueTimer;
  struct MotionObject *user;
  s32 scrollX;
  struct EffectPosition userPosition;
  struct EffectPosition targetPosition;
  struct EffectStep *rock;
  s32 frame;
  s32 hit;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  {
    /* FAKEMATCH: the native loads the BG2PA halfword from the pool before it
     * starts the register's address; pinned to r3 in a one-pass block it
     * does (differ 22 without it). */
    register u16 value __asm__("r3");

    do {
      value = 0x100;
    } while (0);
    REG_BG2PA = value;
  }
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(104,
                BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 1;
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  scroll = 0;
  scrollSpeed = 0;
  cueTimer = 0;
  user = GetBattleObjectSlotFar(effect->actor)->object;
  ObjectDispatch_SetAnimationSpeedFar(user, 48);

  for (frame = 0; frame != 107; frame++) {
    EffectPosition_ProjectScaledBattler(effect->actor, &userPosition);
    EffectPosition_ProjectScaledBattler(effect->actors[0], &targetPosition);
    if (frame == 0)
      Audio_PlayCue(157);
    if (frame == 64)
      Audio_PlayCue(104);

    // Frame 0: the boulders stand in a column 2 pixels apart, on the
    // user's side of the canvas, and are thrown upward at random speeds.
    if (frame == 0) {
      for (i = 0; i != ROCKS; i++) {
        rock = &work->particles[i];
        if (effect->side == 0)
          rock->x = (Random16() % 40 + 8) << 16;
        else
          rock->x = (120 - Random16() % 40) << 16;
        Random16();
        rock->y = (i * 2 + 96) << 16;
        rock->velocityX = 0;
        rock->velocityY = -((Random16() & 127) + 32) << 11;
        rock->z = (Random16() & 15) + 8;
        rock->velocityZ = 0;
        rock->variant = 0;
      }
    }
    if (frame == 52)
      Object_SetMode(user, 2);

    // From frame 64 the background follows the boulders, ever faster.
    if (frame > 64) {
      if (effect->side == 0)
        scroll += scrollSpeed;
      else
        scroll -= scrollSpeed;
      scrollSpeed += 0x8000;
      scrollSpeed = scrollSpeed * 60 / 64;
    }
    scrollX = scroll >> 16;
  __asm__("");
    REG_BG2X = (64 - (userPosition.x - scrollX)) << 8;
    /* FAKEMATCH: the native stores REG_BG2X before it clears the hit flag
     * (differ 16 without the barrier). */
    __asm__("");

    // Each boulder spins through three pictures of its shard (z is its
    // spin rate, velocityZ the angle) and hits the target once it passes
    // it.
    hit = 0;
    for (i = 0; i != ROCKS; i++) {
      s32 cell;
      u32 width;

      rock = &work->particles[i];
      cell = (i % 5) * 3 + rock->velocityZ / 96 % 3;
      width = gData_0819749e[cell];

      blit(canvas, work->sheet + gData_081974bc[cell] + 0x83c,
           HI(rock->x) - width / 2 + scrollX,
           HI(rock->y) - gData_081974ad[cell] / 2, width,
           gData_081974ad[cell]);
      if (rock->variant == 0) {
        if (effect->side == 0) {
          if (targetPosition.x > HI(rock->x) + userPosition.x - 64) {
            rock->variant = 1;
            hit = 1;
          }
        } else {
          if (targetPosition.x < HI(rock->x) + userPosition.x - 64) {
            rock->variant = 1;
            hit = 1;
          }
        }
      }
      BattleEffectKernels_IntegrateVector2(rock, 60, 0);
      rock->velocityZ += rock->z;
      if (frame > i / 2 + 64) {
        if (effect->side == 0)
          rock->velocityX -= 0x8000;
        else
          rock->velocityX += 0x8000;
        if (rock->z > 1)
          rock->z--;
      }
    }

    // A hit flashes and pushes the target; the impact sound plays at most
    // every eight frames.
    if (cueTimer > 0)
      cueTimer--;
    if (hit == 1) {
      if (cueTimer == 0) {
        cueTimer = 8;
        Audio_PlayCue(134);
      }
      BattleEffect_StartActorMotion(effect->actors[0], 1, 2 << 16, 4 << 16, 2 << 16,
                                150);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
      work->shakeFrames = 2;
    }

    Func_08158ce0(2, 4);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  ObjectDispatch_SetAnimationSpeedFar(user, 16);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}
