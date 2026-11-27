/* scActorBob's cycle, in ticks. */
#define BOB_CYCLE_TICKS	 64
/* Its sprite's rotation is the cosine of its swing over this: up to a 64th of a
   turn either way. */
#define BOB_ROCK_DIVISOR 64

/* An actor's script call that never ends: it hops three times every
   BOB_CYCLE_TICKS ticks, at the cycle's start, at tick 16 and at tick 24,
   while its sprite rocks from side to side, once every 16 ticks. */
Sint32 scActorBob(ACTOR *actor)
{
	Sint32	   frm = actor->sceneCnt & (BOB_CYCLE_TICKS - 1);
	FIELD_SPR *spr = actor->spr;

	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	if(frm == 0)
		actor->velY = toFIXED(3);
	if(frm == 16)
		actor->velY = toFIXED(2);
	if(frm == 24)
		actor->velY = toFIXED(3);
	spr->rotation = syCos(frm * (TRIG_TURN / 16)) / BOB_ROCK_DIVISOR;
	actor->sceneCnt++;
	return -1;
}

#undef BOB_CYCLE_TICKS
#undef BOB_ROCK_DIVISOR
