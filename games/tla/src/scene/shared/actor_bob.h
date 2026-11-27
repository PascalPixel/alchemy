/* fdActorBob's cycle, in frames. */
#define BOB_CYCLE_FRAMES 64
/* Its sprite's rotation is the cosine of its swing over this: up to a 64th of
 */
/* a turn either way. */
#define BOB_ROCK_DIVISOR 64

/* An actor's script call that never ends: it hops three times every */
/* BOB_CYCLE_FRAMES frames, at the cycle's start, at frame 16 and at frame 24,
 */
/* while its sprite rocks from side to side, once every 16 frames. */
s32 fdActorBob(ACTOR *actor) {
	s32 frame = actor->sceneCnt & (BOB_CYCLE_FRAMES - 1);
	FIELD_SPR *sprite = actor->spr;

	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	if(frame == 0)
		actor->velY = toFIXED(3);
	if(frame == 16)
		actor->velY = toFIXED(2);
	if(frame == 24)
		actor->velY = toFIXED(3);
	sprite->rotation = syCos(frame * (TRIG_TURN / 16)) / BOB_ROCK_DIVISOR;
	actor->sceneCnt++;
	return -1;
}

#undef BOB_CYCLE_FRAMES
#undef BOB_ROCK_DIVISOR
