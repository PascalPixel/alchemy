/// @file
/// Planet Diver, Haunt and the orbiting spirals.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// One chip of the impact, over a particle's entry with its velocities moved.
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

/// The user flashes and leaps into the air, turns into a ball of fire, falls
/// onto the first target and bursts into chips that bounce on the ground.
///
/// Abilities: Planet Diver.
void BattleEffect_RunPlanetDiver(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  struct Actor *target;
  s32 dx;
  struct Actor *actor;
  u8 *palette;
  s32 frame;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sprites = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      sprites);
  palette = Resource_GetTableEntry(RESOURCE_FLASH_BURST_SHEET);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (i = 0; i != PLANET_DIVER_CLEARED_CHIPS; i++)
    PLANET_DIVER_IMPACT_CHIPS[i].life = 0;
  actor = GetBattleObjectSlotFar(work->args->user)->actor;
  target = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  if (actor->x.fixed > 0)
    dx = -toFIXED(15);
  else
    dx = toFIXED(15);

  for (frame = 0; frame != PLANET_DIVER_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame > 17 || frame == 0) {
      Projection_Unit(work->args->user, &screenPosition);
      screenPosition.x /= 2;
    }

    // The flash spreads from the user.
    if (frame >= 2 && frame <= 3)
      draw[0](canvas, work->sheet, screenPosition.x - FLASH_BURST_WIDTH / 2,
              screenPosition.y - FLASH_BURST_HEIGHT, FLASH_BURST_WIDTH,
              FLASH_BURST_HEIGHT);
    if (frame >= 4 && frame <= 15) {
      for (i = 0; i != 16; i++) {
        s32 x = screenPosition.x + FIXEDtoINT(frame * Trig_Sin(i << 12));
        s32 y =
            screenPosition.y + FIXEDtoINT(frame * Trig_Cos(i << 12)) - frame;

        draw[0](canvas,
                work->sheet + (frame - 4) / 2 * PICTURE_BYTES(FLASH_BURST),
                x - FLASH_BURST_WIDTH / 2, y - FLASH_BURST_HEIGHT,
                FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
      }
    }
    // The user leaps, heading out past its own position.
    if (frame == 4) {
      actor->velocityY = toFIXED(20);
      actor->acceleration = toFIXED(1);
      actor->speedLimit = toFIXED(3);
      actor->gravity = BATTLE_UNIT_GRAVITY;
      actor->steeringFlags = 0;
      actor->snapToTarget = 0;
      Actor_SetMoveTargetFar(actor, actor->x.fixed * 3, 0, actor->z.fixed);
      Actor_SelectAnimationFar(actor, POSE_LEAP);
      work->shakeFrames = 4;
      Sound_PlayFar(SE_WHOOSH);
    }
    if (frame == 16) {
      palette = Resource_GetTableEntry(RESOURCE_FLAMEBALL_SHEET);
      Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
      palette += SHEET_PALETTE_BYTES;
      Decompress_TaggedLz(palette, work->sheet);
      actor->gravity = 0;
      actor->velocityX = 0;
      actor->velocityY = 0;
      actor->z.fixed = target->z.fixed;
      Actor_StartIdleFar(actor);
    }
    if (frame > 17) {
      if (actor->y.fixed > 0) {
        actor->x.fixed += dx;
        actor->y.fixed -= toFIXED(8);
        // The nudge after drawing lasts only to this frame's landing test,
        // where the chips start; the next frame projects the user afresh.
        // On EFFECT_SIDE_PARTY the nudge moves y, so the chips start 8 pixels
        // lower rather than 8 pixels across.
        // XXX (no evidence): whether screenPosition.x += 8 was meant, as side
        // 0 moves x; only the author's intent would say.
        if (work->args->side == EFFECT_SIDE_ENEMIES) {
          draw[0](canvas, work->sheet, screenPosition.x - FLAMEBALL_WIDTH / 2,
                  screenPosition.y - 52, FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
          screenPosition.x -= 8;
        } else {
          draw[1](canvas, work->sheet, screenPosition.x - 26,
                  screenPosition.y - 52, FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
          screenPosition.y += 8;
        }
      }
      if (actor->y.fixed < 0) {
        actor->y.fixed = 0;
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
        work->shakeFrames = 8;
        EFFECT_SHOW_DAMAGE(SE_CRASH);
        BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_HURL);
        BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
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

          EFFECT_DRAW_MOTE(draw[i & 1], canvas, sprites, FIXEDtoINT(chip->x),
                           FIXEDtoINT(chip->y), size);
        }
      }
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#define HAUNT_SCATTERED 512
#define HAUNT_SKULLS 8 ///< skulls drawn around each target
#define HAUNT_SLOTS 64 ///< particles reserved for each target

/// Skulls close in on each target in turn.
///
/// Abilities: Haunt.
void BattleEffect_RunHaunt(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  u8 *palette;
  s32 i;
  s32 k;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  palette = Resource_GetTableEntry(RESOURCE_SKULL_SHEET);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, work->sheet);
  palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

  for (i = 0; i != HAUNT_SCATTERED; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != work->args->targetCount * 32 + 96; frame++) {
    s32 *row;
    struct BattleCamera *camera = gBattleCamera;

    if (frame == 96)
      EFFECT_SHOW_DAMAGE(0);

    row = work->bg2xLines;
    if (work->args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = (toFIXED(6) - Trig_Sin((frame + i) << 11) * 6) >> 10;
    } else {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
    }

    EFFECT_FOR_EACH_TARGET(k, work->args) {
      struct Actor *target =
          GetBattleObjectSlotFar(work->args->targets[k])->actor;

      // Spin the skulls about a point above the target.
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, target->x.fixed, toFIXED(20), target->z.fixed);
      Matrix_TranslateVector(point);
      if (frame > k * 32) {
        Matrix_RotateX(frame << 9);
        if (frame == k * 32 + 32)
          BattleEffect_TintBattler(work->args->targets[k], SPRITE_COLOR_WHITE,
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
              draw[0](canvas, work->sheet + i % 3 * PICTURE_BYTES(SKULL),
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
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

#define ORBITING_SPIRALS_LIFE 60

/// Unused: no row of gAbilityBattleEffects in either game names its kind, and
/// the handler table is reached with no other kind (BattleEffect_RunFromTable),
/// so the player never sees it. Four spirals would circle 40 pixels above each
/// target in turn, 16 frames apart, for 59 frames each, 16 pixels out from
/// the centre; each target would take its damage pose 32 frames into its
/// turn.
void BattleEffect_RunUnusedOrbitingSpirals(struct BattleEffectParams *args) {
  // XXX: do these spirals circle each target as the doc above says? Staging
  // this function, which render guide --only unused has no stage for yet,
  // would settle it.
  struct BattleEffectWork *work;
  void *canvas;
  void *palette;
  BlitterPair draw;
  s32 point[3];
  struct ScreenPoint screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(0);
  palette = Resource_GetTableEntry(RESOURCE_SPIRAL_SHEET);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  Decompress_TaggedLz((u8 *)palette + SHEET_PALETTE_BYTES, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  if (work->args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(104 << 8);
    offsetX = -NARROW_CANVAS_PARTY_X;
  } else {
    offsetX = 0;
  }
  for (frame = 0; frame != (work->args->targetCount * 16) + 48; frame++) {
    struct BattleCamera *camera = gBattleCamera;
    s32 *row;
    s32 i;

    row = work->bg2xLines;
    if (work->args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ =
            (toFIXED(8) - Trig_Sin((frame + i) * (TRIG_TURN / 64)) * 8) >> 10;
    } else {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = (Trig_Sin((frame + i) * (TRIG_TURN / 64)) * 8 >> 10) -
                 (NARROW_CANVAS_PARTY_X << 8);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *actor;

      actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      if (frame > member * 16 &&
          frame < (member * 16) + ORBITING_SPIRALS_LIFE) {
        s32 spin;

        if (frame == (member * 16) + 32) {
          BattleEffect_TintBattler(work->args->targets[member],
                                   SPRITE_COLOR_NORMAL, POSE_DAMAGE, TINT_KEEP,
                                   0);
        }
        SET_VECTOR(point, actor->x.fixed, toFIXED(40), actor->z.fixed);
        Projection_Point(point, &screen);

        // The spirals, a quarter turn apart.
        for (i = 0; i != 4; i++) {
          s32 x;
          s32 y;
          s32 half;

          spin = (frame << 9) + (i << 14);
          x = (screen.x + FIXEDtoINT(Trig_Sin(spin) * 16)) + offsetX;
          y = screen.y + FIXEDtoINT(Trig_Cos(spin) * 16);
          half = frame / 16;
          draw[half & 1](canvas,
                         work->sheet +
                             (frame / 4 - half * 4) * PICTURE_BYTES(SPIRAL),
                         x - SPIRAL_WIDTH / 2, y - SPIRAL_HEIGHT / 2,
                         SPIRAL_WIDTH, SPIRAL_HEIGHT);
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
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
#undef ORBITING_SPIRALS_LIFE
