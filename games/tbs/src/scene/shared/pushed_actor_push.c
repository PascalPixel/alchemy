// The plain push of Altmiller Cave's fallen columns and Crossbone Isle's two logs on the grass.

// Walking into a pushed thing moves it a metatile on, if the way beyond is clear.
static void scPushedActorPushIfClear(void)
{
    PUSHED_ACTOR_PROBE probe;

    fdScriptStart();
    if (scPushedActorFindClearPos(&probe) != 0)
    {
        scPushedActorRoll(probe);
    }
    fdScriptEnd();
}
