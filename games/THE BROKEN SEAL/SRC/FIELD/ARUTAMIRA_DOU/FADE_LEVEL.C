#include "ARUTAMIRA.H"

/* Record the fade level in the scene state; while the event work's word at
   +0xcb8 is clear, the blend takes it at once. */
void ArutamiraDou_SetFadeLevel(s32 level)
{
    u8 *work;
    u8 *fade = &gSceneState[4];

    work = (u8 *)gEventWork;
    *fade = level;
    if (*(s16 *)(work + 0xcb8) == 0) {
        ArutamiraDou_ApplyFadeBlend();
    }
}
