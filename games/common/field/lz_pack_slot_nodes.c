/* fdLZPackInsertSlotNode and fdLZPackRemoveSlotNode, the same in both games.
   Both games' field/lucky_wheels.c include it where the functions stand. */

/* Adds window position index to the chain of the byte it holds. */
void fdLZPackInsertSlotNode(Sint32 idx)
{
	LZ_PACK_STATE *state = LzPackStateWork;

	state->nodes[idx].prev =
		(LZ_PACK_NODE *)&state->bucket[state->bucketBySlot[idx]];
	state->nodes[idx].next = state->bucket[state->bucketBySlot[idx]];
	state->bucket[state->bucketBySlot[idx]] = &state->nodes[idx];
	if(state->nodes[idx].next != NULL)
		state->nodes[idx].next->prev = &state->nodes[idx];
}

/* Removes window position slot from its chain. */
void fdLZPackRemoveSlotNode(Sint32 slot)
{
	if(LzPackStateWork->nodes[slot].prev != NULL) {
		if(LzPackStateWork->nodes[slot].next != NULL)
			LzPackStateWork->nodes[slot].next->prev =
				LzPackStateWork->nodes[slot].prev;
		LzPackStateWork->nodes[slot].prev->next =
			LzPackStateWork->nodes[slot].next;
	}
}
