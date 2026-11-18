/// Whether flame thrower `index` is active.
UNUSED s32 Ride_GetEntryValue(s32 index) {
  /* FAKEMATCH(signature): the entry read through the includer's struct RideRow
   * view, index * sizeof(struct RideEntry) bytes on; as
   * RIDE_WORK->entries[index].active. */
  return ((struct RideRow *)((u8 *)RIDE_WORK +
                             index * sizeof(struct RideEntry)))
      ->entry.active;
}
