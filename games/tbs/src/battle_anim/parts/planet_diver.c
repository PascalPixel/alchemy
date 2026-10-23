/// @file
/// Planet Diver, Haunt and the orbiting spirals.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

// - Planet Diver flashes the user, throws it into the air, turns it into a
//   ball of fire that falls on the target and bursts into 256 chips that
//   bounce on the ground. The impact chips reuse the map grid buffer that other
//   animations address as an array of Particles.
// - The closing skulls (Haunt) gather around each target in turn, eight
//   skulls scattered in 3D that step towards a point above the target while
//   the group spins. They take 64 slots of PARTICLES per target, of which 8
//   are used.
// - The orbiting spirals circle above each target in turn, four of them a
//   quarter turn apart.
//
// The skulls and spirals ripple the animation layer throughout, through the
// HBlank DMA of BG2's affine origin.

// The leap that falls as a ball of fire.

/// One chip of the impact, laid over the same 28-byte entries as PARTICLES
/// but with its velocities in other places than struct Particle's.
struct ImpactChip {
  s32 x;         ///< canvas position, 16.16 fixed point
  s32 y;         ///< canvas position, 16.16; the ground is y 112
  s32 velocityX; ///< 16.16 pixels a frame, damped to 56/64 each frame
  s32 unused0c;  ///< an Particle's velocityX; never read
  s32 velocityY; ///< 16.16 pixels a frame, with 1/8 pixel of gravity
  s32 unused14;  ///< an Particle's velocityZ; never read
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
/// Abilities: Planet Diver (BATTLE_ANIM_PLANET_DIVER).
///
/// The ball falls towards the middle of the field, from whichever side the
/// user stands on; actors[0] is where it lands, and effect->side picks the
/// ball's facing.
///
/// Moves the user's object: it leaps on frame 4 and falls from frame 18. The
/// flash's sheet and palette are replaced by the fireball's on frame 16. The
/// sheet slot holds the mote sprites the chips are drawn from, and the chips
/// themselves live in Ram_MapGridCellBuffer.
void BattleAnim_RunPlanetDiver(struct BattleAnimArgs *effect) {
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  u8 *sprites;
  struct Object *target;
  s32 dx;
  struct Object *actor;
  u8 *palette;
  s32 frame;
  s32 i;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  sprites = ANIM_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  Decompress_Type01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA),
                    sprites);
  // A sheet resource is its palette followed by its packed images.
  palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_Type01(palette, work->sheet);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                    BLITTER_MAX);
  draw[1] = ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE_2);
  // Trails keep 3/4 a frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Four times as many chips are freed as the burst uses.
  for (i = 0; i != PLANET_DIVER_CLEARED_CHIPS; i++)
    PLANET_DIVER_IMPACT_CHIPS[i].life = 0;
  actor = GetBattleObjectSlotFar(work->effect->user)->object;
  target = GetBattleObjectSlotFar(work->effect->targets[0])->object;
  // The ball falls 15 pixels a frame towards the middle of the field.
  if (actor->x > 0)
    dx = -(15 << 16);
  else
    dx = 15 << 16;

  for (frame = 0; frame != PLANET_DIVER_FRAMES; frame++) {
    struct BattleCamera *camera = gCameraWork;

    ANIM_VIEW_FROM_CAMERA(camera);
    // Frame loop. The user's screen position is taken on frame 0 and from
    // frame 18 on, and held in between, so the flash stays where the leap
    // began.
    if (frame > 17 || frame == 0) {
      Projection_Unit(work->effect->user, &screenPosition);
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

        draw[0](canvas, work->sheet + ((frame - 4) / 2 << 11), x - 16, y - 64,
                32, 64);
      }
    }
    // Frame 4: the user leaps, heading out past its own position: 20 pixels
    // a frame upwards against a gravity of 0xab85 (about 0.67 pixels a frame
    // lost each frame), and outwards at up to 3 pixels a frame towards three
    // times its x, away from the middle (x 0).
    if (frame == 4) {
      actor->velocityY = 20 << 16;
      actor->acceleration = 1 << 16;
      actor->speedLimit = 3 << 16;
      actor->gravity = 0xab85;
      actor->steeringFlags = 0;
      actor->snapToTarget = 0;
      Object_SetMoveTargetFar((struct Object *)actor, actor->x * 3, 0,
                              actor->z);
      Object_SelectAnimationFar(actor, POSE_LEAP);
      // A 4-frame shake: frame is 4 here.
      work->shakeFrames = frame;
      Sound_PlaySongFar(SE_136);
    }
    // Frame 16: switch to the fireball images and line the user up with
    // the target's depth.
    if (frame == 16) {
      palette = Resource_GetTableEntry((s32)&ResourceId_FlameballSheet);
      Iwram_CopyWords((void *)BG_PLTT, palette, 128);
      palette += 128;
      Decompress_Type01(palette, work->sheet);
      actor->gravity = 0;
      actor->velocityX = 0;
      actor->velocityY = 0;
      actor->z = target->z;
      Object_StartIdleFar((struct Object *)actor);
    }
    // From frame 18: fall 15 pixels across and 8 down a frame; on reaching
    // the ground, throw out 256 chips, and the target recoils and flashes
    // as the battle shows the damage.
    if (frame > 17) {
      if (actor->y > 0) {
        actor->x += dx;
        actor->y -= 8 << 16;
        // The nudge after drawing lasts only to this frame's landing test,
        // where the chips start; the next frame projects the user afresh.
        // XXX: likely meant as screenPosition.x += 8, because side 0 moves
        // x; a capture of an enemy's Planet Diver would show where its chips
        // start.
        if (work->effect->side == 0) {
          draw[0](canvas, work->sheet, screenPosition.x - 20,
                  screenPosition.y - 52, 40, 64);
          screenPosition.x -= 8;
        } else {
          draw[1](canvas, work->sheet, screenPosition.x - 26,
                  screenPosition.y - 52, 40, 64);
          screenPosition.y += 8;
        }
      }
      if (actor->y < 0) {
        actor->y = 0;
        // Each chip flies off 24 pixels above the landing at a random angle
        // and a speed of 0.5 to 16.5 pixels a frame (speed / 64), doubled
        // vertically, and lives 32 to 39 frames.
        for (i = 0; i != PLANET_DIVER_CHIPS; i++) {
          struct ImpactChip *chip = &PLANET_DIVER_IMPACT_CHIPS[i];
          s32 speed;
          s32 angle;

          speed = 0x3ff;
          // Masked in place, the mask assigned first; as Random16() & mask, 3
          // instructions differ.
          speed &= Random16();
          angle = Random16() & 0xffff;
          chip->x = screenPosition.x << 16;
          chip->y = (screenPosition.y - 24) << 16;
          chip->velocityX = Trig_Sin(angle) * (speed + 32) >> 6;
          chip->velocityY = -(Trig_Cos(angle) * (speed + 32) << 1) >> 6;
          chip->life = (Random16() & 7) + 32;
        }
        work->shakeFrames = 8;
        ANIM_SHOW_DAMAGE(SE_145);
        BattleMotion_ApplyKnockbackFar(work->effect->targets[0],
                                       KNOCKBACK_HURL);
        BattleAnim_TintBattler(work->effect->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
      }
    }

    // Chips: slow to 56/64 of their speed each frame, fall, and bounce at
    // half speed off y 112; only those on the canvas are drawn, shrinking
    // a size every 8 frames of life.
    for (i = 0; i != PLANET_DIVER_CHIPS; i++) {
      struct ImpactChip *chip = &PLANET_DIVER_IMPACT_CHIPS[i];
      s32 y;

      if (chip->life > 0) {
        chip->x += chip->velocityX;
        y = chip->y + chip->velocityY;
        chip->life--;
        chip->y = y;
        chip->velocityX = chip->velocityX * 56 / 64;
        chip->velocityY = chip->velocityY * 56 / 64 + 0x2000;
        if (chip->y > 112 << 16) {
          chip->velocityY = -chip->velocityY / 2;
        } else if ((chip->x >= 0 && chip->x < 127 << 16) && chip->y >= 0) {
          s32 size = chip->life / 8 + 1;

          ANIM_DRAW_MOTE(draw[i & 1], canvas, sprites, (chip->x >> 16),
                         (chip->y >> 16), size);
        }
      }
    }
    BattleCamera_Shake(16, 16);
    ANIM_END_FRAME(work);
  }

  ANIM_FINISH_CANVAS();
}

// Closing skulls.

/// Skull slots scattered at the start: HAUNT_SLOTS for each of up to eight
/// targets.
#define HAUNT_SCATTERED 512
#define HAUNT_SKULLS 8 ///< skulls drawn around each target
#define HAUNT_SLOTS 64 ///< particles reserved for each target

/// Skulls close in on each target in turn.
///
/// Abilities: Haunt (BATTLE_ANIM_HAUNT).
///
/// Around each target, 32 frames apart, eight skulls appear one every 4 frames,
/// scattered up to 64 pixels away, and close in on a point 20 pixels above the
/// target while the group spins; the target flashes 32 frames into its turn,
/// and a skull that reaches the centre is no longer drawn. The animation layer
/// ripples throughout. PARTICLES holds 64 skulls per target, of which the first
/// 8 are used.
///
/// Adds BattleAnim_ArmBg2AffineHBlankDma, which feeds the ripple in
/// work->bg2xLines to BG2X line by line, and removes it before it returns.
void BattleAnim_RunHaunt(struct BattleAnimArgs *effect) {
  struct ScreenPoint screenPosition;
  s32 point[3];
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *palette;
  s32 i;
  s32 k;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_Type01(palette, work->sheet);
  // The skulls are drawn in the pink burst's colours, which replace the
  // skull sheet's own.
  palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
  draw[1] = ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE_2);
  Task_Add(BattleAnim_ArmBg2AffineHBlankDma, ANIM_CALLBACK_ORDER);
  // 4 off every pixel each frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x04040404);

  // Every skull starts up to 64 pixels from its centre on each axis.
  for (i = 0; i != HAUNT_SCATTERED; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  Sound_PlaySongFar(SE_142);

  for (frame = 0; frame != work->effect->targetCount * 32 + 96; frame++) {
    s32 *row;

    camera = gCameraWork;
    // Frame loop: 32 frames a target, then 96 more. The battle shows the
    // damage on frame 96, whatever the number of targets.
    if (frame == 96)
      ANIM_SHOW_DAMAGE(0);

    // Ripple the canvas sideways, line by line: a 16.16 value >> 10 is a
    // quarter of it in BG2X's 20.8, so the sine swings 1.5 texels either
    // way, about a 1.5-texel offset against the enemies. The wave is 32 lines
    // long (1/32 turn a line) and moves up a line each frame.
    row = work->bg2xLines;
    if (work->effect->side == 0) {
      for (i = 0; i != 160; i++)
        *row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
    } else {
      for (i = 0; i != 160; i++)
        *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
    }

    ANIM_FOR_EACH_TARGET(k, work->effect) {
      struct Object *target =
          GetBattleObjectSlotFar(work->effect->targets[k])->object;

      // Centre on the point 20 pixels above the target and spin the group
      // about it, a full turn every 128 frames; target k's turn starts on
      // frame 32k, its skull i on frame 4 * (8k + i), and it flashes 32
      // frames into its turn.
      ANIM_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, target->x, 20 << 16, target->z);
      Transform_ApplyPosition(point);
      if (frame > k * 32) {
        Transform_ApplyPitch(frame << 9);
        if (frame == k * 32 + 32)
          BattleAnim_TintBattler(work->effect->targets[k], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, k, 32);

        // Each skull steps one pixel nearer the centre a frame (its
        // position less itself over its distance in pixels); skull i takes
        // image i % 3 of three 24x24 images. variant counts its steps but is
        // never read.
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
    ANIM_END_FRAME(work);
  }

  // Finish: both callbacks before the blocks they read.
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleAnim_ArmBg2AffineHBlankDma);
  ANIM_RELEASE_BLITTERS();
  BattleAnim_EndCanvasLayer();
}

/// Frames from the start of a target's turn to its end; its spirals show on
/// the 59 frames between.
#define ORBITING_SPIRALS_LIFE 60

/// Unused: no row of ability_anims.tsv in either game names its kind, and the
/// handler table is reached with no other kind (BattleAnim_RunTableEffect),
/// so the player never sees it. Four spirals would circle 40 pixels above each
/// target in turn, 16 frames apart, for 59 frames each, 16 pixels out from
/// the centre; each target would take its damage pose 32 frames into its
/// turn.
///
/// Abilities: none (BATTLE_ANIM_UNUSED_ORBITING_SPIRALS).
///
/// The animation layer ripples throughout, through
/// BattleAnim_ArmBg2AffineHBlankDma, which it adds and removes. It never lets
/// the battle show damage, and leaves the targets' flash timers alone.
void BattleAnim_RunUnusedOrbitingSpirals(struct BattleAnimArgs *effect) {
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  void *palette;
  BlitterPair draw;
  s32 point[3];
  struct ScreenPoint screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  ANIM_START_NARROW_CANVAS(0);
  palette = Resource_GetTableEntry((s32)&ResourceId_SpiralSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  Decompress_Type01((u8 *)palette + 128, work->sheet);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP,
                    BLITTER_MAX);
  draw[1] = ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE_2);
  Task_Add(BattleAnim_ArmBg2AffineHBlankDma, ANIM_CALLBACK_ORDER);
  // Trails keep half a frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  // On the party's side the canvas is scrolled 104 texels (BG2X is 20.8),
  // and the spirals are drawn 112 further left.
  // XXX: the ripple below rewrites BG2X every line with a 112-texel lead,
  // so whether this 104 ever shows is unknown; the HBlank DMA's start
  // timing would settle it.
  if (work->effect->side == 1) {
    REG_BG2X = -0x6800;
    offsetX = -112;
  } else {
    offsetX = 0;
  }
  for (frame = 0; frame != (work->effect->targetCount * 16) + 48; frame++) {
    struct BattleCamera *camera;
    s32 *row;
    s32 i;

    // Frame loop. Ripple the canvas sideways, line by line, 2 texels either
    // way (16.16 >> 10 is a quarter in BG2X's 20.8; the products keep the
    // signed sine out of a left shift). The wave is 64 lines long (1/64 turn
    // a line) and moves up a line each frame.
    camera = gCameraWork;
    row = work->bg2xLines;
    if (work->effect->side == 0) {
      s32 angle;
      s32 base;

      for (i = 0, base = 0x80000, angle = frame << 10; i != 160; i++) {
        *row++ = (base - ((Trig_Sin(angle) * 8))) >> 10;
        angle += 1024;
      }
    } else {
      s32 angle;

      for (i = 0, angle = frame << 10; i != 160; i++) {
        *row++ = (((Trig_Sin(angle) * 8)) >> 10) - 0x7000;
        angle += 1024;
      }
    }
    ANIM_VIEW_FROM_CAMERA(camera);
    member = 0;
    if (work->effect->targetCount != 0) {
      while (member != work->effect->targetCount) {
        struct Object *object;

        object = GetBattleObjectSlotFar(work->effect->targets[member])->object;
        // Target `member` has its turn from frame 16 * member, and takes
        // its damage pose 32 frames in.
        if (frame > member * 16 &&
            frame < (member * 16) + ORBITING_SPIRALS_LIFE) {
          s32 spin;

          if (frame == (member * 16) + 32) {
            BattleAnim_TintBattler(work->effect->targets[member], 0,
                                   POSE_DAMAGE, -1, 0);
          }
          SET_VECTOR(point, object->x, 40 << 16, object->z);
          Projection_Point(point, &screen);

          // The spirals (32x32) a quarter turn apart, circling once every 128
          // frames; each shows its four 1 KB images for 4 frames apiece, and
          // every other 16 frames draws them mirrored both ways, turned half
          // a turn.
          for (i = 0; i != 4; i++) {
            s32 x;
            s32 y;
            s32 half;

            spin = (frame << 9) + (i << 14);
            x = (screen.x + (((Trig_Sin(spin) * 16)) >> 16)) + offsetX;
            y = screen.y + (((Trig_Cos(spin) * 16)) >> 16);
            half = frame / 16;
            draw[half & 1](canvas,
                           work->sheet + (((frame / 4) - (half * 4)) << 10),
                           x - 16, y - 16, 32, 32);
          }
        }
        member++;
      }
    }
    ANIM_PRESENT_FRAME(work);
  }
  // Finish: both callbacks before the blocks they read.
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleAnim_ArmBg2AffineHBlankDma);
  ANIM_RELEASE_BLITTERS();
  BattleAnim_EndCanvasLayer();
}

#undef PLANET_DIVER_IMPACT_CHIPS
#undef PLANET_DIVER_FRAMES
#undef PLANET_DIVER_CLEARED_CHIPS
#undef PLANET_DIVER_CHIPS
#undef HAUNT_SCATTERED
#undef HAUNT_SKULLS
#undef HAUNT_SLOTS
#undef ORBITING_SPIRALS_LIFE
