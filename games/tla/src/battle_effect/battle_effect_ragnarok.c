/// @file
/// Ragnarok, and the great sword that falls onto the target in Titan Blade and
/// Acheron's Grief.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The falling sword's modes (BattleEffect_RunFallingSword).
enum FallingSwordMode {
  FALLING_SWORD_TITAN_BLADE,
  FALLING_SWORD_ACHERONS_GRIEF
};

/// A flash waits out its life, the ring's inner ones least, and in its last
/// FLASH_BURN_FRAMES burns down from FlashBurstSheet's fireball to embers, a
/// picture every three frames.
#define FLASH_LIFE 25
#define FLASH_BURN_FRAMES 18
/// The canvas line the sparks bounce on, and where the falling sword, its
/// pillar of fire and its dust stop.
#define SWORD_GROUND_Y 108

/// FlameBladeSheet's blade, then FlashBurstSheet's pictures.
#define RAGNAROK_BLADE_WIDTH 64
#define RAGNAROK_BLADE_HEIGHT 128
#define RAGNAROK_FLASH_PICTURES PICTURE_BYTES(RAGNAROK_BLADE)
/// The blade comes down to RAGNAROK_BLADE_HOLD_TOP, and is not drawn below
/// RAGNAROK_BLADE_FLOOR.
#define RAGNAROK_BLADE_HOLD_TOP 16
#define RAGNAROK_BLADE_FLOOR 104

#define RAGNAROK_COLUMNS 2  ///< columns that strike
#define RAGNAROK_SEEDED 3   ///< columns given particles; the third is unused
#define RAGNAROK_FLASHES 16 ///< flashes kept per column
#define RAGNAROK_RING 12    ///< of which drawn as the ring
#define RAGNAROK_SPARKS 340 ///< sparks kept per column
#define RAGNAROK_DRAWN_SPARKS 256 ///< of which flown

#define RAGNAROK_FRAMES 80
#define BLADE_SWING_FRAMES 10 ///< the blade comes down
#define BLADE_HOLD_FRAME 20   ///< then holds, then creeps on
#define BLADE_LAST_FRAME 39   ///< the last frame the blade shows
#define FIRST_STRIKE_FRAME 16 ///< each column strikes 8 frames after the last
#define RESULT_FRAME 40       ///< the damage shows

/// A huge flaming blade swings down over the targets and light bursts over
/// them twice.
///
/// Abilities: Ragnarok.
void BattleEffect_RunRagnarok(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 col;
  u8 *cells;
  struct MotionObject *user;
  DrawRectangle draw[2];
  struct ScreenPoint target;
  s32 baseX;
  s32 frame;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  cells = Ram_HeapSlots->battleSheet;

  // Open the canvas, pose the user and place the columns.
  user = BattleObject_GetFar(args->user)->object;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_BuildCanvasBlitters(args->side, draw);
  Actor_SelectAnimationFar(user, POSE_LEAP);
  Actor_SetAnimationSpeedFar(user, ANIM_STEP_NORMAL * 3);
  EffectPosition_ProjectBattler(args->targets[0], &target);
  if (args->side == EFFECT_SIDE_ENEMIES)
    baseX = target.x / 2 - 24;
  else
    baseX = target.x / 2 - 92;

  Resource_LoadAndDecompress(RESOURCE_FLAME_BLADE_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             state->sheet + RAGNAROK_FLASH_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, cells, 0, 0);

  // Each column's flashes (a ring, 2 pixels apart, living 25 to 32 frames)
  // and sparks (from y = 88, at random speeds and angles, living 32 to 39).
  for (col = 0; col != RAGNAROK_SEEDED; col++) {
    for (i = 0; i != RAGNAROK_FLASHES; i++) {
      struct Particle *flash = &state->particles[col * RAGNAROK_FLASHES + i];
      s32 radius = i * 2;
      s32 angle = Random16() & TRIG_ANGLE_MASK;

      flash->x = Trig_Sin(angle) * radius;
      flash->y = -(Trig_Cos(angle) * radius);
      flash->variant = i / 2 + FLASH_LIFE;
    }

    for (i = 0; i != RAGNAROK_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[col * RAGNAROK_SPARKS + i];
      s32 speed;
      s32 angle;

      speed = (Random16() & 0x1ff) + 32;
      angle = Random16() & TRIG_ANGLE_MASK;
      spark->x =
          INTtoFIXED(gRagnarok_StrikeColumns[col + args->side * 3] + baseX);
      spark->y = toFIXED(88);
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }

  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != RAGNAROK_FRAMES; frame++) {
    if (frame == 4)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 8)
      state->shakeFrames = frame;
    if (frame == 18)
      Sound_PlayFar(SE_CRASH);
    if (frame == RESULT_FRAME)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // The blade: down 16 pixels a frame from above the canvas, held, then
    // creeping on.
    if (frame <= BLADE_LAST_FRAME) {
      s32 height;
      s32 x;
      s32 top;

      height = RAGNAROK_BLADE_HEIGHT;
      if (args->side == EFFECT_SIDE_PARTY) {
        if (frame < BLADE_SWING_FRAMES) {
          x = frame * 10 - 8;
          top = frame * 16 - RAGNAROK_BLADE_HEIGHT;
        } else if (frame > BLADE_HOLD_FRAME) {
          x = frame + 62;
          top = frame * 2 - 24;
        } else {
          x = 82;
          top = RAGNAROK_BLADE_HOLD_TOP;
        }
      } else {
        if (frame < BLADE_SWING_FRAMES) {
          x = 128 - frame * 10;
          top = frame * 16 - RAGNAROK_BLADE_HEIGHT;
        } else if (frame > BLADE_HOLD_FRAME) {
          x = 58 - frame;
          top = frame * 2 - 24;
        } else {
          x = 38;
          top = RAGNAROK_BLADE_HOLD_TOP;
        }
      }
      if (top + RAGNAROK_BLADE_HEIGHT > RAGNAROK_BLADE_FLOOR)
        height = height - top - (RAGNAROK_BLADE_HEIGHT - RAGNAROK_BLADE_FLOOR);
      if (height > 0)
        draw[0](canvas, state->sheet, x + baseX - RAGNAROK_BLADE_WIDTH / 2, top,
                RAGNAROK_BLADE_WIDTH, height);
    }

    if (frame > FIRST_STRIKE_FRAME)
      BattleEffect_StepPaletteToResource(RESOURCE_BLAST_SHEET);

    for (col = 0; col != RAGNAROK_COLUMNS; col++) {
      s32 start = col * 8 + FIRST_STRIKE_FRAME;

      // The strike: a shake, two frames of the big flash, then the ring.
      if (frame == start)
        state->shakeFrames = 12;
      if (frame >= start) {
        if (frame < start + 2)
          draw[0](canvas, state->sheet + RAGNAROK_FLASH_PICTURES,
                  gRagnarok_StrikeColumns[col + args->side * 3] + baseX -
                      FLASH_BURST_WIDTH / 2,
                  56, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
        for (i = 0; i != RAGNAROK_RING; i++) {
          struct Particle *flash =
              &state->particles[col * RAGNAROK_FLASHES + i];
          s32 x;
          s32 life;
          s32 y;

          y = FIXED_INTEGER_PART(flash->y);
          x = FIXED_INTEGER_PART(flash->x) +
              gRagnarok_StrikeColumns[col + args->side * 3] + baseX;
          life = flash->variant;
          if (life >= 0 && life < FLASH_BURN_FRAMES)
            draw[0](canvas,
                    state->sheet +
                        gRagnarok_FlashPictures[life / 3] *
                            PICTURE_BYTES(FLASH_BURST) +
                        RAGNAROK_FLASH_PICTURES,
                    x - FLASH_BURST_WIDTH / 2, y + 56, FLASH_BURST_WIDTH,
                    FLASH_BURST_HEIGHT);
          life = flash->variant;
          flash->variant = life > 0 ? life - 1 : -1;
        }
      }

      // The sparks, from six frames after the strike, bouncing on the ground.
      if (frame > start + 5) {
        for (i = 0; i != RAGNAROK_DRAWN_SPARKS; i++) {
          struct Particle *spark = &PARTICLES[col * RAGNAROK_SPARKS + i];

          if (spark->variant > 0) {
            s32 x;
            s32 y;
            s32 size;

            Particle_AdvanceWithGravity2D(spark, PARTICLE_DAMPING_NONE,
                                          toFIXED(1) / 16);
            spark->variant--;
            y = spark->y;
            if (y > toFIXED(SWORD_GROUND_Y)) {
              spark->velocityY = -spark->velocityY / 2;
            } else {
              x = spark->x;
              if ((x >= 0 && x <= toFIXED(127) - 1) && y >= 0) {
                u8 *picture;
                s32 lane;
                s32 py = FIXEDtoINT(y);
                s32 px = FIXEDtoINT(x);

                size = spark->variant / 5 + 1;
                lane = i & 1;
                picture = cells + gBattleEffect_MotePictures[size - 1];
                px -= size / 2;
                py -= size;
                draw[lane](canvas, picture, px, py, size, size * 2);
              }
            }
          }
        }
      }

      // The column's targets flash at the strike and are struck by its
      // sparks.
      for (i = 0; i != args->targetCount; i++) {
        if (frame == start)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 10);
        if (frame == start + 6) {
          EFFECT_STRIKE_TARGET(args, i, 10, KNOCKBACK_HURL);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  Actor_SetAnimationSpeedFar(user, ANIM_STEP_NORMAL);
  EFFECT_FINISH_CANVAS();
}

#undef RAGNAROK_BLADE_WIDTH
#undef RAGNAROK_BLADE_HEIGHT
#undef RAGNAROK_FLASH_PICTURES
#undef RAGNAROK_BLADE_HOLD_TOP
#undef RAGNAROK_BLADE_FLOOR
#undef RAGNAROK_COLUMNS
#undef RAGNAROK_SEEDED
#undef RAGNAROK_FLASHES
#undef RAGNAROK_RING
#undef RAGNAROK_SPARKS
#undef RAGNAROK_DRAWN_SPARKS
#undef RAGNAROK_FRAMES
#undef BLADE_SWING_FRAMES
#undef BLADE_HOLD_FRAME
#undef BLADE_LAST_FRAME
#undef FIRST_STRIKE_FRAME
#undef RESULT_FRAME

/// Abilities: Titan Blade.
void BattleEffect_RunTitanBlade(struct BattleEffectParams *args) {
  BattleEffect_RunFallingSword(args, FALLING_SWORD_TITAN_BLADE);
}

/// Abilities: Acheron's Grief.
void BattleEffect_RunAcheronsGrief(struct BattleEffectParams *args) {
  BattleEffect_RunFallingSword(args, FALLING_SWORD_ACHERONS_GRIEF);
}

/// The sword's sparks, in the map cell buffer after the dust (PARTICLES).
#define TITAN_BLADE_SWORD_SPARKS                                               \
  ((struct Particle *)(Ram_MapCellBuffer +                                     \
                       TITAN_BLADE_DUST * sizeof(struct Particle)))

#define TITAN_BLADE_FRAMES 102
#define TITAN_BLADE_SWORD_PICTURE                                              \
  20000                        ///< the sword picture in the sprite sheet
#define TITAN_BLADE_FLASHES 32 ///< flashes scattered
#define TITAN_BLADE_RING 16    ///< of which drawn as the ring
#define TITAN_BLADE_DUST 684   ///< dust slots in PARTICLES, before the sparks
#define TITAN_BLADE_FIRST_DUST 256 ///< dust lit at once on frame 28
#define TITAN_BLADE_DRAWN_DUST 342 ///< dust moved and drawn
#define TITAN_BLADE_SPARKS 340     ///< sparks after the dust
/// The flash pictures in the sprite sheet, after the 34x104 fire pillar.
#define TITAN_BLADE_FLASH_PICTURES PICTURE_BYTES(FIRE_PILLAR)
/// SwordSheet's great sword, point down.
#define TITAN_BLADE_SWORD_WIDTH 36
#define TITAN_BLADE_SWORD_HEIGHT 120
/// The sword comes down to TITAN_BLADE_SWORD_HOLD_TOP and holds there.
#define TITAN_BLADE_SWORD_HOLD_TOP 16

/// From TITAN_BLADE_PILLAR_FRAME, for TITAN_BLADE_PILLAR_FRAMES, the pillar of
/// fire rises and TITAN_BLADE_DUST_PER_FRAME more dust spouts each frame.
#define TITAN_BLADE_PILLAR_FRAME 32
#define TITAN_BLADE_PILLAR_FRAMES 32
#define TITAN_BLADE_DUST_PER_FRAME 16
/// How far behind their climb the pillar's flames start, which sets the line
/// of FirePillarSheetA's column they show first.
#define TITAN_BLADE_PILLAR_PHASE 256

/// The sword strikes: the crash, the damage and the canvas filled bright.
#define TITAN_BLADE_STRIKE_FRAME 60

/// A great sword falls from the sky onto the target, a pillar of fire and dust
/// spouting under it, and drives into the ground in a burst of flashes and
/// sparks. The sword comes down fast, holds, sinks slowly while the pillar
/// rises and then plunges; the canvas floods white as it arrives and as it
/// strikes, and every target is struck a little after. Acheron's Grief shows
/// the same sword in other colours.
void BattleEffect_RunFallingSword(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *cells;
  struct ScreenPoint target;
  s32 centerX;
  s32 step;
  FIXED origin;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  cells = Ram_HeapSlots->battleSheet;

  // Open the canvas on BG1 over the target and build the blitters.
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EffectPosition_ProjectBattler(args->targets[0], &target);
  centerX = target.x / 2;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[1] = Ram_HeapSlots->ramCodePaired;

  // No caller passes a third mode: both abilities draw the sword.
  if (mode != 2)
    Resource_LoadAndDecompress(RESOURCE_SWORD_SHEET,
                               state->sheet + TITAN_BLADE_SWORD_PICTURE, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FIRE_PILLAR_SHEET_A, state->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             state->sheet + TITAN_BLADE_FLASH_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, cells, 0, 0);

  // Acheron's Grief turns the canvas's colours: red to blue, blue to green,
  // green to red.
  if (mode == FALLING_SWORD_ACHERONS_GRIEF) {
    u16 *color = (u16 *)BG_PLTT;

    for (i = 0; i != CANVAS_LEVEL_BRIGHTEST + 1; i++) {
      s32 red = RGB_RED(*color);
      s32 green = RGB_GREEN(*color);
      s32 blue = RGB_BLUE(*color);

      *color++ = RGB(green, blue, red);
    }
  }

  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  // 32 flashes on rings, two pixels apart.
  for (i = 0; i != TITAN_BLADE_FLASHES; i++) {
    struct Particle *flash = &state->particles[i];
    s32 scale = i * 2;
    s32 angle = Random16() & TRIG_ANGLE_MASK;

    flash->x = Trig_Sin(angle) * scale;
    flash->y = -(Trig_Cos(angle) * scale);
    flash->variant = i / 2 + FLASH_LIFE;
  }

  for (i = 0; i != TITAN_BLADE_DUST; i++) {
    PARTICLES[i].variant = -1;
  }

  // The sparks wait at the target's feet.
  i = 0;
  origin = INTtoFIXED(centerX);
  for (; i != TITAN_BLADE_SPARKS; i++) {
    struct Particle *spark = &TITAN_BLADE_SWORD_SPARKS[i];
    s32 speed;
    s32 angle;

    speed = (Random16() & 0x1ff) + 32;
    angle = Random16() & TRIG_ANGLE_MASK;
    spark->x = origin;
    spark->y = toFIXED(88);
    spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
    spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
    spark->variant = (Random16() & 7) + 32;
  }

  for (frame = 0; frame != TITAN_BLADE_FRAMES; frame++) {
    if (mode == FALLING_SWORD_TITAN_BLADE) {
      if (frame >= 41 && frame <= 63)
        BattleEffect_StepPaletteToResource(RESOURCE_BLAST_SHEET);
    } else if (mode == FALLING_SWORD_ACHERONS_GRIEF) {
      if (frame == 61) {
        Iwram_CopyWordsEntry(
            (void *)BG_PLTT,
            Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
            SHEET_PALETTE_BYTES);
        Palette_MirrorBgColors();
      }
    }

    if (frame == 8)
      state->shakeFrames = frame;
    if (frame == 48)
      state->shakeFrames = 8;
    if (frame == TITAN_BLADE_STRIKE_FRAME)
      state->shakeFrames = 16;

    if (frame == 4)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == TITAN_BLADE_PILLAR_FRAME)
      Sound_PlayFar(SE_SCATTER);
    if (frame == TITAN_BLADE_STRIKE_FRAME) {
      if (mode == FALLING_SWORD_ACHERONS_GRIEF)
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
      else
        EFFECT_SHOW_DAMAGE(SE_CRASH);
    }

    // The flashes, from frame 56.
    if (frame > 55) {
      for (i = 0; i != TITAN_BLADE_RING; i++) {
        struct Particle *flash = &state->particles[i];
        s32 x;
        s32 life;
        s32 y;

        y = FIXED_INTEGER_PART(flash->y);
        x = FIXED_INTEGER_PART(flash->x) + centerX;
        life = flash->variant;
        if (life >= 0 && life < FLASH_BURN_FRAMES)
          draw[0](canvas,
                  state->sheet +
                      gTitanBlade_FlashPictures[life / 3] *
                          PICTURE_BYTES(FLASH_BURST) +
                      TITAN_BLADE_FLASH_PICTURES,
                  x - FLASH_BURST_WIDTH / 2, y + 48, FLASH_BURST_WIDTH,
                  FLASH_BURST_HEIGHT);
        life = flash->variant;
        flash->variant = life > 0 ? life - 1 : -1;
      }
    }

    // The dust: a fountain at frame 28, then more each frame while the
    // pillar rises.
    if (frame == 28) {
      for (i = 0; i != TITAN_BLADE_FIRST_DUST; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant == -1) {
          s32 speed;
          s32 angle;

          speed = Random16() & 63;
          angle = Random16() & TRIG_ANGLE_MASK;
          dust->x = ((Trig_Sin(angle) * speed) >> 3) + origin;
          dust->y = ((Trig_Cos(angle) * speed) >> 2) + toFIXED(96);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
        }
      }
    }

    step = frame - TITAN_BLADE_PILLAR_FRAME;
    if (step >= 0 && step < TITAN_BLADE_PILLAR_FRAMES) {
      s32 spawned;

      spawned = 0;
      for (i = 0; i != TITAN_BLADE_DUST; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant == -1) {
          s32 speed;
          s32 angle;

          speed = Random16() & 63;
          angle = Random16() & TRIG_ANGLE_MASK;
          dust->x = ((Trig_Sin(angle) * speed) >> 3) + origin;
          dust->y = ((Trig_Cos(angle) * speed) >> 2) + toFIXED(96);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
          spawned++;
          if (spawned == TITAN_BLADE_DUST_PER_FRAME)
            break;
        }
      }
    }

    // The pillar of fire, its flames climbing 16 pixels a frame.
    if (step >= 0 && step < TITAN_BLADE_PILLAR_FRAMES) {
      s32 rise = frame * 16;
      s32 height = FIRE_PILLAR_HEIGHT;
      u8 *src = state->sheet;
      s32 x = centerX - FIRE_PILLAR_WIDTH / 2;
      DrawRectangle blit = draw[0];
      s32 scroll = (rise - TITAN_BLADE_PILLAR_PHASE) % height;

      blit(canvas, src, x, SWORD_GROUND_Y - FIRE_PILLAR_HEIGHT - scroll,
           FIRE_PILLAR_WIDTH, height);
      blit(canvas, src, x, SWORD_GROUND_Y - scroll, FIRE_PILLAR_WIDTH, scroll);
    }

    // The dust grows as it falls and toward the end. It stops at frame 71, so
    // only the first two of the later sizes, and never the lift from frame
    // 80, are reached.
    if (frame <= 71) {
      for (i = 0; i != TITAN_BLADE_DRAWN_DUST; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant >= 0) {
          s32 size;

          size = i % 3 + 2;
          if (dust->velocityY > 0)
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
          draw[0](canvas, cells + gBattleEffect_MotePictures[size - 1],
                  FIXED_INTEGER_PART(dust->x) - size / 2,
                  FIXED_INTEGER_PART(dust->y) - size, size, size * 2);

          dust->x += dust->velocityX;
          dust->y += dust->velocityY;
          if (frame > 80)
            dust->velocityY -= toFIXED(1) / 2;
          else
            dust->velocityY += gTitanBlade_DustGravity[i & 3];
          dust->velocityX = dust->velocityX * 62 / DAMPING_SCALE;
          dust->velocityY = dust->velocityY * 62 / DAMPING_SCALE;
          dust->variant++;
          if (dust->velocityY > 0 &&
              FIXED_INTEGER_PART(dust->y) > SWORD_GROUND_Y)
            dust->variant = -1;
        }
      }
    }

    // The sword: down 16 pixels a frame, held, sinking while the pillar
    // rises, plunging after the strike; it is not drawn below the ground.
    if (frame <= 95) {
      s32 x;
      s32 high;
      s32 top;

      x = centerX - TITAN_BLADE_SWORD_WIDTH / 2;
      high = TITAN_BLADE_SWORD_HEIGHT;
      if (frame > TITAN_BLADE_STRIKE_FRAME)
        top = frame * 8 - 450;
      else if (frame > TITAN_BLADE_PILLAR_FRAME)
        top = step / 2 + TITAN_BLADE_SWORD_HOLD_TOP;
      else if (frame <= 9)
        top = frame * 16 - 128;
      else
        top = TITAN_BLADE_SWORD_HOLD_TOP;
      if (top + TITAN_BLADE_SWORD_HEIGHT > SWORD_GROUND_Y)
        high = high - top - (TITAN_BLADE_SWORD_HEIGHT - SWORD_GROUND_Y);
      if (high > 0)
        draw[1](canvas, state->sheet + TITAN_BLADE_SWORD_PICTURE, x, top,
                TITAN_BLADE_SWORD_WIDTH, high);
    }

    // The sparks bounce on the ground.
    if (frame > 59) {
      for (i = 0; i != TITAN_BLADE_SPARKS; i++) {
        struct Particle *spark = &TITAN_BLADE_SWORD_SPARKS[i];

        if (spark->variant > 0) {
          s32 x;
          s32 y;
          s32 size;

          Particle_AdvanceWithGravity2D(spark, PARTICLE_DAMPING_NONE,
                                        toFIXED(1) / 8);
          y = spark->y;
          spark->variant--;
          if (y > toFIXED(SWORD_GROUND_Y)) {
            spark->velocityY = -spark->velocityY / 2;
          } else {
            x = spark->x;
            if ((x >= 0 && x <= toFIXED(127) - 1) && y >= 0) {
              u8 *cell;

              size = spark->variant / 5 + 1;
              cell = cells + gBattleEffect_MotePictures[size - 1];
              x = FIXEDtoINT(x);
              y = FIXEDtoINT(y);
              x -= size / 2;
              y -= size;
              draw[0](canvas, cell, x, y, size, size * 2);
            }
          }
        }
      }
    }

    // The hit, and the canvas flooded as the sword arrives and as it
    // strikes.
    if (frame == 68) {
      for (i = 0; i != args->targetCount; i++) {
        EFFECT_STRIKE_TARGET(args, i, 16, KNOCKBACK_JOLT);
      }
    }

    if (frame == 9) {
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame == TITAN_BLADE_STRIKE_FRAME)
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

#undef TITAN_BLADE_SWORD_SPARKS
#undef TITAN_BLADE_FRAMES
#undef TITAN_BLADE_SWORD_PICTURE
#undef TITAN_BLADE_FLASHES
#undef TITAN_BLADE_RING
#undef TITAN_BLADE_DUST
#undef TITAN_BLADE_FIRST_DUST
#undef TITAN_BLADE_DRAWN_DUST
#undef TITAN_BLADE_SPARKS
#undef TITAN_BLADE_FLASH_PICTURES
#undef TITAN_BLADE_SWORD_WIDTH
#undef TITAN_BLADE_SWORD_HEIGHT
#undef TITAN_BLADE_SWORD_HOLD_TOP
#undef TITAN_BLADE_PILLAR_FRAME
#undef TITAN_BLADE_PILLAR_FRAMES
#undef TITAN_BLADE_DUST_PER_FRAME
#undef TITAN_BLADE_PILLAR_PHASE
#undef TITAN_BLADE_STRIKE_FRAME
#undef FLASH_LIFE
#undef FLASH_BURN_FRAMES
#undef SWORD_GROUND_Y
