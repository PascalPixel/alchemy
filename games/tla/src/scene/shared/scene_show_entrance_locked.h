/* Pressing A at a locked door shows "It seems to be locked." */
/* (MsgShamanEntranceLocked): at Shaman Village's houses until Moapa is beaten,
 */
/* and at Contigo's shut doors while its people are away with the wings. */
void scShowEntranceLocked(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	mnFieldMessageShow((s32)MsgShamanEntranceLocked, 1);
	fdEventEnd();
}
