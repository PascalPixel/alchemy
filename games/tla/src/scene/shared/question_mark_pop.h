// Pops a "?" bubble up at (x, y, z), tilted by `rotation`, for `ticks`.
void scQuestionMarkPop(FIXED x, FIXED y, FIXED z, Uint16 rotation, Sint32 ticks)
{
	ACTOR *mark;
	SPR   *sprite;

	mark = syImportActorCreate(SPRITE_EMOTE, x, y, z);
	if (mark != 0)
	{
		sprite = mark->sprite;
		fdActorSelectAnimation(mark, EMOTE_QUESTION);
		fdActorStartScript(mark, QuestionMarkScript);
		mark->motionFlags = 0;
		mark->wait        = ticks;
		sprite->flags     = 0;
		sprite->rotation  = rotation;
	}
}
