/// @file
/// Planet Diver, Haunt and the orbiting spirals.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// - Planet Diver flashes the user, throws it into the air, turns it into a
//   ball of fire that falls on the target and bursts into 256 chips that
//   bounce on the ground. The impact chips reuse the map grid buffer that other
//   effects address as an array of Particles.
// - The closing skulls (Haunt) gather around each target in turn, eight
//   skulls scattered in 3D that step towards a point above the target while
//   the group spins. They take 64 slots of PARTICLES per target, of which 8
//   are used.
// - The orbiting spirals circle above each target in turn, four of them a
//   quarter turn apart.
//
// The skulls and spirals ripple the effect layer throughout, through the
// HBlank DMA of BG2's affine origin.

// The leap that falls as a ball of fire.

/// One chip of the impact, laid over the same 28-byte entries as PARTICLES
/// but with its velocities in other places than struct Particle's.
struct ImpactChip {
  s32 x;         ///< canvas position, 16.16 fixed point
  s32 y;         ///< canvas position, 16.16; the ground is y 112
  s32 velocityX; ///< 16.16 pixels a frame, damped to 56/64 each frame
  s32 unused0c;  ///< a Particle's velocityX; never read
  s32 velocityY; ///< 16.16 pixels a frame, with 1/8 pixel of gravity
  s32 unused14;  ///< a Particle's velocityZ; never read
  s32 life;      ///< frames left; 0 is free
};

/// Ram_MapGridCellBuffer seen as Planet Diver's chips.
#define PLANET_DIVER_IMPACT_CHIPS ((struct ImpactChip *)Ram_MapGridCellBuffer)

#define PLANET_DIVER_FRAMES 88
#define PLANET_DIVER_CLEARED_CHIPS 1024 ///< map grid buffer slots cleared first
#define PLANET_DIVER_CHIPS 256          ///< chips the fireball bursts into

/// The user flashes and leaps into the air, turns into a ball of fire, falls
/// onto the first target and bursts into chips that bounce on the ground.
///
/// Abilities: Planet Diver (BATTLE_EFFECT_PLANET_DIVER).
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
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA),
                      sprites);
  // A sheet resource is its palette followed by its packed images.
  palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  // Trails keep 3/4 a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Four times as many chips are freed as the burst uses.
  for (i = 0; i != PLANET_DIVER_CLEARED_CHIPS; i++)
    PLANET_DIVER_IMPACT_CHIPS[i].life = 0;
  actor = GetBattleObjectSlotFar(work->args->user)->actor;
  target = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  // The ball falls 15 pixels a frame towards the middle of the field.
  if (actor->x.fixed > 0)
    dx = -px(15);
  else
    dx = px(15);

  for (frame = 0; frame != PLANET_DIVER_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame > 17 || frame == 0) {
      Projection_Unit(work->args->user, &screenPosition);
      screenPosition.x /= 2;
    }

    // The flash: frames 2 and 3 on the user, frames 4 to 15 spreading in
    // 16 directions through six 32x64 images, two frames each.
    if (frame >= 2 && frame <= 3)
      draw[0](canvas, work->sheet, screenPosition.x - 16, screenPosition.y - 64,
              32, 64);
    if (frame >= 4 && frame <= 15) {
      for (i = 0; i != 16; i++) {
        s32 x = screenPosition.x + (frame * Trig_Sin(i << 12) >> 16);
        s32 y = screenPosition.y + (frame * Trig_Cos(i << 12) >> 16) - frame;

        draw[0](canvas, work->sheet + (frame - 4) / 2 * (32 * 64), x - 16,
                y - 64, 32, 64);
      }
    }
    // The user leaps, heading out past its own position.
    if (frame == 4) {
      actor->velocityY = px(20);
      actor->acceleration = px(1);
      actor->speedLimit = px(3);
      actor->gravity = BATTLE_UNIT_GRAVITY;
      actor->steeringFlags = 0;
      actor->snapToTarget = 0;
      Actor_SetMoveTargetFar(actor, actor->x.fixed * 3, 0, actor->z.fixed);
      Actor_SelectAnimationFar(actor, POSE_LEAP);
      work->shakeFrames = 4;
      Sound_PlayFar(SE_WHOOSH);
    }
    // Switch to the fireball images and line the user up with the target's
    // depth.
    if (frame == 16) {
      palette = Resource_GetTableEntry((s32)&ResourceId_FlameballSheet);
      Iwram_CopyWords((void *)BG_PLTT, palette, 128);
      palette += 128;
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
        actor->y.fixed -= px(8);
        // The nudge after drawing lasts only to this frame's landing test,
        // where the chips start; the next frame projects the user afresh.
        // On EFFECT_SIDE_PARTY the nudge moves y, so the chips start 8 pixels
        // lower rather than 8 pixels across.
        // XXX (no evidence): whether screenPosition.x += 8 was meant, as side
        // 0 moves x; only the author's intent would say.
        if (work->args->side == EFFECT_SIDE_ENEMIES) {
          draw[0](canvas, work->sheet, screenPosition.x - 20,
                  screenPosition.y - 52, 40, 64);
          screenPosition.x -= 8;
        } else {
          draw[1](canvas, work->sheet, screenPosition.x - 26,
                  screenPosition.y - 52, 40, 64);
          screenPosition.y += 8;
        }
      }
      if (actor->y.fixed < 0) {
        actor->y.fixed = 0;
        // Each chip flies off 24 pixels above the landing at a random angle
        // and a speed of 0.5 to 16.5 pixels a frame (speed / 64), doubled
        // vertically, and lives 32 to 39 frames.
        for (i = 0; i != PLANET_DIVER_CHIPS; i++) {
          struct ImpactChip *chip = &PLANET_DIVER_IMPACT_CHIPS[i];
          s32 speed;
          s32 angle;

          speed = (Random16() & 0x3ff) + 32;
          angle = Random16() & TRIG_ANGLE_MASK;
          chip->x = px(screenPosition.x);
          chip->y = px(screenPosition.y - 24);
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

    // Chips: slow to 56/64 of their speed each frame, fall, and bounce at half
    // speed off y 112.
    for (i = 0; i != PLANET_DIVER_CHIPS; i++) {
      struct ImpactChip *chip = &PLANET_DIVER_IMPACT_CHIPS[i];
      s32 y;

      if (chip->life > 0) {
        chip->x += chip->velocityX;
        y = chip->y + chip->velocityY;
        chip->life--;
        chip->y = y;
        chip->velocityX = chip->velocityX * 56 / 64;
        chip->velocityY = chip->velocityY * 56 / 64 + px(1) / 8;
        if (chip->y > px(112)) {
          chip->velocityY = -chip->velocityY / 2;
        } else if ((chip->x >= 0 && chip->x < px(127)) && chip->y >= 0) {
          s32 size = chip->life / 8 + 1;

          EFFECT_DRAW_MOTE(draw[i & 1], canvas, sprites, (chip->x >> 16),
                           (chip->y >> 16), size);
        }
      }
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Closing skulls.

/// Skull slots scattered at the start: HAUNT_SLOTS for each of up to eight
/// targets.
#define HAUNT_SCATTERED 512
#define HAUNT_SKULLS 8 ///< skulls drawn around each target
#define HAUNT_SLOTS 64 ///< particles reserved for each target

/// Skulls close in on each target in turn.
///
/// Abilities: Haunt (BATTLE_EFFECT_HAUNT).
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
  palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  // The skulls are drawn in the pink burst's colours, which replace the
  // skull sheet's own.
  palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  // 4 off every pixel each frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

  // Every skull starts up to 64 pixels from its centre on each axis.
  for (i = 0; i != HAUNT_SCATTERED; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != work->args->targetCount * 32 + 96; frame++) {
    s32 *row;
    struct BattleCamera *camera = gBattleCamera;

    // Frame loop: 32 frames a target, then 96 more.
    if (frame == 96)
      EFFECT_SHOW_DAMAGE(0);

    // Ripple the canvas sideways, line by line.
    row = work->bg2xLines;
    if (work->args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = ((6 << 16) - Trig_Sin((frame + i) << 11) * 6) >> 10;
    } else {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
    }

    EFFECT_FOR_EACH_TARGET(k, work->args) {
      struct Actor *target =
          GetBattleObjectSlotFar(work->args->targets[k])->actor;

      // Centre on the point 20 pixels above the target and spin the group about
      // it, a full turn every 128 frames.
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, target->x.fixed, px(20), target->z.fixed);
      Transform_ApplyPosition(point);
      if (frame > k * 32) {
        Transform_ApplyPitch(frame << 9);
        if (frame == k * 32 + 32)
          BattleEffect_TintBattler(work->args->targets[k], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, k, 32);

        // Each skull steps one pixel nearer the centre a frame (its position
        // less itself over its distance in pixels).
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
              draw[0](canvas, work->sheet + i % 3 * (24 * 24),
                      screenPosition.x - 12, screenPosition.y - 12, 24, 24);
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

  // Finish: both tasks before the blocks they read.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

/// Frames from the start of a target's turn to its end; its spirals show on
/// the 59 frames between.
#define ORBITING_SPIRALS_LIFE 60

/// Unused: no row of gAbilityBattleEffects in either game names its kind, and
/// the handler table is reached with no other kind (BattleEffect_RunFromTable),
/// so the player never sees it. Four spirals would circle 40 pixels above each
/// target in turn, 16 frames apart, for 59 frames each, 16 pixels out from
/// the centre; each target would take its damage pose 32 frames into its
/// turn.
///
/// Abilities: none (BATTLE_EFFECT_UNUSED_ORBITING_SPIRALS).
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
  palette = Resource_GetTableEntry((s32)&ResourceId_SpiralSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  Decompress_TaggedLz((u8 *)palette + 128, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                      COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  // Trails keep half a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  // On the party's side the canvas is scrolled 104 texels (BG2X is 20.8), and
  // the spirals are drawn 112 further left.
  if (work->args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(104 << 8);
    offsetX = -112;
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
        *row++ = (px(8) - Trig_Sin((frame + i) * (TRIG_TURN / 64)) * 8) >> 10;
    } else {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ =
            (Trig_Sin((frame + i) * (TRIG_TURN / 64)) * 8 >> 10) - (112 << 8);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *actor;

      actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      // Target `member` has its turn from frame 16 * member, and takes
      // its damage pose 32 frames in.
      if (frame > member * 16 &&
          frame < (member * 16) + ORBITING_SPIRALS_LIFE) {
        s32 spin;

        if (frame == (member * 16) + 32) {
          BattleEffect_TintBattler(work->args->targets[member],
                                   SPRITE_COLOR_NORMAL, POSE_DAMAGE, TINT_KEEP,
                                   0);
        }
        SET_VECTOR(point, actor->x.fixed, px(40), actor->z.fixed);
        Projection_Point(point, &screen);

        // The spirals (32x32) a quarter turn apart, circling once every 128
        // frames.
        for (i = 0; i != 4; i++) {
          s32 x;
          s32 y;
          s32 half;

          spin = (frame << 9) + (i << 14);
          x = (screen.x + (Trig_Sin(spin) * 16 >> 16)) + offsetX;
          y = screen.y + (Trig_Cos(spin) * 16 >> 16);
          half = frame / 16;
          draw[half & 1](canvas,
                         work->sheet + (frame / 4 - half * 4) * (32 * 32),
                         x - 16, y - 16, 32, 32);
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  // Finish: both tasks before the blocks they read.
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
