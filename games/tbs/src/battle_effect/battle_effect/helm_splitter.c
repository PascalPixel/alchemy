/// @file
/// Helm Splitter, formations and the impact burst.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// - Helm Splitter's fire swirl charge leaps the user at the first target,
//   plays the swirl over it and scatters bouncing sparks. The sparks live in
//   PARTICLES, whose 1024 slots are cleared first.
// - The impact burst plays a flash with large and small sparks and three
//   fragments, while the effect's sprites settle into one of the formations
//   of BattleEffect_PlaceFormationObjects. Large sparks and the fragments use
//   work->particles (0 to 63 and 61 to 63, the fragments overwriting the last
//   large sparks), small sparks PARTICLES.

#define HELM_SPLITTER_FRAMES 70
#define HELM_SPLITTER_STRIKE_FRAME 18 ///< the swirl strikes the target
/// The pink star effect, after the swirl.
#define HELM_SPLITTER_STAR_PICTURES 16000
#define HELM_SPLITTER_CLEARED_SLOTS 1024 ///< PARTICLES slots freed first
#define HELM_SPLITTER_SPARKS 16          ///< sparks scattered by the strike
#define HELM_SPLITTER_SPARK_SLOTS 128    ///< spark slots moved and drawn

/// The user leaps at the first target and strikes through a fire swirl; on
/// frame 18 the target flashes and reels and 16 sparks scatter, bouncing off
/// the ground.
///
/// Abilities: Helm Splitter (BATTLE_EFFECT_HELM_SPLITTER).
void BattleEffect_RunHelmSplitter(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *graphics;
  DrawRectangle draw[2];
  struct ScreenPoint position;

  struct Particle *sparks;
  struct Particle *spark;
  s32 *particleLife;
  s32 frame;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  graphics = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireSwirlSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                             work->sheet + HELM_SPLITTER_STAR_PICTURES, 1, 0);
  BattleEffect_FetchRectangleBlitters(work->args->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // Free the spark pool before moving the user: the pointer walks the
  // variant word of each seven-word Particle. Only the first 16 sparks are
  // ever launched, and the first 128 moved.
  particleLife = &PARTICLES->variant;
  for (i = 0; i != HELM_SPLITTER_CLEARED_SLOTS; i++) {
    *particleLife = 0;
    particleLife += 7;
  }
  sparks = PARTICLES;

  // The user leaps 80% of the way to the first target over 20 frames,
  // jumping at 7 pixels a frame. The distance is taken in 1/256 pixels so
  // the squares fit in 32 bits, and a twentieth of it is both the
  // acceleration and the top speed.
  {
    struct Actor *actor = GetBattleObjectSlotFar(work->args->user)->actor;
    struct Actor *target =
        GetBattleObjectSlotFar(work->args->targets[0])->actor;
    s32 diffX = target->x.fixed - actor->x.fixed;
    s32 startX = actor->x.fixed;
    s32 deltaX = diffX * 80 / 100;
    s32 diffZ = target->z.fixed - actor->z.fixed;
    s32 startZ = actor->z.fixed;
    s32 deltaZ = diffZ * 80 / 100;
    s32 x = startX + deltaX;
    s32 z = startZ + deltaZ;
    s32 shortX = deltaX >> 8;
    s32 shortZ = deltaZ >> 8;
    s32 distance;

    distance = Iwram_Sqrt(shortX * shortX + shortZ * shortZ);
    distance = (distance << 8) / 20;
    actor->acceleration = distance;
    actor->speedLimit = distance;
    actor->snapToTarget = 1;
    actor->velocityY = px(7);
    actor->gravity = px(87) / 100;
    actor->bounce = 0;
    actor->steeringFlags = 1;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, x, 0, z);
    Actor_SelectAnimationFar(actor, POSE_LEAP);
  }

  // Play the swirl and impact, then let the sparks bounce and fade.
  for (frame = 0; frame != HELM_SPLITTER_FRAMES; frame++) {
    // The canvas follows the user across the screen: canvas column 80 is
    // kept under the user.
    Projection_Unit(work->args->user, &position);
    REG_BG2X = (80 - position.x) << 8;
    // Frames 8 to 23: the swirl plays its seven pictures, two frames each,
    // at the user's height, placed from the other edge on EFFECT_SIDE_PARTY.
    if (frame >= 8 && frame <= 23) {
      s32 picture = (frame - 8) / 2;

      if (picture > 6)
        picture = 6;
      if (work->args->side == EFFECT_SIDE_ENEMIES) {
        draw[0](canvas, work->sheet + gHelmSplitter_Pictures[picture],
                gHelmSplitter_PictureBiasX[picture] + 30,
                gHelmSplitter_PictureBiasY[picture] + position.y - 60,
                gHelmSplitter_PictureWidths[picture],
                gHelmSplitter_PictureHeights[picture]);
      } else {
        draw[0](canvas, work->sheet + gHelmSplitter_Pictures[picture],
                -gHelmSplitter_PictureBiasX[picture] -
                    gHelmSplitter_PictureWidths[picture] + 108,
                gHelmSplitter_PictureBiasY[picture] + position.y - 60,
                gHelmSplitter_PictureWidths[picture],
                gHelmSplitter_PictureHeights[picture]);
      }
    }
    // The strike: the battle shows the damage, the target flashes and
    // reels, the camera shakes and 16 sparks fly from (64, 80), each living
    // 16 to 31 frames, in random directions at 2 to 2.5 pixels a frame, the
    // vertical part doubled.
    if (frame == HELM_SPLITTER_STRIKE_FRAME) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
      BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_BOUNCE);
      work->shakeFrames = 4;
      for (i = 0; i != HELM_SPLITTER_SPARKS; i++) {
        s32 force = (Random16() & 63) + 256;
        s32 angle = Random16() & TRIG_ANGLE_MASK;

        spark = &sparks[i];
        spark->x = px(64);
        spark->y = px(80);
        spark->velocityX = (Trig_Sin(angle) * force) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * force) >> 6;
        spark->variant = (Random16() & 15) + 16;
      }
    }
    // Sparks fly straight, with no gravity, their speed damped to 60/64. A
    // spark below row 104 has its vertical speed reversed and halved
    // instead of being drawn; one on the canvas is drawn with the pink star
    // effect, each spark at its own point of the six-picture cycle.
    for (i = 0; i != HELM_SPLITTER_SPARK_SLOTS; i++) {
      spark = &sparks[i];
      if (spark->variant > 0) {
        spark->variant--;
        Particle_AdvanceWithGravity2D(spark, 60, 0);
        if (spark->y > px(104)) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x <= px(127) - 1) &&
                   spark->y >= 0) {
          s32 sx = spark->x >> 16;
          s32 sy = spark->y >> 16;

          draw[0](canvas,
                  work->sheet + HELM_SPLITTER_STAR_PICTURES +
                      (((frame + i) / 4 % 6) << 8),
                  sx - 8, sy - 8, 16, 16);
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

/// Arranges work->effects in a formation; x and z are its origin in 16.16
/// pixels.
///
/// Each summon that ends in an impact burst keeps its sprites in its own
/// formation, every frame while it plays and then through
/// BattleEffect_RunImpactBurst. Places the sprites through
/// Actor_ApplyProjectedPlacementFar and changes nothing else.
///
/// formation: the summon's layout (enum SummonFormation); any other value
///            places nothing.
void BattleEffect_PlaceFormationObjects(s32 formation, s32 x, s32 z) {
  static struct SpriteScale normalScale = {{{Q16_ONE, Q16_ONE}}};
  static struct SpriteScale largeScale = {{{Q16_ONE * 5 / 4, Q16_ONE * 5 / 4}}};
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  struct SpriteScale normal = normalScale;
  struct SpriteScale big = largeScale;
  struct SpritePlacement pos;
  s32 i;

  pos.groundHeight = 0;
  pos.height = px(255);
  // Render_ApplyProjectedPlacement draws each picture 255 pixels above its
  // y, while its draw slot, and any shadow, still follow y itself.
  switch (formation) {
  case SUMMON_FORMATION_THOR:
    for (i = 0; i != 9; i++) {
      pos.x = px(i % 3 * 32) + x;
      pos.y = px(i / 3 * 32) + z;
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &pos, &normal, 0);
    }
    break;
  case SUMMON_FORMATION_NEREID:
    for (i = 0; i != 12; i++) {
      pos.x = px(gHelmSplitter_TwelveX[i]) + x - px(16);
      pos.y = px(gHelmSplitter_TwelveZ[i]) + z - px(32);
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &pos, &normal, 0);
    }
    break;
  case SUMMON_FORMATION_CYBELE:
    for (i = 0; i != 8; i++) {
      pos.x = px(gHelmSplitter_RightEightX[i]) + x + px(16);
      pos.y = px(gHelmSplitter_RightEightZ[i]) + z;
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &pos, &normal, 0);
    }
    break;
  case SUMMON_FORMATION_TIAMAT:
    for (i = 0; i != 8; i++) {
      pos.x = px(gHelmSplitter_SmallEightX[i]) + x;
      pos.y = px(gHelmSplitter_SmallEightZ[i]) + z;
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &pos, &big, 0);
    }
    break;
  }
}

/// The fragment pictures in the sheet, four of 12x24, after the flash.
#define IMPACT_BURST_SHARD_PICTURES 23000
#define IMPACT_BURST_FRAGMENT_WIDTH 12  ///< each fragment picture's width
#define IMPACT_BURST_FRAGMENT_HEIGHT 24 ///< and height

#define IMPACT_BURST_FRAMES 72
#define IMPACT_BURST_FRAGMENTS 3     ///< fragments, work->particles 61 to 63
#define IMPACT_BURST_DRAWN_LARGE 30  ///< large sparks drawn
#define IMPACT_BURST_DRAWN_SMALL 60  ///< small sparks drawn
#define IMPACT_BURST_LARGE_SPARKS 64 ///< large sparks thrown
#define IMPACT_BURST_SMALL_SPARKS 64 ///< small sparks thrown
/// the first fragment in work->particles
#define IMPACT_BURST_FIRST_FRAGMENT 61

/// Opens an impact flash and scatters sparks and fragments, keeping the
/// effect's sprites in formation for the first 36 frames.
///
/// The close of four summons: Thor, Nereid, Cybele and Tiamat's
/// BattleEffect_RunDragonFire call it once their own frames are over,
/// each with its own formation. Builds and frees both blitters, starts and
/// stops the canvas transfer, sets REG_BG2PA, REG_BG2X and REG_BLDCNT, loads
/// pictures into the sheet slot and work->sheet and overwrites
/// work->particles 0 to 63 and PARTICLES 0 to 63. It leaves the canvas's
/// VRAM cleared; ending the canvas layer is left to the caller.
///
/// formation: the layout for BattleEffect_PlaceFormationObjects.
/// x, y: the formation's origin in 16.16 pixels; the burst is drawn 40 pixels
/// right of x, halved for the wide canvas.
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y) {
  void **slots;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle additive;
  DrawRectangle maxBlend;
  u8 *sheet;
  s32 orgX;
  struct Particle *particle;
  s32 frame;
  s32 i;

  // BG2 shows the canvas at twice its width, and BG1 and BG2 blend over the
  // layers below.
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = slots[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
  orgX = x;
  x += px(40);
  x /= 2;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  maxBlend = slots[HEAP_SLOT_RAM_CODE - HEAP_SLOT_BATTLE_EFFECT];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  additive = slots[HEAP_SLOT_RAM_CODE_2 - HEAP_SLOT_BATTLE_EFFECT];
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlareSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlareImage,
                             work->sheet + IMPACT_BURST_SHARD_PICTURES, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // Large sparks fly in random directions at 2 to 4 pixels a frame, the
  // vertical part doubled, and live 16 to 31 frames; only the first 30 are
  // drawn.
  for (i = 0; i != IMPACT_BURST_LARGE_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 256;
    s32 angle = Random16() & TRIG_ANGLE_MASK;

    work->particles[i].x = x;
    work->particles[i].y = y;
    work->particles[i].velocityX = (Trig_Sin(angle) * speed) >> 7;
    work->particles[i].velocityY = -((Trig_Cos(angle) * speed) >> 6);
    work->particles[i].variant = (Random16() & 15) + 16;
  }

  // Three fragments, 120 degrees apart on an ellipse half a pixel a frame
  // across and one up and down: the first straight up at 1 a frame, the
  // others about 0.43 across, one each way, and 0.5 down.
  for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].x = x;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].y = y;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityX =
        (Trig_Sin(i * (TRIG_TURN / 3)) * 32) >> 6;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityY =
        -((Trig_Cos(i * (TRIG_TURN / 3)) * 32) >> 5);
  }

  // Small sparks fly in random directions at half a pixel to 4.5 a frame,
  // the vertical part doubled, and live 20 to 35 frames.
  for (i = 0; i != IMPACT_BURST_SMALL_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 32;
    s32 angle = Random16() & TRIG_ANGLE_MASK;

    PARTICLES[i].x = x;
    PARTICLES[i].y = y;
    PARTICLES[i].velocityX = (Trig_Sin(angle) * speed) >> 6;
    PARTICLES[i].velocityY = -((Trig_Cos(angle) * speed) >> 5);
    PARTICLES[i].variant = (Random16() & 15) + 20;
  }

  // Frames 0 to 47 show the flash, and the large sparks join from frame 1.
  // From frame 36 the formation stays where it is, the small sparks fly and
  // the fragments tumble until frame 63.
  for (frame = 0; frame != IMPACT_BURST_FRAMES; frame++) {
    if (frame == 4)
      Sound_PlayFar(SE_CAST);
    if (frame == 32)
      Sound_PlayFar(SE_POWER_UP);

    // The flash: its first picture until frame 12, then the next every five
    // frames, eight in all.
    if (frame <= 47) {
      s32 picture = (frame - 8) / 5;
      u32 width;
      u32 height;

      if (picture < 0)
        picture = 0;
      // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
      width = gHelmSplitter_ImpactBurstPictureWidths[picture];
      height = gHelmSplitter_ImpactBurstPictureHeights[picture];
      maxBlend(canvas, work->sheet + gHelmSplitter_ImpactBurstPictures[picture],
               (x >> 16) - (width >> 1), (y >> 16) - (height >> 1), width,
               height);
#else
      maxBlend(
          canvas, work->sheet + gHelmSplitter_ImpactBurstPictures[picture],
          (x >> 16) -
              ((width = gHelmSplitter_ImpactBurstPictureWidths[picture]) >> 1),
          (y >> 16) -
              ((height = gHelmSplitter_ImpactBurstPictureHeights[picture]) >>
               1),
          width, height);
#endif
    }

    // Large sparks: the first 30 join two each frame, their motes shrinking
    // with the life they have left (size 4, then 3 below 16 frames).
    for (i = 0, particle = work->particles; i != IMPACT_BURST_DRAWN_LARGE;
         i++, particle++) {
      if (frame > i / 2 && particle->variant > 0) {
        s32 size;
        s32 drawX;
        s32 drawY;

        particle->variant--;
        Particle_AdvanceWithGravity2D(particle, 60, 0);
        size = particle->variant / 16 + 3;
        drawX = Q_16_16_TO_INT(particle->x);
        drawY = Q_16_16_TO_INT(particle->y);
        EFFECT_DRAW_MOTE(additive, canvas, sheet, drawX, drawY, size);
      }
    }

    // Small sparks start at frame 36.
    for (i = 0, particle = PARTICLES; i != IMPACT_BURST_DRAWN_SMALL;
         i++, particle++) {
      if (frame > 35 && particle->variant > 0) {
        s32 size;
        s32 drawX;
        s32 drawY;

        particle->variant--;
        Particle_AdvanceWithGravity2D(particle, 60, 0);
        size = particle->variant / 16 + 1;
        drawX = Q_16_16_TO_INT(particle->x);
        drawY = Q_16_16_TO_INT(particle->y);
        EFFECT_DRAW_MOTE(additive, canvas, sheet, drawX, drawY, size);
      }
    }

    // Fragments run on frames 36-63; advance the image every seven frames.
    for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
      struct Particle *shard =
          &work->particles[IMPACT_BURST_FIRST_FRAGMENT + i];

      if (frame >= 36 && frame <= 63) {
        s32 picture;
        s32 drawX;
        s32 drawY;

        Particle_AdvanceWithGravity2D(shard, 64, 0);
        picture = (frame - 36) / 7;
        drawX = Q_16_16_TO_INT(shard->x);
        drawY = Q_16_16_TO_INT(shard->y);
        maxBlend(canvas,
                 work->sheet +
                     picture * (IMPACT_BURST_FRAGMENT_WIDTH *
                                IMPACT_BURST_FRAGMENT_HEIGHT) +
                     IMPACT_BURST_SHARD_PICTURES,
                 drawX - IMPACT_BURST_FRAGMENT_WIDTH / 2,
                 drawY - IMPACT_BURST_FRAGMENT_HEIGHT / 2,
                 IMPACT_BURST_FRAGMENT_WIDTH, IMPACT_BURST_FRAGMENT_HEIGHT);
      }
    }

    if (frame <= 35)
      BattleEffect_PlaceFormationObjects(formation, orgX, y);
    EFFECT_PRESENT_CANVAS(work);
  }

  // Clear the canvas in VRAM (character block 1), so nothing is left on BG2.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  EFFECT_RELEASE_BLITTERS();
}

#undef HELM_SPLITTER_FRAMES
#undef HELM_SPLITTER_STRIKE_FRAME
#undef HELM_SPLITTER_STAR_PICTURES
#undef HELM_SPLITTER_CLEARED_SLOTS
#undef HELM_SPLITTER_SPARKS
#undef HELM_SPLITTER_SPARK_SLOTS
#undef IMPACT_BURST_SHARD_PICTURES
#undef IMPACT_BURST_FRAGMENT_WIDTH
#undef IMPACT_BURST_FRAGMENT_HEIGHT
#undef IMPACT_BURST_FRAMES
#undef IMPACT_BURST_FRAGMENTS
#undef IMPACT_BURST_DRAWN_LARGE
#undef IMPACT_BURST_DRAWN_SMALL
#undef IMPACT_BURST_LARGE_SPARKS
#undef IMPACT_BURST_SMALL_SPARKS
#undef IMPACT_BURST_FIRST_FRAGMENT
