/// 1 when two actors stand side by side.
s32 Actor_IsBeside(struct Actor *a, struct Actor *b) {
  s32 dy;

  if (ABS(a->x.fixed - b->x.fixed) < px(12)) {
    dy = a->y.fixed - b->y.fixed;
    if (dy > -0x80000 && dy < 0x180000 && ABS(a->z.fixed - b->z.fixed) < px(12))
      return 1;
  }
  return 0;
}
