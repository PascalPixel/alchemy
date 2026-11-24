/// @file
/// Mercury Lighthouse and its entrance, once the pool is woken: standing at the
/// pool's heart (CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART) gives the leader
/// three lights, shown as a column of light over them. Each hop into the pool
/// (the floor's region, its cell behavior) spends one, and a light comes to
/// hold the leader up where they land. A hop within the pool with no light left
/// sinks the leader, who is put back at the heart.
/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP takes the lights away, and
/// Retreat removes the column.

/// A charging spark rises a pixel a frame for this many frames, its scale
/// following half a turn of sine.
#define SPARK_RISE_FRAMES 32

/// An actor script call for a charging spark: it rises with the leader,
/// swelling and shrinking, and is released after SPARK_RISE_FRAMES.
static s32 MercuryLighthouse_RiseSpark(struct Actor *effect) {
  struct Actor *anchor = effect->watchedActor;
  s16 step = ++effect->timer;
  FIXED scale;

  if (step >= SPARK_RISE_FRAMES) {
    return 0;
  }

  scale = Trig_Sin(step * (TRIG_TURN / 2 / SPARK_RISE_FRAMES));
  effect->scaleX = scale;
  effect->scaleY = scale;
  effect->x.fixed = anchor->x.fixed;
  effect->y.fixed += toFIXED(1);
  effect->z.fixed = anchor->z.fixed;
  return 1;
}

/// An actor script call for the light that flies ahead to a hop's landing
/// cell: it grows to three quarters of its size over 16 frames.
static s32 MercuryLighthouse_GrowLight(struct Actor *actor) {
  s16 frame;

  frame = ++actor->timer;
  if (frame > 16) {
    return 0;
  }
  actor->scaleX = frame * 3 * (FIXED_ONE / 64);
  actor->scaleY = frame * 3 * (FIXED_ONE / 64);
  return 1;
}

/// An actor script call for a light that goes out: it swells to half as large
/// again over 16 frames before its script releases it.
static s32 MercuryLighthouse_SwellLightOut(struct Actor *actor) {
  s16 frame;

  frame = ++actor->timer;
  if (frame > 16) {
    return 0;
  }
  actor->scaleX = frame * (FIXED_ONE / 32) + FIXED_ONE;
  actor->scaleY = frame * (FIXED_ONE / 32) + FIXED_ONE;
  return 1;
}

/// An actor script call for the column of light: it glides after the leader,
/// an eighth of the way each frame up to 2 pixels, and flickers white every
/// two frames.
static s32 MercuryLighthouse_FollowLeader(struct Actor *actor) {
  struct Actor *leader;
  FIXED targetZ;
  FIXED targetX;
  s32 deltaX;
  s32 deltaZ;
  s32 distance;
  s32 moveDistance;

  actor->speedLimit = toFIXED(2);
  leader = actor->watchedActor;
  actor->acceleration = toFIXED(1);
  targetX = leader->x.fixed;
  targetZ = leader->z.fixed;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;

  // Measure in whole pixels first; use fixed-point precision near the leader.
  deltaX = (targetX - actor->x.fixed) / toFIXED(1);
  deltaZ = (targetZ - actor->z.fixed) / toFIXED(1);
  distance = (u16)Iwram_Sqrt(deltaX * deltaX + deltaZ * deltaZ);
  deltaX = targetX - actor->x.fixed;
  deltaZ = targetZ - actor->z.fixed;
  distance <<= 16;
  if (distance < toFIXED(64))
    distance =
        Math_SqrtFX(Iwram_MulFX(deltaX, deltaX) + Iwram_MulFX(deltaZ, deltaZ));
  moveDistance = distance / 8;
  if (moveDistance > actor->speedLimit)
    moveDistance = actor->speedLimit;

  if (distance < toFIXED(1) / 4) {
    actor->x.fixed = targetX;
    actor->z.fixed = targetZ;
  } else {
    if (distance > moveDistance) {
      deltaX = Iwram_MulFX(Iwram_RatioMulQ14(distance, deltaX), moveDistance);
      deltaZ = Iwram_MulFX(Iwram_RatioMulQ14(distance, deltaZ), moveDistance);
    }
    actor->x.fixed += deltaX;
    actor->z.fixed += deltaZ;
  }

  {
    struct Sprite *sprite = actor->sprite;
    struct SpriteAnimation *entry = sprite->entries[0];

    entry->colorMode = ((gFrameCount >> 1) & 1) * SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
  }
  return 1;
}

/// A pair of charging sparks rises around the leader, one in front and one
/// behind, with SE_PSYNERGY_START.
static void MercuryLighthouse_SpawnSparkPair(void) {
  struct Actor *leader;
  struct Actor *spark;
  struct Sprite *sprite;

  leader = gFieldState->actors[gMainRoutineState.leaderActorId];
  spark = FieldActor_Create(SPRITE_GOLDEN_RINGS, leader->x.fixed,
                            leader->y.fixed, leader->z.fixed);
  if (spark != NULL) {
    spark->groundHeight = leader->groundHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gMercuryLighthouse_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedActor = leader;
    if (sprite != NULL) {
      Sprite_StartAnimation(sprite, 2);
      sprite->flags = 0;
      sprite->image.priority = 1;
    }
  }
  spark = FieldActor_Create(SPRITE_GOLDEN_RINGS, leader->x.fixed,
                            leader->y.fixed, leader->z.fixed);
  if (spark != NULL) {
    spark->groundHeight = leader->groundHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gMercuryLighthouse_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedActor = leader;
    spark->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    if (sprite != NULL) {
      Sprite_StartAnimation(sprite, 1);
      sprite->flags = 0;
    }
  }
  Sound_Play(SE_PSYNERGY_START);
}

/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, the pool's heart: the column of
/// light appears over the leader if it is not there, and grows a stage with
/// each pair of sparks, half a second apart, up to three lights. The column's
/// cell is where a sinking leader comes back to; FLAG_NO_ENCOUNTERS_IN_AREA is
/// set while the leader has lights.
void MercuryLighthouse_ChargeLights(void) {
  struct MercuryLighthouseLights *state;
  struct Actor *leader;
  struct Actor *pillar;
  struct Actor *actor;
  s32 step;

  state = *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  leader = gFieldState->actors[gMainRoutineState.leaderActorId];
  if (state->levels < 0 || state->levels > 2)
    return;
  Event_Start();
  pillar = state->pillar;
  if (pillar == NULL) {
    actor = FieldActor_Create(SPRITE_GOLDEN_RINGS, leader->x.fixed,
                              leader->y.fixed + toFIXED(24), leader->z.fixed);
    if (actor != NULL) {
      struct Sprite *sprite = actor->sprite;

      actor->groundHeight = leader->groundHeight;
      Actor_StartScript(actor, gMercuryLighthouse_PillarScript);
      actor->watchedActor = leader;
      actor->motionFlags = ACTOR_FLOATS;
      actor->y.fixed -= toFIXED(1) / 2;
      if (sprite != NULL) {
        sprite->flags = 0;
        sprite->image.priority = 1;
      }
      actor->spriteStorageKind = 0;
      state->pillar = actor;
      pillar = actor;
    } else {
      pillar = state->pillar;
    }
  }
  for (step = state->levels; step <= 2; step++) {
    MercuryLighthouse_SpawnSparkPair();
    wait(30);
    pillar->spriteStorageKind = 1;
    Actor_SelectAnimation(pillar, 5 - step);
  }
  state->levels = 3;
  state->targetX = (pillar->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  state->targetZ = (pillar->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  GameFlag_Set(FLAG_NO_ENCOUNTERS_IN_AREA);
  Event_End();
}

/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP: the leader loses their lights,
/// and the column swells and fades away.
void MercuryLighthouse_LoseLights(void) {
  struct MercuryLighthouseLights *state =
      *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
          HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  struct Actor *target;

  if (state->levels == 0) {
    return;
  }

  state->levels = 0;
  GameFlag_Clear(FLAG_NO_ENCOUNTERS_IN_AREA);

  target = state->pillar;
  if (target != NULL) {
    target->timer = 0;
    Actor_StartScript(target, gMercuryLighthouse_LightOutScript);
    Actor_SelectAnimation(target, 7);
    state->pillar = NULL;
  }
}

/// Pushing against the pool's edge
/// (CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART,
/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER and
/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP): the leader hops two cells the
/// way the pad points, unless the hop leads into the pool without a light or is
/// blocked. A hop into the pool spends a light, which flies ahead to the
/// landing cell; the column shrinks a stage, or goes with the last light.
/// Landing in the pool with no light under them, the leader sinks until a key
/// is pressed after half a second, and is put back at the heart.
void MercuryLighthouse_HopAcrossPool(void) {
  struct FieldState *fieldState;
  struct MercuryLighthouseLights *state;
  struct Actor *actor;
  struct Actor *source;
  struct Actor *obj;
  struct Sprite *sprite;
  struct MapGridCell *from;
  struct MapGridCell *to;
  u8 saved;
  u8 *flags;
  s32 angle;
  u32 keys;
  s32 n;
  s32 frame;
  struct FieldCoordinate pos[3];

  state = *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  fieldState = gFieldState;
  actor = fieldState->actors[gMainRoutineState.leaderActorId];
  flags = &actor->motionFlags;
  saved = *flags;
  keys = gKeysHeld;
  angle = gMercuryLighthouse_MoveAngles[DPAD_INDEX(keys)];
  if ((s16)gMercuryLighthouse_MoveAngles[DPAD_INDEX(keys)] == -1)
    return;
  pos[0].fixed = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  pos[1].fixed = actor->groundHeight;
  pos[2].fixed = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  from = &MAPGRID_CELLS[(pos[2].fixed / toFIXED(16) * MAPGRID_SIZE) +
                        pos[0].fixed / toFIXED(16)];
  Vector_AddPolarOffset(toFIXED(32), angle, &pos[0].fixed);
  to = &MAPGRID_CELLS[(pos[2].fixed / toFIXED(16) * MAPGRID_SIZE) +
                      pos[0].fixed / toFIXED(16)];
  if (from->behavior != state->poolBehavior &&
      to->behavior == state->poolBehavior && state->levels == 0)
    return;

  Event_Start();
  if (FieldActor_CheckMovementCollision(actor, (struct FieldPosition *)pos) !=
      0)
    return;
  obj = state->stepEffect;
  if (obj != NULL) {
    obj->timer = 0;
    Actor_StartScript(obj, gMercuryLighthouse_LightOutScript);
    Actor_SelectAnimation(obj, 7);
    state->stepEffect = NULL;
  }
  if (to->behavior == state->poolBehavior && state->levels != 0) {
    source = state->pillar;
    obj = FieldActor_Create(SPRITE_GOLDEN_RINGS, source->x.fixed,
                            source->y.fixed, source->z.fixed);
    if (obj != NULL) {
      sprite = obj->sprite;
      obj->groundHeight = source->groundHeight;
      Actor_StartScript(obj, gMercuryLighthouse_GrowLightScript);
      obj->motionFlags = 0;
      obj->timer = 0;
      obj->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      obj->speedLimit = toFIXED(4);
      obj->acceleration = toFIXED(2);
      FieldActor_SetMoveTargetFX(obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
      if (sprite != NULL) {
        Sprite_StartAnimation(sprite, 6);
        sprite->flags = 0;
      }
      state->stepEffect = obj;
    }
    n = state->levels - 1;
    state->levels = n;
    if (n == 0) {
      Actor_Release(state->pillar);
      state->pillar = NULL;
      GameFlag_Clear(FLAG_NO_ENCOUNTERS_IN_AREA);
    } else if (state->pillar != NULL) {
      Actor_SelectAnimation(state->pillar, 6 - n);
    }
  }
  Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
  wait(3);
  Sound_Play(SE_JUMP);
  Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
  actor->speedLimit = toFIXED(3);
  actor->acceleration = toFIXED(2);
  actor->velocityY = toFIXED(4);
  *flags &= ACTOR_JUMP_KEEPS_MOTION;
  Actor_SetSpriteFlags(actor, 0);
  Actor_MoveToAndWaitById(ACTOR_PARTY_LEADER, pos[0].part[0].integer,
                          pos[2].part[0].integer);
  Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
  wait(2);
  if (to->behavior != state->poolBehavior)
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  else
    Sound_Play(SE_SPLASH_SMALL);
  wait(1);
  *flags = saved;
  if (to->behavior == state->poolBehavior && state->stepEffect == NULL) {
    Actor_SelectAnimation(actor, ANIM_LEADER_BATHE);
    Sound_Play(SE_WATER_MOVE);
    for (frame = 0;; frame++) {
      if ((frame & 15) == 0)
        MercuryLighthouse_SpawnRipple(actor);
      if (frame > 31 && gKeysPressed != 0)
        break;
      wait(1);
    }
    Sound_Play(SE_STOP_ACTION_SE);
    wait(1);
    actor->x.fixed = state->targetX;
    actor->z.fixed = state->targetZ;
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  }
  state->rippleTimer = 0;
  Event_End();
  fieldState->walkingDistance +=
      Iwram_MulFX(fieldState->walkingDistanceScale, toFIXED(32));
}

/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER: while the leader stands there,
/// a blue ripple spreads from him every 10 to 39 frames.
void MercuryLighthouse_RippleAroundLeader(void) {
  struct MercuryLighthouseLights *state;
  struct Actor *leader;

  state = *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  leader = gFieldState->actors[gMainRoutineState.leaderActorId];
  if (state->rippleTimer != 0) {
    state->rippleTimer--;
  } else {
    MercuryLighthouse_SpawnRipple(leader);
    state->rippleTimer = (Random_Below(30)) + 10;
  }
}

/// Retreat (PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST)):
/// remove the column of light.
static void MercuryLighthouse_RemoveColumnOnRetreat(void) {
  struct MercuryLighthouseLights **published =
      HeapSlot_AllocEwram(HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, 4);
  struct MercuryLighthouseLights *state;
  struct Actor *pillar;

  if (published == NULL)
    return;
  state = *published;
  pillar = state->pillar;
  if (pillar == NULL)
    return;
  Actor_Release(pillar);
  state->pillar = NULL;
}
