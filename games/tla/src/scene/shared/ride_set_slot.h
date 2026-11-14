/// Sets a ride slot's value.
void Ride_SetSlot(u32 index, s32 value) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);

  if (index <= 3)
    work->slots[index].value = value;
}
