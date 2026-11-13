/// @file
/// The Boreas summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

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
  *(u16 *)BG_PLTT = 0;
  *(u16 *)(BG_PLTT + 1) = 0;
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

  Sound_PlayFar(SE_RUMBLE);

  // The first half: rocks, the great ice block and its spray.
  for (frame = 0; frame != 288; frame++) {
    count = 16;
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 16)
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

    scale.x = Q16_ONE;
    scale.y = Q16_ONE;
    pos[3] = 0;
    pos[1] = px(255);
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
      Sound_PlayFar(SE_HEAVY_IMPACT);
    if (frame == 64)
      Sound_PlayFar(SE_HEAVY_IMPACT);
    if (frame == 104)
      Sound_PlayFar(SE_HEAVY_IMPACT);
    if (frame == 176)
      Sound_PlayFar(SE_HEAVY_IMPACT);
    if (frame == 226)
      Sound_PlayFar(SE_CRASH);
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
          PARTICLES[i].velocityY += px(1) / 8;
        }
      }
    }

    // The spray: 128 drops from (72, 56) down to the left; from 228 each
    // leaves a trail of puffs.
    if (frame > 223) {
      if (frame == 224) {
        for (i = 0; i != 128; i++) {
          PARTICLES[i].x = px(72);
          PARTICLES[i].y = px(56);
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
            PARTICLES[i].x = px(72);
            PARTICLES[i].y = px(56);
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
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);

  for (i = 0; i != 64; i++) {
    work->particles[i].x = ((Random16() & 127) + 64) << 16;
    work->particles[i].z = -px(16);
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
      Sound_PlayFar(SE_WATER);
    if (frame == 72)
      Sound_PlayFar(SE_CRASH);
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
          BattleEffectKernels_IntegrateVector2(q, 64, px(1));
          if (HI(q->y) > 120 && q->velocityY > px(8)) {
            q->velocityY = -q->velocityY / 4;
            q->y = px(120);
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
          work->particles[i].y = -px(16);
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
