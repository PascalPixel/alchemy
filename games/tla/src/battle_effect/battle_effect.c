/// @file
/// The battle effect object, the bank's C in one: the display and HBlank
/// helpers, the canvas layer, the dispatch that runs summon and table effects,
/// and the effects from Ice and Douse to the battler tints, as The Broken
/// Seal's battle_effect.c holds them. battle_effect_internal.h holds the
/// includes every part shares; the parts under battle_effect/ follow in ROM
/// order, then the effects from Cybele to Bolt, the ability effects
/// (ability_effects/), the Liquifier and the summon effects (summon_effects/),
/// each as it stood alone: where two of them name a type or routine otherwise,
/// the later one sees its own through a view.

#include "battle_effect_internal.h"

// The parts, in ROM order.
#include "battle_effect/display.c"
#include "battle_effect/falling.c"
#include "battle_effect/streaks.c"
#include "battle_effect/target_strike.c"
#include "battle_effect/death_plunge.c"
#include "battle_effect/canvas.c"
#include "battle_effect/planet_diver.c"
#include "battle_effect/mirrored_wall.c"
#include "battle_effect/condemn.c"
#include "battle_effect/bind.c"
#include "battle_effect/stars.c"
#include "battle_effect/procne.c"
#include "battle_effect/nereid.c"
#include "battle_effect/vortices.c"
#include "battle_effect/prism.c"
#include "battle_effect/plasma.c"
#include "battle_effect/volcano.c"
#include "battle_effect/target_motes.c"
#include "battle_effect/djinn_heal.c"
#include "battle_effect/dispatch.c"
#include "battle_effect/battler_tint.c"

#undef PHASE_RESULTS
#undef SHARD_COUNT
#undef DROP_COUNT
#undef EFFECT_STEPS
#undef PARTICLES
#undef MOTES
#undef BOLT_COUNT
#undef STAR_COUNT
#undef FORMATION_SIZE
#undef COLUMN_WIDTH
#undef COLUMN_HEIGHT
#undef MOTE_COUNT
#undef PILLAR_FRAMES

#undef EffectSprite_CreateWithTiles
#undef EffectSprite_Acquire
#undef Render_DrawModelList
#undef PHASE_RESULTS
#undef SHARD_COUNT
#undef DROP_COUNT
#undef EFFECT_STEPS
#undef EffectSprite_Acquire
#undef EffectSprite_CreateWithTiles
#undef SLOT_ALT
#undef OBJ_TILES
#undef CLIP
#undef PARTICLES
#undef MOTES
#undef RECORD_FRAMES
#undef BOLT_COUNT
#undef STAR_COUNT
#undef FORMATION_SIZE
#undef COLUMN_WIDTH
#undef COLUMN_HEIGHT
#undef HEAP_BLITTER_ALTERNATE
#undef MOTE_COUNT
#undef SPARK_COUNT
#undef PILLAR_FRAMES
#undef HEAP_BLITTER
#undef Render_DrawModelList
#undef SLOT_BLITTER
#undef BLITTER
#undef HI

// ---- battle_effect/cybele.c ----
#define FxGroup FxGroup__cybele

/// The effects from the Cybele summon to Bolt (Boreas, Break, Drain, Cure
/// Poison, Flare, Quake, Fire, Frost, Ray, Atalanta, Growth, Vine, Thorn and
/// the rest).

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "dma.h"
#include "io_reg.h"
#include "battle.h"
#include "battle_display.h"
#include "gamepak/far_call/battle.h"
#include "graphics.h"
#include "module/battle_effect/battle_effect.h"
#include "module/field/object.h"
#include "module/system/system.h"
#include "gamepak/far_call/field.h"
#include "render_placement.h"
#include "module/window/screens.h"
#include "resource_ids.h"
#include "scene.h"
#include "system.h"

struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

/* VIEW(cybele): tag struct FxGroup as struct FxGroup__cybele;
 * battle_effect/battle_effect has another definition; not tried: struct
 * FxGroup__cybele was not found. */
struct FxGroup {
  u8 unk00[32];
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
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

#include "system.h"
#include "fixed_math.h"
#include "task.h"
#include "input.h"
#include "iwram_call.h"
#include "motobj.h"
#include "transform.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "resource.h"

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
    struct EffectCamera *camera = Ram_HeapSlots->cameraWork;

    // A or B skips the vines to the drop.
    if ((gInput.repeat & 3) != 0 && frame > 190 && frame <= 285) {
      Iwram_ClearWordsEntry(canvas, 0x4000);
      frame = 286;
    }
    if (frame == 224)
      work->transferMode = 0;
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame == 31) {
      work->shakeFrames = 8;
      Sound_PlayFar(157);
      for (i = 0; i != effect->count; i++)
        BattleMotion_ApplyKnockbackFar(effect->actors[i], 6);
    }
    if (frame == 72)
      Sound_PlayFar(136);
    if (frame == 140)
      Sound_PlayFar(156);

    // Drop the formation's objects to 64 pixels up.
    speed += 0x4000;
    height += speed;
    if (height > 64 << 16)
      height = 64 << 16;
    BattleEffect_PlaceFormationObjects(2, 128 << 16, height);
    if (frame >= 48 && frame <= 96) {
      s32 pose = (frame - 48) / 24 % 3;

      /* FAKEMATCH(temporary): the left pose copied into left before the
       * call's other argument is read; as a direct argument, 2 of 1490
       * instructions differ in tla-en. */
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

    // Every four frames, step the 64 BG colours one shade toward grey.
    if (frame >= 224 && frame <= 247 && (frame & 3) == 0) {
      u16 *color = (u16 *)0x05000000;

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
        Sound_PlayFar(134);
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
      // As Ram_HeapSlots->unkb8[1], 267 of 1490 instructions differ.
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

          ember->x = object->x;
          ember->y = 20 << 16;
          ember->z = object->z;
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
  /* FAKEMATCH(wrapper): the burst through BurstAt_, whose parameter sets 128 <<
   * 16 in argument order; as a direct BattleEffect_RunImpactBurst call, 2 of
   * 1490 instructions differ in tla-en. */
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

// Summon 10: a window opens over a violet sky on a field of rocks; a 3x3
// block of ice rises, a great ice block falls into it and throws up debris
// and spray (A or B skips to the second half). Then ice blocks rain on the
// targets, boulders pile up and burst, and their pieces bounce on the
// ground among puffs of smoke.
void BattleEffect_RunBoreas(struct BattleEffectArgument *effect) {
  s32 pts[32][2];
  s32 pos[4];
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle draw;
  DrawRectangle drawAlt;
  u8 *dots;
  s32 step;
  s32 row;
  s32 count;
  s32 i;
  s32 j;
  s32 ox;
  s32 oy;
  s32 y;
  s32 t;
  struct EffectStep *p;
  struct EffectStep *q;
  struct Scale scale;

  // Set up: a blank canvas over a black backdrop, the ice sheet and the
  // sparkle dots, a max and a transparent blitter, the violet sky and the
  // nine ice objects.
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  dots = Ram_HeapSlots->unk64[0];
  step = 1;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  *(u16 *)0x05000000 = 0;
  *(u16 *)0x05000002 = 0;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_WININ = 0x2137;
  BattleEffect_WipeCanvas(1, 0);
  REG_WIN0H = 0xf0f0;
  Resource_LoadAndDecompress((s32)&ResourceId_IceBlockSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, dots, 0, 0);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE, BLITTER_TRANSPARENT << 4 |
                                                       COMPOSITE_CLIP_X |
                                                       COMPOSITE_CLIP_Y);
  drawAlt = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);
  wait(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  BattleEffect_SpawnObjects(9, FORMATION_ICE_BLOCKS, 1);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  work->transferMode = 2;
  work->transferValue = 50;

  for (i = 0; i != 64; i++) {
    PARTICLES[i].x = (Random16() & 63) + 32;
    PARTICLES[i].y = (Random16() & 31) + 120;
    PARTICLES[i].variant = -1;
  }

  // A working copy of the rock points (the scene draws from the table).
  for (i = 0; i != 32; i++) {
    pts[i][0] = gBoreas_Points[i][0];
    pts[i][1] = gBoreas_Points[i][1];
  }

  Sound_PlayFar(141);

  // The first half: rocks, the great ice block and its spray.
  for (frame = 0; frame != 288; frame++) {
    count = 16;
    if ((gInput.repeat & 3) && frame > 16)
      break;

    // Open the window from the left: its right edge speeds up, and each
    // line is pulled in by up to 63 pixels.
    if (frame >= 0 && frame < 16) {
      u16 *edge = (u16 *)Ram_MapCellBuffer;

      if (frame == 1) {
        u8 *inset = Ram_MapCellBuffer + 2;
        s32 n;

        for (n = 0; n != 128; n++)
          inset[n] = Random16() & 63;
        *edge = 0;
        Task_Add(BattleEffect_BuildWindowEdgeTable, TASK_PASS_FRAME);
      }
      *edge += step;
      step += 3;
      if (frame == 15)
        Task_Remove(BattleEffect_BuildWindowEdgeTable);
    }

    // The rocks still standing.
    if (frame > 103)
      count = 0;
    else if (frame > 63)
      count = 6;
    else if (frame > 31)
      count = 10;

    // The ground trembles until frame 167; at 176-179 it jolts as the
    // table says.
    if (frame <= 167) {
      ox = (Random16() & 3) - 1;
      oy = (Random16() & 3) - 1;
      gBgScroll[1].x = ox;
      gBgScroll[1].y = oy + 32;
    } else {
      ox = 0;
      oy = 0;
      gBgScroll[1].x = 0;
      gBgScroll[1].y = 32;
    }
    if (frame >= 176 && frame < 180) {
      ox = -gBoreas_ShakeOffsets[frame - 176];
      oy = gBoreas_ShakeOffsets[frame - 176];
      gBgScroll[1].x = ox;
      gBgScroll[1].y = oy + 32;
    }

    for (i = 0; i != count; i++) {
      s32 n = i % 3;

      draw(canvas, work->sheet + gBoreas_CrystalPictures[n],
           gBoreas_Points[i][0] - ox,
           gBoreas_Points[i][1] - gBoreas_CrystalHeights[n] - oy,
           gBoreas_CrystalWidths[n], gBoreas_CrystalHeights[n]);
    }

    // The nine ice objects in a 3x3 block, 32 pixels apart.
    scale = Data_08196e14;
    if (frame == 174) {
      for (i = 0; i != 9; i++) {
        ((struct SpriteAttributes *)work->objects[i])->priority = 0;
      }
    }
    if (frame > 208)
      Animation_ApplyChildArgumentFar(work->objects[2],
                                      gBoreas_Animations[(frame / 4) & 3]);

    scale.x = 0x10000;
    scale.y = 0x10000;
    pos[3] = 0;
    pos[1] = 255 << 16;
    for (row = 0; row != 3; row++) {
      for (j = 0; j != 3; j++) {
        pos[0] = (j * 32 + 144 - ox) << 16;
        pos[2] = (row * 32 + 76 - oy) << 16;
        Render_ApplyProjectedPlacementFar(work->objects[row * 3 + j], pos,
                                          &scale.x, 0);
      }
    }

    // The great ice block (24x48) drops in from the upper right at 160-175
    // and sinks slowly from 208.
    t = frame - 160;
    if (t >= 0 && t < 158) {
      s32 u = frame - 208;
      s32 sx = 80;
      s32 sy = 8;

      if (frame <= 175) {
        sx = 96 - t;
        sy = t * 4 - 56;
      } else if (frame > 208) {
        sy = u / 4 + 8;
      }
      drawAlt(canvas, work->sheet + 0xc46, sx, sy, 24, 48);
    }

    if (frame == 32)
      Sound_PlayFar(134);
    if (frame == 64)
      Sound_PlayFar(134);
    if (frame == 104)
      Sound_PlayFar(134);
    if (frame == 176)
      Sound_PlayFar(134);
    if (frame == 226)
      Sound_PlayFar(145);
    Transform_Reset();

    // Each time rocks crumble, 32 pieces of debris fly up.
    if (frame == 32) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = ((Random16() & 31) + 68) << 16;
        PARTICLES[i].y = ((Random16() & 31) + 8) << 16;
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 127) - 64) << 11;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == 64) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i + 32].x = (Random16() % 48 + 60) << 16;
        PARTICLES[i + 32].y = ((Random16() & 31) + 52) << 16;
        PARTICLES[i + 32].velocityX = ((Random16() & 127) - 63) << 12;
        PARTICLES[i + 32].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i + 32].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == 104) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = ((Random16() & 63) + 52) << 16;
        PARTICLES[i].y = ((Random16() & 31) + 72) << 16;
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }

    if (frame >= 32 && frame < 208) {
      for (i = 0; i != 64; i++) {
        if (PARTICLES[i].variant >= 0) {
          s32 n;

          if (frame > 191)
            n = i % 7 + 4;
          else
            n = i & 3;
          drawAlt(canvas, work->sheet + gBoreas_DebrisCells[n],
                  HI(PARTICLES[i].x), HI(PARTICLES[i].y),
                  gBoreas_DebrisWidths[n], gBoreas_DebrisHeights[n]);
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          PARTICLES[i].velocityY += 0x2000;
        }
      }
    }

    // The spray: 128 drops from (72, 56) down to the left; from 228 each
    // leaves a trail of puffs.
    if (frame > 223) {
      if (frame == 224) {
        for (i = 0; i != 128; i++) {
          PARTICLES[i].x = 72 << 16;
          PARTICLES[i].y = 56 << 16;
          PARTICLES[i].velocityX = (-(Random16() & 127) - 64) << 11;
          PARTICLES[i].velocityY = ((Random16() & 127) + 16) << 11;
          PARTICLES[i].variant = Random16();
        }
      }
      for (i = 0; i != 128; i++) {
        if (frame >= i / 4 + 224) {
          s32 n = i % 3;

          if ((i & 1) == 0) {
            draw(canvas, work->sheet + gBoreas_SprayCells[n],
                 HI(PARTICLES[i].x), HI(PARTICLES[i].y), Data_081982c0[n],
                 Data_081982c3[n]);
          }
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          if ((PARTICLES[i].x >> 16) < -16 || (PARTICLES[i].y >> 16) > 120) {
            PARTICLES[i].x = 72 << 16;
            PARTICLES[i].y = 56 << 16;
          }
          PARTICLES[i].variant++;
        }
      }
      if (frame == 228) {
        for (i = 0; i != 128; i++) {
          p = &PARTICLES[i];
          q = &TRAILS[i];
          q->x = p->x;
          q->y = p->y;
          q->variant = 0;
        }
      }
      for (i = 0, q = TRAILS, p = PARTICLES; i != 128; i++, q++, p++) {
        if (frame >= i + 228) {
          s32 n = q->variant / 2 % 9;
          s32 sz = gBattleEffect_PuffSizes[n];

          draw(canvas, dots + gBattleEffect_PuffCells[n], HI(q->x) - sz / 2,
               HI(q->y) - sz / 2, sz, sz);
          if (++q->variant == 18) {
            q->x = p->x;
            q->y = p->y;
            q->variant = 0;
          }
        }
      }
    }

    work->transferPending = 1;
    wait(1);
  }

  // The second half: ice rains on the targets, boulders pile up and burst.
  for (i = 0; i != 9; i++)
    Sprite_DestroyFar(work->objects[i]);
  BattleEffect_SetupBlendedDisplay();
  REG_BLDALPHA = 0x1010;
  Sound_PlayFar(390);

  for (i = 0; i != 64; i++) {
    work->particles[i].x = ((Random16() & 127) + 64) << 16;
    work->particles[i].z = -(16 << 16);
    work->particles[i].y = (-(Random16() & 127) - 64) << 16;
    work->particles[i].velocityX = (-(Random16() & 63) - 127) << 12;
    work->particles[i].velocityY = ((Random16() & 63) + 127) << 12;
    work->particles[i].variant = 0;
  }
  for (i = 0; i != 64; i++) {
    PARTICLES[i].x = Random16() & 127;
    PARTICLES[i].y = (Random16() & 63) + i / 2;
    PARTICLES[i].variant = -i / 2;
  }

  for (frame = 0; frame != 146; frame++) {
    count = 0;
    if (frame == 96)
      BattlePlayback_StartFar(134);
    if (frame == 16)
      work->shakeFrames = 32;
    // One boulder every two frames, up to 16.
    if (frame > 16) {
      count = (frame - 16) / 2;
      if (count > 16)
        count = 16;
    }
    if (frame >= 9 && frame < 72 && (frame & 3) == 0)
      Sound_PlayFar(132);
    if (frame == 72)
      Sound_PlayFar(145);
    // A block (40x80) falls in from the upper right.
    if (frame > 64)
      draw(canvas, work->sheet + 0x14f9, (64 - frame) * 7 + 88,
           frame * 14 - 992, 40, 80);
    if (frame <= 71) {
      for (i = 0; i != count; i++) {
        s32 n = i % 3;

        draw(canvas, work->sheet + gBoreas_RisingCrystalPictures[n],
             gBoreas_Points[i][0] - 56,
             gBoreas_Points[i][1] - gBoreas_RisingCrystalHeights[n],
             gBoreas_RisingCrystalWidths[n], gBoreas_RisingCrystalHeights[n]);
      }
    }
    // The boulders burst: 64 pieces fly up from where they stood.
    if (frame == 72) {
      for (i = 0; i != 64; i++) {
        work->particles[i].x = (gBoreas_Points[i & 15][0] - 56) << 16;
        work->particles[i].y = gBoreas_Points[i & 15][1] << 16;
        work->particles[i].velocityX = ((Random16() & 127) - 63) << 13;
        work->particles[i].velocityY = (-(Random16() & 31) - 16) << 14;
      }
      if (frame == 72)
        work->shakeFrames = 4;
    }
    // The pieces: those falling fast bounce at a quarter speed at y = 120.
    if (frame > 71) {
      count = 32;
      if (frame != 72)
        count = 64;
      for (i = 0; i != count; i++) {
        q = &work->particles[i];
        y = HI(q->y);
        if (y <= 135) {
          s32 n = i % 3;

          draw(canvas, work->sheet + gBoreas_FallPictures[n], HI(q->x),
               y - Data_081982db[n], Data_081982d8[n], Data_081982db[n]);
          BattleEffectKernels_IntegrateVector2(q, 64, 1 << 16);
          if (HI(q->y) > 120 && q->velocityY > 8 << 16) {
            q->velocityY = -q->velocityY / 4;
            q->y = 120 << 16;
            work->shakeFrames = 1;
          }
        }
      }
    }
    // The falling ice blocks (24x48), sent up again until frame 47.
    if (frame <= 71) {
      for (i = 0; i != 64; i++) {
        draw(canvas, work->sheet, HI(work->particles[i].x) - 12,
             HI(work->particles[i].y) - 24, 24, 48);
        work->particles[i].x += work->particles[i].velocityX;
        work->particles[i].y += work->particles[i].velocityY;
        if ((work->particles[i].y >> 16) > 120 && frame <= 47) {
          work->particles[i].x = ((Random16() & 127) + 64) << 16;
          work->particles[i].y = -(16 << 16);
        }
      }
    }

    for (i = 0; i != effect->count; i++) {
      if (frame == i * 8 + 32)
        BattleEffect_TintBattler(effect->actors[i], 9, 5, -1, 0);
    }

    // Puffs of smoke, sent up again until frame 127.
    if (frame > 72) {
      for (i = 0; i != 64; i++) {
        if (PARTICLES[i].variant >= 0 && PARTICLES[i].variant < 18) {
          s32 n = PARTICLES[i].variant / 2 % 9;
          s32 sz = gBattleEffect_PuffSizes[n];

          draw(canvas, dots + gBattleEffect_PuffCells[n],
               PARTICLES[i].x - sz / 2, PARTICLES[i].y - sz / 2, sz, sz);
        }
        if (++PARTICLES[i].variant == 18 && frame <= 127) {
          PARTICLES[i].x = Random16() & 127;
          PARTICLES[i].y = (Random16() & 63) + (frame - 54) / 2;
          PARTICLES[i].variant = 0;
        }
      }
    }

    if (frame >= 72 && frame < 80)
      BattleCamera_Shake(8, 8);
    else
      BattleCamera_Shake(2, 2);
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// Break's ice shards (effect kind 43).

#include "battle_action.h"

#include "effect_step.h"

#include "graphics.h"

#include "heap_release.h"

#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))

#define SHARDS_PER_MEMBER 128
#define SHARDS ((struct EffectStep *)Ram_MapCellBuffer)

/// Ice shards burst up from each target in turn, 20 frames apart.
/// Setup: 128 shards per target are scattered around the middle of its body
/// and thrown sideways and up. Frame loop: from its cue a target flashes, and
/// its shards fall under one of four gravities until they drop below y = 112.
/// The effect runs 20 frames per target plus 56.
void BattleEffect_RunBreak(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  s32 *camera;
  s32 point[3];
  struct EffectPosition screen;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet),
                       128);
  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];

  for (i = 0; i != 8 * SHARDS_PER_MEMBER; i++)
    SHARDS[i].variant = -1;

  Transform_Reset();
  Transform_SetLookAt(camera, camera + 3);

  // Scatter each target's shards around the middle of its body.
  for (member = 0; member != effect->count; member++) {
    struct MotionObject *object;
    s32 offset;
    s32 height;
    struct EffectStep *shard;
    struct EffectPosition *position;

    offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
    /* FAKEMATCH(temporary): position holds &screen; as
     * EffectPosition_ProjectPoint(point, &screen), 12 of 337 instructions
     * differ in tla-en. */
    position = &screen;
    object = BattleObject_GetFar(effect->actors[member])->object;
    height = BattleUnit_GetEffectHeightFar(effect->actors[member]) / 2;
    point[0] = object->x;
    point[1] = height;
    point[2] = object->z;
    EffectPosition_ProjectPoint(point, position);
    screen.x >>= 1;
    for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
         i != SHARDS_PER_MEMBER; i++) {
      s32 radius;
      s32 angle;
      s32 lift;

      radius = Random16() & 0xff;
      angle = Random16() & 0xffff;
      shard->x = ((Trig_Sin(angle) * radius) >> 7) + (screen.x << 16);
      shard->y = ((Trig_Cos(angle) * radius) >> 3) + (screen.y << 16);
      shard->velocityX = (128 - (Random16() & 0xff)) << 9;
      lift = Random16() & 0xff;
      shard->variant = 0;
      shard->velocityY = (-lift - 128) << 10;
      shard++;
    }
  }

  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->count * 20 + 56; frame++) {
    if (frame == 32)
      BattlePlayback_StartFar(0);
    for (member = 0; member != effect->count; member++) {
      s32 offset;

      offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
      // The target's cue: a chime and a flash.
      if (frame == member * 20) {
        Sound_PlayFar(143);
        BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 20);
      }
      // Its shards fly, and go out once they fall past y = 112.
      if (frame > member * 20) {
        struct EffectStep *shard;

        for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
             i != SHARDS_PER_MEMBER; i++) {
          if (shard->variant >= 0) {
            s32 size;

            size = i % 3 + 1;
            draw(canvas, sheet + gBattleEffect_MoteCells[size - 1],
                 HI(shard->x) - size / 2, HI(shard->y) - size, size, size * 2);
            BattleEffectKernels_IntegrateVector2(shard, 62,
                                                 gDrain_Gravities[i & 3]);
            shard->variant++;
            if (shard->velocityY > 0 && HI(shard->y) > 112)
              shard->variant = -1;
          }
          shard++;
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Drain, Psy Drain (kind 42).
void BattleEffect_RunDrain(u8 *effect) {
  s32 value = *(s32 *)(effect + 24);

  BattleEffect_RunDrainMotes(effect, (u32)(-value | value) >> 31);
}

/// Abilities: True Collide (kind 451).
void BattleEffect_RunTrueCollide(u8 *effect) {
  BattleEffect_RunDrainMotes(effect, 2);
}

#include "battle_display.h"

#define MOTES_PER_MEMBER 128
#define MOTES ((struct EffectStep *)Ram_MapCellBuffer)

/// Motes of light burst out of each target in turn, 20 frames apart, and
/// are drawn into the user, as The Broken Seal's BattleEffect_RunDrain.
/// Setup: each target's 128 motes start at the middle of its body with
/// random velocities; the canvas darkens by 4 a transfer (in mode 2 it
/// trails instead). Frame loop: a target flashes as its motes leave; ten
/// frames after leaving (one frame later for each mote) a mote accelerates
/// toward the middle of the user by 1/256 of the distance a frame and goes
/// out within 16 pixels of it across. The user glows from frame 40 until 20
/// frames before the end.
void BattleEffect_RunDrainMotes(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  struct BattleCamera *camera;
  struct EffectPosition screen;
  s32 palette;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(1);
  if (mode == 2)
    BattleEffect_DrawShadedCirclesMirrored(sheet, 10, 0x10000);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == 0)
    palette = (s32)&ResourceId_IceBlockSheet;
  else if (mode == 1)
    palette = (s32)&ResourceId_BlastSheet;
  else
    palette = (s32)&ResourceId_VioletPaletteC;
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);

  // The motes leave the middle of each target's body.
  for (i = 0; i != 8 * MOTES_PER_MEMBER; i++)
    MOTES[i].variant = -1;

  for (member = 0; member != effect->count; member++) {
    struct MotionObject *object;
    s32 half;

    object = BattleObject_GetFar(effect->actors[member])->object;
    half = BattleUnit_GetEffectHeightFar(effect->actors[member]) / 2;
    for (i = 0; i != MOTES_PER_MEMBER; i++) {
      struct EffectStep *mote = &MOTES[member * MOTES_PER_MEMBER + i];
      s32 speed;

      mote->x = object->x;
      mote->y = object->y + half;
      mote->z = object->z;
      mote->velocityX = ((Random16() & 0xff) - 128) << 10;
      mote->velocityY = ((Random16() & 0xff) - 128) << 10;
      mote->velocityZ = (speed = (Random16() & 0xff) - 128) << 10;
      if (mode == 2) {
        mote->velocityX <<= 1;
        mote->velocityY <<= 1;
        mote->velocityZ = speed << 11;
      }
      mote->variant = 0;
    }
  }

  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  if (mode == 2) {
    // A violet ramp from color 1, blended 16:6, with a trail.
    u16 *color = BG_PLTT + 1;

    for (i = 1; i != 64; i++) {
      s32 blue = 21 - (i + 1) / 3;

      *color++ = ((21 - i / 3) << 10) | blue;
    }
    REG_BLDALPHA = 0x610;
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    // Each transfer lowers the canvas's pixels by 4: an afterimage.
    work->transferMode = 3;
    work->transferValue = 0x04040404;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    struct MotionObject *user;
    s32 half;

    user = BattleObject_GetFar(effect->actor)->object;
    half = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
    if (frame == 64)
      BattlePlayback_StartFar(133);
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame == 40)
      BattleEffect_TintBattler(effect->actor, 7, -1, -1, 0);
    if (frame == effect->count * 20 + 52)
      BattleEffect_TintBattler(effect->actor, 0, -1, -1, 0);
    for (member = 0; member != effect->count; member++) {
      if (frame == member * 20)
        BattleEffect_TintBattler(effect->actors[member], 7, 5, member, 42);
      if (frame > member * 20) {
        for (i = 0; i != 32; i++) {
          struct EffectStep *mote = &MOTES[member * MOTES_PER_MEMBER + i];

          if (mote->variant >= 0) {
            u8 size = Data_081982f4[mode];

            // The canvas shows at twice the width: halve x.
            EffectPosition_ProjectPoint((s32 *)mote, &screen);
            screen.x >>= 1;
            if (mode == 2) {
              draw(canvas, sheet, screen.x - size / 2, screen.y - size, size,
                   size * 2);
              BattleEffectKernels_IntegrateVector3(mote, 61, 0);
            } else {
              draw(canvas, sheet + gBattleEffect_MoteCells[size - 1],
                   screen.x - size / 2, screen.y - size, size, size * 2);
              BattleEffectKernels_IntegrateVector3(mote, 62, 0);
            }

            // Home in on the user once ten frames out.
            if (frame > member * 20 + i + 10) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = (user->x - mote->x) >> 8;
              dy = (user->y + half - mote->y) >> 8;
              dz = (user->z - mote->z) >> 8;
              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx > -0x1000 && dx < 0x1000) &&
                  (dz > -0x1000 && dz < 0x1000))
                mote->variant = -1;
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"

#include "lib.h"
#include "menu.h"
#include "sound.h"

DEFINE_EFFECT_KIND(BattleEffect_RunPly, BattleEffect_RunHealingMotes, 0)

DEFINE_EFFECT_KIND(BattleEffect_RunWish, BattleEffect_RunHealingMotes, 1)

DEFINE_EFFECT_KIND(BattleEffect_RunCure, BattleEffect_RunHealingMotes, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode8,
                   BattleEffect_RunHealingMotes, 8)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode9,
                   BattleEffect_RunHealingMotes, 9)

void BattleEffect_RunCurePoison(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleEffect_RunHealingMotes(effect, 3);
    return;
  }
  BattleEffect_RunHealingMotes(effect, 4);
}

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode4,
                   BattleEffect_RunHealingMotes, 4)

DEFINE_EFFECT_KIND(BattleEffect_RunTonic, BattleEffect_RunHealingMotes, 5)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode7,
                   BattleEffect_RunHealingMotes, 7)

DEFINE_EFFECT_KIND(BattleEffect_RunPsyCrystal, BattleEffect_RunHealingMotes, 6)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode10,
                   BattleEffect_RunHealingMotes, 10)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode11,
                   BattleEffect_RunHealingMotes, 11)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode12,
                   BattleEffect_RunHealingMotes, 12)

void BattleEffect_RunHealingMotesMode13(struct BattleEffectArgument *effect) {
  effect->prelude = 0;
  effect->variant = 2;
  BattleEffect_RunHealingMotes(effect, 13);
}

#define DJINNI_SIDE(side) ((side) << 4)
#define DJINNI_ANCHORED 32

#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARKS_PER_MEMBER 64
#define BEAM_SHEET 0x800    // the blue beam as unpacked, two bytes a pixel
#define BEAM_BYTES 0x2000   // the beam packed to a byte a pixel
#define DJINNI_SHEET 0x1000 // the Djinni picture

// Motes of light gather around each target in turn, eight frames apart,
// turning as they close in on its middle; a healing chime sounds as they
// gather and the target glows.

// Setup: an optional Djinni appears first (the prelude); the mode chooses
// the canvas scale, the palette, the Djinni and how the 512 motes are
// scattered. Frame loop: the Djinni sways and, after frame 32, rises two
// pixels a frame; each target's motes come out four a frame around a point
// 40 pixels above it, turn about it by the mode's axes, and close in two
// pixels a frame (modes 3-5 let them fade instead, mode 7 throws them).
// The variant chooses 32, 64 or 128 motes a target.
void BattleEffect_RunHealingMotes(struct BattleEffectArgument *effect,
                                  u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 total;
  s32 member;
  u8 *sprites;
  s32 limit;
  s32 shift;
  s32 narrow;
  DrawRectangle *draw;
  s32 frame;
  s32 i;
  u8 *palette;
  s32 origin[3];
  s32 target[3];
  struct EffectPosition base;
  struct EffectPosition screen;
  s32 point[3];
  DrawRectangle blit[2];

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  shift = 0;
  BattleEffect_StartCanvasLayer(0);
  if (mode == 13)
    work->fadeFrames = 24;

  // The prelude: the Djinni appears; origin receives where.
  if (effect->prelude == 1) {
    s32 djinni;

    if (mode == 6)
      djinni = DJINNI_MARS;
    else if (mode == 2 || mode == 7)
      djinni = DJINNI_VENUS;
    else if (mode == 9 || mode == 10)
      djinni = DJINNI_JUPITER;
    else
      djinni = DJINNI_MERCURY;
    if (mode == 6 || mode == 0 || mode == 8 || mode == 9 || mode == 10)
      BattleEffect_RunDjinniEntrance(effect, djinni | DJINNI_SIDE(effect->side),
                                     origin, target);
    else
      BattleEffect_RunDjinniEntrance(
          effect, djinni | DJINNI_SIDE(effect->side) | DJINNI_ANCHORED, origin,
          target);
    effect->variant = 0;
  }

  // Modes 0, 8 and 9 show the canvas at full width, the first target at
  // the middle (64); the rest keep it twice as wide (narrow).
  if (mode == 0 || mode == 8 || mode == 9) {
    EffectPosition_ProjectScaledBattler(effect->actors[0], &base);
    shift = 64 - base.x;
    REG_BG2X = shift << 8;
    REG_BG2PA = 0x100;
    narrow = 0;
  } else {
    narrow = 1;
  }

  // The pictures, the beam packed to a byte a pixel, and the colors.
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet,
                             work->sheet + BEAM_SHEET, 1, 0);
  for (i = 0; i != 288; i++)
    work->sheet[BEAM_BYTES + i] = work->sheet[BEAM_SHEET + i * 2];
  if (mode <= 1 || mode == 3 || mode == 4 || mode == 5 || mode == 8 ||
      mode == 12) {
    if (effect->variant == 0)
      palette = Resource_GetTableEntry((s32)&ResourceId_IceShardSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
  } else if (mode == 9 || mode == 10) {
    palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
  } else if (mode == 6 || mode == 11 || mode == 13) {
    palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
  } else {
    palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  }
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // The Djinni: 40x40 at full width, a 20x40 picture when narrow.
  if (narrow == 0) {
    if (mode == 6)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else if (mode == 2 || mode == 7)
      palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
    else if (mode == 9 || mode == 10)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
  } else {
    if (mode == 6 || mode == 11 || mode == 13)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet);
    else if (mode == 2 || mode == 7)
      palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSmallSheet);
    else if (mode == 9 || mode == 10)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSmallSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet);
  }
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet + DJINNI_SHEET);
  BattleEffect_FetchRectangleBlitters(effect->side,
                                      draw = blit); // [0] MAX, [1] ADDITIVE

  // Scatter the motes; the mode decides how, and how long the effect is.
  if (mode == 0 || mode == 6 || mode == 8 || mode == 9 || mode == 10) {
    // Across and deep 64 pixels each way, 32-95 pixels up.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = ((Random16() & 0xff) - 127) << 15;
      spark->y = ((Random16() & 0x7f) + 64) << 15;
      spark->z = ((Random16() & 0xff) - 127) << 15;
      spark->variant = 0;
    }
    if (mode == 9)
      total = 54;
    else if (mode == 8)
      total = 76;
    else
      total = effect->count * 8 + 58;
  } else if (mode == 7) {
    // From the feet, 48 pixels up, thrown out and up.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->y = 48 << 16;
      spark->x = 0;
      spark->z = 0;
      spark->velocityX = ((Random16() & 0xff) - 127) << 8;
      spark->velocityY = -((Random16() & 0x7f) + 64) << 9;
      spark->velocityZ = ((Random16() & 0xff) - 127) << 8;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  } else if (mode == 12) {
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = ((Random16() & 0xff) - 127) << 15;
      spark->y = ((Random16() & 0x7f) + 64) << 15;
      spark->z = ((Random16() & 0xff) - 127) << 15;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  } else if (mode == 1 || mode == 13) {
    // 64 pixels each way.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = ((Random16() & 0xff) - 127) << 15;
      spark->y = ((Random16() & 0xff) - 127) << 15;
      spark->z = ((Random16() & 0xff) - 127) << 15;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  } else if (mode == 11) {
    // A wide cloud, 100 pixels each way.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = (Random16() % 200 - 100) << 15;
      spark->y = (Random16() % 200 - 100) << 15;
      spark->z = (Random16() % 200 - 100) << 15;
      spark->variant = 0;
    }
    total = effect->count * 8 + 46;
  } else if (mode == 2) {
    // A ring 32-95 pixels across, 50 pixels down, rising.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];
      s32 heading = Random16() & 0xffff;
      s32 radius = (Random16() & 63) + 32;

      spark->x = Trig_Sin(heading) * radius;
      spark->y = -50 << 16;
      spark->z = Trig_Cos(heading) * radius;
      spark->velocityY = ((Random16() & 31) + 32) << 13;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  } else if (mode == 3) {
    // 64 pixels across and deep, 32 up and down.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = ((Random16() & 0xff) - 127) << 15;
      spark->y = ((Random16() & 0xff) - 127) << 14;
      spark->z = ((Random16() & 0xff) - 127) << 15;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  } else {
    // 64 pixels each way.
    for (i = 0; i != 512; i++) {
      struct EffectStep *spark = &SPARKS[i];

      spark->x = ((Random16() & 0xff) - 127) << 15;
      spark->y = ((Random16() & 0xff) - 127) << 15;
      spark->z = ((Random16() & 0xff) - 127) << 15;
      spark->variant = 0;
    }
    total = effect->count * 8 + 58;
  }

  // Motes a target: 32 for variant 0, 128 for variant 2, else 64; mode 8
  // takes 128 and mode 12 16.
  limit = 64;
  if (effect->variant == 0)
    limit = 32;
  else if (effect->variant == 2)
    limit = 128;
  if (mode == 8)
    limit = 128;
  if (mode == 12)
    limit = 16;
  work->transferMode = 2;   // each transfer fades the canvas: a trail
  work->transferValue = 75; // to 3/4
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 40)
      BattlePlayback_StartFar(0);
    if (mode == 7 && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource((s32)&ResourceId_CyanPalette);

    // The Djinni sways, and after frame 32 rises two pixels a frame; for
    // the first four frames it is drawn ADDITIVE as well.
    if (effect->prelude == 1) {
      if (narrow == 0) {
        s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin[1] - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + DJINNI_SHEET, x, y, 40, 40);
        if (frame <= 3)
          draw[1](canvas, work->sheet + DJINNI_SHEET, x, y, 40, 40);
      } else {
        s32 x = ((Trig_Sin(frame << 11) * 10) >> 16) + origin[0] / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin[1] - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + DJINNI_SHEET, x, y, 20, 40);
        if (frame <= 3)
          draw[1](canvas, work->sheet + DJINNI_SHEET, x, y, 20, 40);
      }
    }

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          BattleObject_GetFar(effect->actors[member])->object;
      s32 start = member * 8;

      // The motes turn about a point 40 pixels above the target.
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = object->x;
      point[1] = 40 << 16;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);
      if (frame == start + 20)
        Sound_PlayFar(126);
      if (mode == 8) {
        if (frame == start + 41)
          BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 32);
      } else if (mode == 11) {
        if (frame == start + 24)
          BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 28);
      } else if (frame == start + 36) {
        BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 28);
      }
      if (frame > start) {
        if (mode == 0 || mode == 6 || mode == 7 || mode == 8 || mode == 10 ||
            mode == 12) {
          Transform_ApplyYaw((frame - start) << 9);
        } else if (mode == 9) {
          Transform_ApplyYaw((frame - start) << 11);
        } else if (mode == 11) {
        } else if (mode == 13) {
          Transform_ApplyRoll(frame << 9);
        } else if (mode == 1) {
          Transform_ApplyPitch(frame << 9);
          Transform_ApplyRoll(frame << 9);
        } else if (mode == 2 || mode == 12) {
          Transform_ApplyYaw((frame - member * 40) << 9);
        } else if (mode == 3) {
          Transform_ApplyYaw((frame - start) << 9);
        } else {
          Transform_ApplyYaw((frame - start) << 9);
          Transform_ApplyPitch((frame - start) << 9);
        }

        // Four motes come out a frame; in modes 3-5 each fades at
        // start + i / 2 + 32, in mode 7 at start + i / 2 + 64.
        for (i = 0; i != limit; i++) {
          struct EffectStep *spark = &SPARKS[member * SPARKS_PER_MEMBER + i];
          s32 end;

          if (mode == 3 || mode == 4 || mode == 5)
            end = start + i / 2 + 32;
          else if (mode == 7)
            end = start + i / 2 + 64;
          else
            end = 0x10000;
          if (frame > start + i / 4 && frame < end) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            distance = distance * 48 / 64;
            if (distance != 0) {
              s32 size;

              EffectPosition_ProjectPoint((s32 *)spark, &screen);
              if (mode == 0 || mode == 8 || mode == 9)
                screen.x += shift;
              else
                screen.x >>= 1;
              screen.y += 16;

              // Depth 314-634 gives sizes 3 down to 1; some modes cycle
              // through the nine puffs instead.
              if (screen.depth < 314)
                screen.depth = 314;
              if (screen.depth > 634)
                screen.depth = 634;
              size = 3 - (screen.depth - 314) / 128;
              if (mode == 0 || mode == 8 || mode == 9)
                size = (i * 4 + frame) % 9;
              else if (mode == 11 || mode == 13)
                size = 4 - (screen.depth - 314) / 128;
              else if (mode == 7)
                size = (i * 4 + frame) % 9;
              if (mode == 0 || mode == 3 || mode == 4 || mode == 5 ||
                  mode == 7 || mode == 8 || mode == 9 || mode == 11 ||
                  mode == 13) {
                u8 *cell = work->sheet + gBattleEffect_PuffCells[size];
                u32 edge = gBattleEffect_PuffSizes[size];
                u32 half = edge >> 1;

                draw[1](canvas, cell, screen.x - half, screen.y - half, edge,
                        edge);
              } else if (mode == 12) {
                blit[0](canvas, work->sheet + BEAM_BYTES, screen.x - 6,
                        screen.y - 12, 12, 24);
              } else {
                draw[1](canvas, sprites + gBattleEffect_MoteCells[size - 1],
                        screen.x - size / 2, screen.y - size, size, size * 2);
              }

              // Close in by 1/distance of the way a frame.
              if (mode <= 2 || mode == 6 || mode == 8 || mode == 9 ||
                  mode == 10 || mode == 11 || mode == 13 || mode == 12) {
                spark->x -= spark->x / distance;
                spark->y -= spark->y / distance;
                spark->z -= spark->z / distance;
              }
              if (mode == 7) {
                spark->x += spark->velocityX;
                spark->y += spark->velocityY;
                spark->z += spark->velocityZ;
              }
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Impact, High Impact, Demon Spear, Angel Spear (kind 26); Forge
/// (kind 210); Kindle (kind 223); kind 46 is used by no ability.
DEFINE_EFFECT_KIND(BattleEffect_RunImpact, BattleEffect_RunStatBoost, 0)

/// Abilities: Guard, Protect, Guardian, Protector (kind 28); Corona (kind 211).
DEFINE_EFFECT_KIND(BattleEffect_RunGuard, BattleEffect_RunStatBoost, 1)

/// Abilities: Iron (kind 220).
DEFINE_EFFECT_KIND(BattleEffect_RunIron, BattleEffect_RunStatBoost, 6)

DEFINE_EFFECT_KIND(BattleEffect_RunStatBoostMode7, BattleEffect_RunStatBoost, 7)

/// Abilities: Ward, Resist, Magic Shell, Magic Shield (kind 30); Breeze (kind
/// 215).
DEFINE_EFFECT_KIND(BattleEffect_RunWard, BattleEffect_RunStatBoost, 2)

/// Abilities: Steam (kind 221).
DEFINE_EFFECT_KIND(BattleEffect_RunSteam, BattleEffect_RunStatBoost, 11)

/// Abilities: Granite (kind 200).
DEFINE_EFFECT_KIND(BattleEffect_RunGranite, BattleEffect_RunStatBoost, 3)

/// Abilities: Flash (kind 213).
DEFINE_EFFECT_KIND(BattleEffect_RunFlash, BattleEffect_RunStatBoost, 5)

/// Abilities: Shade (kind 222).
DEFINE_EFFECT_KIND(BattleEffect_RunShade, BattleEffect_RunStatBoost, 8)

/// Abilities: Guard Aura (kind 460).
DEFINE_EFFECT_KIND(BattleEffect_RunGuardAura, BattleEffect_RunStatBoost, 12)

/// Abilities: Zephyr (kind 216).
DEFINE_EFFECT_KIND(BattleEffect_RunZephyr, BattleEffect_RunStatBoost, 4)

/// Abilities: Coal (kind 224).
DEFINE_EFFECT_KIND(BattleEffect_RunCoal, BattleEffect_RunStatBoost, 9)

/// Effect kind 182: the stat boost's mode 10 (a burst with no gathering
/// lights), with no prelude Djinni and the fade step cleared.
void BattleEffect_RunStatBoostMode10(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];

  work->fadeStep = 0;
  effect->prelude = 0;
  BattleEffect_RunStatBoost(effect, 10);
}

// The two blitters' heap slots, byte offsets into Ram_HeapSlots (The Broken
// Seal's HEAP_SLOT_BLITTER and HEAP_SLOT_BLITTER_ALTERNATE).
#define BLITTER_SLOT 104           // Ram_HeapSlots->unk64[1]
#define BLITTER_SLOT_ALTERNATE 188 // Ram_HeapSlots->unkb8[1]

// Build a blitter in its heap slot and fetch its entry.
#define LOAD_BLITTER(out, flags, mode)                                         \
  BattleEffect_BuildBlitter(BLITTER_SLOT, 7, 7, flags, mode);                  \
  out = (DrawRectangle)Ram_HeapSlots->unk64[1]

#define FLARES ((struct EffectStep *)Ram_MapCellBuffer)
#define FLARES_PER_MEMBER 64

static inline CopyWordsFn CopyWordsRoutine(void) {
  /* FAKEMATCH(wrapper): the routine fetched through this inline; as direct
   * calls to Iwram_CopyWords, 4 of 1146 instructions of
   * BattleEffect_RunStatBoost differ in tla-en. */
  return Iwram_CopyWordsEntry;
}

// Lights gather around each target in turn, eight frames apart, turning as
// they close in; at the center they burst, and the mode's picture plays
// there (a dagger, a shield, a streak or a wisp).

// Setup: an optional Djinni appears first (the prelude); the canvas is
// 1.25 times as wide, centered on a lone target or set by the side. Frame
// loop: the Djinni sways and rises; each target's 32 lights come out one a
// frame and close in two pixels a frame; from frame 52 of a target its
// burst plays, six pictures four frames apart, and from frame 80 the
// mode's picture.
void BattleEffect_RunStatBoost(struct BattleEffectArgument *effect, u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 member;
  s32 shift;
  s32 flags;
  s32 start;
  // Holds BLIT_ADDITIVE for the whole function: the lights' second build
  // then reloads its 0 in place of a fresh constant.
  s32 additive = BLIT_ADDITIVE;
  // Holds 0 for the whole function: the center's coordinates are stored from
  // it.
  s32 zero = 0;
  // Each holds COMPOSITE_FLIP_X for the whole function, one for each of the
  // Djinni's builds: reload rebuilds the 4 at each xor, and the two xors stay
  // apart.
  s32 flipX = COMPOSITE_FLIP_X;
  s32 flipX2 = COMPOSITE_FLIP_X;
  s32 frame;
  s32 i;
  s32 origin[3];
  s32 target[3];
  struct EffectPosition base;
  s32 center[3];
  struct EffectPosition screen;
  s32 point[3];

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  start = 0;
  BattleEffect_StartCanvasLayer(0);

  // The prelude: the Djinni appears on the far side; origin receives where.
  if (effect->prelude == 1) {
    if (mode == 3 || mode == 6)
      BattleEffect_RunDjinniEntrance(
          effect,
          DJINNI_VENUS | DJINNI_SIDE(effect->side ^ 1) | DJINNI_ANCHORED,
          origin, target);
    else if (mode == 7 || mode == 8 || mode == 11)
      BattleEffect_RunDjinniEntrance(
          effect,
          DJINNI_MERCURY | DJINNI_SIDE(effect->side ^ 1) | DJINNI_ANCHORED,
          origin, target);
    else if (mode == 2 || mode == 4)
      BattleEffect_RunDjinniEntrance(
          effect,
          DJINNI_JUPITER | DJINNI_SIDE(effect->side ^ 1) | DJINNI_ANCHORED,
          origin, target);
    else if (mode != 10)
      BattleEffect_RunDjinniEntrance(
          effect, DJINNI_MARS | DJINNI_SIDE(effect->side ^ 1) | DJINNI_ANCHORED,
          origin, target);
    origin[0] = origin[0] * 4 / 5;
  }

  // Mode 10 is mode 0 joined at frame 53, with a fade.
  if (mode == 10) {
    work->fadeFrames = 24;
    mode = 0;
    start = 53;
  }
  REG_BG2PA = 0xcc; // 1.25 times as wide

  // The pictures and the colors.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + SERIES_B_FLARES, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                             work->sheet + SERIES_B_BURST, 1, 1);
  if (mode == 3 || mode == 5 || mode == 8 || mode == 12) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletStreakSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 1);
    if (mode == 3)
      CopyWordsRoutine()(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet), 128);
    else if (mode == 5)
      CopyWordsRoutine()(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
    else
      CopyWordsRoutine()(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet), 128);
  } else if (mode == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 1);
  } else if (mode == 9) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 0);
    CopyWordsRoutine()((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                       128);
  } else {
    void *palette;

    if (mode == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet,
                                 work->sheet + SERIES_B_PICTURE, 1, 0);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet,
                                 work->sheet + SERIES_B_PICTURE, 1, 0);
    if (mode == 0)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else if (mode == 2 || mode == 4)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    else if (mode == 6)
      palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
    else if (mode == 7 || mode == 11)
      palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
    else if (mode == 1)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    CopyWordsRoutine()((void *)BG_PLTT, palette, 128);
  }
  if (mode == 3 || mode == 6)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else if (mode == 12)
    Graphics_LoadGreyRamp();
  else if (mode == 2 || mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else if (mode == 7 || mode == 8 || mode == 11)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);

  // The lights: 25 pixels across and deep, 50 up and down.
  for (i = 0; i != 512; i++) {
    struct EffectStep *flare = &FLARES[i];

    flare->x = (Random16() % 200 - 100) << 14;
    flare->y = (Random16() % 200 - 100) << 15;
    flare->z = (Random16() % 200 - 100) << 14;
    flare->variant = 0;
  }

  // A lone target is moved to the canvas's middle (64).
  if (effect->count == 1) {
    EffectPosition_ProjectScaledBattler(effect->actors[0], &base);
    shift = -base.x * 4 / 5 + 64;
  } else {
    if (effect->side == 1)
      shift = -64;
    else
      shift = 0;
  }
  REG_BG2X = shift << 8;
  work->transferMode = 2;   // each transfer fades the canvas: a trail
  work->transferValue = 50; // to 1/2
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (effect->side == 1)
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  else
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;

  for (frame = start; frame != effect->count * 8 + 108; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 0)
      Sound_PlayFar(142);
    if (mode == 6 && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource((s32)&ResourceId_EffectPalette01);
    if (mode == 11 && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource((s32)&ResourceId_JupiterDjinnSheet);
    if (frame == 80)
      BattlePlayback_StartFar(0);

    // The Djinni sways, and after frame 32 rises two pixels a frame; for
    // the first four frames it is drawn ADDITIVE as well.
    prelude = effect->prelude;
    if (prelude == 1) {
      s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
      s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin[1] - 24;

      // The mode is the prelude, 1 here: BLIT_MAX.
      LOAD_BLITTER(draw[0], flags ^ flipX, prelude);
      BattleEffect_BuildBlitter(BLITTER_SLOT_ALTERNATE, 7, 7, flags ^ flipX2,
                                additive);
      // Read through the slot's address, which folds to one constant.
      draw[1] = *(DrawRectangle *)&Ram_HeapSlots->unkb8[1];

      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
      HeapSlot_Release(BLITTER_SLOT_ALTERNATE);
      HeapSlot_Release(BLITTER_SLOT);
    }

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          BattleObject_GetFar(effect->actors[member])->object;
      s32 height =
          BattleUnit_GetEffectHeightFar(effect->actors[member]) * 2 / 3;

      if (frame == member * 8 + 80)
        Sound_PlayFar(212);
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      // The lights turn about two thirds of the target's height.
      point[0] = object->x;
      point[1] = height;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);
      if (start != 0) {
        if (frame == member * 8 + 53)
          BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 16);
      } else {
        if (frame == member * 8 + 48)
          BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 16);

        // The lights turn and close in two pixels a frame.
        if (frame > member * 8) {
          /* FAKEMATCH(barrier): an empty asm through mode keeps it in a high
           * register; without it, 2 of 1146 instructions differ in tla-en. */
          __asm__("" : "+h"(mode));
          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          BattleEffect_BuildBlitter(BLITTER_SLOT_ALTERNATE, 7, 7, flags,
                                    additive);
          {
            void **slot = &Ram_HeapSlots->unkb8[1];

            /* FAKEMATCH(barrier): an empty asm through slot hides that it holds
             * the heap slot's address; as draw[1] = Ram_HeapSlots->unkb8[1], 32
             * of 1146 instructions differ in tla-en. */
            __asm__("" : "=r"(slot) : "0"(slot));
            draw[1] = (DrawRectangle)*slot;
          }
          if (mode == 0) {
            Transform_ApplyPitch(-frame << 10);
          } else if (mode == 1) {
          } else if (mode == 2) {
            Transform_ApplyYaw(frame << 10);
          } else {
            Transform_ApplyYaw(frame << 10);
            Transform_ApplyRoll(frame << 10);
          }
          for (i = 0; i != 32; i++) {
            struct EffectStep *flare = &FLARES[member * FLARES_PER_MEMBER + i];

            if (frame > member * 8 + i) {
              s32 xx = (flare->x >> 8) * (flare->x >> 8);
              s32 yy = (flare->y >> 8) * (flare->y >> 8);
              s32 zz = (flare->z >> 8) * (flare->z >> 8);
              s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

              if (distance != 0) {
                s32 size;

                // Half the width: depth 314-634 gives 6 down to 1.
                EffectPosition_ProjectPoint((s32 *)flare, &screen);
                screen.x = screen.x * 4 / 5 + shift;
                if (screen.depth < 314)
                  screen.depth = 314;
                if (screen.depth > 634)
                  screen.depth = 634;
                size = 6 - (screen.depth - 314) / 64;
                draw[1](canvas,
                        work->sheet + gBattleEffect_FlarePictures[size - 1],
                        screen.x - size, screen.y - size, size * 2, size * 2);
                flare->x -= flare->x / distance;
                flare->y -= flare->y / distance;
                flare->z -= flare->z / distance;
              }
            }
          }
          HeapSlot_Release(BLITTER_SLOT_ALTERNATE);
          HeapSlot_Release(BLITTER_SLOT);
        }
      }

      // Where the center is on the screen.
      center[0] = zero;
      center[1] = zero;
      center[2] = zero;
      EffectPosition_ProjectPoint(center, &screen);
      screen.x = screen.x * 4 / 5 + shift;

      // The burst: from frame 52, six pictures four frames apart.
      if (frame >= member * 8 + 52 && frame < member * 8 + 76) {
        s32 cel;

        // An empty once-only block: its loop notes end CSE's block here, so the
        // burst reads screen from the stack, not through the pointer passed to
        // EffectPosition_ProjectPoint.
        do {
        } while (0);
        cel = (frame - member * 8 - 52) / 4 % 6;

        LOAD_BLITTER(draw[0], flags, BLIT_MAX);
        draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_BURST,
                screen.x - 20, screen.y - 20, 40, 40);
        HeapSlot_Release(BLITTER_SLOT);
      }

      // From frame 80, the mode's picture.
      if (mode == 0) {
        // Seven 24x40 pictures, four frames apart.
        if (frame >= member * 8 + 80 && frame < member * 8 + 108) {
          s32 cel = (frame - member * 8 - 80) / 4 % 7;

          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          draw[0](canvas, work->sheet + cel * 960 + SERIES_B_PICTURE,
                  screen.x - 12, screen.y - 20, 24, 40);
          HeapSlot_Release(BLITTER_SLOT);
        }
      } else if (mode == 3 || mode == 5 || mode == 8 || mode == 12) {
        // Six 32x64 pictures, four frames apart.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          draw[0](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE,
                  screen.x - 16, screen.y - 32, 32, 64);
          HeapSlot_Release(BLITTER_SLOT);
        }
      } else if (mode == 4 || mode == 9) {
        // Six 64x32 pictures two frames apart, mirrored below into 64x64.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 2 % 6;
          u8 *picture;    // the cel, drawn as its upper and mirrored lower half
          s32 lowerFlags; // the lower half drawn flipped

          // The lower half's flags are taken before the first entry is
          // fetched, which leaves the first entry r8 and the second sl.
          BattleEffect_BuildBlitter(BLITTER_SLOT, 7, 7, flags, BLIT_MAX);
          lowerFlags = flags | COMPOSITE_FLIP_Y;
          draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
          BattleEffect_BuildBlitter(BLITTER_SLOT_ALTERNATE, 7, 7, lowerFlags,
                                    BLIT_MAX);
          picture = work->sheet + cel * 2048 + SERIES_B_PICTURE;
          draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
          draw[0](canvas, picture, screen.x - 32, screen.y - 24, 64, 32);
          draw[1](canvas, picture, screen.x - 32, screen.y + 8, 64, 32);
          HeapSlot_Release(BLITTER_SLOT_ALTERNATE);
          HeapSlot_Release(BLITTER_SLOT);
        }
      } else {
        // Six 40x40 pictures four frames apart, ADDITIVE.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          LOAD_BLITTER(draw[0], flags, BLIT_ADDITIVE);
          draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_PICTURE,
                  screen.x - 20, screen.y - 20, 40, 40);
          HeapSlot_Release(BLITTER_SLOT);
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void SummonEffect_RunCanvasMode0(void *action) {
  SummonEffect_RunCanvasEffect(action, 0);
}

void SummonEffect_RunCanvasMode3(void *action) {
  SummonEffect_RunCanvasEffect(action, 3);
}

void SummonEffect_RunCanvasMode1(void *action) {
  SummonEffect_RunCanvasEffect(action, 1);
}

void SummonEffect_RunCanvasMode2(void *action) {
  SummonEffect_RunCanvasEffect(action, 2);
}

// The dagger and shield canvas effect, behind the four wrappers above;
// the Broken Seal's twin is
// BattleEffect_RunStatReduction, which has no mode 3.

#define RAMP_IMAGE 0x2580 // the shard picture, then seven darker copies
#define RAMP_SIZE 696     // one copy

/// A dagger (modes 0 and 3) or a shield (1 and 2) appears over each target
/// in turn, four frames apart, and breaks into shards that fly off and
/// darken.
///
/// Setup: seven darker copies of the shard picture are built by clamping
/// its levels; 24 shards a target start in a grid across the target and
/// scatter with random headings toward the user's side. Frame loop: a
/// target's picture plays for 24 frames; at frame 24 a chime sounds and the
/// shards fly for 36 frames, one shade darker every four frames.
void SummonEffect_RunCanvasEffect(struct BattleEffectArgument *effect,
                                  s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 j;
  s32 xOffset;
  DrawRectangle *draw;
  struct BattleCamera *camera;
  s32 origin[3];
  struct EffectPosition screen;
  s32 point[3];
  DrawRectangle callbacks[2];
  s32 i;
  s32 palette;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  if (mode == 0 || mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work->sheet, 1, 1);
  if (mode == 0)
    palette = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 3) {
    palette = (s32)&ResourceId_GlowOrbSheet;
    /* FAKEMATCH(barrier): an empty asm keeps the mode-3 arm apart from the
     * identical mode-1 arm; without it, 8 of 553 instructions differ in
     * tla-en. */
    asm("");
  } else if (mode == 1)
    palette = (s32)&ResourceId_GlowOrbSheet;
  else
    palette = (s32)&ResourceId_PinkBurstSheet;
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage,
                             work->sheet + RAMP_IMAGE, 0, 0);

  // Copy i holds every level clamped to 64 - 7i.
  for (i = 1; i != 8; i++) {
    for (j = 0; j != RAMP_SIZE; j++) {
      s32 level = work->sheet[RAMP_IMAGE + j];

      if (level > 64 - i * 7)
        level = 64 - i * 7;
      if (level < 0)
        level = 0;
      work->sheet[RAMP_IMAGE + i * RAMP_SIZE + j] = level;
    }
  }

  if (effect->side == 1) {
    REG_BG2X = -0x7000;
    xOffset = -112;
  } else {
    REG_BG2X = 0;
    xOffset = 0;
  }

  // The shards: rows six pixels apart, columns two (dagger) or eight
  // (shield) pixels deep, thrown toward the user's side.
  for (i = 0; i != 512; i++) {
    struct EffectStep *shard = &SHARDS[i];
    s32 radius = 192;
    s32 angle;

    angle = Random16() & 0xffff;
    shard->x = 0;
    if (mode == 0 || mode == 3) {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 2 - 2) << 16;
    } else {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 8 - 16) << 16;
    }
    if (effect->side == 1)
      shard->velocityX = Q16_ONE * 2;
    else
      shard->velocityX = -Q16_ONE * 2;
    shard->velocityY = ((Trig_Cos(angle) * radius) >> 6) + Q16_ONE;
    shard->velocityZ = (Trig_Sin(angle) * radius) >> 6;
    shard->variant = Random16() & 0xff;
  }

  BattleEffect_FetchRectangleBlitters(effect->side, draw = callbacks);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->count * 4 + 64; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    if (frame == 72)
      BattlePlayback_StartFar(0);
    for (j = 0; j != effect->count; j++) {
      struct MotionObject *object;
      s32 time;
      s32 x;
      s32 y;

      time = frame - j * 4;
      object = BattleObject_GetFar(effect->actors[j])->object;
      if (time > 0) {
        // The picture stands 40 pixels above the target.
        Transform_Reset();
        Transform_SetLookAt(camera->eye, camera->pos);
        point[0] = object->x;
        point[1] = 160 << 13;
        point[2] = object->z;
        Transform_Reset();
        Transform_SetLookAt(camera->eye, camera->pos);
        SceneTransform_ApplyPosition(point);
        origin[0] = 0;
        origin[1] = 0;
        origin[2] = 0;
        EffectPosition_ProjectPoint(origin, &screen);
        x = screen.x + xOffset;
        y = screen.y;
        if (mode == 0 || mode == 3) {
          if (time <= 26)
            callbacks[0](canvas, work->sheet + time / 4 % 7 * 960, x - 12,
                         y - 20, 24, 40);
        } else {
          if (time <= 23)
            draw[1](canvas, work->sheet + time / 4 % 6 * 1600, x - 20, y - 20,
                    40, 40);
        }
        // frame - j * 4 is time; this spelling matches here and below.
        if (frame - j * 4 == 24)
          Sound_PlayFar(143);

        // The shards fly for 36 frames, a shade darker every four.
        if (time >= 24 && time <= 60) {
          s32 level;

          level = 0;
          if (frame - j * 4 > 28) {
            level = (time - 24) / 4;
            if (level > 7)
              level = 7;
          }
          for (i = 0; i != 24; i++) {
            struct EffectStep *shard = &SHARDS[j * 32 + i];
            s32 cell;

            cell = i % 4 * 3 + (shard->variant + time) / 8 % 3;
            EffectPosition_ProjectPoint((s32 *)shard, &screen);
            x = screen.x + xOffset;
            y = screen.y;
            callbacks[0](
                canvas,
                work->sheet +
                    (level * RAMP_SIZE + gStatChange_ShardCells[cell]) +
                    RAMP_IMAGE,
                x, y, gStatChange_ShardWidths[cell],
                gStatChange_ShardHeights[cell]);
            BattleEffectKernels_IntegrateVector3(shard, 60, 0);
          }
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#define FLAME_COUNT 9

// Nine flames puff up along a half-circle arc, four frames apart.

// Setup: the flames are placed on an arc 32 pixels wide and 16 high, from
// the left when the first target is an enemy (128 and up), else from the
// right, each one starting four frames after the last. Frame loop: a flame
// steps through six pictures, four frames each; variant 1 adds a copy 16
// pixels higher, variant 2 another 32 higher and a brighter trail. The
// targets are hit from frame 16, eight frames apart.
void BattleEffect_RunFlare(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 i;
  s32 cell;
  u32 w;
  s32 sign;
  s32 org;
  s32 angle;
  s32 stagger;
  struct EffectStep *puff;
  struct EffectStep *p;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // The arc: from -90 degrees in steps of 22.5.
  if (effect->actors[0] > 127) {
    org = 0;
    sign = 1;
  } else {
    org = 64;
    sign = -1;
  }
  angle = -0x4000;
  i = 0;
  stagger = 0;
  puff = work->particles;
  do {
    puff->x = ((sign * ((Trig_Sin(angle) << 5) >> 16)) + org) + 20;
    puff->y = ((Trig_Cos(angle) << 4) >> 16) + 40;
    puff->variant = stagger;
    angle += 0x1000;
    stagger -= 4;
    i += 1;
    puff++;
  } while (i != FLAME_COUNT);

  work->transferMode = 2;
  if (effect->variant == 2)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(136);
  frame = 0;
  do {
    if (frame == 24)
      BattlePlayback_StartFar(133);

    // A flame moves to its next picture every four frames and goes out
    // after the sixth.
    i = 0;
    p = work->particles;
    do {
      if (p->variant >= 0 && p->variant <= 23) {
        cell = p->variant / 4;
        draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
             p->x - ((w = gData_08197467[cell]) >> 1),
             p->y + gBattleEffect_PuffArcCellBiasY[cell], w,
             gBattleEffect_PuffArcCellHeights[cell]);
        if (effect->variant != 0)
          draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
               p->x - ((w = gData_08197467[cell]) >> 1),
               (p->y + gBattleEffect_PuffArcCellBiasY[cell]) - 16, w,
               gBattleEffect_PuffArcCellHeights[cell]);
        if (effect->variant == 2)
          draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
               p->x - ((w = gData_08197467[cell]) >> 1),
               (p->y + gBattleEffect_PuffArcCellBiasY[cell]) - 32, w,
               gBattleEffect_PuffArcCellHeights[cell]);
      }
      i += 1;
      p->variant += 1;
      p++;
    } while (i != FLAME_COUNT);

    // Hit the targets, eight frames apart.
    i = 0;
    while (i != effect->count) {
      if (frame == (i * 8) + 16)
        BattleEffect_TintBattler(effect->actors[i], 10, 5, i, 12);
      i += 1;
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame += 1;
  } while (frame != 60);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

s32 BattleEffect_BuildCenteredIndexList(BattleEffectIndexWindow *window,
                                        s16 *output) {
  s32 length = window->indexRadius * 2 + 1;
  s32 entryCount = 0;
  s32 center = 0;
  s32 entryIndex;

  for (entryIndex = 0; entryIndex != length; entryIndex++) {
    if (window->currentValue == window->values[entryIndex]) {
      center = entryIndex;
      break;
    }
  }
  for (entryIndex = 0; entryIndex != length; entryIndex++) {
    s32 battleValue = center + entryIndex - window->indexRadius;

    if (battleValue >= 0 && battleValue < window->entryLimit) {
      output[entryCount++] = battleValue;
    }
  }
  return entryCount;
}

// Battle effect kind 1, the quake: rock pillars rise under the targets. The
// Broken Seal's BattleEffect_RunQuake, call for call.

#define PILLAR_W 40        // pixels
#define ROCK_OFFSET 0x1e00 // the rock picture in the sheet, after the band

// The byte of pixel (x, y) in a 32-tile-wide 8bpp character block.
#define BG_DOT(x, y)                                                           \
  (((x) & 7) + (((x) / 8) << 6) + (((y) & 7) << 3) + (((y) / 8) << 11))

/// Three rock pillars rise from the ground, left to right four frames apart,
/// and sink again; targets standing on one are thrown into the air.
///
/// Setup: the band of background under the targets (x 32-71, y 96-111) is
/// cut out of BG character memory to cap each pillar. Frame loop: each
/// pillar's height follows a sine over its rise time, a target over a
/// pillar is launched upward, and a target that lands flashes; the camera
/// shakes all the while. The variant chooses the length, the rise time and
/// the shake.
void BattleEffect_RunQuake(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 column;
  DrawRectangle draw;
  struct BattleCamera *camera;
  s32 xOffset;
  struct RockWallHeights heights;
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 row;
  s32 col;
  s32 y;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100; // no horizontal stretch
  REG_BLDCNT = 0;    // no blending
  Resource_LoadAndDecompress((s32)&ResourceId_RockWallSheet,
                             work->sheet + ROCK_OFFSET, 1, 1);

  // Copy the background's characters and cut the band out of them.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)0x06008000, 0x8000);
  for (row = 0; row != 16; row++) {
    y = row + 96;
    for (col = 0; col != PILLAR_W; col++) {
      s32 pixel = BG_DOT(col + 32, y);

      work->sheet[row * PILLAR_W + col] = Ram_MapCellBuffer[pixel];
    }
  }

  // On the far side the canvas moves 112 pixels right, and the targets'
  // screen positions with it.
  if (effect->side == 1) {
    REG_BG2X = -(112 << 8);
    xOffset = -112;
  } else {
    xOffset = 0;
  }
  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  for (member = 0; member != effect->count; member++)
    work->particles[member].variant = 0; // 1 once landed
  work->transferMode = 1;                // clear the canvas on each transfer
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  heights = gRockWall_Heights;
  work->shakeFrames = 128;
  Sound_PlayFar(141);

  for (frame = 0; frame != gQuake_Timings[effect->variant][TIMING_FRAMES];
       frame++) {
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    // The results begin 16 frames before the end.
    if (frame == gQuake_Timings[effect->variant][TIMING_FRAMES] - 16)
      BattlePlayback_StartFar(133);

    // The pillars rise from frames 16, 20 and 24.
    for (column = 0; column != 3; column++) {
      s32 start = column * 4 + 16;

      if ((frame & 31) + 32 == start) // never true: the left side is >= 32
        heights.column[column] += 32;
      if (frame >= start &&
          frame < start + gQuake_Timings[effect->variant][TIMING_RISE_FRAMES]) {
        s32 height;
        s32 top;

        // Up and down along a sine, 1/64 of a turn a frame.
        height =
            (heights.column[column] * Trig_Sin((frame - start) << 10)) >> 16;
        if (height < 0)
          height = -height;
        top = 112 - height;
        draw(canvas, work->sheet + ROCK_OFFSET, column * PILLAR_W + 8, top,
             PILLAR_W, height);
        draw(canvas, work->sheet, column * PILLAR_W + 8, 96 - height, PILLAR_W,
             16);

        // Launch the targets standing on the pillar.
        for (member = 0; member != effect->count; member++) {
          struct MotionObject *object;

          object = BattleObject_GetFar(effect->actors[member])->object;
          point[0] = object->x;
          point[1] = object->y;
          point[2] = object->z;
          EffectPosition_ProjectPoint(point, &screen);
          screen.x += xOffset;
          if (screen.x >= column * PILLAR_W + 8 &&
              screen.x <= column * PILLAR_W + 8 + PILLAR_W && screen.y >= top) {
            object->velocityY = 12 << 16; // 12 pixels a frame upward
            object->verticalMotionStrength = 0xab85;
          }
          if (object->y < 0)
            BattleEffect_TintBattler(effect->actors[member], 0, 5, -1, 0);
        }
      }
    }

    // A target that lands flashes in palette 7 for 5 frames.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object;
      struct EffectStep *state;

      object = BattleObject_GetFar(effect->actors[member])->object;
      state = &work->particles[member];
      if (state->variant == 0 && object->y <= 0 && object->velocityY < 0) {
        state->variant = 1;
        BattleEffect_TintBattler(effect->actors[member], 7, 5, member, 5);
      }
    }
    BattleCamera_Shake(gQuake_Timings[effect->variant][TIMING_SHAKE],
                       gQuake_Timings[effect->variant][TIMING_SHAKE]);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#define EMBERS ((struct EffectStep *)Ram_MapCellBuffer)
#define IN_FLIGHT -1 // an ember's variant; 0 and up count its flare's frames

// Embers spray out of the user, two a frame, then home in on the targets.

// Setup: up to 64 embers start at the top of the user's body with a random
// heading and speed. Frame loop: an ember flies for 48 frames (drawn MAX
// while young, then ADDITIVE), then accelerates toward its target, held to
// 60/64 of its speed until frame 85 of its life. Below the ground it lands:
// it leaves a six-picture flare, its target flashes and the screen shakes
// for two frames. The camera sways between frames 16 and 80. The variant
// chooses the number of embers.
void BattleEffect_RunFire(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 top;
  struct EffectPosition screen;
  DrawRectangle draw[2];
  struct MotionObject *user;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  if (effect->side == 1)
    BattleEffect_StartCanvasLayer(1);
  else
    BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(
      188, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  REG_BLDALPHA = 0x1010;

  // The embers leave the top of the user's body: a random heading and
  // speed across, -16 to 111 sixty-fourths of a pixel up.
  user = BattleObject_GetFar(effect->actor)->object;
  top = user->y + BattleUnit_GetEffectHeightFar(effect->actor);
  for (i = 0; i != 64; i++) {
    struct EffectStep *ember = &EMBERS[i];
    s32 angle;
    s32 speed;

    angle = Random16();
    speed = (Random16() & 127) + 127;
    ember->velocityX = (Trig_Sin(angle) * speed) >> 6;
    ember->velocityY = (s32)(((Random16() & 127) - 16) << 16) >> 6;
    ember->velocityZ = (Trig_Cos(angle) * speed) >> 6;
    ember->x = user->x;
    ember->y = top;
    ember->z = user->z;
    ember->variant = IN_FLIGHT;
  }

  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75; // each transfer fades the canvas to 3/4: a trail
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != (gFire_Counts[effect->variant] >> 1) + 132;
       frame++) {
    if (frame > 16 && frame < 80)
      work->cameraDelta = 0x100;
    else
      work->cameraDelta = 0;
    if (frame == (gFire_Counts[effect->variant] >> 1) + 108)
      BattlePlayback_StartFar(133);
    BattleCamera_SetViewFar(0, 0, 0, 100);
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);

    // The embers in flight, two more each frame.
    for (i = 0; i != gFire_Counts[effect->variant]; i++) {
      struct EffectStep *ember = &EMBERS[i];
      s32 start;

      start = i / 2;
      if (frame > start && ember->variant == IN_FLIGHT) {
        s32 size;

        // Depth 160-799 gives sizes 10 down to 1.
        EffectPosition_ProjectPoint((s32 *)ember, &screen);
        screen.x >>= 1;
        if (screen.depth < 160)
          screen.depth = 160;
        if (screen.depth > 799)
          screen.depth = 799;
        size = 10 - (screen.depth - 160) / 64;
        draw[frame < start + 48 ? 1 : 0](
            canvas, sheet + gBattleEffect_MoteCells[size - 1],
            screen.x - size / 2, screen.y - size, size, size * 2);
        ember->x += ember->velocityX;
        ember->y += ember->velocityY;
        ember->z += ember->velocityZ;
      }

      // After 48 frames it steers toward its target by 1/512 of the
      // distance a frame.
      if (frame > start + 48 && ember->variant == IN_FLIGHT) {
        struct MotionObject *target;
        s32 vx;
        s32 vy;
        s32 vz;

        target = BattleObject_GetFar(effect->actors[i % effect->count])->object;
        vx = ember->velocityX += (target->x - ember->x) >> 9;
        vy = ember->velocityY += (target->y - ember->y) >> 9;
        vz = ember->velocityZ += (target->z - ember->z) >> 9;
        if (frame < start + 85) {
          ember->velocityX = vx * 60 / 64;
          ember->velocityY = vy * 60 / 64;
          ember->velocityZ = vz * 60 / 64;
        }

        // Below the ground it lands: it keeps its screen position for the
        // flare, its target flashes in palette 10 and the screen shakes.
        if (ember->y < 0) {
          ember->variant = 0;
          ember->x = screen.x;
          ember->y = screen.y;
          if (effect->variant == 2)
            Sound_PlayFar(134);
          else
            Sound_PlayFar(136);
          BattleEffect_TintBattler(effect->actors[i % effect->count], 10, 5,
                                   i % effect->count, 4);
          work->shakeFrames = 2;
        }
      }
    }

    // The landing flares: a picture every two frames, six in all.
    for (i = 0; i != gFire_Counts[effect->variant]; i++) {
      struct EffectStep *ember = &EMBERS[i];

      if (ember->variant >= 0 && ember->variant < 12) {
        s32 cel;
        u32 width;

        cel = ember->variant / 2;
        draw[1](canvas, work->sheet + gFire_FlareCells[cel],
                ember->x - ((width = gFire_FlareWidths[cel]) >> 1),
                ember->y + gFire_FlareBiasY[cel] - 56, width,
                gFire_FlareHeights[cel]);
        ember->variant++;
      }
    }

    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleCamera_ApplyPhasedDelta);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#define CHIPS ((struct EffectStep *)Ram_MapCellBuffer)

#define TIMING(field) gFrost_Timings[effect->variant * 3 + (field)]

// Motes swirl up around the targets while ice pillars rise one by one;
// then everything bursts.

// Setup: each mote gets an angle, a radius of 56-119 and a height of -64 to
// -33 pixels. Frame loop: until the burst the motes close in and climb five
// pixels a frame while they turn, and from frame 36 a pillar rises every
// three frames. At the burst the motes spread and fall, 32 chips leap up
// from below, and smoke rises half a pixel a frame through the height the
// motes and chips cover. The variant chooses the number of motes and
// pillars and the frame of the burst.
void BattleEffect_RunFrost(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *dots;
  DrawRectangle *draw;
  s32 count;
  s32 ymin;
  s32 ymax;
  s32 xyz[3];
  struct EffectPosition screen;
  s32 point[3];
  DrawRectangle pair[2];
  struct EffectStep *mote;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  dots = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, dots, 0, 0);
  BattleEffect_FetchRectangleBlitters(0, draw = pair);

  // A mote: x holds its angle, z its radius, y its height.
  for (i = 0; i != 64; i++) {
    struct EffectStep *seed = &work->particles[i];

    seed->x = Random16() & 0xffff;
    seed->z = (Random16() & 63) + 56;
    seed->y = ((Random16() & 31) - 64) << 16;
  }

  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (effect->side == 1)
    REG_BG2X = -0x7000;

  for (frame = 0; frame != TIMING(RISE_FRAME) + 50; frame++) {
    ymin = 120 << 16;
    ymax = 0;
    if (frame == TIMING(RISE_FRAME) + 11)
      BattlePlayback_StartFar(133);
    point[0] = 0;
    point[1] = 0;
    point[2] = 512 << 16;
    Transform_Reset();
    SceneTransform_ApplyPosition(point);
    if (frame >= 36 && frame < 64 && (frame & 3) == 0)
      Sound_PlayFar(115);
    if (frame == 85)
      Sound_PlayFar(136);
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 4 + 40)
        BattleEffect_TintBattler(effect->actors[i], 9, 5, -1, 0);
    }

    // The motes: before the burst they close in and climb to 48 pixels as
    // they turn; after it they spread and fall, and their heights bound the
    // smoke.
    if (frame < TIMING(RISE_FRAME))
      count = TIMING(RISE_MOTES);
    else
      count = 16;
    if (frame < TIMING(RISE_FRAME) + 35) {
      for (i = 0; i != count; i++) {
        if (frame > i) {
          s32 cel = i % 8;

          mote = &work->particles[i];
          if (mote->y < (48 - i / 2) << 16 && mote->y > -(48 << 16)) {
            struct EffectPosition *pos;
            u32 width;
            u32 height;

            xyz[0] = mote->z * Trig_Sin(mote->x);
            xyz[1] = mote->y;
            xyz[2] = mote->z * Trig_Cos(mote->x);
            EffectPosition_ProjectPoint(xyz, pos = &screen);
            pos->x = (pos->x >> 17) + 64;
            pos->y = HI(pos->y) + 60;
            draw[1](canvas, work->sheet + gFrost_MoteCells[cel],
                    pos->x - ((width = gFrost_MoteWidths[cel]) >> 1),
                    pos->y - ((height = gFrost_MoteHeights[cel]) >> 1), width,
                    height);
          }
          if (frame < TIMING(RISE_FRAME)) {
            if (frame > i + 16) {
              if (mote->z > 4)
                mote->z -= 2;
              if (mote->y < 48 << 16)
                mote->y += 5 << 16;
              mote->x += 0x200;
            }
          } else {
            mote->z += 8;
            mote->y -= (i % 5 + 2) << 16;
            if (ymin > mote->y)
              ymin = mote->y;
            if (ymax < mote->y)
              ymax = mote->y;
          }
        }
      }
    }
    ymin += 64 << 16;
    ymax += 64 << 16;

    // The pillars, 32 pixels wide: one more every three frames from frame
    // 36, drawn MAX for the last seven frames before the burst.
    if (frame < TIMING(RISE_FRAME)) {
      for (i = 0; i != TIMING(RISE_COLUMNS); i++) {
        if (i < (frame - 36) / 3) {
          s32 cel = i % 3;
          u32 height;

          if (frame >= TIMING(RISE_FRAME) - 7) {
            draw[1](canvas, work->sheet + gFrost_ColumnCells[cel],
                    gFrost_ColumnSpots[i * 2],
                    gFrost_ColumnSpots[i * 2 + 1] -
                        (height = gFrost_ColumnHeights[cel]),
                    32, height);
          } else {
            pair[0](canvas, work->sheet + gFrost_ColumnCells[cel],
                    gFrost_ColumnSpots[i * 2],
                    gFrost_ColumnSpots[i * 2 + 1] -
                        (height = gFrost_ColumnHeights[cel]),
                    32, height);
          }
        }
      }
    }

    // The chips: at the burst 32 leap up from below (24 are drawn).
    if (frame == TIMING(RISE_FRAME)) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *chip = &CHIPS[i];

        chip->x = (Random16() & 127) << 16;
        chip->y = ((Random16() & 15) + 80) << 16;
        chip->z = ((Random16() & 63) - 32) << 12;
        chip->velocityY = ((-Random16() & 15) - 16) << 13;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (frame >= TIMING(RISE_FRAME)) {
      for (i = 0; i != 24; i++) {
        struct EffectStep *chip = &CHIPS[i];

        if (chip->variant >= 0) {
          s32 cel = i % 8;

          draw[1](canvas, work->sheet + gFrost_ChipCells[cel], HI(chip->x),
                  HI(chip->y), gFrost_ChipWidths[cel], gFrost_ChipHeights[cel]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->variant--;
        }
        if (ymin > chip->y)
          ymin = chip->y;
        if (ymax < chip->y)
          ymax = chip->y;
      }
    }
    ymin >>= 16;
    ymax >>= 16;
    if (ymax <= ymin)
      ymax = ymin + 1;

    // The smoke: from the burst it fills the height of the motes and chips
    // and rises half a pixel a frame.
    if (frame == TIMING(RISE_FRAME)) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *puff = &work->particles[i];

        puff->velocityX = (Random16() & 127) << 16;
        if (ymax == ymin)
          puff->velocityY = ymin << 16;
        else
          puff->velocityY = (Random16() % (ymax - ymin) + ymin) << 16;
        puff->variant = (Random16() & 15) + 20;
      }
    }
    if (frame >= TIMING(RISE_FRAME)) {
      s32 rise = (frame - TIMING(RISE_FRAME)) / 2;

      for (i = 0; i != 32; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (puff->variant >= 0 && puff->variant <= 17) {
          s32 cel = (17 - puff->variant) / 2;
          u32 size;

          draw[1](canvas, dots + gBattleEffect_PuffCells[cel],
                  HI(puff->velocityX) -
                      ((size = gBattleEffect_PuffSizes[cel]) >> 1),
                  HI(puff->velocityY) - (size >> 1) - rise, size, size);
        }
        puff->variant--;
        if ((puff->variant == -1 || puff->variant == 17) &&
            frame < TIMING(RISE_FRAME) + 35) {
          puff->variant = 17;
          puff->velocityX = (Random16() & 127) << 16;
          puff->velocityY = (Random16() % (ymax - ymin) + ymin) << 16;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define SPARK_COUNT 1024

#define BOLT_SHEET 0x60e // the bolt pictures in the sheet
#define BOLT_SIZE 2880   // one 24x120 bolt
#define BOLT_COUNT 4

// Four lightning bolts turn and close in on the targets, throwing sparks
// from their feet.

// Setup: 1,024 sparks wait at the canvas center. Frame loop: for 16
// frames the bolts circle in, their radius shrinking a pixel a frame, the
// canvas wiped every five frames, and each bolt throws the variant's
// number of sparks; the sparks fall and bounce off y = 120 until frame 64.
// The targets are hit from frame 4, four frames apart.
void BattleEffect_RunRay(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 bolt;
  s32 xOffset;
  s32 count;
  DrawRectangle draw[2];
  struct EffectPosition midpoint;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  xOffset = 0;
  if (effect->side == 0)
    BattleEffect_StartTiledCanvasFilled(0x2001);
  else
    BattleEffect_StartTiledCanvas(0x2001);
  if (effect->variant == 2)
    REG_BG2PA = 0x80; // twice as wide
  else
    REG_BG2PA = 0x100;
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188,
                           BLIT_MODE(BLIT_ADDITIVE, CLIP | COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + BOLT_SHEET, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);

  // Scroll the canvas: a fixed offset for the wide variant, else onto the
  // targets' midpoint.
  if (effect->variant == 2) {
    if (work->effect->side == 1)
      REG_BG2X = -0x1000;
    else
      REG_BG2X = 0x1000;
  } else {
    BattleEffect_GetTargetsMidpoint(&midpoint);
    midpoint.x -= 64;
    if (midpoint.x < 0) {
      xOffset = midpoint.x;
      midpoint.x = 0;
    } else if (midpoint.x > 112) {
      xOffset = midpoint.x - 112;
      midpoint.x = 112;
    }
    REG_BG2X = -midpoint.x << 8;
  }

  // The sparks wait at the center.
  for (i = 0; i != SPARK_COUNT; i++) {
    struct EffectStep *spark = &SPARKS[i];
    s32 speed;
    s32 angle;

    speed = (Random16() & 0x3ff) + 256;
    angle = (Random16() & 0x7fff) - 0x4000;
    spark->x = 0x4000;
    spark->y = 0x7000;
    spark->z = (Trig_Sin(angle) * speed) >> 16;
    spark->velocityZ = -(Trig_Cos(angle) * speed * 2) >> 16;
    spark->variant = 0;
  }

  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(138);

  for (frame = 0; frame != 64; frame++) {
    if (frame == 20)
      BattlePlayback_StartFar(133);
    if (frame < 16) {
      // Wipe the canvas every five frames.
      if (frame % 5 == 2)
        Iwram_FillWordsEntry(canvas, 0x4000, 0x10101010);
      for (bolt = 0; bolt != BOLT_COUNT; bolt++) {
        s32 angle;
        s32 x;
        s32 y;

        // The radius across shrinks from 32 a pixel a frame.
        count = 0;
        angle = ((frame << 11) + 0x4000) * bolt;
        x = (((32 - frame) * Trig_Sin(angle)) >> 16) + 64;
        y = -((Trig_Cos(angle) << 3) >> 16) - 8;
        if (effect->variant == 0)
          draw[0](canvas,
                  work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
                  xOffset + x + (Random16() & 7) - 16, y, 24, 120);
        else
          draw[bolt & 1](
              canvas, work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
              xOffset + x + (Random16() & 7) - 16, y, 24, 120);

        // Throw sparks from the bolt's foot.
        for (i = 0; i != SPARK_COUNT; i++) {
          struct EffectStep *spark = &SPARKS[i];

          if (spark->variant == 0) {
            s32 speed;
            s32 spread;

            speed = (Random16() & 0x1ff) + 128;
            spread = (Random16() & 0x7fff) - 0x4000;
            spark->x = x << 16;
            spark->y = (y + 112) << 16;
            spark->velocityX = (Trig_Sin(spread) * speed) >> 9;
            spark->velocityY = -(Trig_Cos(spread) * speed * 2) >> 7;
            spark->variant = (Random16() & 7) + 32;
            count++;
            if (count == gRay_Sparks[effect->variant][1])
              break;
          }
        }
      }
      work->shakeFrames = 1;
    }

    // The sparks fall and bounce off the bottom (y 120).
    for (i = 0; i != SPARK_COUNT; i++) {
      struct EffectStep *spark = &SPARKS[i];

      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, -0x800);
        if (spark->y > 0x780000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;
          s32 size = spark->variant / 8 + 1;

          draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                  xOffset + x - size / 2, y - size, size, size * 2);
        }
      }
    }

    // The targets are hit four frames apart.
    if (frame >= 4 && frame < 96) {
      for (i = 0; i != effect->count; i++) {
        if (frame == i * 4 + 4)
          BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 10);
      }
    }
    BattleCamera_Shake(2, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#include "battle_effect_argument.h"

/// Abilities: Smoke Bomb, Sleep Bomb (kind 380).
DEFINE_EFFECT_KIND(BattleEffect_RunSmokeBomb, BattleEffect_RunEruption, 1)

/// Abilities: Blast, Mad Blast, Fiery Blast (kind 14).
DEFINE_EFFECT_KIND(BattleEffect_RunBlast, BattleEffect_RunEruption, 0)

#define MOTE_SHEET 0x3200 // the mote pictures in the sheet

// A blast of motes and smoke puffs bursts from the near or far edge of the
// field toward the targets.

// Setup: mode 1 builds a grey ramp in the first 64 background colors and
// always plays variant 1; mode 0 copies the fire palette and plays the
// effect's variant. The puffs and motes all start at the side's edge with
// random velocities. Frame loop: the motes fly and fall under gravity,
// sized by depth; the puffs set out one a frame from frame 3 and grow
// through seven pictures. The targets are hit from frame 6, a frame apart;
// in mode 0 they also recoil. The variant chooses the number of motes and
// puffs and the length.
void BattleEffect_RunEruption(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  s32 variant;
  struct EffectPosition screen;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 0);
  if (mode == 1) {
    for (i = 0; i != 64; i++) {
      s32 level = i / 2;

      BG_PLTT[i] = (level << 10) | (level << 5) | level;
    }
    variant = 1;
  } else {
    Iwram_CopyWordsEntry(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_FireStreakSheet), 128);
    variant = effect->variant;
  }

  // Everything starts 100 pixels out on the user's side.
  for (i = 0; i != 32; i++) {
    struct EffectStep *puff = &work->particles[i];

    if (effect->side == 1)
      puff->x = 200 << 14;
    else
      puff->x = -(200 << 14);
    puff->y = 0;
    puff->z = 0;
    puff->velocityX = ((Random16() & 63) - 32) << 13;
    puff->velocityY = ((Random16() & 63) + 16) << 12;
    puff->velocityZ = ((Random16() & 63) - 32) << 13;
    puff->variant = 0;
  }
  for (i = 0; i != 1024; i++) {
    struct EffectStep *mote = &MOTES[i];

    if (effect->side == 1)
      mote->x = 200 << 14;
    else
      mote->x = -(200 << 14);
    mote->y = 0;
    mote->z = 0;
    mote->velocityX = ((Random16() & 63) - 32) << 13;
    mote->velocityY = ((Random16() & 31) + 8) << 13;
    mote->velocityZ = ((Random16() & 63) - 32) << 13;
    mote->variant = 0;
  }

  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != gEruption_Counts[variant][FIELD_FRAMES]; frame++) {
    struct BattleCamera *camera;

    camera = Ram_HeapSlots->cameraWork;
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame == 2)
      Sound_PlayFar(144);
    if (frame == gEruption_Counts[variant][FIELD_FRAMES] - 48)
      BattlePlayback_StartFar(133);

    // The motes: depth 160-799 gives sizes 9 down to 1.
    for (i = 0; i != gEruption_Counts[variant][FIELD_MOTES]; i++) {
      struct EffectStep *mote = &MOTES[i];

      if (mote->y >= 0) {
        s32 size;

        EffectPosition_ProjectPoint((s32 *)mote, &screen);
        screen.x >>= 1;
        screen.x += effect->side * 32 - 16;
        if (screen.depth < 160)
          screen.depth = 160;
        if (screen.depth > 799)
          screen.depth = 799;
        size = 9 - (screen.depth - 160) / 64;
        draw[0](canvas,
                work->sheet +
                    (gBattleEffect_MoteCells[size - 1] + (i & 1) * 770) +
                    MOTE_SHEET,
                screen.x - size / 2, screen.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(mote, 64, -0x2000);
      }
    }

    // The puffs: one more each frame; a picture every three frames.
    if (frame > 2) {
      for (i = 0; i != gEruption_Counts[variant][FIELD_PUFFS]; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (i < frame && puff->y >= 0) {
          EffectPosition_ProjectPoint((s32 *)puff, &screen);
          screen.x >>= 1;
          screen.x += effect->side * 32 - 16;
          if (puff->variant >= 0 && puff->variant <= 20) {
            s32 cel = puff->variant / 3;
            u32 width;

            draw[0](canvas, work->sheet + gEruption_PuffCells[cel],
                    screen.x - ((width = gEruption_PuffSizes[cel]) >> 1),
                    screen.y - (width >> 1), width, width);
          }
          if (puff->variant <= 20)
            puff->variant++;
          BattleEffectKernels_IntegrateVector3(puff, 64, -0x2000);
        }
      }
    }

    // Hit the targets a frame apart; in mode 0 they recoil too.
    if (mode == 0) {
      for (i = 0; i != effect->count; i++) {
        if (frame == i + 6) {
          BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 10);
          BattleMotion_ApplyKnockbackFar(effect->actors[i], 2);
        }
      }
    } else {
      for (i = 0; i != effect->count; i++) {
        if (frame == i + 6)
          BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 10);
      }
    }
    if (frame == 2)
      work->shakeFrames = 6;
    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#include "field_sprite.h"

/// Creates entryCount sprites of one resource in the effect work's object
/// slots, each showing its own animation at the given priority (The Broken
/// Seal's BattleEffect_SpawnObjects).
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  s32 entryIndex = 0;

  if (entryCount == 0)
    return;
  do {
    struct FieldSprite *object = (struct FieldSprite *)Sprite_CreateFar(kind);

    work->objects[entryIndex] = object;
    if (object != NULL) {
      object->nextFrame = 0;
      Animation_ApplyChildArgumentFar(object, entryIndex);
      ((struct FieldSprite *)work->objects[entryIndex])->priority = variant;
    }
    entryIndex++;
  } while (entryIndex != entryCount);
}

void BattleEffect_DoNothing(void) {}

/// Restarts DMA channel 0 as an H-blank transfer of the effect work's line
/// table into BG2X, one word a line.
// The Broken Seal's BattleEffect_ArmBg2AffineHBlankDma.
void BattleEffect_ArmBg2AffineHBlankDma(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  volatile u16 *channel = REG_DMA0;
  u16 control = channel[5];
  u32 io;

  channel[5] = control & 0xc5ff;
  channel[5] &= 0x7fff;
  io = 0x80;
  (void)channel[5];
  Dma_Set(work->bg2xLines, (void *)((io << 19) + 0x28),
          DMA_HBLANK_WORD_PER_LINE, (volatile u32 *)channel);
}

/// BattleEffect_ArmBg2AffineHBlankDma for BG1HOFS and BG1VOFS: the line table
/// moves layer 1.
void BattleEffect_ArmBg1HBlankDma(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  volatile u16 *channel = REG_DMA0;
  u16 control = channel[5];
  u32 io;

  channel[5] = control & 0xc5ff;
  channel[5] &= 0x7fff;
  io = 0x80;
  (void)channel[5];
  Dma_Set(work->bg2xLines, (void *)((io << 19) + 0x14),
          DMA_HBLANK_WORD_PER_LINE, (volatile u32 *)channel);
}

#include "field_event.h"

#define Actor_SetAnimationSpeedFar                                             \
  ((s32 (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode0,
                   BattleEffect_RunProjectileVolleyB, 0)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode12,
                   BattleEffect_RunProjectileVolleyB, 12)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode2,
                   BattleEffect_RunProjectileVolleyB, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode6,
                   BattleEffect_RunProjectileVolleyB, 6)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode3,
                   BattleEffect_RunProjectileVolleyB, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode5,
                   BattleEffect_RunProjectileVolleyB, 5)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode23,
                   BattleEffect_RunProjectileVolleyB, 23)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode7,
                   BattleEffect_RunProjectileVolleyB, 7)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode21,
                   BattleEffect_RunProjectileVolleyB, 21)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode4,
                   BattleEffect_RunProjectileVolleyB, 4)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode8,
                   BattleEffect_RunProjectileVolleyB, 8)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode24,
                   BattleEffect_RunProjectileVolleyB, 24)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode22,
                   BattleEffect_RunProjectileVolleyB, 22)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode9,
                   BattleEffect_RunProjectileVolleyB, 9)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode13,
                   BattleEffect_RunProjectileVolleyB, 13)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode14,
                   BattleEffect_RunProjectileVolleyB, 14)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode15,
                   BattleEffect_RunProjectileVolleyB, 15)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode16,
                   BattleEffect_RunProjectileVolleyB, 16)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode17,
                   BattleEffect_RunProjectileVolleyB, 17)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode18,
                   BattleEffect_RunProjectileVolleyB, 18)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode19,
                   BattleEffect_RunProjectileVolleyB, 19)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode20,
                   BattleEffect_RunProjectileVolleyB, 20)

void BattleEffect_RunVolleyBMode10WithAction(
    struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolleyB(effect, 10);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunVolleyBMode11WithAction(
    struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolleyB(effect, 11);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

// BattleEffect_RunProjectileVolleyB, the body behind the 22 wrappers
// above (23 effect kinds, modes 0-24).

// New in The Lost Age. Its nearest relative is The Broken Seal's
// BattleEffect_RunProjectileVolley (effect.c; shape 0.44, structure 0.53): the
// same frame: a shot launched each frame from the user toward one target
// after another, every live shot drawn and moved, the targets hit four
// frames apart, and a fade over the last eight frames. The mode chooses
// the picture and how it is prepared (holes punched at random, a level
// remap, a doubled beam, a grey or violet ramp), the palette, the blending,
// how many shots and where they aim; modes 5 and 23 draw one big picture at
// the user instead, mode 10 also draws motes into the user, mode 24 adds
// glints around the targets, and modes 11, 14 and 18 hand over to
// SummonEffect_RunCanvasEffect at the end. Resources not yet named in
// directory.s, by row and asset: 0x151 BattleEffect_MarsDjinnSmallSheet,
// 0x157 BattleEffect_VenusDjinnSmallSheet, 0x11c
// BattleEffect_VioletLightningSheetA, 0xd7 Resource_Data0D7, 0x14d
// BattleEffect_BoulderSheet, 0x116 BattleEffect_DemonFaceSheet, 0x16e
// BattleEffect_HeartSheet, 0x161 BattleEffect_SmokeSheet, 0x13e
// BattleEffect_FlashBurstSheet, 0x12f BattleEffect_WaterSpraySheet, 0x164
// BattleEffect_PinkPalette, 0x187 BattleEffect_RockSpireSheet, 0x150
// BattleEffect_MarsDjinnSheet, 0x148 BattleEffect_OrangePaletteB, 0x163
// BattleEffect_LimePalette, 0x166 BattleEffect_CyanPalette, 0x184
// BattleEffect_BlastSheet, 0x190 BattleEffect_BlueBeamSheet and 0xb9
// BattleEffect_EffectPalette01.

// Forms that decided the allocation: i is one counter for every r9 loop;
// user is a pointer spilled to sp+60; targets and aim are arrays; widths and
// heights are set inside the glint loop, their NULL defaults keeping loop.c
// from hoisting them; height 12 is hoisted only as a local; a launch with
// x += dx << 12 sends the whole allocation away.

#define DJINNI_BEHIND 64

// A volley of shots flies from the user at the targets, one launched each
// frame toward the next target.

// Setup: an optional Djinni appears behind the user (the prelude); the
// mode prepares its picture and palette and moves the launch point. Frame
// loop: a shot leaves the user each frame for 48 frames (16 in mode 10),
// aimed near the next target; every live shot is drawn and moved and goes
// out after 24 frames (32 in modes 9 and 17); from frame 16 the targets
// are hit four frames apart and recoil; the blend fades out over the last
// eight frames.
void BattleEffect_RunProjectileVolleyB(struct BattleEffectArgument *effect,
                                       u32 mode) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  s32 target;
  s32 i;
  s32 j;
  u8 *widths;
  u8 *heights;
  s32 limit;
  s32 row;
  s32 column;

  u8 *palette;
  s32 origin[3];
  s32 djinniTarget[3];
  struct EffectPosition userPosition;
  s32 targets[8][3];
  s32 aim[3];
  s32 screen[3];
  DrawRectangle pair[2];

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];
  if (mode == 10 || mode == 11 || mode == 13 || mode == 14 || mode == 15 ||
      mode == 17 || mode == 18 || mode == 19)
    BattleEffect_StartCanvasLayer(1);
  else
    BattleEffect_StartCanvasLayer(0);

  // The prelude: a Djinni behind the user, Mars for modes 9, 22 and 24 and
  // Venus otherwise.
  if (effect->prelude == 1) {
    if (mode == 9 || mode == 22 || mode == 24) {
      BattleEffect_RunDjinniEntrance(
          effect, DJINNI_MARS | DJINNI_SIDE(effect->side) | DJINNI_BEHIND,
          origin, djinniTarget);
      Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                                 work->sheet + VOLLEY_B_DJINNI, 1, 0);
    } else {
      BattleEffect_RunDjinniEntrance(
          effect, DJINNI_VENUS | DJINNI_SIDE(effect->side) | DJINNI_BEHIND,
          origin, djinniTarget);
      Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                                 work->sheet + VOLLEY_B_DJINNI, 1, 0);
    }
    REG_BG2CNT = 0x785;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  REG_BLDALPHA = 0x1010;

  // The blitters: [0] MAX and [1] ADDITIVE, mirrored for the far side.
  if (effect->prelude == 1) {
    if (effect->side == 0) {
      BattleEffect_FetchRectangleBlitters(0, pair);
    } else {
      BattleEffect_LoadBlitter(104,
                               BLIT_MODE(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
      pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      BattleEffect_LoadBlitter(
          188, BLIT_MODE(BLIT_ADDITIVE, CLIP | COMPOSITE_FLIP_X));

      pair[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    }
  } else if (mode == 20) {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_TRANSPARENT, CLIP));
    pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(
        188, BLIT_MODE(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));

    pair[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    REG_BLDALPHA = 0x310;
  } else if (mode == 7 || mode == 21) {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, CLIP));
    pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));

    pair[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  } else {
    BattleEffect_FetchRectangleBlitters(effect->side, pair);
  }

  // The pictures and the colors.
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (mode == 5 || mode == 23) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetA,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
  } else if (mode == 9 || mode == 17) {
    // Remap the levels: 0 stays, 1-16 become 1, the rest drop by 16.
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D7, work->sheet, 0, 0);
    for (i = 0; i != 0xc00; i++) {
      u8 *cell = work->sheet + i;
      u32 value = *cell;
      u8 level = value;

      if (level == 0)
        *cell = 0;
      else if (level <= 16)
        *cell = 1;
      else
        *cell = value - 16;
    }
    if (mode == 17)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
  } else if (mode == 20) {
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
  } else if (mode == 7) {
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
  } else if (mode == 22) {
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (mode == 21) {
    // A violet ramp from color 1, blended 16:4.
    for (i = 1; i != 64; i++) {
      s32 low = 15 - i / 4;
      s32 value = ((31 - i / 2) << 10) | low;

      BG_PLTT[i] = value;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 0);
    REG_BLDALPHA = 0x410;
  } else if (mode == 18) {
    Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (mode == 19) {
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (mode == 24) {
    // Six smoke pictures, each with more holes punched at random.
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 0);
    for (i = 0; i != 6; i++) {
      for (j = 0; j != i * 256 + 512; j++)
        work->sheet[VOLLEY_B_PICTURE + i * 1152 + Random16() % 1152] = 0;
    }
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (mode == 12 || mode == 16) {
    // The beam, each 24-pixel row drawn twice side by side.
    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet, work->sheet, 1,
                               1);
    for (row = 0; row != 24; row++) {
      for (column = 0; column != 24; column++) {
        work->sheet[VOLLEY_B_BEAM + row * 48 + column] =
            work->sheet[row * 24 + column];
        work->sheet[VOLLEY_B_BEAM + row * 48 + 24 + column] =
            work->sheet[row * 24 + column];
      }
    }
    if (mode == 12)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_LimePalette),
                      128);
    else
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet),
                      128);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
    if (mode == 6) {
      // A grey ramp, unblended.
      for (i = 0; i != 64; i++) {
        s32 level = i / 4;

        BG_PLTT[i] = (level << 10) | (level << 5) | level;
      }
      REG_BLDCNT = 0;
    } else {
      switch (mode) {
      case 13:
      case 14:
      case 15:
        // Six flash pictures, each with more holes punched at random.
        for (i = 0; i != 6; i++) {
          for (j = 0; j != i * 512 + 1024; j++)
            work->sheet[VOLLEY_B_PICTURE + (i << 11) + (Random16() & 0x7ff)] =
                0;
        }
        if (mode == 15) {
          REG_BLDCNT = 0;
          for (i = 0; i != 64; i++) {
            s32 level = i / 4;

            BG_PLTT[i] = (level << 10) | (level << 5) | level;
          }
          palette = NULL;
        } else if (mode == 13) {
          REG_BLDCNT = 0;
          palette = Resource_GetTableEntry((s32)&ResourceId_RockSpireSheet);
        } else {
          palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
        }
        break;
      case 0:
        palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
        break;
      case 1:
        palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
        break;
      case 2:
      case 16:
        palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
        break;
      case 3:
        palette = Resource_GetTableEntry((s32)&ResourceId_PinkPalette);
        break;
      case 8:
        palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
        break;
      case 10:
        palette = Resource_GetTableEntry((s32)&ResourceId_EffectPalette01);
        break;
      case 11:
        palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
        break;
      default:
        palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
        break;
      }
      if (palette != NULL) {
        // The entry held in a local loads before the arguments; as a plain
        // Iwram_CopyWords call, 2 of 2239 instructions differ in tla-en.
        __typeof__(Iwram_CopyWordsEntry) routine = Iwram_CopyWordsEntry;
        routine((void *)BG_PLTT, palette, 128);
      }
    }
  }

  // The trail: none for the rock modes, else halved or to 3/4.
  if (mode == 13 || mode == 15 || mode == 20) {
    work->transferMode = 1;
    work->transferValue = 0;
  } else if (mode == 7 || mode == 9 || mode == 17 || mode == 18 || mode == 19 ||
             mode == 24) {
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  for (i = 0; i != 64; i++)
    work->particles[i].variant = -1;
  {
    s32 shots;
    struct EffectPosition *user;
    s32 k;
    if (mode == 8 || mode == 11 || mode == 14)
      shots = 32;
    else if (mode == 20)
      shots = 64;
    else if (mode == 18)
      shots = 48;
    else
      shots = 56;

    // Where the shots leave from, and where the targets are.
    EffectPosition_ProjectSprite(effect->actor, user = &userPosition);
    for (i = 0; i != effect->count; i++)
      EffectPosition_ProjectScaledBattler(effect->actors[i],
                                          (struct EffectPosition *)targets[i]);
    if (mode == 12 || mode == 15) {
      if (effect->side == 0)
        user->x -= 24;
      else
        user->x += 24;
    }
    if (mode == 20) {
      if (effect->side == 0)
        user->x -= 48;
      else
        user->x += 48;
    }
    if (mode == 21) {
      if (effect->side == 0)
        user->x -= 16;
      else
        user->x += 16;
      user->y -= 40;
    }
    if (mode == 16) {
      if (effect->side == 0)
        user->x -= 32;
      else
        user->x += 32;
    }
    if (mode == 3)
      user->y -= 16;
    if (mode == 8)
      user->y -= 40;
    if (mode == 24)
      user->y -= 48;
    if (mode == 22)
      user->y -= 40;
    if (mode == 9)
      user->y -= 40;
    if (mode == 17)
      user->y -= 16;
    if (mode == 19)
      user->y -= 16;
    if (mode == 10) {
      user->y -= 24;
      for (i = 0; i != effect->count; i++)
        targets[i][1] += 80;
    }
    if (mode == 4) {
      if (effect->side == 1)
        user->x += 28;
      else
        user->x -= 28;
    }
    if (mode == 7) {
      if (effect->side == 1)
        user->x += 16;
      else
        user->x -= 16;
    }
    if (mode == 5 || mode == 23) {
      user->x /= 3;
      if (mode == 23)
        user->y -= 16;
      REG_BG2PA = 0x55; // three times as wide
    }

    // Mode 10: 128 motes burst from the middle of the first target.
    if (mode == 10) {
      struct MotionObject *object;
      s32 half;

      object = BattleObject_GetFar(effect->actors[0])->object;
      half = BattleUnit_GetEffectHeightFar(effect->actors[0]) / 2;
      for (i = 0; i != 128; i++) {
        struct EffectStep *mote = &MOTES[i];

        mote->x = object->x;
        mote->y = object->y + half;
        mote->z = object->z;
        mote->velocityX = ((Random16() & 0xff) - 128) << 10;
        mote->velocityY = ((Random16() & 0xff) - 128) << 10;
        mote->velocityZ = ((Random16() & 0xff) - 128) << 10;
        mote->variant = 0;
      }
    }

    for (frame = 0; frame != shots; frame++) {
      target = frame % effect->count;

      // Fade out over the last eight frames.
      if (frame >= shots - 8)
        REG_BLDALPHA = -((frame - shots) * 2) | 0x1000;
      if (mode == 8 && frame > 15)
        BattleEffect_StepPaletteToResource((s32)&ResourceId_CyanPalette);
      if (mode == 10 && frame > 31)
        BattleEffect_StepPaletteToResource((s32)&ResourceId_BlastSheet);
      if (frame == 4) {
        if (mode == 9 || mode == 17)
          Sound_PlayFar(92);
        else
          Sound_PlayFar(136);
      }
      if (mode == 8) {
      } else if (mode == 6) {
        if (frame == 60)
          BattlePlayback_StartFar(134);
      } else if (mode == 24) {
        if (frame == 48)
          BattlePlayback_StartFar(134);
      } else if (frame == 24) {
        BattlePlayback_StartFar(134);
      }

      // The Djinni sways, and after frame 56 rises two pixels a frame.
      if (effect->prelude == 1) {
        s32 x = ((-Trig_Sin(frame << 11) * 4) >> 16) + origin[0] / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 2) >> 16) + origin[1] - 24;

        if (frame > 56)
          y = y - frame * 2 + 112;
        if (frame <= 3)
          pair[1](canvas, work->sheet + VOLLEY_B_DJINNI, x, y, 20, 40);
        pair[1](canvas, work->sheet + VOLLEY_B_DJINNI, x, y, 20, 40);
      }

      if (mode == 5 || mode == 23) {
        // One big picture at the user, three frames a cel.
        if (effect->side == 1)
          pair[0](canvas,
                  work->sheet + VOLLEY_B_PICTURE + frame / 3 % 3 * 0x1200,
                  user->x - 2, user->y - 32, 72, 62);
        else
          pair[0](canvas,
                  work->sheet + VOLLEY_B_PICTURE + frame / 3 % 3 * 0x1200,
                  user->x - 70, user->y - 32, 72, 62);
      } else {
        // Launch a shot at a point near this frame's target.
        aim[0] = targets[target][0] + (Random16() & 31) - 16;
        if (mode == 9 || mode == 17 || mode == 19)
          aim[1] = targets[target][1] + (Random16() & 127) - 32;
        else if (mode == 12 || mode == 15 || mode == 18 || mode == 20 ||
                 mode == 21 || mode == 22 || mode == 16)
          aim[1] = targets[target][1] + (Random16() & 127) - 64;
        else if (mode == 24)
          aim[1] = targets[target][1] + Random16() % 96 - 32;
        else if (mode == 11) {
          aim[1] = targets[target][1] + (Random16() & 255) - 192;
          aim[1] =
              targets[target][1] + ((Trig_Sin(frame << 12) << 8) >> 16) - 128;
        } else
          aim[1] = targets[target][1] + (Random16() & 63) - 16;
        limit = 16;
        if (mode != 10)
          limit = 48;
        if (frame < limit) {
          struct EffectStep *shot = &work->particles[frame];
          s32 x = user->x << 15;
          s32 y;
          s32 dx;
          s32 dy;

          shot->x = x;
          y = user->y << 16;
          shot->y = y;
          dx = aim[0] - user->x;
          shot->velocityX = dx << 11;
          dy = aim[1] - user->y;
          shot->velocityY = dy << 11;
          shot->x = x + (dx << 12);
          shot->y = y + (dy << 12);
          shot->variant = 0;
        }
      }

      // Draw and move the shots in flight.
      for (i = 0; i != 64; i++) {
        struct EffectStep *shot = &work->particles[i];
        s32 age = shot->variant;
        s32 cel;

        if (age < 0)
          continue;
        if (mode == 7 || mode == 21) {
          if (mode == 22 && (i & 1))
            continue;
          if (age > 7)
            pair[effect->side](canvas, work->sheet + VOLLEY_B_PICTURE,
                               HI(shot->x) - 16, HI(shot->y) - 32, 32, 64);
        } else if (mode == 22) {
          if (i & 1)
            continue;
          if (age > 2)
            pair[0](canvas, work->sheet + VOLLEY_B_PICTURE, HI(shot->x) - 16,
                    HI(shot->y) - 32, 32, 64);
        } else if (mode == 12 || mode == 16) {
          if (age > 1)
            pair[0](canvas, work->sheet + VOLLEY_B_BEAM, HI(shot->x) - 12,
                    HI(shot->y) - 24, 24, 48);
        } else if (mode == 9 || mode == 17) {
          cel = i / 2 % 3;

          // Every other shot, wobbling up and down.
          if (i & 1) {
            s32 y = shot->y + (Trig_Sin((i * 2 + frame) << 11) << 5);

            pair[1](canvas, work->sheet + cel * 1024, HI(shot->x) - 16,
                    (y >> 16) - 16, 32, 32);
          }
        } else if (mode == 20) {
          s32 cel = i % 5 * 3 + age % 3;
          u32 width = gSpire_ShardWidths[cel];
          s32 x = HI(shot->x);
          u32 half = width >> 1;
          const u8 *shardHeights = gData_081974ad;
          u32 height;
          s32 y;
          x -= half;
          height = shardHeights[cel];
          y = HI(shot->y) - (height >> 1);

          pair[0](canvas, work->sheet + gSpire_ShardCells[cel] + VOLLEY_B_GLOW,
                  x, y, width, height);
        } else if (mode == 19) {
          s32 wobble = Trig_Sin((i * 2 + frame) << 11);
          s32 y;
          cel = age >> 2;
          y = (shot->y + (wobble << 5)) >> 16;

          pair[1](canvas, work->sheet + cel * 1152, HI(shot->x) - 12, y - 24,
                  24, 48);
        } else if (mode == 18) {
          if (age > 1)
            pair[0](canvas, work->sheet, HI(shot->x) - 11, HI(shot->y) - 21, 22,
                    42);
        } else if (mode == 4) {
          if (age > 5)
            pair[0](canvas, work->sheet + VOLLEY_B_PICTURE + age / 4 * 2048,
                    HI(shot->x) - 16, HI(shot->y) - 32, 32, 64);
        } else if (mode == 24) {
          if (age > 1)
            pair[1](canvas, work->sheet + VOLLEY_B_PICTURE + age / 4 * 1152,
                    HI(shot->x) - 12, HI(shot->y) - 24, 24, 48);
        } else if (mode != 5) {
          if (age > 1)
            pair[0](canvas, work->sheet + VOLLEY_B_PICTURE + age / 4 * 2048,
                    HI(shot->x) - 16, HI(shot->y) - 32, 32, 64);
        }
        shot->x += shot->velocityX;
        shot->y += shot->velocityY;
        if (mode == 10)
          shot->velocityY -= 0x4000;
        shot->variant++;
        if (mode == 9 || mode == 17) {
          if (shot->variant == 32)
            shot->variant = -1;
        } else if (shot->variant == 24) {
          shot->variant = -1;
        }
      }

      // Mode 24: three glints a frame around each target being hit.
      if (mode == 24) {
        for (i = 0; i != effect->count; i++) {
          if (frame >= i * 4 + 16 && frame < shots - 8) {
            // Default: set for real below.
            widths = NULL;
            heights = NULL;
            for (k = 0; k != 3; k++) {
              s32 cel = i & 3;
              s32 angle;
              s32 x;
              s32 y;

              // Default: set for real below.
              widths = NULL;
              heights = NULL;
              angle = Random16() & 0xffff;
              x = (Trig_Sin(angle) << 4) >> 16;
              x += targets[i][0] / 2;
              widths = gData_08197492;
              x -= widths[cel] >> 1;
              y = (Trig_Cos(angle) << 4) >> 16;
              y += targets[i][1];
              heights = gData_08197498;
              y -= heights[cel] >> 1;

              HeapSlot_Release(188);
              HeapSlot_Release(104);
              BattleEffect_BuildBlitter(
                  188, 7, 7,
                  gTargetStrike_GlintDrawFlags[Random16() & 3] | CLIP,
                  BLIT_ADDITIVE);
              ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
                  canvas, work->sheet + gBattleEffect_GlintCells[cel], x, y,
                  widths[cel], heights[cel]);
              HeapSlot_Release(188);
              BattleEffect_FetchRectangleBlitters(effect->side, pair);
            }
          }
        }
      }

      // The hits.
      if (mode == 9 || mode == 17) {
      } else if (mode == 5 || mode == 23) {
        for (i = 0; i != effect->count; i++) {
          if (frame >= i * 4 + 2 && (frame & 7) == i) {
            work->shakeFrames = 8;
            BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 4);
          }
        }
      } else if (mode == 10) {
        if (frame == 18) {
          work->shakeFrames = 4;
          BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 4);
        }
        if (frame == 46)
          BattleEffect_TintBattler(effect->actor, 7, -1, -1, 0);
        if (frame == 54)
          BattleEffect_TintBattler(effect->actor, 0, -1, -1, 0);
      } else if (mode == 8) {
        if (frame == 20) {
          for (i = 0; i != effect->count; i++)
            BattleEffect_TintBattler(effect->actors[i], 6, 5, i, 60);
        }
      } else {
        for (i = 0; i != effect->count; i++) {
          if (frame >= i * 4 + 16 && frame < shots - 8 && (frame & 3) == i) {
            work->shakeFrames = 8;
            if (mode == 6)
              BattleEffect_TintBattler(effect->actors[i], 14, 5, i, 3);
            else
              BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 3);
            if (mode != 11 && mode != 14)
              BattleEffect_StartActorMotion(effect->actors[i], 1, 0x10000,
                                            0x40000, 0x20000, frame + 100);
          }
        }
      }

      // Mode 10: from frame 16 the motes are drawn into the user, and go out
      // once past it.
      if (mode == 10 && frame > 15) {
        struct MotionObject *object;
        s32 half;
        s32 *camera;

        object = BattleObject_GetFar(effect->actor)->object;
        half = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
        camera = Ram_HeapSlots->cameraWork;
        Transform_Reset();
        Transform_SetLookAt(camera, camera + 3);
        for (i = 0; i != 24; i++) {
          struct EffectStep *mote = &MOTES[i];
          s32 height = 12;

          if (mote->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)mote,
                                        (struct EffectPosition *)screen);
            screen[0] >>= 1;
            pair[0](canvas, sheet + gBattleEffect_MoteCells[5], screen[0] - 3,
                    screen[1] - height / 2, 6, height);
            BattleEffectKernels_IntegrateVector3(mote, 62, 0);
            if (frame > i + 16) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = object->x - mote->x;
              dy = object->y + half - mote->y;
              dz = object->z - mote->z;
              /* FAKEMATCH(barrier): an empty asm through dy adds the references
               * that rank it above dz and dx; without it, 18 of 2239
               * instructions differ in tla-en. */
              __asm__("" : "=r"(dy) : "0"(dy));
              /* FAKEMATCH(barrier): an empty asm through dz ranks it above dx,
               * which takes the register after it; without it, 16 of 2239
               * instructions differ in tla-en. */
              __asm__("" : "=r"(dz) : "0"(dz));
              mote->velocityX += dx >> 7;
              mote->velocityY += dy >> 8;
              mote->velocityZ += dz >> 8;
              if ((object->x < 0 ? -object->x : object->x) <
                  (mote->x < 0 ? -mote->x : mote->x))
                mote->variant = -1;
            }
          }
        }
      }
      BattleCamera_Shake(4, 4);
      if (mode != 6)
        BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    if (mode == 11 || mode == 14 || mode == 18) {
      // Hand over to the canvas effect: clear both canvases first.
      Iwram_ClearWords((void *)0x06004000, 0x3c00);
      Iwram_ClearWords(canvas, 0x3c00);
      effect->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      if (mode == 14)
        SummonEffect_RunCanvasMode1(effect);
      else
        SummonEffect_RunCanvasMode3(effect);
    } else {
      BattleEffect_EndCanvasLayer();
    }
  }
}

#define STARS ((struct EffectStep *)Ram_MapCellBuffer)

// Sixty-four stars swirl about a point in front of the camera and are
// drawn in.

// Setup: 256 stars are scattered up to 127 pixels each way. Frame loop
// (160 frames): four stars a frame join, each turning about one of four
// axes as fast as its number, sized by depth; thirty frames after joining
// a star accelerates toward the center by 1/256 of its distance a frame.
void BattleEffect_RunSwirlingStars(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 record[3];
  struct EffectPosition pos;
  DrawRectangle draw[2];
  struct EffectStep *star;
  s32 i;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(effect->side ^ 1, draw);
  for (i = 0; i != 256; i++) {
    star = &STARS[i];
    star->x = ((Random16() & 0xff) - 127) << 16;
    star->y = ((Random16() & 0xff) - 127) << 16;
    star->z = ((Random16() & 0xff) - 127) << 16;
    star->velocityX = 0;
    star->velocityY = 0;
    star->velocityZ = 0;
    star->variant = 0;
  }
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  record[0] = 0;
  record[1] = 160 << 15;
  record[2] = 0;
  for (frame = 0; frame != 160; frame++) {
    s32 *camera;

    // The stars turn about a point 80 pixels in front of the camera.
    camera = Ram_HeapSlots->cameraWork;
    Transform_Reset();
    Transform_SetLookAt(camera, camera + 3);
    SceneTransform_ApplyPosition(record);
    star = STARS;
    for (i = 0; i != 64; i++, star++) {
      if (frame > i / 4 && star->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case 0:
          Transform_ApplyYaw(frame * (i + 32) * 8);
          break;
        case 1:
          Transform_ApplyPitch(-frame * (i + 32) * 8);
          break;
        case 2:
          Transform_ApplyRoll(-frame * (i + 32) * 8);
          break;
        case 3:
          Transform_ApplyPitch(-frame * (i + 32) * 8);
          Transform_ApplyRoll(-frame * (i + 32) * 8);
          break;
        }
        EffectPosition_ProjectPoint((s32 *)star, &pos);
        pos.x >>= 1;
        Transform_Pop();

        // Depth 250-634 gives sizes 9 down to 3.
        if (pos.depth < 250)
          pos.depth = 250;
        if (pos.depth > 0x27a)
          pos.depth = 0x27a;
        size = 9 - (pos.depth - 250) / 64;
        draw[0](canvas,
                work->sheet +
                    ((i & 3) * 770 + gBattleEffect_MoteCells[size - 1]),
                pos.x - size / 2, pos.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(star, 60, 0);

        // Thirty frames in, the star is drawn toward the center.
        if (frame > i / 4 + 30) {
          s32 dx = -star->x >> 8;
          s32 dy = -star->y >> 8;
          s32 dz = -star->z >> 8;

          star->velocityX += dx;
          star->velocityY += dy;
          star->velocityZ += dz;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#define BURST_IMAGE 0x1b00 // the burst pictures, after the smoke

// Bursts of smoke rise from each target in turn, eight frames apart.

// Setup: 64 puffs, six a target, each but the sixth thrown out at a random
// heading. Frame loop (96 frames): for 40 frames from its start a
// target's puffs rise 40 pixels above it through six 24x48 pictures; the
// sixth, drawn ADDITIVE, is the burst itself and falls faster.
void BattleEffect_RunTargetBursts(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  void *palette;
  s32 frame;
  DrawRectangle rectangleB;
  DrawRectangle rectangleA;
  struct EffectStep *step;
  s32 i;
  struct EffectPosition screen;
  s32 record[3];

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage,
                             work->sheet + BURST_IMAGE, 0, 0);
  palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  Iwram_CopyWordsEntry((void *)0x05000000, palette, 128);
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_ADDITIVE, CLIP));
  rectangleA = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, CLIP));
  rectangleB = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  for (i = 0; i != 64; i++) {
    s32 angle;
    s32 magnitude;

    step = &work->particles[i];
    angle = Random16() & 0xffff;
    magnitude = Random16() & 0xff;
    step->x = 0;
    step->y = 0;
    step->z = 0;
    if (i % 6 == 5) {
      step->velocityX = 0;
      step->velocityY = 0;
    } else {
      step->velocityX = (Trig_Sin(angle) * magnitude) >> 7;
      step->velocityY = (Trig_Cos(angle) * magnitude) >> 9;
    }
    step->velocityZ = 0;
    step->variant = 0;
  }

  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != 96; frame++) {
    s32 member;
    s32 *camera;

    camera = Ram_HeapSlots->cameraWork;
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *memberObject;

      // The puffs rise from 40 pixels above the target.
      memberObject = BattleObject_GetFar(effect->actors[member])->object;
      Transform_Reset();
      Transform_SetLookAt(camera, camera + 3);
      record[0] = memberObject->x;
      record[1] = 160 << 13;
      record[2] = memberObject->z;
      SceneTransform_ApplyPosition(record);
      if (frame >= member * 8 && frame < member * 8 + 40) {
        struct EffectStep *p;

        for (i = 0; i != 6; i++) {
          s32 cel;

          // A picture every six frames, holding the last.
          p = &work->particles[member * 6 + i];
          cel = p->variant / 6;
          if (cel > 5)
            cel = 5;
          EffectPosition_ProjectPoint((s32 *)p, &screen);
          screen.x >>= 1;
          if (i == 5) {
            BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
            rectangleA(canvas, work->sheet + cel * 0x480 + BURST_IMAGE,
                       screen.x - 12, screen.y - 36, 24, 48);
          } else {
            BattleEffectKernels_IntegrateVector3(p, 60, 0x200);
            rectangleB(canvas, work->sheet + cel * 0x480, screen.x - 12,
                       screen.y - 36, 24, 48);
          }
          p->variant += 1;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#undef TRAILS

#include "vram_tab.h"

// The blitter slots read as what they hold, a drawing routine.
#define ALTERNATE_BLITTER_SLOT                                                 \
  (((struct {                                                                  \
     u8 before[188];                                                           \
     DrawRectangle draw;                                                       \
   } *)Ram_HeapSlots)                                                          \
       ->draw)

// A sprite object as EffectSprite_CreateWithTiles returns it.
struct SpriteObject {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[6];
  u8 slot; // its row of gResourceTableEntries
};

/* VIEW(cybele): routine EffectSprite_CreateWithTiles as struct SpriteObject
 * *EffectSprite_CreateWithTiles(s32 width, s32 height, u32 shape, u32
 * attributes); battle_effect/ability_effects has struct EffectSprite
 * *EffectSprite_CreateWithTiles(s32 width, s32 height, s32 attributes, s32
 * flags) {; tried as the definition's struct EffectSprite
 * *EffectSprite_CreateWithTiles(s32 width, s32 height, s32 attributes, s32
 * flags), which the object already declares, it does not compile in tla-ja
 * (make modern): games/tla/include/module/battle_effect/battle_effect.h:210:64:
 * note: expected 'struct BattleEffectArgument *' but argument is of type 'u8
 * *'. */
struct SpriteObject *EffectSprite_CreateWithTiles__cybele(
    s32 width, s32 height, u32 shape,
    u32 attributes) __asm__("EffectSprite_CreateWithTiles");
#define EffectSprite_CreateWithTiles EffectSprite_CreateWithTiles__cybele

#define TRAILS ((struct EffectStep *)Ram_MapCellBuffer)
#define MATRICES (Ram_MapCellBuffer + 0x3800)
#define OBJ_VRAM ((u8 *)0x06010000)

#define GIANT_PIECES 11

// Summon 4: a giant strides across a violet sky and cuts the air; sixteen
// slashes gather light and fly, then rain down on the targets.

// Setup: the giant's pieces are built as sprite objects; sixteen slashes
// each get 24 trails of light in random orientations. First loop (220
// frames, A or B skips): the giant follows its path table; from frame 64
// a slash every eight frames draws its trails in, flashes for five frames
// and flies off. Second loop (88 frames): 32 slashes fall from the sky, the
// first 24 two frames apart, and burst into flame on the ground, each one
// shaking the screen and flashing a target.
void BattleEffect_RunAtalanta(struct BattleEffectArgument *effect) {
  s32 point[3];
  struct EffectPosition screen;
  struct SpritePlacement position;
  struct EffectPosition target;
  DrawRectangle draw[2];
  struct BattlePresentationTransition *presentation;
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
  u8 *tiles;
  u8 *picture;

  presentation = Ram_HeapSlots->transitionWork;
  tiles = Ram_MapCellBuffer;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];
  scrollX = gBgScroll[1].x;
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  BattleEffect_ConfigureEffectDisplay();
  BG_PLTT[0] = 0;
  BG_PLTT[1] = 0;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_WipeCanvas(0, 0);
  BattleEffect_SelectLivingTargets(work->effect);

  // The giant: eleven sprite objects with their tiles.
  ((u8 *)Ram_HeapSlots->battleWork)[0x867] = 1;
  picture = Resource_GetTableEntry((s32)&ResourceId_LeafWarriorTiles);
  Iwram_CopyWords(OBJ_PLTT, picture, 512);
  // Two steps: one `+= 512` reuses the size's constant across the call.
  picture += 256;
  picture += 256;
  Decompress_TaggedLz(picture, tiles);
  for (i = 0; i != GIANT_PIECES; i++) {
    struct SpriteObject *piece =
        EffectSprite_CreateWithTiles(32, 32, 0x80002000, 0);

    piece->priority = 1;
    work->objects[i] = piece;
    Iwram_CopyWordsEntry(OBJ_VRAM + gResourceTableEntries[piece->slot].offset,
                         tiles + i * 0x400, 0x400);
  }

  // The slashes, the sky and the giant's path.
  Resource_LoadAndDecompress((s32)&ResourceId_SlashSheet, work->sheet, 1, 1);
  picture = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  Iwram_CopyWordsEntry((void *)BG_PLTT, picture, 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  path = Resource_GetTableEntry((s32)&ResourceId_StagedParticlePath);
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, CLIP));
  BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_ADDITIVE, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = ALTERNATE_BLITTER_SLOT;
  gCameraSceneParameters[4] = 240;
  wait(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  work->scrollTimer = 0;
  work->scrollInterval = 4;
  work->scrollStepX = -1;
  work->scrollStepY = 0;
  Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_FRAME);
  presentation->active = 1;
  BattleEffect_WipeCanvas(0, 1);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  pathX = 0;
  pathY = 0;

  // Each slash: a place on the canvas and 24 trails in random orientations.
  for (i = 0; i != 16; i++) {
    struct EffectStep *slash = &work->particles[i];

    slash->x = (Random16() % 96 + 12) << 16;
    slash->y = ((Random16() & 63) + 32) << 16;
    slash->velocityX = 0;
    slash->velocityY = 0;
    slash->variant = 0;
    for (j = 0; j != 24; j++) {
      TRAILS[i * 24 + j].x = (Random16() & 15) + 48;
      Transform_Reset();
      Transform_ApplyRoll(Random16() & 0xffff);
      Transform_ApplyPitch(Random16() & 0xffff);
      Transform_ApplyYaw(Random16() & 0xffff);
      Transform_Store(MATRICES + (i * 24 + j) * 48);
    }
  }
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BG2CNT = 0x784;

  for (frame = 0; frame != 220 && !(gInput.repeat & 3); frame++) {
    // The giant walks its path: two absolute words, then a step a frame.
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
      for (row = 0; row != 4; row++) {
        for (column = 0; column != 3; column++) {
          if (row != 3 || column != 2) {
            position.x = (pathX + 80 + column * 32) * 65536;
            position.y = (64 - pathY + row * 32) << 16;
            if (row == 3)
              position.x = (pathX + 80 + column * 32 + 32) * 65536;
            Render_ApplyProjectedPlacementFar(work->objects[row * 3 + column],
                                              &position, gAtalanta_UnitScale,
                                              0);
          }
        }
      }
    }
    point[1] = 0;
    point[2] = 0;
    if (frame == 48) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    // From frame 64 a slash every eight frames: its trails draw in, it
    // flashes for five frames and flies off.
    for (i = 0; i != 16; i++) {
      s32 start = i * 8 + 64;

      if (frame >= start) {
        struct EffectStep *slash = &work->particles[i];
        s32 x = HI(slash->x);
        s32 y = HI(slash->y);

        if (frame == i * 8 + 84)
          Sound_PlayFar(212);
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
          for (j = 0; j != 24; j++) {
            struct EffectStep *trail = &TRAILS[i * 24 + j];

            if (trail->x > 0) {
              Transform_Load(MATRICES + (i * 24 + j) * 48);
              point[0] = trail->x;
              EffectPosition_ProjectPoint(point, &screen);
              screen.x = (screen.x >> 1) + x;
              screen.y = screen.y + y + 16;
              trail->x -= 4;
              draw[1](canvas, sheet + gBattleEffect_MoteCells[4], screen.x - 2,
                      screen.y - 5, 5, 10);
            }
          }
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // The giant leaves: the sky stops, the palette is restored and the
  // pieces go.
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  presentation->active = 0;
  gBgScroll[1].x = scrollX;
  BattleEffect_SetupBlendedDisplay();
  Palette_LoadBasicSpriteColors();
  Dma_Set(OBJ_PLTT, (void *)(BG_PLTT + 80), 0x80000010,
          (volatile u32 *)0x040000d4);
  BG_PLTT[94] = BG_PLTT[244];
  for (i = 0; i != GIANT_PIECES; i++)
    Sprite_DestroyFar(work->objects[i]);
  REG_BG2PA = 0x80;
  REG_DISPCNT = 0x7741;
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             Ram_MapCellBuffer, 1, 0);

  // 32 slashes fall from the sky, over the targets where there are any.
  for (i = 0; i != 32; i++) {
    struct EffectStep *slash = &work->particles[i];
    s32 member = i % 6;

    if (member < effect->count) {
      EffectPosition_ProjectScaledBattler(effect->actors[member], &target);
      slash->y = -((Random16() & 31) + 40);
      slash->x = target.x / 2 + (80 - slash->y) / 2;
    } else {
      slash->x = (Random16() & 63) + 80;
      slash->y = -((Random16() & 31) + 40);
    }
    slash->variant = -1;
  }
  for (frame = 0; frame != 88; frame++) {
    for (i = 0; i != 24; i++) {
      struct EffectStep *slash = &work->particles[i];

      if (frame >= i * 2 || frame > 40) {
        if (slash->variant >= 0) {
          // On the ground: six flame pictures, four frames each, the slash
          // still showing for the first three.
          if (slash->variant <= 23) {
            s32 cell = slash->variant / 4;
            u32 width;

            draw[i & 1](canvas,
                        Ram_MapCellBuffer + gBattleEffect_PuffArcCells[cell],
                        slash->x - ((width = gData_08197467[cell]) >> 1) - 8,
                        slash->y + gBattleEffect_PuffArcCellBiasY[cell] - 40,
                        width, gBattleEffect_PuffArcCellHeights[cell]);
            if (slash->variant <= 11) {
              draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4,
                      slash->y - 40, 16, 21);
              draw[0](canvas, work->sheet + SLASH_MIDDLE, slash->x - 16,
                      slash->y - 19, 29, 35);
            }
          }
          slash->variant++;
        } else {
          // Falling: the blade sinks into the ground below y = 56.
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
            Sound_PlayFar(134);
            member = i % 6;
            if (member < effect->count) {
              BattleEffect_TintBattler(effect->actors[member], 7, 5, member, 8);
              BattleMotion_ApplyKnockbackFar(effect->actors[member], 1);
            }
          }
        }
      }
    }
    BattleCamera_Shake(4, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Growth, Mad Growth, Wild Growth (kind 4).
void BattleEffect_RunGrowth(void *object) {
  BattleEffect_RunGrowingColumns(object, 0);
}

/// Abilities: Punji, Punji Trap, Punji Strike (kind 56).
void BattleEffect_RunPunji(void *object) {
  BattleEffect_RunGrowingColumns(object, 1);
}

// The dust puffs at the map cell buffer: 1024 cleared, the first 512 used.
#define PUFFS ((struct EffectStep *)Ram_MapCellBuffer)

#define COLUMN_COUNT 16
#define COLUMN_INTERVAL 4
#define PUFF_COUNT 512
#define PUFF_LIFE 14      // seven cels, two frames each
#define PHASE_RESULTS 133 // BattlePlayback_StartFar: the results phase begins
#define CUE_COLUMN 133

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap
// slot.
#define BLITTER_TRANSPARENT (2 << 4)

void BattleEffect_RunGrowingColumns(struct BattleEffectArgument *effect,
                                    s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  void *rectangle[2];
  void *secondSlot;
  s32 frame;
  s32 duration;
  void **draw;
  s32 fadeStart;
  s32 fadeEnd;
  s32 cueFrame;
  s32 start;
  struct DualColumn *root;
  s32 rootX;
  s32 height;
  s32 limit;
  s32 puffCells;
  struct DualColumn *column;
  s32 length;
  s32 reach;
  s8 pillarHeight;
  s32 n;
  s32 reactFrame;
  s32 i;
  struct EffectStep *puff;
  s32 puffLife;
  s32 cel;

  // Set up: the canvas layer, the sheet for the mode, and the canvas moved
  // 112 pixels right for the other side.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet,
                               1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
  }
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  // Odd columns draw mirrored.
  BattleEffect_LoadBlitter(104, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y);
  rectangle[0] = Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  secondSlot = Ram_HeapSlots->unkb8[1];
  // Columns draw through a pointer to the pair, as the twin does; it holds
  // that one address all its life.
  draw = rectangle;
  draw[1] = secondSlot;

  duration = gGrowth_ColumnCounts[effect->variant] * COLUMN_INTERVAL + 56;

  // Every puff starts free.
  for (i = 0; i != 1024; i++) {
    PUFFS[i].variant = -1;
  }

  // Each column's root, full length (random, capped by its shape) and the
  // frame it starts to grow.
  i = 0;
  do {
    root = (struct DualColumn *)((u8 *)work->particles +
                                 i * sizeof(struct DualColumn));
    rootX = (gGrowth_ColumnX[i] + (7 & Random16())) - 4;
    root->y = i / 2 + 108;
    root->x = rootX;
    height = (63 & Random16()) + 55;
    root->height = height;
    limit = gGrowth_HeightLimits[i % 3];
    if (limit < height) {
      root->height = limit;
    }
    limit = i * COLUMN_INTERVAL + 8;
    root->start = limit;
    i++;
  } while (i != COLUMN_COUNT);

  work->transferMode = 1; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  frame = 0;
  if (duration != 0) {
    cueFrame = duration - 64;
    fadeStart = duration - 20;
    fadeEnd = duration - 4;
    do {
      if (frame == cueFrame) {
        BattlePlayback_StartFar(PHASE_RESULTS);
      }
      if (frame >= fadeStart && frame < fadeEnd) {
        // Fade the canvas out over the last 16 frames.
        REG_BLDCNT = 0x3f44;
        REG_BLDALPHA = ((duration - frame) - 5) | 0x1000;
      }
      // Column i grows 8 pixels a frame from frame i * 4 + 8 to its full
      // length (a pillar no taller than its height table allows); odd
      // columns are mirrored.
      if (frame < fadeEnd) {
        i = 0;
        if (gGrowth_ColumnCounts[effect->variant] != 0) {
          do {
            column = (struct DualColumn *)((u8 *)work->particles +
                                           i * sizeof(struct DualColumn));
            start = i * COLUMN_INTERVAL + 8;
            if (frame == i * COLUMN_INTERVAL + 9) {
              work->shakeFrames = 2;
            }

            if (frame > start) {
              u32 shape = i % 3;
              s32 side;

              length = (frame - start) * 8;
              reach = column->height;
              if (length > reach) {
                length = reach;
              }
              if (mode == 0) {
                {
                  u8 *cell;
                  u32 width;
                  // side is set to 1 and then masked with i, which builds the 1
                  // in its register first; as side = i & 1, 195 of 514
                  // instructions differ in tla-en.
                  side = 1;
                  side &= i;
                  cell = work->sheet + gGrowth_VineCells[shape];
                  width = gGrowth_VineWidths[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, cell, column->x - (width >> 1),
                      column->y - length, width, length);
                }
              } else {
                pillarHeight = gGrowth_PillarHeights[7 & i];
                if (length > pillarHeight) {
                  length = pillarHeight;
                }
                {
                  u8 *cell;
                  u32 width;
                  // side is set to 1 and then masked with i, which builds the 1
                  // in its register first; as side = i & 1, 195 of 514
                  // instructions differ in tla-en.
                  side = 1;
                  side &= i;
                  cell = work->sheet + gGrowth_PillarCells[shape];
                  width = gGrowth_PillarWidths[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, cell, column->x - (width >> 1),
                      column->y - length, width, length);
                }
              }
            }

            // The targets flinch four frames after the column starts; even
            // columns sound the cue.
            n = 0;
            if (effect->count != 0) {
              reactFrame = start + 4;
              do {
                if (frame == reactFrame) {
                  if (!(i & 1)) {
                    Sound_PlayFar(CUE_COLUMN);
                  }
                  BattleEffect_TintBattler(effect->actors[n], 7, 5, n, 3);
                }
                n++;
              } while (n != effect->count);
            } else {
              reactFrame = start + 4;
            }

            // Four and eight frames in, a puff of dust rises at the root.
            if ((frame == reactFrame) || (frame == (start + 8))) {
              struct EffectStep *p = PUFFS;
              n = 0;
              while (n != PUFF_COUNT) {
                if (p->variant == -1) {
                  p->x = ((Random16() & 0xf) + column->x) - 8;
                  p->y = (Random16() & 0xf) + 80;
                  p->variant = 0;
                  break;
                }
                p++;
                n++;
              }
            }
            i++;
          } while (i != gGrowth_ColumnCounts[effect->variant]);
        }
      }

      // The dust: seven cels, two frames each, drawn mirrored about x.
      i = 0;
      puff = PUFFS;
      do {
        puffLife = puff->variant;
        if (puffLife >= 0) {
          cel = puffLife / 2;
          puffCells = 0x1e59;
          if (mode != 0) {
            puffCells = 0xaff;
          }
          ((DrawRectangle)rectangle[0])(
              canvas, work->sheet + (gGrowth_PuffCells[cel] + puffCells),
              puff->x - gGrowth_PuffWidths[cel],
              puff->y - (s8)gGrowth_PuffHeights[cel] / 2,
              gGrowth_PuffWidths[cel], (s8)gGrowth_PuffHeights[cel]);
          ((DrawRectangle)rectangle[1])(
              canvas, work->sheet + (gGrowth_PuffCells[cel] + puffCells),
              puff->x, puff->y - (s8)gGrowth_PuffHeights[cel] / 2,
              gGrowth_PuffWidths[cel], (s8)gGrowth_PuffHeights[cel]);
          puff->variant++;
          if (puff->variant == PUFF_LIFE) {
            puff->variant = -1;
          }
        }
        puff++;
        i++;
      } while (i != PUFF_COUNT);

      BattleCamera_Shake(4, 4);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
    } while (frame != duration);
  }
  // Tear down: stop the transfer, free both blitters, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#define VINE_COUNT 16
#define VINE_FRAMES 70
#define CUE_FRAME 32 // the vines reach the targets
#define CUE_VINES 143
#define CANVAS_MIDDLE 64

// BattleEffect_RunDjinniEntrance's flags: kind | side << 4 | anchor << 5.
#define PREPARE_KIND 4
#define PREPARE_ANCHOR 4

void BattleEffect_RunVine(struct BattleEffectArgument *effect) {
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct EffectPosition screen;
  struct EffectPosition world;
  struct EffectPosition first;
  struct EffectPosition last;
  s32 i;
  s32 frame;
  s32 cell;
  s32 height;
  s32 delay;
  s32 middle;
  struct EffectStep *vine;

  // Set up: the canvas layer, the vine sheet, and the canvas scrolled so
  // that x = 64 falls midway between the first and last targets.
  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      effect, PREPARE_KIND | effect->side << 4 | PREPARE_ANCHOR << 5, &screen,
      &world);
  REG_BG2CNT = 0x785;
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1,
                             1);

  EffectPosition_ProjectScaledBattler(effect->actors[0], &first);
  EffectPosition_ProjectScaledBattler(effect->actors[effect->count - 1], &last);
  middle = first.x;
  middle += (last.x - middle) / 2;
  first.x = middle;
  REG_BG2X = (CANVAS_MIDDLE - first.x) << 8;

  // Odd vines draw mirrored.
  BattleEffect_LoadBlitter(104, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)heap->unk64[1];
  BattleEffect_LoadBlitter(188, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)heap->unkb8[1];

  vine = work->particles;
  i = 0;
  do {
    vine[i].x = gGrowth_ColumnOffsets[i] + CANVAS_MIDDLE;
    i++;
  } while (i != VINE_COUNT);

  work->transferMode = 1; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each frame: vine i starts at frame i * 2 + 4, grows 4 pixels a frame up
  // to 32, and from frame i * 2 + 32 sinks 4 pixels a frame.
  frame = 0;
  do {
    if (frame == CUE_FRAME) {
      Sound_PlayFar(CUE_VINES);
      for (i = 0; i != effect->count; i++)
        BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 16);
    }
    i = 0;
    vine = work->particles;
    do {
      if (frame == i * 4 + 5)
        work->shakeFrames = 2;

      delay = i * 2;
      if (frame > delay + 4) {
        cell = (frame / 4 + i) % 5;
        if (frame < delay + 32) {
          height = (frame - delay) * 4 - 16;
          if (height > 32)
            height = 32;
        } else {
          height = 160 - (frame - delay) * 4;
        }
        if (height > 0)
          draw[i & 1](canvas, work->sheet + (cell << 10), vine->x - 16,
                      (i & 7) - height + 104, 32, height);
      }
      i++;
      vine++;
    } while (i != VINE_COUNT);
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != VINE_FRAMES);

  // Tear down: stop the transfer, free both blitters, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#undef PUFF_COUNT

// The dust puffs, 1024 particles at the map cell buffer.

#define THORN_INTERVAL 8
#define PUFF_COUNT 1024
#define PUFF_SCAN 64

#define CUE_THORN 133

void BattleEffect_RunThorn(struct BattleEffectArgument *effect) {
  void *canvas;
  s32 frame;
  DrawRectangle drawFlip;
  DrawRectangle draw;
  s32 total;
  u32 *heap;
  struct BattleEffectWork *work;
  s32 i;
  s32 j;
  s32 age;
  s32 start;

  // Set up: the canvas layer at full alpha, the thorn sheet, and the canvas
  // moved 112 pixels right for the other side.
  heap = (u32 *)Ram_HeapSlots;
  /* FAKEMATCH(barrier): an empty asm through heap hides that it holds the heap
   * slots' fixed address, so the base stays in r5 until the flipped blitter's
   * slot is read from it; without it, 23 of 453 instructions differ in
   * tla-en. */
  __asm__("" : "+r"(heap));
  work = (struct BattleEffectWork *)heap[23];
  canvas = (void *)heap[24];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  BattleEffect_LoadBlitter(104, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y);
  draw = ((DrawRectangle *)heap)[26];
  BattleEffect_LoadBlitter(188, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  drawFlip = ((DrawRectangle *)heap)[47];
  work->transferMode = 1; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  total = gGrowth_Counts[effect->variant] * THORN_INTERVAL + 56;

  for (i = 0; i != PUFF_COUNT; i++) {
    PUFFS[i].variant = -1; // free
  }

  for (frame = 0; frame != total; frame++) {
    if (frame == total - 64) {
      BattlePlayback_StartFar(PHASE_RESULTS);
    }
    if (frame >= total - 16) {
      // Fade the canvas out over the last 16 frames.
      REG_BLDCNT = 0x3f44;
      REG_BLDALPHA = (total - frame - 1) | 0x1000;
    }

    // Thorn i starts at frame i * 8 + 8. A large one grows 16 pixels a
    // frame to 80 and leans up to 30 pixels; a small one grows 8 pixels a
    // frame to 64 and leans up to 8.
    for (i = 0; i != gGrowth_Counts[effect->variant]; i++) {
      start = i * THORN_INTERVAL + THORN_INTERVAL;
      age = frame - start;
      if (frame > start) {
        s32 height;
        s32 lean;

        if (gGrowth_Kinds[i] < 2) {
          height = age * 16;
          // Six times the age, spelled as the shifts and add the native
          // computes from age in r10 each frame. Written age * 6, loop.c
          // strength-reduces age * 3 into a spilled induction variable of
          // its own: 225 of 453 instructions differ in tla-en.
          lean = (age * 2 + age) * 2;
          if (height > 80) {
            height = 80;
          }
          if (lean > 30) {
            lean = 30;
          }
          if (gGrowth_Kinds[i] & 1) {
            drawFlip(canvas, work->sheet, gGrowth_X[i] - lean, 108 - height, 48,
                     height);
          } else {
            draw(canvas, work->sheet, gGrowth_X[i] + lean, 108 - height, 48,
                 height);
          }
        } else {
          height = age * 8;
          lean = age;
          if (height > 64) {
            height = 64;
          }
          if (age > 8) {
            lean = 8;
          }
          if (gGrowth_Kinds[i] & 1) {
            drawFlip(canvas, work->sheet + 0xf00, gGrowth_X[i] - lean,
                     108 - height, 32, height);
          } else {
            draw(canvas, work->sheet + 0xf00, gGrowth_X[i] + lean, 108 - height,
                 32, height);
          }
        }
        if (frame == start + 1) {
          work->shakeFrames = 3;
        }

        // For two frames after it breaks through, the thorn throws up one
        // dust puff a frame into the first free slot.
        if (frame < start + 3) {
          s32 y = (Random16() & 31) + 72;
          struct EffectStep *puff;

          for (j = 0; j != PUFF_SCAN; j++) {
            puff = &PUFFS[j];
            if (puff->variant == -1) {
              puff->x = gGrowth_X[i] + (Random16() & 31) + 32;
              if (puff->x > 96) {
                puff->x = 96;
              }
              puff->y = y;
              puff->variant = 0;
              break;
            }
          }
        }
      }

      // The targets flinch as the thorn reaches them.
      for (j = 0; j != effect->count; j++) {
        if (frame == start + 4) {
          Sound_PlayFar(CUE_THORN);
          BattleEffect_TintBattler(effect->actors[j], 7, 5, j, 3);
        }
      }
    }

    // The dust: seven cels, two frames each, drawn mirrored about x.
    for (i = 0; i != PUFF_SCAN; i++) {
      if (PUFFS[i].variant >= 0) {
        s32 cel = PUFFS[i].variant / 2;

        draw(canvas, work->sheet + gGrowth_ThornPuffCells[cel],
             PUFFS[i].x - gGrowth_ThornPuffWidths[cel],
             PUFFS[i].y - (s8)gGrowth_ThornPuffHeights[cel] / 2,
             gGrowth_ThornPuffWidths[cel], (s8)gGrowth_ThornPuffHeights[cel]);
        drawFlip(canvas, work->sheet + gGrowth_ThornPuffCells[cel], PUFFS[i].x,
                 PUFFS[i].y - (s8)gGrowth_ThornPuffHeights[cel] / 2,
                 gGrowth_ThornPuffWidths[cel],
                 (s8)gGrowth_ThornPuffHeights[cel]);
        if (++PUFFS[i].variant == PUFF_LIFE) {
          PUFFS[i].variant = -1;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down: stop the transfer, free both blitters, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#define BOLT_INTERVAL 8

#define SPARKS_PER_TARGET 128
#define GROUND_Y 104
#define CUE_THUNDER 138

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap
// slot.
#define BLITTER_MAX (1 << 4)

// The second blitter, loaded again for each glint.
#define DRAW_GLINT ((DrawRectangle)Ram_HeapSlots->unkb8[1])

void BattleEffect_RunBolt(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  u8 *sprites;
  struct EffectPosition pos;
  s32 j;

  // Set up: the canvas layer, the bolt and spark sheets, and every spark
  // and particle free.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  BattleEffect_LoadBlitter(104,
                           BLITTER_MAX | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + 0xc56, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  for (j = 0; j != SPARK_COUNT; j++)
    SPARKS[j].variant = 0;
  for (j = 0; j != 64; j++)
    work->particles[j].variant = -1;
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Sound_PlayFar(CUE_THUNDER);

  for (frame = 0; frame != effect->count * BOLT_INTERVAL + 40; frame++) {
    if (frame == 24)
      BattlePlayback_StartFar(PHASE_RESULTS);

    // The sky flashes as each bolt starts.
    for (i = 0; i != effect->count; i++) {
      if (frame == i * BOLT_INTERVAL)
        Iwram_FillWordsEntry(canvas, 0x4000, 0x10101010);
    }

    for (i = 0; i != effect->count; i++) {
      s32 start = i * BOLT_INTERVAL;

      EffectPosition_ProjectBattler(effect->actors[i], &pos);
      pos.x /= 2;
      if (frame == start + 1)
        work->shakeFrames = 4;
      if (frame == start + 4) {
        BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 6);
        BattleMotion_ApplyKnockbackFar(effect->actors[i], 6);
      }

      // The bolt, 24 wide, grows 64 pixels a frame down to the ground.
      if (frame >= start && frame < start + 16) {
        s32 height = (frame - start) << 6;

        if (height > GROUND_Y)
          height = GROUND_Y;
        for (j = 0; j != gBolt_Counts[effect->variant * 4 + 3]; j++) {
          draw[0](canvas,
                  work->sheet + (((i + frame + j) / 2) & 3) * 2880 + 0xc56,
                  pos.x - 12, 0, 24, height);
        }

        // Sparks fly up from the ground within 90 degrees of straight up.
        if (frame == start + 2) {
          for (j = 0; j != gBolt_Counts[effect->variant * 4]; j++) {
            struct EffectStep *spark = &SPARKS[i * SPARKS_PER_TARGET + j];
            s32 speed = (Random16() & 0x1ff) + 64;
            s32 angle = (Random16() & 0x7fff) - 0x4000;

            spark->x = pos.x << 16;
            spark->y = GROUND_Y << 16;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
            spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
            spark->variant = (Random16() & 7) + 32;
          }
        }
      }

      // Glints: at random places above the target's feet, each with a
      // random palette and flip.
      if (frame >= start + 2 && frame < start + 24) {
        for (j = 0; j != gBolt_Counts[effect->variant * 4 + 1]; j++) {
          s32 kind = j & 3;
          s32 drop = Random16() % gBolt_Counts[effect->variant * 4 + 2];
          s32 spread = gBolt_Counts[effect->variant * 4 + 2] - drop + 1;
          s32 y = pos.y - drop - (gData_08197498[kind] >> 1) + 8;
          u32 r = Random16();
          s32 x = pos.x;
          u32 flip;

          x += r % spread;
          x -= spread / 2;
          x -= gData_08197492[kind] >> 1;
          flip = Random16();

          // An empty once-only loop ends CSE's block, so the two reads of each
          // size table's address stay apart; without it, 172 of 552
          // instructions differ in tla-en.
          do {
          } while (0);
          BattleEffect_BuildBlitter(188, 7, 7,
                                    gBolt_GlintFlips[flip & 3] |
                                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                                    gBolt_GlintModes[effect->variant]);
          DRAW_GLINT(canvas, work->sheet + gBattleEffect_GlintCells[kind], x, y,
                     gData_08197492[kind], gData_08197498[kind]);
          HeapSlot_Release(188);
        }
      }
    }

    // Sparks fall, bounce off the ground at half speed, and shrink as they
    // burn out.
    for (j = 0; j != SPARK_COUNT; j++) {
      struct EffectStep *spark = &SPARKS[j];

      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, 0x1000);
        if (spark->y > GROUND_Y << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x < 127 << 16) && spark->y >= 0) {
          s32 size = spark->variant / 16 + 1;
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;

          draw[0](canvas, sprites + gBattleEffect_MoteCells[size - 1],
                  x - size / 2, y - size, size, size * 2);
        }
      }
    }
    BattleCamera_Shake(2, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down: stop the transfer, free the blitter, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#undef HI
#undef HEAP_BLITTER
#undef HEAP_BLITTER_ALTERNATE
#undef PARTICLES
#undef BLIT_MODE
#undef SHARDS
#undef MOTES
#undef SPARKS
#undef BLITTER_SLOT
#undef FLAME_COUNT
#undef EMBERS
#undef CHIPS
#undef CLIP
#undef SPARK_COUNT
#undef Actor_SetAnimationSpeedFar
#undef STARS
#undef ALTERNATE_BLITTER_SLOT
#undef OBJ_VRAM
#undef COLUMN_COUNT
#undef PHASE_RESULTS
#undef BLITTER_TRANSPARENT
#undef PREPARE_KIND
#undef PREPARE_ANCHOR
#undef BLITTER_MAX

#undef FxGroup
#undef EffectSprite_CreateWithTiles
#undef PUFF_COUNT

// ---- battle_effect/ability_effects.c ----

/* VIEW(ability_effects): tag struct Scale as struct Scale__ability_effects;
 * module/battle_effect/battle_effect.h has the same members, but
 * battle_effect_internal.h declares it before that header defines it, so of a
 * smaller alignment; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:21102: redefinition of `struct
 * Scale'. */
#define Scale Scale__ability_effects
/* VIEW(ability_effects): tag struct WordPair as struct
 * WordPair__ability_effects; module/battle_effect/battle_effect.h has the same
 * members, but battle_effect_internal.h declares it before that header defines
 * it, so of a smaller alignment; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:43815: redefinition of `struct
 * WordPair'. */
#define WordPair WordPair__ability_effects
/* VIEW(ability_effects): tag struct Point2D as struct Point2D__ability_effects;
 * module/battle_effect/battle_effect.h has the same, which the part repeats as
 * it stood alone; tried with the definition the rest of the object has, it does
 * not compile in tla-ja: games/tla/src/battle_effect/battle_effect.c:27342:
 * redefinition of `struct Point2D'. */
#define Point2D Point2D__ability_effects
/* VIEW(ability_effects): tag struct EffectCamera as struct
 * EffectCamera__ability_effects; module/battle_effect/battle_effect.h has the
 * same, which the part repeats as it stood alone; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:27988: redefinition of `struct
 * EffectCamera'. */
#define EffectCamera EffectCamera__ability_effects
/* VIEW(ability_effects): typedef BlitterPair as BlitterPair__ability_effects;
 * module/battle_effect/battle_effect.h has the same, which the part repeats as
 * it stood alone; tried with the definition the rest of the object has, it does
 * not compile in tla-ja: games/tla/src/battle_effect/battle_effect.c:19636:
 * redefinition of `BlitterPair'. */
#define BlitterPair BlitterPair__ability_effects
#define SpriteAttributes SpriteAttributes__ability_effects
#define FxSlot FxSlot__ability_effects
#define SpriteObject SpriteObject__ability_effects
#define Texture Texture__ability_effects
#define Particle Particle__ability_effects
#define ResourceTableEntry ResourceTableEntry__ability_effects
#define CameraWork CameraWork__ability_effects

/// The bank's third object: the effects of abilities and summons from the
/// opening every Djinn effect shares to Meteor Blow, each part named for the
/// first ability it draws. ability_effects_internal.h holds the includes every
/// part shares; the parts under ability_effects/ follow in ROM order.

#include "ability_effects_internal.h"

// The parts, in ROM order.
#include "ability_effects/projectiles.c"
#include "ability_effects/djinn_attacks.c"
#include "ability_effects/hail.c"
#include "ability_effects/thor.c"
#include "ability_effects/spire.c"
#include "ability_effects/flurry.c"
#include "ability_effects/gaia.c"
#include "ability_effects/spider_web.c"
#include "ability_effects/azul.c"
#include "ability_effects/positions.c"
#include "ability_effects/casting_impact.c"
#include "ability_effects/torch.c"
#include "ability_effects/canvas.c"
#include "ability_effects/particle_streams.c"
#include "ability_effects/ramses.c"
#include "ability_effects/annihilation.c"
#include "ability_effects/ragnarok.c"
#include "ability_effects/judgment.c"
#include "ability_effects/kirin.c"
#include "ability_effects/neptune.c"
#include "ability_effects/thrash.c"
#include "ability_effects/undead_sword.c"
#include "ability_effects/crystal.c"
#include "ability_effects/geode.c"
#include "ability_effects/nirvana.c"
#include "ability_effects/heavy_divide.c"
#include "ability_effects/thunder_mine.c"
#include "ability_effects/light_surge.c"
#include "ability_effects/whorl.c"
#include "ability_effects/djinni_charge.c"
#include "ability_effects/rime.c"
#include "ability_effects/gasp.c"
#include "ability_effects/charon.c"
#include "ability_effects/rising_dragon.c"
#include "ability_effects/raging_heat.c"
#include "ability_effects/flora.c"
#include "ability_effects/fire_puppet.c"
#include "ability_effects/fume.c"
#include "ability_effects/ocean_fist.c"
#include "ability_effects/rushing_strike.c"
#include "ability_effects/mirrored_wall.c"
#include "ability_effects/daedalus.c"
#include "ability_effects/vengeance.c"
#include "ability_effects/catastrophe.c"
#include "ability_effects/haures.c"
#include "ability_effects/death_plunge.c"
#include "ability_effects/helm_splitter.c"
#include "ability_effects/dragon_cloud.c"
#include "ability_effects/legend.c"
#include "ability_effects/cards.c"
#include "ability_effects/triple_chomp.c"
#include "ability_effects/breaths.c"
#include "ability_effects/trident.c"
#include "ability_effects/meteor_blow.c"

#undef CameraWork
#undef ResourceTableEntry
#undef Particle
#undef Texture
#undef SpriteObject
#undef FxSlot
#undef SpriteAttributes
#undef Render_ProjectVertices
#undef Render_DrawModelList
#undef Render_ProjectVertices
#undef Render_DrawModelList

#undef Scale
#undef WordPair

#undef Point2D
#undef EffectCamera
#undef BlitterPair

// ---- battle_effect/liquifier.c ----

/// Liquifier, and the canvas bursts: a glowing ball that strikes the target in
/// flames and sparks.

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "task.h"
#include "actor.h"
#include "battle_display.h"
#include "fixed_math.h"
#include "graphics.h"
#include "module/battle_effect/battle_effect.h"
#include "gamepak/far_call/battle.h"
#include "module/system/system.h"
#include "render_model.h"
#include "resource.h"
#include "resource_ids.h"
#include "system.h"

struct Model;

// The integer (pixel) half of a 16.16 value, read as the halfword it is.
#define HI(v) (((s16 *)&(v))[1])

#define KIND123_FRAMES 111

// A model for the ARM renderer (Render_DrawModelList).
/* VIEW(liquifier): tag struct RenderModel as struct RenderModel__liquifier;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunLiquifier moves: 2 of 892 instructions in
 * tla-ja. */
struct RenderModel__liquifier {
  s32 kind;
  s32 unk04;
  const u8 *mesh;
  u8 *vertices;
  struct ModelTexture *texture;
  s32 shade;
};

// The first load names the buffer by its address: as the symbol it would
// combine with the second.
#define EFFECT_PICTURES (Ram_MapCellBuffer + 0x4000)

// Effect kind 123, 111 frames. Sparks gather at the caster (frames 0-15)
// and burst from it toward the target (16-110); eight flat flashes turn about
// the caster; sixteen textured shards fly from the target (54-101), each
// leaving a fading splash; two rings swell at the caster (0-15); every four
// frames from 54 the target is struck, and the last strike ends the phase.
//
// The work's particles 0-31 are the sparks, 32-47 the shards (z holds each
// shard's roll); the map cell buffer is cleared but not drawn.
void BattleEffect_RunLiquifier(struct BattleEffectArgument *effect) {
  struct EffectPosition from;
  struct EffectPosition to;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *graphics;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, EFFECT_PICTURES,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CB, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x1000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gData_02014000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  EffectPosition_ProjectBattler(effect->actor, &from);
  EffectPosition_ProjectBattler(effect->actors[0], &to);

  for (frame = 0; frame != KIND123_FRAMES; frame++) {
    if (frame == 0) {
      struct EffectStep *shard;

      for (i = 0; i != 512; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;

      // The shards start about the target, each with its own roll.
      for (i = 0, shard = &work->particles[32]; i != 16; i++, shard++) {
        shard->x = (to.x + (Random16() & 127) - 64) << 16;
        shard->y = to.y << 16;
        shard->z = (Random16() & 0x1fff) + 0x7000;
      }
    }
    if (frame == 0) {
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 12) {
      work->transferMode = 3;
      work->transferValue = 0x04040404;
    }
    if (frame == 54) {
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 0)
      Sound_PlayFar(139);
    if (frame == 16)
      Sound_PlayFar(134);

    if (frame > 15) {
      // The burst: sparks fly out of the caster, upward and forward.
      if (frame == 16) {
        struct EffectStep *step = work->particles;

        for (i = 0; i != 32; i++) {
          u32 angle = Random16() % 0x5fff + 0x5000;
          s32 speed = (Random16() & 0x1ff) + 0x200;

          step->velocityX = (Trig_Sin(angle) * speed) >> 8;
          step->velocityY = (Trig_Cos(angle) * speed) >> 7;
          step->x = (from.x / 2) << 16;
          step->y = (from.y - (Random16() & 15) - 8) << 16;
          step->variant = 0;
          step++;
        }
      }
      // Two steps a frame, the n-th spark from frame n / 4.
      for (i = 0; i != 32; i++) {
        s32 one = 1;
        struct EffectStep *step = &work->particles[i];

        if (step->variant >= 0 && frame >= i / 4) {
          s32 odd = i;
          s32 size;
          s32 blend;
          s32 j;
          odd &= one;
          size = odd + 5;
          j = 0;
          blend = odd * 4;

          {
            s32 twice = size * 2;
            s32 half = (u32)size >> 1;

            for (; j != 2; j++) {
              (*(DrawRectangle *)((u8 *)draw + blend))(
                  canvas, graphics + gBattleEffect_MoteCells[size - 1],
                  HI(step->x) - half, HI(step->y) - size, size, twice);
              BattleEffectKernels_IntegrateVector2(step, 64, 0);
            }
          }
          step->variant++;
        }
      }
    } else {
      struct EffectStep *step;

      // The gathering: sparks start 64 steps out and fall into the caster.
      if (frame == 0) {
        struct EffectStep *step = work->particles;

        for (i = 0; i != 32; i++) {
          s32 angle = (Random16() & 0x7fff) - 0x4000;
          s32 speed = (Random16() & 0x1ff) + 0x200;
          s32 y;

          step->velocityX = (Trig_Sin(angle) * speed) >> 10;
          step->velocityY = (Trig_Cos(angle) * speed) >> 9;
          step->x = (from.x / 2) << 16;
          y = from.y - (Random16() & 15) - 8;
          step->x -= step->velocityX << 6;
          step->y = (y << 16) - (step->velocityY << 6);
          step->variant = 0;
          step++;
        }
      }
      // Four steps a frame.
      for (i = 0, step = work->particles; i != 32; i++, step++) {
        s32 one = 1;
        if (step->variant >= 0) {
          s32 odd = i;
          s32 size;
          s32 blend;
          s32 j;
          odd &= one;
          size = odd + 2;
          j = 0;
          blend = odd * 4;

          {
            s32 twice = size * 2;
            s32 half = (u32)size >> 1;

            for (; j != 4; j++) {
              (*(DrawRectangle *)((u8 *)draw + blend))(
                  canvas, graphics + gBattleEffect_MoteCells[size - 1],
                  HI(step->x) - half, HI(step->y) - size, size, twice);
              BattleEffectKernels_IntegrateVector2(step, 64, 0);
            }
          }
          step->variant++;
        }
      }
    }

    {
      void *projected = Heap_AllocEwram(32);
      struct RenderModel__liquifier *model = Render_AllocateModelList(1);
      void *splash = gData_02014000;

      // Eight flashes turn about the caster, each for 16 frames from its
      // start, swinging wider and back with a sine.
      model->kind = 6;
      model->texture = &texture;
      model->vertices = projected;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet;
      model->mesh = Data_081992b0;
      for (i = 0; i != 8; i++) {
        s32 start = Data_08199eb4[i];

        if (frame >= start && frame < start + 16) {
          s32 age;

          model->shade = 0;
          age = frame - start;
          Transform_Reset();
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          SceneTransform_ApplyPosition3((from.x - 128) << 16,
                                        (from.y - 74) << 16, 0);
          Transform_ApplyRoll((i << 12) - 0x3800);
          SceneTransform_ApplyScale3(
              0x5555, Trig_Sin(age << 11) * Data_08199ebc[i] / 64 * 2, 0x8000);
          Render_TransformVertices(Data_08199ea4, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // The shards: each from frame 54 + 2i for 16 frames, shrinking.
      texture.logWidth = 5;
      texture.logHeight = 2;
      texture.pixels = work->sheet + 0x1000;
      model->mesh = Data_08199220;
      model->kind = 7;
      for (i = 0; i != 16; i++) {
        s32 zero = 0;
        struct EffectStep *step = &work->particles[32 + i];
        s32 start = i * 2 + 54;

        if (frame >= start && frame < i * 2 + 70) {
          s32 scale;

          scale = 0x20000 - ((frame - start) << 13);
          model->shade = zero;
          Transform_Reset();
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          SceneTransform_ApplyPosition3(step->x - (128 << 16),
                                        step->y - (64 << 16), 0);
          Transform_ApplyRoll(step->z);
          SceneTransform_ApplyScale3(scale, 0x10000, 0x8000);
          Render_TransformVertices(Data_081991c0, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Their splashes: growing and darkening, faster after eight frames.
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = splash;
      model->mesh = gData_08199364;
      model->kind = 7;
      for (i = 0; i != 16; i++) {
        struct EffectStep *step = &work->particles[i + 32];
        s32 age = frame - 54 - i * 2;
        s32 back = -age;

        if (frame >= i * 2 + 54 && frame < i * 2 + 70) {
          s32 shade = back * 4;
          s32 scale = age << 14;

          if (age > 7) {
            shade -= age * 8;
            shade += 64;
          }
          model->shade = shade;
          Transform_Reset();
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          SceneTransform_ApplyPosition3(step->x - (128 << 16),
                                        step->y - (64 << 16), 0);
          Transform_ApplyPitch(0x3c00);
          SceneTransform_ApplyScale3(scale, scale, scale);
          Render_TransformVertices(Data_081991e0, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Two rings at the caster, from frames 0 and 4 for 12 frames each.
      for (i = 0; i != 2; i++) {
        if (frame >= i * 4 && frame < i * 4 + 12) {
          s32 age = frame - i * 4;
          s32 shade = age * 8 - 64;
          s32 scale = 0x18000 - (age << 13);

          if (shade > 0)
            shade = 0;
          model->shade = shade;
          Transform_Reset();
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          SceneTransform_ApplyPosition3((from.x - 128) << 16,
                                        (from.y - 80) << 16, 0);
          SceneTransform_ApplyScale3(scale * 2, scale * 2, scale);
          Transform_ApplyRoll(0x6000 - (i << 14));
          Transform_ApplyPitch(-0x1000);
          Render_TransformVertices(gData_08199210, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // A strike every four frames from 54; the last one ends the phase.
      for (i = 0; i != 8; i++) {
        if (frame == i * 4 + 54) {
          if (i == 7)
            BattlePlayback_StartFar(144);
          else
            Sound_PlayFar(144);
          BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0, 100);
          BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 5);
          work->shakeFrames = 4;
        }
      }
      Heap_Release(model);
      Heap_Release(projected);
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"

#include "motobj.h"
#include "field_event.h"

#define Actor_SetAnimationSpeedFar                                             \
  ((s32 (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)

void BattleEffect_RunCanvasModeByCount(struct BattleEffectArgument *effect) {
  if (effect->kind > 199)
    Func_0818f620(effect, 2);
  else
    Func_0818f620(effect, 1);
}

DEFINE_EFFECT_KIND(BattleEffect_RunCanvasMode0, Func_0818f620, 0)

void BattleEffect_RunCanvasMode3WithAction(
    struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x1c);
  Func_0818f620(effect, 3);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

#include "io_write_queue.h"
#include "bg_scroll.h"
#include "battle_work.h"

// The ball is the last particle, never a local: the native rereads its address.
#define ball (&work->particles[63])

// Draws a disc of the given size from the disc sheet in the map cell buffer
// as four quarter blits about (x, y), with the second blitter loaded for
// each quarter's flip (flags 3, 7, 11 and 15 of the maximum mode). The sheet
// offset comes first: the sum only matches in integer form, cells on the right.
static inline void DrawDisc(void *canvas, s16 *discs, s32 cells, s32 x, s32 y,
                            s32 size, s32 index) {
  BattleEffect_LoadBlitter(188, 1 << 4 | 3);
  ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, (u8 *)(discs[index] + cells),
                                           x - (size >> 1), y - size, size >> 1,
                                           size);
  HeapSlot_Release(188);
  BattleEffect_LoadBlitter(188, 1 << 4 | 7);
  ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, (u8 *)(discs[index] + cells),
                                           x, y - size, size >> 1, size);
  HeapSlot_Release(188);
  BattleEffect_LoadBlitter(188, 1 << 4 | 11);
  ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, (u8 *)(discs[index] + cells),
                                           x - (size >> 1), y, size >> 1, size);
  HeapSlot_Release(188);
  BattleEffect_LoadBlitter(188, 1 << 4 | 15);
  ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, (u8 *)(discs[index] + cells),
                                           x, y, size >> 1, size);
  HeapSlot_Release(188);
}

// The canvas burst family (its wrappers are above). A glowing ball (or
// in mode 0 a growing glow at the target, with sixteen rays that close on
// it) hits the target; at the hit the targets flash and recoil, sixteen
// flames rise and 47 sparks scatter. Modes:
//   0  (BattleEffect_RunCanvasMode0): 78 frames, the glow at the target; the
//      hit at frame 54, then the canvas is cleared and
//      SummonEffect_RunCanvasMode1 runs on;
//   1  (by count, kinds up to 199): 66 frames, a small ball (24 pixels),
//      half-speed sparks, hit at 32, no flames;
//   2  (by count, kinds 200 and up): 66 frames, a large ball (40), hit at
//      32;
//   3  (BattleEffect_RunCanvasMode3WithAction): 58 frames, the ball flies to
//   the
//      midpoint of the targets, its own palette, hit at 24.
// Modes 1 and 2 close by restoring the battle display themselves.
void Func_0818f620(struct BattleEffectArgument *effect, u32 mode) {
  struct EffectPosition from;
  struct EffectPosition to;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2]; // Unused: draw[1] gives the stack frame its 4 bytes.
  u8 *graphics;
  s16 *discs;
  s32 frames;
  s32 hitFrame;
  s32 offset;
  s32 i;
  s32 j;
  s32 cells = (s32)Ram_MapCellBuffer;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  if (mode <= 1)
    BattleEffect_StartCanvasLayer(1);
  else
    BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(104, 1 << 4 | 3);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  discs = (s16 *)work->sheet;
  if (mode == 0)
    frames = 78;
  else if (mode == 3)
    frames = 58;
  else
    frames = 66;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);

  // Discs of sizes 2 to 64 one after another in the map cell buffer, with
  // their offsets at the start of the sheet.
  offset = 0;
  for (i = 0; i != 32; i++) {
    s32 size = i * 2 + 2;

    BattleEffect_DrawShadedCircles((u8 *)(offset + cells), size, 0x14e20);
    discs[i] = offset;
    offset += (i * 2 + 3) * size / 2;
  }
  if (mode == 3)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                    128);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);

  for (frame = 0; frame != frames; frame++) {
    EffectPosition_ProjectBattler(effect->actor, &from);
    if (mode == 3)
      BattleEffect_GetTargetsMidpoint(&to);
    else
      EffectPosition_ProjectBattler(effect->actors[0], &to);
    if (mode == 0 && frame == 0)
      Sound_PlayFar(140);

    if (frame == 0) {
      if (mode == 0) {
        struct EffectStep *step;

        // The rays: one every 1/16 turn, each starting after its own delay.
        for (i = 0; i != 16; i++) {
          step = &work->particles[i];
          step->z = i << 12;
          step->variant = (Random16() & 31) + 16;
        }
      } else {
        // The ball leaves the caster's chest, aimed to arrive in 128
        // frames' worth of steps, already seven steps along.
        ball->x = (from.x / 2) << 16;
        ball->y = (from.y - 36) << 16;
        ball->velocityX = (to.x - from.x) << 10;
        ball->velocityY = (to.y - from.y) << 10;
        ball->x += ball->velocityX * 7;
        ball->y += ball->velocityY * 7;
      }
    }

    if (mode == 0) {
      // Frames 18-54: the glow grows at the target, up to 64 pixels.
      if ((u32)(frame - 18) <= 36) {
        s32 size = frame * 2 - 32;
        s32 x;
        s32 y;

        if (size > 64)
          size = 64;
        x = to.x / 2;
        y = to.y - 32;
        DrawDisc(canvas, discs, cells, x, y, size, (size - 2) >> 1);
      }
    } else if ((u32)(frame - 2) <= 30) {
      // Frames 2-32: the ball flies, growing to 24 (mode 1) or 40 pixels.
      s32 size = frame / 4 * 2 + 16;
      s32 index;
      s32 x;
      s32 y;

      if (mode == 1) {
        if (size > 24)
          size = 24;
      } else if (size > 40) {
        size = 40;
      }
      index = (size - 2) >> 1;
      x = ball->x >> 16;
      y = ball->y >> 16;
      ball->x += ball->velocityX;
      ball->y += ball->velocityY;
      DrawDisc(canvas, discs, cells, x, y, size, index);
    }

    // Mode 0, frames 0-53: sixteen rays of motes close on the target.
    if (mode == 0 && frame <= 53) {
      struct EffectStep *step = work->particles;

      for (i = 0; i != 16; i++) {
        if ((u32)(--step->variant + 32) <= 47) {
          s32 size = 4;

          for (j = 0; j != 16; j++) {
            s32 radius = (step->variant * 2 + j) * 2;

            if (radius >= 0) {
              s32 width = 4;
              s32 x = to.x / 2 + ((Trig_Sin(step->z) * radius) >> 16);
              s32 height = size * 2;
              s32 half = size >> 1;
              s32 y = to.y + ((Trig_Cos(step->z) * radius) >> 16);

              draw[0](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                      x - half, y - 32 - size, width, height);
            }
          }
        }
        step++;
      }
    }

    if (mode == 0)
      hitFrame = 54;
    else if (mode == 3)
      hitFrame = 24;
    else
      hitFrame = 32;
    if (frame >= hitFrame) {
      struct EffectStep *newFlame;
      struct EffectStep *newSpark;
      struct EffectStep *flame;
      struct EffectStep *spark;

      if (frame == hitFrame) {
        // The hit: the targets recoil (mode 1 keeps them still) and flash.
        if (mode == 0 || mode == 2 || mode == 3) {
          BattlePlayback_StartFar(145);
          for (i = 0; i != effect->count; i++)
            BattleMotion_ApplyKnockbackFar(effect->actors[i], 4);
        } else {
          BattlePlayback_StartFar(133);
          for (i = 0; i != effect->count; i++)
            BattleMotion_ApplyKnockbackFar(effect->actors[i], 0);
        }
        for (i = 0; i != effect->count; i++)
          BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 8);
        work->shakeFrames = 8;

        // Sixteen flames, faster one by one.
        for (i = 0; i != 16; i++) {
          s32 angle = Random16() & 0xffff;
          s32 speed;

          newFlame = &work->particles[i];
          speed = i * 4 + 4;
          newFlame->x = (to.x / 2) << 16;
          newFlame->y = (to.y - 32) << 16;
          newFlame->velocityX = (speed * Trig_Sin(angle)) >> 3;
          newFlame->velocityY = -(speed * Trig_Cos(angle)) >> 3;
          newFlame->variant = i / 3 + 17;
        }

        // 47 sparks, at half speed in mode 1.
        for (i = 0; i != 47; i++) {
          s32 speed = (Random16() & 255) + 128;
          s32 angle = Random16() & 0xffff;

          newSpark = &work->particles[16 + i];

          newSpark->x = (to.x / 2) << 16;
          newSpark->y = (to.y - 32) << 16;
          newSpark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          newSpark->velocityY = -(Trig_Cos(angle) * speed) >> 5;
          if (mode == 1) {
            newSpark->velocityX /= 2;
            newSpark->velocityY /= 2;
          }
          newSpark->variant = i / 3 + 17;
        }
      }

      // The flames: six frames of the burst sheet, three frames each.
      if (mode == 0 || mode == 2 || mode == 3) {
        flame = work->particles;
        for (i = 0; i != 16; i++) {
          s32 x = HI(flame->x);
          s32 y = HI(flame->y);

          BattleEffectKernels_IntegrateVector2(flame, 60, 0);
          if ((u32)flame->variant <= 17)
            draw[0](canvas,
                    work->sheet + 0x800 +
                        (Data_08199ec5[flame->variant / 3] << 11),
                    x - 16, y - 32, 32, 64);
          if (flame->variant > 0)
            flame->variant = flame->variant - 1;
          else
            flame->variant = -1;
          flame++;
        }
      }

      // The sparks shrink as they fly.
      for (i = 0; i != 47; i++) {
        spark = &work->particles[16 + i];
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 2) + 1;

          draw[0](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                  HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(spark, 60, 0);
          spark->variant--;
        }
      }
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  if (mode == 0) {
    Iwram_ClearWords((void *)0x06004000, 0x3c00);
    Iwram_ClearWords(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    SummonEffect_RunCanvasMode1(effect);
  } else if (mode == 3) {
    BattleEffect_EndCanvasLayer();
  } else {
    // Back to the battle display: BG1's scroll, BG2 as before, the palette
    // fade off and the backgrounds on again.
    struct BattleWork *battle = Ram_HeapSlots->battleWork;

    Sound_PlayFar(390);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    REG_BG2CNT = 0x787;
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    Task_Remove(Palette_StepFadeTransfer);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    REG_BLDCNT = 0;
    wait(1);
    BattleBackground_SetModeFar(2, *(u16 *)battle->unk670, 0);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7541, 0x20000);
    wait(1);
  }
}

#undef HI
#undef Actor_SetAnimationSpeedFar

// ---- battle_effect/summon_effects.c ----

/* VIEW(summon_effects): tag struct Scale as struct Scale__summon_effects;
 * module/battle_effect/battle_effect.h has the same members, but
 * battle_effect_internal.h declares it before that header defines it, so of a
 * smaller alignment; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:62377: redefinition of `struct
 * Scale'. */
#define Scale Scale__summon_effects
/* VIEW(summon_effects): tag struct WordPair as struct WordPair__summon_effects;
 * module/battle_effect/battle_effect.h has the same members, but
 * battle_effect_internal.h declares it before that header defines it, so of a
 * smaller alignment; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:62372: redefinition of `struct
 * WordPair'. */
#define WordPair WordPair__summon_effects
/* VIEW(summon_effects): tag struct Point2D as struct Point2D__summon_effects;
 * module/battle_effect/battle_effect.h has the same, which the part repeats as
 * it stood alone; tried with the definition the rest of the object has, it does
 * not compile in tla-ja: games/tla/src/battle_effect/battle_effect.c:61784:
 * redefinition of `struct Point2D'. */
#define Point2D Point2D__summon_effects
/* VIEW(summon_effects): typedef BlitterPair as BlitterPair__summon_effects;
 * module/battle_effect/battle_effect.h has the same, which the part repeats as
 * it stood alone; tried with the definition the rest of the object has, it does
 * not compile in tla-ja: games/tla/src/battle_effect/battle_effect.c:61456:
 * redefinition of `BlitterPair'. */
#define BlitterPair BlitterPair__summon_effects
#define Model Model__summon_effects
#define Texture Texture__summon_effects
#define ResourceTableEntry ResourceTableEntry__summon_effects
#define BattleBackgroundId BattleBackgroundId__summon_effects
#define ModelVertex ModelVertex__summon_effects

/// The bank's last object: the Megiddo, Moloch and Iris summons, Odyssey, and
/// the model renderers the effects draw with. summon_effects_internal.h holds
/// the includes every part shares; the parts under summon_effects/ follow in
/// ROM order.

#include "summon_effects_internal.h"

// The parts, in ROM order.
#include "summon_effects/megiddo.c"
#include "summon_effects/odyssey.c"
#include "summon_effects/moloch.c"

#undef ModelVertex
#undef BattleBackgroundId
#undef ResourceTableEntry
#undef gResourceTableEntries
#undef EffectSprite_CreateWithTiles
#undef EffectSprite_Acquire
#undef Texture
#undef Model
#undef Scale
#undef WordPair

#undef Point2D
#undef BlitterPair
