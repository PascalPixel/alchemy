/// Finds the pushable object in front of the leader: its slot, kind and the
/// direction pushed.
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = 8;
  entry = &work->actors[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->sprite)->value28[0] ==
      gPushKinds[k]) {
    x = (PIXEL_PART(leader->x) + (gPushLeaderSteps[*direction] >> 16)) >> 4;
    z = (PIXEL_PART(leader->z) + (s16)gPushLeaderSteps[*direction]) >> 4;
    left = (PIXEL_PART(object->x) + gPushFootprints[k][0]) >> 4;
    top = (PIXEL_PART(object->z) + gPushFootprints[k][1]) >> 4;
    right = (PIXEL_PART(object->x) + gPushFootprints[k][2]) >> 4;
    bottom = (PIXEL_PART(object->z) + gPushFootprints[k][3]) >> 4;
    if (left <= x && x < right && top <= z && z < bottom) {
      *kind = k;
      if (k & 1) {
        if (left != leader->x >> 20) {
          *slot = i;
          return object;
        }
      } else {
        if (top != leader->z >> 20) {
          *slot = i;
          return object;
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
