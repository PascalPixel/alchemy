/// Finds the pushable thing in front of the leader among the placed actors:
/// returns it, with its slot, its kind (its row of gPushKinds) and the leader's
/// facing in sixteenths of a turn. A log, pipe or fallen column is pushed only
/// from its side.
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  struct Actor *actor;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = FACING_INDEX(leader->facing);
  for (i = ACTOR_FIRST_PLACED; i <= FIELD_TREASURE_ACTOR - 1; i++) {
    actor = state->actors[i];
    for (k = 0; k <= PUSH_KINDS - 1; k++) {
      if (((struct Sprite *)actor->sprite)->entries[0]->animationId ==
          gPushKinds[k]) {
        x = (PIXEL_PART(leader->x.fixed) +
             PUSH_STEP_X(gPushLeaderSteps[*direction])) >>
            4;
        z = (PIXEL_PART(leader->z.fixed) +
             PUSH_STEP_Z(gPushLeaderSteps[*direction])) >>
            4;
        left = (PIXEL_PART(actor->x.fixed) + gPushRectangles[k][0]) >> 4;
        top = (PIXEL_PART(actor->z.fixed) + gPushRectangles[k][1]) >> 4;
        right = (PIXEL_PART(actor->x.fixed) + gPushRectangles[k][2]) >> 4;
        bottom = (PIXEL_PART(actor->z.fixed) + gPushRectangles[k][3]) >> 4;
        if (left <= x && x < right && top <= z && z < bottom) {
          *kind = k;
          // An odd kind lies north to south.
          if (k & 1) {
            if (left != FIELD_CELL(leader->x.fixed)) {
              *slot = i;
              return actor;
            }
          } else {
            if (top != FIELD_CELL(leader->z.fixed)) {
              *slot = i;
              return actor;
            }
          }
        }
      }
    }
  }
  return NULL;
}
