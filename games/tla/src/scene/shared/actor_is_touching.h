/// 1 when two actors touch.
s32 Actor_IsTouching(struct Actor *a, struct Actor *b) {
  if (ABS(a->x.fixed - b->x.fixed) < toFIXED(8) &&
      ABS(a->y.fixed - b->y.fixed) < toFIXED(8) &&
      ABS(a->z.fixed - b->z.fixed) < toFIXED(8))
    return 1;
  return 0;
}
