/* Draws (`show` 1) or stops drawing `count` actors, from `first` on. */
void scActorsSetDrawn(s32 show, s32 first, u32 count) {
	u32 i;

	for(i = 0; i < count; i++) {
		ACTOR *actor = fdActorGetById(first + i);

		if(show)
			actor->priFlg &= ~ACTOR_PRIORITY_NOT_DRAWN;
		else
			actor->priFlg |= ACTOR_PRIORITY_NOT_DRAWN;
	}
}
