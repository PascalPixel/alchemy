/// @file
/// Bind's spinning triangle and rune, the unused Triangle Strike built on it,
/// and an unused wash of blue down the battlefield.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

/// Bind's sprite sheet: the rune, the flare pictures, then Luff's Djinni.
#define BIND_FLARE_PICTURES 4096
#define BIND_DJINNI_PICTURE 8192
/// The corners of Bind's triangle.
#define BIND_CORNERS 3
/// The particles each target's triangle keeps, and the flares along each edge.
#define BIND_SLOTS_PER_TARGET 32
#define BIND_EDGE_FLARES 24
/// The frames the triangle of flares shrinks and turns before the rune shows,
/// with the damage.
#define BIND_RUNE_FRAME 64
/// Luff's Djinni sways this far either way, a sway every 32
/// frames.
#define BIND_DJINNI_SWAY 20
/// The frames Triangle Strike's user takes to leap at the target.
#define TRIANGLE_STRIKE_LEAP_FRAMES 16

/// The blue ramp: one colour for each blue level, in BG palette
/// BLUE_RAMP_PALETTE and then BLUE_RAMP_SECOND_PALETTE, each the next's
/// red and green at half its blue. A pixel takes the ramp's colour for its
/// brightest channel, darkest at the top.
#define BLUE_RAMP_PALETTE 2
#define BLUE_RAMP_SECOND_PALETTE 6
#define BLUE_RAMP_COLORS (RGB_LEVEL_MAX + 1)
/// The ramp's edge moves down a quarter of its speed in rows a frame, from this
/// speed and a row faster every four frames, for at most BLUE_RAMP_FRAMES or
/// until it is past BLUE_RAMP_LAST_ROW.
#define BLUE_RAMP_START_SPEED 16
#define BLUE_RAMP_FRAMES 27
#define BLUE_RAMP_LAST_ROW 248
/// A row of the picture's 32 tiles of 256 colours, as a shift: 2048 bytes.
#define BLUE_RAMP_TILE_ROW_SHIFT 11
/// The copy of the battle background the ramp is drawn in.
#define BLUE_RAMP_PIXELS Ram_MapCellBuffer

/// Turns the screen into shades of blue, top down, with a ragged edge; nothing
/// calls it.
UNUSED void BattleEffect_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *p, *pixel;
  u16 *palette;
  u32 delayMask;

  palette = (u16 *)&BG_PLTT_COLOR(BLUE_RAMP_PALETTE, 0);
  for (i = 0; i != BLUE_RAMP_COLORS; i++) {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
  }
  delays = Ram_HeapSlots->battleSheet;
  end = 0;
  speed = BLUE_RAMP_START_SPEED;
  Iwram_CopyWords(BLUE_RAMP_PIXELS, (void *)BG_CHAR_BLOCK(2),
                  BATTLE_BACKGROUND_BYTES);
  delayMask = 63;
  p = delays;
  do {
    *p++ = Random16() & delayMask;
  } while (p != delays + BATTLE_BACKGROUND_WIDTH);
  row = 0;
  for (i = 0; i != BLUE_RAMP_FRAMES; i++) {
    end += speed / 4;
    speed++;
    for (; row != end; row++) {
      for (x = 0; x != BATTLE_BACKGROUND_WIDTH; x++) {
        y = row - delays[x];
        if (y >= 0) {
          if (y < BATTLE_BACKGROUND_HEIGHT) {
            // The picture's 8x8 tiles of 256 colours, 64 bytes each.
            pixel =
                BLUE_RAMP_PIXELS + ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) +
                                    ((y / 8) << BLUE_RAMP_TILE_ROW_SHIFT));
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
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS,
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
  {
    u8 *pixels = BLUE_RAMP_PIXELS;

    for (i = 0; i != BATTLE_BACKGROUND_BYTES; i++)
      *pixels++ +=
          (BLUE_RAMP_SECOND_PALETTE - BLUE_RAMP_PALETTE) * PALETTE_COLORS;
  }
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS,
                  BATTLE_BACKGROUND_BYTES);
  wait(1);
}

/// A triangle of light spins and shrinks onto the first target, changing colour
/// and leaving a fading trail, then a rune stands on the target. Used as the
/// Djinni Luff, the Jupiter Djinni appears over the target first.
///
/// Abilities: Bind; Luff.
void BattleEffect_RunBind(struct BattleEffectParams *args) {
  s32 origin[3];
  s32 originWorld[3];
  struct ScreenPoint anchor;
  s32 vector[3];
  struct ScreenPoint screen;
  s32 scale[3];
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *object;
  s32 memberFrame;
  struct Particle *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraState;
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, the
  // Jupiter Djinni, and a transfer that darkens the canvas a little each frame.
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (args->prelude == 1)
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER, args->side, DJINNI_ANCHOR_OVER_TARGET),
        origin, originWorld);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress(RESOURCE_RUNE_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D,
                             state->sheet + BIND_FLARE_PICTURES, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_JUPITER_DJINN_SHEET,
                             state->sheet + BIND_DJINNI_PICTURE, 1, 0);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

  // The canvas is scrolled so that the target stands at its middle.
  EffectPosition_ProjectScaledBattler(args->targets[0], &anchor);
  shift = CANVAS_SIZE / 2 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != args->targetCount * 20 + 72; frame++) {
    if (frame == BIND_RUNE_FRAME)
      EFFECT_SHOW_DAMAGE(0);
    // The colours cycle through the hues.
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120),
                                    DEGtoANG(120), 0);

    // Luff's Djinni sways where it appeared, then rises away.
    if (args->prelude == 1) {
      s32 x;
      s32 y;

      x = FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 32)) * BIND_DJINNI_SWAY) +
          origin[0] + shift - DJINNI_WIDTH / 2;
      y = FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 4) + origin[1] - 24;
      BattleEffect_FetchRectangleBlitters(args->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, state->sheet + BIND_DJINNI_PICTURE, x, y, DJINNI_WIDTH,
              DJINNI_HEIGHT);
      if (frame <= 3)
        draw[1](canvas, state->sheet + BIND_DJINNI_PICTURE, x, y, DJINNI_WIDTH,
                DJINNI_HEIGHT);
      EFFECT_RELEASE_BLITTERS();
    }

    // Two blitters that keep the brighter pixel, the second mirrored.
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    draw[1] = (DrawRectangle)heap->ramCodePaired;
    if (frame > 16 && (frame & 15) == 0)
      state->transferValue += CANVAS_FILL_WORD(1);

    // Only the first target gets a triangle.
    for (member = 0; member != 1; member++) {
      object = BattleObject_GetFar(args->targets[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Matrix_Initialize();
        Matrix_SetLookAt(camera->eye, camera->focus);
        vector[0] = object->x;
        vector[1] = object->y;
        vector[2] = object->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink onto the target until the rune
        // shows, then hold; flares are stamped along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != BIND_CORNERS; j++) {
            point = &state->particles[member * BIND_SLOTS_PER_TARGET + j];
            Matrix_Initialize();
            if (memberFrame < BIND_RUNE_FRAME) {
              scale[0] = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale[1] = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale[2] = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              Matrix_ScaleVector(scale);
              Matrix_RotateZ((BIND_RUNE_FRAME - memberFrame) << 9);
              Matrix_RotateY((BIND_RUNE_FRAME - memberFrame) << 9);
            }
            Matrix_RotateZ(j * (DEGtoANG(120)));
            EffectPosition_ProjectPoint(gBind_Vertex,
                                        (struct ScreenPoint *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != BIND_CORNERS; j++) {
            struct Particle *from =
                &state->particles[j + member * BIND_SLOTS_PER_TARGET];
            struct Particle *to =
                &state->particles[(j + 1) % BIND_CORNERS +
                                  member * BIND_SLOTS_PER_TARGET];

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

        // Then the rune, its right half mirrored from the left.
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

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Unused: no row of gAbilityBattleEffects in either game names its kind, and
/// the handler table is reached with no other kind (BattleEffect_RunFromTable),
/// so the player never sees it. Its frame loop is BattleEffect_RunBind's,
/// without the Djinni prelude, and on frame 46 it starts the user's approach to
/// the first target.
void BattleEffect_RunUnusedTriangleStrike(struct BattleEffectParams *args) {
  struct ScreenPoint anchor;
  s32 vector[3];
  struct ScreenPoint screen;
  s32 scale[3];
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *object;
  s32 memberFrame;
  struct Particle *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraState;
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, and a
  // transfer that darkens the canvas a little each frame.
  EFFECT_START_NARROW_CANVAS(0);
  Resource_LoadAndDecompress(RESOURCE_RUNE_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D,
                             state->sheet + BIND_FLARE_PICTURES, 0, 0);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

  // The canvas is scrolled so that the target stands at its middle.
  EffectPosition_ProjectScaledBattler(args->targets[0], &anchor);
  shift = CANVAS_SIZE / 2 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != args->targetCount * 20 + 72; frame++) {
    if (frame == BIND_RUNE_FRAME)
      EFFECT_SHOW_DAMAGE(0);
    if (frame == 46)
      BattleMotion_ApproachTargetFar(args->user, args->targets[0],
                                     TRIANGLE_STRIKE_LEAP_FRAMES, 0);
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120),
                                    DEGtoANG(120), 0);

    // Two blitters that keep the brighter pixel, the second mirrored.
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    draw[1] = (DrawRectangle)heap->ramCodePaired;
    if (frame > 16 && (frame & 15) == 0)
      state->transferValue += CANVAS_FILL_WORD(1);

    // Only the first target gets a triangle.
    for (member = 0; member != 1; member++) {
      object = BattleObject_GetFar(args->targets[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Matrix_Initialize();
        Matrix_SetLookAt(camera->eye, camera->focus);
        vector[0] = object->x;
        vector[1] = object->y;
        vector[2] = object->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink onto the target until the rune
        // shows, then hold; flares are stamped along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != BIND_CORNERS; j++) {
            point = &state->particles[member * BIND_SLOTS_PER_TARGET + j];
            Matrix_Initialize();
            if (memberFrame < BIND_RUNE_FRAME) {
              scale[0] = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale[1] = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              scale[2] = FIXED_ONE * 21 / 8 - (memberFrame * 3 << 9);
              Matrix_ScaleVector(scale);
              Matrix_RotateZ((BIND_RUNE_FRAME - memberFrame) << 9);
              Matrix_RotateY((BIND_RUNE_FRAME - memberFrame) << 9);
            }
            Matrix_RotateZ(j * (DEGtoANG(120)));
            EffectPosition_ProjectPoint(gBind_TriangleStrikeVertex,
                                        (struct ScreenPoint *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != BIND_CORNERS; j++) {
            struct Particle *from =
                &state->particles[j + member * BIND_SLOTS_PER_TARGET];
            struct Particle *to =
                &state->particles[(j + 1) % BIND_CORNERS +
                                  member * BIND_SLOTS_PER_TARGET];

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

        // Then the rune, its right half mirrored from the left.
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

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef BIND_FLARE_PICTURES
#undef BIND_DJINNI_PICTURE
#undef BIND_CORNERS
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
#undef BLUE_RAMP_TILE_ROW_SHIFT
#undef BLUE_RAMP_PIXELS
