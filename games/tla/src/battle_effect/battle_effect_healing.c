/// @file
/// The healing motes of Ply, Wish, Cure, Cure Poison, Tonic and Psy Crystal.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The healing motes' modes (BattleEffect_RunHealingMotes), each named for the
/// first ability that plays it.
enum HealingMotesMode {
  HEALING_MODE_PLY,         ///< swirling sparkles, the canvas at its own width
  HEALING_MODE_WISH,        ///< motes tumbling in from all around
  HEALING_MODE_CURE,        ///< a ring of motes below each target
  HEALING_MODE_CURE_POISON, ///< sparkles that go out one by one
  HEALING_MODE_RESTORE,     ///< the same, turning on a second axis
  HEALING_MODE_TONIC,
  HEALING_MODE_PSY_CRYSTAL, ///< motes in the Mars Djinni palette
  HEALING_MODE_SALT,
  HEALING_MODE_SPRING,
  HEALING_MODE_BREATH,
  HEALING_MODE_ETHER,
  HEALING_MODE_AURA,
  HEALING_MODE_EDDY,
  HEALING_MODE_REVERSE_STAR
};

/// Abilities: Ply, Ply Well, Pure Ply, Herb,
/// Nut, Vial, Potion,
/// Soothing Water, Pixie, Faery, Weird Nymph, Succubus,
/// Earnest Ply, Fizz.
void BattleEffect_RunPly(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_PLY);
}

/// Abilities: Wish, Wish Well, Pure Wish, Mist Potion, Elder
/// Wood, Estre Wood, Regenerate, Reflect, Spritz.
void BattleEffect_RunWish(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_WISH);
}

/// Abilities: Cure, Cure Well, Potent Cure, Corn, Flower,
/// Worms.
void BattleEffect_RunCure(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_CURE);
}

/// Abilities: Spring.
void BattleEffect_RunSpring(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_SPRING);
}

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

/// Plays the healing motes as Restore shows them; no ability's effect names
/// this one.
void BattleEffect_RunUnusedRestoreMotes(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_RESTORE);
}

/// Abilities: Tonic.
void BattleEffect_RunTonic(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_TONIC);
}

/// Abilities: Salt.
void BattleEffect_RunSalt(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_SALT);
}

/// Abilities: Psy Crystal, Adept Ring, Ember.
void BattleEffect_RunPsyCrystal(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_PSY_CRYSTAL);
}

/// Abilities: Ether.
void BattleEffect_RunEther(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_ETHER);
}

/// Abilities: Aura, Healing Aura, Cool Aura, Manticore.
void BattleEffect_RunAura(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_AURA);
}

/// Abilities: Eddy.
void BattleEffect_RunEddy(struct BattleEffectParams *args) {
  BattleEffect_RunHealingMotes(args, HEALING_MODE_EDDY);
}

/// Motes of light scattered around each target swirl in towards it.
///
/// Abilities: Reverse Star.
void BattleEffect_RunReverseStar(struct BattleEffectParams *args) {
  args->prelude = 0;
  args->variant = 2;
  BattleEffect_RunHealingMotes(args, HEALING_MODE_REVERSE_STAR);
}

#define SPARKS ((struct Particle *)Ram_MapCellBuffer)
#define SPARKS_PER_MEMBER 64
#define BEAM_SHEET 0x800    ///< the blue beam as unpacked, two bytes a pixel
#define BEAM_BYTES 0x2000   ///< the beam packed to a byte a pixel
#define DJINNI_SHEET 0x1000 ///< the Djinni picture

// Motes of light gather around each target in turn, eight frames apart,
// turning as they close in on its middle; a healing chime sounds as they
// gather and the target glows.

#define HEALING_SPARKS 512 ///< sparks in PARTICLES

/// Motes of light scattered around each target swirl in towards it, one target
/// after another.
void BattleEffect_RunHealingMotes(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 total;
  s32 member;
  u8 *sprites;
  s32 limit;
  s32 shift;
  s32 narrow;
  DrawRectangle *draw;
  s32 frame;
  s32 i;
  u8 *palette;
  s32 origin[3];
  s32 target[3];
  struct ScreenPoint base;
  struct ScreenPoint screen;
  FIXED point[3];
  DrawRectangle blit[2];

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  shift = 0;
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
    args->variant = 0;
  }

  // Modes 0, 8 and 9 show the canvas at full width, the first target at
  // the middle (64); the rest keep it twice as wide (narrow).
  if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_SPRING ||
      mode == HEALING_MODE_BREATH) {
    EffectPosition_ProjectScaledBattler(args->targets[0], &base);
    shift = 64 - base.x;
    REG_BG2X = shift << 8;
    REG_BG2PA = Q_8_8(1);
    narrow = 0;
  } else {
    narrow = 1;
  }

  // The pictures, the beam packed to a byte a pixel, and the colors.
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS, work->sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET, work->sheet + BEAM_SHEET,
                             1, 0);
  for (i = 0; i != 288; i++)
    work->sheet[BEAM_BYTES + i] = work->sheet[BEAM_SHEET + i * 2];
  if (mode <= HEALING_MODE_WISH || mode == HEALING_MODE_CURE_POISON ||
      mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC ||
      mode == HEALING_MODE_SPRING || mode == HEALING_MODE_EDDY) {
    if (args->variant == EFFECT_VARIANT_BASIC)
      palette = Resource_GetTableEntry(RESOURCE_ICE_SHARD_SHEET);
    else
      palette = Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET);
  } else if (mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER) {
    palette = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
  } else if (mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_AURA ||
             mode == HEALING_MODE_REVERSE_STAR) {
    palette = Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B);
  } else {
    palette = Resource_GetTableEntry(RESOURCE_BLAST_SHEET);
  }
  Iwram_CopyWords((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);

  // The Djinni: 40x40 at full width, a 20x40 picture when narrow.
  if (narrow == 0) {
    if (mode == HEALING_MODE_PSY_CRYSTAL)
      palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
    else if (mode == HEALING_MODE_CURE || mode == HEALING_MODE_SALT)
      palette = Resource_GetTableEntry(RESOURCE_VENUS_DJINN_SHEET);
    else if (mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER)
      palette = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
    else
      palette = Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SHEET);
  } else {
    if (mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_AURA ||
        mode == HEALING_MODE_REVERSE_STAR)
      palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET);
    else if (mode == HEALING_MODE_CURE || mode == HEALING_MODE_SALT)
      palette = Resource_GetTableEntry(RESOURCE_VENUS_DJINN_SMALL_SHEET);
    else if (mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER)
      palette = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SMALL_SHEET);
    else
      palette = Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
  }
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, work->sheet + DJINNI_SHEET);
  BattleEffect_FetchRectangleBlitters(args->side,
                                      draw = blit); // [0] MAX, [1] ADDITIVE

  // Scatter the motes; the mode decides how, and how long the effect is.
  if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_PSY_CRYSTAL ||
      mode == HEALING_MODE_SPRING || mode == HEALING_MODE_BREATH ||
      mode == HEALING_MODE_ETHER) {
    // Across and deep 64 pixels each way, 32-95 pixels up.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &SPARKS[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0x7f) + 64) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    if (mode == HEALING_MODE_BREATH)
      total = 54;
    else if (mode == HEALING_MODE_SPRING)
      total = 76;
    else
      total = args->targetCount * 8 + 58;
  } else if (mode == HEALING_MODE_SALT) {
    // From the feet, 48 pixels up, thrown out and up.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &SPARKS[i];

      spark->y = toFIXED(48);
      spark->x = 0;
      spark->z = 0;
      spark->velocityX = ((Random16() & 0xff) - 127) << 8;
      spark->velocityY = -((Random16() & 0x7f) + 64) << 9;
      spark->velocityZ = ((Random16() & 0xff) - 127) << 8;
      spark->variant = 0;
    }
    total = args->targetCount * 8 + 58;
  } else if (mode == HEALING_MODE_EDDY) {
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &SPARKS[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0x7f) + 64) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    total = args->targetCount * 8 + 58;
  } else if (mode == HEALING_MODE_WISH || mode == HEALING_MODE_REVERSE_STAR) {
    // 64 pixels each way.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &SPARKS[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    total = args->targetCount * 8 + 58;
  } else if (mode == HEALING_MODE_AURA) {
    // A wide cloud, 100 pixels each way.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &SPARKS[i];

      SET_POSITION(spark, (Random16() % 200 - 100) << 15,
                   (Random16() % 200 - 100) << 15,
                   (Random16() % 200 - 100) << 15);
      spark->variant = 0;
    }
    total = args->targetCount * 8 + 46;
  } else if (mode == HEALING_MODE_CURE) {
    // A ring 32-95 pixels across, 50 pixels down, rising.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &SPARKS[i];
      s32 heading = Random16() & TRIG_ANGLE_MASK;
      s32 radius = (Random16() & 63) + 32;

      SET_POSITION(spark, Trig_Sin(heading) * radius, -toFIXED(50),
                   Trig_Cos(heading) * radius);
      spark->velocityY = ((Random16() & 31) + 32) << 13;
      spark->variant = 0;
    }
    total = args->targetCount * 8 + 58;
  } else if (mode == HEALING_MODE_CURE_POISON) {
    // 64 pixels across and deep, 32 up and down.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &SPARKS[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 14,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    total = args->targetCount * 8 + 58;
  } else {
    // 64 pixels each way.
    for (i = 0; i != HEALING_SPARKS; i++) {
      struct Particle *spark = &SPARKS[i];

      SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    total = args->targetCount * 8 + 58;
  }

  // Motes a target: 32 for variant 0, 128 for variant 2, else 64; mode 8
  // takes 128 and mode 12 16.
  limit = 64;
  if (args->variant == EFFECT_VARIANT_BASIC)
    limit = 32;
  else if (args->variant == EFFECT_VARIANT_STRONGEST)
    limit = 128;
  if (mode == HEALING_MODE_SPRING)
    limit = 128;
  if (mode == HEALING_MODE_EDDY)
    limit = 16;
  work->transferMode =
      CANVAS_COPY_AND_FADE; // each transfer fades the canvas: a trail
  work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 40)
      EFFECT_SHOW_DAMAGE(0);
    if (mode == HEALING_MODE_SALT && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource(RESOURCE_CYAN_PALETTE);

    // The Djinni sways, and after frame 32 rises two pixels a frame; for
    // the first four frames it is drawn ADDITIVE as well.
    if (args->prelude == 1) {
      if (narrow == 0) {
        s32 x =
            (FIXEDtoINT(Trig_Sin(frame << 11) * 20)) + origin[0] + shift - 20;
        s32 y = (FIXEDtoINT(Trig_Cos(frame << 11) * 4)) + origin[1] - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + DJINNI_SHEET, x, y, DJINNI_WIDTH,
                DJINNI_HEIGHT);
        if (frame <= 3)
          draw[1](canvas, work->sheet + DJINNI_SHEET, x, y, DJINNI_WIDTH,
                  DJINNI_HEIGHT);
      } else {
        s32 x = (FIXEDtoINT(Trig_Sin(frame << 11) * 10)) + origin[0] / 2 - 10;
        s32 y = (FIXEDtoINT(Trig_Cos(frame << 11) * 4)) + origin[1] - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + DJINNI_SHEET, x, y, SMALL_DJINNI_WIDTH,
                SMALL_DJINNI_HEIGHT);
        if (frame <= 3)
          draw[1](canvas, work->sheet + DJINNI_SHEET, x, y, SMALL_DJINNI_WIDTH,
                  SMALL_DJINNI_HEIGHT);
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
          Matrix_RotateY((frame - start) << 9);
        } else if (mode == HEALING_MODE_BREATH) {
          Matrix_RotateY((frame - start) << 11);
        } else if (mode == HEALING_MODE_AURA) {
        } else if (mode == HEALING_MODE_REVERSE_STAR) {
          Matrix_RotateZ(frame << 9);
        } else if (mode == HEALING_MODE_WISH) {
          Matrix_RotateX(frame << 9);
          Matrix_RotateZ(frame << 9);
        } else if (mode == HEALING_MODE_CURE || mode == HEALING_MODE_EDDY) {
          Matrix_RotateY((frame - member * 40) << 9);
        } else if (mode == HEALING_MODE_CURE_POISON) {
          Matrix_RotateY((frame - start) << 9);
        } else {
          Matrix_RotateY((frame - start) << 9);
          Matrix_RotateX((frame - start) << 9);
        }

        // Four motes come out a frame; in modes 3-5 each fades at
        // start + i / 2 + 32, in mode 7 at start + i / 2 + 64.
        for (i = 0; i != limit; i++) {
          struct Particle *spark = &SPARKS[member * SPARKS_PER_MEMBER + i];
          s32 end;

          if (mode == HEALING_MODE_CURE_POISON ||
              mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC)
            end = start + i / 2 + 32;
          else if (mode == HEALING_MODE_SALT)
            end = start + i / 2 + 64;
          else
            end = 65536;
          if (frame > start + i / 4 && frame < end) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            distance = distance * 48 / DAMPING_SCALE;
            if (distance != 0) {
              s32 size;

              EffectPosition_ProjectPoint((s32 *)spark, &screen);
              if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_SPRING ||
                  mode == HEALING_MODE_BREATH)
                screen.x += shift;
              else
                screen.x >>= 1;
              screen.y += 16;

              // Depth 314-634 gives sizes 3 down to 1; some modes cycle
              // through the nine puffs instead.
              CLAMP(screen.depth, 314, 634);
              size = 3 - (screen.depth - 314) / 128;
              if (mode == HEALING_MODE_PLY || mode == HEALING_MODE_SPRING ||
                  mode == HEALING_MODE_BREATH)
                size = (i * 4 + frame) % 9;
              else if (mode == HEALING_MODE_AURA ||
                       mode == HEALING_MODE_REVERSE_STAR)
                size = 4 - (screen.depth - 314) / 128;
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
                blit[0](canvas, work->sheet + BEAM_BYTES, screen.x - 6,
                        screen.y - 12, 12, 24);
              } else {
                draw[1](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                        screen.x - size / 2, screen.y - size, size, size * 2);
              }

              // Close in by 1/distance of the way a frame.
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
