/// Sets whether flame thrower `index` puffs even off screen.
void FlameThrower_SetPuffsOffScreen(s32 index, s16 value) {
  FLAME_THROWERS_STATE->entries[index].offScreen = value;
}
