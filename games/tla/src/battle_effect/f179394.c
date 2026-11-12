#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "task.h"
#include "battle.h"
#include "motobj.h"

struct FxPartyList;
struct Model;
struct StripQuad;

#include "render_model.h"

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void Func_080150e4(s32 angle);  // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void *Resource_GetTableEntry(s32 resource);

void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1]) // the pixel half of a 16.16 value

void Func_0815f0a0(s32 palette);

extern char ResourceId_CornerRuneBSheet;
extern char ResourceId_Data134;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_Data0C2;
extern char ResourceId_OrangePaletteB;
#define TEXTURE_PIXELS (Ram_MapCellBuffer + 0x1000)
#define EMBERS ((struct EffectStep *)(Ram_MapCellBuffer + 0x5000))
// The offsets of the spark cels: cel n is n pixels wide and 2n tall.
extern u16 gData_08197410[]; // gBattleEffect_MoteCells
// Per level (effect->variant): the sparks, their spread, the rings' size
// and the embers.
extern const u8 Data_08199400[][4];
extern const u8 gData_08199364[]; // the ring's mesh
extern const u8 gData_08199210[]; // its corners

#define RAGING_HEAT_FRAMES 96

// Abilities: Raging Heat.
//
// A ring of heat spins over the targets' middle and swells; from frame 56 a
// pillar of flame (two mirrored halves) falls onto them, bursting into
// sparks and embers when it lands, under a second, fading ring.
/// Raging Heat: a spinning ring of heat over the targets, and a pillar of
/// flame that falls onto them and bursts into sparks and embers.
void BattleEffect_RunRagingHeat(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  // The plain and the flipped blitter, as one pair.
  DrawRectangle draw[2];
  u8 *sprites;
  struct EffectPosition targets[8];
  struct ModelTexture texture;
  u8 *buffer;
  s32 centerX;
  s32 baseY;
  struct RenderModel *model;
  struct EffectStep *spark;
  struct EffectStep *particle;
  s32 i;

  buffer = Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0xa10;
  BattleEffect_LoadBlitter(
      SLOT_BLITTER,
      BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                           COMPOSITE_CLIP_Y |
                                                           COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  for (i = 0; i != effect->count; i++) {
    EffectPosition_ProjectScaledBattler(effect->actors[i], &targets[i]);
  }
  centerX = targets[0].x + (targets[effect->count - 1].x - targets[0].x) / 2;
  baseY = targets[0].y - 48;
  Resource_LoadAndDecompress((s32)&ResourceId_CornerRuneBSheet, buffer, 1, 1);
  Func_0816ae40(buffer, work->sheet, 64, 64);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, buffer, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, TEXTURE_PIXELS, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  for (frame = 0; frame != RAGING_HEAT_FRAMES; frame++) {
    if (frame == 0) {
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = 24;
      }
      for (i = 0; i != 256; i++) {
        EMBERS[i].variant = -1;
      }
      Sound_PlayFar(140);
    }
    if ((u32)(frame - 40) <= 15) {
      Func_0815f0a0((s32)&ResourceId_OrangePaletteB);
    }
    if (frame == 56) {
      Sound_PlayFar(144);
    }

    // The pillar falls eight lines a frame and bursts where it lands.
    if (frame > 55) {
      s32 y = baseY + frame * 8 - 480;

      if ((u32)(y - 52) <= 7) {
        for (i = 0, spark = work->particles; i != 32; i++, spark++) {
          u32 random;
          u32 spread;

          spark->variant = -i / 4 + 2;
          spark->x = (centerX / 2) << 16;
          spark->y = ((Random16() & 15) + y + 16) << 16;
          random = Random16();
          spread = Data_08199400[effect->variant][1];
          spark->velocityX = ((random % spread) - (spread >> 1)) << 12;
          spark->velocityY = (192 - (Random16() & 255)) << 10;
        }
        for (i = 0; i != Data_08199400[effect->variant][3]; i++) {
          s32 angle;
          s32 speed;

          EMBERS[i].x = (centerX / 2) << 16;
          EMBERS[i].y = ((Random16() & 15) + y + 24) << 16;
          angle = (Random16() & 0x7fff) + 0x4000;
          speed = (Random16() & 255) + 127;
          EMBERS[i].velocityX = (Trig_Sin(angle) * speed) >> 5;
          EMBERS[i].velocityY = (Trig_Cos(angle) * speed) >> 5;
          EMBERS[i].variant = (Random16() & 15) + 16;
        }
        Func_081180e8(145);
        for (i = 0; i != effect->count; i++) {
          Func_08118088(effect->actors[i], 0);
          Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        }
        work->shakeFrames = 16;
      }
      if (y <= 52) {
        draw[0](canvas, buffer, centerX / 2 - 24, y, 24, 64);
        draw[1](canvas, buffer, centerX / 2, y, 24, 64);
      }
      i = 0;
      /* FAKEMATCH(dead): the test repeats the loop's own; as a plain for loop,
       * 4 of 669 instructions differ in tla-en. */
      if (i != Data_08199400[effect->variant][0]) {
        for (particle = work->particles; i != Data_08199400[effect->variant][0];
             i++, particle++) {
          if ((u32)particle->variant <= 23) {
            if (particle->variant > 1) {
              draw[0](canvas,
                      work->sheet + 0x4000 + ((particle->variant / 4) << 11),
                      HI(particle->x) - 16, HI(particle->y) - 32, 32, 64);
            }
            BattleEffectKernels_IntegrateVector2(particle, 60, -0x1000);
          }
          particle->variant++;
        }
      }
      i = 0;
      if (i != Data_08199400[effect->variant][3]) {
        particle = EMBERS;
        do {
          if (particle->variant >= 0) {
            s32 size = (particle->variant >> 3) + 1;

            draw[0](canvas, sprites + gData_08197410[size - 1],
                    HI(particle->x) - size / 2, HI(particle->y) - size, size,
                    size * 2);
            BattleEffectKernels_IntegrateVector2(particle, 60, 0x1000);
            particle->variant--;
          }
          i++;
          particle++;
        } while (i != Data_08199400[effect->variant][3]);
      }
    }

    // The rings: the spinning one, and from frame 56 the fading one.
    {
      void *vertices;
      s32 shade;

      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      shade = 0;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = gData_08199364;
      model->vertices = vertices;
      model->shade = shade;
      texture.logWidth = 7;
      texture.logHeight = 7;
      for (i = 0; i != 2; i++) {
        s32 size = Data_08199400[effect->variant][2];
        s32 largest = size << 10;
        s32 step = size << 5;
        s32 scale;

        Render_ResetTransformState();
        Func_080151e4(0x8000, 0x10000, 0x8000);
        if (i == 0) {
          Func_08015160((centerX << 16) - 0x800000, (baseY << 16) - 0x400000,
                        0);
        } else {
          Func_08015160((centerX << 16) - 0x800000,
                        ((baseY + 8) << 16) - 0x400000, 0);
        }
        SceneTransform_ApplyPitch(2000);
        if (frame <= 47) {
          SceneTransform_ApplyPitch((48 - frame) << 9);
        }
        if (i == 0) {
          scale = (frame - 16) * step;
          if (scale > largest) {
            scale = largest;
          }
          if (frame > 60) {
            scale = largest - step * (frame * 2 - 120);
          }
          texture.pixels = work->sheet;
          SceneTransform_ApplyYaw(-frame * 3 << 8);
        } else {
          scale = (frame * 4 - 224) * step;
          if (scale < 0) {
            continue;
          }
          scale += 0x4000;
          shade = 512 - frame * 8;
          model->shade = shade;
          texture.pixels = TEXTURE_PIXELS;
        }
        if (scale > 0 && shade > -64) {
          Func_0801521c(scale);
          Render_TransformVertices(gData_08199210, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
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

extern char ResourceId_SmallFlameSheet;

// The life of each spark in the map cell buffer: gMapCellBuffer's
// EffectSteps from their variant field on.
extern s32 Data_02010018[];
extern u8 Data_0819940c[][7];

void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position);
void Func_0815e22c(struct EffectPosition *midpoint);

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

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(HEAP_SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void Graphics_LoadGreyRamp(void);

void Func_08179e6c(void) {
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 y;
  s32 x;

  Graphics_LoadGreyRamp();
  REG_BG2CNT = 0x784;
  PLTT_BACKDROP = 0;
  REG_BG2PA = 0x4e;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  for (y = 0; y != 128; y++) {
    for (x = 0; x != 128; x++) {
      s32 level;

      if (y < 32)
        level = y * 2;
      else if (y > 95)
        level = 255 - y * 2;
      else
        level = 63;
      canvas[(((x / 8) * 16 + y / 8) * 8 + (x & 7)) * 8 + (y & 7)] = level;
    }
  }
}
