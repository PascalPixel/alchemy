/// Sets whether flame thrower `index` puffs even off screen.
void Ride_SetEntryCount(s32 index, s16 value) {
  RIDE_WORK->entries[index].offScreen = value;
}
