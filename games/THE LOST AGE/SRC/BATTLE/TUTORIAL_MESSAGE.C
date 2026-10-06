#include "TYPES.H"
#include "text/MSG_IDS.H"

s32 GameFlag_Test(s32);

s32 Tutorial_GetDjinnGiveMessage(void)
{
    if (GameFlag_Test(0x175))
        return (s32)MsgDjinnGiveTutorialB;
    return (s32)MsgDjinnGiveTutorialA;
}
