/// The leader pushes the object to its resting place: the cells it leaves are
/// floor again and the cells it comes to are walls.
void PushTarget_Push(struct PushTarget target) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *actor = Actor_Get(target.id);
  s32 cell[3];
  FIXED from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 direction;
  s32 speed;
  s32 acceleration;

  // BUG: the leader's facing is taken as a 16.16 number, whose whole part is
  // always 0 for this 16-bit angle, where its sixteenth (FACING_INDEX) was
  // meant: every push takes direction 0 (east), so the pushed thing rolls with
  // animation 2 whichever way it goes and the leader's step after it is none.
#ifdef BUGFIX
  direction = FACING_INDEX(leader->facing);
#else
  direction = FIXEDtoINT(leader->facing);
#endif
  footprint = target.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = actor->x.fixed;
  from[2] = actor->z.fixed;
  cell[0] = FIELD_CELL(actor->x.fixed + toFIXED(gPushFootprints[footprint][0]));
  cell[2] = FIELD_CELL(actor->z.fixed + toFIXED(gPushFootprints[footprint][1]));
  width = (ABS(gPushFootprints[footprint][0]) +
           ABS(gPushFootprints[footprint][2])) >>
          4;
  height = (ABS(gPushFootprints[footprint][1]) +
            ABS(gPushFootprints[footprint][3])) >>
           4;
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_GROUND, cell[0], cell[2],
                                         width, height, CELL_BEHAVIOR_NORMAL);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.5, 0.1);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_PUSH);
  System_Wait(15);
  // The leader follows the object half its way.
  {
    s32 deltaX = FIXEDtoINT(target.x - from[0]) >> 1;
    s32 deltaZ = FIXEDtoINT(target.z - from[2]) >> 1;

    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, deltaX, deltaZ);
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->update = Actor_StopAtPushTarget;
  System_Wait(4);
  if (direction >= FACING_INDEX(FACING_SOUTHWEST) &&
      direction <= FACING_INDEX(FACING_NORTH_NORTHEAST))
    Actor_SelectAnimation(actor, 3);
  else
    Actor_SelectAnimation(actor, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(target.id, 0.5, 0.1);
  FieldActor_SetMoveTargetFX(actor, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.3, 0.1);
  // BUG: the leader's step after the push reads both halves from the step's
  // z: x is its sign and z its half (half of PUSH_STEP_X and PUSH_STEP_Z was
  // meant). With direction 0 both are 0.
  {
    s32 step = gPushLeaderSteps[direction];

#ifdef BUGFIX
    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                               PUSH_STEP_X(step) / 2, PUSH_STEP_Z(step) / 2);
#else
    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                               toFIXED(step) >> 31,
                               FIXEDtoINT(toFIXED(step)) >> 1);
#endif
  }
  if (target.callback != 0)
    ((void (*)(void))target.callback)();
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(actor);
  Event_PlaySound(SE_STOP_EXIT_SE);
  Event_PlaySound(SE_HEAVY_STOP);
  FieldActor_SetPositionAndResetMotion(actor, target.x, actor->y.fixed,
                                       target.z);
  Actor_SelectAnimation(actor, 1);
  target.x = target.x + toFIXED(gPushFootprints[footprint][0]);
  target.z = target.z + toFIXED(gPushFootprints[footprint][1]);
  target.x >>= 20;
  target.z >>= 20;
  originX = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].offsetX);
  originZ = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].offsetY);
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_GROUND, target.x,
                                         target.z, width, height,
                                         CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_OVERHEAD, target.x,
                                         target.z, width, height,
                                         CELL_BEHAVIOR_WALL);
  cell[0] = FIELD_CELL(from[0] + toFIXED(gPushFootprints[footprint][0]));
  cell[2] = FIELD_CELL(from[2] + toFIXED(gPushFootprints[footprint][1]));
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_OVERHEAD, cell[0],
                                         cell[2], width, height,
                                         CELL_BEHAVIOR_NORMAL);
  Event_NoOp();
}
