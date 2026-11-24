/// @file
/// Planet Diver, Haunt and the orbiting spirals.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// One chip of Planet Diver's impact. Chips are kept in particle-sized slots,
/// but with their own layout: the x velocity where a particle keeps z.
struct ImpactChip {
  FIXED x;             ///< canvas position, 16.16 fixed point
  FIXED y;             ///< canvas position, 16.16; the ground is y 112
  FIXED velocityX;     ///< 16.16 pixels a frame, damped to 56/64 each frame
  s32 unusedVelocityX; ///< a Particle's velocityX; never read
  FIXED velocityY;     ///< 16.16 pixels a frame, with 1/8 pixel of gravity
  s32 unusedVelocityZ; ///< a Particle's velocityZ; never read
  s32 life;            ///< frames left; 0 is free
};

/// Ram_MapGridCellBuffer seen as Planet Diver's chips.
#define PLANET_DIVER_IMPACT_CHIPS ((struct ImpactChip *)Ram_MapGridCellBuffer)

#define PLANET_DIVER_FRAMES 88
#define PLANET_DIVER_CLEARED_CHIPS 1024 ///< map grid buffer slots cleared first
#define PLANET_DIVER_CHIPS 256          ///< chips the fireball bursts into
#define PLANET_DIVER_FLASH_BURSTS 16    ///< the ring of flashes round the user

/// The user flashes and leaps into the air, turns into a ball of fire, falls
/// onto the first target and bursts into chips that bounce on the ground.
///
/// Abilities: Planet Diver.
void BattleEffect_RunPlanetDiver(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  struct BattleEffectState *state;
  void *canvas;
  u8 *motes;
  struct Actor *target;
  FIXED stepX;
  struct Actor *user;
  u8 *resource;
  s32 frame;
  s32 i;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      motes);
  resource = Resource_GetTableEntry(RESOURCE_FLASH_BURST_SHEET);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  resource += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(resource, state->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  for (i = 0; i != PLANET_DIVER_CLEARED_CHIPS; i++)
    PLANET_DIVER_IMPACT_CHIPS[i].life = 0;
  user = GetBattleObjectSlotFar(state->args->user)->actor;
  target = GetBattleObjectSlotFar(state->args->targets[0])->actor;
  if (user->x.fixed > 0)
    stepX = -toFIXED(15);
  else
    stepX = toFIXED(15);

  for (frame = 0; frame != PLANET_DIVER_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame > 17 || frame == 0) {
      Projection_Unit(state->args->user, &screenPosition);
      screenPosition.x /= 2;
    }

    // The flash spreads from the user.
    if (frame >= 2 && frame <= 3)
      draw[0](canvas, state->sheet, screenPosition.x - FLASH_BURST_WIDTH / 2,
              screenPosition.y - FLASH_BURST_HEIGHT, FLASH_BURST_WIDTH,
              FLASH_BURST_HEIGHT);
    if (frame >= 4 && frame <= 15) {
      for (i = 0; i != PLANET_DIVER_FLASH_BURSTS; i++) {
        s32 angle = i * (TRIG_TURN / PLANET_DIVER_FLASH_BURSTS);
        s32 x = screenPosition.x + FIXEDtoINT(frame * Trig_Sin(angle));
        s32 y = screenPosition.y + FIXEDtoINT(frame * Trig_Cos(angle)) - frame;

        draw[0](canvas,
                state->sheet + (frame - 4) / 2 * PICTURE_BYTES(FLASH_BURST),
                x - FLASH_BURST_WIDTH / 2, y - FLASH_BURST_HEIGHT,
                FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
      }
    }
    // The user leaps, heading out past its own position.
    if (frame == 4) {
      user->velocityY = toFIXED(20);
      user->acceleration = toFIXED(1);
      user->speedLimit = toFIXED(3);
      user->gravity = BATTLE_UNIT_GRAVITY;
      user->steeringFlags = 0;
      user->snapToTarget = 0;
      Actor_SetMoveTargetFar(user, user->x.fixed * 3, 0, user->z.fixed);
      Actor_SelectAnimationFar(user, POSE_LEAP);
      state->shakeFrames = 4;
      Sound_PlayFar(SE_WHOOSH);
    }
    if (frame == 16) {
      resource = Resource_GetTableEntry(RESOURCE_FLAMEBALL_SHEET);
      Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
      resource += SHEET_PALETTE_BYTES;
      Decompress_TaggedLz(resource, state->sheet);
      user->gravity = 0;
      user->velocityX = 0;
      user->velocityY = 0;
      user->z.fixed = target->z.fixed;
      Actor_StartIdleFar(user);
    }
    if (frame > 17) {
      if (user->y.fixed > 0) {
        user->x.fixed += stepX;
        user->y.fixed -= toFIXED(8);
        // The nudge after drawing lasts only to this frame's landing test,
        // where the chips start; the next frame projects the user afresh.
        // On EFFECT_SIDE_PARTY the nudge moves y, so the chips start 8 pixels
        // lower rather than 8 pixels across.
        // XXX (no evidence): whether the party side's nudge was meant to move
        // x, as the enemies' side does; The Lost Age's Planet Diver was
        // rewritten without it, and nothing else shows the intent.
        if (state->args->side == EFFECT_SIDE_ENEMIES) {
          draw[0](canvas, state->sheet, screenPosition.x - FLAMEBALL_WIDTH / 2,
                  screenPosition.y - 52, FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
          screenPosition.x -= 8;
        } else {
          draw[1](canvas, state->sheet, screenPosition.x - 26,
                  screenPosition.y - 52, FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
          screenPosition.y += 8;
        }
      }
      if (user->y.fixed < 0) {
        user->y.fixed = 0;
        // Chips fly off the landing.
        for (i = 0; i != PLANET_DIVER_CHIPS; i++) {
          struct ImpactChip *chip = &PLANET_DIVER_IMPACT_CHIPS[i];
          s32 speed;
          s32 angle;

          speed = (Random16() & 0x3ff) + 32;
          angle = Random16() & TRIG_ANGLE_MASK;
          chip->x = INTtoFIXED(screenPosition.x);
          chip->y = INTtoFIXED(screenPosition.y - 24);
          chip->velocityX = Trig_Sin(angle) * speed >> 6;
          chip->velocityY = -(Trig_Cos(angle) * speed << 1) >> 6;
          chip->life = (Random16() & 7) + 32;
        }
        state->shakeFrames = 8;
        EFFECT_SHOW_DAMAGE(SE_CRASH);
        BattleMotion_ApplyKnockbackFar(state->args->targets[0], KNOCKBACK_HURL);
        BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
      }
    }

    // Chips slow, fall and bounce.
    for (i = 0; i != PLANET_DIVER_CHIPS; i++) {
      struct ImpactChip *chip = &PLANET_DIVER_IMPACT_CHIPS[i];
      s32 y;

      if (chip->life > 0) {
        chip->x += chip->velocityX;
        y = chip->y + chip->velocityY;
        chip->life--;
        chip->y = y;
        chip->velocityX = chip->velocityX * 56 / DAMPING_SCALE;
        chip->velocityY = chip->velocityY * 56 / DAMPING_SCALE + toFIXED(1) / 8;
        if (chip->y > toFIXED(112)) {
          chip->velocityY = -chip->velocityY / 2;
        } else if ((chip->x >= 0 && chip->x < toFIXED(127)) && chip->y >= 0) {
          s32 size = chip->life / 8 + 1;

          EFFECT_DRAW_MOTE(draw[i & 1], canvas, motes, FIXEDtoINT(chip->x),
                           FIXEDtoINT(chip->y), size);
        }
      }
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

/// The particles Haunt scatters at random about its centre as it starts, up to
/// 64 pixels out along each axis.
#define HAUNT_SCATTERED 512
#define HAUNT_SKULLS 8 ///< skulls drawn around each target
#define HAUNT_SLOTS 64 ///< particles reserved for each target

/// The view ripples as skulls close in on each target in turn.
///
/// Abilities: Haunt.
void BattleEffect_RunHaunt(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  s32 point[3];
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  u8 *resource;
  s32 i;
  s32 k;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  resource = Resource_GetTableEntry(RESOURCE_SKULL_SHEET);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  resource += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(resource, state->sheet);
  resource = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

  for (i = 0; i != HAUNT_SCATTERED; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != state->args->targetCount * 32 + 96; frame++) {
    s32 *row;
    struct BattleCamera *camera = gBattleCamera;

    if (frame == 96)
      EFFECT_SHOW_DAMAGE(0);

    row = state->bg2xLines;
    if (state->args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = CANVAS_WAVE_BG2X(toFIXED(6) -
                                  Trig_Sin((frame + i) * (TRIG_TURN / 32)) * 6);
    } else {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = CANVAS_WAVE_BG2X(Trig_Sin((frame + i) * (TRIG_TURN / 32)) * 6);
    }

    EFFECT_FOR_EACH_TARGET(k, state->args) {
      struct Actor *target =
          GetBattleObjectSlotFar(state->args->targets[k])->actor;

      // Spin the skulls about a point above the target.
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, target->x.fixed, toFIXED(20), target->z.fixed);
      Matrix_TranslateVector(point);
      if (frame > k * 32) {
        Matrix_RotateX(frame << 9);
        if (frame == k * 32 + 32)
          BattleEffect_TintBattler(state->args->targets[k], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, k, 32);

        // Each skull steps nearer the centre.
        for (i = 0; i != HAUNT_SKULLS; i++) {
          struct Particle *skull = &PARTICLES[k * HAUNT_SLOTS + i];

          if (frame > (k * 8 + i) * 4) {
            s32 x = (skull->x >> 8) * (skull->x >> 8);
            s32 y = (skull->y >> 8) * (skull->y >> 8);
            s32 z = (skull->z >> 8) * (skull->z >> 8);
            s32 dist = Iwram_Sqrt(x + y + z) >> 8;

            if (dist != 0) {
              Projection_Point(skull, &screenPosition);
              screenPosition.x >>= 1;
              draw[0](canvas, state->sheet + i % 3 * PICTURE_BYTES(SKULL),
                      screenPosition.x - SKULL_WIDTH / 2,
                      screenPosition.y - SKULL_HEIGHT / 2, SKULL_WIDTH,
                      SKULL_HEIGHT);
              skull->x -= skull->x / dist;
              skull->y -= skull->y / dist;
              skull->z -= skull->z / dist;
              skull->variant++;
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

/// The spirals: ORBITING_SPIRALS a quarter turn apart, circling
/// ORBITING_SPIRALS_RADIUS pixels out, each target's ORBITING_SPIRALS_INTERVAL
/// frames after the one before and for ORBITING_SPIRALS_LIFE frames less one.
#define ORBITING_SPIRALS 4
#define ORBITING_SPIRALS_RADIUS 16
#define ORBITING_SPIRALS_INTERVAL 16
#define ORBITING_SPIRALS_LIFE 60
/// The canvas's scroll on the party side until the first frame's wave rows
/// replace it, 8 pixels short of NARROW_CANVAS_PARTY_X.
#define ORBITING_SPIRALS_PARTY_START_X (NARROW_CANVAS_PARTY_X - 8)

/// Unused: no row of gAbilityBattleEffects in either game names its kind, and
/// the handler table is reached with no other kind (BattleEffect_RunFromTable),
/// so the player never sees it. Staged by hand (Quake's row of
/// gAbilityBattleEffects given its kind, cast by Isaac at Garet, Ivan and Mia),
/// four spirals circle the point 40 pixels above Garet, then Ivan, then Mia,
/// turning a 128th of a turn a frame; each target takes its damage pose 32
/// frames into its turn.
void BattleEffect_RunUnusedOrbitingSpirals(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  void *resource;
  BlitterPair draw;
  s32 point[3];
  struct ScreenPoint screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  EFFECT_START_NARROW_CANVAS(0);
  resource = Resource_GetTableEntry(RESOURCE_SPIRAL_SHEET);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  Decompress_TaggedLz((u8 *)resource + SHEET_PALETTE_BYTES, state->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  if (state->args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(ORBITING_SPIRALS_PARTY_START_X << 8);
    offsetX = -NARROW_CANVAS_PARTY_X;
  } else {
    offsetX = 0;
  }
  for (frame = 0;
       frame != (state->args->targetCount * ORBITING_SPIRALS_INTERVAL) + 48;
       frame++) {
    struct BattleCamera *camera = gBattleCamera;
    s32 *row;
    s32 i;

    row = state->bg2xLines;
    if (state->args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = CANVAS_WAVE_BG2X(toFIXED(8) -
                                  Trig_Sin((frame + i) * (TRIG_TURN / 64)) * 8);
    } else {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ =
            CANVAS_WAVE_BG2X(Trig_Sin((frame + i) * (TRIG_TURN / 64)) * 8) -
            (NARROW_CANVAS_PARTY_X << 8);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_FOR_EACH_TARGET(member, state->args) {
      struct Actor *actor;

      actor = GetBattleObjectSlotFar(state->args->targets[member])->actor;
      if (frame > member * ORBITING_SPIRALS_INTERVAL &&
          frame <
              (member * ORBITING_SPIRALS_INTERVAL) + ORBITING_SPIRALS_LIFE) {
        s32 spin;

        if (frame == (member * ORBITING_SPIRALS_INTERVAL) + 32) {
          BattleEffect_TintBattler(state->args->targets[member],
                                   SPRITE_COLOR_NORMAL, POSE_DAMAGE, TINT_KEEP,
                                   0);
        }
        SET_VECTOR(point, actor->x.fixed, toFIXED(40), actor->z.fixed);
        Projection_Point(point, &screen);

        for (i = 0; i != ORBITING_SPIRALS; i++) {
          s32 x;
          s32 y;
          s32 phase;

          spin = frame * (TRIG_TURN / 128) + i * (TRIG_TURN / ORBITING_SPIRALS);
          x = (screen.x +
               FIXEDtoINT(Trig_Sin(spin) * ORBITING_SPIRALS_RADIUS)) +
              offsetX;
          y = screen.y + FIXEDtoINT(Trig_Cos(spin) * ORBITING_SPIRALS_RADIUS);
          // Each spiral turns through its four pictures, mirrored every other
          // 16 frames.
          phase = frame / 16;
          draw[phase & 1](canvas,
                          state->sheet +
                              (frame / 4 - phase * 4) * PICTURE_BYTES(SPIRAL),
                          x - SPIRAL_WIDTH / 2, y - SPIRAL_HEIGHT / 2,
                          SPIRAL_WIDTH, SPIRAL_HEIGHT);
        }
      }
    }
    EFFECT_PRESENT_CANVAS(state);
  }
  Task_Remove(BattleEffect_TransferCanvas);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

#undef PLANET_DIVER_IMPACT_CHIPS
#undef PLANET_DIVER_FRAMES
#undef PLANET_DIVER_CLEARED_CHIPS
#undef PLANET_DIVER_CHIPS
#undef HAUNT_SCATTERED
#undef HAUNT_SKULLS
#undef HAUNT_SLOTS
#undef PLANET_DIVER_FLASH_BURSTS
#undef ORBITING_SPIRALS
#undef ORBITING_SPIRALS_RADIUS
#undef ORBITING_SPIRALS_INTERVAL
#undef ORBITING_SPIRALS_LIFE
#undef ORBITING_SPIRALS_PARTY_START_X
