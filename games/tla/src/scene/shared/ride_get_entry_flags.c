/// Flame thrower `index`'s flags: which flame it sends, or none
/// (FLAME_THROWER_DRY).
s32 Ride_GetEntryFlags(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK +
                             index * sizeof(struct RideEntry)))
      ->entry.flags;
}
