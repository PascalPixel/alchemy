/// @file
/// Imil's ice, in the town and in Imil Falls Cave: the leader slides across it
/// a cell at a time the way the pad points.
///
/// The including scene names its table of slide headings, by the held
/// directions (DPAD_INDEX), as ICE_SLIDE_HEADINGS.

/// While the leader slides on the ice, turns him a sixteenth of a turn a frame
/// toward the way the pad points, at three times his walking pace, and has him
/// look around when it points nowhere.
static void Actor_SteerByPad(struct Actor *actor) {
  s32 heading;
  u16 facing;
  s32 turn;

  if (actor->timer != 0) {
    actor->timer--;
    return;
  }
  actor->steeringFlags = 0;
  heading = gIce_KeyHeadings[DPAD_INDEX(gKeysHeld)];
  if (heading == KEY_HEADING_NONE) {
    Actor_SelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
    return;
  }
  facing = actor->facing;
  turn = (s16)(heading - facing);
  if (turn > FACING_STEP)
    turn = FACING_STEP;
  if (turn < -FACING_STEP)
    turn = -FACING_STEP;
  actor->facing = facing + turn;
  Actor_SelectAnimation(actor, ANIM_WALK);
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
}

/// Slides the leader along the ice, a cell at a time the way the pad points,
/// until a wall or higher ground stops him or he slides off the ice.
static void Leader_SlideOnIce(void) {
  struct Actor *actor;
  struct FieldPosition probe;
  s32 heading;
  s32 startBehavior;
  s32 nextBehavior;
  FIXED z;
  FIXED x;

  actor = Actor_Lookup(gMainRoutineState.leaderActorId);

  for (;;) {
    heading = ICE_SLIDE_HEADINGS[DPAD_INDEX(gKeysHeld)];
    if ((u16)heading == (u16)KEY_HEADING_NONE) {
      return;
    }

    Event_Start();

    probe.x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    probe.y = actor->y.fixed;
    probe.z = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    z = probe.z;
    x = probe.x;
    startBehavior = MapGrid_GetCellBehaviorAt(actor->layer, x, z);
    FieldVector_AddPolarOffset(16, heading, &probe);

    nextBehavior = MapGrid_GetCellBehaviorAt(actor->layer, probe.x, probe.z);
    if (nextBehavior == CELL_BEHAVIOR_WALL ||
        MapGrid_GetTerrainHeight(actor->layer, probe.x, probe.z) -
                actor->y.fixed >
            toFIXED(8)) {
      actor->facing = heading;
      goto tail;
    }

    probe.x = x;
    probe.z = z;
    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1) / 10;
    actor->timer = 0;
    FieldActor_SetMoveTargetFX(actor, x, actor->y.fixed, z);

    Actor_SelectAnimation(actor, ANIM_WALK);
    Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
    Actor_WaitForMove(actor);
    actor->update = Actor_SteerByPad;

    for (;;) {
      FieldVector_AddPolarOffset(16, heading, &probe);
      nextBehavior = MapGrid_GetCellBehaviorAt(actor->layer, probe.x, probe.z);
      if (nextBehavior == CELL_BEHAVIOR_WALL) {
        break;
      }
      if (MapGrid_GetTerrainHeight(actor->layer, probe.x, probe.z) -
              actor->y.fixed >
          toFIXED(8)) {
        break;
      }
      x = probe.x;
      z = probe.z;
      actor->speedLimit = toFIXED(2);
      actor->acceleration = toFIXED(1) / 10;
      FieldActor_SetMoveTargetFX(actor, probe.x, probe.y, probe.z);
      Actor_WaitForMove(actor);
      if (nextBehavior != startBehavior) {
        goto blocked;
      }
    }

    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1);
    FieldActor_SetMoveTargetFX(actor, x, actor->y.fixed, z);
    Actor_WaitForMove(actor);
    wait(2);
  }

blocked:
  // Sliding off the ice ends the slide and gives back normal steering.
  actor->update = NULL;
  actor->steeringFlags |= ACTOR_FACES_MOTION;

  actor->acceleration = toFIXED(1) / 4;

tail:
  wait(10);
  Event_End();
}
