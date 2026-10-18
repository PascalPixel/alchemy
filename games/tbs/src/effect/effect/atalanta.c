// Part of effect.c, the battle effect object, compiled only through it.
// The Atalanta summon.


// The trail particles' random rotations, 48 bytes each, after the 384
// particles in PARTICLES.
#define STAGED_PARTICLE_MATRICES (Ram_MapCellBuffer + 0x3800)

// The pictures in the slash sheet: five flash frames, then the slash in
// three parts.
enum SlashSheetCell {
  SLASH_FLASH_1 = 0x0000,
  SLASH_FLASH_2 = 0x0188,
  SLASH_FLASH_3_TOP = 0x057c,
  SLASH_FLASH_3_BOTTOM = 0x07d4,
  SLASH_FLASH_4_TOP = 0x0aaa,
  SLASH_FLASH_4_MIDDLE = 0x0c90,
  SLASH_FLASH_4_BOTTOM = 0x0e74,
  SLASH_FLASH_5_TOP = 0x1088,
  SLASH_FLASH_5_MIDDLE = 0x11f8,
  SLASH_FLASH_5_BOTTOM = 0x147c,
  SLASH_TOP = 0x16ac,
  SLASH_MIDDLE = 0x17fc,
  SLASH_BOTTOM = 0x1bf3
};

// Abilities: Atalanta (scene kind 4).
//
// Rain of blades: under a violet sky, a 3x3 formation of objects flies
// along a stored path while sixteen blades gather trails of light, flash
// and fly off; A or B skips ahead. Then 24 blades fall diagonally on the
// targets and burst into flame, hitting each target as they land.
//
// effect: the targets.
void BattleFx_RunAtalanta(struct BattleEffectArgument *effect) {
  s32 point[3];
  struct EffectPosition screen;
  struct SpritePlacement position;
  struct EffectPosition target;
  BlitterPair draw;
  void **slots;
  struct BattleBackgroundView *view;
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

  // The background is work slot 44: the slots before it hold the effect
  // work (-5), the canvas (-4) and the sheet (-3), the slots after it the
  // blitters (2 and 3).
  slots = &gWorkSlot[HEAP_SLOT_BATTLE_BACKGROUND];
  view = slots[0];
  canvas = slots[-4];
  work = slots[-5];
  sheet = slots[-3];
  scrollX = gBgScroll[1].x;
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(0x2000);
  BattleFx_ConfigureEffectDisplay();
  BG_PLTT[0] = 0;
  BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  BattleFx_WipeCanvas(0, 0);
  BattleFx_SelectLivingTargets(work->effect);
  BattleFx_SpawnObjects(9, 370, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SlashSheet, work->sheet, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_LimePalette), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  path = Resource_GetTableEntry((s32)&ResourceId_StagedParticlePath);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_ADDITIVE);
  draw[0] = slots[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_BACKGROUND];
  /* FAKEMATCH: the one-pass block keeps the table address
   * after the second read 2026-10-11: without the block differ 2 (mov r2, sp
   * moves ahead of the first store). */
  do {
    draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_BACKGROUND];
  } while (0);
  gProjection.centerY = 240;
  WaitFrames(1);
  // The violet sky scrolls one pixel left every four frames.
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  work->scrollTimer = 0;
  work->scrollInterval = 4;
  work->scrollStepX = -1;
  work->scrollStepY = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_AdvanceScrollOnInterval, 0x480);
  view->busy = 1;
  BattleFx_WipeCanvas(0, 1);
  EFFECT_SHOW_WIDE_CANVAS(16);
  pathX = 0;
  pathY = 0;
  // Each blade starts at a random spot with 24 trail particles, each on a
  // random rotation kept in STAGED_PARTICLE_MATRICES.
  for (i = 0; i != 16; i++) {
    struct EffectStep *slash = &work->particles[i];

    slash->x = (Random16() % 96 + 12) << 16;
    slash->y = ((Random16() & 63) + 32) << 16;
    slash->velocityX = 0;
    slash->velocityY = 0;
    slash->variant = 0;
    for (j = 0; j != 24; j++) {
      PARTICLES[i * 24 + j].x = (Random16() & 15) + 48;
      Render_ResetTransformState();
      SceneTransform_ApplyRoll(Random16() & 0xffff);
      SceneTransform_ApplyPitch(Random16() & 0xffff);
      SceneTransform_ApplyYaw(Random16() & 0xffff);
      Transform_Store(STAGED_PARTICLE_MATRICES + (i * 24 + j) * 48);
    }
  }
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
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
          Object_ApplyProjectedPlacementFar(work->objects[row * 3 + column],
                                            &position,
                                            &gStagedParticles_UnitScale, 0);
        }
      }
    }
    point[1] = 0;
    point[2] = 0;
    // From frame 48, a fade over 24 frames.
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
        struct EffectStep *slash = &work->particles[i];
        s32 x = HI(slash->x);
        s32 y = HI(slash->y);

        if (frame == i * 8 + 84)
          AudioCommand_PlayFar(212);
        if (frame >= i * 8 + 85) {
          slash->x += slash->velocityX;
          slash->y += slash->velocityY;
          slash->velocityX -= 0x10000;
          slash->velocityY += 0x20000;
          draw[0](canvas, work->sheet + SLASH_TOP, x + 4, y - 40, 16, 21);
          draw[0](canvas, work->sheet + SLASH_MIDDLE, x - 16, y - 19, 29, 35);
          draw[0](canvas, work->sheet + SLASH_BOTTOM, x - 20, y + 16, 21, 24);
        } else if (frame >= i * 8 + 80) {
          switch (frame - start - 16) {
          case 0:
            draw[0](canvas, work->sheet + SLASH_FLASH_1, x - 7, y - 14, 14, 28);
            break;
          case 1:
            draw[0](canvas, work->sheet + SLASH_FLASH_2, x - 11, y - 22, 23,
                    44);
            break;
          case 2:
            draw[0](canvas, work->sheet + SLASH_FLASH_3_TOP, x - 4, y - 31, 20,
                    30);
            draw[0](canvas, work->sheet + SLASH_FLASH_3_BOTTOM, x - 16, y - 1,
                    22, 33);
            break;
          case 3:
            draw[0](canvas, work->sheet + SLASH_FLASH_4_TOP, x + 1, y - 38, 18,
                    27);
            draw[0](canvas, work->sheet + SLASH_FLASH_4_MIDDLE, x - 11, y - 11,
                    22, 22);
            draw[0](canvas, work->sheet + SLASH_FLASH_4_BOTTOM, x - 19, y + 11,
                    19, 28);
            break;
          case 4:
            draw[0](canvas, work->sheet + SLASH_FLASH_5_TOP, x + 4, y - 40, 16,
                    23);
            draw[0](canvas, work->sheet + SLASH_FLASH_5_MIDDLE, x - 10, y - 17,
                    23, 28);
            draw[0](canvas, work->sheet + SLASH_FLASH_5_BOTTOM, x - 20, y + 11,
                    20, 28);
            break;
          }
        } else {
          // The trails: a particle's x is its distance from the blade; while
          // it is positive the particle is drawn on its stored rotation and
          // moves 4 closer.
          for (j = 0; j != 24; j++) {
            struct EffectStep *trail = &PARTICLES[i * 24 + j];

            if (trail->x > 0) {
              Transform_Load(STAGED_PARTICLE_MATRICES + (i * 24 + j) * 48);
              point[0] = trail->x;
#if !TBS_PATCH_1
              {
                /* FAKEMATCH: the named result pointer is a pseudo of its own,
                 * which gives the Japanese frame its 124 bytes and keeps the
                 * trail counter j in a stack slot, as native. 2026-10-13: the
                 * plain &screen argument, as the other editions pass it,
                 * gives a 120-byte frame and j in r9 (tbs-ja differ 372). */
                struct EffectPosition *out = &screen;

                EffectPosition_ApplyBaseAndYOffset(point, out);
              }
              screen.x = (screen.x >> 1) + x;
              screen.y = screen.y + y;
#else
              EffectPosition_ApplyBaseAndYOffset(point, &screen);
              screen.x = (screen.x >> 1) + x;
              screen.y = screen.y + y + 16;
#endif
              trail->x -= 4;
              draw[1](canvas, sheet + gBattleFx_MoteCellOffsets[4],
                      screen.x - 2, screen.y - 5, 5, 10);
            }
          }
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  // Between the parts: stop the scroll, release the formation and load the
  // ember streak sheet into the map cell buffer for the flames.
  Scheduler_RemoveCallback(BattleFx_AdvanceScrollOnInterval);
  view->busy = 0;
  gBgScroll[1].x = scrollX;
  BattleFx_SetupBlendedDisplay();
  for (i = 0; i != 9; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  REG_BG2PA = Q_8_8(0.5);
  REG_DISPCNT = EFFECT_DISPCNT;
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             Ram_MapCellBuffer, 1, 0);
  // Second part: aim 32 blades above the field, in pixels, to fall
  // diagonally: blade i on target i % 6 where there is one, on a random
  // column otherwise.
  for (i = 0; i != 32; i++) {
    struct EffectStep *slash = &work->particles[i];
    s32 member = i % 6;

    if (member < work->effect->count) {
      EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &target);
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
      struct EffectStep *slash = &work->particles[i];

      if (frame >= i * 2 || frame > 40) {
        if (slash->variant >= 0) {
          if (slash->variant <= 23) {
            s32 cell = slash->variant / 4;
            u32 width;

            // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
            width = gBattleFx_PuffArcCellWidths[cell];
            draw[i & 1](canvas,
                        Ram_MapCellBuffer +
                            gBattleFx_PuffArcCellSourceOffsets[cell],
                        slash->x - (width >> 1) - 8,
                        slash->y + gBattleFx_PuffArcCellBiasY[cell] - 40, width,
                        gBattleFx_PuffArcCellHeights[cell]);
#else
            draw[i & 1](
                canvas,
                Ram_MapCellBuffer + gBattleFx_PuffArcCellSourceOffsets[cell],
                slash->x - ((width = gBattleFx_PuffArcCellWidths[cell]) >> 1) -
                    8,
                slash->y + gBattleFx_PuffArcCellBiasY[cell] - 40, width,
                gBattleFx_PuffArcCellHeights[cell]);
#endif
            if (slash->variant <= 11) {
              draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4,
                      slash->y - 40, 16, 21);
              draw[0](canvas, work->sheet + SLASH_MIDDLE, slash->x - 16,
                      slash->y - 19, 29, 35);
            }
          }
          slash->variant++;
        } else {
          s32 height = 24;

          if (slash->y > 56)
            height = height - slash->y + 56;
          draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4, slash->y - 40,
                  16, 21);
          draw[0](canvas, work->sheet + SLASH_MIDDLE, slash->x - 16,
                  slash->y - 19, 29, 35);
          if (height > 0)
            draw[0](canvas, work->sheet + SLASH_BOTTOM, slash->x - 20,
                    slash->y + 16, 21, height);
          slash->x -= 6;
          slash->y += 12;
          if (slash->y > 79) {
            s32 member;

            slash->variant = 0;
            work->shakeFrames = 2;
            AudioCommand_PlayFar(134);
            member = i % 6;
            if (member < work->effect->count) {
              ObjectGroup_UpdateMembers(work->effect->actors[member],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
                                        8);
              BattleMotion_ApplyVariantMotionFar(work->effect->actors[member],
                                                 1);
            }
          }
        }
      }
    }
    Camera_ApplyShake(4, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}
