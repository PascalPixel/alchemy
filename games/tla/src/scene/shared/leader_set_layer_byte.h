/// Puts the leader on map layer 2, through its byte.
void Leader_SetLayerByte(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FIELD_AT((t0 + 0x22), u8, 0) = 2;
}
