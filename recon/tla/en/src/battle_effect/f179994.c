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

// The transform entry as The Broken Seal types it.
#define TransformVector ((s32 (*)(const void *, s32 *))Iwram_TransformVector)

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_SmallFlameSheet;
extern char ResourceId_Data134;
extern char ResourceId_OrangePaletteB;

// The life of each spark in the map cell buffer: gMapCellBuffer's
// EffectSteps from their variant field on.
extern s32 Data_02010018[];
extern u8 Data_0819940c[][7];

// The offsets of the spark cels: cel n is n pixels wide and 2n tall.
extern const u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_FetchRectangleBlitters(s32 side, DrawRectangle *pair);
void *Resource_GetTableEntry(s32 resourceId);
s32 Task_Add(s32 func, s32 priority);
s32 Task_Remove(s32 func);
void Sound_PlayFar(s32 cue);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position);
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
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);
void HeapSlot_Release(s32 slot);
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
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 frame;
  u8 *sparkCels = Ram_HeapSlots->unk64[0];
  struct EffectStep *ring = work->particles;
  DrawRectangle *blitters;
  s32 lift;
  s32 j;
  s32 i;

  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_SmallFlameSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  if (effect->variant == 2) {
    void *source = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

    copy((void *)BG_PLTT, source, 128);
  }
  blitters = draw;
  BattleEffect_FetchRectangleBlitters(0, blitters);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  lift = 0;
  {
    s32 *life = Data_02010018;

    for (i = 0; i != SPARK_COUNT; i++) {
      *life = -1;
      life += 7;
    }
  }

  for (frame = 0; frame != RING_FRAMES; frame++) {
    if (frame == RING_CUE_FRAME)
      Sound_PlayFar(104);
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
      s32 *place;
      s32 x;
      s32 y;

      Render_ResetTransformState();
      Func_08015160(ring->x - (128 << 16), ring->y - (64 << 16), 0);
      Func_080150e4(frame * 256);
      SceneTransform_ApplyPitch(frame * 256 + 500);
      if (i > 7)
        SceneTransform_ApplyPitch(0x4000);
      SceneTransform_ApplyYaw(
          (frame * 4 + i * Data_0819940c[effect->variant][RING_SPACING]) << 8);
      place = vector;
      place[0] = 0;
      place[1] = 0;
      place[2] = lift << 16;
      TransformVector(place, place);
      x = place[0] >> 17;
      place[0] = x + 64;
      y = ((s16 *)place)[3];
      place[1] = y + 64;
      blitters[Data_0819940c[effect->variant][RING_BLITTER]](
          canvas, work->sheet + cel * 1024, x + 56, y + 16, 16, 64);
    }

    // The strike: sparks burst from every target.
    if (frame == RING_HIT_FRAME) {
      Func_081180e8(134);
      for (i = 0; i != effect->count; i++) {
        EffectPosition_ProjectSprite(effect->actors[i], &target);
        Func_08118088(effect->actors[i], 4);
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        for (j = 0; j != Data_0819940c[effect->variant][RING_SPARKS]; j++) {
          struct EffectStep *spark =
              &((struct EffectStep *)Ram_MapCellBuffer)[i * 128 + j];
          s32 angle;
          s32 speed;

          spark->x = (target.x / 2) << 16;
          spark->y = target.y << 16;
          angle = Random16();
          speed = (Random16() & 255) + 127;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 6;
          spark->variant = (Random16() & 15) + 24;
        }
      }
      work->shakeFrames = Data_0819940c[effect->variant][RING_SHAKE];
    }

    // The sparks fly and burn down, drawn by turns with the two blitters.
    for (i = 0; i != SPARK_COUNT; i++) {
      struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

      if (spark->variant >= 0) {
        s32 size = (spark->variant >> 3) + 1;

        blitters[i & 1](canvas, sparkCels + gData_08197410[size - 1],
                        HI(spark->x) - size / 2, HI(spark->y) - size, size,
                        size * 2);
        BattleEffectKernels_IntegrateVector2(spark, 60, -0x4000);
        spark->variant--;
      }
    }

    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(HEAP_SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
