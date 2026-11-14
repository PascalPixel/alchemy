/// 1 when two actors touch.
s32 Actor_IsTouching(struct Actor *a, struct Actor *b) {
  if (ABS(a->x - b->x) < 0x80000 && ABS(a->y - b->y) < 0x80000 &&
      ABS(a->z - b->z) < 0x80000)
    return 1;
  return 0;
}
