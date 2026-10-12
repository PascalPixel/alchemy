/* Near miss of Func_02004538 (22 instructions, draft 23, 3 differ): as below the second test
 * matches, but the draft loads gFrameCount's address into r3 and copies it to r6 where the
 * native loads r6 directly. Without the one-pass block (the form of the battle twin
 * BattleFx_CycleObjectAndTick) the second read stays in r0 and the mask is copied
 * (`ldr r0; movs r3, #15; ands r3, r0`, 8 differ); a count pointer, a modulo and the test
 * inside the block give the same 8. */
#include "types.h"
#include "frame.h"
#include "object_runtime.h"

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02004634(struct ObjectRuntime *object);

void Func_02004538(struct ObjectRuntime *object)
{
    u32 frame = gFrameCount;

    if (frame & 1)
        Animation_ApplyChildValues(object, (frame >> 1) % 6);
    do { } while (0);
    if ((gFrameCount & 15) == 0)
        Func_02004634(object);
}
