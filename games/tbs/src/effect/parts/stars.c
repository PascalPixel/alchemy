// Part of effect.c, the battle effect object, compiled only through it.
// Astral Blast and Shining Star.

// Astral Blast flies three ten-pointed rings at the target, knocking it back;
// Shining Star drops eight ten-pointed stars that bounce near the target.
// Each outline is drawn from a handful of corner points projected onto the
// canvas and stored in the velocity fields of EffectStep records; each edge
// is sampled at evenly spaced points and a mote or flare is stamped at each.

// Abilities: Astral Blast (kind 68).
//
// Three star-shaped rings of light fly from 40 pixels above the user at the
// first target, 12 frames apart, growing and spinning as they go, while the
// camera swings round; each knocks the target back on arrival, and the third
// lets the battle show the damage.
void BattleFx_RunAstralBlast(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  struct EffectPosition base;
  struct Vector3 scale;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  /* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter
     word fills four bytes of the frame. Measured (2026-10-14): declared as one
     DrawRectangle, the frame is 112 bytes for 116 (differ 48, frame offsets
     only). */
  BlitterPair draw;
  s32 i;
  u8 *graphics;
  struct BattleCamera *camera;
  struct MotionObject *caster;
  struct MotionObject *source;
  struct MotionObject *target;
  struct EffectStep *point;
  struct EffectStep *step;
  s32 start;
  s32 peak;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *trails;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
  graphics = WORK_SLOT(HEAP_SLOT_BATTLE_SHEET);
  camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  caster = GetBattleObjectSlotFar(effect->actor)->object;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
  ObjectDispatch_ApplyArgumentToChildrenFar(caster, 2);
  ObjectDispatch_ApplyValueToChildrenFar(caster, 48);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  draw[0] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // Each ring reaches the target in 12 frames; the second and third aim
  // twice as far across, past it.
  for (i = 0, point = work->particles; i != 3; i++) {
    SET_POSITION(point, source->x, source->y + 0x280000, source->z);
    if (i == 0)
      point->velocityX = (target->x - point->x) / 12;
    else
      point->velocityX = (target->x * 2 - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x280000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = 0;
    point++;
  }

  trails = PARTICLES;
  for (frame = 0; frame != 60; frame++) {
    // Swing the camera round, slowing over frames 40 to 47.
    if (frame <= 47) {
      struct BattleCamera *view = WORK_SLOT(HEAP_SLOT_CAMERA);
      s32 speed;

      if (frame <= 39)
        speed = 128;
      else
        speed = 0x300 - frame * 16;
      if (work->effect->side == 0)
        view->yaw -= speed;
      else
        view->yaw += speed;
    }
    for (i = 0; i != 3; i++) {
      start = i * 12;
      if (frame >= start) {
        step = &work->particles[i];
        size = (frame - start) / 4 + 2;
        if (size > 10)
          size = 10;
        EFFECT_VIEW_FROM_CAMERA(camera);
        SceneTransform_ApplyPosition(EffectStep_Coordinates(step));
        // The ring's ten points, alternately near and far from its centre,
        // projected onto the canvas; peak is the farthest depth.
        peak = 0;
        for (j = 0; j != 10; j++) {
          s32 depth;

          Transform_Push();
          SceneTransform_ApplyRoll((frame - start) << 10);
          SceneTransform_ApplyYaw(0x4000);
          scale.x = ((frame - start) << 12) + 0x1000;
          if (scale.x > 0x10000)
            scale.x = 0x10000;
          scale.y = scale.x;
          scale.z = scale.x;
          SceneTransform_ApplyScale(Vector3_Coordinates(&scale));
          SceneTransform_ApplyRoll(j * 0x199a);
          depth = EffectPosition_ApplyBaseAndYOffset(&gStars_Points[j & 1],
                                                     &screenPosition);
          if (peak < depth)
            peak = depth;
          screenPosition.x >>= 1;
          // These first stores read base, which is never set; the next
          // two overwrite them.
          trails[i * 10 + j].velocityX = screenPosition.x + base.x;
          trails[i * 10 + j].velocityY = screenPosition.y + base.y;
          trails[i * 10 + j].velocityX = screenPosition.x;
          trails[i * 10 + j].velocityY = screenPosition.y;
          Transform_Pop();
        }
        // Join the points with 16 motes each, unless the ring is too far
        // away.
        if (peak <= 399999) {
          for (j = 0; j != 10; j++) {
            struct EffectStep *from =
                &((struct EffectStep *)PARTICLES)[j + i * 10];
            struct EffectStep *to =
                &((struct EffectStep *)PARTICLES)[(j + 1) % 10 + i * 10];

            for (k = 0; k != 16; k++) {
              s32 x =
                  from->velocityX + (to->velocityX - from->velocityX) * k / 16;
              s32 y =
                  from->velocityY + (to->velocityY - from->velocityY) * k / 16;

              EFFECT_DRAW_MOTE(draw[0], canvas, graphics, x, y, size);
            }
          }
        }
        step->x += step->velocityX;
        step->y += step->velocityY;
        step->z += step->velocityZ;
        // Ring i hits 10 + i frames after its launch: knock the target 40
        // pixels back, away from the middle, and shake the screen. The third
        // ring lets the battle show the damage; the others flash the target.
        if (frame == start + i + 10) {
          target->acceleration = 0x20000;
          target->speedLimit = 0x80000;
          target->velocityY = 0x50000;
          target->verticalMotionStrength = 0xab85;
          target->autoFaceMotion = 0;
          ObjectDispatch_InitFromTable6Far(target);
          if (target->x < 0)
            Object_SetMoveTargetFar(target, target->x - 0x280000, 0, target->z);
          else
            Object_SetMoveTargetFar(target, target->x + 0x280000, 0, target->z);
          if (i == 2) {
            BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
          } else {
            AudioCommand_PlayFar(SE_HEAVY_IMPACT);
            ObjectGroup_UpdateMembers(work->effect->actors[0],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
          }
          work->shakeFrames = 4;
        }
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Abilities: Shining Star (kind 62).
//
// Eight spinning star outlines fall from 120 pixels above the user towards the
// first target, a few frames apart, and bounce off the ground near it, each
// bounce shaking the screen and flashing the targets; the battle shows the
// damage on frame 96. 128 frames.
void BattleFx_RunShiningStar(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  /* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter
     word fills four bytes of the frame. Measured (2026-10-14): declared as one
     DrawRectangle, the frame is 60 bytes for 64 (differ 40, frame offsets
     only). */
  BlitterPair draw;
  s32 i;
  u8 *graphics;
  struct BattleCamera *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  struct EffectStep *point;
  struct EffectStep *step;
  struct EffectPosition screenPosition;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *trails;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
  graphics = WORK_SLOT(HEAP_SLOT_BATTLE_SHEET);
  camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  draw[0] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // Each star aims at a random spot up to 64 pixels either side of the
  // target, 20 pixels up, over 12 frames, and starts 8i to 8i + 15 frames
  // in (its variant).
  for (i = 0, point = work->particles; i != 8; i++) {
    s32 delay = i * 8;

    SET_POSITION(point, source->x / 2, source->y + 0x780000, source->z);
    // The signed -64..63 pixel offset scales without a negative left shift.
    point->velocityX =
        (target->x + ((s32)((Random16() & 0x7f) - 64) * 65536) - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x140000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  size = 2;
  trails = PARTICLES;
  for (frame = 0; frame != 128; frame++) {
    Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
    if (frame == 96)
      BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
    for (i = 0; i != 8; i++) {
      step = &work->particles[i];
      if (frame >= step->variant) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        EffectPosition_ApplyBaseAndYOffset(step, &screenPosition);
        screenPosition.x >>= 1;
        // On the canvas, draw the star: ten points, alternately near and
        // far, turning as it falls, joined by 12 motes each.
        if (screenPosition.x >= -8 && screenPosition.x <= 127) {
          if (screenPosition.y <= 127) {
            if (screenPosition.y >= -8) {
              for (j = 0; j != 10; j++) {
                trails[i * 10 + j].velocityX =
                    screenPosition.x +
                    ((Trig_Sin(j * 0x199a - ((frame - step->variant) << 11)) *
                      gStars_Radii[j & 1]) /
                         2 >>
                     16);
                trails[i * 10 + j].velocityY =
                    screenPosition.y -
                    (Trig_Cos(j * 0x199a - ((frame - step->variant) << 11)) *
                         gStars_Radii[j & 1] >>
                     16);
              }
              for (j = 0; j != 10; j++) {
                struct EffectStep *from = &trails[j + i * 10];
                struct EffectStep *to = &trails[i * 10 + (j + 1) % 10];

                for (k = 0; k != 12; k++) {
                  s32 x = from->velocityX +
                          (to->velocityX - from->velocityX) * k / 12;
                  s32 y = from->velocityY +
                          (to->velocityY - from->velocityY) * k / 12;

                  EFFECT_DRAW_MOTE(draw[0], canvas, graphics, x, y, size);
                }
              }
            }
          }
        }
        // Bounce below 30 pixels up, losing half the speed across.
        if (step->y <= 0x1dffff) {
          step->velocityY = -step->velocityY;
          step->velocityX /= 2;
          step->velocityZ /= 2;
          work->shakeFrames = 4;
          AudioCommand_PlayFar(SE_HEAVY_IMPACT);
          EFFECT_FOR_EACH_TARGET(j, work->effect)
            ObjectGroup_UpdateMembers(work->effect->actors[j],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 8);
        }
        step->x += step->velocityX;
        step->y += step->velocityY;
        step->z += step->velocityZ;
      }
    }
    Camera_ApplyShake(4, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}
