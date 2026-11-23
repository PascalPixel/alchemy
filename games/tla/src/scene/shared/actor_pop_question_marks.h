/// Four "?" bubbles pop up over an actor in turn, to its right and its left,
/// each tilted, a quarter of a second apart: someone at a loss.
void Actor_PopQuestionMarks(s32 id) {
  struct Actor *actor = Actor_Get(id);

  QuestionMark_Pop(actor->x.fixed + toFIXED(8), actor->y.fixed + toFIXED(28),
                   actor->z.fixed, TRIG_TURN / 16, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  QuestionMark_Pop(actor->x.fixed - toFIXED(8), actor->y.fixed + toFIXED(28),
                   actor->z.fixed, TRIG_TURN * 15 / 16, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  QuestionMark_Pop(actor->x.fixed + toFIXED(8), actor->y.fixed + toFIXED(28),
                   actor->z.fixed, TRIG_TURN / 16, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  QuestionMark_Pop(actor->x.fixed - toFIXED(8), actor->y.fixed + toFIXED(28),
                   actor->z.fixed, TRIG_TURN * 15 / 16, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
}
