/// 1 when the first actor stands just above the second.
s32 Actor_IsAbove(struct Actor *a, struct Actor *b) {
  s32 dy;

  if (ABS(a->x.fixed - b->x.fixed) < 0xc0000) {
    dy = a->y.fixed - b->y.fixed;
    if (dy > 0 && dy < 0x100000 && ABS(a->z.fixed - b->z.fixed) < 0x80000)
      return 1;
  }
  return 0;
}
