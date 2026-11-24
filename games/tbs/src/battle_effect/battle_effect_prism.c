/// @file
/// Prism.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define PRISM_SHARDS_PER_CRYSTAL 16
#define PRISM_GROUND_Y toFIXED(56) ///< y 56, where the crystals shatter
#define PRISM_SHARD_PICTURES                                                   \
  PICTURE_BYTES(CRYSTAL)  ///< the shards, after the crystal
#define PRISM_SHATTERED 1 ///< a crystal's z once it has hit the ground
/// The x in gPrism_ShardStarts over the crystal's middle.
#define PRISM_SHARD_START_MIDDLE 40
/// The shard kinds a crystal breaks into, and the pictures of each as it
/// spins: a shard turns to its next picture each PRISM_SHARD_SPIN_STEP of
/// spin.
#define PRISM_SHARD_KINDS 5
#define PRISM_SHARD_SPIN_PICTURES 3
#define PRISM_SHARD_SPIN_STEP 96

/// The strongest Prism turns the camera over its first frames, at a steady
/// speed and then slowing to a stop over the last eight.
#define PRISM_TURN_FRAMES 104
#define PRISM_TURN_SPEED 192
#define PRISM_TURN_SLOWING_FRAMES 8
/// How much the turn slows each of those frames.
#define PRISM_TURN_SLOWDOWN (PRISM_TURN_SPEED / PRISM_TURN_SLOWING_FRAMES)

/// How far each channel of the phase palette sways either way, in colour
/// levels.
#define PHASE_SWAY_LEVELS 16

/// Crystals drop out of the sky onto the targets' side and shatter on the
/// ground into spinning shards, flashing the targets and shaking the screen.
///
/// Abilities: Prism, Hail Prism, Freeze Prism.
void BattleEffect_RunPrism(struct BattleEffectParams *args) {
  BlitterPair draw;
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  struct Particle *seed;
  struct Particle *crystal;
  struct Particle *shards;
  struct Particle *shard;
  s32 x;
  s32 picture;
  s32 i;
  s32 j;

  shards = PARTICLES;
  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_CRYSTAL_SHEET, state->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(state->args->side, draw);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // The crystals wait above the screen, each eight frames behind the last,
  // and drift sideways so that they land over the targets.
  for (j = 0; j != gPrism_Counts[state->args->variant][PRISM_CRYSTAL_COUNT];
       j++) {
    seed = &state->particles[j];
    // DEAD: x keeps a first random number, drawn and never read; the ROM
    // keeps the mask and the store.
    x = Random16() & 31;
    seed->y = -toFIXED(64);
    if (state->args->side == EFFECT_SIDE_PARTY) {
      x = INTtoFIXED((Random16() & 31) + 80);
      seed->velocityX = (Random16() & 63) << 12;
    } else {
      x = INTtoFIXED((Random16() & 31) + 8);
      seed->velocityX = -(Random16() & 63) << 12;
    }
    seed->x = x - seed->velocityX * 18;
    seed->velocityY = 0;
    seed->z = 0;
    seed->variant = j * 8;
  }

  for (frame = 0; frame != gPrism_Counts[state->args->variant][PRISM_FRAMES];
       frame++) {
    if (state->args->variant == EFFECT_VARIANT_STRONGEST &&
        frame < PRISM_TURN_FRAMES) {
      struct BattleCamera *camera = gBattleCamera;
      s32 speed;

      if (frame < PRISM_TURN_FRAMES - PRISM_TURN_SLOWING_FRAMES)
        speed = PRISM_TURN_SPEED;
      else
        speed = PRISM_TURN_FRAMES * PRISM_TURN_SLOWDOWN -
                frame * PRISM_TURN_SLOWDOWN;
      if (state->args->side == EFFECT_SIDE_ENEMIES)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == gPrism_Counts[state->args->variant][PRISM_FRAMES] - 80)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == gPrism_Counts[state->args->variant][PRISM_FRAMES] - 8) {
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(6));
    }
    if (frame <= gPrism_Counts[state->args->variant][PRISM_FRAMES] - 8) {
      for (i = 0; i != gPrism_Counts[state->args->variant][PRISM_CRYSTAL_COUNT];
           i++) {
        crystal = &state->particles[i];
        if (crystal->z == PRISM_SHATTERED) {
          // A shattered crystal's shards fly and spin; kind 4 asks for pictures
          // past the twelve the tables hold, whose heights read as zero, so
          // those three shards of every crystal never show.
          // XXX (no evidence): whether a fifth kind of shard was meant; the
          // twelve pictures and their tables are all the ROM holds.
          for (j = 0; j != PRISM_SHARDS_PER_CRYSTAL; j++) {
            picture = (j % PRISM_SHARD_KINDS) * PRISM_SHARD_SPIN_PICTURES +
                      (shards[i * PRISM_SHARDS_PER_CRYSTAL + j].variant /
                       PRISM_SHARD_SPIN_STEP) %
                          PRISM_SHARD_SPIN_PICTURES;
            shard = &shards[i * PRISM_SHARDS_PER_CRYSTAL + j];
            draw[j <= 2](
                canvas,
                state->sheet + gPrism_ShardPictures[picture] +
                    PRISM_SHARD_PICTURES,
                FIXEDtoINT(shard->x) - gPrism_ShardWidths[picture] / 2,
                FIXEDtoINT(shard->y) - gPrism_ShardHeights[picture] / 2,
                gPrism_ShardWidths[picture], gPrism_ShardHeights[picture]);
            Particle_AdvanceWithGravity2D(shard, PARTICLE_DAMPING_NONE,
                                          toFIXED(1) / 8);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= crystal->variant) {
          // A falling crystal.
          draw[i & 1](canvas, state->sheet,
                      FIXEDtoINT(crystal->x) - CRYSTAL_WIDTH / 2,
                      FIXEDtoINT(crystal->y), CRYSTAL_WIDTH, CRYSTAL_HEIGHT);
          Particle_AdvanceWithGravity2D(crystal, PARTICLE_DAMPING_NONE,
                                        FIXED_ONE);
          if (crystal->y > PRISM_GROUND_Y) {
            // Shatter it on reaching the ground.
            crystal->z = PRISM_SHATTERED;
            crystal->y = PRISM_GROUND_Y;
            for (j = 0; j != PRISM_SHARDS_PER_CRYSTAL; j++) {
              shard = &shards[i * PRISM_SHARDS_PER_CRYSTAL + j];
              shard->x = INTtoFIXED(gPrism_ShardStarts[j][PRISM_SHARD_START_X] -
                                    PRISM_SHARD_START_MIDDLE) +
                         crystal->x;
              shard->y = INTtoFIXED(gPrism_ShardStarts[j][PRISM_SHARD_START_Y]);
              shard->velocityX = ((Random16() & 127) - 64) << 11;
              shard->velocityY = -(Random16() & 127) << 11;
              if (i & 1) {
                shard->velocityX *= 2;
                shard->velocityY *= 2;
              }
              shard->z = 32;
              shard->variant = 0;
            }
            state->shakeFrames = 8;
            Sound_PlayFar(SE_CLATTER);
            EFFECT_FOR_EACH_TARGET(j, state->args)
              BattleEffect_TintBattler(state->args->targets[j],
                                       SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 4);
          }
        }
      }
    }
    BattleCamera_Shake(state->args->variant * 2 + 4,
                       state->args->variant * 4 + 8);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

/// Rebuilds the background palette's first ramp as a grey whose channels sway
/// with the frame, so the effect shimmers through colours.
void BattleEffect_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                     s32 bluePhase) {
  u16 palette[CANVAS_LEVEL_BRIGHTEST + 1];
  u16 *output;
  s32 phase;
  s32 redOffset;
  s32 greenOffset;
  s32 blueOffset;
  s32 index;

  phase = frame * (TRIG_TURN / 64);
  // Each channel's sway, in the half levels the ramp's index counts.
  redOffset = FIXEDtoINT_TWICE(Trig_Sin(phase + redPhase) * PHASE_SWAY_LEVELS);
  greenOffset =
      FIXEDtoINT_TWICE(Trig_Sin(phase + greenPhase) * PHASE_SWAY_LEVELS);
  blueOffset =
      FIXEDtoINT_TWICE(Trig_Sin(phase + bluePhase) * PHASE_SWAY_LEVELS);

  palette[0] = 0;
  index = 1;
  output = &palette[1];
  for (; index != CANVAS_LEVEL_BRIGHTEST + 1; index++) {
    s32 red;
    s32 green;
    s32 blue;

    red = (index + redOffset) / 2;
    green = (index + greenOffset) / 2;
    blue = (index + blueOffset) / 2;

    CLAMP(red, 0, RGB_LEVEL_MAX);
    CLAMP(green, 0, RGB_LEVEL_MAX);
    CLAMP(blue, 0, RGB_LEVEL_MAX);

    *output++ = RGB(red, green, blue);
  }

  // The entries, black first, are copied to colours 1 to 64, so level n shows
  // entry n - 1, half a level darker.
  // XXX (no evidence): whether the shift by one was meant; on screen it is
  // within one shade, so neither the ROM nor a render tells the two apart.
  Iwram_CopyWords(&BG_PLTT[1], palette, sizeof(palette));
}

#undef PRISM_SHARDS_PER_CRYSTAL
#undef PRISM_GROUND_Y
#undef PRISM_SHARD_PICTURES
#undef PRISM_SHATTERED
#undef PRISM_SHARD_START_MIDDLE
#undef PRISM_SHARD_KINDS
#undef PRISM_SHARD_SPIN_PICTURES
#undef PRISM_SHARD_SPIN_STEP
#undef PRISM_TURN_FRAMES
#undef PRISM_TURN_SPEED
#undef PRISM_TURN_SLOWING_FRAMES
#undef PRISM_TURN_SLOWDOWN
#undef PHASE_SWAY_LEVELS
