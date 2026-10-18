// Part of effect.c, the battle effect object, compiled only through it.
// Prism.

// Falling crystals.

#define PRISM_GROUND_Y 0x380000 // y 56, where the crystals shatter

// Abilities: Prism, Hail Prism, Freeze Prism (kind 8).
//
// Crystals drop out of the sky onto the targets' side, 8 frames apart, and
// shatter on the ground into 16 spinning shards each, flashing the targets and
// shaking the screen; the battle shows the damage 80 frames before the end, and
// the canvas darkens away over the last 8. effect->variant picks the volley
// from gPrism_Counts (crystals and frames: 1 in 110, 4 in 134, 12 in 166); with
// variant 2 the camera also swings round for the first 104 frames. The crystals
// are particles[]: z is 1 once shattered, variant is the frame it starts to
// fall. The shards are 16 per crystal in PARTICLES: z is how fast it
// spins.
void BattleFx_RunPrism(struct BattleEffectArgument *effect) {
  BlitterPair draw;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct EffectStep *seed;
  struct EffectStep *crystal;
  struct EffectStep *shards;
  struct EffectStep *shard;
  s32 x;
  s32 cell;
  s32 i;
  s32 j;

  shards = PARTICLES;
  cursor = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  work = *cursor++;
  canvas = *cursor;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                        50); // trails keep half a frame

  // Line the crystals up above the screen, each starting 8 frames after
  // the last and drifting towards the middle as it falls.
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
    if (work->effect->variant == 2 && frame <= 103) {
      // Swing the camera round, slowing from frame 96.
      struct BattleCamera *camera = gCameraWork;
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
      BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
    if (frame == gPrism_Counts[work->effect->variant][PRISM_FRAMES] - 8) {
      // Darken the canvas away over the last 8 frames.
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x06060606);
    }
    if (frame <= gPrism_Counts[work->effect->variant][PRISM_FRAMES] - 8) {
      for (i = 0;
           i != gPrism_Counts[work->effect->variant][PRISM_CRYSTAL_COUNT];
           i++) {
        crystal = &work->particles[i];
        if (crystal->z == 1) {
          // A shattered crystal: its shards fly and spin, slowing down.
          for (j = 0; j != 16; j++) {
            cell = (j % 5) * 3 + (shards[i * 16 + j].variant / 96) % 3;
            shard = &shards[i * 16 + j];
            draw[j <= 2](canvas,
                         work->sheet + gPrism_ShardOffsets[cell] + 0x800,
                         HI(shard->x) - gPrism_ShardWidths[cell] / 2,
                         HI(shard->y) - gPrism_ShardHeights[cell] / 2,
                         gPrism_ShardWidths[cell], gPrism_ShardHeights[cell]);
            EffectStep_AdvanceWithGravity2D(shard, 64, 0x2000);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= crystal->variant) {
          // A falling crystal (32x64).
          draw[i & 1](canvas, work, HI(crystal->x) - 16, HI(crystal->y), 32,
                      64);
          EffectStep_AdvanceWithGravity2D(crystal, 64, 0x10000);
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
              shard->velocityX = ((Random16() & 127) - 64) << 11;
              shard->velocityY = -(Random16() & 127) << 11;
              if (i & 1) {
                shard->velocityX *= 2;
                shard->velocityY *= 2;
              }
              shard->z = 32;
              shard->variant = 0;
            }
            work->shakeFrames = 8;
            AudioCommand_PlayFar(SE_144);
            EFFECT_FOR_EACH_TARGET(j, work->effect)
              ObjectGroup_UpdateMembers(work->effect->actors[j],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 4);
          }
        }
      }
    }
    Camera_ApplyShake(work->effect->variant * 2 + 4,
                      work->effect->variant * 4 + 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Rebuilds background colours 1 to 63 as a grey ramp whose red, green and
// blue each sway with the frame, out of step by their phases (1/65536ths
// of a turn), so the effect shimmers through colours.
// frame      : the effect's frame; the sway turns 1/64 of a circle a frame
// redPhase, greenPhase, bluePhase : each channel's offset in the sway
void Graphics_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
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
