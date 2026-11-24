/// @file
/// Hail and Ground.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define HAIL_CHIPS 64 ///< ice chips in PARTICLES

#define GROUND_FRAMES 96 ///< frames of Ground
#define HAIL_FRAMES 64   ///< frames of Hail

#define HAIL_CHIP_PICTURES                                                     \
  PICTURE_BYTES(WATER_WAVE) ///< the ice chips, after the wave
/// PortalSheet's portal.
#define PORTAL_WIDTH 24
#define PORTAL_HEIGHT 48
#define HAIL_DJINNI_PICTURE 26048 ///< the small Mercury Djinni

/// After a Mercury Djinni enters, ice chips burst out of the first target and
/// waves of water rise along the bottom of the screen.
///
/// Abilities: Hail.
void BattleEffect_RunHail(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  struct ScreenPoint origin;
  s32 screenY;
  s32 screenX;
  BlitterPair draw;
  struct Particle *chip;
  s32 i;
  s32 frame;
  s32 rise;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(args, ELEMENT_MERCURY, state->args->side,
                                 DJINNI_ANCHOR_OVER_USER, &screenX, &screenY);
  BattleEffect_BuildCanvasBlitters(state->args->side, draw);
  Resource_LoadAndDecompress(RESOURCE_WATER_SPRAY_SHEET, state->sheet, 1, 1);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  Projection_Unit(state->args->targets[0], &origin);

  // The chips burst up from just above the first target.
  for (i = 0; i != HAIL_CHIPS; i++) {
    s32 angle;
    s32 speed;

    angle = (Random16() & 0x7fff) + DEGtoANG(90);
    speed = (Random16() & 0x1ff) + 128;
    PARTICLES[i].x = INTtoFIXED((origin.x / 2 + (Random16() & 0xf)) - 8);
    PARTICLES[i].y = INTtoFIXED(origin.y + 8);
    PARTICLES[i].velocityX = (Trig_Sin(angle) * speed) >> 9;
    PARTICLES[i].velocityY = (Trig_Cos(angle) * speed) >> 6;
    PARTICLES[i].z = Random16() & 0x7f;
    PARTICLES[i].velocityZ = Random16() & 0x7f;
    PARTICLES[i].variant = (Random16() & 0xf) + 32;
  }

  for (frame = 0; frame != HAIL_FRAMES; frame++) {
    if (frame > 47)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(64 - frame);
    // The chips and the Djinni load on frame 1, so on frame 0 the prelude draws
    // whatever the sprite sheet holds.
    if (frame == 1) {
      Resource_LoadAndDecompress(RESOURCE_ICE_CHIP_SHEET,
                                 state->sheet + HAIL_CHIP_PICTURES, 1, 1);
      Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
                                 state->sheet + HAIL_DJINNI_PICTURE, 1, 0);
    }

    // The small Djinni circles.
    if (state->args->prelude == 1) {
      s32 orbit;
      s32 x;
      s32 y;

      orbit = frame * (TRIG_TURN / 32);
      x = FIXEDtoINT(-Trig_Sin(orbit) * 4) + screenX / 2 - 10;
      y = FIXEDtoINT(Trig_Cos(orbit) * 2) + screenY - 22;
      // DEAD: frame never passes 69, the loop ending at frame 63; the ROM
      // keeps the compare and the rise.
      if (frame > 69)
        y = (y - frame * 2) + 138;
      draw[1](canvas, state->sheet + HAIL_DJINNI_PICTURE, x, y,
              SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      if (frame <= 3)
        draw[1](canvas, state->sheet + HAIL_DJINNI_PICTURE, x, y,
                SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
    }

    // The chips fly up, odd ones mirrored.
    for (i = 0; i != HAIL_CHIPS; i++) {
      chip = &PARTICLES[i];
      if (frame >= i / 4 + 4) {
        s32 picture;
        s32 width;
        s32 height;

        picture = (chip->z / 128) & 3;
        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gHail_PictureWidths[picture];
        height = gHail_PictureHeights[picture];
        draw[i & 1](canvas,
                    state->sheet + HAIL_CHIP_PICTURES + gHail_Pictures[picture],
                    FIXEDtoINT(chip->x) - width / 2,
                    FIXEDtoINT(chip->y) - height / 2, width, height);
#else
        draw[i & 1](
            canvas, state->sheet + HAIL_CHIP_PICTURES + gHail_Pictures[picture],
            FIXEDtoINT(chip->x) - (width = gHail_PictureWidths[picture]) / 2,
            FIXEDtoINT(chip->y) - (height = gHail_PictureHeights[picture]) / 2,
            width, height);
#endif
        Particle_AdvanceWithGravity3D(chip, 63, toFIXED(1) / 16);
      }
    }

    if (frame == 8) {
      state->shakeFrames = frame;
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      BattleMotion_ApplyKnockbackFar(state->args->targets[0], KNOCKBACK_LAUNCH);
    }

    // Waves rise along the bottom, drifting away from the user.
    rise = frame * 4;
    if (rise > WATER_WAVE_HEIGHT)
      rise = WATER_WAVE_HEIGHT;
    if (state->args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != 5; i++)
        draw[0](canvas, state->sheet, (i << 5) - (frame / 4 & 31), 120 - rise,
                WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
    } else {
      for (i = 0; i != 5; i++)
        draw[0](canvas, state->sheet,
                ((i << 5) + (frame / 4 & 31)) - WATER_WAVE_WIDTH, 120 - rise,
                WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
    }

    BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

/// The small Venus Djinni in the portal sprite sheet.
#define GROUND_DJINNI_PICTURE 26048
/// The portals set out in state->particles from 0, and the first of them, the
/// ones drawn.
#define GROUND_PORTALS 8
#define GROUND_PORTALS_DRAWN 6
/// The crackles round the portals in state->particles from
/// GROUND_FIRST_CRACKLE, GROUND_CRACKLES_PER_PORTAL a portal.
#define GROUND_FIRST_CRACKLE 32
#define GROUND_CRACKLES_PER_PORTAL 4
#define GROUND_CRACKLES (GROUND_PORTALS * GROUND_CRACKLES_PER_PORTAL)

/// After a Venus Djinni enters, six glowing portals leave the user, swirl and
/// close in on the first target, each striking it as it arrives.
///
/// Abilities: Ground.
void BattleEffect_RunGround(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 i;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  struct Actor *target;
  struct Actor *user;
  struct ScreenPoint position;
  s32 screenX;
  s32 screenY;
  s32 j;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(args, ELEMENT_VENUS, state->args->side,
                                 DJINNI_ANCHOR_OVER_USER, &screenX, &screenY);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (state->args->side == EFFECT_SIDE_PARTY)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Resource_LoadAndDecompress(RESOURCE_PORTAL_SHEET, state->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
                             state->sheet + GROUND_DJINNI_PICTURE, 1, 1);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  user = BattleObject_GetFar(state->args->user)->actor;
  target = BattleObject_GetFar(state->args->targets[0])->actor;
  // The portals fan out from above the user.
  // DEAD: the last two portals are set out but never drawn or moved; the ROM
  // keeps their stores.
  for (i = 0; i != GROUND_PORTALS; i++) {
    struct Particle *portal = &state->particles[i];

    SET_POSITION(portal, user->x.fixed, toFIXED(66), user->z.fixed);
    SET_VELOCITY(portal, (i * toFIXED(80)) >> 5,
                 (s32)INTtoFIXED((Random16() & 127) - 64) >> 6,
                 (s32)INTtoFIXED((Random16() & 255) - 127) >> 5);
    if (portal->x > 0)
      portal->velocityX = -portal->velocityX;
    portal->variant = 1;
  }
  for (frame = 0; frame != GROUND_FRAMES; frame++) {
    if (frame > 16)
      BattleEffect_StepPaletteToResource(RESOURCE_PORTAL_SHEET);
    // The Djinni circles above the user, then rises away.
    if (state->args->prelude == 1) {
      s32 angle = frame * (TRIG_TURN / 32);
      s32 x = FIXEDtoINT(-Trig_Sin(angle) * 4) + screenX / 2 - 10;
      s32 y = FIXEDtoINT(Trig_Cos(angle) * 2) + screenY - 22;

      if (frame > 16)
        y = y - frame * 2 + 32;
      if (state->args->side == EFFECT_SIDE_PARTY)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                            COMPOSITE_ADDITIVE);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_ADDITIVE);
      if (frame <= 3)
        EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(
            canvas, state->sheet + GROUND_DJINNI_PICTURE, x, y,
            SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      draw[0](canvas, state->sheet + GROUND_DJINNI_PICTURE, x, y,
              SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
    }
    // Crackles flicker round each portal.
    // DEAD: the crackles of the two portals never drawn are rolled too; the ROM
    // keeps the rolls.
    if ((frame & 1) == 0) {
      for (i = 0; i != GROUND_CRACKLES; i++) {
        struct Particle *crackle = &state->particles[GROUND_FIRST_CRACKLE + i];

        crackle->velocityX = Random16() % 6 + 3;
        crackle->velocityY = gGround_CrackleFlips[Random16() & 3];
      }
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != GROUND_PORTALS_DRAWN; i++) {
      struct Particle *portal = &state->particles[i];

      if (portal->variant == 1) {
        if (frame > i * 2) {
          s32 x;
          s32 y;

          Projection_Point(&portal->x, &position);
          position.x >>= 1;
          x = position.x - PORTAL_WIDTH / 2;
          y = position.y - PORTAL_HEIGHT / 2;
          draw[0](canvas, state->sheet, x, y, PORTAL_WIDTH, PORTAL_HEIGHT);
          if ((frame & 3) <= 1)
            draw[0](canvas, state->sheet + gGround_PortalPictures[1],
                    x + gGround_PortalX[1], y + gGround_PortalY[1],
                    gGround_PortalWidths[1], gGround_PortalHeights[1]);
          else
            draw[0](canvas, state->sheet + gGround_PortalPictures[2],
                    x + gGround_PortalX[2], y + gGround_PortalY[2],
                    gGround_PortalWidths[2], gGround_PortalHeights[2]);
          for (j = 0; j != GROUND_CRACKLES_PER_PORTAL; j++) {
            struct Particle *crackle =
                &state->particles[GROUND_FIRST_CRACKLE +
                                  i * GROUND_CRACKLES_PER_PORTAL + j];
            s32 crackleX;
            s32 crackleY;

            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, crackle->velocityY,
                                COMPOSITE_LIGHTEN);
            draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
            if (crackle->velocityY & 4)
              crackleX = x - gGround_PortalWidths[crackle->velocityX] -
                         gGround_PortalX[crackle->velocityX] + 24;
            else
              crackleX = x + gGround_PortalX[crackle->velocityX];
            if (crackle->velocityY & 8)
              crackleY = y - gGround_PortalHeights[crackle->velocityX] -
                         gGround_PortalY[crackle->velocityX] + 48;
            else
              crackleY = y + gGround_PortalY[crackle->velocityX];
            draw[1](canvas,
                    state->sheet + gGround_PortalPictures[crackle->velocityX],
                    crackleX, crackleY,
                    gGround_PortalWidths[crackle->velocityX],
                    gGround_PortalHeights[crackle->velocityX]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
          }
          portal->x += portal->velocityX;
          portal->y += portal->velocityY;
          portal->z += portal->velocityZ;
        }
        // The portal homes in on the target and strikes it.
        if (frame > i * 2 + 16) {
          portal->velocityX += (target->x.fixed - portal->x) >> 8;
          portal->velocityY += (toFIXED(20) - portal->y) >> 8;
          portal->velocityZ += (target->z.fixed - portal->z) >> 8;
          if (frame < i * 2 + 85) {
            SET_VELOCITY(portal, portal->velocityX * 60 / DAMPING_SCALE,
                         portal->velocityY * 60 / DAMPING_SCALE,
                         portal->velocityZ * 60 / DAMPING_SCALE);
          }
          if (portal->y < toFIXED(20)) {
            state->shakeFrames = 8;
            portal->variant = 0;
            Sound_PlayFar(SE_HEAVY_IMPACT);
            EFFECT_STRIKE_TARGET(state->args, 0, 4, KNOCKBACK_HURL);
          }
        }
      }
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Shakes the battle view by up to rangeX and rangeY pixels while the effect's
/// shakeFrames last, then puts it back where it was.
void BattleCamera_Shake(s32 rangeX, u32 rangeY) {
  struct BattleEffectState *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

  if (state->shakeFrames > 0) {
    s32 randomX = (rangeX - 1) & Random16();
    s32 halfRange = (s32)rangeY / 2;
    s32 offsetY = ((rangeY - 1) & Random16()) - halfRange;
    s32 offsetX = randomX - halfRange;

    gBgScroll[1].x = offsetX;
    gBgScroll[1].y = offsetY + BATTLE_BACKGROUND_SCROLL_Y;
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X - offsetX;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y - offsetY;
    state->shakeFrames--;
    return;
  }
  gBgScroll[1].x = state->savedBg1X;
  gBgScroll[1].y = state->savedBg1Y;
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
}

#undef HAIL_CHIPS
#undef GROUND_FRAMES
#undef HAIL_FRAMES
#undef HAIL_CHIP_PICTURES
#undef PORTAL_WIDTH
#undef PORTAL_HEIGHT
#undef HAIL_DJINNI_PICTURE
#undef GROUND_DJINNI_PICTURE
#undef GROUND_FIRST_CRACKLE
#undef GROUND_CRACKLES
#undef GROUND_CRACKLES_PER_PORTAL
#undef GROUND_PORTALS
#undef GROUND_PORTALS_DRAWN
