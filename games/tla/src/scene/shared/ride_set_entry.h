/// Sets a ride entry's height and flags from a packed value.
void Ride_SetEntry(s32 index, s32 value) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);
  struct RideEntry *entry = &work->entries[index];

  entry->d = (value & 15) << 16;
  entry->b = value & 0xf0;
}
