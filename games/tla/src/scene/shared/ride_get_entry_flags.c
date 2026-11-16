/// Flame thrower `index`'s flags: which flame it sends and how far.
s32 Ride_GetEntryFlags(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.flags;
}
