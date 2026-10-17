// Draft of Func_0816d3d8, effect kind 100 (Stone Justice), written from the
// listing on The Broken Seal's canvas skeleton. No close twin (shape
// BattleFx_RunShatteringRocks 0.502, call order BattleFx_RunFallingCrystals
// 0.680); the shard drawing follows BattleFx_RunShatteringRocks, and the
// shard tables are its gShatterRocks_ShardWidths, Heights and Cells.

// Listing 374 instructions, draft 374, differ 30.
// Left: order rows only. The native loads the REG_BG2PA value from the
// pool before it starts the register address, sets r1 (224) before r0 at
// the first load and r0 (104) before r1 at the blitter, loads the blitter
// slot before its offset, sets frame (r9) after the two position pointers,
// and loads the height table after the width's shift; the pool offsets
// move with them (about 12 rows).
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
// The helper is a device here: Func_0816aa08 goes from 36 to 135 with
// it, and Func_0816f928 from 218 to 212.

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
 * rock loop keeps i in r8 and spills the hit flag, and the frame grows by a
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

// The Broken Seal's gShatterRocks_ShardWidths, ShardHeights and ShardCells.
extern u8 gData_0819749e[];
extern u8 gData_081974ad[];
extern u16 gData_081974bc[];

// The canvas layer and its helpers, with The Broken Seal's names.
void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143bb8(void); // BattleFx_EndCanvasLayer
void Func_08143000(void); // BattleFx_ProcessPendingGraphicsTransfer
void Func_081963ec(s32 slot, s32 blitter); // BattleFx_LoadWork, packed
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);          // ObjectGroup_UpdateMembers
void Func_081434f8(void);                // ObjectGroup_TickMemberTimers
void Func_08158ce0(s32 mask, u32 range); // Camera_ApplyShake
void BattleFx_StartActorMotion(s32 actor, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void ObjectDispatch_ApplyValueToChildrenFar(struct MotionObject *object,
                                            s32 value);
void Object_SetMode(struct MotionObject *object, s32 mode);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
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
void Func_0816d3d8(struct BattleEffectArgument *effect) {
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
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  Func_081963ec(104,
                BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 1;
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
  scroll = 0;
  scrollSpeed = 0;
  cueTimer = 0;
  user = GetBattleObjectSlotFar(effect->actor)->object;
  ObjectDispatch_ApplyValueToChildrenFar(user, 48);

  for (frame = 0; frame != 107; frame++) {
    EffectPosition_ApplyStepAndYOffset(effect->actor, &userPosition);
    EffectPosition_ApplyStepAndYOffset(effect->actors[0], &targetPosition);
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
    REG_BG2X = (64 - (userPosition.x - scrollX)) << 8;

    // Each boulder spins through three pictures of its shard (z is its
    // spin rate, velocityZ the angle) and hits the target once it passes
    // it.
    hit = 0;
    for (i = 0; i != ROCKS; i++) {
      s32 cell;
      u32 width;
      u32 height;

      rock = &work->particles[i];
      cell = (i % 5) * 3 + rock->velocityZ / 96 % 3;
      width = gData_0819749e[cell];
      height = gData_081974ad[cell];

      blit(canvas, work->sheet + gData_081974bc[cell] + 0x83c,
           HI(rock->x) - width / 2 + scrollX, HI(rock->y) - height / 2, width,
           height);
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
      BattleFxKernels_IntegrateVector2(rock, 60, 0);
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
      BattleFx_StartActorMotion(effect->actors[0], 1, 2 << 16, 4 << 16, 2 << 16,
                                150);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
      work->shakeFrames = 2;
    }

    Func_08158ce0(2, 4);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  ObjectDispatch_ApplyValueToChildrenFar(user, 16);
  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(104);
  Func_08143bb8();
}
