/* Sets the flame throwers going, unless the scene starts where the party was:
   up to four listed actors with their flags and reach, and up to four actors
   their flames stop at; then what is called back when a flame meets a blocked
   one and the step cue a hit raises. Blends the flames over the room and starts
   their puffs. */
void scFlameThrowersStart(
	Uint16 *targets, Uint16 *stops, Sint32 onBlocked, Sint32 hitCue)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;
	FLAME_THROWER	*entry = state->entries;
	Sint32			 id;
	Sint32			 i;
	Sint32			 value;
	Sint32			 one;
	Sint32			 flags;
	Sint32			 reach;
	ACTOR			*actor;
	Sint32			 alpha;
	Sint32			 control;

	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
		for(i = 0; i < 4 && (id = *targets++) > 0; i++) {
			flags = *targets;
			reach = flags & FLAME_THROWER_REACH_BITS;
			flags &= FLAME_THROWER_FLAG_BITS;
			actor = fdActorGet(id);
			targets++;
			if(actor != NULL) {
				fdActorSetSpriteFlags(actor, 0);
				*(Uint8 *)&actor->sceneCount = one = 1;
				if((flags & FLAME_SPARK) == 0)
					actor->collisionFlags = 0;
				entry->active = one;
				entry->flags = flags;
				entry->actor = fdActorGet(id);
				entry->reach = toFIXED(reach);
				entry->timer = i;
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
	state->hitCue = hitCue;
	if(REG_BLDCNT == 0) {
		alpha = BLEND_ALPHA(8, 12);
		REG_BLDALPHA = alpha;
		control = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		REG_BLDCNT = control;
	}
	sySetTickEvent(scFlameThrowersPuff, EVENT_PASS_TICK);
}
