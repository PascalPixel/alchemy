#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "system.h"
#include "task.h"
#include "resource.h"
#include "heap_release.h"
#include "motobj.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
static inline s16 *Fixed_Halves(s32 *value) {
  /* FAKEMATCH(wrapper): the halves read through this inline; as the cast
   * ((s16 *)&v)[1], 324 of 374 instructions differ in tla-en. */
  return (s16 *)value;
}
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
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 blitter); // BattleEffect_BuildBlitter, packed
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames); // BattleEffect_TintBattler
void BattleEffect_CountDownBattlerTints(void);
void Func_08158ce0(s32 mask, u32 range); // Camera_ApplyShake
void BattleEffect_StartActorMotion(s32 actor, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void Actor_SetAnimationSpeedFar(struct MotionObject *object, s32 value);
void Actor_SetMode(struct MotionObject *object, s32 mode);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void Sound_PlayFar(s32 cue);

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
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(
      104, BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  scroll = 0;
  scrollSpeed = 0;
  cueTimer = 0;
  user = GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetAnimationSpeedFar(user, 48);

  for (frame = 0; frame != 107; frame++) {
    EffectPosition_ProjectScaledBattler(effect->actor, &userPosition);
    EffectPosition_ProjectScaledBattler(effect->actors[0], &targetPosition);
    if (frame == 0)
      Sound_PlayFar(157);
    if (frame == 64)
      Sound_PlayFar(104);

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
      Actor_SetMode(user, 2);

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

      rock = &work->particles[i];
      cell = (i % 5) * 3 + rock->velocityZ / 96 % 3;
      width = gData_0819749e[cell];

      blit(canvas, work->sheet + gData_081974bc[cell] + 0x83c,
           HI(rock->x) - width / 2 + scrollX,
           HI(rock->y) - gData_081974ad[cell] / 2, width, gData_081974ad[cell]);
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
        Sound_PlayFar(134);
      }
      BattleEffect_StartActorMotion(effect->actors[0], 1, 2 << 16, 4 << 16,
                                    2 << 16, 150);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
      work->shakeFrames = 2;
    }

    Func_08158ce0(2, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Actor_SetAnimationSpeedFar(user, 16);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
