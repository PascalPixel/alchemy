/// @file
/// Triple Chomp, and the entry points of Beam, Rolling Flame, Ghost Soldier and
/// Blast Breath.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define TRIPLE_CHOMP_FRAMES 50
/// The bites, closing at gTripleChomp_BiteFrames' three frames.
#define TRIPLE_CHOMP_BITES 3
/// The canvas line each bite closes on, this far above the target's place.
#define TRIPLE_CHOMP_BITE_HEIGHT 24

/// The sparks, TRIPLE_CHOMP_SPARK_COUNT of EXTRA_PARTICLES: each bite throws
/// TRIPLE_CHOMP_BITE_SPARKS from the start of its own run of
/// TRIPLE_CHOMP_BITE_SPARK_SLOTS.
#define TRIPLE_CHOMP_SPARK_COUNT 512
#define TRIPLE_CHOMP_BITE_SPARKS 32
#define TRIPLE_CHOMP_BITE_SPARK_SLOTS 128

/// Each bite's ring, FireRingTexture on a square: the first three particles
/// hold where each is spark. A ring swells from a quarter of its size by
/// TRIPLE_CHOMP_RING_GROWTH a frame for TRIPLE_CHOMP_RING_FRAMES, spark whole
/// for the first half and then losing TRIPLE_CHOMP_RING_FADE of its shade a
/// frame.
#define BITE_RING(bite) (&state->particles[bite])
#define TRIPLE_CHOMP_RING_FRAMES 16
#define TRIPLE_CHOMP_RING_GROWTH toFIXED(0.15552)
#define TRIPLE_CHOMP_RING_FADE 8

/// The user lunges most of the way to the target, and three bites close on
/// it, gTripleChomp_BiteFrames apart. Each throws blue sparks that fly off
/// away from the user, jolts the target back and flashes it, and swells a
/// ring of light over it, tilted back and turned a quarter further each bite,
/// that fades as it grows. The last bite shows the damage.
///
/// Abilities: Triple Chomp.
void BattleEffect_RunTripleChomp(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  /* LEFTOVER(frame): draw[1], the idle half of the pair. */
  BlitterPair draw;
  u8 *graphics;
  void *vertices;
  struct RenderModel *model;
  struct ModelTexture texture;
  struct ScreenPoint position;
  struct Particle *spark;
  s32 i;
  s32 j;
  u8 *pixels = Ram_MapCellBuffer;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;

  // Set up the canvas, then let the user lunge.
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  BattleEffect_SetApproachMotion(args->user, args->targets[0], 8, 80);
  wait(8);

  // A clipped, mirrored blitter keeping the brighter for the sparks; the
  // ring's texture, the spark sprite sheet and the blue flame's colours.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, pixels, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                       SHEET_PALETTE_BYTES);

  for (frame = 0; frame != TRIPLE_CHOMP_FRAMES; frame++) {
    if (frame == 0) {
      for (i = 0; i != TRIPLE_CHOMP_SPARK_COUNT; i++)
        EXTRA_PARTICLES[i].variant = -1;
    }

    // This frame's ring: one model drawing the square with the 128x128
    // texture.
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    EffectPosition_ProjectBattler(args->targets[0], &position);
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = pixels;
    model->mesh = gBattleEffect_LargeSquareMesh;
    model->kind = MODEL_SHADED_ADDITIVE;
    model->texture = &texture;
    model->vertices = vertices;

    for (i = 0; i != TRIPLE_CHOMP_BITES; i++) {
      // The bite closes: its sparks fly from the target's middle, and the
      // target jolts and flashes. The last bite shows the damage.
      if (frame == gTripleChomp_BiteFrames[i]) {
        for (j = 0; j != TRIPLE_CHOMP_BITE_SPARKS; j++) {
          struct Particle *spark =
              &EXTRA_PARTICLES[i * TRIPLE_CHOMP_BITE_SPARK_SLOTS + j];
          s32 angle;
          s32 force;

          spark->x = INTtoFIXED(position.x / 2);
          spark->y = INTtoFIXED(position.y - TRIPLE_CHOMP_BITE_HEIGHT);
          angle = Random16();
          force = (Random16() & 255) + 32;
          spark->velocityX = (Trig_Sin(angle) * force) >> 8;
          spark->velocityY = (Trig_Cos(angle) * force) >> 6;
          if (args->side == EFFECT_SIDE_ENEMIES)
            spark->velocityX += -toFIXED(1) / 2;
          else
            spark->velocityX += toFIXED(1) / 2;
          BITE_RING(i)->x = position.x;
          BITE_RING(i)->y = position.y - TRIPLE_CHOMP_BITE_HEIGHT;
          spark->variant = (Random16() & 7) + 16;
        }
        if (i == TRIPLE_CHOMP_BITES - 1)
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        else
          Sound_PlayFar(SE_HEAVY_IMPACT);
        BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(2),
                                      toFIXED(8), 0, i * 20 + 120);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        state->shakeFrames = 8;
      }

      // The bite's ring swells and fades.
      if (frame >= gTripleChomp_BiteFrames[i] &&
          frame < gTripleChomp_BiteFrames[i] + TRIPLE_CHOMP_RING_FRAMES) {
        s32 age = frame - gTripleChomp_BiteFrames[i];
        s32 shade = TRIPLE_CHOMP_RING_FRAMES / 2 * TRIPLE_CHOMP_RING_FADE -
                    age * TRIPLE_CHOMP_RING_FADE;
        FIXED scale = age * TRIPLE_CHOMP_RING_GROWTH + FIXED_ONE / 4;

        if (shade > 0)
          shade = 0;
        if (shade > MODEL_SHADE_INVISIBLE) {
          model->shade = shade;
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_Translate(INTtoFIXED(BITE_RING(i)->x - 128),
                           INTtoFIXED(BITE_RING(i)->y - 64), 0);
          Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
          Matrix_RotateX(-TRIG_TURN / 16);
          Matrix_ScaleUniform(scale);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
      }
    }

    // Each spark is spark and moved twice a frame, shrinking as it dies.
    spark = EXTRA_PARTICLES;
    for (i = 0; i != TRIPLE_CHOMP_SPARK_COUNT; i++) {
      s32 life = spark->variant;

      if (life >= 0) {
        s32 size = (life >> 2) + 3;

        EFFECT_DRAW_MOTE(draw[0], canvas, graphics,
                         FIXED_INTEGER_PART(spark->x),
                         FIXED_INTEGER_PART(spark->y), size);
        Particle_AdvanceWithGravity2D(spark, 62, 0);
        EFFECT_DRAW_MOTE(draw[0], canvas, graphics,
                         FIXED_INTEGER_PART(spark->x),
                         FIXED_INTEGER_PART(spark->y), size);
        Particle_AdvanceWithGravity2D(spark, 62, 0);
        spark->variant--;
      }
      spark++;
    }

    Heap_Release(model);
    Heap_Release(vertices);
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Motes of fire gather on the user and a beam of fire shoots from it across
/// the targets, a ring of light flying along it.
///
/// Abilities: Beam, Cycle Beam, Searing Beam.
void BattleEffect_RunBeam(struct BattleEffectParams *args) {
  BattleEffect_RunBreaths(args, BREATH_BEAM);
}

/// A ball of flame rolls from the user onto the targets and fire billows over
/// them, always at the strongest variant.
///
/// Abilities: Rolling Flame.
void BattleEffect_RunRollingFlame(struct BattleEffectParams *args) {
  args->variant = EFFECT_VARIANT_STRONGEST;
  BattleEffect_RunBreaths(args, BREATH_ROLLING_FLAME);
}

/// A ghostly soldier with a pike appears in the user's stead, gathers violet
/// light and fires a blast of blue light and a ring at the target, always at
/// the strongest variant.
///
/// Abilities: Ghost Soldier, Pike Assault.
void BattleEffect_RunGhostSoldier(struct BattleEffectParams *args) {
  args->variant = EFFECT_VARIANT_STRONGEST;
  BattleEffect_RunBreaths(args, BREATH_PIKE);
}

/// A jet of fire streams from the user onto the targets, crossed rings of fire
/// flaring where it lands.
///
/// Abilities: Blast Breath.
void BattleEffect_RunBlastBreath(struct BattleEffectParams *args) {
  BattleEffect_RunBreaths(args, BREATH_BLAST);
}
#undef TRIPLE_CHOMP_FRAMES
#undef TRIPLE_CHOMP_BITES
#undef TRIPLE_CHOMP_BITE_HEIGHT
#undef TRIPLE_CHOMP_SPARK_COUNT
#undef TRIPLE_CHOMP_BITE_SPARKS
#undef TRIPLE_CHOMP_BITE_SPARK_SLOTS
#undef BITE_RING
#undef TRIPLE_CHOMP_RING_FRAMES
#undef TRIPLE_CHOMP_RING_GROWTH
#undef TRIPLE_CHOMP_RING_FADE
