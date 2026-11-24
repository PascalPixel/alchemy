/// Move sets the shortcut pillar down: on the button's cell it holds the door
/// open.
void JupiterLighthouse_FinishMovingBlock(void) {
  System_Wait(10);
  JupiterLighthouse_CheckPillarOnButton();
}
