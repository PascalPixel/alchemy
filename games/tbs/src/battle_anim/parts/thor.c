/// @file
/// The Thor summon.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// 910 sparks from Ram_MapGridCellBuffer + 0xc58; the smoke pictures come
/// before them.
#define THOR_PARTICLES ((struct Particle *)(Ram_MapGridCellBuffer + 0xc58))

/// The particle sprites, after the light fan pictures.
#define THOR_SPRITES_CELLS 0x1800

/// The violet mote in the second part's sheet.
#define THOR_MOTE_CELLS 0x6000

#define THOR_PARTICLE_COUNT 910 ///< particles in THOR_PARTICLES
#define THOR_DIMMED_DOTS 10000  ///< the darker copies of the dot pictures
#define THOR_DIMMED_DOT_BYTES MOTE_SET_BYTES ///< bytes in each darker copy
#define THOR_DUSK_FRAMES 170      ///< frames, at most, of the dusk part
#define THOR_LIGHTNING_FRAMES 192 ///< frames of the lightning part
#define THOR_BOLTS 30             ///< violet bolts after the pillar
#define THOR_BOLT_SIZE 72         ///< each of the three bolt pictures is 72x72
#define THOR_BOLT_BYTES (THOR_BOLT_SIZE * THOR_BOLT_SIZE)

/// Launches up to max sparks from (px, py), 16.16, into the free slots
/// (variant 0) of THOR_PARTICLES, walking them with spark and counting with
/// index. Each flies at angle (0 straight up, a full turn 0x10000) at a
/// random speed of 32 to 1055 128ths of a pixel a frame across, twice that
/// up and down, and lives 32 to 39 frames. Used for the pillar's 512 sparks
/// and each bolt's 16; the mote's sparks are launched inline. The speed is
/// masked in place, the mask assigned first; as `Random16() & 0x3ff`, 3
/// instructions differ.
#define THOR_LAUNCH_SPARKS(spark, index, px, py, angleExpr, max)               \
  {                                                                            \
    s32 spawned = 0;                                                           \
    for (index = 0, spark = THOR_PARTICLES; index != THOR_PARTICLE_COUNT;      \
         index++, spark++) {                                                   \
      if (spark->variant == 0) {                                               \
        s32 speed = 0x3ff;                                                     \
        s32 angle;                                                             \
        speed &= Random16();                                                   \
        angle = angleExpr;                                                     \
        spark->x = px;                                                         \
        spark->y = py;                                                         \
        speed += 32;                                                           \
        spark->velocityX = (Trig_Sin(angle) * speed) >> 7;                     \
        spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;             \
        spark->variant = (Random16() & 7) + 32;                                \
        if (++spawned == (max)) {                                              \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// Over scrolling dusk clouds a circle of dots turns, sinks into the sky and
/// sends down a beam of light; then a pillar of lightning strikes a formation,
/// violet bolts follow, and the formation is pushed back in smoke.
///
/// Abilities: Thor (SUMMON_ANIM_THOR).
///
/// The scene plays in two parts on the animation canvas. In the dusk part
/// (THOR_DUSK_FRAMES, which A or B cuts short) a circle of 128 dots and three
/// lines across it form over scrolling dusk clouds; the circle turns, sinks
/// into the sky and a beam of light falls from it. In the lightning part
/// (THOR_LIGHTNING_FRAMES) a pillar of lightning strikes the formation and
/// throws sparks, THOR_BOLTS violet bolts follow while a violet mote sways
/// above, and the formation is pushed back in smoke until an impact burst ends
/// the animation. The sparks live in THOR_PARTICLES; a spark whose variant is 0
/// is free, and a positive variant counts the frames it has left.
///
/// Loads the dusk clouds as the battle background (BattleBackground_LoadFar),
/// and for the dusk part sets view->busy and moves gProjection.centerY to
/// 240 (BattleAnim_SetupBlendedDisplay puts the centre back). Spawns the nine
/// formation objects and releases them at the end; the closing
/// BattleAnim_RunImpactBurst builds and frees its own blitters and transfer
/// and overwrites work->particles and PARTICLES 0 to 63. Closes the canvas
/// layer.
void BattleAnim_RunThor(struct BattleAnimArgs *effect) {
  // The three lines' end points, in pairs: x and z on a plane, 96 being its
  // centre. One line runs across at z 135 and two meet at (96, 32).
  static s16 points[6][2] = {
      {8, 135}, {184, 135}, {128, 135}, {96, 32}, {96, 32}, {64, 135},
  };
  u8 ramp[32];
  s32 point[3];
  struct ScreenPoint screen[11];
  s32 position[3];
  s32 scale[3];
  void **slots;
  struct BattleBackgroundWork *view;
  struct BattleAnimWork *work;
  void *canvas;
  u8 *sprites;
  s32 frame;
  /* FAKEMATCH(dead): the blitter kept in a pair, whose unused second word
   * fills the frame; as one DrawRectangle, 101 of 1640 instructions differ in
   * tbs-en. */
  BlitterPair blit;
  s32 size;
  s32 count;
  s32 i;
  s32 j;
  s32 dotCount;
  struct Particle *spark;
  DrawRectangle *blitters; // the heap slots read as blitters
  s32 dimmedOffset = THOR_DIMMED_DOTS;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_BACKGROUND];
  view = slots[0];
  work = slots[HEAP_SLOT_BATTLE_ANIM - HEAP_SLOT_BATTLE_BACKGROUND];
  canvas = slots[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE_BACKGROUND];
  sprites = slots[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_BACKGROUND];
  work->effect = effect;
  // Open the canvas and load the pictures. The light fan sheet is loaded
  // twice, to the same place; the second load changes nothing.
  ANIM_START_NARROW_CANVAS(0x2000);
  Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesC,
                             work->sheet + THOR_SPRITES_CELLS, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // Eight copies of the dot pictures, each with its colour levels capped 7
  // lower than the one before.
  {
    s32 offset;
    for (i = 0, offset = 0; i != 8; i++) {
      u8 *src;
      for (j = 0, src = sprites; j != THOR_DIMMED_DOT_BYTES; j++) {
        s32 v = *src++;

        ANIM_CAP_LEVEL(v, 64 - i * 7);
        (work->sheet + THOR_DIMMED_DOTS)[offset + j] = v;
      }
      offset += j;
    }
  }

  BattleAnim_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0; // background palette 0, colour 1
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = 1;
  work->scrollStepY = 0;
  // The dusk clouds scroll a pixel every other frame; busy keeps the battle
  // camera off BG1's scroll meanwhile.
  view->busy = 1;
  Task_Add(BattleAnim_AdvanceScrollOnInterval, TASK_PRIORITY_VBLANK);
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, TASK_PRIORITY_VBLANK);
  work->transferMode = CANVAS_COPY;
  BattleAnim_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_DuskCloudsBackdrop, 0);
  // The projection centre moves down to line 240 for the dusk part;
  // BattleAnim_SetupBlendedDisplay puts it back.
  gProjection.centerY = 240;
  BattleAnim_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  REG_BG2PA = Q_8_8(0.5);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
  blitters = (DrawRectangle *)gHeapSlots;
  blit[0] = blitters[HEAP_SLOT_RAM_CODE];
  ANIM_SHOW_WIDE_CANVAS(15);
  ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // One row of the beam: brightest in the middle, darker to each side,
  // capped at level 63.
  for (i = 0; i != 16; i++) {
    ramp[i] = i * 8 + 3;
    ramp[31 - i] = i * 8 + 3;
    if (ramp[i] > 63) {
      ramp[i] = 63;
    }
    if (ramp[31 - i] > 63) {
      ramp[31 - i] = 63;
    }
  }
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  // First part, 170 frames (A or B skips it): a circle of dots and
  // three lines across it form and turn over dusk clouds, the circle shrinks
  // into the sky, and a beam of light falls from it.
  for (frame = 0; frame != THOR_DUSK_FRAMES; frame++) {
    size = 2;
    count = frame * 2;
    if (frame == 16) {
      Sound_PlaySongFar(SE_SPIN);
    }
    if (frame == 132) {
      Sound_PlaySongFar(SE_PSYNERGY_SPARKLE);
    }
    if (frame == 151) {
      Sound_PlaySongFar(SE_CRASH);
    }
    if (gKeysRepeat & (KEY_A | KEY_B)) {
      break;
    }
    // Red, green and blue sway a third of a turn apart, so the canvas
    // colours cycle through the hues.
    BattleAnim_UpdatePhasePalette(frame * 2, 0xaaab, 0x5555, 0);
    // The frame before the beam fills the canvas with level 26 (0x1a in each
    // byte) instead of fading it.
    if (frame == 150) {
      ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(26));
    } else {
      ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    }
    // Place the circle and lines: they turn over the clouds until frame
    // 128, then sink and shrink into the sky with growing dots (past 22
    // frames the sink holds at 20, not 22); after frame 150 they grow
    // again, with dots of size 5.
    Projection_Set(0x1fe0000, Iwram_RatioMulQ14(0x1fe0000, 0xc000), 0x7fff0000);
    Transform_Reset();
    if (frame > 128) {
      s32 sinkFrame = frame - 128;
      if (sinkFrame > 22) {
        sinkFrame = 20;
      }
      SET_VECTOR(position, 0, -sinkFrame << 17, 0x2000000);
      size = (sinkFrame >> 2) + 2;
      if (size > 8) {
        size = 8;
      }
      Transform_ApplyPosition(position);
      Transform_ApplyPitch(-0x1000);
      Transform_ApplyRoll(0x8000);
      Transform_ApplyPitch(0x8000);
      Transform_ApplyYaw(sinkFrame << 12);
      if (frame > 150) {
        SET_VECTOR(scale, frame * 0x1800 - 0xd9400, scale[0], scale[0]);
        size = 5;
      } else {
        SET_VECTOR(scale, 0x10000 - sinkFrame * 3 * 0x400, scale[0], scale[0]);
      }
      Transform_ApplyScale(scale);
    } else {
      SET_VECTOR(position, 0, 0, 0x1000000);
      Transform_ApplyPosition(position);
      Transform_ApplyPitch(-0x1000);
      Transform_ApplyRoll(frame << 8);
      Transform_ApplyPitch(frame << 8);
    }

    // The three lines, drawn dot by dot between pairs of points: line i
    // appears from frame 64 + 12 * i and grows 4 dots a frame, up to 48.
    // Each point is projected and halved in x for the double-width canvas,
    // and dot j sits j/48 of the way along (0x555 is 1/48 in 16.16).
    if (frame <= 149) {
      for (i = 0; i != 6; i++) {
        SET_VECTOR(point, (points[i][0] - 96) << 16, 0,
                   (points[i][1] - 96) << 16);
        Projection_Point(point, &screen[i]);
        screen[i].x = (screen[i].x >> 17) + 64;
        screen[i].y = (screen[i].y >> 16) + 60;
      }
      for (i = 0; i != 3; i++) {
        dotCount = count * 2 - i * 48 - 256;
        if (dotCount > 48) {
          dotCount = 48;
        }
        if (dotCount >= 0) {
          for (j = 0; j != dotCount; j++) {
            s32 x = screen[i * 2].x +
                    Iwram_MulQ16(j * (screen[i * 2 + 1].x - screen[i * 2].x),
                                 0x555);
            s32 y = screen[i * 2].y +
                    Iwram_MulQ16(j * (screen[i * 2 + 1].y - screen[i * 2].y),
                                 0x555);
            ANIM_DRAW_MOTE(blit[0], canvas, sprites, x, y, size);
          }
        }
      }
    }

    // The circle of up to 128 dots, two more each frame, darkening once
    // the beam has fallen: from frame 140 it takes the brighter pixel
    // instead of adding, and from frame 157 each frame draws it with a
    // darker copy of the dots, down to the seventh. The test on frame 179
    // always holds, as the part has 170 frames.
    if (frame <= 179) {
      DrawRectangle drawAlt;
      s32 level = 0;

      if (frame > 155) {
        level = frame - 156;
      }
      if (level > 7) {
        level = 7;
      }
      if (frame <= 139) {
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
        drawAlt = ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2];
      } else {
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
        drawAlt = ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2];
      }
      dotCount = count;
      if (dotCount > 128) {
        dotCount = 128;
      }
      for (i = 0; i != dotCount; i++) {
        s32 dotSize = size + 1;
        point[0] = Trig_Sin(i << 9) * 96;
        point[2] = -(Trig_Cos(i << 9) * 96);
        Projection_Point(point, &screen[0]);
        screen[0].x = (screen[0].x >> 17) + 64;
        screen[0].y = (screen[0].y >> 16) + 60;
        drawAlt(canvas,
                work->sheet +
                    (level * THOR_DIMMED_DOT_BYTES +
                     gBattleAnim_MoteCells[dotSize - 1]) +
                    dimmedOffset,
                screen[0].x - dotSize / 2, screen[0].y - dotSize, dotSize,
                dotSize * 2);
      }
      FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
    }

    // The beam, frames 151 to 167: rows of the ramp narrowing at its top,
    // a column of full rows growing 20 pixels a frame and narrowing a
    // pixel each side a frame from frame 152, and the light fan falling
    // down it in two mirrored halves.
    if ((u32)(frame - 151) <= 16) {
      s32 inset = 0;
      s32 beamHeight = (frame - 151) * 20 + 44;
      if (frame > 151) {
        inset = frame - 152;
      }
      if (inset > 15) {
        inset = 15;
      }
      for (i = 1; i != 10; i++) {
        if (inset + i <= 15) {
          blit[0](canvas, &ramp[inset + i], inset + 48 + i, 16 - i,
                  32 - inset * 2 - i * 2, 1);
        }
      }
      for (i = 0; i != beamHeight; i++) {
        s32 fullWidth = 32;
        // loop.c matches this 32 with the smoke calls' 32 and hoists it out of
        // the frame loop, where it loses its register and later reloads rotate:
        // after an earlier move its threshold is 12, and savings 2 x life 19 x
        // 12 = 456 reaches the loop's 448 insns.
        /* FAKEMATCH(barrier): fullWidth's 32 kept in this loop's preheader, and
         * i's life lengthened so dotCount takes r9; without the asm, 330 of
         * 1640 instructions differ in tbs-en. */
        asm("" : "+r"(fullWidth));
        blit[0](canvas, ramp + inset, inset + 48, i + 16, fullWidth - inset * 2,
                1);
      }
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
      ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, work->sheet, 32,
                                          (frame - 151) * 20 - 12, 32, 96);
      FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
      ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, work->sheet, 64,
                                          (frame - 151) * 20 - 12, 32, 96);
      FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
    }
    ANIM_PRESENT_FRAME(work);
  }

  // Second part, 192 frames: a pillar of lightning strikes the
  // formation, throwing sparks, then violet bolts strike again and again while
  // the formation is pushed back in smoke, ending in an impact burst.
  view->busy = 0;
  Task_Remove(BattleAnim_AdvanceScrollOnInterval);
  BattleAnim_SetupBlendedDisplay();
  BattleAnim_SelectLivingTargets(work->effect);
  // Nine objects of resource 0x173, the formation (SUMMON_FORMATION_THOR),
  // released at the end.
  BattleAnim_SpawnObjects(9, 0x173, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                             Ram_MapGridCellBuffer, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_VioletMoteSheet,
                             work->sheet + THOR_MOTE_CELLS, 1, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesB, sprites, 0, 0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  {
    s32 formationX;
    s32 formationY;
    s32 formationSpeedX;
    s32 formationSpeedY;
    s32 pillarX;
    s32 pillarY;

    formationX = 0x500000;
    formationY = 0x400000;
    formationSpeedX = 0;
    formationSpeedY = 0;
    for (i = 0; i != THOR_PARTICLE_COUNT; i++) {
      THOR_PARTICLES[i].variant = 0;
    }
    // Nothing in Thor reads these 16 particles, and BattleAnim_RunImpactBurst
    // overwrites them; only their random draws change what is drawn later.
    // XXX (no evidence): what they were for; no code that reads them is left.
    for (i = 0; i != 16; i++) {
      work->particles[i].x = (Random16() & 31) - 16;
      work->particles[i].y = Random16() & 63;
      work->particles[i].variant = -(Random16() & 15);
    }
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    Sound_PlaySongFar(SE_CRASH);
    // The pillar is 48 pixels wide at canvas x 36 and reaches from the top
    // down to line 112; its sparks start at its foot, x 60.
    pillarX = 60;
    pillarY = 112;

    for (frame = 0; frame != THOR_LIGHTNING_FRAMES; frame++) {
      s32 stormFrame;
      if (frame == 20) {
        Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetB,
                                   work->sheet, 1, 0);
      }
      // A heavy impact sound every 8 frames from frame 80 to 136, under
      // the bolts.
      stormFrame = frame - 80;
      if ((u32)stormFrame <= 59 && !(frame & 7)) {
        Sound_PlaySongFar(SE_HEAVY_IMPACT);
      }
      if (frame == 140) {
        ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
      // The pillar of lightning, frames 8 to 16: its body on frames 9 to
      // 11, its top on 12 to 15, and on frame 10 up to 512 sparks thrown
      // up from its foot. The loop runs once.
      for (j = 0; j != 1; j++) {
        s32 start = j * 128 + 8;
        if (frame >= start && frame < j * 128 + 17) {
          if (frame >= j * 128 + 9 && frame < j * 128 + 12) {
            blit[0](canvas, work->sheet, 36, 0, 48, pillarY);
          }
          if (frame >= j * 128 + 12 && frame < start + 8) {
            blit[0](canvas, work->sheet + (48 * 112), 36, 0, 48, 112);
          }
          if (frame == start + 2) {
            THOR_LAUNCH_SPARKS(spark, i, pillarX << 16, pillarY << 16,
                               (Random16() & 0x7fff) - 0x4000, 512);
            work->shakeFrames = 8;
          }
        }
      }
      // The pillar strikes every target on frame 10.
      if (frame == 10) {
        ANIM_FOR_EACH_TARGET(i, work->effect) {
          BattleAnim_TintBattler(work->effect->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 8);
          BattleMotion_ApplyKnockbackFar(work->effect->targets[i],
                                         KNOCKBACK_HURL);
        }
      }
      // Frames 80 to 143: thirty violet bolts, 2 frames apart, each shown
      // for 2 frames with a jitter, throwing 16 sparks and striking every
      // target in a random colour; the canvas flashes every 8 frames for
      // the first six; and the violet mote sways in four mirrored copies,
      // shedding 4 sparks a frame.
      if ((u32)stormFrame <= 63) {
        s32 moteX;
        s32 moteY;
        s32 mirrorX;
        for (i = 0; i != 6; i++) {
          if (frame == i * 8 + 80) {
            Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
          }
        }
        for (i = 0; i != THOR_BOLTS; i++) {
          if (frame >= i * 2 + 80) {
            if (frame < i * 2 + 82) {
              s32 boltX = (Random16() & 3) - 3;
              s32 boltY = (Random16() & 3) + 32;
              ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                                COMPOSITE_LIGHTEN);
              ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2](
                  canvas, work->sheet + i % 3 * THOR_BOLT_BYTES, boltX, boltY,
                  THOR_BOLT_SIZE, THOR_BOLT_SIZE);
              FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
            }
            if (frame == i * 2 + 80) {
              THOR_LAUNCH_SPARKS(spark, j, 0x100000, 0x500000,
                                 Random16() & 0xffff, 16);
              work->shakeFrames = 8;
              ANIM_FOR_EACH_TARGET(j, work->effect) {
                BattleAnim_TintBattler(work->effect->targets[j],
                                       gThor_TargetPalettes[Random16() & 3],
                                       POSE_DAMAGE, j, 4);
                BattleMotion_ApplyKnockbackFar(work->effect->targets[j],
                                               KNOCKBACK_HURL);
              }
            }
          }
        }
        {
          // The mote: a random offset 8 to 15 pixels from where it rests.
          s32 radius = 7;
          s32 angle;
          // Masked in place, the mask assigned first; as Random16() & mask, 3
          // instructions differ.
          radius &= Random16();
          angle = Random16() & 0xffff;
          moteX = (Trig_Sin(angle) * (radius + 8)) >> 16;
          mirrorX = moteX + 72;
          moteY = 32 - ((Trig_Cos(angle) * (radius + 8)) >> 16);
        }
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
        ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas,
                                            work->sheet + THOR_MOTE_CELLS,
                                            moteX + 60, moteY - 24, 12, 24);
        FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
        ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2](
            canvas, work->sheet + THOR_MOTE_CELLS, mirrorX, moteY - 24, 12, 24);
        FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
        ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2](
            canvas, work->sheet + THOR_MOTE_CELLS, moteX + 60, moteY, 12, 24);
        FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
        ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2](
            canvas, work->sheet + THOR_MOTE_CELLS, mirrorX, moteY, 12, 24);
        FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
        // Four sparks a frame from the right-hand copies, in any direction,
        // at 64 to 127 128ths of a pixel a frame (twice that up and down),
        // living 16 to 23 frames.
        j = 0;
        for (i = 0, spark = THOR_PARTICLES; i != THOR_PARTICLE_COUNT;
             i++, spark++) {
          if (spark->variant == 0) {
            s32 speed = 63;
            s32 angle;
            // Masked in place, the mask assigned first; as Random16() & mask, 3
            // instructions differ.
            speed &= Random16();
            angle = Random16() & 0xffff;
            spark->x = mirrorX << 16;
            spark->y = moteY << 16;
            speed += 64;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
            spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
            spark->variant = (Random16() & 7) + 16;
            if (++j == 4) {
              break;
            }
          }
        }
      }

      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                        COMPOSITE_LIGHTEN);
      {
        // Live sparks keep their line, slowing to 60/64 a frame with no
        // gravity, and one past the ground (y 120) bounces back at half
        // speed instead of being drawn; those on the canvas shrink from 3
        // pixels wide to 1 as their life runs out.
        DrawRectangle draw = ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2];
        struct Particle *spark;
        for (i = 0, spark = THOR_PARTICLES; i != THOR_PARTICLE_COUNT;
             i++, spark++) {
          if (spark->variant > 0) {
            spark->variant--;
            Particle_AdvanceWithGravity2D(spark, 60, 0);
            if (spark->y > 0x780000) {
              spark->velocityY = -spark->velocityY / 2;
            } else if ((u32)spark->x <= 0x7effff && spark->y >= 0) {
              s32 x = spark->x >> 16;
              s32 y = spark->y >> 16;
              s32 dotSize = spark->variant / 16 + 1;
              ANIM_DRAW_MOTE(draw, canvas, sprites, x, y, dotSize);
            }
          }
        }
      }
      FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);

      // The formation is placed from frame 16, its corner at (80, 64), and
      // pushed back from frame 32: right 4 pixels a frame and up 1/4, the
      // push slowing by 1/16 each frame.
      if (frame > 15) {
        if (frame == 32) {
          formationSpeedX = 0x40000;
          formationSpeedY = -0x4000;
        }
        if (frame > 31) {
          formationX += formationSpeedX;
          formationY += formationSpeedY;
          formationSpeedX = formationSpeedX * 60 / 64;
          formationSpeedY = formationSpeedY * 60 / 64;
        }
        BattleAnim_PlaceFormationObjects(SUMMON_FORMATION_THOR, formationX,
                                         formationY);
      }
      // Frames 16 to 143: three puffs of smoke a frame around the
      // formation, from the first three glint cells, placed and flipped at
      // random, up to 8 pixels across and 40 up or down from canvas line
      // 56, clipped at the canvas edges and drawn where brighter.
      if ((u32)(frame - 16) <= 127) {
        s32 formationScreenX;
        for (i = 0, formationScreenX = formationX >> 17; i != 3; i++) {
          s32 glint = i & 3;
          s32 angle = Random16() & 0xffff;
          s32 x = (((Trig_Sin(angle) * 8)) >> 16) + formationScreenX -
                  (gBattleAnim_GlintCellWidths[glint] >> 1);
          s32 y = ((Trig_Cos(angle) * 40) >> 16) -
                  (gBattleAnim_GlintCellHeights[glint] >> 1);
          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            3 | gThor_SmokeFlips[Random16() & 3], 2);
          ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2](
              canvas, Ram_MapGridCellBuffer + gBattleAnim_GlintCells[glint], x,
              y + 56, gBattleAnim_GlintCellWidths[glint],
              gBattleAnim_GlintCellHeights[glint]);
          FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
        }
      }
      if (frame <= 31) {
        BattleCamera_Shake(8, 16);
      } else {
        BattleCamera_Shake(4, 4);
      }
      ANIM_END_FRAME(work);
    }

    FreeHeapSlot(HEAP_SLOT_RAM_CODE);
    Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
    BattleAnim_RunImpactBurst(SUMMON_FORMATION_THOR, formationX, formationY);
  }
  for (i = 0; i != 9; i++) {
    Sprite_DestroyFar(work->animations[i]);
  }
  BattleAnim_EndCanvasLayer();
}

#undef THOR_PARTICLES
#undef THOR_MOTE_CELLS
#undef THOR_PARTICLE_COUNT
#undef THOR_DIMMED_DOTS
#undef THOR_DIMMED_DOT_BYTES
#undef THOR_DUSK_FRAMES
#undef THOR_LIGHTNING_FRAMES
#undef THOR_BOLTS
#undef THOR_BOLT_SIZE
#undef THOR_BOLT_BYTES
#undef THOR_LAUNCH_SPARKS
#undef THOR_SPRITES_CELLS
