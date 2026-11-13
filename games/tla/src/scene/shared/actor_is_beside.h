/// 1 when two actors stand side by side.
s32 Actor_IsBeside(struct Actor *a, struct Actor *b) {
  s32 dy;

  if (ABS(a->x - b->x) < 0xc0000) {
    dy = a->y - b->y;
    if (dy > -0x80000 && dy < 0x180000 && ABS(a->z - b->z) < 0xc0000)
      return 1;
  }
  return 0;
}
