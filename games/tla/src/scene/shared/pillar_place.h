/* Places each pillar of a list of three-halfword entries (actor, kind, */
/* flag) ended by ACTOR_LIST_END, its collision bit cleared, raised or sunk by
 */
/* its flag (fdActorSetCellOpen). */
void scPillarPlace(const u16 *list) {
	const u16 *entry;

	if(list[0] == ACTOR_LIST_END)
		return;
	for(entry = list; entry[0] != ACTOR_LIST_END; entry += 3) {
		u32 id = entry[0];
		ACTOR *actor = fdActorGet(id);
		s16 kind = entry[1];
		s16 flag = entry[2];

		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlags(actor, 0);
		fdActorSetCellOpen(id, (u16)kind, (u16)flag, actor->layer);
	}
}
