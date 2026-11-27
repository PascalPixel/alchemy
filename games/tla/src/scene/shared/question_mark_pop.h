/* Pops a "?" bubble up at (x, y, z), tilted by `rotation`, for `frames`. */
void scQuestionMarkPop(FIXED x, FIXED y, FIXED z, u16 rotation, s32 frames) {
	ACTOR *mark;
	SPR *sprite;

	mark = syImportActorCreate(SPRITE_EMOTE, x, y, z);
	if(mark != 0) {
		sprite = mark->spr;
		fdActorSelectAnimation(mark, EMOTE_QUESTION);
		fdActorStartScript(mark, QuestionMark_Script);
		mark->motionFlg = 0;
		mark->wait = frames;
		sprite->flg = 0;
		sprite->rotation = rotation;
	}
}
