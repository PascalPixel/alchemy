/// Sets a ride slot's value.
void Ride_SetSlot(u32 index, s32 value) {
  struct FlameHitWork *work = RIDE_WORK;

  if (index <= 3)
    work->slots[index].id = value;
}
