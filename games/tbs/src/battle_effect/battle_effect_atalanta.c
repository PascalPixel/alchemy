/// @file
/// The Atalanta summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define ATALANTA_FALL_FRAMES 88 ///< frames of the shafts' fall

#define ATALANTA_SHAFTS 16      ///< shafts of light that gather over the field
#define ATALANTA_TRAIL_MOTES 24 ///< motes in each gathering shaft's trail
#define ATALANTA_FALLING_SHAFTS 24 ///< shafts that rain down on the targets
/// The particles the falling shafts are set up in, more than fall.
#define ATALANTA_FALLING_SHAFT_SLOTS 32

/// The gathering shafts' timing: the first one's trail gathers from frame
/// ATALANTA_SHAFT_FIRST_FRAME and each next one ATALANTA_SHAFT_INTERVAL frames
/// later; a trail gathers for ATALANTA_TRAIL_FRAMES, the shaft flashes in for
/// ATALANTA_FLASH_FRAMES, one a picture, then flies off.
#define ATALANTA_SHAFT_FIRST_FRAME 64
#define ATALANTA_SHAFT_INTERVAL 8
#define ATALANTA_TRAIL_FRAMES 16
#define ATALANTA_FLASH_FRAMES 5

/// A landed shaft's burst of light: the ember streak sprite sheet's six
/// pictures, four frames each, the shaft still drawn over its first twelve
/// frames.
#define ATALANTA_BURST_PICTURES 6
#define ATALANTA_BURST_PICTURE_FRAMES 4
#define ATALANTA_BURST_SHAFT_FRAMES 12

/// Atalanta is three rows of three 32x32 sprites.
#define ATALANTA_SPRITE_ROWS 3
#define ATALANTA_SPRITES (ATALANTA_SPRITE_ROWS * ATALANTA_SPRITE_ROWS)

/// The trail particles' random rotations, after the first 512 particles of
/// PARTICLES, which hold the 384 trail particles.
#define ATALANTA_MATRICES                                                      \
  (Ram_MapGridCellBuffer + 512 * sizeof(struct Particle))

/// How far below its projected point a trail mote is drawn: the Japanese
/// release draws the trails 16 pixels higher than the later releases.
#if !TBS_PATCH_1
#define ATALANTA_TRAIL_DROP 0
#else
#define ATALANTA_TRAIL_DROP 16
#endif

/// The pictures in the slash sprite sheet: five flash pictures, then the shaft
/// in three parts, each in its own size.
#define ATALANTA_SHAFT_FLASH_A_WIDTH 14
#define ATALANTA_SHAFT_FLASH_A_HEIGHT 28
#define ATALANTA_SHAFT_FLASH_B_WIDTH 23
#define ATALANTA_SHAFT_FLASH_B_HEIGHT 44
#define ATALANTA_SHAFT_FLASH_C_TOP_WIDTH 20
#define ATALANTA_SHAFT_FLASH_C_TOP_HEIGHT 30
#define ATALANTA_SHAFT_FLASH_C_BOTTOM_WIDTH 22
#define ATALANTA_SHAFT_FLASH_C_BOTTOM_HEIGHT 33
#define ATALANTA_SHAFT_FLASH_D_TOP_WIDTH 18
#define ATALANTA_SHAFT_FLASH_D_TOP_HEIGHT 27
#define ATALANTA_SHAFT_FLASH_D_MIDDLE_WIDTH 22
#define ATALANTA_SHAFT_FLASH_D_MIDDLE_HEIGHT 22
#define ATALANTA_SHAFT_FLASH_D_BOTTOM_WIDTH 19
#define ATALANTA_SHAFT_FLASH_D_BOTTOM_HEIGHT 28
#define ATALANTA_SHAFT_FLASH_E_TOP_WIDTH 16
#define ATALANTA_SHAFT_FLASH_E_TOP_HEIGHT 23
#define ATALANTA_SHAFT_FLASH_E_MIDDLE_WIDTH 23
#define ATALANTA_SHAFT_FLASH_E_MIDDLE_HEIGHT 28
#define ATALANTA_SHAFT_FLASH_E_BOTTOM_WIDTH 20
#define ATALANTA_SHAFT_FLASH_E_BOTTOM_HEIGHT 28
#define ATALANTA_SHAFT_TOP_WIDTH 16
#define ATALANTA_SHAFT_TOP_HEIGHT 21
#define ATALANTA_SHAFT_MIDDLE_WIDTH 29
#define ATALANTA_SHAFT_MIDDLE_HEIGHT 35
#define ATALANTA_SHAFT_BOTTOM_WIDTH 21
#define ATALANTA_SHAFT_BOTTOM_HEIGHT 24
/// Where each picture begins in the slash sprite sheet, in bytes.
enum SlashSheetPicture {
  ATALANTA_SHAFT_FLASH_A = 0,
  ATALANTA_SHAFT_FLASH_B = ATALANTA_SHAFT_FLASH_A +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_A),
  ATALANTA_SHAFT_FLASH_C_TOP = ATALANTA_SHAFT_FLASH_B +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_B),
  ATALANTA_SHAFT_FLASH_C_BOTTOM = ATALANTA_SHAFT_FLASH_C_TOP +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_C_TOP),
  ATALANTA_SHAFT_FLASH_D_TOP = ATALANTA_SHAFT_FLASH_C_BOTTOM +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_C_BOTTOM),
  ATALANTA_SHAFT_FLASH_D_MIDDLE = ATALANTA_SHAFT_FLASH_D_TOP +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_TOP),
  ATALANTA_SHAFT_FLASH_D_BOTTOM = ATALANTA_SHAFT_FLASH_D_MIDDLE +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_MIDDLE),
  ATALANTA_SHAFT_FLASH_E_TOP = ATALANTA_SHAFT_FLASH_D_BOTTOM +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_BOTTOM),
  ATALANTA_SHAFT_FLASH_E_MIDDLE = ATALANTA_SHAFT_FLASH_E_TOP +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_TOP),
  ATALANTA_SHAFT_FLASH_E_BOTTOM = ATALANTA_SHAFT_FLASH_E_MIDDLE +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_MIDDLE),
  ATALANTA_SHAFT_TOP = ATALANTA_SHAFT_FLASH_E_BOTTOM +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_BOTTOM),
  ATALANTA_SHAFT_MIDDLE = ATALANTA_SHAFT_TOP +
      PICTURE_BYTES(ATALANTA_SHAFT_TOP),
  ATALANTA_SHAFT_BOTTOM = ATALANTA_SHAFT_MIDDLE +
      PICTURE_BYTES(ATALANTA_SHAFT_MIDDLE)
};

/// Under a violet sky Atalanta flies across the screen while sixteen shafts of
/// light gather trails of motes, flash and fly off; then 24 shafts rain down
/// diagonally on the targets and burst in light, hitting each target as they
/// land.
///
/// Abilities: Atalanta.
void BattleEffect_RunAtalanta(struct BattleEffectParams *args) {
  s32 point[3];
  struct ScreenPoint screen;
  struct SpritePlacement position;
  struct ScreenPoint target;
  BlitterPair draw;
  struct BattleBackgroundState *view;
  void **slots;
  void *canvas;
  struct BattleEffectState *state;
  s8 *path;
  s32 i;
  s32 j;
  s32 frame;
  u8 *sheet;
  s32 scrollX;
  s32 pathX;
  s32 pathY;
  s32 row;
  s32 column;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_BACKGROUND];
  view = slots[0];
  canvas = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND,
                          HEAP_SLOT_BATTLE_CANVAS);
  state = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND,
                         HEAP_SLOT_BATTLE_EFFECT);
  sheet = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND,
                         HEAP_SLOT_BATTLE_SHEET);
  scrollX = gBgScroll[1].x;
  state->args = args;
  // The screen wipes to black before Atalanta appears.
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  BG_PLTT[0] = 0;
  BG_PLTT[1] = 0;
  state->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleEffect_ShowLivingTargets(state->args);
  BattleEffect_CreateSprites(ATALANTA_SPRITES, SPRITE_ATALANTA, 1);
  Resource_LoadAndDecompress(RESOURCE_SLASH_SHEET, state->sheet, 1, 1);
  Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(RESOURCE_LIME_PALETTE),
                  SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  path = Resource_GetTableEntry(RESOURCE_ATALANTA_PATH);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE];
  draw[1] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED];
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  // The violet sky scrolls.
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_VIOLET_SKY_BACKDROP, 0);
  state->scrollTimer = 0;
  state->scrollInterval = 4;
  state->scrollStepX = -1;
  state->scrollStepY = 0;
  Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_VBLANK);
  view->busy = 1;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
  pathX = 0;
  pathY = 0;
  // Each shaft starts at a random spot with its trail of motes.
  for (i = 0; i != ATALANTA_SHAFTS; i++) {
    struct Particle *shaft = &state->particles[i];

    shaft->x = INTtoFIXED(Random16() % 96 + 12);
    shaft->y = INTtoFIXED((Random16() & 63) + 32);
    shaft->velocityX = 0;
    shaft->velocityY = 0;
    shaft->variant = 0;
    for (j = 0; j != ATALANTA_TRAIL_MOTES; j++) {
      PARTICLES[i * ATALANTA_TRAIL_MOTES + j].x = (Random16() & 15) + 48;
      Matrix_Initialize();
      Matrix_RotateZ(Random16() & TRIG_ANGLE_MASK);
      Matrix_RotateX(Random16() & TRIG_ANGLE_MASK);
      Matrix_RotateY(Random16() & TRIG_ANGLE_MASK);
      Matrix_Get(ATALANTA_MATRICES +
                 (i * ATALANTA_TRAIL_MOTES + j) * sizeof(gCurrentMatrix));
    }
  }
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  // The gathering (A or B ends it).
  for (frame = 0; frame != 220 && !(gKeysRepeat & (KEY_A | KEY_B)); frame++) {
    // Atalanta flies along her path.
    if (frame <= 209) {
      if (frame == 0) {
        pathX = (path[0] << 8) + (u8)path[1];
        pathY = (path[2] << 8) + (u8)path[3];
        path += 4;
      } else {
        pathX += path[0];
        pathY += path[1];
        path += 2;
      }
      position.groundHeight = 0;
      position.height = toFIXED(255);
      for (row = 0; row != ATALANTA_SPRITE_ROWS; row++) {
        for (column = 0; column != ATALANTA_SPRITE_ROWS; column++) {
          position.x = INTtoFIXED(pathX + 80 + column * 32);
          position.y = INTtoFIXED(64 - pathY + row * 32);
          Actor_ApplyProjectedPlacementFar(
              state->sprites[row * ATALANTA_SPRITE_ROWS + column], &position,
              &gAtalanta_UnitScale, 0);
        }
      }
    }
    point[1] = 0;
    point[2] = 0;
    // The battle background starts to darken.
    if (frame == 48) {
      state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
      state->fadeStep = 0;
    }
    // The shafts appear one after another.
    for (i = 0; i != ATALANTA_SHAFTS; i++) {
      s32 start = i * ATALANTA_SHAFT_INTERVAL + ATALANTA_SHAFT_FIRST_FRAME;

      if (frame >= start) {
        struct Particle *shaft = &state->particles[i];
        s32 x = FIXEDtoINT(shaft->x);
        s32 y = FIXEDtoINT(shaft->y);

        if (frame == start + ATALANTA_TRAIL_FRAMES + ATALANTA_FLASH_FRAMES - 1)
          Sound_PlayFar(SE_POWER_UP);
        if (frame >= start + ATALANTA_TRAIL_FRAMES + ATALANTA_FLASH_FRAMES) {
          shaft->x += shaft->velocityX;
          shaft->y += shaft->velocityY;
          shaft->velocityX -= toFIXED(1);
          shaft->velocityY += toFIXED(2);
          EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, ATALANTA_SHAFT_TOP,
                              x + 4, y - 40);
          EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                              ATALANTA_SHAFT_MIDDLE, x - 16, y - 19);
          EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                              ATALANTA_SHAFT_BOTTOM, x - 20, y + 16);
        } else if (frame >= start + ATALANTA_TRAIL_FRAMES) {
          switch (frame - start - ATALANTA_TRAIL_FRAMES) {
          case 0:
            EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                ATALANTA_SHAFT_FLASH_A, x - 7, y - 14);
            break;
          case 1:
            EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                ATALANTA_SHAFT_FLASH_B, x - 11, y - 22);
            break;
          case 2:
            EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                ATALANTA_SHAFT_FLASH_C_TOP, x - 4, y - 31);
            EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                ATALANTA_SHAFT_FLASH_C_BOTTOM, x - 16, y - 1);
            break;
          case 3:
            EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                ATALANTA_SHAFT_FLASH_D_TOP, x + 1, y - 38);
            EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                ATALANTA_SHAFT_FLASH_D_MIDDLE, x - 11, y - 11);
            EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                ATALANTA_SHAFT_FLASH_D_BOTTOM, x - 19, y + 11);
            break;
          case 4:
            EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                ATALANTA_SHAFT_FLASH_E_TOP, x + 4, y - 40);
            EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                ATALANTA_SHAFT_FLASH_E_MIDDLE, x - 10, y - 17);
            EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                ATALANTA_SHAFT_FLASH_E_BOTTOM, x - 20, y + 11);
            break;
          }
        } else {
          for (j = 0; j != ATALANTA_TRAIL_MOTES; j++) {
            struct Particle *trail = &PARTICLES[i * ATALANTA_TRAIL_MOTES + j];
            s32 size = 5;

            if (trail->x > 0) {
              Matrix_Load(ATALANTA_MATRICES + (i * ATALANTA_TRAIL_MOTES + j) *
                                                  sizeof(gCurrentMatrix));
              point[0] = trail->x;
              Projection_Point(point, &screen);
              screen.x = (screen.x >> 1) + x;
              screen.y = screen.y + y + ATALANTA_TRAIL_DROP;
              trail->x -= 4;
              EFFECT_DRAW_MOTE(draw[1], canvas, sheet, screen.x, screen.y,
                               size);
            }
          }
        }
      }
    }
    EFFECT_PRESENT_CANVAS(state);
  }
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  view->busy = 0;
  gBgScroll[1].x = scrollX;
  BattleEffect_SetupBlendedDisplay();
  for (i = 0; i != ATALANTA_SPRITES; i++)
    Sprite_DestroyFar(state->sprites[i]);
  REG_BG2PA = Q_8_8(0.5);
  REG_DISPCNT = EFFECT_DISPCNT;
  Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, Ram_MapGridCellBuffer,
                             1, 0);
  for (i = 0; i != ATALANTA_FALLING_SHAFT_SLOTS; i++) {
    struct Particle *shaft = &state->particles[i];
    s32 member = i % 6;

    if (member < state->args->targetCount) {
      Projection_ScaledUnit(state->args->targets[member], &target);
      shaft->y = -((Random16() & 31) + 40);
      shaft->x = target.x / 2 + (80 - shaft->y) / 2;
    } else {
      shaft->x = (Random16() & 63) + 80;
      shaft->y = -((Random16() & 31) + 40);
    }
    shaft->variant = -1;
  }
  // The shafts rain down on the targets.
  for (frame = 0; frame != ATALANTA_FALL_FRAMES; frame++) {
    for (i = 0; i != ATALANTA_FALLING_SHAFTS; i++) {
      struct Particle *shaft = &state->particles[i];

      if (frame >= i * 2 || frame > 40) {
        // A landed shaft bursts in light.
        if (shaft->variant >= 0) {
          if (shaft->variant <
              ATALANTA_BURST_PICTURES * ATALANTA_BURST_PICTURE_FRAMES) {
            s32 picture = shaft->variant / ATALANTA_BURST_PICTURE_FRAMES;
            u32 width;

            // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
            width = gBattleEffect_PuffArcPictureWidths[picture];
            draw[i & 1](
                canvas,
                Ram_MapGridCellBuffer + gBattleEffect_PuffArcPictures[picture],
                shaft->x - (width >> 1) - 8,
                shaft->y + gBattleEffect_PuffArcPictureBiasY[picture] - 40,
                width, gBattleEffect_PuffArcPictureHeights[picture]);
#else
            draw[i & 1](
                canvas,
                Ram_MapGridCellBuffer + gBattleEffect_PuffArcPictures[picture],
                shaft->x -
                    ((width = gBattleEffect_PuffArcPictureWidths[picture]) >>
                     1) -
                    8,
                shaft->y + gBattleEffect_PuffArcPictureBiasY[picture] - 40,
                width, gBattleEffect_PuffArcPictureHeights[picture]);
#endif
            if (shaft->variant < ATALANTA_BURST_SHAFT_FRAMES) {
              EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                  ATALANTA_SHAFT_TOP, shaft->x + 4,
                                  shaft->y - 40);
              EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                  ATALANTA_SHAFT_MIDDLE, shaft->x - 16,
                                  shaft->y - 19);
            }
          }
          shaft->variant++;
        } else {
          s32 height = 24;

          if (shaft->y > 56)
            height = height - shaft->y + 56;
          EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, ATALANTA_SHAFT_TOP,
                              shaft->x + 4, shaft->y - 40);
          EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                              ATALANTA_SHAFT_MIDDLE, shaft->x - 16,
                              shaft->y - 19);
          if (height > 0)
            draw[0](canvas, state->sheet + ATALANTA_SHAFT_BOTTOM, shaft->x - 20,
                    shaft->y + 16, ATALANTA_SHAFT_BOTTOM_WIDTH, height);
          shaft->x -= 6;
          shaft->y += 12;
          if (shaft->y > 79) {
            s32 member;

            shaft->variant = 0;
            state->shakeFrames = 2;
            Sound_PlayFar(SE_HEAVY_IMPACT);
            member = i % 6;
            if (member < state->args->targetCount) {
              BattleEffect_TintBattler(state->args->targets[member],
                                       SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
                                       8);
              BattleMotion_ApplyKnockbackFar(state->args->targets[member],
                                             KNOCKBACK_HOP);
            }
          }
        }
      }
    }
    BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(state);
  }
  EFFECT_FINISH_CANVAS();
}

#undef ATALANTA_FALL_FRAMES
#undef ATALANTA_SHAFTS
#undef ATALANTA_TRAIL_MOTES
#undef ATALANTA_FALLING_SHAFTS
#undef ATALANTA_SPRITE_ROWS
#undef ATALANTA_SPRITES
#undef ATALANTA_TRAIL_DROP
#undef ATALANTA_MATRICES
#undef ATALANTA_FALLING_SHAFT_SLOTS
#undef ATALANTA_SHAFT_FIRST_FRAME
#undef ATALANTA_SHAFT_INTERVAL
#undef ATALANTA_TRAIL_FRAMES
#undef ATALANTA_FLASH_FRAMES
#undef ATALANTA_BURST_PICTURES
#undef ATALANTA_BURST_PICTURE_FRAMES
#undef ATALANTA_BURST_SHAFT_FRAMES
