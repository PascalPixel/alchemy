/// The leader steps off a door's button: a door the button held open (0x202
/// set) closes, unless the block now stands on the button
/// (JupiterLighthouse_CheckBlockOnCell).
void JupiterLighthouse_StepOffButton(void) {
  if (Event_GetFlag(0x202) == 1) {
    Event_ClearFlag(0x202);
    JupiterLighthouse_CloseDoor();
  }
  JupiterLighthouse_CheckBlockOnCell();
}
