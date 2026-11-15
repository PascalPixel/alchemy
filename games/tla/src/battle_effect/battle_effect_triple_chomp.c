/// @file
/// Triple Chomp, Rolling Flame and Ghost Soldier.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The sparks live in the map cell buffer's second 16 KB.
#define SPARKS ((struct Particle *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512

/// Each bite's anchor, where its jaw is drawn: the first three particles.
#define BITE_ANCHOR(bite) (&work->particles[bite])

// A texture for the model kernels: its size as powers of two, then its
// pixels.

// Triple Chomp: the caster lunges 80% of the way to the
// target, and three bites close on it at frames 0, 12 and 24. Each bite
// throws 32 blue sparks that fly off away from the caster, jolts and flashes
// the target, and draws a jaw: a textured quad tilted back, turned a quarter
// further each bite, that grows and fades over 16 frames.
/// Triple Chomp's effect: the caster lunges, three bites close on the target.
void BattleEffect_RunTripleChomp(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2]; // Unused: [1] gives the stack frame its 4 bytes.
  u8 *graphics;
  void *vertices;
  struct RenderModel *model;
  struct ModelTexture texture;
  struct ScreenPoint position;
  struct Particle *drawn;
  s32 i;
  s32 j;
  u8 *pixels = Ram_MapCellBuffer;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;

  // Set up the canvas, then let the caster lunge.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  BattleEffect_SetApproachMotion(args->user, args->targets[0], 8, 80);
  wait(8);

  // A clipped, mirrored MAX blitter for the sparks; the jaw's texture, the
  // spark sheet and the blue flame's colours.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, pixels, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                       SHEET_PALETTE_BYTES);

  for (frame = 0; frame != 50; frame++) {
    if (frame == 0) {
      for (i = 0; i != SPARK_COUNT; i++)
        SPARKS[i].variant = -1;
    }

    // This frame's jaw: one model drawing the quad with the 128 x 128
    // texture.
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    EffectPosition_ProjectBattler(args->targets[0], &position);
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = pixels;
    model->mesh = gBattleEffect_LargeSquareMesh;
    model->kind = 7;
    model->texture = &texture;
    model->vertices = vertices;

    for (i = 0; i != 3; i++) {
      // The bite closes: 32 sparks fly from the target's middle, and the
      // target jolts and flashes. The last bite ends the action.
      if (frame == gTripleChomp_BiteFrames[i]) {
        for (j = 0; j != 32; j++) {
          struct Particle *spark = &SPARKS[i * 128 + j];
          s32 angle;
          s32 force;

          spark->x = px(position.x / 2);
          spark->y = px(position.y - 24);
          angle = Random16();
          force = (Random16() & 255) + 32;
          spark->velocityX = (Trig_Sin(angle) * force) >> 8;
          spark->velocityY = (Trig_Cos(angle) * force) >> 6;
          if (args->side == EFFECT_SIDE_ENEMIES)
            spark->velocityX += -(px(1) / 2);
          else
            spark->velocityX += px(1) / 2;
          BITE_ANCHOR(i)->x = position.x;
          BITE_ANCHOR(i)->y = position.y - 24;
          spark->variant = (Random16() & 7) + 16;
        }
        if (i == 2)
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        else
          Sound_PlayFar(SE_HEAVY_IMPACT);
        BattleEffect_StartActorMotion(args->targets[0], 1, px(2), px(8), 0,
                                      i * 20 + 120);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        work->shakeFrames = 8;
      }

      // For 16 frames the jaw grows from a quarter of its size and fades.
      if (frame >= gTripleChomp_BiteFrames[i] &&
          frame < gTripleChomp_BiteFrames[i] + 16) {
        s32 age = frame - gTripleChomp_BiteFrames[i];
        s32 alpha = 64 - age * 8;
        s32 scale = age * 10192 + Q16_ONE / 4;

        if (alpha > 0)
          alpha = 0;
        if (alpha > -64) {
          model->shade = alpha;
          Transform_Reset();
          Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
          Transform_ApplyTranslation(px(BITE_ANCHOR(i)->x - 128),
                                     px(BITE_ANCHOR(i)->y - 64), 0);
          Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
          Transform_ApplyPitch(-TRIG_TURN / 16);
          Transform_ApplyUniformScale(scale);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
      }
    }

    // Each spark is drawn and moved twice a frame, shrinking as it dies.
    drawn = SPARKS;
    for (i = 0; i != SPARK_COUNT; i++) {
      s32 left = drawn->variant;

      if (left >= 0) {
        s32 size = (left >> 2) + 3;

        draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                Q_16_16_UPPER_HALF(drawn->x) - size / 2,
                Q_16_16_UPPER_HALF(drawn->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(drawn, 62, 0);
        draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                Q_16_16_UPPER_HALF(drawn->x) - size / 2,
                Q_16_16_UPPER_HALF(drawn->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(drawn, 62, 0);
        drawn->variant--;
      }
      drawn++;
    }

    Heap_Release(model);
    Heap_Release(vertices);
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Beam, Cycle Beam, Searing Beam.
void BattleEffect_RunBeam(struct BattleEffectParams *args) {
  BattleEffect_RunBreaths(args, BREATH_BEAM);
}

/// Abilities: Rolling Flame.
void BattleEffect_RunRollingFlame(struct BattleEffectParams *args) {
  args->variant = 2;
  BattleEffect_RunBreaths(args, BREATH_ROLLING_FLAME);
}

/// Abilities: Ghost Soldier, Pike Assault.
void BattleEffect_RunGhostSoldier(struct BattleEffectParams *args) {
  args->variant = 2;
  BattleEffect_RunBreaths(args, BREATH_PIKE);
}

/// Abilities: Blast Breath.
void BattleEffect_RunBlastBreath(struct BattleEffectParams *args) {
  BattleEffect_RunBreaths(args, BREATH_BLAST);
}
#undef BLITTER
#undef SPARKS
#undef SPARK_COUNT
#undef BITE_ANCHOR
