/// @file
/// Stat boosts and reductions (Impact, Guard, Ward, Dull, Weaken ...).
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The stat boosts' modes (BattleEffect_RunStatBoost), each named for the
/// effect that plays it.
enum StatBoostMode {
  STAT_BOOST_IMPACT,  ///< a dagger, in Mars colours
  STAT_BOOST_GUARD,   ///< a shield, in Mars colours
  STAT_BOOST_WARD,    ///< a shield, in Jupiter colours
  STAT_BOOST_GRANITE, ///< a pane of light, in Venus colours
  STAT_BOOST_ZEPHYR,  ///< a gust of wind, keeping the palette
  STAT_BOOST_FLASH,   ///< a pane of light, in Mars colours
  STAT_BOOST_IRON,    ///< a shield, in Venus colours, the backdrop turning grey
  STAT_BOOST_UNUSED,  ///< a shield, in Mercury colours; no ability's effect
                      ///< plays it
  STAT_BOOST_SHADE,   ///< a pane of light, in Mercury colours
  STAT_BOOST_COAL,    ///< a gust of wind, in Mars colours
  STAT_BOOST_CRUCIBLE_FIRE, ///< Impact's burst without the swirling flares
  STAT_BOOST_STEAM, ///< a shield, in Mercury colours, the backdrop turning to
                    ///< Jupiter's
  STAT_BOOST_GUARD_AURA ///< a pane of light, in grey
};

/// Light swirls into each target and bursts, then a dagger appears in Mars
/// colours.
///
/// Abilities: Impact, High Impact, Demon Spear, Angel Spear; Forge; Kindle.
void BattleEffect_RunImpact(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_IMPACT);
}

/// Light swirls into each target and bursts, then a shield appears in Mars
/// colours.
///
/// Abilities: Guard, Protect, Guardian, Protector; Corona.
void BattleEffect_RunGuard(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_GUARD);
}

/// Light swirls into each target and bursts, then a shield appears in Venus
/// colours as the backdrop turns grey.
///
/// Abilities: Iron.
void BattleEffect_RunIron(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_IRON);
}

/// Plays the stat boost's unused mode; no ability's effect names this one.
void BattleEffect_RunUnusedStatBoost(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_UNUSED);
}

/// Light swirls into each target and bursts, then a shield appears in Jupiter
/// colours.
///
/// Abilities: Ward, Resist, Magic Shell, Magic Shield; Breeze.
void BattleEffect_RunWard(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_WARD);
}

/// Light swirls into each target and bursts, then a shield appears in Mercury
/// colours as the backdrop turns to Jupiter's colours.
///
/// Abilities: Steam.
void BattleEffect_RunSteam(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_STEAM);
}

/// Light swirls into each target and bursts, then a pane of light appears in
/// Venus colours.
///
/// Abilities: Granite.
void BattleEffect_RunGranite(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_GRANITE);
}

/// Light swirls into each target and bursts, then a pane of light appears in
/// Mars colours.
///
/// Abilities: Flash.
void BattleEffect_RunFlash(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_FLASH);
}

/// Light swirls into each target and bursts, then a pane of light appears in
/// Mercury colours.
///
/// Abilities: Shade.
void BattleEffect_RunShade(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_SHADE);
}

/// Light swirls into each target and bursts, then a pane of light appears in
/// grey.
///
/// Abilities: Guard Aura.
void BattleEffect_RunGuardAura(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_GUARD_AURA);
}

/// Light swirls into each target and bursts, then a gust of wind sweeps across
/// it in the colours already loaded, as this is the one boost that keeps the
/// palette.
///
/// Abilities: Zephyr.
void BattleEffect_RunZephyr(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_ZEPHYR);
}

/// Light swirls into each target and bursts, then a gust of wind sweeps across
/// it in Mars colours.
///
/// Abilities: Coal.
void BattleEffect_RunCoal(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_COAL);
}

/// The stat boost's burst with no swirling flares (STAT_BOOST_FLARES), with no
/// prelude Djinni and the fade step cleared.
///
/// Abilities: Crucible Fire.
void BattleEffect_RunCrucibleFire(struct BattleEffectParams *args) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;

  state->fadeStep = 0;
  args->prelude = 0;
  BattleEffect_RunStatBoost(args, STAT_BOOST_CRUCIBLE_FIRE);
}

/// Where each picture sits in the sprite sheet.
enum StatBoostSheet {
  STAT_BOOST_FLARES = 0,      ///< flares (BlueGlowSheet)
  STAT_BOOST_BURST = 1550,    ///< six bursts of light (GlowOrb)
  STAT_BOOST_PICTURE = 11150, ///< the picture the mode picks
  STAT_BOOST_DJINNI = 26048   ///< the Djinni
};

/// When the first target's flares burst and its sprite appears, with the
/// damage; each later target follows STAT_BOOST_TARGET_DELAY frames behind.
/// Crucible Fire starts at STAT_BOOST_CRUCIBLE_FIRE_FRAME, after the flares.
#define STAT_BOOST_BURST_FRAME 52
#define STAT_BOOST_SPRITE_FRAME 80
#define STAT_BOOST_TARGET_DELAY 8
#define STAT_BOOST_CRUCIBLE_FIRE_FRAME (STAT_BOOST_BURST_FRAME + 1)

/// The particles both kinds of stat change set out: the first half of the pool.
#define STAT_CHANGE_PARTICLES (PARTICLE_POOL_COUNT / 2)
/// Each target swirls in STAT_BOOST_TARGET_FLARES flares from its own run of
/// STAT_BOOST_TARGET_PARTICLES particles.
#define STAT_BOOST_TARGET_PARTICLES 64
#define STAT_BOOST_TARGET_FLARES 32
/// How far the flares turn about the target each frame.
#define STAT_BOOST_FLARE_SPIN (TRIG_TURN / 64)
/// A flare is drawn at size STAT_BOOST_FLARE_SIZE_MAX at depth
/// STAT_BOOST_FLARE_DEPTH_NEAR or nearer, one size smaller every
/// STAT_BOOST_FLARE_DEPTH_STEP farther, and at size 1 from
/// STAT_BOOST_FLARE_DEPTH_FAR on.
#define STAT_BOOST_FLARE_SIZE_MAX 6
#define STAT_BOOST_FLARE_DEPTH_NEAR 314
#define STAT_BOOST_FLARE_DEPTH_STEP 64
#define STAT_BOOST_FLARE_DEPTH_FAR                                             \
  (STAT_BOOST_FLARE_DEPTH_NEAR +                                               \
   (STAT_BOOST_FLARE_SIZE_MAX - 1) * STAT_BOOST_FLARE_DEPTH_STEP)
/// How far the prelude's Djinni sways to either side, in pixels.
#define STAT_BOOST_DJINNI_SWAY 20
/// The shift that sets the canvas, shown 1.25 times as wide, against the
/// screen's right edge over the party.
#define STAT_BOOST_PARTY_CANVAS_X (DISPLAY_WIDTH * 4 / 5 - CANVAS_SIZE)

/// Flares of light swirl in to the centre of each target in turn and burst, and
/// the mode's picture plays over it.
void BattleEffect_RunStatBoost(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle draw[2];
  s32 member;
  s32 canvasShiftX;
  s32 drawFlags;
  s32 start;
  s32 additive = BLITTER_ADDITIVE;
  /* FAKEMATCH(temporary): 0 held in a local for the center's coordinates; as
   * 0. */
  s32 zero = 0;
  s32 flipX = COMPOSITE_FLIP_X;
  s32 frame;
  s32 i;
  s32 origin[3];
  s32 target[3];
  struct ScreenPoint base;
  s32 center[3];
  struct ScreenPoint screen;
  s32 point[3];

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  start = 0;
  BattleEffect_StartCanvasLayer(0);

  // The prelude: the Djinni appears on the far side; origin receives where.
  if (args->prelude == 1) {
    if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_IRON)
      BattleEffect_RunDjinniEntrance(args,
                                     DJINNI_ENTRANCE(ELEMENT_VENUS,
                                                     args->side ^ 1,
                                                     DJINNI_ANCHOR_TARGET_X),
                                     origin, target);
    else if (mode == STAT_BOOST_UNUSED || mode == STAT_BOOST_SHADE ||
             mode == STAT_BOOST_STEAM)
      BattleEffect_RunDjinniEntrance(args,
                                     DJINNI_ENTRANCE(ELEMENT_MERCURY,
                                                     args->side ^ 1,
                                                     DJINNI_ANCHOR_TARGET_X),
                                     origin, target);
    else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
      BattleEffect_RunDjinniEntrance(args,
                                     DJINNI_ENTRANCE(ELEMENT_JUPITER,
                                                     args->side ^ 1,
                                                     DJINNI_ANCHOR_TARGET_X),
                                     origin, target);
    else if (mode != STAT_BOOST_CRUCIBLE_FIRE)
      BattleEffect_RunDjinniEntrance(
          args,
          DJINNI_ENTRANCE(ELEMENT_MARS, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
          origin, target);
    origin[0] = origin[0] * 4 / 5;
  }

  // Crucible Fire is Impact joined after the flares, with a fade.
  if (mode == STAT_BOOST_CRUCIBLE_FIRE) {
    state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    mode = STAT_BOOST_IMPACT;
    start = STAT_BOOST_CRUCIBLE_FIRE_FRAME;
  }
  REG_BG2PA = Q_8_8(0.8); // 1.25 times the width

  // The pictures and the colours.
  Resource_LoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET,
                             state->sheet + STAT_BOOST_FLARES, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_SHEET,
                             state->sheet + STAT_BOOST_BURST, 1, 1);
  if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH ||
      mode == STAT_BOOST_SHADE || mode == STAT_BOOST_GUARD_AURA) {
    Resource_LoadAndDecompress(RESOURCE_VIOLET_STREAK_SHEET,
                               state->sheet + STAT_BOOST_PICTURE, 1, 1);
    if (mode == STAT_BOOST_GRANITE) {
      LOAD_BG_PALETTE(RESOURCE_VENUS_DJINN_SHEET);
    } else if (mode == STAT_BOOST_FLASH) {
      LOAD_BG_PALETTE(RESOURCE_MARS_DJINN_SHEET);
    } else {
      LOAD_BG_PALETTE(RESOURCE_MERCURY_DJINN_SHEET);
    }
  } else if (mode == STAT_BOOST_ZEPHYR) {
    Resource_LoadAndDecompress(RESOURCE_WISP_SHEET,
                               state->sheet + STAT_BOOST_PICTURE, 1, 1);
  } else if (mode == STAT_BOOST_COAL) {
    Resource_LoadAndDecompress(RESOURCE_WISP_SHEET,
                               state->sheet + STAT_BOOST_PICTURE, 1, 0);
    LOAD_BG_PALETTE(RESOURCE_MARS_DJINN_SHEET);
  } else {
    void *palette;

    if (mode == STAT_BOOST_IMPACT)
      Resource_LoadAndDecompress(RESOURCE_DAGGER_SHEET,
                                 state->sheet + STAT_BOOST_PICTURE, 1, 0);
    else
      Resource_LoadAndDecompress(RESOURCE_SHIELD_SHEET,
                                 state->sheet + STAT_BOOST_PICTURE, 1, 0);
    if (mode == STAT_BOOST_IMPACT)
      palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
    else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
      palette = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
    else if (mode == STAT_BOOST_IRON)
      palette = Resource_GetTableEntry(RESOURCE_VENUS_DJINN_SHEET);
    else if (mode == STAT_BOOST_UNUSED || mode == STAT_BOOST_STEAM)
      palette = Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SHEET);
    else if (mode == STAT_BOOST_GUARD)
      palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
    else
      palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
    Iwram_CopyWords((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  }
  if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_IRON)
    Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SHEET,
                               state->sheet + STAT_BOOST_DJINNI, 1, 0);
  else if (mode == STAT_BOOST_GUARD_AURA)
    Graphics_LoadGreyRamp();
  else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
    Resource_LoadAndDecompress(RESOURCE_JUPITER_DJINN_SHEET,
                               state->sheet + STAT_BOOST_DJINNI, 1, 0);
  else if (mode == STAT_BOOST_UNUSED || mode == STAT_BOOST_SHADE ||
           mode == STAT_BOOST_STEAM)
    Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SHEET,
                               state->sheet + STAT_BOOST_DJINNI, 1, 0);
  else
    Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SHEET,
                               state->sheet + STAT_BOOST_DJINNI, 1, 0);

  // The flares: 25 pixels across and deep, 50 up and down.
  for (i = 0; i != STAT_CHANGE_PARTICLES; i++) {
    struct Particle *flare = &PARTICLES[i];

    SET_POSITION(flare, (Random16() % 200 - 100) << 14,
                 (Random16() % 200 - 100) << 15,
                 (Random16() % 200 - 100) << 14);
    flare->variant = 0;
  }

  // A lone target is moved to the canvas's middle.
  if (args->targetCount == 1) {
    EffectPosition_ProjectScaledBattler(args->targets[0], &base);
    canvasShiftX = -base.x * 4 / 5 + CANVAS_SIZE / 2;
  } else {
    if (args->side == EFFECT_SIDE_PARTY)
      canvasShiftX = -STAT_BOOST_PARTY_CANVAS_X;
    else
      canvasShiftX = 0;
  }
  REG_BG2X = canvasShiftX << 8;
  state->transferMode = CANVAS_COPY_AND_FADE;
  state->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  if (args->side == EFFECT_SIDE_PARTY)
    drawFlags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
  else
    drawFlags = EFFECT_CLIP_BOUNDS;

  for (frame = start; frame != args->targetCount * STAT_BOOST_TARGET_DELAY +
                                   STAT_BOOST_SPRITE_FRAME + 28;
       frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraState;
    s32 prelude;

    if (frame == 0)
      Sound_PlayFar(SE_SWIRL);
    if (mode == STAT_BOOST_IRON && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource(RESOURCE_GREY_PALETTE_A);
    if (mode == STAT_BOOST_STEAM && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource(RESOURCE_JUPITER_DJINN_SHEET);
    if (frame == STAT_BOOST_SPRITE_FRAME)
      EFFECT_SHOW_DAMAGE(0);

    // The prelude's Djinni sways, and from frame 32 rises two pixels a frame;
    // for its first four frames it is drawn added as well.
    prelude = args->prelude;
    if (prelude == 1) {
      s32 x = (FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 32)) *
                          STAT_BOOST_DJINNI_SWAY)) +
              origin[0] + canvasShiftX - DJINNI_WIDTH / 2;
      s32 y =
          (FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 4)) + origin[1] - 24;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags ^ flipX, BLITTER_MAX);
      draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, drawFlags ^ flipX,
                          additive);
      draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;

      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, state->sheet + STAT_BOOST_DJINNI, x, y, DJINNI_WIDTH,
              DJINNI_HEIGHT);
      if (frame <= 3)
        draw[1](canvas, state->sheet + STAT_BOOST_DJINNI, x, y, DJINNI_WIDTH,
                DJINNI_HEIGHT);
      EFFECT_RELEASE_BLITTERS();
    }

    for (member = 0; member != args->targetCount; member++) {
      struct MotionObject *object =
          BattleObject_GetFar(args->targets[member])->object;
      s32 height = BattleUnit_GetEffectHeightFar(args->targets[member]) * 2 / 3;

      if (frame == member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_FRAME)
        Sound_PlayFar(SE_POWER_UP);
      // The flares turn about two thirds of the target's height.
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, object->x, height, object->z);
      Matrix_TranslateVector(point);
      if (start != 0) {
        if (frame ==
            member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_CRUCIBLE_FIRE_FRAME)
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, member, 16);
      } else {
        if (frame == member * STAT_BOOST_TARGET_DELAY + 48)
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, member, 16);

        // Flares turn and close in on the centre.
        if (frame > member * STAT_BOOST_TARGET_DELAY) {
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, BLITTER_MAX);
          draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, drawFlags, additive);
          {
            /* FAKEMATCH(dead): slot set to NULL first, never read. */
            void **slot = NULL;

            slot = &Ram_HeapSlots->ramCodePaired;
            draw[1] = (DrawRectangle)*slot;
          }
          if (mode == STAT_BOOST_IMPACT) {
            Matrix_RotateX(-frame * STAT_BOOST_FLARE_SPIN);
          } else if (mode == STAT_BOOST_GUARD) {
          } else if (mode == STAT_BOOST_WARD) {
            Matrix_RotateY(frame * STAT_BOOST_FLARE_SPIN);
          } else {
            Matrix_RotateY(frame * STAT_BOOST_FLARE_SPIN);
            Matrix_RotateZ(frame * STAT_BOOST_FLARE_SPIN);
          }
          for (i = 0; i != STAT_BOOST_TARGET_FLARES; i++) {
            struct Particle *flare =
                &PARTICLES[member * STAT_BOOST_TARGET_PARTICLES + i];

            if (frame > member * STAT_BOOST_TARGET_DELAY + i) {
              s32 xx = (flare->x >> 8) * (flare->x >> 8);
              s32 yy = (flare->y >> 8) * (flare->y >> 8);
              s32 zz = (flare->z >> 8) * (flare->z >> 8);
              s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

              if (distance != 0) {
                s32 size;

                EffectPosition_ProjectPoint((s32 *)flare, &screen);
                screen.x = screen.x * 4 / 5 + canvasShiftX;
                CLAMP(screen.depth, STAT_BOOST_FLARE_DEPTH_NEAR,
                      STAT_BOOST_FLARE_DEPTH_FAR);
                size = STAT_BOOST_FLARE_SIZE_MAX -
                       (screen.depth - STAT_BOOST_FLARE_DEPTH_NEAR) /
                           STAT_BOOST_FLARE_DEPTH_STEP;
                EFFECT_DRAW_FLARE(draw[1], canvas, state->sheet, screen.x,
                                  screen.y, size);
                flare->x -= flare->x / distance;
                flare->y -= flare->y / distance;
                flare->z -= flare->z / distance;
              }
            }
          }
          EFFECT_RELEASE_BLITTERS();
        }
      }

      SET_VECTOR(center, zero, zero, zero);
      EffectPosition_ProjectPoint(center, &screen);
      screen.x = screen.x * 4 / 5 + canvasShiftX;

      // The burst: six pictures four frames apart.
      if (frame >= member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_BURST_FRAME &&
          frame <
              member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_BURST_FRAME + 24) {
        s32 picture;

        picture = (frame - member * STAT_BOOST_TARGET_DELAY -
                   STAT_BOOST_BURST_FRAME) /
                  4 % 6;

        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, BLITTER_MAX);
        draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
        draw[0](canvas,
                state->sheet + picture * PICTURE_BYTES(GLOW_ORB) +
                    STAT_BOOST_BURST,
                screen.x - GLOW_ORB_WIDTH / 2, screen.y - GLOW_ORB_HEIGHT / 2,
                GLOW_ORB_WIDTH, GLOW_ORB_HEIGHT);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      }

      // The mode's picture.
      if (mode == STAT_BOOST_IMPACT) {
        // Seven 24x40 pictures, four frames apart.
        if (frame >=
                member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_FRAME &&
            frame < member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_FRAME +
                        28) {
          s32 picture = (frame - member * STAT_BOOST_TARGET_DELAY -
                         STAT_BOOST_SPRITE_FRAME) /
                        4 % 7;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, BLITTER_MAX);
          draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
          draw[0](canvas,
                  state->sheet + picture * PICTURE_BYTES(DAGGER) +
                      STAT_BOOST_PICTURE,
                  screen.x - DAGGER_WIDTH / 2, screen.y - DAGGER_HEIGHT / 2,
                  DAGGER_WIDTH, DAGGER_HEIGHT);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      } else if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH ||
                 mode == STAT_BOOST_SHADE || mode == STAT_BOOST_GUARD_AURA) {
        // Six 32x64 pictures, four frames apart.
        if (frame >=
                member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_FRAME &&
            frame < member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_FRAME +
                        24) {
          s32 picture = (frame - member * STAT_BOOST_TARGET_DELAY -
                         STAT_BOOST_SPRITE_FRAME) /
                        4 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, BLITTER_MAX);
          draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
          draw[0](canvas,
                  state->sheet + picture * PICTURE_BYTES(VIOLET_STREAK) +
                      STAT_BOOST_PICTURE,
                  screen.x - VIOLET_STREAK_WIDTH / 2,
                  screen.y - VIOLET_STREAK_HEIGHT / 2, VIOLET_STREAK_WIDTH,
                  VIOLET_STREAK_HEIGHT);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      } else if (mode == STAT_BOOST_ZEPHYR || mode == STAT_BOOST_COAL) {
        // Six 64x32 pictures two frames apart, mirrored below into 64x64.
        if (frame >=
                member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_FRAME &&
            frame < member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_FRAME +
                        24) {
          s32 picture = (frame - member * STAT_BOOST_TARGET_DELAY -
                         STAT_BOOST_SPRITE_FRAME) /
                        2 % 6;
          u8 *wisp;       // drawn as its upper and mirrored lower half
          s32 lowerFlags; // the lower half drawn flipped

          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, BLITTER_MAX);
          lowerFlags = drawFlags | COMPOSITE_FLIP_Y;
          draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
          // Built for the 128 by 128 canvas, 7 the shift of each side.
          BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7, lowerFlags,
                                    BLITTER_MAX);
          wisp =
              state->sheet + picture * PICTURE_BYTES(WISP) + STAT_BOOST_PICTURE;
          draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
          draw[0](canvas, wisp, screen.x - WISP_WIDTH / 2, screen.y - 24,
                  WISP_WIDTH, WISP_HEIGHT);
          draw[1](canvas, wisp, screen.x - WISP_WIDTH / 2, screen.y + 8,
                  WISP_WIDTH, WISP_HEIGHT);
          EFFECT_RELEASE_BLITTERS();
        }
      } else {
        // Six 40x40 pictures four frames apart, added to the canvas.
        if (frame >=
                member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_FRAME &&
            frame < member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_FRAME +
                        24) {
          s32 picture = (frame - member * STAT_BOOST_TARGET_DELAY -
                         STAT_BOOST_SPRITE_FRAME) /
                        4 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, BLITTER_ADDITIVE);
          draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
          draw[0](canvas,
                  state->sheet + picture * PICTURE_BYTES(SHIELD) +
                      STAT_BOOST_PICTURE,
                  screen.x - SHIELD_WIDTH / 2, screen.y - SHIELD_HEIGHT / 2,
                  SHIELD_WIDTH, SHIELD_HEIGHT);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      }
    }
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

/// The stat reductions' modes (BattleEffect_RunStatReduction).
enum StatReductionMode {
  STAT_REDUCTION_UNUSED, ///< a dagger, in the pink burst's colours; unused
  STAT_REDUCTION_IMPAIR, ///< a shield, in the glow orb's colours
  STAT_REDUCTION_WEAKEN, ///< a shield, in the pink burst's colours
  STAT_REDUCTION_DULL    ///< a dagger, in the glow orb's colours
};

/// Breaks the dagger sprite into fading shards as Dull does, in the pink
/// burst's colours; no effect table entry reaches it.
UNUSED void BattleEffect_RunUnusedStatReduction(void *action) {
  BattleEffect_RunStatReduction(action, STAT_REDUCTION_UNUSED);
}

/// A dagger appears over each target and breaks into shards that fade through
/// darker copies, in the glow orb's colours.
///
/// Abilities: Dull, Blunt.
void BattleEffect_RunDull(struct BattleEffectParams *args) {
  BattleEffect_RunStatReduction(args, STAT_REDUCTION_DULL);
}

/// A shield appears over each target and breaks into shards that fade through
/// darker copies, in the glow orb's colours.
///
/// Abilities: Impair, Debilitate.
void BattleEffect_RunImpair(struct BattleEffectParams *args) {
  BattleEffect_RunStatReduction(args, STAT_REDUCTION_IMPAIR);
}

/// A shield appears over each target and breaks into fading shards, in the pink
/// burst's colours.
///
/// Abilities: Weaken, Enfeeble.
void BattleEffect_RunWeaken(struct BattleEffectParams *args) {
  BattleEffect_RunStatReduction(args, STAT_REDUCTION_WEAKEN);
}

/// Each target throws STAT_REDUCTION_TARGET_SHARDS shards from its own run of
/// STAT_REDUCTION_TARGET_PARTICLES particles, STAT_REDUCTION_TARGET_DELAY
/// frames after the target before it.
#define STAT_REDUCTION_TARGET_PARTICLES 32
#define STAT_REDUCTION_TARGET_SHARDS 24
#define STAT_REDUCTION_TARGET_DELAY 4

/// A dagger (Dull) or a shield (Impair and Weaken) appears over each target
/// and breaks into shards that fly off and fade.
void BattleEffect_RunStatReduction(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  s32 j;
  s32 xOffset;
  s32 origin[3];
  struct ScreenPoint screen;
  FIXED point[3];
  DrawRectangle draw[2];
  s32 i;
  void *palette;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(0);
  if (mode == STAT_REDUCTION_UNUSED || mode == STAT_REDUCTION_DULL)
    Resource_LoadAndDecompress(RESOURCE_DAGGER_SHEET, state->sheet, 1, 1);
  else
    Resource_LoadAndDecompress(RESOURCE_SHIELD_SHEET, state->sheet, 1, 1);
  if (mode == STAT_REDUCTION_UNUSED)
    palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
  else if (mode == STAT_REDUCTION_DULL)
    palette = Resource_GetTableEntry(RESOURCE_GLOW_ORB_SHEET);
  else if (mode == STAT_REDUCTION_IMPAIR)
    palette = Resource_GetTableEntry(RESOURCE_GLOW_ORB_SHEET);
  else
    palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_PALETTE_RAMP_IMAGE,
                             state->sheet + STAT_REDUCTION_SHARD_PICTURES, 0,
                             0);

  for (i = 1; i != DIMMED_COPIES; i++) {
    for (j = 0; j != STAT_REDUCTION_SHARD_BYTES; j++) {
      s32 level = state->sheet[STAT_REDUCTION_SHARD_PICTURES + j];

      EFFECT_CAP_LEVEL(level, DIMMED_COPY_CAP(i));
      state->sheet[STAT_REDUCTION_SHARD_PICTURES +
                   i * STAT_REDUCTION_SHARD_BYTES + j] = level;
    }
  }

  if (args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
    xOffset = -NARROW_CANVAS_PARTY_X;
  } else {
    REG_BG2X = 0;
    xOffset = 0;
  }

  // The shards start in a grid where the sprite was, in rows 6 pixels apart
  // up from 10 below its centre, four to a row 2 pixels apart from 2 to one
  // side (dagger) or 8 apart from 16 (shield), and fly away from the user's
  // side, scattering and rising.
  for (i = 0; i != STAT_CHANGE_PARTICLES; i++) {
    struct Particle *shard = &PARTICLES[i];
    s32 radius = 192;
    s32 angle;

    angle = Random16() & TRIG_ANGLE_MASK;
    shard->x = 0;
    if (mode == STAT_REDUCTION_UNUSED || mode == STAT_REDUCTION_DULL) {
      shard->y =
          INTtoFIXED((i & (STAT_REDUCTION_TARGET_PARTICLES - 1)) / 4 * 6 - 10);
      shard->z = INTtoFIXED(i % 4 * 2 - 2);
    } else {
      shard->y =
          INTtoFIXED((i & (STAT_REDUCTION_TARGET_PARTICLES - 1)) / 4 * 6 - 10);
      shard->z = INTtoFIXED(i % 4 * 8 - 16);
    }
    if (args->side == EFFECT_SIDE_PARTY)
      shard->velocityX = toFIXED(2);
    else
      shard->velocityX = -toFIXED(2);
    shard->velocityY = ((Trig_Cos(angle) * radius) >> 6) + toFIXED(1);
    shard->velocityZ = (Trig_Sin(angle) * radius) >> 6;
    shard->variant = Random16() & 0xff;
  }

  BattleEffect_BuildCanvasBlitters(args->side, draw);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != args->targetCount * STAT_REDUCTION_TARGET_DELAY + 64;
       frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraState;

    // The loop runs count * STAT_REDUCTION_TARGET_DELAY + 64 frames, so frame
    // 72 comes only with three or more targets. With one or two,
    // BattlePlayback_WaitForReady starts playback once the effect returns, so
    // the messages follow the shards instead of overlapping them.
    if (frame == 72)
      EFFECT_SHOW_DAMAGE(0);
    for (j = 0; j != args->targetCount; j++) {
      struct MotionObject *object;
      s32 targetFrame;
      s32 x;
      s32 y;

      object = BattleObject_GetFar(args->targets[j])->object;
      targetFrame = frame - j * STAT_REDUCTION_TARGET_DELAY;
      if (targetFrame > 0) {
        // DEAD: the view set from the camera a first time, which the second
        // call repeats; the ROM keeps both calls.
        EFFECT_VIEW_FROM_CAMERA(camera);
        SET_VECTOR(point, object->x, toFIXED(20), object->z);
        EFFECT_VIEW_FROM_CAMERA(camera);
        Matrix_TranslateVector(point);
        SET_VECTOR(origin, 0, 0, 0);
        EffectPosition_ProjectPoint(origin, &screen);
        x = screen.x + xOffset;
        y = screen.y;
        if (mode == STAT_REDUCTION_UNUSED || mode == STAT_REDUCTION_DULL) {
          if (targetFrame <= 26)
            draw[0](canvas,
                    state->sheet + targetFrame / 4 % 7 * PICTURE_BYTES(DAGGER),
                    x - DAGGER_WIDTH / 2, y - DAGGER_HEIGHT / 2, DAGGER_WIDTH,
                    DAGGER_HEIGHT);
        } else {
          if (targetFrame <= 23)
            draw[1](canvas,
                    state->sheet + targetFrame / 4 % 6 * PICTURE_BYTES(SHIELD),
                    x - SHIELD_WIDTH / 2, y - SHIELD_HEIGHT / 2, SHIELD_WIDTH,
                    SHIELD_HEIGHT);
        }
        // The chime as the sprite breaks.
        if (targetFrame == 24)
          Sound_PlayFar(SE_CRACK);

        // The shards fly for 36 frames, a shade darker every four.
        if (targetFrame >= 24 && targetFrame <= 60) {
          s32 level;

          level = 0;
          if (targetFrame > 28) {
            level = (targetFrame - 24) / 4;
            if (level > DIMMED_COPIES - 1)
              level = DIMMED_COPIES - 1;
          }
          for (i = 0; i != STAT_REDUCTION_TARGET_SHARDS; i++) {
            struct Particle *shard =
                &PARTICLES[j * STAT_REDUCTION_TARGET_PARTICLES + i];
            s32 picture;

            picture = i % 4 * 3 + (shard->variant + targetFrame) / 8 % 3;
            EffectPosition_ProjectPoint((s32 *)shard, &screen);
            x = screen.x + xOffset;
            y = screen.y;
            draw[0](canvas,
                    state->sheet +
                        (level * STAT_REDUCTION_SHARD_BYTES +
                         gStatChange_ShardPictures[picture]) +
                        STAT_REDUCTION_SHARD_PICTURES,
                    x, y, gStatChange_ShardWidths[picture],
                    gStatChange_ShardHeights[picture]);
            Particle_AdvanceWithGravity3D(shard, 60, 0);
          }
        }
      }
    }
    EFFECT_PRESENT_CANVAS(state);
  }

  EFFECT_FINISH_CANVAS();
}

#undef STAT_BOOST_BURST_FRAME
#undef STAT_BOOST_SPRITE_FRAME
#undef STAT_BOOST_TARGET_DELAY
#undef STAT_BOOST_CRUCIBLE_FIRE_FRAME
#undef STAT_CHANGE_PARTICLES
#undef STAT_BOOST_TARGET_PARTICLES
#undef STAT_BOOST_TARGET_FLARES
#undef STAT_BOOST_FLARE_SPIN
#undef STAT_BOOST_FLARE_SIZE_MAX
#undef STAT_BOOST_FLARE_DEPTH_NEAR
#undef STAT_BOOST_FLARE_DEPTH_STEP
#undef STAT_BOOST_FLARE_DEPTH_FAR
#undef STAT_BOOST_DJINNI_SWAY
#undef STAT_BOOST_PARTY_CANVAS_X
#undef STAT_REDUCTION_TARGET_PARTICLES
#undef STAT_REDUCTION_TARGET_SHARDS
#undef STAT_REDUCTION_TARGET_DELAY
