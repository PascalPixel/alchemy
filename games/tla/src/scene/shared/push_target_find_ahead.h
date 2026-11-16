/// Finds the pushable object in front of the leader: its slot, kind and the
/// direction pushed.
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor **entry;
  struct Actor *actor;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = 8;
  entry = &work->actors[8];
outer:
  actor = *entry;
  k = 0;
inner:
  if (((struct Sprite *)actor->sprite)->entries[0]->animationId ==
      gPushKinds[k]) {
    x = (PIXEL_PART(leader->x.fixed) + (gPushLeaderSteps[*direction] >> 16)) >>
        4;
    z = (PIXEL_PART(leader->z.fixed) + (s16)gPushLeaderSteps[*direction]) >> 4;
    left = (PIXEL_PART(actor->x.fixed) + gPushFootprints[k][0]) >> 4;
    top = (PIXEL_PART(actor->z.fixed) + gPushFootprints[k][1]) >> 4;
    right = (PIXEL_PART(actor->x.fixed) + gPushFootprints[k][2]) >> 4;
    bottom = (PIXEL_PART(actor->z.fixed) + gPushFootprints[k][3]) >> 4;
    if (left <= x && x < right && top <= z && z < bottom) {
      *kind = k;
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
  k++;
  if (k <= 5)
    goto inner;
  entry++;
  i++;
  if (i <= 63)
    goto outer;
  return NULL;
}
