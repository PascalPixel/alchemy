/// 1 when two actors touch.
s32 Actor_IsTouching(struct Actor *a, struct Actor *b) {
  if (ABS(a->x.fixed - b->x.fixed) < 0x80000 &&
      ABS(a->y.fixed - b->y.fixed) < 0x80000 &&
      ABS(a->z.fixed - b->z.fixed) < 0x80000)
    return 1;
  return 0;
}
