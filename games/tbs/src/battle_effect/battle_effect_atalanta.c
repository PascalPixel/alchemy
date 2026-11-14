/// @file
/// The Atalanta summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The trail particles' random rotations, after the particles in PARTICLES.
#define ATALANTA_MATRICES (Ram_MapGridCellBuffer + 14336)

/// The pictures in the slash sheet: five flash pictures, then the shaft in
/// three parts, with the sizes the draws use.
enum SlashSheetPicture {
  /// 14x28
  ATALANTA_SHAFT_FLASH_1 = 0,
  /// 23x44
  ATALANTA_SHAFT_FLASH_2 = ATALANTA_SHAFT_FLASH_1 + 14 * 28,
  /// 20x30
  ATALANTA_SHAFT_FLASH_3_TOP = ATALANTA_SHAFT_FLASH_2 + 23 * 44,
  /// 22x33
  ATALANTA_SHAFT_FLASH_3_BOTTOM = ATALANTA_SHAFT_FLASH_3_TOP + 20 * 30,
  /// 18x27
  ATALANTA_SHAFT_FLASH_4_TOP = ATALANTA_SHAFT_FLASH_3_BOTTOM + 22 * 33,
  /// 22x22
  ATALANTA_SHAFT_FLASH_4_MIDDLE = ATALANTA_SHAFT_FLASH_4_TOP + 18 * 27,
  /// 19x28
  ATALANTA_SHAFT_FLASH_4_BOTTOM = ATALANTA_SHAFT_FLASH_4_MIDDLE + 22 * 22,
  /// 16x23
  ATALANTA_SHAFT_FLASH_5_TOP = ATALANTA_SHAFT_FLASH_4_BOTTOM + 19 * 28,
  /// 23x28
  ATALANTA_SHAFT_FLASH_5_MIDDLE = ATALANTA_SHAFT_FLASH_5_TOP + 16 * 23,
  /// 20x28
  ATALANTA_SHAFT_FLASH_5_BOTTOM = ATALANTA_SHAFT_FLASH_5_MIDDLE + 23 * 28,
  /// 16x21
  ATALANTA_SHAFT_TOP = ATALANTA_SHAFT_FLASH_5_BOTTOM + 20 * 28,
  /// 29x35
  ATALANTA_SHAFT_MIDDLE = ATALANTA_SHAFT_TOP + 16 * 21,
  /// 21x24
  ATALANTA_SHAFT_BOTTOM = ATALANTA_SHAFT_MIDDLE + 29 * 35
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
  struct BattleBackgroundWork *view;
  void **slots;
  void *canvas;
  struct BattleEffectWork *work;
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
  work = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND,
                        HEAP_SLOT_BATTLE_EFFECT);
  sheet = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND,
                         HEAP_SLOT_BATTLE_SHEET);
  scrollX = gBgScroll[1].x;
  work->args = args;
  // The screen wipes to black before Atalanta appears.
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  BG_PLTT[0] = 0;
  BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleEffect_SelectLivingTargets(work->args);
  BattleEffect_SpawnObjects(9, SPRITE_ATALANTA, 1);
  Resource_LoadAndDecompress(RESOURCE_SLASH_SHEET, work->sheet, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(RESOURCE_LIME_PALETTE), 128);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  path = Resource_GetTableEntry(RESOURCE_STAGED_PARTICLE_PATH);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE];
  draw[1] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2];
  gProjection.centerY = 240;
  wait(1);
  // The violet sky scrolls.
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_VIOLET_SKY_BACKDROP, 0);
  work->scrollTimer = 0;
  work->scrollInterval = 4;
  work->scrollStepX = -1;
  work->scrollStepY = 0;
  Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_VBLANK);
  view->busy = 1;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  EFFECT_SHOW_WIDE_CANVAS(16);
  pathX = 0;
  pathY = 0;
  // Each shaft starts at a random spot with its trail of motes.
  for (i = 0; i != 16; i++) {
    struct Particle *shaft = &work->particles[i];

    shaft->x = px(Random16() % 96 + 12);
    shaft->y = px((Random16() & 63) + 32);
    shaft->velocityX = 0;
    shaft->velocityY = 0;
    shaft->variant = 0;
    for (j = 0; j != 24; j++) {
      PARTICLES[i * 24 + j].x = (Random16() & 15) + 48;
      Transform_Reset();
      Transform_ApplyRoll(Random16() & TRIG_ANGLE_MASK);
      Transform_ApplyPitch(Random16() & TRIG_ANGLE_MASK);
      Transform_ApplyYaw(Random16() & TRIG_ANGLE_MASK);
      Transform_Store(ATALANTA_MATRICES + (i * 24 + j) * 48);
    }
  }
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
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
      position.height = px(255);
      for (row = 0; row != 3; row++) {
        for (column = 0; column != 3; column++) {
          position.x = px(pathX + 80 + column * 32);
          position.y = px(64 - pathY + row * 32);
          Actor_ApplyProjectedPlacementFar(work->sprites[row * 3 + column],
                                           &position, &gAtalanta_UnitScale, 0);
        }
      }
    }
    point[1] = 0;
    point[2] = 0;
    if (frame == 48) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }
    // The shafts appear one after another.
    for (i = 0; i != 16; i++) {
      s32 start = i * 8 + 64;

      if (frame >= start) {
        struct Particle *shaft = &work->particles[i];
        s32 x = Q_16_16_TO_INT(shaft->x);
        s32 y = Q_16_16_TO_INT(shaft->y);

        if (frame == i * 8 + 84)
          Sound_PlayFar(SE_POWER_UP);
        if (frame >= i * 8 + 85) {
          shaft->x += shaft->velocityX;
          shaft->y += shaft->velocityY;
          shaft->velocityX -= px(1);
          shaft->velocityY += px(2);
          draw[0](canvas, work->sheet + ATALANTA_SHAFT_TOP, x + 4, y - 40, 16,
                  21);
          draw[0](canvas, work->sheet + ATALANTA_SHAFT_MIDDLE, x - 16, y - 19,
                  29, 35);
          draw[0](canvas, work->sheet + ATALANTA_SHAFT_BOTTOM, x - 20, y + 16,
                  21, 24);
        } else if (frame >= i * 8 + 80) {
          switch (frame - start - 16) {
          case 0:
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_FLASH_1, x - 7, y - 14,
                    14, 28);
            break;
          case 1:
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_FLASH_2, x - 11,
                    y - 22, 23, 44);
            break;
          case 2:
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_FLASH_3_TOP, x - 4,
                    y - 31, 20, 30);
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_FLASH_3_BOTTOM, x - 16,
                    y - 1, 22, 33);
            break;
          case 3:
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_FLASH_4_TOP, x + 1,
                    y - 38, 18, 27);
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_FLASH_4_MIDDLE, x - 11,
                    y - 11, 22, 22);
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_FLASH_4_BOTTOM, x - 19,
                    y + 11, 19, 28);
            break;
          case 4:
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_FLASH_5_TOP, x + 4,
                    y - 40, 16, 23);
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_FLASH_5_MIDDLE, x - 10,
                    y - 17, 23, 28);
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_FLASH_5_BOTTOM, x - 20,
                    y + 11, 20, 28);
            break;
          }
        } else {
          for (j = 0; j != 24; j++) {
            struct Particle *trail = &PARTICLES[i * 24 + j];

            if (trail->x > 0) {
              Transform_Load(ATALANTA_MATRICES + (i * 24 + j) * 48);
              point[0] = trail->x;
              // The Japanese release draws the trails 16 pixels higher;
              // the later releases move them 16 pixels down.
#if !TBS_PATCH_1
              {
                /* FAKEMATCH(temporary): out holds &screen; as
                 * Projection_Point(point, &screen), 386 of 958 instructions
                 * differ in tbs-ja. */
                struct ScreenPoint *out = &screen;

                Projection_Point(point, out);
              }
              screen.x = (screen.x >> 1) + x;
              screen.y = screen.y + y;
#else
              Projection_Point(point, &screen);
              screen.x = (screen.x >> 1) + x;
              screen.y = screen.y + y + 16;
#endif
              trail->x -= 4;
              EFFECT_DRAW_MOTE(draw[1], canvas, sheet, screen.x, screen.y, 5);
            }
          }
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  view->busy = 0;
  gBgScroll[1].x = scrollX;
  BattleEffect_SetupBlendedDisplay();
  for (i = 0; i != 9; i++)
    Sprite_DestroyFar(work->sprites[i]);
  REG_BG2PA = Q_8_8(0.5);
  REG_DISPCNT = EFFECT_DISPCNT;
  Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, Ram_MapGridCellBuffer,
                             1, 0);
  for (i = 0; i != 32; i++) {
    struct Particle *shaft = &work->particles[i];
    s32 member = i % 6;

    if (member < work->args->targetCount) {
      Projection_ScaledUnit(work->args->targets[member], &target);
      shaft->y = -((Random16() & 31) + 40);
      shaft->x = target.x / 2 + (80 - shaft->y) / 2;
    } else {
      shaft->x = (Random16() & 63) + 80;
      shaft->y = -((Random16() & 31) + 40);
    }
    shaft->variant = -1;
  }
  // The shafts rain down on the targets.
  for (frame = 0; frame != 88; frame++) {
    for (i = 0; i != 24; i++) {
      struct Particle *shaft = &work->particles[i];

      if (frame >= i * 2 || frame > 40) {
        // A landed shaft bursts in light.
        if (shaft->variant >= 0) {
          if (shaft->variant <= 23) {
            s32 picture = shaft->variant / 4;
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
            if (shaft->variant <= 11) {
              draw[0](canvas, work->sheet + ATALANTA_SHAFT_TOP, shaft->x + 4,
                      shaft->y - 40, 16, 21);
              draw[0](canvas, work->sheet + ATALANTA_SHAFT_MIDDLE,
                      shaft->x - 16, shaft->y - 19, 29, 35);
            }
          }
          shaft->variant++;
        } else {
          s32 height = 24;

          if (shaft->y > 56)
            height = height - shaft->y + 56;
          draw[0](canvas, work->sheet + ATALANTA_SHAFT_TOP, shaft->x + 4,
                  shaft->y - 40, 16, 21);
          draw[0](canvas, work->sheet + ATALANTA_SHAFT_MIDDLE, shaft->x - 16,
                  shaft->y - 19, 29, 35);
          if (height > 0)
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_BOTTOM, shaft->x - 20,
                    shaft->y + 16, 21, height);
          shaft->x -= 6;
          shaft->y += 12;
          if (shaft->y > 79) {
            s32 member;

            shaft->variant = 0;
            work->shakeFrames = 2;
            Sound_PlayFar(SE_HEAVY_IMPACT);
            member = i % 6;
            if (member < work->args->targetCount) {
              BattleEffect_TintBattler(work->args->targets[member],
                                       SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
                                       8);
              BattleMotion_ApplyKnockbackFar(work->args->targets[member],
                                             KNOCKBACK_HOP);
            }
          }
        }
      }
    }
    BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#undef ATALANTA_MATRICES
