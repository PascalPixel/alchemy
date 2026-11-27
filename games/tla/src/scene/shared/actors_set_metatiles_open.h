/* fdActorSetMetatileOpen for each actor of a list ended by ACTOR_LIST_END. */
void scActorsSetMetatilesOpen(const Uint16 *list)
{
	const Uint16 *spec;
	Uint16		  id;
	ACTOR		 *actor;
	Sint16		  mode;
	Sint16		  flg;
	Sint32		  first;

	first = list[0];
	if(first == ACTOR_LIST_END) {
		return;
	}
	spec = list;
	do {
		id = spec[0];
		actor = fdActorGet(id);
		mode = spec[1];
		flg = spec[2];
		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlags(actor, 0);
		fdActorSetMetatileOpen(id, (Uint16)mode, (Uint16)flg, actor->layer);
		spec += 3;
		first = spec[0];
	} while(first != ACTOR_LIST_END);
}
