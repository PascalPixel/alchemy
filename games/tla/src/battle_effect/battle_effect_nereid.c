/// @file
/// The Nereid summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

// Nereid's drops burst up from the targets, fall back and later rise away; one
// that falls back into the waves frees its slot.

#define NEREID_FRAMES 124

#define NEREID_OBJECTS 12 ///< the sprites that draw Nereid on her turtle
/// Wrapping columns of water.
#define NEREID_COLUMNS 3
#define NEREID_DROPS 1024          ///< drops of spray in the map cell buffer
#define NEREID_COLUMN_PICTURE 1764 ///< the column of water in the sheet
#define NEREID_COLUMN_WIDTH 34
#define NEREID_COLUMN_HEIGHT 104

/// Nereid rides in on her turtle over rising waves, columns of water and spray
/// rise over the targets, and she bursts away. At frame 28, and every frame
/// from 32 to 79, spray bursts up beside her and falls back, and every twelve
/// frames the targets flash and jump. A or B skips frames 33 to 97.
///
/// Abilities: Nereid.
void BattleEffect_RunNereid(struct BattleEffectParams *args) {
  DrawRectangle draw[2];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  graphics = Ram_HeapSlots->battleSheet;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA,
                             work->sheet + NEREID_COLUMN_PICTURE, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct Particle *point;
    struct Particle *drop;
    struct Particle *drops;
    s32 *life;

    // Nereid rides in from the right.
    rise = 0;
    focus.x = px(256);
    focus.y = px(88);
    velocity.x = -px(16);
    velocity.y = -px(4);
    // Particles 24 to 39 are given a place and a delay that nothing reads
    // again.
    EFFECT_FREE_PARTICLES(k, work->particles, 64);
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }

    // Every drop in the map cell buffer starts free.
    life = &((struct Particle *)Ram_MapCellBuffer)->variant;
    for (k = 0; k != NEREID_DROPS; k++) {
      *life = -1;
      life += sizeof(struct Particle) / 4;
    }
    drops = (struct Particle *)Ram_MapCellBuffer;
    BattleEffect_SelectLivingTargets(work->args);
    wait(1);
    BattleEffect_SpawnObjects(NEREID_OBJECTS, SPRITE_NEREID, 2);

    for (frame = 0; frame != NEREID_FRAMES; frame++) {
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 32 && frame <= 97)
        frame = 98;
      if (frame == 120)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      // The waves rise.
      if (frame <= 15)
        rise += 2;

      // She slows, and is pushed back right once she passes x = 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / 64;
        velocity.y = velocity.y * 56 / 64;
        if (focus.x < px(120))
          velocity.x += px(1) / 2;
      }
      BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_NEREID, focus.x,
                                         focus.y);

      // Drops burst up among the targets: every free one at frame 28, then
      // sixteen a frame from 32 to 79.
      if (frame == 28) {
        for (k = 0; k != 256; k++) {
          drop = &drops[k];
          if (drop->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            drop->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            drop->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            drop->velocityX = ((Random16() & 63) - 32) << 14;
            drop->velocityY = (-(Random16() & 63) - 8) << 13;
            drop->variant = 0;
          }
        }
      }
      if (frame >= 32 && frame < 80) {
        s32 spawned = 0;

        for (k = 0; k != NEREID_DROPS; k++) {
          drop = &drops[k];
          if (drop->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            drop->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            drop->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            drop->velocityX = ((Random16() & 63) - 32) << 14;
            drop->velocityY = (-(Random16() & 63) - 8) << 13;
            drop->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        Sound_PlayFar(SE_SCATTER);
      if (frame == 32)
        Sound_PlayFar(SE_CRASH);
      if (frame == 80)
        Sound_PlayFar(SE_CLATTER);

      // The columns of water scroll up.
      if (frame >= 32 && frame < 80) {
        for (k = 0; k != NEREID_COLUMNS; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % NEREID_COLUMN_HEIGHT;

          draw[0](canvas, work->sheet + NEREID_COLUMN_PICTURE,
                  gNereid_Columns[k * 2] - NEREID_COLUMN_WIDTH / 2,
                  gNereid_Columns[k * 2 + 1] - span - NEREID_COLUMN_HEIGHT,
                  NEREID_COLUMN_WIDTH, NEREID_COLUMN_HEIGHT);
          draw[0](canvas, work->sheet + NEREID_COLUMN_PICTURE,
                  gNereid_Columns[k * 2] - NEREID_COLUMN_WIDTH / 2,
                  gNereid_Columns[k * 2 + 1] - span, NEREID_COLUMN_WIDTH, span);
        }
      }

      // The waves along the bottom.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32,
                  120 - rise, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
      }

      // Drops swell as they clear: they fall under gravity until frame 80, then
      // rise away, and a falling drop that reaches y = 104 is freed.
      for (k = 0; k != NEREID_DROPS; k++) {
        drop = &drops[k];
        if (drop->variant >= 0) {
          s32 size = k % 3 + 2;

          if (drop->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[drop->velocityY > 0](
              canvas, graphics + gBattleEffect_MotePictures[size - 1],
              Q_16_16_UPPER_HALF(drop->x) - size / 2,
              Q_16_16_UPPER_HALF(drop->y) - size, size, size * 2);
          drop->x += drop->velocityX;
          drop->y += drop->velocityY;
          if (frame > 80)
            drop->velocityY += -(px(1) / 2);
          else
            drop->velocityY += gNereid_Gravity[k & 3];
          drop->velocityX = drop->velocityX * 62 / 64;
          drop->velocityY = drop->velocityY * 62 / 64;
          drop->variant++;
          if (drop->velocityY > 0 && Q_16_16_UPPER_HALF(drop->y) > 104)
            drop->variant = -1;
        }
      }

      // The targets bounce and flash white every twelve frames from frame 30.
      if (frame <= 79) {
        for (k = 0; k != args->targetCount; k++) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *target =
                  BattleObject_GetFar(args->targets[k])->object;

              BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, TINT_KEEP, 0);
              target->velocityY = px(9) / 2;
              target->verticalMotionStrength = 0xab85;
            }
            if (phase == 6)
              BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_NORMAL,
                                       POSE_DAMAGE, TINT_KEEP, 0);
          }
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    }

    // She bursts away.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_NEREID, focus.x, focus.y);
    for (k = 0; k != NEREID_OBJECTS; k++)
      Sprite_DestroyFar(work->sprites[k]);
  }
  BattleEffect_EndCanvasLayer();
}
