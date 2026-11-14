/// @file
/// The Nereid summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

// Nereid's drops burst up from the targets, fall back and later rise away; one
// that falls back into the waves frees its slot. The Titan Blade throws its
// dust the same way.

#define NEREID_FRAMES 124

#define NEREID_OBJECTS 12 ///< the sprites that draw Nereid on her turtle
/// Wrapping columns of water; gNereid_Columns has a fourth row that nothing
/// draws.
#define NEREID_COLUMNS 3
#define NEREID_DROPS 1024          ///< drops of spray in PARTICLES
#define NEREID_COLUMN_PICTURE 1764 ///< the column of water in the sheet

/// Nereid rides in on her turtle over rising waves, columns of water and spray
/// rise over the targets, and she bursts away.
///
/// Abilities: Nereid.
void BattleEffect_RunNereid(struct BattleEffectParams *args) {
  BlitterPair draw;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;
  u8 *palette;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  graphics = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleEffect_FetchRectangleBlitters(0, draw);

  palette = Resource_GetTableEntry(RESOURCE_WATER_SPRAY_SHEET);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  palette = Resource_GetTableEntry(RESOURCE_FIRE_PILLAR_SHEET_A);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet + NEREID_COLUMN_PICTURE);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      graphics);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct Particle *point;
    struct Particle *drop;

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
    EFFECT_FREE_PARTICLES(k, PARTICLES, NEREID_DROPS);
    BattleEffect_SelectLivingTargets(work->args);
    wait(1);
    BattleEffect_SpawnObjects(NEREID_OBJECTS, SPRITE_NEREID, 2);

    for (frame = 0; frame != NEREID_FRAMES; frame++) {
      if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 32 && frame <= 97)
        frame = 98;
      if (frame == 120)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      // The waves rise.
      if (frame <= 15)
        rise += 2;

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

      // Drops burst up among the targets.
      if (frame == 28) {
        for (k = 0; k != 256; k++) {
          drop = &PARTICLES[k];
          if (drop->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            drop->x = ((Trig_Sin(angle) * radius) >> 3) + px(32);
            drop->y = ((Trig_Cos(angle) * radius) >> 2) + px(96);
            drop->velocityX = ((Random16() & 63) - 32) << 14;
            drop->velocityY = (-(Random16() & 63) - 8) << 13;
            drop->variant = 0;
          }
        }
      }
      if (frame >= 32 && frame < 80) {
        s32 spawned = 0;

        for (k = 0; k != NEREID_DROPS; k++) {
          drop = &PARTICLES[k];
          if (drop->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            drop->x = ((Trig_Sin(angle) * radius) >> 3) + px(32);
            drop->y = ((Trig_Cos(angle) * radius) >> 2) + px(96);
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
          s32 span = (frame * 16 - 256 + k * 25) % 104;

          draw[0](canvas, work->sheet + NEREID_COLUMN_PICTURE,
                  gNereid_Columns[k][NEREID_COLUMN_X] - 17,
                  gNereid_Columns[k][NEREID_COLUMN_Y] - span - 104, 34, 104);
          draw[0](canvas, work->sheet + NEREID_COLUMN_PICTURE,
                  gNereid_Columns[k][NEREID_COLUMN_X] - 17,
                  gNereid_Columns[k][NEREID_COLUMN_Y] - span, 34, span);
        }
      }

      // The waves along the bottom.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32,
                  120 - rise, 32, 32);
      }

      // Drops swell as they clear.
      for (k = 0; k != NEREID_DROPS; k++) {
        drop = &PARTICLES[k];
        if (drop->variant >= 0) {
          s32 size = k % 3 + 2;

          if (drop->velocityY > 0)
            size += 2;
          EFFECT_SWELL_MOTE(size, frame);
          EFFECT_DRAW_MOTE(draw[drop->velocityY > 0], canvas, graphics,
                           Q_16_16_TO_INT(drop->x), Q_16_16_TO_INT(drop->y),
                           size);
          drop->x += drop->velocityX;
          drop->y += drop->velocityY;
          EFFECT_FALL_MOTE(drop->velocityY, gNereid_Gravity[k & 3], frame);
          drop->velocityX = drop->velocityX * 62 / 64;
          drop->velocityY = drop->velocityY * 62 / 64;
          drop->variant++;
          if (drop->velocityY > 0 && Q_16_16_TO_INT(drop->y) > 104)
            drop->variant = -1;
        }
      }

      // The targets bounce and flash white.
      if (frame <= 79) {
        EFFECT_FOR_EACH_TARGET(k, work->args) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct Actor *actor =
                  GetBattleObjectSlotFar(work->args->targets[k])->actor;

              BattleEffect_TintBattler(work->args->targets[k],
                                       SPRITE_COLOR_WHITE, POSE_DAMAGE,
                                       TINT_KEEP, 0);
              actor->velocityY = px(9) / 2;
              actor->gravity = BATTLE_UNIT_GRAVITY;
            }
            if (phase == 6)
              BattleEffect_TintBattler(work->args->targets[k],
                                       SPRITE_COLOR_NORMAL, POSE_DAMAGE,
                                       TINT_KEEP, 0);
          }
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    }

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_NEREID, focus.x, focus.y);
    for (k = 0; k != NEREID_OBJECTS; k++)
      Sprite_DestroyFar(work->sprites[k]);
  }
  BattleEffect_EndCanvasLayer();
}

#undef NEREID_FRAMES
#undef NEREID_OBJECTS
#undef NEREID_COLUMNS
#undef NEREID_DROPS
#undef NEREID_COLUMN_PICTURE
