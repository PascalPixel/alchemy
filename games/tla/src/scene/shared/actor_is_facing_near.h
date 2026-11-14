/// 1 when `other` is within `range` of `self` and `self` faces it, within one
/// eighth of a turn.
s32 Actor_IsFacingNear(struct Actor *self, struct Actor *other, s32 range) {
  s32 result = 0;
  s32 *to;
  s32 *from;

  to = &other->x;
  from = &self->x;
  if (Lemuria_Distance(to, from) < range) {
    u32 angle = (u16)ArcTan2(other->z - self->z, *to - *from);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = self->facing & 0xf000;

    if (ahead == facing || right == facing || left == facing)
      result = 1;
  }
  return result;
}
