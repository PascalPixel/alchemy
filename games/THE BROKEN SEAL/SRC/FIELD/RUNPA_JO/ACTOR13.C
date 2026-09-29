/* The Lunpa fortress: actor 13's scene resource. */
#include "FORTRESS.H"
extern u8 MsgRunpaCantBelieveWhen[];

void ConfigureActor13SceneResource(void)
{
    Event_SetMessage((s32)MsgRunpaCantBelieveWhen);
    Event_ShowMessage(13, 0);
}
