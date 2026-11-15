/// @file
/// Haunt, the orbiting spirals and the member burst.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "system/system_bank.h"
#include "resource_ids.h"

/// The gathering lights' modes (BattleEffect_RunGatheringLights), each named
/// for the effect that plays it, and its row of gGatheringLights_Counts.
enum GatheringLightsMode {
  GATHERING_LIGHTS_DEMON_NIGHT, ///< pink
  GATHERING_LIGHTS_RUMBLE,      ///< Mars Djinn colours
  GATHERING_LIGHTS_HAUNTING,    ///< Mercury Djinn colours
  GATHERING_LIGHTS_THORNY_GRAVE
};

/// The skulls live in the map buffer, 64 for each target.
#define PARTICLES ((struct Particle *)Ram_MapCellBuffer)

#define SKULLS 8 ///< per target
#define SKULL_SIZE 24
#define SKULL_CELL (24 * 24)

/// Skulls close in on each target in turn.
///
/// Abilities: Haunt.
void BattleEffect_RunHaunt(struct BattleEffectParams *args) {
  s32 k;
  struct ScreenPoint pos;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 *camera;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;

  // Set up: the skull sheet with its palette, the pink burst palette, the
  // max and additive blitters, the wave's HBlank DMA and a transfer that
  // fades the canvas by 4 each frame.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet, work->sheet, 1, 1);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet),
                       128);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

  // Scatter every skull up to 64 pixels from its centre.
  for (i = 0; i != 512; i++) {
    SET_POSITION(&PARTICLES[i], ((Random16() & 255) - 127) << 15,
                 ((Random16() & 255) - 127) << 15,
                 ((Random16() & 255) - 127) << 15);
  }
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != args->targetCount * 32 + 64; frame++) {
    s32 *row;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 92)
      EFFECT_SHOW_DAMAGE(0);

    // The background waves sideways, one sine step per line.
    row = work->bg2xLines;
    if (args->side == EFFECT_SIDE_ENEMIES) {
      for (i = 0; i != 160; i++)
        *row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
    } else {
      for (i = 0; i != 160; i++)
        *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
    }

    for (k = 0; k != args->targetCount; k++) {
      struct MotionObject *target =
          BattleObject_GetFar(args->targets[k])->object;

      // Each target's skulls turn about a point 20 pixels above it.
      Transform_Reset();
      Transform_SetLookAt(camera, camera + 3);
      point[0] = target->x;
      point[1] = px(20);
      point[2] = target->z;
      Transform_ApplyPosition(point);
      if (frame > k * 32) {
        Transform_ApplyPitch(frame << 9);
        if (frame == k * 32 + 32)
          BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, k, 32);

        // A skull moves one part in its distance toward the centre each
        // frame, and is gone once it arrives.
        for (i = 0; i != SKULLS; i++) {
          struct Particle *skull = &PARTICLES[k * 64 + i];

          if (frame > (k * 8 + i) * 4) {
            s32 x = (skull->x >> 8) * (skull->x >> 8);
            s32 y = (skull->y >> 8) * (skull->y >> 8);
            s32 z = (skull->z >> 8) * (skull->z >> 8);
            s32 dist = Iwram_Sqrt(x + y + z) >> 8;

            if (dist != 0) {
              EffectPosition_ProjectPoint((s32 *)skull, &pos);
              pos.x >>= 1;
              draw[0](canvas, work->sheet + i % 3 * SKULL_CELL,
                      pos.x - SKULL_SIZE / 2, pos.y - SKULL_SIZE / 2,
                      SKULL_SIZE, SKULL_SIZE);
              skull->x -= skull->x / dist;
              skull->y -= skull->y / dist;
              skull->z -= skull->z / dist;
              skull->variant++;
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

#define SPIRAL_SIZE 32
#define SPIRAL_CELS 4

/// Unused: no row of gAbilityBattleEffects in either game names its kind, and
/// the handler table is reached with no other kind (BattleEffect_RunFromTable),
/// so the player never sees it. Four spirals would circle 40 pixels above each
/// target in turn, 16 frames apart, for 59 frames each, 16 pixels out from
/// the centre; each target would take its damage pose 32 frames into its
/// turn.
void BattleEffect_RunUnusedOrbitingSpirals(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 point[3];
  struct ScreenPoint screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;

  // Set up: the spiral sheet with its palette, the two blitters, the wave's
  // HBlank DMA and the transfer at half strength.
  EFFECT_START_NARROW_CANVAS(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SpiralSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // On the far side the background and the spirals move 112 pixels left.
  if (args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -0x6800;
    offsetX = -112;
  } else {
    offsetX = 0;
  }

  for (frame = 0; frame != (args->targetCount * 16) + 48; frame++) {
    s32 *camera;
    s32 *row;
    s32 i;

    // The background waves sideways line by line.
    camera = Ram_HeapSlots->cameraWork;
    row = work->bg2xLines;
    if (args->side == EFFECT_SIDE_ENEMIES) {
      s32 angle;
      s32 base;

      for (i = 0, base = 0x80000, angle = frame << 10; i != 160; i++) {
        *row++ = (base - (Trig_Sin(angle) << 3)) >> 10;
        angle += 1024;
      }
    } else {
      s32 angle;

      for (i = 0, angle = frame << 10; i != 160; i++) {
        *row++ = ((Trig_Sin(angle) << 3) >> 10) - 0x7000;
        angle += 1024;
      }
    }

    Transform_Reset();
    Transform_SetLookAt(camera, camera + 3);
    member = 0;
    if (args->targetCount != 0) {
      while (member != args->targetCount) {
        struct MotionObject *target;

        target = BattleObject_GetFar(args->targets[member])->object;
        if (frame > member * 16 && frame < (member * 16) + 60) {
          s32 spin;

          // Halfway through, the target's flash stops.
          if (frame == (member * 16) + 32)
            BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_NORMAL,
                                     POSE_DAMAGE, TINT_KEEP, 0);
          point[0] = target->x;
          point[1] = px(40);
          point[2] = target->z;
          EffectPosition_ProjectPoint(point, &screen);

          // The four spirals, a quarter turn apart, each a 32 x 32 cel of
          // the sheet's four-frame animation.
          for (i = 0; i != SPIRAL_CELS; i++) {
            s32 x;
            s32 y;
            s32 half;

            spin = (frame + i * 32) << 9;
            x = (screen.x + ((Trig_Sin(spin) << 4) >> 16)) + offsetX;
            y = screen.y + ((Trig_Cos(spin) << 4) >> 16);
            half = frame / 16;
            draw[half & 1](canvas,
                           work->sheet + (((frame / 4) - (half * 4)) << 10),
                           x - SPIRAL_SIZE / 2, y - SPIRAL_SIZE / 2,
                           SPIRAL_SIZE, SPIRAL_SIZE);
          }
        }
        member++;
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Demon Night.
void BattleEffect_RunDemonNight(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_LIGHTS_DEMON_NIGHT);
}

/// Abilities: Rumble.
void BattleEffect_RunRumble(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_LIGHTS_RUMBLE);
}

/// Abilities: Haunting.
void BattleEffect_RunHaunting(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_LIGHTS_HAUNTING);
}

/// Abilities: Thorny Grave.
void BattleEffect_RunThornyGrave(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_LIGHTS_THORNY_GRAVE);
}

#define MOTES_PER_TARGET 128
#define MOTE_CELL (20 * 32)

/// From the caster's body 128 motes a target scatter every way, and from frame
/// 30 each is pulled toward the middle of its target's body, vanishing once it
/// has crossed it, in the mode's colours. The background sways sideways,
/// swelling and dying away over the effect. In Thorny Grave every mote that
/// arrives also pushes its target back.
void BattleEffect_RunGatheringLights(struct BattleEffectParams *args,
                                     s32 mode) {
  struct ScreenPoint pos;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  s32 *camera;
  s32 palette;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the mote picture, the mode's palette, and the max and additive
  // blitters for the caster's side.
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_MemberBurstImage, work->sheet, 0,
                             0);
  if (mode == GATHERING_LIGHTS_DEMON_NIGHT ||
      mode == GATHERING_LIGHTS_THORNY_GRAVE)
    palette = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == GATHERING_LIGHTS_RUMBLE)
    palette = (s32)&ResourceId_MarsDjinnSheet;
  else
    palette = (s32)&ResourceId_MercuryDjinnSheet;
  {
    const void *source_ = Resource_GetTableEntry(palette);
    __typeof__(Iwram_CopyWordsEntry) routine = Iwram_CopyWordsEntry;
    routine((void *)BG_PLTT, source_, 128);
  }
  if (mode == GATHERING_LIGHTS_THORNY_GRAVE)
    Palette_MirrorBgColors();
  BattleEffect_FetchRectangleBlitters(args->side, draw);

  // The motes start at the caster's body height and scatter every way at up
  // to two pixels a frame.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  for (k = 0; k != args->targetCount; k++) {
    struct MotionObject *source = BattleObject_GetFar(args->user)->object;
    s32 height = BattleUnit_GetEffectHeightFar(args->user);

    for (i = 0; i != MOTES_PER_TARGET; i++) {
      struct Particle *mote = &PARTICLES[k * 128 + i];

      SET_POSITION(mote, source->x, height, source->z);
      mote->velocityX = ((Random16() & 255) - 128) << 10;
      mote->velocityY = ((Random16() & 255) - 128) << 10;
      mote->velocityZ = ((Random16() & 255) - 128) << 10;
      mote->variant = 0;
    }
  }

  Task_Add(BattleEffect_ArmBg1HBlankDma, TASK_PASS_FRAME);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_ENEMY_CRY);

  for (frame = 0;
       frame != gGatheringLights_Counts[mode * 2 + 1] + args->targetCount * 20;
       frame++) {
    s32 *row;
    s32 sway;
    s32 scrollX;

    if (frame == 80) {
      if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else if (mode == GATHERING_LIGHTS_THORNY_GRAVE)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }
    Transform_Reset();
    Transform_SetLookAt(camera, camera + 3);

    // BG1 sways line by line, by up to 16 pixels at the effect's midpoint.
    // A negative scroll wraps into the halfword one short of 0x10000.
    row = work->bg2xLines;
    sway = Trig_Sin((frame << 15) / (gGatheringLights_Counts[mode * 2 + 1] +
                                     args->targetCount * 20)) *
               16 >>
           16;
    scrollX = gBgScroll[1].x;
    for (i = 0; i != 160; i++) {
      s32 x = (Trig_Sin((frame + i) << 10) * sway >> 16) + scrollX;

      if (x < 0)
        x += 0xffff;
      *row++ = x | 32 << 16;
    }

    for (k = 0; k != args->targetCount; k++) {
      struct MotionObject *target =
          BattleObject_GetFar(args->targets[k])->object;
      s32 half = BattleUnit_GetEffectHeightFar(args->targets[k]) / 2;

      if (frame == k * 20 + 71) {
        if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
          Sound_PlayFar(SE_HEAVY_IMPACT);
        else if (mode != GATHERING_LIGHTS_THORNY_GRAVE)
          Sound_PlayFar(SE_IMPACT);
      }
      if (frame == k * 20 + 70)
        BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, k, 26);
      if (frame > k * 20) {
        for (i = 0; i != gGatheringLights_Counts[mode * 2]; i++) {
          struct Particle *mote = &PARTICLES[k * 128 + i];

          if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)mote, &pos);
            pos.x >>= 1;
            draw[0](canvas, work->sheet + i % 3 * MOTE_CELL, pos.x - 10,
                    pos.y - 16, 20, 32);
            BattleEffectKernels_IntegrateVector3(mote, 62, 0);

            // Thirty frames on, the mote is pulled toward the middle of its
            // target's body.
            if (frame > k * 20 + i + 30) {
              s32 x = mote->x;
              s32 dx = (target->x - x) >> 8;
              s32 dy = (target->y + half - mote->y) >> 9;
              s32 dz = (target->z - mote->z) >> 8;
              s32 targetDistance;
              s32 moteDistance;
              s32 crossed;

              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              // It has crossed once it is as far out from the middle as its
              // target is, on the target's side.
              targetDistance = target->x < 0 ? -target->x : target->x;
              moteDistance = x;
              /* FAKEMATCH(barrier): an empty asm through moteDistance. */
              __asm__("" : "=r"(moteDistance) : "0"(moteDistance));
              if (moteDistance < 0)
                moteDistance = -moteDistance;
              /* FAKEMATCH(dead): each arm stores its test in crossed, never
               * read. */
              if (targetDistance <= moteDistance &&
                  ((x < 0 && (crossed = target->x < 0)) ||
                   (x >= 0 && (crossed = target->x >= 0)))) {
                mote->variant = -1;
                if (mode == GATHERING_LIGHTS_THORNY_GRAVE) {
                  BattleEffect_StartActorMotion(args->targets[k], 1, 0x20000,
                                                0x20000, 0x20000, i * 8 + 100);
                  if ((i & 1) && frame != 80)
                    Sound_PlayFar(SE_HEAVY_IMPACT);
                }
              }
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ArmBg1HBlankDma);
  BattleEffect_EndCanvasLayer();
}
