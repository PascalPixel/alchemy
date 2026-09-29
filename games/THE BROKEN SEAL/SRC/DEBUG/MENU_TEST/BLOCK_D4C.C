#include "MENU_TEST.H"

/* Message 0xd4c, which the main image names as a link-time value. */
extern u8 gVal4;

void SceneState_ApplyBlockD4c(void)
{
    CommandTable_RunDirectionalInput((s32)&gVal4, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}
