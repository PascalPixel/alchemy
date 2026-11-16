/// @file
/// Hail and Ground, and the battle camera's shakes.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The ice chips live in the map cell buffer.
#define CHIPS ((struct Particle *)Ram_MapCellBuffer)
#define CHIP_COUNT 64

/// Where IceChipSheet is decoded in the effect's pictures.
#define HAIL_ICE_CHIP_PICTURES 0x400
/// Where MercuryDjinnSmallSheet is decoded in the effect's pictures.
#define HAIL_MERCURY_DJINN_SMALL_PICTURES 0x65c0

#define HAIL_FRAMES 64

/// After a Mercury Djinni enters, ice chips burst out of the first target and
/// waves of water rise along the bottom of the screen.
///
/// Abilities: Hail.
void BattleEffect_RunHail(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 screen[3];
  s32 otherPoint[3];
  s32 spawn[3];
  DrawRectangle blit[2];
  struct Particle *chip;
  s32 i;
  s32 frame;
  s32 rise;

  // Setup: the canvas, the blitter pair for the caster's side, the water.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_MERCURY, args->side, DJINNI_ANCHOR_OVER_USER),
      screen, otherPoint);
  BattleEffect_FetchRectangleBlitters(args->side, blit);
  Resource_LoadAndDecompress(RESOURCE_WATER_SPRAY_SHEET, work->sheet, 1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The chips start in a 16-pixel spread at the first target and fly up
  // at random, each to live 32 to 47 frames.
  EffectPosition_ProjectBattler(args->targets[0], spawn);
  for (i = 0; i != CHIP_COUNT; i++) {
    s32 angle;
    s32 speed;

    angle = (Random16() & 0x7fff) + DEGtoANG(90);
    speed = (Random16() & 0x1ff) + 0x80;
    CHIPS[i].x = INTtoFIXED((spawn[0] / 2 + (Random16() & 0xf)) - 8);
    CHIPS[i].y = INTtoFIXED(spawn[1] + 8);
    CHIPS[i].velocityX = (Trig_Sin(angle) * speed) >> 9;
    CHIPS[i].velocityY = (Trig_Cos(angle) * speed) >> 6;
    CHIPS[i].z = Random16() & 0x7f;
    CHIPS[i].velocityZ = Random16() & 0x7f;
    CHIPS[i].variant = (Random16() & 0xf) + 32;
  }

  for (frame = 0; frame != HAIL_FRAMES; frame++) {
    // The last sixteen frames fade the layer out.
    if (frame > 47)
      REG_BLDALPHA = BLEND_ALPHA(64 - frame, BLEND_WEIGHT_MAX);
    if (frame == 1) {
      Resource_LoadAndDecompress(RESOURCE_ICE_CHIP_SHEET,
                                 work->sheet + HAIL_ICE_CHIP_PICTURES, 1, 1);
      Resource_LoadAndDecompress(
          RESOURCE_MERCURY_DJINN_SMALL_SHEET,
          work->sheet + HAIL_MERCURY_DJINN_SMALL_PICTURES, 1, 0);
    }

    // With its Djinn prelude, the Djinn hovers over the anchor.
    if (args->prelude == 1) {
      s32 orbit;
      s32 x;
      s32 y;

      orbit = frame << 11;
      x = (FIXEDtoINT(-Trig_Sin(orbit) << 2)) + screen[0] / 2 - 10;
      y = (FIXEDtoINT(Trig_Cos(orbit) << 1)) + screen[1] - 22;
      // Never true in 64 frames.
      if (frame > 69)
        y = (y - frame * 2) + 138;
      blit[1](canvas, work->sheet + HAIL_MERCURY_DJINN_SMALL_PICTURES, x, y,
              SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      if (frame <= 3)
        blit[1](canvas, work->sheet + HAIL_MERCURY_DJINN_SMALL_PICTURES, x, y,
                SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
    }

    // Chip i flies from frame 4 + i / 4, through the two blitters in turn.
    for (i = 0, chip = CHIPS; i != CHIP_COUNT; i++, chip++) {
      if (frame >= i / 4 + 4) {
        s32 cel;
        u32 width;
        u32 height;

        cel = (chip->z / 128) & 3;
        blit[i & 1](canvas,
                    work->sheet + HAIL_ICE_CHIP_PICTURES + gHail_Pictures[cel],
                    FIXED_INTEGER_PART(chip->x) -
                        (width = gHail_PictureWidths[cel]) / 2,
                    FIXED_INTEGER_PART(chip->y) -
                        (height = gHail_PictureHeights[cel]) / 2,
                    width, height);
        BattleEffectKernels_IntegrateVector3(chip, 63, toFIXED(1) / 16);
      }
    }

    // The hit: an eight-frame shake, the battle resumes, the first target
    // flashes for sixteen frames and recoils.
    if (frame == 8) {
      work->shakeFrames = frame;
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      EFFECT_STRIKE_TARGET(args, 0, 16, KNOCKBACK_LAUNCH);
    }

    // The water rises 4 pixels a frame to 32 and drifts away from the
    // caster's side.
    rise = frame * 4;
    if (rise > WATER_WAVE_HEIGHT)
      rise = WATER_WAVE_HEIGHT;
    if (args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != 5; i++)
        blit[0](canvas, work->sheet, (i << 5) - (frame / 4 & 31), 120 - rise,
                WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
    } else {
      for (i = 0; i != 5; i++)
        blit[0](canvas, work->sheet,
                ((i << 5) + (frame / 4 & 31)) - WATER_WAVE_WIDTH, 120 - rise,
                WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
    }

    BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(work);
  }

  // Teardown.
  EFFECT_FINISH_CANVAS();
}

// BattleEffect_RunGround: the Venus Djinn's rock toss.

// On screen: the background slowly takes on the portal's colours from
// frame 17. Six rocks leave the caster one after another, every two
// frames, flung out sideways with a random lift and drift; sixteen frames
// after each leaves, it is pulled towards the first target and slowed,
// until it drops below y 20 there: then it vanishes with cue 134, an
// eight-frame shake, and the target flashing and recoiling. Each rock is
// drawn with a rotating highlight and four chips that change shape and
// flip at random every other frame. If the effect has its Djinn prelude,
// a small Venus Djinn hovers over the anchor point, drawn twice as bright
// for the first four frames, and rises off from frame 17.

// Setup: the battle effect canvas and the canvas preparer (anchor
// 2, for the caster's side), a max blitter for the side, the portal sheet
// and the Djinn sheet in the effect work, the transfer callback, and the
// eight rocks' starting points and velocities at the caster.
// Frame loop, 96 frames, as above, with a heavy shake throughout.
// Teardown: the callback and the blitter are released and the canvas
// layer is closed.

#define ROCK_COUNT 6
#define CHIPS_PER_ROCK 4
/// The chips' shapes and flips live in particles[32..63].
#define FIRST_CHIP 32

/// Where VenusDjinnSmallSheet is decoded in the effect's pictures.
#define GROUND_VENUS_DJINN_SMALL_PICTURES 0x65c0

#define GROUND_FRAMES 96

/// PortalSheet's portal.
#define PORTAL_WIDTH 24
#define PORTAL_HEIGHT 48

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
  struct MotionObject *actor;
  s32 screen[3];
  s32 otherPoint[3];
  struct ScreenPoint position;
  s32 j;

  // Setup: the canvas, a max blitter for the side, the two sheets.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      args, DJINNI_ENTRANCE(ELEMENT_VENUS, args->side, DJINNI_ANCHOR_OVER_USER),
      screen, otherPoint);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (args->side == EFFECT_SIDE_PARTY)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_PORTAL_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
                             work->sheet + GROUND_VENUS_DJINN_SMALL_PICTURES, 1,
                             1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The rocks start at the caster, 66 high, flung out to the side away
  // from the caster with a random lift and drift.
  actor = BattleObject_GetFar(args->user)->object;
  {
    struct MotionObject *target = BattleObject_GetFar(args->targets[0])->object;

    for (i = 0; i != 8; i++) {
      struct Particle *rock = &work->particles[i];

      SET_POSITION(rock, actor->x, toFIXED(66), actor->z);
      rock->velocityX = (i * toFIXED(80)) >> 5;
      rock->velocityY = (s32)INTtoFIXED((Random16() & 127) - 64) >> 6;
      rock->velocityZ = (s32)INTtoFIXED((Random16() & 255) - 127) >> 5;
      if (rock->x > 0)
        rock->velocityX = -rock->velocityX;
      rock->variant = 1;
    }

    for (frame = 0; frame != GROUND_FRAMES; frame++) {
      // The background takes on the portal's colours from frame 17.
      if (frame > 16)
        BattleEffect_StepPaletteToResource(RESOURCE_PORTAL_SHEET);

      // With its Djinn prelude, the Djinn hovers over the anchor and rises
      // off from frame 17; the first four frames add a bright copy.
      if (args->prelude == 1) {
        s32 angle = frame << 11;
        s32 x = (FIXEDtoINT(-Trig_Sin(angle) << 2)) + screen[0] / 2 - 10;
        s32 y = (FIXEDtoINT(Trig_Cos(angle) << 1)) + screen[1] - 22;

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
              canvas, work->sheet + GROUND_VENUS_DJINN_SMALL_PICTURES, x, y,
              SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        draw[0](canvas, work->sheet + GROUND_VENUS_DJINN_SMALL_PICTURES, x, y,
                SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      }

      // Every other frame each chip takes a new shape (3 to 8) and flip.
      if ((frame & 1) == 0) {
        for (i = 0; i != 32; i++) {
          struct Particle *chip = &work->particles[FIRST_CHIP + i];

          chip->velocityX = Random16() % 6 + 3;
          chip->velocityY = gHail_CrackleFlips[Random16() & 3];
        }
      }

      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      for (i = 0; i != ROCK_COUNT; i++) {
        struct Particle *rock = &work->particles[i];

        if (rock->variant == 1) {
          // Rock i flies from frame 2i: the rock, its highlight (two shapes
          // in turn, two frames each) and its four chips.
          if (frame > i * 2) {
            s32 x;
            s32 y;

            EffectPosition_ProjectPoint(&rock->x, &position);
            position.x >>= 1;
            x = position.x - 12;
            y = position.y - 24;
            draw[0](canvas, work->sheet, x, y, PORTAL_WIDTH, PORTAL_HEIGHT);
            if ((frame & 3) <= 1)
              draw[0](canvas, work->sheet + gHail_PortalPictures[1],
                      x + gHail_PortalX[1], y + gHail_PortalY[1],
                      gHail_PortalWidths[1], gHail_PortalHeights[1]);
            else
              draw[0](canvas, work->sheet + gHail_PortalPictures[2],
                      x + gHail_PortalX[2], y + gHail_PortalY[2],
                      gHail_PortalWidths[2], gHail_PortalHeights[2]);
            for (j = 0; j != CHIPS_PER_ROCK; j++) {
              struct Particle *chip =
                  &work->particles[FIRST_CHIP + i * CHIPS_PER_ROCK + j];
              s32 chipX;
              s32 chipY;

              // A max blitter with the chip's own flip, for this chip only.
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, chip->velocityY,
                                  BLITTER_MAX);
              draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
              if (chip->velocityY & COMPOSITE_FLIP_X)
                chipX = x - gHail_PortalWidths[chip->velocityX] -
                        gHail_PortalX[chip->velocityX] + 24;
              else
                chipX = x + gHail_PortalX[chip->velocityX];
              if (chip->velocityY & COMPOSITE_FLIP_Y)
                chipY = y - gHail_PortalHeights[chip->velocityX] -
                        gHail_PortalY[chip->velocityX] + 48;
              else
                chipY = y + gHail_PortalY[chip->velocityX];
              draw[1](canvas,
                      work->sheet + gHail_PortalPictures[chip->velocityX],
                      chipX, chipY, gHail_PortalWidths[chip->velocityX],
                      gHail_PortalHeights[chip->velocityX]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
            rock->x += rock->velocityX;
            rock->y += rock->velocityY;
            rock->z += rock->velocityZ;
          }
          // Sixteen frames later it is pulled towards the target, 20 high,
          // and slowed until frame 2i + 85; below y 20 it hits.
          if (frame > i * 2 + 16) {
            rock->velocityX += (target->x - rock->x) >> 8;
            rock->velocityY += (toFIXED(20) - rock->y) >> 8;
            rock->velocityZ += (target->z - rock->z) >> 8;
            if (frame < i * 2 + 85) {
              rock->velocityX = rock->velocityX * 60 / DAMPING_SCALE;
              rock->velocityY = rock->velocityY * 60 / DAMPING_SCALE;
              rock->velocityZ = rock->velocityZ * 60 / DAMPING_SCALE;
            }
            if (rock->y < toFIXED(20)) {
              work->shakeFrames = 8;
              rock->variant = 0;
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

  // Teardown.
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Shakes the battle view while the effect work's shakeFrames lasts, moving
/// BG1 and gProjection's centre by a random offset; once the shake runs
/// out, BG1 returns to its saved position and the centre to the battle's. It
/// takes two random numbers each shaking frame, x first.
///
/// Called by nearly every effect. The x offset is centred on
/// half of rangeY too, so with rangeX smaller the x offset is never positive.
///
/// rangeX: the horizontal range, a power of two.
/// rangeY: the vertical range, a power of two; both offsets are centred on
///         half of it.
void BattleCamera_Shake(s32 rangeX, u32 rangeY) {
  struct BattleEffectWork *work;
  s32 *remainingFrames;

  work = Ram_HeapSlots->battleEffect;
  remainingFrames = &work->shakeFrames;
  if (*remainingFrames > 0) {
    s32 randomX = (rangeX - 1) & Random16();
    s32 halfRange = (s32)rangeY / 2;
    s32 offsetY = ((rangeY - 1) & Random16()) - halfRange;
    s32 offsetX = randomX - halfRange;

    gBgScroll[1].x = offsetX;
    gBgScroll[1].y = offsetY + BATTLE_BACKGROUND_SCROLL_Y;
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X - offsetX;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y - offsetY;
    (*remainingFrames)--;
    return;
  }
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
}

/// While shakeFrames lasts, moves BG1 and the projection centre up or down
/// by a random offset within range; then restores them.
void Camera_ApplyVerticalShake(s32 range) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  s32 *timer;

  timer = &work->shakeFrames;
  if (*timer > 0) {
    s32 shake = ((range - 1) & Random16()) - range / 2;

    gBgScroll[1].y = shake + BATTLE_BACKGROUND_SCROLL_Y;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y - shake;
    (*timer)--;
  } else {
    gBgScroll[1].y = work->savedBg1Y;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  }
}

#undef BLITTER

// Thor: a beam of light falls from a dusk sky, then a pillar of lightning and
// violet bolts strike the foes.
