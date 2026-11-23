/// @file
/// The healing motes of Ply, Wish, Cure, Cure Poison, Tonic and Psy Crystal.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define HEALING_SPARKS 512 ///< sparks in PARTICLES

/// Where the Djinni's picture is decoded in the work sheet, in bytes.
#define HEALING_DJINNI_PICTURE 4096
/// How far the Djinni sways to either side, in screen pixels.
#define HEALING_DJINNI_SWAY 20
/// Each target's motes: HEALING_MOTES for the stronger variant, half that for
/// the basic and twice for the strongest. A target's motes start HEALING_MOTES
/// after the last target's in PARTICLES, so the strongest variant's run on into
/// the next target's.
#define HEALING_MOTES 64
/// The frames each target's ring of Cure motes lags the last target's in its
/// turn.
#define HEALING_CURE_RING_LAG 40
/// A sparkle or mote is drawn at one of three sizes by its depth, from
/// HEALING_MOTE_DEPTH_NEAR, one size smaller every HEALING_MOTE_DEPTH_STEP
/// farther, its depth held below HEALING_MOTE_DEPTH_FAR.
#define HEALING_MOTE_DEPTH_NEAR 314
#define HEALING_MOTE_DEPTH_STEP 128
#define HEALING_MOTE_DEPTH_FAR 634
/// Eddy's bubble: BlueBeamSheet's first picture, a 24x24 blue bubble, unpacked
/// at HEALING_BUBBLE_SHEET and copied to HEALING_BUBBLE at every second pixel,
/// so that the canvas shown at twice its width draws it round.
#define HEALING_BUBBLE_SHEET 2048
#define HEALING_BUBBLE 8192
#define HEALING_BUBBLE_WIDTH (BLUE_BUBBLE_WIDTH / 2)
#define HEALING_BUBBLE_HEIGHT BLUE_BUBBLE_HEIGHT

/// The healing motes' modes (BattleEffect_RunHealingMotes), each named for the
/// first ability that plays it.
enum HealingMotesMode {
  HEALING_MODE_PLY,         ///< swirling sparkles, the canvas at its own width
  HEALING_MODE_WISH,        ///< motes tumbling in from all around
  HEALING_MODE_CURE,        ///< a ring of motes below each target
  HEALING_MODE_CURE_POISON, ///< sparkles that go out one by one
  HEALING_MODE_RESTORE,     ///< the same, turning on a second axis
  HEALING_MODE_TONIC,       ///< played exactly as HEALING_MODE_RESTORE
  HEALING_MODE_PSY_CRYSTAL, ///< motes in orange
  HEALING_MODE_SALT,        ///< sparkles thrown up from the feet, turning cyan
  HEALING_MODE_SPRING,      ///< Ply's sparkles, twice as many
  HEALING_MODE_BREATH,      ///< sparkles whirling fast in Jupiter's colours
  HEALING_MODE_ETHER,       ///< motes in Jupiter's colours
  HEALING_MODE_AURA,        ///< sparkles from a wide cloud, not turning
  HEALING_MODE_EDDY,        ///< blue bubbles
  HEALING_MODE_REVERSE_STAR ///< sparkles turning on the view's axis
};

/// Sparkles of light swirl around each target and close in on it, in the ice
/// shard's or ice block's colours, with the canvas shown at its own width and
/// centred on the first target.
///
/// Abilities: Ply, Ply Well, Pure Ply, Herb, Nut, Vial, Potion, Soothing Water,
/// Pixie, Faery, Weird Nymph, Succubus, Earnest Ply, Fizz.
void BattleEffect_RunPly(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_PLY);
}

/// Healing motes tumble in on each target from all around it.
///
/// Abilities: Wish, Wish Well, Pure Wish, Mist Potion, Elder Wood, Estre Wood,
/// Regenerate, Reflect, Spritz.
void BattleEffect_RunWish(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_WISH);
}

/// A ring of healing motes below each target closes in on it, in the blast
/// sheet's colours.
///
/// Abilities: Cure, Cure Well, Potent Cure, Corn, Flower, Worms.
void BattleEffect_RunCure(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_CURE);
}

/// After a Mercury Djinni appears, Ply's sparkles, twice as many, swirl in on
/// each target.
///
/// Abilities: Spring.
void BattleEffect_RunSpring(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_SPRING);
}

/// After a Jupiter Djinni appears, sparkles in the Jupiter Djinn's colours
/// whirl fast around each target and close in on it.
///
/// Abilities: Breath.
void BattleEffect_RunBreath(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_BREATH);
}

/// Sparkles twinkle around each target, four more a frame, and go out one by
/// one instead of closing in.
///
/// Abilities: Cure Poison, Restore.
void BattleEffect_RunCurePoison(struct BattleEffectParams *args) {
  if (args->variant == EFFECT_VARIANT_BASIC) {
    BattleEffect_RunHealingMotes(args, HEALING_MODE_CURE_POISON);
    return;
  }
  BattleEffect_RunHealingMotes(args, HEALING_MODE_RESTORE);
}

/// Plays the healing motes as Restore shows them; no effect table entry reaches
/// this one.
UNUSED void
BattleEffect_RunUnusedRestoreMotes(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_RESTORE);
}

/// Sparkles twinkle around each target, turning on two axes, and go out one by
/// one; the runner plays HEALING_MODE_TONIC exactly as HEALING_MODE_RESTORE.
///
/// Abilities: Tonic.
void BattleEffect_RunTonic(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_TONIC);
}

/// After a Venus Djinni appears, sparkles burst up and out from each target's
/// feet, their colours turning from the blast sheet's to cyan, and go out one
/// by one.
///
/// Abilities: Salt.
void BattleEffect_RunSalt(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_SALT);
}

/// Healing motes gather around each target in orange; with a prelude, a Mars
/// Djinni appears first.
///
/// Abilities: Psy Crystal, Adept Ring, Ember.
void BattleEffect_RunPsyCrystal(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_PSY_CRYSTAL);
}

/// After a Jupiter Djinni appears, motes in the Jupiter Djinn's colours swirl
/// in on each target.
///
/// Abilities: Ether.
void BattleEffect_RunEther(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_ETHER);
}

/// Sparkles from a wide cloud around each target close in on it without
/// turning, in orange.
///
/// Abilities: Aura, Healing Aura, Cool Aura, Manticore.
void BattleEffect_RunAura(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_AURA);
}

/// After a Mercury Djinni appears, blue bubbles swirl in on each target.
///
/// Abilities: Eddy.
void BattleEffect_RunEddy(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_EDDY);
}

/// Motes of light scattered around each target swirl in towards it.
///
/// Abilities: Reverse Star.
void BattleEffect_RunReverseStar(struct BattleEffectParams *args) {
  args->prelude = 0;
  args->variant = EFFECT_VARIANT_STRONGEST;
  BattleEffect_RunHealingMotes(args, HEALING_MODE_REVERSE_STAR);
}

/// Motes of light scattered around each target swirl in towards it, one target
/// after another, eight frames apart; a healing chime sounds as they gather and
/// the target glows.
void BattleEffect_RunHealingMotes(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frameCount;
  s32 member;
  u8 *sprites;
  s32 motesPerTarget;
  s32 canvasShiftX;
  s32 doubleWidth;
  DrawRectangle *draw;
  s32 frame;
  s32 i;
  u8 *resource;
  s32 origin[3];
  s32 target[3];
  struct ScreenPoint base;
  struct ScreenPoint screen;
  FIXED point[3];
  DrawRectangle blit[2];

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  canvasShiftX = 0;
  BattleEffect_StartCanvasLayer(0);
  if (mode == HEALING_MODE_REVERSE_STAR)
    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;

  // The prelude: the Djinni appears; origin receives where.
  if (args->prelude == 1) {
    s32 djinni;

    if (mode == HEALING_MODE_PSY_CRYSTAL)
      djinni = ELEMENT_MARS;
    else if (mode == HEALING_MODE_CURE || mode == HEALING_MODE_SALT)
      djinni = ELEMENT_VENUS;
    else if (mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER)
      djinni = ELEMENT_JUPITER;
    else
      djinni = ELEMENT_MERCURY;
    if (mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_PLY ||
        mode == HEALING_MODE_SPRING || mode == HEALING_MODE_BREATH ||
        mode == HEALING_MODE_ETHER)
      BattleEffect_RunDjinniEntrance(
          args, DJINNI_ENTRANCE(djinni, args->side, DJINNI_ANCHOR_OVER_TARGET),
          origin, target);
    else
      BattleEffect_RunDjinniEntrance(
          args, DJINNI_ENTRANCE(djinni, args->side, DJINNI_ANCHOR_TARGET_X),
          origin, target);
    args->variant = EFFECT_VARIANT_BASIC;
  }

  // Ply, Spring and Breath show the canvas at its own width, centred on the
  // first target; the rest show it at twice its width.
  if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_SPRING ||
      mode == HEALING_MODE_BREATH) {
    EffectPosition_ProjectScaledBattler(args->targets[0], &base);
    canvasShiftX = CANVAS_SIZE / 2 - base.x;
    REG_BG2X = canvasShiftX << 8;
    REG_BG2PA = Q_8_8(1);
    doubleWidth = 0;
  } else {
    doubleWidth = 1;
  }

  // The pictures, Eddy's bubble narrowed, and the colours.
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS, work->sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET,
                             work->sheet + HEALING_BUBBLE_SHEET, 1, 0);
  for (i = 0; i != PICTURE_BYTES(HEALING_BUBBLE); i++)
    work->sheet[HEALING_BUBBLE + i] = work->sheet[HEALING_BUBBLE_SHEET + i * 2];
  if (mode <= HEALING_MODE_WISH || mode == HEALING_MODE_CURE_POISON ||
      mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC ||
      mode == HEALING_MODE_SPRING || mode == HEALING_MODE_EDDY) {
    if (args->variant == EFFECT_VARIANT_BASIC)
      resource = Resource_GetTableEntry(RESOURCE_ICE_SHARD_SHEET);
    else
      resource = Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET);
  } else if (mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER) {
    resource = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
  } else if (mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_AURA ||
             mode == HEALING_MODE_REVERSE_STAR) {
    resource = Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B);
  } else {
    resource = Resource_GetTableEntry(RESOURCE_BLAST_SHEET);
  }
  Iwram_CopyWords((void *)BG_PLTT, resource, SHEET_PALETTE_BYTES);

  // The Djinni: its 40x40 picture on a canvas at its own width, the small
  // sheet's 20x40 one on a canvas at twice its width.
  if (doubleWidth == 0) {
    if (mode == HEALING_MODE_PSY_CRYSTAL)
      resource = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
    else if (mode == HEALING_MODE_CURE || mode == HEALING_MODE_SALT)
      resource = Resource_GetTableEntry(RESOURCE_VENUS_DJINN_SHEET);
    else if (mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER)
      resource = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
    else
      resource = Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SHEET);
  } else {
    if (mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_AURA ||
        mode == HEALING_MODE_REVERSE_STAR)
      resource = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET);
    else if (mode == HEALING_MODE_CURE || mode == HEALING_MODE_SALT)
      resource = Resource_GetTableEntry(RESOURCE_VENUS_DJINN_SMALL_SHEET);
    else if (mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER)
      resource = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SMALL_SHEET);
    else
      resource = Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
  }
  resource += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(resource, work->sheet + HEALING_DJINNI_PICTURE);
  // blit[0] keeps the brighter pixel and blit[1] adds; draw points at them.
  BattleEffect_FetchRectangleBlitters(args->side, draw = blit);

  // Scatter the motes; the mode decides how, and how long the effect is.
  if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_PSY_CRYSTAL ||
      mode == HEALING_MODE_SPRING || mode == HEALING_MODE_BREATH ||
      mode == HEALING_MODE_ETHER) {
    // Across and deep 64 pixels each way, 32-95 pixels up.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0x7f) + 64) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    if (mode == HEALING_MODE_BREATH)
      frameCount = 54;
    else if (mode == HEALING_MODE_SPRING)
      frameCount = 76;
    else
      frameCount = args->targetCount * 8 + 58;
  } else if (mode == HEALING_MODE_SALT) {
    // From the feet, 48 pixels up, thrown out and up.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];

      spark->y = toFIXED(48);
      spark->x = 0;
      spark->z = 0;
      spark->velocityX = ((Random16() & 0xff) - 127) << 8;
      spark->velocityY = -((Random16() & 0x7f) + 64) << 9;
      spark->velocityZ = ((Random16() & 0xff) - 127) << 8;
      spark->variant = 0;
    }
    frameCount = args->targetCount * 8 + 58;
  } else if (mode == HEALING_MODE_EDDY) {
    // As Ply's.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0x7f) + 64) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = args->targetCount * 8 + 58;
  } else if (mode == HEALING_MODE_WISH || mode == HEALING_MODE_REVERSE_STAR) {
    // 64 pixels each way.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = args->targetCount * 8 + 58;
  } else if (mode == HEALING_MODE_AURA) {
    // A wide cloud, 100 pixels each way.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];

      SET_POSITION(spark, (Random16() % 200 - 100) << 15,
                   (Random16() % 200 - 100) << 15,
                   (Random16() % 200 - 100) << 15);
      spark->variant = 0;
    }
    frameCount = args->targetCount * 8 + 46;
  } else if (mode == HEALING_MODE_CURE) {
    // A ring 32-95 pixels across, 50 pixels down, rising.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];
      s32 heading = Random16() & TRIG_ANGLE_MASK;
      s32 radius = (Random16() & 63) + 32;

      SET_POSITION(spark, Trig_Sin(heading) * radius, -toFIXED(50),
                   Trig_Cos(heading) * radius);
      spark->velocityY = ((Random16() & 31) + 32) << 13;
      spark->variant = 0;
    }
    frameCount = args->targetCount * 8 + 58;
  } else if (mode == HEALING_MODE_CURE_POISON) {
    // 64 pixels across and deep, 32 up and down.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 14,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = args->targetCount * 8 + 58;
  } else {
    // 64 pixels each way.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = args->targetCount * 8 + 58;
  }

  // Motes a target: 32 for variant 0, 128 for variant 2, else 64; mode 8
  // takes 128 and mode 12 16.
  motesPerTarget = HEALING_MOTES;
  if (args->variant == EFFECT_VARIANT_BASIC)
    motesPerTarget = HEALING_MOTES / 2;
  else if (args->variant == EFFECT_VARIANT_STRONGEST)
    motesPerTarget = HEALING_MOTES * 2;
  if (mode == HEALING_MODE_SPRING)
    motesPerTarget = HEALING_MOTES * 2;
  if (mode == HEALING_MODE_EDDY)
    motesPerTarget = HEALING_MOTES / 4;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 40)
      EFFECT_SHOW_DAMAGE(0);
    if (mode == HEALING_MODE_SALT && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource(RESOURCE_CYAN_PALETTE);

    // The Djinni sways, then from frame 32 rises two pixels a frame; for its
    // first four frames it is added over itself as well.
    if (args->prelude == 1) {
      if (doubleWidth == 0) {
        s32 x = FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 32)) *
                           HEALING_DJINNI_SWAY) +
                origin[0] + canvasShiftX - DJINNI_WIDTH / 2;
        s32 y =
            FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 4) + origin[1] - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + HEALING_DJINNI_PICTURE, x, y,
                DJINNI_WIDTH, DJINNI_HEIGHT);
        if (frame <= 3)
          draw[1](canvas, work->sheet + HEALING_DJINNI_PICTURE, x, y,
                  DJINNI_WIDTH, DJINNI_HEIGHT);
      } else {
        s32 x = FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 32)) *
                           (HEALING_DJINNI_SWAY / 2)) +
                origin[0] / 2 - SMALL_DJINNI_WIDTH / 2;
        s32 y =
            FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 4) + origin[1] - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + HEALING_DJINNI_PICTURE, x, y,
                SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        if (frame <= 3)
          draw[1](canvas, work->sheet + HEALING_DJINNI_PICTURE, x, y,
                  SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      }
    }

    for (member = 0; member != args->targetCount; member++) {
      struct MotionObject *object =
          BattleObject_GetFar(args->targets[member])->object;
      s32 start = member * 8;

      // The motes turn about a point 40 pixels above the target.
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      point[0] = object->x;
      point[1] = toFIXED(40);
      point[2] = object->z;
      Matrix_TranslateVector(point);
      if (frame == start + 20)
        Sound_PlayFar(SE_RECOVERY);
      if (mode == HEALING_MODE_SPRING) {
        if (frame == start + 41)
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, member, 32);
      } else if (mode == HEALING_MODE_AURA) {
        if (frame == start + 24)
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, member, 28);
      } else if (frame == start + 36) {
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, member, 28);
      }
      if (frame > start) {
        if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_PSY_CRYSTAL ||
            mode == HEALING_MODE_SALT || mode == HEALING_MODE_SPRING ||
            mode == HEALING_MODE_ETHER || mode == HEALING_MODE_EDDY) {
          Matrix_RotateY((frame - start) * (TRIG_TURN / 128));
        } else if (mode == HEALING_MODE_BREATH) {
          Matrix_RotateY((frame - start) * (TRIG_TURN / 32));
        } else if (mode == HEALING_MODE_AURA) {
          // Aura's motes close in without turning.
        } else if (mode == HEALING_MODE_REVERSE_STAR) {
          Matrix_RotateZ(frame * (TRIG_TURN / 128));
        } else if (mode == HEALING_MODE_WISH) {
          Matrix_RotateX(frame * (TRIG_TURN / 128));
          Matrix_RotateZ(frame * (TRIG_TURN / 128));
        } else if (mode == HEALING_MODE_CURE || mode == HEALING_MODE_EDDY) {
          Matrix_RotateY((frame - member * HEALING_CURE_RING_LAG) *
                         (TRIG_TURN / 128));
        } else if (mode == HEALING_MODE_CURE_POISON) {
          Matrix_RotateY((frame - start) * (TRIG_TURN / 128));
        } else {
          Matrix_RotateY((frame - start) * (TRIG_TURN / 128));
          Matrix_RotateX((frame - start) * (TRIG_TURN / 128));
        }

        // Four motes appear each frame; Cure Poison's, Restore's and Tonic's
        // go out one by one from 32 frames on, Salt's from 64.
        for (i = 0; i != motesPerTarget; i++) {
          struct Particle *spark = &PARTICLES[member * HEALING_MOTES + i];
          s32 endFrame;

          if (mode == HEALING_MODE_CURE_POISON ||
              mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC)
            endFrame = start + i / 2 + 32;
          else if (mode == HEALING_MODE_SALT)
            endFrame = start + i / 2 + 64;
          else
            endFrame = 65536;
          if (frame > start + i / 4 && frame < endFrame) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            // Measured at three quarters, so the motes close in a third
            // faster than 1 / distance of the way.
            distance = distance * 48 / 64;
            if (distance != 0) {
              s32 size;

              EffectPosition_ProjectPoint((s32 *)spark, &screen);
              if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_SPRING ||
                  mode == HEALING_MODE_BREATH)
                screen.x += canvasShiftX;
              else
                screen.x >>= 1;
              screen.y += 16;

              // Aura's and Reverse Star's sizes run one larger; Ply's,
              // Spring's, Breath's and Salt's sparkles turn through the nine
              // sparkle pictures instead.
              CLAMP(screen.depth, HEALING_MOTE_DEPTH_NEAR,
                    HEALING_MOTE_DEPTH_FAR);
              size = 3 - (screen.depth - HEALING_MOTE_DEPTH_NEAR) /
                             HEALING_MOTE_DEPTH_STEP;
              if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_SPRING ||
                  mode == HEALING_MODE_BREATH)
                size = (i * 4 + frame) % 9;
              else if (mode == HEALING_MODE_AURA ||
                       mode == HEALING_MODE_REVERSE_STAR)
                size = 4 - (screen.depth - HEALING_MOTE_DEPTH_NEAR) /
                               HEALING_MOTE_DEPTH_STEP;
              else if (mode == HEALING_MODE_SALT)
                size = (i * 4 + frame) % 9;
              if (mode == HEALING_MODE_PLY ||
                  mode == HEALING_MODE_CURE_POISON ||
                  mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC ||
                  mode == HEALING_MODE_SALT || mode == HEALING_MODE_SPRING ||
                  mode == HEALING_MODE_BREATH || mode == HEALING_MODE_AURA ||
                  mode == HEALING_MODE_REVERSE_STAR) {
                u8 *cell = work->sheet + gBattleEffect_SparklePictures[size];
                u32 edge = gBattleEffect_SparkleSizes[size];
                u32 half = edge >> 1;

                draw[1](canvas, cell, screen.x - half, screen.y - half, edge,
                        edge);
              } else if (mode == HEALING_MODE_EDDY) {
                blit[0](canvas, work->sheet + HEALING_BUBBLE,
                        screen.x - HEALING_BUBBLE_WIDTH / 2,
                        screen.y - HEALING_BUBBLE_HEIGHT / 2,
                        HEALING_BUBBLE_WIDTH, HEALING_BUBBLE_HEIGHT);
              } else {
                EFFECT_DRAW_MOTE(draw[1], canvas, sprites, screen.x, screen.y,
                                 size);
              }

              // Close in by 1 / distance of the way a frame. Cure Poison's,
              // Restore's and Tonic's stay where they are until they go out,
              // and Salt's fly on.
              if (mode <= HEALING_MODE_CURE ||
                  mode == HEALING_MODE_PSY_CRYSTAL ||
                  mode == HEALING_MODE_SPRING || mode == HEALING_MODE_BREATH ||
                  mode == HEALING_MODE_ETHER || mode == HEALING_MODE_AURA ||
                  mode == HEALING_MODE_REVERSE_STAR ||
                  mode == HEALING_MODE_EDDY) {
                spark->x -= spark->x / distance;
                spark->y -= spark->y / distance;
                spark->z -= spark->z / distance;
              }
              if (mode == HEALING_MODE_SALT) {
                spark->x += spark->velocityX;
                spark->y += spark->velocityY;
                spark->z += spark->velocityZ;
              }
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}
