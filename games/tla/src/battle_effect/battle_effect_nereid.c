/// @file
/// The Nereid summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

#define NEREID_FRAMES 124
/// The columns of water rise among the targets, from the crash to the clatter.
#define NEREID_COLUMNS_FRAME 32
#define NEREID_COLUMNS_END_FRAME 80

#define NEREID_OBJECTS 12 ///< the sprites that draw Nereid on her turtle
/// Wrapping columns of water.
#define NEREID_COLUMNS 3
#define NEREID_DROPS PARTICLE_POOL_COUNT ///< drops of spray in PARTICLES
#define NEREID_COLUMN_PICTURE 1764       ///< the column of water in the sheet
/// The canvas line below which a falling drop has sunk into the waves.
#define NEREID_WAVES_Y 104
/// Particles given a place and a delay that nothing reads.
#define NEREID_UNREAD_FIRST 24
#define NEREID_UNREAD_COUNT 16

/// Nereid rides in on her turtle over rising waves, columns of water and spray
/// rise over the targets, and she bursts away. A or B skips ahead to her
/// leaving.
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
  Resource_LoadAndDecompress(RESOURCE_WATER_SPRAY_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FIRE_PILLAR_SHEET_A,
                             work->sheet + NEREID_COLUMN_PICTURE, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
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

    // Nereid rides in from the right.
    rise = 0;
    focus.x = toFIXED(256);
    focus.y = toFIXED(88);
    velocity.x = -toFIXED(16);
    velocity.y = -toFIXED(4);
    EFFECT_FREE_PARTICLES(k, work->particles, 64);
    // DEAD: particles NEREID_UNREAD_FIRST on get a place and delay nothing
    // reads; the ROM keeps the stores.
    for (k = 0; k != NEREID_UNREAD_COUNT; k++) {
      point = &work->particles[NEREID_UNREAD_FIRST + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }

    EFFECT_FREE_PARTICLES(k, PARTICLES, NEREID_DROPS);
    drops = PARTICLES;
    BattleEffect_SelectLivingTargets(work->args);
    wait(1);
    BattleEffect_SpawnObjects(NEREID_OBJECTS, SPRITE_NEREID, 2);

    for (frame = 0; frame != NEREID_FRAMES; frame++) {
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame > NEREID_COLUMNS_FRAME &&
          frame <= 97)
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
        velocity.x = velocity.x * 58 / DAMPING_SCALE;
        velocity.y = velocity.y * 56 / DAMPING_SCALE;
        if (focus.x < toFIXED(120))
          velocity.x += toFIXED(1) / 2;
      }
      BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_NEREID, focus.x,
                                         focus.y);

      // Drops burst up among the targets.
      if (frame == 28) {
        for (k = 0; k != NEREID_BURST_DROPS; k++) {
          drop = &drops[k];
          if (drop->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            drop->x = ((Trig_Sin(angle) * radius) >> 3) + toFIXED(32);
            drop->y = ((Trig_Cos(angle) * radius) >> 2) + toFIXED(96);
            drop->velocityX = ((Random16() & 63) - 32) << 14;
            drop->velocityY = (-(Random16() & 63) - 8) << 13;
            drop->variant = 0;
          }
        }
      }
      if (frame >= NEREID_COLUMNS_FRAME && frame < NEREID_COLUMNS_END_FRAME) {
        s32 spawned = 0;

        for (k = 0; k != NEREID_DROPS; k++) {
          drop = &drops[k];
          if (drop->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            drop->x = ((Trig_Sin(angle) * radius) >> 3) + toFIXED(32);
            drop->y = ((Trig_Cos(angle) * radius) >> 2) + toFIXED(96);
            drop->velocityX = ((Random16() & 63) - 32) << 14;
            drop->velocityY = (-(Random16() & 63) - 8) << 13;
            drop->variant = 0;
            spawned++;
            if (spawned == NEREID_DROPS_A_FRAME)
              break;
          }
        }
      }
      if (frame == 0)
        Sound_PlayFar(SE_SCATTER);
      if (frame == NEREID_COLUMNS_FRAME)
        Sound_PlayFar(SE_CRASH);
      if (frame == NEREID_COLUMNS_END_FRAME)
        Sound_PlayFar(SE_CLATTER);

      // The columns of water scroll up.
      if (frame >= NEREID_COLUMNS_FRAME && frame < NEREID_COLUMNS_END_FRAME) {
        for (k = 0; k != NEREID_COLUMNS; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % FIRE_PILLAR_HEIGHT;

          draw[0](canvas, work->sheet + NEREID_COLUMN_PICTURE,
                  gNereid_Columns[k * 2] - FIRE_PILLAR_WIDTH / 2,
                  gNereid_Columns[k * 2 + 1] - span - FIRE_PILLAR_HEIGHT,
                  FIRE_PILLAR_WIDTH, FIRE_PILLAR_HEIGHT);
          draw[0](canvas, work->sheet + NEREID_COLUMN_PICTURE,
                  gNereid_Columns[k * 2] - FIRE_PILLAR_WIDTH / 2,
                  gNereid_Columns[k * 2 + 1] - span, FIRE_PILLAR_WIDTH, span);
        }
      }

      // The waves along the bottom.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet,
                  k * WATER_WAVE_WIDTH + ((frame / 4) & 31) - WATER_WAVE_WIDTH,
                  120 - rise, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
      }

      // Drops swell as they clear: they fall under gravity until
      // NEREID_COLUMNS_END_FRAME, then rise away, and a falling drop that sinks
      // into the waves is freed.
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
            size = MOTE_PICTURES;
          EFFECT_DRAW_MOTE(draw[drop->velocityY > 0], canvas, graphics,
                           FIXED_INTEGER_PART(drop->x),
                           FIXED_INTEGER_PART(drop->y), size);
          drop->x += drop->velocityX;
          drop->y += drop->velocityY;
          if (frame > NEREID_COLUMNS_END_FRAME)
            drop->velocityY += -toFIXED(1) / 2;
          else
            drop->velocityY += gNereid_Gravity[k & 3];
          drop->velocityX = drop->velocityX * 62 / DAMPING_SCALE;
          drop->velocityY = drop->velocityY * 62 / DAMPING_SCALE;
          drop->variant++;
          if (drop->velocityY > 0 &&
              FIXED_INTEGER_PART(drop->y) > NEREID_WAVES_Y)
            drop->variant = -1;
        }
      }

      // The targets bounce and flash white.
      if (frame < NEREID_COLUMNS_END_FRAME) {
        for (k = 0; k != args->targetCount; k++) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *target =
                  BattleObject_GetFar(args->targets[k])->object;

              BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, TINT_KEEP, 0);
              target->velocityY = toFIXED(9) / 2;
              target->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
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

// NEREID_FRAMES, NEREID_COLUMNS_FRAME, NEREID_COLUMNS_END_FRAME,
// NEREID_OBJECTS, NEREID_COLUMNS, NEREID_DROPS, NEREID_COLUMN_PICTURE,
// NEREID_WAVES_Y, NEREID_UNREAD_FIRST and NEREID_UNREAD_COUNT stay defined:
// particle_streams.c's copy of Nereid runs by them.
