/* fdLZPackCopyBufferedBytes and fdLZPackFindWindowMatch, the same in both
   games. Both games' field/lucky_wheels.c include it where the functions
   stand. */

/* Appends the finished group (a flag byte and up to eight items) to the
   output. */
void fdLZPackCopyBufferedBytes(Uint8 *dst)
{
	LZ_PACK_STATE *state = LzPackStateWork;
	Sint32		   idx;

	for(idx = 0; idx != state->outCnt; idx++) {
		dst[state->outTotal] = state->outBuf[idx];
		state->outTotal++;
	}
}

/* Finds the longest earlier match (up to PACK_MATCH_MAX bytes, at most
   PACK_DISTANCE_MAX back) for the bytes at window position start; matchLen
   stays 1 for none. */
static __inline__ void fdLZPackFindWindowMatch(Sint32 start)
{
	LZ_PACK_NODE *node;
	Sint32		  len;

	LzPackStateWork->matchLen = 1;
	if(LzPackStateWork->bucketBySlot[start] == -1) {
		return;
	}
	node = LzPackStateWork->bucket[LzPackStateWork->bucketBySlot[start]];
	if(start + PACK_MATCH_MAX > PACK_WINDOW_MASK) {
		while(node != NULL) {
			Sint32 dist = (start - node->slot) & PACK_WINDOW_MASK;

			if(dist > 0 && dist <= PACK_DISTANCE_MAX) {
				for(len = 1; len < PACK_MATCH_MAX; len++) {
					if(LzPackStateWork
							->bucketBySlot[(start + len) & PACK_WINDOW_MASK] !=
						LzPackStateWork->bucketBySlot[(node->slot + len) &
							PACK_WINDOW_MASK]) {
						break;
					}
				}
				if(LzPackStateWork->matchLen < len) {
					LzPackStateWork->matchOfs = dist;
					LzPackStateWork->matchLen = len;
					if(len == PACK_MATCH_MAX) {
						return;
					}
				}
			}
			node = node->next;
		}
	} else {
		while(node != NULL) {
			Sint32 dist = (start - node->slot) & PACK_WINDOW_MASK;

			if(dist > 0 && dist <= PACK_DISTANCE_MAX) {
				for(len = 1; len < PACK_MATCH_MAX; len++) {
					if(LzPackStateWork->bucketBySlot[start + len] !=
						LzPackStateWork->bucketBySlot[(node->slot + len) &
							PACK_WINDOW_MASK]) {
						break;
					}
				}
				if(LzPackStateWork->matchLen < len) {
					LzPackStateWork->matchOfs = dist;
					LzPackStateWork->matchLen = len;
					if(len == PACK_MATCH_MAX) {
						return;
					}
				}
			}
			node = node->next;
		}
	}
}
