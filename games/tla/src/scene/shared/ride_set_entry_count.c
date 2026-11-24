/// Sets whether flame thrower `index` puffs even off screen.
void Ride_SetEntryCount(s32 index, s16 value) {
  RIDE_STATE->entries[index].offScreen = value;
}
