/// @file
/// Haunt, the orbiting spirals, and the evil spirits of Demon Night, Rumble,
/// Haunting and Thorny Grave.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "system/system.h"
#include "resource_ids.h"

/// The spirits scattered at the start, up to 64 pixels from their centre.
#define HAUNT_SCATTERED 512
#define HAUNT_SPIRITS 8 ///< evil spirits drawn around each target
#define HAUNT_SLOTS 64  ///< particles reserved for each target

/// Evil spirits (SkullSheet's skulls, smeared pink by the ripple) close in on
/// each target in turn.
///
/// Abilities: Haunt.
void BattleEffect_RunHaunt(struct BattleEffectParams *args) {
  s32 k;
  struct ScreenPoint screenPosition;
  FIXED point[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  struct BattleCamera *camera;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_SKULL_SHEET, work->sheet, 1, 1);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET),
                       SHEET_PALETTE_BYTES);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

  // Scatter every spirit up to 64 pixels from its centre.
  for (i = 0; i != HAUNT_SCATTERED; i++) {
    SET_POSITION(&PARTICLES[i], ((Random16() & 255) - 127) << 15,
                 ((Random16() & 255) - 127) << 15,
                 ((Random16() & 255) - 127) << 15);
  }
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != args->targetCount * 32 + 64; frame++) {
    s32 *row;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 92)
      EFFECT_SHOW_DAMAGE(0);

    // Ripple the canvas line by line.
    row = work->bg2xLines;
    if (args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = CANVAS_WAVE_BG2X(toFIXED(6) -
                                  Trig_Sin((frame + i) * (TRIG_TURN / 32)) * 6);
    } else {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = CANVAS_WAVE_BG2X(Trig_Sin((frame + i) * (TRIG_TURN / 32)) * 6);
    }

    EFFECT_FOR_EACH_TARGET(k, args) {
      struct MotionObject *target =
          BattleObject_GetFar(args->targets[k])->object;

      // Spin the spirits about a point 20 pixels above the target.
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, target->x, toFIXED(20), target->z);
      Matrix_TranslateVector(point);
      if (frame > k * 32) {
        Matrix_RotateX(frame << 9);
        if (frame == k * 32 + 32)
          BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, k, 32);

        // Each spirit steps a part of its distance nearer the centre, and is
        // gone once it arrives.
        for (i = 0; i != HAUNT_SPIRITS; i++) {
          struct Particle *spirit = &PARTICLES[k * HAUNT_SLOTS + i];

          if (frame > (k * 8 + i) * 4) {
            s32 x = (spirit->x >> 8) * (spirit->x >> 8);
            s32 y = (spirit->y >> 8) * (spirit->y >> 8);
            s32 z = (spirit->z >> 8) * (spirit->z >> 8);
            s32 dist = Iwram_Sqrt(x + y + z) >> 8;

            if (dist != 0) {
              EffectPosition_ProjectPoint((s32 *)spirit, &screenPosition);
              screenPosition.x >>= 1;
              draw[0](canvas, work->sheet + i % 3 * PICTURE_BYTES(SKULL),
                      screenPosition.x - SKULL_WIDTH / 2,
                      screenPosition.y - SKULL_HEIGHT / 2, SKULL_WIDTH,
                      SKULL_HEIGHT);
              spirit->x -= spirit->x / dist;
              spirit->y -= spirit->y / dist;
              spirit->z -= spirit->z / dist;
              spirit->variant++;
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

/// The spirals: ORBITING_SPIRALS a quarter turn apart, circling
/// ORBITING_SPIRALS_RADIUS pixels out, each target's ORBITING_SPIRALS_INTERVAL
/// frames after the one before and for ORBITING_SPIRALS_LIFE frames less one.
#define ORBITING_SPIRALS 4
#define ORBITING_SPIRALS_RADIUS 16
#define ORBITING_SPIRALS_INTERVAL 16
#define ORBITING_SPIRALS_LIFE 60
/// The canvas's scroll on the party side until the first frame's wave rows
/// replace it, 8 pixels short of NARROW_CANVAS_PARTY_X.
#define ORBITING_SPIRALS_PARTY_START_X (NARROW_CANVAS_PARTY_X - 8)

/// Unused: no row of gAbilityBattleEffects in either game names its kind, and
/// the handler table is reached with no other kind (BattleEffect_RunFromTable),
/// so the player never sees it. Its code draws the spirals about the point 40
/// pixels above each target, one target after another, turning a 128th of a
/// turn a frame; each target takes its damage pose 32 frames into its turn.
void BattleEffect_RunUnusedOrbitingSpirals(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  FIXED point[3];
  struct ScreenPoint screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(0);
  Resource_LoadAndDecompress(RESOURCE_SPIRAL_SHEET, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  if (args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(ORBITING_SPIRALS_PARTY_START_X << 8);
    offsetX = -NARROW_CANVAS_PARTY_X;
  } else {
    offsetX = 0;
  }

  for (frame = 0; frame != (args->targetCount * ORBITING_SPIRALS_INTERVAL) + 48;
       frame++) {
    struct BattleCamera *camera;
    s32 *row;
    s32 i;

    camera = Ram_HeapSlots->cameraWork;
    row = work->bg2xLines;
    if (args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ = CANVAS_WAVE_BG2X(toFIXED(8) -
                                  Trig_Sin((frame + i) * (TRIG_TURN / 64)) * 8);
    } else {
      for (i = 0; i != DISPLAY_HEIGHT; i++)
        *row++ =
            CANVAS_WAVE_BG2X(Trig_Sin((frame + i) * (TRIG_TURN / 64)) * 8) -
            (NARROW_CANVAS_PARTY_X << 8);
    }

    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_FOR_EACH_TARGET(member, args) {
      struct MotionObject *target;

      target = BattleObject_GetFar(args->targets[member])->object;
      if (frame > member * ORBITING_SPIRALS_INTERVAL &&
          frame <
              (member * ORBITING_SPIRALS_INTERVAL) + ORBITING_SPIRALS_LIFE) {
        s32 spin;

        if (frame == (member * ORBITING_SPIRALS_INTERVAL) + 32)
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_NORMAL,
                                   POSE_DAMAGE, TINT_KEEP, 0);
        SET_VECTOR(point, target->x, toFIXED(40), target->z);
        EffectPosition_ProjectPoint(point, &screen);

        for (i = 0; i != ORBITING_SPIRALS; i++) {
          s32 x;
          s32 y;
          s32 half;

          spin = frame * (TRIG_TURN / 128) + i * (TRIG_TURN / ORBITING_SPIRALS);
          x = (screen.x +
               FIXEDtoINT(Trig_Sin(spin) * ORBITING_SPIRALS_RADIUS)) +
              offsetX;
          y = screen.y + FIXEDtoINT(Trig_Cos(spin) * ORBITING_SPIRALS_RADIUS);
          half = frame / 16;
          draw[half & 1](canvas,
                         work->sheet +
                             (frame / 4 - half * 4) * PICTURE_BYTES(SPIRAL),
                         x - SPIRAL_WIDTH / 2, y - SPIRAL_HEIGHT / 2,
                         SPIRAL_WIDTH, SPIRAL_HEIGHT);
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

/// The evil spirits' modes (BattleEffect_RunGatheringLights), each named for
/// the effect that plays it, and its row of gGatheringLights_Counts.
enum GatheringSpiritsMode {
  GATHERING_SPIRITS_DEMON_NIGHT, ///< pink
  GATHERING_SPIRITS_RUMBLE,      ///< Mars Djinn colours
  GATHERING_SPIRITS_HAUNTING,    ///< Mercury Djinn colours
  GATHERING_SPIRITS_THORNY_GRAVE ///< pink, the ramp reversed
};

/// Sixteen pink spirits burst out of the user and are pulled into each target.
///
/// Abilities: Demon Night.
void BattleEffect_RunDemonNight(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_SPIRITS_DEMON_NIGHT);
}

/// Eight spirits in Mars Djinn colours burst out of the user and are pulled
/// into each target.
///
/// Abilities: Rumble.
void BattleEffect_RunRumble(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_SPIRITS_RUMBLE);
}

/// Twelve spirits in Mercury Djinn colours burst out of the user and are
/// pulled into each target.
///
/// Abilities: Haunting.
void BattleEffect_RunHaunting(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_SPIRITS_HAUNTING);
}

/// Sixteen dark spirits edged in white burst out of the user, are pulled into
/// each target and push it back as they arrive.
///
/// Abilities: Thorny Grave.
void BattleEffect_RunThornyGrave(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_SPIRITS_THORNY_GRAVE);
}

#define GATHERING_SPIRITS_SLOTS 128 ///< particles reserved for each target
/// The targets' turns, this many frames apart: each is struck by its spirits
/// from GATHERING_SPIRITS_HIT_FRAME.
#define GATHERING_SPIRITS_TARGET_FRAMES 20
#define GATHERING_SPIRITS_HIT_FRAME 70
/// The background's widest sway, in pixels, at the effect's midpoint.
#define GATHERING_SPIRITS_SWAY 16
/// A layer's BGnHOFS and BGnVOFS as one word, as an HBlank DMA writes them.
#define BG_OFFSETS(horizontal, vertical) ((horizontal) | (vertical) << 16)
/// The spirits' pictures, three of the sheet's four drawn.
#define SPIRIT_WIDTH 20
#define SPIRIT_HEIGHT 32

/// For each target in turn, spirits burst out of the user every way and from
/// 30 frames on are pulled toward the middle of the target's body, vanishing
/// once they have crossed it, in the mode's colours. The battle background
/// sways sideways, swelling and dying away over the effect. In Thorny Grave
/// every spirit that arrives also pushes its target back.
void BattleEffect_RunGatheringLights(struct BattleEffectParams *args,
                                     s32 mode) {
  struct ScreenPoint screenPosition;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  struct BattleCamera *camera;
  s32 paletteId;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_MEMBER_BURST_IMAGE, work->sheet, 0, 0);
  if (mode == GATHERING_SPIRITS_DEMON_NIGHT ||
      mode == GATHERING_SPIRITS_THORNY_GRAVE)
    paletteId = RESOURCE_PINK_BURST_SHEET;
  else if (mode == GATHERING_SPIRITS_RUMBLE)
    paletteId = RESOURCE_MARS_DJINN_SHEET;
  else
    paletteId = RESOURCE_MERCURY_DJINN_SHEET;
  LOAD_BG_PALETTE(paletteId);
  if (mode == GATHERING_SPIRITS_THORNY_GRAVE)
    Palette_MirrorBgColors();
  BattleEffect_FetchRectangleBlitters(args->side, draw);

  // The spirits fly off the user's body every way, at up to two pixels a
  // frame.
  EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
  EFFECT_FOR_EACH_TARGET(k, args) {
    struct MotionObject *source = BattleObject_GetFar(args->user)->object;
    s32 height = BattleUnit_GetEffectHeightFar(args->user);

    for (i = 0; i != GATHERING_SPIRITS_SLOTS; i++) {
      struct Particle *spirit = &PARTICLES[k * GATHERING_SPIRITS_SLOTS + i];

      SET_POSITION(spirit, source->x, height, source->z);
      SET_VELOCITY(spirit, ((Random16() & 255) - 128) << 10,
                   ((Random16() & 255) - 128) << 10,
                   ((Random16() & 255) - 128) << 10);
      spirit->variant = 0;
    }
  }

  Task_Add(BattleEffect_ArmBg1HBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_ENEMY_CRY);

  for (frame = 0;
       frame != gGatheringLights_Counts[mode * 2 + 1] +
                    args->targetCount * GATHERING_SPIRITS_TARGET_FRAMES;
       frame++) {
    s32 *row;
    s32 sway;
    s32 scrollX;

    if (frame == 80) {
      if (mode == GATHERING_SPIRITS_DEMON_NIGHT)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else if (mode == GATHERING_SPIRITS_THORNY_GRAVE)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);

    // The battle background sways line by line, by up to
    // GATHERING_SPIRITS_SWAY pixels at the effect's midpoint. A negative
    // scroll wraps into the halfword one short of 65536.
    row = work->bg2xLines;
    sway = FIXEDtoINT(
        Trig_Sin(frame * (TRIG_TURN / 2) /
                 (gGatheringLights_Counts[mode * 2 + 1] +
                  args->targetCount * GATHERING_SPIRITS_TARGET_FRAMES)) *
        GATHERING_SPIRITS_SWAY);
    scrollX = gBgScroll[1].x;
    for (i = 0; i != DISPLAY_HEIGHT; i++) {
      s32 x = (FIXEDtoINT(Trig_Sin((frame + i) * (TRIG_TURN / 64)) * sway)) +
              scrollX;

      if (x < 0)
        x += 65535;
      *row++ = BG_OFFSETS(x, BATTLE_BACKGROUND_SCROLL_Y);
    }

    EFFECT_FOR_EACH_TARGET(k, args) {
      struct MotionObject *target =
          BattleObject_GetFar(args->targets[k])->object;
      s32 half = BattleUnit_GetEffectHeightFar(args->targets[k]) / 2;

      if (frame == k * GATHERING_SPIRITS_TARGET_FRAMES +
                       GATHERING_SPIRITS_HIT_FRAME + 1) {
        if (mode == GATHERING_SPIRITS_DEMON_NIGHT)
          Sound_PlayFar(SE_HEAVY_IMPACT);
        else if (mode != GATHERING_SPIRITS_THORNY_GRAVE)
          Sound_PlayFar(SE_IMPACT);
      }
      if (frame ==
          k * GATHERING_SPIRITS_TARGET_FRAMES + GATHERING_SPIRITS_HIT_FRAME)
        BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, k, 26);
      if (frame > k * GATHERING_SPIRITS_TARGET_FRAMES) {
        for (i = 0; i != gGatheringLights_Counts[mode * 2]; i++) {
          struct Particle *spirit = &PARTICLES[k * GATHERING_SPIRITS_SLOTS + i];

          if (frame > k * GATHERING_SPIRITS_TARGET_FRAMES + i * 2 &&
              spirit->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)spirit, &screenPosition);
            screenPosition.x >>= 1;
            draw[0](canvas, work->sheet + i % 3 * PICTURE_BYTES(SPIRIT),
                    screenPosition.x - SPIRIT_WIDTH / 2,
                    screenPosition.y - SPIRIT_HEIGHT / 2, SPIRIT_WIDTH,
                    SPIRIT_HEIGHT);
            Particle_AdvanceWithGravity3D(spirit, 62, 0);

            if (frame > k * GATHERING_SPIRITS_TARGET_FRAMES + i + 30) {
              s32 x = spirit->x;
              s32 dx = (target->x - x) >> 8;
              s32 dy = (target->y + half - spirit->y) >> 9;
              s32 dz = (target->z - spirit->z) >> 8;
              s32 targetDistance;
              s32 spiritDistance;
              s32 crossed;

              spirit->velocityX += dx;
              spirit->velocityY += dy;
              spirit->velocityZ += dz;
              // It has crossed once it is as far out from the middle of the
              // field as its target is, on the target's side.
              targetDistance = target->x < 0 ? -target->x : target->x;
              spiritDistance = x;
              if (spiritDistance < 0)
                spiritDistance = -spiritDistance;
              /* FAKEMATCH(dead): each arm stores its test in crossed, never
               * read. */
              if (targetDistance <= spiritDistance &&
                  ((x < 0 && (crossed = target->x < 0)) ||
                   (x >= 0 && (crossed = target->x >= 0)))) {
                spirit->variant = -1;
                if (mode == GATHERING_SPIRITS_THORNY_GRAVE) {
                  BattleEffect_StartActorMotion(args->targets[k], 1, toFIXED(2),
                                                toFIXED(2), toFIXED(2),
                                                i * 8 + 100);
                  /* FAKEMATCH(dead): spiritDistance set to 0, never read
                   * again. */
                  spiritDistance = 0;
                  if ((i & 1) && frame != 80)
                    Sound_PlayFar(SE_HEAVY_IMPACT);
                }
              }
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ArmBg1HBlankDma);
  BattleEffect_EndCanvasLayer();
}

#undef HAUNT_SCATTERED
#undef HAUNT_SPIRITS
#undef HAUNT_SLOTS
#undef ORBITING_SPIRALS
#undef ORBITING_SPIRALS_RADIUS
#undef ORBITING_SPIRALS_INTERVAL
#undef ORBITING_SPIRALS_LIFE
#undef ORBITING_SPIRALS_PARTY_START_X
#undef GATHERING_SPIRITS_SLOTS
#undef GATHERING_SPIRITS_TARGET_FRAMES
#undef GATHERING_SPIRITS_HIT_FRAME
#undef GATHERING_SPIRITS_SWAY
#undef BG_OFFSETS
#undef SPIRIT_WIDTH
#undef SPIRIT_HEIGHT
