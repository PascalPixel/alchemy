/// @file
/// Ice and Douse: shards and bolts falling on the targets' side.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "module/battle_effect/battle_effect.h"
#include "module/battle_effect/battle_effect.h"
#include "resource_ids.h"

// The blitter pair's heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define PHASE_RESULTS 133

#define SHARD_COUNT(variant) Data_08197521[(variant) * 2]
#define FRAME_COUNT(variant) Data_08197521[(variant) * 2 + 1]

extern u8 gMapCellBuffer[];

// Ice, Ice Horn and Ice Missile (effect kind 7, variants 0 to 2).

// Shards of ice fall in a slanting column onto the targets' side. Each one
// flies down at 8 pixels a frame until it reaches the ground line, then
// flashes every target, shakes the screen and shatters: fourteen frames of
// breaking pictures, with a burst of sparkle puffs in the middle of them.
// Window 0 is narrowed line by line through an HBlank DMA table into a
// slanting band, so the shards appear only over the targets.

// Particles 0-31 are the shards and 32-63 the puffs. A particle's variant is
// its age: -1 is a shard still falling or a free puff.

void BattleEffect_RunIce(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *graphics;
  void *draw[2];
  struct EffectStep *shard;
  struct EffectStep *puff;
  u16 *line;
  s32 i;
  s32 k;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas, the shard pictures in the sheet and the puffs.
  BattleEffect_StartCanvasLayer(0x2001);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_IceShardSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, graphics, 0, 0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BLDCNT = 0x3f44;
  REG_WININ = 0x3337;

  // Line the shards up above the screen, each 8 pixels higher and further
  // out than the one before, with a little scatter.
  {
    s32 y;
    s32 slant;

    for (i = 0, y = -128, slant = -16; i != 32; y -= 64, i++, slant -= 8) {
      s32 x;

      shard = &work->particles[i];
      x = (Random16() & 63) + (Random16() & 7) + 24;
      if (effect->side == 1)
        x = x + slant + 24;
      else
        x = x - slant + 80;
      shard->x = x << 3;
      shard->variant = -1;
      shard->y = y;
    }
  }
  for (i = 0; i != 32; i++)
    work->particles[32 + i].variant = -1;

  // Falling shards draw at their maximum; the puffs add their light. Both
  // mirror for the other side.
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_Y));
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                             BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X));
    BattleEffect_LoadBlitter(
        SLOT_BLITTER_ALTERNATE,
        BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X));
  }
  draw[0] = Ram_HeapSlots->unk64[1];
  draw[1] = Ram_HeapSlots->unkb8[1];

  // Window 0's left and right edge for each line: a band slanting with the
  // shards' fall, then the targets' full width, then nothing.
  if (effect->side == 0) {
    line = gMapCellBuffer;
    for (i = 0; i != 160; i++) {
      if (i >= 8 && i <= 103)
        line[i] = (0xf0 - i) | ((0x70 - i) << 8);
      else if (i <= 135)
        line[i] = 0x888;
      else
        line[i] = 0x100;
    }
  } else {
    line = gMapCellBuffer;
    for (i = 0; i != 160; i++) {
      if (i >= 8 && i <= 95)
        line[i] = (i - 8 + 160) | ((i + 0x18) << 8);
      else if (i <= 135)
        line[i] = 0x78f8;
      else
        line[i] = 0x100;
    }
  }

  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_FRAME);
  work->transferMode = 2;
  if (effect->variant == 1)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != FRAME_COUNT(effect->variant); frame++) {
    if (frame == FRAME_COUNT(effect->variant) - 16)
      BattlePlayback_StartFar(PHASE_RESULTS);

    for (i = 0; i != SHARD_COUNT(effect->variant); i++) {
      shard = &work->particles[i];

      // A falling shard moves 8 pixels down and 8 across; at the ground
      // line it lands.
      if (shard->variant == -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        ((DrawRectangle)draw[effect->variant == 2 ? 1 : 0])(canvas, work->sheet,
                                                            x, y, 32, 32);
        if (shard->y <= 0x27f) {
          if (effect->side == 0)
            shard->x -= 64;
          else
            shard->x += 64;
          shard->y += 64;
        } else {
          if ((i & 3) == 0)
            Sound_PlayFar(115);
          work->shakeFrames = 2;
          shard->variant = 0;
          for (k = 0; k != effect->count; k++)
            Func_0814cd48(effect->actors[k], 9, 5, k, 8);
        }
      }

      // A landed shard breaks over fourteen frames, three to a picture,
      // and throws a sparkle puff on each of frames 9 to 11.
      if (shard->variant != -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        if (shard->variant >= 1 && shard->variant <= 14) {
          ((DrawRectangle)draw[effect->variant == 2 ? 1 : 0])(
              canvas, work->sheet + 0x400 + ((shard->variant / 3) << 10), x, y,
              32, 32);
        }
        if (shard->variant >= 9 && shard->variant <= 11) {
          for (k = 0; k != 32; k++) {
            puff = &work->particles[32 + k];
            if (puff->variant == -1) {
              puff->variant = 18;
              puff->x = (((Random16() & 31) + shard->x / 8) << 3) + 8;
              puff->y = ((Random16() & 15) + shard->y / 8 - 15) << 3;
              break;
            }
          }
        }
        if (shard->variant <= 14)
          shard->variant++;
      }
    }

    // Each puff counts down from 18 through its nine pictures, two frames
    // apiece, centred on its point.
    for (i = 0; i != 32; i++) {
      puff = &work->particles[32 + i];
      if (puff->variant != -1) {
        if (puff->variant <= 17) {
          s32 image = puff->variant / 2;
          s32 x = puff->x / 8 - (Data_0819745e[image] >> 1);
          s32 y = puff->y / 8 - (Data_0819745e[image] >> 1);

          ((DrawRectangle)draw[effect->variant == 2 ? 1 : 0])(
              canvas, graphics + Data_0819744c[image], x, y,
              Data_0819745e[image], Data_0819745e[image]);
        }
        if (puff->variant > -1)
          puff->variant--;
      }
    }

    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(4, 4);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
  BattleEffect_ConfigureEffectDisplay();
}

#undef FRAME_COUNT

#define gDropSplashes ((struct DropSplashBuffer *)Ram_MapCellBuffer)

#define DROP_COUNT(variant) Data_08197527[(variant) * 4]
#define SPLASH_COUNT(variant) Data_08197527[(variant) * 4 + 1]
#define DROP_SPACING(variant) Data_08197527[(variant) * 4 + 2]
#define FRAME_COUNT(variant) Data_08197527[(variant) * 4 + 3]

// Douse, Drench and Deluge (effect kind 9, variants 0 to 2).

// Drops of water fall in a slanting column onto the targets' side, 8 pixels
// down and 4 across a frame. At the ground line each one flashes every
// target and bursts: its splash pictures play for eight frames while a spray
// of droplets is thrown up and falls back under gravity. Window 0 is narrowed
// line by line through an HBlank DMA table into a slanting band over the
// targets. Deluge throws its droplets higher.

// Particles hold the drops; the splash droplets live in the map cell buffer
// after the line table. A variant of -1 is a drop still falling or a free
// droplet.

void BattleEffect_RunDouse(struct BattleEffectArgument *effect) {
  void **heapCache;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  struct EffectStep *drop;
  struct EffectStep *splash;
  u16 *line;
  s32 i;
  s32 k;

  heapCache = (void **)Ram_HeapSlots;
  work = heapCache[23];
  canvas = heapCache[24];

  // Set up the canvas, the drop pictures and the droplets.
  BattleEffect_StartCanvasLayer(0x2001);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet,
                             work->sheet + 0x604, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, work->sheet, 0,
                             0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BLDCNT = 0x3f44;
  REG_WININ = 0x3337;

  // Drops draw at their maximum; droplets add their light.
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_Y));
  draw[0] = heapCache[26];
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                           BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_Y));
  draw[1] = heapCache[47];

  for (i = 0; i != 512; i++)
    gDropSplashes->splashes[i].variant = -1;

  // Line the drops up above the screen along the slant, each the variant's
  // spacing higher than the one before.
  for (i = 0; i != 64; i++) {
    s32 x;
    s32 y;

    drop = &work->particles[i];
    x = Random16() & 63;
    y = -(i * DROP_SPACING(effect->variant) + 16);
    if (effect->side == 1)
      x = x + y / 2 - 48;
    else
      x = x - y / 2 + 72;
    drop->x = x << 3;
    drop->y = y << 3;
    drop->variant = -1;
  }

  // Window 0's left and right edge for each line: a band slanting with the
  // drops' fall, then the targets' width, then nothing.
  if (effect->side == 0) {
    line = gMapCellBuffer;
    for (i = 0; i != 160; i++) {
      if (i >= 8 && i <= 103)
        line[i] = ((0x34 - i / 2) << 8) | (0xb4 - i / 2);
      else if (i <= 135)
        line[i] = 0x80;
      else
        line[i] = 0x100;
    }
  } else {
    line = gMapCellBuffer;
    for (i = 0; i != 160; i++) {
      if (i >= 8 && i <= 103)
        line[i] = ((i / 2 + 60) << 8) | (i / 2 + 188);
      else if (i <= 135)
        line[i] = 0x70f0;
      else
        line[i] = 0x100;
    }
  }

  // Douse copies the canvas plainly; Drench and Deluge blend it in.
  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_FRAME);
  if (effect->variant == 0) {
    work->transferMode = 1;
    work->transferValue = 0;
  } else if (effect->variant == 1) {
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != FRAME_COUNT(effect->variant); frame++) {
    if (frame == FRAME_COUNT(effect->variant) - 64)
      BattlePlayback_StartFar(PHASE_RESULTS);

    for (i = 0; i != DROP_COUNT(effect->variant); i++) {
      s32 x;
      s32 y;

      drop = &work->particles[i];
      x = drop->x / 8;
      y = drop->y / 8;
      if (drop->variant == -1) {
        // Falling: draw the drop and move it on, or land it.
        draw[0](canvas, work->sheet + 0x604, x, y, 24, 24);
        if (drop->y <= 0x27f) {
          if (effect->side == 0)
            drop->x -= 32;
          else
            drop->x += 32;
          drop->y += 64;
        } else {
          // Landed: throw this drop's droplets up from its middle in 16.16
          // fixed point, sound every fourth landing and flash the targets.
          drop->variant = 0;
          for (k = 0; k != SPLASH_COUNT(effect->variant); k++) {
            splash =
                &gDropSplashes->splashes[i * SPLASH_COUNT(effect->variant) + k];
            splash->x = (x + 12) << 16;
            splash->y = y << 16;
            splash->velocityX = ((Random16() & 255) - 128) << 9;
            if (effect->variant == 2)
              splash->velocityY = ((Random16() & 0x1ff) - 0x180) << 10;
            else
              splash->velocityY = ((Random16() & 255) - 255) << 10;
            splash->variant = (Random16() & 15) + 16;
          }
          if ((i & 3) == 0)
            Sound_PlayFar(132);
          for (k = 0; k != effect->count; k++)
            Func_0814cd48(effect->actors[k], 7, 5, k, 2);
        }
      } else {
        // Landed: four frames of the small splash, four of the large one.
        if (drop->variant >= 0 && drop->variant <= 3)
          draw[0](canvas, work->sheet + 0x844, x, y, 24, 24);
        else if (drop->variant <= 7)
          draw[0](canvas, work->sheet + 0xa84, x - 9, y - 9, 42, 42);
        if (drop->variant <= 14)
          drop->variant++;
      }
    }

    // Each droplet shrinks as its life runs out, centred on its point.
    for (i = 0; i != 512; i++) {
      splash = &gDropSplashes->splashes[i];
      if (splash->variant != -1) {
        s32 size = splash->variant + 1;

        if (size > 6)
          size = 6;
        draw[1](canvas, work->sheet + Data_08197424[size - 1],
                ((s16 *)&splash->x)[1] - size, ((s16 *)&splash->y)[1] - size,
                size * 2, size * 2);
        BattleEffectKernels_IntegrateVector2(splash, 60, 0x2000);
        splash->variant--;
      }
    }

    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
  BattleEffect_ConfigureEffectDisplay();
}
