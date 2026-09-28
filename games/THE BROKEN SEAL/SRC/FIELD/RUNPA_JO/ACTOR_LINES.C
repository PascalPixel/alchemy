/* The Lunpa fortress: two actors' lines. */
#include "FORTRESS.H"

void ConfigureSceneActor12Variant(void)
{
    Actor_RunRepeatedMotion(12, 2);
    Event_SetMessage(0x243f);
    Event_ShowMessage(12, 0);
}

void ConfigureSceneActor18(void)
{
    Event_SetMessage(0x2459);
    Event_AskYesNo(18, 0);
}
