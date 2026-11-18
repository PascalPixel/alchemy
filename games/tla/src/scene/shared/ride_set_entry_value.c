/// Turns flame thrower `index` on or off.
UNUSED void Ride_SetEntryValue(s32 index, s16 value) {
  /* FAKEMATCH(signature): the entry written through the includer's struct
   * RideRow view, index * sizeof(struct RideEntry) bytes on; as
   * RIDE_WORK->entries[index].active = value. */
  ((struct RideRow *)((u8 *)RIDE_WORK + index * sizeof(struct RideEntry)))
      ->entry.active = value;
}
