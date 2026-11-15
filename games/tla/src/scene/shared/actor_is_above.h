/// 1 when the first actor stands just above the second.
s32 Actor_IsAbove(struct Actor *a, struct Actor *b) {
  s32 dy;

  if (ABS(a->x.fixed - b->x.fixed) < px(12)) {
    dy = a->y.fixed - b->y.fixed;
    if (dy > 0 && dy < px(16) && ABS(a->z.fixed - b->z.fixed) < px(8))
      return 1;
  }
  return 0;
}
