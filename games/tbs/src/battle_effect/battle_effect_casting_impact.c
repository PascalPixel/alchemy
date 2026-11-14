/// @file
/// The casting impacts, the Djinn attacks and the enemy abilities that open
/// the same way: sparks close in on the caster, a streak shoots at the target
/// and a ring of light bursts over it in a spray of sparks, and then each
/// impact plays its own effect over the target (enum CastingImpact).
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Steps background colours 0 to 63 one level towards a palette resource.
void BattleEffect_StepPaletteToResource(s32 resourceId) {
  u16 target[64];
  u16 *color = (u16 *)BG_PLTT;
  s32 i;

  Iwram_CopyWords(target, Resource_GetTableEntry(resourceId), 128);
  target[0] = 0;
  for (i = 0; i != 64; i++, color++) {
    s32 red = *color & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 blue = (*color >> 10) & 0x1f;
    s32 targetRed = target[i] & 0x1f;
    s32 targetGreen = (target[i] >> 5) & 0x1f;
    s32 targetBlue = (target[i] >> 10) & 0x1f;

    if (red < targetRed)
      red++;
    else if (red > targetRed)
      red--;
    if (green < targetGreen)
      green++;
    else if (green > targetGreen)
      green--;
    if (blue < targetBlue)
      blue++;
    else if (blue > targetBlue)
      blue--;
    target[i] = RGB(red, green, blue);
  }
  Iwram_CopyWords((u16 *)BG_PLTT, target, 128);
}

/// Clears the prelude image, stops its tasks and releases its blitters
/// before the next effect takes over.
static inline void CastingImpact_EndPrelude(struct BattleEffectWork *work,
                                            void *canvas) {
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  work->args->prelude = 0;
  Task_Remove(Palette_StepFadeTransfer);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
}

/// The casting impacts, by effect kind less EFFECT_RANGE_CASTING_IMPACT: each
/// is named for the first ability whose gAbilityBattleEffects entry selects it;
/// the four no ability selects are named UNUSED_ and for the effect they
/// hand off to.
enum CastingImpact {
  CASTING_IMPACT_IMPACT_BANE = 0,      ///< smoke
  CASTING_IMPACT_IMPACT_SMOG,          ///< smoke
  CASTING_IMPACT_IMPACT_MIST,          ///< smoke
  CASTING_IMPACT_IMPACT_FEVER,         ///< smoke
  CASTING_IMPACT_IMPACT_FLINT,         ///< nothing
  CASTING_IMPACT_IMPACT_SCORCH,        ///< flash bursts rise from the target
  CASTING_IMPACT_IMPACT_SQUALL,        ///< two bolts strike among glints
  CASTING_IMPACT_IMPACT_SAP,           ///< motes fly back to the caster
  CASTING_IMPACT_IMPACT_TERRA_STRIKE,  ///< a rock spire grows and sinks
  CASTING_IMPACT_IMPACT_POISON_CLOUD,  ///< smoke
  CASTING_IMPACT_IMPACT_DEADLY_POISON, ///< smoke
  CASTING_IMPACT_IMPACT_MORTAL_DANGER, ///< also Death Size; the reaper swings
                                       ///< its scythe
  CASTING_IMPACT_IMPACT_BAD_OMEN,      ///< skulls rise from the target
  CASTING_IMPACT_IMPACT_LIFE_NOURISH,  ///< motes fly back to the caster
  CASTING_IMPACT_IMPACT_AQUA_SOCK,     ///< a blue flame column falls onto the
                                       ///< target
  CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS, ///< hands off
  CASTING_IMPACT_IMPACT_FROST_BITE,       ///< ice chips converge on the target
  CASTING_IMPACT_IMPACT_UNUSED_GATHERING, ///< hands off
  CASTING_IMPACT_IMPACT_LIFE_LEECH,       ///< motes fly back to the caster
  CASTING_IMPACT_IMPACT_PSY_LEECH,        ///< motes fly back to the caster
  CASTING_IMPACT_IMPACT_BROIL,            ///< ember streaks spread sideways
  CASTING_IMPACT_IMPACT_MELTDOWN,    ///< nothing; hands to the stat reduction
  CASTING_IMPACT_IMPACT_HEAT_MIRAGE, ///< also Heat Flash; smoke
  CASTING_IMPACT_IMPACT_BARRAGE,     ///< flash bursts rise from the target
  CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME, ///< hands off
  CASTING_IMPACT_IMPACT_ACID_BATH,           ///< smoke
  CASTING_IMPACT_IMPACT_UNUSED_COMETS,       ///< hands off
  CASTING_IMPACT_IMPACT_STUN_VOLTAGE,        ///< two bolts strike among glints
  CASTING_IMPACT_IMPACT_BLINDING_SMOG,       ///< smoke
  CASTING_IMPACT_IMPACT_MURK,                ///< smoke
  CASTING_IMPACT_IMPACT_CYCLONE_SLASH,  ///< a tornado stands over the target
  CASTING_IMPACT_IMPACT_PSYPHON_SEAL,   ///< a rune opens over the target
  CASTING_IMPACT_IMPACT_ASURA,          ///< a masked swordsman slashes
  CASTING_IMPACT_IMPACT_FORCIBLE_ARM,   ///< a flame falls diagonally
  CASTING_IMPACT_IMPACT_TRUNCHEON_FIST, ///< a flame falls diagonally
  CASTING_IMPACT_IMPACT_SEVERE_BLOW,    ///< hands off
};

/// The impact orb, 20x48 pictures packed at half width, after the six 48x72
/// pictures over the caster.
#define CASTING_IMPACT_ORB_PICTURES (6 * 48 * 72)
/// The Djinni's 20x40 picture, in Ram_MapGridCellBuffer after the impact's own.
#define CASTING_IMPACT_DJINNI_PICTURE 15446
#define CASTING_IMPACT_GATHER_FRAMES 32 ///< frames the sparks gather
#define CASTING_IMPACT_SPARKS 64        ///< sparks in work->particles
/// The impact pictures' particles, in Ram_MapGridCellBuffer after the Djinni.
#define CASTING_IMPACT_IMPACT_PARTICLES                                        \
  ((struct Particle *)(Ram_MapGridCellBuffer + 16384))
#define CASTING_IMPACT_IMPACT_PARTICLE_COUNT 64

/// Asura's sheet: the masked swordsman with his sword low, then swung up, then
/// the red crescent his stroke leaves.
#define ASURA_SHEET_SWUNG (80 * 96)
#define ASURA_SHEET_CRESCENT (ASURA_SHEET_SWUNG + 80 * 104)

/// Where Mortal Danger draws `picture`, one of the reaper's seven pictures
/// (REAPER_PICTURE_X), for the casting `side`.
#define CASTING_IMPACT_PICTURE_X(side, picture)                                \
  gCastingImpact_PictureX[(side)][(picture)]

/// Gathers sparks around the caster, launches the Djinni and draws the
/// impact on the first target.
///
/// Abilities: Bane; Smog; Mist; Fever; Flint; Scorch; Squall; Sap; Terra
/// Strike; Poison Cloud; Deadly Poison; Mortal Danger, Death Size; Bad Omen;
/// Life Nourish; Aqua Sock; Frost Bite; Life Leech; Psy Leech; Broil; Meltdown;
/// Heat Mirage, Heat Flash; Barrage; Acid Bath; Stun Voltage; Blinding Smog;
/// Murk; Cyclone Slash; Psyphon Seal; Asura; Forcible Arm; Truncheon Fist;
/// Severe Blow.
void BattleEffect_RunCastingImpact(struct BattleEffectParams *args, s32 mode) {
  void **slots;
  void **slot;
  s32 pictureOffset;
  struct ScreenPoint casterPos;
  struct ScreenPoint targetPos;
  struct FixedPointPosition djinnPos;
  s32 phase;
  s32 i;
  s32 *scanline;
  s32 n;
  s32 djinnX;
  DrawRectangle *slotDraw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  u8 *sprites;
  s32 scrollPos;
  s32 scrollSpeed;
  s32 duration;
  struct FixedPointPosition *position;
  struct Actor *casterActor;
  struct Particle *particle;
  s32 pairX;
  s32 stripX;
  struct FixedPointPosition velocity;
  struct ScreenPoint sparkScreen;
  BlitterPair blitters;
  struct ScreenPoint projected;
  u8 *pictures;

  slots = gHeapSlots + HEAP_SLOT_BATTLE_EFFECT;
  slot = slots;
  work = (struct BattleEffectWork *)*slot++;
  canvas = *slot;
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  sprites = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  pictures = Ram_MapGridCellBuffer;
  work->args = args;
  if (mode == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
      mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE ||
      mode == CASTING_IMPACT_IMPACT_ASURA) {
    BattleEffect_StartTiledCanvas(0);
  } else {
    BattleEffect_StartCanvasLayer(0);
  }
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // Setup: the pictures.
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_STREAK_SHEET, work, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, pictures, 1, 0);
  BattleEffect_PackTileRows(pictures, work->sheet + CASTING_IMPACT_ORB_PICTURES,
                            40, 288);
  if (mode == CASTING_IMPACT_IMPACT_SCORCH ||
      mode == CASTING_IMPACT_IMPACT_BARRAGE) {
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, pictures, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_BAD_OMEN) {
    Resource_LoadAndDecompress(RESOURCE_SKULL_SHEET, pictures, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_SQUALL ||
             mode == CASTING_IMPACT_IMPACT_STUN_VOLTAGE) {
    Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, pictures, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET,
                               pictures + GLINT_SET_BYTES, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
    Resource_LoadAndDecompress(RESOURCE_RUNE_SHEET, pictures, 1, 1);
  } else if (mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
    Resource_LoadAndDecompress(RESOURCE_ROCK_SPIRE_SHEET, pictures, 1, 1);
  } else if (mode == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
    Resource_LoadAndDecompress(RESOURCE_BLUE_FLAME_SHEET, pictures, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_CYCLONE_SLASH) {
    Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, pictures, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_FROST_BITE) {
    Resource_LoadAndDecompress(RESOURCE_ICE_CHIP_SHEET, pictures, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_BROIL) {
    Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, pictures, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
             mode == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
    Resource_LoadAndDecompress(RESOURCE_FLAME_SHEET_A, pictures, 1, 0);
  } else if (mode != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
             mode != CASTING_IMPACT_IMPACT_ASURA) {
    Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET, pictures, 1, 0);
  }
  // The Djinni of the impact's element.
  switch (mode) {
  case CASTING_IMPACT_IMPACT_BANE:
  case CASTING_IMPACT_IMPACT_FLINT:
  case CASTING_IMPACT_IMPACT_SAP:
  case CASTING_IMPACT_IMPACT_TERRA_STRIKE:
  case CASTING_IMPACT_IMPACT_POISON_CLOUD:
  case CASTING_IMPACT_IMPACT_DEADLY_POISON:
  case CASTING_IMPACT_IMPACT_MORTAL_DANGER:
  case CASTING_IMPACT_IMPACT_BAD_OMEN:
  case CASTING_IMPACT_IMPACT_LIFE_NOURISH:
  case CASTING_IMPACT_IMPACT_FORCIBLE_ARM:
    Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
                               pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
    break;
  case CASTING_IMPACT_IMPACT_MIST:
  case CASTING_IMPACT_IMPACT_AQUA_SOCK:
  case CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS:
  case CASTING_IMPACT_IMPACT_FROST_BITE:
  case CASTING_IMPACT_IMPACT_UNUSED_GATHERING:
  case CASTING_IMPACT_IMPACT_LIFE_LEECH:
  case CASTING_IMPACT_IMPACT_PSY_LEECH:
    Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
                               pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
    break;
  case CASTING_IMPACT_IMPACT_FEVER:
  case CASTING_IMPACT_IMPACT_SCORCH:
  case CASTING_IMPACT_IMPACT_BROIL:
  case CASTING_IMPACT_IMPACT_MELTDOWN:
  case CASTING_IMPACT_IMPACT_HEAT_MIRAGE:
  case CASTING_IMPACT_IMPACT_BARRAGE:
  case CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME:
  case CASTING_IMPACT_IMPACT_ACID_BATH:
  case CASTING_IMPACT_IMPACT_TRUNCHEON_FIST:
  case CASTING_IMPACT_IMPACT_SEVERE_BLOW:
    Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET,
                               pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
    break;
  case CASTING_IMPACT_IMPACT_SMOG:
  case CASTING_IMPACT_IMPACT_SQUALL:
  case CASTING_IMPACT_IMPACT_UNUSED_COMETS:
  case CASTING_IMPACT_IMPACT_STUN_VOLTAGE:
  case CASTING_IMPACT_IMPACT_BLINDING_SMOG:
  case CASTING_IMPACT_IMPACT_MURK:
  case CASTING_IMPACT_IMPACT_CYCLONE_SLASH:
  case CASTING_IMPACT_IMPACT_PSYPHON_SEAL:
  case CASTING_IMPACT_IMPACT_ASURA:
    Resource_LoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET,
                               pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
    break;
  // Unused: impact mode 100, which this game never dispatches
  // (BattleEffect_RunByIdRange passes only the 36 impacts); The Lost Age's
  // dispatcher reaches it, and its twin of this routine loads the same Mercury
  // sheet for it.
  case EFFECT_RANGE_CASTING_IMPACT:
    Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
                               pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
    break;
  }
  // Trails: Bad Omen's last longer than the others'.
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (mode == CASTING_IMPACT_IMPACT_BAD_OMEN) {
    work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  } else {
    work->transferValue = CANVAS_FADE_HALF;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  Projection_ScaledUnit(work->args->targets[0], &targetPos);
  Projection_ScaledUnit(work->args->user, &casterPos);
  BattleEffect_FetchRectangleBlitters(work->args->side, blitters);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  casterActor = GetBattleObjectSlotFar(work->args->user)->actor;

  for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
    particle = &work->particles[i];
    SET_POSITION(particle, (Random16() & 63) + 32, 0, 0);
    SET_VELOCITY(particle, Random16() & TRIG_ANGLE_MASK,
                 Random16() & TRIG_ANGLE_MASK, Random16() & TRIG_ANGLE_MASK);
  }
  // Stop the caster and place the Djinni overhead.
  Actor_SetAnimationSpeedFar(casterActor, 0);
  position = &djinnPos;
  SET_POSITION(position, casterActor->x.fixed, casterActor->y.fixed + px(80),
               casterActor->z.fixed);
  {
    s32 savedVelocityX = casterActor->velocityX;
    s32 savedVelocityY = casterActor->velocityY;
    s32 savedVelocityZ = casterActor->velocityZ;
    s32 savedAcceleration = casterActor->acceleration;
    s32 savedVerticalStrength = casterActor->gravity;

    SET_VELOCITY(casterActor, 0, 0, 0);
    casterActor->acceleration = 0;
    casterActor->gravity = 0;
    Projection_ScaledUnit(work->args->user, &casterPos);
    casterPos.x = casterPos.x / 2;
    Sound_PlayFar(SE_POWER_UP);

    // Phase 1, the gathering (CASTING_IMPACT_GATHER_FRAMES).
    for (frame = 0; frame != CASTING_IMPACT_GATHER_FRAMES; frame++) {
      n = 0;
      for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
        struct Particle *step = &work->particles[i];
        if (step->x >= 0) {
          if (frame >= i / 4) {
            {
              s32 size = 5;
              Transform_Reset();
              Transform_ApplyRoll(step->velocityZ);
              Transform_ApplyPitch(step->velocityX);
              Transform_ApplyYaw(step->velocityY);
              Projection_Point(step, &sparkScreen);
              sparkScreen.x = sparkScreen.x / 2 + casterPos.x;
              // Djinni kinds circle the caster's chest; the rest circle lower,
              // Severe Blow lowest.
              if (mode <= CASTING_IMPACT_IMPACT_SAP)
                sparkScreen.y = sparkScreen.y + casterPos.y - 8;
              else if (mode == CASTING_IMPACT_IMPACT_SEVERE_BLOW)
                sparkScreen.y = sparkScreen.y + casterPos.y + 44;
              else
                sparkScreen.y = sparkScreen.y + casterPos.y + 12;
              // DEAD: the depth is clamped to -60..60 and moved to 0..120 but
              // never read; the ROM keeps the clamp and the move.
              CLAMP(sparkScreen.depth, -60, 60);
              sparkScreen.depth += 60;
              EFFECT_DRAW_MOTE(blitters[1], canvas, sprites, sparkScreen.x,
                               sparkScreen.y, size);
            }
            step->x -= 4;
          }
          n++;
        }
      }
      if (mode <= CASTING_IMPACT_IMPACT_SAP) {
        if (n <= 63) {
          EFFECT_VIEW_FROM_CAMERA(camera);
          Projection_Point(&djinnPos, &sparkScreen);
          sparkScreen.x /= 2;
          djinnX = sparkScreen.x;
          blitters[0](canvas, pictures + CASTING_IMPACT_DJINNI_PICTURE,
                      sparkScreen.x - 10, sparkScreen.y - 4, 20, 40);
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    }
    /* FAKEMATCH(dead): position set again to the &djinnPos it holds. */
    position = &djinnPos;

    // Phase 2: place BG2 so the impact stands over the target.
    if (mode == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
      REG_BG2PA = Q_8_8(1);
      if (work->args->side == EFFECT_SIDE_ENEMIES) {
        REG_BG2X = (32 - targetPos.x) << 8;
      } else {
        REG_BG2X = (96 - targetPos.x) << 8;
      }
    } else {
      if (mode == CASTING_IMPACT_IMPACT_ASURA) {
        REG_BG2PA = Q_8_8(1);
        if (work->args->side == EFFECT_SIDE_ENEMIES) {
          scrollPos = -px(128);
          scrollSpeed = px(12);
        } else {
          scrollPos = px(8);
          scrollSpeed = -px(12);
        }
        REG_BG2X = (scrollPos >> 16) << 8;
      }
    }
    if (mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
      REG_BG2PA = Q_8_8(1);
      REG_BG2X = (64 - targetPos.x) << 8;
      work->transferMode = CANVAS_COPY_AND_CLEAR;
      work->transferValue = 0;
      Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
      Iwram_ClearWords(canvas, CANVAS_BYTES);
      REG_BLDCNT = 0;
    }
    if (mode == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
      REG_BG2PA = Q_8_8(1);
      if (work->args->side == EFFECT_SIDE_ENEMIES) {
        REG_BG2X = (32 - targetPos.x) << 8;
      } else {
        REG_BG2X = (96 - targetPos.x) << 8;
      }
    }
    // The four unused impacts clear the canvas and hand off to another effect
    // without giving the caster its motion back: the ice chips to Blizzard's
    // volley, the Mystic Flame one to Mystic Flame's orbs and the comets to
    // PROJECTILE_VOLLEY_IMPACT_COMETS; the gathering ends here.
    if (mode == CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS ||
        mode == CASTING_IMPACT_IMPACT_UNUSED_GATHERING ||
        mode == CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME ||
        mode == CASTING_IMPACT_IMPACT_UNUSED_COMETS) {
      CastingImpact_EndPrelude(work, canvas);
      Actor_SelectAnimationFar(casterActor, POSE_CAST);
      if (mode == CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS) {
        BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_BLIZZARD);
      }
      if (mode == CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME) {
        BattleEffect_RunMysticFlame(args);
      }
      if (mode != CASTING_IMPACT_IMPACT_UNUSED_COMETS) {
        return;
      }
      BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_IMPACT_COMETS);
      return;
    }
    Actor_SetAnimationSpeedFar(casterActor, ANIMATION_STEP_NORMAL);
    SET_VELOCITY(casterActor, savedVelocityX, savedVelocityY, savedVelocityZ);
    casterActor->acceleration = savedAcceleration;
    casterActor->gravity = savedVerticalStrength;
  }
  // Severe Blow, with the caster's motion back, clears the canvas and hands
  // off to BattleEffect_RunSparkGroups' rising sparks, with
  // SPARK_GROUPS_SEVERE_BLOW.
  if (mode == CASTING_IMPACT_IMPACT_SEVERE_BLOW) {
    CastingImpact_EndPrelude(work, canvas);
    args->variant = SPARK_GROUPS_SEVERE_BLOW;
    BattleEffect_RunSparkGroups(args, SPARK_GROUPS_RISING);
    return;
  }

  {
    struct Actor *targetActor =
        GetBattleObjectSlotFar(work->args->targets[0])->actor;

    SET_POSITION(&velocity, (targetActor->x.fixed - djinnPos.x) / 6,
                 (targetActor->y.fixed - djinnPos.y + px(30)) / 6,
                 (targetActor->z.fixed - djinnPos.z) / 6);
    for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
      particle = &work->particles[i];
      particle->variant = 0;
    }
    // Sparks scatter from the target.
    if (mode != CASTING_IMPACT_IMPACT_AQUA_SOCK) {
      s32 height = BattleUnit_GetEffectHeightFar(work->args->targets[0]) / 2;
      for (i = 0; i != 32; i++) {
        particle = &work->particles[i];
        SET_POSITION(particle, targetActor->x.fixed, height,
                     targetActor->z.fixed);
        if (mode == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
          SET_VELOCITY(particle, ((Random16() & 255) - 127) << 12,
                       ((Random16() & 255) - 64) << 10,
                       ((Random16() & 255) - 127) << 12);
        } else {
          SET_VELOCITY(particle, ((Random16() & 255) - 127) << 12,
                       ((Random16() & 255) - 64) << 12,
                       ((Random16() & 255) - 127) << 12);
        }
        particle->variant = i / 2 + 32;
      }
    }
    // Mortal Danger and Asura replace the caster's overlay and the impact's
    // pictures.
    if (mode == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
      Resource_LoadAndDecompress(RESOURCE_COUNTER_REVEAL_SHEET, work, 1, 1);
      Resource_LoadAndDecompress(RESOURCE_RED_CRESCENT_SHEET_A, pictures, 1, 0);
      REG_BLDALPHA = BLEND_ALPHA(16, 14);
    }
    if (mode == CASTING_IMPACT_IMPACT_ASURA) {
      Resource_LoadAndDecompress(RESOURCE_BLUE_BEAST_SHEET, work->imageStorage,
                                 1, 1);
      Resource_LoadAndDecompress(RESOURCE_RED_CRESCENT_SHEET_B, pictures, 1, 0);
      REG_BLDALPHA = BLEND_ALPHA(16, 14);
    }
    // The impact's own particles fly out from the target.
    if (mode != CASTING_IMPACT_IMPACT_SAP &&
        mode != CASTING_IMPACT_IMPACT_LIFE_NOURISH &&
        mode != CASTING_IMPACT_IMPACT_LIFE_LEECH &&
        mode != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
        mode != CASTING_IMPACT_IMPACT_ASURA &&
        mode != CASTING_IMPACT_IMPACT_PSY_LEECH) {
      s32 height = 0;
      if (mode != CASTING_IMPACT_IMPACT_BAD_OMEN)
        height = px(20);
      for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++) {
        particle = &CASTING_IMPACT_IMPACT_PARTICLES[i];
        SET_POSITION(particle, targetActor->x.fixed, height,
                     targetActor->z.fixed);
        if (mode == CASTING_IMPACT_IMPACT_SCORCH ||
            mode == CASTING_IMPACT_IMPACT_BARRAGE) {
          SET_VELOCITY(particle, ((Random16() & 255) - 127) << 11,
                       (Random16() & 255) << 11,
                       ((Random16() & 255) - 127) << 11);
        } else if (mode == CASTING_IMPACT_IMPACT_ACID_BATH) {
          SET_VELOCITY(particle, ((Random16() & 255) - 127) << 11,
                       (Random16() & 127) << 10,
                       ((Random16() & 255) - 127) << 11);
        } else {
          SET_VELOCITY(particle, ((Random16() & 255) - 127) << 10,
                       (Random16() & 127) << 10,
                       ((Random16() & 255) - 127) << 10);
        }
        particle->variant = 0;
      }
    }
    // Mist, Fever, Bad Omen, Heat Mirage, Blinding Smog and Murk wave BG2 by
    // scanline.
    if ((mode >= CASTING_IMPACT_IMPACT_MIST &&
         mode <= CASTING_IMPACT_IMPACT_FEVER) ||
        mode == CASTING_IMPACT_IMPACT_BAD_OMEN ||
        mode == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
        mode == CASTING_IMPACT_IMPACT_MURK ||
        mode == CASTING_IMPACT_IMPACT_BLINDING_SMOG) {
      Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
    }

    // Unused: the last test, for impact mode 100, which this game never
    // dispatches; The Lost Age's twin lists mode 100 in the same test.
    if ((mode >= CASTING_IMPACT_IMPACT_FLINT &&
         mode <= CASTING_IMPACT_IMPACT_SQUALL) ||
        mode == CASTING_IMPACT_IMPACT_BARRAGE ||
        mode == CASTING_IMPACT_IMPACT_CYCLONE_SLASH ||
        mode == CASTING_IMPACT_IMPACT_STUN_VOLTAGE ||
        mode == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
        mode == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST ||
        mode == EFFECT_RANGE_CASTING_IMPACT) {
      duration = 32;
    } else if ((mode >= CASTING_IMPACT_IMPACT_BANE &&
                mode <= CASTING_IMPACT_IMPACT_FEVER) ||
               mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE ||
               mode == CASTING_IMPACT_IMPACT_POISON_CLOUD ||
               mode == CASTING_IMPACT_IMPACT_DEADLY_POISON ||
               mode == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
               mode == CASTING_IMPACT_IMPACT_ACID_BATH ||
               mode == CASTING_IMPACT_IMPACT_MURK ||
               mode == CASTING_IMPACT_IMPACT_PSYPHON_SEAL ||
               mode == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
      duration = 48;
    } else if (mode == CASTING_IMPACT_IMPACT_MELTDOWN) {
      duration = 20;
    } else if (mode == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
               mode == CASTING_IMPACT_IMPACT_ASURA ||
               mode == CASTING_IMPACT_IMPACT_BROIL) {
      duration = 40;
    } else if (mode == CASTING_IMPACT_IMPACT_BLINDING_SMOG ||
               mode == CASTING_IMPACT_IMPACT_BAD_OMEN) {
      duration = 64;
    } else {
      duration = 80;
    }
    // Phase 3, the impact, for duration frames.
    for (frame = 0; frame != duration; frame++) {
      if (mode != CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
        if (mode != CASTING_IMPACT_IMPACT_ASURA) {
          s32 base;
          scanline = work->bg2xLines;
          i = 0;
          base = 4 << 16;
          phase = frame * (TRIG_TURN / 16);
          for (; i != DISPLAY_HEIGHT; i++) {
            s32 offset = (base - ((Trig_Sin(phase) * 4))) >> 10;
            *scanline++ = offset;
            phase += TRIG_TURN / 32;
          }
        }
      }
      // Follow the caster for the first three frames.
      if (frame <= 2) {
        Projection_ScaledUnit(work->args->user, &casterPos);
        casterPos.x = casterPos.x / 2;
        casterPos.y += 16;
      }
      if (mode != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
          mode != CASTING_IMPACT_IMPACT_TERRA_STRIKE &&
          mode != CASTING_IMPACT_IMPACT_ASURA &&
          mode != CASTING_IMPACT_IMPACT_FORCIBLE_ARM &&
          mode != CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
        if (frame <= 11) {
          if (work->args->side == EFFECT_SIDE_ENEMIES) {
            blitters[0](canvas, work->sheet + frame / 2 * (48 * 72),
                        casterPos.x - 32, casterPos.y - 40, 48, 72);
          } else {
            blitters[0](canvas, work->sheet + frame / 2 * (48 * 72),
                        casterPos.x, casterPos.y - 40, 48, 72);
          }
        }
      }
      // Step the background palette towards the impact's colours.
      switch (mode) {
      case CASTING_IMPACT_IMPACT_FORCIBLE_ARM:
        BattleEffect_StepPaletteToResource(RESOURCE_FLAME_SHEET_A);
        break;
      case CASTING_IMPACT_IMPACT_AQUA_SOCK:
        BattleEffect_StepPaletteToResource(RESOURCE_BLUE_FLAME_SHEET);
        break;
      case CASTING_IMPACT_IMPACT_PSYPHON_SEAL:
        BattleEffect_StepPaletteToResource(RESOURCE_RUNE_SHEET);
        break;
      case CASTING_IMPACT_IMPACT_TERRA_STRIKE:
        BattleEffect_StepPaletteToResource(RESOURCE_ROCK_SPIRE_SHEET);
        break;
      case CASTING_IMPACT_IMPACT_BANE:
      case CASTING_IMPACT_IMPACT_DEADLY_POISON:
        BattleEffect_StepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
        break;
      case CASTING_IMPACT_IMPACT_BAD_OMEN:
      case CASTING_IMPACT_IMPACT_LIFE_NOURISH:
      case CASTING_IMPACT_IMPACT_ACID_BATH:
        BattleEffect_StepPaletteToResource(RESOURCE_PINK_BURST_SHEET);
        break;
      case CASTING_IMPACT_IMPACT_LIFE_LEECH:
        BattleEffect_StepPaletteToResource(RESOURCE_ICE_BLOCK_SHEET);
        break;
      case CASTING_IMPACT_IMPACT_PSY_LEECH:
        BattleEffect_StepPaletteToResource(RESOURCE_BLAST_SHEET);
        break;
      case CASTING_IMPACT_IMPACT_MIST:
      case CASTING_IMPACT_IMPACT_MURK:
        BattleEffect_StepPaletteToResource(RESOURCE_TAN_PALETTE);
        break;
      case CASTING_IMPACT_IMPACT_SMOG:
      case CASTING_IMPACT_IMPACT_BLINDING_SMOG:
        BattleEffect_StepPaletteToResource(RESOURCE_CYAN_PALETTE);
        break;
      case CASTING_IMPACT_IMPACT_FEVER:
      case CASTING_IMPACT_IMPACT_BROIL:
      case CASTING_IMPACT_IMPACT_HEAT_MIRAGE:
        BattleEffect_StepPaletteToResource(RESOURCE_EMBER_STREAK_SHEET);
        break;
      case CASTING_IMPACT_IMPACT_POISON_CLOUD:
        BattleEffect_StepPaletteToResource(RESOURCE_LIME_PALETTE);
        break;
      case CASTING_IMPACT_IMPACT_SCORCH:
      case CASTING_IMPACT_IMPACT_BARRAGE:
        BattleEffect_StepPaletteToResource(RESOURCE_FLASH_BURST_SHEET);
        break;
      }
      // The orb flashes over the target and sparks scatter.
      if (mode != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
          mode != CASTING_IMPACT_IMPACT_TERRA_STRIKE &&
          mode != CASTING_IMPACT_IMPACT_ASURA) {
        if (frame >= 4 && frame <= 15) {
          blitters[1](canvas,
                      work->sheet + (frame - 4) / 2 * (20 * 48) +
                          CASTING_IMPACT_ORB_PICTURES,
                      targetPos.x / 2 - 8, casterPos.y - 24, 20, 48);
        }
        EFFECT_VIEW_FROM_CAMERA(camera);
        if (frame > 3) {
          for (i = 0; i != 128; i++) {
            s32 index = i / 2;
            struct Particle *step = &work->particles[index];
            s32 size = step->variant;
            if (size > 0) {
              Projection_Point(step, &projected);
              size >>= 4;
              size++;
              projected.x /= 2;
              EFFECT_DRAW_MOTE(blitters[index & 1], canvas, sprites,
                               projected.x, projected.y, size);
              Particle_AdvanceWithGravity3D(step, 60, -px(1) / 16);
              step->variant--;
            }
          }
        }
      }
      // The impact's own effect.
      if (mode == CASTING_IMPACT_IMPACT_SAP ||
          mode == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
          mode == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
          mode == CASTING_IMPACT_IMPACT_PSY_LEECH) {
        // Motes burst from the target and fly back into the caster, who glows
        // white.
        if (frame == 50) {
          BattleEffect_TintBattler(work->args->user, SPRITE_COLOR_WHITE,
                                   TINT_KEEP, TINT_KEEP, 0);
        }
        if (frame == 79) {
          BattleEffect_TintBattler(work->args->user, SPRITE_COLOR_NORMAL,
                                   TINT_KEEP, TINT_KEEP, 0);
        }
        if (frame == 12) {
          particle = CASTING_IMPACT_IMPACT_PARTICLES;
          for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT;
               i++, particle++) {
            SET_POSITION(particle, targetActor->x.fixed, px(20),
                         targetActor->z.fixed);
            SET_VELOCITY(particle, ((Random16() & 255) - 128) << 10,
                         ((Random16() & 255) - 128) << 10,
                         ((Random16() & 255) - 128) << 10);
            particle->variant = 0;
          }
        }
        if (frame > 11) {
          struct Particle *step = CASTING_IMPACT_IMPACT_PARTICLES;
          struct Actor *caster;
          s32 height;
          caster = GetBattleObjectSlotFar(work->args->user)->actor;
          height = BattleUnit_GetEffectHeightFar(work->args->user) / 2;
          for (i = 0; i != 32; i++, step++) {
            if (step->variant >= 0) {
              s32 size = (i & 1) + 6;
              Projection_Point(step, &projected);
              projected.x >>= 1;
              blitters[0](canvas,
                          sprites + gBattleEffect_MotePictures[size - 1],
                          projected.x - (u32)size / 2, projected.y - size, size,
                          size * 2);
              Particle_AdvanceWithGravity3D(step, 62, 0);
              if (frame > i + 22) {
                s32 dx = (caster->x.fixed - step->x) >> 8;
                s32 dy = (caster->y.fixed + height - step->y) >> 8;
                s32 dz = (caster->z.fixed - step->z) >> 8;
                step->velocityX += dx;
                step->velocityY += dy;
                step->velocityZ += dz;
                if ((dx >= -0xfff && dx <= 0xfff) &&
                    (dz >= -0xfff && dz <= 0xfff))
                  step->variant = -1;
              }
            }
          }
        }
      } else if (mode == CASTING_IMPACT_IMPACT_MELTDOWN) {
        // Draw nothing; finish with BattleEffect_RunImpair.
      } else if (mode == CASTING_IMPACT_IMPACT_SQUALL ||
                 mode == CASTING_IMPACT_IMPACT_STUN_VOLTAGE) {
        // Two bolts strike the target.
        if (frame >= 6 && frame <= 19) {
          phase = frame;
          i = 0;
          do {
            blitters[0](canvas,
                        ((phase / 2) & 3) * LIGHTNING_BOLT_PICTURE_BYTES +
                            (pictures + GLINT_SET_BYTES),
                        targetPos.x / 2 - 8, 0, 24, 104);
            i++;
            phase += 3;
          } while (i != 2);
        }
        if (frame >= 8 && frame <= 23) {
          for (i = 0; i != 3; i++) {
            s32 image = i & 3;
            s32 angle = Random16() & TRIG_ANGLE_MASK;
            s32 imageX;
            s32 imageY;
            s32 drawFlags;
            imageX = Trig_Sin(angle);
            imageX <<= 3;
            imageX >>= 16;
            imageX += targetPos.x / 2;
            imageX -= gBattleEffect_GlintPictureWidths[image] / 2;
            imageY = Trig_Cos(angle);
            imageY <<= 5;
            imageY >>= 16;
            imageY -= gBattleEffect_GlintPictureHeights[image] / 2;
            EFFECT_RELEASE_BLITTERS();
            drawFlags = Random16();
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                                gCastingImpact_GlintDrawFlags[drawFlags & 3] |
                                    EFFECT_CLIP_BOUNDS,
                                COMPOSITE_LIGHTEN);
            EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2)(
                canvas, pictures + gBattleEffect_GlintPictures[image], imageX,
                imageY + 56, gBattleEffect_GlintPictureWidths[image],
                gBattleEffect_GlintPictureHeights[image]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
            BattleEffect_FetchRectangleBlitters(work->args->side, blitters);
          }
        }
      } else if (mode == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
        // A flame column falls onto the target.
        s32 fall;
        s32 scroll;
        EFFECT_RELEASE_BLITTERS();
        if (frame >= 0 && frame <= 23) {
          DrawRectangle *draw;
          s32 width;
          s32 columnX;
          s32 columnY;
          s32 top;
          s32 originX;

          originX = targetPos.x / 2;
          fall = frame * 32 - 232;
          scroll = frame * 16 - 48;
          if (fall > 0)
            fall = 0;
          while (scroll > 104)
            scroll -= 104;
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                              COMPOSITE_LIGHTEN);
          draw = (DrawRectangle *)gHeapSlots;
          draw += HEAP_SLOT_RAM_CODE_2;
          columnX = originX - 8;
          columnY = fall + scroll;
          top = columnY - 104;
          width = 17;
          EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2)(canvas, pictures, columnX, top,
                                               width, 104);
          EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2)(canvas, pictures, columnX,
                                               columnY, width, 104 - scroll);
          EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2)(canvas, pictures + (17 * 104),
                                               originX - width, fall + 47,
                                               width * 2, 65);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
          if (frame == 8) {
            work->shakeFrames = frame;
          }
          if (frame > 1) {
            s32 emitted = 0;
            struct Particle *step;
            for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
              if ((step = &work->particles[i])->variant == 0) {
                SET_POSITION(step, targetActor->x.fixed, px(20),
                             targetActor->z.fixed);
                SET_VELOCITY(step, ((Random16() & 255) - 127) << 12,
                             ((Random16() & 255) - 64) << 10,
                             ((Random16() & 255) - 127) << 12);
                step->variant = i / 2 + 32;
                emitted++;
                if (emitted == 4)
                  break;
              }
            }
          }
        }
        BattleEffect_FetchRectangleBlitters(work->args->side, blitters);
      } else if (mode == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
        s32 height;
        EFFECT_RELEASE_BLITTERS();
        // The rune's two halves, the second mirrored.
        if (frame >= 4 && frame <= 23) {
          pairX = targetPos.x / 2;
          height = 48;
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                              COMPOSITE_LIGHTEN);
          slotDraw = (DrawRectangle *)gHeapSlots;
          slotDraw[HEAP_SLOT_RAM_CODE_2](canvas, pictures, pairX - 24, 48, 24,
                                         height);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              COMPOSITE_LIGHTEN);
          EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2)(canvas, pictures, pairX, 48, 24,
                                               height);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        }
        BattleEffect_FetchRectangleBlitters(work->args->side, blitters);
      } else if (mode == CASTING_IMPACT_IMPACT_CYCLONE_SLASH) {
        // A tornado stands over the target.
        s32 image;
        if (frame > 15) {
          REG_BLDALPHA = BLEND_ALPHA(32 - frame, 16);
        }
        if (frame > 5) {
          stripX = targetPos.x / 2 - 20;
          image = frame / 2 % 3;
          pictureOffset = image * (2 * 40 * 32);
          blitters[0](canvas, pictures + GLINT_SET_BYTES + pictureOffset,
                      stripX, 16, 40, 32);
          blitters[0](canvas,
                      image * (40 * 32) +
                          (pictures + GLINT_SET_BYTES + 6 * (40 * 32)),
                      stripX, 48, 40, 32);
          blitters[0](canvas,
                      pictureOffset + (pictures + GLINT_SET_BYTES + 40 * 32),
                      stripX, 80, 40, 32);
        }
      } else if (mode == CASTING_IMPACT_IMPACT_SCORCH ||
                 mode == CASTING_IMPACT_IMPACT_BARRAGE) {
        struct Particle *step = CASTING_IMPACT_IMPACT_PARTICLES;
        for (i = 0; i != 16; i++, step++) {
          // Flash bursts rise.
          if (frame >= i / 2 + 4) {
            s32 age = step->variant;
            if (age <= 11) {
              s32 image = age / 2;
              Projection_Point(step, &projected);
              projected.x /= 2;
              blitters[0](canvas, pictures + image * (32 * 64),
                          projected.x - 16, projected.y - 32, 32, 64);
              Particle_AdvanceWithGravity3D(step, 60, px(1) / 16);
              step->variant++;
            }
          }
        }
      } else if (mode == CASTING_IMPACT_IMPACT_FLINT) {
        // Draw nothing.
      } else if (mode == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
        s32 imageY;
        s32 sway;
        // Condemn's reaper and crescent.
        Trig_Sin(frame * (TRIG_TURN / 128));
        sway = Trig_Cos(frame * (TRIG_TURN / 128));
        sway <<= 2;
        sway >>= 16;
        // targetPos.y is in whole pixels, so the shift leaves 0 for any
        // target on the screen: the pictures stand at a fixed height.
        imageY = (targetPos.y >> 16) + sway + 16;
        if (frame <= 3) {
          blitters[0](canvas, work->sheet,
                      CASTING_IMPACT_PICTURE_X(work->args->side, 0),
                      imageY + gCastingImpact_PictureY[0], 57, 98);
        } else {
          if (frame <= 7) {
            blitters[0](canvas, work->sheet,
                        CASTING_IMPACT_PICTURE_X(work->args->side, 0),
                        imageY + gCastingImpact_PictureY[0], 57, 98);
          }
          blitters[0](canvas, work->sheet + REAPER_SHEET_SWUNG,
                      CASTING_IMPACT_PICTURE_X(work->args->side, 1),
                      imageY + gCastingImpact_PictureY[1], 99, 69);
          if (frame == 4 || frame == 5) {
            Iwram_FillWords(canvas, CANVAS_BYTES,
                            CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          }
          if (frame == 6 || frame == 7) {
            blitters[0](canvas, work->sheet + REAPER_SHEET_CRESCENT,
                        CASTING_IMPACT_PICTURE_X(work->args->side, 2),
                        imageY + gCastingImpact_PictureY[2], 128, 91);
          }
          if (frame == 8 || frame == 9) {
            blitters[0](canvas, pictures + RED_CRESCENT_FADE_1,
                        CASTING_IMPACT_PICTURE_X(work->args->side, 3),
                        imageY + gCastingImpact_PictureY[3], 128, 91);
          }
          if (frame == 10 || frame == 11) {
            blitters[0](canvas, pictures + RED_CRESCENT_FADE_2,
                        CASTING_IMPACT_PICTURE_X(work->args->side, 4),
                        imageY + gCastingImpact_PictureY[4], 128, 59);
          }
          if (frame == 12 || frame == 13) {
            blitters[0](canvas, pictures + RED_CRESCENT_FADE_3,
                        CASTING_IMPACT_PICTURE_X(work->args->side, 5),
                        imageY + gCastingImpact_PictureY[5], 122, 29);
          }
          if (frame == 14 || frame == 15) {
            blitters[0](canvas, pictures + RED_CRESCENT_FADE_4,
                        CASTING_IMPACT_PICTURE_X(work->args->side, 6),
                        imageY + gCastingImpact_PictureY[6], 76, 25);
          }
        }
      } else if (mode == CASTING_IMPACT_IMPACT_ASURA) {
        // The swordsman slides in and swings, and his red crescent fades.
        scrollPos += scrollSpeed;
        if (frame > 6) {
          scrollSpeed = (scrollSpeed * 48) / 64;
        }
        REG_BG2X = (scrollPos >> 16) << 8;
        {
          s32 fade;
          fade = frame - 16;
          if (fade >= 0 && fade <= 15) {
            REG_BLDALPHA = BLEND_ALPHA(16 - fade, 16);
          }
        }
        if (frame == 4 || frame == 5) {
          Iwram_FillWords(canvas, CANVAS_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        }
        if (frame <= 3) {
          if (work->args->side == EFFECT_SIDE_PARTY) {
            blitters[0](canvas, work->imageStorage, 0, 24, 80, 104);
          } else {
            blitters[0](canvas, work->imageStorage, 48, 24, 80, 104);
          }
        } else {
          if (frame <= 7) {
            if (work->args->side == EFFECT_SIDE_PARTY) {
              blitters[0](canvas, work->imageStorage, 0, 24, 80, 104);
            } else {
              blitters[0](canvas, work->imageStorage, 48, 24, 80, 104);
            }
          }
          if (work->args->side == EFFECT_SIDE_PARTY) {
            blitters[0](canvas, work->imageStorage + ASURA_SHEET_SWUNG, 16, 16,
                        80, 104);
          } else {
            blitters[0](canvas, work->imageStorage + ASURA_SHEET_SWUNG, 32, 16,
                        80, 104);
          }
          if (frame == 6 || frame == 7) {
            blitters[0](canvas, work->imageStorage + ASURA_SHEET_CRESCENT, 0,
                        16, 128, 91);
          }
          if (frame == 8 || frame == 9) {
            blitters[0](canvas, pictures, 0, 16, 128, 91);
          }
          if (frame == 10 || frame == 11) {
            blitters[0](canvas, pictures + (128 * 91), 0, 16, 128, 59);
          }
          if (frame == 12 || frame == 13) {
            blitters[0](canvas, pictures + (128 * 91 + 128 * 59), 0, 16, 128,
                        29);
          }
          if (frame == 14 || frame == 15) {
            blitters[0](canvas, pictures + (128 * 91 + 128 * 59 + 128 * 29), 0,
                        16, 128, 26);
          }
        }
      } else if (mode == CASTING_IMPACT_IMPACT_BROIL) {
        // Ember streaks.
        for (i = 0; i != 12; i++) {
          if (frame >= i + 6 && frame < i + 18) {
            s32 image = (frame - i - 6) / 2;
            s32 imageX =
                targetPos.x / 2 - gBattleEffect_PuffArcPictureWidths[image] / 2;
            s32 mirrored;
            if (i & 1)
              imageX += (1 + i) / 2 * 3;
            else
              imageX -= (i + 1) / 2 * 3;
            mirrored = 1;
            if (i != 0) {
              mirrored = 0;
              if (((i - 1) & 3) > 1)
                mirrored = 1;
            }
            blitters[mirrored](
                canvas, pictures + gBattleEffect_PuffArcPictures[image], imageX,
                gBattleEffect_PuffArcPictureBiasY[image] + 48,
                gBattleEffect_PuffArcPictureWidths[image],
                gBattleEffect_PuffArcPictureHeights[image]);
          }
        }
      } else if (mode == CASTING_IMPACT_IMPACT_FROST_BITE) {
        // Ice chips close in on the target.
        if (frame == 0) {
          for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++) {
            particle = &CASTING_IMPACT_IMPACT_PARTICLES[i];
            SET_POSITION(particle, (Random16() & 127) + 32, 0, 0);
            SET_VELOCITY(particle, Random16() & TRIG_ANGLE_MASK,
                         Random16() & TRIG_ANGLE_MASK, Random16() & 0xffff);
          }
          // Chip 63's y, not its radius, is set to 159, so it flies up to 159
          // pixels off its line and the target gives the last flinch with no
          // chip on it, as the last chip crosses the sky above.
          // XXX (no evidence): whether its radius was meant, as 159 is the
          // largest the others draw (32 + 127); only the author's intent would
          // say.
          CASTING_IMPACT_IMPACT_PARTICLES[63].y = 159;
        }
        {
          struct Particle *step = CASTING_IMPACT_IMPACT_PARTICLES;
          for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++, step++) {
            if (step->x >= 0 && frame >= i / 2) {
              s32 image = i & 3;
              Transform_Reset();
              Transform_ApplyPitch(step->velocityX);
              Transform_ApplyYaw(step->velocityY);
              Projection_Point(step, &projected);
              projected.x = projected.x / 2 + targetPos.x / 2;
              projected.y += targetPos.y + 32;
              blitters[1](canvas,
                          pictures + gCastingImpact_IceChipPictures[image],
                          projected.x - 4, projected.y - 4, 8, 8);
              step->x -= 6;
              if (step->x < 0 && ((i & 7) == 0 || i == 63)) {
                Sound_PlayFar(SE_IMPACT);
                BattleEffect_TintBattler(work->args->targets[0],
                                         SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
              }
            }
          }
        }
      } else if (mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
        s32 height;
        // A rock spire rises under the target and sinks.
        if (frame >= 5 && frame <= 49) {
          if (frame > 25) {
            height = 196 - (frame << 2);
          } else {
            height = (frame << 4) - 64;
          }
          if (height > 96) {
            height = 96;
          }
          blitters[0](canvas, pictures, 48, 104 - height, 32, height);
        }
      } else if (mode == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
                 mode == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
        // A 32x64 flame falls diagonally onto the target over six frames.
        s32 offset;
        s32 imageX;
        s32 imageY;
        if (frame <= 5) {
          if (work->args->side == EFFECT_SIDE_ENEMIES) {
            imageX = targetPos.x;
            imageX += (u32)imageX >> 31;
            imageX >>= 1;
            offset = (6 - frame) * 3;
            imageX += offset * 2;
            imageY = targetPos.y - offset * 4 + 24;
          } else {
            imageX = targetPos.x;
            imageX += (u32)imageX >> 31;
            imageX >>= 1;
            offset = (6 - frame) * 3;
            imageX -= offset * 2;
            imageY = targetPos.y - offset * 4 + 24;
          }
          blitters[1](canvas, pictures, imageX - 16, imageY - 32, 32, 64);
        }
      } else if (mode == CASTING_IMPACT_IMPACT_BAD_OMEN) {
        // Skulls rise from the target and fade.
        if (frame > 47) {
          REG_BLDALPHA = BLEND_ALPHA(64 - frame, 16);
        }
        {
          struct Particle *step = CASTING_IMPACT_IMPACT_PARTICLES;
          for (i = 0; i != 16; i++, step++) {
            s32 image = i % 3;
            Projection_Point(step, &projected);
            projected.x /= 2;
            blitters[i & 1](canvas, pictures + image * (24 * 24),
                            projected.x - 12, projected.y - 12, 24, 24);
            Particle_AdvanceWithGravity3D(step, 60, 1 << ((i & 3) + 11));
            step->variant++;
          }
        }
      } else if (mode != EFFECT_RANGE_CASTING_IMPACT) {
        // Unused: the test leaves impact mode 100 without smoke, a mode this
        // game never dispatches; The Lost Age's twin makes the same test.
        // The smoke impacts: a puff rises each frame.
        struct Particle *step = CASTING_IMPACT_IMPACT_PARTICLES;
        for (i = 0; i != 16; i++, step++) {
          if (frame >= i + 4) {
            s32 age = step->variant;
            if (age <= 23) {
              s32 image = age / 4;
              Projection_Point(step, &projected);
              projected.x /= 2;
              blitters[i & 1](canvas, pictures + image * (24 * 48),
                              projected.x - 12, projected.y - 24, 24, 48);
              if (mode == CASTING_IMPACT_IMPACT_ACID_BATH)
                Particle_AdvanceWithGravity3D(step, 60, px(1) / 64);
              else
                Particle_AdvanceWithGravity3D(step, 60, px(1) / 16);
              step->variant++;
            }
          }
        }
      }
      // From Bane to Sap the Djinni flies to the target in six frames.
      if (mode <= CASTING_IMPACT_IMPACT_SAP) {
        if (frame <= 5) {
          Projection_Point(&djinnPos, &projected);
          projected.x /= 2;
          djinnX = projected.x;
          blitters[1](canvas, pictures + CASTING_IMPACT_DJINNI_PICTURE,
                      djinnX - 10, projected.y - 4, 20, 40);
          position->x += velocity.x;
          position->y += velocity.y;
          position->z += velocity.z;
        }
      }
      // The cues.
      if (frame == 3) {
        EFFECT_SHOW_DAMAGE(-1);
      }
      if (frame == 4) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
      }
      if (frame == 6) {
        // Unused: the last test, for impact mode 100, which this game never
        // dispatches; The Lost Age's twin lists mode 100 in the same test.
        if (mode == CASTING_IMPACT_IMPACT_FLINT ||
            mode == CASTING_IMPACT_IMPACT_SCORCH ||
            mode == CASTING_IMPACT_IMPACT_SAP ||
            mode == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
            mode == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
            mode == CASTING_IMPACT_IMPACT_PSY_LEECH ||
            mode == CASTING_IMPACT_IMPACT_BARRAGE ||
            mode == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST ||
            mode == EFFECT_RANGE_CASTING_IMPACT) {
          BattleMotion_ApplyKnockbackFar(work->args->targets[0],
                                         KNOCKBACK_HURL);
          work->shakeFrames = 8;
        } else if (mode == CASTING_IMPACT_IMPACT_BROIL ||
                   mode == CASTING_IMPACT_IMPACT_AQUA_SOCK ||
                   mode == CASTING_IMPACT_IMPACT_FORCIBLE_ARM) {
          BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_HOP);
          work->shakeFrames = 2;
        } else if (mode == CASTING_IMPACT_IMPACT_CYCLONE_SLASH ||
                   mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
          BattleMotion_ApplyKnockbackFar(work->args->targets[0],
                                         KNOCKBACK_LAUNCH);
          work->shakeFrames = 8;
        }
      }
      if (frame == 6) {
        BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
      }
      if (frame == 14) {
        BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
  }
  // Phase 4: Meltdown hands the cleared canvas to the stat reduction (Impair),
  // as CastingImpact_EndPrelude does.
  if (mode == CASTING_IMPACT_IMPACT_MELTDOWN) {
    Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
    Iwram_ClearWords(canvas, CANVAS_BYTES);
    work->args->prelude = 0;
    Task_Remove(Palette_StepFadeTransfer);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleEffect_RunImpair(args);
  } else {
    if (mode == CASTING_IMPACT_IMPACT_MIST ||
        mode == CASTING_IMPACT_IMPACT_FEVER ||
        mode == CASTING_IMPACT_IMPACT_BAD_OMEN ||
        mode == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
        mode == CASTING_IMPACT_IMPACT_BLINDING_SMOG ||
        mode == CASTING_IMPACT_IMPACT_MURK) {
      Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
    }
    EFFECT_FINISH_CANVAS();
  }
}

#undef CASTING_IMPACT_ORB_PICTURES
#undef CASTING_IMPACT_DJINNI_PICTURE
#undef CASTING_IMPACT_GATHER_FRAMES
#undef CASTING_IMPACT_SPARKS
#undef CASTING_IMPACT_IMPACT_PARTICLES
#undef CASTING_IMPACT_IMPACT_PARTICLE_COUNT
#undef CASTING_IMPACT_PICTURE_X
