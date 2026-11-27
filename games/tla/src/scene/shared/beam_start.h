/* Sets the flame throwers going, unless the scene starts where the party was:
 */
/* up to four listed actors with their flags and reach, and up to four actors */
/* their flames stop at; then what is called back when a flame meets a blocked
 */
/* one and the step trigger a hit raises. Blends the flames over the room and */
/* starts their puffs. */
void scFlameThrowersStart(u16 *targets, u16 *stops, s32 onBlocked,
						  s32 hitTrigger) {
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;
	FLAME_THROWER *entry = state->entries;
	s32 id;
	s32 i;
	s32 value;
	s32 one;
	s32 flags;
	s32 reach;
	ACTOR *actor;
	s32 alpha;
	s32 control;

	if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
		for(i = 0; i < 4 && (id = *targets++) > 0; i++) {
			flags = *targets;
			reach = flags & FLAME_THROWER_REACH_BITS;
			flags &= FLAME_THROWER_FLAG_BITS;
			actor = fdActorGet(id);
			targets++;
			if(actor != NULL) {
				fdActorSetSprFlags(actor, 0);
				*(u8 *)&actor->sceneCnt = one = 1;
				if((flags & FLAME_SPARK) == 0)
					actor->collisionFlg = 0;
				entry->active = one;
				entry->flg = flags;
				entry->actor = fdActorGet(id);
				entry->reach = toFIXED(reach);
				entry->tmr = i;
				entry++;
			}
		}
		if(stops != NULL) {
			for(i = 0; i < 4; i++) {
				value = *stops++;
				state->slots[i].id = value;
				if(value <= 0)
					break;
				state->slots[i].blocked = *stops++;
			}
		}
	}
	state->onBlocked = (void (*)(void))onBlocked;
	state->hitTrigger = hitTrigger;
	if(REG_BLDCNT == 0) {
		alpha = BLEND_ALPHA(8, 12);
		REG_BLDALPHA = alpha;
		control = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		REG_BLDCNT = control;
	}
	syTaskAddFrame(scFlameThrowersPuff, TASK_PASS_FRAME);
}
