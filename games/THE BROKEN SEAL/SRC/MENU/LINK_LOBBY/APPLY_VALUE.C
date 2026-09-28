/* The link lobby: a value applied through the shop confirmation. */
#include "LOBBY.H"

s32 State_ApplyValueAndGetResult(s32 arg0)
{
    Engine_EventBegin(arg0);
    Engine_SanctumOpen(arg0);
    return Engine_EventEnd();
}
