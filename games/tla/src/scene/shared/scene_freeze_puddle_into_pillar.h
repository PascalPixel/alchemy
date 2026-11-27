// Frost freezes one of the puddles in Daila's streets or its sanctuary into a pillar of ice, which
// then takes its priority from its metatile and is no longer drawn underfoot.
void scFreezePuddleIntoPillar(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR       *pillar;
	Sint32       flags;

	fdFlagBlockApplyFlags(DailaRoomsPillarFrozenFlags);
	pillar                 = state->movedBlock;
	flags                  = ACTOR_PRIORITY_AUTOMATIC;
	flags                 |= pillar->priorityFlags;
	flags                 &= (Uint8)~ACTOR_PRIORITY_UNDERFOOT;
	pillar->priorityFlags  = flags;
}
