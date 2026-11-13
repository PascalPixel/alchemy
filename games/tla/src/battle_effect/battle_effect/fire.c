/// @file
/// Fire.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

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
