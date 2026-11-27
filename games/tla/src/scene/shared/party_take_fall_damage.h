/* The HP each party member loses when a flame or a rolling fireball knocks the
 */
/* leader down. */
#define FALL_DAMAGE 10

/* A flame or a rolling fireball has knocked the leader down: each party member
 */
/* loses FALL_DAMAGE HP and the leader shows the hit; each member it brings */
/* down is listed, loses their poison and raises */
/* FIELD_RAISED_TRIGGER_PARTY_FELL, as a hazard's damage does. */
void gmPartyTakeFallDamage(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 leader = syFieldPartyGetLeaderActor();
	s32 count;
	s32 i;
	s16 hp[8];

	/* DEAD: the leader's actor, looked up and not used (a hazard's damage makes
	 */
	/* him flinch with it); the ROM keeps the call. */
	(void)fdActorGet(leader);
	count = gmPartyCountMembers();
	for(i = 0; i < count; i++)
		hp[i] = syUnitGet(MainRoutineStateWork.party[i])->hp;
	fdPartyAddHp(-FALL_DAMAGE, 0);
	state->fallenCnt = 0;
	state->survivorCnt = 0;
	fdActorSetAttachedEffect(leader,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	for(i = 0; i < count; i++) {
		MAIN_ROUTINE_STATE *main = &MainRoutineStateWork;
		UNIT *unit = syUnitGet(main->party[i]);

		if(unit->hp > 0) {
			state->survivorCnt++;
		}
		else if(hp[i] != 0) {
			state->fallenOwners[(s16)state->fallenCnt++] = main->party[i];
			state->raisedTrigger = FIELD_RAISED_TRIGGER_PARTY_FELL;
			unit->poison = 0;
		}
	}
}
