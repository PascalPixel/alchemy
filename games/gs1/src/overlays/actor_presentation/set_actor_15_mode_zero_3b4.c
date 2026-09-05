#include "types.h"

extern void Func_02002fd6(s32 actor, s32 mode, s32 frames);

/* Complete 16-byte actor-15 mode wrapper before the no-op leaf at 0x9ec. */
void SceneActor_SetActor15ModeZero(void)
{
    Func_02002fd6(15, 0, 6);
}
