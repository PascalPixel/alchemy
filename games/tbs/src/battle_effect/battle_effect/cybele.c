/// @file
/// The Cybele summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// Cybele reuses work->particles over the effect:
// - 0 to 15: vines (only the first ten are drawn).
// - 32 to 47: thorns, until frame 127.
// - 16 to 63: chips, thrown on frame 128 (only 32 to 63 are drawn); from
//   frame 128 the sparks, six per target from 16 on, share these slots; from
//   the third target's last two (slots 32 and 33) on, they share them with
//   drawn chips.
// - 0 on: dust clouds, ten per target, from frame 302.
#define CYBELE_BLAST_PICTURES                                                  \
  5760                        ///< the blast pictures, after the yellow ring
#define CYBELE_FIRST_SPARK 16 ///< the first spark in work->particles
#define CYBELE_FIRST_THORN 32 ///< the first thorn in work->particles
#define CYBELE_FIRST_CHIP 16  ///< the first chip in work->particles
#define CYBELE_DUST_PICTURE 6 ///< the dust cloud in gCybele_BlastPictures

/// The vine pictures in the thorn vine sheet, each starting where the one
/// before ends, below a 24x8 tip at 0. The flowers, thorns and sparks follow
/// (gCybele_FlowerPictures, gCybele_ThornPictures, gCybele_SparkPictures).
enum ThornVineSheetPicture {
  /// 24x64
  CYBELE_VINE_STEM = 24 * 8,
  /// a thinner vine: its 24x8 tip, then its 24x64 stem
  CYBELE_THIN_VINE = CYBELE_VINE_STEM + 24 * 64
};

/// Cybele drops in at the right and thorns fly from her mouth; flowering
/// vines grow out of the ground, lifting the targets while sparks flash around
/// them; then the picture greys and dissolves pixel by pixel, and the targets
/// drop, each landing on a spreading ring in a cloud of dust.
///
/// Abilities: Cybele (SUMMON_EFFECT_CYBELE).
void BattleEffect_RunCybele(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
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

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_SpawnObjects(8, SPRITE_CYBELE, 1);

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

  EFFECT_START_NARROW_CANVAS(0);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_ThornVineSheet, work->sheet, 1,
                             1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  REG_BG2X = -(32 << 8); // -32 in 20.8: the canvas shows 32 pixels right

  // Vines (0 to 15): x, the y of the root, the growth speed and the frames
  // before the vine starts to grow.
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

    thorn->x = px(128);
    thorn->y = px(64);
    thorn->velocityX = -((Random16() & 255) + 200) << 9;
    thorn->velocityY = 0;
    thorn->variant = 0;
  }
  BattleEffect_SelectLivingTargets(work->args);

  height = -px(64);
  speed = 0;
  for (frame = 0; frame != 366; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    // A or B skips the vines straight to the drop.
    if ((gKeysRepeat & (KEY_A | KEY_B)) != 0 && frame > 190 && frame <= 285) {
      Iwram_ClearWords(canvas, CANVAS_BYTES);
      frame = 286;
    }
    // From frame 224 the canvas keeps what is drawn, so the picture holds
    // while it greys and dissolves.
    if (frame == 224)
      work->transferMode = CANVAS_COPY;
    EFFECT_VIEW_FROM_CAMERA(camera);
    // Frame 31: Cybele lands; the screen shakes and the targets flinch.
    if (frame == 31) {
      work->shakeFrames = 8;
      Sound_PlayFar(SE_GATE_MOVE);
      EFFECT_FOR_EACH_TARGET(i, work->args)
        BattleMotion_ApplyKnockbackFar(work->args->targets[i],
                                       KNOCKBACK_BOUNCE);
    }
    if (frame == 72)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 140)
      Sound_PlayFar(SE_SWITCH_MECHANISM);

    // Drop Cybele's sprites to a height of 64 pixels, and from frame
    // 48 to 96 take the two halves of Cybele's face through three poses:
    // her mouth opens as the thorns fly and shuts again.
    speed += px(1) / 4;
    height += speed;
    if (height > px(64))
      height = px(64);
    BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_CYBELE, px(128),
                                       height);
    if (frame >= 48 && frame <= 96) {
      s32 pose = (frame - 48) / 24 % 3;

      Sprite_StartAnimationFar(
          work->sprites[3], gCybele_ObjectPoses[pose][CYBELE_POSE_FACE_LEFT]);
      Sprite_StartAnimationFar(
          work->sprites[4], gCybele_ObjectPoses[pose][CYBELE_POSE_FACE_RIGHT]);
    }

    // Frames 72 to 127: thorn i flies from frame 72 + i until it reaches
    // the ground at y 104, its picture changing every 4 frames.
    if (frame >= 72 && frame <= 127) {
      for (i = 0; i != 16; i++) {
        struct Particle *thorn = &work->particles[CYBELE_FIRST_THORN + i];

        if (frame >= i + 72 && thorn->y < px(104)) {
          s32 pictureIndex = (frame + i) / 4 % 5;
          s32 x = Q_16_16_TO_INT(thorn->x);
          s32 y = thorn->y >> 16;
          u8 *picture = work->sheet + gCybele_ThornPictures[pictureIndex];
          u32 width = gCybele_ThornWidths[pictureIndex];
          u32 tall;

          x -= width >> 1;
          tall = gCybele_ThornHeights[pictureIndex];
          y -= tall >> 1;
          draw[0](canvas, picture, x, y, width, tall);
          Particle_AdvanceWithGravity2D(thorn, 64, px(1) / 16);
        }
      }
    }
    // Frame 128.
    if (frame == 128) {
      for (i = 0; i != 48; i++) {
        struct Particle *chip = &work->particles[CYBELE_FIRST_CHIP + i];

        chip->x = (Random16() % 96) << 16;
        chip->y = ((Random16() & 7) + 88) << 16;
        chip->velocityX = ((Random16() & 255) - 128) << 11;
        chip->velocityY = -(Random16() & 255) << 11;
        chip->variant = -(Random16() & 15) - 16;
      }
      REG_BG2X = 0;
    }

    // Frames 128 to 224: grow the vines, shaking the screen while they grow.
    phase = frame - 128;
    if (phase >= 0 && phase <= 96) {
      rise = phase;
      if (rise > 80)
        rise = 80;
      else
        work->shakeFrames = 2;
      for (i = 0; i != 10; i++) {
        struct Particle *vine = &work->particles[i];

        if (rise > vine->variant) {
          u8 *sheet = work->sheet;
          s32 top;
          s32 wrap;
          s32 k;

          if (i > 5)
            sheet = work->sheet + CYBELE_THIN_VINE;
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
              draw[i & 1](canvas, sheet + skip + CYBELE_VINE_STEM, vine->x, y,
                          24, rows);
            }
          }
          // A flower, on the odd vines only.
          if ((i & 1) != 0) {
            s32 y = ((vine->y - top) & 127) - 16;
            s32 kind = i % 3;
            s32 full = gCybele_FlowerSizes[kind];
            s32 rows = full;

            if (y + rows > vine->y)
              rows -= y + rows - vine->y;
            if (rows > 0)
              draw[i & 1](canvas, work->sheet + gCybele_FlowerPictures[kind],
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
          s32 pictureIndex = i % 5;

          draw[0](canvas, work->sheet + gCybele_ChipPictures[pictureIndex],
                  Q_16_16_TO_INT(chip->x), Q_16_16_TO_INT(chip->y),
                  gCybele_ChipWidths[pictureIndex],
                  gCybele_ChipHeights[pictureIndex]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->velocityY += px(1) / 4;
          if ((chip->y < 0 || chip->y > px(120)) && frame <= 159) {
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

    // Frames 128 to 300: six sparks keep flashing around each target's body.
    if (phase >= 0 && phase <= 172) {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        struct Actor *actor =
            GetBattleObjectSlotFar(work->args->targets[i])->actor;

        SET_VECTOR(point, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        Projection_Point(point, &screen);
        for (j = 0; j != 6; j++) {
          struct Particle *spark =
              &work->particles[CYBELE_FIRST_SPARK + i * 6 + j];

          if (spark->variant == 0) {
            spark->x = screen.x + (Random16() & 15) - 8;
            spark->y = screen.y + (Random16() & 15) - 40;
          }
          if (spark->variant >= 0 && spark->variant <= 4) {
            u8 *picture = work->sheet + gCybele_SparkPictures[spark->variant];
            u32 size = gCybele_SparkSizes[spark->variant];
            u32 half = size >> 1;

            draw[0](canvas, picture, spark->x - half, spark->y - half, size,
                    size);
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
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        if (frame > i * 8 + 160) {
          struct BattleObject *slot =
              GetBattleObjectSlotFar(work->args->targets[i]);
          struct Actor *actor = slot->actor;
          void *record;
          s32 n;

          actor->y.fixed += px(8);
          if (actor->y.fixed > px(128))
            actor->y.fixed = px(128);
          actor->gravity = 0;
          for (n = 0; (record = GetMotionRecordFar(slot->actor, n)) != 0; n++)
            Sprite_StartAnimationFar(record, POSE_DAMAGE);
        }
      }
    }
    // Drop the targets from 96 pixels, 5 frames apart, and hit each one 16
    // frames later.
    EFFECT_FOR_EACH_TARGET(i, work->args) {
      if (frame == i * 5 + 286) {
        struct Actor *actor =
            GetBattleObjectSlotFar(work->args->targets[i])->actor;

        // BATTLE_UNIT_GRAVITY is the battle units' usual gravity, which the
        // lift cleared (battle/motion.c sets the same).
        actor->y.fixed = px(96);
        actor->gravity = BATTLE_UNIT_GRAVITY;
      }
      if (frame == i * 5 + 302) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
        Sound_PlayFar(SE_HEAVY_IMPACT);
        work->shakeFrames = 8;
      }
    }

    // Frame 302.
    if (frame == 302) {
      EFFECT_RELEASE_BLITTERS();
      Resource_LoadAndDecompress((s32)&ResourceId_YellowRingSheet, work->sheet,
                                 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                                 work->sheet + CYBELE_BLAST_PICTURES, 1, 1);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
      REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
                   BLDCNT_TGT2_ALL;
      REG_BG2PA = Q_8_8(0.5);
      REG_BG2X = 0;
      EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                            CANVAS_FADE_THREE_QUARTERS);
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        struct Actor *actor =
            GetBattleObjectSlotFar(work->args->targets[i])->actor;

        for (j = 0; j != 10; j++) {
          struct Particle *dust = &work->particles[i * 10 + j];

          SET_POSITION(dust, actor->x.fixed, px(20), actor->z.fixed);
          SET_VELOCITY(dust, (Trig_Sin(j * 0x3334) * 4),
                       (Random16() & 0x7fff) + px(1),
                       (Trig_Cos(j * 0x3334) * 4));
          dust->variant = 0;
        }
      }
    }
    // Target i's ring spreads from frame 302 + 4i.
    if (frame > 301) {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        s32 start = i * 4 + 302;

        if (frame >= start && frame < i * 4 + 314) {
          s32 pictureIndex = (frame - start) / 2;
          struct Actor *actor =
              GetBattleObjectSlotFar(work->args->targets[i])->actor;

          SET_VECTOR(point, actor->x.fixed, 0, actor->z.fixed);
          Projection_Point(point, &screen);
          screen.x >>= 1;
          draw[0](canvas, work->sheet + pictureIndex * (20 * 24), screen.x - 20,
                  screen.y - 24, 20, 24);
          draw[1](canvas, work->sheet + pictureIndex * (20 * 24), screen.x,
                  screen.y - 24, 20, 24);
        }
        if (frame >= start + 6) {
          for (j = 0; j != 5; j++) {
            struct Particle *dust = &work->particles[i * 10 + j];

            Projection_Point(&dust->x, &screen);
            screen.x >>= 1;
            if (dust->variant >= 0 && dust->variant <= 26) {
              s32 pictureIndex = CYBELE_DUST_PICTURE;
              s32 offset = gCybele_BlastPictures[pictureIndex];
              u32 size = gCybele_BlastSizes[pictureIndex];

              draw[0](canvas, work->sheet + offset, screen.x - (size >> 1),
                      screen.y - (size >> 1), size, size);
            }
            Particle_AdvanceWithGravity2D(dust, 60, px(1) / 16);
            dust->variant++;
          }
        }
      }
    }

    // The camera shake, by phase: Cybele's landing and the thorns, the vines
    // and the dissolve, the landings.
    if (frame <= 127)
      BattleCamera_Shake(4, 16);
    else if (frame <= 301)
      BattleCamera_Shake(2, 2);
    else
      BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(work);
  }

  // The end: the impact burst builds its own blitters and starts its own
  // transfer, so the effect's are released first.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  BattleEffect_RunImpactBurst(SUMMON_FORMATION_CYBELE, px(128), height);
  for (i = 0; i != 8; i++)
    Sprite_DestroyFar(work->sprites[i]);
  BattleEffect_EndCanvasLayer();
}

#undef CYBELE_FIRST_SPARK
#undef CYBELE_FIRST_THORN
#undef CYBELE_FIRST_CHIP
#undef CYBELE_DUST_PICTURE
#undef CYBELE_BLAST_PICTURES
