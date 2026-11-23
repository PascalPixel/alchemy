/// @file
/// Helm Splitter, formations and the impact burst.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define HELM_SPLITTER_FRAMES 70
#define HELM_SPLITTER_STRIKE_FRAME 18 ///< the swirl strikes the target
/// The pink star effect, after the swirl.
#define HELM_SPLITTER_STAR_PICTURES 16000
#define HELM_SPLITTER_STAR_SIZE 16 ///< PinkStarSheet's six stars
#define HELM_SPLITTER_CLEARED_SLOTS                                            \
  PARTICLE_POOL_COUNT                 ///< PARTICLES slots freed first
#define HELM_SPLITTER_SPARKS 16       ///< sparks scattered by the strike
#define HELM_SPLITTER_SPARK_SLOTS 128 ///< spark slots moved and drawn

/// The user leaps at the first target and strikes through a fire swirl; the
/// target reels and sparks scatter, bouncing off the ground.
///
/// Abilities: Helm Splitter.
void BattleEffect_RunHelmSplitter(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *graphics;
  DrawRectangle draw[2];
  struct ScreenPoint position;

  struct Particle *sparks;
  struct Particle *spark;
  s32 frame;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  graphics = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_SWIRL_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PINK_STAR_SHEET,
                             work->sheet + HELM_SPLITTER_STAR_PICTURES, 1, 0);
  BattleEffect_FetchRectangleBlitters(work->args->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (i = 0; i != HELM_SPLITTER_CLEARED_SLOTS; i++)
    PARTICLES[i].variant = 0;
  sparks = PARTICLES;

  {
    struct Actor *actor = GetBattleObjectSlotFar(work->args->user)->actor;
    struct Actor *target =
        GetBattleObjectSlotFar(work->args->targets[0])->actor;
    FIXED diffX = target->x.fixed - actor->x.fixed;
    FIXED startX = actor->x.fixed;
    FIXED deltaX = diffX * 80 / 100;
    FIXED diffZ = target->z.fixed - actor->z.fixed;
    FIXED startZ = actor->z.fixed;
    FIXED deltaZ = diffZ * 80 / 100;
    FIXED x = startX + deltaX;
    FIXED z = startZ + deltaZ;
    s32 shortX = deltaX >> 8;
    s32 shortZ = deltaZ >> 8;
    s32 distance;

    distance = Iwram_Sqrt(shortX * shortX + shortZ * shortZ);
    distance = (distance << 8) / 20;
    actor->acceleration = distance;
    actor->speedLimit = distance;
    actor->snapToTarget = 1;
    actor->velocityY = toFIXED(7);
    actor->gravity = toFIXED(87) / 100;
    actor->bounce = 0;
    actor->steeringFlags = 1;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, x, 0, z);
    Actor_SelectAnimationFar(actor, POSE_LEAP);
  }

  // Play the swirl and impact, then let the sparks bounce and fade.
  for (frame = 0; frame != HELM_SPLITTER_FRAMES; frame++) {
    Projection_Unit(work->args->user, &position);
    REG_BG2X = (80 - position.x) << 8;
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
    // The strike.
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
        spark->x = toFIXED(64);
        spark->y = toFIXED(80);
        spark->velocityX = (Trig_Sin(angle) * force) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * force) >> 6;
        spark->variant = (Random16() & 15) + 16;
      }
    }
    for (i = 0; i != HELM_SPLITTER_SPARK_SLOTS; i++) {
      spark = &sparks[i];
      if (spark->variant > 0) {
        spark->variant--;
        Particle_AdvanceWithGravity2D(spark, 60, 0);
        if (spark->y > toFIXED(104)) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x <= toFIXED(127) - 1) &&
                   spark->y >= 0) {
          s32 sx = FIXEDtoINT(spark->x);
          s32 sy = FIXEDtoINT(spark->y);

          draw[0](canvas,
                  work->sheet + HELM_SPLITTER_STAR_PICTURES +
                      (frame + i) / 4 % 6 *
                          (HELM_SPLITTER_STAR_SIZE * HELM_SPLITTER_STAR_SIZE),
                  sx - HELM_SPLITTER_STAR_SIZE / 2,
                  sy - HELM_SPLITTER_STAR_SIZE / 2, HELM_SPLITTER_STAR_SIZE,
                  HELM_SPLITTER_STAR_SIZE);
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

/// The formations' sizes: Thor's 3 by 3 grid and its spacing in pixels,
/// Nereid's twelve, and Cybele's and Tiamat's eight.
#define FORMATION_GRID_COLUMNS 3
#define FORMATION_GRID_SPACING 32
#define FORMATION_TWELVE 12
#define FORMATION_EIGHT 8

/// Places a summon's figure, the sprites in work->sprites, in its formation;
/// x and z are its origin in 16.16 pixels.
void BattleEffect_PlaceFormationObjects(s32 formation, FIXED x, FIXED z) {
  static struct SpriteScale normalScale = {{{FIXED_ONE, FIXED_ONE}}};
  static struct SpriteScale largeScale = {
      {{FIXED_ONE * 5 / 4, FIXED_ONE * 5 / 4}}};
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  struct SpriteScale normal = normalScale;
  struct SpriteScale big = largeScale;
  struct SpritePlacement pos;
  s32 i;

  pos.groundHeight = 0;
  pos.height = toFIXED(255);
  // Render_ApplyProjectedPlacement draws each sprite 255 pixels above its y,
  // while its draw slot and shadow follow y itself.
  switch (formation) {
  case SUMMON_FORMATION_THOR:
    for (i = 0; i != FORMATION_GRID_COLUMNS * FORMATION_GRID_COLUMNS; i++) {
      pos.x =
          INTtoFIXED(i % FORMATION_GRID_COLUMNS * FORMATION_GRID_SPACING) + x;
      pos.y =
          INTtoFIXED(i / FORMATION_GRID_COLUMNS * FORMATION_GRID_SPACING) + z;
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &pos, &normal, 0);
    }
    break;
  case SUMMON_FORMATION_NEREID:
    for (i = 0; i != FORMATION_TWELVE; i++) {
      pos.x = INTtoFIXED(gHelmSplitter_TwelveX[i]) + x - toFIXED(16);
      pos.y = INTtoFIXED(gHelmSplitter_TwelveZ[i]) + z - toFIXED(32);
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &pos, &normal, 0);
    }
    break;
  case SUMMON_FORMATION_CYBELE:
    for (i = 0; i != FORMATION_EIGHT; i++) {
      pos.x = INTtoFIXED(gHelmSplitter_RightEightX[i]) + x + toFIXED(16);
      pos.y = INTtoFIXED(gHelmSplitter_RightEightZ[i]) + z;
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &pos, &normal, 0);
    }
    break;
  case SUMMON_FORMATION_TIAMAT:
    for (i = 0; i != FORMATION_EIGHT; i++) {
      pos.x = INTtoFIXED(gHelmSplitter_SmallEightX[i]) + x;
      pos.y = INTtoFIXED(gHelmSplitter_SmallEightZ[i]) + z;
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &pos, &big, 0);
    }
    break;
  }
}

/// The fragment pictures in the sheet, four of 12x24, after the flash.
#define IMPACT_BURST_FRAGMENT_PICTURES 23000
#define IMPACT_BURST_FRAGMENT_WIDTH 12 ///< each fragment picture's width
#define IMPACT_BURST_FRAGMENT_HEIGHT 24

#define IMPACT_BURST_FRAMES 72
#define IMPACT_BURST_FRAGMENTS 3     ///< fragments, work->particles 61 to 63
#define IMPACT_BURST_DRAWN_LARGE 30  ///< large sparks drawn
#define IMPACT_BURST_DRAWN_SMALL 60  ///< small sparks drawn
#define IMPACT_BURST_LARGE_SPARKS 64 ///< large sparks thrown
#define IMPACT_BURST_SMALL_SPARKS 64 ///< small sparks thrown
/// the first fragment in work->particles
#define IMPACT_BURST_FIRST_FRAGMENT 61
/// The fragments' speed apart.
#define IMPACT_BURST_FRAGMENT_SPEED 32
/// A spark's life left per size of its mote: a spark shrinks one size every
/// IMPACT_BURST_LIFE_PER_SIZE frames.
#define IMPACT_BURST_LIFE_PER_SIZE 16

/// Ends a summon: a flash opens where its figure stands and scatters sparks and
/// three fragments, the figure holding its formation for the first 36 frames.
void BattleEffect_RunImpactBurst(s32 formation, s32 x, FIXED y) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle additive;
  DrawRectangle maxBlend;
  u8 *sheet;
  FIXED originX;
  struct Particle *particle;
  s32 frame;
  s32 i;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  work = *cursor++;
  canvas = *cursor;
  sheet =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_EFFECT, HEAP_SLOT_BATTLE_SHEET);
  originX = x;
  x += toFIXED(40);
  x /= 2;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  maxBlend = slots[HEAP_SLOT_RAM_CODE - HEAP_SLOT_BATTLE_EFFECT];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  additive = slots[HEAP_SLOT_RAM_CODE_PAIRED - HEAP_SLOT_BATTLE_EFFECT];
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FLARE_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(
      RESOURCE_FLARE_IMAGE, work->sheet + IMPACT_BURST_FRAGMENT_PICTURES, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // Large sparks fly.
  for (i = 0; i != IMPACT_BURST_LARGE_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 256;
    s32 angle = Random16() & TRIG_ANGLE_MASK;

    work->particles[i].x = x;
    work->particles[i].y = y;
    work->particles[i].velocityX = (Trig_Sin(angle) * speed) >> 7;
    work->particles[i].velocityY = -((Trig_Cos(angle) * speed) >> 6);
    work->particles[i].variant = (Random16() & 15) + 16;
  }

  // Three fragments fly apart.
  for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].x = x;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].y = y;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityX =
        (Trig_Sin(i * (DEGtoANG(120))) * IMPACT_BURST_FRAGMENT_SPEED) >> 6;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityY =
        -((Trig_Cos(i * (DEGtoANG(120))) * IMPACT_BURST_FRAGMENT_SPEED) >> 5);
  }

  // Small sparks fly.
  for (i = 0; i != IMPACT_BURST_SMALL_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 32;
    s32 angle = Random16() & TRIG_ANGLE_MASK;

    PARTICLES[i].x = x;
    PARTICLES[i].y = y;
    PARTICLES[i].velocityX = (Trig_Sin(angle) * speed) >> 6;
    PARTICLES[i].velocityY = -((Trig_Cos(angle) * speed) >> 5);
    PARTICLES[i].variant = (Random16() & 15) + 20;
  }

  for (frame = 0; frame != IMPACT_BURST_FRAMES; frame++) {
    if (frame == 4)
      Sound_PlayFar(SE_CAST);
    if (frame == 32)
      Sound_PlayFar(SE_POWER_UP);

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
               FIXEDtoINT(x) - (width >> 1), FIXEDtoINT(y) - (height >> 1),
               width, height);
#else
      maxBlend(
          canvas, work->sheet + gHelmSplitter_ImpactBurstPictures[picture],
          FIXEDtoINT(x) -
              ((width = gHelmSplitter_ImpactBurstPictureWidths[picture]) >> 1),
          FIXEDtoINT(y) -
              ((height = gHelmSplitter_ImpactBurstPictureHeights[picture]) >>
               1),
          width, height);
#endif
    }

    for (i = 0; i != IMPACT_BURST_DRAWN_LARGE; i++) {
      particle = &work->particles[i];
      if (frame > i / 2 && particle->variant > 0) {
        s32 size;
        s32 drawX;
        s32 drawY;

        particle->variant--;
        Particle_AdvanceWithGravity2D(particle, 60, 0);
        size = particle->variant / IMPACT_BURST_LIFE_PER_SIZE + 3;
        drawX = FIXED_INTEGER_PART(particle->x);
        drawY = FIXED_INTEGER_PART(particle->y);
        EFFECT_DRAW_MOTE(additive, canvas, sheet, drawX, drawY, size);
      }
    }

    for (i = 0, particle = PARTICLES; i != IMPACT_BURST_DRAWN_SMALL;
         i++, particle++) {
      if (frame > 35 && particle->variant > 0) {
        s32 size;
        s32 drawX;
        s32 drawY;

        particle->variant--;
        Particle_AdvanceWithGravity2D(particle, 60, 0);
        size = particle->variant / IMPACT_BURST_LIFE_PER_SIZE + 1;
        drawX = FIXED_INTEGER_PART(particle->x);
        drawY = FIXED_INTEGER_PART(particle->y);
        EFFECT_DRAW_MOTE(additive, canvas, sheet, drawX, drawY, size);
      }
    }

    for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
      struct Particle *fragment =
          &work->particles[IMPACT_BURST_FIRST_FRAGMENT + i];

      if (frame >= 36 && frame <= 63) {
        s32 picture;
        s32 drawX;
        s32 drawY;

        Particle_AdvanceWithGravity2D(fragment, PARTICLE_DAMPING_NONE, 0);
        picture = (frame - 36) / 7;
        drawX = FIXED_INTEGER_PART(fragment->x);
        drawY = FIXED_INTEGER_PART(fragment->y);
        maxBlend(canvas,
                 work->sheet + picture * PICTURE_BYTES(IMPACT_BURST_FRAGMENT) +
                     IMPACT_BURST_FRAGMENT_PICTURES,
                 drawX - IMPACT_BURST_FRAGMENT_WIDTH / 2,
                 drawY - IMPACT_BURST_FRAGMENT_HEIGHT / 2,
                 IMPACT_BURST_FRAGMENT_WIDTH, IMPACT_BURST_FRAGMENT_HEIGHT);
      }
    }

    if (frame <= 35)
      BattleEffect_PlaceFormationObjects(formation, originX, y);
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  EFFECT_RELEASE_BLITTERS();
}

#undef HELM_SPLITTER_FRAMES
#undef HELM_SPLITTER_STRIKE_FRAME
#undef HELM_SPLITTER_STAR_PICTURES
#undef HELM_SPLITTER_STAR_SIZE
#undef HELM_SPLITTER_CLEARED_SLOTS
#undef HELM_SPLITTER_SPARKS
#undef HELM_SPLITTER_SPARK_SLOTS
#undef IMPACT_BURST_FRAGMENT_PICTURES
#undef IMPACT_BURST_FRAGMENT_WIDTH
#undef IMPACT_BURST_FRAGMENT_HEIGHT
#undef IMPACT_BURST_FRAMES
#undef IMPACT_BURST_FRAGMENTS
#undef IMPACT_BURST_DRAWN_LARGE
#undef IMPACT_BURST_DRAWN_SMALL
#undef IMPACT_BURST_LARGE_SPARKS
#undef IMPACT_BURST_SMALL_SPARKS
#undef IMPACT_BURST_FIRST_FRAGMENT
#undef IMPACT_BURST_FRAGMENT_SPEED
#undef IMPACT_BURST_LIFE_PER_SIZE
#undef FORMATION_GRID_COLUMNS
#undef FORMATION_GRID_SPACING
#undef FORMATION_TWELVE
#undef FORMATION_EIGHT
