/* Pound's state on a listed actor, an entry of (actor, kind, flag) ended by */
/* ACTOR_LIST_END: while it is the target, it sinks a step a frame for the */
/* first eight frames, and once the fist has landed (POUND_STAGE_LANDED) its */
/* flag is set and its cell opened. */
void fdPoundSinkTarget(u16 *list, s32 mode, s32 count) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;

	for(; *list != ACTOR_LIST_END; list += 3) {
		u32 entry = *list;
		s32 id = state->tgtId;

		if(id == entry) {
			ACTOR *actor = fdActorGetById(id);
			s32 kind;
			s32 flag;

			list++;
			kind = list[0];
			flag = list[1];
			if(count < 8)
				actor->scaleY -= FIXED_ONE * 31 / 256;
			if(mode == POUND_STAGE_LANDED) {
				syGameFlagSet(flag);
				fdActorSetCellOpen(id, kind, flag, actor->layer);
				actor->scaleY = FIXED_ONE;
			}
			break;
		}
	}
}
