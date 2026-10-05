#include "TYPES.H"

s32 GameFlag_Test(s32);
extern char MsgDjinnGiveTutorialA;
extern char MsgDjinnGiveTutorialB;

s32 Tutorial_GetDjinnGiveMessage(void)
{
    if (GameFlag_Test(0x175))
        return (s32)&MsgDjinnGiveTutorialB;
    return (s32)&MsgDjinnGiveTutorialA;
}
