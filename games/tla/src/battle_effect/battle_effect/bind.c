/// @file
/// Bind and Triangle Strike.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "module/battle_effect/battle_effect.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "resource_ids.h"

#define PIXEL_BUFFER Ram_MapCellBuffer

/// Turns the background canvas into a blue ramp, row by row from the top, each
/// column starting after its own random delay.
void BattleEffect_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *p, *pixel;
  u16 *palette;
  u32 delayMask;

  palette = (u16 *)0x05000040;
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  delays = Ram_HeapSlots->unk64[0];
  end = 0;
  speed = 16;
  Iwram_CopyWords(PIXEL_BUFFER, (void *)0x06008000, 0x7800);
  delayMask = 63;
  p = delays;
  do {
    *p++ = Random16() & delayMask;
  } while (p != delays + 256);
  row = 0;
  i = 0;
  do {
    end += speed / 4;
    speed++;
    while (row != end) {
      x = 0;
      do {
        y = row - delays[x];
        if (y >= 0) {
          if (y <= 119) {
            pixel = PIXEL_BUFFER + ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) +
                                    ((y / 8) << 11));
            color = ((s16 *)0x05000000)[*pixel];
            red = color & 31;
            green = (color >> 5) & 31;
            blue = (color >> 10) & 31;
            if (red < green)
              red = green;
            if (red < blue)
              red = blue;
            *pixel = 63 - red;
          }
        }
        x++;
      } while (x != 256);
      row++;
    }
    Iwram_CopyWords((void *)0x06008000, PIXEL_BUFFER, 0x7800);
    wait(1);
    if (end > 248)
      break;
    i++;
  } while (i != 27);
  palette = (u16 *)0x050000c0;
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  {
    u8 *buffer = PIXEL_BUFFER;

    i = 0;
    do {
      *buffer++ += 64;
      i++;
    } while (i != 0x7800);
  }
  Iwram_CopyWords((void *)0x06008000, PIXEL_BUFFER, 0x7800);
  wait(1);
}

void SceneTransform_ApplyScale(s32 *scale);

#define FLARE_SHEET 0x1000 // the flares, after the rune picture
#define DJINN_SHEET 0x2000 // the Jupiter Djinn, after the flares
#define DJINN_SIZE 40

#define CORNERS 3
#define EDGE_STEPS 24 // flares drawn along each edge

// Spinning triangle (kinds 38 and 218): as BattleEffect_RunTriangleStrike, a
// triangle of flares spins down onto the target and leaves the rune picture
// where it lands; when the effect has a prelude, the Jupiter Djinn also circles
// above its place for the first frames and rises away after frame 32.
void BattleEffect_RunBind(struct BattleEffectArgument *effect) {
  s32 origin[3];
  s32 originSize[3];
  struct EffectPosition anchor;
  s32 vector[3];
  struct EffectPosition screen;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 member;
  s32 *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *motion;
  s32 memberFrame;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 28
  // of 518 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, the
  // Djinn with its palette, and a transfer that fades the canvas by 4 each
  // frame.
  BattleEffect_StartCanvasLayer(1);
  if (effect->prelude == 1)
    BattleEffect_RunDjinniEntrance(effect, effect->side << 4 | 3, origin,
                                   originSize);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + FLARE_SHEET, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                             work->sheet + DJINN_SHEET, 1, 0);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    if (frame == 64)
      BattlePlayback_StartFar(0);
    BattleEffect_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);

    // The prelude: the Djinn circles where the effect was prepared, and
    // rises away after frame 32; its blitters live for this frame only.
    if (effect->prelude == 1) {
      s32 x;
      s32 y;

      x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
      y = ((Trig_Cos(frame << 11) << 2) >> 16) + origin[1] - 24;
      BattleEffect_FetchRectangleBlitters(effect->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE, DJINN_SIZE);
      if (frame <= 3)
        draw[1](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE,
                DJINN_SIZE);
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
    }

    // A fresh pair of max blitters each frame, the second flipped.
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    for (member = 0; member != 1; member++) {
      motion = GetBattleObjectSlotFar(effect->actors[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Transform_Reset();
        Transform_SetLookAt(camera, camera + 3);
        vector[0] = motion->x;
        vector[1] = motion->y;
        vector[2] = motion->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * 32 + j];
            Transform_Reset();
            if (memberFrame <= 63) {
              scale[0] = 0x2a000 - (memberFrame * 3 << 9);
              scale[1] = 0x2a000 - (memberFrame * 3 << 9);
              scale[2] = 0x2a000 - (memberFrame * 3 << 9);
              SceneTransform_ApplyScale(scale);
              Transform_ApplyRoll((64 - memberFrame) << 9);
              Transform_ApplyYaw((64 - memberFrame) << 9);
            }
            Transform_ApplyRoll(j * 0x5555);
            EffectPosition_ProjectPoint(Data_0819790c,
                                        (struct EffectPosition *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != CORNERS; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - memberFrame / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + gBattleEffect_FlarePictures[size - 1] +
                          FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// Triangle strike (kind 67): a triangle of flares spins down onto the
// target, shrinking, for 64 frames; then the rune picture is stamped in two
// halves where it landed. The caster approaches the target at frame 46, the
// palette pulses throughout and the trail fades a step every 16 frames.
void BattleEffect_RunTriangleStrike(struct BattleEffectArgument *effect) {
  struct EffectPosition anchor;
  s32 vector[3];
  struct EffectPosition screen;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 member;
  s32 *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *object;
  s32 memberFrame;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 32
  // of 428 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, and a
  // transfer that fades the canvas by 4 each frame.
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + FLARE_SHEET, 0, 0);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    if (frame == 64)
      BattlePlayback_StartFar(0);
    if (frame == 46)
      BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
    BattleEffect_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);

    // A fresh pair of max blitters each frame, the second flipped.
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    for (member = 0; member != 1; member++) {
      object = GetBattleObjectSlotFar(effect->actors[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Transform_Reset();
        Transform_SetLookAt(camera, camera + 3);
        vector[0] = object->x;
        vector[1] = object->y;
        vector[2] = object->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * 32 + j];
            Transform_Reset();
            if (memberFrame <= 63) {
              scale[0] = 0x2a000 - (memberFrame * 3 << 9);
              scale[1] = 0x2a000 - (memberFrame * 3 << 9);
              scale[2] = 0x2a000 - (memberFrame * 3 << 9);
              SceneTransform_ApplyScale(scale);
              Transform_ApplyRoll((64 - memberFrame) << 9);
              Transform_ApplyYaw((64 - memberFrame) << 9);
            }
            Transform_ApplyRoll(j * 0x5555);
            EffectPosition_ProjectPoint(Data_08197918,
                                        (struct EffectPosition *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != CORNERS; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - memberFrame / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + gBattleEffect_FlarePictures[size - 1] +
                          FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
