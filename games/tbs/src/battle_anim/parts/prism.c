/// @file
/// Prism.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

#define PRISM_GROUND_Y 0x380000     ///< y 56, where the crystals shatter
#define PRISM_SHARD_CELLS (32 * 64) ///< the shards, after the 32x64 crystal

/// Crystals drop out of the sky onto the targets' side, 8 frames apart, and
/// shatter on the ground into 16 spinning shards each, flashing the targets and
/// shaking the screen; the battle shows the damage 80 frames before the end,
/// and the canvas darkens away over the last 8.
///
/// Abilities: Prism, Hail Prism, Freeze Prism (BATTLE_ANIM_PRISM).
///
/// effect->variant picks the volley from gPrism_Counts (crystals and frames: 1
/// in 110, 4 in 134, 12 in 166); for Freeze Prism the camera also swings round
/// for the first 104 frames. The crystals are particles[]: z is 1 once
/// shattered, variant is the frame it starts to fall. The shards are 16 per
/// crystal in PARTICLES: z is how fast it spins.
///
/// Positions are canvas pixels in 16.16. Loads the crystal palette into the
/// first 64 background colours and sets REG_BLDALPHA, the camera's yaw
/// (Freeze Prism) and work->shakeFrames, and flashes the targets at each
/// shatter.
void BattleAnim_RunPrism(struct BattleAnimArgs *effect) {
  BlitterPair draw;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  struct Particle *seed;
  struct Particle *crystal;
  struct Particle *shards;
  struct Particle *shard;
  s32 x;
  s32 cell;
  s32 i;
  s32 j;

  shards = PARTICLES;
  cursor = &gHeapSlots[HEAP_SLOT_BATTLE_ANIM];
  work = *cursor++;
  canvas = *cursor;
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
  BattleAnim_FetchRectangleBlitters(work->effect->side, draw);
  // Trails keep half of each pixel a frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  // Line the crystals up above the screen, each starting 8 frames after
  // the last and drifting towards the middle as it falls: each starts 18
  // frames' drift short of its random x. They start at y -64, a crystal's
  // height above the canvas, drifting up to 4 pixels a frame across.
  for (j = 0; j != gPrism_Counts[work->effect->variant][PRISM_CRYSTAL_COUNT];
       j++) {
    seed = &work->particles[j];
    // This first random number is drawn and discarded.
    x = Random16() & 31;
    seed->y = -0x400000;
    if (work->effect->side == 1) {
      x = ((Random16() & 31) + 80) << 16;
      seed->velocityX = (Random16() & 63) << 12;
    } else {
      x = ((Random16() & 31) + 8) << 16;
      seed->velocityX = -(Random16() & 63) << 12;
    }
    seed->x = x - seed->velocityX * 18;
    seed->velocityY = 0;
    seed->z = 0;
    seed->variant = j * 8;
  }

  for (frame = 0; frame != gPrism_Counts[work->effect->variant][PRISM_FRAMES];
       frame++) {
    if (work->effect->variant == ANIM_VARIANT_STRONGEST && frame <= 103) {
      // Swing the camera round, slowing from frame 96: 192/65536 turn a
      // frame, 19296/65536 in all, under a third of a turn.
      struct BattleCamera *camera = gBattleCamera;
      s32 speed;

      if (frame <= 95)
        speed = 192;
      else
        speed = 2496 - frame * 24;
      if (work->effect->side == 0)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == gPrism_Counts[work->effect->variant][PRISM_FRAMES] - 80)
      ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == gPrism_Counts[work->effect->variant][PRISM_FRAMES] - 8) {
      // Darken the canvas away over the last 8 frames, 6 levels a frame.
      ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x06060606);
    }
    if (frame <= gPrism_Counts[work->effect->variant][PRISM_FRAMES] - 8) {
      for (i = 0;
           i != gPrism_Counts[work->effect->variant][PRISM_CRYSTAL_COUNT];
           i++) {
        crystal = &work->particles[i];
        if (crystal->z == 1) {
          // A shattered crystal: its shards fly and spin, slowing down. A
          // shard draws one of three pictures of its kind (j % 5), stepping
          // every 96 of its spin (variant); shards 0 to 2 draw additively.
          // Kind 4 (shards 4, 9 and 14) asks for cells 12 to 14, past the
          // twelve pictures of the gPrism_Shard tables, and reads what follows
          // each table: its heights are the zero bytes that follow
          // gPrism_ShardHeights, and a blitter given no rows draws nothing,
          // so those three shards of every crystal never show.
          // XXX (no evidence): whether a fifth kind of shard was meant; the
          // twelve pictures and their tables are all the ROM holds.
          for (j = 0; j != 16; j++) {
            cell = (j % 5) * 3 + (shards[i * 16 + j].variant / 96) % 3;
            shard = &shards[i * 16 + j];
            draw[j <= 2](
                canvas,
                work->sheet + gPrism_ShardOffsets[cell] + PRISM_SHARD_CELLS,
                Q_16_16_TO_INT(shard->x) - gPrism_ShardWidths[cell] / 2,
                Q_16_16_TO_INT(shard->y) - gPrism_ShardHeights[cell] / 2,
                gPrism_ShardWidths[cell], gPrism_ShardHeights[cell]);
            // Shards gain 1/8 pixel a frame of downward speed each frame.
            // Their spin starts at 32 and slows by 1 every other frame, down
            // to 1.
            Particle_AdvanceWithGravity2D(shard, 64, 0x2000);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= crystal->variant) {
          // A falling crystal (32x64), odd ones additive, gaining a pixel a
          // frame of downward speed each frame.
          draw[i & 1](canvas, work->sheet, Q_16_16_TO_INT(crystal->x) - 16,
                      Q_16_16_TO_INT(crystal->y), 32, 64);
          Particle_AdvanceWithGravity2D(crystal, 64, 0x10000);
          if (crystal->y > PRISM_GROUND_Y) {
            // Shatter it on reaching the ground (y 56).
            crystal->z = 1;
            crystal->y = PRISM_GROUND_Y;
            for (j = 0; j != 16; j++) {
              shard = &shards[i * 16 + j];
              shard->x =
                  ((gPrism_ShardStarts[j][PRISM_SHARD_START_X] - 40) << 16) +
                  crystal->x;
              shard->y = gPrism_ShardStarts[j][PRISM_SHARD_START_Y] << 16;
              // Up to 2 pixels a frame either way across, and up to 4 up.
              shard->velocityX = ((Random16() & 127) - 64) << 11;
              shard->velocityY = -(Random16() & 127) << 11;
              // Odd crystals' shards fly twice as fast.
              if (i & 1) {
                shard->velocityX *= 2;
                shard->velocityY *= 2;
              }
              shard->z = 32;
              shard->variant = 0;
            }
            work->shakeFrames = 8;
            Sound_PlaySongFar(SE_CLATTER);
            ANIM_FOR_EACH_TARGET(j, work->effect)
            BattleAnim_TintBattler(work->effect->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 4);
          }
        }
      }
    }
    // Shake ranges 4 by 8, 6 by 12 and 8 by 16 pixels by variant. Hail
    // Prism's are not powers of two, so BattleCamera_Shake's masks (range - 1)
    // leave gaps in its offsets.
    BattleCamera_Shake(work->effect->variant * 2 + 4,
                       work->effect->variant * 4 + 8);
    ANIM_END_FRAME(work);
  }

  ANIM_FINISH_CANVAS();
}

// The 64 entries, black first, are copied to colours 1 to 64, so level n shows
// entry n - 1: half a level darker, at most one shade of 32 in each channel.
// Colour 64 is overwritten too, but it is colour 0 of background palette 4,
// which a 16-colour layer never draws; the canvas's levels stop at 63 and the
// battlefield's colours start at 96 (BATTLE_BG_PALETTE).
// XXX (no evidence): whether the shift by one was meant; on screen it is
// within one shade, so neither the ROM nor a render tells the two apart.
/// Rebuilds the background palette's first ramp as a grey ramp whose red,
/// green and blue each sway with the frame, out of step by their phases
/// (1/65536ths of a turn), so the animation shimmers through colours.
///
/// Called each frame by BattleAnim_RunBind and
/// BattleAnim_RunUnusedTriangleStrike (bind.c), BattleAnim_RunShiningStar
/// (stars.c) and BattleAnim_RunThor (thor.c), which all pass phases a third
/// of a turn apart (0xaaab, 0x5555 and 0); Thor passes twice its frame, for a
/// faster shimmer. Level n's channel is (n + sway) / 2, the sway up to 32
/// levels either way, clamped to 0 to 31. Rewrites background colours 1 to 64.
///
/// frame      : the animation's frame; the sway turns 1/64 of a circle a frame
/// redPhase, greenPhase, bluePhase : each channel's offset in the sway
void BattleAnim_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                   s32 bluePhase) {
  u16 palette[64];
  u16 *output;
  s32 phase;
  s32 redOffset;
  s32 greenOffset;
  s32 blueOffset;
  s32 index;

  phase = frame * 0x400;
  redOffset = (Trig_Sin(phase + redPhase) * 16) >> 15;
  greenOffset = (Trig_Sin(phase + greenPhase) * 16) >> 15;
  blueOffset = (Trig_Sin(phase + bluePhase) * 16) >> 15;

  palette[0] = 0;
  index = 1;
  output = &palette[1];
  do {
    s32 red;
    s32 green;
    s32 blue;

    red = (index + redOffset) / 2;
    green = (index + greenOffset) / 2;
    blue = (index + blueOffset) / 2;

    CLAMP(red, 0, 31);
    CLAMP(green, 0, 31);
    CLAMP(blue, 0, 31);

    *output++ = RGB(red, green, blue);
    index++;
  } while (index != 64);

  Iwram_CopyWords((void *)&BG_PLTT[1], palette, sizeof(palette));
}

#undef PRISM_GROUND_Y
#undef PRISM_SHARD_CELLS
