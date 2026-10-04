#include "TYPES.H"

s32 GameFlag_Test(s32);
extern char MsgDjinnGiveTutorialA;
extern char MsgDjinnGiveTutorialB;

/* The Djinn hand-over tutorial's message: the later wording once flag
   0x175 is set. */
s32 Tutorial_GetDjinnGiveMessage(void)
{
    if (GameFlag_Test(0x175))
        return (s32)&MsgDjinnGiveTutorialB;
    return (s32)&MsgDjinnGiveTutorialA;
}
