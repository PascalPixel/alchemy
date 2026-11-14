/// 1 when the first actor stands just above the second.
s32 Actor_IsAbove(struct Actor *a, struct Actor *b) {
  s32 dy;

  if (ABS(a->x - b->x) < 0xc0000) {
    dy = a->y - b->y;
    if (dy > 0 && dy < 0x100000 && ABS(a->z - b->z) < 0x80000)
      return 1;
  }
  return 0;
}
