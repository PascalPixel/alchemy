/* Tunnel Ruins and Venus Lighthouse: blocks the leader can stand beside, behind
   or on. */

/* An update function: draw the block under the actors
   (ACTOR_PRIORITY_UNDERFOOT) unless the leader is north of it and no higher
   than its height plus their distance and 4 pixels, that is, behind it rather
   than on it. */
static UNREAD_RESULT scActorUpdateBlockDrawOrder(ACTOR *actor)
{
	ACTOR *leader;
	Uint8  flag;
	leader = fdActorGetById(ACTOR_PARTY_LEADER);
	flag = actor->priorityFlags | ACTOR_PRIORITY_UNDERFOOT;
	actor->priorityFlags = flag;
	if(leader->z.fixed < actor->z.fixed) {
		FIXED reach = actor->z.fixed - leader->z.fixed;
		FIXED top;
		reach += toFIXED(4);
		top = actor->y.fixed + reach;
		if(leader->y.fixed <= top) {
			flag &= ~ACTOR_PRIORITY_UNDERFOOT;
			actor->priorityFlags = flag;
		}
	}
	RETURN_UNREAD(0);
}

/* After Move, puts a pair of pillars back in order, the first south of the
   second: unless it already is, the two trade places and the game waits a
   tick. Nothing visible changes, since the pillars of a pair look alike, and
   the scenes that call it treat the two alike afterwards. */
static void scActorSwapPositionsByDepth(Sint32 first, Sint32 second)
{
	ACTOR *a = fdActorGetById(first);
	ACTOR *b = fdActorGetById(second);

	if(a->z.fixed <= b->z.fixed) {
		FIXED swap;

		swap = a->x.fixed;
		a->x.fixed = b->x.fixed;
		b->x.fixed = swap;
		swap = a->y.fixed;
		a->y.fixed = b->y.fixed;
		b->y.fixed = swap;
		swap = a->z.fixed;
		a->z.fixed = b->z.fixed;
		b->z.fixed = swap;
		sySynch(1);
	}
}
