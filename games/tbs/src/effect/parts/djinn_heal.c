/// @file
/// Djinn healing.
// Part of effect.c.

#include "../effect_internal.h"

#define DJINN_HEAL_SWIRL_FRAMES 98 ///< frames of the swirl
#define DJINN_HEAL_SWIRL_MOTES 32  ///< motes swirled in

/// A small Mercury Djinni healing the party: the user glows while motes
/// swirl round a point that rises from the user to 90 pixels up over the
/// first target's x (frames 30 to 69); the Djinni appears there on frame 83.
/// Then, for each target in turn, 8 frames apart, sparkles spiral in on it
/// with the recovery sound and a glow, while the Djinni sways and finally
/// flies off. No effect table entry points here, and nothing calls it.
///
/// PARTICLES 0 to 63 hold the swirl's motes (the first 32 drawn), then 0 to
/// 511 the healing sparkles, 64 for each target; both are offsets from the
/// point the transform is centred on, in 16.16. work->sheet holds the
/// Djinni, the battle sheet slot first the motes and then the sparkle dots.
UNUSED void BattleEffect_RunDjinnHeal(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 member;
  u8 *sheet;
  DrawRectangle *draw;
  struct MotionObject *actor;
  struct MotionObject *goal;
  s32 frame;
  s32 i;
  s32 position[3];
  s32 target[3];
  s32 delta[3];
  s32 djinniPoint[3];
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition view;
  BlitterPair callbacks;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(work->effect->side ^ 1, draw = callbacks);
  actor = GetBattleObjectSlotFar(work->effect->actor)->object;
  goal = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // The swirl's motes start 20 to 51 pixels up and fly out level at 4 to 12
  // pixels a frame.
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 heading = Random16() & 0xffff;
    s32 speed = (Random16() & 0xff) + 128;

    SET_POSITION(spark, 0, ((Random16() & 31) + 20) << 16, 0);
    SET_VELOCITY(spark, (Trig_Sin(heading) * speed) >> 5, 0,
                 (Trig_Cos(heading) * speed) >> 5);
    spark->variant = 0;
  }
  // Trails keep 75% of each pixel a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // The swirl: frame 8 plays its sound, frames 30 to 69 carry it in 40
  // equal steps from the user's feet to 90 pixels up at the first target's
  // x and depth 0, frame 80 plays the Djinni's sound and frame 83 shows it.
  // target keeps that point for the healing.
  SET_VECTOR(position, actor->x, 0, actor->z);
  SET_VECTOR(target, goal->x, 180 << 15, 0);
  SET_VECTOR(delta, (target[0] - position[0]) / 40,
             (target[1] - position[1]) / 40, (target[2] - position[2]) / 40);
  for (frame = 0; frame != DJINN_HEAL_SWIRL_FRAMES; frame++) {
    struct BattleCamera *camera = gCameraWork;
    if (frame == 8)
      AudioCommand_PlayFar(SE_212);
    if (frame == 80)
      AudioCommand_PlayFar(SE_142);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame >= 30 && frame < 70) {
      position[0] += delta[0];
      position[1] += delta[1];
      position[2] += delta[2];
    }
    Transform_ApplyPosition(position);
    // The user glows white for the first 24 frames.
    if (frame == 0)
      ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1, -1,
                                0);
    if (frame == 24)
      ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
    // The swirl: 32 motes, mote i from frame i + 1, each turning about its
    // own axis (by i & 3: yaw, pitch, roll, or pitch and roll), its speed
    // damped to 60/64 each frame and, from 30 frames after it appears,
    // pulled back towards the centre by 1/256 of its offset a frame. Their
    // size runs 9 to 3 over depths 250 to 634.
    for (i = 0; i != DJINN_HEAL_SWIRL_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];
      if (frame > i && spark->variant == 0) {
        s32 size;
        Transform_Push();
        switch (i & 3) {
        case SPIN_YAW:
          Transform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case SPIN_PITCH:
          Transform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case SPIN_ROLL:
          Transform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        case SPIN_PITCH_AND_ROLL:
          Transform_ApplyPitch(-frame * (i * 32 + 256));
          Transform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        }
        EffectPosition_ProjectPoint(spark, &screen);
        screen.x >>= 1;
        Transform_Pop();
        CLAMP(screen.depth, 250, 634);
        size = 9 - (screen.depth - 250) / 64;
        EFFECT_DRAW_MOTE(callbacks[0], canvas, sheet, screen.x, screen.y, size);
        EffectStep_AdvanceWithGravity3D(spark, 60, 0);
        if (frame > i + 30) {
          s32 dx = -spark->x >> 8;
          s32 dy = -spark->y >> 8;
          s32 dz = -spark->z >> 8;

          spark->velocityX += dx;
          spark->velocityY += dy;
          spark->velocityZ += dz;
        }
      }
    }
    // The Djinni appears at the swirl's centre, bobbing 4 pixels up and down
    // every 64 frames.
    if (frame > 82) {
      SET_VECTOR(djinniPoint, 0, (Trig_Sin(frame << 10) * 4), 0);
      EffectPosition_ProjectPoint(djinniPoint, &screen);
      screen.x >>= 1;
      draw[1](canvas, work->sheet, screen.x - 10, screen.y - 17, 20, 34);
    }
    EFFECT_PRESENT_FRAME(work);
  }
  // The healing: sparkles scattered round each target, x and z within 64
  // pixels and 32 to 95 up, as BattleEffect_RunHealingMotes scatters them for
  // Ply. Target n starts on frame n * 8; the last gets 72 frames.
  for (i = 0; i != 512; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                 ((Random16() & 0x7f) + 64) << 15,
                 ((Random16() & 0xff) - 127) << 15);
    spark->variant = 0;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);
  for (frame = 0; frame != work->effect->count * 8 + 72; frame++) {
    struct BattleCamera *camera = gCameraWork;
    EFFECT_VIEW_FROM_CAMERA(camera);
    // The Djinni sways 40 pixels either way in depth every 32 frames, and
    // flies up 4 pixels a frame for the last 32.
    if (frame >= work->effect->count * 8 + 40)
      target[1] += 0x40000;
    SET_VECTOR(point, target[0], target[1],
               target[2] + Trig_Sin(frame << 11) * 40);
    EffectPosition_ProjectPoint(point, &view);
    view.x >>= 1;
    callbacks[0](canvas, work->sheet, view.x - 10, view.y - 17, 20, 34);
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(work->effect->actors[member])->object;
      s32 start = member * 8;
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(position, object->x, 160 << 14, object->z);
      Transform_ApplyPosition(position);
      // 30 frames into its turn the target hears the recovery sound, and
      // it glows from 40 to 64; its 64 sparkles spiral in, sparkle i from
      // frame i / 2 + 1 of the turn, centred 40 pixels above it. The view
      // turns 1/128 of a circle a frame, and each sparkle steps 2 pixels
      // towards the centre (distance counts 2-pixel steps) until within 2.
      if (frame == start + 30)
        AudioCommand_PlayFar(SE_RECOVERY);
      if (frame == start + 40)
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, -1, -1, 0);
      if (frame == start + 64)
        ObjectGroup_UpdateMembers(work->effect->actors[member], 0, -1, -1, 0);
      if (frame > start) {
        Transform_ApplyYaw((frame - start) << 9);
        for (i = 0; i != 64; i++) {
          struct EffectStep *spark = &PARTICLES[member * 64 + i];

          if (frame > start + i / 2) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0) {
              s32 cellIndex;

              EffectPosition_ProjectPoint(spark, &view);
              view.x >>= 1;
              CLAMP(view.depth, 314, 634);
              /* FAKEMATCH(dead): BattleEffect_RunHealingMotes' size by depth, a
               * store nothing reads; without it, 219 of 747 instructions
               * differ in tbs-en. */
              cellIndex = 3 - (view.depth - 314) / 128;
              cellIndex = (i * 4 + frame) % 9;
              {
                u8 *cell = sheet + gBattleEffect_PuffCells[cellIndex];
                u32 edge = gBattleEffect_PuffSizes[cellIndex];
                u32 half = edge >> 1;

                draw[1](canvas, cell, view.x - half, view.y - half, edge, edge);
              }
              spark->x -= spark->x / distance;
              spark->y -= spark->y / distance;
              spark->z -= spark->z / distance;
            }
          }
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

/// Turns the battle camera's yaw by work->cameraDelta, or by half of it on a
/// frame the effect has not marked.
///
/// Runs each frame as a scheduler callback added by BattleEffect_RunFire
/// (fire.c) and BattleEffect_RunFroth (vortices.c), which remove it before they
/// return. Each of them moves cameraPhase from CAMERA_PHASE_IDLE to
/// CAMERA_PHASE_FRAME_DONE at the end of its own frame. Then the callback
/// turns the whole delta and clears the phase; otherwise it turns half the
/// delta and moves the phase between CAMERA_PHASE_IDLE and
/// CAMERA_PHASE_HALF_TURNED, which the effects leave unmarked. Changes
/// the camera slot's yaw and work->cameraPhase.
/// XXX: likely to keep the turn smooth when an effect frame runs long,
/// because every frame still turns at least half the delta; a run with a
/// slow effect frame would settle it.
void Camera_ApplyPhasedDelta(void) {
  // Both come from the one slot table; a separate camera base compiles 4
  // bytes longer.
  void **slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  struct BattleEffectWork *work = slots[0];
  struct BattleCamera *camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  s32 *phase = &work->cameraPhase;

  if (*phase == CAMERA_PHASE_FRAME_DONE) {
    camera->yaw += work->cameraDelta;
    *phase = CAMERA_PHASE_IDLE;
  } else {
    camera->yaw += work->cameraDelta / 2;
    if (*phase == CAMERA_PHASE_HALF_TURNED)
      *phase = CAMERA_PHASE_IDLE;
    else
      *phase = CAMERA_PHASE_HALF_TURNED;
  }
}

#undef DJINN_HEAL_SWIRL_FRAMES
#undef DJINN_HEAL_SWIRL_MOTES
