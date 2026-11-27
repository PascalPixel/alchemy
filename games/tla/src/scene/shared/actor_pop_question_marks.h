// Four "?" bubbles pop up over an actor in turn, to its right and its left, each tilted, a quarter
// of a second apart: someone at a loss.
void scActorPopQuestionMarks(Sint32 id)
{
    ACTOR *actor = fdActorGet(id);

    scQuestionMarkPop(actor->x.fixed + toFIXED(8),
                      actor->y.fixed + toFIXED(28),
                      actor->z.fixed,
                      TRIG_TURN / 16,
                      15);
    fdScriptPlaySound(SE_LIGHT_APPEAR);
    syFieldScriptWait(15);
    scQuestionMarkPop(actor->x.fixed - toFIXED(8),
                      actor->y.fixed + toFIXED(28),
                      actor->z.fixed,
                      TRIG_TURN * 15 / 16,
                      15);
    fdScriptPlaySound(SE_LIGHT_APPEAR);
    syFieldScriptWait(15);
    scQuestionMarkPop(actor->x.fixed + toFIXED(8),
                      actor->y.fixed + toFIXED(28),
                      actor->z.fixed,
                      TRIG_TURN / 16,
                      15);
    fdScriptPlaySound(SE_LIGHT_APPEAR);
    syFieldScriptWait(15);
    scQuestionMarkPop(actor->x.fixed - toFIXED(8),
                      actor->y.fixed + toFIXED(28),
                      actor->z.fixed,
                      TRIG_TURN * 15 / 16,
                      15);
    fdScriptPlaySound(SE_LIGHT_APPEAR);
    syFieldScriptWait(15);
}
