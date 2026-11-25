/// @file
/// Vengeance and Life Shear, a geyser, swarms of stars and a column of bubbles,
/// and the Coatlicue summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The star swarm's modes (BattleEffect_RunStarSwarm).
enum StarSwarmMode { STAR_SWARM_DJINN_STUN, STAR_SWARM_MYSTIC_CALL };

/// The geyser's modes (BattleEffect_RunGeyser), each named for the effect that
/// plays it.
enum GeyserMode { GEYSER_PURGATORY, GEYSER_DJINN_STORM, GEYSER_DJINN_BLAST };

// Vengeance: the archer's flaming arrows.

#define VENGEANCE_FRAMES 124
#define VENGEANCE_ARROWS 7 ///< arrows shot, one every VENGEANCE_SHOT_INTERVAL
#define VENGEANCE_FIRST_SHOT 22    ///< the first arrow's turn
#define VENGEANCE_SHOT_INTERVAL 12 ///< frames from one arrow's turn to the next
/// The frames from an arrow's turn to its flight, while it rests on the drawn
/// bow (the first rests there from the start); then the string is at rest
/// until the next arrow's turn.
#define VENGEANCE_NOCK_FRAMES 4
/// The frames a struck arrow stays in the target, its flame out.
#define VENGEANCE_STUCK_FRAMES 4
/// Vengeance's sprite sheet: the archer twice, her bow drawn and its string at
/// rest after a shot, then the flame around an arrow's head, the arrow, and her
/// shoulder, drawn again over a nocked arrow so that its end passes behind it.
#define VENGEANCE_ARCHER_WIDTH 88
#define VENGEANCE_ARCHER_HEIGHT 96
#define VENGEANCE_ARCHER_LOOSED PICTURE_BYTES(VENGEANCE_ARCHER)
#define VENGEANCE_FLAME_TIP (2 * PICTURE_BYTES(VENGEANCE_ARCHER))
#define VENGEANCE_FLAME_TIP_WIDTH 17
#define VENGEANCE_FLAME_TIP_HEIGHT 12
#define VENGEANCE_ARROW                                                        \
  (VENGEANCE_FLAME_TIP + PICTURE_BYTES(VENGEANCE_FLAME_TIP))
#define VENGEANCE_ARROW_WIDTH 51
#define VENGEANCE_ARROW_HEIGHT 12
#define VENGEANCE_SHOULDER (VENGEANCE_ARROW + PICTURE_BYTES(VENGEANCE_ARROW))
#define VENGEANCE_SHOULDER_WIDTH 14
#define VENGEANCE_SHOULDER_HEIGHT 13
/// Where the archer stands on the canvas on the enemies' side, and her
/// shoulder over her picture; on the party's side both are mirrored.
#define VENGEANCE_ARCHER_X 32
#define VENGEANCE_SHOULDER_X (VENGEANCE_ARCHER_X + 56)
#define VENGEANCE_SHOULDER_Y 29
/// The screen line the archer's picture starts on: her windows cover her
/// height from it.
#define VENGEANCE_TOP 40
/// The arrows fly this far down the canvas, plus their own y, level with her
/// bow. The canvas below her picture is a strip the far half of the map
/// shows level with them, in its tile rows 3 to 6, and each arrow is drawn
/// there again, to fly on across that half.
#define VENGEANCE_ARROW_Y 24
#define VENGEANCE_STRIP_Y VENGEANCE_ARCHER_HEIGHT
/// The camera turns this far a frame while she slides in.
#define VENGEANCE_CAMERA_TURN (TRIG_TURN / 512)
/// On the enemies' side, while she slides in, her windows open this far
/// right of the background's place.
#define VENGEANCE_WINDOW_LEFT 120

/// An archer slides in from the far side on a window of her own, her bow
/// drawn, and from frame 26 looses a flaming arrow every twelve frames; seven
/// fly across, and those the level picks strike the target, the last one
/// hardest. Everything is mirrored on the party's side.
///
/// Abilities: Vengeance.
void BattleEffect_RunVengeance(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  // The plain blitter and the flipped one.
  DrawRectangle draw[2];
  struct ScreenPoint targetScreen;
  struct Particle *arrow;
  FIXED backgroundX;
  s32 speed;
  s32 loosed;
  s32 row;
  s32 column;
  s32 i;
  s32 strikes;
  s32 frames;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartTiledCanvas(0);
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 4);

  // The map, 256 pixels wide, two tiles to an entry: the half on her side
  // shows the canvas's first twelve tile rows, where she is drawn; the far
  // half shows in its rows 3 to 6 the strip below her picture.
  i = 0;
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 16; column++, i += 2) {
      s32 tileX;
      s32 tileY;
      s32 tile;
      s16 entry;

      // The strip takes the left half on the enemies' side and the right half
      // on the party's.
      if (args->side == EFFECT_SIDE_ENEMIES && column <= 7) {
        if ((u32)(row - VENGEANCE_ARROW_Y / 8) > 3) {
          tileX = 0;
          tileY = 0;
        } else {
          tileX = column;
          tileY = row + (VENGEANCE_STRIP_Y - VENGEANCE_ARROW_Y) / 8;
        }
      } else if (args->side == EFFECT_SIDE_ENEMIES || column <= 7) {
        if (row >= VENGEANCE_STRIP_Y / 8) {
          tileX = 0;
          tileY = 0;
        } else {
          tileX = column & 7;
          tileY = row;
        }
      } else if ((u32)(row - VENGEANCE_ARROW_Y / 8) <= 3) {
        tileX = column & 7;
        tileY = row + (VENGEANCE_STRIP_Y - VENGEANCE_ARROW_Y) / 8;
      } else {
        tileX = 0;
        tileY = 0;
      }
      tile = (tileY * 8 + tileX) * 2;
      entry = ((tile + 1) << 8) | tile;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + i) = entry;
    }
  }
  REG_BG2Y = -(VENGEANCE_TOP << 8);
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN0V = WIN_RANGE(VENGEANCE_TOP, VENGEANCE_TOP + VENGEANCE_ARCHER_HEIGHT);
  REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN1V = WIN_RANGE(VENGEANCE_TOP, VENGEANCE_TOP + VENGEANCE_ARCHER_HEIGHT);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_OBJ |
               WINOUT_WIN01_CLR | WINOUT_WINOBJ_CLR;
  Resource_LoadAndDecompress(RESOURCE_ARCHER_SHEET, state->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  backgroundX = toFIXED(86);
  speed = toFIXED(16);
  frames = VENGEANCE_FRAMES;

  for (frame = 0; frame != frames; frame++) {
    camera = Ram_HeapSlots->cameraState;
    EffectPosition_ProjectScaledBattler(args->targets[0], &targetScreen);
    if (frame <= 31) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        camera->yaw = (s16)camera->yaw - VENGEANCE_CAMERA_TURN;
      } else {
        camera->yaw = (s16)camera->yaw + VENGEANCE_CAMERA_TURN;
      }
    }
    if (frame >= frames - BLEND_WEIGHT_MAX) {
      REG_BLDALPHA = BLEND_ALPHA(frames - frame - 1, BLEND_WEIGHT_MAX);
    }
    if (frame == 0) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        backgroundX = toFIXED(86);
        speed = toFIXED(16);
      } else {
        backgroundX = -toFIXED(86);
        speed = -toFIXED(16);
      }
      for (i = 0; i != VENGEANCE_ARROWS; i++) {
        arrow = &state->particles[i];
        if (args->side == EFFECT_SIDE_ENEMIES) {
          arrow->x = toFIXED(160);
          arrow->velocityX = toFIXED(12);
        } else {
          arrow->x = toFIXED(32);
          arrow->velocityX = -toFIXED(12);
        }
        arrow->y = toFIXED(12);
        strikes = gVengeance_ArrowStrikes[args->variant][i];
        if (strikes != 0) {
          strikes = 1;
        }
        arrow->z = strikes;
        arrow->velocityY = (Random16() & TRIG_ANGLE_MASK) - DEGtoANG(180);
        arrow->velocityZ = 0;
        arrow->variant = 0;
      }
    }

    // The archer slides in, her window following.
    if (frame <= 24) {
      REG_BG2X = -(FIXEDtoINT(backgroundX)) << 8;
      if (args->side == EFFECT_SIDE_ENEMIES) {
        REG_WIN0H = WIN_RANGE(FIXEDtoINT(backgroundX) + VENGEANCE_WINDOW_LEFT,
                              DISPLAY_WIDTH);
        REG_WIN1H = WIN_RANGE(FIXEDtoINT(backgroundX) + VENGEANCE_WINDOW_LEFT,
                              DISPLAY_WIDTH);
      } else {
        REG_WIN0H = FIXEDtoINT(backgroundX) + DISPLAY_WIDTH;
        REG_WIN1H = FIXEDtoINT(backgroundX) + DISPLAY_WIDTH;
      }
    } else {
      REG_BG2X = -(FIXEDtoINT(backgroundX)) << 8;
      REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
      REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
    }
    backgroundX -= speed;
    speed = speed * 52 / DAMPING_SCALE;

    // The bow is drawn until the first shot and while each arrow is nocked.
    loosed = 0;
    if (frame >=
        VENGEANCE_FIRST_SHOT + VENGEANCE_ARROWS * VENGEANCE_SHOT_INTERVAL) {
      loosed = 1;
    } else if (frame >= VENGEANCE_FIRST_SHOT &&
               (frame - VENGEANCE_FIRST_SHOT) % VENGEANCE_SHOT_INTERVAL >=
                   VENGEANCE_NOCK_FRAMES) {
      loosed = 1;
    }
    if (args->side == EFFECT_SIDE_ENEMIES) {
      if (loosed == 0) {
        draw[0](canvas, state->sheet, VENGEANCE_ARCHER_X, 0,
                VENGEANCE_ARCHER_WIDTH, VENGEANCE_ARCHER_HEIGHT);
      } else {
        draw[0](canvas, state->sheet + VENGEANCE_ARCHER_LOOSED,
                VENGEANCE_ARCHER_X, 0, VENGEANCE_ARCHER_WIDTH,
                VENGEANCE_ARCHER_HEIGHT);
      }
    } else if (loosed == 0) {
      draw[1](canvas, state->sheet,
              CANVAS_SIZE - VENGEANCE_ARCHER_X - VENGEANCE_ARCHER_WIDTH, 0,
              VENGEANCE_ARCHER_WIDTH, VENGEANCE_ARCHER_HEIGHT);
    } else {
      draw[1](canvas, state->sheet + VENGEANCE_ARCHER_LOOSED,
              CANVAS_SIZE - VENGEANCE_ARCHER_X - VENGEANCE_ARCHER_WIDTH, 0,
              VENGEANCE_ARCHER_WIDTH, VENGEANCE_ARCHER_HEIGHT);
    }

    // The arrows: each is nocked in its turn, flies once it has rested on the
    // bow, and a striking one stays in the target a few frames.
    for (i = 0; i != VENGEANCE_ARROWS; i++) {
      arrow = &state->particles[i];
      if (frame < i * VENGEANCE_SHOT_INTERVAL + VENGEANCE_FIRST_SHOT &&
          (frame >= VENGEANCE_FIRST_SHOT || i != 0)) {
        continue;
      }
      if (frame >= i * VENGEANCE_SHOT_INTERVAL + VENGEANCE_FIRST_SHOT +
                       VENGEANCE_NOCK_FRAMES &&
          arrow->variant == 0) {
        arrow->x -= arrow->velocityX;
        arrow->y += arrow->velocityY;
      }
      if (arrow->variant > VENGEANCE_STUCK_FRAMES) {
        continue;
      }
      if (arrow->variant > 0) {
        arrow->variant++;
      }
      if (arrow->z == 1 && arrow->variant == 0) {
        s32 hit = 0;

        if (args->side == EFFECT_SIDE_ENEMIES) {
          if ((FIXEDtoINT(arrow->x)) < targetScreen.x / 2 + 16) {
            hit = 1;
          }
        } else if ((FIXEDtoINT(arrow->x)) > targetScreen.x / 2 + 48) {
          hit = 1;
        }
        if (hit == 1) {
          state->shakeFrames = 4;
          if (i == VENGEANCE_ARROWS - 1) {
            BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          } else {
            // Each arrow pushes the target further back: to 110 percent of
            // its place, ten more for each arrow before.
            BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(2),
                                          toFIXED(8), toFIXED(2), i * 10 + 110);
            Sound_PlayFar(SE_IMPACT);
          }
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 4);
          // A struck arrow keeps its place against the target, in pixels.
          arrow->variant = 1;
          if (args->side == EFFECT_SIDE_ENEMIES) {
            arrow->x = targetScreen.x / 2 - FIXED_INTEGER_PART(arrow->x);
          } else {
            arrow->x =
                targetScreen.x / 2 - FIXED_INTEGER_PART(arrow->x) + CANVAS_SIZE;
          }
        }
      }
      // Each arrow is drawn in her half and again in the strip, a canvas
      // width along, with the flame around its head while it flies.
      if (args->side == EFFECT_SIDE_ENEMIES) {
        if (arrow->variant == 0) {
          draw[0](canvas, state->sheet + VENGEANCE_ARROW,
                  (FIXEDtoINT(arrow->x)) -
                      (CANVAS_SIZE - VENGEANCE_FLAME_TIP_WIDTH),
                  FIXED_INTEGER_PART(arrow->y) + VENGEANCE_ARROW_Y,
                  VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
          draw[0](canvas, state->sheet + VENGEANCE_ARROW,
                  FIXED_INTEGER_PART(arrow->x) + VENGEANCE_FLAME_TIP_WIDTH,
                  FIXED_INTEGER_PART(arrow->y) + VENGEANCE_STRIP_Y,
                  VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
          draw[0](canvas, state->sheet + VENGEANCE_FLAME_TIP,
                  FIXED_INTEGER_PART(arrow->x) - CANVAS_SIZE,
                  FIXED_INTEGER_PART(arrow->y) + VENGEANCE_ARROW_Y,
                  VENGEANCE_FLAME_TIP_WIDTH, VENGEANCE_FLAME_TIP_HEIGHT);
          draw[0](canvas, state->sheet + VENGEANCE_FLAME_TIP,
                  FIXED_INTEGER_PART(arrow->x),
                  FIXED_INTEGER_PART(arrow->y) + VENGEANCE_STRIP_Y,
                  VENGEANCE_FLAME_TIP_WIDTH, VENGEANCE_FLAME_TIP_HEIGHT);
        } else {
          draw[0](canvas, state->sheet + VENGEANCE_ARROW,
                  targetScreen.x / 2 + arrow->x -
                      (CANVAS_SIZE - VENGEANCE_FLAME_TIP_WIDTH),
                  FIXED_INTEGER_PART(arrow->y) + VENGEANCE_ARROW_Y,
                  VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
          draw[0](canvas, state->sheet + VENGEANCE_ARROW,
                  targetScreen.x / 2 + arrow->x + VENGEANCE_FLAME_TIP_WIDTH,
                  FIXED_INTEGER_PART(arrow->y) + VENGEANCE_STRIP_Y,
                  VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
        }
      } else if (arrow->variant == 0) {
        draw[1](canvas, state->sheet + VENGEANCE_ARROW, FIXEDtoINT(arrow->x),
                FIXED_INTEGER_PART(arrow->y) + VENGEANCE_ARROW_Y,
                VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
        draw[1](canvas, state->sheet + VENGEANCE_ARROW,
                FIXED_INTEGER_PART(arrow->x) - CANVAS_SIZE,
                FIXED_INTEGER_PART(arrow->y) + VENGEANCE_STRIP_Y,
                VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
        draw[1](canvas, state->sheet + VENGEANCE_FLAME_TIP,
                FIXED_INTEGER_PART(arrow->x) + VENGEANCE_ARROW_WIDTH,
                FIXED_INTEGER_PART(arrow->y) + VENGEANCE_ARROW_Y,
                VENGEANCE_FLAME_TIP_WIDTH, VENGEANCE_FLAME_TIP_HEIGHT);
        draw[1](canvas, state->sheet + VENGEANCE_FLAME_TIP,
                FIXED_INTEGER_PART(arrow->x) -
                    (CANVAS_SIZE - VENGEANCE_ARROW_WIDTH),
                FIXED_INTEGER_PART(arrow->y) + VENGEANCE_STRIP_Y,
                VENGEANCE_FLAME_TIP_WIDTH, VENGEANCE_FLAME_TIP_HEIGHT);
      } else {
        draw[1](canvas, state->sheet + VENGEANCE_ARROW,
                targetScreen.x / 2 + arrow->x,
                FIXED_INTEGER_PART(arrow->y) + VENGEANCE_ARROW_Y,
                VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
        draw[1](canvas, state->sheet + VENGEANCE_ARROW,
                targetScreen.x / 2 + arrow->x - CANVAS_SIZE,
                FIXED_INTEGER_PART(arrow->y) + VENGEANCE_STRIP_Y,
                VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
      }
    }

    // Her shoulder over the nocked arrow, while the bow is drawn.
    if (loosed == 0) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, VENGEANCE_SHOULDER,
                            VENGEANCE_SHOULDER_X, VENGEANCE_SHOULDER_Y);
      } else {
        EFFECT_DRAW_PICTURE(draw[1], canvas, state->sheet, VENGEANCE_SHOULDER,
                            CANVAS_SIZE - VENGEANCE_SHOULDER_X -
                                VENGEANCE_SHOULDER_WIDTH,
                            VENGEANCE_SHOULDER_Y);
      }
    }
    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(1, 4);
    EFFECT_PRESENT_CANVAS(state);
  }
  REG_WIN0V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM);
  REG_WIN1V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM);
  EFFECT_FINISH_CANVAS();
}

/// The shears, in the state's first particle: roll is its z, snip its velocityZ
/// and timer its variant.
struct Shears {
  s32 x;
  s32 y;
  s32 roll;
  s32 velocityX;
  s32 velocityY;
  s32 snip; ///< steps of the blades closing, a 32nd of a turn each
  s32 timer;
};

#define LIFE_SHEAR_FRAMES 90
#define LIFE_SHEAR_SNIP_FRAME 50 ///< the shears snap shut through the target
/// Life Shear's sprite sheet, decoded into the map cell buffer: a woman leaping
/// and the same woman striking, then the shears' two blades, each with its
/// pivot ring and handle loop (a pale one 128 by 64 and an orange one 128 by
/// 32), and the flame the flash is made of, 32 by 32.
#define LIFE_SHEAR_WOMAN_WIDTH 55
#define LIFE_SHEAR_WOMAN_HEIGHT 88
#define LIFE_SHEAR_STRIKING                                                    \
  (Ram_MapCellBuffer + PICTURE_BYTES(LIFE_SHEAR_WOMAN))
#define LIFE_SHEAR_STRIKING_WIDTH 59
#define LIFE_SHEAR_STRIKING_HEIGHT 89
#define LIFE_SHEAR_PALE_BLADE                                                  \
  (LIFE_SHEAR_STRIKING + PICTURE_BYTES(LIFE_SHEAR_STRIKING))
#define LIFE_SHEAR_PALE_BLADE_WIDTH 128
#define LIFE_SHEAR_PALE_BLADE_HEIGHT 64
#define LIFE_SHEAR_ORANGE_BLADE                                                \
  (LIFE_SHEAR_PALE_BLADE + PICTURE_BYTES(LIFE_SHEAR_PALE_BLADE))
#define LIFE_SHEAR_ORANGE_BLADE_WIDTH 128
#define LIFE_SHEAR_ORANGE_BLADE_HEIGHT 32
#define LIFE_SHEAR_FLAME                                                       \
  (LIFE_SHEAR_ORANGE_BLADE + PICTURE_BYTES(LIFE_SHEAR_ORANGE_BLADE))
/// Where her striking picture stands on the canvas on the enemies' side, and
/// how far she glides to it; on the party's side both are mirrored.
#define LIFE_SHEAR_STRIKE_X 64
/// The shears' scale until they tumble away, and the flash's as it starts.
#define LIFE_SHEAR_SHEARS_SCALE 45536
#define LIFE_SHEAR_FLASH_SCALE 85536
/// How far the flash's four flames are turned, beyond their quarter turns.
#define LIFE_SHEAR_FLASH_ANGLE 22096

/// A woman (a tiled background picture) glides in from the far side and
/// strikes from frame 24: a pair of shears (two blade models turning about
/// their pivot) swings back and into the target at frame 32, opens, and
/// snaps shut through it at frame 50 in a flash of four flames. From frame 62
/// she drifts off and the shears tumble away. Everything is mirrored on the
/// party's side.
///
/// Abilities: Life Shear.
void BattleEffect_RunLifeShear(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle blit[2];
  struct ScreenPoint target;
  struct ModelTexture texture;
  FIXED womanX;
  FIXED womanY;
  s32 speed;
  s32 frame;
  struct Shears *shears;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  if (args->side == EFFECT_SIDE_ENEMIES) {
    BattleEffect_StartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
  } else {
    BattleEffect_StartTiledCanvasFilled(BGCNT_WRAP | BGCNT_PRIORITY(1));
  }
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 6);
  Resource_LoadAndDecompress(RESOURCE_LIFE_SHEAR_SHEET, gMapGridCellBuffer, 1,
                             1);
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_TRANSPARENT);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_TRANSPARENT);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  womanX = 0;
  womanY = 0;
  speed = 0;
  EffectPosition_ProjectScaledBattler(args->targets[0], &target);

  for (frame = 0; frame != LIFE_SHEAR_FRAMES; frame++) {
    s32 x;
    s32 shift;
    s32 phase;
    s32 scale;
    s32 timer;
    s32 width = 7;

    shift = 0;
    if (frame == 0) {
      Sound_PlayFar(SE_CURSE_FALL);
      if (args->side == EFFECT_SIDE_ENEMIES) {
        state->particles[0].x = toFIXED(32);
        state->particles[0].y = -toFIXED(64);
        state->particles[0].z = -7216;
      } else {
        state->particles[0].x = -toFIXED(32);
        state->particles[0].y = -toFIXED(64);
        state->particles[0].z = -7216;
      }
      state->particles[0].velocityZ = 0;
      state->particles[0].variant = 0;
      womanY = toFIXED(24);
      if (args->side == EFFECT_SIDE_ENEMIES) {
        womanX = INTtoFIXED(target.x + 176);
        speed = toFIXED(15);
      } else {
        womanX = INTtoFIXED(target.x - 304);
        speed = -toFIXED(15);
      }
    }
    if (frame > 61) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        womanX += toFIXED(2);
      } else {
        womanX -= toFIXED(2);
      }
      womanY -= toFIXED(14) / 3;
    }
    // She glides in, slowing; the background carries her up to 120 pixels
    // across, and the canvas the rest.
    womanX -= speed;
    speed = speed * 60 / DAMPING_SCALE;
    x = FIXEDtoINT(womanX);
    if (x > 120) {
      shift = x - 120;
      x = 120;
    } else if (x < 0) {
      shift = x;
      x = 0;
    }
    REG_BG2X = -x << 8;
    if ((u32)(frame - 24) <= 39) {
      x = args->side == EFFECT_SIDE_ENEMIES
              ? LIFE_SHEAR_STRIKE_X
              : CANVAS_SIZE - LIFE_SHEAR_STRIKE_X - LIFE_SHEAR_STRIKING_WIDTH;
      blit[0](canvas, LIFE_SHEAR_STRIKING, x + shift, FIXEDtoINT(womanY),
              LIFE_SHEAR_STRIKING_WIDTH, LIFE_SHEAR_STRIKING_HEIGHT);
    } else {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        x = frame * 2 + 16;
        if (x > LIFE_SHEAR_STRIKE_X) {
          x = LIFE_SHEAR_STRIKE_X;
        }
      } else {
        x = 57 - frame * 2;
        if (x <= 8) {
          x = 9;
        }
      }
      blit[0](canvas, gMapGridCellBuffer, x + shift, FIXEDtoINT(womanY),
              LIFE_SHEAR_WOMAN_WIDTH, LIFE_SHEAR_WOMAN_HEIGHT);
    }

    // The shears' path: still, a backswing from timer 24 and the strike at
    // 33.
    for (i = 0; i != 1; i++) {
      shears = &((struct Shears *)state->particles)[i];
      if (++shears->timer <= 0) {
        continue;
      }
      if (shears->timer == 1) {
        shears->velocityX = 0;
        shears->velocityY = 0;
      }
      if (shears->timer == 24) {
        shears->velocityX = -toFIXED(4);
        shears->velocityY = toFIXED(8);
      }
      if (shears->timer == 33) {
        shears->velocityX = toFIXED(4);
        shears->velocityY = -toFIXED(8);
        state->shakeFrames = 4;
        speed = 0;
        Sound_PlayFar(SE_IMPACT);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
      }
      if (shears->timer == 35) {
        shears->velocityX = 0;
        shears->velocityY = 0;
      }
      // From frame 64 they tumble away, turning faster each frame.
      if (frame > 63) {
        s32 tumble = (frame - 64) * (TRIG_TURN / 512);

        shears->velocityY -= toFIXED(1) / 4;
        if (tumble > TRIG_TURN / 64) {
          tumble = TRIG_TURN / 64;
        }
        shears->roll -= tumble;
      }
      if (shears->timer > 8) {
        if (args->side == EFFECT_SIDE_ENEMIES) {
          shears->x += shears->velocityX;
        } else {
          shears->x -= shears->velocityX;
        }
        shears->y += shears->velocityY;
      }
    }

    // The snip, the shears' two blades and the flash, drawn as models.
    {
      void *vertices = Heap_AllocEwram(32);
      struct RenderModel *model = Render_AllocateModelList(1);
      s32 shade = 0;

      phase = frame - LIFE_SHEAR_SNIP_FRAME;
      if (phase == -4) {
        Sound_PlayFar(SE_POWER_UP);
      }
      if (phase == 0) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        state->shakeFrames = 12;
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
      }
      // The blades close from frame 44, shut just after the snip, and part a
      // little again.
      timer = state->particles[0].variant;
      if ((u32)(frame - 44) <= 13) {
        state->particles[0].velocityZ++;
      }
      if (timer >= 0) {
        scale = LIFE_SHEAR_SHEARS_SCALE;
        if (frame > 63) {
          scale = scale - (frame - 64) * (FIXED_ONE / 128);
        }
        if (scale > 0) {
          model->shade = shade;
          model->kind = MODEL_SHADED;
          model->vertices = vertices;
          model->texture = &texture;
          Matrix_Initialize();
          Matrix_Translate(state->particles[0].x + INTtoFIXED(shift),
                           state->particles[0].y, 0);
          if (args->side == EFFECT_SIDE_PARTY) {
            Matrix_RotateY(DEGtoANG(180));
          }
          Matrix_RotateZ(state->particles[0].z);
          Matrix_ScaleUniform(scale);
          texture.logHeight = 5;
          texture.pixels = LIFE_SHEAR_ORANGE_BLADE;
          texture.logWidth = width;
          model->mesh = gLifeShear_OrangeBladeMesh;
          Matrix_Push();
          Matrix_RotateZ(
              (Trig_Sin(state->particles[0].velocityZ * (TRIG_TURN / 32)) >>
               3) -
              DEGtoANG(45));
          Render_TransformVertices(gLifeShear_BladeCorners, vertices, 4);
          Render_DrawModelList(model);
          Matrix_Pop();
          model->mesh = gLifeShear_PaleBladeMesh;
          texture.logWidth = width;
          texture.logHeight = 6;
          texture.pixels = LIFE_SHEAR_PALE_BLADE;
          Matrix_RotateZ(
              DEGtoANG(45) -
              (Trig_Sin(state->particles[0].velocityZ * (TRIG_TURN / 32)) >>
               3));
          Matrix_Scale(FIXED_ONE, 90000, FIXED_ONE);
          Render_TransformVertices(gLifeShear_BladeCorners, vertices, 4);
          Render_DrawModelList(model);
        }

        // The flash: four flames, long and short in turn, shrinking over eight
        // frames.
        if ((u32)phase <= 7) {
          scale = LIFE_SHEAR_FLASH_SCALE -
                  (frame - LIFE_SHEAR_SNIP_FRAME) * (FIXED_ONE / 8);
          Matrix_Initialize();
          if (args->side == EFFECT_SIDE_ENEMIES) {
            Matrix_Translate(-toFIXED(16), 0, 0);
          } else {
            Matrix_Translate(toFIXED(16), 0, 0);
          }
          Matrix_Scale(scale, scale, scale);
          if (args->side == EFFECT_SIDE_PARTY) {
            Matrix_RotateY(DEGtoANG(180));
          }
          model->kind = MODEL_SHADED_MAX;
          model->mesh = gBattleEffect_SmallSquareMeshReversed;
          texture.logWidth = 5;
          texture.logHeight = 5;
          texture.pixels = LIFE_SHEAR_FLAME;
          for (i = 0; i != 4; i++) {
            Matrix_Push();
            Matrix_RotateZ(i * (DEGtoANG(90)) + LIFE_SHEAR_FLASH_ANGLE);
            Matrix_Scale(FIXED_ONE / 4,
                         (i & 1) * (FIXED_ONE / 2) + FIXED_ONE * 3 / 4,
                         FIXED_ONE / 2);
            Matrix_ScaleUniform(FIXED_ONE);
            Render_TransformVertices(gBattleEffect_StandingSquareCorners,
                                     vertices, 4);
            Render_DrawModelList(model);
            Matrix_Pop();
          }
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Abilities: Purgatory.
void BattleEffect_RunPurgatory(struct BattleEffectParams *args) {
  BattleEffect_RunGeyser(args, GEYSER_PURGATORY);
}

/// Abilities: Djinn Blast.
void BattleEffect_RunDjinnBlast(struct BattleEffectParams *args) {
  BattleEffect_RunGeyser(args, GEYSER_DJINN_BLAST);
}

/// Abilities: Djinn Storm.
void BattleEffect_RunDjinnStorm(struct BattleEffectParams *args) {
  BattleEffect_RunGeyser(args, GEYSER_DJINN_STORM);
}

#define GEYSER_FRAMES 54
#define GEYSER_DRAW_FRAMES 40 ///< the column is drawn
#define GEYSER_PEAK_FRAME 20  ///< the result phase starts
#define GEYSER_DROP_COUNT 512 ///< drops in PARTICLES, used in a ring
/// The drops that burst on the first frame, and Djinn Storm's that follow
/// each frame to frame 23; Djinn Blast's are fewer.
#define GEYSER_BURST 200
#define GEYSER_STREAM 16
#define DJINN_BLAST_BURST 100
#define DJINN_BLAST_STREAM 2
/// A drop's picture is a size for every eight frames of its life past these,
/// no smaller than its mode's smallest.
#define GEYSER_DROP_SHRUNK_LIFE 16
/// How far the column leans back.
#define GEYSER_TILT 2000

/// Copies a sheet's 64 colours into the background palette.
static inline void Geyser_CopyPalette(const void *palette) {
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
}

/// A geyser between the targets. A textured column, its texture scrolling
/// upward, stands on the floor at the targets' midpoint, and drops burst
/// from its foot, fall back under gravity and bounce once on the floor.
/// Purgatory's is the cross texture in orange; its column rises and falls on
/// a sine and 200 drops burst at once. Djinn Storm's column keeps widening
/// and fades from frame 32, with 200 drops and 16 more each frame to frame
/// 23; Djinn Blast's is thin, with 100 drops and 2 more each frame. Theirs
/// also step the palette through the Mercury, Mars and Jupiter Djinn's. At
/// frame 20 the target is knocked back and the result phase starts.
void BattleEffect_RunGeyser(struct BattleEffectParams *args, s32 mode) {
  struct ModelTexture texture;
  DrawRectangle draw[2];
  struct ScreenPoint midpoint;
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  void *canvas = Ram_HeapSlots->battleCanvas;
  u8 *sparkCels = Ram_HeapSlots->battleSheet;
  struct Particle *drop;
  struct RenderModel *column;
  void *vertices;
  s32 frame;
  s32 i;
  s32 count;
  s32 scale;
  s32 shade;

  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (mode == GEYSER_PURGATORY)
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 12);
  else
    REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_BuildCanvasBlitters(0, draw);
  {
    s32 spawned;
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
    Task_Add((s32)BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
    spawned = 0;
    Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sparkCels, 0, 0);
    if (mode == GEYSER_PURGATORY) {
      Resource_LoadAndDecompress(RESOURCE_CROSS_TEXTURE_SHEET, state->sheet, 1,
                                 1);
      Geyser_CopyPalette(Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B));
    } else {
      Resource_LoadAndDecompress(RESOURCE_EMBER_TEXTURE, state->sheet, 0, 0);
      Geyser_CopyPalette(Resource_GetTableEntry(RESOURCE_BLAST_SHEET));
    }

    for (frame = 0; frame != GEYSER_FRAMES; frame++) {
      // The geyser follows the targets' midpoint while it rises.
      if (frame < GEYSER_PEAK_FRAME)
        BattleEffect_GetTargetsMidpoint(&midpoint);
      if (frame == 0)
        Sound_PlayFar(SE_CLATTER);
      if (frame == 0)
        state->shakeFrames = 16;
      if (frame == GEYSER_PEAK_FRAME) {
        state->shakeFrames = 8;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HOP);
      }

      // The first frame clears every drop.
      if (frame == 0) {
        spawned = 0;
        // UB: the lives are cleared through one s32 pointer stepped a whole
        // drop at a time, past the drop's other members.
#ifdef UBFIX
        for (i = 0; i != GEYSER_DROP_COUNT; i++)
          PARTICLES[i].variant = -1;
#else
        {
          s32 *life;

          i = 0;
          life = &PARTICLES->variant;
          for (; i != GEYSER_DROP_COUNT; i++) {
            *life = -1;
            life += sizeof(struct Particle) / sizeof(*life);
          }
        }
#endif
      }

      // Djinn Storm and Djinn Blast step the palette three times a frame from
      // frame 8:
      // toward the Mercury Djinn's, from frame 20 the Mars Djinn's and from
      // frame 32 the Jupiter Djinn's.
      if (mode == GEYSER_DJINN_STORM || mode == GEYSER_DJINN_BLAST) {
        for (i = 0; i != 3; i++) {
          if (frame >= 8) {
            if (frame < 20)
              BattleEffect_StepPaletteToResource(RESOURCE_MERCURY_DJINN_SHEET);
            else if (frame < 32)
              BattleEffect_StepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
            else
              BattleEffect_StepPaletteToResource(RESOURCE_JUPITER_DJINN_SHEET);
          }
        }
      }

      // How many drops burst this frame.
      count = 0;
      if (mode == GEYSER_DJINN_BLAST) {
        if (frame == 0)
          count = DJINN_BLAST_BURST;
        if (frame >= 1 && frame < 24)
          count = DJINN_BLAST_STREAM;
      } else {
        if (frame == 0)
          count = GEYSER_BURST;
        if (mode == GEYSER_DJINN_STORM || mode == GEYSER_DJINN_BLAST) {
          if (frame >= 1 && frame < 24)
            count = GEYSER_STREAM;
        }
      }
      for (i = 0; i != count; i++, spawned++) {
        s32 speed;
        s32 angle;

        drop = &PARTICLES[spawned & (GEYSER_DROP_COUNT - 1)];
        speed = (Random16() & 1023) + 32;
        angle = (Random16() & 0x7fff) - DEGtoANG(90);
        drop->x = INTtoFIXED(midpoint.x / 2);
        drop->y = toFIXED(108);
        if (mode == GEYSER_DJINN_BLAST)
          drop->velocityX = (Trig_Sin(angle) * speed) >> 8;
        else
          drop->velocityX = (Trig_Sin(angle) * speed) >> 6;
        drop->velocityY = -(Trig_Cos(angle) * speed * 2) >> 7;
        drop->variant = (Random16() & 7) + 48;
      }

      // Each drop falls, bounces off the floor at y 120, and is drawn while
      // it is on the canvas, its cel shrinking as it burns down.
      drop = PARTICLES;
      for (i = 0; i != GEYSER_DROP_COUNT; i++, drop++) {
        if (drop->variant > 0) {
          s32 x;
          s32 y;

          drop->variant--;
          Particle_AdvanceWithGravity2D(drop, 60, gGeyser_DropGravity[mode]);
          y = FIXEDtoINT(drop->y);
          if (y > 120) {
            drop->velocityY = -drop->velocityY / 2;
          } else if (drop->x >= 0 && (x = FIXEDtoINT(drop->x)) <= 126 &&
                     drop->y >= 0) {
            s32 size = (drop->variant - GEYSER_DROP_SHRUNK_LIFE) / 8;

            if (size < gGeyser_DropMinSizes[mode])
              size = gGeyser_DropMinSizes[mode];
            draw[i & 1](canvas,
                        sparkCels + gBattleEffect_MotePictures[size - 1],
                        x - size / 2, y - size, size, size * 2);
          }
        }
      }

      // The column, for the first 40 frames.
      if (frame < GEYSER_DRAW_FRAMES) {
        vertices = Heap_AllocEwram(512);
        column = Render_AllocateModelList(1);
        texture.logWidth = 5;
        texture.logHeight = 5;
        texture.pixels = state->sheet;
        column->scrollU = (frame << 2) & 31;
        column->scrollV = (frame << 1) & 31;
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(midpoint.x / 2 - CANVAS_SIZE / 2),
                         -toFIXED(16), 0);
        Matrix_RotateX(GEYSER_TILT);
        // At half brightness, the Djinn's fading from frame 32.
        if (mode == GEYSER_PURGATORY) {
          // Up on a sine in 16 frames, held, and down again from frame 24.
          if (frame < 16)
            scale = Trig_Sin(frame * (TRIG_TURN / 64));
          else if (frame < 24)
            scale = FIXED_ONE;
          else
            scale = Trig_Sin(frame * (TRIG_TURN / 64) - DEGtoANG(45));
          shade = MODEL_SHADE_INVISIBLE / 2;
        } else if (mode == GEYSER_DJINN_STORM) {
          // Wider every frame.
          scale = frame * (FIXED_ONE / 16);
          shade = MODEL_SHADE_INVISIBLE / 2;
          if (frame > 31)
            shade = 224 - (frame << 3);
        } else {
          scale = FIXED_ONE / 4;
          shade = MODEL_SHADE_INVISIBLE / 2;
          if (frame > 31)
            shade = 224 - (frame << 3);
        }
        column->kind = MODEL_SHADED_ADDITIVE;
        column->shade = shade;
        column->mesh = gGeyser_Mesh;
        column->texture = &texture;
        column->vertices = vertices;
        Matrix_Scale(scale * 2, FIXED_ONE * 7, scale * 2);
        Render_TransformVertices(gBattleEffect_OctagonBandCorners, vertices,
                                 16);
        Render_DrawModelList(column);
        Heap_Release(column);
        Heap_Release(vertices);
      }

      BattleCamera_Shake(4, 16);
      EFFECT_END_FRAME(state);
    }
  }
  EFFECT_RELEASE_BLITTERS();
  Task_Remove((s32)BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Djinn Stun.
void BattleEffect_RunDjinnStun(struct BattleEffectParams *args) {
  BattleEffect_RunStarSwarm(args, STAR_SWARM_DJINN_STUN);
}

/// Abilities: Mystic Call.
void BattleEffect_RunMysticCall(struct BattleEffectParams *args) {
  BattleEffect_RunStarSwarm(args, STAR_SWARM_MYSTIC_CALL);
}

#define STARS_PER_TARGET 64               ///< each target's slice of PARTICLES
#define STAR_TOTAL (6 * STARS_PER_TARGET) ///< six targets' slices
/// The stars of each target's swarm.
#define DJINN_STUN_STARS 16
#define MYSTIC_CALL_STARS 48
/// A star is drawn STAR_LARGEST pixels wide at STAR_NEAR_DEPTH or nearer, a
/// pixel less for every STAR_DEPTH_STEP deeper, down to 3.
#define STAR_LARGEST 9
#define STAR_NEAR_DEPTH 250
#define STAR_DEPTH_STEP 64
#define STAR_FAR_DEPTH (STAR_NEAR_DEPTH + 6 * STAR_DEPTH_STEP)
/// A star's speed: STAR_SPEED_MIN and a random part, masked by
/// STAR_SPEED_RANDOM.
#define STAR_SPEED_MIN 128
#define STAR_SPEED_RANDOM 255

/// Swarms of stars over the targets. Each target gets a swarm, eight frames
/// after the one before: its stars burst from a point that rises out of it,
/// each turning about the point on one of four axes and drawn smaller the
/// deeper it is, and from frame 24 they are pulled back into the point,
/// each one gone once it has arrived. The palette steps through the
/// Mercury, Mars and Jupiter Djinn's. Djinn Stun: 16 stars a target, the
/// target flashes as its swarm starts, and the effect lasts eight frames
/// a target more than 38. Mystic Call: 48 stars, a flash at the start and 55
/// frames later, 72 frames in all.
void BattleEffect_RunStarSwarm(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  void *canvas = Ram_HeapSlots->battleCanvas;
  DrawRectangle draw[2];
  s32 member;
  u8 *sparkCels = Ram_HeapSlots->battleSheet;
  s32 frames;
  struct BattleCamera *camera;
  s32 frame;
  s32 i;
  struct ScreenPoint screen;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  state->fadeStep = 0;
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sparkCels, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  if (mode == STAR_SWARM_DJINN_STUN)
    frames = args->targetCount * 8 + 38;
  else
    frames = 72;
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Task_Add((s32)BattleEffect_TransferCanvas, TASK_PASS_VBLANK);

  for (frame = 0; frame != frames; frame++) {
    camera = Ram_HeapSlots->cameraState;

    // The first frame sets each target's point on it, rising, and every
    // star at its point with a random push across the floor.
    if (frame == 0) {
      for (i = 0; i != args->targetCount; i++) {
        struct MotionObject *object =
            BattleObject_GetFar(args->targets[i])->object;

        state->particles[i].x = object->x;
        state->particles[i].y = 0;
        state->particles[i].z = object->z;
        if (mode == STAR_SWARM_MYSTIC_CALL) {
          state->particles[i].velocityX = 0;
          state->particles[i].velocityY = toFIXED(1) / 2;
        } else {
          state->particles[i].velocityX = -object->x / 40;
          state->particles[i].velocityY = toFIXED(3);
        }
        state->particles[i].velocityZ = 0;
      }
      for (i = 0; i != STAR_TOTAL; i++) {
        struct Particle *star = &PARTICLES[i];
        s32 angle = Random16() & TRIG_ANGLE_MASK;
        s32 speed = STAR_SPEED_RANDOM;

        // Each star flies off at a random angle and a random speed.
        speed &= Random16();
        SET_POSITION(star, 0, INTtoFIXED((Random16() & 31) + 20), 0);
        speed += STAR_SPEED_MIN;
        star->velocityX = (Trig_Sin(angle) * speed) >> 5;
        star->velocityY = 0;
        star->velocityZ = (Trig_Cos(angle) * speed) >> 5;
        star->variant = 0;
      }
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_BLAST_SHEET),
                           SHEET_PALETTE_BYTES);
    }

    // Two palette steps a frame from frame 8.
    for (i = 0; i != 2; i++) {
      if (frame >= 8) {
        if (frame < 20)
          BattleEffect_StepPaletteToResource(RESOURCE_MERCURY_DJINN_SHEET);
        else if (frame < 32)
          BattleEffect_StepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
        else
          BattleEffect_StepPaletteToResource(RESOURCE_JUPITER_DJINN_SHEET);
      }
    }

    for (member = 0; member != args->targetCount; member++) {
      struct Particle *point = &state->particles[member];
      s32 startFrame = member;
      s32 starCount;

      startFrame *= 8;
      starCount = DJINN_STUN_STARS;
      if (mode != STAR_SWARM_DJINN_STUN)
        starCount = MYSTIC_CALL_STARS;
      if (frame == startFrame) {
        if (mode == STAR_SWARM_DJINN_STUN)
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, member, 16);
        else
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, member, 16);
      }
      if (frame == startFrame + 55 && mode != STAR_SWARM_DJINN_STUN)
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, member, 16);

      // The swarm turns about its point, which rises from frame 4.
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      if (frame >= startFrame + 4 && startFrame < 44)
        Particle_AdvanceWithGravity3D(point, PARTICLE_DAMPING_NONE, 0);
      Matrix_TranslateVector(&point->x);

      // Each star turns about the point on one of four axes, faster the
      // later it is in the swarm.
      for (i = 0; i != starCount; i++) {
        struct Particle *star = &PARTICLES[member * STARS_PER_TARGET + i];

        if (frame >= i / 8 + startFrame && star->variant == 0) {
          s32 size;

          Matrix_Push();
          switch ((member + i) & 3) {
          case 0:
            Matrix_RotateY(frame * (i + 8) * (TRIG_TURN / 2048));
            break;
          case 1:
            Matrix_RotateX(-frame * (i + 8) * (TRIG_TURN / 2048));
            break;
          case 2:
            Matrix_RotateZ(-frame * (i + 8) * (TRIG_TURN / 2048));
            break;
          case 3:
            Matrix_RotateX(-frame * (i + 8) * (TRIG_TURN / 2048));
            Matrix_RotateZ(-frame * (i + 8) * (TRIG_TURN / 2048));
            break;
          }
          EffectPosition_ProjectPoint(&star->x, &screen);
          screen.x >>= 1;
          Matrix_Pop();
          CLAMP(screen.depth, STAR_NEAR_DEPTH, STAR_FAR_DEPTH);
          size =
              STAR_LARGEST - (screen.depth - STAR_NEAR_DEPTH) / STAR_DEPTH_STEP;
          draw[0](canvas, sparkCels + gBattleEffect_MotePictures[size - 1],
                  screen.x - size / 2, screen.y - size, size, size * 2);
          Particle_AdvanceWithGravity3D(star, 60, 0);

          // The pull back into the point; a star within four pixels of it
          // across the floor is gone.
          if (frame >= i / 8 + 24) {
            s32 dx = -star->x >> 7;
            s32 dy = -star->y >> 7;
            s32 dz = -star->z >> 7;

            star->velocityX += dx;
            star->velocityY += dy;
            star->velocityZ += dz;
            star->velocityX = star->velocityX * 62 / DAMPING_SCALE;
            star->velocityY = star->velocityY * 62 / DAMPING_SCALE;
            star->velocityZ = star->velocityZ * 62 / DAMPING_SCALE;
            if ((dx >= -2047 && dx <= 2047) && (dz >= -2047 && dz <= 2047))
              star->variant = -1;
          }
        }
      }
    }

    BattleCamera_Shake(4, 16);
    EFFECT_END_FRAME(state);
  }

  Task_Remove((s32)BattleEffect_TransferCanvas);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}

/// Unused: nothing calls it. Opens the canvas layer added over the battle,
/// darkens the background behind it, and plays the column of bubbles.
UNUSED void BattleEffect_StartBubbleColumn(struct BattleEffectParams *args) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  state->fadeStep = 0;
  BattleEffect_RunBubbleColumn(args);
}

#define BUBBLE_COLUMN_BUBBLES 256
#define BUBBLE_FRAMES 60
/// The bubbles, in the map cell buffer: each keeps its picture in z and its
/// life in variant, -1 once it has left the top of the canvas.
#define BUBBLES ((struct Particle *)(Ram_MapCellBuffer + 20480))
/// A bubble's variant while it floats: any value from 0 up.
#define BUBBLE_FLOATING 32

/// Unused: only BattleEffect_StartBubbleColumn plays it. A column of bubbles.
/// On the first frame 256 glowing ovals of random size appear below the
/// canvas; for 60 frames they float up and drift sideways, drawn with the max
/// blitter in the blue beam's palette, and each one is gone once it has risen
/// past the top. From frame 18 the targets flash one by one, eight frames
/// apart, each with the recovery sound.
void BattleEffect_RunBubbleColumn(struct BattleEffectParams *args) {
  void *canvas = Ram_HeapSlots->battleCanvas;
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  /* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle draw[2];
  s32 frame;
  s32 i;
  struct Particle *bubble;

  // Each frame the transfer task copies the canvas and clears it.
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;

  // The blue beam's palette colours the bubble cels.
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_BLUE_BEAM_SHEET),
                       SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_BUBBLE_ANIMATION_SHEET, state->sheet, 0,
                             0);

  for (frame = 0; frame != BUBBLE_FRAMES; frame++) {
    // The first frame scatters every bubble below the bottom left.
    if (frame == 0) {
      struct Particle *spawn = BUBBLES;

      for (i = 0; i != BUBBLE_COLUMN_BUBBLES; i++) {
        spawn->x = INTtoFIXED(Random16() & 127);
        spawn->y = INTtoFIXED((Random16() & 127) + 120);
        spawn->velocityX = ((Random16() & 31) - 16) << 13;
        spawn->velocityY = (8 - (Random16() & 31)) << 12;
        spawn->z = (Random16() & 7) + 5;
        spawn->variant = BUBBLE_FLOATING;
        spawn++;
      }
    }

    // Draw each bubble centred on its place, let it rise, and drop it once
    // it is wholly above the canvas.
    bubble = BUBBLES;
    for (i = 0; i != BUBBLE_COLUMN_BUBBLES; i++) {
      if (bubble->variant >= 0) {
        s32 cel = bubble->z;
        u8 *src = state->sheet + gBubbleColumn_Pictures[cel];
        s32 centerX = FIXED_INTEGER_PART(bubble->x);
        u8 width = gBubbleColumn_PictureWidths[cel];
        s32 x = centerX - (width >> 1);
        s32 centerY = FIXED_INTEGER_PART(bubble->y);
        u8 height = gBubbleColumn_PictureHeights[cel];

        draw[0](canvas, src, x, centerY - (height >> 1), width, height);
        Particle_AdvanceWithGravity2D(bubble, 63, -toFIXED(1) / 4);
        if (bubble->y < -INTtoFIXED(gBubbleColumn_PictureHeights[cel]))
          bubble->variant = -1;
      }
      bubble++;
    }

    // The targets flash in turn from frame 18; the first hit lets the
    // battle show the damage.
    for (i = 0; i != args->targetCount; i++) {
      if (frame == i * 8 + 18) {
        if (i == 0) {
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 16);
          EFFECT_SHOW_DAMAGE(SE_RECOVERY);
          Sound_PlayFar(SE_RECOVERY);
        } else {
          Sound_PlayFar(SE_RECOVERY);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 16);
        }
      }
    }

    EFFECT_END_FRAME(state);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

// Coatlicue's summon.

/// The backdrop's colour on each screen line, which DMA writes in the HBlank
/// before the line: from BATTLE_VIEW_TOP - 1 a gradient over the battle view,
/// each channel a level darker every so many lines, blue and green starting
/// above red, all lifted by `lift`; black below it.
#define GRADIENT_BLUE_LIFT 32
#define GRADIENT_GREEN_LIFT 16
#define FILL_GRADIENT(table, i, lift, c)                                       \
  do {                                                                         \
    for (; i != BATTLE_VIEW_BOTTOM - 1; i++) {                                 \
      s32 t_ = i - (BATTLE_VIEW_TOP - 1);                                      \
      s32 b_ = (lift) - t_ / GRADIENT_BLUE_LINES + GRADIENT_BLUE_LIFT;         \
      s32 g_ = (lift) - t_ / GRADIENT_GREEN_LINES + GRADIENT_GREEN_LIFT;       \
      s32 r_ = (lift) - t_ / GRADIENT_RED_LINES;                               \
                                                                               \
      if (b_ < 0)                                                              \
        b_ = 0;                                                                \
      if (b_ > RGB_LEVEL_MAX)                                                  \
        b_ = RGB_LEVEL_MAX;                                                    \
      if (g_ < 0)                                                              \
        g_ = 0;                                                                \
      if (g_ > RGB_LEVEL_MAX)                                                  \
        g_ = RGB_LEVEL_MAX;                                                    \
      if (r_ < 0)                                                              \
        r_ = 0;                                                                \
      if (r_ > RGB_LEVEL_MAX)                                                  \
        r_ = RGB_LEVEL_MAX;                                                    \
      c = RGB(r_, g_, b_);                                                     \
      *table++ = c;                                                            \
    }                                                                          \
    for (; i != DISPLAY_HEIGHT; i++) {                                         \
      *table++ = 0;                                                            \
    }                                                                          \
  } while (0)

/// Draws bubble picture n of the copy at `base`, centred on (x, y).
#define BLIT_BUBBLE(base, n, x, y)                                             \
  do {                                                                         \
    u8 *src_ = state->sheet + (base) + gCoatlicue_Pictures[n];                 \
    u8 w_ = gCoatlicue_PictureWidths[n];                                       \
    s32 x_ = (x) - (w_ >> 1);                                                  \
    u8 h_ = gCoatlicue_PictureHeights[n];                                      \
                                                                               \
    blit[0](canvas, src_, x_, (y) - (h_ >> 1), w_, h_);                        \
  } while (0)

/// Draws a rising bubble, moves it on, and drops it once it has left the top
/// of the canvas.
#define DRAW_BUBBLE(p)                                                         \
  do {                                                                         \
    s32 n_ = (p)->z;                                                           \
                                                                               \
    {                                                                          \
      u8 *src_ =                                                               \
          state->sheet + COATLICUE_BUBBLE_PICTURES + gCoatlicue_Pictures[n_];  \
      s32 x0_ = FIXED_INTEGER_PART((p)->x);                                    \
      u8 w_ = gCoatlicue_PictureWidths[n_];                                    \
      s32 x_ = x0_ - (w_ >> 1);                                                \
      s32 y_ = FIXED_INTEGER_PART((p)->y);                                     \
      u8 h_ = gCoatlicue_PictureHeights[n_];                                   \
                                                                               \
      blit[0](canvas, src_, x_, y_ - (h_ >> 1), w_, h_);                       \
    }                                                                          \
    Particle_AdvanceWithGravity2D(p, 63, -toFIXED(1) / 4);                     \
    if ((p)->y < -INTtoFIXED(gCoatlicue_PictureHeights[n_])) {                 \
      (p)->variant = -1;                                                       \
    }                                                                          \
  } while (0)

/// Coatlicue's pictures in the effect's sprite sheet: CogTexture's quarter
/// mirrored into a whole cog, 128 by 128; ScaleTexture, 128 by 16; then
/// BubbleAnimationSheet's fifteen pictures and a copy of them at half
/// brightness.
#define COATLICUE_COG 0
#define COATLICUE_SCALES (128 * 128)
#define COATLICUE_BUBBLE_PICTURES (COATLICUE_SCALES + 128 * 16)
#define BUBBLE_SHEET_BYTES 3000
#define BUBBLE_PICTURE_COUNT 15
#define COATLICUE_DIM_BUBBLE_PICTURES                                          \
  (COATLICUE_BUBBLE_PICTURES + BUBBLE_SHEET_BYTES)
/// The coils' band in the map cell buffer, past the fire ring's texture: its
/// corners, two for each of its segments, and its mesh.
#define COATLICUE_COIL_CORNERS (Ram_MapCellBuffer + 17408)
#define COATLICUE_COIL_MESH (Ram_MapCellBuffer + 18432)
#define COATLICUE_COIL_SEGMENTS 32
/// Coatlicue's figure, CoatlicueTiles: twelve 32x32 sprites, three across.
#define COATLICUE_SPRITES 12
#define COATLICUE_COLUMNS 3
#define COATLICUE_SPRITE_SIZE 32
#define COATLICUE_BUBBLE_COUNT 384
/// The bubbles of the cloud scattered at frame 310, the only ones drawn from
/// frame 360, and after them those of the stream from frame 274, one a frame.
#define COATLICUE_CLOUD_BUBBLES 80
#define COATLICUE_STREAM_BUBBLES 24
/// A bubble of the cloud is drawn at its kind's own picture at this depth,
/// six pictures smaller for every COATLICUE_CLOUD_DEPTH_STEP deeper.
#define COATLICUE_CLOUD_NEAR_DEPTH 228
#define COATLICUE_CLOUD_DEPTH_STEP 50
/// The bursts, columns of bubbles rising from the floor, each frame in
/// gCoatlicue_BurstFrames, in the state's particles.
#define COATLICUE_BURSTS 9
#define COATLICUE_BURST_BUBBLES 7
/// Pressing A or B whitens the canvas over these frames and ends the summon.
#define COATLICUE_SKIP_FRAMES 18
/// The canvas's level as it flashes.
#define COATLICUE_FLASH_LEVEL 47
/// The backdrop's brightness at its flare, from frame 300.
#define COATLICUE_BACKDROP_FLARE 14
/// How far the cog and the fire ring lean back, the cog at last
/// COATLICUE_TILT_MIN; and how far the cog is rolled while it spins up.
#define COATLICUE_TILT 4000
#define COATLICUE_TILT_MIN 3000
#define COATLICUE_COG_ROLL 74752
/// The cog of light appears at COATLICUE_COG_FRAME, fading in over eight
/// frames and growing over 64 from a quarter of its size to
/// COATLICUE_COG_SCALE; at COATLICUE_COG_SWELL_FRAME it swells over 16 more to
/// COATLICUE_COG_SCALE_MAX, leaning back to COATLICUE_TILT_MIN a 1024th of a
/// turn a frame, and from COATLICUE_COG_FADE_FRAME it fades away.
#define COATLICUE_COG_FRAME 210
#define COATLICUE_COG_SWELL_FRAME 274
#define COATLICUE_COG_FADE_FRAME 310
#define COATLICUE_COG_SCALE 55536
#define COATLICUE_COG_SCALE_MAX 85536
#define COATLICUE_COG_GROWTH ((COATLICUE_COG_SCALE - FIXED_ONE / 4) / 64)
#define COATLICUE_COG_SWELL                                                    \
  ((COATLICUE_COG_SCALE_MAX - COATLICUE_COG_SCALE) / 16)
/// The cog's glows about her each grow to full size over 22 frames.
#define COATLICUE_GLOW_GROWTH (FIXED_ONE / 22)
/// The cloud of bubbles is drawn squashed: about 0.85 of its size across, 0.69
/// high and 0.42 deep. Flung away at
/// COATLICUE_FLING_FRAME, it stretches across from a 16th of its width by
/// COATLICUE_FLING_STRETCH a frame.
#define COATLICUE_CLOUD_WIDTH_SCALE 55536
#define COATLICUE_CLOUD_HEIGHT_SCALE 45536
#define COATLICUE_CLOUD_DEPTH_SCALE 27768
#define COATLICUE_FLING_FRAME 540
#define COATLICUE_FLING_STRETCH 1500
/// The fire ring's flashes at gCoatlicue_RingFrames, as it shines at
/// gCoatlicue_FlashFrames, the cog's glows at gCoatlicue_GlowFrames, and the
/// two coils of scaled water.
#define COATLICUE_RINGS 4
#define COATLICUE_FLASHES 3
#define COATLICUE_GLOWS 3
/// As it shines the fire ring brightens COATLICUE_FLASH_SHADE_STEP a frame
/// from COATLICUE_FLASH_SHADE, to its full light from its fourth frame.
#define COATLICUE_FLASH_SHADE (-56)
#define COATLICUE_FLASH_SHADE_STEP 16
#define COATLICUE_COILS 2
/// Each coil leans forward by COATLICUE_COIL_TILT as it starts, the second at
/// COATLICUE_SECOND_COIL_START, and leans back COATLICUE_COIL_TILT_SPEED a
/// frame.
#define COATLICUE_COIL_TILT 3000
#define COATLICUE_COIL_TILT_SPEED 22
#define COATLICUE_SECOND_COIL_START 82
/// From COATLICUE_COIL_SHRINK_FRAME the coils shrink away a 16th a frame from
/// COATLICUE_COIL_SHRINK_SCALE, the second from COATLICUE_SECOND_COIL_EXTRA
/// more, each gone below a quarter.
#define COATLICUE_COIL_SHRINK_FRAME 294
#define COATLICUE_COIL_SHRINK_SCALE 75536
#define COATLICUE_SECOND_COIL_EXTRA 26384
/// The healing's frames after the summon, and how far toward blue it lifts
/// the battle background.
#define COATLICUE_WASH_FRAMES 60
#define COATLICUE_WASH_COGS 3
#define COATLICUE_WASH_BLUE 11

#define COATLICUE_FRAMES 602 ///< frames of the summon after its opening

/// Coatlicue: the battle gives way to deep blue, where two coils of scaled
/// water turn and a ring of light flashes. From frame 210 a cog of light
/// rises; a white flash at 273 brings Coatlicue in as a cyan silhouette among
/// rising bubbles, and she takes her colours. The bubbles gather into a cloud
/// that spins, flashes three times and is flung away as she leaves; then
/// three cogs of blue light rise over the party, and each target flashes as
/// it is healed. A or B skips the summon.
///
/// Abilities: Coatlicue.
void BattleEffect_RunCoatlicue(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectState *state;
  u8 *sprites;
  u8 *cellBuffer = gMapGridCellBuffer;
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle blit[2];
  struct Point2D grid;
  s32 skip;
  u16 *table;
  s32 frame;
  s32 i;
  s32 k;
  s32 y;
  s32 lift;
  s32 color;
  s32 scale;
  s32 shade;
  s32 rise;
  s32 spin;
  /* FAKEMATCH(temporary): zero, 0 held in a local; as 0 at its uses. */
  s32 zero;
  u8 *coilMesh = COATLICUE_COIL_MESH;
  s16 units[UNIT_MAX];
  FIXED placement[4];
  struct Scale figureScale;
  struct ScreenPoint bubbleScreen;
  struct ModelTexture texture;
  struct ModelTexture washTexture;
  struct Sprite *sprite;
  struct Particle *bubble;

  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FillWhiteBand();
  state->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  y = -80;
  for (frame = 0; frame != 27; frame++) {
    y += 4;
    REG_BG2X = y << 8;
    EFFECT_PRESENT_CANVAS(state);
  }
  zero = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  table = state->backdropLines;
  lift = -8;
  color = 0;
  for (i = 0; i != BATTLE_VIEW_TOP - 1; i++) {
    *table++ = color;
  }
  FILL_GRADIENT(table, i, lift, color);
  Task_Add(BattleEffect_ArmPaletteHBlankDma, TASK_PASS_VBLANK + 126);
  skip = 0;
  Mesh_BuildCylinderBand(COATLICUE_COIL_CORNERS, coilMesh,
                         COATLICUE_COIL_SEGMENTS);
  units[0] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(units, 0);
  {
    u8 *tiles = Resource_GetTableEntry(RESOURCE_COATLICUE_TILES);

    Iwram_CopyWords((void *)OBJ_PLTT, tiles, OBJ_PLTT_SIZE);
    Decompress_TaggedLz(tiles + OBJ_PLTT_SIZE, cellBuffer);
    {
      u8 *cells;

      i = 0;
      cells = gMapGridCellBuffer;
      for (; i != COATLICUE_SPRITES; cells += OBJ_32X32_8BPP_BYTES, i++) {
        sprite = EffectSprite_CreateWithTiles(
            32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
        sprite->image.priority = 2;
        state->sprites[i] = sprite;
        Iwram_CopyWords((void *)OBJ_TILES(sprite), cells, OBJ_32X32_8BPP_BYTES);
      }
    }
  }
  Graphics_LoadGreyRamp();
  Resource_LoadAndDecompress(RESOURCE_COG_TEXTURE, cellBuffer, 0, 0);
  Picture_MirrorIntoQuadrants(cellBuffer, state->sheet + COATLICUE_COG, 64, 64);
  Resource_LoadAndDecompress(RESOURCE_SCALE_TEXTURE,
                             state->sheet + COATLICUE_SCALES, 0, 0);
  {
    u8 *src = state->sheet + COATLICUE_BUBBLE_PICTURES;

    Resource_LoadAndDecompress(RESOURCE_BUBBLE_ANIMATION_SHEET, src, 0, 0);
    for (i = 0; i != BUBBLE_SHEET_BYTES; i++) {
      state->sheet[COATLICUE_DIM_BUBBLE_PICTURES + i] = src[i] >> 1;
    }
  }
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cellBuffer, 0, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  for (frame = 0; frame != COATLICUE_FRAMES; frame++) {
    // A 128th of a turn a frame.
    spin = ((frame - 1) << 9) + TRIG_TURN / 128;
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame <= 581 && skip == 0) {
      skip = 1;
    }
    // Once skipped, the canvas whitens four levels a frame.
    if (skip > 0) {
      s32 *value = &state->transferValue;
      s32 v = CANVAS_FILL_WORD(skip) << 2;

      *value = v;
      if (v > CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST)) {
        *value = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
      }
      state->transferMode = CANVAS_COPY_AND_CLEAR;
      if (++skip == COATLICUE_SKIP_FRAMES + 1) {
        break;
      }
    }
    if (frame == 0) {
      for (i = 0; i != COATLICUE_BUBBLE_COUNT; i++) {
        BUBBLES[i].variant = -1;
      }
      for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
        state->particles[i].variant = -1;
      }
      grid.x = toFIXED(88);
      grid.y = toFIXED(36);
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if (frame == 82) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(COATLICUE_FLASH_LEVEL));
      Sound_PlayFar(SE_PSYNERGY_START);
    }
    if (frame == 210) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(COATLICUE_FLASH_LEVEL));
      Sound_PlayFar(SE_PSYNERGY_START);
    }
    if (frame == 274) {
      Sound_PlayFar(SE_DJINNI_SET);
    }
    if (frame == 310) {
      Sound_PlayFar(SE_CURSE_FALL);
    }
    if (frame == 316) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 466) {
      Sound_PlayFar(SE_SWIRL);
    }
    if (frame == 498) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 503) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 508) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 520) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 540) {
      Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
    }
    // The backdrop brightens a level every 32 frames until it is 8 up; it
    // flares from frame 300, settles back from 312, and brightens from 540.
    // DEAD: frame never reaches 1999999, so the test always passes; the ROM
    // keeps its load and compare.
    if (frame <= 1999999) {
      u16 *row = state->backdropLines;
      s32 brightness = frame / 32 - 8;

      if (brightness > 8) {
        brightness = 8;
      }
      color = 0;
      for (i = 0; i != BATTLE_VIEW_TOP - 1; i++) {
        *row++ = color;
      }
      if (frame > 539) {
        brightness = (frame - 540) / 2 + 8;
      } else if (frame > 311) {
        brightness = COATLICUE_BACKDROP_FLARE - (frame - 312) / 4;
        if (brightness <= 7) {
          brightness = 8;
        }
      } else if (frame > 299) {
        brightness += (frame - 300) / 2;
        if (brightness > COATLICUE_BACKDROP_FLARE) {
          brightness = COATLICUE_BACKDROP_FLARE;
        }
      }
      i = BATTLE_VIEW_TOP - 1;
      FILL_GRADIENT(row, i, brightness, color);
    }
    // From frame 282 every sprite colour is one cyan, brightening: her
    // silhouette.
    if ((u32)(frame - 282) <= 17) {
      s32 green = (frame - 282) / 2 + 14;
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 blue = frame - 253;
      u32 color;

      if (green > RGB_LEVEL_MAX) {
        green = RGB_LEVEL_MAX;
      }
      if (blue > RGB_LEVEL_MAX) {
        blue = RGB_LEVEL_MAX;
      }
      i = 0;
      color = RGB(0, green, blue);
      for (; i != OBJ_PLTT_SIZE / 2; i++) {
        *dst++ = color;
      }
    }
    // From frame 300 every sprite colour steps two levels a frame toward
    // Coatlicue's own, lifted by redLift, greenLift and blueLift: first to
    // white, then to a pale cyan, then to her colours.
    if (frame > 299) {
      u16 *source = Resource_GetTableEntry(RESOURCE_COATLICUE_TILES);
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 redLift = RGB_LEVEL_MAX;
      s32 greenLift = RGB_LEVEL_MAX;
      s32 blueLift = 15;

      if (frame > 315) {
        redLift = 4;
        greenLift = 12;
        blueLift = 16;
      }
      if (frame > 337) {
        redLift = 0;
        greenLift = 0;
        blueLift = 0;
      }
      for (i = 0; i != OBJ_PLTT_SIZE / 2; i++, source++, dst++) {
        s32 blue = RGB_BLUE(*dst);
        s32 green = RGB_GREEN(*dst);
        s32 red = RGB_RED(*dst);
        s32 goalBlue = RGB_BLUE(*source) + blueLift;
        s32 goalGreen = RGB_GREEN(*source) + greenLift;
        s32 goalRed = RGB_RED(*source) + redLift;

        if (blue < goalBlue - 1) {
          blue += 2;
        } else if (blue > goalBlue + 1) {
          blue -= 2;
        }
        if (green < goalGreen - 1) {
          green += 2;
        } else if (green > goalGreen + 1) {
          green -= 2;
        }
        if (red < goalRed - 1) {
          red += 2;
        } else if (red > goalRed + 1) {
          red -= 2;
        }
        if (blue > RGB_LEVEL_MAX) {
          blue = RGB_LEVEL_MAX;
        }
        if (green > RGB_LEVEL_MAX) {
          green = RGB_LEVEL_MAX;
        }
        if (red > RGB_LEVEL_MAX) {
          red = RGB_LEVEL_MAX;
        }
        if (blue < 0) {
          blue = 0;
        }
        if (green < 0) {
          green = 0;
        }
        if (red < 0) {
          red = 0;
        }
        *dst = RGB(red, green, blue);
      }
    }
    // From frame 520 her colours brighten, red fastest and blue slowest.
    if (frame > 519) {
      s32 lift = frame - 520;
      u16 *source = Resource_GetTableEntry(RESOURCE_COATLICUE_TILES);
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 half;

      i = 0;
      half = lift / 2;
      for (; i != OBJ_PLTT_SIZE / 2; i++, source++, dst++) {
        s32 blue;
        s32 green;
        s32 red;

        red = RGB_RED(*source) + lift;
        green = RGB_GREEN(*source) + half;
        blue = RGB_BLUE(*source) + lift / 4;

        if (red > RGB_LEVEL_MAX) {
          red = RGB_LEVEL_MAX;
        }
        if (green > RGB_LEVEL_MAX) {
          green = RGB_LEVEL_MAX;
        }
        if (blue > RGB_LEVEL_MAX) {
          blue = RGB_LEVEL_MAX;
        }
        *dst = RGB(red, green, blue);
      }
    }
    // Her figure, twelve sprites, from frame 292: back in place at 360, it
    // drifts a quarter pixel a frame from 396 to 507 and moves off 16 pixels
    // a frame from 540.
    if ((u32)(frame - 292) <= 253) {
      figureScale = gCoatlicue_UnitScale;
      placement[1] = toFIXED(255);
      placement[3] = zero;
      if (frame == 360) {
        grid.y = toFIXED(36);
      }
      if ((u32)(frame - 396) <= 111) {
        grid.y += toFIXED(1) / 4;
      }
      if (frame > 539) {
        grid.y += toFIXED(16);
      }
      for (i = 0; i != COATLICUE_SPRITES; i++) {
        placement[0] =
            INTtoFIXED((i % COATLICUE_COLUMNS) * COATLICUE_SPRITE_SIZE) +
            grid.x;
        placement[2] =
            INTtoFIXED((i / COATLICUE_COLUMNS) * COATLICUE_SPRITE_SIZE) +
            grid.y;
        Render_ApplyProjectedPlacementFar(state->sprites[i], placement,
                                          (s32 *)&figureScale, 0);
      }
    }
    // The stream, from frame 274.
    {
      s32 d = frame - 274;
      if ((u32)d <= COATLICUE_STREAM_BUBBLES - 1) {
        struct Particle *bubble = &(&BUBBLES[COATLICUE_CLOUD_BUBBLES])[d];
        {
          s32 x = INTtoFIXED((Random16() & 15) + 52);

          bubble->y = toFIXED(100);
          bubble->x = x;
        }
        bubble->velocityX = ((Random16() & 31) - 16) << 12;
        bubble->velocityY = -((Random16() & 15) + 8) << 12;
        bubble->z = (Random16() & 7) + 5;
        bubble->variant = BUBBLE_FLOATING;
      }
    }
    // The cloud, at frame 310.
    if (frame == 310) {
      struct Particle *bubble = BUBBLES;

      for (i = 0; i != COATLICUE_CLOUD_BUBBLES; i++, bubble++) {
        bubble->x = INTtoFIXED((Random16() & 63) + 28);
        bubble->y = INTtoFIXED(Random16() % 80 + 56);
        bubble->velocityX = ((Random16() & 31) - 16) << 12;
        bubble->velocityY = (8 - (Random16() & 31)) << 12;
        bubble->z = (Random16() & 7) + 5;
        bubble->variant = BUBBLE_FLOATING;
      }
    }
    for (k = 0; k != COATLICUE_BURSTS; k++) {
      if (frame == gCoatlicue_BurstFrames[k]) {
        for (i = 0; i != COATLICUE_BURST_BUBBLES; i++) {
          struct Particle *bubble =
              &state->particles[(k % COATLICUE_BURSTS) *
                                    COATLICUE_BURST_BUBBLES +
                                i];

          bubble->x = INTtoFIXED(Random16() % gCoatlicue_BurstSpreadX[k] +
                                 gCoatlicue_BurstX[k]);
          bubble->y =
              INTtoFIXED((Random16() & 3) + i * gCoatlicue_BurstSpacingY[k] +
                         gCoatlicue_BurstY[k]);
          bubble->velocityX = gCoatlicue_BurstVelocityX[k] * (toFIXED(1) / 16);
          bubble->velocityY = gCoatlicue_BurstVelocityY[k] * (toFIXED(1) / 16);
          bubble->z = INTtoFIXED((Random16() & 3) + gCoatlicue_BurstZ[k]);
          bubble->velocityZ = gCoatlicue_BurstVelocityZ[k] * (toFIXED(1) / 64);
          bubble->variant = zero;
        }
      }
    }
    // Until frame 360 the bubbles rise on the canvas; then the cloud's turn
    // as one body, which spins from 454, flips at 518 and is flung up at 540.
    if (frame <= 359) {
      bubble = BUBBLES;

      for (i = 0; i != COATLICUE_BUBBLE_COUNT; i++, bubble++) {
        if (bubble->variant >= 0) {
          DRAW_BUBBLE(bubble);
        }
      }
    } else {
      Matrix_Initialize();
      Matrix_Translate(0, 0, toFIXED(256));
      if (frame > 539) {
        Matrix_Scale(frame * COATLICUE_FLING_STRETCH -
                         (COATLICUE_FLING_FRAME * COATLICUE_FLING_STRETCH -
                          FIXED_ONE / 16),
                     FIXED_ONE, FIXED_ONE);
      } else if (frame > 517) {
        Matrix_Scale(0, 0, FIXED_ONE);
      } else if (frame > 485) {
        s32 scale = 518 * (FIXED_ONE / 32) - frame * (FIXED_ONE / 32);

        Matrix_Scale(scale, scale, FIXED_ONE);
      }
      if (frame > 539) {
        Matrix_RotateY(frame * (TRIG_TURN / 64));
      } else if (frame > 517) {
        Matrix_RotateY(TRIG_TURN);
        Matrix_RotateX(DEGtoANG(180));
      } else if (frame > 453) {
        // A 64th of a turn a frame, rocking as it turns.
        s32 a = (frame - 454) << 10;

        Matrix_RotateY(a);
        Matrix_RotateX(Trig_Sin(a) >> 3);
      }
      Matrix_Scale(COATLICUE_CLOUD_WIDTH_SCALE, COATLICUE_CLOUD_HEIGHT_SCALE,
                   COATLICUE_CLOUD_DEPTH_SCALE);
      if (frame == 360) {
        struct Particle *bubble;

        if (skip == 0) {
          EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
                              CANVAS_FADE_THREE_QUARTERS);
        }
        bubble = BUBBLES;
        for (i = 0; i != COATLICUE_BUBBLE_COUNT; i++, bubble++) {
          bubble->variant = Random16() & 3;
          SET_POSITION(bubble, INTtoFIXED(Random16() % 160 - 80),
                       INTtoFIXED(Random16() % 160 + 48),
                       INTtoFIXED(Random16() % 160 - 80));
          bubble->velocityX = zero;
          // Each rises half a pixel to a pixel and a half a frame, and a
          // pixel more for the first kind, half as much for each after.
          bubble->velocityY =
              (-((Random16() & 63) + 32) << 10) - (1 << (16 - bubble->variant));
          bubble->velocityZ = Random16();
        }
      }
      if (frame == 540) {
        struct Particle *bubble = BUBBLES;

        i = 0;
        for (; i != COATLICUE_BUBBLE_COUNT; i++, bubble++) {
          bubble->y = zero;
          bubble->velocityY = -((Random16() & 63) + 32) << 15;
        }
      }
      {
        bubble = BUBBLES;

        for (i = 0; i != COATLICUE_CLOUD_BUBBLES; i++, bubble++) {
          s32 n;

          if (bubble->variant < 0) {
            continue;
          }
          EffectPosition_ProjectPoint(bubble, &bubbleScreen);
          bubbleScreen.depth =
              FIXED_INTEGER_PART(ScreenPoint_Coordinates(&bubbleScreen)[2]) -
              COATLICUE_CLOUD_NEAR_DEPTH;
          n = bubbleScreen.depth * 6 / COATLICUE_CLOUD_DEPTH_STEP +
              gCoatlicue_PictureBias[bubble->variant];
          if (n < 0) {
            n = 0;
          }
          if (n > BUBBLE_PICTURE_COUNT - 1) {
            n = BUBBLE_PICTURE_COUNT - 1;
          }
          {
            s32 x =
                FIXED_INTEGER_PART(ScreenPoint_Coordinates(&bubbleScreen)[0]);

            bubbleScreen.y =
                FIXED_INTEGER_PART(ScreenPoint_Coordinates(&bubbleScreen)[1]);
            bubbleScreen.x = x + 60;
          }
          if (frame > 539) {
            bubbleScreen.y = bubbleScreen.y + (frame << 4) - 8608;
          } else if (frame > 517) {
            bubbleScreen.y = bubbleScreen.y + 16;
          } else if (frame <= 453) {
            bubbleScreen.y = bubbleScreen.y + 80;
          } else {
            bubbleScreen.y = bubbleScreen.y - frame + 534;
          }
          if (bubble->variant <= 1) {
            BLIT_BUBBLE(COATLICUE_BUBBLE_PICTURES, n, bubbleScreen.x,
                        bubbleScreen.y);
          } else {
            BLIT_BUBBLE(COATLICUE_DIM_BUBBLE_PICTURES, n, bubbleScreen.x,
                        bubbleScreen.y);
          }
          if (frame <= 453 || frame >= i / 16 + 540) {
            bubble->y += bubble->velocityY;
          }
        }
      }
    }
    // Each burst rises as a column, its bubbles stacked from the lowest and
    // swaying, until its last has left the top.
    for (k = 0; k != COATLICUE_BURSTS; k++) {
      FIXED top = 0;

      for (i = 0; i != COATLICUE_BURST_BUBBLES; i++) {
        struct Particle *step =
            &state->particles[k * COATLICUE_BURST_BUBBLES + i];

        if (state
                ->particles[k * COATLICUE_BURST_BUBBLES +
                            COATLICUE_BURST_BUBBLES - 1]
                .variant >= 0) {
          s32 n = BUBBLE_PICTURE_COUNT - 1 - FIXED_INTEGER_PART(step->z);
          s32 x = FIXEDtoINT(
              step->x + (Trig_Sin((frame - (i % COATLICUE_BURST_BUBBLES) * 8) *
                                  (TRIG_TURN / 64))
                         << 2));

          if (i == 0) {
            top = step->y;
          }
          if (n < 0) {
            n = 0;
          }
          {
            u8 *src = state->sheet + COATLICUE_DIM_BUBBLE_PICTURES +
                      gCoatlicue_Pictures[n];
            u8 w = gCoatlicue_PictureWidths[n];
            s32 x0 = x - (w >> 1);
            s32 y0 = FIXEDtoINT(top);
            u8 h = gCoatlicue_PictureHeights[n];

            blit[0](canvas, src, x0, y0 - (h >> 1), w, h);
          }
          Particle_AdvanceWithGravity3D(step, PARTICLE_DAMPING_NONE, 0);
          if (step->z < 0) {
            step->z = zero;
          }
          if (i == COATLICUE_BURST_BUBBLES - 1 &&
              step->y < -INTtoFIXED(gCoatlicue_PictureHeights[n])) {
            step->variant = -1;
          }
          top = top + step->z + toFIXED(8);
        }
      }
    }
    {
      void *verts;
      s32 cogFrame;
      struct RenderModel *model;

      verts = Heap_AllocEwram(512);
      model = Render_AllocateModelList(1);
      model->texture = &texture;
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->vertices = verts;
      // The cog, from frame 210: it rises and grows as it fades in, spinning,
      // then from 274 rises and swells again, leaning back, and from 310
      // fades away.
      cogFrame = frame - COATLICUE_COG_FRAME;
      if ((u32)cogFrame <= 107) {
        s32 pitch;

        rise = frame * (toFIXED(3) / 8) - toFIXED(315) / 4;
        pitch = COATLICUE_TILT;
        if (rise > toFIXED(24)) {
          rise = toFIXED(24);
        }
        scale = (frame - COATLICUE_COG_FRAME) * COATLICUE_COG_GROWTH +
                FIXED_ONE / 4;
        if (scale > COATLICUE_COG_SCALE) {
          scale = COATLICUE_COG_SCALE;
        }
        if (frame >= COATLICUE_COG_SWELL_FRAME) {
          rise = frame * (toFIXED(1) / 2) - toFIXED(113);
          if (rise > toFIXED(32)) {
            rise = toFIXED(32);
          }
          pitch =
              COATLICUE_TILT + COATLICUE_COG_SWELL_FRAME * 64 - (frame << 6);
          if (pitch < COATLICUE_TILT_MIN) {
            pitch = COATLICUE_TILT_MIN;
          }
          scale =
              scale + (frame - COATLICUE_COG_SWELL_FRAME) * COATLICUE_COG_SWELL;
          if (scale > COATLICUE_COG_SCALE_MAX) {
            scale = COATLICUE_COG_SCALE_MAX;
          }
        }
        model->kind = MODEL_SHADED_MAX;
        texture.pixels = state->sheet + COATLICUE_COG;
        shade = (frame << 3) - (COATLICUE_COG_FRAME + 8) * 8;
        if (shade > 0) {
          shade = 0;
        }
        if (frame >= COATLICUE_COG_FADE_FRAME) {
          shade = (COATLICUE_COG_FADE_FRAME - frame) << 3;
        }
        model->shade = shade;
        Matrix_Initialize();
        Matrix_Translate(0, rise, 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_ScaleUniform(scale);
        if (frame < COATLICUE_COG_SWELL_FRAME) {
          Matrix_RotateZ(spin - COATLICUE_COG_ROLL);
        }
        Matrix_RotateX(pitch);
        Matrix_RotateY(spin);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 64);
        Render_DrawModelList(model);
      }
      // A white flash from frame 273, four levels dimmer each frame.
      if ((u32)(frame - 273) <= 7 && skip == 0) {
        s32 d = frame - 273;
        s32 *value;

        state->transferMode = CANVAS_COPY_AND_CLEAR;
        value = &state->transferValue;
        *value = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST) -
                 (CANVAS_FILL_WORD(d) << 2);
      }
      if (frame == 281 && skip == 0) {
        EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      }
      // The fire ring, from frame 274, growing and spinning, fading from 294.
      if ((u32)(frame - 274) <= 27) {
        scale = (frame - 266) * (FIXED_ONE / 32);
        texture.pixels = cellBuffer;
        model->kind = MODEL_SHADED_ADDITIVE;
        shade = 0;
        if (frame > 293) {
          shade = (294 - frame) << 3;
        }
        model->shade = shade;
        Matrix_Initialize();
        Matrix_Translate(0, toFIXED(28), 0);
        Matrix_ScaleUniform(scale);
        Matrix_RotateX(COATLICUE_TILT);
        Matrix_RotateY(frame * (TRIG_TURN / 128));
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 64);
        Render_DrawModelList(model);
      }
      // The fire ring flashes outward for ten frames from each of its frames,
      // the last two about her.
      if (frame > 80) {
        for (i = 0; i != COATLICUE_RINGS; i++) {
          s32 startFrame = gCoatlicue_RingFrames[i];

          if (frame > startFrame && frame < startFrame + 10) {
            s32 d;

            texture.pixels = gMapGridCellBuffer;
            shade = MODEL_SHADE_INVISIBLE / 4;
            d = frame - startFrame;
            model->kind = MODEL_SHADED_MAX;
            model->shade = shade;
            Matrix_Initialize();
            scale = d * (FIXED_ONE / 8);
            if (i > 1) {
              s32 rise = -toFIXED(44);

              // From frame 540 they move off with her.
              if (frame > 539) {
                rise += INTtoFIXED((frame - 539) * 16);
              }
              Matrix_Translate(-toFIXED(4), rise, 0);
              Matrix_Scale(scale, scale, scale);
            } else {
              Matrix_Scale(scale, d * (FIXED_ONE / 4), scale);
            }
            Matrix_RotateX(DEGtoANG(90));
            Matrix_RotateY(frame * (TRIG_TURN / 128));
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     64);
            Render_DrawModelList(model);
          }
        }
      }
      // At each of its flash frames the canvas flashes and the fire ring
      // shines about her, shrinking, turned a quarter more each time.
      if (frame > 80) {
        for (i = 0; i != COATLICUE_FLASHES; i++) {
          if (frame == gCoatlicue_FlashFrames[i]) {
            Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                                 CANVAS_FILL_WORD(COATLICUE_FLASH_LEVEL));
          }
          if (frame > gCoatlicue_FlashFrames[i] &&
              frame < gCoatlicue_FlashFrames[i] + 10) {
            s32 d = frame - gCoatlicue_FlashFrames[i];
            scale = FIXED_ONE * 5 / 4 - d * (FIXED_ONE / 8);
            texture.pixels = cellBuffer;
            model->kind = MODEL_SHADED_ADDITIVE;
            shade = d * COATLICUE_FLASH_SHADE_STEP + COATLICUE_FLASH_SHADE;
            if (shade > 0) {
              shade = 0;
            }
            model->shade = shade;
            Matrix_Initialize();
            Matrix_Translate(-toFIXED(4), -toFIXED(44), 0);
            Matrix_Scale(scale, scale << 1, scale);
            Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
            Matrix_RotateX(-TRIG_TURN / 16);
            Matrix_RotateY(frame * (TRIG_TURN / 128));
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     64);
            Render_DrawModelList(model);
          }
        }
      }
      // The cog glows about her from each of its glow frames, growing.
      if (frame > 80) {
        // UB: the loop reads three glow frames from a table of one; the two
        // after it are the coils' bytes, read as frames 20992 and 4096, which
        // never come.
#ifdef UBFIX
        for (i = 0; i != 1; i++) {
#else
        for (i = 0; i != COATLICUE_GLOWS; i++) {
#endif
          s32 startFrame = gCoatlicue_GlowFrames[i];

          if (frame > startFrame && frame < startFrame + 100) {
            rise = -toFIXED(44);
            scale = (frame - startFrame) * COATLICUE_GLOW_GROWTH;
            if (scale > FIXED_ONE - 1) {
              scale = FIXED_ONE;
            }
            texture.pixels = state->sheet + COATLICUE_COG;
            shade = MODEL_SHADE_INVISIBLE / 4;
            model->kind = MODEL_SHADED_MAX;
            model->shade = shade;
            Matrix_Initialize();
            // From frame 540 it moves off with her.
            if (frame > 539) {
              rise = INTtoFIXED((frame - 539) * 16) - toFIXED(44);
            }
            Matrix_Translate(-toFIXED(4), rise, 0);
            Matrix_Scale(scale, scale / 2, scale);
            Matrix_RotateX(DEGtoANG(90));
            Matrix_RotateY(frame * (TRIG_TURN / 32));
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     64);
            Render_DrawModelList(model);
          }
        }
      }
      // The coils, bands of scaled water about her: each grows from its start,
      // the second sweeping in from the left and fading in; from frame 210
      // they shrink with the cog's rise, from 274 swell and fade, and from
      // 294 shrink away.
      texture.logWidth = 7;
      texture.logHeight = 4;
      texture.pixels = state->sheet + COATLICUE_SCALES;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->mesh = coilMesh;
      model->vertices = verts;
      model->scrollU = zero;
      model->scrollV = zero;
      for (i = 0; i != COATLICUE_COILS; i++) {
        if (frame < gCoatlicue_CoilStarts[i]) {
          continue;
        }
        if (frame < COATLICUE_COIL_SHRINK_FRAME) {
          scale = gCoatlicue_CoilScales[i] +
                  (frame - gCoatlicue_CoilStarts[i]) * gCoatlicue_CoilGrowth[i];
          if (scale > gCoatlicue_CoilMaxScales[i]) {
            scale = gCoatlicue_CoilMaxScales[i];
          }
          if (frame > 209) {
            scale -= cogFrame * gCoatlicue_CoilGrowth[i];
          }
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          if (frame > 273) {
            scale = scale + (frame - 274) * (FIXED_ONE / 8);
          }
          Matrix_ScaleUniform(scale);
          if (i == 0) {
            if (frame > 209) {
              s32 distance = cogFrame;

              if (distance > 48) {
                distance = 48;
              }
              Matrix_Translate(0, INTtoFIXED(distance), 0);
            }
            Matrix_RotateZ(gCoatlicue_CoilRolls[i] - (frame << 8));
            shade = MODEL_SHADE_INVISIBLE / 4;
          } else {
            s32 x = -48;

            if (frame > 127) {
              x = frame - 176;
            }
            if (x > -16) {
              x = -16;
            }
            Matrix_Translate(INTtoFIXED(x), 0, 0);
            Matrix_RotateZ(gCoatlicue_CoilRolls[i] + (frame << 8));
            if (frame > 209) {
              Matrix_RotateZ((frame - 210) * (TRIG_TURN / 256));
            }
            shade = ((frame - gCoatlicue_CoilStarts[i]) << 2) +
                    MODEL_SHADE_INVISIBLE;
            if (shade > MODEL_SHADE_INVISIBLE / 4) {
              shade = MODEL_SHADE_INVISIBLE / 4;
            }
          }
          if (frame > 273) {
            shade = 1080 - (frame << 2);
            if (i == 0) {
              Matrix_RotateZ((274 - frame) * (TRIG_TURN / 64));
            } else {
              Matrix_RotateZ((frame - 274) * (TRIG_TURN / 128));
            }
          }
        } else {
          shade = (frame << 2) - (i << 3) - 1240;
          if (shade > MODEL_SHADE_INVISIBLE / 4) {
            shade = MODEL_SHADE_INVISIBLE / 4;
          }
          scale = i * COATLICUE_SECOND_COIL_EXTRA - frame * (FIXED_ONE / 16) +
                  (COATLICUE_COIL_SHRINK_FRAME * (FIXED_ONE / 16) +
                   COATLICUE_COIL_SHRINK_SCALE);
          if (scale < FIXED_ONE / 4) {
            continue;
          }
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_Translate(0, toFIXED(32), 0);
          Matrix_ScaleUniform(scale);
        }
        if (shade > MODEL_SHADE_INVISIBLE) {
          model->shade = shade;
          Matrix_RotateX((frame - i * COATLICUE_SECOND_COIL_START) *
                             COATLICUE_COIL_TILT_SPEED -
                         COATLICUE_COIL_TILT);
          Matrix_RotateY(-frame * (TRIG_TURN / 64));
          Render_TransformVertices(COATLICUE_COIL_CORNERS, verts,
                                   2 * COATLICUE_COIL_SEGMENTS);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }
    while (state->vblanksSinceTransfer <= 1) {
    }
    EFFECT_PRESENT_CANVAS(state);
  }
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
  for (i = 0; i != COATLICUE_SPRITES; i++) {
    Sprite_DestroyFar(state->sprites[i]);
  }
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT,
          (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  BattleEffect_ShowLivingTargets(state->args);
  wait(1);
  Task_Remove(BattleEffect_ArmPaletteHBlankDma);
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  wait(1);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                       SHEET_PALETTE_BYTES);
  Iwram_FillWords(canvas, CANVAS_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  // The healing: the battle background lifted toward blue, bubbles rising,
  // the targets flashing in turn, and three cogs of light rising over them.
  Palette_BrightenSavedBattleBg(0, 0, COATLICUE_WASH_BLUE);
  frame = 0;
  for (; frame != COATLICUE_WASH_FRAMES; frame++) {
    s32 scale;
    s32 height;
    struct RenderModel *washModel;
    void *washVertices;
    s32 cogFrame;

    if (frame == 16) {
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);
    }
    if (frame > 52) {
      Palette_BrightenBgEntries(-4, -4, -4);
    }
    if (frame > 48) {
      s32 fade = frame - 48;

      Palette_BrightenSavedBattleBg(-fade, -fade,
                                    COATLICUE_WASH_BLUE - fade * 2);
    }
    if (frame == 0) {
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
      {
        struct Particle *bubble = BUBBLES;

        for (i = 0; i != BUBBLE_COLUMN_BUBBLES; i++, bubble++) {
          bubble->x = INTtoFIXED(Random16() & 127);
          bubble->y = INTtoFIXED((Random16() & 255) + 120);
          bubble->velocityX = ((Random16() & 31) - 16) << 13;
          bubble->velocityY = (8 - (Random16() & 31)) << 12;
          bubble->z = (Random16() & 7) + 5;
          bubble->variant = BUBBLE_FLOATING;
        }
      }
    }
    for (i = 0; i != BUBBLE_COLUMN_BUBBLES; i++) {
      struct Particle *bubble = &BUBBLES[i];

      if (bubble->variant >= 0) {
        DRAW_BUBBLE(bubble);
      }
    }
    for (i = 0; i != args->targetCount; i++) {
      if (frame == i * 8 + 18) {
        if (i == 0) {
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, 0, 16);
          EFFECT_SHOW_DAMAGE(SE_RECOVERY);
          Sound_PlayFar(SE_RECOVERY);
        } else {
          Sound_PlayFar(SE_RECOVERY);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 16);
        }
      }
    }
    washVertices = Heap_AllocEwram(512);
    washModel = Render_AllocateModelList(1);
    washModel->texture = &washTexture;
    washTexture.logWidth = 7;
    washTexture.logHeight = 7;
    washModel->mesh = gBattleEffect_LargeSquareMesh;
    washModel->vertices = washVertices;
    washModel->kind = MODEL_SHADED_MAX;
    washTexture.pixels = state->sheet + COATLICUE_COG;
    // The cogs rise from frame 16, eight frames apart, each growing as it
    // rises and fading after 16 frames.
    i = 0;
    height = frame * 8 - 128;
    scale = (frame - 16) * (FIXED_ONE / 8);
    cogFrame = frame - 16;
    for (; i != COATLICUE_WASH_COGS;
         height -= 64, scale -= FIXED_ONE, i++, cogFrame -= 8) {
      if (cogFrame >= 0) {
        s32 washShade = 0;

        if (cogFrame > 15) {
          washShade = (16 - cogFrame) << 3;
        }
        washModel->shade = washShade;
        if (washShade > MODEL_SHADE_INVISIBLE) {
          Matrix_Initialize();
          Matrix_Translate(0, INTtoFIXED(112 - height), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_ScaleUniform(scale);
          Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
          Matrix_RotateX(-TRIG_TURN / 16);
          Matrix_RotateY(frame * (TRIG_TURN / 64));
          Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                   washVertices, 4);
          Render_DrawModelList(washModel);
        }
      }
    }
    Heap_Release(washModel);
    Heap_Release(washVertices);
    EFFECT_END_FRAME(state);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}
#undef VENGEANCE_FRAMES
#undef VENGEANCE_ARROWS
#undef VENGEANCE_FIRST_SHOT
#undef VENGEANCE_SHOT_INTERVAL
#undef VENGEANCE_NOCK_FRAMES
#undef VENGEANCE_STUCK_FRAMES
#undef VENGEANCE_ARCHER_WIDTH
#undef VENGEANCE_ARCHER_HEIGHT
#undef VENGEANCE_ARCHER_LOOSED
#undef VENGEANCE_FLAME_TIP
#undef VENGEANCE_FLAME_TIP_WIDTH
#undef VENGEANCE_FLAME_TIP_HEIGHT
#undef VENGEANCE_ARROW
#undef VENGEANCE_ARROW_WIDTH
#undef VENGEANCE_ARROW_HEIGHT
#undef VENGEANCE_SHOULDER
#undef VENGEANCE_SHOULDER_WIDTH
#undef VENGEANCE_SHOULDER_HEIGHT
#undef VENGEANCE_ARCHER_X
#undef VENGEANCE_SHOULDER_X
#undef VENGEANCE_SHOULDER_Y
#undef VENGEANCE_TOP
#undef VENGEANCE_ARROW_Y
#undef VENGEANCE_STRIP_Y
#undef VENGEANCE_CAMERA_TURN
#undef VENGEANCE_WINDOW_LEFT
#undef LIFE_SHEAR_FRAMES
#undef LIFE_SHEAR_SNIP_FRAME
#undef LIFE_SHEAR_WOMAN_WIDTH
#undef LIFE_SHEAR_WOMAN_HEIGHT
#undef LIFE_SHEAR_STRIKING
#undef LIFE_SHEAR_STRIKING_WIDTH
#undef LIFE_SHEAR_STRIKING_HEIGHT
#undef LIFE_SHEAR_PALE_BLADE
#undef LIFE_SHEAR_PALE_BLADE_WIDTH
#undef LIFE_SHEAR_PALE_BLADE_HEIGHT
#undef LIFE_SHEAR_ORANGE_BLADE
#undef LIFE_SHEAR_ORANGE_BLADE_WIDTH
#undef LIFE_SHEAR_ORANGE_BLADE_HEIGHT
#undef LIFE_SHEAR_FLAME
#undef LIFE_SHEAR_STRIKE_X
#undef LIFE_SHEAR_SHEARS_SCALE
#undef LIFE_SHEAR_FLASH_SCALE
#undef LIFE_SHEAR_FLASH_ANGLE
#undef GEYSER_FRAMES
#undef GEYSER_DRAW_FRAMES
#undef GEYSER_PEAK_FRAME
#undef GEYSER_DROP_COUNT
#undef GEYSER_BURST
#undef GEYSER_STREAM
#undef DJINN_BLAST_BURST
#undef DJINN_BLAST_STREAM
#undef GEYSER_DROP_SHRUNK_LIFE
#undef GEYSER_TILT
#undef STARS_PER_TARGET
#undef STAR_TOTAL
#undef DJINN_STUN_STARS
#undef MYSTIC_CALL_STARS
#undef STAR_LARGEST
#undef STAR_NEAR_DEPTH
#undef STAR_DEPTH_STEP
#undef STAR_FAR_DEPTH
#undef STAR_SPEED_MIN
#undef STAR_SPEED_RANDOM
#undef BUBBLE_COLUMN_BUBBLES
#undef BUBBLE_FRAMES
#undef BUBBLES
#undef BUBBLE_FLOATING
#undef GRADIENT_BLUE_LIFT
#undef GRADIENT_GREEN_LIFT
#undef FILL_GRADIENT
#undef BLIT_BUBBLE
#undef DRAW_BUBBLE
#undef COATLICUE_COG
#undef COATLICUE_SCALES
#undef COATLICUE_BUBBLE_PICTURES
#undef BUBBLE_SHEET_BYTES
#undef BUBBLE_PICTURE_COUNT
#undef COATLICUE_DIM_BUBBLE_PICTURES
#undef COATLICUE_COIL_CORNERS
#undef COATLICUE_COIL_MESH
#undef COATLICUE_COIL_SEGMENTS
#undef COATLICUE_SPRITES
#undef COATLICUE_COLUMNS
#undef COATLICUE_SPRITE_SIZE
#undef COATLICUE_BUBBLE_COUNT
#undef COATLICUE_CLOUD_BUBBLES
#undef COATLICUE_STREAM_BUBBLES
#undef COATLICUE_CLOUD_NEAR_DEPTH
#undef COATLICUE_CLOUD_DEPTH_STEP
#undef COATLICUE_BURSTS
#undef COATLICUE_BURST_BUBBLES
#undef COATLICUE_SKIP_FRAMES
#undef COATLICUE_FLASH_LEVEL
#undef COATLICUE_BACKDROP_FLARE
#undef COATLICUE_TILT
#undef COATLICUE_TILT_MIN
#undef COATLICUE_COG_ROLL
#undef COATLICUE_COG_SCALE
#undef COATLICUE_COG_SCALE_MAX
#undef COATLICUE_COG_FRAME
#undef COATLICUE_COG_SWELL_FRAME
#undef COATLICUE_COG_FADE_FRAME
#undef COATLICUE_COG_GROWTH
#undef COATLICUE_COG_SWELL
#undef COATLICUE_GLOW_GROWTH
#undef COATLICUE_CLOUD_WIDTH_SCALE
#undef COATLICUE_CLOUD_HEIGHT_SCALE
#undef COATLICUE_CLOUD_DEPTH_SCALE
#undef COATLICUE_FLING_FRAME
#undef COATLICUE_FLING_STRETCH
#undef COATLICUE_COIL_SHRINK_FRAME
#undef COATLICUE_COIL_SHRINK_SCALE
#undef COATLICUE_SECOND_COIL_EXTRA
#undef COATLICUE_RINGS
#undef COATLICUE_FLASHES
#undef COATLICUE_GLOWS
#undef COATLICUE_FLASH_SHADE
#undef COATLICUE_FLASH_SHADE_STEP
#undef COATLICUE_COILS
#undef COATLICUE_COIL_TILT
#undef COATLICUE_COIL_TILT_SPEED
#undef COATLICUE_SECOND_COIL_START
#undef COATLICUE_WASH_FRAMES
#undef COATLICUE_WASH_COGS
#undef COATLICUE_WASH_BLUE
#undef COATLICUE_FRAMES
