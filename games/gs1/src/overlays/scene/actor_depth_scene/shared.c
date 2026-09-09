#include "types.h"

#define FieldScene_RequestAndWaitFrames Func_02000314
/*
 * resource_387: issue a scene request and then wait.
 */

/* Old-style: the two imports are called with different arities. */
void Func_020014e8();

void Func_02001446();          /* Wait this many frames. */

/*
 * The owner exists for the argument shuffle: the frame count is saved before
 * the first call clobbers its register, so it survives to reach the second.
 * The twenty-two byte owner loads no literal and has no pool.
 */
void FieldScene_RequestAndWaitFrames(s32 selector, s32 frames)
{
    Func_020014e8(selector, 0);
    Func_02001446(frames);
}
