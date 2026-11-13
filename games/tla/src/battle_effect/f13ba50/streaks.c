/// @file
/// Streak volleys and mote fountains.
// Part of f13ba50.c.

#include "../f13ba50_internal.h"

struct BattleEffectArgument;

void Func_0813c5f4(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Sonic Wave (kind 300).
void BattleEffect_RunSonicWave(void *object) { Func_0813c5f4(object, 0); }

/// Abilities: Shriek, Harpy (kind 301).
void BattleEffect_RunShriek(void *object) { Func_0813c5f4(object, 1); }

/// Abilities: Banshee Howl (kind 302).
void BattleEffect_RunBansheeHowl(void *object) { Func_0813c5f4(object, 2); }

/// Abilities: War Cry (kind 304).
void BattleEffect_RunWarCry(void *object) { Func_0813c5f4(object, 4); }

/// Abilities: Wicked Howl (kind 305).
void BattleEffect_RunWickedHowl(void *object) { Func_0813c5f4(object, 3); }

/// The streak volley in lime (mode 1). UNUSED: no effect table entry
/// points here.
void BattleEffect_RunLimeStreaks(void *object) { Func_0813c5f4(object, 1); }



#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// Four signed bytes for each mode (The Broken Seal's gStreaks_Styles).
extern s8 Data_08197533[];
#define RECORD(mode, field) Data_08197533[(mode) * 4 + (field)]
#define RECORD_ANIMATE 0 // cycle the streak through its three pictures
#define RECORD_HIT 1     // sound the hit and flash the target
#define RECORD_MOTION 2  // the target's recoil motion, or -1
#define RECORD_SHAKE 3   // tremble the target after the hit

// The struck target's sideways tremble, in pixels (gStreaks_Tremble).
extern s8 Data_08197547[];

#define STREAK_MAX 3 // streaks for each target

extern char ResourceId_FiveModeImage;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_LimePalette;
extern char ResourceId_VioletPaletteD;
extern char ResourceId_RedPaletteB;
extern char ResourceId_CyanPalette;

void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);

// Sonic Wave (mode 0), Shriek (1), Banshee Howl (2), Wicked Howl (3) and War
// Cry (4): effect kinds 300 to 305.

// Three streaks rise 20 pixels above the user and fly at each target in turn,
// 6 frames apart, reaching it in 24 frames; each target starts 32 frames
// after the one before. A streak is a 20x40 picture drawn twice, its upper
// half mirrored, and the canvas wobbles line by line on a sine wave while the
// transfer halves the old picture each frame into a trail. The mode chooses
// the colour, whether the streak animates, the hit's sound and flash, a
// recoil, and a tremble of the target.

void Func_0813c5f4(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  void *canvas;
  s32 k;
  DrawRectangle drawLower;
  DrawRectangle drawUpper;
  struct BattleCamera *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  s32 resource;
  s32 frame;
  s32 j;
  s32 streaks = STREAK_MAX;
  s32 lift = 20 << 16;

  heap = Ram_HeapSlots;
  /* FAKEMATCH(barrier): an empty asm through heap hides that it holds the
   * heap slots' fixed address, so later reads go through its register, not
   * the address built again; without it, 16 of 580 instructions differ in
   * tla-en. */
  __asm__("" : "=r"(heap) : "0"(heap));
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  camera = heap->cameraWork;
  BattleEffect_StartCanvasLayer(1);

  // The upper half draws mirrored top to bottom; the far side mirrors
  // left to right as well.
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_Y));
    drawUpper = heap->unk64[1];
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));
    drawLower = heap->unkb8[1];
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP));
    drawUpper = heap->unk64[1];
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    drawLower = heap->unkb8[1];
  }

  // The streak pictures, and the mode's colours.
  Resource_LoadAndDecompress((s32)&ResourceId_FiveModeImage, work->sheet, 0, 0);
  switch (mode) {
  case 0:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    resource = (s32)&ResourceId_VioletPaletteD;
    break;
  case 3:
    resource = (s32)&ResourceId_RedPaletteB;
    break;
  case 4:
  default:
    resource = (s32)&ResourceId_CyanPalette;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  work->transferMode = 2;   // each transfer halves the old picture,
  work->transferValue = 50; // leaving a trail
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  source = GetBattleObjectSlotFar(effect->actor)->object;
  {
    s32 total = streaks * effect->count * 6 + 48;

    // Each streak flies from 20 pixels above the user to 20 pixels above
    // its target in 24 frames.
    for (k = 0; k != effect->count; k++) {
      target = GetBattleObjectSlotFar(effect->actors[k])->object;
      for (j = 0; j != streaks; j++) {
        struct EffectStep *streak = &work->particles[k * streaks + j];

        streak->x = source->x;
        streak->y = source->y + lift;
        streak->z = source->z;
        streak->velocityX = (target->x - streak->x) / 24;
        streak->velocityY = (target->y + lift - streak->y) / 24;
        streak->velocityZ = (target->z - streak->z) / 24;
        streak->variant = 0; // frames since it set out
      }
    }

    for (frame = 0; frame != total; frame++) {
      s32 *row = work->bg2xLines;

      // Wobble BG2's 160 lines on a sine wave, and fade the last 16
      // frames out.
      for (k = 0; k != 160; k++)
        *row++ = (0x80000 - Trig_Sin((frame + k) << 12) * 2) >> 10;
      if (frame > total - 16)
        REG_BLDALPHA = (total - frame) | 0x1000;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      for (k = 0; k != effect->count; k++) {
        // Each target's streaks set out with the mode's cry.
        if (frame == k * 32) {
          if (mode == 1)
            Sound_PlayFar(192);
          if (mode == 2)
            Sound_PlayFar(189);
          if (mode == 3)
            Sound_PlayFar(148);
          if (mode == 4)
            Sound_PlayFar(138);
        }
        if (frame >= k * 32) {
          for (j = 0; j != streaks; j++) {
            // lift is reused for the streak's start frame; a local of its own
            // differs.
            lift = k * 32 + j * 6;
            if (frame >= lift) {
              struct EffectStep *streak = &work->particles[k * streaks + j];
              s32 step;

              // The picture changes every 8 frames, stopping at the sixth.
              EffectPosition_ProjectPoint((s32 *)streak, &position);
              position.x >>= 1;
              step = streak->variant / 8;
              if (step > 5)
                step = 5;
              if (RECORD(mode, RECORD_ANIMATE)) {
                u8 *cell = &work->sheet[step * 800 + frame / 2 % 3 * 4800];

                drawUpper(canvas, cell, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, cell, position.x - 10, position.y, 20, 40);
              } else {
                u8 *cell = &work->sheet[step * 800 + 2 * 4800];

                drawUpper(canvas, cell, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, cell, position.x - 10, position.y, 20, 40);
              }
              BattleEffectKernels_IntegrateVector3(streak, 64, 0);
              streak->variant++;
            }
          }

          // From 6 frames after the hit, tremble the target sideways for
          // 32 frames.
          if (RECORD(mode, RECORD_SHAKE) && frame >= k * 32 + 30 &&
              frame < k * 32 + 62) {
            struct MotionObject *struck =
                GetBattleObjectSlotFar(effect->actors[k])->object;

            struck->x += Data_08197547[(frame - k * 32 - 30) % 8] << 16;
            if (struck->x > 0)
              struck->x += 0x8000;
            else
              struck->x -= 0x8000;
            Func_0814cd48(effect->actors[k], -1, 5, -1, 0);
          }

          // The hit: a sound and two flashes, 16 frames apart. The first
          // hit lets the battle show its results.
          if (RECORD(mode, RECORD_HIT)) {
            if (frame == k * 32 + 24) {
              Sound_PlayFar(133);
              if (k == 0)
                Func_081180e8(-1);
              Func_0814cd48(effect->actors[k], 7, 5, k, 8);
            }
            if (frame == k * 32 + 40)
              Func_0814cd48(effect->actors[k], 7, 5, k, 8);
          }
          if (RECORD(mode, RECORD_MOTION) != -1 && frame == k * 32 + 24) {
            work->shakeFrames = 4;
            Func_08118088(effect->actors[k], RECORD(mode, RECORD_MOTION));
          }
        }
      }
      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

void Func_0813cb34(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Sleep Star (kind 349).
void BattleEffect_RunSleepStar(void *object) { Func_0813cb34(object, 1); }

/// Abilities: Soothing Star (kind 344).
void BattleEffect_RunSoothingStar(void *object) { Func_0813cb34(object, 0); }

/// Abilities: Slaver (kind 395).
void BattleEffect_RunSlaver(void *object) { Func_0813cb34(object, 2); }

typedef DrawRectangle BlitterPair[2];

enum { MODE_SOOTHING_STAR, MODE_SLEEP_STAR, MODE_SLAVER };

// The motes live in the map cell buffer.
#define EFFECT_STEPS ((struct EffectStep *)Ram_MapCellBuffer)

// The mote's cell for each size from 1 to 6 (The Broken Seal's
// gBattleEffect_MoteCells).
extern u16 gData_08197410[];

extern char ResourceId_Data134; // The Broken Seal's ParticleSpritesA
extern char ResourceId_OrangePaletteA;
extern char ResourceId_EarthWallSheet;
extern char ResourceId_GreenPalette;

void SummonEffect_RunCanvasMode1(void *effect);

// The IWRAM routines are called through one inline each, as The Broken Seal's
// iwram_call.h layer calls them: called directly, CSE shares the size
// between the two clears.
static inline s32 ClearWords(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_ClearWords, 16 of 477 instructions of Func_0813cb34 differ in
   * tla-en. */
  return Iwram_ClearWords(destination, size);
}

// Soothing Star (mode 0, effect kind 344), Sleep Star (mode 1, kind 349) and
// Slaver (mode 2, kind 395).

// 256 motes burst from 5 pixels above the user and spread in every
// direction; the first 128 are drawn, a quarter of them starting every 8
// frames, each swaying on a sine wave and sized by its depth. Soothing Star's
// burst is half as wide and sounds as it touches each target; Sleep Star's
// drifts away from the user's side. From frame 48 the targets flash one
// after another, 8 frames apart, and the first flash lets the battle show its
// results. Slaver instead throws green drops up in a half-circle fan, turned
// by the user's side, with a sound at the start; they fall back and bounce
// on the ground for 96 frames, and then Impair's canvas effect runs on the
// targets.

void Func_0813cb34(struct BattleEffectArgument *effect, s32 mode) {
  s32 point[3];
  struct EffectPosition position;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 frames;
  struct MotionObject *source;
  struct EffectStep *mote;
  s32 resource;
  s32 frame;
  s32 i;
  s32 group = 32; // motes that start together

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  if (mode == MODE_SOOTHING_STAR)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(1);

  // Motes draw at their maximum; the second blitter, mirrored top to
  // bottom, is loaded but not used.
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                           BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_Y));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == MODE_SOOTHING_STAR)
    resource = (s32)&ResourceId_OrangePaletteA;
  else if (mode == MODE_SLEEP_STAR)
    resource = (s32)&ResourceId_EarthWallSheet;
  else
    resource = (s32)&ResourceId_GreenPalette;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Launch every mote from the user in 16.16 fixed point, with a speed and
  // a direction of its own. Slaver throws them up, faster, in a half-circle
  // fan turned by the user's side, from up to 16 pixels higher.
  source = GetBattleObjectSlotFar(effect->actor)->object;
  for (i = 0; i != 256; i++) {
    s32 speed;
    s32 angle;

    mote = &EFFECT_STEPS[i];
    if (mode == MODE_SLAVER) {
      if (effect->side == 0)
        angle = (Random16() & 0x7fff) + 0x8000;
      else
        angle = Random16() & 0x7fff;
      speed = (Random16() & 0x3ff) + 0x800;
      mote->y = source->y + (Random16() << 4) + 0x50000;
    } else {
      angle = Random16();
      speed = (Random16() & 0x3ff) + 32;
      mote->y = source->y + 0x50000;
    }
    mote->x = source->x;
    mote->z = source->z;
    mote->velocityX = Trig_Sin(angle) * speed >> 8;
    if (mode == MODE_SLAVER) {
      mote->velocityY = ((Random16() & 255) + 128) << 10;
      mote->velocityZ = -(Trig_Cos(angle) * speed) >> 9;
    } else {
      mote->velocityY = ((Random16() & 255) - 32) << 9;
      mote->velocityZ = -(Trig_Cos(angle) * speed << 1) >> 8;
    }
    mote->variant = (Random16() & 31) + 48;
    if (mode == MODE_SOOTHING_STAR) {
      mote->velocityX /= 2;
      mote->velocityZ /= 2;
    }
  }

  frames = 96;
  if (mode != MODE_SLAVER)
    frames = 128;
  for (frame = 0; frame != frames; frame++) {
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (mode == MODE_SLAVER && frame == 0)
      Sound_PlayFar(103);

    for (i = 0; i != 128; i++) {
      mote = &EFFECT_STEPS[i];
      if (frame >= i / group * 8 && mote->variant >= 0) {
        s32 size;

        // Sway the mote, project it and size it by its depth: 6 pixels
        // near, 1 far.
        point[0] = mote->x + Trig_Sin((i * 4 + mote->variant) << 10) * 16;
        point[1] = mote->y;
        point[2] = mote->z;
        EffectPosition_ProjectPoint(point, &position);
        position.x >>= 1;
        if (position.depth <= 313)
          position.depth = 314;
        if (position.depth > 634)
          position.depth = 634;
        size = 6 - (position.depth - 314) / 64;
        draw[0](canvas, sheet + gData_08197410[size - 1], position.x - size / 2,
                position.y - size, size, size * 2);

        // Slaver's drops fall and bounce off the ground; the others rise
        // slowly. Sleep Star's drift away from the user's side.
        if (mode == MODE_SLAVER) {
          BattleEffectKernels_IntegrateVector3(mote, 62, -0x2000);
          if (mote->y <= 0x4ffff)
            mote->velocityY = -mote->velocityY;
        } else {
          BattleEffectKernels_IntegrateVector3(mote, 62, 0x400);
        }
        if (mode == MODE_SLEEP_STAR) {
          if (source->x < 0)
            mote->velocityX += 0x2000;
          else
            mote->velocityX -= 0x2000;
        }
        mote->variant--;
      }
    }

    // From frame 48, flash the targets one after another, 8 frames apart.
    if (mode == MODE_SLEEP_STAR) {
      for (i = 0; i != effect->count; i++) {
        if (frame == i * 8 + 48) {
          Func_081180e8(-1);
          Func_0814cd48(effect->actors[i], 7, 5, i, 8);
        }
      }
    } else if (mode == MODE_SOOTHING_STAR) {
      for (i = 0; i != effect->count; i++) {
        if (frame == i * 8 + 48) {
          Sound_PlayFar(126);
          Func_081180e8(-1);
          Func_0814cd48(effect->actors[i], 7, -1, i, 8);
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  if (mode == MODE_SLAVER) {
    // Clear the canvas and run Impair's effect on the targets.
    ClearWords((void *)0x06004000, 0x3c00);
    ClearWords(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    SummonEffect_RunCanvasMode1(effect);
  } else {
    BattleEffect_EndCanvasLayer();
  }
}
