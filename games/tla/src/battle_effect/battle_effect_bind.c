/// @file
/// Bind and Triangle Strike.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

#define PIXEL_BUFFER Ram_MapCellBuffer

/// Turns the background canvas into a blue ramp, row by row from the top, each
/// column starting after its own random delay.
void BattleEffect_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *p, *pixel;
  u16 *palette;
  u32 delayMask;

  palette = (u16 *)&BG_PLTT_COLOR(2, 0);
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  delays = Ram_HeapSlots->battleSheet;
  end = 0;
  speed = 16;
  Iwram_CopyWords(PIXEL_BUFFER, (void *)BG_CHAR_BLOCK(2),
                  BATTLE_BACKGROUND_BYTES);
  delayMask = 63;
  p = delays;
  do {
    *p++ = Random16() & delayMask;
  } while (p != delays + 256);
  row = 0;
  i = 0;
  do {
    end += speed / 4;
    speed++;
    while (row != end) {
      x = 0;
      do {
        y = row - delays[x];
        if (y >= 0) {
          if (y <= 119) {
            pixel = PIXEL_BUFFER + ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) +
                                    ((y / 8) << 11));
            color = ((s16 *)BG_PLTT)[*pixel];
            red = RGB_RED(color);
            green = RGB_GREEN(color);
            blue = RGB_BLUE(color);
            if (red < green)
              red = green;
            if (red < blue)
              red = blue;
            *pixel = 63 - red;
          }
        }
        x++;
      } while (x != 256);
      row++;
    }
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), PIXEL_BUFFER, 0x7800);
    wait(1);
    if (end > 248)
      break;
    i++;
  } while (i != 27);
  palette = (u16 *)&BG_PLTT_COLOR(6, 0);
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  {
    u8 *buffer = PIXEL_BUFFER;

    i = 0;
    do {
      *buffer++ += 64;
      i++;
    } while (i != 0x7800);
  }
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), PIXEL_BUFFER,
                  BATTLE_BACKGROUND_BYTES);
  wait(1);
}

#define FLARE_SHEET 0x1000 ///< the flares, after the rune picture
#define DJINN_SHEET 0x2000 ///< the Jupiter Djinn, after the flares
#define DJINN_SIZE 40

#define CORNERS 3
#define EDGE_STEPS 24 ///< flares drawn along each edge

/// The particles each target's triangle keeps, and the flares along each edge.
#define BIND_SLOTS_PER_TARGET 32
/// The frames the triangle of flares shrinks and turns before the rune shows,
/// with the damage.
#define BIND_RUNE_FRAME 64

#define BIND_EDGE_FLARES 24

/// A triangle of light spins and shrinks onto the first target, changing colour
/// and leaving a fading trail, then a rune stands on the target.
///
/// Abilities: Bind; Luff.
void BattleEffect_RunBind(struct BattleEffectParams *args) {
  s32 origin[3];
  s32 originSize[3];
  struct ScreenPoint anchor;
  s32 vector[3];
  struct ScreenPoint screen;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *motion;
  s32 memberFrame;
  struct Particle *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  // heap holds the heap slots from here for the second blitter's read, so the
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, the
  // Djinn with its palette, and a transfer that fades the canvas by 4 each
  // frame.
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (args->prelude == 1)
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER, args->side, DJINNI_ANCHOR_OVER_TARGET),
        origin, originSize);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress(RESOURCE_RUNE_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D,
                             work->sheet + FLARE_SHEET, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_JUPITER_DJINN_SHEET,
                             work->sheet + DJINN_SHEET, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(args->targets[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != args->targetCount * 20 + 72; frame++) {
    if (frame == BIND_RUNE_FRAME)
      EFFECT_SHOW_DAMAGE(0);
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120),
                                    DEGtoANG(120), 0);

    // The prelude: the Djinn circles where the effect was prepared, and
    // rises away after frame 32; its blitters live for this frame only.
    if (args->prelude == 1) {
      s32 x;
      s32 y;

      x = (FIXEDtoINT(Trig_Sin(frame << 11) * 20)) + origin[0] + shift - 20;
      y = (FIXEDtoINT(Trig_Cos(frame << 11) << 2)) + origin[1] - 24;
      BattleEffect_FetchRectangleBlitters(args->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE, DJINN_SIZE);
      if (frame <= 3)
        draw[1](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE,
                DJINN_SIZE);
      EFFECT_RELEASE_BLITTERS();
    }

    // A fresh pair of max blitters each frame, the second flipped.
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    draw[1] = (DrawRectangle)heap->ramCodePaired;
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += CANVAS_FILL_WORD(1);

    for (member = 0; member != 1; member++) {
      motion = BattleObject_GetFar(args->targets[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Matrix_Initialize();
        Matrix_SetLookAt(camera->eye, camera->focus);
        vector[0] = motion->x;
        vector[1] = motion->y;
        vector[2] = motion->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * BIND_SLOTS_PER_TARGET + j];
            Matrix_Initialize();
            if (memberFrame <= 63) {
              scale[0] = 0x2a000 - (memberFrame * 3 << 9);
              scale[1] = 0x2a000 - (memberFrame * 3 << 9);
              scale[2] = 0x2a000 - (memberFrame * 3 << 9);
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
          for (j = 0; j != CORNERS; j++) {
            struct Particle *from =
                &work->particles[j + member * BIND_SLOTS_PER_TARGET];
            struct Particle *to =
                &work->particles[(j + 1) % 3 + member * BIND_SLOTS_PER_TARGET];

            size = 5 - memberFrame / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x = from->velocityX +
                      k * (to->velocityX - from->velocityX) / BIND_EDGE_FLARES;
              s32 y = from->velocityY +
                      k * (to->velocityY - from->velocityY) / BIND_EDGE_FLARES;

              draw[0](canvas,
                      work->sheet + gBattleEffect_FlarePictures[size - 1] +
                          FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - RUNE_HALF_WIDTH,
                  screen.y - RUNE_HALF_HEIGHT / 2, RUNE_HALF_WIDTH,
                  RUNE_HALF_HEIGHT);
          draw[1](canvas, work->sheet, screen.x,
                  screen.y - RUNE_HALF_HEIGHT / 2, RUNE_HALF_WIDTH,
                  RUNE_HALF_HEIGHT);
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_CANVAS(work);
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
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
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

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  // heap holds the heap slots from here for the second blitter's read, so the
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, and a
  // transfer that fades the canvas by 4 each frame.
  EFFECT_START_NARROW_CANVAS(0);
  Resource_LoadAndDecompress(RESOURCE_RUNE_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D,
                             work->sheet + FLARE_SHEET, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(args->targets[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != args->targetCount * 20 + 72; frame++) {
    if (frame == BIND_RUNE_FRAME)
      EFFECT_SHOW_DAMAGE(0);
    if (frame == 46)
      BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16, 0);
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120),
                                    DEGtoANG(120), 0);

    // A fresh pair of max blitters each frame, the second flipped.
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    draw[1] = (DrawRectangle)heap->ramCodePaired;
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += CANVAS_FILL_WORD(1);

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

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * BIND_SLOTS_PER_TARGET + j];
            Matrix_Initialize();
            if (memberFrame <= 63) {
              scale[0] = 0x2a000 - (memberFrame * 3 << 9);
              scale[1] = 0x2a000 - (memberFrame * 3 << 9);
              scale[2] = 0x2a000 - (memberFrame * 3 << 9);
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
          for (j = 0; j != CORNERS; j++) {
            struct Particle *from =
                &work->particles[j + member * BIND_SLOTS_PER_TARGET];
            struct Particle *to =
                &work->particles[(j + 1) % 3 + member * BIND_SLOTS_PER_TARGET];

            size = 5 - memberFrame / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x = from->velocityX +
                      k * (to->velocityX - from->velocityX) / BIND_EDGE_FLARES;
              s32 y = from->velocityY +
                      k * (to->velocityY - from->velocityY) / BIND_EDGE_FLARES;

              draw[0](canvas,
                      work->sheet + gBattleEffect_FlarePictures[size - 1] +
                          FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - RUNE_HALF_WIDTH,
                  screen.y - RUNE_HALF_HEIGHT / 2, RUNE_HALF_WIDTH,
                  RUNE_HALF_HEIGHT);
          draw[1](canvas, work->sheet, screen.x,
                  screen.y - RUNE_HALF_HEIGHT / 2, RUNE_HALF_WIDTH,
                  RUNE_HALF_HEIGHT);
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
