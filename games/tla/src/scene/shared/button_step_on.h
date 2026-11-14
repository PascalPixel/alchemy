/// The leader steps onto a door's button: the door opens and 0x202 says the
/// button holds it.
void JupiterLighthouse_StepOnButton(void) {
  if (!Event_GetFlag(0x202)) {
    Event_SetFlag(0x202);
    JupiterLighthouse_OpenDoor(1);
  }
}
