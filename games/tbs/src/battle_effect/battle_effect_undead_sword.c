/// @file
/// Undead Sword.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define UNDEAD_SWORD_FRAMES 54

/// The lime column sheet's left half of a jagged, pointed blade of light,
/// drawn twice, the second mirrored beside it.
#define LIME_COLUMN_WIDTH 14
#define LIME_COLUMN_HEIGHT 80
/// The blade's foot: the canvas row it stands on, which the clouds billow from.
#define UNDEAD_SWORD_FOOT_Y 112
/// The clouds' pictures, after the half blade.
#define UNDEAD_SWORD_PUFF_SHEET PICTURE_BYTES(LIME_COLUMN)
#define UNDEAD_SWORD_PUFFS 16
/// A cloud's seven pictures, three frames each, the last held until it goes.
#define UNDEAD_SWORD_PUFF_PICTURES 7
#define UNDEAD_SWORD_PUFF_FRAMES 29

/// A blade of lime light thrusts up out of the ground under the first target
/// and sinks again, amid clouds of lime smoke that billow up round it.
///
/// Abilities: Undead Sword.
void BattleEffect_RunUndeadSword(struct BattleEffectParams *args) {
  void **slots;
  void **slot;
  struct BattleEffectWork *work;
  void *canvas;
  void *sprites;
  DrawRectangle draw[2];
  struct ScreenPoint screenPosition;
  struct Particle *puff;
  struct Particle *particle;
  s32 frame;
  s32 i;
  s32 height;
  s32 composite;
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
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_LIME_COLUMN_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET,
                             work->sheet + UNDEAD_SWORD_PUFF_SHEET, 1, 0);
  BattleEffect_FetchRectangleBlitters(work->args->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The canvas is scrolled to stand its middle over the target.
  Projection_Unit(work->args->targets[0], &screenPosition);
  REG_BG2X = (CANVAS_SIZE / 2 - screenPosition.x) << 8;

  // The clouds burst out from the blade's foot and drift upward.
  for (i = 0, puff = work->particles; i != UNDEAD_SWORD_PUFFS; i++) {
    speed = (Random16() & 0x1ff) + 128;
    angle = Random16() & TRIG_ANGLE_MASK;
    puff->x = toFIXED(CANVAS_SIZE / 2);
    puff->y = toFIXED(UNDEAD_SWORD_FOOT_Y);
    puff->velocityX = (Trig_Sin(angle) * speed) >> 8;
    puff->velocityY = (Trig_Cos(angle) * speed) >> 9;
    puff->variant = Random16() & 7;
    puff++;
  }
  work->shakeFrames = 8;
  for (frame = 0; frame != UNDEAD_SWORD_FRAMES; frame++) {
    // The strike, with a second 8-frame shake.
    if (frame == 10) {
      work->shakeFrames = 8;
      EFFECT_SHOW_DAMAGE(SE_POWER_UP);
      BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_NUDGE);
      BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
    }

    // The blade thrusts up 12 pixels a frame and sinks 6 a frame. While it
    // grows it is added onto its fading trail and glows; at full height it
    // only lightens it.
    if (frame > 7) {
      if (frame <= 31)
        height = frame * 12 - 96;
      else
        height = 272 - frame * 6;
      if (height > 0) {
        if (height > LIME_COLUMN_HEIGHT) {
          height = LIME_COLUMN_HEIGHT;
          composite = COMPOSITE_LIGHTEN;
        } else {
          composite = COMPOSITE_ADDITIVE;
        }
        for (i = 0, x = 50; i != 2; i++, x += LIME_COLUMN_WIDTH) {
          if (i == 0)
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                                composite);
          else
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                                composite);
          draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
          EFFECT_BLITTER(HEAP_SLOT_RAM_CODE)(canvas, work->sheet, x,
                                             UNDEAD_SWORD_FOOT_Y - height,
                                             LIME_COLUMN_WIDTH, height);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      }
    }

    // The clouds billow out two a frame from frame 8.
    BattleEffect_FetchRectangleBlitters(work->args->side, draw);
    for (i = 0, particle = work->particles; i != UNDEAD_SWORD_PUFFS; i++) {
      if (frame >= i / 2 + 8 && particle->variant < UNDEAD_SWORD_PUFF_FRAMES) {
        s32 drawX;
        s32 drawY;

        picture = particle->variant / 3;
        drawX = FIXEDtoINT(particle->x);
        drawY = FIXEDtoINT(particle->y);
        if (picture > UNDEAD_SWORD_PUFF_PICTURES - 1)
          picture = UNDEAD_SWORD_PUFF_PICTURES - 1;
        src = work->sheet + gUndeadSword_SparkPictures[picture] +
              UNDEAD_SWORD_PUFF_SHEET;
        size = gUndeadSword_SparkSizes[picture];
        draw[0](canvas, src, drawX - size / 2, drawY - size / 2, size, size);
        particle->variant++;
        Particle_AdvanceWithGravity2D(particle, 62, -toFIXED(1) / 8);
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

#undef UNDEAD_SWORD_FRAMES
#undef LIME_COLUMN_WIDTH
#undef LIME_COLUMN_HEIGHT
#undef UNDEAD_SWORD_FOOT_Y
#undef UNDEAD_SWORD_PUFF_SHEET
#undef UNDEAD_SWORD_PUFFS
#undef UNDEAD_SWORD_PUFF_PICTURES
#undef UNDEAD_SWORD_PUFF_FRAMES
