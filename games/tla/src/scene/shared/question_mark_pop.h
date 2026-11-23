/// Pops a "?" bubble up at (x, y, z), tilted by `rotation`, for `frames`.
void QuestionMark_Pop(FIXED x, FIXED y, FIXED z, u16 rotation, s32 frames) {
  struct Actor *mark;
  struct Sprite *sprite;

  mark = Import_ActorCreate(SPRITE_EMOTE, x, y, z);
  if (mark != 0) {
    sprite = mark->sprite;
    Actor_SelectAnimation(mark, EMOTE_QUESTION);
    Actor_StartScript(mark, gQuestionMark_Script);
    mark->motionFlags = 0;
    mark->wait = frames;
    sprite->flags = 0;
    sprite->rotation = rotation;
  }
}
