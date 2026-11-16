/// Sets a ride entry's height and flags from a packed value.
void Ride_SetEntry(s32 index, s32 value) {
  struct FlameHitWork *work = RIDE_WORK;
  struct RideEntry *entry = &work->entries[index];

  entry->reach = (value & 15) << 16;
  entry->flags = value & 0xf0;
}
