// Steps an actor toward its target; 0 when it gets there.
Sint32 scActorStepToWatched(ACTOR *actor)
{
	Uint8 left;

	actor->x.fixed += actor->bounce;
	actor->y.fixed += actor->sceneWord;
	actor->z.fixed += actor->gravity;
	left            = --actor->sceneCount;
	return left != 0;
}
