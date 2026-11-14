/// Opens the door for good, setting 0xa1b, once the block 16 stands on
/// its button, cell (8, 37).
void JupiterLighthouse_CheckBlockOnCell(void) {
  struct Actor *object = Actor_Get(16);

  if (!Event_GetFlag(0xa1b) && object->x >> 20 == 8 && object->z >> 20 == 37) {
    Event_SetFlag(0xa1b);
    JupiterLighthouse_OpenDoor(1);
  }
}
