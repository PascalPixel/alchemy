#include "REGION.H"

/* Hides an actor by clearing its sprite flags. */
s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
    Actor_SetSpriteFlags(no, 0);
    return 0;
}
