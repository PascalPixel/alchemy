/// Turns flame thrower `index` on or off.
UNUSED void Ride_SetEntryValue(s32 index, s16 value) {
  ((struct RideRow *)((u8 *)RIDE_WORK + index * sizeof(struct RideEntry)))
      ->entry.active = value;
}
