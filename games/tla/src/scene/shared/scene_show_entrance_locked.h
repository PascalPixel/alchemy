// Pressing A at a locked door shows "It seems to be locked." (MsgShamanEntranceLocked): at Shaman
// Village's houses until Moapa is beaten, and at Contigo's shut doors while its people are away
// with the wings.
void scShowEntranceLocked(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	mnFieldMessageShow((Sint32)MsgShamanEntranceLocked, 1);
	fdScriptEnd();
}
