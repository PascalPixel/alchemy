/// @file
/// The Atalanta summon.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// The trail particles' random rotations, 48 bytes each, after the 384
/// particles in PARTICLES (28 bytes each, so they end at 0x2a00).
#define ATALANTA_MATRICES (Ram_MapGridCellBuffer + 0x3800)

/// The pictures in the slash sheet: five flash frames, then the slash in
/// three parts. Each starts where the one before ends; the notes give the
/// sizes the draws use. A flash picture's number is the frame of the flash it
/// is drawn on.
enum SlashSheetCell {
  ATALANTA_SLASH_FLASH_1 = 0x0000,        ///< 14x28
  ATALANTA_SLASH_FLASH_2 = 0x0188,        ///< 23x44
  ATALANTA_SLASH_FLASH_3_TOP = 0x057c,    ///< 20x30
  ATALANTA_SLASH_FLASH_3_BOTTOM = 0x07d4, ///< 22x33
  ATALANTA_SLASH_FLASH_4_TOP = 0x0aaa,    ///< 18x27
  ATALANTA_SLASH_FLASH_4_MIDDLE = 0x0c90, ///< 22x22
  ATALANTA_SLASH_FLASH_4_BOTTOM = 0x0e74, ///< 19x28
  ATALANTA_SLASH_FLASH_5_TOP = 0x1088,    ///< 16x23
  ATALANTA_SLASH_FLASH_5_MIDDLE = 0x11f8, ///< 23x28
  ATALANTA_SLASH_FLASH_5_BOTTOM = 0x147c, ///< 20x28
  ATALANTA_SLASH_TOP = 0x16ac,            ///< 16x21
  ATALANTA_SLASH_MIDDLE = 0x17fc,         ///< 29x35
  ATALANTA_SLASH_BOTTOM = 0x1bf3          ///< 21x24
};

/// Under a violet sky, a 3x3 formation of objects flies along a stored path
/// while sixteen blades gather trails of light, flash and fly off; then 24
/// blades rain down diagonally on the targets and burst into flame, hitting
/// each target as they land.
///
/// Abilities: Atalanta (SUMMON_ANIM_ATALANTA).
///
/// A or B skips the rest of the gathering, which lasts up to 220 frames; the
/// fall lasts 88. work->particles holds the 16 gathering blades and then the
/// falling ones; PARTICLES holds 24 trail particles for each gathering blade,
/// with their rotations in ATALANTA_MATRICES. The formation is nine objects in
/// work->objects, released between the parts. Loads the violet sky into BG1
/// and scrolls it with BattleAnim_AdvanceScrollOnInterval during the
/// gathering, holding view->busy; restores gBgScroll[1].x and gives BG1 back
/// with BattleAnim_SetupBlendedDisplay between the parts. Leaves the ember
/// sheet in Ram_MapGridCellBuffer.
///
/// effect: the targets.
void BattleAnim_RunAtalanta(struct BattleAnimArgs *effect) {
  s32 point[3];
  struct ScreenPoint screen;
  struct SpritePlacement position;
  struct ScreenPoint target;
  BlitterPair draw;
  void **slots;
  struct BattleBackgroundWork *view;
  void *canvas;
  struct BattleAnimWork *work;
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

  // The slots are read from the background's: the animation work, the canvas
  // and the sheet come before it.
  slots = &gHeapSlots[HEAP_SLOT_BATTLE_BACKGROUND];
  view = slots[0];
  canvas = slots[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE_BACKGROUND];
  work = slots[HEAP_SLOT_BATTLE_ANIM - HEAP_SLOT_BATTLE_BACKGROUND];
  sheet = slots[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_BACKGROUND];
  scrollX = gBgScroll[1].x;
  work->effect = effect;
  // Setup: the canvas, with colours 0 and 1 black, is wiped to colour 1 from
  // the top, so the screen goes black before the formation is spawned.
  ANIM_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleAnim_ConfigureEffectDisplay();
  BG_PLTT[0] = 0;
  BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, TASK_PRIORITY_VBLANK);
  BattleAnim_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleAnim_SelectLivingTargets(work->effect);
  // The formation: nine objects of SPRITE_ATALANTA, sprite priority 1.
  BattleAnim_SpawnObjects(9, SPRITE_ATALANTA, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SlashSheet, work->sheet, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_LimePalette), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  path = Resource_GetTableEntry((s32)&ResourceId_StagedParticlePath);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
  draw[0] = ANIM_BLITTERS[HEAP_SLOT_RAM_CODE];
  draw[1] = ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2];
  // The projection centre moves down to line 240 for the gathering;
  // BattleAnim_SetupBlendedDisplay puts it back at 120 between the parts.
  gProjection.centerY = 240;
  WaitFrames(1);
  // The violet sky scrolls one pixel left every four frames.
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_VioletSkyBackdrop, 0);
  work->scrollTimer = 0;
  work->scrollInterval = 4;
  work->scrollStepX = -1;
  work->scrollStepY = 0;
  Task_Add(BattleAnim_AdvanceScrollOnInterval, TASK_PRIORITY_VBLANK);
  view->busy = 1;
  BattleAnim_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  ANIM_SHOW_WIDE_CANVAS(16);
  pathX = 0;
  pathY = 0;
  // Each blade starts at a random spot on the canvas, x 12 to 107 and y 32
  // to 95, with 24 trail particles, each on a random rotation kept in
  // ATALANTA_MATRICES.
  for (i = 0; i != 16; i++) {
    struct Particle *slash = &work->particles[i];

    slash->x = (Random16() % 96 + 12) << 16;
    slash->y = ((Random16() & 63) + 32) << 16;
    slash->velocityX = 0;
    slash->velocityY = 0;
    slash->variant = 0;
    for (j = 0; j != 24; j++) {
      PARTICLES[i * 24 + j].x = (Random16() & 15) + 48;
      Transform_Reset();
      Transform_ApplyRoll(Random16() & 0xffff);
      Transform_ApplyPitch(Random16() & 0xffff);
      Transform_ApplyYaw(Random16() & 0xffff);
      Transform_Store(ATALANTA_MATRICES + (i * 24 + j) * 48);
    }
  }
  // Trails: each transfer keeps half of every canvas level.
  ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  // The gathering: 220 frames, or until A or B is pressed.
  for (frame = 0; frame != 220 && !(gKeysRepeat & (KEY_A | KEY_B)); frame++) {
    // The formation follows the path through frame 209: two 16-bit
    // coordinates, then a signed byte step for each axis every frame. Its
    // nine objects stand 32 pixels apart, the top-left one at the path
    // position.
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
      position.height = 0xff0000;
      for (row = 0; row != 3; row++) {
        for (column = 0; column != 3; column++) {
          position.x = (pathX + 80 + column * 32) << 16;
          position.y = (64 - pathY + row * 32) << 16;
          Object_ApplyProjectedPlacementFar(work->animations[row * 3 + column],
                                            &position, &gAtalanta_UnitScale, 0);
        }
      }
    }
    point[1] = 0;
    point[2] = 0;
    // From frame 48 the battle background darkens over 24 frames
    // (Palette_StepFadeTransfer, which the canvas layer added as a task).
    if (frame == 48) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }
    // Blade i appears at frame i * 8 + 64: its trails close in for 16
    // frames, it flashes for 5 (with a sound on the last), and from frame
    // i * 8 + 85 it flies off, accelerating down and to the left.
    for (i = 0; i != 16; i++) {
      s32 start = i * 8 + 64;

      if (frame >= start) {
        struct Particle *slash = &work->particles[i];
        s32 x = Q_16_16_TO_INT(slash->x);
        s32 y = Q_16_16_TO_INT(slash->y);

        if (frame == i * 8 + 84)
          Sound_PlaySongFar(SE_POWER_UP);
        if (frame >= i * 8 + 85) {
          slash->x += slash->velocityX;
          slash->y += slash->velocityY;
          slash->velocityX -= 0x10000;
          slash->velocityY += 0x20000;
          draw[0](canvas, work->sheet + ATALANTA_SLASH_TOP, x + 4, y - 40, 16,
                  21);
          draw[0](canvas, work->sheet + ATALANTA_SLASH_MIDDLE, x - 16, y - 19,
                  29, 35);
          draw[0](canvas, work->sheet + ATALANTA_SLASH_BOTTOM, x - 20, y + 16,
                  21, 24);
        } else if (frame >= i * 8 + 80) {
          // The flash: five pictures, one a frame, the later ones in parts.
          switch (frame - start - 16) {
          case 0:
            draw[0](canvas, work->sheet + ATALANTA_SLASH_FLASH_1, x - 7, y - 14,
                    14, 28);
            break;
          case 1:
            draw[0](canvas, work->sheet + ATALANTA_SLASH_FLASH_2, x - 11,
                    y - 22, 23, 44);
            break;
          case 2:
            draw[0](canvas, work->sheet + ATALANTA_SLASH_FLASH_3_TOP, x - 4,
                    y - 31, 20, 30);
            draw[0](canvas, work->sheet + ATALANTA_SLASH_FLASH_3_BOTTOM, x - 16,
                    y - 1, 22, 33);
            break;
          case 3:
            draw[0](canvas, work->sheet + ATALANTA_SLASH_FLASH_4_TOP, x + 1,
                    y - 38, 18, 27);
            draw[0](canvas, work->sheet + ATALANTA_SLASH_FLASH_4_MIDDLE, x - 11,
                    y - 11, 22, 22);
            draw[0](canvas, work->sheet + ATALANTA_SLASH_FLASH_4_BOTTOM, x - 19,
                    y + 11, 19, 28);
            break;
          case 4:
            draw[0](canvas, work->sheet + ATALANTA_SLASH_FLASH_5_TOP, x + 4,
                    y - 40, 16, 23);
            draw[0](canvas, work->sheet + ATALANTA_SLASH_FLASH_5_MIDDLE, x - 10,
                    y - 17, 23, 28);
            draw[0](canvas, work->sheet + ATALANTA_SLASH_FLASH_5_BOTTOM, x - 20,
                    y + 11, 20, 28);
            break;
          }
        } else {
          // The trails: a particle's x is its distance from the blade (48 to
          // 63, plain pixels, not 16.16); while it is positive the particle is
          // drawn on its stored rotation and moves 4 closer, so all are in
          // within the blade's 16 gathering frames.
          for (j = 0; j != 24; j++) {
            struct Particle *trail = &PARTICLES[i * 24 + j];

            if (trail->x > 0) {
              Transform_Load(ATALANTA_MATRICES + (i * 24 + j) * 48);
              point[0] = trail->x;
              // The Japanese release draws the blade trails 16 pixels higher;
              // the later releases move them 16 pixels down.
#if !TBS_PATCH_1
              {
                /* FAKEMATCH(temporary): the result pointer named, which gives
                 * the 124-byte frame and j a stack slot; as &screen, 166 of
                 * 958 instructions differ in tbs-ja. */
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
              draw[1](canvas, sheet + gBattleAnim_MoteCells[4], screen.x - 2,
                      screen.y - 5, 5, 10);
            }
          }
        }
      }
    }
    ANIM_PRESENT_FRAME(work);
  }
  // Between the parts: stop the scroll, give BG1 back to the battle, release
  // the formation and load the ember streak sheet into the map grid cell buffer
  // for the flames.
  Task_Remove(BattleAnim_AdvanceScrollOnInterval);
  view->busy = 0;
  gBgScroll[1].x = scrollX;
  BattleAnim_SetupBlendedDisplay();
  for (i = 0; i != 9; i++)
    Sprite_DestroyFar(work->animations[i]);
  REG_BG2PA = Q_8_8(0.5);
  REG_DISPCNT = ANIM_DISPCNT;
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             Ram_MapGridCellBuffer, 1, 0);
  // Second part: aim 32 blades above the field, in pixels, to fall
  // diagonally: blade i on target i % 6 where there is one, on a random
  // column otherwise. Only the first 24 fall. The x start allows for the
  // blade's fall of 6 across for every 12 down, to land on the target's
  // half-width x.
  for (i = 0; i != 32; i++) {
    struct Particle *slash = &work->particles[i];
    s32 member = i % 6;

    if (member < work->effect->targetCount) {
      Projection_ScaledUnit(work->effect->targets[member], &target);
      slash->y = -((Random16() & 31) + 40);
      slash->x = target.x / 2 + (80 - slash->y) / 2;
    } else {
      slash->x = (Random16() & 63) + 80;
      slash->y = -((Random16() & 31) + 40);
    }
    slash->variant = -1;
  }
  // 88 frames: 24 blades fall, blade i from frame 2i (all of them from
  // frame 41), the lower part cut short near the ground. Past row 79 a
  // blade lands, shaking the screen and hitting target i % 6 if there is
  // one, and bursts into flame for 24 frames, its slash still standing for
  // the first 12.
  for (frame = 0; frame != 88; frame++) {
    for (i = 0; i != 24; i++) {
      struct Particle *slash = &work->particles[i];

      if (frame >= i * 2 || frame > 40) {
        // Landed (variant counts the frames since): the flame's six
        // pictures, 4 frames each, the two blitters alternating by blade.
        if (slash->variant >= 0) {
          if (slash->variant <= 23) {
            s32 cell = slash->variant / 4;
            u32 width;

            // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
            width = gBattleAnim_PuffArcCellWidths[cell];
            draw[i & 1](canvas,
                        Ram_MapGridCellBuffer + gBattleAnim_PuffArcCells[cell],
                        slash->x - (width >> 1) - 8,
                        slash->y + gBattleAnim_PuffArcCellBiasY[cell] - 40,
                        width, gBattleAnim_PuffArcCellHeights[cell]);
#else
            draw[i & 1](
                canvas, Ram_MapGridCellBuffer + gBattleAnim_PuffArcCells[cell],
                slash->x -
                    ((width = gBattleAnim_PuffArcCellWidths[cell]) >> 1) - 8,
                slash->y + gBattleAnim_PuffArcCellBiasY[cell] - 40, width,
                gBattleAnim_PuffArcCellHeights[cell]);
#endif
            if (slash->variant <= 11) {
              draw[0](canvas, work->sheet + ATALANTA_SLASH_TOP, slash->x + 4,
                      slash->y - 40, 16, 21);
              draw[0](canvas, work->sheet + ATALANTA_SLASH_MIDDLE,
                      slash->x - 16, slash->y - 19, 29, 35);
            }
          }
          slash->variant++;
        } else {
          s32 height = 24;

          // Falling: the blade's lower part is cut off below row 96.
          if (slash->y > 56)
            height = height - slash->y + 56;
          draw[0](canvas, work->sheet + ATALANTA_SLASH_TOP, slash->x + 4,
                  slash->y - 40, 16, 21);
          draw[0](canvas, work->sheet + ATALANTA_SLASH_MIDDLE, slash->x - 16,
                  slash->y - 19, 29, 35);
          if (height > 0)
            draw[0](canvas, work->sheet + ATALANTA_SLASH_BOTTOM, slash->x - 20,
                    slash->y + 16, 21, height);
          slash->x -= 6;
          slash->y += 12;
          if (slash->y > 79) {
            s32 member;

            slash->variant = 0;
            work->shakeFrames = 2;
            Sound_PlaySongFar(SE_HEAVY_IMPACT);
            member = i % 6;
            if (member < work->effect->targetCount) {
              BattleAnim_TintBattler(work->effect->targets[member],
                                     SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
                                     8);
              BattleMotion_ApplyKnockbackFar(work->effect->targets[member],
                                             KNOCKBACK_HOP);
            }
          }
        }
      }
    }
    BattleCamera_Shake(4, 8);
    ANIM_END_FRAME(work);
  }
  ANIM_FINISH_CANVAS();
}

#undef ATALANTA_MATRICES
