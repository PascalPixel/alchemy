/// @file
/// The Cybele summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#include "gamepak/far_call/battle.h"
#include "field/object.h"
#include "system/system_bank.h"
#include "gamepak/far_call/field.h"
#include "window/screens.h"
#include "resource_ids.h"

/// The menu work as Cybele reads it: its busy flag.
struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

/// Writes the canvas's tile map for BG2, then closes the canvas layer.
void BattleEffect_InitializeTilemapAndFlushQueue(void) {
  BattleEffect_StartTiledCanvas(1);
  BattleEffect_EndCanvasLayer();
}

#define FORMATION_THORN_VINES 645 ///< eight objects

/// Runs the impact burst at (x, y) in a formation.
static __inline__ void BurstAt_(s32 formation, s32 x, s32 y) {
  BattleEffect_RunImpactBurst(formation, x, y);
}

/// Where BlastSheet is decoded in the effect's pictures.
#define CYBELE_BLAST_PICTURES 0x1680

/// Cybele drops in at the right and thorns fly from her mouth; flowering
/// vines grow out of the ground, lifting the targets while sparks flash around
/// them; then the picture greys and dissolves pixel by pixel, and the targets
/// drop, each landing on a spreading ring in a cloud of dust. A or B skips to
/// the drop.
///
/// Abilities: Cybele.
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
  DrawRectangle draw[2];

  // Set up: the formation's eight objects, then the order in which the
  // picture is erased: eight shuffled runs of 0..127.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_SpawnObjects(8, FORMATION_THORN_VINES, 1);

  order = Ram_MapCellBuffer;
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

  // The canvas, unscaled and moved 32 pixels left, the vine sheet and two
  // transparent blitters, the second mirrored.
  EFFECT_START_NARROW_CANVAS(0);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_ThornVineSheet, work->sheet, 1,
                             1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  REG_BG2X = -0x2000;

  // The vines (particles 0-15): x, the root's y, growth speed and delay.
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
  // The glints (16-31): a negative variant is the wait before each shows.
  for (i = 0; i != 16; i++) {
    struct Particle *spark = &work->particles[16 + i];

    spark->x = (Random16() & 63) + 32;
    spark->y = (Random16() & 31) + 64;
    spark->variant = -(Random16() & 7) - 8;
  }
  // The thorns (32-47) fly in from the upper right.
  for (i = 0; i != 16; i++) {
    struct Particle *thorn = &work->particles[32 + i];

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
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // A or B skips the vines to the drop.
    if ((gInput.repeat & (KEY_A | KEY_B)) != 0 && frame > 190 && frame <= 285) {
      Iwram_ClearWordsEntry(canvas, CANVAS_BYTES);
      frame = 286;
    }
    if (frame == 224)
      work->transferMode = CANVAS_COPY;
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->focus);
    if (frame == 31) {
      work->shakeFrames = 8;
      Sound_PlayFar(SE_GATE_MOVE);
      for (i = 0; i != args->targetCount; i++)
        BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_BOUNCE);
    }
    if (frame == 72)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 140)
      Sound_PlayFar(SE_SWITCH_MECHANISM);

    // Drop the formation's objects to 64 pixels up.
    speed += px(1) / 4;
    height += speed;
    if (height > px(64))
      height = px(64);
    BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_CYBELE, px(128),
                                       height);
    if (frame >= 48 && frame <= 96) {
      s32 pose = (frame - 48) / 24 % 3;

      /* FAKEMATCH(temporary): the left pose copied into left before the call's
       * other argument is read. */
      {
        u8 left = gCybele_ObjectPoses[pose * 2];

        Sprite_StartAnimationFar(work->sprites[3], left);
      }
      Sprite_StartAnimationFar(work->sprites[4],
                               gCybele_ObjectPoses[pose * 2 + 1]);
    }

    // The thorns fly in, one a frame.
    if (frame >= 72 && frame <= 127) {
      for (i = 0; i != 16; i++) {
        struct Particle *thorn = &work->particles[32 + i];

        if (frame >= i + 72 && thorn->y < px(104)) {
          s32 cel = (frame + i) / 4 % 5;
          s32 x = Q_16_16_UPPER_HALF(thorn->x);
          s32 y = Q_16_16_TO_INT(thorn->y);
          u8 *cell = work->sheet + gCybele_ThornPictures[cel];
          u32 width = gCybele_ThornWidths[cel];
          u32 tall;

          x -= width >> 1;
          tall = gCybele_ThornHeights[cel];
          y -= tall >> 1;
          draw[0](canvas, cell, x, y, width, tall);
          BattleEffectKernels_IntegrateVector2(thorn, 64, px(1) / 16);
        }
      }
    }
    // Debris (16-63) bursts up from the ground.
    if (frame == 128) {
      for (i = 0; i != 48; i++) {
        struct Particle *chip = &work->particles[16 + i];

        chip->x = px(Random16() % 96);
        chip->y = px((Random16() & 7) + 88);
        chip->velocityX = ((Random16() & 255) - 128) << 11;
        chip->velocityY = -(Random16() & 255) << 11;
        chip->variant = -(Random16() & 15) - 16;
      }
      REG_BG2X = 0;
    }

    // The vines grow (shaking the screen while they do): a tip (24x8),
    // three 24x64 stems cut at the top of the screen and at the root, and
    // leaves on the odd vines.
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
            sheet = work->sheet + 0x6c0;
          top = (rise - vine->variant) * vine->velocityY;
          for (wrap = top; wrap > 184; wrap -= 64)
            ;
          if (wrap <= 119)
            draw[i & 1](canvas, sheet, vine->x, vine->y - wrap - 8, 24, 8);
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
    // Debris that falls or leaves the screen bursts up again until frame
    // 159.
    if (phase >= 0 && phase <= 95) {
      for (i = 0; i != 32; i++) {
        struct Particle *chip = &work->particles[32 + i];

        if (chip->variant >= 0) {
          s32 cel = i % 5;

          draw[0](canvas, work->sheet + gCybele_ChipPictures[cel],
                  Q_16_16_UPPER_HALF(chip->x), Q_16_16_UPPER_HALF(chip->y),
                  gCybele_ChipWidths[cel], gCybele_ChipHeights[cel]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->velocityY += px(1) / 4;
          if ((chip->y < 0 || chip->y > 120 << 16) && frame <= 159) {
            chip->x = px(Random16() % 96);
            chip->y = px((Random16() & 7) + 88);
            chip->velocityX = ((Random16() & 255) - 128) << 11;
            chip->velocityY = -(Random16() & 255) << 11;
          }
        }
        chip->variant++;
      }
    }

    // Every four frames, step the 64 BG colours one shade toward grey.
    if (frame >= 224 && frame <= 247 && (frame & 3) == 0) {
      u16 *color = (u16 *)BG_PLTT;

      for (i = 0; i != 64; i++) {
        s32 red = RGB_RED(*color);
        s32 green = RGB_GREEN(*color);
        s32 blue = RGB_BLUE(*color);
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

    // Six glints flash in turn around each target.
    if (phase >= 0 && phase <= 172) {
      for (i = 0; i != args->targetCount; i++) {
        struct MotionObject *object =
            BattleObject_GetFar(args->targets[i])->object;

        point[0] = object->x;
        point[1] = object->y;
        point[2] = object->z;
        EffectPosition_ProjectPoint(point, &screen);
        for (j = 0; j != 6; j++) {
          struct Particle *spark = &work->particles[16 + i * 6 + j];

          if (spark->variant == 0) {
            spark->x = screen.x + (Random16() & 15) - 8;
            spark->y = screen.y + (Random16() & 15) - 40;
          }
          if (spark->variant >= 0 && spark->variant <= 4) {
            u8 *cell = work->sheet + gCybele_SparkPictures[spark->variant];
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

    // Erase the 128x128 picture two lines a frame, each line's dots in the
    // shuffled order.
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

    // Lift the targets to 128 pixels, eight frames apart.
    if (frame >= 161 && frame <= 223) {
      for (i = 0; i != args->targetCount; i++) {
        if (frame > i * 8 + 160) {
          struct BattleObject *slot = BattleObject_GetFar(args->targets[i]);
          struct MotionObject *object = slot->object;
          void *record;
          s32 n;

          object->y += px(8);
          if (object->y > px(128))
            object->y = px(128);
          object->verticalMotionStrength = 0;
          for (n = 0; (record = GetMotionRecordFar(slot->object, n)) != 0; n++)
            Sprite_StartAnimationFar(record, 5);
        }
      }
    }
    // Drop the targets from 96 pixels, five frames apart, and strike each
    // 16 frames later.
    for (i = 0; i != args->targetCount; i++) {
      if (frame == i * 5 + 286) {
        struct MotionObject *object =
            BattleObject_GetFar(args->targets[i])->object;

        object->y = px(96);
        object->verticalMotionStrength = 0xab85;
      }
      if (frame == i * 5 + 302) {
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
        Sound_PlayFar(SE_HEAVY_IMPACT);
        work->shakeFrames = 8;
      }
    }

    // Switch to the explosion: the blast sheet, max blitters, and ten
    // embers 72 degrees apart around each target.
    if (frame == 302) {
      EFFECT_RELEASE_BLITTERS();
      Resource_LoadAndDecompress((s32)&ResourceId_YellowRingSheet, work->sheet,
                                 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                                 work->sheet + CYBELE_BLAST_PICTURES, 1, 1);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
      draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
      // ramCodePaired, reached from the slot before it.
      draw[1] = (DrawRectangle) * (&Ram_HeapSlots->unkb8 + 1);
      REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
                   BLDCNT_TGT2_ALL;
      REG_BG2PA = Q_8_8(0.5);
      REG_BG2X = 0;
      EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                            CANVAS_FADE_THREE_QUARTERS);
      for (i = 0; i != args->targetCount; i++) {
        struct MotionObject *object =
            BattleObject_GetFar(args->targets[i])->object;

        for (j = 0; j != 10; j++) {
          struct Particle *ember = &work->particles[i * 10 + j];

          SET_POSITION(ember, object->x, px(20), object->z);
          ember->velocityX = Trig_Sin(j * 0x3334) << 2;
          ember->velocityY = (Random16() & 0x7fff) + px(1);
          ember->velocityZ = Trig_Cos(j * 0x3334) << 2;
          ember->variant = 0;
        }
      }
    }
    // The explosions (40x24 in two mirrored halves, six cels) and the five
    // embers after them.
    if (frame > 301) {
      for (i = 0; i != args->targetCount; i++) {
        s32 start = i * 4 + 302;

        if (frame >= start && frame < i * 4 + 314) {
          s32 cel = (frame - start) / 2;
          struct MotionObject *object =
              BattleObject_GetFar(args->targets[i])->object;

          point[0] = object->x;
          point[1] = 0;
          point[2] = object->z;
          EffectPosition_ProjectPoint(point, &screen);
          screen.x >>= 1;
          draw[0](canvas, work->sheet + cel * 480, screen.x - 20, screen.y - 24,
                  20, 24);
          draw[1](canvas, work->sheet + cel * 480, screen.x, screen.y - 24, 20,
                  24);
        }
        if (frame >= start + 6) {
          for (j = 0; j != 5; j++) {
            struct Particle *ember = &work->particles[i * 10 + j];

            EffectPosition_ProjectPoint(&ember->x, &screen);
            screen.x >>= 1;
            if (ember->variant >= 0 && ember->variant <= 26) {
              s32 cel = 6;
              u8 *source = work->sheet + gCybele_BlastPictures[cel];
              u32 size = gCybele_BlastSizes[cel];

              draw[0](canvas, source, screen.x - (size >> 1),
                      screen.y - (size >> 1), size, size);
            }
            BattleEffectKernels_IntegrateVector2(ember, 60, px(1) / 16);
            ember->variant++;
          }
        }
      }
    }

    if (frame <= 127)
      BattleCamera_Shake(4, 16);
    else if (frame <= 301)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  // Tear down.
  /* FAKEMATCH(wrapper): the burst through BurstAt_. */
  BurstAt_(2, px(128), height);
  for (i = 0; i != 8; i++)
    Sprite_DestroyFar(work->sprites[i]);
  BattleEffect_EndCanvasLayer();
}

#define FORMATION_ICE_BLOCKS 643 ///< nine objects in a 3x3 block

/// Rocks, spray and puffs live in the map cell buffer; the spray's trails
/// follow the first 128 particles.
#define PARTICLES ((struct Particle *)Ram_MapCellBuffer)
#define TRAILS ((struct Particle *)(Ram_MapCellBuffer + 0xe00))

/// An object's sprite attributes, as far as this scene sets them.
struct SpriteAttributes {
  u8 unk00[9];
  u8 tileHi : 2;
  u8 priority : 2;
  u8 palette : 4;
};
