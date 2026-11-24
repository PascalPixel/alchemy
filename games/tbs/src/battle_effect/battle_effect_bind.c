/// @file
/// Bind and Triangle Strike.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Bind's sprite sheet: the rune, the flare pictures, then the Djinni.
#define BIND_FLARE_PICTURES 4096
#define BIND_DJINNI_PICTURE 8192
/// The particles each target's triangle keeps, and the flares along each edge.
#define BIND_SLOTS_PER_TARGET 32
#define BIND_EDGE_FLARES 24
/// The frames the triangle of flares shrinks and turns before the rune shows,
/// with the damage.
#define BIND_RUNE_FRAME 64
/// The Djinni of Bind's prelude sways this far either way, a sway every 32
/// frames.
#define BIND_DJINNI_SWAY 20
/// The frames Triangle Strike's user takes to leap at the target.
#define TRIANGLE_STRIKE_LEAP_FRAMES 16

/// The blue ramp: one colour for each blue level from BG palette
/// BLUE_RAMP_PALETTE (later from BLUE_RAMP_SECOND_PALETTE), each colour's red
/// and green half its blue. A pixel takes the colour for RGB_LEVEL_MAX less its
/// brightest channel, so the background turns blue with its light and dark
/// swapped.
#define BLUE_RAMP_PALETTE 2
#define BLUE_RAMP_SECOND_PALETTE 6
#define BLUE_RAMP_COLORS (RGB_LEVEL_MAX + 1)
/// The ramp's edge moves down a quarter of its speed in rows a frame, from this
/// speed and a row faster every four frames, for at most BLUE_RAMP_FRAMES or
/// until it is past BLUE_RAMP_LAST_ROW.
#define BLUE_RAMP_START_SPEED 16
#define BLUE_RAMP_FRAMES 27
#define BLUE_RAMP_LAST_ROW 248

/// Turns the screen into shades of blue, top down, with a ragged edge; nothing
/// calls it.
UNUSED void BattleEffect_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *pixel;
  u16 *palette;
  palette = (u16 *)&BG_PLTT_COLOR(BLUE_RAMP_PALETTE, 0);
  for (i = 0; i != BLUE_RAMP_COLORS; i++) {
    s32 half;
    half = i / 2;
    *palette++ = RGB(half, half, i);
  }
  delays = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  end = 0;
  speed = BLUE_RAMP_START_SPEED;
  Iwram_CopyWords(BLUE_RAMP_PIXELS, BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES);
  for (x = 0; x != BATTLE_BACKGROUND_WIDTH; x++) {
    delays[x] = Random16() & 63;
  }
  row = 0;
  for (i = 0; i != BLUE_RAMP_FRAMES; i++) {
    end += speed / 4;
    speed++;
    for (; row != end; row++) {
      for (x = 0; x != BATTLE_BACKGROUND_WIDTH; x++) {
        y = row - delays[x];
        if (y >= 0) {
          if (y < BATTLE_BACKGROUND_HEIGHT) {
            pixel = BLUE_RAMP_PIXELS + BACKGROUND_PIXEL_OFFSET(x, y);
            color = ((s16 *)BG_PLTT)[*pixel];
            red = RGB_RED(color);
            green = RGB_GREEN(color);
            blue = RGB_BLUE(color);
            if (red < green)
              red = green;
            if (red < blue)
              red = blue;
            *pixel = BLUE_RAMP_PALETTE * PALETTE_COLORS + RGB_LEVEL_MAX - red;
          }
        }
      }
    }
    Iwram_CopyWords(BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS,
                    BATTLE_BACKGROUND_BYTES);
    wait(1);
    if (end > BLUE_RAMP_LAST_ROW)
      break;
  }
  // Then onto a second blue ramp.
  palette = (u16 *)&BG_PLTT_COLOR(BLUE_RAMP_SECOND_PALETTE, 0);
  for (i = 0; i != BLUE_RAMP_COLORS; i++) {
    s32 half;
    half = i / 2;
    *palette++ = RGB(half, half, i);
  }
  for (i = 0; i != BATTLE_BACKGROUND_BYTES; i++) {
    BLUE_RAMP_PIXELS[i] +=
        (BLUE_RAMP_SECOND_PALETTE - BLUE_RAMP_PALETTE) * PALETTE_COLORS;
  }
  Iwram_CopyWords(BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS, BATTLE_BACKGROUND_BYTES);
  wait(1);
}

/// A triangle of light spins and shrinks onto the first target, changing colour
/// and leaving a fading trail, then a rune stands on the target. The triangle's
/// corners are projected onto the canvas and each edge is stamped with flares.
///
/// Abilities: Bind; Luff.
void BattleEffect_RunBind(struct BattleEffectParams *args) {
  struct ScreenPoint anchor;
  // The target's position, then each corner projected around it.
  struct ScreenPoint corner;
  struct ScreenPoint screen;
  struct Vector3 scale;
  s32 originX;
  s32 originY;
  BlitterPair draw;
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  u8 *palette;
  struct Actor *actor;
  s32 memberFrame;
  struct Particle *point;
  s32 size;
  s32 j;
  s32 k;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  graphics = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  state->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (state->args->prelude == 1)
    BattleEffect_RunDjinniEntrance(args, ELEMENT_JUPITER, state->args->side,
                                   DJINNI_ANCHOR_OVER_TARGET, &originX,
                                   &originY);
  REG_BG2PA = Q_8_8(1);

  palette = Resource_GetTableEntry(RESOURCE_RUNE_SHEET);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, state->sheet);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_YELLOW_GLOW_SHEET),
                      graphics);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_BLUE_GLOW_SHEET),
                      state->sheet + BIND_FLARE_PICTURES);
  palette = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, state->sheet + BIND_DJINNI_PICTURE);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
  Projection_ScaledUnit(state->args->targets[0], &anchor);
  shift = CANVAS_SIZE / 2 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != state->args->targetCount * 20 + 72; frame++) {
    if (frame == BIND_RUNE_FRAME)
      EFFECT_SHOW_DAMAGE(0);
    // The colours cycle through the hues.
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120),
                                    DEGtoANG(120), 0);

    // The Djinni sways, then rises away.
    if (state->args->prelude == 1) {
      s32 x;
      s32 y;

      x = FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 32)) * BIND_DJINNI_SWAY) +
          originX + shift - DJINNI_WIDTH / 2;
      y = FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 4) + originY - 24;
      BattleEffect_BuildCanvasBlitters(state->args->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, state->sheet + BIND_DJINNI_PICTURE, x, y, DJINNI_WIDTH,
              DJINNI_HEIGHT);
      if (frame <= 3)
        draw[1](canvas, state->sheet + BIND_DJINNI_PICTURE, x, y, DJINNI_WIDTH,
                DJINNI_HEIGHT);
      EFFECT_RELEASE_BLITTERS();
    }
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
    if (frame > 16 && (frame & 15) == 0)
      state->transferValue += CANVAS_FILL_WORD(1);

    // Only the first target's triangle is drawn, though the frames are counted
    // for every target.
    for (member = 0; member != 1; member++) {
      memberFrame = frame - member * 8;
      actor = BattleObject_GetFar(state->args->targets[member])->actor;
      if (memberFrame >= 0 && memberFrame < 96) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        SET_VECTOR(ScreenPoint_Coordinates(&corner), actor->x.fixed,
                   actor->y.fixed, actor->z.fixed);
        Projection_Point(&corner, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;
        // The triangle.
        if (memberFrame <= 67) {
          for (j = 0; j != 3; j++) {
            point = &state->particles[member * BIND_SLOTS_PER_TARGET + j];
            Matrix_Initialize();
            if (memberFrame < BIND_RUNE_FRAME) {
              scale.x = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale.y = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale.z = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              Matrix_ScaleVector(Vector3_Coordinates(&scale));
              Matrix_RotateZ((BIND_RUNE_FRAME - memberFrame) << 9);
              Matrix_RotateY((BIND_RUNE_FRAME - memberFrame) << 9);
            }
            Matrix_RotateZ(j * (DEGtoANG(120)));
            Projection_Point(gBind_Vertex, &corner);
            point->velocityX = corner.x + screen.x;
            point->velocityY = corner.y + screen.y + 16;
          }
          for (j = 0; j != 3; j++) {
            struct Particle *from =
                &state->particles[j + member * BIND_SLOTS_PER_TARGET];
            struct Particle *to =
                &state->particles[(j + 1) % 3 + member * BIND_SLOTS_PER_TARGET];

            size = 5 - memberFrame / 16;
            for (k = 0; k != BIND_EDGE_FLARES; k++) {
              s32 x = from->velocityX +
                      k * (to->velocityX - from->velocityX) / BIND_EDGE_FLARES;
              s32 y = from->velocityY +
                      k * (to->velocityY - from->velocityY) / BIND_EDGE_FLARES;

              EFFECT_DRAW_FLARE(draw[0], canvas,
                                state->sheet + BIND_FLARE_PICTURES, x, y, size);
            }
          }
        }
        // Then the rune, in two mirrored halves.
        if (memberFrame >= BIND_RUNE_FRAME) {
          draw[0](canvas, state->sheet, screen.x - RUNE_HALF_WIDTH,
                  screen.y - RUNE_HALF_HEIGHT / 2, RUNE_HALF_WIDTH,
                  RUNE_HALF_HEIGHT);
          draw[1](canvas, state->sheet, screen.x,
                  screen.y - RUNE_HALF_HEIGHT / 2, RUNE_HALF_WIDTH,
                  RUNE_HALF_HEIGHT);
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_CANVAS(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

/// Unused: no row of gAbilityBattleEffects in either game names its kind, and
/// the handler table is reached with no other kind (BattleEffect_RunFromTable),
/// so the player never sees it. Its frame loop is BattleEffect_RunBind's,
/// without the Djinni prelude, and on frame 46 it starts the user's approach to
/// the first target.
void BattleEffect_RunUnusedTriangleStrike(struct BattleEffectParams *args) {
  struct ScreenPoint anchor;
  // The target's position, then each corner projected around it.
  struct ScreenPoint corner;
  struct ScreenPoint screen;
  struct Vector3 scale;
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  u8 *palette;
  struct Actor *actor;
  s32 memberFrame;
  struct Particle *point;
  s32 size;
  s32 j;
  s32 k;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  graphics = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  state->args = args;
  EFFECT_START_NARROW_CANVAS(0);

  palette = Resource_GetTableEntry(RESOURCE_RUNE_SHEET);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, state->sheet);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_YELLOW_GLOW_SHEET),
                      graphics);
  palette = Resource_GetTableEntry(RESOURCE_BLUE_GLOW_SHEET);
  Decompress_TaggedLz(palette, state->sheet + BIND_FLARE_PICTURES);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
  Projection_ScaledUnit(state->args->targets[0], &anchor);
  shift = CANVAS_SIZE / 2 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != state->args->targetCount * 20 + 72; frame++) {
    if (frame == BIND_RUNE_FRAME)
      EFFECT_SHOW_DAMAGE(0);
    // The user sets off towards the target.
    if (frame == 46)
      BattleMotion_ApproachTargetFar(state->args->user, state->args->targets[0],
                                     TRIANGLE_STRIKE_LEAP_FRAMES, 0);
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120),
                                    DEGtoANG(120), 0);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
    if (frame > 16 && (frame & 15) == 0)
      state->transferValue += CANVAS_FILL_WORD(1);

    // Only the first target's triangle is drawn, though the frames are counted
    // for every target.
    for (member = 0; member != 1; member++) {
      memberFrame = frame - member * 8;
      actor = BattleObject_GetFar(state->args->targets[member])->actor;
      if (memberFrame >= 0 && memberFrame < 96) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        SET_VECTOR(ScreenPoint_Coordinates(&corner), actor->x.fixed,
                   actor->y.fixed, actor->z.fixed);
        Projection_Point(&corner, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;
        if (memberFrame <= 67) {
          for (j = 0; j != 3; j++) {
            point = &state->particles[member * BIND_SLOTS_PER_TARGET + j];
            Matrix_Initialize();
            if (memberFrame < BIND_RUNE_FRAME) {
              scale.x = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale.y = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale.z = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              Matrix_ScaleVector(Vector3_Coordinates(&scale));
              Matrix_RotateZ((BIND_RUNE_FRAME - memberFrame) << 9);
              Matrix_RotateY((BIND_RUNE_FRAME - memberFrame) << 9);
            }
            Matrix_RotateZ(j * (DEGtoANG(120)));
            Projection_Point(gBind_TriangleStrikeVertex, &corner);
            point->velocityX = corner.x + screen.x;
            point->velocityY = corner.y + screen.y + 16;
          }
          for (j = 0; j != 3; j++) {
            struct Particle *from =
                &state->particles[j + member * BIND_SLOTS_PER_TARGET];
            struct Particle *to =
                &state->particles[(j + 1) % 3 + member * BIND_SLOTS_PER_TARGET];

            size = 5 - memberFrame / 16;
            for (k = 0; k != BIND_EDGE_FLARES; k++) {
              s32 x = from->velocityX +
                      k * (to->velocityX - from->velocityX) / BIND_EDGE_FLARES;
              s32 y = from->velocityY +
                      k * (to->velocityY - from->velocityY) / BIND_EDGE_FLARES;

              EFFECT_DRAW_FLARE(draw[0], canvas,
                                state->sheet + BIND_FLARE_PICTURES, x, y, size);
            }
          }
        }
        // Then the rune, in two mirrored halves.
        if (memberFrame >= BIND_RUNE_FRAME) {
          draw[0](canvas, state->sheet, screen.x - RUNE_HALF_WIDTH,
                  screen.y - RUNE_HALF_HEIGHT / 2, RUNE_HALF_WIDTH,
                  RUNE_HALF_HEIGHT);
          draw[1](canvas, state->sheet, screen.x,
                  screen.y - RUNE_HALF_HEIGHT / 2, RUNE_HALF_WIDTH,
                  RUNE_HALF_HEIGHT);
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_CANVAS(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

#undef BIND_FLARE_PICTURES
#undef BIND_DJINNI_PICTURE
#undef BIND_SLOTS_PER_TARGET
#undef BIND_EDGE_FLARES
#undef BIND_RUNE_FRAME
#undef BIND_DJINNI_SWAY
#undef TRIANGLE_STRIKE_LEAP_FRAMES
#undef BLUE_RAMP_PALETTE
#undef BLUE_RAMP_SECOND_PALETTE
#undef BLUE_RAMP_COLORS
#undef BLUE_RAMP_START_SPEED
#undef BLUE_RAMP_FRAMES
#undef BLUE_RAMP_LAST_ROW
