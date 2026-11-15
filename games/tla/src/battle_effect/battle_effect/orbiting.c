/// @file
/// Swirling stars, orbiting skulls and target bursts.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "macros.h"

#define STARS ((struct Particle *)Ram_MapCellBuffer)

// Sixty-four stars swirl about a point in front of the camera and are
// drawn in.

/// Swirling stars: stars spin about a point above the field, each on its own
/// axis, then are drawn back towards it.
void BattleEffect_RunSwirlingStars(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 record[3];
  struct ScreenPoint pos;
  DrawRectangle draw[2];
  struct Particle *star;
  s32 i;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(args->side ^ 1, draw);
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
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
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
                    ((i & 3) * 770 + gBattleEffect_MotePictures[size - 1]),
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
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS();
}

#define BURST_IMAGE 0x1b00 ///< the burst pictures, after the smoke

// Bursts of smoke rise from each target in turn, eight frames apart.

/// Target bursts: on each target in turn a burst flares while puffs of smoke
/// scatter around it.
void BattleEffect_RunTargetBursts(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  void *palette;
  s32 frame;
  DrawRectangle rectangleB;
  DrawRectangle rectangleA;
  struct Particle *step;
  s32 i;
  struct ScreenPoint screen;
  s32 record[3];

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage,
                             work->sheet + BURST_IMAGE, 0, 0);
  palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  rectangleA = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  rectangleB = (DrawRectangle)Ram_HeapSlots->ramCode2;

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

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != 96; frame++) {
    s32 member;
    s32 *camera;

    camera = Ram_HeapSlots->cameraWork;
    for (member = 0; member != args->targetCount; member++) {
      struct MotionObject *memberObject;

      // The puffs rise from 40 pixels above the target.
      memberObject = BattleObject_GetFar(args->targets[member])->object;
      Transform_Reset();
      Transform_SetLookAt(camera, camera + 3);
      record[0] = memberObject->x;
      record[1] = 160 << 13;
      record[2] = memberObject->z;
      SceneTransform_ApplyPosition(record);
      if (frame >= member * 8 && frame < member * 8 + 40) {
        struct Particle *p;

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
            BattleEffectKernels_IntegrateVector3(p, 62, px(1) / 32);
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
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS();
}

#undef TRAILS

#include "graphics/vram_tab.h"

/// The second blitter slot read as what it holds, a drawing routine.
#define ALTERNATE_BLITTER_SLOT                                                 \
  (((struct {                                                                  \
     u8 before[188];                                                           \
     DrawRectangle draw;                                                       \
   } *)Ram_HeapSlots)                                                          \
       ->draw)

/// A sprite object as EffectSprite_CreateWithTiles returns it.
struct SpriteObject {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[6];
  u8 slot; // its row of gVramCacheEntries
};
