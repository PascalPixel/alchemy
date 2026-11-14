/// @file
/// Raging Heat and Juggle.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

struct FxPartyList;
struct Model;
struct StripQuad;

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define TEXTURE_PIXELS (Ram_MapCellBuffer + 0x1000)
#define EMBERS ((struct EffectStep *)(Ram_MapCellBuffer + 0x5000))

#define RAGING_HEAT_FRAMES 96

// Abilities: Raging Heat.
//
// A ring of heat spins over the targets' middle and swells; from frame 56 a
// pillar of flame (two mirrored halves) falls onto them, bursting into
// sparks and embers when it lands, under a second, fading ring.
/// Raging Heat: a spinning ring of heat over the targets, and a pillar of
/// flame that falls onto them and bursts into sparks and embers.
void BattleEffect_RunRagingHeat(struct BattleEffectArgument *args) {
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
  REG_BLDALPHA = BLEND_ALPHA(16, 10);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  for (i = 0; i != args->count; i++) {
    EffectPosition_ProjectScaledBattler(args->actors[i], &targets[i]);
  }
  centerX = targets[0].x + (targets[args->count - 1].x - targets[0].x) / 2;
  baseY = targets[0].y - 48;
  Resource_LoadAndDecompress((s32)&ResourceId_CornerRuneBSheet, buffer, 1, 1);
  Picture_MirrorIntoQuadrants(buffer, work->sheet, 64, 64);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, buffer, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, TEXTURE_PIXELS, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != RAGING_HEAT_FRAMES; frame++) {
    if (frame == 0) {
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = 24;
      }
      for (i = 0; i != 256; i++) {
        EMBERS[i].variant = -1;
      }
      Sound_PlayFar(SE_SPIN);
    }
    if ((u32)(frame - 40) <= 15) {
      BattleEffect_StepPaletteToResource((s32)&ResourceId_OrangePaletteB);
    }
    if (frame == 56) {
      Sound_PlayFar(SE_CLATTER);
    }

    // The pillar falls eight lines a frame and bursts where it lands.
    if (frame > 55) {
      s32 y = baseY + frame * 8 - 480;

      if ((u32)(y - 52) <= 7) {
        for (i = 0, spark = work->particles; i != 32; i++, spark++) {
          u32 random;
          u32 spread;

          spark->variant = -i / 4 + 2;
          spark->x = px(centerX / 2);
          spark->y = px((Random16() & 15) + y + 16);
          random = Random16();
          spread = gRagingHeat_Styles[args->variant][1];
          spark->velocityX = ((random % spread) - (spread >> 1)) << 12;
          spark->velocityY = (192 - (Random16() & 255)) << 10;
        }
        for (i = 0; i != gRagingHeat_Styles[args->variant][3]; i++) {
          s32 angle;
          s32 speed;

          EMBERS[i].x = px(centerX / 2);
          EMBERS[i].y = px((Random16() & 15) + y + 24);
          angle = (Random16() & 0x7fff) + TRIG_TURN / 4;
          speed = (Random16() & 255) + 127;
          EMBERS[i].velocityX = (Trig_Sin(angle) * speed) >> 5;
          EMBERS[i].velocityY = (Trig_Cos(angle) * speed) >> 5;
          EMBERS[i].variant = (Random16() & 15) + 16;
        }
        EFFECT_SHOW_DAMAGE(SE_CRASH);
        for (i = 0; i != args->count; i++) {
          BattleMotion_ApplyKnockbackFar(args->actors[i], 0);
          BattleEffect_TintBattler(args->actors[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
        }
        work->shakeFrames = 16;
      }
      if (y <= 52) {
        draw[0](canvas, buffer, centerX / 2 - 24, y, 24, 64);
        draw[1](canvas, buffer, centerX / 2, y, 24, 64);
      }
      i = 0;
      /* FAKEMATCH(dead): the test repeats the loop's own. */
      if (i != gRagingHeat_Styles[args->variant][0]) {
        for (particle = work->particles;
             i != gRagingHeat_Styles[args->variant][0]; i++, particle++) {
          if ((u32)particle->variant <= 23) {
            if (particle->variant > 1) {
              draw[0](canvas,
                      work->sheet + 0x4000 + ((particle->variant / 4) << 11),
                      Q_16_16_UPPER_HALF(particle->x) - 16,
                      Q_16_16_UPPER_HALF(particle->y) - 32, 32, 64);
            }
            BattleEffectKernels_IntegrateVector2(particle, 60, -px(1) / 16);
          }
          particle->variant++;
        }
      }
      i = 0;
      if (i != gRagingHeat_Styles[args->variant][3]) {
        particle = EMBERS;
        do {
          if (particle->variant >= 0) {
            s32 size = (particle->variant >> 3) + 1;

            draw[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                    Q_16_16_UPPER_HALF(particle->x) - size / 2,
                    Q_16_16_UPPER_HALF(particle->y) - size, size, size * 2);
            BattleEffectKernels_IntegrateVector2(particle, 60, px(1) / 16);
            particle->variant--;
          }
          i++;
          particle++;
        } while (i != gRagingHeat_Styles[args->variant][3]);
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
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->vertices = vertices;
      model->shade = shade;
      texture.logWidth = 7;
      texture.logHeight = 7;
      for (i = 0; i != 2; i++) {
        s32 size = gRagingHeat_Styles[args->variant][2];
        s32 largest = size << 10;
        s32 step = size << 5;
        s32 scale;

        Transform_Reset();
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        if (i == 0) {
          SceneTransform_ApplyPosition3((centerX << 16) - 0x800000,
                                        (baseY << 16) - 0x400000, 0);
        } else {
          SceneTransform_ApplyPosition3((centerX << 16) - 0x800000,
                                        ((baseY + 8) << 16) - 0x400000, 0);
        }
        Transform_ApplyPitch(2000);
        if (frame <= 47) {
          Transform_ApplyPitch((48 - frame) << 9);
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
          Transform_ApplyYaw(-frame * 3 << 8);
        } else {
          scale = (frame * 4 - 224) * step;
          if (scale < 0) {
            continue;
          }
          scale += Q16_ONE / 4;
          shade = 512 - frame * 8;
          model->shade = shade;
          texture.pixels = TEXTURE_PIXELS;
        }
        if (scale > 0 && shade > -64) {
          SceneTransform_ApplyUniformScale(scale);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#define HEAP_SLOT_BLITTER 104 // Ram_HeapSlots->unk64[1]

#define RING_FRAMES 122
#define RING_CUE_FRAME 16
#define RING_RISE_FRAME 8   // the ring starts to rise over the caster
#define RING_THROW_FRAME 64 // it flies at the targets
#define RING_HIT_FRAME 88
#define SPARK_COUNT 768

// gJuggle_Styles[variant]: seven bytes for each strength.
enum {
  RING_LIFT_STEP, // pixels the ring rises each frame
  RING_LIFT_MAX,  // and its highest
  RING_FLAMES,    // flames in the ring
  RING_SPACING,   // their spacing round it
  RING_SPARKS,    // sparks at each target
  RING_SHAKE,     // frames the screen shakes at the hit
  RING_BLITTER    // which of the pair draws the flames
};

// The transform entry, typed as its callers use it.
#define TransformVector                                                        \
  ((s32 (*)(const void *, s32 *))Iwram_TransformVectorEntry)

// A ring of small flames. The flames circle the caster in a ring that rises
// over its head from frame 8, each turning its own way through four cels.
// At frame 64 the ring flies at the targets' midpoint, slowing as it
// nears; at frame 88 it strikes: every target is knocked back and flashes,
// sparks burst from each, and the screen shakes. The variant sets the
// ring's rise, its flames and sparks and the shake; variant 2 also takes
// the orange palette.
void BattleEffect_RunJuggle(struct BattleEffectArgument *args) {
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
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_SmallFlameSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  if (args->variant == 2) {
    void *source = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, source, 128);
  }
  blitters = draw;
  BattleEffect_FetchRectangleBlitters(0, blitters);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
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
      Sound_PlayFar(SE_104);
    if (frame == 0)
      lift = 0;

    // The ring rises over the caster, and follows it until it is thrown.
    EffectPosition_ProjectBattler(args->actor, &caster);
    if (frame >= RING_RISE_FRAME)
      lift += gJuggle_Styles[args->variant][RING_LIFT_STEP];
    if (lift > gJuggle_Styles[args->variant][RING_LIFT_MAX])
      lift = gJuggle_Styles[args->variant][RING_LIFT_MAX];
    if (frame < RING_THROW_FRAME) {
      ring->x = caster.x << 16;
      ring->y = px(caster.y - lift - 32);
    } else {
      // The throw: z and velocityZ hold the pull toward the midpoint, and
      // the ring keeps three quarters of its speed each frame.
      if (frame == RING_THROW_FRAME) {
        BattleEffect_GetTargetsMidpoint(&midpoint);
        ring->x = caster.x << 16;
        ring->y = px(caster.y - lift - 32);
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
    for (i = 0; i != gJuggle_Styles[args->variant][RING_FLAMES]; i++) {
      s32 cel = (frame / 2 + i * 4) & 3;
      s32 *place;
      s32 x;
      s32 y;

      Transform_Reset();
      SceneTransform_ApplyPosition3(ring->x - px(128), ring->y - px(64), 0);
      Transform_ApplyRoll(frame * 256);
      Transform_ApplyPitch(frame * 256 + 500);
      if (i > 7)
        Transform_ApplyPitch(TRIG_TURN / 4);
      Transform_ApplyYaw(
          (frame * 4 + i * gJuggle_Styles[args->variant][RING_SPACING]) << 8);
      place = vector;
      place[0] = 0;
      place[1] = 0;
      place[2] = lift << 16;
      TransformVector(place, place);
      x = place[0] >> 17;
      place[0] = x + 64;
      y = ((s16 *)place)[3];
      place[1] = y + 64;
      blitters[gJuggle_Styles[args->variant][RING_BLITTER]](
          canvas, work->sheet + cel * 1024, x + 56, y + 16, 16, 64);
    }

    // The strike: sparks burst from every target.
    if (frame == RING_HIT_FRAME) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      for (i = 0; i != args->count; i++) {
        EffectPosition_ProjectSprite(args->actors[i], &target);
        BattleMotion_ApplyKnockbackFar(args->actors[i], 4);
        BattleEffect_TintBattler(args->actors[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
        for (j = 0; j != gJuggle_Styles[args->variant][RING_SPARKS]; j++) {
          struct EffectStep *spark =
              &((struct EffectStep *)Ram_MapCellBuffer)[i * 128 + j];
          s32 angle;
          s32 speed;

          spark->x = px(target.x / 2);
          spark->y = target.y << 16;
          angle = Random16();
          speed = (Random16() & 255) + 127;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 6;
          spark->variant = (Random16() & 15) + 24;
        }
      }
      work->shakeFrames = gJuggle_Styles[args->variant][RING_SHAKE];
    }

    // The sparks fly and burn down, drawn by turns with the two blitters.
    for (i = 0; i != SPARK_COUNT; i++) {
      struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

      if (spark->variant >= 0) {
        s32 size = (spark->variant >> 3) + 1;

        blitters[i & 1](canvas,
                        sparkCels + gBattleEffect_MotePictures[size - 1],
                        Q_16_16_UPPER_HALF(spark->x) - size / 2,
                        Q_16_16_UPPER_HALF(spark->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(spark, 60, -px(1) / 4);
        spark->variant--;
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Loads the grey ramp and fills the 128x128 canvas with a vertical gradient
/// that rises over the top 32 rows and falls over the bottom 32.
void RagingHeat_FillGradientCanvas(void) {
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 y;
  s32 x;

  Graphics_LoadGreyRamp();
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  PLTT_BACKDROP = 0;
  REG_BG2PA = 0x4e;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
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
#undef SLOT_BLITTER
#undef BLITTER
#undef TEXTURE_PIXELS
#undef EMBERS
#undef RAGING_HEAT_FRAMES
#undef HEAP_SLOT_BLITTER
#undef RING_FRAMES
#undef RING_CUE_FRAME
#undef RING_RISE_FRAME
#undef RING_THROW_FRAME
#undef RING_HIT_FRAME
#undef SPARK_COUNT
#undef TransformVector
