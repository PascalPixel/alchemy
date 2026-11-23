/// @file
/// Kolima Forest (SceneId_KolimaForestEntrance and SceneId_KolimaForestCliff):
/// the hop east from the bank onto the log floating in the river
/// (ACTOR_RIVER_LOG, which each scene names).

/// The log's animation as the leader lands on it, one of its sheet's.
#define RIVER_LOG_LANDING_ANIMATION 7

/// Hops the leader to the 16.16 position request, unless something blocks the
/// way: the leader crouches and jumps, lands on the log as it plays its
/// landing animation, sinks two pixels and rises back. Returns 1 after a hop.
static s32 KolimaForest_HopTo(struct FieldPosition *request) {
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
  u8 saved = actor->motionFlags;
  s32 ret = FieldActor_CheckMovementCollision(actor, request);

  if (ret == 0) {
    Event_Start();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = toFIXED(3);
    actor->acceleration = toFIXED(2);
    actor->velocityY = toFIXED(4);
    actor->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(actor, 0);
    Actor_MoveToAndWaitById(
        ACTOR_PARTY_LEADER,
        ((struct FieldCoordinate *)&request->x)->part[0].integer,
        ((struct FieldCoordinate *)&request->z)->part[0].integer);
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    actor->motionFlags = ret;
    Actor_SetAnimation(ACTOR_RIVER_LOG, RIVER_LOG_LANDING_ANIMATION);
    actor->y.fixed -= toFIXED(1);
    actor->groundHeight -= toFIXED(1);
    wait(2);
    actor->y.fixed -= toFIXED(1);
    actor->groundHeight -= toFIXED(1);
    wait(10);
    actor->y.fixed += toFIXED(1);
    actor->groundHeight += toFIXED(1);
    wait(4);
    actor->y.fixed += toFIXED(1);
    actor->groundHeight += toFIXED(1);
    actor->motionFlags = saved;
    Event_End();
    return 1;
  }
  return 0;
}

/// Walking east off the bank, the leader hops onto the log: 30 pixels east,
/// or, in every edition but the Japanese, to the middle of the cell 40 pixels
/// east.
static void KolimaForest_HopEast(void) {
  struct FieldPosition target;
#if !TBS_PATCH_1
  target.x = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed + toFIXED(30);
  target.y = Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  target.z = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed;
#else
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 base = leader->x.fixed & FIELD_CELL_MASK;

  target.x = base + toFIXED(8);
  target.y = leader->y.fixed;
  target.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  target.x = base + toFIXED(40);
#endif
  KolimaForest_HopTo(&target);
}
