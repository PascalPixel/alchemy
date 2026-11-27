/* Places each pillar of a list of three-halfword entries (actor, kind, flag)
   ended by ACTOR_LIST_END, its collision bit cleared, raised or sunk by its
   flag (fdActorSetMetatileOpen). */
void scPillarPlace(const Uint16 *list)
{
	const Uint16 *entry;

	if(list[0] == ACTOR_LIST_END)
		return;
	for(entry = list; entry[0] != ACTOR_LIST_END; entry += 3) {
		Uint32 id = entry[0];
		ACTOR *actor = fdActorGet(id);
		Sint16 kind = entry[1];
		Sint16 flg = entry[2];

		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlags(actor, 0);
		fdActorSetMetatileOpen(id, (Uint16)kind, (Uint16)flg, actor->layer);
	}
}
