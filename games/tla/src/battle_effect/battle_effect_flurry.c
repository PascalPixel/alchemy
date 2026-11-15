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

/// The motes, in the map cell buffer; the wave, smoke and flash pictures follow
/// them there.
#define MOTES ((struct Particle *)Ram_MapCellBuffer)
#define FLURRY_MOTE_COUNT 256
/// The wave pictures, past the motes.
#define FLURRY_WAVE_PICTURES (Ram_MapCellBuffer + 0x2400)
/// The smoke pictures, past the waves.
#define FLURRY_SMOKE_PICTURES (Ram_MapCellBuffer + 0x3788)
/// The flash pictures, packed to half width past the smoke.
#define FLURRY_FLASH_PICTURES (Ram_MapCellBuffer + 0x5288)

/// The bytes of a gFlurry_Styles row, one row a mode.
enum FlurryColumn {
  FLURRY_ARC_SHEET, ///< the arcs (enum FlurryArcs)
  FLURRY_PALETTE,   ///< whose palette (enum FlurryPalette)
  FLURRY_STRIKES,
  FLURRY_MOTES,    ///< motes thrown by each strike, at most the 32 of
                   ///< its run of MOTES
  FLURRY_INTERVAL, ///< frames between strikes
  FLURRY_FRAMES,   ///< frames in the whole effect
  FLURRY_FLASH,    ///< 1 fills the canvas with level 47 on each hit
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
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  struct BattleCamera *camera;
  struct MotionObject *target;
  s32 resource;
  u8 *cels;
  s32 row;
  s32 col;
  s32 i;
  s32 j;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (args->prelude == 1)
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG, args->side,
                        DJINNI_ANCHOR_OVER_USER),
        screen, otherPoint);

  // The pictures and the palette.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  // The orb (40x288) packed to half width in the map cell buffer: the
  // canvas is shown twice as wide.
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1,
                             0);
  cels = FLURRY_WAVE_PICTURES;
  for (row = 0; row != 288; row++) {
    for (col = 0; col != YELLOW_ORB_WIDTH; col++) {
      s32 dst = row * YELLOW_ORB_PACKED_WIDTH + col / 2;
      s32 src = row * YELLOW_ORB_WIDTH + col;

      (cels + dst)[0x2e88] = work->sheet[src];
    }
  }
  // The glints' sheet takes the orb's place.
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (gFlurry_Styles[mode][FLURRY_ARC_SHEET] == FLURRY_ARCS_AND_WAVES)
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA,
                               work->sheet + 0xc80, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB,
                               work->sheet + 0xc80, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, FLURRY_WAVE_PICTURES,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, FLURRY_SMOKE_PICTURES,
                             1, 0);
  switch (gFlurry_Styles[mode][FLURRY_PALETTE]) {
  case FLURRY_PALETTE_MARS_DJINNI:
    resource = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case FLURRY_PALETTE_LIME:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case FLURRY_PALETTE_BLUE_ARC:
    resource = (s32)&ResourceId_BlueArcSheetB;
    break;
  default:
    resource = (s32)&ResourceId_EmberStreakSheet;
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
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  if (gFlurry_Styles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_RIPPLE)
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);

  // The motes wait 25 pixels above the target, aimed away from the
  // caster; none is out yet.
  target = BattleObject_GetFar(args->targets[0])->object;
  for (i = 0; i != FLURRY_MOTE_COUNT; i++) {
    struct Particle *mote = &MOTES[i];

    SET_POSITION(mote, target->x, target->y + toFIXED(25), target->z);
    mote->velocityX = (Random16() & 255) << 12;
    mote->velocityY = ((Random16() & 255) - 127) << 12;
    mote->velocityZ = ((Random16() & 255) - 127) << 12;
    if (mote->x > 0)
      mote->velocityX = -mote->velocityX;
    mote->variant = -1;
  }
  // The smoke puffs start at the target's feet, one a frame.
  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    struct Particle *puff = &work->particles[i];

    SET_POSITION(puff, INTtoFIXED(targetPos.x / 2), toFIXED(88), 0);
    puff->velocityX = ((Random16() & 255) - 128) << 9;
    puff->velocityY = ((Random16() & 255) - 128) << 9;
    puff->variant = -i;
    puff->velocityZ = 0;
  }

  for (frame = 0; frame != gFlurry_Styles[mode][FLURRY_FRAMES]; frame++) {
    s32 *lines;
    s32 strikes;

    // The ripple's line table: BG2 swings six pixels on a sine wave.
    lines = work->bg2xLines;
    for (i = 0; i != DISPLAY_HEIGHT; i++)
      *lines++ = (0x60000 - Trig_Sin((frame + i) * 0x800) * 6) >> 10;
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
    draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    draw[1] = ((struct {
                u8 before[188];
                DrawRectangle draw;
              } *)Ram_HeapSlots)
                  ->draw;
    for (i = 0; i != strikes; i++) {
      s32 start = i * gFlurry_Styles[mode][FLURRY_INTERVAL];

      if (frame >= start && frame < start + 6) {
        s32 step = frame - start;

        if ((i & 3) <= 1 ||
            gFlurry_Styles[mode][FLURRY_ARC_SHEET] == FLURRY_ARCS) {
          // The arc: six 48x72 cels.
          if (args->side == EFFECT_SIDE_ENEMIES)
            draw[i & 1](canvas,
                        work->sheet + 0xc80 + step * PICTURE_BYTES(BLUE_ARC),
                        targetPos.x / 2 - 16, targetPos.y - 40, BLUE_ARC_WIDTH,
                        BLUE_ARC_HEIGHT);
          else
            draw[i & 1](canvas,
                        work->sheet + 0xc80 + step * PICTURE_BYTES(BLUE_ARC),
                        targetPos.x / 2 - 32, targetPos.y - 40, BLUE_ARC_WIDTH,
                        BLUE_ARC_HEIGHT);
        } else {
          // The wave: six 48x16 cels.
          if (args->side == EFFECT_SIDE_ENEMIES)
            draw[i & 1](
                canvas, FLURRY_WAVE_PICTURES + step * PICTURE_BYTES(WAVE),
                targetPos.x / 2 - 16, actorPos.y - 8, WAVE_WIDTH, WAVE_HEIGHT);
          else
            draw[i & 1](
                canvas, FLURRY_WAVE_PICTURES + step * PICTURE_BYTES(WAVE),
                targetPos.x / 2 - 32, actorPos.y - 8, WAVE_WIDTH, WAVE_HEIGHT);
        }
      }
      // The hit; the last strike knocks the target back.
      if (frame == start + 2) {
        if (gFlurry_Styles[mode][FLURRY_FLASH] == 1)
          Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(47));
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
        if (i == strikes - 1) {
          BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
          work->shakeFrames = 8;
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        } else {
          if (i & 1)
            BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_JOLT);
          work->shakeFrames = 4;
          Sound_PlayFar(SE_HEAVY_IMPACT);
        }
        for (j = 0; j != gFlurry_Styles[mode][FLURRY_MOTES]; j++)
          MOTES[i * 32 + j].variant = (Random16() & 7) + 15;
      }
      // The orb: six 20x48 cels, two frames each.
      if (frame >= start + 2 && frame < start + 14)
        draw[0](canvas, FLURRY_FLASH_PICTURES + ((frame - start - 2) / 2) * 960,
                targetPos.x / 2 - 10, targetPos.y - 24, 20,
                YELLOW_ORB_PACKED_HEIGHT);
    }
    EFFECT_RELEASE_BLITTERS();

    // The motes, until their life runs out, bouncing off the ground.
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_MAX);
    draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    draw[1] = ((struct {
                u8 before[188];
                DrawRectangle draw;
              } *)Ram_HeapSlots)
                  ->draw;
    for (i = 0; i != FLURRY_MOTE_COUNT; i++) {
      struct Particle *mote = &MOTES[i];
      s32 size = mote->variant;

      if (size > 0) {
        EffectPosition_ProjectPoint(&mote->x, &pos);
        pos.x /= 2;
        size >>= 3;
        size++;
        draw[(i / 2) & 1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                          pos.x - size / 2, pos.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(mote, 60, -toFIXED(1) / 64);
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
          s32 x = spot.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
                  gBattleEffect_GlintPictureWidths[cel] / 2;
          s32 y = spot.y - ((Trig_Cos(angle) * radius) >> 17) -
                  gBattleEffect_GlintPictureHeights[cel] / 2;
          u32 flip = Random16();
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              gFlurry_GlintFlips[flip & 3] | COMPOSITE_CLIP_X |
                                  COMPOSITE_CLIP_Y,
                              BLITTER_ADDITIVE);
          ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
              canvas, work->sheet + gBattleEffect_GlintPictures[cel], x, y,
              gBattleEffect_GlintPictureWidths[cel],
              gBattleEffect_GlintPictureHeights[cel]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        }
      }
      // The smoke: sixteen puffs rise from the target's feet, six cels of
      // four frames each.
      if (gFlurry_Styles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_RIPPLE) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                            BLITTER_ADDITIVE);
        draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
        for (i = 0; i != 16; i++) {
          struct Particle *puff = &work->particles[i];

          if ((u32)puff->variant <= 23) {
            draw[0](canvas,
                    FLURRY_SMOKE_PICTURES +
                        (puff->variant / 4) * PICTURE_BYTES(SMOKE_PUFF),
                    FIXED_INTEGER_PART(puff->x) - 12,
                    FIXED_INTEGER_PART(puff->y) - 24, SMOKE_PUFF_WIDTH,
                    SMOKE_PUFF_HEIGHT);
            BattleEffectKernels_IntegrateVector2(puff, 62, -toFIXED(1) / 16);
          }
          puff->variant++;
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  // Teardown.
  if (gFlurry_Styles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_RIPPLE)
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef FLURRY_DEFINE_BY_VARIANT
#undef FLURRY_MOTE_COUNT
#undef FLURRY_WAVE_PICTURES
#undef FLURRY_SMOKE_PICTURES
#undef FLURRY_FLASH_PICTURES
