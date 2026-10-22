// Draft (tla-en): listing 552 instructions, draft 556, common 396, differ 316.
// Effect kind 78, written from the listing on ☀️'s skeleton; its nearest ☀️
// shapes are BattleEffect_RunPlasma (0.49) and
// BattleEffect_RunHelmSplitter (0.67 by calls), neither a twin.
// Written 2026-10-15 (lane N2).
// Left:
// - the native frame is 120 bytes, here 124: one more spill slot, so
// every spilled value (canvas sp+60, frame sp+56, the spark cels sp+52,
// the pair's address sp+48, lift sp+44 in the native) sits one word
// off, and work takes sl where the native has r8.
// - the flame loop rebuilds variant * 7 for each table read; the native
// keeps frame / 2's sign word and frame * 4 spilled (sp+28, sp+32).
// - the hit loop's walkers (the target place sp+24, the actor offset
// sp+20, the spark offset sp+12 by 0xe00) and the kept table base.
// Measured: the first draft 426; a block-scoped pointer for each spark
// loop and the speed's +127 folded into its first assignment: 316;
// work->particles in place of the ring pointer, frame declared first:
// 320 (frame stays 124).
// Resources 0xeb (BattleEffect_SmallFlameSheet) and 0x148
// (BattleEffect_OrangePaletteB) need names in system/resource/directory.s
// when this is adopted.
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "types.h"

#define HEAP_SLOT_BLITTER 104           // Ram_HeapSlots->unk64[1]
#define HEAP_SLOT_BLITTER_ALTERNATE 188 // Ram_HeapSlots->unkb8[1]

#define RING_FRAMES 122
#define RING_CUE_FRAME 16
#define RING_RISE_FRAME 8   // the ring starts to rise over the caster
#define RING_THROW_FRAME 64 // it flies at the targets
#define RING_HIT_FRAME 88
#define SPARK_COUNT 768

// Data_0819940c[variant]: seven bytes for each strength.
enum {
  RING_LIFT_STEP, // pixels the ring rises each frame
  RING_LIFT_MAX,  // and its highest
  RING_FLAMES,    // flames in the ring
  RING_SPACING,   // their spacing round it
  RING_SPARKS,    // sparks at each target
  RING_SHAKE,     // frames the screen shakes at the hit
  RING_BLITTER    // which of the pair draws the flames
};

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_SmallFlameSheet;
extern char ResourceId_Data134;
extern char ResourceId_OrangePaletteB;

// The life of each spark in the map cell buffer: gMapCellBuffer's
// EffectSteps from their variant field on.
extern s32 Data_02010018[];
extern const u8 Data_0819940c[][7];

// The offsets of the spark cels: cel n is n pixels wide and 2n tall.
extern const u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_FetchRectangleBlitters(s32 side, DrawRectangle *pair);
void *Resource_GetTableEntry(s32 resourceId);
void Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
void Audio_PlayCue(s32 cue);
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
void EffectPosition_ProjectSprite(s32 id,
                                             struct EffectPosition *position);
void Func_0815e22c(struct EffectPosition *midpoint);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080150e4(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_081180e8(s32 phase);
void Func_08118088(s32 actor, s32 motion);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_EndCanvasLayer(void);

// A ring of small flames. The flames circle the caster in a ring that rises
// over its head from frame 8, each turning its own way through four cels.
// At frame 64 the ring flies at the targets' midpoint, slowing as it
// nears; at frame 88 it strikes: every target is knocked back and flashes,
// sparks burst from each, and the screen shakes. The variant sets the
// ring's rise, its flames and sparks and the shake; variant 2 also takes
// the orange palette.
void BattleEffect_RunJuggle(struct BattleEffectArgument *effect) {
  struct EffectPosition caster;
  struct EffectPosition midpoint;
  s32 vector[3];
  struct EffectPosition target;
  DrawRectangle draw[2];
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *sparkCels = Ram_HeapSlots->unk64[0];
  struct EffectStep *ring = work->particles;
  s32 frame;
  s32 spin;
  s32 lift;
  s32 i;
  s32 member;

  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_SmallFlameSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  if (effect->variant == 2)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  BattleEffect_FetchRectangleBlitters(0, draw);
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  lift = 0;
  {
    s32 *life = Data_02010018;

    for (i = 0; i != SPARK_COUNT; i++) {
      *life = -1;
      life += 7;
    }
  }

  spin = 0;
  for (frame = 0; frame != RING_FRAMES; frame++) {
    if (frame == RING_CUE_FRAME)
      Audio_PlayCue(104);
    if (frame == 0)
      lift = 0;

    // The ring rises over the caster, and follows it until it is thrown.
    EffectPosition_ProjectBattler(effect->actor, &caster);
    if (frame >= RING_RISE_FRAME)
      lift += Data_0819940c[effect->variant][RING_LIFT_STEP];
    if (lift > Data_0819940c[effect->variant][RING_LIFT_MAX])
      lift = Data_0819940c[effect->variant][RING_LIFT_MAX];
    if (frame < RING_THROW_FRAME) {
      ring->x = caster.x << 16;
      ring->y = (caster.y - lift - 32) << 16;
    } else {
      // The throw: z and velocityZ hold the pull toward the midpoint, and
      // the ring keeps three quarters of its speed each frame.
      if (frame == RING_THROW_FRAME) {
        Func_0815e22c(&midpoint);
        ring->x = caster.x << 16;
        ring->y = (caster.y - lift - 32) << 16;
        ring->velocityX = 0;
        ring->velocityY = 0;
        ring->z = ((midpoint.x << 16) - ring->x) / 64;
        ring->velocityZ = (((midpoint.y - 32) << 16) - ring->y) / 64;
      }
      ring->x += ring->velocityX;
      ring->y += ring->velocityY;
      ring->velocityX += ring->z;
      ring->velocityY += ring->velocityZ;
      ring->velocityX = ring->velocityX * 48 / 64;
      ring->velocityY = ring->velocityY * 48 / 64;
    }

    // The flames: each one's place on the ring turned into the canvas, the
    // second eight on a ring tipped on its side.
    for (i = 0; i != Data_0819940c[effect->variant][RING_FLAMES]; i++) {
      s32 cel = (frame / 2 + i * 4) & 3;
      s32 x;
      s32 y;

      Render_ResetTransformState();
      Func_08015160(ring->x - (128 << 16), ring->y - (64 << 16), 0);
      Func_080150e4(spin);
      SceneTransform_ApplyPitch(spin + 500);
      if (i > 7)
        SceneTransform_ApplyPitch(0x4000);
      SceneTransform_ApplyYaw(
          (frame * 4 + i * Data_0819940c[effect->variant][RING_SPACING]) << 8);
      vector[0] = 0;
      vector[1] = 0;
      vector[2] = lift << 16;
      Iwram_TransformVector(vector, vector);
      x = vector[0] >> 17;
      vector[0] = x + 64;
      y = HI(vector[1]);
      vector[1] = y + 64;
      draw[Data_0819940c[effect->variant][RING_BLITTER]](
          canvas, work->sheet + cel * 1024, x + 56, y + 16, 16, 64);
    }

    // The strike: sparks burst from every target.
    if (frame == RING_HIT_FRAME) {
      Func_081180e8(134);
      for (member = 0; member != effect->count; member++) {
        struct EffectStep *spark;

        EffectPosition_ProjectSprite(effect->actors[member],
                                                &target);
        Func_08118088(effect->actors[member], 4);
        Func_0814cd48(effect->actors[member], 7, 5, member, 16);
        spark = &((struct EffectStep *)Ram_MapCellBuffer)[member * 128];
        for (i = 0; i != Data_0819940c[effect->variant][RING_SPARKS];
             i++, spark++) {
          s32 angle;
          s32 speed;

          spark->x = (target.x / 2) << 16;
          spark->y = target.y << 16;
          angle = Random16();
          speed = (Random16() & 255) + 127;
          spark->velocityX = (speed * Trig_Sin(angle)) >> 6;
          spark->velocityY = (speed * Trig_Cos(angle)) >> 6;
          spark->variant = (Random16() & 15) + 24;
        }
      }
      work->shakeFrames = Data_0819940c[effect->variant][RING_SHAKE];
    }

    // The sparks fly and burn down, drawn by turns with the two blitters.
    {
      struct EffectStep *spark = (struct EffectStep *)Ram_MapCellBuffer;

      for (i = 0; i != SPARK_COUNT; i++, spark++) {
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 3) + 1;

          draw[i & 1](canvas, sparkCels + gData_08197410[size - 1],
                      HI(spark->x) - size / 2, HI(spark->y) - size, size,
                      size * 2);
          BattleEffectKernels_IntegrateVector2(spark, 60, -0x4000);
          spark->variant--;
        }
      }
    }

    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
    spin += 256;
  }

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Scheduler_RemoveCallback((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
