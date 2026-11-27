/* Frost freezes one of the puddles in Daila's streets or its sanctuary into a
 */
/* pillar of ice, which then takes its priority from its cell and is no longer
 */
/* drawn underfoot. */
void scFreezePuddleIntoPillar(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *pillar;
	s32 flags;

	fdFlagBlockApplyFlags(DailaRooms_PillarFrozenFlags);
	pillar = state->movedBlock;
	flags = ACTOR_PRIORITY_AUTOMATIC;
	flags |= pillar->priFlg;
	flags &= (u8)~ACTOR_PRIORITY_UNDERFOOT;
	pillar->priFlg = flags;
}
