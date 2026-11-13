/// @file
/// Undead Sword.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The blast sheet's seven spark pictures, after the lime column's 14x80
/// picture.
#define UNDEAD_SWORD_SPARK_SHEET (14 * 80)

/// Two mirrored columns of light rise at the first target's feet and sink
/// again, scattering 16 sparks that drift upward; the target is struck on frame
/// 10.
///
/// Abilities: Undead Sword (BATTLE_EFFECT_UNDEAD_SWORD).
void BattleEffect_RunUndeadSword(struct BattleEffectParams *args) {
  void **slots;
  void **slot;
  struct BattleEffectWork *work;
  void *canvas;
  void *sprites;
  DrawRectangle draw[2];
  struct ScreenPoint screenPosition;
  struct Particle *spark;
  struct Particle *particle;
  s32 frame;
  s32 i;
  s32 height;
  s32 mode;
  s32 x;
  u8 *src;
  s32 speed;
  s32 angle;
  s32 picture;
  s32 size;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  slot = slots;
  work = *slot++;
  canvas = *slot;
  work->args = args;
  sprites =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_EFFECT, HEAP_SLOT_BATTLE_SHEET);
  EFFECT_START_NARROW_CANVAS(0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + UNDEAD_SWORD_SPARK_SHEET, 1, 0);
  BattleEffect_FetchRectangleBlitters(work->args->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Shift BG2 so that canvas x 64 lines up with the target.
  Projection_Unit(work->args->targets[0], &screenPosition);
  REG_BG2X = (64 - screenPosition.x) << 8;

  // Scatter sparks in all directions from (64, 112), at 1/2 to 2.5 pixels a
  // frame across and half that up or down, then drift upward; each starts 0
  // to 7 frames old, so they die at different times.
  for (i = 0, spark = work->particles; i != 16; i++) {
    speed = (Random16() & 0x1ff) + 128;
    angle = Random16() & TRIG_ANGLE_MASK;
    spark->x = px(64);
    spark->y = px(112);
    spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
    spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
    spark->variant = Random16() & 7;
    spark++;
  }
  work->shakeFrames = 8;
  for (frame = 0; frame != 54; frame++) {
    // The strike, with a second 8-frame shake.
    if (frame == 10) {
      work->shakeFrames = 8;
      EFFECT_SHOW_DAMAGE(SE_POWER_UP);
      BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_NUDGE);
      BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
    }

    // The columns grow 12 pixels a frame from frame 8 and shrink 6 a frame
    // after frame 31, held at 80 pixels between.
    if (frame > 7) {
      if (frame <= 31)
        height = frame * 12 - 96;
      else
        height = 272 - frame * 6;
      if (height > 0) {
        if (height > 80) {
          height = 80;
          mode = COMPOSITE_LIGHTEN;
        } else {
          mode = COMPOSITE_ADDITIVE;
        }
        for (i = 0, x = 50; i != 2; i++, x += 14) {
          if (i == 0)
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, mode);
          else
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, mode);
          draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
          EFFECT_BLITTER(HEAP_SLOT_RAM_CODE)(canvas, work->sheet, x,
                                             112 - height, 14, height);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      }
    }

    // Two more sparks start each frame from frame 8.
    BattleEffect_FetchRectangleBlitters(work->args->side, draw);
    for (i = 0, particle = work->particles; i != 16; i++) {
      if (frame >= i / 2 + 8 && particle->variant <= 28) {
        s32 drawX;
        s32 drawY;

        picture = particle->variant / 3;
        drawX = Q_16_16_TO_INT(particle->x);
        drawY = Q_16_16_TO_INT(particle->y);
        if (picture > 6)
          picture = 6;
        src = work->sheet + gUndeadSword_SparkPictures[picture] +
              UNDEAD_SWORD_SPARK_SHEET;
        size = gUndeadSword_SparkSizes[picture];
        draw[0](canvas, src, drawX - size / 2, drawY - size / 2, size, size);
        particle->variant++;
        Particle_AdvanceWithGravity2D(particle, 62, -px(1) / 8);
      }
      particle++;
    }
    EFFECT_RELEASE_BLITTERS();
    if (frame <= 7)
      BattleCamera_Shake(2, 2);
    else
      BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef UNDEAD_SWORD_SPARK_SHEET
