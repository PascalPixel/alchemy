/// @file
/// Meteor Blow.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// One star of the sky the meteor flies through: a point on a sphere and the
/// picture it is drawn with, or METEOR_BLOW_PIXEL_STAR.
struct MeteorStar {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
};

extern const struct Scale gMeteorBlow_DoubleScale;

/// The sky, drawn over the battle background's image in the map cell buffer:
/// METEOR_BLOW_STAR_COUNT stars on a sphere METEOR_BLOW_SKY_RADIUS across,
/// centred at (METEOR_BLOW_SKY_CENTER_X, METEOR_BLOW_SKY_CENTER_Y). Every
/// METEOR_BLOW_STARS_PER_ORB-th is a glowing orb, one of GlowOrbSetSheet's
/// METEOR_BLOW_ORB_PICTURE_COUNT pictures in turn; the rest are single pixels
/// of the first orb's middle row, METEOR_BLOW_BIG_ORB_SIZE across. The stars
/// are kept in the effect's pictures after the orbs'.
#define METEOR_BLOW_STAR_COUNT 256
#define METEOR_BLOW_SKY_RADIUS 160
#define METEOR_BLOW_SKY_CENTER_X 120
#define METEOR_BLOW_SKY_CENTER_Y 64
#define METEOR_BLOW_STARS_PER_ORB 32
#define METEOR_BLOW_ORB_PICTURE_COUNT 7
#define METEOR_BLOW_PIXEL_STAR METEOR_BLOW_ORB_PICTURE_COUNT
#define METEOR_BLOW_BIG_ORB_SIZE 30
#define METEOR_BLOW_STARS                                                      \
  (METEOR_BLOW_BIG_ORB_SIZE * METEOR_BLOW_BIG_ORB_SIZE + 3 * 16 * 16 +         \
   10 * 16 + 2 * 8 * 8)
/// The view starts turned up METEOR_BLOW_START_PITCH and round
/// METEOR_BLOW_START_YAW, and turns a 256th of a turn a frame on each.
#define METEOR_BLOW_START_PITCH 3328
#define METEOR_BLOW_START_YAW 9472
/// From frame 48 the turn round slows by 4 a frame, until it stops at
/// METEOR_BLOW_YAW_STILL_FRAME. The turn up would slow by 8 a frame from frame
/// 160, as if it were to stop at METEOR_BLOW_PITCH_STILL_FRAME, but the sky
/// is drawn only to frame 115.
#define METEOR_BLOW_YAW_STILL_FRAME 112
#define METEOR_BLOW_PITCH_STILL_FRAME 152

/// The sky's colour, the backdrop's a line at a time over the battle view:
/// each line's level is a quarter of its line plus a base, half of
/// state->frame (which rises as the meteor comes) over
/// METEOR_BLOW_SKY_BASE_LEVEL; its blue rises from level
/// METEOR_BLOW_SKY_BLUE_LEVEL and its green, with half as much red, from
/// METEOR_BLOW_SKY_GREEN_LEVEL.
#define METEOR_BLOW_SKY_BASE_LEVEL 16
#define METEOR_BLOW_SKY_BLUE_LEVEL 32
#define METEOR_BLOW_SKY_GREEN_LEVEL 80

/// The cone of flame the meteor plunges in: METEOR_BLOW_CONE_ROWS rings of two
/// corners down an arc of a circle METEOR_BLOW_CONE_RADIUS across, six degrees
/// apart, a strip of DustBlobSheet's picture drawn METEOR_BLOW_CONE_STRIPS
/// times round it, each METEOR_BLOW_CONE_STRIP_SCROLL texels further down the
/// picture; its corners, their projections and its mesh are kept in the
/// effect's pictures.
#define METEOR_BLOW_CONE_ROWS 16
#define METEOR_BLOW_CONE_RADIUS 48
#define METEOR_BLOW_CONE_STRIPS 5
#define METEOR_BLOW_CONE_STRIP_SCROLL 20
#define METEOR_BLOW_CONE_PROJECTED 22336
#define METEOR_BLOW_CONE_CORNERS 24384
#define METEOR_BLOW_CONE_MESH 25408

/// Where DustBlobSheet, BillowTexture and FlameBandTexture are decoded in the
/// effect's pictures.
#define METEOR_BLOW_DUST_BLOB_PICTURES 8192
#define METEOR_BLOW_BILLOW 16384
#define METEOR_BLOW_FLAME_BAND 20480

/// The meteor's step on one axis each frame of its flight: its speed times the
/// heading's sine or cosine, times 64.
#define METEOR_BLOW_FLIGHT_STEP(trig, speed) (((trig) * (speed)) >> 10)
/// The meteor's heading starts at DEGtoANG(135); from frame 24 it swings
/// towards 0 by METEOR_BLOW_HEADING_TURN a frame, as if it had swung since
/// METEOR_BLOW_HEADING_TURN_FRAME, and stays at 0 from frame 102.
#define METEOR_BLOW_HEADING_TURN 286
#define METEOR_BLOW_HEADING_TURN_FRAME 16

/// The landing: the meteor comes down on the foes from METEOR_BLOW_LANDING_X,
/// and METEOR_BLOW_SPARKS burst from it at METEOR_BLOW_BURST_FRAME. The battle
/// background is tinted dark orange for it, its red raised by
/// METEOR_BLOW_LANDING_TINT_RED levels and its green and blue lowered.
#define METEOR_BLOW_FRAMES 50
#define METEOR_BLOW_LANDING_X 144
#define METEOR_BLOW_BURST_FRAME 16
#define METEOR_BLOW_SPARKS 256
#define METEOR_BLOW_LANDING_TINT_RED 5
#define METEOR_BLOW_LANDING_TINT_GREEN (-4)
#define METEOR_BLOW_LANDING_TINT_BLUE (-12)
/// The billows lean back METEOR_BLOW_BILLOW_TILT and the rings of fire
/// METEOR_BLOW_RING_TILT.
#define METEOR_BLOW_BILLOW_TILT 1000
#define METEOR_BLOW_RING_TILT 2000

/// Meteor Blow: the screen goes black and the view flies through a sky of
/// stars as a burning meteor comes in, growing, and plunges in a cone of
/// flame; then it lands on the foes, rings of fire and billows spread from it
/// and sparks fly up as every foe is thrown back.
///
/// Abilities: Meteor Blow.
void BattleEffect_RunMeteorBlow(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectState *state;
  u8 *map;
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle blit[2];
  u8 *sprites;
  u8 *coneProjected;
  u8 *coneCorners;
  u8 *coneMesh;
  struct BattleBackgroundState *background;
  struct MeteorStar *stars;
  u16 pitch;
  u16 yaw;
  FIXED x;
  FIXED z;
  s32 size;
  FIXED scale;
  s32 meteorX;
  s32 meteorZ;
  void *verts;
  s32 billowX;
  s32 canvasX;
  s32 blank;
  s32 offset;
  s32 frame;
  s32 arcAngle;
  s32 j;
  s32 i;
  s32 position[3];
  s32 screen[3];
  s32 point[3];
  FIXED flightPlace[4];
  FIXED meteorPlace[4];
  struct ModelTexture landingTexture;
  struct Scale meteorScale;
  s32 meteorSize[2];
  struct ModelTexture texture;

  canvas = Ram_HeapSlots->battleCanvas;
  map = gMapGridCellBuffer;
  state = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  state->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  state->transferMode = CANVAS_COPY;
  blank = 0;
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  Task_Remove(BattleEffect_TransferCanvas);
  Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_SET_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_DUST_BLOB_SHEET,
                             state->sheet + METEOR_BLOW_DUST_BLOB_PICTURES, 1,
                             1);
  Iwram_CopyWordsEntry(sprites,
                       state->sheet + METEOR_BLOW_BIG_ORB_SIZE / 2 *
                                          METEOR_BLOW_BIG_ORB_SIZE,
                       METEOR_BLOW_BIG_ORB_SIZE);
  LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
  Graphics_LoadGreyRamp();
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  IWRAM_FILL_WORDS(map, 256 * 128, CANVAS_FILL_WORD(1));
  Iwram_CopyWordsEntry((void *)BG_CHAR_BLOCK(2), map, BATTLE_BACKGROUND_BYTES);
  BattleEffect_ShowLivingTargets(state->args);
  BattleEffect_CreateSprites(1, SPRITE_METEOR, 1);
  REG_BLDCNT = blank;
  REG_BG2PA = Q_8_8(1);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
  Iwram_CopyWordsEntry((void *)BG_CHAR_BLOCK(1), canvas, CANVAS_BYTES);
  Task_Add(BattleEffect_TransferFullScreenImage, TASK_PASS_VBLANK);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  coneProjected = state->sheet + METEOR_BLOW_CONE_PROJECTED;
  coneCorners = state->sheet + METEOR_BLOW_CONE_CORNERS;
  coneMesh = state->sheet + METEOR_BLOW_CONE_MESH;
  background = Ram_HeapSlots->battleBackground;
  stars = (struct MeteorStar *)(state->sheet + METEOR_BLOW_STARS);
  // Default: the flight sets these at frame 0.
  pitch = 0;
  yaw = 0;
  x = 0;
  z = 0;
  size = 0;
  scale = FIXED_ONE;
  Mesh_BuildGrid(coneMesh, 2, METEOR_BLOW_CONE_ROWS, 32, 2);
  for (offset = 0, frame = 0, arcAngle = 0; frame != METEOR_BLOW_CONE_ROWS;
       arcAngle += DEGtoANG(6), offset += 2 * sizeof(struct ModelVertex),
      frame++) {
    s32 y = FIXEDtoINT(-Trig_Cos(arcAngle) * METEOR_BLOW_CONE_RADIUS);
    s32 r = FIXEDtoINT(Trig_Sin(arcAngle) * METEOR_BLOW_CONE_RADIUS);
    struct ModelVertex *v;

    j = 0;
    v = (struct ModelVertex *)(coneCorners + offset);
    for (; j != 2; j++, v++) {
      v->x = FIXEDtoINT(Trig_Sin(j * DEGtoANG(45)) * r);
      v->y = y;
      v->z = FIXEDtoINT(Trig_Cos(j * DEGtoANG(45)) * r);
    }
  }
  // The sky: the blitter and the transfer, then the stars on their sphere.
  {
    s32 transferMode = CANVAS_COPY_AND_DARKEN;
    s32 star;
    s32 *starKind;
    struct MeteorStar *current;

    BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 8, 7, EFFECT_CLIP_BOUNDS,
                              BLITTER_MAX);
    blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    // DEAD: both fields are set again before anything reads them; the ROM
    // keeps the two stores.
    EFFECT_SET_TRANSFER(state, transferMode, CANVAS_FILL_WORD(4));
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    Task_Add(BattleEffect_ArmMeteorBlowPaletteDma, TASK_PASS_VBLANK + 126);
    gBgScroll[1].x = 0;
    background->busy = 1;
    starKind = &stars->kind;
    current = stars;
    for (star = 0; star != METEOR_BLOW_STAR_COUNT; star++,
        starKind += sizeof(struct MeteorStar) / sizeof(s32), current++) {
      s32 a = (star & (METEOR_BLOW_STAR_COUNT - 1)) * (TRIG_TURN / 2) /
              METEOR_BLOW_STAR_COUNT;
      s32 r = METEOR_BLOW_SKY_RADIUS;
      s32 b;

      current->y = Trig_Sin(a) * r;
      r = FIXEDtoINT(Trig_Cos(a) * r);
      b = Random16() & (DEGtoANG(180) - 1);
      current->x = Trig_Sin(b) * r;
      current->z = Trig_Cos(b) * r;
      if ((star & (METEOR_BLOW_STARS_PER_ORB - 1)) == 0) {
        *starKind =
            (star / METEOR_BLOW_STARS_PER_ORB) % METEOR_BLOW_ORB_PICTURE_COUNT;
      } else {
        *starKind = METEOR_BLOW_PIXEL_STAR;
      }
    }
  }

  // The flight through the stars.
  for (frame = 0;
       frame != 160 && !(gInput.repeat & KEY_A) && !(gInput.repeat & KEY_B);
       frame++) {
    s32 fade;
    s32 speed;

    if (frame == 0) {
      x = toFIXED(272);
      z = toFIXED(88);
      size = FIXED_ONE / 64;
      pitch = METEOR_BLOW_START_PITCH;
      yaw = METEOR_BLOW_START_YAW;
      state->frame = 8;
      Sound_PlayFar(SE_WAVE);
    }
    if (frame == 120) {
      Sound_PlayFar(SE_WAVE_HIGH);
    }
    fade = frame - 96;
    if ((u32)fade <= 16) {
      REG_BLDALPHA = BLEND_ALPHA(16 - fade, BLEND_WEIGHT_MAX);
    }
    if (frame == 115) {
      REG_BLDALPHA = EFFECT_BLEND_ADD;
    }
    if (frame == 116) {
      Task_Remove(BattleEffect_TransferFullScreenImage);
      Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2CNT,
                          BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                              BGCNT_SCREEN_BASE(7),
                          IO_WRITE_HALFWORD);
    }
    if (frame > 128) {
      BattleEffect_StepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
    }
    if (frame == 117) {
      state->frame = 45;
      EFFECT_FREE_PARTICLES(i, state->particles, ARRAY_COUNT(state->particles));
      scale = FIXED_ONE;
    }
    // DEAD: frame never reaches 1600 here; the ROM keeps the compare and
    // branch.
    if (frame <= 1599) {
      u16 *dst = state->backdropLines;
      s32 value = state->frame;
      s32 base = value / 2 + METEOR_BLOW_SKY_BASE_LEVEL;

      if (frame > 79) {
        state->frame++;
        if (state->frame > 236) {
          state->frame = 236;
        }
      }
      for (i = 0; i != BATTLE_VIEW_TOP - 1; i++) {
        *dst++ = 0;
      }
      for (; i != BATTLE_VIEW_BOTTOM - 1; i++) {
        s32 g = i - BATTLE_VIEW_TOP;
        s32 b = g / 4;

        value = b + base;
        b = value - METEOR_BLOW_SKY_BLUE_LEVEL;
        g = value - METEOR_BLOW_SKY_GREEN_LEVEL;

        if (b < 0)
          b = 0;
        if (b > RGB_LEVEL_MAX)
          b = RGB_LEVEL_MAX;
        if (g < 0)
          g = 0;
        if (g > RGB_LEVEL_MAX)
          g = RGB_LEVEL_MAX;
        *dst++ = RGB(g >> 1, g, b);
      }
      for (; i != DISPLAY_HEIGHT; i++) {
        *dst++ = 0;
      }
    }

    if (frame > 115) {
      struct RenderModel *model;
      s32 shade;
      s32 scroll;
      s32 angle;

      model = Render_AllocateModelList(1);
      shade = frame * 4 - 528;
      if (shade > 0) {
        shade = 0;
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      texture.pixels = state->sheet + METEOR_BLOW_DUST_BLOB_PICTURES;
      model->kind = MODEL_SHADED_MAX;
      model->texture = &texture;
      model->mesh = coneMesh;
      model->vertices = coneProjected;
      model->shade = shade;
      model->scrollU = 0;
      if (frame > 127) {
        s32 d = (frame << 6) - FIXED_ONE / 8;

        if (d > FIXED_ONE / 64 - 1) {
          d = FIXED_ONE / 64;
        }
        scale += d;
        Matrix_Scale(scale, scale, scale);
      }
      i = 0;
      angle = -DEGtoANG(90);
      scroll = frame * 2;
      for (; i != METEOR_BLOW_CONE_STRIPS; i++) {
        model->scrollV = ~scroll & TEXTURE_SCROLL_MASK;
        Matrix_Initialize();
        Matrix_Translate(-toFIXED(2), -toFIXED(64), 0);
        Matrix_Scale(FIXED_ONE * 2625 / 4096, FIXED_ONE * 9317 / 4096,
                     FIXED_ONE);
        Matrix_Scale(scale, scale, scale);
        Matrix_RotateZ(DEGtoANG(180));
        Matrix_RotateY(angle);
        Render_TransformVertices(coneCorners, coneProjected,
                                 2 * METEOR_BLOW_CONE_ROWS);
        Render_DrawModelList(model);
        angle += TRIG_TURN * 3 / 32;
        scroll += METEOR_BLOW_CONE_STRIP_SCROLL;
      }
      Heap_Release(model);
    } else {
      // DEAD: frame is at most 115 here, so the second arm never runs; the ROM
      // keeps both compares and the arm.
      if (frame <= 159) {
        pitch += TRIG_TURN / 256;
      } else if (frame <= 191) {
        pitch = pitch - frame * 8 + METEOR_BLOW_PITCH_STILL_FRAME * 8;
      }
      if (frame <= 47) {
        yaw += TRIG_TURN / 256;
      } else if (frame <= 111) {
        yaw = yaw - frame * 4 + METEOR_BLOW_YAW_STILL_FRAME * 4;
      }
      position[0] = METEOR_BLOW_SKY_CENTER_X;
      position[1] = METEOR_BLOW_SKY_CENTER_Y;
      position[2] = 0;
      Matrix_Initialize();
      Matrix_TranslateVector(position);
      Matrix_RotateX(pitch);
      Matrix_RotateY(yaw);
      if (frame <= 112) {
        struct MeteorStar *s = stars;

        for (i = 0; i != METEOR_BLOW_STAR_COUNT; i++, s++) {
          s32 kind = s->kind;

          point[0] = FIXED_INTEGER_PART(s->x);
          point[1] = FIXED_INTEGER_PART(s->y);
          point[2] = FIXED_INTEGER_PART(s->z);
          Matrix_ProjectPoint(point, screen);
          if (screen[2] > 0) {
            if (kind == METEOR_BLOW_PIXEL_STAR) {
              blit[0](gMapGridCellBuffer,
                      sprites + i % METEOR_BLOW_BIG_ORB_SIZE, screen[0],
                      screen[1], 1, 1);
            } else {
              blit[0](gMapGridCellBuffer,
                      state->sheet + gMeteorBlow_Pictures[kind], screen[0],
                      screen[1], gMeteorBlow_PictureWidths[kind],
                      gMeteorBlow_PictureHeights[kind]);
            }
          }
        }
      } else {
        Iwram_ClearWordsEntry(gMapGridCellBuffer, BATTLE_BACKGROUND_BYTES);
      }
    }

    meteorSize[1] = size;
    meteorSize[0] = size;
    flightPlace[3] = 0;
    flightPlace[1] = toFIXED(255);
    flightPlace[0] = x;
    flightPlace[2] = z;
    Render_ApplyProjectedPlacementFar(state->sprites[0], flightPlace,
                                      meteorSize, 0);
    speed = toFIXED(1) / 32;
    {
      s32 a = DEGtoANG(135);

      if (frame > 23) {
        a = a - frame * METEOR_BLOW_HEADING_TURN +
            METEOR_BLOW_HEADING_TURN_FRAME * METEOR_BLOW_HEADING_TURN;
        if (a < 0) {
          a = 0;
        }
      }
      if (frame > 87) {
        speed = speed - (frame << 6) + toFIXED(11) / 128;
        if (speed < 0) {
          speed = 0;
        }
      }
      x -= METEOR_BLOW_FLIGHT_STEP(Trig_Sin(a), speed);
      z += METEOR_BLOW_FLIGHT_STEP(Trig_Cos(a), speed);
      size += FIXED_ONE / 128;
    }
    if (frame > 127) {
      speed = frame * (toFIXED(1) / 8) - toFIXED(16);
      if (speed > toFIXED(2) - 1) {
        speed = toFIXED(2);
      }
      z += speed;
      size += FIXED_ONE / 128;
    } else if (size > FIXED_ONE) {
      size = FIXED_ONE;
    }
    EFFECT_PRESENT_CANVAS(state);
  }

  wait(1);
  background->busy = 0;
  Task_Remove(BattleEffect_ArmMeteorBlowPaletteDma);
  if (frame <= 115) {
    Task_Remove(BattleEffect_TransferFullScreenImage);
  } else {
    Task_Remove(BattleEffect_TransferCanvas);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_SetupBlendedDisplay();
  // DEAD: frame 0 sets meteorX and meteorZ again before any read; the ROM keeps
  // both moves.
  meteorX = 200;
  meteorZ = 0;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG0 |
               WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2;
  REG_WIN0V = WIN_RANGE(16, 124);
  REG_WIN1V = WIN_RANGE(0, 136);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_BILLOW_TEXTURE,
                             state->sheet + METEOR_BLOW_BILLOW, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE,
                             state->sheet + METEOR_BLOW_FLAME_BAND, 0, 0);
  LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
  for (i = 0; i != METEOR_BLOW_SPARKS; i++)
    PARTICLES[i].variant = 0;
  Palette_BrightenBattleBg(METEOR_BLOW_LANDING_TINT_RED,
                           METEOR_BLOW_LANDING_TINT_GREEN,
                           METEOR_BLOW_LANDING_TINT_BLUE);

  // The landing: rings of fire and billows spread, and sparks fly.
  for (frame = 0; frame != METEOR_BLOW_FRAMES; frame++) {
    struct RenderModel *model;
    s32 angle;
    s32 back;
    s32 shade;
    s32 step;

    if (frame == 0) {
      meteorX = METEOR_BLOW_LANDING_X;
      meteorZ = 0;
    }
    if (frame <= 33) {
      meteorScale = gMeteorBlow_DoubleScale;
      meteorPlace[3] = 0;
      meteorPlace[1] = toFIXED(255);
      meteorPlace[0] = INTtoFIXED(meteorX);
      meteorPlace[2] = INTtoFIXED(meteorZ);
      if (frame <= 15) {
        meteorX -= 1;
        meteorZ += 9;
      } else {
        meteorZ += 2;
      }
      Render_ApplyProjectedPlacementFar(state->sprites[0], meteorPlace,
                                        (s32 *)&meteorScale, 0);
    }
    if (frame == 34) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if (frame == METEOR_BLOW_BURST_FRAME) {
      struct Particle *p;
      s32 count = METEOR_BLOW_SPARKS;

      p = (struct Particle *)gMapGridCellBuffer;
      i = 0;
      canvasX = INTtoFIXED(meteorX / 2);
      /* FAKEMATCH(dead): updates of meteorX that cancel out, after it is halved
       * for the sparks' start. */
      meteorX++;
      meteorX--;
      for (; i != count; i++, p++) {
        s32 r;
        s32 a;

        /* FAKEMATCH(dead): updates of count. */
        count++;
        count--;
        r = (Random16() & 511) + count;
        a = (Random16() & (DEGtoANG(180) - 1)) + DEGtoANG(90);

        p->x = canvasX;
        p->y = toFIXED(114);
        p->velocityX = (Trig_Sin(a) * r) >> 6;
        p->velocityY = (Trig_Cos(a) * r * 2) >> 6;
        p->variant = (Random16() & 7) + 24;
      }
      state->shakeFrames = 4;
      Sound_PlayFar(SE_CRASH);
      for (i = 0; i != args->targetCount; i++) {
        BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 8);
      }
    }

    verts = Heap_AllocEwram(1024);
    model = Render_AllocateModelList(1);
    shade = 0;
    model->texture = &landingTexture;
    model->vertices = verts;
    model->shade = shade;
    landingTexture.logWidth = 6;
    landingTexture.logHeight = 6;
    landingTexture.pixels = state->sheet + METEOR_BLOW_BILLOW;
    model->kind = MODEL_SHADED_ADDITIVE;
    model->mesh = gBattleEffect_RingMesh;
    billowX = meteorX / 2 - 60;
    step = frame * (TRIG_TURN / 32);
    i = 0;
    back = -step;
    angle = step;
    for (; i != 4; i++, back += DEGtoANG(45), angle += DEGtoANG(45)) {
      s32 start = gMeteorBlow_WaveStarts[i] + METEOR_BLOW_BURST_FRAME;

      if (frame > start) {
        shade = (start - frame) * 8 + 56;

        if (shade > -16) {
          shade = -16;
        }
        if (shade > MODEL_SHADE_INVISIBLE) {
          s32 s = (frame - start) * gMeteorBlow_WaveGrowth[i] * 2000 +
                  FIXED_ONE * 2673 / 4096;

          model->shade = shade;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(billowX), toFIXED(40), 0);
          Matrix_Scale(s, s * gMeteorBlow_WaveStretch[i], s / 4);
          Matrix_RotateX(METEOR_BLOW_BILLOW_TILT);
          if (i & 1) {
            Matrix_RotateY(angle);
          } else {
            Matrix_RotateY(back);
          }
          Render_TransformVertices(gBattleEffect_RingCorners, verts, 32);
          Render_DrawModelList(model);
        }
      }
    }
    model->mesh = gBattleEffect_CylinderMesh;
    landingTexture.pixels = state->sheet + METEOR_BLOW_FLAME_BAND;
    landingTexture.logWidth = 7;
    landingTexture.logHeight = 7;
    /* FAKEMATCH(dead): meteorX halved by hand, the division's own instructions
     * written out so the halving is not shared with billowX's; as
     * meteorX / 2 - CANVAS_SIZE / 2. */
    canvasX = ((meteorX + (s32)((u32)meteorX >> 31)) >> 1) - CANVAS_SIZE / 2;
    for (i = 0; i != 3; i++) {
      s32 start = gMeteorBlow_RingStarts[i] + 8;

      if (frame > start) {
        shade = (start - frame) * 8 + 40;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > MODEL_SHADE_INVISIBLE) {
          s32 s = (frame - start) * gMeteorBlow_RingGrowth[i] * 1500 +
                  FIXED_ONE * 131 / 512;

          model->shade = shade;
          model->scrollU = ((frame + i * 5) & 7) << 4;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(canvasX),
                           INTtoFIXED(gMeteorBlow_RingHeights[i]), 0);
          Matrix_Scale(s, s * 2, s);
          Matrix_RotateX(METEOR_BLOW_RING_TILT);
          Render_TransformVertices(gBattleEffect_CylinderCorners, verts, 32);
          Render_DrawModelList(model);
        }
      }
    }
    Heap_Release(model);
    Heap_Release(verts);

    {
      struct Particle *p;

      p = (struct Particle *)gMapGridCellBuffer;
      for (i = 0; i != METEOR_BLOW_SPARKS; i++, p++) {
        if (p->variant >= 0) {
          s32 n = (p->variant >> 2) + 1;

          blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                  FIXED_INTEGER_PART(p->x) - n / 2,
                  FIXED_INTEGER_PART(p->y) - n, n, n * 2);
          p->variant--;
          Particle_AdvanceWithGravity2D(p, 56, toFIXED(1) / 8);
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Sprite_DestroyFar(state->sprites[0]);
  BattleEffect_EndCanvasLayer();
}
