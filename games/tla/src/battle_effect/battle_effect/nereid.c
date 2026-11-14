/// @file
/// The Nereid summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "module/battle_effect/battle_effect.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

#undef FORMATION_RESOURCE

// Nereid, summon 2.

#define SPARK_COUNT 1024
#define FORMATION_SIZE 12
#define FORMATION_RESOURCE 649
#define COLUMN_SHEET 0x6e4 // the fire pillar, after the water spray band
#define COLUMN_WIDTH 34
#define COLUMN_HEIGHT 104

/// Summon 2: a formation of twelve sweeps in from the right and settles over
/// the enemies while a band of spray rises from below. At frame 28, and every
/// frame from 32 to 79, sparks burst up from beside the formation and fall
/// back; three columns of fire stream up meanwhile, and every twelve frames
/// the targets flash and jump. After frame 80 the sparks rise away. A or B
/// skips frames 33 to 97.
/// Nereid (summon 2, BattleEffect_RunSceneEffect's case 2), ported call for
/// call from The Broken Seal's BattleEffect_RunNereid. This game reads its heap
/// blocks canvas, work, sheet; loads three resources where The Broken Seal
/// copies a palette and decodes two sheets; takes its blitter pair from
/// BattleEffect_FetchRectangleBlitters and its formation from resource 649; and
/// reads the effect argument in the hit loop.
void BattleEffect_RunNereid(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up: half-and-half blending, the max and additive blitters, the
  // spray band and the fire column into the work sheet, the sparks into the
  // sheet slot, and the transfer at three quarters.
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA,
                             work->sheet + COLUMN_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *sparks;
    s32 *life;

    // The formation enters from (256, 88), moving left and up.
    rise = 0;
    focus.x = px(256);
    focus.y = px(88);
    velocity.x = -px(16);
    velocity.y = -px(4);
    for (k = 0; k != 64; k++)
      work->particles[k].variant = -1;
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }

    // Every spark in the map buffer starts free (variant -1).
    life = &((struct EffectStep *)Ram_MapCellBuffer)->variant;
    for (k = 0; k != SPARK_COUNT; k++) {
      *life = -1;
      life += sizeof(struct EffectStep) / 4;
    }
    sparks = (struct EffectStep *)Ram_MapCellBuffer;
    BattleEffect_SelectLivingTargets(work->effect);
    wait(1);
    BattleEffect_SpawnObjects(FORMATION_SIZE, FORMATION_RESOURCE, 2);

    for (frame = 0; frame != 124; frame++) {
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 32 && frame <= 97)
        frame = 98;
      if (frame == 120)
        BattlePlayback_StartFar(134);
      if (frame <= 15)
        rise += 2;

      // The formation slows (x 58/64, y 56/64 each frame) and is pushed
      // back right once it passes x = 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / 64;
        velocity.y = velocity.y * 56 / 64;
        if (focus.x < px(120))
          velocity.x += px(1) / 2;
      }
      BattleEffect_PlaceFormationObjects(1, focus.x, focus.y);

      // Sparks burst up from around (32, 96): every free one at frame 28,
      // then sixteen a frame from 32 to 79.
      if (frame == 28) {
        for (k = 0; k != 256; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
          }
        }
      }
      if (frame >= 32 && frame < 80) {
        s32 spawned = 0;

        for (k = 0; k != SPARK_COUNT; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        Sound_PlayFar(SE_SCATTER);
      if (frame == 32)
        Sound_PlayFar(SE_CRASH);
      if (frame == 80)
        Sound_PlayFar(SE_CLATTER);

      // Three columns of fire scroll up while the sparks burst.
      if (frame >= 32 && frame < 80) {
        for (k = 0; k != 3; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % COLUMN_HEIGHT;

          draw[0](canvas, work->sheet + COLUMN_SHEET,
                  gNereid_Columns[k * 2] - COLUMN_WIDTH / 2,
                  gNereid_Columns[k * 2 + 1] - span - COLUMN_HEIGHT,
                  COLUMN_WIDTH, COLUMN_HEIGHT);
          draw[0](canvas, work->sheet + COLUMN_SHEET,
                  gNereid_Columns[k * 2] - COLUMN_WIDTH / 2,
                  gNereid_Columns[k * 2 + 1] - span, COLUMN_WIDTH, span);
        }
      }

      // The spray band rises sixteen pixels and drifts right.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32,
                  120 - rise, 32, 32);
      }

      // Each live spark: rising ones are drawn larger and by the max
      // blitter, falling ones by the additive one; all grow from frame 68.
      // They fall under gravity until frame 80, then accelerate upward, and
      // a falling spark that reaches y = 104 is freed.
      for (k = 0; k != SPARK_COUNT; k++) {
        spark = &sparks[k];
        if (spark->variant >= 0) {
          s32 size = k % 3 + 2;

          if (spark->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[spark->velocityY > 0](
              canvas, graphics + gBattleEffect_MoteCells[size - 1],
              HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
          spark->x += spark->velocityX;
          spark->y += spark->velocityY;
          if (frame > 80)
            spark->velocityY += -(px(1) / 2);
          else
            spark->velocityY += Data_08197998[k & 3];
          spark->velocityX = spark->velocityX * 62 / 64;
          spark->velocityY = spark->velocityY * 62 / 64;
          spark->variant++;
          if (spark->velocityY > 0 && HI(spark->y) > 104)
            spark->variant = -1;
        }
      }

      // From frame 30 to 79 the targets flash and are knocked upward every
      // twelve frames, and stop flashing six frames later.
      if (frame <= 79) {
        for (k = 0; k != effect->count; k++) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *target =
                  BattleObject_GetFar(effect->actors[k])->object;

              BattleEffect_TintBattler(effect->actors[k], 7, 5, -1, 0);
              target->velocityY = px(9) / 2;
              target->verticalMotionStrength = 0xab85;
            }
            if (phase == 6)
              BattleEffect_TintBattler(effect->actors[k], 0, 5, -1, 0);
          }
        }
      }
      work->transferPending = 1;
      wait(1);
    }

    // Tear down: the formation bursts where it stands and is released.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    BattleEffect_RunImpactBurst(1, focus.x, focus.y);
    for (k = 0; k != FORMATION_SIZE; k++)
      Sprite_DestroyFar(work->objects[k]);
  }
  BattleEffect_EndCanvasLayer();
}
