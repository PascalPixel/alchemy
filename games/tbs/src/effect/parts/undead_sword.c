/// @file
/// Undead Sword.
// Part of effect.c.

#include "../effect_internal.h"

/// The blast sheet's seven spark pictures, after the lime column's 14x80
/// picture.
#define UNDEAD_SWORD_SPARK_SHEET (14 * 80)

/// Two mirrored columns of light rise at the first target's feet and sink
/// again, scattering 16 sparks that drift upward; the target is struck on frame
/// 10.
///
/// Abilities: Undead Sword (BATTLE_FX_UNDEAD_SWORD).
///
/// The effect runs 54 frames. BG2 is moved so that the canvas's centre line
/// stands on the target. The sparks are work->particles 0 to 15, variant their
/// age in frames.
void BattleEffect_RunUndeadSword(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  void *sprites;
  DrawRectangle draw[2];
  struct EffectPosition screenPosition;
  struct EffectStep *spark;
  struct EffectStep *particle;
  s32 frame;
  s32 i;
  s32 height;
  s32 mode;
  s32 x;
  u8 *src;
  s32 speed;
  s32 angle;
  s32 cell;
  s32 size;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  sprites = slots[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
  EFFECT_BEGIN_NARROW_CANVAS(0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + UNDEAD_SWORD_SPARK_SHEET, 1, 0);
  BattleEffect_FetchRectangleBlitters(work->effect->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Shift BG2 so that canvas x 64 lines up with the target.
  EffectPosition_ProjectBattler(work->effect->actors[0], &screenPosition);
  REG_BG2X = (64 - screenPosition.x) << 8;

  // Scatter sparks in all directions from (64, 112), at 1/2 to 2.5 pixels a
  // frame across and half that up or down, then drift upward; each starts 0
  // to 7 frames old, so they die at different times.
  for (i = 0, spark = work->particles; i != 16; i++) {
    speed = (Random16() & 0x1ff) + 128;
    angle = Random16() & 0xffff;
    spark->x = 0x400000;
    spark->y = 0x700000;
    spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
    spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
    spark->variant = Random16() & 7;
    spark++;
  }
  work->shakeFrames = 8;
  for (frame = 0; frame != 54; frame++) {
    // Frame 10: the strike, with a second 8-frame shake.
    if (frame == 10) {
      work->shakeFrames = 8;
      EFFECT_SHOW_DAMAGE(SE_212);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0],
                                         KNOCKBACK_NUDGE);
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
    }

    // The columns grow 12 pixels a frame from frame 8 and shrink 6 a frame
    // after frame 31, held at 80 pixels between. Held, they are drawn
    // brightest-wins, otherwise additive. The two 14-pixel halves stand on
    // line 112 at x 50 and 64, the second mirrored, so the light is one
    // 28-pixel column about x 64; each half builds its own blitter and
    // releases it at once.
    if (frame > 7) {
      if (frame <= 31)
        height = frame * 12 - 96;
      else
        height = 272 - frame * 6;
      if (height > 0) {
        if (height > 80) {
          height = 80;
          mode = COMPOSITE_MAX;
        } else {
          mode = COMPOSITE_ADDITIVE;
        }
        for (i = 0, x = 50; i != 2; i++, x += 14) {
          if (i == 0)
            EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, mode);
          else
            EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, mode);
          draw[0] = ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER]);
          ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER])(
              canvas, work->sheet, x, 112 - height, 14, height);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
      }
    }

    // Two more sparks start each frame from frame 8; each advances to the
    // next of seven pictures every three frames and drifts upward, pulled by
    // 1/8 pixel a frame each frame and slowed to 62/64, until it is 29 frames
    // old. The blitters are fetched and released each frame, as the columns
    // took the slot.
    BattleEffect_FetchRectangleBlitters(work->effect->side, draw);
    for (i = 0, particle = work->particles; i != 16; i++) {
      if (frame >= i / 2 + 8 && particle->variant <= 28) {
        s32 px;
        s32 py;

        cell = particle->variant / 3;
        px = Q_16_16_TO_INT(particle->x);
        py = Q_16_16_TO_INT(particle->y);
        if (cell > 6)
          cell = 6;
        src = work->sheet + gUndeadSword_SparkCells[cell] +
              UNDEAD_SWORD_SPARK_SHEET;
        size = gUndeadSword_SparkSizes[cell];
        draw[0](canvas, src, px - size / 2, py - size / 2, size, size);
        particle->variant++;
        EffectStep_AdvanceWithGravity2D(particle, 62, -0x2000);
      }
      particle++;
    }
    EFFECT_RELEASE_BLITTERS();
    if (frame <= 7)
      Camera_ApplyShake(2, 2);
    else
      Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef UNDEAD_SWORD_SPARK_SHEET
