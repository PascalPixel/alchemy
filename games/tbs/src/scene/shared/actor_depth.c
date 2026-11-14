
// Tunnel Ruins and Venus Lighthouse: blocks the leader can stand beside, behind
// or on.

/// An update routine: draw the block under the actors
/// (ACTOR_PRIORITY_UNDERFOOT) unless the leader is north of it and no higher
/// than its height plus their distance and 4 pixels, that is, behind it rather
/// than on it.
static UNREAD_RESULT Actor_SetFlagBitByRelativeDepth(struct Actor *actor) {
  struct Actor *ref;
  u8 flag;
  ref = Actor_GetById(ACTOR_PARTY_LEADER);
  flag = actor->priorityFlags | ACTOR_PRIORITY_UNDERFOOT;
  actor->priorityFlags = flag;
  if (ref->z.fixed < actor->z.fixed) {
    s32 lim = actor->z.fixed - ref->z.fixed;
    s32 ay;
    lim += px(4);
    ay = actor->y.fixed;
    ay += lim;
    if (ref->y.fixed <= ay) {
      flag &= ~ACTOR_PRIORITY_UNDERFOOT;
      actor->priorityFlags = flag;
    }
  }
  RETURN_UNREAD(0);
}

/// Unless actor first is south of actor second, swaps their
/// positions and waits a frame.
static void Actor_SwapPositionsByDepth(s32 first, s32 second) {
  struct Actor *a = Actor_GetById(first);
  struct Actor *b = Actor_GetById(second);

  if (a->z.fixed <= b->z.fixed) {
    s32 t;

    t = a->x.fixed;
    a->x.fixed = b->x.fixed;
    b->x.fixed = t;
    t = a->y.fixed;
    a->y.fixed = b->y.fixed;
    b->y.fixed = t;
    t = a->z.fixed;
    a->z.fixed = b->z.fixed;
    b->z.fixed = t;
    wait(1);
  }
}
