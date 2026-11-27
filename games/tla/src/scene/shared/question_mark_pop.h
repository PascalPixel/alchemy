/* Pops a "?" bubble up at (x, y, z), tilted by `rotation`, for `ticks`. */
void scQuestionMarkPop(FIXED x, FIXED y, FIXED z, Uint16 rotation, Sint32 ticks)
{
	ACTOR *mark;
	SPR	  *spr;

	mark = syImportActorCreate(SPRITE_EMOTE, x, y, z);
	if(mark != 0) {
		spr = mark->spr;
		fdActorSelectAnimation(mark, EMOTE_QUESTION);
		fdActorStartScript(mark, QuestionMarkScript);
		mark->motionFlg = 0;
		mark->wait = ticks;
		spr->flg = 0;
		spr->rotation = rotation;
	}
}
