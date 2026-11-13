/// @file
/// The Cybele summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"
#include "macros.h"

#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "battle/battle.h"
#include "battle_display.h"
#include "gamepak/far_call/battle.h"
#include "graphics/graphics.h"
#include "module/battle_effect/battle_effect.h"
#include "module/field/object.h"
#include "module/system/system.h"
#include "gamepak/far_call/field.h"
#include "graphics/render_placement.h"
#include "module/window/screens.h"
#include "resource_ids.h"
#include "system/scene.h"
#include "system/system.h"

struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

struct DescriptorEntry {
  s16 id;
  u8 unk02[6];
};

extern struct DescriptorEntry gData_080eff28[];

void Shop_InitializeWork(void);
s32 Inn_RoomPrice(s32 innId);
void Shop_ReleaseWork(void);

void BattleEffect_InitializeTilemapAndFlushQueue(void) {
  BattleEffect_StartTiledCanvas(1);
  BattleEffect_EndCanvasLayer();
}

#include "fixed_math.h"
#include "system/task.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "battle/motobj.h"
#include "system/transform.h"
#include "battle/battle_effect.h"
#include "battle_effect_work.h"
#include "graphics/resource.h"

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

#define FORMATION_THORN_VINES 645 // eight objects

static __inline__ void BurstAt_(s32 formation, s32 x, s32 y) {
  BattleEffect_RunImpactBurst(formation, x, y);
}

// Summon 5, the thorn vines:thorns fly in, vines grow up under the targets
// and lift them while the ground throws up debris; the picture is erased
// dot by dot, the targets are dropped from on high, and each lands in an
// explosion with a ring of embers. A or B skips to the drop.
void BattleEffect_RunCybele(struct BattleEffectArgument *effect) {
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
  struct EffectPosition screen;
  DrawRectangle draw[2];

  // Set up: the formation's eight objects, then the order in which the
  // picture is erased: eight shuffled runs of 0..127.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
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
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_ThornVineSheet, work->sheet, 1,
                             1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_TRANSPARENT << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_BG2X = -0x2000;

  // The vines (particles 0-15): x, the root's y, growth speed and delay.
  for (i = 0; i != 16; i++) {
    struct EffectStep *vine = &work->particles[i];

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
    struct EffectStep *spark = &work->particles[16 + i];

    spark->x = (Random16() & 63) + 32;
    spark->y = (Random16() & 31) + 64;
    spark->variant = -(Random16() & 7) - 8;
  }
  // The thorns (32-47) fly in from the upper right.
  for (i = 0; i != 16; i++) {
    struct EffectStep *thorn = &work->particles[32 + i];

    thorn->x = 128 << 16;
    thorn->y = 64 << 16;
    thorn->velocityX = -((Random16() & 255) + 200) << 9;
    thorn->velocityY = 0;
    thorn->variant = 0;
  }
  BattleEffect_SelectLivingTargets(work->effect);

  height = -(64 << 16);
  speed = 0;
  for (frame = 0; frame != 366; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // A or B skips the vines to the drop.
    if ((gInput.repeat & (KEY_A | KEY_B)) != 0 && frame > 190 && frame <= 285) {
      Iwram_ClearWordsEntry(canvas, CANVAS_BYTES);
      frame = 286;
    }
    if (frame == 224)
      work->transferMode = 0;
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame == 31) {
      work->shakeFrames = 8;
      Sound_PlayFar(SE_GATE_MOVE);
      for (i = 0; i != effect->count; i++)
        BattleMotion_ApplyKnockbackFar(effect->actors[i], 6);
    }
    if (frame == 72)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 140)
      Sound_PlayFar(SE_SWITCH_MECHANISM);

    // Drop the formation's objects to 64 pixels up.
    speed += 0x4000;
    height += speed;
    if (height > 64 << 16)
      height = 64 << 16;
    BattleEffect_PlaceFormationObjects(2, 128 << 16, height);
    if (frame >= 48 && frame <= 96) {
      s32 pose = (frame - 48) / 24 % 3;

      /* FAKEMATCH(temporary): the left pose copied into left before the call's
       * other argument is read. */
      {
        u8 left = gCybele_ObjectPoses[pose * 2];

        Animation_ApplyChildArgumentFar(work->objects[3], left);
      }
      Animation_ApplyChildArgumentFar(work->objects[4],
                                      gCybele_ObjectPoses[pose * 2 + 1]);
    }

    // The thorns fly in, one a frame.
    if (frame >= 72 && frame <= 127) {
      for (i = 0; i != 16; i++) {
        struct EffectStep *thorn = &work->particles[32 + i];

        if (frame >= i + 72 && thorn->y < (104 << 16)) {
          s32 cel = (frame + i) / 4 % 5;
          s32 x = HI(thorn->x);
          s32 y = thorn->y >> 16;
          u8 *cell = work->sheet + gCybele_ThornPictures[cel];
          u32 width = Data_0819820c[cel];
          u32 tall;

          x -= width >> 1;
          tall = Data_08198211[cel];
          y -= tall >> 1;
          draw[0](canvas, cell, x, y, width, tall);
          BattleEffectKernels_IntegrateVector2(thorn, 64, 0x1000);
        }
      }
    }
    // Debris (16-63) bursts up from the ground.
    if (frame == 128) {
      for (i = 0; i != 48; i++) {
        struct EffectStep *chip = &work->particles[16 + i];

        chip->x = (Random16() % 96) << 16;
        chip->y = ((Random16() & 7) + 88) << 16;
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
        struct EffectStep *vine = &work->particles[i];

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
    // Debris that falls or leaves the screen bursts up again until frame
    // 159.
    if (phase >= 0 && phase <= 95) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *chip = &work->particles[32 + i];

        if (chip->variant >= 0) {
          s32 cel = i % 5;

          draw[0](canvas, work->sheet + gCybele_ChipPictures[cel], HI(chip->x),
                  HI(chip->y), gCybele_ChipWidths[cel],
                  gCybele_ChipHeights[cel]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->velocityY += px(1) / 4;
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

    // Every four frames, step the 64 BG colours one shade toward grey.
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
        *color++ = (blue << 10) | (green << 5) | red;
      }
    }

    // Six glints flash in turn around each target.
    if (phase >= 0 && phase <= 172) {
      for (i = 0; i != effect->count; i++) {
        struct MotionObject *object =
            BattleObject_GetFar(effect->actors[i])->object;

        point[0] = object->x;
        point[1] = object->y;
        point[2] = object->z;
        EffectPosition_ProjectPoint(point, &screen);
        for (j = 0; j != 6; j++) {
          struct EffectStep *spark = &work->particles[16 + i * 6 + j];

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
      for (i = 0; i != effect->count; i++) {
        if (frame > i * 8 + 160) {
          struct BattleObj *slot = BattleObject_GetFar(effect->actors[i]);
          struct MotionObject *object = slot->object;
          void *record;
          s32 n;

          object->y += 8 << 16;
          if (object->y > 128 << 16)
            object->y = 128 << 16;
          object->verticalMotionStrength = 0;
          for (n = 0; (record = GetMotionRecordFar(slot->object, n)) != 0; n++)
            Animation_ApplyChildArgumentFar(record, 5);
        }
      }
    }
    // Drop the targets from 96 pixels, five frames apart, and strike each
    // 16 frames later.
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 5 + 286) {
        struct MotionObject *object =
            BattleObject_GetFar(effect->actors[i])->object;

        object->y = 96 << 16;
        object->verticalMotionStrength = 0xab85;
      }
      if (frame == i * 5 + 302) {
        BattleEffect_TintBattler(effect->actors[i], 7, -1, i, 8);
        Sound_PlayFar(SE_HEAVY_IMPACT);
        work->shakeFrames = 8;
      }
    }

    // Switch to the explosion: the blast sheet, max blitters, and ten
    // embers 72 degrees apart around each target.
    if (frame == 302) {
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
      Resource_LoadAndDecompress((s32)&ResourceId_YellowRingSheet, work->sheet,
                                 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                                 work->sheet + 0x1680, 1, 1);
      BattleEffect_LoadBlitter(
          HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                   COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
      draw[1] = (DrawRectangle) * (Ram_HeapSlots->unkb8 + 1);
      REG_BLDCNT = 0x3f46;
      REG_BG2PA = 0x80;
      REG_BG2X = 0;
      work->transferMode = 2;
      work->transferValue = 75;
      Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
      for (i = 0; i != effect->count; i++) {
        struct MotionObject *object =
            BattleObject_GetFar(effect->actors[i])->object;

        for (j = 0; j != 10; j++) {
          struct EffectStep *ember = &work->particles[i * 10 + j];

          SET_POSITION(ember, object->x, 20 << 16, object->z);
          ember->velocityX = Trig_Sin(j * 0x3334) << 2;
          ember->velocityY = (Random16() & 0x7fff) + (1 << 16);
          ember->velocityZ = Trig_Cos(j * 0x3334) << 2;
          ember->variant = 0;
        }
      }
    }
    // The explosions (40x24 in two mirrored halves, six cels) and the five
    // embers after them.
    if (frame > 301) {
      for (i = 0; i != effect->count; i++) {
        s32 start = i * 4 + 302;

        if (frame >= start && frame < i * 4 + 314) {
          s32 cel = (frame - start) / 2;
          struct MotionObject *object =
              BattleObject_GetFar(effect->actors[i])->object;

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
            struct EffectStep *ember = &work->particles[i * 10 + j];

            EffectPosition_ProjectPoint(&ember->x, &screen);
            screen.x >>= 1;
            if (ember->variant >= 0 && ember->variant <= 26) {
              s32 cel = 6;
              u8 *source = work->sheet + gCybele_EmberOffsets[cel];
              u32 size = gCybele_BlastSizes[cel];

              draw[0](canvas, source, screen.x - (size >> 1),
                      screen.y - (size >> 1), size, size);
            }
            BattleEffectKernels_IntegrateVector2(ember, 60, 0x1000);
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
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattlePlayback_StartFar(134);
  // Tear down.
  /* FAKEMATCH(wrapper): the burst through BurstAt_. */
  BurstAt_(2, 128 << 16, height);
  for (i = 0; i != 8; i++)
    Sprite_DestroyFar(work->objects[i]);
  BattleEffect_EndCanvasLayer();
}

#include "bg_scroll.h"

#define FORMATION_ICE_BLOCKS 643 // nine objects in a 3x3 block

// Rocks, spray and puffs live in the map cell buffer; the spray's trails
// follow the first 128 particles.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)
#define TRAILS ((struct EffectStep *)(Ram_MapCellBuffer + 0xe00))

// An object's sprite attributes, as far as this scene sets them.
struct SpriteAttributes {
  u8 unk00[9];
  u8 tileHi : 2;
  u8 priority : 2;
  u8 palette : 4;
};
