/// @file
/// Swirling stars, orbiting skulls and target bursts.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "macros.h"

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
    SET_POSITION(star, ((Random16() & 0xff) - 127) << 16,
                 ((Random16() & 0xff) - 127) << 16,
                 ((Random16() & 0xff) - 127) << 16);
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
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, 128);
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
    SET_POSITION(step, 0, 0, 0);
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

#include "graphics/vram_tab.h"

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
