/* fdPartyAddPp, the same in both games. Both games'
   field/field_mode_walking.c include it where the function stands. */

/* Gives each character in the party amount PP (PP regenerates as the party
   walks). */
void fdPartyAddPp(Sint32 amount, Sint32 unused)
{
	Sint32 partyCnt = gmPartyCountMembersStub();
	Sint32 partyIdx;

	for(partyIdx = 0; partyIdx < partyCnt; partyIdx++)
		gmUnitAddPpStub(MainRoutineStateWork.party[partyIdx], amount);
}
