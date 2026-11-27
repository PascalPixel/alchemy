/* Pound's state on a listed actor, an entry of (actor, kind, flag) ended by
   ACTOR_LIST_END: while it is the target, it sinks a step a tick for the first
   eight ticks, and once the fist has landed (POUND_STAGE_LANDED) its flag is
   set and its metatile opened. */
void fdPoundSinkTarget(Uint16 *list, Sint32 mode, Sint32 count)
{
	FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;

	for(; *list != ACTOR_LIST_END; list += 3) {
		Uint32 entry = *list;
		Sint32 id = state->targetId;

		if(id == entry) {
			ACTOR *actor = fdActorGetById(id);
			Sint32 kind;
			Sint32 flag;

			list++;
			kind = list[0];
			flag = list[1];
			if(count < 8)
				actor->scaleY -= toFIXED(31) / 256;
			if(mode == POUND_STAGE_LANDED) {
				syGameFlagSet(flag);
				fdActorSetMetatileOpen(id, kind, flag, actor->layer);
				actor->scaleY = toFIXED(1);
			}
			break;
		}
	}
}
