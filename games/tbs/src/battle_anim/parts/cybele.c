/// @file
/// The Cybele summon.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

// Cybele reuses work->particles over the scene:
// - 0 to 15: vines (only the first ten are drawn).
// - 32 to 47: thorns, until frame 127.
// - 16 to 63: chips, thrown on frame 128 (only 32 to 63 are drawn); from
//   frame 128 the sparks, six per target from 16 on, share these slots; from
//   the third target's last two (slots 32 and 33) on, they share them with
//   drawn chips.
// - 0 on: embers, ten per target, from frame 302.
#define CYBELE_FRAMES 366
#define CYBELE_CHIP_COUNT 48      ///< chips thrown by the thorns
#define CYBELE_VINES 10           ///< vines drawn
#define CYBELE_BLAST_CELLS 0x1680 ///< the blast pictures, after the yellow ring
#define CYBELE_FIRST_SPARK 16     ///< the first spark in work->particles
#define CYBELE_FIRST_THORN 32     ///< the first thorn in work->particles
#define CYBELE_FIRST_CHIP 16      ///< the first chip in work->particles
#define CYBELE_EMBER_CELL 6       ///< the ember picture in gCybele_EmberOffsets

/// Thorns fly in from the right and vines grow out of the ground, lifting the
/// targets while sparks flash around them; then the picture dissolves pixel by
/// pixel, and the targets drop and explode.
///
/// Abilities: Cybele (SUMMON_ANIM_CYBELE).
///
/// The scene runs for CYBELE_FRAMES, in these stages:
///
/// - 0 to 127: the formation objects drop in, landing on frame 31; from 72
///   a thorn flies in from the right each frame.
/// - 128 to 224: vines grow out of the ground, shaking the screen, while
///   chips fly; sparks flash around the targets, which rise from 161.
/// - 224 to 247: the first 64 background colours fade to grey.
/// - 233 on: the canvas dissolves from the top, two rows a frame.
/// - 286 on: the targets drop, 5 frames apart.
/// - 302 on: each target is hit 16 frames after it drops and bursts into a
///   blast and embers.
///
/// Pressing A or B between frames 191 and 285 skips to the drop. At the end
/// the battle shows the damage and an impact burst
/// (BattleAnim_RunImpactBurst) explodes at the formation.
///
/// Besides the canvas and work->particles (above), it writes the dissolve
/// order into the first 1024 bytes of Ram_MapGridCellBuffer, greys the first 64
/// background colours, lifts and drops the targets' objects (their y and
/// gravity), and sets REG_BG2X, REG_BG2PA, REG_BLDCNT and work->shakeFrames.
/// It spawns eight formation objects and releases them, the transfer callback
/// and the blitters before it closes the canvas layer.
void BattleAnim_RunCybele(struct BattleAnimArgs *effect) {
  void **cursor;
  struct BattleAnimWork *work;
  u8 *canvas;
  s32 frame;
  s32 height;
  s32 speed;
  s32 phase;
  s32 rise;
  s32 i;
  s32 j;
  u8 *order;
  s32 point[3];
  struct ScreenPoint screen;
  BlitterPair draw;

  cursor = gBattleAnimWork;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleAnim_SpawnObjects(8, 0x177, 1);

  // Setup. The dissolve order: eight shuffled runs of the columns 0 to 127,
  // each shuffled by 128 random swaps.
  order = Ram_MapGridCellBuffer;
  for (i = 0; i != 1024; i++)
    order[i] = i & 127;
  for (i = 0; i != 8; i++) {
    for (j = 0; j != 128; j++) {
      s32 a = Random16() & 127;
      s32 b = Random16() & 127;
      u8 *to = &order[i * 128 + b];
      u8 *from = &order[i * 128 + a];
      u8 t = *to;

      *to = *from;
      *from = t;
    }
  }

  ANIM_START_NARROW_CANVAS(0);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_ThornVineSheet, work->sheet, 1,
                             1);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                    COMPOSITE_TRANSPARENT);
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                    COMPOSITE_TRANSPARENT);
  draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  REG_BG2X = -0x2000; // -32 in 20.8: the canvas shows 32 pixels right

  // Vines (0 to 15): x, the y of the root, the growth speed and the frames
  // before the vine starts to grow. Vines 0 to 4 stand 20 pixels apart from
  // x 0, rooted at y 104 to 111; the rest sit 10 pixels further right and 4
  // lower, so the ten drawn make two staggered rows.
  for (i = 0; i != 16; i++) {
    struct Particle *vine = &work->particles[i];

    if (i <= 4) {
      vine->x = i * 20;
      vine->y = (Random16() & 7) + 104;
    } else {
      vine->x = i * 20 - 90;
      vine->y = (Random16() & 7) + 108;
    }
    vine->velocityY = (Random16() & 7) + 4;
    vine->variant = (Random16() & 15) + 16;
  }
  // Sparks (16 to 31): a negative variant is the wait before one appears.
  for (i = 0; i != 16; i++) {
    struct Particle *spark = &work->particles[CYBELE_FIRST_SPARK + i];

    spark->x = (Random16() & 63) + 32;
    spark->y = (Random16() & 31) + 64;
    spark->variant = -(Random16() & 7) - 8;
  }
  // Thorns (32 to 47): fly left from the top right.
  for (i = 0; i != 16; i++) {
    struct Particle *thorn = &work->particles[CYBELE_FIRST_THORN + i];

    thorn->x = 128 << 16;
    thorn->y = 64 << 16;
    thorn->velocityX = -((Random16() & 255) + 200) << 9;
    thorn->velocityY = 0;
    thorn->variant = 0;
  }
  BattleAnim_SelectLivingTargets(work->effect);

  height = -(64 << 16);
  speed = 0;
  for (frame = 0; frame != CYBELE_FRAMES; frame++) {
    struct BattleCamera *camera = gCameraWork;

    // A or B skips the vines straight to the drop.
    if ((gKeysRepeat & (KEY_A | KEY_B)) != 0 && frame > 190 && frame <= 285) {
      Iwram_ClearWords(canvas, CANVAS_BYTES);
      frame = 286;
    }
    // From frame 224 the canvas keeps what is drawn, so the picture holds
    // while it greys and dissolves.
    if (frame == 224)
      work->transferMode = CANVAS_COPY;
    ANIM_VIEW_FROM_CAMERA(camera);
    // Frame 31: the formation lands; the screen shakes and the targets
    // flinch.
    if (frame == 31) {
      work->shakeFrames = 8;
      Sound_PlaySongFar(SE_GATE_MOVE);
      ANIM_FOR_EACH_TARGET(i, work->effect)
      BattleMotion_ApplyKnockbackFar(work->effect->targets[i],
                                     KNOCKBACK_BOUNCE);
    }
    if (frame == 72)
      Sound_PlaySongFar(SE_136);
    if (frame == 140)
      Sound_PlaySongFar(SE_156);

    // Drop the formation objects to a height of 64 pixels, and from frame
    // 48 to 96 wave objects 3 and 4 through three poses.
    speed += 0x4000;
    height += speed;
    if (height > 64 << 16)
      height = 64 << 16;
    BattleAnim_PlaceFormationObjects(SUMMON_FORMATION_CYBELE, 128 << 16,
                                     height);
    if (frame >= 48 && frame <= 96) {
      s32 pose = (frame - 48) / 24 % 3;

      Sprite_StartAnimFar(work->animations[3],
                          gCybele_ObjectPoses[pose][CYBELE_POSE_OBJECT3]);
      Sprite_StartAnimFar(work->animations[4],
                          gCybele_ObjectPoses[pose][CYBELE_POSE_OBJECT4]);
    }

    // Frames 72 to 127: thorn i flies from frame 72 + i until it reaches
    // the ground at y 104, its picture changing every 4 frames.
    if (frame >= 72 && frame <= 127) {
      for (i = 0; i != 16; i++) {
        struct Particle *thorn = &work->particles[CYBELE_FIRST_THORN + i];

        if (frame >= i + 72 && thorn->y < (104 << 16)) {
          s32 cellIndex = (frame + i) / 4 % 5;
          s32 x = Q_16_16_TO_INT(thorn->x);
          s32 y = thorn->y >> 16;
          u8 *cell = work->sheet + gCybele_ThornOffsets[cellIndex];
          u32 width = gCybele_ThornWidths[cellIndex];
          u32 tall;

          x -= width >> 1;
          tall = gCybele_ThornHeights[cellIndex];
          y -= tall >> 1;
          draw[0](canvas, cell, x, y, width, tall);
          Particle_AdvanceWithGravity2D(thorn, 64, 0x1000);
        }
      }
    }
    // Frame 128: throw the chips (16 to 63) up from the ground, each
    // waiting 16 to 31 frames to appear, and undo the canvas's 32-pixel
    // shift.
    if (frame == 128) {
      for (i = 0; i != CYBELE_CHIP_COUNT; i++) {
        struct Particle *chip = &work->particles[CYBELE_FIRST_CHIP + i];

        chip->x = (Random16() % 96) << 16;
        chip->y = ((Random16() & 7) + 88) << 16;
        chip->velocityX = ((Random16() & 255) - 128) << 11;
        chip->velocityY = -(Random16() & 255) << 11;
        chip->variant = -(Random16() & 15) - 16;
      }
      REG_BG2X = 0;
    }

    // Frames 128 to 224: grow the vines, shaking the screen while they
    // grow. A vine is a 24x8 tip on a stem of three 24x64 pieces; its
    // height wraps every 64 pixels past 184, so a grown vine keeps its
    // length. Odd vines are mirrored and carry a leaf; vines 6 to 9 use the
    // second stem picture.
    phase = frame - 128;
    if (phase >= 0 && phase <= 96) {
      rise = phase;
      if (rise > 80)
        rise = 80;
      else
        work->shakeFrames = 2;
      for (i = 0; i != CYBELE_VINES; i++) {
        struct Particle *vine = &work->particles[i];

        if (rise > vine->variant) {
          u8 *sheet = work->sheet;
          s32 top;
          s32 wrap;
          s32 k;

          // The second picture follows the first's tip and stem.
          if (i > 5)
            sheet = work->sheet + 24 * (8 + 64);
          // velocityY is the vine's growth in pixels a frame, variant its
          // delay.
          top = (rise - vine->variant) * vine->velocityY;
          for (wrap = top; wrap > 184; wrap -= 64)
            ;
          // The tip (24x8), until it has passed the top of the canvas.
          if (wrap <= 119)
            draw[i & 1](canvas, sheet, vine->x, vine->y - wrap - 8, 24, 8);
          // The stem: three 24x64 pieces, clipped at the top of the screen and
          // at the root.
          for (k = 0; k != 3; k++) {
            s32 y = vine->y - wrap + k * 64;
            s32 skip = 0;
            s32 rows = 64;

            if (y >= -64) {
              if (y < 0) {
                skip = -y * 24;
                rows = y + 64;
                y = 0;
              }
              if (y + rows > vine->y)
                rows -= y + rows - vine->y;
              draw[i & 1](canvas, sheet + skip + 192, vine->x, y, 24, rows);
            }
          }
          // A leaf, on the odd vines only.
          if ((i & 1) != 0) {
            s32 y = ((vine->y - top) & 127) - 16;
            s32 kind = i % 3;
            s32 full = gCybele_LeafSizes[kind];
            s32 rows = full;

            if (y + rows > vine->y)
              rows -= y + rows - vine->y;
            if (rows > 0)
              draw[i & 1](canvas, work->sheet + gCybele_LeafOffsets[kind],
                          vine->x + 8, y, full, rows);
          }
        }
      }
    }
    // Chips: until frame 159, one that leaves the screen is thrown up again.
    if (phase >= 0 && phase <= 95) {
      for (i = 0; i != 32; i++) {
        struct Particle *chip = &work->particles[32 + i];

        if (chip->variant >= 0) {
          s32 cellIndex = i % 5;

          draw[0](canvas, work->sheet + gCybele_ChipOffsets[cellIndex],
                  Q_16_16_TO_INT(chip->x), Q_16_16_TO_INT(chip->y),
                  gCybele_ChipWidths[cellIndex],
                  gCybele_ChipHeights[cellIndex]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->velocityY += 0x4000;
          if ((chip->y < 0 || chip->y > 120 << 16) && frame <= 159) {
            chip->x = (Random16() % 96) << 16;
            chip->y = ((Random16() & 7) + 88) << 16;
            chip->velocityX = ((Random16() & 255) - 128) << 11;
            chip->velocityY = -(Random16() & 255) << 11;
          }
        }
        chip->variant++;
      }
    }

    // Every 4 frames, move the 64 background colours one step towards grey.
    if (frame >= 224 && frame <= 247 && (frame & 3) == 0) {
      u16 *color = (u16 *)BG_PLTT;

      for (i = 0; i != 64; i++) {
        s32 red = *color & 31;
        s32 green = (*color >> 5) & 31;
        s32 blue = (*color >> 10) & 31;
        s32 grey = (red + green + blue) / 3;

        if (red > grey)
          red--;
        if (red < grey)
          red++;
        if (green > grey)
          green--;
        if (green < grey)
          green++;
        if (blue > grey)
          blue--;
        if (blue < grey)
          blue++;
        *color++ = RGB(red, green, blue);
      }
    }

    // Frames 128 to 300: six sparks keep flashing around each target's
    // body. Each lasts five frames, then until frame 199 waits up to 7
    // frames and flashes somewhere else.
    if (phase >= 0 && phase <= 172) {
      ANIM_FOR_EACH_TARGET(i, work->effect) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(work->effect->targets[i])->object;

        SET_VECTOR(point, object->x, object->y, object->z);
        Projection_Point(point, &screen);
        for (j = 0; j != 6; j++) {
          struct Particle *spark =
              &work->particles[CYBELE_FIRST_SPARK + i * 6 + j];

          if (spark->variant == 0) {
            spark->x = screen.x + (Random16() & 15) - 8;
            spark->y = screen.y + (Random16() & 15) - 40;
          }
          if (spark->variant >= 0 && spark->variant <= 4) {
            u8 *cell = work->sheet + gCybele_SparkOffsets[spark->variant];
            u32 size = gCybele_SparkSizes[spark->variant];
            u32 half = size >> 1;

            draw[0](canvas, cell, spark->x - half, spark->y - half, size, size);
          }
          spark->variant++;
          if (frame <= 199 && spark->variant == 5)
            spark->variant = -(Random16() & 7);
        }
      }
    }

    // Dissolve the 128x128 canvas from the top, two rows a frame, clearing
    // the pixels in the shuffled order: each row loses four pixels at a
    // time over 16 frames, its columns taken from run y & 7 of the order.
    if (frame > 232) {
      s32 row = frame * 2 - 496;

      for (i = 0; i != 32; i++) {
        for (j = 0; j != 4; j++) {
          s32 y = row + i;

          if (y >= 0 && y <= 127) {
            s32 x = order[((y & 7) * 32 + i) * 4 + j];

            canvas[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
          }
        }
      }
      row++;
      for (i = 0; i != 32; i++) {
        for (j = 0; j != 4; j++) {
          s32 y = row + i;

          if (y >= 0 && y <= 127) {
            s32 x = order[((y & 7) * 32 + i) * 4 + j];

            canvas[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
          }
        }
      }
    }

    // Lift the targets to a height of 128 pixels, 8 frames apart, without
    // gravity, every sprite of each in its damage pose.
    if (frame >= 161 && frame <= 223) {
      ANIM_FOR_EACH_TARGET(i, work->effect) {
        if (frame > i * 8 + 160) {
          struct BattleObject *slot =
              GetBattleObjectSlotFar(work->effect->targets[i]);
          struct MotionObject *object = slot->object;
          void *record;
          s32 n;

          object->y += 8 << 16;
          if (object->y > 128 << 16)
            object->y = 128 << 16;
          object->verticalMotionStrength = 0;
          for (n = 0; (record = GetMotionRecordFar(slot->object, n)) != 0; n++)
            Sprite_StartAnimFar(record, 5);
        }
      }
    }
    // Drop the targets from 96 pixels, 5 frames apart, and hit each one 16
    // frames later.
    ANIM_FOR_EACH_TARGET(i, work->effect) {
      if (frame == i * 5 + 286) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(work->effect->targets[i])->object;

        // 0xab85 is the battle units' usual gravity, which the lift
        // cleared (battle/motion.c sets the same).
        object->y = 96 << 16;
        object->verticalMotionStrength = 0xab85;
      }
      if (frame == i * 5 + 302) {
        ObjectGroup_UpdateMembers(work->effect->targets[i], SPRITE_COLOR_WHITE,
                                  -1, i, 8);
        Sound_PlaySongFar(SE_HEAVY_IMPACT);
        work->shakeFrames = 8;
      }
    }

    // Frame 302: switch to the blast sheet, brightest-wins blitters and a
    // canvas that keeps 75% of each frame, and give each target ten
    // embers, 72 degrees apart (two rings of five), of which only the first
    // ring is drawn.
    if (frame == 302) {
      ANIM_RELEASE_BLITTERS();
      Resource_LoadAndDecompress((s32)&ResourceId_YellowRingSheet, work->sheet,
                                 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                                 work->sheet + CYBELE_BLAST_CELLS, 1, 1);
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
      draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
      draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
      REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
                   BLDCNT_TGT2_ALL;
      REG_BG2PA = Q_8_8(0.5);
      REG_BG2X = 0;
      ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
      ANIM_FOR_EACH_TARGET(i, work->effect) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(work->effect->targets[i])->object;

        for (j = 0; j != 10; j++) {
          struct Particle *ember = &work->particles[i * 10 + j];

          SET_POSITION(ember, object->x, 20 << 16, object->z);
          SET_VELOCITY(ember, (Trig_Sin(j * 0x3334) * 4),
                       (Random16() & 0x7fff) + (1 << 16),
                       (Trig_Cos(j * 0x3334) * 4));
          ember->variant = 0;
        }
      }
    }
    // Target i's blast plays from frame 302 + 4i: six 40x24 pictures, two
    // frames each, drawn as two mirrored halves. Six frames in, the first
    // five of its embers show for 27 frames. The 2D step leaves z alone, so
    // they spread only across, and rise: their speed up and the 1/16 pixel
    // added each frame both point up in the projection. The canvas is now
    // shown at twice its width, so x positions are halved.
    if (frame > 301) {
      ANIM_FOR_EACH_TARGET(i, work->effect) {
        s32 start = i * 4 + 302;

        if (frame >= start && frame < i * 4 + 314) {
          s32 cellIndex = (frame - start) / 2;
          struct MotionObject *object =
              GetBattleObjectSlotFar(work->effect->targets[i])->object;

          SET_VECTOR(point, object->x, 0, object->z);
          Projection_Point(point, &screen);
          screen.x >>= 1;
          draw[0](canvas, work->sheet + cellIndex * (20 * 24), screen.x - 20,
                  screen.y - 24, 20, 24);
          draw[1](canvas, work->sheet + cellIndex * (20 * 24), screen.x,
                  screen.y - 24, 20, 24);
        }
        if (frame >= start + 6) {
          for (j = 0; j != 5; j++) {
            struct Particle *ember = &work->particles[i * 10 + j];

            Projection_Point(&ember->x, &screen);
            screen.x >>= 1;
            if (ember->variant >= 0 && ember->variant <= 26) {
              s32 cellIndex = CYBELE_EMBER_CELL;
              s32 offset = gCybele_EmberOffsets[cellIndex];
              u32 size = gCybele_EmberSizes[cellIndex];

              draw[0](canvas, work->sheet + offset, screen.x - (size >> 1),
                      screen.y - (size >> 1), size, size);
            }
            Particle_AdvanceWithGravity2D(ember, 60, 0x1000);
            ember->variant++;
          }
        }
      }
    }

    // The camera shake, by phase: the formation and thorns, the vines and
    // the dissolve, the blasts.
    if (frame <= 127)
      BattleCamera_Shake(4, 16);
    else if (frame <= 301)
      BattleCamera_Shake(2, 2);
    else
      BattleCamera_Shake(4, 8);
    ANIM_END_FRAME(work);
  }

  // The end: the impact burst builds its own blitters and starts its own
  // transfer, so the scene's are released first.
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  ANIM_RELEASE_BLITTERS();
  ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  BattleAnim_RunImpactBurst(SUMMON_FORMATION_CYBELE, 128 << 16, height);
  for (i = 0; i != 8; i++)
    Sprite_DestroyFar(work->animations[i]);
  BattleAnim_EndCanvasLayer();
}

#undef CYBELE_FRAMES
#undef CYBELE_CHIP_COUNT
#undef CYBELE_VINES
#undef CYBELE_FIRST_SPARK
#undef CYBELE_FIRST_THORN
#undef CYBELE_FIRST_CHIP
#undef CYBELE_EMBER_CELL
#undef CYBELE_BLAST_CELLS
