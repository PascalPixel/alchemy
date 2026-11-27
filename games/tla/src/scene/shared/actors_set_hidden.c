/* Draws (`show` 1) or stops drawing `cnt` actors, from `first` on. */
void scActorsSetDrawn(Sint32 show, Sint32 first, Uint32 cnt)
{
	Uint32 i;

	for(i = 0; i < cnt; i++) {
		ACTOR *actor = fdActorGetById(first + i);

		if(show)
			actor->priFlg &= ~ACTOR_PRIORITY_NOT_DRAWN;
		else
			actor->priFlg |= ACTOR_PRIORITY_NOT_DRAWN;
	}
}
