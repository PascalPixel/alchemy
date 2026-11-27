/* Steps an actor toward its target; 0 when it gets there. */
s32 fdActorStepToWatched(ACTOR *actor) {
	u8 left;

	actor->x.fixed += actor->bounce;
	actor->y.fixed += actor->sceneWord;
	actor->z.fixed += actor->gravity;
	left = --actor->sceneCnt;
	return left != 0;
}
