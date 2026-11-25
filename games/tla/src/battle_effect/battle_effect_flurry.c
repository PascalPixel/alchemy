/// @file
/// Gust, Flying Attack, the rush bursts and the claws: the flurry of strikes
/// they all play.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Each plays the flurry mode `basic`, `stronger` or `strongest` as the
/// effect's variant (enum EffectVariant) says.
#define FLURRY_DEFINE_BY_VARIANT(name, basic, stronger, strongest)             \
  void name(struct BattleEffectParams *args) {                                 \
    if (args->variant == EFFECT_VARIANT_BASIC) {                               \
      BattleEffect_RunFlurry(args, basic);                                     \
      return;                                                                  \
    }                                                                          \
    if (args->variant == EFFECT_VARIANT_STRONGER) {                            \
      BattleEffect_RunFlurry(args, stronger);                                  \
      return;                                                                  \
    }                                                                          \
    BattleEffect_RunFlurry(args, strongest);                                   \
  }

/// The user strikes the first target once, twice or three times as the
/// variant grows with a double or triple blow, and each strike fills the
/// canvas with light.
///
/// Abilities: Gust; Rapid Smash, Sonic Smash, Slice.
FLURRY_DEFINE_BY_VARIANT(BattleEffect_RunGust, FLURRY_GUST, FLURRY_GUST_DOUBLE,
                         FLURRY_GUST_TRIPLE)

/// The user strikes the first target five times.
///
/// Abilities: Bear Claw; Counterstrike.
void BattleEffect_RunBearClaw(struct BattleEffectParams *args) {
  BattleEffect_RunFlurry(args, FLURRY_BEAR_CLAW);
}

/// Ransack plays the same five strikes as Bear Claw.
///
/// Abilities: Ransack; Mad Dash.
void BattleEffect_RunRansack(struct BattleEffectParams *args) {
  BattleEffect_RunFlurry(args, FLURRY_BEAR_CLAW);
}

/// Seven strikes land on the first target in the lime palette.
///
/// Abilities: Bacteria Rush.
void BattleEffect_RunBacteriaRush(struct BattleEffectParams *args) {
  BattleEffect_RunFlurry(args, FLURRY_BACTERIA_RUSH);
}

/// Nine strikes land on the first target in the blue palette.
///
/// Abilities: Freebite Rush.
void BattleEffect_RunFreebiteRush(struct BattleEffectParams *args) {
  BattleEffect_RunFlurry(args, FLURRY_FREEBITE_RUSH);
}

/// One, two or three strikes land on the first target as the variant grows
/// with a double or triple blow.
///
/// Abilities: Flying Attack; Swift Strike.
FLURRY_DEFINE_BY_VARIANT(BattleEffect_RunFlyingAttack, FLURRY_FLYING_ATTACK,
                         FLURRY_FLYING_ATTACK_DOUBLE,
                         FLURRY_FLYING_ATTACK_TRIPLE)

/// Three strikes land on the first target in the Mars Djinni's colours.
///
/// Abilities: Ur Flash.
void BattleEffect_RunUrFlash(struct BattleEffectParams *args) {
  BattleEffect_RunFlurry(args, FLURRY_UR_FLASH);
}

/// The user lunges further at the first target and strikes once, in the
/// embers' colours.
///
/// Abilities: Claw Attack.
void BattleEffect_RunClawAttack(struct BattleEffectParams *args) {
  BattleEffect_RunFlurry(args, FLURRY_CLAW_ATTACK);
}

/// One strike lands on the first target while glints flicker around it.
///
/// Abilities: Crusher Grip.
void BattleEffect_RunCrusherGrip(struct BattleEffectParams *args) {
  BattleEffect_RunFlurry(args, FLURRY_CRUSHER_GRIP);
}

/// One strike lands on the first target while the battle ripples and smoke
/// rises from its feet.
///
/// Abilities: Vanish Claw.
void BattleEffect_RunVanishClaw(struct BattleEffectParams *args) {
  BattleEffect_RunFlurry(args, FLURRY_VANISH_CLAW);
}

/// Doublestep plays Gust's one, two or three strikes as the variant grows.
///
/// Abilities: Doublestep.
FLURRY_DEFINE_BY_VARIANT(BattleEffect_RunDoublestep, FLURRY_GUST,
                         FLURRY_GUST_DOUBLE, FLURRY_GUST_TRIPLE)

/// The motes in PARTICLES, waiting above the target; the wave, smoke and orb
/// pictures follow them in the map cell buffer.
#define FLURRY_MOTE_COUNT 256
/// Each strike's own run of the motes, of which it throws gFlurry_Styles'
/// FLURRY_MOTES.
#define FLURRY_STRIKE_MOTES 32
/// The wave pictures, past the motes, where the effect's own pictures begin.
#define FLURRY_WAVE_PICTURES (Ram_MapCellBuffer + 9216)
/// The smoke pictures, past the waves: an offset into the effect's pictures.
#define FLURRY_SMOKE_PICTURES 5000
/// Where BlueArcSheetA or BlueArcSheetB is decoded in the effect's pictures.
#define FLURRY_BLUE_ARC_PICTURES 3200
/// The yellow orb's pictures, packed to half width past the smoke: an offset
/// into the effect's pictures.
#define FLURRY_ORB_PICTURES 11912
/// The level the canvas fills with on each hit, where the style flashes.
#define FLURRY_FLASH_LEVEL 47

/// Vanish Claw's smoke: sixteen puffs, each through six pictures four frames
/// apart.
#define FLURRY_SMOKE_PUFFS 16
#define FLURRY_SMOKE_PICTURE_FRAMES 4
#define FLURRY_SMOKE_FRAMES (6 * FLURRY_SMOKE_PICTURE_FRAMES)

/// The bytes of a gFlurry_Styles row, one row a mode.
enum FlurryColumn {
  FLURRY_ARC_SHEET, ///< the arcs (enum FlurryArcs)
  FLURRY_PALETTE,   ///< whose palette (enum FlurryPalette)
  FLURRY_STRIKES,
  FLURRY_MOTES,    ///< motes thrown by each strike, at most its run of
                   ///< FLURRY_STRIKE_MOTES
  FLURRY_INTERVAL, ///< frames between strikes
  FLURRY_FRAMES,   ///< frames in the whole effect
  FLURRY_FLASH,    ///< 1 fills the canvas with FLURRY_FLASH_LEVEL on each hit
  FLURRY_EXTRA     ///< what else plays around the target (enum FlurryExtra)
};

/// The user closes on the first target and strikes it again and again, each
/// strike an arc or a wave with a flash and a spray of motes; the last knocks
/// the target back.
void BattleEffect_RunFlurry(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint actorPos;
  struct ScreenPoint targetPos;
  s32 screen[3];
  s32 otherPoint[3];
  struct ScreenPoint pos;
  struct ScreenPoint spot;
  void *draw[2];
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  u8 *sheet;
  struct BattleCamera *camera;
  struct MotionObject *target;
  s32 resource;
  u8 *pictures = FLURRY_WAVE_PICTURES;
  s32 row;
  s32 col;
  s32 i;
  s32 j;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraState;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (args->prelude == 1)
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG, args->side,
                        DJINNI_ANCHOR_OVER_USER),
        screen, otherPoint);

  // The pictures and the palette.
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
  // The orb's pictures, packed to half width: the canvas is shown twice as
  // wide.
  Resource_LoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, state->sheet, 1, 0);
  for (row = 0; row != YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT; row++) {
    for (col = 0; col != YELLOW_ORB_WIDTH; col++) {
      s32 dst = row * YELLOW_ORB_PACKED_WIDTH + col / 2;
      s32 src = row * YELLOW_ORB_WIDTH + col;

      pictures[dst + FLURRY_ORB_PICTURES] = state->sheet[src];
    }
  }
  // The glints' sprite sheet takes the orb's place.
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, state->sheet, 1, 0);
  if (gFlurry_Styles[mode][FLURRY_ARC_SHEET] == FLURRY_ARCS_AND_WAVES)
    Resource_LoadAndDecompress(RESOURCE_BLUE_ARC_SHEET_A,
                               state->sheet + FLURRY_BLUE_ARC_PICTURES, 1, 1);
  else
    Resource_LoadAndDecompress(RESOURCE_BLUE_ARC_SHEET_B,
                               state->sheet + FLURRY_BLUE_ARC_PICTURES, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_WAVE_SHEET, pictures, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET,
                             pictures + FLURRY_SMOKE_PICTURES, 1, 0);
  switch (gFlurry_Styles[mode][FLURRY_PALETTE]) {
  case FLURRY_PALETTE_MARS_DJINNI:
    resource = RESOURCE_MARS_DJINN_SHEET;
    break;
  case FLURRY_PALETTE_LIME:
    resource = RESOURCE_LIME_PALETTE;
    break;
  case FLURRY_PALETTE_BLUE_ARC:
    resource = RESOURCE_BLUE_ARC_SHEET_B;
    break;
  default:
    resource = RESOURCE_EMBER_STREAK_SHEET;
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource),
                       SHEET_PALETTE_BYTES);

  // The user closes in; Claw Attack lunges further and waits.
  if (mode == FLURRY_CLAW_ATTACK) {
    BattleMotion_ApproachTargetFar(args->user, args->targets[0], 8, toFIXED(6));
    wait(7);
  } else {
    BattleMotion_ApproachTargetFar(args->user, args->targets[0], 4, 0);
  }
  wait(1);

  EffectPosition_ProjectScaledBattler(args->targets[0], &targetPos);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  if (gFlurry_Styles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_RIPPLE)
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);

  // The motes wait 25 pixels above the target, aimed away from the
  // caster; none is out yet.
  target = BattleObject_GetFar(args->targets[0])->object;
  for (i = 0; i != FLURRY_MOTE_COUNT; i++) {
    struct Particle *mote = &PARTICLES[i];

    SET_POSITION(mote, target->x, target->y + toFIXED(25), target->z);
    SET_VELOCITY(mote, (Random16() & 255) << 12,
                 ((Random16() & 255) - 127) << 12,
                 ((Random16() & 255) - 127) << 12);
    if (mote->x > 0)
      mote->velocityX = -mote->velocityX;
    mote->variant = -1;
  }
  // The smoke puffs start at the target's feet, one a frame.
  for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
    struct Particle *puff = &state->particles[i];

    SET_POSITION(puff, INTtoFIXED(targetPos.x / 2), toFIXED(88), 0);
    puff->velocityX = ((Random16() & 255) - 128) << 9;
    puff->velocityY = ((Random16() & 255) - 128) << 9;
    puff->variant = -i;
    puff->velocityZ = 0;
  }

  for (frame = 0; frame != gFlurry_Styles[mode][FLURRY_FRAMES]; frame++) {
    s32 *lines;
    s32 strikes;

    // The ripple's line table: each line of BG2 swings up to six pixels on a
    // sine wave.
    lines = state->bg2xLines;
    for (i = 0; i != DISPLAY_HEIGHT; i++)
      *lines++ = CANVAS_WAVE_BG2X(toFIXED(6) -
                                  Trig_Sin((frame + i) * (TRIG_TURN / 32)) * 6);
    if (mode == FLURRY_CRUSHER_GRIP && frame == 8)
      Sound_PlayFar(SE_RISE_WITH_SPARKS);
    strikes = gFlurry_Styles[mode][FLURRY_STRIKES];
    EffectPosition_ProjectScaledBattler(args->user, &actorPos);
    actorPos.x /= 2;

    // The arcs and waves, mirrored on odd strikes.
    if (args->side == EFFECT_SIDE_ENEMIES) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
    }
    draw[0] = Ram_HeapSlots->ramCode;
    draw[1] = Ram_HeapSlots->ramCodePaired;
    for (i = 0; i != strikes; i++) {
      s32 start = i * gFlurry_Styles[mode][FLURRY_INTERVAL];

      if (frame >= start && frame < start + 6) {
        s32 step = frame - start;

        if ((i & 3) <= 1 ||
            gFlurry_Styles[mode][FLURRY_ARC_SHEET] == FLURRY_ARCS) {
          // The arc's six pictures.
          if (args->side == EFFECT_SIDE_ENEMIES)
            ((DrawRectangle)draw[i & 1])(canvas,
                                         state->sheet +
                                             FLURRY_BLUE_ARC_PICTURES +
                                             step * PICTURE_BYTES(BLUE_ARC),
                                         targetPos.x / 2 - 16, targetPos.y - 40,
                                         BLUE_ARC_WIDTH, BLUE_ARC_HEIGHT);
          else
            ((DrawRectangle)draw[i & 1])(canvas,
                                         state->sheet +
                                             FLURRY_BLUE_ARC_PICTURES +
                                             step * PICTURE_BYTES(BLUE_ARC),
                                         targetPos.x / 2 - 32, targetPos.y - 40,
                                         BLUE_ARC_WIDTH, BLUE_ARC_HEIGHT);
        } else {
          // The wave's six pictures.
          if (args->side == EFFECT_SIDE_ENEMIES)
            ((DrawRectangle)draw[i & 1])(
                canvas, pictures + step * PICTURE_BYTES(WAVE),
                targetPos.x / 2 - 16, actorPos.y - 8, WAVE_WIDTH, WAVE_HEIGHT);
          else
            ((DrawRectangle)draw[i & 1])(
                canvas, pictures + step * PICTURE_BYTES(WAVE),
                targetPos.x / 2 - 32, actorPos.y - 8, WAVE_WIDTH, WAVE_HEIGHT);
        }
      }
      // The hit; the last strike knocks the target back.
      if (frame == start + 2) {
        if (gFlurry_Styles[mode][FLURRY_FLASH] == 1)
          Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                               CANVAS_FILL_WORD(FLURRY_FLASH_LEVEL));
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
        if (i == strikes - 1) {
          BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
          state->shakeFrames = 8;
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        } else {
          if (i & 1)
            BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_JOLT);
          state->shakeFrames = 4;
          Sound_PlayFar(SE_HEAVY_IMPACT);
        }
        for (j = 0; j != gFlurry_Styles[mode][FLURRY_MOTES]; j++)
          PARTICLES[i * FLURRY_STRIKE_MOTES + j].variant =
              (Random16() & 7) + 15;
      }
      // The orb's six pictures, two frames each.
      if (frame >= start + 2 && frame < start + 14)
        ((DrawRectangle)draw[0])(
            canvas,
            pictures + FLURRY_ORB_PICTURES +
                ((frame - start - 2) / 2) * PICTURE_BYTES(YELLOW_ORB_PACKED),
            targetPos.x / 2 - YELLOW_ORB_PACKED_WIDTH / 2,
            targetPos.y - YELLOW_ORB_PACKED_HEIGHT / 2, YELLOW_ORB_PACKED_WIDTH,
            YELLOW_ORB_PACKED_HEIGHT);
    }
    EFFECT_RELEASE_BLITTERS();

    // The motes, until their life runs out, bouncing off the ground.
    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_MAX);
    draw[0] = Ram_HeapSlots->ramCode;
    draw[1] = Ram_HeapSlots->ramCodePaired;
    for (i = 0; i != FLURRY_MOTE_COUNT; i++) {
      struct Particle *mote = &PARTICLES[i];
      s32 size = mote->variant;

      if (size > 0) {
        EffectPosition_ProjectPoint(&mote->x, &pos);
        pos.x /= 2;
        size >>= 3;
        size++;
        EFFECT_DRAW_MOTE((DrawRectangle)draw[(i / 2) & 1], canvas, sheet, pos.x,
                         pos.y, size);
        Particle_AdvanceWithGravity3D(mote, 60, -toFIXED(1) / 64);
        if (mote->y < toFIXED(8))
          mote->velocityY = -mote->velocityY / 2;
        mote->variant--;
      }
    }
    EFFECT_RELEASE_BLITTERS();

    if (frame > 3) {
      // The glints: three a frame around the target, each with its own
      // random flip, until frame 31.
      if (gFlurry_Styles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_GLINTS &&
          frame <= 31) {
        EffectPosition_ProjectScaledBattler(args->targets[0], &spot);
        for (i = 0; i != 3; i++) {
          s32 angle = Random16() & TRIG_ANGLE_MASK;
          s32 cel = frame & 3;
          s32 radius = (Random16() & 31) + 4;
          s32 x = spot.x / 2 + (FIXEDtoINT(Trig_Sin(angle) * radius) >> 1) -
                  gBattleEffect_GlintPictureWidths[cel] / 2;
          s32 y = spot.y - (FIXEDtoINT(Trig_Cos(angle) * radius) >> 1) -
                  gBattleEffect_GlintPictureHeights[cel] / 2;
          u32 flip = Random16();
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              gFlurry_GlintFlips[flip & 3] | COMPOSITE_CLIP_X |
                                  COMPOSITE_CLIP_Y,
                              BLITTER_ADDITIVE);
          ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
              canvas, state->sheet + gBattleEffect_GlintPictures[cel], x, y,
              gBattleEffect_GlintPictureWidths[cel],
              gBattleEffect_GlintPictureHeights[cel]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        }
      }
      // The smoke: the puffs rise from the target's feet.
      if (gFlurry_Styles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_RIPPLE) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                            BLITTER_ADDITIVE);
        draw[0] = Ram_HeapSlots->ramCode;
        for (i = 0; i != FLURRY_SMOKE_PUFFS; i++) {
          struct Particle *puff = &state->particles[i];

          if (puff->variant >= 0 && puff->variant < FLURRY_SMOKE_FRAMES) {
            ((DrawRectangle)draw[0])(
                canvas,
                pictures + FLURRY_SMOKE_PICTURES +
                    (puff->variant / FLURRY_SMOKE_PICTURE_FRAMES) *
                        PICTURE_BYTES(SMOKE_PUFF),
                FIXED_INTEGER_PART(puff->x) - SMOKE_PUFF_WIDTH / 2,
                FIXED_INTEGER_PART(puff->y) - SMOKE_PUFF_HEIGHT / 2,
                SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
            Particle_AdvanceWithGravity2D(puff, 62, -toFIXED(1) / 16);
          }
          puff->variant++;
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  if (gFlurry_Styles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_RIPPLE)
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

#undef FLURRY_DEFINE_BY_VARIANT
#undef FLURRY_MOTE_COUNT
#undef FLURRY_STRIKE_MOTES
#undef FLURRY_WAVE_PICTURES
#undef FLURRY_SMOKE_PICTURES
#undef FLURRY_BLUE_ARC_PICTURES
#undef FLURRY_ORB_PICTURES
#undef FLURRY_FLASH_LEVEL
#undef FLURRY_SMOKE_PUFFS
#undef FLURRY_SMOKE_PICTURE_FRAMES
#undef FLURRY_SMOKE_FRAMES
