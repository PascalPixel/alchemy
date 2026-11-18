/// Whether flame thrower `index` is active.
UNUSED s32 Ride_GetEntryValue(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK +
                             index * sizeof(struct RideEntry)))
      ->entry.active;
}
