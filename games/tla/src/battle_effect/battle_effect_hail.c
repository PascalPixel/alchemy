/// @file
/// Hail and Ground, and the battle camera's shakes.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define HAIL_CHIPS 64 ///< ice chips in PARTICLES

#define GROUND_FRAMES 96 ///< frames of Ground
#define HAIL_FRAMES 64   ///< frames of Hail
/// Hail's last frames, which fade the canvas out.
#define HAIL_FADE_FRAMES 16

/// The ice chips, after the wave.
#define HAIL_CHIP_PICTURES PICTURE_BYTES(WATER_WAVE)
/// PortalSheet's portal.
#define PORTAL_WIDTH 24
#define PORTAL_HEIGHT 48
#define HAIL_DJINNI_PICTURE 26048 ///< the small Mercury Djinni

/// After a Mercury Djinni enters, ice chips burst out of the first target and
/// waves of water rise along the bottom of the screen.
///
/// Abilities: Hail.
void BattleEffect_RunHail(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 screen[3];
  s32 world[3];
  struct ScreenPoint origin;
  BlitterPair draw;
  struct Particle *chip;
  s32 i;
  s32 frame;
  s32 rise;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_MERCURY, args->side, DJINNI_ANCHOR_OVER_USER),
      screen, world);
  BattleEffect_FetchRectangleBlitters(args->side, draw);
  Resource_LoadAndDecompress(RESOURCE_WATER_SPRAY_SHEET, work->sheet, 1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The chips burst up from just above the first target.
  EffectPosition_ProjectBattler(args->targets[0], &origin);
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
    if (frame >= HAIL_FRAMES - HAIL_FADE_FRAMES)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(HAIL_FRAMES - frame);
    // The chips and the Djinni load on frame 1, so on frame 0 the prelude draws
    // whatever the sprite sheet holds.
    if (frame == 1) {
      Resource_LoadAndDecompress(RESOURCE_ICE_CHIP_SHEET,
                                 work->sheet + HAIL_CHIP_PICTURES, 1, 1);
      Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
                                 work->sheet + HAIL_DJINNI_PICTURE, 1, 0);
    }

    // The small Djinni circles.
    if (args->prelude == 1) {
      s32 orbit;
      s32 x;
      s32 y;

      orbit = frame * (TRIG_TURN / 32);
      x = FIXEDtoINT(-Trig_Sin(orbit) * 4) + screen[0] / 2 - 10;
      y = FIXEDtoINT(Trig_Cos(orbit) * 2) + screen[1] - 22;
      // DEAD: frame never passes 69, the loop ending at frame 63; the ROM
      // keeps the compare and the rise.
      if (frame > 69)
        y = (y - frame * 2) + 138;
      draw[1](canvas, work->sheet + HAIL_DJINNI_PICTURE, x, y,
              SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      if (frame <= 3)
        draw[1](canvas, work->sheet + HAIL_DJINNI_PICTURE, x, y,
                SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
    }

    // The chips fly up, odd ones mirrored.
    for (i = 0, chip = PARTICLES; i != HAIL_CHIPS; i++, chip++) {
      if (frame >= i / 4 + 4) {
        s32 picture;
        u32 width;
        u32 height;

        picture = (chip->z / 128) & 3;
        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gHail_PictureWidths[picture];
        height = gHail_PictureHeights[picture];
        draw[i & 1](canvas,
                    work->sheet + HAIL_CHIP_PICTURES + gHail_Pictures[picture],
                    FIXED_INTEGER_PART(chip->x) - width / 2,
                    FIXED_INTEGER_PART(chip->y) - height / 2, width, height);
#else
        draw[i & 1](canvas,
                    work->sheet + HAIL_CHIP_PICTURES + gHail_Pictures[picture],
                    FIXED_INTEGER_PART(chip->x) -
                        (width = gHail_PictureWidths[picture]) / 2,
                    FIXED_INTEGER_PART(chip->y) -
                        (height = gHail_PictureHeights[picture]) / 2,
                    width, height);
#endif
        Particle_AdvanceWithGravity3D(chip, 63, toFIXED(1) / 16);
      }
    }

    if (frame == 8) {
      work->shakeFrames = frame;
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      EFFECT_STRIKE_TARGET(args, 0, 16, KNOCKBACK_LAUNCH);
    }

    // Waves rise along the bottom, drifting away from the user.
    rise = frame * 4;
    if (rise > WATER_WAVE_HEIGHT)
      rise = WATER_WAVE_HEIGHT;
    if (args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != 5; i++)
        draw[0](canvas, work->sheet, (i << 5) - (frame / 4 & 31), 120 - rise,
                WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
    } else {
      for (i = 0; i != 5; i++)
        draw[0](canvas, work->sheet,
                ((i << 5) + (frame / 4 & 31)) - WATER_WAVE_WIDTH, 120 - rise,
                WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
    }

    BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

/// The small Venus Djinni in the portal sprite sheet.
#define GROUND_DJINNI_PICTURE 26048
/// The crackles round the portals, four a portal, in work->particles from
/// GROUND_FIRST_CRACKLE.
#define GROUND_FIRST_CRACKLE 32
#define GROUND_CRACKLES 32
#define GROUND_CRACKLES_PER_PORTAL 4
#define GROUND_PORTALS 6

/// After a Venus Djinni enters, six glowing portals leave the user, swirl and
/// close in on the first target, each striking it as it arrives.
///
/// Abilities: Ground.
void BattleEffect_RunGround(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  struct MotionObject *user;
  s32 screen[3];
  s32 world[3];
  struct ScreenPoint position;
  s32 j;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      args, DJINNI_ENTRANCE(ELEMENT_VENUS, args->side, DJINNI_ANCHOR_OVER_USER),
      screen, world);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (args->side == EFFECT_SIDE_PARTY)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_PORTAL_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
                             work->sheet + GROUND_DJINNI_PICTURE, 1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  user = BattleObject_GetFar(args->user)->object;
  {
    struct MotionObject *target = BattleObject_GetFar(args->targets[0])->object;

    // The portals fan out from above the user.
    for (i = 0; i != 8; i++) {
      struct Particle *portal = &work->particles[i];

      SET_POSITION(portal, user->x, toFIXED(66), user->z);
      portal->velocityX = (i * toFIXED(80)) >> 5;
      portal->velocityY = (s32)INTtoFIXED((Random16() & 127) - 64) >> 6;
      portal->velocityZ = (s32)INTtoFIXED((Random16() & 255) - 127) >> 5;
      if (portal->x > 0)
        portal->velocityX = -portal->velocityX;
      portal->variant = 1;
    }

    for (frame = 0; frame != GROUND_FRAMES; frame++) {
      // The canvas's colours step towards the portal sprite sheet's, and the
      // portals turn from yellow to violet.
      if (frame > 16)
        BattleEffect_StepPaletteToResource(RESOURCE_PORTAL_SHEET);
      // The Djinni circles above the user, then rises away.
      if (args->prelude == 1) {
        s32 angle = frame * (TRIG_TURN / 32);
        s32 x = FIXEDtoINT(-Trig_Sin(angle) * 4) + screen[0] / 2 - 10;
        s32 y = FIXEDtoINT(Trig_Cos(angle) * 2) + screen[1] - 22;

        if (frame > 16)
          y = y - frame * 2 + 32;
        if (args->side == EFFECT_SIDE_PARTY)
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              BLITTER_ADDITIVE);
        else
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                              BLITTER_ADDITIVE);
        if (frame <= 3)
          ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
              canvas, work->sheet + GROUND_DJINNI_PICTURE, x, y,
              SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        draw[0](canvas, work->sheet + GROUND_DJINNI_PICTURE, x, y,
                SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      }
      // Crackles flicker round each portal.
      if ((frame & 1) == 0) {
        for (i = 0; i != GROUND_CRACKLES; i++) {
          struct Particle *crackle = &work->particles[GROUND_FIRST_CRACKLE + i];

          crackle->velocityX = Random16() % 6 + 3;
          crackle->velocityY = gHail_CrackleFlips[Random16() & 3];
        }
      }
      EFFECT_VIEW_FROM_CAMERA(camera);
      for (i = 0; i != GROUND_PORTALS; i++) {
        struct Particle *portal = &work->particles[i];

        if (portal->variant == 1) {
          if (frame > i * 2) {
            s32 x;
            s32 y;

            EffectPosition_ProjectPoint(&portal->x, &position);
            position.x >>= 1;
            x = position.x - PORTAL_WIDTH / 2;
            y = position.y - PORTAL_HEIGHT / 2;
            draw[0](canvas, work->sheet, x, y, PORTAL_WIDTH, PORTAL_HEIGHT);
            if ((frame & 3) <= 1)
              draw[0](canvas, work->sheet + gHail_PortalPictures[1],
                      x + gHail_PortalX[1], y + gHail_PortalY[1],
                      gHail_PortalWidths[1], gHail_PortalHeights[1]);
            else
              draw[0](canvas, work->sheet + gHail_PortalPictures[2],
                      x + gHail_PortalX[2], y + gHail_PortalY[2],
                      gHail_PortalWidths[2], gHail_PortalHeights[2]);
            for (j = 0; j != GROUND_CRACKLES_PER_PORTAL; j++) {
              struct Particle *crackle =
                  &work->particles[GROUND_FIRST_CRACKLE +
                                   i * GROUND_CRACKLES_PER_PORTAL + j];
              s32 crackleX;
              s32 crackleY;

              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, crackle->velocityY,
                                  BLITTER_MAX);
              draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
              if (crackle->velocityY & COMPOSITE_FLIP_X)
                crackleX = x - gHail_PortalWidths[crackle->velocityX] -
                           gHail_PortalX[crackle->velocityX] + PORTAL_WIDTH;
              else
                crackleX = x + gHail_PortalX[crackle->velocityX];
              if (crackle->velocityY & COMPOSITE_FLIP_Y)
                crackleY = y - gHail_PortalHeights[crackle->velocityX] -
                           gHail_PortalY[crackle->velocityX] + PORTAL_HEIGHT;
              else
                crackleY = y + gHail_PortalY[crackle->velocityX];
              draw[1](canvas,
                      work->sheet + gHail_PortalPictures[crackle->velocityX],
                      crackleX, crackleY,
                      gHail_PortalWidths[crackle->velocityX],
                      gHail_PortalHeights[crackle->velocityX]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
            portal->x += portal->velocityX;
            portal->y += portal->velocityY;
            portal->z += portal->velocityZ;
          }
          // The portal homes in on the target and strikes it.
          if (frame > i * 2 + 16) {
            portal->velocityX += (target->x - portal->x) >> 8;
            portal->velocityY += (toFIXED(20) - portal->y) >> 8;
            portal->velocityZ += (target->z - portal->z) >> 8;
            if (frame < i * 2 + 85) {
              portal->velocityX = portal->velocityX * 60 / DAMPING_SCALE;
              portal->velocityY = portal->velocityY * 60 / DAMPING_SCALE;
              portal->velocityZ = portal->velocityZ * 60 / DAMPING_SCALE;
            }
            if (portal->y < toFIXED(20)) {
              work->shakeFrames = 8;
              portal->variant = 0;
              Sound_PlayFar(SE_HEAVY_IMPACT);
              EFFECT_STRIKE_TARGET(args, 0, 4, KNOCKBACK_HURL);
            }
          }
        }
      }
      BattleCamera_Shake(16, 16);
      EFFECT_END_FRAME(work);
    }
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Shakes the battle view while the effect's shake lasts: BG1 and the
/// projection's centre move by a random offset each frame, and once the shake
/// runs out BG1 goes back to where it was.
///
/// rangeX, rangeY: the offsets' ranges, powers of two. Both offsets are centred
///                 on half of rangeY, so with rangeX the smaller the view only
///                 shakes to the left.
void BattleCamera_Shake(s32 rangeX, u32 rangeY) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  if (work->shakeFrames > 0) {
    s32 randomX = (rangeX - 1) & Random16();
    s32 halfRange = (s32)rangeY / 2;
    s32 offsetY = ((rangeY - 1) & Random16()) - halfRange;
    s32 offsetX = randomX - halfRange;

    gBgScroll[1].x = offsetX;
    gBgScroll[1].y = offsetY + BATTLE_BACKGROUND_SCROLL_Y;
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X - offsetX;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y - offsetY;
    work->shakeFrames--;
    return;
  }
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
}

/// Shakes the battle view up and down only, by a random offset within range,
/// while the effect's shake lasts.
void Camera_ApplyVerticalShake(s32 range) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  if (work->shakeFrames > 0) {
    s32 offsetY = ((range - 1) & Random16()) - range / 2;

    gBgScroll[1].y = offsetY + BATTLE_BACKGROUND_SCROLL_Y;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y - offsetY;
    work->shakeFrames--;
  } else {
    gBgScroll[1].y = work->savedBg1Y;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  }
}

#undef BLITTER

// Thor: a beam of light falls from a dusk sky, then a pillar of lightning and
// violet bolts strike the foes.
