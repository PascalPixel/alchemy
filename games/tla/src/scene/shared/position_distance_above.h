/// The distance in whole pixels between two 16.16 positions of x and y, the
/// second raised 6 pixels.
s32 Position_DistanceAbove(s32 *a, s32 *b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a - *b + 0x60000) >> 16;

  return Iwram_Sqrt(dx * dx + dy * dy);
}
